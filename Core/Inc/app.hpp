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



enum UserEvent
{
    BUTTON_PRESS,
    ENCODER_DOWN,
    ENCODER_UP,
    NO_EVENT
};

enum State
{
    STARTUP,
    SETUP,
    TIMER_ACTIVE,
    TIMER_FINISHED
};
void run();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

void syncEpochTime(TimeStamp currTime);

void systemInit(RTCDriver& rtc, Oled& oled);

static UserEvent taskCheckInputs();

static void changeState(UserEvent event);

static void startTimer();

static void taskUpdateTime(RTCDriver& rtc);

static void taskUpdateDisplay(Oled& oled);

static void getDateAndTimeString(const char *dateAndTime);

static void getTimerString(char *timerString);

#endif /* __cplusplus */\

#endif /* APP_H */
