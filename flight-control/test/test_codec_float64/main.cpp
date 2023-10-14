
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

void testFloat64(float64 fv) {
    WriterReaderBuffer wr;

    CodecUtil::writeFloat64(&wr, fv);
    float64 fv2;
    CodecUtil::readFloat64(&wr, fv2);

    EXPECT_TRUE(Float::isSame(fv, fv2, 23));
}

TEST(TestCodec, testFloat64) {
    testFloatSame();
    testFloat64(1.0);
    testFloat64(1.0e2);
    testFloat64(1.0e3);
    testFloat64(1.0e-1);
    testFloat64(1.0e-2);
    testFloat64(1.0e-3);

    testFloat64(0.0);
    testFloat64(-1.0e-1);
    testFloat64(-1.0e-2);
    testFloat64(-1.0e-3);
}

int main(int argc, char **argv) {

    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}