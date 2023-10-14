
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
void testFloatSame() {
    
    EXPECT_TRUE(Float::isSame(0.01,0.011, 2));
    EXPECT_FALSE(Float::isSame(0.01,0.011, 5));

}

void testFloat32(float32 fv) {
    WriterReaderBuffer wr;

    CodecUtil::writeFloat32(&wr, fv);
    float32 fv2;
    CodecUtil::readFloat32(&wr, fv2);

    EXPECT_TRUE(Float::isSame(fv, fv2, 23));
}

TEST(TestCodec, testFloat) {
    testFloatSame();
    testFloat32(1.0f);
    testFloat32(1.0e2f);
    testFloat32(1.0e3f);
    testFloat32(1.0e-1f);
    testFloat32(1.0e-2f);
    testFloat32(1.0e-3f);

    testFloat32(0.0f);
    testFloat32(-1.0e-1f);
    testFloat32(-1.0e-2f);
    testFloat32(-1.0e-3f);
}

int main(int argc, char **argv) {

    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}