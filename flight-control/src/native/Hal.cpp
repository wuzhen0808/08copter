
#include "a8/Hal.hpp"
#include <iostream>
#ifndef HAL_CPP
#define HAL_CPP
namespace a8 {
namespace hal {
using namespace std;
int map(int value, int from1, int to1, int from2, int to2) {
    return 0;
}
unsigned long millis() {
    return 0;
}
void print(char *msg) {
    cout << msg;
}
void println(int ivalue) {
    cout << ivalue << endl;
}
void println(char *msg) {
    cout << msg << endl;
}
} // namespace hal
} // namespace a8

#endif