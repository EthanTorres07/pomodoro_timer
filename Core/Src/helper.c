/**
 * @file    helper.c
 * @brief   [Internal implementation wrapper]
 *
 * @author  Ethan Torres
 * @date    Jul 9, 2026
 * @version 1.0.0
 * @copyright Copyright (c) 2026 All rights reserved.
 */

#include <stdint.h>


uint16_t packed_bcd_to_int(uint16_t bcd_val) {
    int result = 0;
    int multiplier = 1;

    while (bcd_val > 0) {
        // Extract the lowest 4 bits (nibble)
        unsigned char nibble = bcd_val & 0x0F;

        // BCD digits must be between 0 and 9
        if (nibble > 9) {
            return -1;
        }

        result += nibble * multiplier;
        multiplier *= 10;
        bcd_val >>= 4; // Shift right by 4 bits to get the next nibble
    }

    return result;
}
