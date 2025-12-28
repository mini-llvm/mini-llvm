add_executable(mini-llc
    main.cpp
)
target_link_libraries(mini-llc PRIVATE mini-llvm)

install(TARGETS mini-llc)
