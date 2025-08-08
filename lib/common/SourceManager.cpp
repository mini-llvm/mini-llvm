// SPDX-License-Identifier: MIT

#include "mini-llvm/common/SourceManager.h"

#include <algorithm>
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

using namespace mini_llvm;

void SourceManager::setSource(std::string source) {
    source_ = std::move(source);
    if (!source_.empty() && source_.back() != '\n') {
        source_.push_back('\n');
    }
    lines_.clear();
    size_t n = source_.size();
    size_t i = 0;
    for (size_t j = 0; j < n; ++j) {
        if (source_[j] == '\n') {
            lines_.emplace_back(source_.data() + i, source_.data() + j + 1);
            i = j + 1;
        }
    }
    sums_.clear();
    size_t sum = 0;
    for (std::string_view line : lines_) {
        sum += line.size();
        sums_.push_back(sum);
    }
}

std::pair<size_t, size_t> SourceManager::lineColumn(size_t location) const {
    size_t line = std::lower_bound(sums_.begin(), sums_.end(), location + 1) - sums_.begin();
    size_t column;
    if (line == 0) {
        column = location;
    } else {
        column = location - sums_[line - 1];
    }
    return {line, column};
}
