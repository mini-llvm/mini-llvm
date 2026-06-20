// SPDX-License-Identifier: MIT

#include "mini-llvm/ir_reader/IRReader.h"

#include <optional>
#include <vector>

#include "mini-llvm/common/Diagnostic.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir_reader/Lexer.h"
#include "mini-llvm/ir_reader/Parser.h"
#include "mini-llvm/ir_reader/Token.h"

namespace mini_llvm::ir {

std::optional<Module> parseModule(const char *source) {
    std::vector<Token> tokens;
    Module M;
    try {
        tokens = lex(source);
        M = parseModule(tokens);
        return M;
    } catch (const LexException &) {
        return std::nullopt;
    } catch (const ParseException &) {
        return std::nullopt;
    }
}

std::optional<Module> parseModule(const char *source, std::vector<Diagnostic> &diags) {
    std::vector<Token> tokens;
    Module M;
    try {
        tokens = lex(source);
        M = parseModule(tokens);
        return M;
    } catch (const LexException &e) {
        diags.push_back(Diagnostic::error(e.message(), e.location() - source));
        return std::nullopt;
    } catch (const ParseException &e) {
        diags.push_back(Diagnostic::error(e.message(), e.location()->location - source));
        return std::nullopt;
    }
}

} // namespace mini_llvm::ir
