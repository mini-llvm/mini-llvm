#pragma once

#include <utility>

#include "mini-llvm/utils/Panic.h"

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
class IndirectCall;
class IntToPtr;
class Load;
class LSHR;
class Mul;
class Or;
class Phi;
class PtrToInt;
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
    virtual void visitIndirectCall(IndirectCall &I) { visitIndirectCall(std::as_const(I)); }
    virtual void visitIntToPtr(IntToPtr &I) {visitIntToPtr(std::as_const(I)); }
    virtual void visitLoad(Load &I) { visitLoad(std::as_const(I)); }
    virtual void visitLSHR(LSHR &I) { visitLSHR(std::as_const(I)); }
    virtual void visitMul(Mul &I) { visitMul(std::as_const(I)); }
    virtual void visitOr(Or &I) { visitOr(std::as_const(I)); }
    virtual void visitPhi(Phi &I) { visitPhi(std::as_const(I)); }
    virtual void visitPtrToInt(PtrToInt &I) { visitPtrToInt(std::as_const(I)); }
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

    virtual void visitAdd(const Add &) { panic(); }
    virtual void visitAlloca(const Alloca &) { panic(); }
    virtual void visitAnd(const And &) { panic(); }
    virtual void visitASHR(const ASHR &) { panic(); }
    virtual void visitBitCast(const BitCast &) { panic(); }
    virtual void visitBr(const Br &) { panic(); }
    virtual void visitCall(const Call &) { panic(); }
    virtual void visitCondBr(const CondBr &) { panic(); }
    virtual void visitFAdd(const FAdd &) { panic(); }
    virtual void visitFCmp(const FCmp &) { panic(); }
    virtual void visitFDiv(const FDiv &) { panic(); }
    virtual void visitFMul(const FMul &) { panic(); }
    virtual void visitFNeg(const FNeg &) { panic(); }
    virtual void visitFPExt(const FPExt &) { panic(); }
    virtual void visitFPToSI(const FPToSI &) { panic(); }
    virtual void visitFPToUI(const FPToUI &) { panic(); }
    virtual void visitFPTrunc(const FPTrunc &) { panic(); }
    virtual void visitFRem(const FRem &) { panic(); }
    virtual void visitFSub(const FSub &) { panic(); }
    virtual void visitGetElementPtr(const GetElementPtr &) { panic(); }
    virtual void visitICmp(const ICmp &) { panic(); }
    virtual void visitIndirectCall(const IndirectCall &) { panic(); }
    virtual void visitIntToPtr(const IntToPtr &) { panic(); }
    virtual void visitLoad(const Load &) { panic(); }
    virtual void visitLSHR(const LSHR &) { panic(); }
    virtual void visitMul(const Mul &) { panic(); }
    virtual void visitOr(const Or &) { panic(); }
    virtual void visitPhi(const Phi &) { panic(); }
    virtual void visitPtrToInt(const PtrToInt &) { panic(); }
    virtual void visitRet(const Ret &) { panic(); }
    virtual void visitSDiv(const SDiv &) { panic(); }
    virtual void visitSelect(const Select &) { panic(); }
    virtual void visitSExt(const SExt &) { panic(); }
    virtual void visitSHL(const SHL &) { panic(); }
    virtual void visitSIToFP(const SIToFP &) { panic(); }
    virtual void visitSRem(const SRem &) { panic(); }
    virtual void visitStore(const Store &) { panic(); }
    virtual void visitSub(const Sub &) { panic(); }
    virtual void visitTerminator(const Terminator &) { panic(); }
    virtual void visitTrunc(const Trunc &) { panic(); }
    virtual void visitUDiv(const UDiv &) { panic(); }
    virtual void visitUIToFP(const UIToFP &) { panic(); }
    virtual void visitURem(const URem &) { panic(); }
    virtual void visitXor(const Xor &) { panic(); }
    virtual void visitZExt(const ZExt &) { panic(); }
};

} // namespace mini_llvm::ir
