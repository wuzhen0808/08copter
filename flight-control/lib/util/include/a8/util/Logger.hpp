#ifndef LOGGER__
#define LOGGER__

#include "a8/util/Shared.hpp"
#include "a8/util/Text.hpp"

namespace a8 {
namespace util {

class Logger {
public:
    Logger(Text name);
    ~Logger();
    void info(Text msg);

private:
    Text name;
};
} // namespace util

} // namespace a8

#endif