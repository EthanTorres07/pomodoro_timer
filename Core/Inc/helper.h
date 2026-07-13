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
uint8_t decToBcd(uint8_t val);
void getCompileTime(uint8_t *sec, uint8_t *min, uint8_t *hour, uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year);



#endif /* INC_HELPER_H_ */
