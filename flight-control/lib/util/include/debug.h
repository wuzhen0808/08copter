// #define A8_DEBUG_STRING
#pragma once

#ifdef A8_DEBUG
#undef A8_DEBUG
#endif

#ifdef A8_DEBUG

#include <iostream>
#include <stdio.h>
using std::cout;
using std::endl;
#define LOG(msg) cout << msg << endl
#define LOG2(msg, msg2) cout << msg << msg2 << endl
#define WARN(msg) cout << msg << endl

#else
#define LOG(msg)
#define LOG2(msg, msg2)
#define WARN(msg)
#endif