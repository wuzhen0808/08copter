#include "a8/Hal.hpp"
#include "a8/util/Logger.hpp"
#define INC (10)
namespace a8 {
namespace util {
using namespace a8::hal;

Logger::Logger(Text name) {
    this->name = name;
}

Logger::~Logger(){
    
}
void Logger::info(Text msg) {
    println(msg.getText());
}

} // namespace util
} // namespace a8