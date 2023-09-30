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
        cout << ">>MyObj()" << endl;
        cout << "<<MyObj()" << endl;
    }

    MyObj(int value) {
        cout << ">>MyObj(int)" << endl;
        this->value = value;
        cout << "<<MyObj(int)" << endl;
    }
    /*
    MyObj(const MyObj &copy) { // copy constructor?
        cout << ">>MyObj(MyObj&)" << endl;
        value = copy.value;
        cout << "<<MyObj(MyObj&)" << endl;
    }
    */

    ~MyObj() {
        cout << ">>-MyObj()" << endl;
        cout << "this->value:" << value << endl;
        cout << "<<-MyObj()" << endl;
    }
    /*
     */
    MyObj& operator=(const MyObj &o1) {
        cout << ">>MyObj &operator=(MyObj &o1)" << endl;
        this->value = o1.value;
        cout << "this:" << this << "=o1:" << &o1 << endl;
        cout << "<<MyObj &operator=(MyObj &o1)" << endl;
        return *this;
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

MyObj &refInAndRefOut(MyObj &&rVRef) {
    rVRef.value++;
    return rVRef;
}

MyObj &moveOut(int i) {
    MyObj *obj1 = new MyObj(i);
    return *obj1;
}

MyObj &testRRef(MyObj &obj1) {
    return obj1;
}
/*
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

*/
void testAssignOperator() {

    MyObj o1(1);
    MyObj o2;
    o2 = o1;
}

void testAssignOperator2() {

    MyObj o1(1);

    MyObj o2;

    MyObj o3;
    cout << &o3 << "/" << &o2 << "/" << &o1 << endl;

    o3 = o2 = o1;
}

MyObj getMyObj() {
    MyObj o1(1);
    return o1;
}
void testCopyConstructor() {

    MyObj o1(1);
    MyObj o2 = o1;
}

int main(int argc, char **argv) {
    testAssignOperator2();
    return 0;
}