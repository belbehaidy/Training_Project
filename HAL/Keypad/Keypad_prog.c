/*
 * Keypad_prog.c
 *
 *  Created on: Aug 13, 2022
 *      Author: Ahmed El-Gaafrawy
 */

#include <util\delay.h>
#include "../../Libraries/stdTypes.h"
#include "../../Libraries/errorState.h"


#include "../../MCAL/DIO/DIO_int.h"

#include "Keypad_config.h"
#include "Keypad_priv.h"

extern u8 Kpad_u8MaxSideKeys;
extern u8 Kpad_u8RowNum;
extern u8 Kpad_u8ColNum;

extern u8 Kpad_u8KeyValues[];
extern KP_Output_t KeyPadCols[];
extern KP_Input_t KeyPadRows[];

ES_t Keypad_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK , Local_enuError[2];
	u8 Local_u8Iter=0 , Local_u8Flag = 0;

	if( (Kpad_u8RowNum <= Kpad_u8MaxSideKeys) && (Kpad_u8ColNum <= Kpad_u8MaxSideKeys ) )
	{
		for( ; Local_u8Iter < Kpad_u8RowNum ; Local_u8Iter++)
		{
			Local_enuError[0] = DIO_enuSetPinDirection	( KeyPadRows[Local_u8Iter].InputGrp , KeyPadRows[Local_u8Iter].InputPin , DIO_u8INPUT	);
			Local_enuError[1] = DIO_enuSetPinValue		( KeyPadRows[Local_u8Iter].InputGrp , KeyPadRows[Local_u8Iter].InputPin , KeyPadRows[Local_u8Iter].InputState	);

			if(Local_enuError[0] != ES_OK || Local_enuError[1] != ES_OK )
			{
				Local_u8Flag = 1;
				break;
			}
		}
		if(!Local_u8Flag)
		{
			for(Local_u8Iter = 0 ; Local_u8Iter < Kpad_u8ColNum ; Local_u8Iter++)
			{
				Local_enuError[0] = DIO_enuSetPinDirection	( KeyPadCols[Local_u8Iter].OutputGrp , KeyPadCols[Local_u8Iter].OutputPin , DIO_u8OUTPUT );
				Local_enuError[1] = DIO_enuSetPinValue		( KeyPadCols[Local_u8Iter].OutputGrp , KeyPadCols[Local_u8Iter].OutputPin , DIO_u8HIGH	 );

				if(Local_enuError[0] != ES_OK || Local_enuError[1] != ES_OK )
				{
					Local_u8Flag = 1;
					break;
				}
			}
		}
		if( !Local_u8Flag )
			Local_enuErrorState = ES_OK;
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	return Local_enuErrorState;//DONE
}

ES_t Keypad_enuGetPressedKey(u8 * Copy_pu8KeyValue)

{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_pu8KeyValue != NULL)
	{
		u8 Local_u8RowValue = 1 ,  Local_u8Pressed = 0 ;

		*Copy_pu8KeyValue = 0xff;

		for(u8 Local_u8ColIter = 0 ;Local_u8ColIter < Kpad_u8ColNum ; Local_u8ColIter++)
		{
			Local_enuErrorState = DIO_enuSetPinValue( KeyPadCols[Local_u8ColIter].OutputGrp , KeyPadCols[Local_u8ColIter].OutputPin , DIO_u8LOW );

			for(u8 Local_u8RowIter = 0 ;(Local_enuErrorState == ES_OK ) && ( Local_u8RowIter < Kpad_u8RowNum ) ; Local_u8RowIter++)
			{
				Local_enuErrorState = DIO_enuGetPinValue( KeyPadRows[Local_u8RowIter].InputGrp , KeyPadRows[Local_u8RowIter].InputPin , &Local_u8RowValue);

				if( (Local_u8RowValue == DIO_u8LOW) && ( Local_enuErrorState == ES_OK) )
				{
					_delay_ms(KEYPAD_BOUNCE_DELAY);
					Local_enuErrorState = DIO_enuGetPinValue( KeyPadRows[Local_u8RowIter].InputGrp , KeyPadRows[Local_u8RowIter].InputPin , &Local_u8RowValue);

					if( Local_enuErrorState == ES_OK  && Local_u8RowValue == DIO_u8LOW )
					{
						Local_u8Pressed = 1 ;
						*Copy_pu8KeyValue = Kpad_u8KeyValues[( (Local_u8RowIter*Kpad_u8ColNum) + Local_u8ColIter) ];

						while( Local_u8RowValue == DIO_u8LOW  && Local_enuErrorState == ES_OK )
						{
							Local_enuErrorState = DIO_enuGetPinValue( KeyPadRows[Local_u8RowIter].InputGrp , KeyPadRows[Local_u8RowIter].InputPin , &Local_u8RowValue);
						}
						break;
					}
				}
			}

			if( (Local_u8Pressed == 1) || (Local_enuErrorState != ES_OK) )
				break;
			else
				Local_enuErrorState = DIO_enuSetPinValue( KeyPadCols[Local_u8ColIter].OutputGrp , KeyPadCols[Local_u8ColIter].OutputPin , DIO_u8HIGH );
		}
	}
	else Local_enuErrorState = ES_NULL_POINTER;

	return Local_enuErrorState;//DONE
}

ES_t Keypad_Sleep(void)
{
	ES_t Local_enuErrorState = ES_NOK ;
	u8 Local_u8Iter=0 ;

	if( (Kpad_u8RowNum <= Kpad_u8MaxSideKeys) && (Kpad_u8ColNum <= Kpad_u8MaxSideKeys ) )
	{
		for( ; Local_u8Iter < Kpad_u8RowNum ; Local_u8Iter++)
		{
			Local_enuErrorState = DIO_enuSetPinValue( KeyPadRows[Local_u8Iter].InputGrp , KeyPadRows[Local_u8Iter].InputPin , DIO_u8FLOAT	);

			if(Local_enuErrorState != ES_OK  )	break;
		}

		if( Local_enuErrorState == ES_OK )
		{
			for(Local_u8Iter = 0 ; Local_u8Iter < Kpad_u8ColNum ; Local_u8Iter++)
			{
				Local_enuErrorState = DIO_enuSetPinValue( KeyPadCols[Local_u8Iter].OutputGrp , KeyPadCols[Local_u8Iter].OutputPin , DIO_u8LOW );

				if(Local_enuErrorState != ES_OK  )	break;
			}
		}
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	return Local_enuErrorState;//DONE
}

ES_t Keypad_Awake(void)
{
	ES_t Local_enuErrorState = ES_NOK ;
	u8 Local_u8Iter=0 ;

	if( (Kpad_u8RowNum <= Kpad_u8MaxSideKeys) && (Kpad_u8ColNum <= Kpad_u8MaxSideKeys ) )
	{
		for( ; Local_u8Iter < Kpad_u8RowNum ; Local_u8Iter++)
		{
			Local_enuErrorState = DIO_enuSetPinValue( KeyPadRows[Local_u8Iter].InputGrp , KeyPadRows[Local_u8Iter].InputPin , KeyPadRows[Local_u8Iter].InputState );

			if(Local_enuErrorState != ES_OK  )	break;
		}

		if( Local_enuErrorState == ES_OK )
		{
			for(Local_u8Iter = 0 ; Local_u8Iter < Kpad_u8ColNum ; Local_u8Iter++)
			{
				Local_enuErrorState = DIO_enuSetPinValue( KeyPadCols[Local_u8Iter].OutputGrp , KeyPadCols[Local_u8Iter].OutputPin , DIO_u8HIGH );

				if(Local_enuErrorState != ES_OK  )
				{
					break;
				}
			}

		}

	}
	else Local_enuErrorState = ES_OUT_RANGE;

	return Local_enuErrorState;//DONE
}
