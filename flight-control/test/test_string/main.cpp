#include <gtest/gtest.h>
#include "a8/util/Buffer.hpp"
using namespace a8::util;

TEST(TestBuffer,smokeTest){
    Buffer<char>* buffer = new Buffer<char>();
    EXPECT_EQ(buffer->getLength(), 0);    
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS())
        ;

    return 0;
}