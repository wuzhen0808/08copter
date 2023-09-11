#ifndef HAL__
#define HAL__
#include "a8/core/core.hpp"

namespace a8 {
namespace core {
class Hal {
public:    
    static int map(int value, int from1, int to1, int from2, int to2);
    static ulong millis();
    static void print(char *msg);
    static void println(char *msg);
}; // namespace hal

} // namespace core

} // namespace a8

#endif 