/*
 * LD_prog.c
 *
 *  Created on: Aug 6, 2022
 *      Author: basse
 */

#include "../../Libraries/stdTypes.h"
#include "../../Libraries/errorState.h"

#include "../../MCAL/DIO/DIO_int.h"

#include "LD_config.h"
#include "LD_priv.h"

extern u8 LD_u8LD_MaxNum;
extern u8 LD_Mode;
extern u8 LD_u8On;
extern u8 LD_u8Off;

extern LD_t LD[];


ES_t LD_enuInit(void)
{
	ES_t Local_enuErrorStatus = ES_NOK , Local_AenuStatus[2];

	if( LD_Mode == ACTIVE_HIGH || LD_Mode == ACTIVE_LOW )
	{
		for(u8 Local_u8Iter; Local_u8Iter < LD_u8LD_MaxNum ; Local_u8Iter++)
		{
			Local_AenuStatus[0] = DIO_enuSetPinDirection	( LD[Local_u8Iter].LD_Grp , LD[Local_u8Iter].LD_Pin , DIO_u8OUTPUT);
			Local_AenuStatus[1] = DIO_enuSetPinValue		( LD[Local_u8Iter].LD_Grp , LD[Local_u8Iter].LD_Pin , LD_u8Off);

			if(Local_AenuStatus[0] != ES_OK || Local_AenuStatus[1] != ES_OK) break;
		}

		if(Local_AenuStatus[0] == ES_OK && Local_AenuStatus[1] == ES_OK)
			Local_enuErrorStatus = ES_OK;
	}
	else Local_enuErrorStatus = ES_OUT_RANGE;

	return Local_enuErrorStatus;
}

ES_t LD_enuSetState(u8 Copy_u8LD_Num ,u8 Copy_u8LDSetValue)
{
	ES_t Local_enuErrorStatus = ES_NOK ;

	if( LD_Mode == ACTIVE_HIGH || LD_Mode == ACTIVE_LOW )
	{
		if( Copy_u8LD_Num < LD_u8LD_MaxNum)
		{
			if( Copy_u8LDSetValue == LD_u8On || Copy_u8LDSetValue == LD_u8Off )
			{
				Local_enuErrorStatus = DIO_enuSetPinValue(LD[Copy_u8LD_Num].LD_Grp , LD[Copy_u8LD_Num].LD_Pin , Copy_u8LDSetValue);
			}
			else Local_enuErrorStatus = ES_OUT_RANGE;
		}
		else Local_enuErrorStatus = ES_OUT_RANGE;
	}
	else Local_enuErrorStatus = ES_OUT_RANGE;


	return Local_enuErrorStatus;
}
