#ifndef _bounded_pqueue_test_h_
#define _bounded_pqueue_test_h_

#include "gtest/gtest.h"
#include "bounded_pqueue.h"

class BoundedPQueueTest : public ::testing::Test {

};

TEST_F(BoundedPQueueTest, testAll) {
    BoundedPQueue<int> bpq1(10);
    EXPECT_EQ(10, bpq1.maxSize());
    EXPECT_EQ(0, bpq1.size());
    EXPECT_EQ(true, bpq1.empty());
    bpq1.enqueue(100, 3.5);
    bpq1.enqueue(88, 2.0);
    bpq1.enqueue(99, 4.5);
    bpq1.enqueue(3, 5.1);
    bpq1.enqueue(2, 0.5);
    bpq1.enqueue(7, 2.5);
    bpq1.enqueue(123, 1.2);
    bpq1.enqueue(25, 3.5);
    bpq1.enqueue(72, 1.4);
    bpq1.enqueue(88, 2.6);
    bpq1.enqueue(9, 3.2);
    bpq1.enqueue(56, 2.4);
    bpq1.enqueue(90, 1.5);
    EXPECT_EQ(10, bpq1.size());
    EXPECT_EQ(3, bpq1.best());
    EXPECT_EQ(123, bpq1.worst());
    bpq1.dequeue();
    EXPECT_EQ(9, bpq1.size());
    EXPECT_EQ(99, bpq1.best());
    bpq1.enqueue(77, 3.4);
    bpq1.enqueue(35, 1.4);
    EXPECT_EQ(10, bpq1.size());
    EXPECT_EQ(99, bpq1.best());
    EXPECT_EQ(35, bpq1.worst());
}


#endif /*_bounded_pqueue_test_h_*/