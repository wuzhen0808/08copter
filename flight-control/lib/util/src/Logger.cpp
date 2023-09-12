#include "a8/hal/Hal.hpp"
#include "a8/util/Logger.hpp"
#define INC (10)

using namespace a8::hal;

namespace a8 {
namespace util {

Logger::Logger(String name) {
    this->name = name;
}

Logger::~Logger(){
    
}
void Logger::info(String msg) {
    S->out->println(msg);
}

} // namespace util
} // namespace a8