// SPDX-License-Identifier: MIT

#include "mini-llvm/ir_reader/Parser.h"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/ir/Argument.h"
#include "mini-llvm/ir/Attribute/AlwaysInline.h"
#include "mini-llvm/ir/Attribute/ArgMemOnly.h"
#include "mini-llvm/ir/Attribute/InaccessibleMemOnly.h"
#include "mini-llvm/ir/Attribute/InaccessibleMemOrArgMemOnly.h"
#include "mini-llvm/ir/Attribute/NoInline.h"
#include "mini-llvm/ir/Attribute/ReadNone.h"
#include "mini-llvm/ir/Attribute/ReadOnly.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/ArrayConstant.h"
#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Constant/FloatConstant.h"
#include "mini-llvm/ir/Constant/I16Constant.h"
#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Constant/I64Constant.h"
#include "mini-llvm/ir/Constant/I8Constant.h"
#include "mini-llvm/ir/Constant/NullPtrConstant.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/GlobalVar.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Add.h"
#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Instruction/And.h"
#include "mini-llvm/ir/Instruction/ASHR.h"
#include "mini-llvm/ir/Instruction/BitCast.h"
#include "mini-llvm/ir/Instruction/Br.h"
#include "mini-llvm/ir/Instruction/Call.h"
#include "mini-llvm/ir/Instruction/CondBr.h"
#include "mini-llvm/ir/Instruction/FAdd.h"
#include "mini-llvm/ir/Instruction/FCmp.h"
#include "mini-llvm/ir/Instruction/FDiv.h"
#include "mini-llvm/ir/Instruction/FMul.h"
#include "mini-llvm/ir/Instruction/FNeg.h"
#include "mini-llvm/ir/Instruction/FPExt.h"
#include "mini-llvm/ir/Instruction/FPToSI.h"
#include "mini-llvm/ir/Instruction/FPToUI.h"
#include "mini-llvm/ir/Instruction/FPTrunc.h"
#include "mini-llvm/ir/Instruction/FRem.h"
#include "mini-llvm/ir/Instruction/FSub.h"
#include "mini-llvm/ir/Instruction/GetElementPtr.h"
#include "mini-llvm/ir/Instruction/ICmp.h"
#include "mini-llvm/ir/Instruction/IndirectCall.h"
#include "mini-llvm/ir/Instruction/IntToPtr.h"
#include "mini-llvm/ir/Instruction/Load.h"
#include "mini-llvm/ir/Instruction/LSHR.h"
#include "mini-llvm/ir/Instruction/Mul.h"
#include "mini-llvm/ir/Instruction/Or.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Instruction/PtrToInt.h"
#include "mini-llvm/ir/Instruction/Ret.h"
#include "mini-llvm/ir/Instruction/SDiv.h"
#include "mini-llvm/ir/Instruction/Select.h"
#include "mini-llvm/ir/Instruction/SExt.h"
#include "mini-llvm/ir/Instruction/SHL.h"
#include "mini-llvm/ir/Instruction/SIToFP.h"
#include "mini-llvm/ir/Instruction/SRem.h"
#include "mini-llvm/ir/Instruction/Store.h"
#include "mini-llvm/ir/Instruction/Sub.h"
#include "mini-llvm/ir/Instruction/Trunc.h"
#include "mini-llvm/ir/Instruction/UDiv.h"
#include "mini-llvm/ir/Instruction/UIToFP.h"
#include "mini-llvm/ir/Instruction/URem.h"
#include "mini-llvm/ir/Instruction/Xor.h"
#include "mini-llvm/ir/Instruction/ZExt.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/ArrayType.h"
#include "mini-llvm/ir/Type/BasicBlockType.h"
#include "mini-llvm/ir/Type/Double.h"
#include "mini-llvm/ir/Type/Float.h"
#include "mini-llvm/ir/Type/FloatingType.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/ir/Type/I1.h"
#include "mini-llvm/ir/Type/I16.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir/Type/I64.h"
#include "mini-llvm/ir/Type/I8.h"
#include "mini-llvm/ir/Type/IntegerOrPointerType.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/ir_reader/Symbol.h"
#include "mini-llvm/ir_reader/Token.h"
#include "mini-llvm/utils/BigInteger.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

using enum Token::Kind;

using Location = std::vector<Token>::const_iterator;

namespace {

class Undefined final : public Value {
public:
    explicit Undefined(std::unique_ptr<Type> type, Location location)
        : type_(std::move(type)), location_(location) {}

    std::unique_ptr<Type> type() const override {
        return type_->clone();
    }

    Location location() const {
        return location_;
    }

    std::string format() const override {
        abort();
    }

    std::string formatAsOperand() const override {
        abort();
    }

    std::unique_ptr<Value> clone() const override {
        abort();
    }

private:
    std::unique_ptr<Type> type_;
    Location location_;
};

} // namespace

class Parser::Impl {
public:
    explicit Impl(std::vector<Token>::const_iterator current)
        : current_(current) {}

    Module parseModule() {
        Module M;

        std::vector<std::pair<GlobalVar *, Location>> globalVars;
        std::vector<std::pair<Function *, Location>> functions;

        while (current_->kind != kEOF) {
            if (current_->kind == kAt) {
                bool isDeclaration;
                std::shared_ptr<GlobalVar> G = parseGlobalVarHeader(isDeclaration);
                M.appendGlobalVar(G);
                if (!isDeclaration) {
                    globalVars.emplace_back(&*G, current_);
                    int count = 0;
                    do {
                        switch (current_->kind) {
                        case kEOF:
                            if (count == 0) {
                                throw ParseException("expected initializer", current_);
                            } else {
                                throw ParseException("expected ']'", current_);
                            }

                        case kLeftBracket:
                            ++count;
                            ++current_;
                            break;

                        case kRightBracket:
                            --count;
                            ++current_;
                            break;

                        case kAt:
                            ++current_;
                            if (current_->kind == kEOF) {
                                throw ParseException("expected identifier name", current_);
                            }
                            ++current_;
                            break;

                        default:
                            ++current_;
                        }
                    } while (count > 0);
                }
            } else if (current_->kind == kDefine || current_->kind == kDeclare) {
                bool isDeclaration;
                std::shared_ptr<Function> F = parseFunctionHeader(isDeclaration);
                M.appendFunction(F);
                if (!isDeclaration) {
                    functions.emplace_back(&*F, current_);
                    if (current_->kind != kLeftBrace) {
                        throw ParseException("expected '{'", current_);
                    }
                    ++current_;
                    while (current_->kind != kEOF && current_->kind != kRightBrace) ++current_;
                    if (current_->kind != kRightBrace) {
                        throw ParseException("expected '}'", current_);
                    }
                    ++current_;
                }
            } else {
                throw ParseException("expected '@', 'define' or 'declare'", current_);
            }
        }

        for (const auto &[G, current] : globalVars) {
            current_ = current;
            parseGlobalVarInitializer(*G);
        }

        for (const auto &[F, current] : functions) {
            current_ = current;
            parseFunctionBody(*F);
        }

        return M;
    }

    std::shared_ptr<GlobalVar> parseGlobalVarHeader(bool &isDeclaration) {
        Location symbolLocation = current_;
        Symbol symbol = parseSymbol(Symbol::Scope::kGlobal);
        if (symbolTable_.contains(symbol)) {
            throw ParseException("redefinition of global identifier", symbolLocation);
        }

        if (current_->kind != kEqual) {
            throw ParseException("expected '='", current_);
        }
        ++current_;

        isDeclaration = false;
        if (current_->kind == kExternal) {
            isDeclaration = true;
            ++current_;
        }

        Linkage linkage = Linkage::kExternal;
        if (current_->kind == kInternal) {
            if (isDeclaration) {
                throw ParseException("invalid linkage", current_);
            }
            linkage = Linkage::kInternal;
            ++current_;
        } else if (current_->kind == kPrivate) {
            if (isDeclaration) {
                throw ParseException("invalid linkage", current_);
            }
            linkage = Linkage::kPrivate;
            ++current_;
        }

        bool isConstant;
        if (current_->kind == kGlobal) {
            isConstant = false;
        } else if (current_->kind == kConstant) {
            isConstant = true;
        } else {
            throw ParseException("expected 'global' or 'constant'", current_);
        }
        ++current_;

        std::unique_ptr<Type> valueType = parseType();

        std::shared_ptr<GlobalVar> G;
        if (isDeclaration) {
            G = std::make_shared<GlobalVar>(std::move(valueType), linkage);
        } else {
            G = std::make_shared<GlobalVar>(std::move(valueType), linkage, isConstant, nullptr);
        }
        G->setName(symbol.name);
        symbolTable_.put(symbol, G);
        return G;
    }

    void parseGlobalVarInitializer(GlobalVar &G) {
        G.setInitializer(cast<Constant>(parseValue(*G.valueType())));
    }

    std::shared_ptr<Function> parseFunctionHeader(bool &isDeclaration) {
        switch (current_->kind) {
            case kDefine: isDeclaration = false; break;
            case kDeclare: isDeclaration = true; break;
            default: throw ParseException("expected 'define' or 'declare'", current_);
        }
        ++current_;

        Linkage linkage = Linkage::kExternal;
        if (current_->kind == kInternal) {
            if (isDeclaration) {
                throw ParseException("invalid linkage", current_);
            }
            linkage = Linkage::kInternal;
            ++current_;
        } else if (current_->kind == kPrivate) {
            if (isDeclaration) {
                throw ParseException("invalid linkage", current_);
            }
            linkage = Linkage::kPrivate;
            ++current_;
        }

        Location returnTypeLocation = current_;
        std::unique_ptr<Type> returnType = parseType();
        if (*returnType == BasicBlockType()) {
            throw ParseException("invalid return type", returnTypeLocation);
        }

        Location symbolLocation = current_;
        Symbol symbol = parseSymbol(Symbol::Scope::kGlobal);
        if (symbolTable_.contains(symbol)) {
            throw ParseException("redefinition of global identifier", symbolLocation);
        }

        std::vector<std::unique_ptr<Type>> paramTypes;
        std::vector<std::string> paramNames;
        bool isVarArgs = false;

        if (current_->kind != kLeftParen) {
            throw ParseException("expected '('", current_);
        }
        ++current_;
        if (current_->kind != kRightParen) {
            Location typeLocation;
            std::unique_ptr<Type> type;
            if (current_->kind == kEllipsis) {
                isVarArgs = true;
                ++current_;
            } else {
                typeLocation = current_;
                type = parseType();
                if (*type == Void() || *type == BasicBlockType()) {
                    throw ParseException("invalid parameter type", typeLocation);
                }
                paramTypes.push_back(std::move(type));
                if (!isDeclaration) {
                    Symbol paramSymbol = parseSymbol(Symbol::Scope::kLocal);
                    paramNames.push_back(std::move(paramSymbol.name));
                }
            }
            while (!isVarArgs && current_->kind == kComma) {
                ++current_;
                if (current_->kind == kEllipsis) {
                    isVarArgs = true;
                    ++current_;
                } else {
                    typeLocation = current_;
                    type = parseType();
                    if (*type == Void() || *type == BasicBlockType()) {
                        throw ParseException("invalid parameter type", typeLocation);
                    }
                    paramTypes.push_back(std::move(type));
                    if (!isDeclaration) {
                        Location paramSymbolLocation = current_;
                        Symbol paramSymbol = parseSymbol(Symbol::Scope::kLocal);
                        if (std::ranges::find(paramNames, paramSymbol.name) != paramNames.end()) {
                            throw ParseException("redefinition of parameter", paramSymbolLocation);
                        }
                        paramNames.push_back(std::move(paramSymbol.name));
                    }
                }
            }
        }
        if (current_->kind != kRightParen) {
            throw ParseException("expected ')'", current_);
        }
        ++current_;

        std::unique_ptr<FunctionType> type = std::make_unique<FunctionType>(
            std::move(returnType), std::move(paramTypes), isVarArgs
        );
        std::shared_ptr<Function> F = std::make_shared<Function>(std::move(type), linkage);
        F->setName(symbol.name);
        symbolTable_.put(symbol, F);

        if (!isDeclaration) {
            for (auto [arg, paramName] : std::views::zip(args(*F), paramNames)) {
                arg.setName(paramName);
            }
        }

        for (;;) {
            bool shouldBreak = false;
            switch (current_->kind) {
            case kAlwaysInline:
                F->addAttr(std::make_unique<AlwaysInline>());
                ++current_;
                break;

            case kArgMemOnly:
                F->addAttr(std::make_unique<ArgMemOnly>());
                ++current_;
                break;

            case kInaccessibleMemOnly:
                F->addAttr(std::make_unique<InaccessibleMemOnly>());
                ++current_;
                break;

            case kInaccessibleMemOrArgMemOnly:
                F->addAttr(std::make_unique<InaccessibleMemOrArgMemOnly>());
                ++current_;
                break;

            case kNoInline:
                F->addAttr(std::make_unique<NoInline>());
                ++current_;
                break;

            case kReadNone:
                F->addAttr(std::make_unique<ReadNone>());
                ++current_;
                break;

            case kReadOnly:
                F->addAttr(std::make_unique<ReadOnly>());
                ++current_;
                break;

            default:
                shouldBreak = true;
                break;
            }
            if (shouldBreak) {
                break;
            }
        }

        return F;
    }

    void parseFunctionBody(Function &F) {
        for (Argument &arg : args(F)) {
            Symbol symbol{Symbol::Scope::kLocal, arg.name()};
            symbolTable_.put(symbol, share(arg));
        }

        if (current_->kind != kLeftBrace) {
            throw ParseException("expected '{'", current_);
        }
        ++current_;

        Location current = current_;
        while (current_->kind != kRightBrace) {
            if (current_->kind == kName && std::next(current_)->kind == kColon) {
                Location symbolLocation = current_;
                Symbol symbol{Symbol::Scope::kLocal, std::get<std::string>(current_->value)};
                if (symbolTable_.contains(symbol)) {
                    throw ParseException("redefinition of label", symbolLocation);
                }
                symbolTable_.put(symbol, std::make_shared<BasicBlock>());
            }
            ++current_;
        }
        current_ = current;

        while (current_->kind != kRightBrace) {
            if (!(current_->kind == kName && std::next(current_)->kind == kColon)) {
                throw ParseException("expected label name", current_);
            }
            Symbol symbol{Symbol::Scope::kLocal, std::get<std::string>(current_->value)};
            std::shared_ptr<BasicBlock> block = cast<BasicBlock>(symbolTable_[symbol]);
            parseBasicBlock(*block);
            F.append(std::move(block));
        }
        if (F.empty()) {
            throw ParseException("function body cannot be empty", current_);
        }
        ++current_;

        for (const auto &[symbol, value] : symbolTable_) {
            if (auto *undefined = dynamic_cast<const Undefined *>(&*value)) {
                throw ParseException("undefined local identifier", undefined->location());
            }
        }

        for (auto i = symbolTable_.begin(); i != symbolTable_.end();) {
            const auto &[symbol, value] = *i;
            if (symbol.scope == Symbol::Scope::kLocal) {
                i = symbolTable_.erase(i);
            } else {
                ++i;
            }
        }
    }

    void parseBasicBlock(BasicBlock &B) {
        if (current_->kind != kName) {
            throw ParseException("expected label name", current_);
        }
        Symbol symbol{Symbol::Scope::kLocal, std::get<std::string>(current_->value)};
        ++current_;

        if (current_->kind != kColon) {
            throw ParseException("expected ':'", current_);
        }
        ++current_;

        B.setName(symbol.name);

        while (current_->kind != kName && current_->kind != kRightBrace) {
            std::shared_ptr<Instruction> I = parseInstruction();
            B.append(I);
        }
    }

    std::shared_ptr<Instruction> parseInstruction() {
        std::shared_ptr<Instruction> I;

        if (current_->kind == kPercent) {
            Location symbolLocation = current_;
            Symbol symbol = parseSymbol(Symbol::Scope::kLocal);

            if (current_->kind != kEqual) {
                throw ParseException("expected '='", current_);
            }
            ++current_;

            switch (current_->kind) {
                case kFNeg: {
                    ++current_;
                    Location typeLocation = current_;
                    std::unique_ptr<Type> type = parseType();
                    if (!dynamic_cast<const FloatingType *>(&*type)) {
                        throw ParseException("must be a floating point type", typeLocation);
                    }
                    std::shared_ptr<Value> value = parseValue(*type);
                    I = std::make_shared<FNeg>(std::move(value));
                    break;
                }

                case kAdd:
                case kSub:
                case kFMul:
                case kSDiv:
                case kUDiv:
                case kSRem:
                case kURem:
                case kAnd:
                case kOr:
                case kXor:
                case kSHL:
                case kLSHR:
                case kASHR:
                case kFAdd:
                case kFSub:
                case kMul:
                case kFDiv:
                case kFRem: {
                    Token::Kind mnemonic = current_->kind;
                    ++current_;

                    Location typeLocation = current_;
                    std::unique_ptr<Type> type = parseType();
                    std::shared_ptr<Value> lhs = parseValue(*type);

                    if (current_->kind != kComma) {
                        throw ParseException("expected ','", current_);
                    }
                    ++current_;

                    std::shared_ptr<Value> rhs = parseValue(*type);

                    switch (mnemonic) {
                        case kAdd:
                        case kSub:
                        case kMul:
                        case kSDiv:
                        case kUDiv:
                        case kSRem:
                        case kURem:
                        case kAnd:
                        case kOr:
                        case kXor:
                        case kSHL:
                        case kLSHR:
                        case kASHR:
                            if (!dynamic_cast<const IntegerType *>(&*type)) {
                                throw ParseException("must be an integer type", typeLocation);
                            }
                            break;

                        case kFAdd:
                        case kFSub:
                        case kFMul:
                        case kFDiv:
                        case kFRem:
                            if (!dynamic_cast<const FloatingType *>(&*type)) {
                                throw ParseException("must be a floating point type", typeLocation);
                            }
                            break;

                        default:
                            abort();
                    }

                    switch (mnemonic) {
                        case kAdd: I = std::make_shared<Add>(std::move(lhs), std::move(rhs)); break;
                        case kSub: I = std::make_shared<Sub>(std::move(lhs), std::move(rhs)); break;
                        case kMul: I = std::make_shared<Mul>(std::move(lhs), std::move(rhs)); break;
                        case kSDiv: I = std::make_shared<SDiv>(std::move(lhs), std::move(rhs)); break;
                        case kUDiv: I = std::make_shared<UDiv>(std::move(lhs), std::move(rhs)); break;
                        case kSRem: I = std::make_shared<SRem>(std::move(lhs), std::move(rhs)); break;
                        case kURem: I = std::make_shared<URem>(std::move(lhs), std::move(rhs)); break;
                        case kAnd: I = std::make_shared<And>(std::move(lhs), std::move(rhs)); break;
                        case kOr: I = std::make_shared<Or>(std::move(lhs), std::move(rhs)); break;
                        case kXor: I = std::make_shared<Xor>(std::move(lhs), std::move(rhs)); break;
                        case kSHL: I = std::make_shared<SHL>(std::move(lhs), std::move(rhs)); break;
                        case kLSHR: I = std::make_shared<LSHR>(std::move(lhs), std::move(rhs)); break;
                        case kASHR: I = std::make_shared<ASHR>(std::move(lhs), std::move(rhs)); break;
                        case kFAdd: I = std::make_shared<FAdd>(std::move(lhs), std::move(rhs)); break;
                        case kFSub: I = std::make_shared<FSub>(std::move(lhs), std::move(rhs)); break;
                        case kFMul: I = std::make_shared<FMul>(std::move(lhs), std::move(rhs)); break;
                        case kFDiv: I = std::make_shared<FDiv>(std::move(lhs), std::move(rhs)); break;
                        case kFRem: I = std::make_shared<FRem>(std::move(lhs), std::move(rhs)); break;
                        default: abort();
                    }

                    break;
                }

                case kICmp: {
                    ++current_;

                    ICmp::Condition cond;
                    switch (current_->kind) {
                        case kEQ: cond = ICmp::Condition::kEQ; break;
                        case kNE: cond = ICmp::Condition::kNE; break;
                        case kSLT: cond = ICmp::Condition::kSLT; break;
                        case kSGT: cond = ICmp::Condition::kSGT; break;
                        case kSLE: cond = ICmp::Condition::kSLE; break;
                        case kSGE: cond = ICmp::Condition::kSGE; break;
                        case kULT: cond = ICmp::Condition::kULT; break;
                        case kUGT: cond = ICmp::Condition::kUGT; break;
                        case kULE: cond = ICmp::Condition::kULE; break;
                        case kUGE: cond = ICmp::Condition::kUGE; break;
                        default: throw ParseException("expected icmp condition", current_);
                    }
                    ++current_;

                    Location typeLocation = current_;
                    std::unique_ptr<Type> type = parseType();
                    if (!dynamic_cast<const IntegerOrPointerType *>(&*type)) {
                        throw ParseException("must be an integer type or ptr", typeLocation);
                    }
                    std::shared_ptr<Value> lhs = parseValue(*type);
                    if (current_->kind != kComma) {
                        throw ParseException("expected ','", current_);
                    }
                    ++current_;
                    std::shared_ptr<Value> rhs = parseValue(*type);

                    I = std::make_shared<ICmp>(cond, std::move(lhs), std::move(rhs));
                    break;
                }

                case kFCmp: {
                    ++current_;

                    FCmp::Condition cond;
                    switch (current_->kind) {
                        case kOEQ: cond = FCmp::Condition::kOEQ; break;
                        case kONE: cond = FCmp::Condition::kONE; break;
                        case kOLT: cond = FCmp::Condition::kOLT; break;
                        case kOGT: cond = FCmp::Condition::kOGT; break;
                        case kOLE: cond = FCmp::Condition::kOLE; break;
                        case kOGE: cond = FCmp::Condition::kOGE; break;
                        default: throw ParseException("expected fcmp condition", current_);
                    }
                    ++current_;

                    Location typeLocation = current_;
                    std::unique_ptr<Type> type = parseType();
                    if (!dynamic_cast<const FloatingType *>(&*type)) {
                        throw ParseException("must be a floating point type", typeLocation);
                    }
                    std::shared_ptr<Value> lhs = parseValue(*type);
                    if (current_->kind != kComma) {
                        throw ParseException("expected ','", current_);
                    }
                    ++current_;
                    std::shared_ptr<Value> rhs = parseValue(*type);

                    I = std::make_shared<FCmp>(cond, std::move(lhs), std::move(rhs));
                    break;
                }

                case kTrunc:
                case kSExt:
                case kZExt:
                case kFPTrunc:
                case kFPExt:
                case kSIToFP:
                case kUIToFP:
                case kFPToSI:
                case kFPToUI:
                case kPtrToInt:
                case kIntToPtr:
                case kBitCast: {
                    Token::Kind mnemonic = current_->kind;
                    ++current_;

                    Location type1Location = current_;
                    std::unique_ptr<Type> type1 = parseType();
                    if (mnemonic == kTrunc
                            || mnemonic == kSExt
                            || mnemonic == kZExt
                            || mnemonic == kSIToFP
                            || mnemonic == kUIToFP
                            || mnemonic == kIntToPtr) {
                        if (!dynamic_cast<const IntegerType *>(&*type1)) {
                            throw ParseException("must be an integer type", type1Location);
                        }
                    } else if (mnemonic == kFPTrunc || mnemonic == kFPExt || mnemonic == kFPToSI || mnemonic == kFPToUI) {
                        if (!dynamic_cast<const FloatingType *>(&*type1)) {
                            throw ParseException("must be a floating point type", type1Location);
                        }
                    } else if (mnemonic == kPtrToInt) {
                        if (*type1 != Ptr()) {
                            throw ParseException("must be ptr", type1Location);
                        }
                    }
                    std::shared_ptr<Value> value = parseValue(*type1);

                    if (current_->kind != kTo) {
                        throw ParseException("expected 'to'", current_);
                    }
                    ++current_;

                    Location type2Location = current_;
                    std::unique_ptr<Type> type2 = parseType();

                    if (mnemonic == kTrunc || mnemonic == kSExt || mnemonic == kZExt || mnemonic == kFPToSI || mnemonic == kFPToUI) {
                        if (!dynamic_cast<const IntegerType *>(&*type2)) {
                            throw ParseException("must be an integer type", type2Location);
                        }

                        std::unique_ptr<IntegerType> integerType2 = cast<IntegerType>(std::move(type2));

                        switch (mnemonic) {
                            case kTrunc: I = std::make_shared<Trunc>(std::move(value), std::move(integerType2)); break;
                            case kSExt: I = std::make_shared<SExt>(std::move(value), std::move(integerType2)); break;
                            case kZExt: I = std::make_shared<ZExt>(std::move(value), std::move(integerType2)); break;
                            case kFPToSI: I = std::make_shared<FPToSI>(std::move(value), std::move(integerType2)); break;
                            case kFPToUI: I = std::make_shared<FPToUI>(std::move(value), std::move(integerType2)); break;
                            default: abort();
                        }
                    } else if (mnemonic == kFPTrunc || mnemonic == kFPExt || mnemonic == kSIToFP || mnemonic == kUIToFP) {
                        if (!dynamic_cast<const FloatingType *>(&*type2)) {
                            throw ParseException("must be a floating point type", type2Location);
                        }

                        std::unique_ptr<FloatingType> floatingType2 = cast<FloatingType>(std::move(type2));

                        switch (mnemonic) {
                            case kFPTrunc: I = std::make_shared<FPTrunc>(std::move(value), std::move(floatingType2)); break;
                            case kFPExt: I = std::make_shared<FPExt>(std::move(value), std::move(floatingType2)); break;
                            case kSIToFP: I = std::make_shared<SIToFP>(std::move(value), std::move(floatingType2)); break;
                            case kUIToFP: I = std::make_shared<UIToFP>(std::move(value), std::move(floatingType2)); break;
                            default: abort();
                        }
                    } else if (mnemonic == kPtrToInt) {
                        if (!dynamic_cast<const IntegerType *>(&*type2)) {
                            throw ParseException("must be an integer type", type2Location);
                        }
                        I = std::make_shared<PtrToInt>(std::move(value), cast<IntegerType>(std::move(type2)));
                    } else if (mnemonic == kIntToPtr) {
                        if (*type2 != Ptr()) {
                            throw ParseException("must be ptr", type2Location);
                        }
                        I = std::make_shared<IntToPtr>(std::move(value));
                    } else if (mnemonic == kBitCast) {
                        if (*type1 == Ptr() && *type2 != Ptr()) {
                            throw ParseException("must be ptr", type2Location);
                        }
                        if (*type1 != Ptr() && *type2 == Ptr()) {
                            throw ParseException("must not be ptr", type2Location);
                        }
                        I = std::make_shared<BitCast>(std::move(value), std::move(type2));
                    } else {
                        abort();
                    }

                    break;
                }

                case kAlloca: {
                    ++current_;

                    std::unique_ptr<Type> allocatedType = parseType();
                    I = std::make_shared<Alloca>(std::move(allocatedType));
                    break;
                }

                case kLoad: {
                    ++current_;

                    std::unique_ptr<Type> type = parseType();

                    if (current_->kind != kComma) {
                        throw ParseException("expected ','", current_);
                    }
                    ++current_;

                    Location ptrTypeLocation = current_;
                    std::unique_ptr<Type> ptrType = parseType();
                    if (*ptrType != Ptr()) {
                        throw ParseException("must be ptr", ptrTypeLocation);
                    }

                    std::shared_ptr<Value> ptr = parseValue(Ptr());

                    I = std::make_shared<Load>(std::move(type), std::move(ptr));
                    break;
                }

                case kSelect: {
                    ++current_;

                    Location condTypeLocation = current_;
                    std::unique_ptr<Type> condType = parseType();
                    if (*condType != I1()) {
                        throw ParseException("must be i1", condTypeLocation);
                    }

                    std::shared_ptr<Value> cond = parseValue(I1());

                    if (current_->kind != kComma) {
                        throw ParseException("expected ','", current_);
                    }
                    ++current_;

                    std::unique_ptr<Type> trueValueType = parseType();
                    std::shared_ptr<Value> trueValue = parseValue(*trueValueType);

                    if (current_->kind != kComma) {
                        throw ParseException("expected ','", current_);
                    }
                    ++current_;

                    std::unique_ptr<Type> falseValueType = parseType();
                    std::shared_ptr<Value> falseValue = parseValue(*trueValueType);

                    I = std::make_shared<Select>(std::move(cond), std::move(trueValue), std::move(falseValue));
                    break;
                }

                case kGetElementPtr: {
                    ++current_;

                    Location sourceTypeLocation = current_;
                    std::unique_ptr<Type> sourceType = parseType();
                    if (*sourceType == Void() || *sourceType == BasicBlockType()) {
                        throw ParseException("invalid source type", sourceTypeLocation);
                    }

                    if (current_->kind != kComma) {
                        throw ParseException("expected ','", current_);
                    }
                    ++current_;

                    Location ptrTypeLocation = current_;
                    std::unique_ptr<Type> ptrType = parseType();
                    if (*ptrType != Ptr()) {
                        throw ParseException("must be ptr", ptrTypeLocation);
                    }

                    std::shared_ptr<Value> ptr = parseValue(Ptr());

                    std::vector<std::shared_ptr<Value>> indices;
                    Location idxTypeLocation;
                    std::unique_ptr<Type> idxType;
                    std::unique_ptr<Type> type = sourceType->clone();
                    if (current_->kind == kComma) {
                        ++current_;
                        idxTypeLocation = current_;
                        idxType = parseType();
                        if (!dynamic_cast<const IntegerType *>(&*idxType)) {
                            throw ParseException("must be an integer type", idxTypeLocation);
                        }
                        indices.push_back(parseValue(*idxType));
                        while (current_->kind == kComma) {
                            ++current_;
                            idxTypeLocation = current_;
                            idxType = parseType();
                            if (!dynamic_cast<const IntegerType *>(&*idxType)) {
                                throw ParseException("must be an integer type", idxTypeLocation);
                            }
                            indices.push_back(parseValue(*idxType));
                            if (!dynamic_cast<const ArrayType *>(&*type)) {
                                throw ParseException("too many indices", idxTypeLocation);
                            }
                            type = static_cast<const ArrayType *>(&*type)->elementType();
                        }
                    }

                    I = std::make_shared<GetElementPtr>(std::move(sourceType), std::move(ptr), std::move(indices));

                    break;
                }

                case kCall: {
                    ++current_;

                    Location returnTypeLocation = current_;
                    std::unique_ptr<Type> returnType = parseType();
                    if (*returnType == Void()) {
                        throw ParseException("must not be void", returnTypeLocation);
                    }

                    bool hasParamTypes = false;
                    std::vector<std::unique_ptr<Type>> paramTypes;
                    bool isVarArgs = false;

                    if (current_->kind == kLeftParen) {
                        hasParamTypes = true;
                        ++current_;
                        if (current_->kind != kRightParen) {
                            if (current_->kind == kEllipsis) {
                                isVarArgs = true;
                                ++current_;
                            } else {
                                paramTypes.push_back(parseType());
                            }
                            while (!isVarArgs && current_->kind == kComma) {
                                ++current_;
                                if (current_->kind == kEllipsis) {
                                    isVarArgs = true;
                                    ++current_;
                                } else {
                                    paramTypes.push_back(parseType());
                                }
                            }
                        }
                        if (current_->kind != kRightParen) {
                            throw ParseException("expected ')'", current_);
                        }
                        ++current_;
                    }

                    std::shared_ptr<Value> callee = parseValue(Ptr());

                    if (current_->kind != kLeftParen) {
                        throw ParseException("expected '('", current_);
                    }
                    ++current_;

                    std::vector<std::shared_ptr<Value>> args;
                    if (current_->kind != kRightParen) {
                        args.push_back(parseValue(*parseType()));
                        while (current_->kind == kComma) {
                            ++current_;
                            args.push_back(parseValue(*parseType()));
                        }
                    }

                    if (current_->kind != kRightParen) {
                        throw ParseException("expected ')'", current_);
                    }
                    ++current_;

                    if (dynamic_cast<const Function *>(&*callee)) {
                        std::shared_ptr<Function> calleeFunction = cast<Function>(callee);

                        if (hasParamTypes) {
                            std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
                                std::move(returnType), std::move(paramTypes), isVarArgs
                            );
                            if (*calleeFunction->functionType() != *functionType) {
                                throw ParseException("function type mismatch", returnTypeLocation);
                            }
                        } else {
                            if (*calleeFunction->functionType()->returnType() != *returnType) {
                                throw ParseException("return type mismatch", returnTypeLocation);
                            }
                        }

                        I = std::make_shared<Call>(std::move(calleeFunction), std::move(args));
                    } else {
                        if (!hasParamTypes) {
                            for (const auto &arg : args) {
                                paramTypes.push_back(arg->type());
                            }
                        }

                        std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
                            std::move(returnType), std::move(paramTypes), isVarArgs
                        );

                        I = std::make_shared<IndirectCall>(std::move(functionType), std::move(callee), std::move(args));
                    }

                    break;
                }

                case kPhi: {
                    ++current_;

                    std::shared_ptr<Phi> phi = std::make_shared<Phi>(parseType());

                    auto parseIncoming = [this](Phi &phi) {
                        if (current_->kind != kLeftBracket) {
                            throw ParseException("expected '['", current_);
                        }
                        ++current_;

                        std::shared_ptr<Value> value = parseValue(*phi.type());

                        if (current_->kind != kComma) {
                            throw ParseException("expected ','", current_);
                        }
                        ++current_;

                        Location blockLocation = current_;
                        std::shared_ptr<BasicBlock> block = cast<BasicBlock>(parseValue(BasicBlockType()));

                        if (current_->kind != kRightBracket) {
                            throw ParseException("expected ']'", current_);
                        }
                        ++current_;

                        if (hasIncomingBlock(phi, *block)) {
                            throw ParseException("duplicate incoming block", blockLocation);
                        }

                        phi.addIncoming(block, value);
                    };

                    parseIncoming(*phi);
                    while (current_->kind == kComma) {
                        ++current_;
                        parseIncoming(*phi);
                    }

                    I = phi;
                    break;
                }

                default: {
                    throw ParseException("expected instruction mnemonic", current_);
                }
            }

            I->setName(symbol.name);

            if (symbolTable_.contains(symbol)) {
                std::shared_ptr<Value> II = symbolTable_[symbol];
                if (!dynamic_cast<const Undefined *>(&*II)) {
                    throw ParseException("redefinition of local identifier", symbolLocation);
                }
                if (*II->type() != *I->type()) {
                    throw ParseException("type mismatch", symbolLocation);
                }
                replaceAllUsesWith(*II, I);
                symbolTable_[symbol] = I;

                std::unordered_set<const Instruction *> visited;
                std::queue<const Instruction *> Q;
                visited.insert(&*I);
                Q.push(&*I);
                while (!Q.empty()) {
                    const Instruction *u = Q.front();
                    Q.pop();
                    for (const UseBase &use : uses(*u)) {
                        if (auto *v = dynamic_cast<const Instruction *>(use.user())) {
                            if (!dynamic_cast<const Phi *>(v)) {
                                if (v == &*I) {
                                    throw ParseException("circular reference", symbolLocation);
                                }
                                if (visited.insert(v).second) {
                                    Q.push(v);
                                }
                            }
                        }
                    }
                }
            } else {
                symbolTable_.put(symbol, I);
            }
        } else if (current_->kind == kStore) {
            ++current_;

            std::unique_ptr<Type> type = parseType();
            std::shared_ptr<Value> value = parseValue(*type);

            if (current_->kind != kComma) {
                throw ParseException("expected ','", current_);
            }
            ++current_;

            Location ptrTypeLocation = current_;
            std::unique_ptr<Type> ptrType = parseType();
            if (*ptrType != Ptr()) {
                throw ParseException("must be ptr", ptrTypeLocation);
            }

            std::shared_ptr<Value> ptr = parseValue(Ptr());

            return std::make_shared<Store>(std::move(value), std::move(ptr));
        } else if (current_->kind == kCall) {
            ++current_;

            Location returnTypeLocation = current_;
            std::unique_ptr<Type> returnType = parseType();
            if (*returnType != Void()) {
                throw ParseException("must be void", returnTypeLocation);
            }

            bool hasParamTypes = false;
            std::vector<std::unique_ptr<Type>> paramTypes;
            bool isVarArgs = false;

            if (current_->kind == kLeftParen) {
                hasParamTypes = true;
                ++current_;
                if (current_->kind != kRightParen) {
                    if (current_->kind == kEllipsis) {
                        isVarArgs = true;
                        ++current_;
                    } else {
                        paramTypes.push_back(parseType());
                    }
                    while (!isVarArgs && current_->kind == kComma) {
                        ++current_;
                        if (current_->kind == kEllipsis) {
                            isVarArgs = true;
                            ++current_;
                        } else {
                            paramTypes.push_back(parseType());
                        }
                    }
                }
                if (current_->kind != kRightParen) {
                    throw ParseException("expected ')'", current_);
                }
                ++current_;
            }

            std::shared_ptr<Value> callee = parseValue(Ptr());

            if (current_->kind != kLeftParen) {
                throw ParseException("expected '('", current_);
            }
            ++current_;

            std::vector<std::shared_ptr<Value>> args;
            if (current_->kind != kRightParen) {
                args.push_back(parseValue(*parseType()));
                while (current_->kind == kComma) {
                    ++current_;
                    args.push_back(parseValue(*parseType()));
                }
            }

            if (current_->kind != kRightParen) {
                throw ParseException("expected ')'", current_);
            }
            ++current_;

            if (dynamic_cast<const Function *>(&*callee)) {
                std::shared_ptr<Function> calleeFunction = cast<Function>(callee);

                if (hasParamTypes) {
                    std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
                        std::move(returnType), std::move(paramTypes), isVarArgs
                    );
                    if (*calleeFunction->functionType() != *functionType) {
                        throw ParseException("function type mismatch", returnTypeLocation);
                    }
                } else {
                    if (*calleeFunction->functionType()->returnType() != *returnType) {
                        throw ParseException("return type mismatch", returnTypeLocation);
                    }
                }

                return std::make_shared<Call>(std::move(calleeFunction), std::move(args));
            } else {
                if (!hasParamTypes) {
                    for (const auto &arg : args) {
                        paramTypes.push_back(arg->type());
                    }
                }

                std::unique_ptr<FunctionType> functionType = std::make_unique<FunctionType>(
                    std::move(returnType), std::move(paramTypes), isVarArgs
                );

                return std::make_shared<IndirectCall>(std::move(functionType), std::move(callee), std::move(args));
            }
        } else if (current_->kind == kBr) {
            ++current_;

            Location typeLocation = current_;
            std::unique_ptr<Type> type = parseType();

            if (*type == BasicBlockType()) {
                std::shared_ptr<BasicBlock> dest = cast<BasicBlock>(parseValue(BasicBlockType()));
                return std::make_shared<Br>(std::move(dest));
            } else if (*type == I1()) {
                std::shared_ptr<Value> cond = parseValue(I1());

                if (current_->kind != kComma) {
                    throw ParseException("expected ','", current_);
                }
                ++current_;

                Location trueDestTypeLocation = current_;
                std::unique_ptr<Type> trueDestType = parseType();
                if (*trueDestType != BasicBlockType()) {
                    throw ParseException("must be label", trueDestTypeLocation);
                }

                std::shared_ptr<BasicBlock> trueDest = cast<BasicBlock>(parseValue(BasicBlockType()));

                if (current_->kind != kComma) {
                    throw ParseException("expected ','", current_);
                }
                ++current_;

                Location falseDestTypeLocation = current_;
                std::unique_ptr<Type> falseDestType = parseType();
                if (*falseDestType != BasicBlockType()) {
                    throw ParseException("must be label", falseDestTypeLocation);
                }

                std::shared_ptr<BasicBlock> falseDest = cast<BasicBlock>(parseValue(BasicBlockType()));

                return std::make_shared<CondBr>(std::move(cond), std::move(trueDest), std::move(falseDest));
            } else {
                throw ParseException("must be label or i1", typeLocation);
            }
        } else if (current_->kind == kRet) {
            ++current_;

            std::unique_ptr<Type> type = parseType();
            if (*type == Void()) {
                return std::make_shared<Ret>();
            }

            std::shared_ptr<Value> value = parseValue(*type);
            return std::make_shared<Ret>(std::move(value));
        } else {
            throw ParseException("expected local identifier or instruction mnemonic", current_);
        }

        return I;
    }

    std::shared_ptr<Value> parseValue(const Type &type) {
        switch (current_->kind) {
            case kPercent:
                return parseIdentifier(type);

            case kAt:
            case kNumber:
            case kTrue:
            case kFalse:
            case kNull:
            case kZeroInitializer:
            case kLeftBracket:
            case kString:
            case kPoison:
                return parseConstant(type);

            default:
                throw ParseException("expected identifier or constant", current_);
        }
    }

    std::shared_ptr<Value> parseIdentifier(const Type &type) {
        Location symbolLocation = current_;
        Symbol symbol = parseSymbol();
        if (symbolTable_.contains(symbol)) {
            std::shared_ptr<Value> value = symbolTable_[symbol];
            if (*value->type() != type) {
                throw ParseException("type mismatch", symbolLocation);
            }
            return value;
        }
        if (symbol.scope == Symbol::Scope::kGlobal) {
            throw ParseException("undefined global identifier", symbolLocation);
        }
        if (type == BasicBlockType()) {
            throw ParseException("undefined label", symbolLocation);
        }
        std::shared_ptr<Value> undefined = std::make_shared<Undefined>(type.clone(), symbolLocation);
        symbolTable_.put(symbol, undefined);
        return undefined;
    }

    std::shared_ptr<Constant> parseConstant(const Type &type) {
        if (current_->kind == kZeroInitializer) {
            ++current_;
            return type.zeroValue();
        }
        if (type == I1()) {
            switch (current_->kind) {
                case kTrue:
                    ++current_;
                    return std::make_unique<I1Constant>(true);

                case kFalse:
                    ++current_;
                    return std::make_unique<I1Constant>(false);

                case kPoison:
                    ++current_;
                    return std::make_unique<PoisonValue>(std::make_unique<I1>());

                default:
                    throw ParseException("expected 'true', 'false', 'poison' or 'zeroinitializer'", current_);
            }
        }
        if (type == I8()) {
            switch (current_->kind) {
                case kNumber: {
                    int8_t value = static_cast<int8_t>(std::get<BigInteger>(current_->value).toInt64() & 0xff);
                    ++current_;
                    return std::make_unique<I8Constant>(value);
                }
                case kPoison: {
                    ++current_;
                    return std::make_unique<PoisonValue>(std::make_unique<I8>());
                }
                default: {
                    throw ParseException("expected number, 'poison' or 'zeroinitializer'", current_);
                }
            }
        }
        if (type == I16()) {
            switch (current_->kind) {
                case kNumber: {
                    int16_t value = static_cast<int16_t>(std::get<BigInteger>(current_->value).toInt64() & 0xffff);
                    ++current_;
                    return std::make_unique<I16Constant>(value);
                }
                case kPoison: {
                    ++current_;
                    return std::make_unique<PoisonValue>(std::make_unique<I16>());
                }
                default: {
                    throw ParseException("expected number, 'poison' or 'zeroinitializer'", current_);
                }
            }
        }
        if (type == I32()) {
            switch (current_->kind) {
                case kNumber: {
                    int32_t value = static_cast<int32_t>(std::get<BigInteger>(current_->value).toInt64() & 0xffffffff);
                    ++current_;
                    return std::make_unique<I32Constant>(static_cast<int32_t>(value));
                }
                case kPoison: {
                    ++current_;
                    return std::make_unique<PoisonValue>(std::make_unique<I32>());
                }
                default: {
                    throw ParseException("expected number, 'poison' or 'zeroinitializer'", current_);
                }
            }
        }
        if (type == I64()) {
            switch (current_->kind) {
                case kNumber: {
                    int64_t value = std::get<BigInteger>(current_->value).toInt64();
                    ++current_;
                    return std::make_unique<I64Constant>(value);
                }
                case kPoison: {
                    ++current_;
                    return std::make_unique<PoisonValue>(std::make_unique<I64>());
                }
                default: {
                    throw ParseException("expected number, 'poison' or 'zeroinitializer'", current_);
                }
            }
        }
        if (type == Float()) {
            switch (current_->kind) {
                case kNumber: {
                    float value = static_cast<float>(std::bit_cast<double>(std::get<BigInteger>(current_->value).toInt64()));
                    ++current_;
                    return std::make_unique<FloatConstant>(value);
                }
                case kPoison: {
                    ++current_;
                    return std::make_unique<PoisonValue>(std::make_unique<Float>());
                }
                default: {
                    throw ParseException("expected number, 'poison' or 'zeroinitializer'", current_);
                }
            }
        }
        if (type == Double()) {
            switch (current_->kind) {
                case kNumber: {
                    double value = std::bit_cast<double>(std::get<BigInteger>(current_->value).toInt64());
                    ++current_;
                    return std::make_unique<DoubleConstant>(value);
                }
                case kPoison: {
                    ++current_;
                    return std::make_unique<PoisonValue>(std::make_unique<Double>());
                }
                default: {
                    throw ParseException("expected number, 'poison' or 'zeroinitializer'", current_);
                }
            }
        }
        if (type == Ptr()) {
            switch (current_->kind) {
                case kAt: {
                    return cast<Constant>(parseIdentifier(type));
                }
                case kNull: {
                    ++current_;
                    return std::make_unique<NullPtrConstant>();
                }
                case kPoison: {
                    ++current_;
                    return std::make_unique<PoisonValue>(std::make_unique<Ptr>());
                }
                default: {
                    throw ParseException("expected '@', 'null', 'poison' or 'zeroinitializer'", current_);
                }
            }
        }
        if (auto *arrayType = dynamic_cast<const ArrayType *>(&type)) {
            if (current_->kind == kPoison) {
                ++current_;
                return std::make_unique<PoisonValue>(arrayType->clone());
            }
            Location elementsLocation = current_;
            std::vector<std::shared_ptr<Constant>> elements;
            if (current_->kind == kLeftBracket) {
                ++current_;
                if (current_->kind != kRightBracket) {
                    Location elementTypeLocation;
                    std::unique_ptr<Type> elementType;
                    elementTypeLocation = current_;
                    elementType = parseType();
                    if (*elementType != *arrayType->elementType()) {
                        throw ParseException("element type mismatch", elementTypeLocation);
                    }
                    elements.push_back(parseConstant(*arrayType->elementType()));
                    while (current_->kind == kComma) {
                        ++current_;
                        elementTypeLocation = current_;
                        elementType = parseType();
                        if (*elementType != *arrayType->elementType()) {
                            throw ParseException("element type mismatch", elementTypeLocation);
                        }
                        elements.push_back(parseConstant(*arrayType->elementType()));
                    }
                }
                if (current_->kind != kRightBracket) {
                    throw ParseException("expected ']'", current_);
                }
                ++current_;
                if (elements.size() != arrayType->numElements()) {
                    throw ParseException("mismatch in number of elements", elementsLocation);
                }
            } else if (current_->kind == kString) {
                if (ir::I8() != *arrayType->elementType()) {
                    throw ParseException("element type mismatch", elementsLocation);
                }
                if (std::get<std::vector<int8_t>>(current_->value).size() != arrayType->numElements()) {
                    throw ParseException("mismatch in number of elements", elementsLocation);
                }
                for (int8_t element : std::get<std::vector<int8_t>>(current_->value)) {
                    elements.push_back(std::make_shared<I8Constant>(element));
                }
                ++current_;
            } else {
                throw ParseException("expected '[', string, 'poison' or 'zeroinitializer'", current_);
            }
            return std::make_unique<ArrayConstant>(cast<ArrayType>(arrayType->clone()), std::move(elements));
        }
        throw ParseException("expected constant", current_);
    }

    std::unique_ptr<Type> parseType() {
        switch (current_->kind) {
            case kI1: ++current_; return std::make_unique<I1>();
            case kI8: ++current_; return std::make_unique<I8>();
            case kI16: ++current_; return std::make_unique<I16>();
            case kI32: ++current_; return std::make_unique<I32>();
            case kI64: ++current_; return std::make_unique<I64>();
            case kFloat: ++current_; return std::make_unique<Float>();
            case kDouble: ++current_; return std::make_unique<Double>();
            case kPtr: ++current_; return std::make_unique<Ptr>();
            case kLabel: ++current_; return std::make_unique<BasicBlockType>();
            case kVoid: ++current_; return std::make_unique<Void>();

            case kLeftBracket: {
                ++current_;
                if (current_->kind != kNumber) {
                    throw ParseException("expected number", current_);
                }
                BigInteger numElements = std::get<BigInteger>(current_->value);
                if (numElements < 0) {
                    throw ParseException("number of elements must be non-negative", current_);
                }
                if (numElements > std::numeric_limits<int64_t>::max()) {
                    throw ParseException("number of elements is too large", current_);
                }
                ++current_;
                if (current_->kind != kX) {
                    throw ParseException("expected 'x'", current_);
                }
                ++current_;
                Location elementTypeLocation = current_;
                std::unique_ptr<Type> elementType = parseType();
                if (*elementType == Void() || *elementType == BasicBlockType()) {
                    throw ParseException("invalid element type", elementTypeLocation);
                }
                if (current_->kind != kRightBracket) {
                    throw ParseException("expected ']'", current_);
                }
                ++current_;
                return std::make_unique<ArrayType>(std::move(elementType), static_cast<size_t>(numElements.toInt64()));
            }

            default: {
                throw ParseException("expected type", current_);
            }
        }
    }

    Symbol parseSymbol(std::optional<Symbol::Scope> scope = std::nullopt) {
        Symbol symbol;

        switch (current_->kind) {
            case kAt: symbol.scope = Symbol::Scope::kGlobal; break;
            case kPercent: symbol.scope = Symbol::Scope::kLocal; break;
            default: throw ParseException("expected '@' or '%'", current_);
        }
        ++current_;

        if (current_->kind == kName) {
            symbol.name = std::get<std::string>(current_->value);
            ++current_;
        } else {
            throw ParseException("expected identifier name", current_);
        }

        if (scope && symbol.scope != *scope) {
            switch (*scope) {
                case Symbol::Scope::kGlobal: throw ParseException("expected global identifier", current_);
                case Symbol::Scope::kLocal: throw ParseException("expected local identifier", current_);
            }
        }

        return symbol;
    }

private:
    std::vector<Token>::const_iterator current_;
    HashMap<Symbol, std::shared_ptr<Value>> symbolTable_;
};

Parser::Parser(std::vector<Token>::const_iterator current)
    : impl_(std::make_unique<Impl>(current)) {}

Parser::~Parser() = default;

Parser::Parser(Parser &&) noexcept = default;

Parser &Parser::operator=(Parser &&) noexcept = default;

Module Parser::parseModule() {
    return impl_->parseModule();
}

std::shared_ptr<GlobalVar> Parser::parseGlobalVarHeader(bool &isDeclaration) {
    return impl_->parseGlobalVarHeader(isDeclaration);
}

void Parser::parseGlobalVarInitializer(GlobalVar &G) {
    impl_->parseGlobalVarInitializer(G);
}

std::shared_ptr<Function> Parser::parseFunctionHeader(bool &isDeclaration) {
    return impl_->parseFunctionHeader(isDeclaration);
}

void Parser::parseFunctionBody(Function &F) {
    impl_->parseFunctionBody(F);
}

void Parser::parseBasicBlock(BasicBlock &B) {
    impl_->parseBasicBlock(B);
}

std::shared_ptr<Instruction> Parser::parseInstruction() {
    return impl_->parseInstruction();
}

std::shared_ptr<Value> Parser::parseValue(const Type &type) {
    return impl_->parseValue(type);
}

std::shared_ptr<Value> Parser::parseIdentifier(const Type &type) {
    return impl_->parseIdentifier(type);
}

std::shared_ptr<Constant> Parser::parseConstant(const Type &type) {
    return impl_->parseConstant(type);
}

std::unique_ptr<Type> Parser::parseType() {
    return impl_->parseType();
}

Symbol Parser::parseSymbol(std::optional<Symbol::Scope> scope) {
    return impl_->parseSymbol(scope);
}

Module ir::parseModule(const std::vector<Token> &tokens) {
    return Parser(tokens.begin()).parseModule();
}
