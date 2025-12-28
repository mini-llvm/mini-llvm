// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>

namespace mini_llvm {

template <typename T>
class Matrix {
public:
    constexpr Matrix()
        : rows_(0), cols_(0), data_() {}

    constexpr Matrix(size_t rows, size_t cols)
        : rows_(rows), cols_(cols), data_(rows * cols) {}

    constexpr Matrix(size_t rows, size_t cols, const T &value)
        : rows_(rows), cols_(cols), data_(rows * cols, value) {}

    constexpr Matrix(const Matrix &) = default;
    constexpr Matrix(Matrix &&) noexcept = default;

    constexpr Matrix &operator=(const Matrix &) = default;
    constexpr Matrix &operator=(Matrix &&) noexcept = default;

    constexpr size_t rows() const noexcept {
        return rows_;
    }

    constexpr size_t cols() const noexcept {
        return cols_;
    }

    constexpr size_t size() const noexcept {
        return rows_ * cols_;
    }

    constexpr T *data() noexcept {
        return data_.data();
    }

    constexpr const T *data() const noexcept {
        return data_.data();
    }

    constexpr T &operator[](size_t i, size_t j) noexcept {
        assert(i < rows_ && j < cols_);
        return data_[i * cols_ + j];
    }

    constexpr const T &operator[](size_t i, size_t j) const noexcept {
        assert(i < rows_ && j < cols_);
        return data_[i * cols_ + j];
    }

    constexpr void fill(const T &value) {
        std::fill(data_.begin(), data_.end(), value);
    }

    constexpr void resize(size_t rows, size_t cols) {
        data_.resize(rows * cols);
        rows_ = rows;
        cols_ = cols;
    }

    constexpr void resize(size_t rows, size_t cols, const T &value) {
        data_.resize(rows * cols, value);
        rows_ = rows;
        cols_ = cols;
    }

private:
    size_t rows_, cols_;
    std::vector<T> data_;
};

} // namespace mini_llvm
