cmake_minimum_required(VERSION 3.20)

project(
    OrderedVectorLib
        VERSION 1.0.0
        LANGUAGES CXX
)

add_library(
    OrderedVectorLib 
        INTERFACE
)

target_include_directories(
    OrderedVectorLib
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
)

target_compile_features(
    OrderedVectorLib
        INTERFACE
            cxx_std_17   
)

target_sources(
    OrderedVectorLib
        INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/include/ordered_vector/ordered_vector.h
)

add_subdirectory(examples)
add_subdirectory(tests)
