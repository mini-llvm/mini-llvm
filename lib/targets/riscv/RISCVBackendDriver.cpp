// SPDX-License-Identifier: MIT

#include "mini-llvm/targets/riscv/RISCVBackendDriver.h"

#include <cassert>
#include <cstdlib>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <unordered_set>
#include <utility>

#include "mini-llvm/codegen/RegisterAllocator.h"
#include "mini-llvm/common/ExtensionMode.h"
#include "mini-llvm/common/Precision.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/mc/Module.h"
#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/BasicBlockBuilder.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/Add.h"
#include "mini-llvm/mir/Instruction/FakeUse.h"
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

using namespace mini_llvm;

class RISCVBackendDriver::Impl {
public:
    explicit Impl(RegisterAllocator *allocator)
        : allocator_(allocator) {}

    void run(const ir::Module &IM, mir::Module &MM, mc::Module &MCM) {
        mir::RISCVRegister *sp = mir::RISCVRegister::get("sp"),
                           *fp = mir::RISCVRegister::get("fp"),
                           *t6 = mir::RISCVRegister::get("t6");

        RISCVMIRGen(&IM, &MM).emit();

        {
            using namespace mir;

            for (Function &F : functions(MM)) {
                for (BasicBlock &B : F) {
                    if (&B != &F.entry() && !dynamic_cast<const RISCVRet *>(&B.back())) {
                        B.add(std::prev(B.end()), std::make_unique<FakeUse>(fp));
                    }
                }
            }
        }

        mir::RISCVPassManager passManager;
        passManager.runBeforeRegisterAllocation(MM);

        {
            using namespace mir;

            for (Function &F : functions(MM)) {
                if (!F.isDeclaration()) {
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
                        physRegs.erase(sp);
                        physRegs.erase(fp);
                        physRegs.erase(t6);

                        StackSlot *endSlot = &F.stackFrame().back();

                        std::move_only_function<void (Register *reg, StackSlot *slot, const BasicBlockBuilder &builder)> load, store;

                        switch (Class) {
                            case RegisterClass::kGPR:
                                load = [fp, t6, endSlot](Register *reg, StackSlot *slot, const BasicBlockBuilder &builder) {
                                    int width = slot->size();
                                    std::shared_ptr<Register> dst = share(*reg);
                                    ExtensionMode extMode = width == 8 ? ExtensionMode::kNo : ExtensionMode::kSign;
                                    builder.add(std::make_unique<LI>(8, share(*t6), std::make_unique<StackOffsetImmediate>(endSlot, slot)));
                                    builder.add(std::make_unique<Add>(8, share(*t6), share(*t6), share(*fp)));
                                    builder.add(std::make_unique<Load>(width, std::move(dst), MemoryOperand(share(*t6)), extMode));
                                };

                                store = [fp, t6, endSlot](Register *reg, StackSlot *slot, const BasicBlockBuilder &builder) {
                                    int width = slot->size();
                                    std::shared_ptr<Register> src = share(*reg);
                                    builder.add(std::make_unique<LI>(8, share(*t6), std::make_unique<StackOffsetImmediate>(endSlot, slot)));
                                    builder.add(std::make_unique<Add>(8, share(*t6), share(*t6), share(*fp)));
                                    builder.add(std::make_unique<Store>(width, MemoryOperand(share(*t6)), std::move(src)));
                                };

                                break;

                            case RegisterClass::kFPR:
                                load = [fp, t6, endSlot](Register *reg, StackSlot *slot, const BasicBlockBuilder &builder) {
                                    Precision precision;
                                    switch (slot->size()) {
                                        case 4: precision = Precision::kSingle; break;
                                        case 8: precision = Precision::kDouble; break;
                                        default: abort();
                                    }
                                    std::shared_ptr<Register> dst = share(*reg);
                                    builder.add(std::make_unique<LI>(8, share(*t6), std::make_unique<StackOffsetImmediate>(endSlot, slot)));
                                    builder.add(std::make_unique<Add>(8, share(*t6), share(*t6), share(*fp)));
                                    builder.add(std::make_unique<FLoad>(precision, std::move(dst), MemoryOperand(share(*t6))));
                                };

                                store = [fp, t6, endSlot](Register *reg, StackSlot *slot, const BasicBlockBuilder &builder) {
                                    Precision precision;
                                    switch (slot->size()) {
                                        case 4: precision = Precision::kSingle; break;
                                        case 8: precision = Precision::kDouble; break;
                                        default: abort();
                                    }
                                    std::shared_ptr<Register> src = share(*reg);
                                    builder.add(std::make_unique<LI>(8, share(*t6), std::make_unique<StackOffsetImmediate>(endSlot, slot)));
                                    builder.add(std::make_unique<Add>(8, share(*t6), share(*t6), share(*fp)));
                                    builder.add(std::make_unique<FStore>(precision, MemoryOperand(share(*t6)), std::move(src)));
                                };

                                break;
                        }

                        if (!allocator_->allocate(F, virtRegs, physRegs, load, store)) {
                            abort();
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
                    save.erase(sp);
                    save.erase(fp);

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
                            prologueBlock->add(savePos, std::make_unique<LI>(8, share(*t6), std::make_unique<StackOffsetImmediate>(startSlot, slot)));
                            prologueBlock->add(savePos, std::make_unique<Add>(8, share(*t6), share(*t6), share(*sp)));
                            prologueBlock->add(savePos, std::make_unique<Store>(8, MemoryOperand(share(*t6)), reg));
                            epilogueBlock->add(restorePos, std::make_unique<LI>(8, share(*t6), std::make_unique<StackOffsetImmediate>(startSlot, slot)));
                            epilogueBlock->add(restorePos, std::make_unique<Add>(8, share(*t6), share(*t6), share(*sp)));
                            epilogueBlock->add(restorePos, std::make_unique<Load>(8, reg, MemoryOperand(share(*t6))));
                            break;
                        case RegisterClass::kFPR:
                            prologueBlock->add(savePos, std::make_unique<LI>(8, share(*t6), std::make_unique<StackOffsetImmediate>(startSlot, slot)));
                            prologueBlock->add(savePos, std::make_unique<Add>(8, share(*t6), share(*t6), share(*sp)));
                            prologueBlock->add(savePos, std::make_unique<FStore>(Precision::kDouble, MemoryOperand(share(*t6)), reg));
                            epilogueBlock->add(restorePos, std::make_unique<LI>(8, share(*t6), std::make_unique<StackOffsetImmediate>(startSlot, slot)));
                            epilogueBlock->add(restorePos, std::make_unique<Add>(8, share(*t6), share(*t6), share(*sp)));
                            epilogueBlock->add(restorePos, std::make_unique<FLoad>(Precision::kDouble, reg, MemoryOperand(share(*t6))));
                            break;
                        default:
                            abort();
                        }
                    }

                    prologueBlock->remove(savePos);
                    epilogueBlock->remove(restorePos);

                    for (BasicBlock &B : F) {
                        for (BasicBlock::const_iterator i = B.begin(); i != B.end();) {
                            if (dynamic_cast<const FakeUse *>(&*i)) {
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

        RISCVMCGen(&MM, &MCM).emit();

        mc::RISCVPassManager mcPassManager;
        mcPassManager.run(MCM);
    }

private:
    RegisterAllocator *allocator_;
};

RISCVBackendDriver::RISCVBackendDriver(RegisterAllocator *allocator)
    : impl_(std::make_unique<Impl>(allocator)) {}

RISCVBackendDriver::~RISCVBackendDriver() = default;

RISCVBackendDriver::RISCVBackendDriver(RISCVBackendDriver &&) noexcept = default;

RISCVBackendDriver &RISCVBackendDriver::operator=(RISCVBackendDriver &&) noexcept = default;

void RISCVBackendDriver::run(const ir::Module &IM, mir::Module &MM, mc::Module &MCM) {
    impl_->run(IM, MM, MCM);
}
