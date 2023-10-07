#include "TersePrinter.h"
#include "TestSystem.h"
#include "a8/hal.h"
#include "a8/hal/native/FGSocketOutputReader.h"
#include "a8/hal/native/NativeSystem.h"
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
#include "input_output/net_fdm.hxx"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

using namespace a8::util;
using namespace a8::hal;
using namespace a8::hal::native;
using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListener;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

System *a8::hal::S = new TestSystem();

static const int endianTest = 1;

bool isLittleEndian() {
    return *((char *)&endianTest) != 0;
}
TEST(TestJsbSim, testNetData) {
    uint32_t version = 538976288;
    float *fvalue = (float *)&version;
    cout<<version<<"'float="<<*fvalue;

    EXPECT_EQ(402653184, htonl(24));
    EXPECT_EQ(24, ntohl(402653184));
    
    EXPECT_EQ(1, 0);
}
TEST(TestJsbSim, littleEndian) {
    bool isl = isLittleEndian();
    EXPECT_TRUE(isl);
}

TEST(TestString, smockTest1) {

    FGNetFDM data;
    data.num_engines = FGNetFDM::FG_MAX_ENGINES;
    data.num_tanks = FGNetFDM::FG_MAX_TANKS;
    data.num_wheels = FGNetFDM::FG_MAX_WHEELS;

    String line;
    FGSocketOuputReader::buildDataLine(data, line);
    EXPECT_EQ(0, 0)
        << "altitude:" << data.altitude << ",agl:" << data.agl << "/" << line.text() << endl;
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}