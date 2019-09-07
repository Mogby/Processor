cmake_minimum_required(VERSION 3.13)
project(processor)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)
configure_file(examples/test_program.asm test_program.asm)
configure_file(examples/fib.asm fib.asm)
configure_file(examples/qsolver.asm qsolver.asm)

add_executable(processor main.cpp)
target_link_libraries(processor cpu)