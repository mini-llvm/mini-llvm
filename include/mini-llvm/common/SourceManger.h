#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace mini_llvm {

class SourceManager {
public:
    void setSource(std::string source);

    const std::string &source() const {
        return source_;
    }

    size_t lineCount() const {
        return lines_.size();
    }

    std::string_view line(size_t i) const {
        return lines_[i];
    }

    std::pair<size_t, size_t> lineColumn(size_t location) const;

private:
    std::string source_;
    std::vector<std::string_view> lines_;
    std::vector<size_t> sums_;
};

} // namespace mini_llvm
