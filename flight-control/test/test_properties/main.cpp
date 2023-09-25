#include "a8/hal/Hal.h"
#include "a8/util/Buffer.h"
#include "a8/util/Properties.h"
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
String getString(void *value, const String &defValue) {

    if (value == 0) {
        return defValue;
    }
    String *ret = static_cast<String *>(value);

    return *ret;
}
TEST(TestProperties, smockTest1) {
    Properties pts;

    pts.set("A", "AV");
    String A = pts.getString("A", "NULL");
    EXPECT_EQ(A, "AV");

    pts.set("B", 1);
    int B = pts.getInt("B", 0);
    EXPECT_EQ(B, 1);

    pts.set("C", true);
    bool C = pts.getBool("C", false);
    EXPECT_EQ(C, true);

    pts.set("D", 1.2f);
    float D = pts.getFloat("D", -1.0f);
    EXPECT_EQ(D, 1.2f);

    pts.set("A", "AV2");
    String A2 = pts.getString("A", "NULL");
    EXPECT_EQ(A2, "AV2");

    pts.set("B", 2);
    int B2 = pts.getInt("B", -1);
    EXPECT_EQ(B2, 2);

    pts.set("C", false);
    bool C2 = pts.getBool("C", false);
    EXPECT_EQ(C2, false);

    pts.set("D", 2.3f);
    float D2 = pts.getFloat("D", -1.0f);

    EXPECT_EQ(D2, 2.3f);
    /*
     */
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}