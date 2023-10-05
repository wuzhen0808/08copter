#include "a8/hal.h"
#include "a8/util/Buffer.h"
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
    int *i1 = new int(1);
    int *i2 = new int(2);
    int *i3 = new int(3);
    Buffer<int *> *buf = new Buffer<int *>();
    buf->append(i1);
    buf->append(i2);
    buf->append(i3);
    EXPECT_EQ(3, buf->length());
    buf->removeEle(i2);
    EXPECT_EQ(2, buf->length());
    int i11 = *buf->get(0);
    EXPECT_EQ(1, i11);
    int i31 = *buf->get(1);
    EXPECT_EQ(3, i31);
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}