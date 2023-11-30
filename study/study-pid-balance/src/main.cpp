#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
float rad_to_deg = 180 / 3.141592654;
float elapsedTime, timeMs, timeMsPrev;
float desired_angle = 0;
float error, previous_error;

// pid
float PID;
float pid_p = 0;
float pid_i = 0;
float pid_d = 0;
/////////////////PID CONSTANTS/////////////////
double kp = 3.55;  // 3.55
double ki = 0.005; // 0.003
double kd = 2.05;  // 2.05
// pwm
float pwmLeft, pwmRight;
float throttle = 1300;
long pwmLimit = 1400;
long pwmManualAdjust = 0;
long timeLimitSeconds = 20;
long startTimeMs;
// servos

const int len = 4;
int pins[len] = {17, 18, 19, 20};
Servo *servos = new Servo[len];

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
} // namespace study
int setupWire(int sdaPin, int sclPin) {
    bool ok = Wire.begin(sdaPin, sclPin);
    study::log("failed to setup wire.", true);
    return ok ? 1 : -1;
}

int setupSerial() {
    Serial.begin(115200);
    while (Serial.available()) {
        Serial.read();
    }
    return 1;
}

int setupMpu9250() {
    Wire.beginTransmission(0x68);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
    return 1;
}

int readRawXyz(int reg, int &x, int &y, int &z) {
    Wire.beginTransmission(0x68);
    Wire.write(reg); // Ask for the 0x3B register- correspond to Acc
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 6, 1);
    x = Wire.read() << 8 | Wire.read(); // each value needs two registres
    y = Wire.read() << 8 | Wire.read();
    z = Wire.read() << 8 | Wire.read();
    return 1;
}
int readRawAcc(int &x, int &y, int &z) {
    return readRawXyz(0x3B, x, y, z);
}
int readRawGyro(int &x, int &y, int &z) {
    return readRawXyz(0x43, x, y, z);
}
void calculateAccAngle(int rx, int ry, int rz, float &x, float &y, float &z) {
    x = atan((rx / 16384.0) / sqrt(pow((rx / 16384.0), 2) + pow((rz / 16384.0), 2))) * rad_to_deg;
    y = atan(-1 * (rx / 16384.0) / sqrt(pow((ry / 16384.0), 2) + pow((rz / 16384.0), 2))) * rad_to_deg;
    z = 0;
}
void calculateGyroAngle(int rx, int ry, int rz, float &x, float &y, float &z) {
    x = rx / 131.0f;
    y = ry / 131.0f;
    z = rz / 131.0f;
}

void calculateTotalAngle(float ax, float ay, float az, float gx, float gy, float gz, float &tx, float &ty, float &tz) {

    /*---X axis angle---*/
    tx = 0.98 * (tx + gx * elapsedTime) + 0.02 * ax;
    /*---Y axis angle---*/
    ty = 0.98 * (ty + gy * elapsedTime) + 0.02 * ay;
    tz = 0;
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
    setupSerial();
    int ret = setupWire(2, 1);

    if (ret < 0) {

        return;
    }
    study::log("success of setupWire.", true);
    ret = setupMpu9250();
    if (ret < 0) {
        return;
    }
    study::log("success of setupMpu9250.", true);
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
    timeMsPrev = timeMs;
    timeMs = millis();
    elapsedTime = (timeMs - timeMsPrev) / 1000.0f;

    int arx;
    int ary;
    int arz;
    int ret = readRawAcc(arx, ary, arz);
    delay(100);
    if (ret > 0) {
        //   log("acc-raw :", arx, ary, arz, false);
    } else {
        study::log("failed to readRawAcc.", false);
    }
    int grx;
    int gry;
    int grz;

    ret = readRawGyro(grx, gry, grz);
    if (ret > 0) {
        //    log("\tgyro-raw:,", grx, gry, grz, false);
    } else {
        study::log("failed to readRawGyro.", false);
    }
    float ax;
    float ay;
    float az;
    calculateAccAngle(arx, ary, arz, ax, ay, az);
    // log("\tacc:", ax, ay, az, false);

    float gx;
    float gy;
    float gz;
    calculateGyroAngle(grx, gry, grz, gx, gy, gz);
    // log("\tgyro:", gx, gy, gz, false);

    float tx, ty, tz;
    calculateTotalAngle(ax, ay, az, gx, gy, gz, tx, ty, tz);
    study::log("\ttotal:", tx, ty, tz, false);

    study::log("\n");
    // pid
    error = ty - desired_angle;
    pid_p = kp * error;
    if (-3 < error < 3) {
        pid_i = pid_i + (ki * error);
    }
    pid_d = kd * ((error - previous_error) / elapsedTime);
    //
    PID = pid_p + pid_i + pid_d;
    if (PID < -1000) {
        PID = -1000;
    }
    if (PID > 1000) {
        PID = 1000;
    }
    //
    pwmLeft = throttle + PID;
    pwmRight = throttle - PID;

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
    study::log(")\n");

    pwmLeft += pwmManualAdjust;
    pwmRight += pwmManualAdjust;

    // apply pwm
    if (timeMs - startTimeMs > timeLimitSeconds * 1000) {
        study::log("timeLimitSeconds exceed, stop all servos.");
        pwmLeft = 1000;
        pwmRight = 1000;
    }
    if (false) {

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

    // pre error
    previous_error = error; // Remember to store the previous error.
}