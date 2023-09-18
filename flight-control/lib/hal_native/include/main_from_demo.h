#pragma once

/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/******************************************************************************
 * This project provides two demo applications.  A simple blinky style project,
 * and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting is used to select between the two.
 * The simply blinky demo is implemented and described in main_blinky.c.  The
 * more comprehensive test and demo application is implemented and described in
 * main_full.c.
 *
 * This file implements the code that is not demo specific, including the
 * hardware setup and FreeRTOS hook functions.
 *
 *******************************************************************************
 * NOTE: Windows will not be running the FreeRTOS demo threads continuously, so
 * do not expect to get real time behaviour from the FreeRTOS Windows port, or
 * this demo application.  Also, the timing information in the FreeRTOS+Trace
 * logs have no meaningful units.  See the documentation page for the Windows
 * port for further information:
 * https://www.FreeRTOS.org/FreeRTOS-Windows-Simulator-Emulator-for-Visual-Studio-and-Eclipse-MingW.html
 *
 *
 *
 *******************************************************************************
 */

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* This project provides two demo applications.  A simple blinky style demo
 * application, and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting is used to select between the two.
 *
 * If mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is 1 then the blinky demo will be built.
 * The blinky demo is implemented and described in main_blinky.c.
 *
 * If mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is not 1 then the comprehensive test and
 * demo application will be built.  The comprehensive test and demo application is
 * implemented and described in main_full.c. */
#define mainCREATE_SIMPLE_BLINKY_DEMO_ONLY    1

/* This demo uses heap_5.c, and these constants define the sizes of the regions
 * that make up the total heap.  heap_5 is only used for test and example purposes
 * as this demo could easily create one large heap region instead of multiple
 * smaller heap regions - in which case heap_4.c would be the more appropriate
 * choice.  See http://www.freertos.org/a00111.html for an explanation. */
#define mainREGION_1_SIZE                     8201
#define mainREGION_2_SIZE                     40905
#define mainREGION_3_SIZE                     50007

/* This demo allows for users to perform actions with the keyboard. */
#define mainNO_KEY_PRESS_VALUE                -1
#define mainOUTPUT_TRACE_KEY                  't'
#define mainINTERRUPT_NUMBER_KEYBOARD         3

/* This demo allows to save a trace file. */
#define mainTRACE_FILE_NAME                   "Trace.dump"

#ifdef __cplusplus
extern "C" {
#endif

void main_from_demo();

#ifdef __cplusplus
}
#endif

/*
 * Only the comprehensive demo uses application hook (callback) functions.  See
 * https://www.FreeRTOS.org/a00016.html for more information.
 */
//void vFullDemoTickHookFunction( void );
void vFullDemoIdleFunction( void );

/*
 * Prototypes for the standard FreeRTOS application hook (callback) functions
 * implemented within this file.  See http://www.freertos.org/a00016.html .
 */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask,
                                    char * pcTaskName );
void vApplicationTickHook( void );
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize );
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize );

/*
 * Windows thread function to capture keyboard input from outside of the
 * FreeRTOS simulator. This thread passes data safely into the FreeRTOS
 * simulator using a stream buffer.
 */
static DWORD WINAPI prvWindowsKeyboardInputThread( void * pvParam );

/*
 * Interrupt handler for when keyboard input is received.
 */
static uint32_t prvKeyboardInterruptHandler( void );

/*
 * Keyboard interrupt handler for the blinky demo.
 */
extern void vBlinkyKeyboardInterruptHandler( int xKeyPressed );
