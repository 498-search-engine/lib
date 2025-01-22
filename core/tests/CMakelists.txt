# Fetch Google Tests
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Find all test files
file(GLOB TEST_SOURCES "*_test.cpp")


# 1. Create a <name>_test executable for each found test file
foreach(TEST_SOURCE ${TEST_SOURCES})
    # Extract the filename without extension
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    # Create the test executable
    add_executable(${TEST_NAME} ${TEST_SOURCE})

    # Link against gtest and your core library
    target_link_libraries(${TEST_NAME}
        PRIVATE
            gtest_main  # from googletest
            core        # link our library
    )

    # Register the test
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()


# 2. Create a single test executable containing all tests
add_executable(core_tests ${TEST_SOURCES})

# Link against gtest and your core library
target_link_libraries(core_tests
    PRIVATE
        gtest_main  # from googletest
        core        # link our library
)

# Register the combined test executable
add_test(NAME core_tests COMMAND core_tests)

# Add a custom target that executes the test command (i.e. `make run_tests`)
add_custom_target(run_tests
    COMMAND core_tests
    DEPENDS core_tests
)
