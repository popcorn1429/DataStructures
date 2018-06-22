#ifndef __binary_tree_test_h__
#define __binary_tree_test_h__

#include "binary_tree.h"
#include "gtest/gtest.h"

class BinaryTreeTest : public ::testing::Test {
protected:
    BinaryTreeTest() {}

    virtual void SetUp() {
        //nothing to do ...
    }

    virtual void TearDown() {
        //nothing to do ...
    }

    ~BinaryTreeTest() {}
};

TEST_F(BinaryTreeTest, testConstruct) {
    BinaryTree<int> btr1;
    EXPECT_EQ(true, btr1.isEmpty());
    btr1.insert(3);
    btr1.insert(2);
    btr1.insert(5);

    EXPECT_EQ(2, btr1.search(2)->constRefData());
    EXPECT_EQ(NULL, btr1.search(10));
}

#endif /*__binary_tree_test_h__*/