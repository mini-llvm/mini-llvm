#pragma once

#include <memory>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir_parser/IRParser.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

inline std::shared_ptr<Function> parseFunction(const char *source) {
    return share(parseModule(source).value().functions.front());
}

} // namespace mini_llvm::ir
