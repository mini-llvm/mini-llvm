// SPDX-License-Identifier: MIT

#include "mini-llvm/common/SourceManager.h"

#include <algorithm>
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

using namespace mini_llvm;

void SourceManager::setSource(std::string source) {
    if (!source.empty() && source.back() != '\n') {
        source.push_back('\n');
    }
    source_ = std::move(source);
    lineStarts_.clear();
    lineEnds_.clear();
    size_t i = 0, j;
    while ((j = source_.find('\n', i)) != std::string::npos) {
        lineStarts_.push_back(i);
        lineEnds_.push_back(j);
        i = j + 1;
    }
}

std::string_view SourceManager::line(size_t lineNum) const {
    size_t i = lineStarts_[lineNum - 1];
    size_t j = lineEnds_[lineNum - 1];
    return std::string_view(source_).substr(i, j - i + 1);
}

std::pair<size_t, size_t> SourceManager::lineColumnNum(size_t location) const {
    size_t lineNum = std::upper_bound(lineStarts_.begin(), lineStarts_.end(), location) - lineStarts_.begin();
    size_t columnNum = location - lineStarts_[lineNum - 1] + 1;
    return {lineNum, columnNum};
}
