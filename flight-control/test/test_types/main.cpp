
#include "a8/link/SensorsData.h"
#include "a8/util.h"
#include "a8/util/net.h"
#include <gtest/gtest.h>
#include <iostream>
#include <math.h>
using namespace a8::util;
using namespace std;
using namespace testing;
using namespace a8::util::net;
using namespace a8::link;
TEST(TestTypes, testLiteral) {

    short s1 = -0x0001;         // 16bits
    unsigned short u1 = 0xFFFF; // 16bits
    unsigned short u11 = (unsigned int)s1;
    EXPECT_EQ(u1, u11);

    char c1 = -0x01;
    unsigned char uc1 = 0xFF;
    unsigned char uc11 = (unsigned char)c1;
    EXPECT_EQ(uc1, uc11);

    int i1 = -0x00000001;          // 16bits
    unsigned int ui1 = 0xFFFFFFFF; // 16bits
    unsigned int ui11 = (unsigned int)i1;
    EXPECT_EQ(ui1, ui11);
}
TEST(TestTypes, testCpp) {

    EXPECT_EQ(1, sizeof(char));
    EXPECT_EQ(1, sizeof(unsigned char));

    EXPECT_EQ(2, sizeof(short));          // 16 bits
    EXPECT_EQ(2, sizeof(unsigned short)); // 16 bits

    EXPECT_EQ(2, sizeof(short int));
    EXPECT_EQ(2, sizeof(unsigned short int));

    EXPECT_EQ(4, sizeof(int));
    EXPECT_EQ(4, sizeof(unsigned int));

    EXPECT_EQ(4, sizeof(long));
    EXPECT_EQ(4, sizeof(unsigned long));

    EXPECT_EQ(4, sizeof(float));

    EXPECT_EQ(8, sizeof(long long));
    EXPECT_EQ(8, sizeof(unsigned long long));

    EXPECT_EQ(8, sizeof(double));
}
TEST(TestTypes, testDefinedTypes) {

    EXPECT_EQ(1, sizeof(int8));
    EXPECT_EQ(1, sizeof(uint8));

    EXPECT_EQ(2, sizeof(int16));  // 16 bits
    EXPECT_EQ(2, sizeof(uint16)); // 16 bits

    EXPECT_EQ(4, sizeof(int32));
    EXPECT_EQ(4, sizeof(uint32));

    EXPECT_EQ(4, sizeof(float32));

    EXPECT_EQ(8, sizeof(int64));
    EXPECT_EQ(8, sizeof(uint64));

    EXPECT_EQ(8, sizeof(float64));
}

int main(int argc, char **argv) {

    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}