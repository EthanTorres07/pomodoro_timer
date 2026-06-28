/**
 * @file    app.hpp
 * @brief   Main application for pomodoro timer
 *
 */

#ifndef APP_H
#define APP_H

#include "rtc.hpp"

void run();

void syncEpochTime(TimeStamp currTime);

#endif
