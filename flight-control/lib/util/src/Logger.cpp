#include "a8/core/Hal.hpp"
#include "a8/util/Logger.hpp"
#define INC (10)
namespace a8 {
namespace util {
using namespace a8::core;

Logger::Logger(string name) {
    this->name = name;
}

Logger::~Logger(){
    
}
void Logger::info(string msg) {
    Hal::println(msg.getText());
}

} // namespace util
} // namespace a8