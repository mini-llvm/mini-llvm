// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib>
#include <utility>

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class Add;
class AddI;
class And;
class AndI;
class Br;
class CmpBr;
class Cmp;
class CmpSet;
class CmpZBr;
class CmpZ;
class CmpZSet;
class CondBr;
class CondSet;
class FAdd;
class FCmp;
class FCmpSet;
class FCvt;
class FCvtFS;
class FCvtFU;
class FCvtSF;
class FCvtUF;
class FDiv;
class FLoad;
class FMov;
class FMovFI;
class FMovIF;
class FMul;
class FNeg;
class FStore;
class FSub;
class LA;
class LI;
class FakeUse;
class Load;
class Marker;
class Mov;
class Mul;
class Neg;
class Not;
class Or;
class OrI;
class SDiv;
class SExt;
class SHL;
class SHLI;
class SHRA;
class SHRAI;
class SHRL;
class SHRLI;
class SRem;
class Store;
class Sub;
class SubI;
class UDiv;
class URem;
class Xor;
class XorI;

class MINI_LLVM_EXPORT InstructionVisitor {
public:
    virtual ~InstructionVisitor() = default;

    InstructionVisitor() = default;

    InstructionVisitor(const InstructionVisitor &) = delete;
    InstructionVisitor &operator=(const InstructionVisitor &) = delete;

    InstructionVisitor(InstructionVisitor &&) = delete;
    InstructionVisitor &operator=(InstructionVisitor &&) = delete;

    virtual void visitAdd(Add &I) { visitAdd(std::as_const(I)); }
    virtual void visitAddI(AddI &I) { visitAddI(std::as_const(I)); }
    virtual void visitAnd(And &I) { visitAnd(std::as_const(I)); }
    virtual void visitAndI(AndI &I) { visitAndI(std::as_const(I)); }
    virtual void visitBr(Br &I) { visitBr(std::as_const(I)); }
    virtual void visitCmpBr(CmpBr &I) { visitCmpBr(std::as_const(I)); }
    virtual void visitCmp(Cmp &I) { visitCmp(std::as_const(I)); }
    virtual void visitCmpSet(CmpSet &I) { visitCmpSet(std::as_const(I)); }
    virtual void visitCmpZBr(CmpZBr &I) { visitCmpZBr(std::as_const(I)); }
    virtual void visitCmpZ(CmpZ &I) { visitCmpZ(std::as_const(I)); }
    virtual void visitCmpZSet(CmpZSet &I) { visitCmpZSet(std::as_const(I)); }
    virtual void visitCondBr(CondBr &I) { visitCondBr(std::as_const(I)); }
    virtual void visitCondSet(CondSet &I) { visitCondSet(std::as_const(I)); }
    virtual void visitFAdd(FAdd &I) { visitFAdd(std::as_const(I)); }
    virtual void visitFCmp(FCmp &I) { visitFCmp(std::as_const(I)); }
    virtual void visitFCmpSet(FCmpSet &I) { visitFCmpSet(std::as_const(I)); }
    virtual void visitFCvt(FCvt &I) { visitFCvt(std::as_const(I)); }
    virtual void visitFCvtFS(FCvtFS &I) { visitFCvtFS(std::as_const(I)); }
    virtual void visitFCvtFU(FCvtFS &I) { visitFCvtFS(std::as_const(I)); }
    virtual void visitFCvtSF(FCvtSF &I) { visitFCvtSF(std::as_const(I)); }
    virtual void visitFCvtUF(FCvtSF &I) { visitFCvtSF(std::as_const(I)); }
    virtual void visitFDiv(FDiv &I) { visitFDiv(std::as_const(I)); }
    virtual void visitFLoad(FLoad &I) { visitFLoad(std::as_const(I)); }
    virtual void visitFMov(FMov &I) { visitFMov(std::as_const(I)); }
    virtual void visitFMovFI(FMovFI &I) { visitFMovFI(std::as_const(I)); }
    virtual void visitFMovIF(FMovIF &I) { visitFMovIF(std::as_const(I)); }
    virtual void visitFMul(FMul &I) { visitFMul(std::as_const(I)); }
    virtual void visitFNeg(FNeg &I) { visitFNeg(std::as_const(I)); }
    virtual void visitFStore(FStore &I) { visitFStore(std::as_const(I)); }
    virtual void visitFSub(FSub &I) { visitFSub(std::as_const(I)); }
    virtual void visitLA(LA &I) { visitLA(std::as_const(I)); }
    virtual void visitLI(LI &I) { visitLI(std::as_const(I)); }
    virtual void visitLive(FakeUse &I) { visitLive(std::as_const(I)); }
    virtual void visitLoad(Load &I) { visitLoad(std::as_const(I)); }
    virtual void visitMarker(Marker &I) { visitMarker(std::as_const(I)); }
    virtual void visitMov(Mov &I) { visitMov(std::as_const(I)); }
    virtual void visitMul(Mul &I) { visitMul(std::as_const(I)); }
    virtual void visitNeg(Neg &I) { visitNeg(std::as_const(I)); }
    virtual void visitNot(Not &I) { visitNot(std::as_const(I)); }
    virtual void visitOr(Or &I) { visitOr(std::as_const(I)); }
    virtual void visitOrI(OrI &I) { visitOrI(std::as_const(I)); }
    virtual void visitSDiv(SDiv &I) { visitSDiv(std::as_const(I)); }
    virtual void visitSExt(SExt &I) { visitSExt(std::as_const(I)); }
    virtual void visitSHL(SHL &I) { visitSHL(std::as_const(I)); }
    virtual void visitSHLI(SHLI &I) { visitSHLI(std::as_const(I)); }
    virtual void visitSHRA(SHRA &I) { visitSHRA(std::as_const(I)); }
    virtual void visitSHRAI(SHRAI &I) { visitSHRAI(std::as_const(I)); }
    virtual void visitSHRL(SHRL &I) { visitSHRL(std::as_const(I)); }
    virtual void visitSHRLI(SHRLI &I) { visitSHRLI(std::as_const(I)); }
    virtual void visitSRem(SRem &I) { visitSRem(std::as_const(I)); }
    virtual void visitStore(Store &I) { visitStore(std::as_const(I)); }
    virtual void visitSub(Sub &I) { visitSub(std::as_const(I)); }
    virtual void visitSubI(SubI &I) { visitSubI(std::as_const(I)); }
    virtual void visitUDiv(UDiv &I) { visitUDiv(std::as_const(I)); }
    virtual void visitURem(URem &I) { visitURem(std::as_const(I)); }
    virtual void visitXor(Xor &I) { visitXor(std::as_const(I)); }
    virtual void visitXorI(XorI &I) { visitXorI(std::as_const(I)); }

    virtual void visitAdd(const Add &) { abort(); }
    virtual void visitAddI(const AddI &) { abort(); }
    virtual void visitAnd(const And &) { abort(); }
    virtual void visitAndI(const AndI &) { abort(); }
    virtual void visitBr(const Br &) { abort(); }
    virtual void visitCmpBr(const CmpBr &) { abort(); }
    virtual void visitCmp(const Cmp &) { abort(); }
    virtual void visitCmpSet(const CmpSet &) { abort(); }
    virtual void visitCmpZBr(const CmpZBr &) { abort(); }
    virtual void visitCmpZ(const CmpZ &) { abort(); }
    virtual void visitCmpZSet(const CmpZSet &) { abort(); }
    virtual void visitCondBr(const CondBr &) { abort(); }
    virtual void visitCondSet(const CondSet &) { abort(); }
    virtual void visitFAdd(const FAdd &) { abort(); }
    virtual void visitFCmp(const FCmp &) { abort(); }
    virtual void visitFCmpSet(const FCmpSet &) { abort(); }
    virtual void visitFCvt(const FCvt &) { abort(); }
    virtual void visitFCvtFS(const FCvtFS &) { abort(); }
    virtual void visitFCvtFU(const FCvtFU &) { abort(); }
    virtual void visitFCvtSF(const FCvtSF &) { abort(); }
    virtual void visitFCvtUF(const FCvtUF &) { abort(); }
    virtual void visitFDiv(const FDiv &) { abort(); }
    virtual void visitFLoad(const FLoad &) { abort(); }
    virtual void visitFMov(const FMov &) { abort(); }
    virtual void visitFMovFI(const FMovFI &) { abort(); }
    virtual void visitFMovIF(const FMovIF &) { abort(); }
    virtual void visitFMul(const FMul &) { abort(); }
    virtual void visitFNeg(const FNeg &) { abort(); }
    virtual void visitFStore(const FStore &) { abort(); }
    virtual void visitFSub(const FSub &) { abort(); }
    virtual void visitLA(const LA &) { abort(); }
    virtual void visitLI(const LI &) { abort(); }
    virtual void visitLive(const FakeUse &) { abort(); }
    virtual void visitLoad(const Load &) { abort(); }
    virtual void visitMarker(const Marker &) { abort(); }
    virtual void visitMov(const Mov &) { abort(); }
    virtual void visitMul(const Mul &) { abort(); }
    virtual void visitNeg(const Neg &) { abort(); }
    virtual void visitNot(const Not &) { abort(); }
    virtual void visitOr(const Or &) { abort(); }
    virtual void visitOrI(const OrI &) { abort(); }
    virtual void visitSDiv(const SDiv &) { abort(); }
    virtual void visitSExt(const SExt &) { abort(); }
    virtual void visitSHL(const SHL &) { abort(); }
    virtual void visitSHLI(const SHLI &) { abort(); }
    virtual void visitSHRA(const SHRA &) { abort(); }
    virtual void visitSHRAI(const SHRAI &) { abort(); }
    virtual void visitSHRL(const SHRL &) { abort(); }
    virtual void visitSHRLI(const SHRLI &) { abort(); }
    virtual void visitSRem(const SRem &) { abort(); }
    virtual void visitStore(const Store &) { abort(); }
    virtual void visitSub(const Sub &) { abort(); }
    virtual void visitSubI(const SubI &) { abort(); }
    virtual void visitUDiv(const UDiv &) { abort(); }
    virtual void visitURem(const URem &) { abort(); }
    virtual void visitXor(const Xor &) { abort(); }
    virtual void visitXorI(const XorI &) { abort(); }
};

} // namespace mini_llvm::mir
