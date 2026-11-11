cmake_minimum_required(VERSION 3.20)

project(Forge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(tensor STATIC src/tensor.cpp)

target_include_directories(tensor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(test_tensor tests/test_tensor.cpp)

target_link_libraries(test_tensor PRIVATE tensor)

enable_testing()

add_test(NAME tensor_tests COMMAND test_tensor)

target_compile_options(tensor PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
)

target_compile_options(test_tensor PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
)