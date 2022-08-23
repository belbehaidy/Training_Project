/*
 * HVAC_CONTROL.c

 *
 *  Created on: Aug 21, 2022
 *      Author: Bassem El-Behaidy
 */

#include <util/delay.h>
#include "../Libraries/stdTypes.h"
#include "../Libraries/errorState.h"

#include "../MCAL/DIO/DIO_int.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/ADC/ADC_int.h"
#include "../MCAL/GIE/GIE_int.h"

#include "../HAL/Keypad/Keypad_int.h"
#include "../HAL/LCD/LCD_int.h"
#include "../HAL/LD/LD_int.h"
#include "../HAL/Switch/Switch_int.h"
#include "../HAL/LM35/LM35_int.h"


#include "HVAC_config.h"
#include "HVAC_priv.h"

OnOff_t CompressorStatus = OFF;
Temp_t TemperatureStatus = NORMAL ;

u16 Global_u16TempRead , Global_u16PrevRead ;
bool  Global_boolPower = FALSE , Global_boolRead = FALSE , Global_boolSetMode = FALSE , Initial_Start = TRUE , Sleep_Mode = FALSE;
u8 Global_u8SetTemp = 27 ;


int
main(void)
{
//	ES_t  Local_enuErrorState ;
//	u8 Local_u8Key = KEYPAD_NOT_PRESSED ;
	u16 Local_u8ActTemp;

	if( HVAC_enuInit() == ES_OK )
	{
		LD_enuSetState( HOT_LD , LD_ON);
		if( HVAC_enuSetup() == ES_OK )
		{
			for(;;)
			{
				if( Global_boolPower == TRUE)
				{
					if( Initial_Start == TRUE)
					{
						HVAC_Awake();
					}
					if( Global_boolSetMode == TRUE)
					{
						HVAC_SetTemp();
						Global_boolSetMode = FALSE;
					}

					ADC_enuStartConversion();
					while( Global_boolRead == FALSE);
					Global_boolRead = FALSE;
					Local_u8ActTemp = Global_u16TempRead / (ADC_GAIN_FACTOR * TEMP_COEFFICENT) ;
					if( ABS_DIFF( Global_u16TempRead , Global_u16PrevRead ) >= ( ADC_GAIN_FACTOR * TEMP_COEFFICENT ) )
					{
						DISPLAY_TEMPERATURE_MESSAGE;
						LCD_enuWriteIntegerNum( (s32) Local_u8ActTemp );
						LCD_enuGoToPosition(2,12);
						if( CompressorStatus == ON)
						{
							LCD_enuWriteString("ON");
						}
						else
						{
							LCD_enuWriteString("OFF");
						}
					}
					Global_u16PrevRead = Global_u16TempRead;
					HVAC_CheckTemperatureStatus( Local_u8ActTemp );
					HVAC_AdjustCompressorStatus();
					HVAC_StandByMode();
				}
				else if ( Sleep_Mode == TRUE )
				{
					HVAC_Sleep();
				}
			}
		}
	}

}

ES_t HVAC_enuInit(void)
{
	DIO_enuInit();
	EXTI_enuInit();
	EXTI_enuEnableInterrupt(INT0);
	ADC_enuInit();
	GIE_enuEnable();

	LCD_enuInit();
	Keypad_enuInit();
	Switch_enuInit();
	LD_enuInit();
	LM35_enuInit();

	return ES_OK;
}

void ADC_vidISR(void)
{
	ADC_enuRead( &Global_u16TempRead );
	Global_boolRead = TRUE;
}

void EXTI_vidISR_INT0(void) /////Need to be completed
{
	u8 Local_u8KeyValue = SWITCH_UNPRESSED;

	if( Switch_enuGetPressed( POWER_SWITCH , &Local_u8KeyValue) == ES_OK)
	{
		if( Local_u8KeyValue ==  POWER_SWITCH_PRESSED )
		{
			_delay_ms( SWITCH_BOUNCE_DELAY );
			if( Switch_enuGetPressed( POWER_SWITCH , &Local_u8KeyValue) == ES_OK)
			{
				if( (Local_u8KeyValue ==  POWER_SWITCH_PRESSED)  && ( Global_boolPower == FALSE) )
				{
					Global_boolPower = TRUE;
					Initial_Start = TRUE;
				}
				else if( Local_u8KeyValue ==  POWER_SWITCH_PRESSED )
				{
					_delay_ms( SWITCH_BOUNCE_DELAY * 100 );
					if( Switch_enuGetPressed( POWER_SWITCH , &Local_u8KeyValue) == ES_OK)
					{
						if( Local_u8KeyValue ==  POWER_SWITCH_PRESSED )
						{
							Global_boolSetMode = TRUE ;
						}
					}
					else
					{
							Global_boolPower = FALSE ;
							Sleep_Mode = TRUE;
					}
				}
			}

		}
	}
}



ES_t HVAC_enuSetup(void)
{
	ES_t Local_enuErrorState = ES_NOK , Local_AenuErrorState[9];
	u8 Local_u8Iter = 0 ;

	//LD_enuSetState( HOT_LD , LD_ON);

	Local_AenuErrorState[0] = ADC_enuDisable();
	Local_AenuErrorState[1] = ADC_enuDisableAutoTrigger();
	Local_AenuErrorState[2] = ADC_enuCallBack ( ADC_vidISR );
	Local_AenuErrorState[3] = ADC_enuEnableInterrupt();
	Local_AenuErrorState[4] = ADC_enuEnable();
	Local_AenuErrorState[5] = ADC_enuStartConversion();
	Local_AenuErrorState[6] = EXTI_enuCallBack( EXTI_vidISR_INT0 , INT0);
	Local_AenuErrorState[7] = LCD_enuWriteCommand(0x0C);
	Local_AenuErrorState[8] = ADC_enuStartConversion();

	Global_u16PrevRead = Global_u16TempRead;
	Global_boolRead = FALSE ;

	for(; ( Local_u8Iter < 9 ) && (Local_AenuErrorState[Local_u8Iter] == ES_OK) ; Local_u8Iter++);

	if( Local_u8Iter == 9 ) Local_enuErrorState = ES_OK ;

	return Local_enuErrorState ;
}

void HVAC_Awake(void)
{
	DISPLAY_INIT_MESSAGE;
	ADC_enuEnable();
	ADC_enuStartConversion();
	ADC_enuPollingRead(&Global_u16TempRead);
	ADC_enuStartConversion();
	ADC_enuPollingRead(&Global_u16TempRead);
	Global_u16PrevRead = Global_u16TempRead;
	DISPLAY_TEMPERATURE_MESSAGE;
	LCD_enuWriteIntegerNum( (s32) ( Global_u16TempRead / (ADC_GAIN_FACTOR * TEMP_COEFFICENT) ) );
	LCD_enuGoToPosition(2,12);
	LCD_enuWriteString("OFF");
	Initial_Start = FALSE ;
}
void HVAC_Sleep(void)
{
	DISPLAY_CLOSE_MESSAGE;
	CompressorStatus = OFF;
	LD_enuSetState( HOT_LD , LD_OFF);
	LD_enuSetState( NORM_LD , LD_OFF);
	LD_enuSetState( COLD_LD , LD_OFF);
	TemperatureStatus = NORMAL ;
	ADC_enuDisable();
	_delay_ms(5000);
	LCD_enuWriteCommand(0x8);
	Sleep_Mode = FALSE;
}

void HVAC_StandByMode(void)
{
	for(u16 Local_u16Iter = 0 ; Local_u16Iter < 6000 ; Local_u16Iter++)
	{
		_delay_ms( SWITCH_BOUNCE_DELAY);
		if( Global_boolPower == FALSE || Global_boolSetMode == TRUE ) break;
	}
}



void HVAC_SetTemp(void)
{
	LCD_enuWriteCommand(0x01);
	LCD_enuWriteString("Set Mode");
	_delay_ms(5000);
}


void HVAC_CheckTemperatureStatus( u8 Copy_u8ActTempValue )
{
	if( Copy_u8ActTempValue > (Global_u8SetTemp + TEMP_TOLERANCE) )
	{
		TemperatureStatus = HOT ;
		LD_enuSetState( NORM_LD , LD_OFF);
		LD_enuSetState( COLD_LD , LD_OFF);
		LD_enuSetState( HOT_LD , LD_ON);
	}
	else if( Copy_u8ActTempValue < (Global_u8SetTemp - TEMP_TOLERANCE) )
	{
		TemperatureStatus = COLD ;
		LD_enuSetState( HOT_LD , LD_OFF);
		LD_enuSetState( NORM_LD , LD_OFF);
		LD_enuSetState( COLD_LD , LD_ON);
	}
	else
	{
		TemperatureStatus = NORMAL ;
		LD_enuSetState( HOT_LD , LD_OFF);
		LD_enuSetState( COLD_LD , LD_OFF);
		LD_enuSetState( NORM_LD , LD_ON);
	}
}

void HVAC_AdjustCompressorStatus(void)
{
	switch( CompressorStatus)
	{
		case OFF	:	if(TemperatureStatus == HOT) CompressorStatus = ON ;
						break;
		case ON		:	if(TemperatureStatus == COLD) CompressorStatus = OFF ;
							break;
	}
}
