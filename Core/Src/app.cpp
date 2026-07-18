/**
 * @file    app.cpp
 * @brief   Main application for pomodoro timer
 *
 * @author  Ethan Torres
 * @date    Jun 28, 2026
 * @version 1.0.0
 * @copyright Copyright (c) 2026 All rights reserved.
 */

#include "main.h"
#include "rotary_encoder.hpp"
#include "rtc.hpp"
#include "i2c.h"
#include "spi.h"
#include "oled.hpp"
#include "app.hpp"
#include "font5x7.hpp"
#include <string.h>

extern "C"
{
    volatile uint32_t _epochTime = 0;
}

static uint32_t timerValue = 0;

static State systemState = STARTUP;

static uint32_t lastSyncTime;


/**
 * Contains the interrupt-driven main application
 */
void run()
{
    systemState = STARTUP;

    RotaryEncoder encoder(0, 0, 0, 0);
    RTCDriver rtc(&hi2c1, 0x00); // Edit with actual address ***************
    Oled oled(&hspi1);

    systemInit(rtc, oled);
}

/**
 * @brief Syncs epochTime with parameter currTime
 * @param currTime TimeStamp from RTC
 */
void syncEpochTime(TimeStamp currTime)
{
    uint16_t year = currTime.year;
    uint8_t month = currTime.month;
    uint16_t day = currTime.date;
    uint16_t hour = currTime.hour;
    uint16_t minute = currTime.minute;
    uint16_t second = currTime.second;

    // Adjust month and year if it's January or February
    if (month <= 2)
    {
        month += 12;
        year -= 1;
    }
    // Calculate days since March 1, 0000 using the Gregorian calendar rules
    uint32_t era = year / 400;
    uint16_t yOfEra = year % 400;
    uint32_t dayOfYear = (153 * (month - 3) + 2) / 5 + day - 1;
    uint32_t dayOfEra = yOfEra * 365 + yOfEra / 4 - yOfEra / 100 +
            dayOfYear;
    uint32_t totalDays = era * 146097 + dayOfEra;

    // Subtract the number of days between 0000-03-01 and the Unix Epoch
    // (1970-01-01)
    int32_t days_since_epoch = totalDays - 719468;

    // Convert days to seconds and add the current day's time
    _epochTime = (days_since_epoch * 86400UL) +
                              (hour * 3600UL) +
                              (minute * 60UL) +
                              second;

    lastSyncTime = HAL_GetTick();
}

/**
 * @brief Initializes objects of application
 * @param rtc     real-time clock of timer
 * @param oled    main oled display of timer
 */
void systemInit(RTCDriver& rtc, Oled& oled)
{
    rtc.init();
    oled.init();
}

/**
 * @brief Checks system inputs
 * @retval The input that was provided
 */
static UserEvent taskCheckInputs()
{

    if (g_buttonPressed)
    {
        g_buttonPressed = false;
        return BUTTON_PRESS;
    }

    // Reset and read encoder
    // Implemented this way due to ISR structure (see stm32f4xx_it.c)
    __disable_irq();
    uint8_t delta = g_counter;
    g_counter = 0;
    __enable_irq();

    if (delta > 0)
    {
        return ENCODER_UP;
    }
    else if (delta < 0)
    {
        return ENCODER_DOWN;
    }

    return NO_EVENT;
}

/**
 * @brief Changes system state based on inputs/system variables
 * @param event the user input last done on the system
 */
static void changeState(UserEvent event)
{
    switch (systemState)
    {
    case STARTUP:
        // Inputs do nothing during startup
        break;

    case SETUP:
        if (event == BUTTON_PRESS)
        {
            // Start timer
            startTimer();
            systemState = TIMER_ACTIVE;
        }
        else if (event == ENCODER_DOWN)
        {
            // Decrease timer by 30 seconds
            if (timerValue > 0)
            {
                timerValue -= 30;
            }
        }
        else if (event == ENCODER_UP)
        {

            // Increase timer by 30 seconds
            timerValue += 30;
        }
        break;

    case TIMER_ACTIVE:
        if (event == BUTTON_PRESS)
        {
            // Stop timer
            timerValue = 0;
            systemState = TIMER_FINISHED;
        }
        break;

    case TIMER_FINISHED:
        if (event == BUTTON_PRESS)
        {
            // Add vibration and ringer

            // Go back to setup
            systemState = SETUP;
            timerValue = 0;
        }
    }
}

/**
 * @brief Checks if timer is finished and syncs _epochTime with RTC
 * @param rtc RTC being interacted with
 */
static void taskUpdateTime(RTCDriver& rtc)
{
    if (systemState == TIMER_ACTIVE && timerValue - _epochTime == 0)
    {
        systemState = TIMER_FINISHED;
    }

    // If its been an hour and there is no active timer sync the onboard time
    // with RTC (1 hour sync time is based on specs of RTC)
    if (systemState == SETUP && (lastSyncTime - HAL_GetTick() > (3600 * 1000)))
    {
        syncEpochTime(rtc.getDateAndTime());
    }
}

/**
 * @brief Starts the timer by adding timer value to current epoch time to be
 * compared to upon interrupt
 */
static void startTimer()
{
    timerValue += _epochTime;
}

/**
 * @brief Updates display based on state
 * @param oled OLED being updated
 */
static void taskUpdateDisplay(Oled& oled)
{
    switch (systemState)
    {
    case STARTUP:
    {
        const char *startupString = "Initializing...";

        // Offset coordinates because drawString() draws coordinates are from
        // beginning of string
        uint8_t centerX = (128 - (FONT_WIDTH * strlen(startupString))) / 2;
        uint8_t centerY = (64 - FONT_HEIGHT) / 2;
        oled.drawString(startupString, centerX, centerY);
        break;
    }
    case SETUP:
        break;
    }
}

/**
 * @brief Returns a string of the current date and time based on _epochTime
 * @retval date and time in the form of MM/DD/YY HH:SS
 */
static char *getDateAndTimeString()
{

}
