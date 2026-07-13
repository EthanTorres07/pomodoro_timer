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
#include "helper.h"
#include <string.h>

RTCDriver::RTCDriver(I2C_HandleTypeDef* i2cHandle, uint8_t mainAddress)
: m_i2cHandle(i2cHandle), m_i2cAddress(mainAddress)
{
}

/**
 * @brief Sends an I2C request to fill the TimeStamp field and returns the field
 * @retval the buffer containing the current time stamp
 */
TimeStamp RTCDriver::getDateAndTime()
{
    HAL_I2C_Mem_Read_DMA(
            m_i2cHandle,
            m_i2cAddress,
            REG_SECONDS,
            I2C_MEMADD_SIZE_8BIT,
            (uint8_t*) &m_timeBuffer,
            sizeof(m_timeBuffer));

    // Extract necessary values from transmitted data
    uint8_t mask = 0xF;
    m_timeBuffer.second &= (mask | 0x7);
    m_timeBuffer.minute &= (mask | 0x7);
    m_timeBuffer.hour &= (mask | 0x03);
    m_timeBuffer.date &= (mask | 0x03);
    m_timeBuffer.month &= (mask | 0x01);
    m_timeBuffer.year &= (mask | 0xF0);

    m_timeBuffer.second = packed_bcd_to_int(m_timeBuffer.second);
    m_timeBuffer.minute = packed_bcd_to_int(m_timeBuffer.minute);
    m_timeBuffer.hour = packed_bcd_to_int(m_timeBuffer.hour);
    m_timeBuffer.date = packed_bcd_to_int(m_timeBuffer.date);
    m_timeBuffer.month = packed_bcd_to_int(m_timeBuffer.month);
    m_timeBuffer.year = packed_bcd_to_int(m_timeBuffer.year);





    return m_timeBuffer;
}

/**
 * @brief Parse the compilation date and time
 */
void getCompileTime(uint8_t *sec, uint8_t *min, uint8_t *hour, uint8_t *date,
        uint8_t *month, uint8_t *year) {
    // 1. Parse __TIME__ ("HH:MM:SS")
    const char* timeStr = __TIME__;
    uint8_t rawHour = (timeStr[0] - '0') * 10 + (timeStr[1] - '0');
    uint8_t rawMin  = (timeStr[3] - '0') * 10 + (timeStr[4] - '0');
    uint8_t rawSec  = (timeStr[6] - '0') * 10 + (timeStr[7] - '0');

    // 2. Parse __DATE__ ("Mmm DD YYYY")
    const char* dateStr = __DATE__;
    uint8_t rawDate = (dateStr[4] == ' ' ? 0 : dateStr[4] - '0') * 10 +
            (dateStr[5] - '0');
    uint8_t rawYear = (dateStr[9] - '0') * 10 + (dateStr[10] - '0');

    // Find the month index (1-12)
    uint8_t rawMonth = 1;
    const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; i++) {
        if (strncmp(dateStr, months[i], 3) == 0) {
            rawMonth = i + 1;
            break;
        }
    }

    // 3. Convert all values to BCD for the DS3231 registers
    *sec   = decToBcd(rawSec);
    *min   = decToBcd(rawMin);
    *hour  = decToBcd(rawHour); // Defaults to 24-hour mode formatting
    *date  = decToBcd(rawDate);
    *month = decToBcd(rawMonth);
    *year  = decToBcd(rawYear);

}

void RTCDriver::init()
{
    TimeStamp ts;
    getCompileTime(&ts.second, &ts.minute, &ts.hour, &ts.date, &ts.month,
            &ts.year);

    // Write initial time to RTC
    HAL_I2C_Mem_Write_DMA(m_i2cHandle,
            m_i2cAddress,
            REG_SECONDS,
            I2C_MEMADD_SIZE_8BIT,
            (uint8_t*) &ts,
            sizeof(ts));

}
