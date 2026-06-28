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

uint32_t _epochTime;

void run()
{
    RotaryEncoder encoder(0, 0, 0, 0);
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

}
