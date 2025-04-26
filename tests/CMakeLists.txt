enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES "*_test.cpp")

file(COPY config DESTINATION ${CMAKE_BINARY_DIR}/tests)

set(ALL_TEST_TARGETS)

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME}
        PRIVATE
            gtest_main
            lib::core
    )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    list(APPEND ALL_TEST_TARGETS ${TEST_NAME})
endforeach()

# Add a meta target to build all tests
add_custom_target(all_tests DEPENDS ${ALL_TEST_TARGETS})
