#include <stdio.h>
#include <conio.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h" 
#include "queue.h"
#include "timers.h"
 
/*
 * Prototypes for the standard FreeRTOS application hook (callback) functions
 * implemented within this file.  See http://www.freertos.org/a00016.html .
 */
void vApplicationMallocFailedHook(void);
void vApplicationDaemonTaskStartupHook(void);

/* Notes if the trace is running or not. */
static BaseType_t xTraceRunning = pdFALSE;


// My timers

static uint32_t ulCallCount = 0;
 
static void prvAutoReloadTimerCallback(TimerHandle_t xTimer);


int main(void) {
	TimerHandle_t xAutoReloadTimer;
	 
	BaseType_t xTimerStarted;

	/* Create your timer */
	xAutoReloadTimer = xTimerCreate(
		"Auto reload",
		pdMS_TO_TICKS(1000),
		pdTRUE,
		(void *)1,
		prvAutoReloadTimerCallback);

	/* start your timer */
	if (xAutoReloadTimer != NULL) {
		xTimerStarted = xTimerStart(xAutoReloadTimer, 0);
	}
	else {
		printf("Error on timer init\n");
	}

	/* Start the scheduler. */
	vTaskStartScheduler();

	for (;;);

}

// Implement your timer task
void prvAutoReloadTimerCallback(TimerHandle_t xTimer) {
	TickType_t xTimeNow;
	// get the tick count
	xTimeNow = xTaskGetTickCount();
	printf("Auto reload time call back executing @ %d\n", xTimeNow);
}
 
 

void vAssertCalled(unsigned long ulLine, const char* const pcFileName)
{
	static BaseType_t xPrinted = pdFALSE;
	volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

	/* Called if an assertion passed to configASSERT() fails.  See
	http://www.freertos.org/a00110.html#configASSERT for more information. */

	/* Parameters are not used. */
	(void)ulLine;
	(void)pcFileName;

	printf("ASSERT! Line %ld, file %s, GetLastError() %ld\r\n", ulLine, pcFileName, GetLastError());

	taskENTER_CRITICAL();
	{
		/* Stop the trace recording. */
		if (xPrinted == pdFALSE)
		{
			xPrinted = pdTRUE;
			if (xTraceRunning == pdTRUE)
			{
				// vTraceStop();
				// prvSaveTraceFile();
			}
		}

		/* Cause debugger break point if being debugged. */
		__debugbreak();

		/* You can step out of this function to debug the assertion by using
		the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
		value. */
		while (ulSetToNonZeroInDebuggerToContinue == 0)
		{
			__asm { NOP };
			__asm { NOP };
		}
	}
	taskEXIT_CRITICAL();
}


void vApplicationMallocFailedHook(void)
{
	vAssertCalled(__LINE__, __FILE__);
}


void vApplicationDaemonTaskStartupHook(void)
{
	// printf("Timer Pre-Initlaization code\r\n....................\n");
}


