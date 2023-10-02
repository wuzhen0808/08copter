#include "a8/hal.h"
#include "a8/util/Buffer.h"
#include "a8/util/Float.h"
#include "a8/util/Math.h"
#include "a8/util/Properties.h"
#include "a8/util/String.h"
#include "a8/util/StringReader.h"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

using namespace a8::util;
using namespace a8::hal;
using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListener;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

template <typename T>
Buffer<T> get(T (*func)(double)) {
    Buffer<T> buf;
    String str("1.0");

    buf = Float::parseAll<T>(str, ',', func);
    return buf;
}
template <typename T>
static Buffer<T> parseAll3(String fStr, char sep, T (*converter)(double)) {
    Buffer<T> ret;
    ret.append(converter(1));
    return ret;
}

template <typename T>
static Buffer<T> parseAll4(String str, char sep, T (*converter)(double)) {
    Buffer<T> ret;
    ret.append(1);
    return ret;
}
template <typename T>
Buffer<T> get2(T (*func)(double)) {
    Buffer<T> buf;
    String str("1.0");

    buf = Float::parseAll2<T>(str, ',', func);
    return buf;
}
template <typename T>
Buffer<T> get3(T (*func)(double)) {
    Buffer<T> buf;
    String str("1.0");

    buf = parseAll3<T>(str, ',', func);
    return buf;
}
template <typename T>
Buffer<T> get4(T (*func)(double)) {
Buffer<T>
    buf = parseAll4<T>(',', func);
    return buf;
}
Buffer<int> get2() {
    Buffer<int> buf;
    buf.append(1);
    return buf;
}
Buffer<int> get1() {
    Buffer<int> buf = get2();
    return buf;
}
int getInt(int def) {
    // Buffer<int> buf = get<int>([](double d) { return (int)d; });
    Buffer<int> buf = get1();
    if (buf.isEmpty()) {
        return def;
    }
    return buf.get(0);
}

TEST(TestBuffer, testParse2) {
    Buffer<int> buf = get3<int>([](double d) { return (int)d; });

    EXPECT_EQ(1, buf.getLength());
}
/*
TEST(TestBuffer, testGet) {
    int i = getInt(-1);
    EXPECT_EQ(1, i);
}
TEST(TestBuffer, testParse) {
    // Buffer<int> buf = get<int>([](double d) { return (int)d; });
    String str("1.0");
    Buffer<int> buf = Float::parseAll<int>(str, ',', [](double d) { return (int)d; });

    EXPECT_EQ(1, buf.getLength());
}
TEST(TestBuffer, testIntBuffer) {
    int i = getInt(-1);

    EXPECT_EQ(1, i);
}
*/

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}