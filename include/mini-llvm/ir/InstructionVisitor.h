// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib>
#include <utility>

#include "mini-llvm/utils/Compiler.h"

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

class MINI_LLVM_EXPORT InstructionVisitor {
public:
    virtual ~InstructionVisitor() = default;

    InstructionVisitor() = default;

    InstructionVisitor(const InstructionVisitor &) = delete;
    InstructionVisitor &operator=(const InstructionVisitor &) = delete;

    InstructionVisitor(InstructionVisitor &&) = delete;
    InstructionVisitor &operator=(InstructionVisitor &&) = delete;

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

    virtual void visitAdd(const Add &) { abort(); }
    virtual void visitAlloca(const Alloca &) { abort(); }
    virtual void visitAnd(const And &) { abort(); }
    virtual void visitASHR(const ASHR &) { abort(); }
    virtual void visitBitCast(const BitCast &) { abort(); }
    virtual void visitBr(const Br &) { abort(); }
    virtual void visitCall(const Call &) { abort(); }
    virtual void visitCondBr(const CondBr &) { abort(); }
    virtual void visitFAdd(const FAdd &) { abort(); }
    virtual void visitFCmp(const FCmp &) { abort(); }
    virtual void visitFDiv(const FDiv &) { abort(); }
    virtual void visitFMul(const FMul &) { abort(); }
    virtual void visitFNeg(const FNeg &) { abort(); }
    virtual void visitFPExt(const FPExt &) { abort(); }
    virtual void visitFPToSI(const FPToSI &) { abort(); }
    virtual void visitFPToUI(const FPToUI &) { abort(); }
    virtual void visitFPTrunc(const FPTrunc &) { abort(); }
    virtual void visitFRem(const FRem &) { abort(); }
    virtual void visitFSub(const FSub &) { abort(); }
    virtual void visitGetElementPtr(const GetElementPtr &) { abort(); }
    virtual void visitICmp(const ICmp &) { abort(); }
    virtual void visitIndirectCall(const IndirectCall &) { abort(); }
    virtual void visitIntToPtr(const IntToPtr &) { abort(); }
    virtual void visitLoad(const Load &) { abort(); }
    virtual void visitLSHR(const LSHR &) { abort(); }
    virtual void visitMul(const Mul &) { abort(); }
    virtual void visitOr(const Or &) { abort(); }
    virtual void visitPhi(const Phi &) { abort(); }
    virtual void visitPtrToInt(const PtrToInt &) { abort(); }
    virtual void visitRet(const Ret &) { abort(); }
    virtual void visitSDiv(const SDiv &) { abort(); }
    virtual void visitSelect(const Select &) { abort(); }
    virtual void visitSExt(const SExt &) { abort(); }
    virtual void visitSHL(const SHL &) { abort(); }
    virtual void visitSIToFP(const SIToFP &) { abort(); }
    virtual void visitSRem(const SRem &) { abort(); }
    virtual void visitStore(const Store &) { abort(); }
    virtual void visitSub(const Sub &) { abort(); }
    virtual void visitTerminator(const Terminator &) { abort(); }
    virtual void visitTrunc(const Trunc &) { abort(); }
    virtual void visitUDiv(const UDiv &) { abort(); }
    virtual void visitUIToFP(const UIToFP &) { abort(); }
    virtual void visitURem(const URem &) { abort(); }
    virtual void visitXor(const Xor &) { abort(); }
    virtual void visitZExt(const ZExt &) { abort(); }
};

} // namespace mini_llvm::ir
