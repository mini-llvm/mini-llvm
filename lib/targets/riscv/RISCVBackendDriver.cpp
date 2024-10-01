#include "mini-llvm/targets/riscv/RISCVBackendDriver.h"

#include <cassert>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/codegen/register_allocator/LinearScanAllocator.h"
#include "mini-llvm/codegen/register_allocator/NaiveAllocator.h"
#include "mini-llvm/codegen/RegisterAllocator.h"
#include "mini-llvm/common/ExtensionMode.h"
#include "mini-llvm/common/Precision.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/mc/Program.h"
#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/BasicBlockBuilder.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/FLoad.h"
#include "mini-llvm/mir/Instruction/FStore.h"
#include "mini-llvm/mir/Instruction/Load.h"
#include "mini-llvm/mir/Instruction/Store.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterKind.h"
#include "mini-llvm/mir/StackRelativeOffsetImmediate.h"
#include "mini-llvm/mir/StackSlot.h"
#include "mini-llvm/mir/VirtualRegister.h"
#include "mini-llvm/targets/riscv/codegen/RISCVMCGen.h"
#include "mini-llvm/targets/riscv/codegen/RISCVMIRGen.h"
#include "mini-llvm/targets/riscv/mir/Instruction/RISCVRet.h"
#include "mini-llvm/targets/riscv/mir/RISCVRegister.h"
#include "mini-llvm/targets/riscv/opt/mc/RISCVPassManager.h"
#include "mini-llvm/targets/riscv/opt/mir/RISCVPassManager.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;

mini_llvm::mc::Program RISCVBackendDriver::run(const ir::Module &IM) {
    mir::Module MM;
    RISCVMIRGen(&IM, &MM).emit();

    mir::RISCVPassManager passManager;
    passManager.runBeforeRegisterAllocation(MM);

    {
        using namespace mir;
        using namespace mir::riscv;

        for (Function &F : MM.functions) {
            if (!F.empty()) {
                for (RegisterKind kind : {RegisterKind::kInteger, RegisterKind::kFloating}) {
                    std::unordered_set<VirtualRegister *> virtRegs;
                    for (const BasicBlock &B : F) {
                        for (const Instruction &I : B) {
                            for (const RegisterOperand *op : I.regOps()) {
                                if (op->kind() == kind) {
                                    if (auto *virtReg = dynamic_cast<VirtualRegister *>(&**op)) {
                                        virtRegs.insert(virtReg);
                                    }
                                }
                            }
                        }
                    }

                    std::unordered_set<PhysicalRegister *> physRegs;
                    for (PhysicalRegister *physReg : riscvRegs()) {
                        if (physReg->kind() == kind && physReg->isAllocatable()) {
                            physRegs.insert(physReg);
                        }
                    }
                    physRegs.erase(sp());
                    physRegs.erase(fp());

                    auto load = [endSlot = &F.stackFrame().back()](PhysicalRegister *physReg, StackSlot *slot, const BasicBlockBuilder &builder) {
                        std::shared_ptr<Register> dst = share(*physReg);
                        MemoryOperand src(share(*fp()), std::make_unique<StackRelativeOffsetImmediate>(endSlot, slot));

                        switch (physReg->kind()) {
                            case RegisterKind::kInteger: {
                                int width = slot->size();
                                ExtensionMode extensionMode = width == 8 ? ExtensionMode::kNo : ExtensionMode::kSign;
                                builder.add(std::make_unique<Load>(width, std::move(dst), std::move(src), extensionMode));
                                break;
                            }

                            case RegisterKind::kFloating: {
                                Precision precision;
                                switch (slot->size()) {
                                    case 4: precision = Precision::kSingle; break;
                                    case 8: precision = Precision::kDouble; break;
                                    default: std::unreachable();
                                }
                                builder.add(std::make_unique<FLoad>(precision, std::move(dst), std::move(src)));
                                break;
                            }

                            default: std::unreachable();
                        }
                    };

                    auto store = [endSlot = &F.stackFrame().back()](PhysicalRegister *physReg, StackSlot *slot, const BasicBlockBuilder &builder) {
                        MemoryOperand dst(share(*fp()), std::make_unique<StackRelativeOffsetImmediate>(endSlot, slot));
                        std::shared_ptr<Register> src = share(*physReg);

                        switch (physReg->kind()) {
                            case RegisterKind::kInteger: {
                                int width = slot->size();
                                builder.add(std::make_unique<Store>(width, std::move(dst), std::move(src)));
                                break;
                            }

                            case RegisterKind::kFloating: {
                                Precision precision;
                                switch (slot->size()) {
                                    case 4: precision = Precision::kSingle; break;
                                    case 8: precision = Precision::kDouble; break;
                                    default: std::unreachable();
                                }
                                builder.add(std::make_unique<FStore>(precision, std::move(dst), std::move(src)));
                                break;
                            }

                            default: std::unreachable();
                        }
                    };

                    try {
                        LinearScanAllocator allocator;
                        allocator.allocate(F, 8, virtRegs, physRegs, load, store);
                    } catch (const RegisterAllocationException &) {
                        NaiveAllocator allocator;
                        allocator.allocate(F, 8, virtRegs, physRegs, load, store);
                    }
                }

                std::unordered_set<PhysicalRegister *> save;
                for (const BasicBlock &B : F) {
                    for (const Instruction &I : B) {
                        for (Register *reg : def(I)) {
                            if (auto *physReg = dynamic_cast<PhysicalRegister *>(reg);
                                    physReg && !physReg->isVolatile() && physReg->isAllocatable()) {
                                save.insert(physReg);
                            }
                        }
                    }
                }
                save.erase(sp());
                save.erase(fp());
                BasicBlock *prologueBlock = &F.entry(),
                           *epilogueblock = nullptr;
                for (BasicBlock &B : F) {
                    if (dynamic_cast<const RISCVRet *>(&B.back())) {
                        assert(epilogueblock == nullptr);
                        epilogueblock = &B;
                    }
                }
                for (PhysicalRegister *physReg : save) {
                    StackSlot *startSlot = &F.stackFrame().front(),
                              *slot = &F.stackFrame().add(std::prev(F.stackFrame().end()), 8, 8);
                    MemoryOperand mem(share(*sp()), std::make_unique<StackRelativeOffsetImmediate>(startSlot, slot));
                    std::shared_ptr<Register> reg = share(*physReg);
                    switch (physReg->kind()) {
                    case RegisterKind::kInteger:
                        prologueBlock->add(std::prev(prologueBlock->end(), 2), std::make_unique<Store>(8, mem.clone(), reg));
                        epilogueblock->add(std::prev(epilogueblock->end(), 2), std::make_unique<Load>(8, reg, mem.clone()));
                        break;
                    case RegisterKind::kFloating:
                        prologueBlock->add(std::prev(prologueBlock->end(), 2), std::make_unique<FStore>(Precision::kDouble, mem.clone(), reg));
                        epilogueblock->add(std::prev(epilogueblock->end(), 2), std::make_unique<FLoad>(Precision::kDouble, reg, mem.clone()));
                        break;
                    default:
                        std::unreachable();
                    }
                }
            }
        }
    }

    passManager.runAfterRegisterAllocation(MM);

    int count = 0;
    for (mir::Function &F : MM.functions) {
        for (mir::BasicBlock &B : F) {
            B.setName(".L" + std::to_string(count++));
        }
    }

    mc::Program program;
    RISCVMCGen(&MM, &program).emit();

    mc::RISCVPassManager mcPassManager;
    mcPassManager.run(program);

    return program;
}
