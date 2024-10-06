#include "mini-llvm/ir/Type/I8.h"

#include <cstdint>
#include <memory>

#include "mini-llvm/ir/Constant/I8Constant.h"
#include "mini-llvm/ir/Type.h"

using namespace mini_llvm::ir;

std::unique_ptr<Constant> I8::zeroValue() const {
    return std::make_unique<I8Constant>(static_cast<uint8_t>(0));
}
