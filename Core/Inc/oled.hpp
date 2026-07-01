/**
 * @file    oled.hpp
 * @brief   API for a TBD OLED Display
 * @details Used to send data to be displayed
 *
 */

#ifndef INC_OLED_HPP_
#define INC_OLED_HPP_

#include "main.h"

class Oled
{
private:
    SPI_HandleTypeDef *m_hspi;
    const uint8_t      m_width;
    const uint8_t      m_height;
    const uint16_t     m_bufferSize;

public:
    Oled(SPI_HandleTypeDef *hspi);
    void init();
    void updateDisplay(buffer);

};




#endif /* INC_OLED_HPP_ */
