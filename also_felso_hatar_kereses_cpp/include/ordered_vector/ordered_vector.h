#pragma once

#include <ordered_vector/implementation/ordered_vector_impl.h>

namespace ordered_vector
{

template<typename T>
size_t LowerBound(const std::vector<T>& array, const T& value)
{
    return impl::LowerBound(array, value);
}

template<typename T>
size_t UpperBound(const std::vector<T>& array, const T& value)
{
    return impl::UpperBound(array, value);
}

}