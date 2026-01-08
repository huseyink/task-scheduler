/******************************************************************************
 * File        : task.h
 * Author      : Huseyink
 * Date        : Apr 19, 2025
 * Version     : 1.0.0
 * Description : Task Frequency Scheduler Module
 ******************************************************************************/

#ifndef TASK_H_
#define TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Macros --------------------------------------------------------------------*/
#ifndef NULL
#define NULL ((void *)0)
#endif

/* Types ---------------------------------------------------------------------*/

// Callback function type
typedef void (*task_handler_cb_t)(void);

// Task types
typedef enum
{
    TASK_1HZ = 0,
    TASK_2HZ,
    TASK_5HZ,
    TASK_10HZ,
    TASK_20HZ,
    TASK_50HZ,
    TASK_100HZ,
    TASK_200HZ,

    TASK_COUNT
} task_type_t;

/* Function Prototypes -------------------------------------------------------*/
void task_tick(void);
void task_handler(void);
void task_register_handler(task_type_t task_type, task_handler_cb_t handler);

#ifdef __cplusplus
}
#endif

#endif /* TASK_H_ */
