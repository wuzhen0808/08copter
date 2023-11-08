#include "a8/hal.h"
#include "a8/util/Buffer.h"
#include "a8/util/LinkedList.h"
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

TEST(TestBuffer, testNullPointerSet3) {
    Buffer<LinkedList<int> *> *buf = new Buffer<LinkedList<int> *>();
    LinkedList<int> *list0 = 0;
    LinkedList<int> *list1 = new LinkedList<int>();
    buf->set(1, 0, list1);
    LinkedList<int> *list00 = buf->get(0);
    EXPECT_EQ(list00, list0);

    LinkedList<int> *list11 = buf->get(1);
    EXPECT_EQ(list11, list1);
}

TEST(TestBuffer, testNullPointerSet2) {
    Buffer<LinkedList<int *> *> *buf = new Buffer<LinkedList<int *> *>();
    LinkedList<int *> *list0 = 0;
    LinkedList<int *> *list1 = new LinkedList<int *>();
    buf->set(1, 0, list1);
    LinkedList<int *> *list00 = buf->get(0);
    EXPECT_EQ(list00, list0);

    LinkedList<int *> *list11 = buf->get(1);
    EXPECT_EQ(list11, list1);
}
TEST(TestBuffer, testNullPointerSet) {
    Buffer<int *> *buf = new Buffer<int *>();
    int *i1 = new int(1);
    int *i0 = 0;
    buf->set(1, 0, i1);
    int *i00 = buf->get(0);
    EXPECT_EQ(i00, i0);

    int *i11 = buf->get(1);
    EXPECT_EQ(i1, i11);
}

TEST(TestBuffer, testNullPointer) {
    Buffer<int *> *buf = new Buffer<int *>();
    int *null = 0;
    buf->append(null);
    int *n1 = buf->get(0);
    EXPECT_EQ(n1, null);
}

TEST(TestBuffer, testSet) {
    Buffer<int> *buf = new Buffer<int>();
    buf->set(1, 0, 1);
    int i1 = buf->get(1);
    EXPECT_EQ(1, i1);
}

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