#pragma once

#include <utility>

namespace mini_llvm {

template <typename F>
class ScopeGuard {
public:
    explicit ScopeGuard(F f) : f_(std::move(f)) {}

    ~ScopeGuard() {
        f_();
    }

    ScopeGuard(const ScopeGuard &) = delete;
    ScopeGuard &operator=(const ScopeGuard &) = delete;
    ScopeGuard(ScopeGuard &&) = default;
    ScopeGuard &operator=(ScopeGuard &&) = default;

private:
    F f_;
};

} // namespace mini_llvm
