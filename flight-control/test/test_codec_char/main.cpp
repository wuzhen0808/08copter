
#include "a8/link/SensorsData.h"
#include "a8/util.h"
#include "a8/util/net.h"
#include <gtest/gtest.h>
#include <iostream>
#include <math.h>
using namespace testing;
using namespace a8::util;

void testReadChar(String binStr, char expectCh) {
    Buffer<char> buf;
    int ret = Char::binaryToChars(binStr, buf);
    EXPECT_EQ(1, ret);
    EXPECT_EQ(1, buf.len());
    EXPECT_EQ(expectCh, buf[0]);
}

TEST(TestIntCodec, testReadChar) {
    testReadChar("00000001", (char)1);
    testReadChar("00000010", (char)2);
    testReadChar("00000011", (char)3);
    testReadChar("11111111", (char)-1);
    testReadChar("11111110", (char)-2);
}

int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}