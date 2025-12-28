// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/mir/passes/LiveVariableAnalysis.h"

#include <memory>
#include <ranges>
#include <unordered_set>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/SetOps.h"

using namespace mini_llvm::mir;
using namespace mini_llvm::set_ops;

class LiveVariableAnalysis::Impl {
public:
    void runOnFunction(const Function &F) {
        HashMap<const BasicBlock *, std::unordered_set<Register *>> Use, Def;
        for (const BasicBlock &B : F) {
            Use.put(&B, use(B));
            Def.put(&B, def(B));
        }
        for (const BasicBlock &B : F) {
            liveIn_.put(&B, {});
            liveOut_.put(&B, {});
        }

        bool changed;
        do {
            changed = false;
            for (const BasicBlock &B : F) {
                std::unordered_set<Register *> liveOut;
                for (const BasicBlock *successor : successors(B)) {
                    liveOut |= liveIn_[successor];
                }
                std::unordered_set<Register *> liveIn = (liveOut - Def[&B]) | Use[&B];
                if (liveOut != liveOut_[&B]) {
                    liveOut_[&B] = liveOut;
                    changed = true;
                }
                if (liveIn != liveIn_[&B]) {
                    liveIn_[&B] = liveIn;
                    changed = true;
                }
            }
        } while (changed);

        for (const BasicBlock &B : F) {
            std::unordered_set<Register *> live(liveOut_[&B]);
            for (const Instruction &I : std::views::reverse(B)) {
                liveOut2_.put(&I, live);
                live = (live - def(I)) | use(I);
                liveIn2_.put(&I, live);
            }
        }
    }

    std::unordered_set<Register *> liveIn(const BasicBlock &B) const {
        return liveIn_[&B];
    }

    std::unordered_set<Register *> liveOut(const BasicBlock &B) const {
        return liveOut_[&B];
    }

    std::unordered_set<Register *> liveIn(const Instruction &I) const {
        return liveIn2_[&I];
    }

    std::unordered_set<Register *> liveOut(const Instruction &I) const {
        return liveOut2_[&I];
    }

private:
    HashMap<const BasicBlock *, std::unordered_set<Register *>> liveIn_, liveOut_;
    HashMap<const Instruction *, std::unordered_set<Register *>> liveIn2_, liveOut2_;
};

LiveVariableAnalysis::LiveVariableAnalysis() : impl_(std::make_unique<Impl>()) {}

LiveVariableAnalysis::~LiveVariableAnalysis() = default;

void LiveVariableAnalysis::runOnFunction(const Function &F) {
    impl_->runOnFunction(F);
}

std::unordered_set<Register *> LiveVariableAnalysis::liveIn(const BasicBlock &B) const {
    return impl_->liveIn(B);
}

std::unordered_set<Register *> LiveVariableAnalysis::liveOut(const BasicBlock &B) const {
    return impl_->liveOut(B);
}

std::unordered_set<Register *> LiveVariableAnalysis::liveIn(const Instruction &I) const {
    return impl_->liveIn(I);
}

std::unordered_set<Register *> LiveVariableAnalysis::liveOut(const Instruction &I) const {
    return impl_->liveOut(I);
}
