#ifndef __sorted_linked_list_test_h__
#define __sorted_linked_list_test_h__
#include "sorted_linked_list.h"
#include "gtest/gtest.h"

class SortedLinkedListTest : public ::testing::Test {
protected:
    SortedLinkedListTest() {}

    virtual void SetUp() {
        //nothing to do ...
    }

    virtual void TearDown() {
        //nothing to do ...
    }

    ~SortedLinkedListTest() {}
};

TEST_F(SortedLinkedListTest, testConstruct) {
    SortedLinkedList<int> lstInt;
    EXPECT_EQ(true, lstInt.empty());
    lstInt.insert(5);
    lstInt.insert(3);
    lstInt.insert(9);
    EXPECT_EQ(3, lstInt.size());
    lstInt.print();

    lstInt.insert(0);
    lstInt.insert(5);
    auto pNode = lstInt.search(5);
    EXPECT_EQ(5, pNode->getData());
    lstInt.removeAll(5);
    lstInt.print();
}



#endif /*__sorted_linked_list_test_h__*/