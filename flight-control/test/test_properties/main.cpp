#include "a8/hal/Hal.h"
#include "a8/util/Buffer.h"
#include "a8/util/Float.h"
#include "a8/util/Math.h"
#include "a8/util/Properties.h"
#include "a8/util/String.h"
#include "a8/util/StringReader.h"
#include "a8/util/StringWriter.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

using namespace a8::util;
using namespace a8::hal;
using namespace std;

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

TEST(TestProperties, testEmpty) {
    Properties pts;
    int i1 = pts.getInt("NoThis", -1);
    EXPECT_EQ(-1, i1);
}

TEST(TestFloat, testParseFloat) {

    long power0 = Math::power(10, 0);
    EXPECT_EQ(1, power0);
    long power1 = Math::power(10, 1);
    EXPECT_EQ(10, power1);

    long power2 = Math::power(10, 2);
    EXPECT_EQ(100, power2);

    double power_1 = Math::power(10, -1);
    EXPECT_EQ(.1, power_1);

    double d1 = Float::parse("2.3");
    EXPECT_EQ(d1, 2.3);

    float f1 = Float::parse("1.2");
    EXPECT_EQ(f1, 1.2f);
}
TEST(TestProperties, testLoad) {
    Properties pts;
    Reader *sr = new StringReader("s1=s1v\n"
                                  "s2=s2v\n"
                                  "i1(int)=1\n"
                                  "f1(float)=1.2\n");
    pts.load(*sr);
    StringWriter sw;
    pts.write(sw);
    cout << sw.toString().txt() << endl;

    String s1 = pts.getString("s1", "N");
    String s2 = pts.getString("s2", "N");
    int i1 = pts.getInt("i1", -1);
    float f1 = pts.getFloat("f1", -0.0f);
    EXPECT_EQ(s1, "s1v");
    EXPECT_EQ(s2, "s2v");
    EXPECT_EQ(i1, 1);
    EXPECT_TRUE(Float::isNear(f1, 1.2f));
    EXPECT_EQ(f1, 1.2f);
}

TEST(TestProperties, smockTest1) {
    Properties pts;

    pts.set("A", "AV");
    String A = pts.getString("A", "NULL");
    EXPECT_EQ(A, "AV");

    pts.set("B", 1);
    int B = pts.getInt("B", 0);
    EXPECT_EQ(B, 1);

    pts.set("D", 1.2f);
    float D = pts.getFloat("D", .0f);
    EXPECT_EQ(D, 1.2f);

    pts.set("A", "AV2");
    String A2 = pts.getString("A", "NULL");
    EXPECT_EQ(A2, "AV2");

    pts.set("B", 2);
    int B2 = pts.getInt("B", -1);
    EXPECT_EQ(B2, 2);

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