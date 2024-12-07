cmake_minimum_required(VERSION 3.15)

# If Conan toolchain is used, include it
if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
    include("${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
endif()

project(Compiler VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")



# Access the LEX executable from the environment variable
if(DEFINED ENV{LEX})
    set(FLEX_EXECUTABLE $ENV{LEX})
else()
    message(FATAL_ERROR "FLEX_EXECUTABLE not found. Please ensure winflexbison is installed correctly.")
endif()

message(STATUS "Using Flex executable: ${FLEX_EXECUTABLE}")


if(WIN32)
    # Use the path from the LEX environment variable to deduce the include directory
    get_filename_component(FLEX_INCLUDE_DIR $ENV{LEX} DIRECTORY)
    set(FLEX_INCLUDE_DIR "${FLEX_INCLUDE_DIR}/../include")
else()
    find_package(FLEX REQUIRED)
    set(FLEX_INCLUDE_DIR ${FLEX_INCLUDE_DIRS})
endif()

# Include the Flex headers dynamically (no hardcoding)
include_directories(${FLEX_INCLUDE_DIR})

set(FLEX_SOURCE ${CMAKE_SOURCE_DIR}/src/scanner.l)
set(FLEX_OUTPUT ${CMAKE_BINARY_DIR}/scanner.cpp)



add_custom_command(
    OUTPUT ${FLEX_OUTPUT}
    COMMAND ${FLEX_EXECUTABLE} -DYY_NO_UNISTD_H --outfile=${FLEX_OUTPUT} ${FLEX_SOURCE}
    MAIN_DEPENDENCY ${FLEX_SOURCE}
    COMMENT "Running Flex on ${FLEX_SOURCE}"
)


# Add the generated lexer file to the sources
set(SOURCES
    src/main.cpp
    src/Compiler.cpp
    src/Token.cpp
    src/TokenTable.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/FileReader.cpp
    src/Symbol.cpp
    src/SymbolTable.cpp
    src/ErrorManager.cpp
    ${FLEX_OUTPUT}
)

add_executable(Compiler ${SOURCES})

target_include_directories(Compiler PUBLIC ${CMAKE_SOURCE_DIR}/include)
include_directories(${FLEX_INCLUDE_DIRS})

set_target_properties(Compiler PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

add_custom_command(
    TARGET Compiler
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/example/test.cmm
        ${CMAKE_BINARY_DIR}/bin/test.cmm
    COMMENT "Copying test.cmm to the build directory"
)


if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

enable_testing()

# Find Google Test (provided by Conan)
find_package(GTest REQUIRED)

# Function to add a compiler test
function(add_compiler_test target_name test_file)
    add_executable(${target_name} ${test_file})
    target_include_directories(${target_name} PUBLIC ${CMAKE_SOURCE_DIR}/include)
    target_sources(${target_name} PRIVATE
        src/Token.cpp
        src/TokenTable.cpp
        src/Compiler.cpp
        src/Lexer.cpp
        src/Parser.cpp
        src/FileReader.cpp
        src/Symbol.cpp
        src/SymbolTable.cpp
        src/ErrorManager.cpp
        ${FLEX_OUTPUT}
    )
    target_link_libraries(${target_name} GTest::gtest_main)
    add_test(NAME ${target_name} COMMAND ${target_name})
endfunction()

# Add tests
add_compiler_test(parser_test tests/parser_test.cpp)
add_compiler_test(lexer_test tests/lexer_test.cpp)
add_compiler_test(semantic_test tests/semantic_test.cpp)

add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMENT "Running all tests"
)

add_custom_target(run
    COMMAND $<TARGET_FILE:Compiler>
    DEPENDS Compiler
    WORKING_DIRECTORY $<TARGET_FILE_DIR:Compiler>
    COMMENT "Running the Compiler executable"
)
