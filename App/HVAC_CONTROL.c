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

u16 Global_u16TempRead = 0 ;
bool  Global_boolPower = FALSE , Global_boolRead = FALSE , Global_boolSetMode = FALSE , Initial_Start = TRUE , Sleep_Mode = FALSE;
u8 Global_u8SetTemp = 27 ;


int
main(void)
{
//	ES_t  Local_enuErrorState ;
//	u8 Local_u8Key = KEYPAD_NOT_PRESSED ;
	u16 Local_u16ActTemp , Local_u8DisplTemp = 25 ;
	OnOff_t Local_enuDisplComp = OFF , Local_enuSetDisplay = OFF;

	if( HVAC_enuInit() == ES_OK )
	{
		if( HVAC_enuSetup() == ES_OK )
		{
			for(;;)
			{
				if( Global_boolPower == TRUE)
				{
					if( Initial_Start == TRUE)
					{
						HVAC_Awake(&Local_u16ActTemp);
						Local_enuSetDisplay = ON;
					}
					if( Global_boolSetMode == TRUE)
					{
						HVAC_SetTemp();
						Local_enuSetDisplay = ON;
					}

					HVAC_CheckTemperatureStatus( Local_u16ActTemp );
					HVAC_AdjustCompressorStatus();

					if( Local_u16ActTemp != Local_u8DisplTemp || Local_enuDisplComp != CompressorStatus || Local_enuSetDisplay == ON )
					{
						DISPLAY_STATUS_MESSAGE( Local_u16ActTemp ) ;
						Local_u8DisplTemp = Local_u16ActTemp;
						Local_enuDisplComp = CompressorStatus;
						if ( Local_enuSetDisplay == ON ) Local_enuSetDisplay = OFF;
					}
					HVAC_StandByMode();
					HVAC_UpdateReading(&Local_u16ActTemp);
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
	if( ADC_enuRead( &Global_u16TempRead ) == ES_OK )
		Global_boolRead = TRUE;
}

void EXTI_vidISR_INT0(void)
{
	u8 Local_u8KeyValue = SWITCH_UNPRESSED;

	if( Switch_enuGetPressed( POWER_SWITCH , &Local_u8KeyValue) == ES_OK)
	{
		if( ( Global_boolPower == FALSE) && (Local_u8KeyValue == POWER_SW_PRESSED ) )
		{
			_delay_ms( SWITCH_BOUNCE_DELAY );
			if( Switch_enuGetPressed( POWER_SWITCH , &Local_u8KeyValue) == ES_OK )
			{
				if( Local_u8KeyValue ==  POWER_SW_PRESSED )
				{
					Global_boolPower = TRUE;
					Initial_Start = TRUE;
				}
			}
		}
		else if( (Global_boolPower == TRUE) && (Local_u8KeyValue ==  POWER_SW_PRESSED ) )
		{
			_delay_ms( POWER_OFF_DELAY );
			if( Switch_enuGetPressed( POWER_SWITCH , &Local_u8KeyValue) == ES_OK )
			{
				if( Local_u8KeyValue ==  POWER_SW_PRESSED )
				{
					Global_boolPower = FALSE ;
					Sleep_Mode = TRUE;
				}
				else
				{
					Global_boolSetMode = TRUE ;
				}
			}
		}
	}
}



ES_t HVAC_enuSetup(void)
{
	ES_t Local_enuErrorState = ES_NOK , Local_AenuErrorState[7];
	u8 Local_u8Iter = 0 ;

	Local_AenuErrorState[0] = ADC_enuDisable();
	Local_AenuErrorState[1] = ADC_enuDisableAutoTrigger();
	Local_AenuErrorState[2] = ADC_enuCallBack ( ADC_vidISR );
	Local_AenuErrorState[3] = ADC_enuEnableInterrupt();
	Local_AenuErrorState[4] = ADC_enuEnable();
	Local_AenuErrorState[5] = EXTI_enuCallBack( EXTI_vidISR_INT0 , INT0);
	Local_AenuErrorState[6] = LCD_enuWriteCommand(0x0C);

	for(; ( Local_u8Iter < 7 ) && (Local_AenuErrorState[Local_u8Iter] == ES_OK) ; Local_u8Iter++);
	if( Local_u8Iter == 7 )
		Local_enuErrorState = ES_OK ;

	return Local_enuErrorState ;
}

void HVAC_Awake(u16 *Copy_u16AvgTempValue)
{
	ADC_enuEnable();
	DISPLAY_INIT_MESSAGE;
	HVAC_UpdateReading( Copy_u16AvgTempValue );
	CompressorStatus = ON ;
	_delay_ms(2000);
	Initial_Start = FALSE ;
}
void HVAC_Sleep(void)
{
	DISPLAY_CLOSE_MESSAGE;
	LD_enuSetState( HOT_LD , LD_OFF);
	LD_enuSetState( NORM_LD , LD_OFF);
	LD_enuSetState( COLD_LD , LD_OFF);
	ADC_enuDisable();
	_delay_ms(5000);
	LCD_enuWriteCommand(0x08);
	Sleep_Mode = FALSE;
	Initial_Start = TRUE ;
}

void HVAC_StandByMode(void)
{
	for(u16 Local_u16Iter = 0 ; Local_u16Iter < STANDBY_TOTAL_DELAYS ; Local_u16Iter++)    //Delay between readings = STANDBY_TOTAL_DELAYS * STANDBY_CHECK_DELAY
	{
		_delay_ms( STANDBY_CHECK_DELAY);
		if( Global_boolPower == FALSE || Global_boolSetMode == TRUE ) break;
	}
}



void HVAC_SetTemp(void)
{
	u8 Local_u8IncKeyValue , Local_u8DecKeyValue ,Local_u8Counter = STANDBY_TOTAL_DELAYS , Local_u8SetTemp = Global_u8SetTemp;

	DISPLAY_SET_MODE_MESSAGE;
	_delay_ms(500);

	while( Local_u8Counter > 0)
	{
		if( ( Switch_enuGetPressed( INCREMENT_SWITCH , &Local_u8IncKeyValue) == ES_OK ) && ( Switch_enuGetPressed( DECREMENT_SWITCH , &Local_u8DecKeyValue) == ES_OK ) )
		{
			if( Local_u8IncKeyValue == INC_SW_PRESSED && Local_u8DecKeyValue != DEC_SW_PRESSED )
			{
				_delay_ms( SWITCH_BOUNCE_DELAY );
				if( Switch_enuGetPressed( INCREMENT_SWITCH , &Local_u8IncKeyValue) == ES_OK )
				{
					if( Local_u8IncKeyValue == INC_SW_PRESSED )
					{
						Global_u8SetTemp++;
						Local_u8Counter = STANDBY_TOTAL_DELAYS ;
					}
				}
			}
			else if( Local_u8DecKeyValue == DEC_SW_PRESSED && Local_u8IncKeyValue != INC_SW_PRESSED )
			{
				_delay_ms( SWITCH_BOUNCE_DELAY );
				if( Switch_enuGetPressed( DECREMENT_SWITCH , &Local_u8DecKeyValue) == ES_OK )
				{
					if( Local_u8DecKeyValue == DEC_SW_PRESSED )
					{
						Global_u8SetTemp--;
						Local_u8Counter = STANDBY_TOTAL_DELAYS ;
					}
				}
			}

		}


		if ( Global_u8SetTemp != Local_u8SetTemp )
		{
			DISPLAY_SET_MODE_MESSAGE;
			Local_u8SetTemp = Global_u8SetTemp  ;
		}
		else _delay_ms(100);

		_delay_ms( STANDBY_CHECK_DELAY );
		Local_u8Counter-- ;
	}
	Global_boolSetMode = FALSE;
}


void HVAC_CheckTemperatureStatus(u16 Copy_u16ActTempValue )
{
	if( Copy_u16ActTempValue > (Global_u8SetTemp + TEMP_TOLERANCE) )
	{
		TemperatureStatus = HOT ;
		LD_enuSetState( NORM_LD , LD_OFF);
		LD_enuSetState( COLD_LD , LD_OFF);
		LD_enuSetState( HOT_LD , LD_ON);
	}
	else if( Copy_u16ActTempValue < (Global_u8SetTemp - TEMP_TOLERANCE) )
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
		case (OFF)	:	if( TemperatureStatus == HOT) CompressorStatus = ON ;
						break;
		case (ON)	:	if( TemperatureStatus == COLD) CompressorStatus = OFF ;
							break;
	}
}

void HVAC_UpdateReading(u16 *Copy_u16AvgTempValue )
{
	u16 Local_u16AccumReadings = 0;

	for( u8 Local_u8Iter = 0 ; Local_u8Iter < TEMP_AVG_READINGS ; Local_u8Iter++)
	{
		ADC_enuStartConversion();
		while( Global_boolRead == FALSE);
		Local_u16AccumReadings += ( Global_u16TempRead / (ADC_GAIN_FACTOR * TEMP_COEFFICENT) );
		Global_boolRead = FALSE;
	}

	Local_u16AccumReadings = ( (Local_u16AccumReadings + (TEMP_AVG_READINGS -1) )/  TEMP_AVG_READINGS  )  ;
	Local_u16AccumReadings = (Local_u16AccumReadings - ( Local_u16AccumReadings / 33 ) )  ;

	if(Local_u16AccumReadings < 2  )	Local_u16AccumReadings = 2 ; 	//Minimum Correct Reading.

	*Copy_u16AvgTempValue = Local_u16AccumReadings;

}
