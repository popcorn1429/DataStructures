#ifndef _priority_queue_test_h_
#define _priority_queue_test_h_

#include "priority_queue.h"
#include "gtest/gtest.h"

class PriorityQueueTest : public ::testing::Test {
public:
    PriorityQueueTest() {}
    ~PriorityQueueTest() {}
protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
private:
    //nothing
};

TEST_F(PriorityQueueTest, testAll) {
    PriorityQueue<int> pq1;
    EXPECT_EQ(true, pq1.empty());
    pq1.push(8, 2.8);
    pq1.push(5, 3.1);
    pq1.push(9, 1.5);
    pq1.push(0, 2.7);
    pq1.push(10, 1.5);
    EXPECT_EQ(false, pq1.empty());
    EXPECT_EQ(5, pq1.size());
    EXPECT_EQ(5, pq1.peek());
    pq1.pop();
    EXPECT_EQ(4, pq1.size());
    EXPECT_EQ(8, pq1.peek());
    pq1.pop();
    EXPECT_EQ(3, pq1.size());
    EXPECT_EQ(0, pq1.peek());
    pq1.pop();
    EXPECT_EQ(2, pq1.size());
    EXPECT_EQ(9, pq1.peek());
    pq1.pop();
    EXPECT_EQ(1, pq1.size());
    EXPECT_EQ(10, pq1.peek());
    pq1.pop();
    EXPECT_EQ(true, pq1.empty());
}

#endif /*_priority_queue_test_h_*/