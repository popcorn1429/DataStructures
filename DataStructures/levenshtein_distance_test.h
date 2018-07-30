#ifndef __levenshtein_distance_test_h__
#define __levenshtein_distance_test_h__
#include "levenshtein_distance.h"
#include "gtest/gtest.h"

class LevenshteinDistanceTest : public ::testing::Test {
protected:
    LevenshteinDistanceTest() {}
    virtual ~LevenshteinDistanceTest() {}

    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(LevenshteinDistanceTest, testAll) {
    char str1[10] = "dailei";
    char str2[10] = "caiqing";
    EXPECT_EQ(5, LevenshteinDistance(str1, str1 + strlen(str1), str2, str2 + strlen(str2)));

    char str3[10] = "";
    char str4[10] = "dailei";
    EXPECT_EQ(6, LevenshteinDistance(str3, str3 + strlen(str3), str4, str4 + strlen(str4)));
}

#endif /*__levenshtein_distance_test_h__*/
