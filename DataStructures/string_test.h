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

    String<wchar_t> wstr(L"�ҵ���");
    EXPECT_EQ(4, wstr.size());
    EXPECT_EQ(32, wstr.capacity());
    wstr.append(L"���ⶼ��Щ��ʲô�������ܲ���������ˣ�˰�����");
    EXPECT_EQ(28, wstr.size());
    EXPECT_EQ(32, wstr.capacity());
    wstr += L"����ˡ�";
    EXPECT_EQ(32, wstr.size());
    EXPECT_EQ(64, wstr.capacity());
}

#endif /*__STRING_TEST_H__*/