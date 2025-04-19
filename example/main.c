#include <stdio.h>
#include <windows.h>
#include "task.h"

static void app_1hz_handler(void)
{
    printf("[1Hz]   Task running\n");
}

static void app_10hz_handler(void)
{
    printf("[10Hz]  Task running\n");
}

static void app_100hz_handler(void)
{
    printf("[100Hz] Task running\n");
}

int main(void)
{
    // Register a few task handlers
    task_register_handler(TASK_1HZ, app_1hz_handler);
    task_register_handler(TASK_10HZ, app_10hz_handler);
    task_register_handler(TASK_100HZ, app_100hz_handler);

    printf("Task Scheduler Simulation (CTRL+C to exit)\n");

    while (1)
    {
        task_tick();    // Simulate 1ms timer tick
        task_handler(); // Run any ready tasks

        Sleep(1); // Delay ~1ms
    }

    return 0;
}
