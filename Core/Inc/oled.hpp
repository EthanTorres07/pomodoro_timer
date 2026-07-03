/**
 * @file    oled.hpp
 * @brief   API for a SSD1306 OLED display
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
    const uint8_t buffer[m_bufferSize];

    void drawPixel();

public:
    Oled(SPI_HandleTypeDef *hspi);
    void init();
    void updateDisplay(uint8_t x, uint8_t y, bool turnOn);

};




#endif /* INC_OLED_HPP_ */
