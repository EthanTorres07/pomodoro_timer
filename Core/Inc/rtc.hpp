/**
 * @file    rtc.hpp
 * @brief   API for the DS3231 real-time clock
 * @details Uses registers on the DS3231 to show real world time as well as
 *          the on-chip oscillator to cause a tick every second to create a
 *          timer
 *
 * @note    [Hardware/Configuration requirements]
 * @warning [Critical runtime/safety warnings]
 */

#ifndef INC_RTC_HPP_
#define INC_RTC_HPP_
#pragma pack(push, 1)


#include "main.h"
#include "globals.h"

#define RTC_BUFFER_SIZE 7

class RTCDriver
{
private:
    I2C_HandleTypeDef* m_i2cHandle;
    uint8_t m_i2cAddress;

    struct
    {
        uint8_t second;
        uint8_t minute;
        uint8_t hour;
        uint8_t day;
        uint8_t date;
        uint8_t month;
        uint16_t year;
     } TimeStamp;

    TimeStamp m_timeBuffer;
    uint32_t m_remainingTime;

    enum Register : uint8_t
    {
        REG_SECONDS = 0x00,
        REG_MINUTES = 0x01,
        REG_HOURS   = 0x02,
        REG_DAY     = 0x03,
        REG_DATE    = 0x04,
        REG_MONTH   = 0x05,
        REG_YEAR    = 0x06
    };

public:
    RTCDriver(I2C_HandleTypeDef* i2cHandle, uint8_t i2cAddress, uint32_t year,
            uint32_t month, uint32_t day, uint32_t hour, uint32_t minute,
            uint32_t second);
    TimeStamp getDateAndTime();

};

#pragma pack(pop)
#endif /* INC_RTC_HPP_ */

