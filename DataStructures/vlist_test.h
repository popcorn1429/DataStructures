#ifndef __VLIST_TEST_H__
#define __VLIST_TEST_H__

#include "vlist.h"
#include "gtest/gtest.h"

class VListTest : public ::testing::Test {

};

TEST_F(VListTest, testAllCases) {
    VList<double> vlist1;
    EXPECT_TRUE(vlist1.empty());
    vlist1.add(3.5);
    vlist1.push(5.20);
    EXPECT_EQ(2, vlist1.size());
    vlist1.front() = 1.35;
    EXPECT_DOUBLE_EQ(1.35, vlist1.front());
    EXPECT_DOUBLE_EQ(5.20, vlist1.back());
    
    VList<double>::iterator itr = vlist1.begin();
    auto itr1(itr);
    auto itr2 = itr1;
    ++itr2;
    auto itr3 = itr2;
    itr3++;
    EXPECT_DOUBLE_EQ(1.35, *itr1);
    EXPECT_DOUBLE_EQ(5.20, *itr2);
    auto itr4 = vlist1.end();
    EXPECT_EQ(itr3, itr4);

    *itr2 = 2.50;
    EXPECT_DOUBLE_EQ(2.5, vlist1.back());

    itr2 = vlist1.insert(itr2, 3.3);
    EXPECT_EQ(3, vlist1.size());
    EXPECT_DOUBLE_EQ(3.3, *itr2);
    itr3 = itr2;
    ++itr3;
    itr4 = itr3;
    itr4++;
    EXPECT_DOUBLE_EQ(2.5, *itr3);
    EXPECT_EQ(vlist1.end(), itr4);

}

#endif /*__VLIST_TEST_H__*/