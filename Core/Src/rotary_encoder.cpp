/**
 * @file    rotary_encoder.cpp
 * @brief   API for a rotary encoder
 *
 * @author  Ethan Torres
 * @date    Jun 23, 2026
 * @version 1.0.0
 * @copyright Copyright (c) 2026 All rights reserved.
 */

#include "main.h"
#include "rotary_encoder.hpp"
#include <stdio.h>

/**
 * @brief RotaryEncoder constructor
 * @param port  The port the encoder pins are connected to
 * @param pinA  Pin A
 * @param pinB  Pin B
 * @param pinSW The push button pin
 */
RotaryEncoder::RotaryEncoder(GPIO_TypeDef* port, uint16_t pinA, uint16_t pinB,
        uint16_t pinSW) : m_port(port), m_pinA(pinA), m_pinB(pinB),
        m_pinSW(pinSW), m_count(0)
{
}

/**
 * @brief Update the encoder's count
 * @param count  The new value of m_count
 */
void RotaryEncoder::updateCount(uint16_t count)
{
    m_count = count;
}
