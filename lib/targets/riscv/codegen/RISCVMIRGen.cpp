#include "mini-llvm/targets/riscv/codegen/RISCVMIRGen.h"

#include <cassert>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "mini-llvm/common/ExtensionMode.h"
#include "mini-llvm/common/Precision.h"
#include "mini-llvm/ir/Argument.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/ArrayConstant.h"
#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Constant/FloatConstant.h"
#include "mini-llvm/ir/Constant/FloatingConstant.h"
#include "mini-llvm/ir/Constant/I16Constant.h"
#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Constant/I64Constant.h"
#include "mini-llvm/ir/Constant/I8Constant.h"
#include "mini-llvm/ir/Constant/IntegerConstant.h"
#include "mini-llvm/ir/Constant/NullPtrConstant.h"
#include "mini-llvm/ir/ConstantVisitor.h"
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
#include "mini-llvm/ir/Instruction/FSub.h"
#include "mini-llvm/ir/Instruction/GetElementPtr.h"
#include "mini-llvm/ir/Instruction/ICmp.h"
#include "mini-llvm/ir/Instruction/Load.h"
#include "mini-llvm/ir/Instruction/LSHR.h"
#include "mini-llvm/ir/Instruction/Mul.h"
#include "mini-llvm/ir/Instruction/Or.h"
#include "mini-llvm/ir/Instruction/Phi.h"
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
#include "mini-llvm/ir/Type/Double.h"
#include "mini-llvm/ir/Type/Float.h"
#include "mini-llvm/ir/Type/FloatingType.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/ir/Type/I1.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/ir/TypeVisitor.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/BasicBlockBuilder.h"
#include "mini-llvm/mir/Condition.h"
#include "mini-llvm/mir/Constant/I16ArrayConstant.h"
#include "mini-llvm/mir/Constant/I16ArrayConstant.h"
#include "mini-llvm/mir/Constant/I16Constant.h"
#include "mini-llvm/mir/Constant/I16Constant.h"
#include "mini-llvm/mir/Constant/I32ArrayConstant.h"
#include "mini-llvm/mir/Constant/I32ArrayConstant.h"
#include "mini-llvm/mir/Constant/I32Constant.h"
#include "mini-llvm/mir/Constant/I32Constant.h"
#include "mini-llvm/mir/Constant/I64ArrayConstant.h"
#include "mini-llvm/mir/Constant/I64ArrayConstant.h"
#include "mini-llvm/mir/Constant/I64Constant.h"
#include "mini-llvm/mir/Constant/I64Constant.h"
#include "mini-llvm/mir/Constant/I8ArrayConstant.h"
#include "mini-llvm/mir/Constant/I8ArrayConstant.h"
#include "mini-llvm/mir/Constant/I8Constant.h"
#include "mini-llvm/mir/Constant/I8Constant.h"
#include "mini-llvm/mir/Constant/PtrConstant.h"
#include "mini-llvm/mir/Constant/ZeroConstant.h"
#include "mini-llvm/mir/ConstantVisitor.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/GlobalVar.h"
#include "mini-llvm/mir/Immediate.h"
#include "mini-llvm/mir/Instruction/Add.h"
#include "mini-llvm/mir/Instruction/AddI.h"
#include "mini-llvm/mir/Instruction/And.h"
#include "mini-llvm/mir/Instruction/AndI.h"
#include "mini-llvm/mir/Instruction/Br.h"
#include "mini-llvm/mir/Instruction/CmpBr.h"
#include "mini-llvm/mir/Instruction/CmpSet.h"
#include "mini-llvm/mir/Instruction/CmpZBr.h"
#include "mini-llvm/mir/Instruction/CmpZSet.h"
#include "mini-llvm/mir/Instruction/FAdd.h"
#include "mini-llvm/mir/Instruction/FCmpSet.h"
#include "mini-llvm/mir/Instruction/FCvt.h"
#include "mini-llvm/mir/Instruction/FCvtFS.h"
#include "mini-llvm/mir/Instruction/FCvtFU.h"
#include "mini-llvm/mir/Instruction/FCvtSF.h"
#include "mini-llvm/mir/Instruction/FCvtUF.h"
#include "mini-llvm/mir/Instruction/FDiv.h"
#include "mini-llvm/mir/Instruction/FLoad.h"
#include "mini-llvm/mir/Instruction/FMov.h"
#include "mini-llvm/mir/Instruction/FMovFI.h"
#include "mini-llvm/mir/Instruction/FMovIF.h"
#include "mini-llvm/mir/Instruction/FMul.h"
#include "mini-llvm/mir/Instruction/FNeg.h"
#include "mini-llvm/mir/Instruction/FStore.h"
#include "mini-llvm/mir/Instruction/FSub.h"
#include "mini-llvm/mir/Instruction/LA.h"
#include "mini-llvm/mir/Instruction/LI.h"
#include "mini-llvm/mir/Instruction/Load.h"
#include "mini-llvm/mir/Instruction/Mov.h"
#include "mini-llvm/mir/Instruction/Mul.h"
#include "mini-llvm/mir/Instruction/Or.h"
#include "mini-llvm/mir/Instruction/SDiv.h"
#include "mini-llvm/mir/Instruction/SExt.h"
#include "mini-llvm/mir/Instruction/SHL.h"
#include "mini-llvm/mir/Instruction/SHLI.h"
#include "mini-llvm/mir/Instruction/SHRA.h"
#include "mini-llvm/mir/Instruction/SHRAI.h"
#include "mini-llvm/mir/Instruction/SHRL.h"
#include "mini-llvm/mir/Instruction/SHRLI.h"
#include "mini-llvm/mir/Instruction/SRem.h"
#include "mini-llvm/mir/Instruction/Store.h"
#include "mini-llvm/mir/Instruction/Sub.h"
#include "mini-llvm/mir/Instruction/UDiv.h"
#include "mini-llvm/mir/Instruction/URem.h"
#include "mini-llvm/mir/Instruction/Xor.h"
#include "mini-llvm/mir/Instruction/XorI.h"
#include "mini-llvm/mir/IntegerImmediate.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/StackRelativeOffsetImmediate.h"
#include "mini-llvm/mir/StackSlot.h"
#include "mini-llvm/mir/VirtualRegister.h"
#include "mini-llvm/targets/riscv/mir/Instruction/RISCVCall.h"
#include "mini-llvm/targets/riscv/mir/Instruction/RISCVRet.h"
#include "mini-llvm/targets/riscv/mir/RISCVRegister.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;
using namespace mini_llvm::mir::riscv;

namespace {

class TypeVisitorImpl final : public ir::TypeVisitor {
public:
    explicit TypeVisitorImpl(GlobalVar &globalVar,
                             const std::vector<std::shared_ptr<ir::Constant>> &flattened)
        : globalVar_(globalVar), flattened_(flattened) {}

    void visitI1(const ir::I1 &) override {
        visitIntegerType<ir::I1Constant, I8ArrayConstant, int8_t>();
    }

    void visitI8(const ir::I8 &) override {
        visitIntegerType<ir::I8Constant, I8ArrayConstant, int8_t>();
    }

    void visitI16(const ir::I16 &) override {
        visitIntegerType<ir::I16Constant, I16ArrayConstant, int16_t>();
    }

    void visitI32(const ir::I32 &) override {
        visitIntegerType<ir::I32Constant, I32ArrayConstant, int32_t>();
    }

    void visitI64(const ir::I64 &) override {
        visitIntegerType<ir::I64Constant, I64ArrayConstant, int64_t>();
    }

    void visitFloat(const ir::Float &) override {
        visitFloatingType<ir::FloatConstant, I32ArrayConstant, int32_t>();
    }

    void visitDouble(const ir::Double &) override {
        visitFloatingType<ir::DoubleConstant, I64ArrayConstant, int64_t>();
    }

private:
    GlobalVar &globalVar_;
    const std::vector<std::shared_ptr<ir::Constant>> &flattened_;

    template <typename IConst, typename MConst, typename Integer>
    void visitIntegerType() {
        std::vector<Integer> elements;
        for (const auto &element : flattened_) {
            elements.push_back(static_cast<Integer>(static_cast<const IConst *>(&*element)->value()));
        }
        globalVar_.setInitializer(std::make_unique<MConst>(std::move(elements)));
    }

    template <typename IConst, typename MConst, typename Integer>
    void visitFloatingType() {
        std::vector<Integer> elements;
        for (const auto &element : flattened_) {
            elements.push_back(std::bit_cast<Integer>(static_cast<const IConst *>(&*element)->value()));
        }
        globalVar_.setInitializer(std::make_unique<MConst>(std::move(elements)));
    }
};

class ConstantVisitorImpl final : public ir::ConstantVisitor {
public:
    explicit ConstantVisitorImpl(GlobalVar &globalVar,
                                 const std::unordered_map<const ir::GlobalVar *, GlobalVar *> &globalVarMap)
        : globalVar_(globalVar), globalVarMap_(globalVarMap) {}

    void visitI1Constant(const ir::I1Constant &C) override {
        visitIntegerConstant<ir::I1Constant, I8Constant, int8_t>(C);
    }

    void visitI8Constant(const ir::I8Constant &C) override {
        visitIntegerConstant<ir::I8Constant, I8Constant, int8_t>(C);
    }

    void visitI16Constant(const ir::I16Constant &C) override {
        visitIntegerConstant<ir::I16Constant, I16Constant, int16_t>(C);
    }

    void visitI32Constant(const ir::I32Constant &C) override {
        visitIntegerConstant<ir::I32Constant, I32Constant, int32_t>(C);
    }

    void visitI64Constant(const ir::I64Constant &C) override {
        visitIntegerConstant<ir::I64Constant, I64Constant, int64_t>(C);
    }

    void visitFloatConstant(const ir::FloatConstant &C) override {
        visitFloatingConstant<ir::FloatConstant, I32Constant, int32_t>(C);
    }

    void visitDoubleConstant(const ir::DoubleConstant &C) override {
        visitFloatingConstant<ir::DoubleConstant, I64Constant, int64_t>(C);
    }

    void visitNullPtrConstant(const ir::NullPtrConstant &) override {
        globalVar_.setInitializer(std::make_unique<ZeroConstant>(8));
    }

    void visitGlobalVar(const ir::GlobalVar &G) override {
        GlobalVar *ptr = globalVarMap_.at(&G);
        globalVar_.setInitializer(std::make_unique<PtrConstant>(8, ptr));
    }

    void visitArrayConstant(const ir::ArrayConstant &C) override {
        if (C == *C.type()->zeroValue()) {
            globalVar_.setInitializer(std::make_unique<ZeroConstant>(C.type()->size(8)));
        } else {
            std::vector<std::shared_ptr<ir::Constant>> flattened = flatten(C);

            std::unique_ptr<ir::Type> type = C.type();
            while (dynamic_cast<const ir::ArrayType *>(&*type)) {
                type = static_cast<const ir::ArrayType *>(&*type)->elementType();
            }

            TypeVisitorImpl visitor(globalVar_, flattened);
            type->accept(visitor);
        }
    }

private:
    GlobalVar &globalVar_;
    const std::unordered_map<const ir::GlobalVar *, GlobalVar *> &globalVarMap_;

    template <typename IConst, typename MConst, typename Integer>
    void visitIntegerConstant(const IConst &C) {
        if (C == *C.type()->zeroValue()) {
            globalVar_.setInitializer(std::make_unique<ZeroConstant>(C.type()->size(8)));
        } else {
            globalVar_.setInitializer(std::make_unique<MConst>(static_cast<Integer>(C.value())));
        }
    }

    template <typename IConst, typename MConst, typename Integer>
    void visitFloatingConstant(const IConst &C) {
        if (C == *C.type()->zeroValue()) {
            globalVar_.setInitializer(std::make_unique<ZeroConstant>(C.type()->size(8)));
        } else {
            globalVar_.setInitializer(std::make_unique<MConst>(std::bit_cast<Integer>(C.value())));
        }
    }
};

class InstructionVisitorImpl final : public ir::InstructionVisitor {
public:
    InstructionVisitorImpl(Function &function,
                           const std::unordered_map<const ir::GlobalVar *, GlobalVar *> &globalVarMap,
                           const std::unordered_map<const ir::Function *, Function *> &functionMap,
                           const std::unordered_map<const ir::BasicBlock *, BasicBlock *> &blockMap,
                           const std::unordered_map<const ir::Alloca *, StackSlot *> &memoryMap,
                           const std::unordered_map<const ir::Value *, std::shared_ptr<Register>> &valueMap,
                           BasicBlock *epilogueBlock)
        : function_(function),
          globalVarMap_(globalVarMap),
          functionMap_(functionMap),
          blockMap_(blockMap),
          memoryMap_(memoryMap),
          valueMap_(valueMap),
          epilogueBlock_(epilogueBlock) {}

    BasicBlockBuilder &builder() {
        return builder_;
    }

    const BasicBlockBuilder &builder() const {
        return builder_;
    }

    void visitAdd(const ir::Add &I) override {
        visitBinaryIntegerArithmeticOperator<ir::Add, Add>(I);
    }

    void visitSub(const ir::Sub &I) override {
        visitBinaryIntegerArithmeticOperator<ir::Sub, Sub>(I);
    }

    void visitMul(const ir::Mul &I) override {
        visitBinaryIntegerArithmeticOperator<ir::Mul, Mul>(I);
    }

    void visitSDiv(const ir::SDiv &I) override {
        visitBinaryIntegerArithmeticOperator<ir::SDiv, SDiv>(I);
    }

    void visitUDiv(const ir::UDiv &I) override {
        visitBinaryIntegerArithmeticOperator<ir::UDiv, UDiv>(I);
    }

    void visitSRem(const ir::SRem &I) override {
        visitBinaryIntegerArithmeticOperator<ir::SRem, SRem>(I);
    }

    void visitURem(const ir::URem &I) override {
        visitBinaryIntegerArithmeticOperator<ir::URem, URem>(I);
    }

    void visitAnd(const ir::And &I) override {
        visitBinaryIntegerBitwiseOperator<ir::And, And>(I);
    }

    void visitOr(const ir::Or &I) override {
        visitBinaryIntegerBitwiseOperator<ir::Or, Or>(I);
    }

    void visitXor(const ir::Xor &I) override {
        visitBinaryIntegerBitwiseOperator<ir::Xor, Xor>(I);
    }

    void visitSHL(const ir::SHL &I) override {
        visitBinaryIntegerArithmeticOperator<ir::SHL, SHL>(I);
    }

    void visitLSHR(const ir::LSHR &I) override {
        visitBinaryIntegerArithmeticOperator<ir::LSHR, SHRL>(I);
    }

    void visitASHR(const ir::ASHR &I) override {
        visitBinaryIntegerArithmeticOperator<ir::ASHR, SHRA>(I);
    }

    void visitTrunc(const ir::Trunc &I) override {
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src = prepareRegister(*I.value());
        int dstWidth = I.type()->size(8),
            srcWidth = I.value()->type()->size(8);
        assert(dstWidth < srcWidth);
        if (dstWidth == 4) {
            builder_.add(std::make_unique<SExt>(8, 4, std::move(dst), std::move(src)));
        } else {
            builder_.add(std::make_unique<Mov>(8, dst, src));
            builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - dstWidth * 8)));
            builder_.add(std::make_unique<SHRAI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - dstWidth * 8)));
        }
    }

    void visitSExt(const ir::SExt &I) override {
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src = prepareRegister(*I.value());
        if (*I.value()->type() == ir::I1()) {
            builder_.add(std::make_unique<Mov>(8, dst, src));
            builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(63)));
            builder_.add(std::make_unique<SHRAI>(8, dst, dst, std::make_unique<IntegerImmediate>(63)));
        } else {
            int dstWidth = I.type()->size(8),
                srcWidth = I.value()->type()->size(8);
            assert(dstWidth > srcWidth);
            if (srcWidth == 4) {
                builder_.add(std::make_unique<SExt>(8, 4, std::move(dst), std::move(src)));
            } else {
                builder_.add(std::make_unique<Mov>(8, dst, src));
                builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - srcWidth * 8)));
                builder_.add(std::make_unique<SHRAI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - srcWidth * 8)));
            }
        }
    }

    void visitZExt(const ir::ZExt &I) override {
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src = prepareRegister(*I.value());
        if (*I.value()->type() == ir::I1()) {
            builder_.add(std::make_unique<Mov>(8, dst, src));
            builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(63)));
            builder_.add(std::make_unique<SHRLI>(8, dst, dst, std::make_unique<IntegerImmediate>(63)));
        } else {
            int dstWidth = I.type()->size(8),
                srcWidth = I.value()->type()->size(8);
            assert(dstWidth > srcWidth);
            if (srcWidth == 1) {
                builder_.add(std::make_unique<AndI>(8, dst, src, std::make_unique<IntegerImmediate>(0xff)));
            } else {
                builder_.add(std::make_unique<Mov>(8, dst, src));
                builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - srcWidth * 8)));
                builder_.add(std::make_unique<SHRLI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - srcWidth * 8)));
            }
        }
    }

    void visitICmp(const ir::ICmp &I) override {
        int width = I.opType()->size(8);
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src1 = prepareRegister(*I.lhs()),
                                  src2 = prepareRegister(*I.rhs());

        switch (I.cond()) {
            case ir::ICmp::Condition::kEQ:
            case ir::ICmp::Condition::kNE: {
                builder_.add(std::make_unique<Xor>(8, dst, src1, src2));

                Condition cond;
                if (I.cond() == ir::ICmp::Condition::kEQ) cond = Condition::kEQZ;
                if (I.cond() == ir::ICmp::Condition::kNE) cond = Condition::kNEZ;
                builder_.add(std::make_unique<CmpZSet>(8, 8, cond, dst, dst));
                builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(63)));
                builder_.add(std::make_unique<SHRAI>(8, dst, dst, std::make_unique<IntegerImmediate>(63)));

                break;
            }

            case ir::ICmp::Condition::kSLT:
            case ir::ICmp::Condition::kSGT:
            case ir::ICmp::Condition::kSLE:
            case ir::ICmp::Condition::kSGE:
            case ir::ICmp::Condition::kULT:
            case ir::ICmp::Condition::kUGT:
            case ir::ICmp::Condition::kULE:
            case ir::ICmp::Condition::kUGE: {
                Condition cond;
                bool zeroExtend;
                bool negate;
                switch (I.cond()) {
                    case ir::ICmp::Condition::kSLT: cond = Condition::kSLT; zeroExtend = false; negate = false; break;
                    case ir::ICmp::Condition::kSGT: cond = Condition::kSGT; zeroExtend = false; negate = false; break;
                    case ir::ICmp::Condition::kSLE: cond = Condition::kSGT; zeroExtend = false; negate = true; break;
                    case ir::ICmp::Condition::kSGE: cond = Condition::kSLT; zeroExtend = false; negate = true; break;
                    case ir::ICmp::Condition::kULT: cond = Condition::kULT; zeroExtend = true; negate = false; break;
                    case ir::ICmp::Condition::kUGT: cond = Condition::kUGT; zeroExtend = true; negate = false; break;
                    case ir::ICmp::Condition::kULE: cond = Condition::kUGT; zeroExtend = true; negate = true; break;
                    case ir::ICmp::Condition::kUGE: cond = Condition::kULT; zeroExtend = true; negate = true; break;
                    default: std::unreachable();
                }

                if (zeroExtend) {
                    std::shared_ptr<Register> tmp1 = std::make_shared<VirtualRegister>(),
                                              tmp2 = std::make_shared<VirtualRegister>();
                    if (width == 1) {
                        builder_.add(std::make_unique<AndI>(8, tmp1, src1, std::make_unique<IntegerImmediate>(0xff)));
                        builder_.add(std::make_unique<AndI>(8, tmp2, src2, std::make_unique<IntegerImmediate>(0xff)));
                    } else {
                        builder_.add(std::make_unique<Mov>(8, tmp1, src1));
                        builder_.add(std::make_unique<SHLI>(8, tmp1, tmp1, std::make_unique<IntegerImmediate>(64 - width * 8)));
                        builder_.add(std::make_unique<SHRLI>(8, tmp1, tmp1, std::make_unique<IntegerImmediate>(64 - width * 8)));

                        builder_.add(std::make_unique<Mov>(8, tmp2, src2));
                        builder_.add(std::make_unique<SHLI>(8, tmp2, tmp2, std::make_unique<IntegerImmediate>(64 - width * 8)));
                        builder_.add(std::make_unique<SHRLI>(8, tmp2, tmp2, std::make_unique<IntegerImmediate>(64 - width * 8)));
                    }
                    src1 = std::move(tmp1);
                    src2 = std::move(tmp2);
                }

                builder_.add(std::make_unique<CmpSet>(8, 8, cond, dst, src1, src2));
                builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(63)));
                builder_.add(std::make_unique<SHRAI>(8, dst, dst, std::make_unique<IntegerImmediate>(63)));

                if (negate) {
                    builder_.add(std::make_unique<XorI>(8, dst, dst, std::make_unique<IntegerImmediate>(-1)));
                }

                break;
            }

            default: std::unreachable();
        }
    }

    void visitFNeg(const ir::FNeg &I) override {
        visitUnaryFloatingArithmeticOperator<ir::FNeg, FNeg>(I);
    }

    void visitFAdd(const ir::FAdd &I) override {
        visitBinaryFloatingArithmeticOperator<ir::FAdd, FAdd>(I);
    }

    void visitFSub(const ir::FSub &I) override {
        visitBinaryFloatingArithmeticOperator<ir::FSub, FSub>(I);
    }

    void visitFMul(const ir::FMul &I) override {
        visitBinaryFloatingArithmeticOperator<ir::FMul, FMul>(I);
    }

    void visitFDiv(const ir::FDiv &I) override {
        visitBinaryFloatingArithmeticOperator<ir::FDiv, FDiv>(I);
    }

    void visitFPTrunc(const ir::FPTrunc &I) override {
        Precision dstPrecision = static_cast<const ir::FloatingType *>(&*I.type())->precision(),
                  srcPrecision = static_cast<const ir::FloatingType *>(&*I.value()->type())->precision();
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src = prepareRegister(*I.value());
        builder_.add(std::make_unique<FCvt>(dstPrecision, srcPrecision, std::move(dst), std::move(src)));
    }

    void visitFPExt(const ir::FPExt &I) override {
        Precision dstPrecision = static_cast<const ir::FloatingType *>(&*I.type())->precision(),
                  srcPrecision = static_cast<const ir::FloatingType *>(&*I.value()->type())->precision();
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src = prepareRegister(*I.value());
        builder_.add(std::make_unique<FCvt>(dstPrecision, srcPrecision, std::move(dst), std::move(src)));
    }

    void visitSIToFP(const ir::SIToFP &I) override {
        Precision dstPrecision = static_cast<const ir::FloatingType *>(&*I.type())->precision();
        int srcWidth = I.value()->type()->size();
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src = prepareRegister(*I.value());
        builder_.add(std::make_unique<FCvtFS>(dstPrecision, srcWidth, std::move(dst), std::move(src)));
    }

    void visitUIToFP(const ir::UIToFP &I) override {
        Precision dstPrecision = static_cast<const ir::FloatingType *>(&*I.type())->precision();
        int srcWidth = I.value()->type()->size();
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src = prepareRegister(*I.value());
        builder_.add(std::make_unique<FCvtFU>(dstPrecision, srcWidth, std::move(dst), std::move(src)));
    }

    void visitFPToSI(const ir::FPToSI &I) override {
        int dstWidth = I.type()->size();
        Precision srcPrecision = static_cast<const ir::FloatingType *>(&*I.value()->type())->precision();
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src = prepareRegister(*I.value());
        builder_.add(std::make_unique<FCvtSF>(dstWidth, srcPrecision, std::move(dst), std::move(src)));
    }

    void visitFPToUI(const ir::FPToUI &I) override {
        int dstWidth = I.type()->size();
        Precision srcPrecision = static_cast<const ir::FloatingType *>(&*I.value()->type())->precision();
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src = prepareRegister(*I.value());
        builder_.add(std::make_unique<FCvtUF>(dstWidth, srcPrecision, std::move(dst), std::move(src)));
    }

    void visitFCmp(const ir::FCmp &I) override {
        Precision precision = static_cast<const ir::FloatingType *>(&*I.opType())->precision();
        Condition cond;
        bool negate;
        switch (I.cond()) {
            case ir::FCmp::Condition::kOEQ: cond = Condition::kOEQ; negate = false; break;
            case ir::FCmp::Condition::kONE: cond = Condition::kOEQ; negate = true; break;
            case ir::FCmp::Condition::kOLT: cond = Condition::kOLT; negate = false; break;
            case ir::FCmp::Condition::kOGT: cond = Condition::kOGT; negate = false; break;
            case ir::FCmp::Condition::kOLE: cond = Condition::kOLE; negate = false; break;
            case ir::FCmp::Condition::kOGE: cond = Condition::kOGE; negate = false; break;
            default: std::unreachable();
        }
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src1 = prepareRegister(*I.lhs()),
                                  src2 = prepareRegister(*I.rhs());
        builder_.add(std::make_unique<FCmpSet>(8, precision, cond, dst, src1, src2));
        builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(63)));
        builder_.add(std::make_unique<SHRAI>(8, dst, dst, std::make_unique<IntegerImmediate>(63)));
        if (negate) {
            builder_.add(std::make_unique<XorI>(8, dst, dst, std::make_unique<IntegerImmediate>(-1)));
        }
    }

    void visitBitCast(const ir::BitCast &I) override {
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src = prepareRegister(*I.value());
        if (dynamic_cast<const ir::IntegerType *>(&*I.type()) && dynamic_cast<const ir::IntegerType *>(&*I.value()->type())) {
            builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.type()) && dynamic_cast<const ir::FloatingType *>(&*I.value()->type())) {
            builder_.add(std::make_unique<FMov>(Precision::kDouble, std::move(dst), std::move(src)));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.type()) && dynamic_cast<const ir::IntegerType *>(&*I.value()->type())) {
            builder_.add(std::make_unique<FMovFI>(Precision::kDouble, std::move(dst), std::move(src)));
        } else if (dynamic_cast<const ir::IntegerType *>(&*I.type()) && dynamic_cast<const ir::FloatingType *>(&*I.value()->type())) {
            builder_.add(std::make_unique<FMovIF>(Precision::kDouble, std::move(dst), std::move(src)));
        } else {
            std::unreachable();
        }
    }

    void visitSelect(const ir::Select &I) override {
        BasicBlock &trueBlock = function_.append(),
                   &falseBlock = function_.append(),
                   &endBlock = function_.append();

        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  cond = prepareRegister(*I.cond()),
                                  src1 = prepareRegister(*I.trueValue()),
                                  src2 = prepareRegister(*I.falseValue());

        builder_.add(std::make_unique<CmpZBr>(8, Condition::kNEZ, cond, &trueBlock, &falseBlock));

        builder_.setPos(&trueBlock, trueBlock.end());
        if (dynamic_cast<const ir::IntegerType *>(&*I.type())) {
            builder_.add(std::make_unique<Mov>(8, dst, src1));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.type())) {
            Precision precision = static_cast<const ir::FloatingType *>(&*I.type())->precision();
            builder_.add(std::make_unique<FMov>(precision, dst, src1));
        } else {
            std::unreachable();
        }
        builder_.add(std::make_unique<Br>(&endBlock));

        builder_.setPos(&falseBlock, falseBlock.end());
        if (dynamic_cast<const ir::IntegerType *>(&*I.type())) {
            builder_.add(std::make_unique<Mov>(8, dst, src2));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.type())) {
            Precision precision = static_cast<const ir::FloatingType *>(&*I.type())->precision();
            builder_.add(std::make_unique<FMov>(precision, dst, src2));
        } else {
            std::unreachable();
        }
        builder_.add(std::make_unique<Br>(&endBlock));

        builder_.setPos(&endBlock, endBlock.end());
    }

    void visitAlloca(const ir::Alloca &I) override {
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src1 = share(*fp());
        StackSlot *endSlot = &function_.stackFrame().back(),
                  *slot = memoryMap_.at(&I);
        std::unique_ptr<Immediate> src2 =
            std::make_unique<StackRelativeOffsetImmediate>(endSlot, slot);
        builder_.add(std::make_unique<AddI>(8, std::move(dst), std::move(src1), std::move(src2)));
    }

    void visitLoad(const ir::Load &I) override {
        MemoryOperand src(prepareRegister(*I.ptr()), std::make_unique<IntegerImmediate>(0));
        std::shared_ptr<Register> dst = valueMap_.at(&I);

        if (dynamic_cast<const ir::IntegerType *>(&*I.type())) {
            int width = I.type()->size(8);
            ExtensionMode extensionMode = width == 8 ? ExtensionMode::kNo : ExtensionMode::kSign;
            builder_.add(std::make_unique<Load>(width, std::move(dst), std::move(src), extensionMode));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.type())) {
            Precision precision = static_cast<const ir::FloatingType *>(&*I.type())->precision();
            builder_.add(std::make_unique<FLoad>(precision, std::move(dst), std::move(src)));
        } else {
            std::unreachable();
        }
    }

    void visitStore(const ir::Store &I) override {
        MemoryOperand dst(prepareRegister(*I.ptr()), std::make_unique<IntegerImmediate>(0));
        std::shared_ptr<Register> src = prepareRegister(*I.value());

        if (dynamic_cast<const ir::IntegerType *>(&*I.value()->type())) {
            int width = I.value()->type()->size(8);
            builder_.add(std::make_unique<Store>(width, std::move(dst), std::move(src)));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.value()->type())) {
            Precision precision = static_cast<const ir::FloatingType *>(&*I.value()->type())->precision();
            builder_.add(std::make_unique<FStore>(precision, std::move(dst), std::move(src)));
        } else {
            std::unreachable();
        }
    }

    void visitGetElementPtr(const ir::GetElementPtr &I) override {
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src = prepareRegister(*I.ptr());

        builder_.add(std::make_unique<Mov>(8, dst, src));

        auto i = I.idx_begin();
        std::unique_ptr<ir::Type> type = I.sourceType();

        for (;;) {
            std::shared_ptr<Register> idx = prepareRegister(**i);
            std::shared_ptr<Register> size = std::make_shared<VirtualRegister>();
            builder_.add(std::make_unique<LI>(8, size, std::make_unique<IntegerImmediate>(type->size(8))));
            std::shared_ptr<Register> offset = std::make_shared<VirtualRegister>();
            builder_.add(std::make_unique<Mul>(8, offset, idx, size));
            builder_.add(std::make_unique<Add>(8, dst, dst, offset));

            ++i;
            if (i == I.idx_end()) break;
            type = static_cast<const ir::ArrayType *>(&*type)->elementType();
        }
    }

    void visitCall(const ir::Call &I) override {
        Function *callee = functionMap_.at(&*I.callee());

        size_t numIntegerArgs = 0,
               numFloatingArgs = 0;
        std::vector<const ir::Value *> stackArgs;
        for (const ir::Use<ir::Value> &arg : args(I)) {
            if (I.callee()->functionType()->isVarArgs()) {
                if (numIntegerArgs < 8) {
                    std::shared_ptr<Register> dst = share(*riscvIntegerArgRegs()[numIntegerArgs]),
                                              src = prepareRegister(*arg);
                    if (dynamic_cast<const ir::FloatingType *>(&*arg->type())) {
                        Precision precision = static_cast<const ir::FloatingType *>(&*arg->type())->precision();
                        std::shared_ptr<Register> tmp = std::make_shared<VirtualRegister>();
                        builder_.add(std::make_unique<FMovIF>(precision, tmp, src));
                        src = std::move(tmp);
                    }
                    builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                    ++numIntegerArgs;
                } else {
                    stackArgs.push_back(&*arg);
                }
            } else {
                if (dynamic_cast<const ir::IntegerType *>(&*arg->type())) {
                    if (numIntegerArgs < 8) {
                        std::shared_ptr<Register> dst = share(*riscvIntegerArgRegs()[numIntegerArgs]),
                                                  src = prepareRegister(*arg);
                        builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                        ++numIntegerArgs;
                    } else {
                        stackArgs.push_back(&*arg);
                    }
                } else if (dynamic_cast<const ir::FloatingType *>(&*arg->type())) {
                    if (numFloatingArgs < 8) {
                        Precision precision = static_cast<const ir::FloatingType *>(&*arg->type())->precision();
                        std::shared_ptr<Register> dst = share(*riscvFloatingArgRegs()[numFloatingArgs]),
                                                  src = prepareRegister(*arg);
                        builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
                        ++numFloatingArgs;
                    } else {
                        stackArgs.push_back(&*arg);
                    }
                } else {
                    std::unreachable();
                }
            }
        }

        if (!stackArgs.empty()) {
            int n = stackArgs.size();
            builder_.add(std::make_unique<AddI>(8, share(*sp()), share(*sp()), std::make_unique<IntegerImmediate>(-(n * 8 + 15) / 16 * 16)));
            for (int i = 0; i < n; ++i) {
                MemoryOperand dst(share(*sp()), std::make_unique<IntegerImmediate>(i * 8));
                std::shared_ptr<Register> src = prepareRegister(*stackArgs[i]);
                if (dynamic_cast<const ir::IntegerType *>(&*stackArgs[i]->type())) {
                    builder_.add(std::make_unique<Store>(8, std::move(dst), std::move(src)));
                } else if (dynamic_cast<const ir::FloatingType *>(&*stackArgs[i]->type())) {
                    Precision precision = static_cast<const ir::FloatingType *>(&*stackArgs[i]->type())->precision();
                    builder_.add(std::make_unique<FStore>(precision, std::move(dst), std::move(src)));
                } else {
                    std::unreachable();
                }
            }
        }

        builder_.add(std::make_unique<RISCVCall>(callee, numIntegerArgs, numFloatingArgs));

        if (!stackArgs.empty()) {
            int n = stackArgs.size();
            builder_.add(std::make_unique<AddI>(8, share(*sp()), share(*sp()), std::make_unique<IntegerImmediate>((n * 8 + 15) / 16 * 16)));
        }

        if (dynamic_cast<const ir::IntegerType *>(&*I.type())) {
            std::shared_ptr<Register> dst = valueMap_.at(&I),
                                      src = share(*riscvIntegerResultRegs()[0]);
            builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.type())) {
            Precision precision = static_cast<const ir::FloatingType *>(&*I.type())->precision();
            std::shared_ptr<Register> dst = valueMap_.at(&I),
                                      src = share(*riscvFloatingResultRegs()[0]);
            builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
        } else {
            assert(*I.type() == ir::Void());
        }
    }

    void visitBr(const ir::Br &I) override {
        BasicBlock *dest = blockMap_.at(&*I.dest());
        std::vector<const ir::Phi *> phis;
        for (const ir::Instruction &II : *I.dest()) {
            if (auto *phi = dynamic_cast<const ir::Phi *>(&II)) {
                phis.push_back(phi);
            }
        }
        if (!phis.empty()) {
            BasicBlockBuilder builder = builder_;
            BasicBlock &middle = function_.append();
            builder_.setPos(&middle);
            std::unordered_map<const ir::Phi *, std::shared_ptr<Register>> tmps;
            for (const ir::Phi *phi : phis) {
                tmps[phi] = std::make_shared<VirtualRegister>();
            }
            for (const ir::Phi *phi : phis) {
                std::shared_ptr<Register> dst = tmps[phi],
                                          src = prepareRegister(*getIncomingValue(*phi, *I.parent()));
                if (dynamic_cast<const ir::IntegerType *>(&*phi->type())) {
                    builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                } else if (dynamic_cast<const ir::FloatingType *>(&*phi->type())) {
                    Precision precision = static_cast<const ir::FloatingType *>(&*phi->type())->precision();
                    builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
                } else {
                    std::unreachable();
                }
            }
            for (const ir::Phi *phi : phis) {
                std::shared_ptr<Register> dst = valueMap_.at(phi),
                                          src = tmps[phi];
                if (dynamic_cast<const ir::IntegerType *>(&*phi->type())) {
                    builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                } else if (dynamic_cast<const ir::FloatingType *>(&*phi->type())) {
                    Precision precision = static_cast<const ir::FloatingType *>(&*phi->type())->precision();
                    builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
                } else {
                    std::unreachable();
                }
            }
            builder_.add(std::make_unique<Br>(dest));
            builder_ = builder;
            dest = &middle;
        }
        builder_.add(std::make_unique<Br>(dest));
    }

    void visitCondBr(const ir::CondBr &I) override {
        BasicBlock *trueDest = blockMap_.at(&*I.trueDest());
        BasicBlock *falseDest = blockMap_.at(&*I.falseDest());
        {
            std::vector<const ir::Phi *> phis;
            for (const ir::Instruction &II : *I.trueDest()) {
                if (auto *phi = dynamic_cast<const ir::Phi *>(&II)) {
                    phis.push_back(phi);
                }
            }
            if (!phis.empty()) {
                BasicBlockBuilder builder = builder_;
                BasicBlock &middle = function_.append();
                builder_.setPos(&middle);
                std::unordered_map<const ir::Phi *, std::shared_ptr<Register>> tmps;
                for (const ir::Phi *phi : phis) {
                    tmps[phi] = std::make_shared<VirtualRegister>();
                }
                for (const ir::Phi *phi : phis) {
                    std::shared_ptr<Register> dst = tmps[phi],
                                              src = prepareRegister(*getIncomingValue(*phi, *I.parent()));
                    if (dynamic_cast<const ir::IntegerType *>(&*phi->type())) {
                        builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                    } else if (dynamic_cast<const ir::FloatingType *>(&*phi->type())) {
                        Precision precision = static_cast<const ir::FloatingType *>(&*phi->type())->precision();
                        builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
                    } else {
                        std::unreachable();
                    }
                }
                for (const ir::Phi *phi : phis) {
                    std::shared_ptr<Register> dst = valueMap_.at(phi),
                                              src = tmps[phi];
                    if (dynamic_cast<const ir::IntegerType *>(&*phi->type())) {
                        builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                    } else if (dynamic_cast<const ir::FloatingType *>(&*phi->type())) {
                        Precision precision = static_cast<const ir::FloatingType *>(&*phi->type())->precision();
                        builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
                    } else {
                        std::unreachable();
                    }
                }
                builder_.add(std::make_unique<Br>(trueDest));
                builder_ = builder;
                trueDest = &middle;
            }
        }
        {
            std::vector<const ir::Phi *> phis;
            for (const ir::Instruction &II : *I.falseDest()) {
                if (auto *phi = dynamic_cast<const ir::Phi *>(&II)) {
                    phis.push_back(phi);
                }
            }
            if (!phis.empty()) {
                BasicBlockBuilder builder = builder_;
                BasicBlock &middle = function_.append();
                builder_.setPos(&middle);
                std::unordered_map<const ir::Phi *, std::shared_ptr<Register>> tmps;
                for (const ir::Phi *phi : phis) {
                    tmps[phi] = std::make_shared<VirtualRegister>();
                }
                for (const ir::Phi *phi : phis) {
                    std::shared_ptr<Register> dst = tmps[phi],
                                              src = prepareRegister(*getIncomingValue(*phi, *I.parent()));
                    if (dynamic_cast<const ir::IntegerType *>(&*phi->type())) {
                        builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                    } else if (dynamic_cast<const ir::FloatingType *>(&*phi->type())) {
                        Precision precision = static_cast<const ir::FloatingType *>(&*phi->type())->precision();
                        builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
                    } else {
                        std::unreachable();
                    }
                }
                for (const ir::Phi *phi : phis) {
                    std::shared_ptr<Register> dst = valueMap_.at(phi),
                                              src = tmps[phi];
                    if (dynamic_cast<const ir::IntegerType *>(&*phi->type())) {
                        builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                    } else if (dynamic_cast<const ir::FloatingType *>(&*phi->type())) {
                        Precision precision = static_cast<const ir::FloatingType *>(&*phi->type())->precision();
                        builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
                    } else {
                        std::unreachable();
                    }
                }
                builder_.add(std::make_unique<Br>(falseDest));
                builder_ = builder;
                falseDest = &middle;
            }
        }
        if (auto *icmp = dynamic_cast<const ir::ICmp *>(&*I.cond())) {
            int width = icmp->opType()->size(8);
            std::shared_ptr<Register> src1 = prepareRegister(*icmp->lhs()),
                                      src2 = prepareRegister(*icmp->rhs());
            Condition cond;
            switch (icmp->cond()) {
                case ir::ICmp::Condition::kEQ: cond = Condition::kEQ; break;
                case ir::ICmp::Condition::kNE: cond = Condition::kNE; break;
                case ir::ICmp::Condition::kSLT: cond = Condition::kSLT; break;
                case ir::ICmp::Condition::kSGT: cond = Condition::kSGT; break;
                case ir::ICmp::Condition::kSLE: cond = Condition::kSLE; break;
                case ir::ICmp::Condition::kSGE: cond = Condition::kSGE; break;
                case ir::ICmp::Condition::kULT: cond = Condition::kULT; break;
                case ir::ICmp::Condition::kUGT: cond = Condition::kUGT; break;
                case ir::ICmp::Condition::kULE: cond = Condition::kULE; break;
                case ir::ICmp::Condition::kUGE: cond = Condition::kUGE; break;
            }
            builder_.add(std::make_unique<CmpBr>(width, cond, std::move(src1), std::move(src2), trueDest, falseDest));
        } else {
            std::shared_ptr<Register> cond = prepareRegister(*I.cond());
            builder_.add(std::make_unique<CmpZBr>(8, Condition::kNEZ, std::move(cond), trueDest, falseDest));
        }
    }

    void visitRet(const ir::Ret &I) override {
        if (dynamic_cast<const ir::IntegerType *>(&*I.value()->type())) {
            std::shared_ptr<Register> dst = share(*riscvIntegerResultRegs()[0]),
                                      src = prepareRegister(*I.value());
            builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.value()->type())) {
            Precision precision = static_cast<const ir::FloatingType *>(&*I.value()->type())->precision();
            std::shared_ptr<Register> dst = share(*riscvFloatingResultRegs()[0]),
                                      src = prepareRegister(*I.value());
            builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
        } else {
            assert(*I.value()->type() == ir::Void());
        }
        builder_.add(std::make_unique<Br>(epilogueBlock_));
    }

    void visitPhi(const ir::Phi &) override {}

private:
    Function &function_;
    const std::unordered_map<const ir::GlobalVar *, GlobalVar *> &globalVarMap_;
    const std::unordered_map<const ir::Function *, Function *> &functionMap_;
    const std::unordered_map<const ir::BasicBlock *, BasicBlock *> &blockMap_;
    const std::unordered_map<const ir::Alloca *, StackSlot *> &memoryMap_;
    const std::unordered_map<const ir::Value *, std::shared_ptr<Register>> &valueMap_;
    BasicBlock *epilogueBlock_;
    BasicBlockBuilder builder_;

    template <typename IInstr, typename MInstr>
    void visitBinaryIntegerArithmeticOperator(const IInstr &I) {
        int width = I.opType()->size(8);
        ExtensionMode extensionMode = width == 8 ? ExtensionMode::kNo : ExtensionMode::kSign;
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src1 = prepareRegister(*I.lhs()),
                                  src2 = prepareRegister(*I.rhs());
        if (width == 4) {
            builder_.add(std::make_unique<MInstr>(4, dst, src1, src2, extensionMode));
        } else {
            builder_.add(std::make_unique<MInstr>(8, dst, src1, src2, extensionMode));
            if (width < 8) {
                builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - width * 8)));
                builder_.add(std::make_unique<SHRAI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - width * 8)));
            }
        }
    }

    template <typename IInstr, typename MInstr>
    void visitBinaryIntegerBitwiseOperator(const IInstr &I) {
        int width = 8;
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src1 = prepareRegister(*I.lhs()),
                                  src2 = prepareRegister(*I.rhs());
        builder_.add(std::make_unique<MInstr>(width, std::move(dst), std::move(src1), std::move(src2)));
    }

    template <typename IInstr, typename MInstr>
    void visitUnaryFloatingArithmeticOperator(const IInstr &I) {
        Precision precision = static_cast<const ir::FloatingType *>(&*I.value()->type())->precision();
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src = prepareRegister(*I.value());
        builder_.add(std::make_unique<MInstr>(precision, std::move(dst), std::move(src)));
    }

    template <typename IInstr, typename MInstr>
    void visitBinaryFloatingArithmeticOperator(const IInstr &I) {
        Precision precision = static_cast<const ir::FloatingType *>(&*I.opType())->precision();
        std::shared_ptr<Register> dst = valueMap_.at(&I),
                                  src1 = prepareRegister(*I.lhs()),
                                  src2 = prepareRegister(*I.rhs());
        builder_.add(std::make_unique<MInstr>(precision, std::move(dst), std::move(src1), std::move(src2)));
    }

    std::shared_ptr<Register> prepareRegister(const ir::Value &value) {
        if (dynamic_cast<const ir::Argument *>(&value) || dynamic_cast<const ir::Instruction *>(&value)) {
            return valueMap_.at(&value);
        }
        if (auto *G = dynamic_cast<const ir::GlobalVar *>(&value)) {
            std::shared_ptr<Register> reg = std::make_shared<VirtualRegister>();
            GlobalVar *ptr = globalVarMap_.at(G);
            builder_.add(std::make_unique<LA>(8, reg, ptr));
            return reg;
        }
        if (auto *C = dynamic_cast<const ir::IntegerConstant *>(&value)) {
            std::shared_ptr<Register> reg = std::make_shared<VirtualRegister>();
            std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(C->signExtendedValue());
            builder_.add(std::make_unique<LI>(8, reg, std::move(imm)));
            return reg;
        }
        if (auto *C = dynamic_cast<const ir::FloatingConstant *>(&value)) {
            std::shared_ptr<Register> reg1 = std::make_shared<VirtualRegister>(),
                                      reg2 = std::make_shared<VirtualRegister>();
            std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(C->bitPattern());
            builder_.add(std::make_unique<LI>(8, reg1, std::move(imm)));
            Precision precision = static_cast<const ir::FloatingType *>(&*C->type())->precision();
            builder_.add(std::make_unique<FMovFI>(precision, reg2, reg1));
            return reg2;
        }
        if (dynamic_cast<const ir::NullPtrConstant *>(&value)) {
            std::shared_ptr<Register> reg = std::make_shared<VirtualRegister>();
            std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(0);
            builder_.add(std::make_unique<LI>(8, reg, std::move(imm)));
            return reg;
        }
        std::unreachable();
    }
};

} // namespace

void RISCVMIRGen::emit() {
    for (const ir::GlobalVar &IG : IM_->globalVars) {
        GlobalVar &MG = MM_->globalVars.append(std::make_unique<GlobalVar>(IG.name(), IG.linkage()));
        globalVarMap_[&IG] = &MG;
    }
    for (const ir::Function &IF : IM_->functions) {
        Function &MF = MM_->functions.append(std::make_unique<Function>(IF.name(), IF.linkage()));
        functionMap_[&IF] = &MF;
    }
    for (auto &[IG, MG] : globalVarMap_) {
        if (IG->hasInitializer()) {
            emitGlobalVar(*IG, *MG);
        }
    }
    for (auto &[IF, MF] : functionMap_) {
        if (!IF->empty()) {
            emitFunction(*IF, *MF);
        }
    }
}

void RISCVMIRGen::emitGlobalVar(const ir::GlobalVar &IG, GlobalVar &MG) {
    ConstantVisitorImpl visitor(MG, globalVarMap_);
    IG.initializer().accept(visitor);
}

void RISCVMIRGen::emitFunction(const ir::Function &IF, Function &MF) {
    BasicBlock &prologueBlock = MF.append();

    std::unordered_map<const ir::BasicBlock *, BasicBlock *> blockMap;
    for (const ir::BasicBlock &IB : IF) {
        blockMap[&IB] = &MF.append();
    }

    BasicBlock &epilogueBlock = MF.append();

    StackSlot &startSlot = MF.stackFrame().append(0, 16);
    StackSlot &raSlot = MF.stackFrame().append(8, 8);
    StackSlot &fpSlot = MF.stackFrame().append(8, 8);

    std::unordered_map<const ir::Alloca *, StackSlot *> memoryMap;
    for (const ir::BasicBlock &IB : IF) {
        for (const ir::Instruction &II : IB) {
            if (auto *alloca = dynamic_cast<const ir::Alloca *>(&II)) {
                int size = alloca->allocatedType()->size(8);
                int alignment = alloca->allocatedType()->alignment(8);
                memoryMap[alloca] = &MF.stackFrame().append(size, alignment);
            }
        }
    }

    StackSlot &endSlot = MF.stackFrame().append(0, 16);

    prologueBlock.append(std::make_unique<AddI>(8,
        share(*sp()), share(*sp()), std::make_unique<StackRelativeOffsetImmediate>(&endSlot, &startSlot)));

    prologueBlock.append(std::make_unique<Store>(8,
        MemoryOperand(share(*sp()), std::make_unique<StackRelativeOffsetImmediate>(&startSlot, &raSlot)), share(*ra())));

    prologueBlock.append(std::make_unique<Store>(8,
        MemoryOperand(share(*sp()), std::make_unique<StackRelativeOffsetImmediate>(&startSlot, &fpSlot)), share(*fp())));

    prologueBlock.append(std::make_unique<AddI>(8,
        share(*fp()), share(*sp()), std::make_unique<StackRelativeOffsetImmediate>(&startSlot, &endSlot)));

    prologueBlock.append(std::make_unique<Br>(blockMap[&IF.entry()]));

    epilogueBlock.append(std::make_unique<Load>(8,
        share(*ra()), MemoryOperand(share(*sp()), std::make_unique<StackRelativeOffsetImmediate>(&startSlot, &raSlot))));

    epilogueBlock.append(std::make_unique<Load>(8,
        share(*fp()), MemoryOperand(share(*sp()), std::make_unique<StackRelativeOffsetImmediate>(&startSlot, &fpSlot))));

    epilogueBlock.append(std::make_unique<AddI>(8,
        share(*sp()), share(*sp()), std::make_unique<StackRelativeOffsetImmediate>(&startSlot, &endSlot)));

    int numIntegerResults = 0,
        numFloatingResults = 0;
    std::unique_ptr<ir::Type> returnType = IF.functionType()->returnType();
    if (dynamic_cast<const ir::IntegerType *>(&*returnType)) {
        ++numIntegerResults;
    } else if (dynamic_cast<const ir::FloatingType *>(&*returnType)) {
        ++numFloatingResults;
    } else {
        assert(*returnType == ir::Void());
    }
    epilogueBlock.append(std::make_unique<RISCVRet>(numIntegerResults, numFloatingResults));

    std::unordered_map<const ir::Value *, std::shared_ptr<Register>> valueMap;
    size_t numIntegerArgs = 0,
           numFloatingArgs = 0,
           numStackArgs = 0;
    for (const ir::Argument &arg : args(IF)) {
        if (dynamic_cast<const ir::IntegerType *>(&*arg.type())) {
            if (numIntegerArgs < 8) {
                std::shared_ptr<Register> dst = std::make_shared<VirtualRegister>(),
                                          src = share(*riscvIntegerArgRegs()[numIntegerArgs]);
                blockMap[&IF.entry()]->append(std::make_unique<Mov>(8, dst, src));
                valueMap[&arg] = dst;
                ++numIntegerArgs;
            } else {
                std::shared_ptr<Register> dst = std::make_shared<VirtualRegister>();
                MemoryOperand src(share(*fp()), std::make_unique<IntegerImmediate>(numStackArgs * 8));
                blockMap[&IF.entry()]->append(std::make_unique<Load>(8, dst, std::move(src)));
                valueMap[&arg] = dst;
                ++numStackArgs;
            }
        } else if (dynamic_cast<const ir::FloatingType *>(&*arg.type())) {
            Precision precision = static_cast<const ir::FloatingType *>(&*arg.type())->precision();
            if (numFloatingArgs < 8) {
                std::shared_ptr<Register> dst = std::make_shared<VirtualRegister>(),
                                          src = share(*riscvFloatingArgRegs()[numFloatingArgs]);
                blockMap[&IF.entry()]->append(std::make_unique<FMov>(precision, dst, src));
                valueMap[&arg] = dst;
                ++numFloatingArgs;
            } else {
                std::shared_ptr<Register> dst = std::make_shared<VirtualRegister>();
                MemoryOperand src(share(*fp()), std::make_unique<IntegerImmediate>(numStackArgs * 8));
                blockMap[&IF.entry()]->append(std::make_unique<FLoad>(precision, dst, std::move(src)));
                valueMap[&arg] = dst;
                ++numStackArgs;
            }
        } else {
            std::unreachable();
        }
    }
    for (const ir::BasicBlock &IB : IF) {
        for (const ir::Instruction &II : IB) {
            if (*II.type() != ir::Void()) {
                valueMap[&II] = std::make_shared<VirtualRegister>();
            }
        }
    }

    InstructionVisitorImpl visitor(MF, globalVarMap_, functionMap_, blockMap, memoryMap, valueMap, &epilogueBlock);
    for (const ir::BasicBlock &IB : IF) {
        BasicBlock *MB = blockMap[&IB];
        visitor.builder().setPos(MB);
        for (const ir::Instruction &II : IB) {
            II.accept(visitor);
        }
    }
}
