
#include "a8/util.h"
#include "a8/util/net.h"
#include <gtest/gtest.h>
#include <iostream>
using namespace a8::util;
using namespace std;
using namespace testing;
using namespace a8::util::net;

TEST(TestCodec, testSimpleCodec) {
    SimpleCodec *cd1 = new SimpleCodec();
    const int type_ = 1;
    const int headerLen_ = 3;

    cd1->add(type_, CodecFunc::writeString, CodecFunc::readString);

    String str("123");
    StringWriter writer;

    // encode
    Result rst;
    int ret = cd1->write(&writer, type_, &str, rst);
    EXPECT_EQ(ret, headerLen_ + str.len());
    String str2 = writer.toString();
    EXPECT_EQ(str2.len(), headerLen_ + str.len());

    char typeC = str2.charAt(0);
    EXPECT_EQ(typeC, type_);
    char lenLow = str2.charAt(1);
    EXPECT_EQ(lenLow, 3);
    char lenHigh = str2.charAt(2);
    EXPECT_EQ(lenHigh, 0);

    String str3 = str2.subStr(headerLen_); // remove header[0]:ver,header[1]:type,header[2,3]:length of body.
    EXPECT_EQ(str, str3);

    // decode
    StringReader reader(str2);

    int type = 0;
    void *data2 = 0;
    ret = cd1->read(&reader, type, data2, rst);
    EXPECT_EQ(ret, str.len() + headerLen_);
    EXPECT_EQ(type, type_);
    String *str4 = static_cast<String *>(data2);
    EXPECT_EQ(*str4, str);
}

int writeInt16(Writer *writer, int iValue) {
    char ch1 = iValue >> 0 & 0x00FF;
    char ch2 = iValue >> 8 & 0x00FF;
    writer->write(ch1);
    writer->write(ch2);
    return 2;
}
int readInt16(Reader *reader, char *buf) {
    reader->read(buf, 2);
    int i = 0;
    i = i + ((buf[0] & 0x00FF) << 0);
    i = i + ((buf[1] & 0x00FF) << 8);
    return i;
}
TEST(TestCodec, testShift) {
    EXPECT_EQ(21, 0x0015);
    EXPECT_EQ(1 << 8, 0x0100);
    EXPECT_EQ(21 << 8, 0x1500);
    cout << "21<<8=" << (21 << 8);
    EXPECT_EQ(21 & 0x00FF, 0x0015);
    EXPECT_EQ(21 << 8 & 0x00FF, 0x0000);
    char c1 = 21;
    char c2 = 0;

    EXPECT_EQ(c1, 21);
    EXPECT_EQ(c2 << 8, 0x0000);

    EXPECT_EQ((c1 & 0x00FF) << 0, 0x0015);
    EXPECT_EQ((c2 & 0x00FF) << 8, 0x0000);

    EXPECT_EQ(0x0015 + 0x0000, 21);

    int i1 = (c1 & 0x00FF) << 0;
    int i2 = (c2 & 0x00FF) << 8;
    int i = i1 + i2;
    EXPECT_EQ(i, 21);
}

TEST(TestCodec, testInt16) {
    StringWriter sw;

    writeInt16(&sw, 21);
    String str = sw.toString();
    const char *buf1 = str.text();
    EXPECT_EQ(buf1[0], 21);
    EXPECT_EQ(buf1[1], 0);

    StringReader sr(str);
    char buf2[2];
    int i2 = readInt16(&sr, buf2);
    EXPECT_EQ(buf2[0], 21);
    EXPECT_EQ(buf2[1], 0);

    EXPECT_EQ(i2, 21);
}
int main(int argc, char **argv) {

    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}