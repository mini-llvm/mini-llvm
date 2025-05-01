#pragma once

namespace mini_llvm::mir {

class Pass {
public:
    virtual ~Pass() = default;
    virtual const char *name() const = 0;
};

} // namespace mini_llvm::mir
