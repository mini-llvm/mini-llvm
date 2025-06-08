#pragma once

#include <cstdlib>
#include <utility>

#include "mini-llvm/mir/InstructionVisitor.h"

namespace mini_llvm::mir {

class RISCVCall;
class RISCVJALR;
class RISCVRet;

class RISCVInstructionVisitor : public InstructionVisitor {
public:
    virtual void visitRISCVCall(RISCVCall &I) { visitRISCVCall(std::as_const(I)); }
    virtual void visitRISCVJALR(RISCVJALR &I) { visitRISCVJALR(std::as_const(I)); }
    virtual void visitRISCVRet(RISCVRet &I) { visitRISCVRet(std::as_const(I)); }

    virtual void visitRISCVCall(const RISCVCall &) { abort(); }
    virtual void visitRISCVJALR(const RISCVJALR &) { abort(); }
    virtual void visitRISCVRet(const RISCVRet &) { abort(); }
};

} // namespace mini_llvm::mir
