#ifndef __MATRIX_TEST_H__
#define __MATRIX_TEST_H__
#include "matrix.h"
#include "gtest/gtest.h"

class MatrixTest : public ::testing::Test {
protected:
    //...
};

TEST_F(MatrixTest, testAllCases) {
    Matrix<2, 4> m24_1;
    EXPECT_EQ(8, m24_1.size());

    Matrix<4, 4> m44_1 = Identity<4>();
    EXPECT_EQ(1, m44_1[0][0]);
    EXPECT_EQ(1, m44_1[2][2]);
    EXPECT_EQ(0, m44_1[0][2]);

    double src[16] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6};
    Matrix<4, 2> m42_1(src, src+8);
    Matrix<4, 2> m42_2 = m44_1*m42_1;
    EXPECT_TRUE(m42_1 == m42_2);

}


#endif /*__MATRIX_TEST_H__*/