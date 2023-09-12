#include "a8/core/Copter.hpp"
#include <iostream>
#include "a8/native/NativeSystem.h"

using namespace a8::native;

System * a8::hal::S = new NativeSystem();

int main(int argc, char **argv) {
    cout << "Hello,main()";
    return 0;
}



