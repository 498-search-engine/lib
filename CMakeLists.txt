cmake_minimum_required(VERSION 3.20)
project(lib VERSION 1.0
        DESCRIPTION "Search engine core library"
        LANGUAGES CXX)

# Check if we're not using Apple Clang on macOS
# since it is not well supported on Apple Clang and causes issues.
execute_process(COMMAND ${CMAKE_CXX_COMPILER} --version OUTPUT_VARIABLE COMPILER_VERSION)
if(NOT (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND COMPILER_VERSION MATCHES "Apple" AND CMAKE_SYSTEM_NAME MATCHES "Darwin"))
    find_package(OpenMP REQUIRED)
    if(OpenMP_CXX_FOUND)
        message(STATUS "Found OpenMP: ${OpenMP_CXX_FLAGS}")
    endif()
else()
    # Define an empty INTERFACE target (no actual linking)
    if(NOT TARGET OpenMP::OpenMP_CXX)
        add_library(OpenMP::OpenMP_CXX INTERFACE IMPORTED)
    endif()
    
    message(STATUS "Skipping OpenMP for AppleClang on macOS")
endif()
find_package(OpenSSL REQUIRED)

# Create main library target
add_library(core
    src/core/optional.cpp
    # Add other source files
)

# Create alias target
add_library(lib::core ALIAS core)

# Set up include paths
target_include_directories(core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        # template internals
        ${CMAKE_CURRENT_SOURCE_DIR}/detail
)

# Only apply `-fopenmp` on non-Apple Clang systems
if(NOT (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND COMPILER_VERSION MATCHES "Apple" AND CMAKE_SYSTEM_NAME MATCHES "Darwin"))
    target_compile_options(core PRIVATE -fopenmp)
else()
    message(STATUS "Skipping -fopenmp for Apple Clang (unsupported)")
endif()

target_link_libraries(core
    PUBLIC
        OpenSSL::Crypto
        OpenSSL::SSL
        OpenMP::OpenMP_CXX
)

# Set up compile features
target_compile_features(core PUBLIC cxx_std_20)

# Optional: Set up installation
if(NOT SKIP_INSTALL)
    include(GNUInstallDirs)
    install(TARGETS core
        EXPORT lib-core-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

# Add tests if building standalone
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    enable_testing()
    add_subdirectory(tests)
endif()