// SPDX-License-Identifier: MIT

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "mini-llvm/codegen/register_allocators/GraphColoringAllocator.h"
#include "mini-llvm/codegen/register_allocators/LinearScanAllocator.h"
#include "mini-llvm/codegen/register_allocators/NaiveAllocator.h"
#include "mini-llvm/codegen/RegisterAllocator.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir_reader/IRReader.h"
#include "mini-llvm/mc/Module.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/opt/ir/PassManager.h"
#include "mini-llvm/targets/riscv/RISCVBackendDriver.h"

using namespace mini_llvm;

namespace {

void testRISCV(ir::Module &IM, RegisterAllocator &allocator) {
    ir::PassManager passManager(8);
    passManager.run(IM);

    RISCVBackendDriver driver(&allocator);

    mir::Module MM;
    mc::Module MCM;

    driver.run(IM, MM, MCM);
}

} // namespace

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 1) {
        return 0;
    }

    std::unique_ptr<RegisterAllocator> allocator;
    switch (*data % 3) {
    case 0:
        allocator = std::make_unique<GraphColoringAllocator>();
        break;
    case 1:
        allocator = std::make_unique<LinearScanAllocator>();
        break;
    case 2:
        allocator = std::make_unique<NaiveAllocator>();
        break;
    }

    std::string source(reinterpret_cast<const char *>(data + 1), size - 1);

    std::optional<ir::Module> IM = ir::parseModule(source);
    if (!IM) {
        return 0;
    }
    if (!IM->isWellFormed()) {
        return 0;
    }

    testRISCV(*IM, *allocator);

    return 0;
}
