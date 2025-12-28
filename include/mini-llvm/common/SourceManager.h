// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm {

class MINI_LLVM_EXPORT SourceManager {
public:
    void setSource(std::string source);

    const std::string &source() const & {
        return source_;
    }

    std::string &&source() && {
        return std::move(source_);
    }

    size_t numLines() const {
        return lineStarts_.size();
    }

    std::string_view line(size_t lineNum) const;

    std::pair<size_t, size_t> lineColumnNum(size_t location) const;

private:
    std::string source_;
    std::vector<size_t> lineStarts_;
    std::vector<size_t> lineEnds_;
};

} // namespace mini_llvm
