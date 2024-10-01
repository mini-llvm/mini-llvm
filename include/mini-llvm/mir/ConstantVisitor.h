#pragma once

#include <utility>

namespace mini_llvm::mir {

class I16ArrayConstant;
class I16Constant;
class I32ArrayConstant;
class I32Constant;
class I64ArrayConstant;
class I64Constant;
class I8ArrayConstant;
class I8Constant;
class PtrConstant;
class ZeroConstant;

class ConstantVisitor {
public:
    virtual ~ConstantVisitor() = default;

    virtual void visitI16ArrayConstant(I16ArrayConstant &C) { visitI16ArrayConstant(std::as_const(C)); }
    virtual void visitI16Constant(I16Constant &C) { visitI16Constant(std::as_const(C)); }
    virtual void visitI32ArrayConstant(I32ArrayConstant &C) { visitI32ArrayConstant(std::as_const(C)); }
    virtual void visitI32Constant(I32Constant &C) { visitI32Constant(std::as_const(C)); }
    virtual void visitI64ArrayConstant(I64ArrayConstant &C) { visitI64ArrayConstant(std::as_const(C)); }
    virtual void visitI64Constant(I64Constant &C) { visitI64Constant(std::as_const(C)); }
    virtual void visitI8ArrayConstant(I8ArrayConstant &C) { visitI8ArrayConstant(std::as_const(C)); }
    virtual void visitI8Constant(I8Constant &C) { visitI8Constant(std::as_const(C)); }
    virtual void visitPtrConstant(PtrConstant &C) { visitPtrConstant(std::as_const(C)); }
    virtual void visitZeroConstant(ZeroConstant &C) { visitZeroConstant(std::as_const(C)); }

    virtual void visitI16ArrayConstant(const I16ArrayConstant &) { std::unreachable(); }
    virtual void visitI16Constant(const I16Constant &) { std::unreachable(); }
    virtual void visitI32ArrayConstant(const I32ArrayConstant &) { std::unreachable(); }
    virtual void visitI32Constant(const I32Constant &) { std::unreachable(); }
    virtual void visitI64ArrayConstant(const I64ArrayConstant &) { std::unreachable(); }
    virtual void visitI64Constant(const I64Constant &) { std::unreachable(); }
    virtual void visitI8ArrayConstant(const I8ArrayConstant &) { std::unreachable(); }
    virtual void visitI8Constant(const I8Constant &) { std::unreachable(); }
    virtual void visitPtrConstant(const PtrConstant &) { std::unreachable(); }
    virtual void visitZeroConstant(const ZeroConstant &) { std::unreachable(); }
};

} // namespace mini_llvm::mir
