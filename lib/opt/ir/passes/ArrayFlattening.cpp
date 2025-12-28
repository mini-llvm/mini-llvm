// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/ArrayFlattening.h"

#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant/I64Constant.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Add.h"
#include "mini-llvm/ir/Instruction/GetElementPtr.h"
#include "mini-llvm/ir/Instruction/Mul.h"
#include "mini-llvm/ir/Instruction/SExt.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/ArrayType.h"
#include "mini-llvm/ir/Type/I64.h"
#include "mini-llvm/ir/Type/I8.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::ir;

bool ArrayFlattening::runOnFunction(Function &F) {
    bool changed = false;

    for (BasicBlock &B : F) {
        for (auto i = B.begin(); i != B.end();) {
            Instruction &I = *i++;
            if (auto *gep = dynamic_cast<GetElementPtr *>(&I)) {
                size_t n = gep->idx_size();
                std::unique_ptr<Type> type = gep->sourceType();
                std::vector<int> sizes;
                if (n > 0) {
                    sizes.push_back(type->size(pointerSize_));
                    for (size_t j = 1; j < n; ++j) {
                        type = static_cast<const ArrayType *>(&*type)->elementType();
                        sizes.push_back(type->size(pointerSize_));
                    }
                }
                if (*type != I8() || n > 1) {
                    std::vector<std::shared_ptr<Value>> indices;
                    if (n > 0) {
                        std::vector<std::shared_ptr<Value>> terms;
                        for (size_t j = 0; j < n; ++j) {
                            std::shared_ptr<Value> idx = share(*gep->idx(j));
                            if (*idx->type() != I64()) {
                                std::shared_ptr<Instruction> sext = std::make_shared<SExt>(idx, std::make_unique<I64>());
                                addToParent(*gep, sext);
                                idx = sext;
                            }
                            std::shared_ptr<Instruction> mul = std::make_shared<Mul>(
                                idx, std::make_shared<I64Constant>(sizes[j])
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

    assert(F.isWellFormed());
    return changed;
}
