/**
 * @file    app.hpp
 * @brief   Main application for pomodoro timer
 *
 */

#ifndef APP_H
#define APP_H

#include "rtc.hpp"

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint32_t _epochTime;

#ifdef __cplusplus
}
#endif

void run();

void syncEpochTime(TimeStamp currTime);

#endif
