/*
 * LM35_prog.c
 *
 *  Created on: Aug 21, 2022
 *      Author: basse
 */

#include "..\..\Libraries\stdTypes.h"
#include "..\..\Libraries\errorState.h"

#include "..\..\MCAL\DIO\DIO_int.h"

#include "LM35_config.h"

ES_t LM35_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK , Local_AenuErrorState[2];

	Local_AenuErrorState[0] = DIO_enuSetPinDirection( LM35_VSS_GRP , LM35_VSS_PIN , DIO_u8OUTPUT);
	Local_AenuErrorState[1] = DIO_enuSetPinValue( LM35_VSS_GRP , LM35_VSS_PIN , DIO_u8HIGH);

	if( Local_AenuErrorState[0] == ES_OK && Local_AenuErrorState[1] == ES_OK )
	{
		Local_AenuErrorState[0] = DIO_enuSetPinDirection( LM35_GROUND_GRP , LM35_GROUND_PIN , DIO_u8OUTPUT );
		Local_AenuErrorState[1] = DIO_enuSetPinValue( LM35_GROUND_GRP , LM35_GROUND_PIN , DIO_u8LOW);

		if( Local_AenuErrorState[0] == ES_OK && Local_AenuErrorState[1] == ES_OK )
		{
			Local_AenuErrorState[0] = DIO_enuSetPinDirection( TEMP_VALUE_GRP , TEMP_VALUE_PIN , DIO_u8INPUT );
			Local_AenuErrorState[1] = DIO_enuSetPinValue( TEMP_VALUE_GRP , TEMP_VALUE_PIN , TEMP_VALUE_STATE );
		}

	}

	if( Local_AenuErrorState[0] == ES_OK && Local_AenuErrorState[1] == ES_OK )
		Local_enuErrorState = ES_OK;

	return Local_enuErrorState ;
}
