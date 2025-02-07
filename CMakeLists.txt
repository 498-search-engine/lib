cmake_minimum_required(VERSION 3.20)
project(lib VERSION 1.0
        DESCRIPTION "Search engine core library"
        LANGUAGES CXX)

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
)

target_link_libraries(core
    PUBLIC
        OpenSSL::Crypto
        OpenSSL::SSL
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