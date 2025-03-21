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

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME}
        PRIVATE
            gtest_main
            core
    )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
