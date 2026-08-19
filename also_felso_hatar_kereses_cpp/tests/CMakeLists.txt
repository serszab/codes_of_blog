include(FetchContent)

FetchContent_Declare(
    GTest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.18.0
)

FetchContent_MakeAvailable(GTest)

add_executable(
    OrderedVectorTests
        test_ordered_vector.cpp
)

target_link_libraries(
    OrderedVectorTests
        PRIVATE
            OrderedVectorLib
            GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(OrderedVectorTests)
