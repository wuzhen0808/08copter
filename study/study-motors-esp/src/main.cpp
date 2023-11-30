// Refrerence:
// https://blog.csdn.net/weixin_45206768/article/details/114234545
#include <Arduino.h>
#include <ESP32Servo.h>
int len = 4;
// int pins[] = {19, 20, 47, 48};
// int pins[] = {19, 20, 47, 48};
int pins[] = {17, 18, 19, 20};
// int myServo2Attach = 5;//5
// int myServo3Attach = 9;//5
// int myServo4Attach = 11;//11

Servo *myServos;

long lastPwm = 1000;
long pwm = 1000;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    while (Serial.available()) {
        Serial.read();
    }

    // LEDC_MAX_BIT_WIDTH;
    delay(2000);
    // ESP32PWM::allocateTimer(0);
    // ESP32PWM::allocateTimer(1);
    // ESP32PWM::allocateTimer(2);
    // ESP32PWM::allocateTimer(3);
    myServos = new Servo[len];
    for (int i = 0; i < len; i++) {
        if (i == 0 || i == 1) {
            myServos[i].setPeriodHertz(50);
        }
        if (i == 2) {
            myServos[i].setPeriodHertz(100);
        }
        if (i == 3) {
            myServos[i].setPeriodHertz(100);
        }
    }
    for (int i = 0; i < len; i++) {

        int channel = myServos[i].attach(pins[i]);
        Serial.print("servo[");
        Serial.print(i);
        Serial.print("]");
        Serial.print("channel:");
        Serial.print(channel);
        Serial.print(",pin:");
        Serial.println(pins[i]);
    }
    for (int i = 0; i < len; i++) {
        myServos[i].writeMicroseconds(1000);
    }
}
void loop() {

    // put your main code here, to run repeatedly:
    Serial.println("Please Input(+/-):");
    while (!Serial.available()) {
        delay(1);
    }
    String comStr = Serial.readString();

    Serial.println(comStr);
    if (comStr == "=" || comStr == "+") {
        pwm += 100;
    } else if (comStr == "-") {
        pwm -= 100;
    }
    for (int i = 0; i < len; i++) {
        myServos[i].writeMicroseconds(pwm);
    }
    Serial.print("pwm:");
    Serial.println(pwm);
}
