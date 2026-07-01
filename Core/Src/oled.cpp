/**
 * @file    oled.cpp
 * @brief   API for SSD1306 OLED display
 *
 * @author  Ethan Torres
 * @date    Jul 1, 2026
 * @version 1.0.0
 * @copyright Copyright (c) 2026 All rights reserved.
 */

#include "oled.hpp"
#include "font5x7.hpp"

Oled::Oled(SPI_HandleTypeDef *hspi) : m_hspi(hspi), m_width(128), m_height(64),
m_bufferSize(1024)
{
}

void Oled::init()
{
    // Array of commands to be sent for display initialization
    uint8_t initCommands[] =
    {
            0xAE,       // 1. Turn Display OFF (Sleep Mode)

            0x20, 0x00, // 2. Set Memory Addressing Mode to Horizontal Mode

            0x40,       // 3. Set Display Start Line to 0

            0xA1,       // 4. Set Segment Re-map (Horizontal Flip: Map column 127 to SEG0)
            0xC8,       // 5. Set COM Output Scan Direction (Vertical Flip)

            0xA8, 0x3F, // 6. Set Multiplex Ratio (128x64 pixels uses 63, which is 0x3F)

            0xD3, 0x00, // 7. Set Display Offset to 0 (No vertical shift)

            0xDA, 0x12, // 8. Set COM Pins Hardware Configuration (Alternative configuration)

            0xD5, 0x80, // 9. Set Display Clock Divide Ratio/Oscillator Frequency (Default recommended)

            0xD9, 0xF1, // 10. Set Pre-charge Period (Controls electrical stability)

            0xDB, 0x40, // 11. Set VCOMH Deselect Level

            0x8D, 0x14, // 12. CRITICAL: Enable Internal Charge Pump (0x14 = Enable, 0x10 = Disable)

            0xA4,       // 13. Entire Display ON (Resume to follow GDDRAM RAM content)
            0xA6,       // 14. Set Normal/Inverse Display (0xA6 = Normal, 0xA7 = Inverted color)

            0xAF        // 15. Turn Display ON (Exit Sleep Mode)
    };

    HAL_SPI_Transmit_DMA(m_hspi, initCommands, sizeof(initCommands));
}
