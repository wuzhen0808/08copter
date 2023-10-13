
#include "a8/link/SensorsData.h"
#include "a8/util.h"
#include "a8/util/net.h"
#include <gtest/gtest.h>
#include <iostream>
#include <math.h>
using namespace testing;
using namespace a8::util;
using namespace std;
using namespace a8::util::net;
using namespace a8::link;

template <typename T>
void testToBinary(T intV, const String str) {
    String str2;
    int len2 = Int::toBinary<T>(intV, str2);
    int len = sizeof(T) * 8;
    EXPECT_EQ(len, len2);
    EXPECT_EQ(str, str2);
}
TEST(TestCodec, testToBinary) {

    testToBinary<int8>(1, "00000001");
    testToBinary<int8>(2, "00000010");
    testToBinary<int8>(3, "00000011");
    testToBinary<int8>(4, "00000100");
    testToBinary<int8>(-1, "11111111");
    testToBinary<int8>(-2, "11111110");
    testToBinary<int8>(-3, "11111101");
    testToBinary<int8>(-4, "11111100");

    testToBinary<int16>(1, "0000000000000001");
    testToBinary<int16>(-1, "1111111111111111");

    testToBinary<int32>(1, "00000000000000000000000000000001");
    testToBinary<int32>(-1, "11111111111111111111111111111111");

    testToBinary<int64>(1, "0000000000000000000000000000000000000000000000000000000000000001");
    testToBinary<int64>(-1, "1111111111111111111111111111111111111111111111111111111111111111");

    testToBinary<int32>(5368708, "00000000010100011110101110000100");
}

template <typename T>
void testParseBinary(const String &str, T intV) {

    T intV2;
    int ret = Int::parseBinary<T>(str, 0, intV2);
    EXPECT_EQ(1, ret);
    EXPECT_EQ(intV2, intV);
}

TEST(TestCodec, testParseBinary) {
    testParseBinary<int8>("00000001", 1);
    testParseBinary<int8>("00000010", 2);
    testParseBinary<int8>("00000011", 3);
    testParseBinary<int8>("11111111", -1);
    testParseBinary<int8>("11111110", -2);
    testParseBinary<int8>("11111101", -3);
    testParseBinary<int32>("00000000010100011110101110000100", 5368708);
}

int main(int argc, char **argv) {

    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}