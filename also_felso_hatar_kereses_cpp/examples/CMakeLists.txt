add_executable(
    OrderedVectorExample
        example.cpp
)

target_link_libraries(
    OrderedVectorExample
        PRIVATE
            OrderedVectorLib
)