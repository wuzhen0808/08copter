#include "a8/util.h"
#include <Arduino.h>

void a8::util::Debug::debug(const char *str) {
    Serial.println(str);
}

void a8::util::Debug::debug(const char *exp1, float exp2) {
    Serial.print(exp1);
    Serial.println(exp2);
}

void a8::util::Debug::debug(const char *exp1, float exp2, float exp3, float exp4, float exp5, float exp6) {
    Serial.print(exp1);
    Serial.print(",");
    Serial.print(exp2);
    Serial.print(",");
    Serial.print(exp3);
    Serial.print(",");
    Serial.print(exp4);
    Serial.print(",");
    Serial.print(exp5);
    Serial.print(",");
    Serial.println(exp6);
}
void a8::util::Debug::debug(const char *exp1, float exp2, float exp3, float exp4) {
    Serial.print(exp1);
    Serial.print(",");
    Serial.print(exp2);
    Serial.print(",");
    Serial.print(exp3);
    Serial.print(",");
    Serial.println(exp4);
}
