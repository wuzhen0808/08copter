#include <Arduino.h>
#include "setup_.h"
#include "a8/util.h"
using namespace a8::util;
void testStdIo(){
    char buf[15];
    int len = snprintf(buf, 15, "%e", 1.0f);
    Serial.print("testStdIo:");
    Serial.println(len);
    Serial.print("buf:");
    Serial.println(buf);

    a8::util::String msg;
    msg.append(1.1f);
    Serial.print("string-float:");
    Serial.println(msg.text());
    
}
void setup(){
    Serial.begin(9600);
    delay(1000);
    Serial.println(">>setup");
    testStdIo();
    setup_();
    Serial.println("<<setup");
    //Serial.begin(9600);
    //delay(1000);
    //Serial.println("hello");
}

void loop(){

}