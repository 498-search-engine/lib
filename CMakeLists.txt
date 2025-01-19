cmake_minimum_required(VERSION 3.30)
project(lib VERSION 1.0
        DESCRIPTION "a search engine std lib"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Option to build tests (default ON, can turn off if desired)
option(BUILD_TESTS "Build tests" ON)

# Add the core library
add_subdirectory(core)

# If tests are enabled, set up the tests directory
if (BUILD_TESTS)
    enable_testing()  # Enable CTest
    add_subdirectory(core/tests)
endif()
