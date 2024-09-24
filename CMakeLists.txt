cmake_minimum_required(VERSION 3.15)

# If Conan toolchain is used, include it
if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
    include("${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
endif()

project(Compiler VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/main.cpp
    src/Compiler.cpp
    src/TableDrivenLexer.cpp
    src/Token.cpp
    src/Automata.cpp
    src/TokenBuffer.cpp
    src/TokenTable.cpp
)

add_executable(Compiler ${SOURCES})

target_include_directories(Compiler PUBLIC ${CMAKE_SOURCE_DIR}/include)

set_target_properties(Compiler PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)


file(COPY ${CMAKE_SOURCE_DIR}/example DESTINATION ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()


enable_testing()

# Find Google Test (provided by Conan)
set(gtest_DIR "${CMAKE_BINARY_DIR}/gtest")
find_package(GTest CONFIG REQUIRED)
get_target_property(GTEST_INCLUDE_DIRS GTest::gtest INTERFACE_INCLUDE_DIRECTORIES)
message("GTEST_INCLUDE_DIRS: ${GTEST_INCLUDE_DIRS}")


# Add the test executable
add_executable(lexer_test tests/lexer_test.cpp)

target_include_directories(lexer_test PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    $<TARGET_PROPERTY:GTest::gtest,INTERFACE_INCLUDE_DIRECTORIES>
)

# Link Google Test libraries to the lexer_test executable
target_link_libraries(lexer_test GTest::gtest_main)

target_sources(lexer_test PRIVATE
    src/TableDrivenLexer.cpp
    src/Token.cpp
    src/TokenBuffer.cpp
    src/Automata.cpp
    src/TokenTable.cpp
)

# Register the test with CTest
add_test(NAME LexerTest COMMAND lexer_test)

add_custom_target(run
    COMMAND $<TARGET_FILE:Compiler>
    DEPENDS Compiler
    WORKING_DIRECTORY $<TARGET_FILE_DIR:Compiler>
    COMMENT "Running the Compiler executable"
)