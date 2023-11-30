// Refrerence:
// https://blog.csdn.net/weixin_45206768/article/details/114234545
#include <Arduino.h>
#include <Servo.h>

int myServo1Attach = 11;//3
// int myServo2Attach = 5;//5
// int myServo3Attach = 9;//5
// int myServo4Attach = 11;//11

Servo myServo1;

long lastPwm = 1000;
long pwm = 1000;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    while (Serial.available()) {
        Serial.read();
    }
    delay(2000);

    bool attached = myServo1.attach(myServo1Attach);
    Serial.print("attached:");
    Serial.print(attached);
    Serial.print(",pin:");
    Serial.println(myServo1Attach);

    myServo1.writeMicroseconds(1000);
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
    myServo1.writeMicroseconds(pwm);
    Serial.print("pwm:");
    Serial.println(pwm);
}
