#pragma once

#include <vector>

namespace ordered_vector::impl
{

template<typename T>
size_t BinarySearch(const std::vector<T>& array, const T& value)
{
    int leftIndex = 0;
    int rightIndex = array.size() - 1;
    auto middleIndex = (leftIndex + rightIndex) / 2;

    while (leftIndex <= rightIndex && array[middleIndex] != value) {
        if (value < array[middleIndex]) {
            rightIndex = middleIndex - 1;
        } 
        else { 
            leftIndex = middleIndex + 1;
        }
        middleIndex = (leftIndex + rightIndex) / 2;
    }

    return (leftIndex > rightIndex) ? array.size() : static_cast<size_t>(middleIndex);
}

template<typename T>
size_t LowerBound(const std::vector<T>& array, const T value)
{
    int leftIndex = 0;
    int rightIndex = array.size() - 1;
    auto middleIndex = (leftIndex + rightIndex) / 2;
    size_t result = array.size();

    while (leftIndex <= rightIndex) {
        if (value <= array[middleIndex]) {
            rightIndex = middleIndex - 1;
            result = static_cast<size_t>(middleIndex);
        }
        else {
            leftIndex = middleIndex + 1;
        }
        middleIndex = (leftIndex + rightIndex) / 2;
    }

    return result;
}

template<typename T>
size_t UpperBound(const std::vector<T>& array, const T value)
{
    int leftIndex = 0;
    int rightIndex = array.size() - 1;
    auto middleIndex = (leftIndex + rightIndex) / 2;
    size_t result = array.size();

    while (leftIndex <= rightIndex) {
        if (value < array[middleIndex]) {
            rightIndex = middleIndex - 1;
            result = static_cast<size_t>(middleIndex);
        }
        else {
            leftIndex = middleIndex + 1;
        }
        middleIndex = (leftIndex + rightIndex) / 2;
    }

    return result;
}

}