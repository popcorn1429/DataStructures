#ifndef __grid_test_h__
#define __grid_test_h__
#include "grid.h"
#include "gtest/gtest.h"

class GridTest : public ::testing::Test {
protected:
    GridTest() {}
    virtual ~GridTest() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(GridTest, testAll) {
    Grid<int> grid1;
    EXPECT_TRUE(grid1.empty());
    grid1.resize(5, 5);
    EXPECT_FALSE(grid1.empty());
    EXPECT_EQ(25, grid1.size());

    grid1[3][3] = 10;
    auto itr1 = grid1.row_begin(3);
    auto itr2 = grid1.row_end(3);
    EXPECT_EQ(5, std::distance(itr1, itr2));
    int sum = 0;
    for (auto itr = itr1; itr != itr2; ++itr) {
        sum += *itr;
    }
    EXPECT_EQ(10, sum);

    Grid<int> grid2(5, 5);
    EXPECT_TRUE(grid1 > grid2);
}

#endif /*__grid_test_h__*/