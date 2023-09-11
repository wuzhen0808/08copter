#include "a8/core/Hal.hpp"
#include "a8/util/Logger.hpp"
#define INC (10)
namespace a8 {
namespace util {
using namespace a8::core::hal;

Logger::Logger(string name) {
    this->name = name;
}

Logger::~Logger(){
    
}
void Logger::info(string msg) {
    println(msg.getText());
}

} // namespace util
} // namespace a8