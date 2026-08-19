#include <ordered_vector/ordered_vector.h>

#include <gtest/gtest.h>

struct TestCase
{
    std::vector<int> array;
    int value;
    size_t expectedLowerBoundIndex;
    size_t expectedUpperBoundIndex;
};

class OrderedVectorTest : public testing::TestWithParam<TestCase>
{
};

TEST_P(OrderedVectorTest, LowerAndUpperBound)
{
    auto params = GetParam();
    EXPECT_EQ(ordered_vector::LowerBound(params.array, params.value), params.expectedLowerBoundIndex);
    EXPECT_EQ(ordered_vector::UpperBound(params.array, params.value), params.expectedUpperBoundIndex);
}

INSTANTIATE_TEST_SUITE_P(
    OrderedVectorTests,
    OrderedVectorTest,
    testing::Values(
        TestCase{ {}, 42, 0, 0 }, // empty array
        TestCase{ { 42 }, 42, 0, 1 }, // one element array with matching
        TestCase{ { 40 }, 42, 1, 1 }, // one element array with smaller item than value
        TestCase{ { 45 }, 42, 0, 0 }, // one element array with bigger item than value
        TestCase{ { 42, 42, 42, 42 }, 42, 0, 4 }, // array with all searched values
        TestCase{ { 41, 42, 43, 44 }, 42, 1, 2 }, // array with only one searched values
        TestCase{ { 41, 42, 42, 44 }, 42, 1, 3 }, // array with more than one searched values
        TestCase{ { 41, 43, 44, 45 }, 42, 1, 1 }, // array without the searched values
        TestCase{ { 41, 43, 44, 45 }, 46, 4, 4 }, // array when all items are smaller than value
        TestCase{ { 41, 43, 44, 45 }, 40, 0, 0 } // array when all items are bigger than value
    )
);