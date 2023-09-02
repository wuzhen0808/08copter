/*
  US-100 Ultrasonic Sensor - Serial Mode
  modified on 26 Sep 2020
  by Mohammad Reza Akbari @ Electropeak
  Home
*/

#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2);

unsigned int HighByte = 0;
unsigned int LowByte  = 0;
unsigned int Len  = 0;

void setup() {
  
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  mySerial.flush();
  mySerial.write(0X55);                           // trig US-100 begin to measure the distance
  delay(500);                                  
  if (mySerial.available() >= 2)                  // check receive 2 bytes correctly
  {
    HighByte = mySerial.read();
    LowByte  = mySerial.read();
    Len  = HighByte * 256 + LowByte;          // Calculate the distance
    if ((Len > 1) && (Len < 10000))
    {
      Serial.print("Distance: ");
      Serial.print(Len, DEC);          
      Serial.println("mm");                  
    }
  }
  delay(300);                                    
} 
