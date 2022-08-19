/*
 * Keypad_int.h
 *
 *  Created on: Aug 13, 2022
 *      Author: Ahmed El-Gaafrawy
 */

#ifndef HAL_KEYPAD_KEYPAD_INT_H_
#define HAL_KEYPAD_KEYPAD_INT_H_

ES_t Keypad_enuInit(void);

ES_t Keypad_Sleep(void);

ES_t Keypad_Awake(void);

ES_t Keypad_enuGetPressedKey(u8 * Copy_pu8KeyValue);

#define KEYPAD_NOT_PRESSED				255

#endif /* HAL_KEYPAD_KEYPAD_INT_H_ */
