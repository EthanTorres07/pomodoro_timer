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
    uint8_t m_buffer[1024];

    bool drawPixel(uint8_t x, uint8_t y, bool turnOn);
    bool drawChar(char ch, uint8_t x, uint8_t y);

public:
    Oled(SPI_HandleTypeDef *hspi);
    void init();
    bool drawString(const char *string, uint8_t x, uint8_t y);

};




#endif /* INC_OLED_HPP_ */
