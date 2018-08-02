#ifndef __STRING_TEST_H__
#define __STRING_TEST_H__

#include "string.h"
#include "gtest/gtest.h"
class StringTest : public ::testing::Test {

};

TEST_F(StringTest, testAllCases) {
    String<char> str("dailei");
    EXPECT_EQ(6, str.size());
    EXPECT_EQ(32, str.capacity());

    String<wchar_t> wstr(L"我的神啊");
    EXPECT_EQ(4, wstr.size());
    EXPECT_EQ(32, wstr.capacity());
    wstr.append(L"，这都是些个什么鬼啊，还能不能愉快的玩耍了啊！！");
    EXPECT_EQ(28, wstr.size());
    EXPECT_EQ(32, wstr.capacity());
    wstr += L"告辞了。";
    EXPECT_EQ(32, wstr.size());
    EXPECT_EQ(64, wstr.capacity());
}

#endif /*__STRING_TEST_H__*/