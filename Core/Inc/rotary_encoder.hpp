/**
 * @file    rotary_encoder.hpp
 * @brief   API for a quadrature rotary encoder module
 * @details Contains current count of encoder and method to update the count
 *
 */

#ifndef INC_ROTARY_ENCODER_HPP_
#define INC_ROTARY_ENCODER_HPP_
#pragma pack(push, 1)


class RotaryEncoder
{
private:
    GPIO_TypeDef* m_port;
    uint16_t m_pinA;
    uint16_t m_pinB;
    uint16_t m_pinSW;
    uint32_t m_count;

public:
    RotaryEncoder(GPIO_TypeDef* port, uint16_t pinA, uint16_t pinB,
            uint16_t pinSW);
    void updateCount(uint16_t count);
};

#pragma pack(pop)
#endif /* INC_ROTARY_ENCODER_HPP_ */
