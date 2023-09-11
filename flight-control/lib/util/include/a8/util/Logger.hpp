#ifndef LOGGER__
#define LOGGER__

#include "a8/util/Shared.hpp"
#include "a8/util/string.hpp"

namespace a8 {
namespace util {

class Logger {
public:
    Logger(string name);
    ~Logger();
    void info(string msg);

private:
    string name;
};
} // namespace util

} // namespace a8

#endif