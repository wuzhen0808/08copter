#include "MyCommon.h"
// #include "MyPID.h"
// #include <Arduino.h>
// #include <ESP32Servo.h>
// #include <MPU9250.h>
// #include <Wire.h>

void setup() {
    int ret = 0;
    /*
    study::setupSerial();
    ret = study::setupWire();
    if (ret < 0) {
        return;
    }
    ret = study::setupMpu9250();
    if (ret < 0) {
        return;
    }
    */
    ret = study::setupExample();
    
    study::log("success of setup.", true);
}
int skips = 0;
void loop() {
    // if (!study::mpuUpdate()) {
    //     // study::log("skip loop for no update of mpu9250.", true);
    //     skips++;
    //     if (skips > 1000) {
    //         study::log("skip 1000 loop for no update of mpu9250.", true);
    //         skips = 0;
    //     }
    //     delay(10);
    //     return;
    // }
    delay(1000);

    // timeMsPrev = timeMs;
    // timeMs = millis();
    // elapsedTime = (timeMs - timeMsPrev) / 1000.0f;

    // float roll = study::mpuGetRoll();
    // float pitch = study::mpuGetPitch();
    // float yaw = study::mpuGetYaw();

    // study::log(roll, false);
    // study::log(",", false);
    // study::log(pitch, false);
    // study::log(",", false);
    // study::log(yaw, true);
}