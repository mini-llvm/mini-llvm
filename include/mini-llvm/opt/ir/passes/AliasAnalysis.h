// SPDX-License-Identifier: MIT

#pragma once

#include <memory>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/opt/ir/FunctionAnalysis.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

enum class AliasResult {
    kMayAlias,
    kMustAlias,
    kPartialAlias,
    kNoAlias,
};

class MINI_LLVM_EXPORT AliasAnalysis final : public FunctionAnalysis {
public:
    AliasAnalysis();

    ~AliasAnalysis() override;

    AliasAnalysis(const AliasAnalysis &) = delete;
    AliasAnalysis &operator=(const AliasAnalysis &) = delete;

    AliasAnalysis(AliasAnalysis &&) = delete;
    AliasAnalysis &operator=(AliasAnalysis &&) = delete;

    void runOnFunction(const Function &F) override;
    AliasResult alias(const Value &ptr1, const Value &ptr2) const;
    AliasResult alias(const Value &ptr1, int size1, const Value &ptr2, int size2) const;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace mini_llvm::ir
