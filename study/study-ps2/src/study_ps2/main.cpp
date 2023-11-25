#include <Arduino.h>
#define XPIN 17
#define YPIN 18
#define ZPING 5
#define XSTOPNUM 124
#define YSTOPNUM 122
class Joystick {
public:
    int rawValueLeftMost = -1;
    int rawValueZeroLeft = -1;
    int rawValueZeroRight = -1;
    int rawValueRightMost = -1;
    //
    bool reverse = true;
    int pin = -1;
    int lastRawValue = -1;

public:
    Joystick() {
    }

    void setPin(int pin) {
        this->pin = pin;
        pinMode(pin, INPUT);
    }

    void detectOne(long timeSpan) {

        while (true) {
            Serial.println("going to study the joystick argument(min/max value), please move the joystick around.");
            this->resolveMinMax(this->rawValueLeftMost, this->rawValueRightMost, timeSpan);

            if (this->rawValueRightMost - this->rawValueLeftMost < 1000) {
                Serial.print("the norm area is too small:");
                Serial.print(this->rawValueLeftMost);
                Serial.print("/");
                Serial.print(this->rawValueRightMost);
                Serial.println(",retrying.");
                continue;
            } else if (this->rawValueRightMost < this->rawValueZeroRight || this->rawValueLeftMost > this->rawValueZeroLeft) {
                Serial.print("the norm area is not fully cover the zero area:");
                Serial.print(this->rawValueLeftMost);
                Serial.print("/");
                Serial.print(this->rawValueZeroLeft);
                Serial.print("/");
                Serial.print(this->rawValueZeroRight);
                Serial.print("/");
                Serial.print(this->rawValueRightMost);
                Serial.println(",retrying.");
                continue;

            } else {
                break;
            }
        }

        Serial.print("got min/max value:");
        Serial.print(this->rawValueLeftMost);
        Serial.print("/");
        Serial.print(this->rawValueRightMost);
        Serial.println();
    }

    void detectZero(long timeSpan) {
        while (true) {
            Serial.println("going to study the joystick argument(middle values), do not touch the joy sticks.");

            resolveMinMax(this->rawValueZeroLeft, this->rawValueZeroRight, timeSpan);

            if (this->rawValueZeroRight - this->rawValueZeroLeft > 100) {
                Serial.print("the zero area is too large:");
                Serial.print(this->rawValueZeroLeft);
                Serial.print("/");
                Serial.print(this->rawValueZeroRight);
                Serial.println(",retrying.");
                continue;
            } else {
                break;
            }
        }
        Serial.print("got zero area:");
        Serial.print(this->rawValueZeroLeft);
        Serial.print("/");
        Serial.print(this->rawValueZeroRight);
        Serial.println();
    }

    void resolveMinMax(int &min, int &max, long timeSpan) {
        long started = millis();
        long lastUpdated = started;
        for (int i = 0;; i++) {

            int rValue = readRaw();
            if (i == 0) {
                min = max = rValue;
            } else {
                if (rValue < min) {
                    min = rValue;
                    lastUpdated = millis();
                }
                if (rValue > max) {
                    max = rValue;
                    lastUpdated = millis();
                }
            }
            if (millis() - lastUpdated > timeSpan) {
                break;
            }
        }
    }

    int getLastRawRead() {
        return this->lastRawValue;
    }

    int readRaw() {
        int rawValue = analogRead(pin);
        lastRawValue = rawValue;
        return rawValue;
    }

    bool isMiddle(int rawValue) {
        return rawValue >= this->rawValueZeroLeft && rawValue <= this->rawValueZeroRight;
    }

    float readNorm() {
        int one;
        int zero;
        int value = readRaw();
        int direct = reverse ? -1 : 1;

        if (value > this->rawValueZeroRight) {
            one = this->rawValueRightMost;
            zero = this->rawValueZeroRight;
        } else if (value < this->rawValueZeroLeft) {
            one = this->rawValueLeftMost;
            zero = this->rawValueZeroLeft;
            direct = -direct;
        } else {
            return 0.0f;
        }
        return (float)(value - zero) * direct / (float)(one - zero);
    }
};
Joystick jsX;
Joystick jsY;

void setup() {

    Serial.begin(115200);

    jsX.setPin(17);
    jsY.setPin(18);

    jsX.detectZero(3000);
    jsY.detectZero(3000);

    jsX.detectOne(3000);
    jsY.detectOne(3000);

    Serial.println();

    Serial.println("done of setup.");
}

float lastXValue = 0;

float lastYValue = 0;

bool isNear(float f1, float f2) {
    float delta = f1 - f2;
    return delta < 0.01 && delta > -0.01;
}
void loop() {

    float xValue = jsX.readNorm();
    float yValue = jsY.readNorm();

    float deltaX = xValue - lastXValue;

    if (!isNear(xValue, lastXValue) || !isNear(yValue, lastYValue)) {
        Serial.print(xValue);
        Serial.print("/");
        Serial.print(jsX.getLastRawRead());
        Serial.print("/");
        Serial.print(jsX.rawValueLeftMost);
        Serial.print("/");
        Serial.print(jsX.rawValueZeroLeft);
        Serial.print("/");
        Serial.print(jsX.rawValueZeroRight);
        Serial.print("/");
        Serial.print(jsX.rawValueRightMost);

        Serial.print(",");
        Serial.print(yValue);
        Serial.print("/");
        Serial.print(jsY.getLastRawRead());
        Serial.println();

        lastXValue = xValue;
        lastYValue = yValue;
    }
    delay(10);
}