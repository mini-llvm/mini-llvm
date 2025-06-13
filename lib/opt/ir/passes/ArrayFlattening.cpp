#include "mini-llvm/opt/ir/passes/ArrayFlattening.h"

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Add.h"
#include "mini-llvm/ir/Instruction/GetElementPtr.h"
#include "mini-llvm/ir/Instruction/Mul.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/ArrayType.h"
#include "mini-llvm/ir/Type/I8.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"

using namespace mini_llvm::ir;

bool ArrayFlattening::runOnFunction(Function &F) {
    bool changed = false;

    for (BasicBlock &B : F) {
        for (auto i = B.begin(); i != B.end();) {
            Instruction &I = *i++;
            if (auto *gep = dynamic_cast<GetElementPtr *>(&I)) {
                std::vector<int> sizes;
                std::unique_ptr<Type> type = gep->sourceType();
                for (;;) {
                    sizes.push_back(type->size(pointerSize_));
                    if (!dynamic_cast<const ArrayType *>(&*type)) {
                        break;
                    }
                    type = static_cast<const ArrayType *>(&*type)->elementType();
                }
                size_t n = gep->idx_size();
                if (*type != I8() || n > 1) {
                    std::vector<std::shared_ptr<Value>> indices;
                    if (n > 0) {
                        std::vector<std::shared_ptr<Value>> terms;
                        for (size_t j = 0; j < n; ++j) {
                            std::shared_ptr<Instruction> mul = std::make_shared<Mul>(
                                share(*gep->idx(j)), gep->idx(j)->type()->constant(sizes[j])
                            );
                            addToParent(*gep, mul);
                            terms.push_back(mul);
                        }
                        std::shared_ptr<Value> sum = terms[0];
                        for (size_t j = 1; j < n; ++j) {
                            std::shared_ptr<Instruction> add = std::make_shared<Add>(sum, terms[j]);
                            addToParent(*gep, add);
                            sum = add;
                        }
                        indices.push_back(sum);
                    }
                    std::shared_ptr<Instruction> newGep = std::make_shared<GetElementPtr>(
                        std::make_unique<I8>(), share(*gep->ptr()), std::move(indices)
                    );
                    addToParent(*gep, newGep);
                    replaceAllUsesWith(*gep, newGep);
                    removeFromParent(*gep);
                    changed = true;
                }
            }
        }
    }

    return changed;
}
