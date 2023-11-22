// Refer to:
// https://github.com/arduino/ArduinoCore-renesas/blob/main/libraries/Arduino_FreeRTOS/examples/FreeRTOS-Blink/FreeRTOS-Blink.ino
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

void loop_thread_func(void *pvParameters);

void blinky_thread_func(void *pvParameters);

TaskHandle_t loop_task, blinky_task;

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  Serial.begin(115200);
  while (!Serial) { }

  /* Init a task that calls 'loop'
   * since after the call to
   * 'vTaskStartScheduler' we'll never
   * get out of setup() and therefore
   * would never get to loop(), as we
   * are leaving the default execution
   * flow.
   */
  auto const rc_loop = xTaskCreate
    (
      loop_thread_func,
      static_cast<const char*>("Loop Thread"),
      512 / 4,   /* usStackDepth in words */
      nullptr,   /* pvParameters */
      1,         /* uxPriority */
      &loop_task /* pxCreatedTask */
    );

  if (rc_loop != pdPASS) {
    Serial.println("Failed to create 'loop' thread");
    return;
  }

  auto const rc_blinky = xTaskCreate
    (
      blinky_thread_func,
      static_cast<const char*>("Blinky Thread"),
      512 / 4,     /* usStackDepth in words */
      nullptr,     /* pvParameters */
      1,           /* uxPriority */
      &blinky_task /* pxCreatedTask */
    );

  if (rc_blinky != pdPASS) {
    Serial.println("Failed to create 'loop' thread");
    return;
  }

  Serial.println("Starting scheduler ...");
  /* Start the scheduler. */
  vTaskStartScheduler();
  /* We'll never get here. */
  for( ;; );
}

void loop()
{
  Serial.print("in loop()");
  Serial.println(millis());

  vTaskDelay(configTICK_RATE_HZ/4);
}

void loop_thread_func(void *pvParameters)
{
  for(;;)
  {
    loop();
    taskYIELD();
  }
}

void blinky_thread_func(void *pvParameters)
{
  /* setup() */
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  /* loop() */
  for(;;)
  {
    PinStatus pinStatus = digitalRead(LED_BUILTIN);
    Serial.print("pinStatus:");
    Serial.print(pinStatus);
    Serial.print("=>");
    if(pinStatus == LOW){
        pinStatus = HIGH;
    }else {
        pinStatus = LOW;
    }
    Serial.print(pinStatus);
    digitalWrite(LED_BUILTIN, pinStatus);
    vTaskDelay(configTICK_RATE_HZ);
  }
}