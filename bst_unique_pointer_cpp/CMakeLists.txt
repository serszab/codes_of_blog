cmake_minimum_required(VERSION 3.20)

project(
    BinarySearchTree
        VERSION 1.0.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

add_library(
    BSTLib 
        INTERFACE
)

target_include_directories(
    BSTLib
        INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_sources(
    BSTLib
        INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/bst.h
)

add_executable(
    BST
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(
    BST
        PRIVATE
            BSTLib
)

include(FetchContent)

FetchContent_Declare(
    GTest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.18.0
)

FetchContent_MakeAvailable(GTest)

add_executable(
    BSTTests
        tests/test_bst.cpp
)

target_link_libraries(
    BSTTests
        PRIVATE
            BSTLib
            GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(BSTTests)