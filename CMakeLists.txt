# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the project name and specify the C++ standard
project(Compiler VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define the source files
set(SOURCES
    src/main.cpp
    src/Compiler.cpp
    src/TableDrivenLexer.cpp
    src/Token.cpp
)

# Add the executable target
add_executable(Compiler ${SOURCES})

# Include header files directory (so the compiler knows where to find them)
target_include_directories(Compiler PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Set output directories (optional but recommended)
set_target_properties(Compiler PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# Copy the tests directory to the appropriate output directory (Debug/Release)
file(COPY ${CMAKE_SOURCE_DIR}/tests DESTINATION ${CMAKE_BINARY_DIR}/bin)

# Optionally set default build type (for non-IDE environments)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()
