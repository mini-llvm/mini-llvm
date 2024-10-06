#pragma once

#include <exception>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/opt/ir/FunctionAnalysis.h"
#include "mini-llvm/opt/ir/ModuleAnalysis.h"

namespace mini_llvm::ir {

class VerificationException : public std::exception {
public:
    constexpr const char *what() const noexcept override {
        return "VerificationException";
    }

private:
    std::string message_;
};

class VerificationAnalysis final : public FunctionAnalysis, public ModuleAnalysis {
public:
    void runOnFunction(const Function &F) override;
    void runOnModule(const Module &M) override;
};

} // namespace mini_llvm::ir
