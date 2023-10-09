
#include "a8/util.h"
#include "a8/util/net.h"
#include <gtest/gtest.h>
#include <iostream>
using namespace a8::util;
using namespace std;
using namespace testing;
using namespace a8::util::net;
class Skeleton {

public:
    String lastMessage;
    int lastType = 0;
    void onMessage(String msg) {
        this->lastMessage = msg;
    }
};
void bridge_(int type, void *data, void *context) {

    Skeleton *sk = static_cast<Skeleton *>(context);
    sk->lastType = type;
    if (type == 1) {
        sk->onMessage(*static_cast<String *>(data));
    }
}

TEST(TestCodec, testSimpleCodec) {
    SimpleCodec *cd1 = new SimpleCodec();
    const int type_ = 1;
    const int headerLen_ = cd1->getHeaderLength();

    cd1->add<String>(type_, CodecUtil::writeString_, CodecUtil::readString_);

    String str("123");
    BufferWriter writer;
    const int strTail_ = 1;
    // encode
    Result rst;
    int ret = cd1->write(&writer, type_, &str, rst);
    EXPECT_EQ(ret, headerLen_ + str.len() + strTail_); //[00000001-<string>-00000000]:[type-<string>-<tail>]

    // check the encoded string.
    EXPECT_EQ(writer.len(), headerLen_ + str.len() + strTail_);
    // check first byte, it's the type id of data.
    char typeC = writer[0];
    EXPECT_EQ(typeC, type_);

    // check the body
    Buffer<char> subBuffer = writer.subBuffer(headerLen_); // remove header, get the body.
    String str2 = StringUtil::toString(subBuffer);
    EXPECT_EQ(str, str2);

    // decode
    BufferReader reader(writer);
    char buf[5];
    int len = reader.read(buf, 5);
    EXPECT_EQ(5, len);
    EXPECT_EQ(type_, (int)buf[0]);
    EXPECT_EQ('1', buf[1]);
    EXPECT_EQ('2', buf[2]);
    EXPECT_EQ('3', buf[3]);
    EXPECT_EQ('\0', buf[4]);

    reader.reset();
    
    Skeleton skeleton;
    ret = cd1->read(&reader, bridge_, &skeleton, rst);
    if (ret < 0) {
        cout << "//:" << rst.errorMessage.text() << ":///" << endl;
    }
    EXPECT_EQ(ret, headerLen_ + str.len() + strTail_); //[00000001-<string>-00000000]:[type-<string>-<tail>]

    // check the received type and body.
    EXPECT_EQ(type_, skeleton.lastType);
    EXPECT_EQ(str, skeleton.lastMessage);
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