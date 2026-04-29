#include <gtest/gtest.h>

#include <vector>
#include "sorting.h"

TEST(SortTest, BubbleSortBasic) {
    std::vector<int> v = {5, 3, 4, 1, 2};
    bubble_sort(v.begin(), v.end());
    EXPECT_EQ(v, std::vector<int>({1,2,3,4,5}));
}

TEST(SortTest, QuickSortBasic) {
    std::vector<int> v = {5, 3, 4, 1, 2};
    quick_sort(v.begin(), v.end());
    EXPECT_EQ(v, std::vector<int>({1,2,3,4,5}));
}

TEST(SortTest, EmptyArray) {
    std::vector<int> v;
    quick_sort(v.begin(), v.end());
    EXPECT_TRUE(v.empty());
}

TEST(SortTest, SingleElement) {
    std::vector<int> v = {42};
    bubble_sort(v.begin(), v.end());
    EXPECT_EQ(v, std::vector<int>({42}));
}

TEST(SortTest, Duplicates) {
    std::vector<int> v = {3, 1, 2, 1, 3};
    quick_sort(v.begin(), v.end());
    EXPECT_EQ(v, std::vector<int>({1,1,2,3,3}));
}

TEST(FooSuite, FooTest)
{
    EXPECT_EQ(0, 0);
}
