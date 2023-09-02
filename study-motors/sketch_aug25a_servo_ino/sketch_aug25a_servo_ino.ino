// Refrerence:
// https://blog.csdn.net/weixin_45206768/article/details/114234545
#include <Servo.h>

int myServo1Attach = 3;
int myServo2Attach = 5;
int myServo3Attach = 9;
int myServo4Attach = 11;

Servo myServo1;
Servo myServo2;
Servo myServo3;
Servo myServo4;

int myServo1Enable = 0;
int myServo2Enable = 0;
int myServo3Enable = 0;
int myServo4Enable = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(Serial.read()>=0){}
  
  myServo1.attach(myServo1Attach);  
  myServo2.attach(myServo2Attach);  
  myServo3.attach(myServo3Attach);  
  myServo4.attach(myServo4Attach);  

  myServo1.writeMicroseconds(1000);
  myServo2.writeMicroseconds(1000);
  myServo3.writeMicroseconds(1000);
  myServo4.writeMicroseconds(1000);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    delay(10);
    
    String comStr = Serial.readString();
    Serial.println(comStr);

    int comInt = comStr.toInt();
    
    if(comInt == 1){      
      myServo1Enable = myServo1Enable == 0?1:0;
    } else if(comInt == 2){
      myServo2Enable = myServo2Enable == 0?1:0;
    } else if(comInt == 3){
      myServo3Enable = myServo3Enable == 0?1:0;
    } else if(comInt == 4){
      myServo4Enable = myServo4Enable == 0?1:0;
    } else {
      comInt = constrain(comInt, 1000, 2000);  
      if(myServo1Enable){
        myServo1.writeMicroseconds(comInt);
      }
      if(myServo2Enable){
        myServo2.writeMicroseconds(comInt);
      }
      if(myServo3Enable){
        myServo3.writeMicroseconds(comInt);
      }
      if(myServo4Enable){
        myServo4.writeMicroseconds(comInt);
      }
      Serial.print("");
      Serial.println(comInt);
      
    } 
    Serial.print("All ServoEnableStatus:");
    Serial.print(myServo1Enable);
    Serial.print(myServo2Enable);
    Serial.print(myServo3Enable);
    Serial.println(myServo4Enable);
  }
}





