/*
 * PWM_prog.c
 *
 *  Created on: Sep 5, 2022
 *      Author: basse
 */

#include "../../Libraries/stdTypes.h"
#include "../../Libraries/errorState.h"

#include "PWM_config.h"
#include "PWM_priv.h"

extern u8 PWM_u8MaxNum ;
extern u16 Timer1Max ;
extern PWM_t PWMs[];


static u16 Global_u16OCR1A_Value = 0x0000 , Global_u16OCR1B_Value = 0x0000 , Global_u16ICR1_Value = 0x0000;
static u32 Global_u32Timer1_Clk ;


PWM_Int_Pointers_t Global_AstrPWM_Pointers[PWM_INTERRUPTS] =	{
#if PWM==0
															{ TOIE0 ,	NULL, NULL},
															{ OCIE0 ,	NULL, NULL},
#endif
															{ TOIE1 ,	NULL, NULL},
															{ OCIE1B ,	NULL, NULL},
															{ OCIE1A ,	NULL, NULL},
															{ TICIE1 ,	NULL, NULL},
#if PWM==0
															{ TOIE2 ,	NULL, NULL},
															{ OCIE2 ,	NULL, NULL},
#endif
														};


ES_t PWM_enuInit( void )
{
	ES_t Local_enuErrorState = ES_NOK ;

	u8 Local_u8ClkSelect , Local_u8Flag = 0 ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < PWM_u8MaxNum ; Local_u8Iter++ )
	{
#if PWM==0
		if( Timers[Local_u8Iter].TimerNum == TIMER0)
		{
			TIMSK &= ~( TC0_INT_EN_MASK ) ;			// Disable All Interrupts before setting all conditions
			TCCR0 = 0x00 ;							// Masks all bits in TCCR0
			OCR0  = 0x00 ;							// Clears Output Compare Register

			/****************************************/
			/*			Set Clock Selection			*/
			/****************************************/
			Global_u32Timer0_Clk = PWM_u32TimerClock( TIMER0 , Timers[Local_u8Iter].ClkSelect ,&Local_u8ClkSelect);
			TCCR0 |= ( Local_u8ClkSelect  << CLK0_SEL_BITS );

			/****************************************/
			/*		Set Compare Output Mode			*/
			/****************************************/
			if( Timers[Local_u8Iter].CompOutMode == COMP_NON_INVERTED ||
				Timers[Local_u8Iter].CompOutMode == COMP_INVERTED		)
			{
				TCCR0 |= ( ( Timers[Local_u8Iter].CompOutMode - COMP_NORMAL ) << COMP0_MAT_OUT_MODE_BITS );
			}
			else
			{
				Local_enuErrorState = ES_OUT_RANGE ;
				#warning "PWM_Init(): Non Supported Compare Output mode for Timer0. Timer0 COM mode is set to COMP_NORMAL"
			}

			/****************************************/
			/*		Set Waveform Generation Mode	*/
			/****************************************/
			if( Timers[Local_u8Iter].WaveGenMode == WGM_PC_8_bit		||
				Timers[Local_u8Iter].WaveGenMode == WGM_FAST_8_bit	)
			{
				u8 Local_u8WGM;
				switch( Timers[Local_u8Iter].WaveGenMode )
				{
					case WGM_PC_8_bit		:	Local_u8WGM = WGM_MODE_01;
												break;
					case WGM_FAST_8_bit		:	Local_u8WGM = WGM_MODE_03;
												break;
				}
				TCCR0 |= ( BIT0_MASK << WGM00_BIT );
				TCCR0 |= ( ( Local_u8WGM >> BIT0_MASK ) << WGM01_BIT );
			}
			else
			{
				Local_enuErrorState = ES_OUT_RANGE ;
				#warning "PWM_Init(): Non Supported Waveform Gen mode for Timer0. Timer0 WGM is set to WGM_NORMAL_MODE"
			}

			/****************************************/
			/*			Set Interrupt Mode			*/
			/****************************************/
			switch( Timers[Local_u8Iter].InterruptMode )
			{
				case TC_OVERFLOW_INT	:	TIMSK |= ( BIT0_MASK << TOIE0_BIT );	/*	Enable TOIE0 Interrupt */
											break;
				case TC_OUT_COMP_INT	:	TIMSK |= ( BIT0_MASK << OCIE0_BIT );	/*	Enable OCIE0 Interrupt */
											break;

				default :	Local_enuErrorState = ES_OUT_RANGE ;
							#warning " PWM_Init(): Non Supported Interrupt mode for Timer0. Timer0 Interrupts are Disabled"
							/*Timer Interrupts are already disabled at beginning of setting value, no extra action is needed*/
			}
		}


		else
#endif
		if( PWMs[Local_u8Iter].TimerNum == TIMER1A || PWMs[Local_u8Iter].TimerNum == TIMER1B )
		{
			if( ! Local_u8Flag )
			{
				Local_u8Flag = 1 ;
				TIMSK &= ~( TC1_INT_EN_MASK ) ;				// Disable All Interrupts before setting all conditions
				TCCR1A = 0x00 ;								// Masks all bits in TCCR1A
				TCCR1B = 0x00 ;								// Masks all bits in TCCR1B
				u8 Local_u8CopySREG = SREG ;
				asm( "CLI" );
				OCR1AH = 0x00 ;								// Clears 1A HIGH-byte Output Compare Register
				OCR1AL = 0x00 ;								// Clears 1A LOW-byte Output Compare Register
				OCR1BH = 0x00 ;								// Clears 1B HIGH-byte Output Compare Register
				OCR1BL = 0x00 ;								// Clears 1B LOW-byte Output Compare Register
				SREG = Local_u8CopySREG ;

				/****************************************/
				/*			Set Clock Selection			*/
				/****************************************/
				Global_u32Timer1_Clk = PWM_u32TimerClock( TIMER1A , PWMs[Local_u8Iter].ClkSelect ,&Local_u8ClkSelect);
				TCCR1B |= ( Local_u8ClkSelect << CLK1_SEL_BITS );

				/****************************************/
				/*		Set Waveform Generation Mode	*/
				/****************************************/
				if( PWMs[Local_u8Iter].WaveGenMode > WGM_MODE_00	&& PWMs[Local_u8Iter].WaveGenMode <= WGM_FAST_OCR1A	&&
					PWMs[Local_u8Iter].WaveGenMode != WGM_MODE_04	&& PWMs[Local_u8Iter].WaveGenMode != WGM_MODE_12	&&
					PWMs[Local_u8Iter].WaveGenMode != WGM_MODE_13	)
				{
					u8 Local_u8WGM = PWMs[Local_u8Iter].WaveGenMode - WGM_MODE_00 ;
					TCCR1A |= ( ( Local_u8WGM & TWO_BITS_MASK ) << WGM1A_SEL_BITS );
					TCCR1B |= ( ( ( Local_u8WGM >> BIT1_MASK) & TWO_BITS_MASK ) << WGM1B_SEL_BITS );
				}
				else
				{
					Local_enuErrorState = ES_OUT_RANGE ;
					#warning "Timer_Init(): Non Supported Waveform Gen mode for Timer1. Timer1 WGM is set to WGM_NORMAL_MODE"
				}
			}
			/****************************************/
			/*		Set Compare Output Mode			*/
			/****************************************/
			if( PWMs[Local_u8Iter].CompOutMode == COMP_NON_INVERTED ||
				PWMs[Local_u8Iter].CompOutMode == COMP_INVERTED		)
			{
				if( PWMs[Local_u8Iter].TimerNum == TIMER1A )
				{
					/****************************************/
					/*		Set 1A Compare Output Mode		*/
					/****************************************/
					TCCR1A |= ( ( PWMs[Local_u8Iter].CompOutMode - COMP_NORMAL ) << COM1A_SEL_BITS );
				}
				else
				{
					/****************************************/
					/*		Set 1B Compare Output Mode		*/
					/****************************************/
					TCCR1A |= ( ( PWMs[Local_u8Iter].CompOutMode - COMP_NORMAL ) << COM1B_SEL_BITS );
				}
			}
			else
			{
				Local_enuErrorState = ES_OUT_RANGE ;
				#warning "PWM_Init(): Non Supported Compare Output mode for Timer1. Timer1 COM mode is set to COMP_NORMAL"
			}

			/****************************************/
			/*			Set Interrupt Mode			*/
			/****************************************/
			switch( PWMs[Local_u8Iter].InterruptMode )
			{
				case TC_OVERFLOW_INT	:	TIMSK |= ( BIT0_MASK << TOIE1_BIT );	/*	Enable TOIE1 Interrupt */
											break;
				case TC1_INPUT_CAPT_INT	:	TIMSK |= ( BIT0_MASK << TICIE1_BIT );	/*	Enable TICIE1 Interrupt */
											break;
				case TC_OUT_COMP_INT	:	if( PWMs[Local_u8Iter].TimerNum == TIMER1B )
											{
												TIMSK |= ( BIT0_MASK << OCIE1B_BIT );	/*	Enable OCIE1B Interrupt */
											}
											else
											{
												TIMSK |= ( BIT0_MASK << OCIE1A_BIT );	/*	Enable OCIE1A Interrupt */
											}
											break;
				default :	Local_enuErrorState = ES_OUT_RANGE ;
							#warning " Timer_Init(): Non Supported Interrupt mode for Timer1. Timer1 Interrupts are Disabled"
							/*Timer Interrupts are already disabled at beginning of setting value, no extra action is needed*/
			}
		}
#if PWM==0
		else if( Timers[Local_u8Iter].TimerNum == TIMER2)
		{
			TIMSK &= ~( TC2_INT_EN_MASK ) ;			// Disable All Interrupts before setting all conditions
			TCCR2 = 0x00 ;							// Masks all bits in TCCR2
			OCR2  = 0x00 ;							// Clears Output Compare Register

			/****************************************/
			/*			Set Clock Selection			*/
			/****************************************/
			Global_u32Timer2_Clk = PWM_u32TimerClock( TIMER2 , Timers[Local_u8Iter].ClkSelect ,&Local_u8ClkSelect);
			TCCR2 |= ( Local_u8ClkSelect << CLK2_SEL_BITS );

			/****************************************/
			/*		Set Compare Output Mode			*/
			/****************************************/
			if( Timers[Local_u8Iter].CompOutMode == COMP_NON_INVERTED ||
				Timers[Local_u8Iter].CompOutMode == COMP_INVERTED		)
			{
				TCCR2 |= ( ( Timers[Local_u8Iter].CompOutMode - COMP_NORMAL ) << COMP0_MAT_OUT_MODE_BITS );
			}
			else
			{
				Local_enuErrorState = ES_OUT_RANGE ;
				#warning "PWM_Init(): Non Supported Compare Output mode for Timer2. Timer2 COM mode is set to COMP_NORMAL"
			}

			/****************************************/
			/*		Set Waveform Generation Mode	*/
			/****************************************/
			if( Timers[Local_u8Iter].WaveGenMode == WGM_PC_8_bit		||
				Timers[Local_u8Iter].WaveGenMode == WGM_FAST_8_bit	)
			{
				u8 Local_u8WGM;
				switch( Timers[Local_u8Iter].WaveGenMode )
				{
					case WGM_PC_8_bit		:	Local_u8WGM = WGM_MODE_01;
												break;
					case WGM_FAST_8_bit		:	Local_u8WGM = WGM_MODE_03;
												break;
				}
				TCCR2 |= ( BIT0_MASK << WGM20_BIT );
				TCCR2 |= ( ( Local_u8WGM >> BIT0_MASK ) << WGM21_BIT );
			}
			else
			{
				Local_enuErrorState = ES_OUT_RANGE ;
				#warning "PWM_Init(): Non Supported Waveform Gen mode for Timer2. Timer2 WGM is set to WGM_NORMAL_MODE"
			}

			/****************************************/
			/*			Set Interrupt Mode			*/
			/****************************************/
			switch( Timers[Local_u8Iter].InterruptMode )
			{
				case TC_OVERFLOW_INT	:	TIMSK |= ( BIT0_MASK << TOIE2_BIT );	/*	Enable TOIE2 Interrupt */
											break;
				case TC_OUT_COMP_INT	:	TIMSK |= ( BIT0_MASK << OCIE2_BIT );	/*	Enable OCIE2 Interrupt */
											break;

				default :	Local_enuErrorState = ES_OUT_RANGE ;
							#warning " PWM_Init(): Non Supported Interrupt mode for Timer2. Timer2 Interrupts are Disabled"
							/*Timer Interrupts are already disabled at beginning of setting value, no extra action is needed*/
			}
		}
#endif
		else
		{
			Local_enuErrorState = ES_OUT_RANGE ;
			#warning " PWM_Init(): Non Supported Timer Number , Initialization is terminated."
			break;
		}
	}

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

static u32 PWM_u32TimerClock( u8 Copy_u8TimerNum ,u8 Copy_u8ClkSelectNum ,u8 *Copy_pu8ClkSelectPrescalar )
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
#if PWM==0
		case PRES_32 	:	if( Copy_u8TimerNum == TIMER2 )
							{
								*Copy_pu8ClkSelectPrescalar = 3;
								Copy_u32TimerClk >>= 5 ;
								break;
							}
							#warning "Clock Select Number selected is not supported in Timer0/Timer1 , shifted to PRES_64"
							/*	NO BREAK IN CASE OF NOT TIMER2	*/
#endif
		case PRES_64 	:	Copy_u32TimerClk >>= 6 ;
							//if( Copy_u8TimerNum == TIMER2 )	*Copy_pu8ClkSelectPrescalar = 4;
							//else
							*Copy_pu8ClkSelectPrescalar = 3;
						break;
#if PWM==0
		case PRES_128 	:	if( Copy_u8TimerNum == TIMER2 )
							{
								*Copy_pu8ClkSelectPrescalar = 5;
								Copy_u32TimerClk >>= 7 ;
								break;
							}
							#warning "Clock Select Number selected is not supported in Timer0/Timer1 , shifted to PRES_256"
							/*	NO BREAK IN CASE OF NOT TIMER2	*/
#endif
		case PRES_256	 :	Copy_u32TimerClk >>= 8 ;
							//if( Copy_u8TimerNum == TIMER2 )	*Copy_pu8ClkSelectPrescalar = 6;
							//else
							*Copy_pu8ClkSelectPrescalar = 4;
						break;
		case PRES_1024 :	Copy_u32TimerClk >>= 10 ;
							//if( Copy_u8TimerNum == TIMER2 )	*Copy_pu8ClkSelectPrescalar = 7;
							//else
							*Copy_pu8ClkSelectPrescalar = 5;
						break;
		case EXT_CLK_FALL	 :		Copy_u32TimerClk = EXT_CLOCK ;
									//if( Copy_u8TimerNum == TIMER0 || Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B || Copy_u8TimerNum == TIMER1 )
									//{
										*Copy_pu8ClkSelectPrescalar = 6;
										//break;
									//}
									//#warning "External Clock is not supported in Timer2 , NO Clock mode is selected"
						break;
		case EXT_CLK_RISE	 :		Copy_u32TimerClk = EXT_CLOCK ;
									//if( Copy_u8TimerNum == TIMER0 || Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B || Copy_u8TimerNum == TIMER1 )
									//{
										*Copy_pu8ClkSelectPrescalar = 7;
									//	break;
									//}
									//#warning "External Clock is not supported in Timer2 , NO Clock mode is selected"
						break;
		default:	Copy_u32TimerClk = 0;
					#warning "Unsupported Clock mode is selected , Timer is disabled"
	}

	return Copy_u32TimerClk ;

}

ES_t PWM_enuGetClock( u8 Copy_u8TimerNum , u32 *Copy_pu32TimerClk)
{
	ES_t Local_enuErrorState = ES_NOK ;

	if( Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B )
		*Copy_pu32TimerClk = Global_u32Timer1_Clk;
	else Local_enuErrorState = ES_OUT_RANGE ;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}


ES_t PWM_enuSetClkPrescaler( u8 Copy_u8TimerNum , u8 Copy_u8PrescalerValue )
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8ClkSelectPrescaler = NO_CLK ;
	u8 Local_u8CopyTIMSK = TIMSK ;									// Saving a Copy of Timer Interrupt Mask Register

	u32 Local_u32TimerClk = PWM_u32TimerClock( Copy_u8TimerNum , Copy_u8PrescalerValue , &Local_u8ClkSelectPrescaler ) ;

#if PWM==0
	if( Copy_u8TimerNum == TIMER0 )
	{
		TIMSK &= ~( TC0_INT_EN_MASK ) ;								// Disable Timer0 Interrupts
		TCCR0 &= ~( CLK0_SEL_BITS_MASK ) ;							// Masking Clock Select bits
		TCCR0 |= ( Local_u8ClkSelectPrescaler  << CLK0_SEL_BITS ) ;	// Setting New Prescaler
		Global_u32Timer0_Clk = Local_u32TimerClk ;					// Saving New Actual Timer0 Clock
		Timers[ TIMER0 - TIMER0 ].ClkSelect = Copy_u8PrescalerValue ;
	}
	else
#endif
	if( Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B )
	{
		TIMSK &= ~( TC1_INT_EN_MASK ) ;								// Disable Timer1 Interrupts
		TCCR1B &= ~( CLK1_SEL_BITS_MASK );							// Masking Clock Select bits
		TCCR1B |= ( Local_u8ClkSelectPrescaler  << CLK1_SEL_BITS );	// Setting New Prescaler
		Global_u32Timer1_Clk = Local_u32TimerClk ;					// Saving New Actual Timer1 Clock
		PWMs[ TIMER1A - TIMER1A ].ClkSelect = Copy_u8PrescalerValue ;
		PWMs[ TIMER1B - TIMER1A ].ClkSelect = Copy_u8PrescalerValue ;
	}
#if PWM==0
	else if( Copy_u8TimerNum == TIMER2 )
	{
		TIMSK &= ~( TC2_INT_EN_MASK ) ;								// Disable Timer2 Interrupts
		TCCR2 &= ~( CLK2_SEL_BITS_MASK );							// Masking Clock Select bits
		TCCR2 |= ( Local_u8ClkSelectPrescaler  << CLK2_SEL_BITS );	// Setting New Prescaler
		Global_u32Timer2_Clk = Local_u32TimerClk ;					// Saving New Actual Timer2 Clock
		Timers[ TIMER2 - TIMER0 ].ClkSelect = Copy_u8PrescalerValue ;
	}
#endif
	else Local_enuErrorState = ES_OUT_RANGE ;

	TIMSK = Local_u8CopyTIMSK ;										// Re-setting Timer Interrupt Mask Register to its Status

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t PWM_enuGetClkSelect( u8 Copy_u8TimerNum , u16 *Copy_pu8TimerClkSelect )
{
	ES_t Local_enuErrorState = ES_OUT_RANGE ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < PWM_u8MaxNum ; Local_u8Iter++ )
	{
		if( PWMs[Local_u8Iter].TimerNum == Copy_u8TimerNum )
		{
			*Copy_pu8TimerClkSelect = PWMs[Local_u8Iter].ClkSelect;
			Local_enuErrorState = ES_OK ;
		}
	}


	return Local_enuErrorState ;
}

ES_t PWM_enuSetCOM_Mode( u8 Copy_u8TimerNum , u8 Copy_u8COM_Mode )
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8CopyTIMSK = TIMSK ;														// Saving a Copy of Timer Interrupt Mask Register

	if( Copy_u8COM_Mode == COMP_NON_INVERTED ||	Copy_u8COM_Mode == COMP_INVERTED )
	{
#if PWM==0
		if( Copy_u8TimerNum == TIMER0 )
		{
			TIMSK &= ~( TC0_INT_EN_MASK ) ;													// Disable Timer0 Interrupts
			TCCR0 &= ~( COMP0_MAT_OUT_MODE_BITS_MASK ) ;									// Masking COM Select bits
			TCCR0 |= ( ( Copy_u8COM_Mode - COMP_NORMAL) << COMP0_MAT_OUT_MODE_BITS ) ;		// Setting New Compare Output Mode
			Timers[ TIMER0 - TIMER0 ].CompOutMode = Copy_u8COM_Mode ;
		}
		else
#endif
		if( Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B )
		{
			TIMSK &= ~( TC1_INT_EN_MASK ) ;													// Disable Timer1 Interrupts
			if( Copy_u8TimerNum == TIMER1A )
			{
				TCCR1A &= ~( COM1A_SEL_BITS_MASK ) ;										// Masking COM Select bits
				TCCR1A |= ( ( Copy_u8COM_Mode - COMP_NORMAL) << COM1A_SEL_BITS ) ;			// Setting New Compare Output Mode
				PWMs[ TIMER1A - TIMER1A ].CompOutMode = Copy_u8COM_Mode ;
			}
			else if( Copy_u8TimerNum == TIMER1B )
			{
				TCCR1B &= ~( COM1B_SEL_BITS_MASK ) ;										// Masking COM Select bits
				TCCR1B |= ( ( Copy_u8COM_Mode - COMP_NORMAL) << COM1B_SEL_BITS ) ;			// Setting New Compare Output Mode
				PWMs[ TIMER1B - TIMER1A ].CompOutMode = Copy_u8COM_Mode ;
			}
			else
			{
				#warning "Timer_enuSetCOM_Mode() : For setting COM mode please Specify TIMER1A / TIMER1B , TIMER1 choice is not allowed. No Action Taken. "
			}
		}
#if PWM==0
		else if( Copy_u8TimerNum == TIMER2 )
		{
			TIMSK &= ~( TC2_INT_EN_MASK ) ;													// Disable Timer2 Interrupts
			TCCR2 &= ~( COMP2_MAT_OUT_MODE_BITS_MASK ) ;									// Masking COM Select bits
			TCCR2 |= ( ( Copy_u8COM_Mode - COMP_NORMAL) << COMP2_MAT_OUT_MODE_BITS ) ;		// Setting New Compare Output Mode
			Timers[ TIMER2 - TIMER0 ].CompOutMode = Copy_u8COM_Mode ;
		}
#endif
		else Local_enuErrorState = ES_OUT_RANGE ;

		TIMSK = Local_u8CopyTIMSK ;															// Re-setting Timer Interrupt Mask Register to its Status

	}
	else
	{
		Local_enuErrorState = ES_OUT_RANGE ;
		#warning "PWM_enuSetCOM_Mode(): Non Supported Compare Output mode, No Action Taken ."
	}


	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t PWM_enuGetCOM_Mode( u8 Copy_u8TimerNum , u8 *Copy_pu8PWM_OC_Mode)
{
	ES_t Local_enuErrorState = ES_OUT_RANGE ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < PWM_u8MaxNum ; Local_u8Iter++ )
	{
		if( PWMs[Local_u8Iter].TimerNum == Copy_u8TimerNum )
		{
			*Copy_pu8PWM_OC_Mode = PWMs[Local_u8Iter].CompOutMode;
			Local_enuErrorState = ES_OK ;
		}
	}

	return Local_enuErrorState ;
}


ES_t PWM_enuSetWGM_Mode( u8 Copy_u8TimerNum , u8 Copy_u8WGM_Mode )
{
	ES_t Local_enuErrorState = ES_NOK;


	if( Copy_u8WGM_Mode > WGM_MODE_00	&& Copy_u8WGM_Mode <= WGM_FAST_OCR1A &&
		Copy_u8WGM_Mode != WGM_MODE_04	&& Copy_u8WGM_Mode != WGM_MODE_12 && Copy_u8WGM_Mode != WGM_MODE_13 )
	{

		u8 Local_u8CopyTIMSK = TIMSK ;									// Saving a Copy of Timer Interrupt Mask Register

#if PWM==0
		if( Copy_u8TimerNum == TIMER0 && ( Copy_u8WGM_Mode == WGM_PC_8_bit || Copy_u8WGM_Mode == WGM_FAST_8_bit ) )
		{
			TIMSK &= ~( TC0_INT_EN_MASK ) ;								// Disable Timer0 Interrupts
			TCCR0 &= ~( BIT0_MASK<< WGM01_BIT ) ;						// Masking WGM01 Select bit
			u8 Local_u8WGM;
			switch( Copy_u8WGM_Mode )
			{
				case WGM_PC_8_bit		:	Local_u8WGM = WGM_MODE_01;
											break;
				case WGM_FAST_8_bit		:	Local_u8WGM = WGM_MODE_03;
											break;
			}
			TCCR0 |= ( BIT0_MASK<< WGM00_BIT ) ;						// Setting WGM00 Select bit to 1
			TCCR0 |= ((Local_u8WGM >> BIT0_MASK ) << WGM01_BIT );		// Setting WGM01 Select bit to New Waveform Generation Mode
			Timers[ TIMER0 - TIMER0 ].WaveGenMode = Copy_u8WGM_Mode ;
		}
		else
#endif
		if( ( Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B ) &&
			Copy_u8WGM_Mode > WGM_MODE_00	&& Copy_u8WGM_Mode <= WGM_FAST_OCR1A &&
			Copy_u8WGM_Mode != WGM_MODE_04	&& Copy_u8WGM_Mode != WGM_MODE_12 && Copy_u8WGM_Mode != WGM_MODE_13 )
		{
			u8 Local_u8WGM = Copy_u8WGM_Mode - WGM_MODE_00 ;
			TIMSK &= ~( TC1_INT_EN_MASK ) ;													// Disable Timer1 Interrupts
			TCCR1B &= ~( WGM1B_SEL_BITS_MASK );												// Masking WGM1B Select bits
			TCCR1A |= ( ( Local_u8WGM & TWO_BITS_MASK ) << WGM1A_SEL_BITS );				// Setting WGM10 & WGM11 Select bits
			TCCR1B |= ( ( ( Local_u8WGM >> BIT1_MASK) & TWO_BITS_MASK ) << WGM1B_SEL_BITS );// Setting WGM12 & WGM13 Select bits
			PWMs[ TIMER1A - TIMER1A ].WaveGenMode = Copy_u8WGM_Mode ;
			PWMs[ TIMER1B - TIMER1A ].WaveGenMode = Copy_u8WGM_Mode ;
		}
#if PWM==0
		else if( Copy_u8TimerNum == TIMER2 && ( Copy_u8WGM_Mode == WGM_PC_8_bit || Copy_u8WGM_Mode == WGM_FAST_8_bit ) )
		{
			TIMSK &= ~( TC2_INT_EN_MASK ) ;								// Disable Timer2 Interrupts
			TCCR2 &= ~( BIT0_MASK<< WGM21_BIT ) ;						// Masking WGM21 Select bit
			u8 Local_u8WGM;
			switch( Copy_u8WGM_Mode )
			{
				case WGM_PC_8_bit		:	Local_u8WGM = WGM_MODE_01;
											break;
				case WGM_FAST_8_bit		:	Local_u8WGM = WGM_MODE_03;
											break;
			}
			TCCR0 |= ( BIT0_MASK<< WGM20_BIT ) ;						// Setting WGM20 Select bit to 1
			TCCR0 |= (( Local_u8WGM >> BIT0_MASK ) << WGM21_BIT );		// Setting WGM21 Select bit to NEW Waveform Generation Mode
			Timers[ TIMER2 - TIMER0 ].WaveGenMode = Copy_u8WGM_Mode ;
		}
#endif
		else Local_enuErrorState = ES_OUT_RANGE ;

		TIMSK = Local_u8CopyTIMSK ;															// Re-setting Timer Interrupt Mask Register to its Status
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t PWM_enuGetWGM_Mode( u8 Copy_u8TimerNum , u8 *Copy_pu8TimerWGM_Mode)
{
	ES_t Local_enuErrorState = ES_OUT_RANGE ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < PWM_u8MaxNum ; Local_u8Iter++ )
	{
		if( PWMs[Local_u8Iter].TimerNum == Copy_u8TimerNum )
		{
			*Copy_pu8TimerWGM_Mode = PWMs[Local_u8Iter].WaveGenMode;
			Local_enuErrorState = ES_OK ;
		}
	}
	return Local_enuErrorState ;
}
/*
ES_t Timer_enuReset( u8 Copy_u8TimerNum )
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8Temp = SREG ;									// Saving a Copy of AVR Status Register
	asm( "CLI" );												// Disable All Interrupts while writing to Counter Register

	if( Copy_u8TimerNum == TIMER0 )
	{
		TCNT0 = 0x00 ;
	}
	else if( Copy_u8TimerNum == TIMER1 || Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B )
	{
		TCNT1H = 0x00 ;
		TCNT1L = 0x00 ;
	}
	else if( Copy_u8TimerNum == TIMER2 )
	{
		TCNT2 = 0x00 ;
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	SREG = Local_u8Temp;										// Re-setting AVR Status Register to its Status

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}
*/
/*
ES_t Timer_enuReadCounterValue( u8 Copy_u8TimerNum , void *Copy_pCounterValue )
{
	ES_t Local_enuErrorState = ES_NOK;

	if( Copy_pCounterValue != NULL )
	{
		if( Copy_u8TimerNum == TIMER0 )
		{
			*( (u8 *)Copy_pCounterValue ) = TCNT0 ;
		}
		else if( Copy_u8TimerNum == TIMER1 || Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B )
		{
			u8 Local_u8Temp = SREG ;
			asm( "CLI" );
			*( (u16 *)Copy_pCounterValue )  =  TCNT1L ;
			*( (u16 *)Copy_pCounterValue ) |= ( (u16) TCNT1H << 8 ) ;
			SREG = Local_u8Temp;
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
*/
ES_t PWM_enuSetOCRnValue( u8 Copy_u8TimerNum , u16 Copy_u16OCRnValue )
{
	ES_t Local_enuErrorState = ES_NOK ;

#if PWM==0
	if( Copy_u8TimerNum == TIMER0 && Copy_u16OCRnValue <= Timer0Max )
	{
		OCR0 = Copy_u16OCRnValue ;
		Global_u8OCR0_Value = OCR0 ;
	}
	else
#endif
	if( Copy_u8TimerNum == TIMER1A && Copy_u16OCRnValue <= Timer1Max )
	{
		u8 Local_u8Temp = SREG ;
		asm( "CLI" );
		OCR1AH  = Copy_u16OCRnValue >> 8 ;
		OCR1AL 	= Copy_u16OCRnValue ;
		SREG = Local_u8Temp;
		Global_u16OCR1A_Value = Copy_u16OCRnValue ;
	}
	else if( Copy_u8TimerNum == TIMER1B && Copy_u16OCRnValue <= Timer1Max )
	{
		u8 Local_u8Temp = SREG ;
		asm( "CLI" );
		OCR1BH  = Copy_u16OCRnValue >> 8 ;
		OCR1BL 	= Copy_u16OCRnValue ;
		SREG = Local_u8Temp;
		Global_u16OCR1B_Value = Copy_u16OCRnValue ;
	}
#if PWM==0
	else if( Copy_u8TimerNum == TIMER2 && Copy_u16OCRnValue <= Timer2Max )
	{
		OCR2 = Copy_u16OCRnValue ;
		Global_u8OCR2_Value = OCR2 ;
	}
#endif
	else Local_enuErrorState = ES_OUT_RANGE ;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t PWM_enuReadOCRnValue( u8 Copy_u8TimerNum , void *Copy_pCounterValue )
{
	ES_t Local_enuErrorState = ES_NOK ;

#if PWM==0
		if( Copy_u8TimerNum == TIMER0 )
		{
			*( (u8 *)Copy_pCounterValue ) = Global_u8OCR0_Value ;
		}
		else
#endif
		if( Copy_u8TimerNum == TIMER1A )
		{
			*( (u16 *)Copy_pCounterValue ) = Global_u16OCR1A_Value ;
		}
		else if( Copy_u8TimerNum == TIMER1B )
		{
			*( (u16 *)Copy_pCounterValue ) = Global_u16OCR1B_Value ;
		}
#if PWM==0
		else if( Copy_u8TimerNum == TIMER2 )
		{
			*( (u8 *)Copy_pCounterValue ) = Global_u8OCR2_Value ;
		}
#endif
		else Local_enuErrorState = ES_OUT_RANGE ;

		return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t PWM_enuSetICR1Value( u16 Copy_u16ICR1Value )
{
	u8 Local_u8Temp = SREG ;
	asm( "CLI" );
	ICR1H = Copy_u16ICR1Value >> 8 ;
	ICR1L = Copy_u16ICR1Value ;
	SREG = Local_u8Temp;
	Global_u16ICR1_Value = Copy_u16ICR1Value ;
	return ES_OK;
}

ES_t PWM_enuSetInterruptMode( u8 Copy_u8TimerNum , u8 Copy_u8TimerInterruptMode )/////////////************************///////////////////////
{
	return ES_OK;
}

ES_t PWM_enuGetInterruptMode( u8 Copy_u8TimerNum , u8 *Copy_pu8TimerInterruptMode )////////////////*****************************//////////////////////
{
	return ES_OK;
}

ES_t PWM_enuSetDutyCycle( u8 Copy_u8TimerNum , f32 Copy_f32DutyCycle )//////////***********************************/////////////////////
{
	return ES_OK;
}




#if	PWM == 0


ES_t Timer_enuReadICR1Value( u16 *Copy_pu16ICR1Value )
{
	*Copy_pu16ICR1Value = Global_u16ICR1_Value ;
	return ES_OK;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void Timer_vidTOIE0(void *Copy_pTOV0Counter)
{
	( *( (u16 *)Copy_pTOV0Counter) )++;
}

void Timer_vidOCIE0(void *Copy_pOC0Counter )
{
	( *( (u16 *)Copy_pOC0Counter) )++;
}*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

ES_t PWM_enuInterruptEnable( u8 Copy_u8TimerIntName)
{
	ES_t Local_enuErrorState = ES_NOK;

	asm(" CLI ") ;														// Disable ALL Interrupts

	switch ( Copy_u8TimerIntName )
	{

#if PWM==0
		case TOIE0	:	TIMSK |= (BIT0_MASK << TOIE0_BIT);
						break;
		case OCIE0	:	TIMSK |= (BIT0_MASK << OCIE0_BIT);
						break;
		case TOIE2	:	TIMSK |= (BIT0_MASK << TOIE2_BIT);
						break;
		case OCIE2	:	TIMSK |= (BIT0_MASK << OCIE2_BIT);
						break;
#endif
		case TOIE1	:	TIMSK |= (BIT0_MASK << TOIE1_BIT);
						break;
		case OCIE1B	:	TIMSK |= (BIT0_MASK << OCIE1B_BIT);
						break;
		case OCIE1A	:	TIMSK |= (BIT0_MASK << OCIE1A_BIT);
						break;
		case TICIE1	:	TIMSK |= (BIT0_MASK << TICIE1_BIT);
						break;
		default		:	Local_enuErrorState = ES_OUT_RANGE;
						#warning " Timer_enuInterruptEnable() : Unidentified Interrupt Name, No Action Taken."
	}
	asm(" SEI ") ;														// Enable ALL Interrupts

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t PWM_enuInterruptDisable( u8 Copy_u8TimerIntName)
{
	ES_t Local_enuErrorState = ES_NOK;

	asm(" CLI ") ;														// Disable ALL Interrupts

	switch ( Copy_u8TimerIntName )
	{

#if PWM==0
		case TOIE0	:	TIMSK &= ~(BIT0_MASK << TOIE0_BIT);
						break;
		case OCIE0	:	TIMSK &= ~(BIT0_MASK << OCIE0_BIT);
						break;
		case TOIE2	:	TIMSK &= ~(BIT0_MASK << TOIE2_BIT);
						break;
		case OCIE2	:	TIMSK &= ~(BIT0_MASK << OCIE2_BIT);
						break;
#endif
		case TOIE1	:	TIMSK &= ~(BIT0_MASK << TOIE1_BIT);
						break;
		case OCIE1B	:	TIMSK &= ~(BIT0_MASK << OCIE1B_BIT);
						break;
		case OCIE1A	:	TIMSK &= ~(BIT0_MASK << OCIE1A_BIT);
						break;
		case TICIE1	:	TIMSK &= ~(BIT0_MASK << TICIE1_BIT);
						break;
		default		:	Local_enuErrorState = ES_OUT_RANGE;
						#warning " Timer_enuInterruptEnable() : Unidentified Interrupt Name, No Action Taken."
	}

	asm(" SEI ") ;														// Re-Enable ALL Interrupts

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

bool PWM_IsInterruptEnabled( u8 Copy_u8TimerIntName )
{
	u8 Local_u8Status = 0 ;

	switch ( Copy_u8TimerIntName )
	{
#if PWM==0

		case TOIE0	:	Local_u8Status |= ( (TIMSK >> TOIE0_BIT) & BIT0_MASK );
						break;
		case OCIE0	:	Local_u8Status |= ( (TIMSK >> OCIE0_BIT) & BIT0_MASK );
						break;
		case TOIE2	:	Local_u8Status |= ( (TIMSK >> TOIE2_BIT) & BIT0_MASK );
						break;
		case OCIE2	:	Local_u8Status |= ( (TIMSK >> OCIE2_BIT) & BIT0_MASK );
						break;

#endif
		case TOIE1	:	Local_u8Status |= ( (TIMSK >> TOIE1_BIT) & BIT0_MASK );
						break;
		case OCIE1B	:	Local_u8Status |= ( (TIMSK >> OCIE1B_BIT) & BIT0_MASK );
						break;
		case OCIE1A	:	Local_u8Status |= ( (TIMSK >> OCIE1A_BIT) & BIT0_MASK );
						break;
		case TICIE1	:	Local_u8Status |= ( (TIMSK >> TICIE1_BIT) & BIT0_MASK );
						break;
	}
	return ( ( Local_u8Status )? TRUE : FALSE ) ;
}


ES_t PWM_enuCallBack( u8 Copy_u8TimerIntName , void (*Copy_pAppFun)(void*) , void *Copy_pAppVar)
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 found = 0 ;

	if( Copy_pAppFun != NULL )
	{
		for( u8 Local_u8Iter; Local_u8Iter < PWM_INTERRUPTS ; Local_u8Iter++ )
		{
			if( Global_AstrPWM_Pointers[Local_u8Iter].InterruptName == Copy_u8TimerIntName )
			{
				Global_AstrPWM_Pointers[Local_u8Iter].ptrFun = Copy_pAppFun;
				Global_AstrPWM_Pointers[Local_u8Iter].ptrVar = Copy_pAppVar;
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


void __vector_6( void )__attribute__((signal));		/*	Timer1	Capture Event Interrupt	ISR		*/
void __vector_7( void )__attribute__((signal));		/*	Timer1	Compare Match A Interrupt ISR	*/
void __vector_8( void )__attribute__((signal));		/*	Timer1	Compare Match B Interrupt ISR	*/
void __vector_9( void )__attribute__((signal));		/*	Timer1	Overflow Interrupt	ISR			*/

#if PWM==0
void __vector_4( void )__attribute__((signal));		/*	Timer2	Compare Match Interrupt	ISR		*/
void __vector_5( void )__attribute__((signal));		/*	Timer2	Overflow Interrupt	ISR			*/
void __vector_10( void )__attribute__((signal));	/*	Timer0	Compare Match Interrupt	ISR		*/
void __vector_11( void )__attribute__((signal));	/*	Timer0	Overflow Interrupt	ISR			*/

void __vector_4( void )/*	OCIE2	*/
{
	if( Global_AstrPWM_Pointers[7].ptrFun != NULL )
	{
		(*Global_AstrPWM_Pointers[7].ptrFun)( Global_AstrPWM_Pointers[7].ptrVar );
	}
}

void __vector_5( void )/*	TOIE2	*/
{
	if( Global_AstrPWM_Pointers[6].ptrFun != NULL )
	{
		(*Global_AstrPWM_Pointers[6].ptrFun)( Global_AstrPWM_Pointers[6].ptrVar );
	}
}
#endif


void __vector_6( void )/*	TICIE1	*/
{
	if( Global_AstrPWM_Pointers[3].ptrFun != NULL )
	{
		(*Global_AstrPWM_Pointers[3].ptrFun)( Global_AstrPWM_Pointers[3].ptrVar );
	}
}
void __vector_7( void )/*	OCIE1A	*/
{
	if( Global_AstrPWM_Pointers[2].ptrFun != NULL )
	{
		(*Global_AstrPWM_Pointers[2].ptrFun)( Global_AstrPWM_Pointers[2].ptrVar );
	}
}
void __vector_8( void )/*	OCIE1B	*/
{
	if( Global_AstrPWM_Pointers[1].ptrFun != NULL )
	{
		(*Global_AstrPWM_Pointers[1].ptrFun)( Global_AstrPWM_Pointers[1].ptrVar );
	}
}
void __vector_9( void )/*	TOIE1	*/
{
	if( Global_AstrPWM_Pointers[0].ptrFun != NULL )
	{
		(*Global_AstrPWM_Pointers[0].ptrFun)( Global_AstrPWM_Pointers[0].ptrVar );
	}
}

#if PWM==0
void __vector_10( void )/*	OCIE0	*/
{
	if( Global_AstrPWM_Pointers[1].ptrFun != NULL )
	{
		(*Global_AstrPWM_Pointers[1].ptrFun)( Global_AstrPWM_Pointers[1].ptrVar );
	}
}

void __vector_11( void )/*	TOIE0	*/
{
	if( Global_AstrPWM_Pointers[0].ptrFun != NULL )
	{
		(*Global_AstrPWM_Pointers[0].ptrFun)( Global_AstrPWM_Pointers[0].ptrVar );
	}
}

#endif

