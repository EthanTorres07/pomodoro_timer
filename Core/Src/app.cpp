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
#include <ctime>
#include <stdio.h>

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
            toggleVibrate();
            toggleBuzzer();

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
 *
 * @note Coordinates must be offset because drawString() draws coordinates are
 * from beginning of string
 *
 */
static void taskUpdateDisplay(Oled& oled)
{
    uint8_t x;
    uint8_t y;
    switch (systemState)
    {
    case STARTUP:
    {
        // Message in dead center
        const char *startupString = "Initializing...";

        x = (128 - (FONT_WIDTH * strlen(startupString))) / 2;
        y = (64 - FONT_HEIGHT) / 2;
        oled.drawString(startupString, x, y);
        break;
    }

    // SETUP and TIMER_ACTIVE do same thing in this function
    case SETUP:
    case TIMER_ACTIVE:
    {
        // Date and time centered in upper third
        char dateAndTime[25];
        getDateAndTimeString(dateAndTime);
        x = (128 - (FONT_WIDTH * strlen(dateAndTime))) / 2;
        y = (64 - FONT_HEIGHT) / 3;

        // Timer in dead center
        char timerString[9];
        getTimerString(timerString);

        x = (128 - (FONT_WIDTH * strlen(timerString))) / 2;
        y = (64 - FONT_HEIGHT) / 2;
        oled.drawString(timerString, x, y);
        break;
    }

    case TIMER_FINISHED:
    {
        // Date and time centered in upper third
        char dateAndTime[25];
        getDateAndTimeString(dateAndTime);
        x = (128 - (FONT_WIDTH * strlen(dateAndTime))) / 2;
        y = (64 - FONT_HEIGHT) / 3;

        // Message in dead center (blinking)
        char message[] = "TIMER FINISHED";
        bool isVisible = true;
        x = (128 - (FONT_WIDTH * strlen(message))) / 2;
        y = (64 - FONT_HEIGHT) / 2;


        uint32_t currTime = HAL_GetTick();
        static uint32_t lastUpdate = currTime;

        if (currTime - lastUpdate >= 1000)
        {
            isVisible = !isVisible;
        }
        if (isVisible)
        {
            oled.drawString(message, x, y);

        }
        else
        {
            // Clear by drawing a string the same length as message
            oled.drawString("              ", x, y);
        }

        lastUpdate = currTime;

        break;
    }
    }
}

/**
 * @brief Returns a string of the current date and time based on _epochTime
 * using ctime function acstime
 * @param dateAndTime buffer to put time in
 * @retval date and time in the form of Www Mmm dd hh:mm:ss yyyy\n
 */
static void getDateAndTimeString(char *dateAndTime)
{
    // Convert epoch to a structural representation of local time
    struct tm *timeinfo = localtime((const time_t*) _epochTime);

    dateAndTime = asctime(timeinfo);

}

/**
 * @brief returns string containing live timer value
 * @param timerString timer string buffer
 * @retval the formatted timer string in HH:MM:SS format
 */
static void getTimerString(char *timerString)
{
    // 1. Calculate time components
    uint32_t hours   = timerValue / 3600;
    uint32_t minutes = (timerValue % 3600) / 60;
    uint32_t seconds = timerValue % 60;

    // 2. Format into an HH:MM:SS string

    // "%02u" forces 2 digits with leading zeros if needed
    snprintf(timerString, sizeof(*timerString), "%02u:%02u:%02u", hours, minutes,
            seconds);

}

/**
 * @brief toggles vibration motor
 */
static void toggleVibrate()
{
    HAL_GPIO_TogglePin(GPIOC, VIB_OUT_Pin);
}

/**
 * @brief toggles buzzer
 */
static void toggleBuzzer()
{
    HAL_GPIO_TogglePin(GPIOC, BZR_OUT_Pin);
}
