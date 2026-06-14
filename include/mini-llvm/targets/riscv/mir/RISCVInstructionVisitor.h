// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib> // IWYU pragma: keep
#include <utility>

#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class RISCVCall;
class RISCVJALR;
class RISCVRet;

class MINI_LLVM_EXPORT RISCVInstructionVisitor : public InstructionVisitor {
public:
    virtual void visitRISCVCall(RISCVCall &I) { visitRISCVCall(std::as_const(I)); }
    virtual void visitRISCVJALR(RISCVJALR &I) { visitRISCVJALR(std::as_const(I)); }
    virtual void visitRISCVRet(RISCVRet &I) { visitRISCVRet(std::as_const(I)); }

    virtual void visitRISCVCall(const RISCVCall &/*I*/) { abort(); }
    virtual void visitRISCVJALR(const RISCVJALR &/*I*/) { abort(); }
    virtual void visitRISCVRet(const RISCVRet &/*I*/) { abort(); }
};

} // namespace mini_llvm::mir
