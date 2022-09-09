/*
 * UART_prog.c
 *
 *  Created on: Sep 9, 2022
 *      Author: basse
 */

#include "..\..\Libraries\stdTypes.h"
#include "..\..\Libraries\errorState.h"

#include "UART_priv.h"
#include "UART_config.h"

UART_Int_t Global_AstrPointers[UART_INTERRUPTS] =	{
														{ RXCIE ,	NULL, NULL},
														{ UDRIE ,	NULL, NULL},
														{ TXCIE ,	NULL, NULL}
													};



ES_t UART_enuInit( void )
{
	ES_t Local_enuErrorState = ES_NOK;

	UCSRC |= (BIT_MASK << URSEL_BIT );
	UCSRC &= 0x80;

#if SYNC_MODE == ASYNCHRONOUS
	UCSRC &= ~(BIT_MASK << UMSEL_BIT );

	#if OPERATING_MODE == NORMAL_SPEED
	UCSRA &=~(BIT_MASK << U2X_BIT );
	#elif OPERATING_MODE == DOUBLE_SPEED
	UCSRA |= (BIT_MASK << U2X_BIT );
	#else
	#warning " UART_enuInit(): Unidentified Operating Mode. Switched to NORMAL_SPEED "
	#endif

#elif SYNC_MODE == SYNCHRONOUS
	UCSRC |= (BIT_MASK << UMSEL_BIT );

	#if OPERATING_MODE == MASTER_SPEED
	UCSRA &=~(BIT_MASK << U2X_BIT );

	#else
	#warning " UART_enuInit(): Unidentified Operating Mode for Synchronous Operation . Switched to MASTER_SPEED "
	#endif

#else
#error " UART_enuInit(): Unidentified Synchronization Mode. "
#endif





	return Local_enuErrorState ;
}

ES_t UART_ReceiveChar( void *Copy_pReceivedData)
{
	ES_t Local_enuErrorState = ES_NOK;

	if( Copy_pReceivedData != NULL )
	{
		while( !( ( UCSRA >> RXC_BIT ) & BIT_MASK ) );		/*	Waiting for RX Complete Flag	*/

		/*	Reading Data From UDR according to Frame Data bits 	*/
		if( DATA_BITS == NINE_BITS )
		{
			u8 Local_u8CopySERG = SREG;
			asm(" CLI ");
			*( (u16 *)Copy_pReceivedData )  = 	0x00;
			*( (u16 *)Copy_pReceivedData )  =	(u16)( ( UCSRB >> RXB8_BIT ) & BIT_MASK ) << BYTE_SHIFT ;
			*( (u16 *)Copy_pReceivedData ) |=	RXB ;
			SREG = Local_u8CopySERG ;
		}
		else
		{
			*( (u8 *)Copy_pReceivedData ) = RXB ;
		}
		Local_enuErrorState = ES_OK;
	}
	else Local_enuErrorState = ES_NULL_POINTER;

	return Local_enuErrorState ;
}

ES_t UART_SendChar( void *Copy_pData)
{
	ES_t Local_enuErrorState = ES_NOK;

	while( !( ( UCSRA>> UDRE_BIT ) & BIT_MASK ) );		/*	Waiting for UDR Empty Flag	*/

	/*	Writing Data To UDR according to Frame Data bits 	*/
	if( DATA_BITS == NINE_BITS )
	{
		u8 Local_u8CopySERG = SREG;
		asm(" CLI ");
		UCSRB &= ~(BIT_MASK << TXB8_BIT );
		UCSRB |= ( ( (*( (u16 *)Copy_pData )) >> BYTE_SHIFT ) & BIT_MASK ) << TXB8_BIT ;
		TXB = *( (u16 *)Copy_pData ) ;
		SREG = Local_u8CopySERG ;
	}
	else
	{
		TXB = *( (u8 *)Copy_pData ) ;					/*	Writing Data to UDR			*/
	}
	Local_enuErrorState = ES_OK ;

	return Local_enuErrorState ;
}

ES_t UART_enuCallBack( u8 Copy_u8InterruptName , void ( *Copy_pAppFun(void *) ) , void *Copy_pAppVar )
{
	ES_t Local_enuErrorState = ES_NOK;

	if( Copy_pAppFun != NULL )
	{

		u8 Local_u8Iter = 0 ;
		for( ; Local_u8Iter < UART_INTERRUPTS ; Local_u8Iter++ )
		{
			if( Global_AstrPointers[Local_u8Iter].InterruptName == Copy_u8InterruptName )
			{
				Global_AstrPointers[Local_u8Iter].ptrFun = Copy_pAppFun ;
				Global_AstrPointers[Local_u8Iter].ptrVar = Copy_pAppVar ;
				Local_enuErrorState = ES_OK;
				break;
			}
		}
		if( Local_u8Iter ==  UART_INTERRUPTS )
		{
			Local_enuErrorState = ES_OUT_RANGE;
			#warning " UART_enuCallBack() : Unidentified Interrupt Name, No Action Taken."
		}

	}
	else Local_enuErrorState = ES_NULL_POINTER ;

	return Local_enuErrorState ;
}


void __vector_13( void )__attribute__((signal));
void __vector_14( void )__attribute__((signal));
void __vector_15( void )__attribute__((signal));

void __vector_13( void )	/*	RXCIE : RX Complete Interrupt	*/
{
	if( Global_AstrPointers[0] != NULL)
	{
		(*(Global_AstrPointers[0].ptrFun))(Global_AstrPointers.ptrVar);
	}
}

void __vector_14( void )	/*	UDRE : UDR register Empty Interrupt	*/
{
	if( Global_AstrPointers[1] != NULL)
	{
		(*(Global_AstrPointers[1].ptrFun))(Global_AstrPointers.ptrVar);
	}
}

void __vector_15( void )	/*	TXCIE : TX Complete Interrupt	*/
{
	if( Global_AstrPointers[2] != NULL)
	{
		(*(Global_AstrPointers[2].ptrFun))(Global_AstrPointers.ptrVar);
	}
}
