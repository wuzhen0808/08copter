#include <Arduino.h>

uint8_t globalCounter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  Serial.println ("Blink with ESP32");
  pinMode (LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  static uint8_t localCounter = 0;
  
  digitalWrite (LED_BUILTIN, HIGH);
  delay (100);
  digitalWrite (LED_BUILTIN, LOW);
  delay (100);
  
  localCounter += 2;
  globalCounter++;
}