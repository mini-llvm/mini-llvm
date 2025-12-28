// SPDX-License-Identifier: MIT

#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

namespace mini_llvm {

template <typename>
class FunctionRef;

template <typename R, typename... Args>
class FunctionRef<R (Args...)> {
public:
    template <typename F>
        requires std::is_function_v<F> &&
                 std::is_invocable_r_v<R, F &, Args...>
    FunctionRef(F *f) noexcept
        : bound_(const_cast<void *>(reinterpret_cast<const void *>(f))),
            thunk_([](void *bound, Args &&...args) {
                return std::invoke_r<R>(
                    reinterpret_cast<F *>(bound), std::forward<Args>(args)...); }) {}

    template <typename F>
        requires (!std::is_same_v<std::remove_cv_t<std::remove_reference_t<F>>, FunctionRef> &&
                  std::is_invocable_r_v<R, std::remove_reference_t<F> &, Args...>)
    FunctionRef(F &&f) noexcept
        : bound_(const_cast<void *>(reinterpret_cast<const void *>(std::addressof(f)))),
            thunk_([](void *bound, Args &&...args) {
                return std::invoke_r<R>(
                    *reinterpret_cast<std::remove_reference_t<F> *>(bound), std::forward<Args>(args)...); }) {}

    FunctionRef(const FunctionRef &) = default;

    constexpr FunctionRef &operator=(const FunctionRef &) noexcept = default;

    R operator()(Args... args) const {
        return thunk_(bound_, std::forward<Args>(args)...);
    }

private:
    void *bound_;
    R (*thunk_)(void *, Args &&...);
};

} // namespace mini_llvm
