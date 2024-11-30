#pragma once

#include <utility>

#include "mini-llvm/utils/Panic.h"

namespace mini_llvm::ir {

class ArrayConstant;
class DoubleConstant;
class FloatConstant;
class Function;
class GlobalVar;
class I16Constant;
class I1Constant;
class I32Constant;
class I64Constant;
class I8Constant;
class NullPtrConstant;
class PoisonValue;
class VoidValue;

class ConstantVisitor {
public:
    virtual ~ConstantVisitor() = default;

    virtual void visitArrayConstant(ArrayConstant &C) { visitArrayConstant(std::as_const(C)); }
    virtual void visitDoubleConstant(DoubleConstant &C) { visitDoubleConstant(std::as_const(C)); }
    virtual void visitFloatConstant(FloatConstant &C) { visitFloatConstant(std::as_const(C)); }
    virtual void visitFunction(Function &C) { visitFunction(std::as_const(C)); }
    virtual void visitGlobalVar(GlobalVar &C) { visitGlobalVar(std::as_const(C)); }
    virtual void visitI16Constant(I16Constant &C) { visitI16Constant(std::as_const(C)); }
    virtual void visitI1Constant(I1Constant &C) { visitI1Constant(std::as_const(C)); }
    virtual void visitI32Constant(I32Constant &C) { visitI32Constant(std::as_const(C)); }
    virtual void visitI64Constant(I64Constant &C) { visitI64Constant(std::as_const(C)); }
    virtual void visitI8Constant(I8Constant &C) { visitI8Constant(std::as_const(C)); }
    virtual void visitNullPtrConstant(NullPtrConstant &C) { visitNullPtrConstant(std::as_const(C)); }
    virtual void visitPoisonValue(PoisonValue &C) { visitPoisonValue(std::as_const(C)); }
    virtual void visitVoidValue(VoidValue &C) { visitVoidValue(std::as_const(C)); }

    virtual void visitArrayConstant(const ArrayConstant &) { panic(); }
    virtual void visitDoubleConstant(const DoubleConstant &) { panic(); }
    virtual void visitFloatConstant(const FloatConstant &) { panic(); }
    virtual void visitFunction(const Function &) { panic(); }
    virtual void visitGlobalVar(const GlobalVar &) { panic(); }
    virtual void visitI16Constant(const I16Constant &) { panic(); }
    virtual void visitI1Constant(const I1Constant &) { panic(); }
    virtual void visitI32Constant(const I32Constant &) { panic(); }
    virtual void visitI64Constant(const I64Constant &) { panic(); }
    virtual void visitI8Constant(const I8Constant &) { panic(); }
    virtual void visitNullPtrConstant(const NullPtrConstant &) { panic(); }
    virtual void visitPoisonValue(const PoisonValue &) { panic(); }
    virtual void visitVoidValue(const VoidValue &) { panic(); }
};

} // namespace mini_llvm::ir
