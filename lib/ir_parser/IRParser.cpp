#include "mini-llvm/ir_parser/IRParser.h"

#include <optional>
#include <vector>

#include "mini-llvm/common/Diagnostic.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir_parser/Lexer.h"
#include "mini-llvm/ir_parser/Parser.h"
#include "mini-llvm/ir_parser/Token.h"
#include "mini-llvm/utils/Strings.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

std::optional<Module> ir::parseModule(const char *source, std::vector<Diagnostic> &diags) {
    std::vector<Token> tokens;
    Module M;
    try {
        tokens = lex(source);
        M = parseModule(tokens);
        return M;
    } catch (const LexException &e) {
        size_t line, column;
        computeLineColumn(source, e.location(), line, column);
        diags.push_back(Diagnostic::error(e.message(), "<source>", line, column));
        return std::nullopt;
    } catch (const ParseException &e) {
        size_t line, column;
        computeLineColumn(source, e.location()->location, line, column);
        diags.push_back(Diagnostic::error(e.message(), "<source>", line, column));
        return std::nullopt;
    }
}

std::optional<Module> ir::parseModule(const char *source) {
    std::vector<Diagnostic> diags;
    return parseModule(source, diags);
}
