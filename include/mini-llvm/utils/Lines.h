#pragma once

#include <cstddef>
#include <string_view>
#include <utility>
#include <vector>

namespace mini_llvm {

class Lines {
public:
    using iterator = std::vector<std::string_view>::const_iterator;
    using reverse_iterator = std::vector<std::string_view>::const_reverse_iterator;

    explicit Lines(std::string_view text);

    std::string_view text() const {
        return text_;
    }

    size_t size() const {
        return lines_.size();
    }

    bool empty() {
        return lines_.empty();
    }

    iterator begin() const {
        return lines_.begin();
    }

    iterator end() const {
        return lines_.end();
    }

    reverse_iterator rbegin() const {
        return lines_.rbegin();
    }

    reverse_iterator rend() const {
        return lines_.rend();
    }

    std::string_view front() const {
        return lines_.front();
    }

    std::string_view back() const {
        return lines_.back();
    }

    std::string_view operator[](size_t i) {
        return lines_[i];
    }

    std::pair<size_t, size_t> getLineColumn(size_t offset);

private:
    std::string_view text_;
    std::vector<std::string_view> lines_;
    std::vector<size_t> sums_;
};

} // namespace mini_llvm
