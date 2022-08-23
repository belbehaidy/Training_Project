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

#define ABS_DIFF(x,y)	( ((x)-(y)) > 0 ? ((x)-(y)) : ((y)-(x)) )


#define DISPLAY_INIT_MESSAGE		do{\
										LCD_enuWriteCommand(0x0C);\
										LCD_enuWriteCommand(0x01);\
										LCD_enuGoToPosition(1,6);\
										LCD_enuWriteString("Hi...");\
									}while(0)
#define DISPLAY_TEMPERATURE_MESSAGE		do{\
										LCD_enuWriteCommand(0x01);\
										LCD_enuGoToPosition(1,3);\
										LCD_enuWriteString("Temperature");\
										LCD_enuGoToPosition(2,3);\
									}while(0)
#define DISPLAY_CLOSE_MESSAGE		do{\
										LCD_enuWriteCommand(0x01);\
										LCD_enuGoToPosition(2,3);\
										LCD_enuWriteString("Bye..Bye!!!");\
									}while(0)


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
void HVAC_CheckTemperatureStatus( u8 Copy_u8ActTempValue );
void HVAC_AdjustCompressorStatus(void);
void HVAC_Awake(void);
void HVAC_Sleep(void);
void HVAC_StandByMode(void);
void HVAC_SetTemp(void);


#endif /* _HVAC_PRIV_H_ */
