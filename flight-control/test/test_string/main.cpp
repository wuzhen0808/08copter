#include "a8/Hal.hpp"
#include "a8/util/Buffer.hpp"
#include "a8/util/Text.hpp"
#include "native/Hal.cpp"
#include <gtest/gtest.h>

using namespace a8::util;
using namespace a8::hal;

// TEST(TestBuffer, smokeTest) {
//     //Buffer<char> *buffer = new Buffer<char>();
//     //EXPECT_EQ(buffer->getLength(), 0);

// }
void log(Text msg) {
    // a8::hal::println(msg.getText());
}

TEST(TestString, smockTest) {
    Text s1;
    cout << "SizeOf" << endl;

    // Text s2 = s1;
    // println("after log(s1)");
    //  log(string("s2"));
    EXPECT_EQ(s1.getLength(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS())
        ;

    return 0;
}