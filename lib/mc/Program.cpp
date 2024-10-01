#include "mini-llvm/mc/Program.h"

#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/mc/Fragment.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mc;

std::string Program::format() const {
    std::vector<const Fragment *> data, rodata, bss, text;
    for (const Fragment &fragment : *this) {
        switch (fragment.section()) {
            case Section::kData: data.push_back(&fragment); break;
            case Section::kROData: rodata.push_back(&fragment); break;
            case Section::kBSS: bss.push_back(&fragment); break;
            case Section::kText: text.push_back(&fragment); break;
            default: std::unreachable();
        }
    }
    StringJoiner formatted("\n");
    if (!data.empty()) {
        formatted.add("  .data");
        for (const Fragment *fragment : data) {
            formatted.addFormat("{}", *fragment);
        }
    }
    if (!rodata.empty()) {
        formatted.add("  .section .rodata");
        for (const Fragment *fragment : rodata) {
            formatted.addFormat("{}", *fragment);
        }
    }
    if (!bss.empty()) {
        formatted.add("  .bss");
        for (const Fragment *fragment : bss) {
            formatted.addFormat("{}", *fragment);
        }
    }
    if (!text.empty()) {
        formatted.add("  .text");
        for (const Fragment *fragment : text) {
            formatted.addFormat("{}", *fragment);
        }
    }
    return formatted.toString();
}
