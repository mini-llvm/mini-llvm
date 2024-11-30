#pragma once

#include <utility>

#include "mini-llvm/utils/Panic.h"

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
class Load;
class Mov;
class Mul;
class Not;
class Or;
class OrI;
class Placeholder;
class SDiv;
class CondSet;
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

class InstructionVisitor {
public:
    virtual ~InstructionVisitor() = default;

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
    virtual void visitLoad(Load &I) { visitLoad(std::as_const(I)); }
    virtual void visitMov(Mov &I) { visitMov(std::as_const(I)); }
    virtual void visitMul(Mul &I) { visitMul(std::as_const(I)); }
    virtual void visitNot(Not &I) { visitNot(std::as_const(I)); }
    virtual void visitOr(Or &I) { visitOr(std::as_const(I)); }
    virtual void visitOrI(OrI &I) { visitOrI(std::as_const(I)); }
    virtual void visitPlaceholder(Placeholder &I) { visitPlaceholder(std::as_const(I)); }
    virtual void visitSDiv(SDiv &I) { visitSDiv(std::as_const(I)); }
    virtual void visitSet(CondSet &I) { visitSet(std::as_const(I)); }
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

    virtual void visitAdd(const Add &) { panic(); }
    virtual void visitAddI(const AddI &) { panic(); }
    virtual void visitAnd(const And &) { panic(); }
    virtual void visitAndI(const AndI &) { panic(); }
    virtual void visitBr(const Br &) { panic(); }
    virtual void visitCmpBr(const CmpBr &) { panic(); }
    virtual void visitCmp(const Cmp &) { panic(); }
    virtual void visitCmpSet(const CmpSet &) { panic(); }
    virtual void visitCmpZBr(const CmpZBr &) { panic(); }
    virtual void visitCmpZ(const CmpZ &) { panic(); }
    virtual void visitCmpZSet(const CmpZSet &) { panic(); }
    virtual void visitCondBr(const CondBr &) { panic(); }
    virtual void visitFAdd(const FAdd &) { panic(); }
    virtual void visitFCmp(const FCmp &) { panic(); }
    virtual void visitFCmpSet(const FCmpSet &) { panic(); }
    virtual void visitFCvt(const FCvt &) { panic(); }
    virtual void visitFCvtFS(const FCvtFS &) { panic(); }
    virtual void visitFCvtFU(const FCvtFU &) { panic(); }
    virtual void visitFCvtSF(const FCvtSF &) { panic(); }
    virtual void visitFCvtUF(const FCvtUF &) { panic(); }
    virtual void visitFDiv(const FDiv &) { panic(); }
    virtual void visitFLoad(const FLoad &) { panic(); }
    virtual void visitFMov(const FMov &) { panic(); }
    virtual void visitFMovFI(const FMovFI &) { panic(); }
    virtual void visitFMovIF(const FMovIF &) { panic(); }
    virtual void visitFMul(const FMul &) { panic(); }
    virtual void visitFNeg(const FNeg &) { panic(); }
    virtual void visitFStore(const FStore &) { panic(); }
    virtual void visitFSub(const FSub &) { panic(); }
    virtual void visitLA(const LA &) { panic(); }
    virtual void visitLI(const LI &) { panic(); }
    virtual void visitLoad(const Load &) { panic(); }
    virtual void visitMov(const Mov &) { panic(); }
    virtual void visitMul(const Mul &) { panic(); }
    virtual void visitNot(const Not &) { panic(); }
    virtual void visitOr(const Or &) { panic(); }
    virtual void visitOrI(const OrI &) { panic(); }
    virtual void visitPlaceholder(const Placeholder &) { panic(); }
    virtual void visitSDiv(const SDiv &) { panic(); }
    virtual void visitSet(const CondSet &) { panic(); }
    virtual void visitSExt(const SExt &) { panic(); }
    virtual void visitSHL(const SHL &) { panic(); }
    virtual void visitSHLI(const SHLI &) { panic(); }
    virtual void visitSHRA(const SHRA &) { panic(); }
    virtual void visitSHRAI(const SHRAI &) { panic(); }
    virtual void visitSHRL(const SHRL &) { panic(); }
    virtual void visitSHRLI(const SHRLI &) { panic(); }
    virtual void visitSRem(const SRem &) { panic(); }
    virtual void visitStore(const Store &) { panic(); }
    virtual void visitSub(const Sub &) { panic(); }
    virtual void visitSubI(const SubI &) { panic(); }
    virtual void visitUDiv(const UDiv &) { panic(); }
    virtual void visitURem(const URem &) { panic(); }
    virtual void visitXor(const Xor &) { panic(); }
    virtual void visitXorI(const XorI &) { panic(); }
};

} // namespace mini_llvm::mir
