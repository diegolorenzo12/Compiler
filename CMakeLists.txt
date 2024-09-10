# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.15)

# Define the project name and specify the C++ standard
project(Compiler VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# If Conan toolchain is used, include it
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

# Define the source files
set(SOURCES
    src/main.cpp
    src/Compiler.cpp
    src/TableDrivenLexer.cpp
    src/Token.cpp
)

# Add the executable target for the compiler
add_executable(Compiler ${SOURCES})

# Include header files directory (so the compiler knows where to find them)
target_include_directories(Compiler PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Set output directories (optional but recommended)
set_target_properties(Compiler PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)


file(COPY ${CMAKE_SOURCE_DIR}/example DESTINATION ${CMAKE_BINARY_DIR}/bin)


if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Enable testing
enable_testing()

# Find Google Test (provided by Conan)
find_package(GTest REQUIRED)

# Add the test executable
add_executable(lexer_test tests/lexer_test.cpp)

# Include directories for lexer_test (same as Compiler)
target_include_directories(lexer_test PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Link Google Test libraries to the lexer_test executable
target_link_libraries(lexer_test GTest::gtest GTest::gtest_main)

target_sources(lexer_test PRIVATE
    src/TableDrivenLexer.cpp
    src/Token.cpp
)

# Register the test with CTest
add_test(NAME LexerTest COMMAND lexer_test)
