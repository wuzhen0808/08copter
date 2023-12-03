#pragma once
#include <Arduino.h>
#include <Wire.h>
float rad_to_deg = 180 / 3.141592654;

namespace study {

void log(const char *msg) {
    Serial.print(msg);
}

void log(const char *msg, bool ln) {
    log(msg);
    if (ln) {
        Serial.println();
    }
}
void log(const int msg) {
    Serial.print(msg);
}
void log(const int msg, bool ln) {
    log(msg);
    if (ln) {
        Serial.println();
    }
}

void log(const long msg) {
    Serial.print(msg);
}
void log(const float msg) {
    Serial.print(msg);
}
void log(const float msg, bool ln) {
    log(msg);
    if (ln) {
        Serial.println();
    }
}

void log(int x, int y, int z, bool ln) {
    log("(", false);
    log(x, false);
    log(",", false);
    log(y, false);
    log(",", false);
    log(z, false);
    log(")", ln);
}
void log(const char *prompt, int x, int y, int z, bool ln) {
    log(prompt, false);
    log(x, y, z, ln);
}
void log(float x, float y, float z, bool ln) {
    log("(", false);
    log(x, false);
    log(",", false);
    log(y, false);
    log(",", false);
    log(z, false);
    log(")", ln);
}
void log(const char *prompt, float x, float y, float z, bool ln) {
    log(prompt, false);
    log(x, y, z, ln);
}

int setupWire() {
    bool ok = Wire.begin(2, 1);
    if (!ok) {
        study::log("failed to setup wire.", true);
    } else {
        study::log("success of setup wire.", true);
    }
    return ok ? 1 : -1;
}

int setupSerial() {
    Serial.begin(115200);
    while (Serial.available()) {
        Serial.read();
    }
    return 1;
}

} // namespace study