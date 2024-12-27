add_library(mini-llvm
    codegen/register_allocator/LinearScanAllocator.cpp
    codegen/register_allocator/NaiveAllocator.cpp
    ir/BasicBlock.cpp
    ir/Constant/ArrayConstant.cpp
    ir/Constant/FloatingConstant.cpp
    ir/Constant/IntegerConstant.cpp
    ir/Function.cpp
    ir/GlobalVar.cpp
    ir/Instruction.cpp
    ir/Instruction/BinaryFloatingArithmeticOperators.cpp
    ir/Instruction/BinaryFloatingRelationalOperators.cpp
    ir/Instruction/BinaryIntegerArithmeticOperators.cpp
    ir/Instruction/BinaryIntegerRelationalOperators.cpp
    ir/Instruction/BitCast.cpp
    ir/Instruction/Call.cpp
    ir/Instruction/FloatingCastingOperators.cpp
    ir/Instruction/FloatingToIntegerCastingOperators.cpp
    ir/Instruction/GetElementPtr.cpp
    ir/Instruction/IndirectCall.cpp
    ir/Instruction/IntegerCastingOperators.cpp
    ir/Instruction/IntegerToFloatingCastingOperators.cpp
    ir/Instruction/Phi.cpp
    ir/Instruction/Select.cpp
    ir/Instruction/UnaryFloatingArithmeticOperators.cpp
    ir/Module.cpp
    ir/Type/ArrayType.cpp
    ir/Type/Double.cpp
    ir/Type/Float.cpp
    ir/Type/FunctionType.cpp
    ir/Type/I1.cpp
    ir/Type/I16.cpp
    ir/Type/I32.cpp
    ir/Type/I64.cpp
    ir/Type/I8.cpp
    ir/Type/Ptr.cpp
    ir/Type/Void.cpp
    ir/Use.cpp
    ir/Value.cpp
    ir_parser/IRParser.cpp
    ir_parser/Lexer.cpp
    ir_parser/Parser.cpp
    mc/Fragment.cpp
    mc/Instruction.cpp
    mc/Program.cpp
    mir/BasicBlock.cpp
    mir/Constant/I16ArrayConstant.cpp
    mir/Constant/I32ArrayConstant.cpp
    mir/Constant/I64ArrayConstant.cpp
    mir/Constant/I8ArrayConstant.cpp
    mir/Function.cpp
    mir/GlobalVar.cpp
    mir/Instruction.cpp
    mir/Instruction/Terminator.cpp
    mir/Module.cpp
    mir/StackFrame.cpp
    mir/StackSlot.cpp
    opt/ir/FunctionTransform.cpp
    opt/ir/passes/AlgebraicSimplification.cpp
    opt/ir/passes/BasicBlockMerging.cpp
    opt/ir/passes/BranchSimplification.cpp
    opt/ir/passes/ConstantFolding.cpp
    opt/ir/passes/DeadCodeElimination.cpp
    opt/ir/passes/DominatorTreeAnalysis.cpp
    opt/ir/passes/FunctionInlining.cpp
    opt/ir/passes/JumpThreading.cpp
    opt/ir/passes/Mem2Reg.cpp
    opt/ir/passes/PoisonPropagation.cpp
    opt/ir/passes/UnreachableBlockElimination.cpp
    opt/ir/passes/VerificationAnalysis.cpp
    opt/ir/PassManager.cpp
    opt/mc/FragmentTransform.cpp
    opt/mc/passes/RedundantLabelElimination.cpp
    opt/mir/BasicBlockTransform.cpp
    opt/mir/FunctionTransform.cpp
    opt/mir/passes/ConstantPropagation.cpp
    opt/mir/passes/CopyPropagation.cpp
    opt/mir/passes/DeadCodeElimination.cpp
    opt/mir/passes/LiveVariableAnalysis.cpp
    opt/mir/passes/NullOperationElimination.cpp
    opt/mir/passes/RegisterReuse.cpp
    opt/mir/passes/ZeroRegisterReplacement.cpp
    targets/riscv/codegen/RISCVMCGen.cpp
    targets/riscv/codegen/RISCVMIRGen.cpp
    targets/riscv/mir/Instruction/RISCVCall.cpp
    targets/riscv/mir/Instruction/RISCVRet.cpp
    targets/riscv/mir/RISCVRegister.cpp
    targets/riscv/opt/mc/passes/RISCVFallthrough.cpp
    targets/riscv/opt/mc/RISCVPassManager.cpp
    targets/riscv/opt/mir/RISCVPassManager.cpp
    targets/riscv/RISCVBackendDriver.cpp
    utils/FileSystem.cpp
    utils/Strings.cpp
)
target_compile_features(mini-llvm PUBLIC cxx_std_23)
target_include_directories(mini-llvm PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
add_library(mini-llvm::mini-llvm ALIAS mini-llvm)

install(TARGETS mini-llvm EXPORT mini-llvm-targets)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION include)
install(EXPORT mini-llvm-targets
        FILE mini-llvm-targets.cmake
        NAMESPACE mini-llvm::
        DESTINATION lib/cmake/mini-llvm)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/mini-llvm-config.cmake.in"
    "${PROJECT_BINARY_DIR}/mini-llvm-config.cmake"
    INSTALL_DESTINATION lib/cmake/mini-llvm
)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/mini-llvm-config-version.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY AnyNewerVersion
)
install(FILES
    "${PROJECT_BINARY_DIR}/mini-llvm-config.cmake"
    "${PROJECT_BINARY_DIR}/mini-llvm-config-version.cmake"
    DESTINATION lib/cmake/mini-llvm
)

export(EXPORT mini-llvm-targets
       FILE "${PROJECT_BINARY_DIR}/mini-llvm-targets.cmake"
       NAMESPACE mini-llvm::)
