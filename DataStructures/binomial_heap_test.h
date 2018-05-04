#ifndef _binomial_heap_test_h_
#define _binomial_heap_test_h_

#include "binomial_heap.h"
#include "gtest/gtest.h"

class BinomialHeapTest : public ::testing::Test {
protected:
    BinomialHeapTest() {}

    virtual void SetUp() {
        //nothing to do ...
    }
    
    virtual void TearDown() {
        //nothing to do ...
    }

    ~BinomialHeapTest() {}
};

TEST_F(BinomialHeapTest, testConstruct) {
    BinomialHeap<int> heap1;
    EXPECT_EQ(true, heap1.empty());
    heap1.push(3);
    heap1.push(2);
    heap1.push(5);
    EXPECT_EQ(3, heap1.size());

    BinomialHeap<int> heap2(heap1);
    EXPECT_EQ(3, heap2.size());
    EXPECT_EQ(2, heap2.top());

    heap1.pop();
    heap1.merge(heap2);
    EXPECT_EQ(5, heap1.size());
    EXPECT_EQ(true, heap2.empty());
    EXPECT_EQ(2, heap1.top());
}

#endif /*_binomial_heap_test_h_*/