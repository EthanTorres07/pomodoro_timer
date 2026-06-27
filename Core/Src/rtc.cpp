/**
 * @file    rtc.cpp
 * @brief   API for the DS3231 real-time clock
 *
 * @author  Ethan Torres
 * @date    Jun 23, 2026
 * @version 1.0.0
 * @copyright Copyright (c) 2026 All rights reserved.
 */

#include "rtc.hpp"

RTCDriver::RTCDriver(I2C_HandleTypeDef* i2cHandle, uint8_t mainAddress, uint32_t year,
        uint32_t month, uint32_t day, uint32_t hour, uint32_t minute,
        uint32_t second) : m_i2cHandle(i2cHandle), m_i2cAddress(mainAddress),
        m_year(year), m_month(month), m_day(day), m_hour(hour),
        m_minute(minute), m_second(second), m_remainingTime(0)
{
}
