

#include <Arduino.h>
#include <HeliOS.h>
#include <Wire.h>

void task2(xTask task_, xTaskParm parm_)
{
  Serial.println("on task2");
  delay(1000000);
}

void blinkTask_main(xTask task_, xTaskParm parm_)
{
  Serial.println("on task1");
  int ledState = DEREF_TASKPARM(int, parm_);

  if (ledState)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    ledState = 0;
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    ledState = 1;
  }

  DEREF_TASKPARM(int, parm_) = ledState;

  return;
}
String lastError = "none";

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  delay(2000);
  Serial.println("setup");
  xTask blink;
  xTask taskId2;
  int ledState;

  pinMode(LED_BUILTIN, OUTPUT);

  if (ERROR(xSystemInit()))
  {
    lastError = "error1";
    xSystemHalt();
  }
  Serial.println("1ok");
  if (ERROR(xTaskCreate(&blink, (const xByte *)"BLINKTSK", blinkTask_main, &ledState)))
  {
    lastError = "error2";
    xSystemHalt();
  }
  if (ERROR(xTaskCreate(&taskId2, (const xByte *)"TASK2", task2, &ledState)))
  {
    lastError = "error2";
    xSystemHalt();
  }
  
  Serial.println("2ok");
  if (ERROR(xTaskWait(blink)))
  {
    lastError = "error3";
    xSystemHalt();
  }

  Serial.println("2ok");
  if (ERROR(xTaskWait(taskId2)))
  {    
    xSystemHalt();
  }
  
  
   if (ERROR(xTaskChangePeriod(blink, 1000)))
   {
     lastError = "error4";
     xSystemHalt();
   }
   if (ERROR(xTaskChangePeriod(taskId2, 1000)))
   {
     xSystemHalt();
   }

// xTaskGetTaskState()

  Serial.println("4ok");
  if (ERROR(xTaskStartScheduler()))
  {
    lastError = "error5";
    Serial.println("5error");
    xSystemHalt();
  }
  Serial.println("calling xSystemHalt()");
  xSystemHalt();
}

void loop()
{
  Serial.print(lastError);
  Serial.println("in loop");
  delay(1000);
}