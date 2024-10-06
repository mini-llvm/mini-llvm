#pragma once

#include <utility>

namespace mini_llvm::ir {

class Add;
class Alloca;
class And;
class ASHR;
class BitCast;
class Br;
class Call;
class CondBr;
class FAdd;
class FCmp;
class FDiv;
class FMul;
class FNeg;
class FPExt;
class FPToSI;
class FPToUI;
class FPTrunc;
class FRem;
class FSub;
class GetElementPtr;
class ICmp;
class Load;
class LSHR;
class Mul;
class Or;
class Phi;
class Ret;
class SDiv;
class Select;
class SExt;
class SHL;
class SIToFP;
class SRem;
class Store;
class Sub;
class Terminator;
class Trunc;
class UDiv;
class UIToFP;
class URem;
class Xor;
class ZExt;

class InstructionVisitor {
public:
    virtual ~InstructionVisitor() = default;

    virtual void visitAdd(Add &I) { visitAdd(std::as_const(I)); }
    virtual void visitAlloca(Alloca &I) { visitAlloca(std::as_const(I)); }
    virtual void visitAnd(And &I) { visitAnd(std::as_const(I)); }
    virtual void visitASHR(ASHR &I) { visitASHR(std::as_const(I)); }
    virtual void visitBitCast(BitCast &I) { visitBitCast(std::as_const(I)); }
    virtual void visitBr(Br &I) { visitBr(std::as_const(I)); }
    virtual void visitCall(Call &I) { visitCall(std::as_const(I)); }
    virtual void visitCondBr(CondBr &I) { visitCondBr(std::as_const(I)); }
    virtual void visitFAdd(FAdd &I) { visitFAdd(std::as_const(I)); }
    virtual void visitFCmp(FCmp &I) { visitFCmp(std::as_const(I)); }
    virtual void visitFDiv(FDiv &I) { visitFDiv(std::as_const(I)); }
    virtual void visitFMul(FMul &I) { visitFMul(std::as_const(I)); }
    virtual void visitFNeg(FNeg &I) { visitFNeg(std::as_const(I)); }
    virtual void visitFPExt(FPExt &I) { visitFPExt(std::as_const(I)); }
    virtual void visitFPToSI(FPToSI &I) { visitFPToSI(std::as_const(I)); }
    virtual void visitFPToUI(FPToUI &I) { visitFPToUI(std::as_const(I)); }
    virtual void visitFPTrunc(FPTrunc &I) { visitFPTrunc(std::as_const(I)); }
    virtual void visitFRem(FRem &I) { visitFRem(std::as_const(I)); }
    virtual void visitFSub(FSub &I) { visitFSub(std::as_const(I)); }
    virtual void visitGetElementPtr(GetElementPtr &I) { visitGetElementPtr(std::as_const(I)); }
    virtual void visitICmp(ICmp &I) { visitICmp(std::as_const(I)); }
    virtual void visitLoad(Load &I) { visitLoad(std::as_const(I)); }
    virtual void visitLSHR(LSHR &I) { visitLSHR(std::as_const(I)); }
    virtual void visitMul(Mul &I) { visitMul(std::as_const(I)); }
    virtual void visitOr(Or &I) { visitOr(std::as_const(I)); }
    virtual void visitPhi(Phi &I) { visitPhi(std::as_const(I)); }
    virtual void visitRet(Ret &I) { visitRet(std::as_const(I)); }
    virtual void visitSDiv(SDiv &I) { visitSDiv(std::as_const(I)); }
    virtual void visitSelect(Select &I) { visitSelect(std::as_const(I)); }
    virtual void visitSExt(SExt &I) { visitSExt(std::as_const(I)); }
    virtual void visitSHL(SHL &I) { visitSHL(std::as_const(I)); }
    virtual void visitSIToFP(SIToFP &I) { visitSIToFP(std::as_const(I)); }
    virtual void visitSRem(SRem &I) { visitSRem(std::as_const(I)); }
    virtual void visitStore(Store &I) { visitStore(std::as_const(I)); }
    virtual void visitSub(Sub &I) { visitSub(std::as_const(I)); }
    virtual void visitTerminator(Terminator &I) { visitTerminator(std::as_const(I)); }
    virtual void visitTrunc(Trunc &I) { visitTrunc(std::as_const(I)); }
    virtual void visitUDiv(UDiv &I) { visitUDiv(std::as_const(I)); }
    virtual void visitURem(URem &I) { visitURem(std::as_const(I)); }
    virtual void visitUIToFP(UIToFP &I) { visitUIToFP(std::as_const(I)); }
    virtual void visitXor(Xor &I) { visitXor(std::as_const(I)); }
    virtual void visitZExt(ZExt &I) { visitZExt(std::as_const(I)); }

    virtual void visitAdd(const Add &) { std::unreachable(); }
    virtual void visitAlloca(const Alloca &) { std::unreachable(); }
    virtual void visitAnd(const And &) { std::unreachable(); }
    virtual void visitASHR(const ASHR &) { std::unreachable(); }
    virtual void visitBitCast(const BitCast &) { std::unreachable(); }
    virtual void visitBr(const Br &) { std::unreachable(); }
    virtual void visitCall(const Call &) { std::unreachable(); }
    virtual void visitCondBr(const CondBr &) { std::unreachable(); }
    virtual void visitFAdd(const FAdd &) { std::unreachable(); }
    virtual void visitFCmp(const FCmp &) { std::unreachable(); }
    virtual void visitFDiv(const FDiv &) { std::unreachable(); }
    virtual void visitFMul(const FMul &) { std::unreachable(); }
    virtual void visitFNeg(const FNeg &) { std::unreachable(); }
    virtual void visitFPExt(const FPExt &) { std::unreachable(); }
    virtual void visitFPToSI(const FPToSI &) { std::unreachable(); }
    virtual void visitFPToUI(const FPToUI &) { std::unreachable(); }
    virtual void visitFPTrunc(const FPTrunc &) { std::unreachable(); }
    virtual void visitFRem(const FRem &) { std::unreachable(); }
    virtual void visitFSub(const FSub &) { std::unreachable(); }
    virtual void visitGetElementPtr(const GetElementPtr &) { std::unreachable(); }
    virtual void visitICmp(const ICmp &) { std::unreachable(); }
    virtual void visitLoad(const Load &) { std::unreachable(); }
    virtual void visitLSHR(const LSHR &) { std::unreachable(); }
    virtual void visitMul(const Mul &) { std::unreachable(); }
    virtual void visitOr(const Or &) { std::unreachable(); }
    virtual void visitPhi(const Phi &) { std::unreachable(); }
    virtual void visitRet(const Ret &) { std::unreachable(); }
    virtual void visitSDiv(const SDiv &) { std::unreachable(); }
    virtual void visitSelect(const Select &) { std::unreachable(); }
    virtual void visitSExt(const SExt &) { std::unreachable(); }
    virtual void visitSHL(const SHL &) { std::unreachable(); }
    virtual void visitSIToFP(const SIToFP &) { std::unreachable(); }
    virtual void visitSRem(const SRem &) { std::unreachable(); }
    virtual void visitStore(const Store &) { std::unreachable(); }
    virtual void visitSub(const Sub &) { std::unreachable(); }
    virtual void visitTerminator(const Terminator &) { std::unreachable(); }
    virtual void visitTrunc(const Trunc &) { std::unreachable(); }
    virtual void visitUDiv(const UDiv &) { std::unreachable(); }
    virtual void visitUIToFP(const UIToFP &) { std::unreachable(); }
    virtual void visitURem(const URem &) { std::unreachable(); }
    virtual void visitXor(const Xor &) { std::unreachable(); }
    virtual void visitZExt(const ZExt &) { std::unreachable(); }
};

} // namespace mini_llvm::ir
