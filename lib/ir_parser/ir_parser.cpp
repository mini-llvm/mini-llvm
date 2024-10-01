#include "mini-llvm/ir_parser/ir_parser.h"

#include <memory>
#include <vector>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/GlobalVar.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir_parser/Lexer.h"
#include "mini-llvm/ir_parser/Parser.h"
#include "mini-llvm/ir_parser/Token.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

Module ir::parseModule(const char *source) {
    return Parser(std::vector<Token>(Lexer(source)).begin()).parseModule();
}

std::shared_ptr<GlobalVar> ir::parseGlobalVar(const char *source) {
    Module M = parseModule(source);
    return share(M.globalVars.front());
}

std::shared_ptr<Function> ir::parseFunction(const char *source) {
    Module M = parseModule(source);
    return share(M.functions.front());
}
