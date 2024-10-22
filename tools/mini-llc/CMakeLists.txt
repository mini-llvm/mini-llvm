add_executable(mini-llc
    main.cpp
)
target_compile_features(mini-llc PRIVATE cxx_std_23)
target_link_libraries(mini-llc PRIVATE mini-llvm)

install(TARGETS mini-llc)
