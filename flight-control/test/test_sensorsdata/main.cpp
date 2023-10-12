
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

void testWriteReadInt8(char iValue) {
    BufferWriter writer;
    int len = CodecUtil::writeInt8(&writer, iValue);
    EXPECT_EQ(1, len);
    char ch = writer.get(0);
    EXPECT_EQ(iValue, ch);

    BufferReader reader(writer);
    char iV2;
    int len2 = CodecUtil::readInt8(&reader, iV2);
    EXPECT_EQ(1, len2);
    EXPECT_EQ(iValue, iV2);
}

void testWriteReadInt16(short iValue) {
    BufferWriter writer;
    int len = CodecUtil::writeInt16(&writer, iValue);
    EXPECT_EQ(2, len);

    BufferReader reader(writer);
    short iV2;
    int len2 = CodecUtil::readInt16(&reader, iV2);
    EXPECT_EQ(2, len2);
    EXPECT_EQ(iValue, iV2);
}

void testWriteReadInt32(int iValue) {
    BufferWriter writer;
    int len = CodecUtil::writeInt32(&writer, iValue);
    EXPECT_EQ(4, len);

    BufferReader reader(writer);
    int iV2;
    int len2 = CodecUtil::readInt32(&reader, iV2);
    EXPECT_EQ(4, len2);
    EXPECT_EQ(iValue, iV2);
}

void testWriteReadInt64(long long iValue) {
    BufferWriter writer;
    int len = CodecUtil::writeInt64(&writer, iValue);
    EXPECT_EQ(8, len);
    char c0 = writer.get(0);
    char c1 = writer.get(1);
    char c2 = writer.get(2);
    char c3 = writer.get(3);
    char c4 = writer.get(4);
    char c5 = writer.get(5);
    char c6 = writer.get(6);
    char c7 = writer.get(7);
    if (iValue >= 0) {
        EXPECT_EQ(0x00, (int)c0);
    } else {
        EXPECT_EQ((char)0xFF, c0);
    }

    BufferReader reader(writer);
    long long iV2;
    int len2 = CodecUtil::readInt64(&reader, iV2);
    EXPECT_EQ(8, len2);
    EXPECT_EQ(iValue, iV2);
}

TEST(TestCodec, testCpp) {

    EXPECT_EQ(2, sizeof(short));          // 16 bits
    EXPECT_EQ(2, sizeof(unsigned short)); // 16 bits

    EXPECT_EQ(1, sizeof(char));
    EXPECT_EQ(1, sizeof(unsigned char));

    EXPECT_EQ(4, sizeof(int));
    EXPECT_EQ(4, sizeof(unsigned int));

    EXPECT_EQ(4, sizeof(long));
    EXPECT_EQ(4, sizeof(unsigned long));

    EXPECT_EQ(4, sizeof(float));

    EXPECT_EQ(8, sizeof(long long));
    EXPECT_EQ(8, sizeof(unsigned long long));

    EXPECT_EQ(8, sizeof(double));

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

TEST(TestCodec, testInt) {
    testWriteReadInt8(1);
    testWriteReadInt8(-1);
    testWriteReadInt8(0);
    testWriteReadInt8(2);
    testWriteReadInt8(-2);
    testWriteReadInt8(3);
    testWriteReadInt8(-3);

    testWriteReadInt16(1);
    testWriteReadInt16(-1);
    testWriteReadInt16(0);
    testWriteReadInt16(2);
    testWriteReadInt16(-2);
    testWriteReadInt16(3);
    testWriteReadInt16(-3);

    testWriteReadInt32(1);
    testWriteReadInt32(-1);
    testWriteReadInt32(0);
    testWriteReadInt32(2);
    testWriteReadInt32(-2);
    testWriteReadInt32(3);
    testWriteReadInt32(-3);

    testWriteReadInt64(1);
    testWriteReadInt64(-1);
    testWriteReadInt64(0);
    testWriteReadInt64(2);
    testWriteReadInt64(-2);
    testWriteReadInt64(3);
    testWriteReadInt64(-3);
}

TEST(TestCodec, testMath) {
    float f1 = 1.0;
    int ex = 0;
    float f11 = std::frexp(f1, &ex);
    EXPECT_EQ(0.5, f11);
    EXPECT_EQ(1, ex);

    float f2 = std::ldexp(f11, ex);
    EXPECT_EQ(1.0, f2);
}
TEST(TestCodec, testFloat) {
    BufferWriter writer;
    float f1 = 1.0f;
    CodecUtil::writeFloat(&writer, f1);
    BufferReader reader(writer);

    float f2 = 0.0f;
    CodecUtil::readFloat(&reader, f2);
    EXPECT_EQ(1.0f, f2);
}
/*
TEST(TestCodec, testInt16) {
    BufferWriter bw;
    SensorsData sdata;
    sdata.altitude = 1.2;

    int len1 = SensorsData::write(&bw, sdata);
    const char *buf1 = bw.buffer();
    BufferReader br(bw);

    SensorsData sdata2;
    int len2 = SensorsData::read(&br, sdata2);
    EXPECT_EQ(len1, len2);
    EXPECT_EQ(sdata, sdata2);
}
*/

int main(int argc, char **argv) {

    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}