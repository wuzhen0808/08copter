
#include "a8/hal/Hal.h"
#include "a8/hal/System.h"
#include "a8/hal/native/NativeApplication.h"
#include "a8/hal/native/NativeFileReader.h"
#include "a8/hal/native/NativeSystem.h"
#include "a8/util/Properties.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace a8::hal;
using namespace a8::hal::native;

System *a8::hal::S = new NativeSystem();

int main(int argc, char **argv) {
    NativeApplication::start(argc, argv);
}
