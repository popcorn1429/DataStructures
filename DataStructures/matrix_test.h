#ifndef __MATRIX_TEST_H__
#define __MATRIX_TEST_H__
#include "matrix.h"
#include "gtest/gtest.h"

class MatrixTest : public ::testing::Test {
protected:
    //...
};

TEST_F(MatrixTest, testAllCases) {
    Matrix<2, 4> m_2_4;
    Matrix<4, 2> m_4_2;
    EXPECT_EQ(8, m_2_4.size());



    Matrix<4, 4> m_4_4 = Identity<4>();
    EXPECT_EQ(1, m_4_4[0][0]);
    EXPECT_EQ(1, m_4_4[2][2]);
    EXPECT_EQ(0, m_4_4[0][2]);



}


#endif /*__MATRIX_TEST_H__*/