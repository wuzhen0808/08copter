// https://github.com/hideakitai/MPU9250
#include <Arduino.h>
#include <Wire.h>
#include "MPU9250.h"

MPU9250 mpu; // You can also use MPU9255 as is

void setup() {
    Serial.begin(115200);
    Wire.begin(2, 1);
    delay(2000);
    
    mpu.setup(0x68); // change to your own address
    mpu.selectFilter(QuatFilterSel::NONE);
    mpu.setFilterIterations(15);
    delay(6000);
    //Serial.println("calibrateAccelGyro...");
    //mpu.calibrateAccelGyro();
    //Serial.println("calibrateMag...");
    //mpu.calibrateMag();
    //Serial.println("calibrated");
}

void loop() {
    if (mpu.update()) {
        /*
        Serial.print("(");
        Serial.print(mpu.getAccX());
        Serial.print(", ");
        Serial.print(mpu.getAccY());
        Serial.print(", ");
        Serial.print(mpu.getAccZ());
        Serial.print(");");
        Serial.print("(");
        Serial.print(mpu.getGyroX());
        Serial.print(", ");
        Serial.print(mpu.getGyroY());
        Serial.print(", ");
        Serial.print(mpu.getGyroZ());
        Serial.print(");");
        Serial.print("(");
        Serial.print(mpu.getMagX());
        Serial.print(", ");
        Serial.print(mpu.getMagY());
        Serial.print(", ");
        Serial.print(mpu.getMagZ());
        Serial.print("); ");
        */
        Serial.print("(");
        Serial.print(mpu.getQuaternionX());
        Serial.print(", ");
        Serial.print(mpu.getQuaternionY());
        Serial.print(", ");
        Serial.print(mpu.getQuaternionZ());
        Serial.print(", ");
        Serial.print(mpu.getQuaternionW());
        Serial.print(");");

        Serial.print("\t(");
        Serial.print(mpu.getEulerX());
        Serial.print(", ");
        Serial.print(mpu.getEulerY());
        Serial.print(", ");
        Serial.print(mpu.getEulerZ());

        Serial.print(");");
        Serial.print("\t(");
        Serial.print(mpu.getRoll());
        Serial.print(", ");
        Serial.print(mpu.getPitch());
        Serial.print(", ");
        Serial.print(mpu.getYaw());
        Serial.print(");");
        Serial.print("(temperature:");
        Serial.print(mpu.getTemperature());
        Serial.print(");");

    } else {
        Serial.print("No update.");
        // delay(1000);
    }
    Serial.println("");
}