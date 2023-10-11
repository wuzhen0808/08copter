
#include "a8/hal/native.h"
#include "a8/hal/native/winsock2windows.h"
#include <chrono>
#include <iostream>
#include <math.h>

using namespace a8::util;
using namespace a8::hal;
namespace a8::hal::native {
using namespace std::chrono;
/**
 * https://cplusplus.com/reference/ctime/strftime/
 */

NativeSystem::NativeSystem() {
    out = new NativeOutput();
}

void NativeSystem::sleep(long ms) {
    Sleep(ms);
}

long long NativeSystem::getSteadyTime() {
    time_point<steady_clock> now = steady_clock::now();
    return duration_cast<milliseconds>(now.time_since_epoch()).count();
}

long long NativeSystem::getSysTime() {
    time_point<system_clock> now = system_clock::now();
    return duration_cast<milliseconds>(now.time_since_epoch()).count();
}

void NativeSystem::formatTime(long long ms, String *str) {
    std::tm tm;
    long long sec = ms / 1000LL;
    int millis = ms - sec * 1000LL;
    localtime_s(&tm, &sec);
    char buf[50];
    int len = std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &tm);
    str->append(buf, len);
    str->appendFormat(".%.03d", millis);
    len = std::strftime(buf, sizeof(buf), "%z", &tm);
    str->append(buf, len);
}
int NativeSystem::openFile(String fpath, Reader *&fReaderCb) {
    fReaderCb = new NativeFileReader(fpath);
    return 1;
}

String NativeSystem::getEnv(String name) {
    char *value = std::getenv(name.text());
    String str;
    str << value;
    return str;
}

} // namespace a8::hal::native
