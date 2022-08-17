/*
 * LCD_int.h
 *
 *  Created on: Aug 12, 2022
 *      Author: Ahmed El-Gaafrawy
 */

#ifndef LCD_INT_H_
#define LCD_INT_H_


/****************************************/
/*        Text Direction Options		*/
/****************************************/
#define ENGLISH		DISPLAY_DIR_RIGHT	//Default
#define ARABIC		DISPLAY_DIR_LEFT

/****************************************/
/* 			LCD APIs					*/
/****************************************/

ES_t LCD_enuInit(void);

ES_t LCD_enuWriteCommand(u8 Copy_u8Cmnd);

ES_t LCD_enuWriteData(u8 Copy_u8Data);

ES_t LCD_enuSetTextDirection(u8 Copy_u8Direction); 	/* ENGLISH		/ 		ARABIC*/

ES_t LCD_enuGoToPosition(u8 Copy_u8Row , u8 Copy_u8Column);

ES_t LCD_enuWriteString(const char * Copy_pcString);

ES_t LCD_enuWriteExtraPattern(u8 * Copy_pu8Pattern , u8 Copy_u8NumOfPatterns , u8 Copy_u8Start , u8 Copy_u8DisplayDirection);

ES_t LCD_enuWriteIntegerNum (s32 Copy_s32Num);

ES_t LCD_enuWriteFloatNum   (f32 Copy_f32Num);

#endif /* LCD_INT_H_ */
