
#include "a8/link/SensorsData.h"
#include "a8/util.h"
#include "a8/util/net.h"
#include <gtest/gtest.h>
#include <iostream>
#include <math.h>
using namespace testing;
using namespace a8::util;

template <typename T, typename U>
void testWriteInt(T intV, const String &binStr2) {
    WriterReaderBuffer wr;
    int ret = CodecUtil::writeInt<T, U>(&wr, intV);
    EXPECT_EQ(sizeof(T), ret);
    EXPECT_EQ(sizeof(T), wr.len());
    String binStr;
    ret = Int::toBinaries<char>(wr, binStr);
    EXPECT_EQ(sizeof(T) * 8, ret);
    EXPECT_EQ(ret, binStr.len());
    EXPECT_EQ(ret, binStr2.len());
    EXPECT_EQ(binStr2, binStr);
}

template <typename T, typename U>
void testReadInt(const String &binStr, const T &intV) {
    WriterReaderBuffer wr;
    int ret = Char::writeBinary(&wr, binStr);
    EXPECT_EQ(sizeof(T), ret);

    T intV2;
    ret = CodecUtil::readInt<T, U>(&wr, intV2);
    EXPECT_EQ(sizeof(T), ret);
    EXPECT_EQ(intV, intV2);
}
template <typename T, typename U>
bool testWriteRead(T intV) {
    WriterReaderBuffer wrb;
    int ret = CodecUtil::writeInt<T, U>(&wrb, intV);
    EXPECT_EQ(sizeof(T), ret);

    T intV2;
    ret = CodecUtil::readInt<T, U>(&wrb, intV2);
    EXPECT_EQ(sizeof(T), ret);
    EXPECT_EQ(intV, intV2);
    return intV == intV2;
}
TEST(TestIntCode, testReadWrite16) {
    int max = Math::power(2,16);
    int min = -max;
    
    for (int i = min; i < max; i++) {
        bool ok = testWriteRead<int16, uint16>(i);
        if (!ok) {
            break;
        }
    }
}
TEST(TestIntCode, testReadWrite8) {
    for (int i = -128; i < 128; i++) {
        bool ok = testWriteRead<int8, uint8>(i);
        if (!ok) {
            break;
        }
    }
}
TEST(TestIntCodec, testReadInt16X) {
    testWriteInt<int16, uint16>(128, "0000000010000000");
    testReadInt<int16, uint16>("0000000010000000", 128);
}

// TEST(TestIntCodec, testReadInt5368708) {
//     EXPECT_EQ(0b00000000010100011110101110000100, 5368708);
//     testReadInt<int32, uint32>("00000000010100011110101110000100", 5368708);
// }

TEST(TestIntCodec, testWriteInt32) {
    testWriteInt<int32, uint32>(1, "00000000000000000000000000000001");
    testWriteInt<int32, uint32>(2, "00000000000000000000000000000010");
    testWriteInt<int32, uint32>(3, "00000000000000000000000000000011");
    testWriteInt<int32, uint32>(0, "00000000000000000000000000000000");
    testWriteInt<int32, uint32>(-1, "11111111111111111111111111111111");
    testWriteInt<int32, uint32>(-2, "11111111111111111111111111111110");
    testWriteInt<int32, uint32>(-3, "11111111111111111111111111111101");
}
TEST(TestIntCodec, testReadInt32) {
    testReadInt<int32, uint32>("00000000000000000000000000000001", 1);
    testReadInt<int32, uint32>("00000000000000000000000000000010", 2);
    testReadInt<int32, uint32>("00000000000000000000000000000011", 3);
    testReadInt<int32, uint32>("00000000000000000000000000000000", 0);
    testReadInt<int32, uint32>("11111111111111111111111111111111", -1);
    testReadInt<int32, uint32>("11111111111111111111111111111110", -2);
    testReadInt<int32, uint32>("11111111111111111111111111111101", -3);
}

TEST(TestIntCodec, testWriteInt16) {
    testWriteInt<int16, uint16>(1, "0000000000000001");
    testWriteInt<int16, uint16>(2, "0000000000000010");
    testWriteInt<int16, uint16>(3, "0000000000000011");
    testWriteInt<int16, uint16>(0, "0000000000000000");
    testWriteInt<int16, uint16>(-1, "1111111111111111");
    testWriteInt<int16, uint16>(-2, "1111111111111110");
    testWriteInt<int16, uint16>(-3, "1111111111111101");
}
TEST(TestIntCodec, testReadInt16) {
    testReadInt<int16, uint16>("0000000000000001", 1);
    testReadInt<int16, uint16>("0000000000000010", 2);
    testReadInt<int16, uint16>("0000000000000011", 3);
    testReadInt<int16, uint16>("0000000000000000", 0);
    testReadInt<int16, uint16>("1111111111111111", -1);
    testReadInt<int16, uint16>("1111111111111110", -2);
    testReadInt<int16, uint16>("1111111111111101", -3);
}

TEST(TestIntCodec, testWriteInt8) {
    testWriteInt<int8, uint8>(1, "00000001");
    testWriteInt<int8, uint8>(2, "00000010");
    testWriteInt<int8, uint8>(3, "00000011");
    testWriteInt<int8, uint8>(0, "00000000");
    testWriteInt<int8, uint8>(-1, "11111111");
    testWriteInt<int8, uint8>(-2, "11111110");
    testWriteInt<int8, uint8>(-3, "11111101");
}

TEST(TestIntCodec, testReadInt8) {
    testReadInt<int8, uint8>("00000000", 0);
    testReadInt<int8, uint8>("00000001", 1);
    testReadInt<int8, uint8>("00000010", 2);
    testReadInt<int8, uint8>("00000011", 3);
    testReadInt<int8, uint8>("11111111", -1);
    testReadInt<int8, uint8>("11111110", -2);
    testReadInt<int8, uint8>("11111101", -3);
}
int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}