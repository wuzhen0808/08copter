#include "a8/util/Logger.hpp"
#include <Arduino.h>
#define INC (10)
namespace a8 {
namespace util {

Logger::Logger(char *name) {
    this->name = name;
}
Logger::~Logger(){
    
}
void Logger::info(char *string) {
    Serial.println(string);
}

} // namespace util
} // namespace a8