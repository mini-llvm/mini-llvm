// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib>
#include <utility>

#include "mini-llvm/utils/Compiler.h"

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

class MINI_LLVM_EXPORT ConstantVisitor {
public:
    virtual ~ConstantVisitor() = default;

    ConstantVisitor() = default;

    ConstantVisitor(const ConstantVisitor &) = delete;
    ConstantVisitor &operator=(const ConstantVisitor &) = delete;

    ConstantVisitor(ConstantVisitor &&) = delete;
    ConstantVisitor &operator=(ConstantVisitor &&) = delete;

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

    virtual void visitArrayConstant(const ArrayConstant &) { abort(); }
    virtual void visitDoubleConstant(const DoubleConstant &) { abort(); }
    virtual void visitFloatConstant(const FloatConstant &) { abort(); }
    virtual void visitFunction(const Function &) { abort(); }
    virtual void visitGlobalVar(const GlobalVar &) { abort(); }
    virtual void visitI16Constant(const I16Constant &) { abort(); }
    virtual void visitI1Constant(const I1Constant &) { abort(); }
    virtual void visitI32Constant(const I32Constant &) { abort(); }
    virtual void visitI64Constant(const I64Constant &) { abort(); }
    virtual void visitI8Constant(const I8Constant &) { abort(); }
    virtual void visitNullPtrConstant(const NullPtrConstant &) { abort(); }
    virtual void visitPoisonValue(const PoisonValue &) { abort(); }
    virtual void visitVoidValue(const VoidValue &) { abort(); }
};

} // namespace mini_llvm::ir
