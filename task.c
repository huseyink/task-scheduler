/******************************************************************************
 * File        : task.c
 * Author      : Huseyink
 * Date        : Apr 19, 2025
 * Version     : 1.0.0
 * Description : Task Frequency Scheduler Module
 *
 * This module provides a lightweight cooperative task scheduler based on a fixed 1ms system tick.
 * It manages multiple periodic tasks at predefined frequencies (1Hz to 200Hz) using a simple
 * flag mechanism and user-registered callback functions.
 *
 * Features:
 *  - Fixed frequency task execution: 1Hz, 2Hz, 5Hz, 10Hz, 20Hz, 50Hz, 100Hz, 200Hz
 *  - Efficient tick-based scheduling with minimal overhead
 *  - Task overflow detection (missed execution)
 *  - Flexible handler registration using function pointers
 *
 * Usage:
 *  - Call `task_tick()` from a 1ms tick interrupt or timer
 *  - Call `task_handler()` periodically from the main loop
 *  - Use `task_register_handle()` to assign handlers for each task frequency
 *  - Monitor execution reliability with `task_overflow_count`
 *
 * Limitations:
 *  - Only one handler per task frequency (no multi-handler support)
 *  - Tasks must execute quickly to avoid flag overflows
 *
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "task.h"

/* Defines/macros ------------------------------------------------------------*/

// Tick thresholds for each frequency (assuming 1ms tick)
#define TICK_1HZ     (1000)
#define TICK_2HZ     (500)
#define TICK_5HZ     (200)
#define TICK_10HZ    (100)
#define TICK_20HZ    (50)
#define TICK_50HZ    (20)
#define TICK_100HZ   (10)
#define TICK_200HZ   (5)

/* Types ---------------------------------------------------------------------*/

/* Private Variables ---------------------------------------------------------*/

// Global tick count
static uint32_t tick_count;

// Task tick values
static const uint16_t task_ticks[TASK_COUNT] =
{
    [TASK_1HZ]   = TICK_1HZ,
    [TASK_2HZ]   = TICK_2HZ,
    [TASK_5HZ]   = TICK_5HZ,
    [TASK_10HZ]  = TICK_10HZ,
    [TASK_20HZ]  = TICK_20HZ,
    [TASK_50HZ]  = TICK_50HZ,
    [TASK_100HZ] = TICK_100HZ,
    [TASK_200HZ] = TICK_200HZ
};

// Global flags for each task
volatile uint8_t task_flags[TASK_COUNT] = {0};

// Overflow counters for each task
static uint8_t task_overflow_count[TASK_COUNT] = {0};

// Callback array
static task_handler_cb_t task_handler_cb[TASK_COUNT] = {0};

/* Private Function Prototypes -----------------------------------------------*/

/* Private Function Definitions ----------------------------------------------*/

/* Function Definitions ------------------------------------------------------*/

/**
 * @brief
 *
 * Increment the tick count and set the flags for tasks based on their frequencies.
 * The task_flags are set to 1 when their respective frequency interval has passed.
 *
 * Resets tick_count after reaching 1 second (1000 ms).
 */
void task_tick(void)
{
	tick_count++;

	for (int i = 0; i < TASK_COUNT; i++)
	{
		if (tick_count % task_ticks[i] == 0)
		{
			if (task_flags[i] == 1)
			{
				// Previous flag was not cleared, overflow occurred
				task_overflow_count[i]++;
			}

			task_flags[i] = 1;
		}
	}

	if (tick_count >= TICK_1HZ)
	{
		tick_count = 0; // Reset every 1 second
	}
}

/**
 * @brief
 *
 * Check if any task flags are set. If a flag is set, reset it and call the respective task handler.
 * This function processes the tasks that are ready to run (based on frequency).
 */
void task_handler(void)
{
	for (uint8_t i = 0; i < TASK_COUNT; i++)
	{
		if (task_flags[i])
		{
			task_flags[i] = 0;
			if (task_handler_cb[i] != NULL)
			{
				task_handler_cb[i]();
			}
		}
	}
}

/**
 * @brief
 *
 * Register a task handler for a specific task type.
 * The handler will be called when the task flag for that type is set.
 *
 * @param task_type   The task type (e.g., TASK_1HZ, TASK_2HZ, etc.)
 * @param handler     The function to be called for the task
 */
void task_register_handler(task_type_t task_type, task_handler_cb_t handler)
{
    if (task_type < TASK_COUNT) {
        task_handler_cb[task_type] = handler;
    }
}
