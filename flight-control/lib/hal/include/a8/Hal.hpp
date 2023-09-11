#ifndef HAL__
#define HAL__

namespace a8 {
namespace hal {
int map(int value, int from1, int to1, int from2, int to2);
unsigned long millis();
void print(char *msg);
void println(char *msg);
void println(int ivalue);

} // namespace hal
} // namespace a8

#endif