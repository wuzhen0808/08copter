#include "MyCommon.h"
#include "MyPID.h"
#include <Arduino.h>
#include <ESP32Servo.h>
#include <MPU9250.h>
#include <Wire.h>

float elapsedTime, timeMs, timeMsPrev;
float desired_angle = 0;
float error;
// mpu9250
MPU9250 mpu;

// pwm
float pwmLeft, pwmRight;
float throttle = 1230;
long pwmLimit = 1600;
long pwmManualAdjust = 0;
long timeLimitSeconds = 60;
long startTimeMs;
// servos

const int len = 4;
int pins[len] = {17, 18, 19, 20};
Servo *servos = new Servo[len];
// statistics
float maxRoll = 0;

int setupMpu9250() {
    mpu.setup(0x68);
    mpu.selectFilter(QuatFilterSel::MADGWICK);
    mpu.setFilterIterations(1);
    study::log("success of setupMpu9250.", true);
    return 1;
}

int setupServos() {
    for (int i = 0; i < len; i++) {
        if (i == 0 || i == 1) {
            servos[i].setPeriodHertz(50);
        } else {
            servos[i].setPeriodHertz(100);
        }
        int channel = servos[i].attach(pins[i]);

        study::log("servo[");
        study::log(i);
        study::log("] attached to pin:");
        study::log(pins[i]);
        study::log(",channel:");
        study::log(channel);
        study::log("\n");
    }
    for (int i = 0; i < len; i++) {
        servos[i].writeMicroseconds(1000);
    }
    return 1;
}

void setup() {
    study::setupSerial();
    int ret = study::setupWire();
    if (ret < 0) {
        return;
    }
    ret = setupMpu9250();
    if (ret < 0) {
        return;
    }

    ret = setupServos();
    if (ret < 0) {
        return;
    }
    study::log("success of setupServos.", true);

    study::log("delay some time ... ", true);
    delay(5000); //
    study::log("success of setup.", true);
    timeMs = millis();
    startTimeMs = timeMs;
}

void loop() {
    if (!mpu.update()) {
        study::log("skip loop for no update of mpu9250.", true);
        return;
    }

    timeMsPrev = timeMs;
    timeMs = millis();
    elapsedTime = (timeMs - timeMsPrev) / 1000.0f;

    float roll = mpu.getRoll();
    float pitch = mpu.getPitch();
    float yaw = mpu.getYaw();
    study::log("\trpy:", roll, pitch, yaw, false);
    if (pitch > maxRoll) {
        maxRoll = roll;
    }
    // pid
    error = desired_angle - roll  ;
    study::updatePid(error, elapsedTime);
    //
    study::log("error(");
    study::log(error);
    study::log(")PID(");
    study::log(study::PID);
    study::log(")");
    pwmLeft = throttle + study::PID;
    pwmRight = throttle - study::PID;

    /*Once again we map the PWM values to be sure that we won't pass the min
    and max values. Yes, we've already maped the PID values. But for example, for
    throttle value of 1300, if we sum the max PID value we would have 2300us and
    that will mess up the ESC.*/
    // Right
    if (pwmRight < 1000) {
        pwmRight = 1000;
    }
    if (pwmRight > 2000) {
        pwmRight = 2000;
    }
    // Left
    if (pwmLeft < 1000) {
        pwmLeft = 1000;
    }
    if (pwmLeft > 2000) {
        pwmLeft = 2000;
    }
    study::log("\tpwm:(");
    study::log(pwmLeft);
    study::log(",");
    study::log(pwmRight);
    study::log(",");
    study::log(pwmManualAdjust);
    study::log(")");
    study::log("maxTy:");
    study::log(maxRoll);

    pwmLeft += pwmManualAdjust;
    pwmRight += pwmManualAdjust;

    // apply pwm
    if (timeLimitSeconds > 0 && timeMs - startTimeMs > timeLimitSeconds * 1000) {
        study::log("timeLimitSeconds exceed, stop all servos.");
        pwmLeft = 1000;
        pwmRight = 1000;
    }

    if (pwmLeft > pwmLimit) {
        pwmLeft = pwmLimit;
    }
    if (pwmRight > pwmLimit) {
        pwmRight = pwmLimit;
    }

    servos[0].writeMicroseconds(pwmLeft);
    servos[1].writeMicroseconds(pwmRight);

    study::log("\n");

    // pre error
    // previous_error = error; // Remember to store the previous error.
}