/**
 * @file    globals.h
 * @brief   Global variables used in the project
 *
 */

#ifndef INC_GLOBALS_H_
#define INC_GLOBALS_H_

#include <stdbool.h>

#define G_DEBOUNCE_TIME_MS 5

enum TimerStatus
{
    FINISHED,
    IN_PROGRESS
};

extern volatile int32_t g_counter;
extern volatile bool g_checkTimer;
extern volatile bool g_buttonPressed;





#endif /* INC_GLOBALS_H_ */
