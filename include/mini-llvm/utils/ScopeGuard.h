#pragma once

#include <functional>
#include <tuple>
#include <utility>

namespace mini_llvm {

class ScopeGuard {
public:
    template <typename F, typename... Args>
    explicit ScopeGuard(F &&f, Args &&...args)
        : f_([f = std::forward<F>(f), args = std::tuple(std::forward<Args>(args)...)] { std::apply(f, args); }) {}

    ~ScopeGuard() {
        f_();
    }

    ScopeGuard(const ScopeGuard &) = delete;
    ScopeGuard &operator=(const ScopeGuard &) = delete;
    ScopeGuard(ScopeGuard &&) = default;
    ScopeGuard &operator=(ScopeGuard &&) = default;

private:
    std::function<void ()> f_;
};

} // namespace mini_llvm
