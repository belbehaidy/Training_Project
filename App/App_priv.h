/*
 * App_priv.h

 *
 *  Created on: Aug 18, 2022
 *      Author: Bassem EL-Behaidy
 */

#ifndef APP_APP_PRIV_H_
#define APP_APP_PRIV_H_

#define DISPLAY_WELCOME_MESSAGE		do{\
										LCD_enuWriteCommand(0x01);\
										LCD_enuWriteCommand(0xC);\
										LCD_enuGoToPosition(2,4);\
										LCD_enuWriteString("Welcome...");\
									}while(0)
#define DISPLAY_NUMBER_MESSAGE		do{\
										LCD_enuWriteCommand(0x01);\
										LCD_enuGoToPosition(1,3);\
										LCD_enuWriteString("Enter Number");\
										LCD_enuWriteCommand(0x0F);\
										LCD_enuGoToPosition(2,3);\
									}while(0)
#define DISPLAY_COUNTER_MESSAGE		do{\
										LCD_enuWriteCommand(0xC);\
										LCD_enuWriteCommand(0x01);\
										LCD_enuWriteString("Counter=");\
										LCD_enuGoToPosition(2,5);\
										LCD_enuWriteIntegerNum(Local_s32Num);\
									}while(0)


#define SHUTDOWN					9
#define POWER_OFF					10
#define POWER_ON					11
#define CONTINUE					12
#define RESTART						13
#define NO_COUNT					14
#define COUNT_UP					15
#define COUNT_DOWN					16

/****************************************************/
/*			Switches Configuration					*/
/****************************************************/
#define SWITCH_BOUNCE_DELAY			3
#define SWITCH_UNPRESSED			111

#define POWER_SWITCH				SW_ONE
#define POWER_SWITCH_PRESSED		DIO_u8HIGH

#define COUNTER_SWITCH				SW_TWO
#define COUNTER_SWITCH_PRESSED		DIO_u8HIGH


#endif /* APP_APP_PRIV_H_ */
