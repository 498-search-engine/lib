cmake_minimum_required(VERSION 3.30)

add_library(core INTERFACE)

# Set include directories
target_include_directories(core
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Set C++ standard for consuming targets
target_compile_features(core INTERFACE cxx_std_17)

# Add strict warning flags
if(MSVC)
    target_compile_options(core INTERFACE /W4)
else()
    target_compile_options(core INTERFACE -Wall -Wextra -Wpedantic)
endif()

# Testing
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# Installation rules
include(GNUInstallDirs)
install(TARGETS core
    EXPORT core-targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/core
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export targets
install(EXPORT core-targets
    FILE core-targets.cmake
    NAMESPACE core::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/core
)

# Generate and install config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "core-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/core-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/core-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/core
)