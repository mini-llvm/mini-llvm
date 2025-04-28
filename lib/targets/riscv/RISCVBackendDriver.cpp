#include "mini-llvm/targets/riscv/RISCVBackendDriver.h"

#include <cassert>
#include <cstdlib>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "mini-llvm/codegen/register_allocators/LinearScanAllocator.h"
#include "mini-llvm/codegen/register_allocators/NaiveAllocator.h"
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
#include "mini-llvm/mir/Instruction/FMov.h"
#include "mini-llvm/mir/Instruction/FStore.h"
#include "mini-llvm/mir/Instruction/LI.h"
#include "mini-llvm/mir/Instruction/Live.h"
#include "mini-llvm/mir/Instruction/Load.h"
#include "mini-llvm/mir/Instruction/Marker.h"
#include "mini-llvm/mir/Instruction/Mov.h"
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

using namespace mini_llvm;

mini_llvm::mc::Program RISCVBackendDriver::run(const ir::Module &IM) {
    mir::Module MM;
    RISCVMIRGen(&IM, &MM).emit();

    {
        using namespace mir;
        using namespace mir::riscv;

        for (Function &F : functions(MM)) {
            for (BasicBlock &B : F) {
                if (&B != &F.entry() && !dynamic_cast<const RISCVRet *>(&B.back())) {
                    B.add(std::prev(B.end()), std::make_unique<Live>(fp()));
                }
            }
        }
    }

    mir::RISCVPassManager passManager;
    passManager.runBeforeRegisterAllocation(MM);

    {
        using namespace mir;
        using namespace mir::riscv;

        for (Function &F : functions(MM)) {
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
                    physRegs.erase(t6());

                    StackSlot *endSlot = &F.stackFrame().back();

                    auto load = [endSlot](PhysicalRegister *physReg, StackSlot *slot, const BasicBlockBuilder &builder) {
                        std::shared_ptr<Register> dst = share(*physReg);

                        switch (physReg->Class()) {
                            case RegisterClass::kGPR: {
                                int width = slot->size();
                                ExtensionMode extMode = width == 8 ? ExtensionMode::kNo : ExtensionMode::kSign;
                                builder.add(std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(endSlot, slot)));
                                builder.add(std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*fp())));
                                builder.add(std::make_unique<Load>(width, std::move(dst), MemoryOperand(share(*t6())), extMode));
                                break;
                            }

                            case RegisterClass::kFPR: {
                                Precision precision;
                                switch (slot->size()) {
                                    case 4: precision = Precision::kSingle; break;
                                    case 8: precision = Precision::kDouble; break;
                                    default: abort();
                                }
                                builder.add(std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(endSlot, slot)));
                                builder.add(std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*fp())));
                                builder.add(std::make_unique<FLoad>(precision, std::move(dst), MemoryOperand(share(*t6()))));
                                break;
                            }

                            default: abort();
                        }
                    };

                    auto store = [endSlot](PhysicalRegister *physReg, StackSlot *slot, const BasicBlockBuilder &builder) {
                        std::shared_ptr<Register> src = share(*physReg);

                        switch (physReg->Class()) {
                            case RegisterClass::kGPR: {
                                int width = slot->size();
                                builder.add(std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(endSlot, slot)));
                                builder.add(std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*fp())));
                                builder.add(std::make_unique<Store>(width, MemoryOperand(share(*t6())), std::move(src)));
                                break;
                            }

                            case RegisterClass::kFPR: {
                                Precision precision;
                                switch (slot->size()) {
                                    case 4: precision = Precision::kSingle; break;
                                    case 8: precision = Precision::kDouble; break;
                                    default: abort();
                                }
                                builder.add(std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(endSlot, slot)));
                                builder.add(std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*fp())));
                                builder.add(std::make_unique<FStore>(precision, MemoryOperand(share(*t6())), std::move(src)));
                                break;
                            }

                            default: abort();
                        }
                    };

                    std::unordered_multimap<VirtualRegister *, PhysicalRegister *> hints;

                    for (const BasicBlock &B : F) {
                        for (const Instruction &I : B) {
                            if (auto *mov = dynamic_cast<const Mov *>(&I)) {
                                if (auto *physReg = dynamic_cast<PhysicalRegister *>(&*mov->dst())) {
                                    if (auto *virtReg = dynamic_cast<VirtualRegister *>(&*mov->src())) {
                                        hints.emplace(virtReg, physReg);
                                    }
                                }
                                if (auto *virtReg = dynamic_cast<VirtualRegister *>(&*mov->dst())) {
                                    if (auto *physReg = dynamic_cast<PhysicalRegister *>(&*mov->src())) {
                                        hints.emplace(virtReg, physReg);
                                    }
                                }
                            }
                            if (auto *fmov = dynamic_cast<const FMov *>(&I)) {
                                if (auto *physReg = dynamic_cast<PhysicalRegister *>(&*fmov->dst())) {
                                    if (auto *virtReg = dynamic_cast<VirtualRegister *>(&*fmov->src())) {
                                        hints.emplace(virtReg, physReg);
                                    }
                                }
                                if (auto *virtReg = dynamic_cast<VirtualRegister *>(&*fmov->dst())) {
                                    if (auto *physReg = dynamic_cast<PhysicalRegister *>(&*fmov->src())) {
                                        hints.emplace(virtReg, physReg);
                                    }
                                }
                            }
                        }
                    }

                    if (LinearScanAllocator allocator; !allocator.allocate(F, 8, virtRegs, physRegs, load, store, hints)) {
                        if (NaiveAllocator allocator; !allocator.allocate(F, 8, virtRegs, physRegs, load, store, hints)) {
                            assert(false);
                            abort();
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
                if (!epilogueBlock) {
                    epilogueBlock = &F.append();
                    epilogueBlock->append(std::make_unique<Marker>(RISCVMIRGen::kRestore));
                    epilogueBlock->append(std::make_unique<RISCVRet>(0, 0));
                }

                BasicBlock::const_iterator savePos,
                                           restorePos;
                for (BasicBlock::const_iterator i = prologueBlock->begin(), e = prologueBlock->end(); i != e; ++i) {
                    if (auto *marker = dynamic_cast<const Marker *>(&*i); marker && marker->id() == RISCVMIRGen::kSave) {
                        savePos = i;
                    }
                }
                for (BasicBlock::const_iterator i = epilogueBlock->begin(), e = epilogueBlock->end(); i != e; ++i) {
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
                        prologueBlock->add(savePos, std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(startSlot, slot)));
                        prologueBlock->add(savePos, std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*sp())));
                        prologueBlock->add(savePos, std::make_unique<Store>(8, MemoryOperand(share(*t6())), reg));
                        epilogueBlock->add(restorePos, std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(startSlot, slot)));
                        epilogueBlock->add(restorePos, std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*sp())));
                        epilogueBlock->add(restorePos, std::make_unique<Load>(8, reg, MemoryOperand(share(*t6()))));
                        break;
                    case RegisterClass::kFPR:
                        prologueBlock->add(savePos, std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(startSlot, slot)));
                        prologueBlock->add(savePos, std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*sp())));
                        prologueBlock->add(savePos, std::make_unique<FStore>(Precision::kDouble, MemoryOperand(share(*t6())), reg));
                        epilogueBlock->add(restorePos, std::make_unique<LI>(8, share(*t6()), std::make_unique<StackOffsetImmediate>(startSlot, slot)));
                        epilogueBlock->add(restorePos, std::make_unique<Add>(8, share(*t6()), share(*t6()), share(*sp())));
                        epilogueBlock->add(restorePos, std::make_unique<FLoad>(Precision::kDouble, reg, MemoryOperand(share(*t6()))));
                        break;
                    default:
                        abort();
                    }
                }

                prologueBlock->remove(savePos);
                epilogueBlock->remove(restorePos);

                for (BasicBlock &B : F) {
                    for (BasicBlock::const_iterator i = B.begin(); i != B.end();) {
                        if (dynamic_cast<const Live *>(&*i)) {
                            B.remove(i++);
                        } else {
                            ++i;
                        }
                    }
                }
            }
        }
    }

    passManager.runAfterRegisterAllocation(MM);

    mc::Program program;
    RISCVMCGen(&MM, &program).emit();

    mc::RISCVPassManager mcPassManager;
    mcPassManager.run(program);

    return program;
}
