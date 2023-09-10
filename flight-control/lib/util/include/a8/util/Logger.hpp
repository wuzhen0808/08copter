#ifndef LOGGER__
#define LOGGER__

namespace a8 {
namespace util {

class Logger {
public:
    Logger(char* name);
    ~Logger();
    void info(char* string);

private:
    char* name;
};
} // namespace util

} // namespace a8

#endif