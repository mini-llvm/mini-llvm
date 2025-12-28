// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction/Br.h"

#include <format>
#include <memory>
#include <string>

#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::ir;

std::string Br::format() const {
    return std::format("br {} {:o}", *dest()->type(), *dest());
}

std::unique_ptr<Value> Br::clone() const {
    return std::make_unique<Br>(weaken(*dest()));
}
