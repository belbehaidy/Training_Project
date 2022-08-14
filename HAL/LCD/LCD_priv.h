/*
 * LCD_priv.h
 *
 *  Created on: Aug 12, 2022
 *      Author: Ahmed El-Gaafrawy
 */

#ifndef LCD_PRIV_H_
#define LCD_PRIV_H_


static ES_t LCD_enuWriteNLatch(u8 Copy_u8Byte);

#define NC					32

#define EIGHT_BIT			78
#define FOUR_BIT			21

#define ZERO_ASCII			'0'
#define MINUS_SIGN			'-'
#define DOT					'.'

#define CGROM_BASE_ADD		0x40

#define PATTERN_1			101
#define PATTERN_2			102
#define PATTERN_3			103
#define PATTERN_4			104
#define PATTERN_5			105
#define PATTERN_6			106
#define PATTERN_7			107
#define PATTERN_8			108

#define PATTERN_BYTES		8

#define DISPLAY_DIR_RIGHT		110
#define DISPLAY_DIR_LEFT		111




#endif /* LCD_PRIV_H_ */
