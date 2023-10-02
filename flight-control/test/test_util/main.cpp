#include "a8/hal/Hal.h"
#include "a8/util/Buffer.h"
#include "a8/util/Float.h"
#include "a8/util/Math.h"
#include "a8/util/Properties.h"
#include "a8/util/String.h"
#include "a8/util/StringReader.h"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

using namespace a8::util;
using namespace a8::hal;
using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListener;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

TEST(TestBuffer, testParse2) {
    char *buf = 0;
    int len = 0;
    int cap = 0;
    Lang::appendFormat(buf, len, cap, 8, "%i", 1);
    cout << buf << endl;
    EXPECT_EQ(len, 1);

    EXPECT_EQ(cap, 8);

    EXPECT_FALSE(buf == 0);

    EXPECT_EQ(buf[0], '1');
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}