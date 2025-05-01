#pragma once

namespace mini_llvm::ir {

class Pass {
public:
    virtual ~Pass() = default;
    virtual const char *name() const = 0;
};

} // namespace mini_llvm::ir
