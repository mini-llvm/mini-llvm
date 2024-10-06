add_executable(xllc
    main.cpp
)
target_compile_features(xllc PRIVATE cxx_std_23)
target_link_libraries(xllc PRIVATE mini-llvm)
