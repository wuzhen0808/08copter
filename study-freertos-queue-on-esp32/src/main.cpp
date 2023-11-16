#include <Arduino.h>
TaskHandle_t task1;
TaskHandle_t task2;
QueueHandle_t queue;

void log(const char *msg) {
    Serial.println(msg);
}

void log(int i) {
    Serial.println(i);
}

void task1Callback(void *tData) {
    int i = 0;
    while (true) {
        vTaskDelay(1000);
        BaseType_t ret = xQueueSend(queue, &i, 1000);
        if (ret != pdTRUE) {
            log("failed to send, retrying.");
            continue;
        }

        i++;
    }
}

void task2Callback(void *tData) {
    while (true) {
        int i;
        BaseType_t ret = xQueueReceive(queue, &i, 1000);
        if (ret != pdTRUE) {
            log("timeout to receive, retrying.");
            continue;
        }
        log("received,i:");
        log(i);
    }
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    while (Serial.available()) {
        Serial.read();
    }
    queue = xQueueCreate(10, sizeof(int));
    BaseType_t ret;
    // ret = xTaskCreatePinnedToCore(task1Callback, "sender", 8192, 0, 1, &task1, 0);
    // if (ret != pdTRUE) {
    //     log("failed to create task1");
    // }
    ret = xTaskCreatePinnedToCore(task2Callback, "receiver", 8192, 0, 1, &task2, 1);
    if (ret != pdTRUE) {
        log("failed to create task2");
    }
}

void loop() {
}