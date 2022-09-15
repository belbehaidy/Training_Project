/*
 * TIMER_pro.c
 *
 *  Created on: Aug 26, 2022
 *      Author: basse
 */

#include "../../Libraries/stdTypes.h"
#include "../../Libraries/errorState.h"

#include "TIMER_config.h"
#include "TIMER_priv.h"



extern u8 Timers_u8MaxNum , Timer0Max , Timer2Max ;
extern Timer_t	Timers[];

static u8 Global_u8OCR0_Value = 0x00  , Global_u8OCR2_Value = 0x00 ;
static u32 Global_u32Timer0_Clk , Global_u32Timer2_Clk ;



TimerInt_Pointers_t Global_AstrTimerPointers[TIMERS_INTERRUPTS] =	{
															{ TOIE0 ,	NULL, NULL},
															{ OCIE0 ,	NULL, NULL},
															{ TOIE2 ,	NULL, NULL},
															{ OCIE2 ,	NULL, NULL},
														};


ES_t Timer_enuInit( void )
{
	ES_t Local_enuErrorState = ES_NOK ;

	u8 Local_u8ClkSelect , Local_u8Flag = 0 ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < Timers_u8MaxNum ; Local_u8Iter++ )
	{
		if( Timers[Local_u8Iter].TimerNum == TIMER0)
		{
			TIMSK &= ~( TC0_INT_EN_MASK ) ;			// Disable All Interrupts before setting all conditions
			TCCR0 = 0x00 ;							// Masks all bits in TCCR0
			OCR0  = 0x00 ;							// Clears Output Compare Register
			/*	Set Clock Selection	*/
			Global_u32Timer0_Clk = Timer_u32TimerClock( TIMER0 , Timers[Local_u8Iter].ClkSelect ,&Local_u8ClkSelect);
			TCCR0 |= ( Local_u8ClkSelect  << CLK0_SEL_BITS );
			/*	Set Compare Output Mode	*/
			TCCR0 |= ( ( Timers[Local_u8Iter].CompOutMode - COMP_NORMAL ) << COMP0_MAT_OUT_MODE_BITS );
			/*	Set Waveform Generation Mode	*/
			if( Timers[Local_u8Iter].WaveGenMode == WGM_NORMAL_MODE	||
				Timers[Local_u8Iter].WaveGenMode == WGM_CTC_MODE	)
			{
				TCCR0 |= ( ( ( Timers[Local_u8Iter].WaveGenMode - WGM_NORMAL_MODE ) >> BIT0_MASK ) << WGM01_BIT );
			}
			else
			{
				Local_enuErrorState = ES_OUT_RANGE ;
				#warning "Timer_Init(): Non Supported Waveform Gen mode for Timer0. Timer0 WGM is set to WGM_NORMAL_MODE"
			}
			/*	Set Interrupt Mode	*/
			switch( Timers[Local_u8Iter].InterruptMode )
			{
				case TC_OVERFLOW_INT	:	TIMSK |= ( BIT0_MASK << TOIE0_BIT );	/*	Enable TOIE0 Interrupt */
											break;
				case TC_OUT_COMP_INT	:	TIMSK |= ( BIT0_MASK << OCIE0_BIT );	/*	Enable OCIE0 Interrupt */
											break;

				default :	Local_enuErrorState = ES_OUT_RANGE ;
							#warning " Timer_Init(): Non Supported Interrupt mode for Timer0. Timer0 Interrupts are Disabled"
							/*Timer Interrupts are already disabled at beginning of setting value, no extra action is needed*/
			}
		}
		else if( Timers[Local_u8Iter].TimerNum == TIMER2)
		{
			TIMSK &= ~( TC2_INT_EN_MASK ) ;			// Disable All Interrupts before setting all conditions
			TCCR2 = 0x00 ;							// Masks all bits in TCCR2
			OCR2  = 0x00 ;							// Clears Output Compare Register

			/*	Set Clock Selection	*/
			Global_u32Timer2_Clk = Timer_u32TimerClock( TIMER2 , Timers[Local_u8Iter].ClkSelect ,&Local_u8ClkSelect);
			TCCR2 |= ( Local_u8ClkSelect << CLK2_SEL_BITS );
			/*	Set Compare Output Mode	*/
			TCCR2 |= ( ( Timers[Local_u8Iter].CompOutMode - COMP_NORMAL ) << COMP2_MAT_OUT_MODE_BITS );
			/*	Set Waveform Generation Mode	*/
			if( Timers[Local_u8Iter].WaveGenMode == WGM_NORMAL_MODE ||
				Timers[Local_u8Iter].WaveGenMode == WGM_CTC_MODE	 )
			{
				TCCR2 |= ( ( ( Timers[Local_u8Iter].WaveGenMode - WGM_NORMAL_MODE ) >> BIT0_MASK ) << WGM21_BIT );
			}
			else
			{
				Local_enuErrorState = ES_OUT_RANGE ;
				#warning "Timer_Init(): Non Supported Waveform Gen mode for Timer2. Timer2 WGM is set to WGM_NORMAL_MODE"
			}
			/*	Set Interrupt Mode	*/
			switch( Timers[Local_u8Iter].InterruptMode )
			{
				case TC_OVERFLOW_INT	:	TIMSK |= ( BIT0_MASK << TOIE2_BIT );	/*	Enable TOIE2 Interrupt */
											break;
				case TC_OUT_COMP_INT	:	TIMSK |= ( BIT0_MASK << OCIE2_BIT );	/*	Enable OCIE2 Interrupt */
											break;

				default :	Local_enuErrorState = ES_OUT_RANGE ;
							#warning " Timer_Init(): Non Supported Interrupt mode for Timer2. Timer2 Interrupts are Disabled"
							/*Timer Interrupts are already disabled at beginning of setting value, no extra action is needed*/
			}
		}
		else
		{
			Local_enuErrorState = ES_OUT_RANGE ;
			#warning " Timer_Init(): Non Supported Timer Number , Initialization is terminated."
			break;
		}
	}

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

static u32 Timer_u32TimerClock( u8 Copy_u8TimerNum ,u8 Copy_u8ClkSelectNum ,u8 *Copy_pu8ClkSelectPrescalar )
{
	u32 Copy_u32TimerClk = CPU_CLOCK;


	switch( Copy_u8ClkSelectNum )
	{
		case NO_CLK		:	*Copy_pu8ClkSelectPrescalar = 0;
							Copy_u32TimerClk = 0 ;
							break;
		case PRES_1		:	*Copy_pu8ClkSelectPrescalar = 1;
						break;
		case PRES_8 	:	*Copy_pu8ClkSelectPrescalar = 2;
							Copy_u32TimerClk >>= 3 ;
						break;
		case PRES_32 	:	if( Copy_u8TimerNum == TIMER2 )
							{
								*Copy_pu8ClkSelectPrescalar = 3;
								Copy_u32TimerClk >>= 5 ;
								break;
							}
							#warning "Clock Select Number selected is not supported in Timer0/Timer1 , shifted to PRES_64"
							/*	NO BREAK IN CASE OF NOT TIMER2	*/
		case PRES_64 	:	Copy_u32TimerClk >>= 6 ;
							if( Copy_u8TimerNum == TIMER2 )	*Copy_pu8ClkSelectPrescalar = 4;
							else *Copy_pu8ClkSelectPrescalar = 3;
						break;
		case PRES_128 	:	if( Copy_u8TimerNum == TIMER2 )
							{
								*Copy_pu8ClkSelectPrescalar = 5;
								Copy_u32TimerClk >>= 7 ;
								break;
							}
							#warning "Clock Select Number selected is not supported in Timer0/Timer1 , shifted to PRES_256"
							/*	NO BREAK IN CASE OF NOT TIMER2	*/
		case PRES_256	 :	Copy_u32TimerClk >>= 8 ;
							if( Copy_u8TimerNum == TIMER2 )	*Copy_pu8ClkSelectPrescalar = 6;
							else *Copy_pu8ClkSelectPrescalar = 4;
						break;
		case PRES_1024 :	Copy_u32TimerClk >>= 10 ;
							if( Copy_u8TimerNum == TIMER2 )	*Copy_pu8ClkSelectPrescalar = 7;
							else *Copy_pu8ClkSelectPrescalar = 5;
						break;
		case EXT_CLK_FALL	 :		Copy_u32TimerClk = EXT_CLOCK ;
									if( Copy_u8TimerNum == TIMER0 )
									{
										*Copy_pu8ClkSelectPrescalar = 6;
										break;
									}
									#warning "External Clock is not supported in Timer2 , NO Clock mode is selected"
						break;
		case EXT_CLK_RISE	 :		Copy_u32TimerClk = EXT_CLOCK ;
									if( Copy_u8TimerNum == TIMER0 )
									{
										*Copy_pu8ClkSelectPrescalar = 7;
										break;
									}
									#warning "External Clock is not supported in Timer2 , NO Clock mode is selected"
						break;
		default:	Copy_u32TimerClk = 0;
					#warning "Unsupported Clock mode is selected , Timer is disabled"
	}

	return Copy_u32TimerClk ;

}

ES_t Timer_enuGetClock( u8 Copy_u8TimerNum , u32 *Copy_pu32TimerClk)
{
	ES_t Local_enuErrorState = ES_NOK ;

	if( Copy_u8TimerNum == TIMER0 )
		*Copy_pu32TimerClk = Global_u32Timer0_Clk;
	else if( Copy_u8TimerNum == TIMER2 )
		*Copy_pu32TimerClk = Global_u32Timer2_Clk;
	else Local_enuErrorState = ES_OUT_RANGE ;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}


ES_t Timer_enuSetClkPrescaler( u8 Copy_u8TimerNum , u8 Copy_u8PrescalerValue )
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8ClkSelectPrescaler = NO_CLK ;
	u8 Local_u8CopyTIMSK = TIMSK ;									// Saving a Copy of Timer Interrupt Mask Register

	u32 Local_u32TimerClk = Timer_u32TimerClock( Copy_u8TimerNum , Copy_u8PrescalerValue , &Local_u8ClkSelectPrescaler ) ;

	if( Copy_u8TimerNum == TIMER0 )
	{
		TIMSK &= ~( TC0_INT_EN_MASK ) ;								// Disable Timer0 Interrupts
		TCCR0 &= ~( CLK0_SEL_BITS_MASK ) ;							// Masking Clock Select bits
		TCCR0 |= ( Local_u8ClkSelectPrescaler  << CLK0_SEL_BITS ) ;	// Setting New Prescaler
		Global_u32Timer0_Clk = Local_u32TimerClk ;					// Saving New Actual Timer0 Clock
		Timers[ TIMER0 - TIMER0 ].ClkSelect = Copy_u8PrescalerValue ;
	}
	else if( Copy_u8TimerNum == TIMER2 )
	{
		TIMSK &= ~( TC2_INT_EN_MASK ) ;								// Disable Timer2 Interrupts
		TCCR2 &= ~( CLK2_SEL_BITS_MASK );							// Masking Clock Select bits
		TCCR2 |= ( Local_u8ClkSelectPrescaler  << CLK2_SEL_BITS );	// Setting New Prescaler
		Global_u32Timer2_Clk = Local_u32TimerClk ;					// Saving New Actual Timer2 Clock
		Timers[ TIMER2 - TIMER0 ].ClkSelect = Copy_u8PrescalerValue ;
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	TIMSK = Local_u8CopyTIMSK ;										// Re-setting Timer Interrupt Mask Register to its Status

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t Timer_enuGetClkSelect( u8 Copy_u8TimerNum , u16 *Copy_pu8TimerClkSelect )
{
	ES_t Local_enuErrorState = ES_OUT_RANGE ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < Timers_u8MaxNum ; Local_u8Iter++ )
	{
		if( Timers[Local_u8Iter].TimerNum == Copy_u8TimerNum )
		{
			*Copy_pu8TimerClkSelect = Timers[Local_u8Iter].ClkSelect;
			Local_enuErrorState = ES_OK ;
		}
	}


	return Local_enuErrorState ;
}

ES_t Timer_enuSetOCn_Mode( u8 Copy_u8TimerNum , u8 Copy_u8COM_Mode )
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8CopyTIMSK = TIMSK ;														// Saving a Copy of Timer Interrupt Mask Register

	if( Copy_u8TimerNum == TIMER0 )
	{
		TIMSK &= ~( TC0_INT_EN_MASK ) ;													// Disable Timer0 Interrupts
		TCCR0 &= ~( COMP0_MAT_OUT_MODE_BITS_MASK ) ;									// Masking COM Select bits
		TCCR0 |= ( ( Copy_u8COM_Mode - COMP_NORMAL) << COMP0_MAT_OUT_MODE_BITS ) ;		// Setting New Compare Output Mode
		Timers[ TIMER0 - TIMER0 ].CompOutMode = Copy_u8COM_Mode ;
	}
	else if( Copy_u8TimerNum == TIMER2 )
	{
		TIMSK &= ~( TC2_INT_EN_MASK ) ;													// Disable Timer2 Interrupts
		TCCR2 &= ~( COMP2_MAT_OUT_MODE_BITS_MASK ) ;									// Masking COM Select bits
		TCCR2 |= ( ( Copy_u8COM_Mode - COMP_NORMAL) << COMP2_MAT_OUT_MODE_BITS ) ;		// Setting New Compare Output Mode
		Timers[ TIMER2 - TIMER0 ].CompOutMode = Copy_u8COM_Mode ;
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	TIMSK = Local_u8CopyTIMSK ;															// Re-setting Timer Interrupt Mask Register to its Status

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t Timer_enuGetOCn_Mode( u8 Copy_u8TimerNum , u8 *Copy_pu8TimerCOM_Mode)
{
	ES_t Local_enuErrorState = ES_OUT_RANGE ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < Timers_u8MaxNum ; Local_u8Iter++ )
	{
		if( Timers[Local_u8Iter].TimerNum == Copy_u8TimerNum )
		{
			*Copy_pu8TimerCOM_Mode = Timers[Local_u8Iter].CompOutMode;
			Local_enuErrorState = ES_OK ;
		}
	}

	return Local_enuErrorState ;
}


ES_t Timer_enuSetTimer_Mode( u8 Copy_u8TimerNum , u8 Copy_u8WGM_Mode )
{
	ES_t Local_enuErrorState = ES_NOK;


	if( Copy_u8WGM_Mode == WGM_NORMAL_MODE || Copy_u8WGM_Mode == WGM_CTC_MODE || Copy_u8WGM_Mode == WGM_CTC_IRC1_MODE )
	{
		u8 Local_u8CopyTIMSK = TIMSK ;														// Saving a Copy of Timer Interrupt Mask Register

		if( Copy_u8TimerNum == TIMER0 && ( Copy_u8WGM_Mode == WGM_NORMAL_MODE || Copy_u8WGM_Mode == WGM_CTC_MODE ) )
		{
			TIMSK &= ~( TC0_INT_EN_MASK ) ;													// Disable Timer0 Interrupts
			TCCR0 &= ~( BIT0_MASK<< WGM01_BIT ) ;											// Masking WGM01 Select bit
			TCCR0 |= ((( Copy_u8WGM_Mode - WGM_NORMAL_MODE ) >> BIT0_MASK ) << WGM01_BIT );	// Setting New Waveform Generation Mode
			Timers[ TIMER0 - TIMER0 ].WaveGenMode = Copy_u8WGM_Mode ;
		}
		else if( Copy_u8TimerNum == TIMER2 && ( Copy_u8WGM_Mode == WGM_NORMAL_MODE || Copy_u8WGM_Mode == WGM_CTC_MODE ) )
		{
			TIMSK &= ~( TC2_INT_EN_MASK ) ;													// Disable Timer2 Interrupts
			TCCR2 &= ~( BIT0_MASK<< WGM21_BIT ) ;											// Masking WGM21 Select bit
			TCCR2 |= ((( Copy_u8WGM_Mode - WGM_NORMAL_MODE ) >> BIT0_MASK ) << WGM21_BIT  );// Setting New Waveform Generation Mode
			Timers[ TIMER2 - TIMER0 ].WaveGenMode = Copy_u8WGM_Mode ;
		}
		else Local_enuErrorState = ES_OUT_RANGE ;

		TIMSK = Local_u8CopyTIMSK ;															// Re-setting Timer Interrupt Mask Register to its Status
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t Timer_enuGetTimer_Mode( u8 Copy_u8TimerNum , u8 *Copy_pu8TimerWGM_Mode)
{
	ES_t Local_enuErrorState = ES_OUT_RANGE ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < Timers_u8MaxNum ; Local_u8Iter++ )
	{
		if( Timers[Local_u8Iter].TimerNum == Copy_u8TimerNum )
		{
			*Copy_pu8TimerWGM_Mode = Timers[Local_u8Iter].WaveGenMode;
			Local_enuErrorState = ES_OK ;
		}
	}
	return Local_enuErrorState ;
}

ES_t Timer_enuReset( u8 Copy_u8TimerNum )
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8Temp = SREG ;									// Saving a Copy of AVR Status Register
	asm( "CLI" );												// Disable All Interrupts while writing to Counter Register

	if( Copy_u8TimerNum == TIMER0 )
	{
		TCNT0 = 0x00 ;
	}
	else if( Copy_u8TimerNum == TIMER2 )
	{
		TCNT2 = 0x00 ;
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	SREG = Local_u8Temp;										// Re-setting AVR Status Register to its Status

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t Timer_enuPreLoad( u8 Copy_u8TimerNum , u16 Copy_u16PreLoad) ///////////////////////////////////////////////////////////////////
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8Temp = SREG ;									// Saving a Copy of AVR Status Register
	asm( "CLI" );												// Disable All Interrupts while writing to Counter Register

	if( Copy_u8TimerNum == TIMER0 )
	{
		TCNT0 = (u8)Copy_u16PreLoad ;
	}
	else if( Copy_u8TimerNum == TIMER2 )
	{
		TCNT2 = (u8)Copy_u16PreLoad ;
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	SREG = Local_u8Temp;										// Re-setting AVR Status Register to its Status

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t Timer_enuReadCounterValue( u8 Copy_u8TimerNum , void *Copy_pCounterValue )
{
	ES_t Local_enuErrorState = ES_NOK;

	if( Copy_pCounterValue != NULL )
	{
		if( Copy_u8TimerNum == TIMER0 )
		{
			*( (u8 *)Copy_pCounterValue ) = TCNT0 ;
		}
		else if( Copy_u8TimerNum == TIMER2 )
		{
			*( (u8 *)Copy_pCounterValue ) = TCNT2 ;
		}
		else Local_enuErrorState = ES_OUT_RANGE ;
	}
	else Local_enuErrorState = ES_NULL_POINTER ;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t Timer_enuSetOCRnValue( u8 Copy_u8TimerNum , u16 Copy_u16OCRnValue )
{
	ES_t Local_enuErrorState = ES_NOK ;

	if( Copy_u8TimerNum == TIMER0 && Copy_u16OCRnValue <= Timer0Max )
	{
		OCR0 = Copy_u16OCRnValue ;
		Global_u8OCR0_Value = OCR0 ;
	}
	else if( Copy_u8TimerNum == TIMER2 && Copy_u16OCRnValue <= Timer2Max )
	{
		OCR2 = Copy_u16OCRnValue ;
		Global_u8OCR2_Value = OCR2 ;
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t Timer_enuReadOCRnValue( u8 Copy_u8TimerNum , void *Copy_pCounterValue )
{
	ES_t Local_enuErrorState = ES_NOK ;

		if( Copy_u8TimerNum == TIMER0 )
		{
			*( (u8 *)Copy_pCounterValue ) = Global_u8OCR0_Value ;
		}
		else if( Copy_u8TimerNum == TIMER2 )
		{
			*( (u8 *)Copy_pCounterValue ) = Global_u8OCR2_Value ;
		}
		else Local_enuErrorState = ES_OUT_RANGE ;

		return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ES_t Timer_PollingDelay(u8 Copy_u8TimerNum ,u16 Copy_u16Delay_ms ) // POLLING Delay*******************************************///////////////////////////////////
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8COMmode = 0;						/////////////////////////////////////////////////////******************************************
	u32 Local_u32OverFlowCounts ;

	for( u8 Local_u8Iter = 0 ; Local_u8Iter < Timers_u8MaxNum ; Local_u8Iter++ )
	{
		if( Timers[Local_u8Iter].TimerNum == Copy_u8TimerNum )
			{
				Local_u8COMmode = Timers[Local_u8Iter].CompOutMode;
				break;
			}
	}

	if( Copy_u8TimerNum == TIMER0)
	{
		if( Global_u32Timer0_Clk )
		{
			switch( Local_u8COMmode )
			{
			case  COMP_NORMAL		:	Local_u32OverFlowCounts = ( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer0_Clk ) / ( 1000UL ) )
																										+ TIMER0_MAX ) / ( TIMER0_MAX + 1 );	/*	Total Overflows	*/
										TCNT0 = ( TIMER0_MAX + 1 ) -
												( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer0_Clk ) / ( 1000UL ) ) % (u16)( TIMER0_MAX + 1 ) );	/* PRE-LOAD value	*/									/*	PreLoad Value	*/

										while( Local_u32OverFlowCounts > TIMER0_BOTTOM )									/*	Delay LOOP							*/
										{
											while( !(( TIFR >> TOV0_BIT) & BIT0_MASK ) );									/*	Waiting for Timer0 Overflow flag	*/
											Local_u32OverFlowCounts--;
											TIFR |= (BIT0_MASK << TOV0_BIT);												/*	Clearing Timer0 Overflow flag		*/
										}
										break;

			case  COMP_TOG_ON_MATCH	:	Local_u32OverFlowCounts = ( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer0_Clk ) / (2 * 1000UL ) )
																				+ Global_u8OCR0_Value ) / (u16)( Global_u8OCR0_Value + 1 ) ;				/*	Total Overflows	*/
										TCNT0 = ( Global_u8OCR0_Value + 1 ) -
												( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer0_Clk ) / ( 2* 1000UL ) ) % (u16)( Global_u8OCR0_Value + 1 ) );/*	PreLoad Value	*/

										while( Local_u32OverFlowCounts > TIMER0_BOTTOM )									/*	Delay LOOP								*/
										{
										while( !(( TIFR >> OCF0_BIT) & BIT0_MASK ) );										/*	Waiting for Timer0 Output Compare flag	*/
										Local_u32OverFlowCounts--;
										TIFR |= (BIT0_MASK << OCF0_BIT);													/*	Clearing Timer0 Overflow flag			*/
										}
										break;

			case  COMP_NON_INVERTED	:	/*	Same as COMP_INVERTED case	*/
			case  COMP_INVERTED		:	Local_u32OverFlowCounts = ( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer0_Clk ) / ( 1000UL ) )
																				+ Global_u8OCR0_Value ) / ( Global_u8OCR0_Value + 1 );						/*	Total Overflows	*/
										TCNT0 = ( Global_u8OCR0_Value + 1 ) -
												( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer0_Clk ) / ( 1000UL ) ) % (u16)( Global_u8OCR0_Value + 1 ) );	/*	PreLoad Value	*/

										while( Local_u32OverFlowCounts > TIMER0_BOTTOM )
										{
										while( !(( TIFR >> OCF0_BIT) & BIT0_MASK ) );	/*	Waiting for Timer0 Output Compare flag	*/
										Local_u32OverFlowCounts--;
										TIFR |= (BIT0_MASK << OCF0_BIT);	/*	Clearing Timer0 Overflow flag	*/
										}
										break;
			default		: Local_enuErrorState = ES_OUT_RANGE;
										break;
			}
		}
	}
#if MASK==0

	else if( Copy_u8TimerNum == TIMER1A)
	{
		if( Global_u32Timer1_Clk )
		{
			u16 Local_u16CounterPreLoad ;
			u8 Local_u8CopySREG = SREG ;

			switch( Local_u8COMmode )
			{
			case  COMP_NORMAL		:	Local_u32OverFlowCounts = ( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer1_Clk ) / ( 1000UL ) )
																										+ TIMER1_MAX ) / ( TIMER1_MAX + 1 );	/*	Total Overflows	*/
										Local_u16CounterPreLoad = ( TIMER1_MAX + 1 ) -
												( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer1_Clk ) / ( 1000UL ) ) % (u16)( TIMER1_MAX + 1 ) );	/* PRE-LOAD value	*/
										asm( "CLI" );
										TCNT1H = ( Local_u16CounterPreLoad >> 8) ;
										TCNT1L = Local_u16CounterPreLoad ;
										SREG = Local_u8CopySREG ;
										while( Local_u32OverFlowCounts > TIMER1_BOTTOM )									/*	Delay LOOP							*/
										{
											while( !(( TIFR >> TOV1_BIT) & BIT0_MASK ) );									/*	Waiting for Timer1 Overflow flag	*/
											Local_u32OverFlowCounts--;
											TIFR |= (BIT0_MASK << TOV1_BIT);												/*	Clearing Timer1 Overflow flag		*/
										}
										break;

			case  COMP_TOG_ON_MATCH	:	Local_u32OverFlowCounts = ( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer1_Clk ) / (2 * 1000UL ) )
																				+ Global_u8OCR1A_Value ) / (u16)( Global_u8OCR1A_Value + 1 ) ;	/*	Total Overflows	*/
										Local_u16CounterPreLoad = ( Global_u8OCR1A_Value + 1 ) -
												( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer1_Clk ) / ( 2* 1000UL ) ) % (u16)( Global_u8OCR1A_Value + 1 ) );	/*	PreLoad Value	*/
										asm( "CLI" );
										TCNT1H = ( Local_u16CounterPreLoad >> 8) ;
										TCNT1L = Local_u16CounterPreLoad ;
										SREG = Local_u8CopySREG ;
										while( Local_u32OverFlowCounts > TIMER1_BOTTOM )									/*	Delay LOOP							*/
										{
										while( !(( TIFR >> OCF1A_BIT) & BIT0_MASK ) );										/*	Waiting for Timer1A Output Compare flag	*/
										Local_u32OverFlowCounts--;
										TIFR |= (BIT0_MASK << OCF1A_BIT);													/*	Clearing Timer1A Output Compare flag	*/
										}
										break;

			case  COMP_NON_INVERTED	:	/*	Same as COMP_INVERTED case	*/
			case  COMP_INVERTED		:	Local_u32OverFlowCounts = ( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer1_Clk ) / ( 1000UL ) )
																				+ Global_u8OCR1A_Value ) / ( Global_u8OCR1A_Value + 1 );					/*	Total Overflows	*/
										Local_u16CounterPreLoad = ( Global_u8OCR1A_Value + 1 ) -
												( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer1_Clk ) / ( 1000UL ) ) % (u16)( Global_u8OCR1A_Value + 1 )) ;	/*	PreLoad Value	*/
										asm( "CLI" );
										TCNT1H = ( Local_u16CounterPreLoad >> 8) ;
										TCNT1L = Local_u16CounterPreLoad ;
										SREG = Local_u8CopySREG ;
										while( Local_u32OverFlowCounts > TIMER1_BOTTOM )
										{
										while( !(( TIFR >> OCF1A_BIT) & BIT0_MASK ) );										/*	Waiting for Timer1A Output Compare flag	*/
										Local_u32OverFlowCounts--;
										TIFR |= (BIT0_MASK << OCF1A_BIT);													/*	Clearing Timer1A Output Compare flag	*/
										}
										break;
			default		: Local_enuErrorState = ES_OUT_RANGE;
										break;
			}
		}

	}
	else if( Copy_u8TimerNum == TIMER1B)
	{
		if( Global_u32Timer1_Clk )
		{
			u16 Local_u16CounterPreLoad ;
			u8 Local_u8CopySREG = SREG ;

			switch( Local_u8COMmode )
			{
			case  COMP_NORMAL		:	Local_u32OverFlowCounts = ( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer1_Clk ) / ( 1000UL ) )
																										+ TIMER1_MAX ) / ( TIMER1_MAX + 1 );	/*	Total Overflows	*/
										Local_u16CounterPreLoad = ( TIMER1_MAX + 1 ) -
												( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer1_Clk ) / ( 1000UL ) ) % (u16)( TIMER1_MAX + 1 ) );	/* PRE-LOAD value	*/
										asm( "CLI" );
										TCNT1H = ( Local_u16CounterPreLoad >> 8) ;
										TCNT1L = Local_u16CounterPreLoad ;
										SREG = Local_u8CopySREG ;
										while( Local_u32OverFlowCounts > TIMER1_BOTTOM )									/*	Delay LOOP							*/
										{
											while( !(( TIFR >> TOV1_BIT) & BIT0_MASK ) );									/*	Waiting for Timer1 Overflow flag	*/
											Local_u32OverFlowCounts--;
											TIFR |= (BIT0_MASK << TOV1_BIT);												/*	Clearing Timer1 Overflow flag		*/
										}
										break;

			case  COMP_TOG_ON_MATCH	:	Local_u32OverFlowCounts = ( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer1_Clk ) / (2 * 1000UL ) )
																				+ Global_u8OCR1B_Value ) / (u16)( Global_u8OCR1B_Value + 1 ) ;	/*	Total Overflows	*/
										Local_u16CounterPreLoad = ( Global_u8OCR1B_Value + 1 ) -
												( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer1_Clk ) / ( 2* 1000UL ) ) % (u16)( Global_u8OCR1B_Value + 1 ) );	/*	PreLoad Value	*/
										asm( "CLI" );
										TCNT1H = ( Local_u16CounterPreLoad >> 8) ;
										TCNT1L = Local_u16CounterPreLoad ;
										SREG = Local_u8CopySREG ;
										while( Local_u32OverFlowCounts > TIMER1_BOTTOM )									/*	Delay LOOP							*/
										{
										while( !(( TIFR >> OCF1B_BIT) & BIT0_MASK ) );										/*	Waiting for Timer1A Output Compare flag	*/
										Local_u32OverFlowCounts--;
										TIFR |= (BIT0_MASK << OCF1B_BIT);													/*	Clearing Timer1A Output Compare flag	*/
										}
										break;

			case  COMP_NON_INVERTED	:	/*	Same as COMP_INVERTED case	*/
			case  COMP_INVERTED		:	Local_u32OverFlowCounts = ( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer1_Clk ) / ( 1000UL ) )
																				+ Global_u8OCR1B_Value ) / ( Global_u8OCR1B_Value + 1 );					/*	Total Overflows	*/
										Local_u16CounterPreLoad = ( Global_u8OCR1B_Value + 1 ) -
												( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer1_Clk ) / ( 1000UL ) ) % (u16)( Global_u8OCR1B_Value + 1 )) ;	/*	PreLoad Value	*/
										asm( "CLI" );
										TCNT1H = ( Local_u16CounterPreLoad >> 8) ;
										TCNT1L = Local_u16CounterPreLoad ;
										SREG = Local_u8CopySREG ;
										while( Local_u32OverFlowCounts > TIMER1_BOTTOM )
										{
										while( !(( TIFR >> OCF1B_BIT) & BIT0_MASK ) );										/*	Waiting for Timer1A Output Compare flag	*/
										Local_u32OverFlowCounts--;
										TIFR |= (BIT0_MASK << OCF1B_BIT);													/*	Clearing Timer1A Output Compare flag	*/
										}
										break;
			default		: Local_enuErrorState = ES_OUT_RANGE;
										break;
			}
		}

	}
#endif

	else if( Copy_u8TimerNum == TIMER2 )
	{
		if( Global_u32Timer2_Clk )
		{
			switch( Local_u8COMmode )
			{
			case  COMP_NORMAL		:	Local_u32OverFlowCounts = ( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer2_Clk ) / ( 1000UL ) )
																										+ TIMER2_MAX ) / ( TIMER2_MAX + 1 );	/*	Total Overflows	*/
										TCNT2 = ( TIMER2_MAX + 1 ) -
												( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer2_Clk ) / ( 1000UL ) ) % (u16)( TIMER2_MAX + 1 ) );	/* PRE-LOAD value	*/									/*	PreLoad Value	*/

										while( Local_u32OverFlowCounts > TIMER2_BOTTOM )									/*	Delay LOOP							*/
										{
											while( !(( TIFR >> TOV2_BIT) & BIT0_MASK ) );									/*	Waiting for Timer2 Overflow flag	*/
											Local_u32OverFlowCounts--;
											TIFR |= (BIT0_MASK << TOV2_BIT);												/*	Clearing Timer2 Overflow flag		*/
										}
										break;

			case  COMP_TOG_ON_MATCH	:	Local_u32OverFlowCounts = ( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer2_Clk ) / (2 * 1000UL ) )
																				+ Global_u8OCR2_Value ) / (u16)( Global_u8OCR2_Value + 1 ) ;				/*	Total Overflows	*/
										TCNT2 = ( Global_u8OCR2_Value + 1 ) -
												( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer2_Clk ) / ( 2* 1000UL ) ) % (u16)( Global_u8OCR2_Value + 1 ) );/*	PreLoad Value	*/

										while( Local_u32OverFlowCounts > TIMER2_BOTTOM )									/*	Delay LOOP								*/
										{
										while( !(( TIFR >> OCF2_BIT) & BIT0_MASK ) );										/*	Waiting for Timer2 Output Compare flag	*/
										Local_u32OverFlowCounts--;
										TIFR |= (BIT0_MASK << OCF2_BIT);													/*	Clearing Timer2 Overflow flag			*/
										}
										break;

			case  COMP_NON_INVERTED	:	/*	Same as COMP_INVERTED case	*/
			case  COMP_INVERTED		:	Local_u32OverFlowCounts = ( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer2_Clk ) / ( 1000UL ) )
																				+ Global_u8OCR2_Value ) / ( Global_u8OCR2_Value + 1 );						/*	Total Overflows	*/
										TCNT2 = ( Global_u8OCR2_Value + 1 ) -
												( ( ( Copy_u16Delay_ms * (u64)Global_u32Timer2_Clk ) / ( 1000UL ) ) % (u16)( Global_u8OCR2_Value + 1 ) );	/*	PreLoad Value	*/

										while( Local_u32OverFlowCounts > TIMER2_BOTTOM )
										{
										while( !(( TIFR >> OCF2_BIT) & BIT0_MASK ) );										/*	Waiting for Timer2 Output Compare flag	*/
										Local_u32OverFlowCounts--;
										TIFR |= (BIT0_MASK << OCF2_BIT);													/*	Clearing Timer2 Overflow flag	*/
										}
										break;
			default		: Local_enuErrorState = ES_OUT_RANGE;
										break;
			}
		}

	}
	else Local_enuErrorState = ES_OUT_RANGE;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}


ES_t Timer_ISR_Delay(u8 Copy_u8TimerNum , u16 Copy_u16Delay_ms , void (*Copy_pFun)(void *) , void *Copy_pAppVar ) // ISR Delay
{
	ES_t Local_enuErrorState = ES_NOK;

	if(Copy_pFun != NULL)
	{



	}
	else Local_enuErrorState = ES_NULL_POINTER;


	return Local_enuErrorState ;
}




/*
void Timer_vidTOIE0(void *Copy_pTOV0Counter)
{
	( *( (u16 *)Copy_pTOV0Counter) )++;
}

void Timer_vidOCIE0(void *Copy_pOC0Counter )
{
	( *( (u16 *)Copy_pOC0Counter) )++;
}

*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ES_t Timer_enuInterruptEnable( u8 Copy_u8TimerIntName)
{
	ES_t Local_enuErrorState = ES_NOK;

	asm(" CLI ") ;														// Disable ALL Interrupts

	switch ( Copy_u8TimerIntName )
	{
		case TOIE0	:	TIMSK |= (BIT0_MASK << TOIE0_BIT);
						break;
		case OCIE0	:	TIMSK |= (BIT0_MASK << OCIE0_BIT);
						break;
#if MASK==0
		case TOIE1	:	TIMSK |= (BIT0_MASK << TOIE1_BIT);
						break;
		case OCIE1B	:	TIMSK |= (BIT0_MASK << OCIE1B_BIT);
						break;
		case OCIE1A	:	TIMSK |= (BIT0_MASK << OCIE1A_BIT);
						break;
		case TICIE1	:	TIMSK |= (BIT0_MASK << TICIE1_BIT);
						break;
#endif
		case TOIE2	:	TIMSK |= (BIT0_MASK << TOIE2_BIT);
						break;
		case OCIE2	:	TIMSK |= (BIT0_MASK << OCIE2_BIT);
						break;
		default		:	Local_enuErrorState = ES_OUT_RANGE;
						#warning " Timer_enuInterruptEnable() : Unidentified Interrupt Name, No Action Taken."
	}
	asm(" SEI ") ;														// Enable ALL Interrupts

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t Timer_enuInterruptDisable( u8 Copy_u8TimerIntName)
{
	ES_t Local_enuErrorState = ES_NOK;

	asm(" CLI ") ;														// Disable ALL Interrupts

	switch ( Copy_u8TimerIntName )
	{
		case TOIE0	:	TIMSK &= ~(BIT0_MASK << TOIE0_BIT);
						break;
		case OCIE0	:	TIMSK &= ~(BIT0_MASK << OCIE0_BIT);
						break;
#if MASK==0
		case TOIE1	:	TIMSK &= ~(BIT0_MASK << TOIE1_BIT);
						break;
		case OCIE1B	:	TIMSK &= ~(BIT0_MASK << OCIE1B_BIT);
						break;
		case OCIE1A	:	TIMSK &= ~(BIT0_MASK << OCIE1A_BIT);
						break;
		case TICIE1	:	TIMSK &= ~(BIT0_MASK << TICIE1_BIT);
						break;
#endif
		case TOIE2	:	TIMSK &= ~(BIT0_MASK << TOIE2_BIT);
						break;
		case OCIE2	:	TIMSK &= ~(BIT0_MASK << OCIE2_BIT);
						break;
		default		:	Local_enuErrorState = ES_OUT_RANGE;
						#warning " Timer_enuInterruptEnable() : Unidentified Interrupt Name, No Action Taken."
	}

	asm(" SEI ") ;														// Re-Enable ALL Interrupts

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

bool Timer_IsInterruptEnabled( u8 Copy_u8TimerIntName )
{
	u8 Local_u8Status = 0 ;

	switch ( Copy_u8TimerIntName )
	{
		case TOIE0	:	Local_u8Status |= ( (TIMSK >> TOIE0_BIT) & BIT0_MASK );
						break;
		case OCIE0	:	Local_u8Status |= ( (TIMSK >> OCIE0_BIT) & BIT0_MASK );
						break;
#if MASK==0
		case TOIE1	:	Local_u8Status |= ( (TIMSK >> TOIE1_BIT) & BIT0_MASK );
						break;
		case OCIE1B	:	Local_u8Status |= ( (TIMSK >> OCIE1B_BIT) & BIT0_MASK );
						break;
		case OCIE1A	:	Local_u8Status |= ( (TIMSK >> OCIE1A_BIT) & BIT0_MASK );
						break;
		case TICIE1	:	Local_u8Status |= ( (TIMSK >> TICIE1_BIT) & BIT0_MASK );
						break;
#endif
		case TOIE2	:	Local_u8Status |= ( (TIMSK >> TOIE2_BIT) & BIT0_MASK );
						break;
		case OCIE2	:	Local_u8Status |= ( (TIMSK >> OCIE2_BIT) & BIT0_MASK );
						break;
	}
	return ( ( Local_u8Status)? TRUE : FALSE ) ;
}


ES_t Timer_enuCallBack( u8 Copy_u8TimerIntName , void (*Copy_pAppFun)(void*) , void *Copy_pAppVar)
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 found = 0 ;

	if( Copy_pAppFun != NULL )
	{
		for( u8 Local_u8Iter; Local_u8Iter < TIMERS_INTERRUPTS ; Local_u8Iter++ )
		{
			if( Global_AstrTimerPointers[Local_u8Iter].InterruptName == Copy_u8TimerIntName )
			{
				Global_AstrTimerPointers[Local_u8Iter].ptrFun = Copy_pAppFun;
				Global_AstrTimerPointers[Local_u8Iter].ptrVar = Copy_pAppVar;
				Local_enuErrorState = ES_OK ;
				found = 1;
				break;
			}
		}
		if(!found) Local_enuErrorState = ES_OUT_RANGE ;
	}
	else Local_enuErrorState = ES_NULL_POINTER;

	return Local_enuErrorState ;
}

void __vector_4( void )__attribute__((signal));		/*	Timer2	Compare Match Interrupt	ISR		*/
void __vector_5( void )__attribute__((signal));		/*	Timer2	Overflow Interrupt	ISR			*/
#if MASK== 0
void __vector_6( void )__attribute__((signal));		/*	Timer1	Capture Event Interrupt	ISR		*/
void __vector_7( void )__attribute__((signal));		/*	Timer1	Compare Match A Interrupt ISR	*/
void __vector_8( void )__attribute__((signal));		/*	Timer1	Compare Match B Interrupt ISR	*/
void __vector_9( void )__attribute__((signal));		/*	Timer1	Overflow Interrupt	ISR			*/
#endif
void __vector_10( void )__attribute__((signal));	/*	Timer0	Compare Match Interrupt	ISR		*/
void __vector_11( void )__attribute__((signal));	/*	Timer0	Overflow Interrupt	ISR			*/

void __vector_4( void )/*	OCIE2	*/
{
	if( Global_AstrTimerPointers[3].ptrFun != NULL )
	{
		(*Global_AstrTimerPointers[3].ptrFun)( Global_AstrTimerPointers[3].ptrVar );
	}
}

void __vector_5( void )/*	TOIE2	*/
{
	if( Global_AstrTimerPointers[2].ptrFun != NULL )
	{
		(*Global_AstrTimerPointers[2].ptrFun)( Global_AstrTimerPointers[2].ptrVar );
	}
}
#if MASK==0
void __vector_6( void )/*	TICIE1	*/
{
	if( Global_AstrTimerPointers[5].ptrFun != NULL )
	{
		(*Global_AstrTimerPointers[5].ptrFun)( Global_AstrTimerPointers[5].ptrVar );
	}
}
void __vector_7( void )/*	OCIE1A	*/
{
	if( Global_AstrTimerPointers[4].ptrFun != NULL )
	{
		(*Global_AstrTimerPointers[4].ptrFun)( Global_AstrTimerPointers[4].ptrVar );
	}
}
void __vector_8( void )/*	OCIE1B	*/
{
	if( Global_AstrTimerPointers[3].ptrFun != NULL )
	{
		(*Global_AstrTimerPointers[3].ptrFun)( Global_AstrTimerPointers[3].ptrVar );
	}
}
void __vector_9( void )/*	TOIE1	*/
{
	if( Global_AstrTimerPointers[2].ptrFun != NULL )
	{
		(*Global_AstrTimerPointers[2].ptrFun)( Global_AstrTimerPointers[2].ptrVar );
	}
}

#endif

void __vector_10( void )/*	OCIE0	*/
{
	if( Global_AstrTimerPointers[1].ptrFun != NULL )
	{
		(*Global_AstrTimerPointers[1].ptrFun)( Global_AstrTimerPointers[1].ptrVar );
	}
}

void __vector_11( void )/*	TOIE0	*/
{
	if( Global_AstrTimerPointers[0].ptrFun != NULL )
	{
		(*Global_AstrTimerPointers[0].ptrFun)( Global_AstrTimerPointers[0].ptrVar );
	}
}




