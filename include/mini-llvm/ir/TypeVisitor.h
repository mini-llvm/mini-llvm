#pragma once

#include <utility>

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

    virtual void visitArrayType(const ArrayType &) { std::unreachable(); }
    virtual void visitBasicBlockType(const BasicBlockType &) { std::unreachable(); }
    virtual void visitDouble(const Double &) { std::unreachable(); }
    virtual void visitFloat(const Float &) { std::unreachable(); }
    virtual void visitFunctionType(const FunctionType &) { std::unreachable(); }
    virtual void visitI1(const I1 &) { std::unreachable(); }
    virtual void visitI16(const I16 &) { std::unreachable(); }
    virtual void visitI32(const I32 &) { std::unreachable(); }
    virtual void visitI64(const I64 &) { std::unreachable(); }
    virtual void visitI8(const I8 &) { std::unreachable(); }
    virtual void visitPtr(const Ptr &) { std::unreachable(); }
    virtual void visitVoid(const Void &) { std::unreachable(); }
};

} // namespace mini_llvm::ir
