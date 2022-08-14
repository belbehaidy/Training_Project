/*
 * bassem.c

 */

/*
#include <util/delay.h>

#include "stdTypes.h"
#include "errorState.h"

#include "DIO_int.h"

#include "SevSeg_int.h"
#include "Sw_Zero_int.h"
#include "Sw_One_int.h"
#include "Sw_Two_int.h"

#define TOTAL_MODULES 	2
#define ACTIVE 			1
#define IDLE			0
#define FRAME_NUM		10

u8 SevSeg_u8DisplayValue=0 ;
u8 SevSeg_u8DelayCounter = FRAME_NUM;

ES_t DisplayFrame(u8 Copy_u8DisFrmValue, u8 Copy_u8ModuleDelay);

int
main(void)			//User Input Counter on the Seven-Segments from 0 - 99 / Button 2 to increment / Button 0 to decrement /Button 1 has 3 States 1-IDLE 2-Auto Increment & 3-Auto Decrement
{
	u8 Local_u8SevSegModuleDelay ;
	u8 Local_u8Sw_ZeroPressed=IDLE , Local_u8Sw_ZeroHold=IDLE , Local_u8Sw_ZeroValue;
	u8 Local_u8Sw_TwoPressed=IDLE , Local_u8Sw_TwoHold=IDLE , Local_u8Sw_TwoValue;
	u8 Local_u8Sw_OnePressed=IDLE , Local_u8Sw_OneHold=IDLE , Local_u8Sw_OneUnPressed=IDLE , Local_u8Sw_OneToggleEn=IDLE, Local_u8Sw_OneValue  ;
/*
	if( SevSeg_enuInit() == ES_OK &&  Sw_Two_enuInit() == ES_OK && Sw_One_enuInit() == ES_OK /*&& Sw_Zero_enuInit() == ES_OK*///)
/*	{
		if(  SevSeg_enuFrameModuleDelay(TOTAL_MODULES , &Local_u8SevSegModuleDelay ) == ES_OK )
		{
			for(;;)
			{
				if( DisplayFrame(SevSeg_u8DisplayValue,Local_u8SevSegModuleDelay) == ES_OK)
				{
					if( Sw_Two_enuGetPressed( &Local_u8Sw_TwoValue) == ES_OK)
					{
						if( Local_u8Sw_TwoValue == DIO_u8HIGH && Local_u8Sw_TwoPressed==IDLE && Local_u8Sw_TwoHold==IDLE  )
						{
							Local_u8Sw_TwoPressed = ACTIVE;
							SevSeg_u8DisplayValue++;
							if( SevSeg_u8DisplayValue == 100 )
								SevSeg_u8DisplayValue = 0;
							Local_u8Sw_OneToggleEn=IDLE ; //Disable Toggling with Switch 1

						}
						else if( Local_u8Sw_TwoValue == DIO_u8HIGH && Local_u8Sw_TwoPressed== ACTIVE && Local_u8Sw_TwoHold==IDLE )
						{
							Local_u8Sw_TwoHold = ACTIVE;
							//Local_u8Sw_TwoPressed =IDLE;
						}
						else if( Local_u8Sw_TwoValue == DIO_u8HIGH && Local_u8Sw_TwoPressed==ACTIVE && Local_u8Sw_TwoHold==ACTIVE )
						{	/*Change condition Local_u8Sw_TwoValue == DIO_u8LOW , to enforce Single Press Effect*/
/*							Local_u8Sw_TwoPressed = IDLE;
							Local_u8Sw_TwoHold = IDLE;
						}
						//Local_u8SwTwoDone = ES_OK;
					}
					if(Sw_Zero_enuGetPressed( &Local_u8Sw_ZeroValue) == ES_OK)
					{
						if( Local_u8Sw_ZeroValue == DIO_u8HIGH && Local_u8Sw_ZeroPressed==IDLE && Local_u8Sw_ZeroHold==IDLE )
						{
							Local_u8Sw_ZeroPressed = ACTIVE;
							if( SevSeg_u8DisplayValue == 0 )
								SevSeg_u8DisplayValue = 100;
							SevSeg_u8DisplayValue--;
							Local_u8Sw_OneToggleEn=IDLE ; //Disable Toggling with Switch 1

						}
						else if( Local_u8Sw_ZeroValue == DIO_u8HIGH && Local_u8Sw_ZeroPressed==ACTIVE && Local_u8Sw_ZeroHold==IDLE )
						{
							Local_u8Sw_ZeroHold = ACTIVE;
							//Local_u8Sw_ZeroPressed = IDLE;
						}
						else if( Local_u8Sw_ZeroValue == DIO_u8HIGH && Local_u8Sw_ZeroPressed==ACTIVE && Local_u8Sw_ZeroHold==ACTIVE )
						{*/	/*Change condition Local_u8Sw_ZeroValue == IDLE , to enforce Single Press Effect*/
/*							Local_u8Sw_ZeroPressed = IDLE;
							Local_u8Sw_ZeroHold = IDLE;
						}
						//Local_u8SwZeroDone = ES_OK;
					}
					if( Sw_One_enuGetPressed( &Local_u8Sw_OneValue) == ES_OK)
					{
						if( Local_u8Sw_OneValue == DIO_u8HIGH && Local_u8Sw_OnePressed==IDLE && Local_u8Sw_OneHold==IDLE && Local_u8Sw_OneUnPressed==IDLE && Local_u8Sw_OneToggleEn==IDLE)
						{
							Local_u8Sw_OnePressed =ACTIVE;
						}
						else if( Local_u8Sw_OneValue == DIO_u8HIGH && Local_u8Sw_OnePressed==ACTIVE && Local_u8Sw_OneHold==IDLE && Local_u8Sw_OneUnPressed==IDLE && Local_u8Sw_OneToggleEn==IDLE)
						{
							Local_u8Sw_OneHold = ACTIVE;
						}
						else if( Local_u8Sw_OneValue == DIO_u8LOW && Local_u8Sw_OnePressed==ACTIVE && Local_u8Sw_OneHold==ACTIVE && Local_u8Sw_OneUnPressed==IDLE && Local_u8Sw_OneToggleEn==IDLE)
						{

							Local_u8Sw_OneUnPressed = ACTIVE ;
							Local_u8Sw_OneToggleEn = ACTIVE ;
							Local_u8Sw_OnePressed = IDLE ;
							Local_u8Sw_OneHold = IDLE ;
						}
						else if( Local_u8Sw_OneValue == DIO_u8LOW && Local_u8Sw_OnePressed==0 && Local_u8Sw_OneHold==IDLE && Local_u8Sw_OneUnPressed==ACTIVE && Local_u8Sw_OneToggleEn==ACTIVE)
						{

							SevSeg_u8DelayCounter--;
							if(SevSeg_u8DelayCounter==0)
							{
								SevSeg_u8DelayCounter = FRAME_NUM;
								SevSeg_u8DisplayValue++;
								if( SevSeg_u8DisplayValue == 100 )
										SevSeg_u8DisplayValue = 0;
							}

						}
						else if( Local_u8Sw_OneValue == DIO_u8HIGH && Local_u8Sw_OnePressed==IDLE && Local_u8Sw_OneHold==IDLE && Local_u8Sw_OneUnPressed==ACTIVE && Local_u8Sw_OneToggleEn==ACTIVE)
						{
							Local_u8Sw_OnePressed = ACTIVE;
						}
						else if( Local_u8Sw_OneValue == DIO_u8HIGH && Local_u8Sw_OnePressed==ACTIVE && Local_u8Sw_OneHold==IDLE && Local_u8Sw_OneUnPressed==ACTIVE && Local_u8Sw_OneToggleEn==ACTIVE)
						{
							Local_u8Sw_OneHold = ACTIVE;
						}
						else if( Local_u8Sw_OneValue == DIO_u8LOW && Local_u8Sw_OnePressed==ACTIVE && Local_u8Sw_OneHold==ACTIVE && Local_u8Sw_OneUnPressed==ACTIVE && Local_u8Sw_OneToggleEn==ACTIVE)
						{
							Local_u8Sw_OneToggleEn = IDLE;
							Local_u8Sw_OnePressed = IDLE;
							Local_u8Sw_OneHold = IDLE;
						}
						else if( Local_u8Sw_OneValue == DIO_u8HIGH && Local_u8Sw_OnePressed==IDLE && Local_u8Sw_OneHold==IDLE && Local_u8Sw_OneUnPressed==ACTIVE && Local_u8Sw_OneToggleEn==IDLE)
						{
							Local_u8Sw_OnePressed = ACTIVE;
						}
						else if( Local_u8Sw_OneValue == DIO_u8HIGH && Local_u8Sw_OnePressed==ACTIVE && Local_u8Sw_OneHold==IDLE && Local_u8Sw_OneUnPressed==ACTIVE && Local_u8Sw_OneToggleEn==IDLE)
						{
							Local_u8Sw_OneHold = ACTIVE;
						}
						else if( Local_u8Sw_OneValue == DIO_u8LOW && Local_u8Sw_OnePressed==ACTIVE && Local_u8Sw_OneHold==ACTIVE && Local_u8Sw_OneUnPressed==ACTIVE &&  Local_u8Sw_OneToggleEn==IDLE)
						{
							Local_u8Sw_OneToggleEn = ACTIVE;
							Local_u8Sw_OneUnPressed = IDLE;
						}
						else if( Local_u8Sw_OneValue == DIO_u8LOW && Local_u8Sw_OnePressed==ACTIVE && Local_u8Sw_OneHold==ACTIVE && Local_u8Sw_OneUnPressed==IDLE && Local_u8Sw_OneToggleEn==ACTIVE)
						{
							Local_u8Sw_OneHold = IDLE;
							Local_u8Sw_OnePressed = IDLE;
						}
						else if( Local_u8Sw_OneValue == DIO_u8LOW && Local_u8Sw_OnePressed==IDLE && Local_u8Sw_OneHold==IDLE && Local_u8Sw_OneUnPressed==IDLE && Local_u8Sw_OneToggleEn==ACTIVE)
						{
							SevSeg_u8DelayCounter--;
							if(SevSeg_u8DelayCounter==0)
							{
								SevSeg_u8DelayCounter = FRAME_NUM;
								if( SevSeg_u8DisplayValue == 0 )
									SevSeg_u8DisplayValue = 100;
								SevSeg_u8DisplayValue--;
							}
						}
						else if( Local_u8Sw_OneValue == DIO_u8HIGH && Local_u8Sw_OnePressed==IDLE && Local_u8Sw_OneHold==IDLE && Local_u8Sw_OneUnPressed==IDLE && Local_u8Sw_OneToggleEn==ACTIVE)
						{
							Local_u8Sw_OneToggleEn = IDLE;
						}
						//Local_u8SwOneDone = ES_OK;
					}

				}

			}
		}
	}
}

ES_t DisplayFrame(u8 Copy_u8DisFrmValue , u8 Copy_u8ModuleDelay)
{
	ES_t Local_enuErrorState = ES_NOK , Local_u8Done = ES_NOK;

	if( SevSeg_enuSetDigitValue( (Copy_u8DisFrmValue % SevSeg_u8DIGIT_BASE) ) == ES_OK )
	{
		if( SevSeg_enuModuleEnable(SevSeg_u8MODULE_1) == ES_OK )
		{
			_delay_ms ( Copy_u8ModuleDelay );
			Local_u8Done = SevSeg_enuModuleDisable(SevSeg_u8MODULE_1);

		}
	}
	if( Local_u8Done == ES_OK && SevSeg_enuSetDigitValue( (Copy_u8DisFrmValue / SevSeg_u8DIGIT_BASE) ) == ES_OK )
	{
		if( SevSeg_enuModuleEnable(SevSeg_u8MODULE_2) == ES_OK)
		{
			_delay_ms ( Copy_u8ModuleDelay );
			Local_enuErrorState = SevSeg_enuModuleDisable(SevSeg_u8MODULE_2);
		}
	}

	return Local_enuErrorState ;
}
*/
/*
int
main(void)				//Infinite Counter on the Seven-Segments from 0 - 99
{
	u8 Local_u8SevSegModuleDelay;

	if( SevSeg_enuInit() == ES_OK )
	{
		if(  SevSeg_enuFrameModuleDelay(TOTAL_MODULES , &Local_u8SevSegModuleDelay ) == ES_OK )
		{
			for(;;)
			{
				for(u8 Local_u8DigitCounter=0 ; Local_u8DigitCounter < 100 ; Local_u8DigitCounter++)
				{
					for(u8 Local_u8InnerCounter=0 ; Local_u8InnerCounter < 20 ; Local_u8InnerCounter++)
					{
						if( SevSeg_enuSetDigitValue( (Local_u8DigitCounter % SevSeg_u8DIGIT_BASE) ) == ES_OK )
						{
							if( SevSeg_enuModuleEnable(SevSeg_u8MODULE_1) == ES_OK )
							{
								_delay_ms ( Local_u8SevSegModuleDelay );
								SevSeg_enuModuleDisable(SevSeg_u8MODULE_1);
							}
						}
						if( SevSeg_enuSetDigitValue( (Local_u8DigitCounter / SevSeg_u8DIGIT_BASE) ) == ES_OK )
						{
							if( SevSeg_enuModuleEnable(SevSeg_u8MODULE_2) == ES_OK)
							{
								_delay_ms ( Local_u8SevSegModuleDelay );
								SevSeg_enuModuleDisable(SevSeg_u8MODULE_2);
							}
						}
					}
				}
			}
		}
	}
}

*/
/*
#define PORTA 		*((volatile u8*)0x3B)
#define DDRA 		*((volatile u8*)0x3A)
#define PINA 		*((volatile u8*)0x39)

#define PORTB 		*((volatile u8*)0x38)
#define DDRB 		*((volatile u8*)0x37)
#define PINB 		*((volatile u8*)0x36)

#define PORTC 		*((volatile u8*)0x35)
#define DDRC 		*((volatile u8*)0x34)
#define PINC 		*((volatile u8*)0x33)

#define PORTD 		*((volatile u8*)0x32)
#define DDRD 		*((volatile u8*)0x31)
#define PIND 		*((volatile u8*)0x30)

//u8 cat_seg []= {	0x3f,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
//					0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71	   };

int
main(void)
{
	//port A 4-7 //high
	DDRA |= 0xF0;
	PORTA |= 0xF0;

	//PORRT B Control 1&2
	DDRB |=0x06;
	PORTB &=~(0x06);


	for(;;)
	{

		for(u8 i=0;i<100;i++)
		{

			for(u8 j=0;j<15;j++)
			{
				PORTA &=0x0F;//masking with zero
				PORTA = ((i%10)<<4);
				PORTB |= (1<<1);
				_delay_ms(15);
				PORTB &= ~(1<<1);

				PORTA &= 0x0F;//masking with zero
				PORTA = ((i/10)<<4);
				PORTB |= (1<<2);
				_delay_ms(15);
				PORTB &= ~(1<<2);

			}
		}
	}
}

/*
int
main(void)
{
	//First 7-segment Com-Anode
	//	PORTA 0-6
	DDRA = 0x7F;
	PORTA = 0x7F;

	// Second 7-segment Com-Cathode
	//B3 -> a
	//B4 -> b
	//B5 -> c
	//B6 -> d
	//B7 -> e
	//D1 -> f
	//D0 -> g

	DDRB = 0xF8;
	PORTB &= 0x07;
	DDRD = 0x03;
	PORTD &= 0xFC;

	for(;;)
	{
		for(u8 i=0;i<16;i++)
		{
			PORTA |= 0x7F;
			PORTA &= (~cat_seg[i]);

			for(u8 j=0;j<16;j++)
			{
				PORTB &= 0x07;
				PORTB |= ( ( cat_seg[j] & (0x1F) ) << 3 );
				PORTD &= 0xFC;
				PORTD |= (((cat_seg[j]>>5)&1)<<1);
				PORTD |= (((cat_seg[j]>>6)&1)<<0);
				_delay_ms(500);
			}
		}
	}



}



/*
int
main(void)
{
	// output // high
	DDRA = 0x7F;
	PORTA = 0x7F ;
	// output // low
	DDRB = 0x7F;
	PORTB = 0x00;

	for(;;)
	{
		for(u8 i=0;i<16;i++)
		{
			PORTA = 0x7F;
			PORTA = ~(cat_seg[i]);
			for(u8 j=0;j<16;j++)
			{
				PORTB = 0x00;
				PORTB = cat_seg[j];
				_delay_ms(50);
			}
		}
	}
}

int
main(void)
{
	u8 press=0 , hold=0 ,unpress=0;

	//input // float
	DDRD &= ~(1<<2);
	PORTD &= ~(1<<2);

	// output // low
	DDRD |= (1<<3);
	PORTD &=~ (1<<3);

	while (1)
	{

		_delay_ms(100);
		if (  ((PIND>>2)&1) && press == 0 && unpress == 0  && hold == 0 )
		{
			PORTD |= (1<<3);
			press = 1 ;
		}
		else if (  ((PIND>>2)&1) && press == 1  && hold == 0  && unpress ==0 )
		{
			hold = 1;
		}
		else if(  ((PIND>>2)&1) == 0 && press == 1  && hold == 1  && unpress ==0 )
		{
			unpress = 1;
			hold = 0 ;
		}
		else if (  ((PIND>>2)&1) && press == 1  && hold == 0  && unpress == 1 )
		{
			PORTD &=~ (1<<3);
			press = 0 ;
		}
		else if (  ((PIND>>2)&1) && press == 0  && hold == 0  && unpress == 1 )
		{
			hold = 1 ;
		}
		else if (  ((PIND>>2)&1)==0 && press == 0  && hold == 1  && unpress == 1 )
		{
			hold =0;
			press =0;
			unpress = 0;
		}

	}


}

/*
int
main(void)
{
	//input // float
	DDRD &= ~(1<<2);
	PORTD &= ~(1<<2);

	// output // low
	DDRC |= (1<<2);
	PORTC &=~ (1<<2);

	while (1)
	{
		if (  ((PIND>>2)&1) )
		{
			PORTC |= (1<<2);
		}
		else
		{
			PORTC &=~ (1<<2);
		}
	}

}

*/
/*

int
main(void)
{
	u8 i=0;
	DDRA = 0b11111111 ; //0xff // 255 // 0c377
	PORTA = 0;
	while (1)
	{
		for (i= 0; i<=7 ; i++)
		{
			PORTA |= (1<< i);
			_delay_ms(250);
		}

		for (i= 0; i<=7 ; i++)
		{
			PORTA &=~(1<< i);
			_delay_ms(250);
		}
	}
}

*/
