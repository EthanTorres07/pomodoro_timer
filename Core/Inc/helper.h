/**
 * @file    helper.h
 * @brief   Helper functions
 */

#ifndef INC_HELPER_H_
#define INC_HELPER_H_


/**
 * Converts a packed BCD value (e.g., 0x596) to a standard integer.
 * Returns -1 if an invalid BCD digit (> 9) is detected.
 */
int packed_bcd_to_int(unsigned int bcd_val);


#endif /* INC_HELPER_H_ */
