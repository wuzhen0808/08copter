#include "a8/hal/System.h"
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace a8::util;
using namespace a8::hal;
using namespace std;

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
        value = copy.value;
        cout << "MyObj(MyObj&)" << endl;
    }
    MyObj(MyObj &&move) {
        value = move.value;
        cout << "MyObj(MyObj&&)" << endl;
    }
    ~MyObj() {
        cout << "~MyObj()" << endl;
    }

    MyObj plus(MyObj &obj2) {
        MyObj ret(this->value + obj2.value);
        return ret;
    }

    MyObj operator+(MyObj &obj2) {
        MyObj ret(this->value + obj2.value);
        return ret;
    }

    int value;
};

MyObj& refInAndRefOut(MyObj&& rVRef) {
    rVRef.value++;
    return rVRef;
}

MyObj& moveOut(int i) {
    MyObj* obj1 = new MyObj(i);
    return *obj1;
}

MyObj& testRRef(MyObj& obj1){
    return obj1;
}

TEST(TestString, testMove) {
    
    cout << ">>testRRef" << endl;
    MyObj &&obj0 = MyObj(1);
    MyObj obj00 = testRRef(obj0);
    
    cout << obj0.value << endl;
    cout << "<<testRRef" << endl;
    cout << ">>testMove" << endl;
    
    MyObj obj1 = moveOut(1);
    
    cout << obj1.value << endl;
    cout << "<<testMove" << endl;

    cout << ">>testRRef" << endl;
    
    MyObj& obj2 = refInAndRefOut(MyObj(2));    
    cout << obj2.value << endl;
    cout << "<<testRef" << endl;
}

TEST(TestString, smockTest) {

    MyObj o1(1);
    MyObj o2(2);
    // cout << (o1 + o2).value << endl;
    cout << (o1.plus(o2).value) << endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS())
        ;

    return 0;
}