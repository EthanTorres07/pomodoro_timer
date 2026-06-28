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

RTCDriver::RTCDriver(I2C_HandleTypeDef* i2cHandle, uint8_t mainAddress)
: m_i2cHandle(i2cHandle), m_i2cAddress(mainAddress)
{
}

/**
 * @brief Sends an I2C request to fill the TimeStamp field
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
    m_timeBuffer.second &= mask;
    m_timeBuffer.minute &= mask;
    m_timeBuffer.hour &= mask;
    m_timeBuffer.date &= mask;
    m_timeBuffer.month &= mask;
    m_timeBuffer.year &= mask;
}
