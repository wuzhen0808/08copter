#include <Arduino.h>
#include <Servo.h>
#include <driver/mcpwm.h>
namespace study::motors::esp {
class MotorsEspExample {
    int len;
    Servo *servos;
    int *pins;
    long timeLimit = 5 * 1000; //
    long speedLimit = 1500;
    long loops;

public:
    MotorsEspExample() {
        this->len = 0;
        this->servos = 0;
        this->loops = 0;
    }
    ~MotorsEspExample() {
        if (this->servos != 0) {
            delete[] servos;
        }
    }
    void setup(int *pins, int len) {
        this->len = len;
        servos = new Servo[len];
        this->pins = new int[len];
        for (int i = 0; i < len; i++) {
            this->pins[i] = pins[i];
            bool ok = servos[i].attach(pins[i]);
            if (!ok) {
                log("failed to attach.");
            }
        }
        for (int i = 0; i < len; i++) {
            servos[i].writeMicroseconds(this->pins[i], 1000);
        }
    }

    void log(const char *msg) {
        Serial.println(msg);
    }

    void loop() {
        this->loops++;
        if (loops % 10 > 0) {
            return;
        }

        float speed = (float)this->loops / (float)timeLimit;

        if (speed > 0.5f) {
            speed = 1 - speed;
        }

        if (speed < 0) {
            speed = 0;
        }

        long ms = 1000 + 1000 * (speed * 2);
        if (ms > speedLimit) {
            ms = speedLimit;
        }

        for (int i = 0; i < len; i++) {
            servos[i].writeMicroseconds(this->pins[i], ms);
        }
    }
};
} // namespace study::motors::esp