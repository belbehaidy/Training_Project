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

UART_Int_t Global_AstrUART_Pointers[UART_INTERRUPTS] =	{
														{ RXCIE ,	NULL, NULL},
														{ UDRIE ,	NULL, NULL},
														{ TXCIE ,	NULL, NULL}
													};



ES_t UART_enuInit( void )
{
	ES_t Local_enuErrorState = ES_NOK;

	UCSRC |= (BIT_MASK << URSEL_BIT );
	UCSRC &= 0x80 ;
	UCSRB = 0x00 ;
	UCSRA = 0x00 ;

	/************************************************/
	/*	Setting Multi-processor Communication Mode	*/
	/************************************************/
#if MULTI_PROCESSOR == MPCM_ON
	UCSRC |= (BIT_MASK << URSEL_BIT );
	UCSRA |= (BIT_MASK << MPCM_BIT );
#endif

	/****************************************************************/
	/*	Setting Synchronous Mode / Operating Mode / Clock Polarity	*/
	/****************************************************************/
#if SYNC_MODE == ASYNCHRONOUS
	UCSRC |= (BIT_MASK << URSEL_BIT );
	UCSRC &= ~(BIT_MASK << UMSEL_BIT );
	UCSRC &= ~(BIT_MASK << UCPOL_BIT );

	#if OPERATING_MODE == NORMAL_SPEED
	UCSRA &=~(BIT_MASK << U2X_BIT );
	#elif OPERATING_MODE == DOUBLE_SPEED
	UCSRA |= (BIT_MASK << U2X_BIT );
	#else
	#warning " UART_enuInit(): Unidentified Operating Mode. Switched to NORMAL_SPEED "
	#endif

#elif SYNC_MODE == SYNCHRONOUS
	UCSRA &=~(BIT_MASK << U2X_BIT );
	UCSRC |= (BIT_MASK << URSEL_BIT );
	UCSRC |= (BIT_MASK << UMSEL_BIT );

	#if CLOCK_POLARITY == TX_RISING_EDGE
		UCSRC |= (BIT_MASK << URSEL_BIT );
		UCSRC &= ~(BIT_MASK << UCPOL_BIT );
	#elif CLOCK_POLARITY == RX_RISING_EDGE
		UCSRC |= (BIT_MASK << URSEL_BIT );
		UCSRC |= (BIT_MASK << UCPOL_BIT );
	#else
	#warning " UART_enuInit(): Unidentified Clock Polarity for Synchronous Operation . Switched to TX_RISING_EDGE Polarity "
	#endif

#else
#error " UART_enuInit(): Unidentified Synchronization Mode. "
#endif
	UCSRA &= TWO_BITS_MASK ;

	/************************************************/
	/*			Setting Frame Data bits 			*/
	/************************************************/
#if DATA_BITS >= FIVE_BITS && DATA_BITS <= EIGHT_BITS
	UCSRC |= (BIT_MASK << URSEL_BIT );
	UCSRC |= ( ( DATA_BITS - FIVE_BITS )& TWO_BITS_MASK ) << UCSZ0_BIT ;
#elif DATA_BITS == NINE_BITS
	UCSRB |= (BIT_MASK << UCSZ2_BIT );
	UCSRC |= (BIT_MASK << URSEL_BIT );
	UCSRC |= (TWO_BITS_MASK << UCSZ0_BIT ) ; ////////////////////////////////////////
#else
	UCSRC |= (BIT_MASK << URSEL_BIT );
	UCSRC |= (TWO_BITS_MASK << UCSZ0_BIT ) ;
#warning " UART_enuInit(): Unidentified Frame Data bits choice. 8-bits Choice is selected "
#endif

	/************************************************/
	/*			Setting Frame Parity bits 			*/
	/************************************************/
#if PARITY_STATUS == NO_PARITY
	UCSRC |= (BIT_MASK << URSEL_BIT );
	UCSRC &= ~(BIT_MASK << UPM1_BIT );
	UCSRC &= ~(BIT_MASK << UPM0_BIT );
#elif PARITY_STATUS == EVEN_PARITY
	UCSRC |= (BIT_MASK << URSEL_BIT );
	UCSRC |= (BIT_MASK << UPM1_BIT );
	UCSRC &= ~(BIT_MASK << UPM0_BIT );
#elif PARITY_STATUS == ODD_PARITY
	UCSRC |= (BIT_MASK << URSEL_BIT );
	UCSRC |= (BIT_MASK << UPM1_BIT );
	UCSRC |= (BIT_MASK << UPM0_BIT );
#else
#warning " UART_enuInit(): Unidentified Parity bits choice. NO Parity is selected "
#endif

	/************************************************/
	/*			Setting Frame STOP bits 			*/
	/************************************************/
#if STOP_STATUS == ONE_STOP_BIT
	UCSRC |= (BIT_MASK << URSEL_BIT );
	UCSRC &= ~(BIT_MASK << USBS_BIT );
#elif STOP_STATUS == TWO_STOP_BIT
	UCSRC |= (BIT_MASK << URSEL_BIT );
	UCSRC |= (BIT_MASK << USBS_BIT );
#else
#warning " UART_enuInit(): Unidentified STOP bits choice. 1-STOP bit choice is selected "
#endif

	/************************************************/
	/*				Setting BAUD Rate	 			*/
	/************************************************/
	UBRRH &= ~(BIT_MASK << URSEL_BIT );
	UBRRH |= ( ( ( UBRR_VALUE( BAUD_RATE ) ) >> BYTE_SHIFT ) & NIBBLE_MASK ) ;
	UBRRL =  UBRR_VALUE( BAUD_RATE ) ;

	/************************************************/
	/*				Setting UART Mode	 			*/
	/************************************************/
	#if UART_MODE == TRANSMITTER
	UCSRB |= ( BIT_MASK << TXEN_BIT );
#elif UART_MODE == RECEIVER
	UCSRB |= ( BIT_MASK << RXEN_BIT );
#elif UART_MODE == TRANSCEIVER
	UCSRB |= ( BIT_MASK << TXEN_BIT );
	UCSRB |= ( BIT_MASK << RXEN_BIT );
#else
#warning " UART_enuInit(): Unidentified UART Mode. Disabled UART "
#endif

	return Local_enuErrorState ;
}

ES_t UART_ReceiveFrame( void *Copy_pReceivedData)
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

ES_t UART_SendFrame( void *Copy_pData)
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


ES_t UART_enuSendPacket( void *Copy_pcData )
{
	ES_t Local_enuErrorState = ES_NOK;

	/*	Writing Data To UDR according to Frame Data bits 	*/
	if( DATA_BITS == NINE_BITS )
	{
		while( *((u16 *)Copy_pcData) != '\0' )
		{
			while( !( ( UCSRA>> UDRE_BIT ) & BIT_MASK ) );		/*	Waiting for UDR Empty Flag	*/
			u8 Local_u8CopySERG = SREG;
			asm(" CLI ");
			UCSRB &= ~(BIT_MASK << TXB8_BIT );
			UCSRB |= ( ( (*( (u16 *)Copy_pcData )) >> BYTE_SHIFT ) & BIT_MASK ) << TXB8_BIT ;
			TXB = *( (u16 *)Copy_pcData ) ;
			SREG = Local_u8CopySERG ;
			(u16 *)Copy_pcData++ ;
		}
	}
	else
	{
		while( *((u8 *)Copy_pcData) != '\0' )
		{
			while( !( ( UCSRA>> UDRE_BIT ) & BIT_MASK ) );		/*	Waiting for UDR Empty Flag	*/
			TXB = *( (u8 *)Copy_pcData ) ;						/*	Writing Data to UDR			*/
			(u8 *)Copy_pcData++ ;
		}

	}
	Local_enuErrorState = ES_OK ;


	return Local_enuErrorState ;
}

ES_t UART_enuRecievePacket(void *Copy_pcData)
{
	ES_t Local_enuErrorState = ES_NOK;

		if( Copy_pcData != NULL )
		{
			while( !( ( UCSRA >> RXC_BIT ) & BIT_MASK ) );		/*	Waiting for RX Complete Flag	*/

			/*	Reading Data From UDR according to Frame Data bits 	*/
			if( DATA_BITS == NINE_BITS )
			{

				while( *((u8 *)Copy_pcData) != '\0' )
				{
					while( !( ( UCSRA >> RXC_BIT ) & BIT_MASK ) );		/*	Waiting for RX Complete Flag	*/
					u8 Local_u8CopySERG = SREG;
					asm(" CLI ");
					*( (u16 *)Copy_pcData )  = 	0x00;
					*( (u16 *)Copy_pcData )  =	(u16)( ( UCSRB >> RXB8_BIT ) & BIT_MASK ) << BYTE_SHIFT ;
					*( (u16 *)Copy_pcData ) |=	RXB ;
					SREG = Local_u8CopySERG ;
					(u16 *)Copy_pcData++ ;
				}

			}
			else
			{
				while( *((u8 *)Copy_pcData) != '\0' )
				{
					while( !( ( UCSRA >> RXC_BIT ) & BIT_MASK ) );		/*	Waiting for RX Complete Flag	*/
					*( (u8 *)Copy_pcData ) = RXB ;
					(u8 *)Copy_pcData++ ;
				}

			}
			Local_enuErrorState = ES_OK;
		}
		else Local_enuErrorState = ES_NULL_POINTER;

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
			if( Global_AstrUART_Pointers[Local_u8Iter].InterruptName == Copy_u8InterruptName )
			{
				Global_AstrUART_Pointers[Local_u8Iter].ptrFun = Copy_pAppFun ;
				Global_AstrUART_Pointers[Local_u8Iter].ptrVar = Copy_pAppVar ;
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
	if( Global_AstrUART_Pointers[0].ptrFun != NULL)
	{
		( *Global_AstrUART_Pointers[0].ptrFun )( Global_AstrUART_Pointers[0].ptrVar );
	}
}

void __vector_14( void )	/*	UDRE : UDR register Empty Interrupt	*/
{
	if( Global_AstrUART_Pointers[1].ptrFun != NULL)
	{
		( *Global_AstrUART_Pointers[1].ptrFun )( Global_AstrUART_Pointers[1].ptrVar );
	}
}

void __vector_15( void )	/*	TXCIE : TX Complete Interrupt	*/
{
	if( Global_AstrUART_Pointers[2].ptrFun != NULL)
	{
		( *Global_AstrUART_Pointers[2].ptrFun)( Global_AstrUART_Pointers[2].ptrVar );
	}
}
