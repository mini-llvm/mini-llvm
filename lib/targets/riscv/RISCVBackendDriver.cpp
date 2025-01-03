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
#include "mini-llvm/common/ExtensionMode.h"
#include "mini-llvm/common/Precision.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/mc/Program.h"
#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/BasicBlockBuilder.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/Add.h"
#include "mini-llvm/mir/Instruction/FLoad.h"
#include "mini-llvm/mir/Instruction/FStore.h"
#include "mini-llvm/mir/Instruction/LI.h"
#include "mini-llvm/mir/Instruction/Load.h"
#include "mini-llvm/mir/Instruction/Marker.h"
#include "mini-llvm/mir/Instruction/Store.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterClass.h"
#include "mini-llvm/mir/StackOffsetImmediate.h"
#include "mini-llvm/mir/StackSlot.h"
#include "mini-llvm/mir/VirtualRegister.h"
#include "mini-llvm/targets/riscv/codegen/RISCVMCGen.h"
#include "mini-llvm/targets/riscv/codegen/RISCVMIRGen.h"
#include "mini-llvm/targets/riscv/mir/Instruction/RISCVRet.h"
#include "mini-llvm/targets/riscv/mir/RISCVRegister.h"
#include "mini-llvm/targets/riscv/opt/mc/RISCVPassManager.h"
#include "mini-llvm/targets/riscv/opt/mir/RISCVPassManager.h"
#include "mini-llvm/utils/Memory.h"
#include "mini-llvm/utils/Panic.h"

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
                for (RegisterClass Class : {RegisterClass::kGPR, RegisterClass::kFPR}) {
                    std::unordered_set<VirtualRegister *> virtRegs;
                    for (const BasicBlock &B : F) {
                        for (const Instruction &I : B) {
                            for (const RegisterOperand *op : I.regOps()) {
                                if (op->Class() == Class) {
                                    if (auto *virtReg = dynamic_cast<VirtualRegister *>(&**op)) {
                                        virtRegs.insert(virtReg);
                                    }
                                }
                            }
                        }
                    }

                    std::unordered_set<PhysicalRegister *> physRegs;
                    for (PhysicalRegister *physReg : riscvRegs()) {
                        if (physReg->Class() == Class && physReg->isAllocatable()) {
                            physRegs.insert(physReg);
                        }
                    }
                    physRegs.erase(sp());
                    physRegs.erase(fp());

                    StackSlot *endSlot = &F.stackFrame().back();

                    if (endSlot->offset() >= 2048) {
                        physRegs.erase(t6());
                    }

                    auto load = [endSlot](PhysicalRegister *physReg, StackSlot *slot, const BasicBlockBuilder &builder) {
                        std::shared_ptr<Register> dst = share(*physReg);

                        switch (physReg->Class()) {
                            case RegisterClass::kGPR: {
                                int width = slot->size();
                                ExtensionMode extensionMode = width == 8 ? ExtensionMode::kNo : ExtensionMode::kSign;
                                if (endSlot->offset() - slot->offset() < 2048) {
                                    MemoryOperand src(share(*fp()), std::make_unique<StackOffsetImmediate>(endSlot, slot));
                                    builder.add(std::make_unique<Load>(width, std::move(dst), std::move(src), extensionMode));
                                } else {
                                    builder.add(std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(endSlot, slot)));
                                    builder.add(std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*fp())));
                                    builder.add(std::make_unique<Load>(width, std::move(dst), MemoryOperand(share(*t6())), extensionMode));
                                }
                                break;
                            }

                            case RegisterClass::kFPR: {
                                Precision precision;
                                switch (slot->size()) {
                                    case 4: precision = Precision::kSingle; break;
                                    case 8: precision = Precision::kDouble; break;
                                    default: panic();
                                }
                                if (endSlot->offset() - slot->offset() < 2048) {
                                    MemoryOperand src(share(*fp()), std::make_unique<StackOffsetImmediate>(endSlot, slot));
                                    builder.add(std::make_unique<FLoad>(precision, std::move(dst), std::move(src)));
                                } else {
                                    builder.add(std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(endSlot, slot)));
                                    builder.add(std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*fp())));
                                    builder.add(std::make_unique<FLoad>(precision, std::move(dst), MemoryOperand(share(*t6()))));
                                }
                                break;
                            }

                            default: panic();
                        }
                    };

                    auto store = [endSlot](PhysicalRegister *physReg, StackSlot *slot, const BasicBlockBuilder &builder) {
                        std::shared_ptr<Register> src = share(*physReg);

                        switch (physReg->Class()) {
                            case RegisterClass::kGPR: {
                                int width = slot->size();
                                if (endSlot->offset() - slot->offset() < 2048) {
                                    MemoryOperand dst(share(*fp()), std::make_unique<StackOffsetImmediate>(endSlot, slot));
                                    builder.add(std::make_unique<Store>(width, std::move(dst), std::move(src)));
                                } else {
                                    builder.add(std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(endSlot, slot)));
                                    builder.add(std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*fp())));
                                    builder.add(std::make_unique<Store>(width, MemoryOperand(share(*t6())), std::move(src)));
                                }
                                break;
                            }

                            case RegisterClass::kFPR: {
                                Precision precision;
                                switch (slot->size()) {
                                    case 4: precision = Precision::kSingle; break;
                                    case 8: precision = Precision::kDouble; break;
                                    default: panic();
                                }
                                if (endSlot->offset() - slot->offset() < 2048) {
                                    MemoryOperand dst(share(*fp()), std::make_unique<StackOffsetImmediate>(endSlot, slot));
                                    builder.add(std::make_unique<FStore>(precision, std::move(dst), std::move(src)));
                                } else {
                                    builder.add(std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(endSlot, slot)));
                                    builder.add(std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*fp())));
                                    builder.add(std::make_unique<FStore>(precision, MemoryOperand(share(*t6())), std::move(src)));
                                }
                                break;
                            }

                            default: panic();
                        }
                    };

                    LinearScanAllocator allocator;
                    if (!allocator.allocate(F, 8, virtRegs, physRegs, load, store)) {
                        NaiveAllocator allocator;
                        if (!allocator.allocate(F, 8, virtRegs, physRegs, load, store)) {
                            panic();
                        }
                    }
                }

                std::unordered_set<PhysicalRegister *> save;
                for (const BasicBlock &B : F) {
                    for (const Instruction &I : B) {
                        for (Register *reg : def(I)) {
                            if (auto *physReg = dynamic_cast<PhysicalRegister *>(reg);
                                    physReg && physReg->isPreserved() && physReg->isAllocatable()) {
                                save.insert(physReg);
                            }
                        }
                    }
                }
                save.erase(sp());
                save.erase(fp());

                BasicBlock *prologueBlock = &F.entry(),
                           *epilogueBlock = nullptr;
                for (BasicBlock &B : F) {
                    if (dynamic_cast<const RISCVRet *>(&B.back())) {
                        assert(epilogueBlock == nullptr);
                        epilogueBlock = &B;
                    }
                }

                BasicBlock::const_iterator savePos,
                                           restorePos;
                for (BasicBlock::const_iterator i = prologueBlock->begin(); i != prologueBlock->end(); ++i) {
                    if (auto *marker = dynamic_cast<const Marker *>(&*i); marker && marker->id() == RISCVMIRGen::kSave) {
                        savePos = i;
                    }
                }
                for (BasicBlock::const_iterator i = epilogueBlock->begin(); i != epilogueBlock->end(); ++i) {
                    if (auto *marker = dynamic_cast<const Marker *>(&*i); marker && marker->id() == RISCVMIRGen::kRestore) {
                        restorePos = i;
                    }
                }

                for (PhysicalRegister *physReg : save) {
                    StackSlot *startSlot = &F.stackFrame().front(),
                              *slot = &F.stackFrame().add(std::prev(F.stackFrame().end()), 8, 8);
                    std::shared_ptr<Register> reg = share(*physReg);
                    switch (physReg->Class()) {
                    case RegisterClass::kGPR:
                        if (slot->offset() < 2048) {
                            MemoryOperand mem(share(*sp()), std::make_unique<StackOffsetImmediate>(startSlot, slot));
                            prologueBlock->add(savePos, std::make_unique<Store>(8, mem.clone(), reg));
                            epilogueBlock->add(restorePos, std::make_unique<Load>(8, reg, mem.clone()));
                        } else {
                            prologueBlock->add(
                                savePos,
                                std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(startSlot, slot)));
                            prologueBlock->add(
                                savePos,
                                std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*sp())));
                            prologueBlock->add(
                                savePos,
                                std::make_unique<Store>(8, MemoryOperand(share(*t6())), reg));
                            epilogueBlock->add(
                                restorePos,
                                std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(startSlot, slot)));
                            epilogueBlock->add(
                                restorePos,
                                std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*sp())));
                            epilogueBlock->add(
                                restorePos,
                                std::make_unique<Load>(8, reg, MemoryOperand(share(*t6()))));
                        }
                        break;
                    case RegisterClass::kFPR:
                        if (slot->offset() < 2048) {
                            MemoryOperand mem(share(*sp()), std::make_unique<StackOffsetImmediate>(startSlot, slot));
                            prologueBlock->add(std::prev(prologueBlock->end(), 2), std::make_unique<FStore>(Precision::kDouble, mem.clone(), reg));
                            epilogueBlock->add(std::prev(epilogueBlock->end(), 2), std::make_unique<FLoad>(Precision::kDouble, reg, mem.clone()));
                        } else {
                            prologueBlock->add(
                                savePos,
                                std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(startSlot, slot)));
                            prologueBlock->add(
                                savePos,
                                std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*sp())));
                            prologueBlock->add(
                                savePos,
                                std::make_unique<FStore>(Precision::kDouble, MemoryOperand(share(*t6())), reg));
                            epilogueBlock->add(
                                restorePos,
                                std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(startSlot, slot)));
                            epilogueBlock->add(
                                restorePos,
                                std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*sp())));
                            epilogueBlock->add(
                                restorePos,
                                std::make_unique<FLoad>(Precision::kDouble, reg, MemoryOperand(share(*t6()))));
                        }
                        break;
                    default:
                        panic();
                    }
                }

                prologueBlock->remove(savePos);
                epilogueBlock->remove(restorePos);
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
