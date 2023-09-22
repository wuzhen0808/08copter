
#include "a8/hal/Hal.h"
#include "a8/hal/System.h"
#include "a8/hal/native/NativeApplication.h"
#include "a8/hal/native/NativeSystem.h"
#include <iostream>
#include <stdio.h>

using namespace a8::hal;
using namespace a8::hal::native;

System *a8::hal::S = new NativeSystem();

int main(int argc, char **argv) {
    Application *app = new NativeApplication();
    app->start();
}
