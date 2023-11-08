#include "a8/hal.h"
#include "a8/util/HashTable.h"
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

int hashCode(int ele) {
    return ele % 10;
}

bool equals(int e1, int e2) {
    return e1 == e2;
}

TEST(TestHashTable, testHashTable) {

    HashTable<int, String> ht(hashCode, equals);

    // set
    String v1 = "V1";
    String v2 = "V1";
    ht.set(1, v1);
    ht.set(2, v2);

    // get
    String def = "0";
    String v11 = ht.get(1, def);
    EXPECT_EQ(v1, v11);

    String v22 = ht.get(2, def);
    EXPECT_EQ(v2, v22);

    // get not exits element.
    String v33 = ht.get(3, def);
    EXPECT_EQ(v33, def);

    // remove  elements.
    int len1 = ht.remove(1);
    EXPECT_EQ(1, len1);
    bool len2 = ht.remove(2);
    EXPECT_EQ(1, len2);

    // remove not exists elements.
    int len3 = ht.remove(3);
    EXPECT_EQ(0, len3);
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}