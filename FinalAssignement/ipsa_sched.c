/*
 * FreeRTOS V202107.00
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
 * NOTE 1: The FreeRTOS demo threads will not be running continuously, so
 * do not expect to get real time behaviour from the FreeRTOS Linux port, or
 * this demo application.  Also, the timing information in the FreeRTOS+Trace
 * logs have no meaningful units.  See the documentation page for the Linux
 * port for further information:
 * https://freertos.org/FreeRTOS-simulator-for-Linux.html
 *
 * NOTE 2:  This project provides two demo applications.  A simple blinky style
 * project, and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting in main.c is used to select
 * between the two.  See the notes on using mainCREATE_SIMPLE_BLINKY_DEMO_ONLY
 * in main.c.  This file implements the simply blinky version.  Console output
 * is used in place of the normal LED toggling.
 *
 * NOTE 3:  This file only contains the source code that is specific to the
 * basic demo.  Generic functions, such FreeRTOS hook functions, are defined
 * in main.c.
 ******************************************************************************
 *
 * main_blinky() creates one queue, one software timer, and two tasks.  It then
 * starts the scheduler.
 *
 * The Queue Send Task:
 * The queue send task is implemented by the working() function in
 * this file.  It uses vTaskDelayUntil() to create a periodic task that sends
 * the value 100 to the queue every 200 milliseconds (please read the notes
 * above regarding the accuracy of timing under Linux).
 *
 * The Queue Send Software Timer:
 * The timer is an auto-reload timer with a period of two seconds.  The timer's
 * callback function writes the value 200 to the queue.  The callback function
 * is implemented by prvQueueSendTimerCallback() within this file.
 *
 * The Queue Receive Task:
 * The queue receive task is implemented by the prvQueueReceiveTask() function
 * in this file.  prvQueueReceiveTask() waits for data to arrive on the queue.
 * When data is received, the task checks the value of the data, then outputs a
 * message to indicate if the data came from the queue send task or the queue
 * send software timer.
 *
 * Expected Behaviour:
 * - The queue send task writes to the queue every 200ms, so every 200ms the
 *   queue receive task will output a message indicating that data was received
 *   on the queue from the queue send task.
 * - The queue send software timer has a period of two seconds, and is reset
 *   each time a key is pressed.  So if two seconds expire without a key being
 *   pressed then the queue receive task will output a message indicating that
 *   data was received on the queue from the queue send software timer.
 *
 * NOTE:  Console input and output relies on Linux system calls, which can
 * interfere with the execution of the FreeRTOS Linux port. This demo only
 * uses Linux system call occasionally. Heavier use of Linux system calls
 * may crash the port.
 */

#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"

/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY    ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY       ( tskIDLE_PRIORITY + 1 )
/* The rate at which data is sent to the queue.  The times are converted from
 * milliseconds to ticks using the pdMS_TO_TICKS() macro. */
//working frequency is 0.0405 seconds
#define WORKING_FREQUENCY_MS              pdMS_TO_TICKS( 405UL )
//temperature frequency is 0.0315 seconds
#define TEMPERATURE_FREQUENCY_MS          pdMS_TO_TICKS( 315UL )
//multiplication frequency is 0.297 seconds
#define MULTIPLICATION_FREQUENCY_MS       pdMS_TO_TICKS( 297UL )
//binary search frequency is 0.027 seconds
#define BINARYSEARCH_FREQUENCY_MS         pdMS_TO_TICKS( 27UL )


/* The number of items the queue can hold at once. */
#define mainQUEUE_LENGTH                   ( 4 )

/* The values sent to the queue receive task from the queue send task and the
 * queue send software timer respectively. */
#define mainVALUE_SENT_FROM_TASK           ( 100UL )
#define mainVALUE_SENT_FROM_TIMER          ( 200UL )

/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void working( void * pvParameters );
static void temperature( void * pvParameters );
static void Multiplication( void * pvParameters );
static void BinarySearch( void * pvParameters );



/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/* A software timer that is started from the tick hook. */
static TimerHandle_t xTimer = NULL;

/*-----------------------------------------------------------*/

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
void main_ipsa_sched( void )
{

    /* Create the queue. */
    xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

    if( xQueue != NULL )
    {
        /* Start the two tasks as described in the comments at the top of this
         * file. */
        //xTaskCreate( prvQueueReceiveTask,             /* The function that implements the task. */
        //             "Rx",                            /* The text name assigned to the task - for debug only as it is not used by the kernel. */
        //             configMINIMAL_STACK_SIZE,        /* The size of the stack to allocate to the task. */
        //             NULL,                            /* The parameter passed to the task - not used in this simple case. */
        //             mainQUEUE_RECEIVE_TASK_PRIORITY, /* The priority assigned to the task. */
        //             NULL );                          /* The task handle is not required, so NULL is passed. */

        xTaskCreate( working, "TX", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );
        xTaskCreate( temperature, "TX", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );
        xTaskCreate( Multiplication, "TX", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );
        xTaskCreate( BinarySearch, "TX", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );

        if( xTimer != NULL )
        {
            xTimerStart( xTimer, 0 );
        }

        /* Start the tasks and timer running. */
        vTaskStartScheduler();
    }

    /* If all is well, the scheduler will now be running, and the following
     * line will never be reached.  If the following line does execute, then
     * there was insufficient FreeRTOS heap memory available for the idle and/or
     * timer tasks	to be created.  See the memory management section on the
     * FreeRTOS web site for more details. */
    for( ; ; )
    {
    }
}
/*-----------------------------------------------------------*/

static void working( void * pvParameters )
{
    TickType_t xNextWakeTime;
    const TickType_t xBlockTime = WORKING_FREQUENCY_MS;
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK;

    /* Prevent the compiler warning about the unused parameter. */
    ( void ) pvParameters;

    /* Initialise xNextWakeTime - this only needs to be done once. */
    xNextWakeTime = xTaskGetTickCount();

    for( ; ; )
    {
        /* Place this task in the blocked state until it is time to run again.
        *  The block time is specified in ticks, pdMS_TO_TICKS() was used to
        *  convert a time specified in milliseconds into a time specified in ticks.
        *  While in the Blocked state this task will not consume any CPU time. */

        printf( "Working\n" );
        vTaskDelayUntil( &xNextWakeTime, xBlockTime );

        /* Send to the queue - causing the queue receive task to unblock and
         * write to the console.  0 is used as the block time so the send operation
         * will not block - it shouldn't need to block as the queue should always
         * have at least one space at this point in the code. */
        xQueueSend( xQueue, &ulValueToSend, 0U );
    }
}
/*-----------------------------------------------------------*/
static void temperature( void * pvParameters )
{
    TickType_t xNextWakeTime;
    const TickType_t xBlockTime = WORKING_FREQUENCY_MS;
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK;

    /* Prevent the compiler warning about the unused parameter. */
    ( void ) pvParameters;

    /* Initialise xNextWakeTime - this only needs to be done once. */
    xNextWakeTime = xTaskGetTickCount();

    for( ; ; )
    {
        /* Place this task in the blocked state until it is time to run again.
        *  The block time is specified in ticks, pdMS_TO_TICKS() was used to
        *  convert a time specified in milliseconds into a time specified in ticks.
        *  While in the Blocked state this task will not consume any CPU time. */

        // Convert a fixed Fahrenheit temperature value to degree Celsius
        int fahrenheit = 98;
        float celsius = (fahrenheit - 32) * 5 / 9;
        printf( "Temperature: %f\n", celsius );

        vTaskDelayUntil( &xNextWakeTime, xBlockTime );

        /* Send to the queue - causing the queue receive task to unblock and
         * write to the console.  0 is used as the block time so the send operation
         * will not block - it shouldn't need to block as the queue should always
         * have at least one space at this point in the code. */
        xQueueSend( xQueue, &ulValueToSend, 0U );
    }
}

static void Multiplication( void * pvParameters )
{
    TickType_t xNextWakeTime;
    const TickType_t xBlockTime = WORKING_FREQUENCY_MS;
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK;

    /* Prevent the compiler warning about the unused parameter. */
    ( void ) pvParameters;

    /* Initialise xNextWakeTime - this only needs to be done once. */
    xNextWakeTime = xTaskGetTickCount();

    for( ; ; )
    {
        /* Place this task in the blocked state until it is time to run again.
        *  The block time is specified in ticks, pdMS_TO_TICKS() was used to
        *  convert a time specified in milliseconds into a time specified in ticks.
        *  While in the Blocked state this task will not consume any CPU time. */

        //Define any two long int big numbers and multiply them, print the result
        long int a = 123456789, b = 987654321, c;
        c = a * b;
        printf( "Multiplication of %ld and %ld is %ld\n", a, b, c );


        vTaskDelayUntil( &xNextWakeTime, xBlockTime );

        /* Send to the queue - causing the queue receive task to unblock and
         * write to the console.  0 is used as the block time so the send operation
         * will not block - it shouldn't need to block as the queue should always
         * have at least one space at this point in the code. */
        xQueueSend( xQueue, &ulValueToSend, 0U );
    }
}

static void BinarySearch( void * pvParameters )
{
    TickType_t xNextWakeTime;
    const TickType_t xBlockTime = WORKING_FREQUENCY_MS;
    const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK;

    /* Prevent the compiler warning about the unused parameter. */
    ( void ) pvParameters;

    /* Initialise xNextWakeTime - this only needs to be done once. */
    xNextWakeTime = xTaskGetTickCount();

    for( ; ; )
    {
        /* Place this task in the blocked state until it is time to run again.
        *  The block time is specified in ticks, pdMS_TO_TICKS() was used to
        *  convert a time specified in milliseconds into a time specified in ticks.
        *  While in the Blocked state this task will not consume any CPU time. */

        //Binary search a list of 50 elements for a given value
        int recherche=10;
        int ma_liste[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
                          16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
                          31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,
                          46,47,48,49,50};
        int a;
        int cond = 0;
        int index=0;
        
        for (int i = 0; i < sizeof(ma_liste) / sizeof(ma_liste[0]); i++) {
            if (ma_liste[i] == recherche) {
                printf("La valeur %d est dans le tableau à la position %d\n", recherche, i);
                cond = 1;
                index=i;
            }
            
        }      



        vTaskDelayUntil( &xNextWakeTime, xBlockTime );

        /* Send to the queue - causing the queue receive task to unblock and
         * write to the console.  0 is used as the block time so the send operation
         * will not block - it shouldn't need to block as the queue should always
         * have at least one space at this point in the code. */
        xQueueSend( xQueue, &ulValueToSend, 0U );
    }
}
