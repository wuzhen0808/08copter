#include "a8/util.h"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
using namespace std;
using namespace a8::util;
using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListener;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

TEST(TestMath, testLog10) {
    float exp = Math::log10<float>(1.0f);
    EXPECT_EQ((int)exp, 0);
}

template <typename T, typename P>
void testPower(T m, int precision, P expectedResult) {
    cout << "m:" << m << ",precision:" << precision << ",expected:" << expectedResult;
    P power = Math::pow10<P>((P)precision);
    P number = P(m * power);
    EXPECT_EQ(number, expectedResult);
}

TEST(TestMath, testPowerFloatInt) {
    testPower<float, int>(0.123456, 6, 123456);
}

TEST(TestMath, testPowerDoubleI) {
    testPower<float, int>(0.1234567, 7, 1234567);
}

TEST(TestMath, testPowerDoubleL) {
    testPower<double, long>(0.1234567, 7, 1234567);
}

TEST(TestMath, testPowerDoubleLL) {
    testPower<double, long long>(0.1234567890123456, 16, 1234567890123456);
}

TEST(TestMath, testPowerFloatULL) {
    testPower<float, unsigned long long>(0.1234567, 7, 1234567);
}

TEST(TestFormat, testFormatInt) {
    int len = 20;
    char buf[len];
    int value = 1234567890;
    int len2 = Format::formatAsInt<int>(buf, len, value, true);
    cout << buf << "\n";
    EXPECT_EQ(len2, 10);
}

TEST(TestFormat, testFormatIntNeg) {
    int len = 20;
    char buf[len];
    int value = -1234567890;
    int len2 = Format::formatAsInt<int>(buf, len, value, true);
    cout << buf << "\n";
    EXPECT_EQ(len2, 11);
}

TEST(TestFormat, testFormatLong) {
    int len = 20;
    char buf[len];
    long value = 1234567890L;
    int len2 = Format::formatAsInt<long>(buf, len, value, true);
    cout << buf << "\n";
    EXPECT_EQ(len2, 10);
}

TEST(TestFormat, testFormatLongNeg) {
    int len = 20;
    char buf[len];
    long value = -1234567890L;
    int len2 = Format::formatAsInt<long>(buf, len, value, true);
    cout << buf << "\n";
    EXPECT_EQ(len2, 11);
}

TEST(TestFormat, testFormatLongLong) {
    int len = 20;
    char buf[len];
    long long value = 1234567890123456LL;
    int len2 = Format::formatAsInt<long long>(buf, len, value, true);
    cout << value << "=?";
    cout << buf << "\n";
    EXPECT_EQ(len2, 16);
}

TEST(TestFormat, testFormatLongLongNeg) {
    int len = 20;
    char buf[len];
    long long value = -1234567890123456LL;
    int len2 = Format::formatAsInt<long long>(buf, len, value, true);
    cout << value << "=?";
    cout << buf << "\n";
    EXPECT_EQ(len2, 17);
}

TEST(TestFormat, testStringLong) {
    int len = 20;
    char buf[len];
    long value = 1234567890L;
    int len2 = Format::formatAsInt<long>(buf, len, value, true);
    cout << value << "=?";
    cout << buf << "\n";
    EXPECT_EQ(len2, 10);
}

TEST(TestFormat, testStringLongLong) {
    int len = 20;
    char buf[len];
    long long value = 1234567890123456789LL;
    int len2 = Format::formatAsInt<long long>(buf, len, value, true);
    cout << value << "=?";
    cout << buf << "\n";
    EXPECT_EQ(len2, 19);
}

TEST(TestFormat, testStringFloat) {
    int len = 20;
    char buf[len];
    float value = 1234567.91;
    Format::AutoOffsetFloat format(8, 2);
    int len2 = Format::formatAsFloat<float, long>(buf, len, value, &format, true);
    cout << value << "=?";
    cout << buf << "\n";
    EXPECT_EQ(len2, 10);
}

TEST(TestFormat, testStringFloatNeg) {
    int len = 20;
    char buf[len];
    float value = -999999.68;
    Format::AutoOffsetFloat format(8, 2);
    int len2 = Format::formatAsFloat<float, long>(buf, len, value, &format, true);
    cout << value << "=?";
    cout << buf << "\n";
    EXPECT_EQ(len2, 10);
}

TEST(TestFormat, testStringDouble) {
    int len = 20;
    char buf[len];
    double value = 1234567890123456;
    Format::AutoOffsetFloat format(16, 0);
    int len2 = Format::formatAsFloat<double, long long>(buf, len, value, &format, true);
    cout << value << "=?";
    cout << buf << "\n";
    EXPECT_EQ(len2, 16);
}

TEST(TestFormat, testFormatDoubleNeg) {
    int len = 20;
    char buf[len];
    double value = -1234567890123456;
    Format::AutoOffsetFloat format(16, 0);
    int len2 = Format::formatAsFloat<double, long long>(buf, len, value, &format, true);
    cout << value << "=?";
    cout << buf << "\n";
    EXPECT_EQ(len2, 17);
}

TEST(TestFormat, testStringFormatDoubleNeg) {
    int len = 20;
    char buf[len];
    double value = -1234567890123456;
    Format::AutoOffsetFloat format(16, 0);
    String str;
    str.setFloatFormat(&format);
    str << value;
    cout << value << "=?";
    cout << str.text() << "\n";
    EXPECT_EQ(str.len(), 17);
}

TEST(TestFormat, testStringFormatNegFloatAsDouble) {
    int len = 20;
    char buf[len];
    float fValue = -208.12f;
    Format::AutoOffsetFloat format(15, 0);
    String str;
    str.setFloatFormat(&format);
    str << (double)fValue;
    cout << fValue << "=?";
    cout << (double)fValue << "=?";
    cout << str.text() << "\n";
    EXPECT_EQ(str.len(), 4);
}

TEST(TestFormat, testStringFormatDouble2) {
    int len = 20;
    char buf[len];
    double fValue = 3.21;
    Format::AutoOffsetFloat format(16, 2);
    String str;
    str.setFloatFormat(&format);
    str << fValue;
    cout << fValue << "=?";
    cout << str.text() << "\n";
    EXPECT_EQ(str.len(), 4);
}

TEST(TestFormat, testStringFormatNegDouble2) {
    int len = 20;
    char buf[len];
    double fValue = -3.21;
    Format::AutoOffsetFloat format(15, 2);
    String str;
    str.setFloatFormat(&format);
    str << fValue;
    cout << fValue << "=?";
    cout << str.text() << "\n";
    EXPECT_EQ(str.len(), 5);
}

TEST(TestFormat, testStringLongDouble) {
    int len = 20;
    char buf[len];
    long double value = 123456789012345678;
    Format::AutoOffsetFloat format(18, 0);
    int len2 = Format::formatAsFloat<long double, long long>(buf, len, value, &format, true);
    cout << value << "=?";
    cout << buf << "\n";
    EXPECT_EQ(len2, 18);
}

TEST(TestFormat, testMore) {
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    // logFile.close();
    return 0;
}