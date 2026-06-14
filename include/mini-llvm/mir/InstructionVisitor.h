// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib> // IWYU pragma: keep
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

    virtual void visitAdd(const Add &/*I*/) { abort(); }
    virtual void visitAddI(const AddI &/*I*/) { abort(); }
    virtual void visitAnd(const And &/*I*/) { abort(); }
    virtual void visitAndI(const AndI &/*I*/) { abort(); }
    virtual void visitBr(const Br &/*I*/) { abort(); }
    virtual void visitCmpBr(const CmpBr &/*I*/) { abort(); }
    virtual void visitCmp(const Cmp &/*I*/) { abort(); }
    virtual void visitCmpSet(const CmpSet &/*I*/) { abort(); }
    virtual void visitCmpZBr(const CmpZBr &/*I*/) { abort(); }
    virtual void visitCmpZ(const CmpZ &/*I*/) { abort(); }
    virtual void visitCmpZSet(const CmpZSet &/*I*/) { abort(); }
    virtual void visitCondBr(const CondBr &/*I*/) { abort(); }
    virtual void visitCondSet(const CondSet &/*I*/) { abort(); }
    virtual void visitFAdd(const FAdd &/*I*/) { abort(); }
    virtual void visitFCmp(const FCmp &/*I*/) { abort(); }
    virtual void visitFCmpSet(const FCmpSet &/*I*/) { abort(); }
    virtual void visitFCvt(const FCvt &/*I*/) { abort(); }
    virtual void visitFCvtFS(const FCvtFS &/*I*/) { abort(); }
    virtual void visitFCvtFU(const FCvtFU &/*I*/) { abort(); }
    virtual void visitFCvtSF(const FCvtSF &/*I*/) { abort(); }
    virtual void visitFCvtUF(const FCvtUF &/*I*/) { abort(); }
    virtual void visitFDiv(const FDiv &/*I*/) { abort(); }
    virtual void visitFLoad(const FLoad &/*I*/) { abort(); }
    virtual void visitFMov(const FMov &/*I*/) { abort(); }
    virtual void visitFMovFI(const FMovFI &/*I*/) { abort(); }
    virtual void visitFMovIF(const FMovIF &/*I*/) { abort(); }
    virtual void visitFMul(const FMul &/*I*/) { abort(); }
    virtual void visitFNeg(const FNeg &/*I*/) { abort(); }
    virtual void visitFStore(const FStore &/*I*/) { abort(); }
    virtual void visitFSub(const FSub &/*I*/) { abort(); }
    virtual void visitLA(const LA &/*I*/) { abort(); }
    virtual void visitLI(const LI &/*I*/) { abort(); }
    virtual void visitLive(const FakeUse &/*I*/) { abort(); }
    virtual void visitLoad(const Load &/*I*/) { abort(); }
    virtual void visitMarker(const Marker &/*I*/) { abort(); }
    virtual void visitMov(const Mov &/*I*/) { abort(); }
    virtual void visitMul(const Mul &/*I*/) { abort(); }
    virtual void visitNeg(const Neg &/*I*/) { abort(); }
    virtual void visitNot(const Not &/*I*/) { abort(); }
    virtual void visitOr(const Or &/*I*/) { abort(); }
    virtual void visitOrI(const OrI &/*I*/) { abort(); }
    virtual void visitSDiv(const SDiv &/*I*/) { abort(); }
    virtual void visitSExt(const SExt &/*I*/) { abort(); }
    virtual void visitSHL(const SHL &/*I*/) { abort(); }
    virtual void visitSHLI(const SHLI &/*I*/) { abort(); }
    virtual void visitSHRA(const SHRA &/*I*/) { abort(); }
    virtual void visitSHRAI(const SHRAI &/*I*/) { abort(); }
    virtual void visitSHRL(const SHRL &/*I*/) { abort(); }
    virtual void visitSHRLI(const SHRLI &/*I*/) { abort(); }
    virtual void visitSRem(const SRem &/*I*/) { abort(); }
    virtual void visitStore(const Store &/*I*/) { abort(); }
    virtual void visitSub(const Sub &/*I*/) { abort(); }
    virtual void visitSubI(const SubI &/*I*/) { abort(); }
    virtual void visitUDiv(const UDiv &/*I*/) { abort(); }
    virtual void visitURem(const URem &/*I*/) { abort(); }
    virtual void visitXor(const Xor &/*I*/) { abort(); }
    virtual void visitXorI(const XorI &/*I*/) { abort(); }
};

} // namespace mini_llvm::mir
