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

/**
 * @brief Main scheduler structure to hold all dynamic state variables.
 */
typedef struct
{
    uint32_t tick_count;                           // Global tick counter
    volatile uint8_t flags[TASK_COUNT];            // Execution flags
    uint8_t overflow_count[TASK_COUNT];            // Missed deadline counters
    task_handler_cb_t handler_cb[TASK_COUNT];      // Function pointers
} task_scheduler_t;

/* Private Variables ---------------------------------------------------------*/

// Configuration Table (Kept separate as it is CONSTANT data, saves RAM)
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

// The Singleton Scheduler Instance
// Initialized to 0 automatically by static rules, but explicit {0} is good practice.
static task_scheduler_t scheduler = {0};

/* Function Definitions ------------------------------------------------------*/

/**
 * @brief
 * Increment the tick count and set the flags for tasks based on their frequencies.
 */
void task_tick(void)
{
    scheduler.tick_count++;

    for (uint8_t i = 0; i < TASK_COUNT; i++)
    {
        // Check if it's time to run this task
        if (scheduler.tick_count % task_ticks[i] == 0)
        {
            if (scheduler.flags[i] == 1)
            {
                // Previous flag was not cleared, overflow occurred
                scheduler.overflow_count[i]++;
            }

            scheduler.flags[i] = 1;
        }
    }

    if (scheduler.tick_count >= TICK_1HZ)
    {
        scheduler.tick_count = 0; // Reset every 1 second
    }
}

/**
 * @brief
 * Check if any task flags are set. If a flag is set, reset it and call the handler.
 */
void task_handler(void)
{
    for (uint8_t i = 0; i < TASK_COUNT; i++)
    {
        if (scheduler.flags[i])
        {
            // Clear flag first to allow re-triggering if handler takes too long (optional safety)
            scheduler.flags[i] = 0;

            if (scheduler.handler_cb[i] != NULL)
            {
                scheduler.handler_cb[i]();
            }
        }
    }
}

/**
 * @brief
 * Register a task handler for a specific task type.
 */
void task_register_handler(task_type_t task_type, task_handler_cb_t handler)
{
    if (task_type < TASK_COUNT)
    {
        scheduler.handler_cb[task_type] = handler;
    }
}
