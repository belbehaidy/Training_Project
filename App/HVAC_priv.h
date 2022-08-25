/*
 * HVAC_priv.h
 *
 *  Created on: Aug 23, 2022
 *      Author: bassem El-Behaidy
 */

#ifndef _HVAC_PRIV_H_
#define _HVAC_PRIV_H_


typedef enum { ON , OFF } OnOff_t;
typedef enum { COLD , NORMAL , HOT } Temp_t;

//#define ABS_DIFF(x,y)	( ((x)-(y)) > 0 ? ((x)-(y)) : ((y)-(x)) )


#define DISPLAY_INIT_MESSAGE		do{\
										LCD_enuWriteCommand(0x01);\
										LCD_enuGoToPosition(1,6);\
										LCD_enuWriteString("Hi...");\
										LCD_enuWriteCommand(0x0C);\
									}while(0)
#define DISPLAY_SET_MODE_MESSAGE	do{\
										LCD_enuWriteCommand(0x01);\
										LCD_enuGoToPosition(1,5);\
										LCD_enuWriteString("Set mode");\
										LCD_enuGoToPosition(2,8);\
										LCD_enuWriteIntegerNum( (s32) Global_u8SetTemp );\
									}while(0)
#define DISPLAY_STATUS_MESSAGE( x )		do{\
										LCD_enuWriteCommand(0x01);\
										LCD_enuGoToPosition(1,3);\
										LCD_enuWriteString("Temperature");\
										LCD_enuGoToPosition(2,3);\
										LCD_enuWriteIntegerNum( (s32)x );\
										LCD_enuGoToPosition(2,12);\
										if( CompressorStatus == ON ) LCD_enuWriteString("ON");\
										else LCD_enuWriteString("OFF");\
									}while(0)
#define DISPLAY_CLOSE_MESSAGE		do{\
										LCD_enuWriteCommand(0x01);\
										LCD_enuGoToPosition(2,3);\
										LCD_enuWriteString("Bye..Bye!!!");\
									}while(0)

#define STANDBY_CHECK_DELAY				10
#define STANDBY_TOTAL_DELAYS			20

#define POWER_OFF					10
#define POWER_ON					11

#define INCREMENT					15
#define DECREMENT					16

#define TEMP_UNREAD					20
#define TEMP_READ					21



ES_t HVAC_enuInit(void);
void ADC_vidISR(void);
void EXTI_vidISR_INT0(void);
ES_t HVAC_enuSetup(void);
void HVAC_CheckTemperatureStatus(u16 Copy_u8ActTempValue );
void HVAC_AdjustCompressorStatus(void);
void HVAC_Awake(u16 *Copy_u16AvgTempValue);
void HVAC_Sleep(void);
void HVAC_StandByMode(void);
void HVAC_SetTemp(void);
void HVAC_UpdateReading(u16 *Copy_u16AvgTempValue );


#endif /* _HVAC_PRIV_H_ */
