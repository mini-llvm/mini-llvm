#include "mini-llvm/utils/Lines.h"

#include <algorithm>
#include <cstddef>
#include <numeric>
#include <string_view>
#include <utility>

using namespace mini_llvm;

Lines::Lines(std::string_view text) : text_(text) {
    size_t n = text.size();
    size_t i = 0;
    for (size_t j = 0; j < n; ++j) {
        if (text[j] == '\n') {
            lines_.emplace_back(text.data() + i, text.data() + j + 1);
            i = j + 1;
        }
    }
    if (i < n) {
        lines_.emplace_back(text.data() + i, text.data() + n);
    }
    for (std::string_view line : lines_) {
        sums_.push_back(line.size());
    }
    std::partial_sum(sums_.begin(), sums_.end(), sums_.begin());
}

std::pair<size_t, size_t> Lines::getLineColumn(size_t offset) {
    offset = std::min(offset, text_.size() - 1);
    size_t line = std::lower_bound(sums_.begin(), sums_.end(), offset + 1) - sums_.begin();
    size_t column;
    if (line == 0) {
        column = offset;
    } else {
        column = offset - sums_[line - 1];
    }
    return {line, column};
}
