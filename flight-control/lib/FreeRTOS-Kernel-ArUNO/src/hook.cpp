#include "FreeRTOS.h"
#include "task.h"
#include <Arduino.h>

void vAssertCalled(unsigned long ulLine,
                   const char *const pcFileName) {
    volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

    /* Called if an assertion passed to configASSERT() fails.  See
     * http://www.freertos.org/a00110.html#configASSERT for more information. */

    /* Parameters are not used. */
    (void)ulLine;
    (void)pcFileName;

    taskENTER_CRITICAL();
    {
        Serial.println("ASSERT!");

        /* You can step out of this function to debug the assertion by using
         * the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
         * value. */
        while (ulSetToNonZeroInDebuggerToContinue == 0) {
            __asm volatile("NOP");
            __asm volatile("NOP");
        }
    }
    taskEXIT_CRITICAL();
}
// #endif
extern "C" void vApplicationMallocFailedHook(void) {
    /* vApplicationMallocFailedHook() will only be called if
     * configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
     * function that will get called if a call to pvPortMalloc() fails.
     * pvPortMalloc() is called internally by the kernel whenever a task, queue,
     * timer or semaphore is created.  It is also called by various parts of the
     * demo application. If heap_1.c, heap_2.c or heap_4.c is being used, then the
     * size of the heap available to pvPortMalloc() is defined by
     * configTOTAL_HEAP_SIZE in FreeRTOSConfig.h, and the xPortGetFreeHeapSize()
     * API function can be used to query the size of free heap space that remains
     * (although it does not provide information on how the remaining heap might be
     * fragmented).  See http://www.freertos.org/a00111.html for more
     * information. */
    Serial.println("vApplicationMallocFailedHook");
    vAssertCalled(__LINE__, __FILE__);
}

extern "C" void vApplicationDaemonTaskStartupHook(void) {
    // vAssertCalled(__LINE__, __FILE__);
    Serial.println("vApplicationDaemonTaskStartupHook");
}

extern "C" void vApplicationTickHook(void) {
    // Serial.println("vApplicationTickHook");
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    Serial.println("vApplicationStackOverflowHook");
}
