#include "mini-llvm/ir_parser/IRParser.h"

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir_parser/Lexer.h"
#include "mini-llvm/ir_parser/Parser.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

Module ir::parseModule(const char *source) {
    return parseModule(lex(source));
}
