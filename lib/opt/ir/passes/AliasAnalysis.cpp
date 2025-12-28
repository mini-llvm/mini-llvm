// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/AliasAnalysis.h"

#include <cassert>
#include <memory>

#include "mini-llvm/ir/Argument.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant/NullPtrConstant.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/GlobalValue.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Instruction/GetElementPtr.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"
#include "mini-llvm/utils/HashMap.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

void dfs(const DTNode *node, HashMap<const Value *, const Value *> &objects) {
    for (const Instruction &I : *node->block) {
        if (objects.find(&I) == objects.end()) {
            if (auto *gep = dynamic_cast<const GetElementPtr *>(&I)) {
                const Value *ptr = &*gep->ptr();
                if (dynamic_cast<const GlobalValue *>(ptr) ||
                        dynamic_cast<const Argument *>(ptr) ||
                        dynamic_cast<const Alloca *>(ptr)) {
                    objects.put(&I, ptr);
                    continue;
                }
                if (auto i = objects.find(ptr); i != objects.end()) {
                    objects.put(&I, i->second);
                    continue;
                }
                continue;
            }
            if (auto *phi = dynamic_cast<const Phi *>(&I)) {
                if (*phi->type() == Ptr() && phi->incoming_size() == 1) {
                    const Value *ptr = &*phi->incoming_begin()->value;
                    if (dynamic_cast<const GlobalValue *>(ptr) ||
                            dynamic_cast<const Argument *>(ptr) ||
                            dynamic_cast<const Alloca *>(ptr)) {
                        objects.put(&I, ptr);
                        continue;
                    }
                    if (auto i = objects.find(ptr); i != objects.end()) {
                        objects.put(&I, i->second);
                        continue;
                    }
                    continue;
                }
                continue;
            }
        }
    }
    for (const DTNode *child : node->children) {
        dfs(child, objects);
    }
}

} // namespace

class AliasAnalysis::Impl {
public:
    void runOnFunction(const Function &F) {
        DominatorTreeAnalysis domTree;
        domTree.runOnFunction(F);

        dfs(domTree.node(F.entry()), objects_);
    }

    AliasResult alias(const Value &ptr1, const Value &ptr2) const {
        assert(*ptr1.type() == Ptr());
        assert(*ptr2.type() == Ptr());

        if (dynamic_cast<const NullPtrConstant *>(&ptr1) || dynamic_cast<const NullPtrConstant *>(&ptr2)) {
            return AliasResult::kNoAlias;
        }
        const Value *object1 = objects_.get(&ptr1).value_or(&ptr1),
                    *object2 = objects_.get(&ptr2).value_or(&ptr2);
        if ((dynamic_cast<const GlobalValue *>(object1) && dynamic_cast<const GlobalValue *>(object2) && object1 != object2) ||
                (dynamic_cast<const Alloca *>(object1) && dynamic_cast<const Alloca *>(object2) && object1 != object2) ||
                (dynamic_cast<const GlobalValue *>(object1) && dynamic_cast<const Alloca *>(object2)) ||
                (dynamic_cast<const Alloca *>(object1) && dynamic_cast<const GlobalValue *>(object2)) ||
                (dynamic_cast<const Argument *>(object1) && dynamic_cast<const Alloca *>(object2)) ||
                (dynamic_cast<const Alloca *>(object1) && dynamic_cast<const Argument *>(object2))) {
            return AliasResult::kNoAlias;
        }
        return AliasResult::kMayAlias;
    }

    AliasResult alias(const Value &ptr1, int size1, const Value &ptr2, int size2) const {
        assert(*ptr1.type() == Ptr());
        assert(size1 > 0);
        assert(*ptr2.type() == Ptr());
        assert(size2 > 0);

        if (dynamic_cast<const NullPtrConstant *>(&ptr1) || dynamic_cast<const NullPtrConstant *>(&ptr2)) {
            return AliasResult::kNoAlias;
        }
        if (&ptr1 == &ptr2 && size1 == size2) {
            return AliasResult::kMustAlias;
        }
        if (&ptr1 == &ptr2 && size1 != size2) {
            return AliasResult::kPartialAlias;
        }
        const Value *object1 = objects_.get(&ptr1).value_or(&ptr1),
                    *object2 = objects_.get(&ptr2).value_or(&ptr2);
        if ((dynamic_cast<const GlobalValue *>(object1) && dynamic_cast<const GlobalValue *>(object2) && object1 != object2) ||
                (dynamic_cast<const Alloca *>(object1) && dynamic_cast<const Alloca *>(object2) && object1 != object2) ||
                (dynamic_cast<const GlobalValue *>(object1) && dynamic_cast<const Alloca *>(object2)) ||
                (dynamic_cast<const Alloca *>(object1) && dynamic_cast<const GlobalValue *>(object2)) ||
                (dynamic_cast<const Argument *>(object1) && dynamic_cast<const Alloca *>(object2)) ||
                (dynamic_cast<const Alloca *>(object1) && dynamic_cast<const Argument *>(object2))) {
            return AliasResult::kNoAlias;
        }
        return AliasResult::kMayAlias;
    }

private:
    HashMap<const Value *, const Value *> objects_;
};

AliasAnalysis::AliasAnalysis() : impl_(std::make_unique<Impl>()) {}

AliasAnalysis::~AliasAnalysis() = default;

void AliasAnalysis::runOnFunction(const Function &F) {
    impl_->runOnFunction(F);
}

AliasResult AliasAnalysis::alias(const Value &ptr1, const Value &ptr2) const {
    return impl_->alias(ptr1, ptr2);
}

AliasResult AliasAnalysis::alias(const Value &ptr1, int size1, const Value &ptr2, int size2) const {
    return impl_->alias(ptr1, size1, ptr2, size2);
}
