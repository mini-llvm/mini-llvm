#pragma once

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace mini_llvm {

class ScopeGuard {
public:
    template <typename F, typename T>
    class Wrapper {
    public:
        Wrapper(F &&f, T &&t)
            : f_(std::forward<F>(f)), t_(std::forward<T>(t)) {}

        void operator()() {
            std::apply(std::forward<F>(f_), std::forward<T>(t_));
        }

    private:
        F f_;
        T t_;
    };

public:
    template <typename F, typename... Args>
    explicit ScopeGuard(F &&f, Args &&...args)
        : f_(Wrapper<std::decay_t<F>, std::tuple<std::decay_t<Args>...>>(
            std::forward<std::decay_t<F>>(f),
            std::tuple<std::decay_t<Args>...>(std::forward<std::decay_t<Args>...>(args)...))), dismissed_(false) {}

    ~ScopeGuard() {
        if (!dismissed_) {
            f_();
        }
    }

    ScopeGuard(const ScopeGuard &) = delete;
    ScopeGuard(ScopeGuard &&) = delete;

    ScopeGuard &operator=(const ScopeGuard &) = delete;
    ScopeGuard &operator=(ScopeGuard &&) = delete;

    void dismiss() {
        dismissed_ = true;
    }

private:
    std::function<void ()> f_;
    bool dismissed_;
};

} // namespace mini_llvm
