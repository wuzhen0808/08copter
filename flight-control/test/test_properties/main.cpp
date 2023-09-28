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
String getString(void *value, const String &defValue) {

    if (value == 0) {
        return defValue;
    }
    String *ret = static_cast<String *>(value);

    return *ret;
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
                                  "l1(long)=2\n"
                                  "f1(float)=1.2\n"
                                  "d1(double)=2.3\n"
                                  "c1(char)=c\n"
                                  "b1(bool)=true\n"
                                  "b2(bool)=y\n"
                                  "b3(bool)=TRUE\n"
                                  "b4(bool)=Y\n"
                                  "b5(bool)=false\n");
    pts.load(*sr);

    String s1 = pts.getString("s1", "N");
    String s2 = pts.getString("s2", "N");
    int i1 = pts.getInt("i1", -1);
    long l1 = pts.getLong("l1", -1);
    float f1 = pts.getFloat("f1", -0.0f);
    double d1 = pts.getDouble("d1", -0.0);
    char c1 = pts.getChar("c1", '0');
    bool b1 = pts.getBool("b1", false);
    bool b2 = pts.getBool("b2", false);
    bool b3 = pts.getBool("b3", false);
    bool b4 = pts.getBool("b4", false);
    bool b5 = pts.getBool("b5", true);
    EXPECT_EQ(s1, "s1v");
    EXPECT_EQ(s2, "s2v");
    EXPECT_EQ(i1, 1);
    EXPECT_EQ(l1, 2);
    EXPECT_TRUE(Float::isNear(f1, 1.2f));
    EXPECT_EQ(f1, 1.2f);
    EXPECT_TRUE(Float::isNear(d1, 2.3));
    EXPECT_EQ(d1, 2.3);

    EXPECT_EQ(c1, 'c');
    EXPECT_EQ(b1, true);
    EXPECT_EQ(b2, true);
    EXPECT_EQ(b3, true);
    EXPECT_EQ(b4, true);
    EXPECT_EQ(b5, false);
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
    float D = pts.getFloat("D", .0f);
    EXPECT_EQ(D, 1.2f);

    pts.set("F", 2.3);
    Buffer<String> names = pts.getNames();
    EXPECT_EQ(5, names.getLength());
    int idxF = names.indexOf("F");
    EXPECT_EQ(idxF, 4);
    double F = pts.getDouble("F", .0);
    EXPECT_EQ(F, 2.3);

    pts.set("G", 'G');
    float G = pts.getChar("G", '0');
    EXPECT_EQ(G, 'G');

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