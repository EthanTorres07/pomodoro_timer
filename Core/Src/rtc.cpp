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
