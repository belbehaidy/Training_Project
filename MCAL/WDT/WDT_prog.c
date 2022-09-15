/*
 * WDT_prog.c
 *
 *  Created on: Sep 15, 2022
 *      Author: basse
 */

#include"..\..\Libraries\stdTypes.h"
#include"..\..\Libraries\errorState.h"

#include"WDT_priv.h"
#include"WDT_config.h"


ES_t WDT_Init(void)
{
	ES_t Local_enuErrorState = ES_NOK ;

	WDTCR = 0x00 ;
	if( WDT_PRES >= WDT_PRES_16K && WDT_PRES <= WDT_PRES_2048K )
	{
		WDTCR |= ( ( ( WDT_PRES - WDT_PRES_16K)& THREE_BITS_MASK ) << WDP0 ) ;
		Local_enuErrorState = ES_OK ;
	}
	else
	{

		WDTCR |= ( THREE_BITS_MASK << WDP0 ) ;
		Local_enuErrorState = ES_OUT_RANGE ;
		#warning "WDT_Init(): Unidentified Prescale Option . WDT_PRES_2048K is selected ."
	}

	return Local_enuErrorState ;
}

ES_t WDT_SetPrescale( u8 Copy_u8PrescaleValue )
{

	ES_t Local_enuErrorState = ES_NOK ;

	if( Copy_u8PrescaleValue >= WDT_PRES_16K && Copy_u8PrescaleValue <= WDT_PRES_2048K )
	{
		u8 Local_u8CopyWDTCR = WDTCR ;

		Local_u8CopyWDTCR |= ( BIT_MASK <<  WDTOE );
		Local_u8CopyWDTCR |= ( BIT_MASK <<  WDE ) ;

		WDTCR = Local_u8CopyWDTCR;
		WDTCR = 0x00;

		WDTCR |= ( ( (WDT_PRES - WDT_PRES_16K)& THREE_BITS_MASK ) << WDP0 ) ;

		WDTCR |= ( BIT_MASK <<  WDE ) ;

		Local_enuErrorState = ES_OK ;
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	return Local_enuErrorState ;
}

ES_t WDT_Enable(void)
{
	WDTCR |= ( BIT_MASK <<  WDE ) ;

	return ES_OK ;
}


ES_t WDT_Disable(void)
{
	u8 Local_u8Init_WDTCR = WDTCR ;
	u8 Local_u8Final_WDTCR = Local_u8Init_WDTCR & THREE_BITS_MASK ;

	Local_u8Init_WDTCR |= ( BIT_MASK <<  WDTOE );
	Local_u8Init_WDTCR |= ( BIT_MASK <<  WDE ) ;

	WDTCR = Local_u8Init_WDTCR;
	WDTCR = Local_u8Final_WDTCR;

	return ES_OK ;
}

void WDT_Reset(void)
{
	asm("WDR");
}
