add_executable(mini-llc
    main.cpp
)
if(MSVC)
    target_compile_options(mini-llc PRIVATE /utf-8)
endif()
target_link_libraries(mini-llc PRIVATE mini-llvm)

install(TARGETS mini-llc)
