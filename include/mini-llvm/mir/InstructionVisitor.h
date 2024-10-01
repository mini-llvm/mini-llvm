#pragma once

#include <utility>

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

    virtual void visitAdd(const Add &) { std::unreachable(); }
    virtual void visitAddI(const AddI &) { std::unreachable(); }
    virtual void visitAnd(const And &) { std::unreachable(); }
    virtual void visitAndI(const AndI &) { std::unreachable(); }
    virtual void visitBr(const Br &) { std::unreachable(); }
    virtual void visitCmpBr(const CmpBr &) { std::unreachable(); }
    virtual void visitCmp(const Cmp &) { std::unreachable(); }
    virtual void visitCmpSet(const CmpSet &) { std::unreachable(); }
    virtual void visitCmpZBr(const CmpZBr &) { std::unreachable(); }
    virtual void visitCmpZ(const CmpZ &) { std::unreachable(); }
    virtual void visitCmpZSet(const CmpZSet &) { std::unreachable(); }
    virtual void visitCondBr(const CondBr &) { std::unreachable(); }
    virtual void visitFAdd(const FAdd &) { std::unreachable(); }
    virtual void visitFCmp(const FCmp &) { std::unreachable(); }
    virtual void visitFCmpSet(const FCmpSet &) { std::unreachable(); }
    virtual void visitFCvt(const FCvt &) { std::unreachable(); }
    virtual void visitFCvtFS(const FCvtFS &) { std::unreachable(); }
    virtual void visitFCvtFU(const FCvtFU &) { std::unreachable(); }
    virtual void visitFCvtSF(const FCvtSF &) { std::unreachable(); }
    virtual void visitFCvtUF(const FCvtUF &) { std::unreachable(); }
    virtual void visitFDiv(const FDiv &) { std::unreachable(); }
    virtual void visitFLoad(const FLoad &) { std::unreachable(); }
    virtual void visitFMov(const FMov &) { std::unreachable(); }
    virtual void visitFMovFI(const FMovFI &) { std::unreachable(); }
    virtual void visitFMovIF(const FMovIF &) { std::unreachable(); }
    virtual void visitFMul(const FMul &) { std::unreachable(); }
    virtual void visitFNeg(const FNeg &) { std::unreachable(); }
    virtual void visitFStore(const FStore &) { std::unreachable(); }
    virtual void visitFSub(const FSub &) { std::unreachable(); }
    virtual void visitLA(const LA &) { std::unreachable(); }
    virtual void visitLI(const LI &) { std::unreachable(); }
    virtual void visitLoad(const Load &) { std::unreachable(); }
    virtual void visitMov(const Mov &) { std::unreachable(); }
    virtual void visitMul(const Mul &) { std::unreachable(); }
    virtual void visitNot(const Not &) { std::unreachable(); }
    virtual void visitOr(const Or &) { std::unreachable(); }
    virtual void visitOrI(const OrI &) { std::unreachable(); }
    virtual void visitSDiv(const SDiv &) { std::unreachable(); }
    virtual void visitSet(const CondSet &) { std::unreachable(); }
    virtual void visitSExt(const SExt &) { std::unreachable(); }
    virtual void visitSHL(const SHL &) { std::unreachable(); }
    virtual void visitSHLI(const SHLI &) { std::unreachable(); }
    virtual void visitSHRA(const SHRA &) { std::unreachable(); }
    virtual void visitSHRAI(const SHRAI &) { std::unreachable(); }
    virtual void visitSHRL(const SHRL &) { std::unreachable(); }
    virtual void visitSHRLI(const SHRLI &) { std::unreachable(); }
    virtual void visitSRem(const SRem &) { std::unreachable(); }
    virtual void visitStore(const Store &) { std::unreachable(); }
    virtual void visitSub(const Sub &) { std::unreachable(); }
    virtual void visitSubI(const SubI &) { std::unreachable(); }
    virtual void visitUDiv(const UDiv &) { std::unreachable(); }
    virtual void visitURem(const URem &) { std::unreachable(); }
    virtual void visitXor(const Xor &) { std::unreachable(); }
    virtual void visitXorI(const XorI &) { std::unreachable(); }
};

} // namespace mini_llvm::mir
