
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

void testFloat32(float fV){
    BufferReaderWriter 
    CodecUtil::writeFloat32(fV);

}

TEST(TestCodec, testFloat) {

    String str;
    int len = StringUtil::toBinary<int32>(5368708, str);
    EXPECT_EQ(32, len);

    testWriteReadInt32(5368708);

    testFloat32(1.0f, 1, 0);
    testFloat32(1.0e2f, 1, 2);
    testFloat32(1.0e3f, 1, 3);
    testFloat32(1.0e-1f, 1, -1);
    testFloat32(1.0e-2f, 1, -2);
    testFloat32(1.0e-3f, 1, -3);

    testFloat32(0.0f, 0, 0);
    testFloat32(-1.0e-1f, -1, -1);
    testFloat32(-1.0e-2f, -1, -2);
    testFloat32(-1.0e-3f, -1, -3);
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