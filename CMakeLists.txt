cmake_minimum_required(VERSION 3.20...4.1.2)

project(mini-llvm VERSION 0.1.0 LANGUAGES CXX)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    message(WARNING "CMAKE_BUILD_TYPE is not set, defaulting to Release.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic)
        add_compile_definitions("$<$<CONFIG:Debug>:_GLIBCXX_ASSERTIONS>")
    endif()
endif()

option(BUILD_SHARED_LIBS "Build shared libraries." OFF)

option(MINI_LLVM_TESTS "Build tests." OFF)
option(MINI_LLVM_FUZZ "Build fuzz targets." OFF)
option(MINI_LLVM_DOCS "Build docs." OFF)

add_library(mini-llvm
    lib/codegen/register_allocators/GraphColoringAllocator.cpp
    lib/codegen/register_allocators/LinearScanAllocator.cpp
    lib/codegen/register_allocators/NaiveAllocator.cpp
    lib/common/SourceManager.cpp
    lib/ir/BasicBlock.cpp
    lib/ir/Constant/ArrayConstant.cpp
    lib/ir/Function.cpp
    lib/ir/GlobalVar.cpp
    lib/ir/Instruction.cpp
    lib/ir/Instruction/Alloca.cpp
    lib/ir/Instruction/BinaryFloatingArithmeticOperators.cpp
    lib/ir/Instruction/BinaryFloatingOperator.cpp
    lib/ir/Instruction/BinaryFloatingRelationalOperators.cpp
    lib/ir/Instruction/BinaryIntegerArithmeticOperator.cpp
    lib/ir/Instruction/BinaryIntegerArithmeticOperators.cpp
    lib/ir/Instruction/BinaryIntegerOperator.cpp
    lib/ir/Instruction/BinaryIntegerRelationalOperators.cpp
    lib/ir/Instruction/BitCast.cpp
    lib/ir/Instruction/Br.cpp
    lib/ir/Instruction/Call.cpp
    lib/ir/Instruction/CondBr.cpp
    lib/ir/Instruction/FloatingCastingOperator.cpp
    lib/ir/Instruction/FloatingCastingOperators.cpp
    lib/ir/Instruction/FloatingToIntegerCastingOperator.cpp
    lib/ir/Instruction/FloatingToIntegerCastingOperators.cpp
    lib/ir/Instruction/GetElementPtr.cpp
    lib/ir/Instruction/IndirectCall.cpp
    lib/ir/Instruction/IntegerCastingOperator.cpp
    lib/ir/Instruction/IntegerCastingOperators.cpp
    lib/ir/Instruction/IntegerToFloatingCastingOperator.cpp
    lib/ir/Instruction/IntegerToFloatingCastingOperators.cpp
    lib/ir/Instruction/IntToPtr.cpp
    lib/ir/Instruction/Load.cpp
    lib/ir/Instruction/Phi.cpp
    lib/ir/Instruction/PtrToInt.cpp
    lib/ir/Instruction/Ret.cpp
    lib/ir/Instruction/Select.cpp
    lib/ir/Instruction/Store.cpp
    lib/ir/Instruction/UnaryFloatingArithmeticOperators.cpp
    lib/ir/Instruction/UnaryFloatingOperator.cpp
    lib/ir/Module.cpp
    lib/ir/Type/ArrayType.cpp
    lib/ir/Type/Double.cpp
    lib/ir/Type/Float.cpp
    lib/ir/Type/FunctionType.cpp
    lib/ir/Type/I1.cpp
    lib/ir/Type/I16.cpp
    lib/ir/Type/I32.cpp
    lib/ir/Type/I64.cpp
    lib/ir/Type/I8.cpp
    lib/ir/Type/Ptr.cpp
    lib/ir/Type/Void.cpp
    lib/ir/Use.cpp
    lib/ir/Value.cpp
    lib/ir_reader/IRReader.cpp
    lib/ir_reader/Lexer.cpp
    lib/ir_reader/Parser.cpp
    lib/mc/GlobalValue.cpp
    lib/mc/Module.cpp
    lib/mc/StringDirective.cpp
    lib/mc/Symbol.cpp
    lib/mir/BasicBlock.cpp
    lib/mir/Constant/I16ArrayConstant.cpp
    lib/mir/Constant/I32ArrayConstant.cpp
    lib/mir/Constant/I64ArrayConstant.cpp
    lib/mir/Constant/I8ArrayConstant.cpp
    lib/mir/Constant/PtrArrayConstant.cpp
    lib/mir/Constant/PtrConstant.cpp
    lib/mir/Function.cpp
    lib/mir/GlobalValue.cpp
    lib/mir/GlobalVar.cpp
    lib/mir/Instruction.cpp
    lib/mir/Instruction/BinaryOperator.cpp
    lib/mir/Instruction/BinaryOperatorI.cpp
    lib/mir/Instruction/Load.cpp
    lib/mir/Instruction/Terminator.cpp
    lib/mir/Instruction/UnaryOperator.cpp
    lib/mir/Module.cpp
    lib/mir/StackFrame.cpp
    lib/mir/StackSlot.cpp
    lib/mir/VirtualRegister.cpp
    lib/opt/ir/FunctionTransform.cpp
    lib/opt/ir/passes/AlgebraicSimplification.cpp
    lib/opt/ir/passes/AliasAnalysis.cpp
    lib/opt/ir/passes/ArrayFlattening.cpp
    lib/opt/ir/passes/AttributeDeduction.cpp
    lib/opt/ir/passes/BasicBlockMerging.cpp
    lib/opt/ir/passes/BranchSimplification.cpp
    lib/opt/ir/passes/ConstantFolding.cpp
    lib/opt/ir/passes/DeadCodeElimination.cpp
    lib/opt/ir/passes/DeadStoreElimination.cpp
    lib/opt/ir/passes/DominatorTreeAnalysis.cpp
    lib/opt/ir/passes/FunctionInlining.cpp
    lib/opt/ir/passes/GlobalCodeMotion.cpp
    lib/opt/ir/passes/GlobalDeadCodeElimination.cpp
    lib/opt/ir/passes/GlobalValueNumbering.cpp
    lib/opt/ir/passes/InstructionCombining.cpp
    lib/opt/ir/passes/JumpThreading.cpp
    lib/opt/ir/passes/LoopTreeAnalysis.cpp
    lib/opt/ir/passes/Mem2Reg.cpp
    lib/opt/ir/passes/PoisonPropagation.cpp
    lib/opt/ir/passes/RedundantLoadElimination.cpp
    lib/opt/ir/passes/StrengthReduction.cpp
    lib/opt/ir/passes/TailCallElimination.cpp
    lib/opt/ir/passes/TailDuplication.cpp
    lib/opt/ir/passes/UnreachableBlockElimination.cpp
    lib/opt/ir/PassManager.cpp
    lib/opt/mc/GlobalValueTransform.cpp
    lib/opt/mir/BasicBlockTransform.cpp
    lib/opt/mir/FunctionTransform.cpp
    lib/opt/mir/passes/BasicBlockMerging.cpp
    lib/opt/mir/passes/BasicBlockReordering.cpp
    lib/opt/mir/passes/BranchPredictionAnalysis.cpp
    lib/opt/mir/passes/CopyPropagation.cpp
    lib/opt/mir/passes/DeadCodeElimination.cpp
    lib/opt/mir/passes/JumpThreading.cpp
    lib/opt/mir/passes/LiveVariableAnalysis.cpp
    lib/opt/mir/passes/NullOperationElimination.cpp
    lib/opt/mir/passes/RegisterReuse.cpp
    lib/opt/mir/passes/StackOffsetEvaluation.cpp
    lib/opt/mir/passes/TailDuplication.cpp
    lib/opt/mir/passes/UnreachableBlockElimination.cpp
    lib/opt/mir/passes/ZeroRegisterReplacement.cpp
    lib/targets/riscv/codegen/RISCVMCGen.cpp
    lib/targets/riscv/codegen/RISCVMIRGen.cpp
    lib/targets/riscv/mc/RISCVAddress.cpp
    lib/targets/riscv/mc/RISCVDataDirective.cpp
    lib/targets/riscv/mc/RISCVInstruction.cpp
    lib/targets/riscv/mir/Instruction/RISCVCall.cpp
    lib/targets/riscv/mir/Instruction/RISCVJALR.cpp
    lib/targets/riscv/mir/Instruction/RISCVRet.cpp
    lib/targets/riscv/mir/RISCVRegister.cpp
    lib/targets/riscv/opt/mc/passes/RISCVFallthrough.cpp
    lib/targets/riscv/opt/mc/passes/RISCVUnusedLabelElimination.cpp
    lib/targets/riscv/opt/mc/RISCVPassManager.cpp
    lib/targets/riscv/opt/mir/passes/RISCVConstantPropagation.cpp
    lib/targets/riscv/opt/mir/RISCVPassManager.cpp
    lib/targets/riscv/RISCVBackendDriver.cpp
    lib/utils/BigInteger.cpp
    lib/utils/CommandLineParser.cpp
    lib/utils/Dot.cpp
    lib/utils/FileHandle.cpp
    lib/utils/IO.cpp
    lib/utils/Strings.cpp
    lib/utils/SystemError.cpp
    lib/utils/Unicode.cpp
    lib/utils/Windows.cpp
)
set_target_properties(mini-llvm PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)
if(BUILD_SHARED_LIBS)
    target_compile_definitions(mini-llvm PUBLIC MINI_LLVM_SHARED)
    target_compile_definitions(mini-llvm PRIVATE MINI_LLVM_EXPORTS)
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(mini-llvm PUBLIC /utf-8 /EHsc)
endif()
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

add_subdirectory(tools)

if(MINI_LLVM_TESTS)
    enable_testing()

    add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL SYSTEM)

    add_subdirectory(unittests)
endif()

if(MINI_LLVM_FUZZ)
    add_subdirectory(fuzz)
endif()

if(MINI_LLVM_DOCS)
    find_package(Doxygen REQUIRED)

    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_FILE_PATTERNS *.h *.cpp)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_EXCLUDE_SYMBOLS detail)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET "${PROJECT_SOURCE_DIR}/third_party/doxygen-awesome-css/doxygen-awesome.css")
    set(DOXYGEN_HTML_COLORSTYLE LIGHT)

    doxygen_add_docs(
        docs
        "${PROJECT_SOURCE_DIR}/include"
        ALL
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    )
endif()
