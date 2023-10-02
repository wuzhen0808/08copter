#include "TersePrinter.h"
#include "TestSystem.h"
#include "a8/hal.h"
#include "a8/native/NativeSystem.h"
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
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

System *a8::hal::S = new TestSystem();

TEST(TestString, smockTest1) {
    
    String s1;
    EXPECT_EQ(s1.getLength(), 0);

    String s2 = s1;
    EXPECT_EQ(s2.getLength(), 0);

    EXPECT_TRUE(s1 == s2);
    String *s3 = new String();
    EXPECT_TRUE(*s3 == s2);
    delete s3;
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();    
    return 0;
}