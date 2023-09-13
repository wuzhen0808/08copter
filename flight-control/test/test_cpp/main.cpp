#include "a8/hal/System.h"
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace a8::util;
using namespace a8::hal;
using namespace  std;

class MyObj {

public:
    MyObj() {
        cout << "MyObj()" << endl;
    }
    MyObj(int value) {
        cout << "MyObj(int)" << endl;
        this->value = value;
    }
    MyObj(MyObj &copy) {
        cout << "MyObj(MyObj&)" << endl;
    }
    ~MyObj() {
        cout << "~MyObj()" << endl;
    }

    MyObj plus(MyObj &obj2){
        MyObj ret(this->value + obj2.value);
        return ret;
    }
    
    MyObj operator+(MyObj &obj2) {
        MyObj ret(this->value + obj2.value);
        return ret;
    }

    int value;
};

TEST(TestString, smockTest) {

    MyObj o1(1);
    MyObj o2(2);
    //cout << (o1 + o2).value << endl;
    cout << (o1.plus(o2).value) <<endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS())
        ;

    return 0;
}