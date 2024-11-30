#pragma once

#include <utility>

#include "mini-llvm/utils/Panic.h"

namespace mini_llvm::ir {

class ArrayType;
class BasicBlockType;
class Double;
class Float;
class FunctionType;
class I1;
class I16;
class I32;
class I64;
class I8;
class Ptr;
class Void;

class TypeVisitor {
public:
    virtual ~TypeVisitor() = default;

    virtual void visitArrayType(ArrayType &type) { visitArrayType(std::as_const(type)); }
    virtual void visitBasicBlockType(BasicBlockType &type) { visitBasicBlockType(std::as_const(type)); }
    virtual void visitDouble(Double &type) { visitDouble(std::as_const(type)); }
    virtual void visitFloat(Float &type) { visitFloat(std::as_const(type)); }
    virtual void visitFunctionType(FunctionType &type) { visitFunctionType(std::as_const(type)); }
    virtual void visitI1(I1 &type) { visitI1(std::as_const(type)); }
    virtual void visitI16(I16 &type) { visitI16(std::as_const(type)); }
    virtual void visitI32(I32 &type) { visitI32(std::as_const(type)); }
    virtual void visitI64(I64 &type) { visitI64(std::as_const(type)); }
    virtual void visitI8(I8 &type) { visitI8(std::as_const(type)); }
    virtual void visitPtr(Ptr &type) { visitPtr(std::as_const(type)); }
    virtual void visitVoid(Void &type) { visitVoid(std::as_const(type)); }

    virtual void visitArrayType(const ArrayType &) { panic(); }
    virtual void visitBasicBlockType(const BasicBlockType &) { panic(); }
    virtual void visitDouble(const Double &) { panic(); }
    virtual void visitFloat(const Float &) { panic(); }
    virtual void visitFunctionType(const FunctionType &) { panic(); }
    virtual void visitI1(const I1 &) { panic(); }
    virtual void visitI16(const I16 &) { panic(); }
    virtual void visitI32(const I32 &) { panic(); }
    virtual void visitI64(const I64 &) { panic(); }
    virtual void visitI8(const I8 &) { panic(); }
    virtual void visitPtr(const Ptr &) { panic(); }
    virtual void visitVoid(const Void &) { panic(); }
};

} // namespace mini_llvm::ir
