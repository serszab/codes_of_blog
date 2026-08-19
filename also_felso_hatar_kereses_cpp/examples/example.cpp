#include <ordered_vector/ordered_vector.h>

#include <iostream>

int main()
{
    const std::vector<int> array{ 2, 4, 6, 6, 7, 9 };
    const int value = 6;

    std::cout << "Number of " << value << " in the array is " << ordered_vector::UpperBound(array, value) - ordered_vector::LowerBound(array, value) << std::endl;
}