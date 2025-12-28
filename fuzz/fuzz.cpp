// SPDX-License-Identifier: MIT

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

#include "mini-llvm/codegen/register_allocators/GraphColoringAllocator.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir_reader/IRReader.h"
#include "mini-llvm/mc/Module.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/opt/ir/PassManager.h"
#include "mini-llvm/targets/riscv/RISCVBackendDriver.h"

using namespace mini_llvm;

namespace {

void testRISCV(ir::Module &IM) {
    ir::PassManager passManager(8);
    passManager.run(IM);

    GraphColoringAllocator allocator;

    RISCVBackendDriver driver(&allocator);

    mir::Module MM;
    mc::Module MCM;

    driver.run(IM, MM, MCM);
}

} // namespace

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    std::string source(reinterpret_cast<const char *>(data), size);

    std::optional<ir::Module> IM = ir::parseModule(source);
    if (!IM) {
        return 0;
    }
    if (!IM->isWellFormed()) {
        return 0;
    }

    testRISCV(*IM);

    return 0;
}
