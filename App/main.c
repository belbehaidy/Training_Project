/*
 * main.c
 *
 *  Created on: Aug 5, 2022
 *      Author: Bassem El-behaidy
 */


int
main(void)
{

}

/*
#include "../Libraries/stdTypes.h"
#include "../Libraries/errorState.h"

#include "../MCAL/DIO/DIO_int.h"
//#include "../HAL/LCD/LCD_int.h"

//#include "../HAL/Keypad/Keypad_int.h"
//#include "../HAL/Switch/Switch_int.h"

#include "../MCAL/TIMER/TIMER_int.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/GIE/GIE_int.h"
//#include "../MCAL/ADC/ADC_int.h"


void TOIE0_ISR(void*);
void EXT_INT0_ISR(void);

u8 OnCounts = 0 , CycleCounts = 0 , OVF_counts=0 , CycleOVFs = 0 ,OnOVFs = 0 , Duty , state = 0 ;
u16 freq;
u32 Counts = 0 ;

void *pNULL = NULL;

int
main(void)
{



	DIO_enuInit();
	EXTI_enuInit();
	Timer_enuInit();

	Timer_enuCallBack( TOIE0 , TOIE0_ISR , pNULL);
	Timer_enuInterruptEnable( TOIE0 );

	EXTI_enuCallBack( INT0 , EXT_INT0_ISR );
	EXTI_enuSetSenseLevel( INT0 , RISING_EDGE);
	EXTI_enuEnableInterrupt( INT0 );

	GIE_enuEnable();


	while( 1 )
	{
		if( state == 1 )
		{
			Timer_enuReset( TIMER0 );
			OVF_counts = 0 ;
			EXTI_enuSetSenseLevel( INT0 , FALLING_EDGE);
		}
		else if( state == 2)
		{
			Timer_enuReadCounterValue( TIMER0 , &OnCounts) ;
			OnOVFs = OVF_counts ;
			EXTI_enuSetSenseLevel( INT0 , RISING_EDGE);

		}
		else if( state ==3)
		{
			Timer_enuReadCounterValue( TIMER0 , &CycleCounts) ;
			CycleOVFs = OVF_counts ;
			Timer_enuReset( TIMER0 ) ;
			EXTI_enuDisableInterrupt( INT0 );
			Timer_enuInterruptDisable( TOIE0 );
			u32 Local_u32Timer0_Clk ;
			Duty = ( OnCounts += (OnOVFs * 256) )/ ( CycleCounts += (CycleOVFs * 256) );
			if ( Timer_enuGetClock( TIMER0 , &Local_u32Timer0_Clk ) == ES_OK )
			freq = 1/( Local_u32Timer0_Clk * CycleCounts);
		}
	}

}

void EXT_INT0_ISR(void)
{
	state++ ;
}

void TOIE0_ISR(void* Copy_pvidVar)
{
	OVF_counts++;
}




/*
#include <util/delay.h>


#define TCCR0 		*((volatile u8*)0x53)
#define TCNT0 		*((volatile u8*)0x52)
#define OCR0 		*((volatile u8*)0x5C)
#define TIMSK 		*((volatile u8*)0x59)

u32 ovf , count;
u8 preload;

int
main(void)
{

	DIO_enuSetPinDirection(DIO_u8GROUP_C , DIO_u8PIN2 , DIO_u8OUTPUT);
	DIO_enuSetPinDirection(DIO_u8GROUP_C , DIO_u8PIN4 , DIO_u8OUTPUT);
	DIO_enuSetPinDirection(DIO_u8GROUP_C , DIO_u8PIN6 , DIO_u8OUTPUT);

	//ovf , 1024 pres , disconnected
	TCCR0 = 0x4D;
	OCR0 = 255;
	u32 counts = (500llu * F_CPU)/(1024 * 1000UL);
	ovf = (counts+255)/256;
	//ovf = (counts+509)/510;
	count = ovf;
	preload = 256 - (counts % 256);
	//preload = 255 - (counts % 255);
	TCNT0 =preload;
	TIMSK |= (1<<0);
	TIMSK |= (1<<1);
	GIE_enuEnable();
	while (1);
}


void __vector_10(void)__attribute__((signal));
void __vector_10(void)
{
	DIO_enuTogglePinValue(DIO_u8GROUP_C , DIO_u8PIN6);
	DIO_enuTogglePinValue(DIO_u8GROUP_C , DIO_u8PIN6);
}

void __vector_11(void)__attribute__((signal));
void __vector_11(void)
{
	DIO_enuTogglePinValue(DIO_u8GROUP_C , DIO_u8PIN4);
	count --;
	DIO_enuTogglePinValue(DIO_u8GROUP_C , DIO_u8PIN4);
	if ( ! count)
	{
		TCNT0 =preload;

		DIO_enuTogglePinValue(DIO_u8GROUP_C , DIO_u8PIN2);

		count = ovf;
	}
}


*/







/*
#include <util/delay.h>
#include "../Libraries/stdTypes.h"
#include "../Libraries/errorState.h"

#include "../MCAL/DIO/DIO_int.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/ADC/ADC_int.h"

#include "../HAL/Keypad/Keypad_int.h"
#include "../HAL/LCD/LCD_int.h"
#include "../HAL/LD/LD_int.h"
#include "../HAL/Switch/Switch_int.h"

#include "App_priv.h"

u8 Global_u8Power = POWER_OFF ,Global_u8Restart = CONTINUE ,Global_u8Counter = NO_COUNT;

ES_t SwitchIsPressed(u8 Copy_u8SwitchNum , u8 Copy_u8PressedValue , u8 *Copy_pu8SwitchStatus);
ES_t DetectPowerStatus(void);
ES_t CheckCounterStatus(void);
ES_t ReadInputDigit(u8 *Copy_u8Digit);

int
main(void)
{
	ES_t  Local_enuErrorState ;
	u8 Local_u8Key = KEYPAD_NOT_PRESSED ;
	s32 Local_s32Num = 0 ;


	LCD_enuInit();
	Keypad_enuInit();
	LD_enuInit();
	Switch_enuInit();
	ADC_enuInit();
	EXTI_enuInit();




	LCD_enuWriteCommand(0x08); //Turn Display OFF

	do
	{
		Local_enuErrorState = DetectPowerStatus();
	}while( (Global_u8Power == POWER_OFF) && Local_enuErrorState ==ES_OK);

	if(Global_u8Power == POWER_ON )
	{
		for(;;)
		{
			if(Global_u8Power == SHUTDOWN)
			{
				do
				{
					Local_enuErrorState = DetectPowerStatus();
					_delay_ms(100);
				}while( (Global_u8Power == SHUTDOWN) && Local_enuErrorState ==ES_OK);
				Keypad_Awake();
			}
			if(Global_u8Power == POWER_ON)
			{
				DISPLAY_WELCOME_MESSAGE;
				_delay_ms(5000);
				DISPLAY_NUMBER_MESSAGE;

				while(  (Local_enuErrorState = ReadInputDigit(&Local_u8Key) ) == ES_OK  )
				{
					if( Local_u8Key != 'C' )
					{
						LCD_enuWriteData(Local_u8Key);
						Local_s32Num = (Local_s32Num*10) + (Local_u8Key - '0');
						Local_u8Key = KEYPAD_NOT_PRESSED;
					}
					else break;
				}
				if( Global_u8Restart == RESTART)
				{
					Global_u8Restart = CONTINUE;
					continue;
				}
				if(Local_enuErrorState == ES_OK)
				{
					DISPLAY_COUNTER_MESSAGE;

					for(;;)
					{
						if( Global_u8Counter != NO_COUNT)
						{
							LCD_enuGoToPosition(2,5);
							LCD_enuWriteIntegerNum(Local_s32Num);
						}
						_delay_ms(980);

						if( (Local_enuErrorState = DetectPowerStatus() ) ==ES_OK )
						{
							if( Global_u8Power == POWER_OFF)	break;
						}
						else break;
						if( (Local_enuErrorState = CheckCounterStatus() ) ==ES_OK )
						{
							switch(Global_u8Counter)
							{
							case COUNT_UP	: Local_s32Num++;
												break;
							case COUNT_DOWN	: Local_s32Num--;
												break;
							case NO_COUNT	: 	break;
							}
						}
						else break;

						////////////////////////Continue from HERE






					}
					if( Global_u8Power == POWER_OFF) 	continue;



				}
				else break;
			}
			else
			{
				LCD_enuWriteCommand(0x01);
				if( Local_enuErrorState == ES_OK && Global_u8Power == POWER_OFF )
				{
					Keypad_Sleep();
					LCD_enuGoToPosition(2,3);
					LCD_enuWriteString("Bye..Bye..!");
					_delay_ms(3000);
					LCD_enuWriteCommand(0x01);
					LCD_enuWriteCommand(0x08);
					Global_u8Power = SHUTDOWN;
				}
				else
				{
					LCD_enuGoToPosition(1,3);
					LCD_enuWriteString("***ERROR***");
					LCD_enuGoToPosition(2,6);
					LCD_enuWriteString("Shutdown?");
					do
					{
						Local_enuErrorState = DetectPowerStatus();
					}while( (Global_u8Power != POWER_OFF) && Local_enuErrorState ==ES_OK);
					LCD_enuWriteCommand(0x01);
					LCD_enuWriteCommand(0x08);
				}

			}
		}
	}
	else
	{
		LCD_enuWriteCommand(0x01);
		LCD_enuWriteCommand(0x0C);
		LCD_enuGoToPosition(1,1);
		LCD_enuWriteString("Power Switch");
		LCD_enuGoToPosition(2,1);
		LCD_enuWriteString("ERROR...!!!");
		while(1);
	}
}

ES_t ReadInputDigit(u8 *Copy_u8Digit)
{
	ES_t Local_enuErrorState = ES_NOK;

	if(Copy_u8Digit != NULL)
	{
		while( (Local_enuErrorState = Keypad_enuGetPressedKey(Copy_u8Digit)) == ES_OK  &&  *Copy_u8Digit == KEYPAD_NOT_PRESSED )
		{
			Local_enuErrorState = Keypad_enuGetPressedKey(Copy_u8Digit);
		}

		if( (*Copy_u8Digit<'0' || *Copy_u8Digit>'9') && *Copy_u8Digit != 'C' )
		{
			Local_enuErrorState = ES_OUT_RANGE;
			Global_u8Restart = RESTART ;
			LCD_enuGoToPosition(1,5);
			LCD_enuWriteString("NUMBERS");
			LCD_enuGoToPosition(2,5);
			LCD_enuWriteString("ONLY!!!");
			for(u8 Local_u8Iter = 0; Local_u8Iter<5 ; Local_u8Iter++)
			{
				LCD_enuWriteCommand(0x08);
				_delay_ms(10);
				LCD_enuWriteCommand(0xC);
				_delay_ms(300);
			}
			_delay_ms(3000);
		}

	}
	else Local_enuErrorState = ES_NULL_POINTER;



	return Local_enuErrorState;

}

ES_t DetectPowerStatus(void)
{
	ES_t Local_enuErrorState = ES_NOK;
	u8 Local_u8SwitchValue;

	extern u8 Global_u8Power;

	if( (Local_enuErrorState = SwitchIsPressed( POWER_SWITCH , POWER_SWITCH_PRESSED ,&Local_u8SwitchValue) ) == ES_OK )
	{
		if( Local_u8SwitchValue != SWITCH_UNPRESSED && Local_u8SwitchValue == POWER_SWITCH_PRESSED  )
		{
			switch(Global_u8Power)
			{
				case SHUTDOWN	:
				case POWER_OFF	: Global_u8Power = POWER_ON;
									break;
				case POWER_ON	: Global_u8Power = POWER_OFF;
									break;
			}
		}
	}

	return Local_enuErrorState;
}


ES_t SwitchIsPressed(u8 Copy_u8SwitchNum , u8 Copy_u8PressedValue , u8 *Copy_pu8SwitchStatus)
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8SwitchValue ;

	if(Copy_pu8SwitchStatus != NULL)
	{
		*Copy_pu8SwitchStatus = SWITCH_UNPRESSED;

		if( Copy_u8SwitchNum == POWER_SWITCH	|| Copy_u8SwitchNum == COUNTER_SWITCH )
		{
			if( ( Local_enuErrorState = Switch_enuGetPressed(Copy_u8SwitchNum , &Local_u8SwitchValue) ) == ES_OK )
			{
				if( Local_u8SwitchValue == Copy_u8PressedValue)
				{
					_delay_ms(SWITCH_BOUNCE_DELAY);
					Local_enuErrorState = Switch_enuGetPressed(Copy_u8SwitchNum , &Local_u8SwitchValue);
					if( Local_u8SwitchValue == Copy_u8PressedValue)
					{
						*Copy_pu8SwitchStatus = Copy_u8PressedValue;
					}
				}
			}
		}
		else Local_enuErrorState = ES_OUT_RANGE ;
	}
	else Local_enuErrorState = ES_NULL_POINTER ;

	return Local_enuErrorState;
}

ES_t CheckCounterStatus(void)
{
	ES_t Local_enuErrorState = ES_NOK;
	u8 Local_u8SwitchValue;


	if( (Local_enuErrorState = SwitchIsPressed( COUNTER_SWITCH , COUNTER_SWITCH_PRESSED ,&Local_u8SwitchValue) ) == ES_OK )
	{
		if( Local_u8SwitchValue != SWITCH_UNPRESSED && Local_u8SwitchValue == COUNTER_SWITCH_PRESSED)
		{
			switch(Global_u8Counter)
			{
				case NO_COUNT	: Global_u8Counter = COUNT_UP;
									break;
				case COUNT_UP	: Global_u8Counter = COUNT_DOWN;
									break;
				case COUNT_DOWN	: Global_u8Counter = NO_COUNT;
									break;
			}
		}
	}

	return Local_enuErrorState;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
#include "../Libraries/stdTypes.h"
#include "../Libraries/errorState.h"

#include "../MCAL/DIO/DIO_int.h"
#include "../HAL/LCD/LCD_int.h"
#include "../HAL/Keypad/Keypad_int.h"

#include <util/delay.h>

#define LENGTH	4
#define MAX_TRIALS 3

int
main(void) //using Proteus
{
	char Local_u8Auser[]= "7+12" , Local_u8Apass[]="x-34";
	char Local_u8AuserInput[LENGTH+1] = {0} , Local_u8ApassInput[LENGTH+1] = {0};
	u8 Local_u8Counter , Local_u8Trials=0 , key=KEYPAD_NOT_PRESSED , flag=0;

	LCD_enuInit();
	Keypad_enuInit();

	while (1)
	{
		while(Local_u8Trials < MAX_TRIALS)
		{
			LCD_enuWriteCommand(0x1);
			LCD_enuGoToPosition(1,1);
			LCD_enuWriteString("USER:");
			LCD_enuGoToPosition(2,1);
			LCD_enuWriteString("PASS:");
			LCD_enuGoToPosition(1,6);
			for(u8 Local_u8Counter=0;Local_u8Counter<LENGTH;Local_u8Counter++ )
			{
				if( Keypad_enuGetPressedKey(&key) == ES_OK )
				{
					while( key == KEYPAD_NOT_PRESSED )
					{
						Keypad_enuGetPressedKey(&key);
					}
					Local_u8AuserInput[Local_u8Counter] = key;
					LCD_enuWriteData(key);
				}
			}
			for(Local_u8Counter = 0; Local_u8Counter<LENGTH && (Local_u8AuserInput[Local_u8Counter] == Local_u8Auser[Local_u8Counter]) ;Local_u8Counter++ );

			if( Local_u8Counter != LENGTH )
			{
				flag = 1;
			}

			LCD_enuGoToPosition(2,6);
			for(u8 Local_u8Counter=0;Local_u8Counter<LENGTH;Local_u8Counter++ )
			{
				if( Keypad_enuGetPressedKey(&key) == ES_OK )
				{
					while( key == KEYPAD_NOT_PRESSED )
					{
						Keypad_enuGetPressedKey(&key);
					}
					Local_u8ApassInput[Local_u8Counter] = key;
					LCD_enuWriteData('*');
				}
			}
			for(Local_u8Counter = 0; Local_u8Counter<LENGTH && (Local_u8ApassInput[Local_u8Counter] == Local_u8Apass[Local_u8Counter]) ;Local_u8Counter++ );

			if( flag || Local_u8Counter != LENGTH )
			{
				Local_u8Trials++;
				flag = 0;
				continue;
			}
			else
			{
				break;
			}

		}
		LCD_enuWriteCommand(0x1);
		LCD_enuGoToPosition(2,2);
		if(Local_u8Trials == MAX_TRIALS)
		{
			LCD_enuWriteString("***LOCKED***");
		}
		else
		{
			LCD_enuWriteString("..WELCOME..");
		}
		for(;;);
	}
}
*/
/*
int
main(void)
{

	u8 bassem[]={0x00, 0x00, 0x01, 0x01, 0x1F, 0x04, 0x00, 0x00,
				0x04,  0x04, 0x04, 0x04, 0x03, 0x00, 0x00, 0x00,
				0x00,  0x00, 0x00, 0x15, 0x1F, 0x00, 0x00, 0x00,
				0x00,  0x00, 0x04, 0x0A, 0x0F, 0x10, 0x10, 0x10	};
	/*
	u8 behaidy[]={0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00,
					0x04, 0x04, 0x04, 0x04, 0x18, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x04, 0x1F, 0x08, 0x00, 0x00,
					0x00, 0x04, 0x0A, 0x04, 0x1F, 0x04, 0x0A, 0x04,
					0x00, 0x00, 0x00, 0x02, 0x1f, 0x0C, 0x00, 0x00,
					0x00, 0x00, 0x04, 0x02, 0x0F, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x06, 0x15, 0x12, 0x0E, 0x00
	};*/
/*	LCD_enuInit();

	LCD_enuWriteCommand(0x40);
	for (u8 i =0 ; i<32; i++)
	{
		LCD_enuWriteData(bassem[i]);
	}
	LCD_enuGoToPosition(1,10);
	LCD_enuWriteCommand(0x04);
	for(u8 Local_u8Counter=0 ; Local_u8Counter<4 ;Local_u8Counter++)
	{
		LCD_enuWriteData(Local_u8Counter);
	}

	LCD_enuGoToPosition(2,1);
	LCD_enuWriteCommand(0x14);
	LCD_enuWriteFloatNum(-251.625);
	/*
	LCD_enuWriteCommand(0x40);
	for (u8 i =0 ; i<56; i++)
	{
		LCD_enuWriteData(behaidy[i]);
	}
	LCD_enuGoToPosition(2,12);
	LCD_enuWriteCommand(0x04);
	for(u8 Local_u8Counter=0 ; Local_u8Counter<7 ;Local_u8Counter++)
	{
		LCD_enuWriteData(Local_u8Counter);
	}
	*/
/*
}


/*
int
main(void)
{
	u8 pin2 ,pin6;
	LCD_enuInit();
	LCD_enuWriteData('A');
	LCD_enuWriteData('h');
	LCD_enuWriteData('m');
	LCD_enuWriteData('e');
	LCD_enuWriteData('d');
DIO_enuSetPinDirection(DIO_u8GROUP_D , DIO_u8PIN2 , DIO_u8INPUT);
DIO_enuSetPinDirection(DIO_u8GROUP_D , DIO_u8PIN6 , DIO_u8INPUT);

DIO_enuSetPinValue(DIO_u8GROUP_D , DIO_u8PIN2 , DIO_u8FLOAT);
DIO_enuSetPinValue(DIO_u8GROUP_D , DIO_u8PIN6 , DIO_u8FLOAT);
	LCD_enuWriteString("Ahmed Reda El-Gaafrawy");
	LCD_enuGoToPosition(2,4);
	while(1)
	{
		DIO_enuGetPinValue(DIO_u8GROUP_D , DIO_u8PIN2 , &pin2);
		DIO_enuGetPinValue(DIO_u8GROUP_D , DIO_u8PIN6 , &pin6);
		if (pin2)
		{
			LCD_enuWriteCommand(0x18);
		}
		else if (pin6)
		{
			LCD_enuWriteCommand(0x1C);
		}
	}
}

*/

/*
int
main(void)
{
	u8 pinValue;
	DIO_enuSetPinDirection(DIO_u8GROUP_D, DIO_u8PIN2 , DIO_u8INPUT);
	DIO_enuSetPinValue(DIO_u8GROUP_D, DIO_u8PIN2 , DIO_u8FLOAT);

	DIO_enuSetPinDirection(DIO_u8GROUP_C, DIO_u8PIN2 , DIO_u8OUTPUT);
	DIO_enuSetPinValue(DIO_u8GROUP_D, DIO_u8PIN2 , DIO_u8LOW);

	while (1)
	{
		DIO_enuGetPinValue(DIO_u8GROUP_D, DIO_u8PIN2 , &pinValue);
		if (pinValue)
		{
			DIO_enuSetPinValue(DIO_u8GROUP_C, DIO_u8PIN2 , DIO_u8HIGH);
		}
		else
			DIO_enuSetPinValue(DIO_u8GROUP_C, DIO_u8PIN2 , DIO_u8LOW);
	}
}
*/


//#include "DIO_int.h"
//#include "Sw_int.h"
//#include "LD_int.h"

//#include <util/delay.h>
/*
int
main(void)
{
	u8 pinValue;

	if( (LD_enuInit()== ES_OK) && (Sw_enuInit() == ES_OK ))
	{
		while(1)
		{
			if(Sw_enuGetPressed(&pinValue) == ES_OK)
			{
				LD_enuSetState(pinValue);
				_delay_ms(500);
			}
		}
	}
}
/*
int
main(void)
{
	u8 pinValue;

	DIO_enuSetPinDirection(DIO_u8GROUP_D, DIO_u8PIN2 , DIO_u8INPUT);
	DIO_enuSetPinValue(DIO_u8GROUP_D, DIO_u8PIN2 , DIO_u8FLOAT);

	DIO_enuSetPinDirection(DIO_u8GROUP_C, DIO_u8PIN2 , DIO_u8OUTPUT);
	DIO_enuSetPinValue(DIO_u8GROUP_C, DIO_u8PIN2 , DIO_u8LOW);

	while (1)
	{
		DIO_enuGetPinValue(DIO_u8GROUP_D, DIO_u8PIN2 , &pinValue);
		if (pinValue)
		{
			DIO_enuSetPinValue(DIO_u8GROUP_C, DIO_u8PIN2 , DIO_u8HIGH);
		}
		else
			DIO_enuSetPinValue(DIO_u8GROUP_C, DIO_u8PIN2 , DIO_u8LOW);
	}
}




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


int
main(void)
{
	DDRA |= 0xf0;
	DDRB |= (3<<1);
	u8 r,l;
	for(;;)
	{
		for(u8 i=0; i<100 ; i++)
		{
			r=i%10;
			l=i/10;
			for (u8 j=0; j<15 ;j++)
			{
				PORTB &=~ (3<<1);

				PORTA &= 0x0f;
				PORTA |= (r<<4);
				PORTB |= (1<<1);
				_delay_ms(10);

				PORTB &=~(1<<1);
				PORTA &= 0x0f;
				PORTA |= (l<<4);
				PORTB |= (1<<2);
				_delay_ms(10);
				PORTB &=~(1<<2);
			}
		}
	}
}
*/



/*
u8 cat_seg []= {	0x3f,0x06,0x5B,0x4F,0x66,
					0x6D,0x7D,0x07,0x7F,0x6F,0x77,
					0x7C,0x39,0x5E,0x79,0x71
			   };


int
main(void)
{
	DDRA |= (1<<2); //a
	DDRC |= (1<<2); //b
	DDRC |= (1<<0); //c
	DDRA |= (1<<0); //d
	DDRB |= (1<<2); //e
	DDRB |= (1<<0); //f
	DDRB |= (1<<7); //g

	for(;;)
	{
		for (u8 i=0 ; i<16 ; i++)
		{
			PORTA &=~ ( 1 <<2 );
			PORTC &=~ ( 1 <<2 );
			PORTC &=~ ( 1 <<0 );
			PORTA &=~ ( 1 <<0 );
			PORTB &=~ ( 1 <<2 );
			PORTB &=~ ( 1 <<0 );
			PORTB &=~ ( 1 <<7 );


			PORTA |= ( ((~cat_seg[i]>>0)&1) <<2 );
			PORTC |= ( ((~cat_seg[i]>>1)&1) <<2 );
			PORTC |= ( ((~cat_seg[i]>>2)&1) <<0 );
			PORTA |= ( ((~cat_seg[i]>>3)&1) <<0 );
			PORTB |= ( ((~cat_seg[i]>>4)&1) <<2 );
			PORTB |= ( ((~cat_seg[i]>>5)&1) <<0 );
			PORTB |= ( ((~cat_seg[i]>>6)&1) <<7 );

			_delay_ms(500);
		}

	}
}

*/
/*
int
main(void)
{
	DDRA = 0xff ;
	PORTA = 0;
	while (1)
	{
		for (u8  i= 0; i<16 ; i++)
		{
			PORTA = ~ cat_seg[i];
			_delay_ms(500);
		}
	}
}
*/

/*
int
main(void)
{
	u8 press=0 , hold=0 ,unpress=0;

	//input // float
	DDRD &= ~(1<<2);
	PORTD &= ~(1<<2);

	// output // low
	DDRC |= (1<<2);
	PORTC &=~ (1<<2);

	while (1)
	{
		if (  ((PIND>>2)&1) && press == 0 && unpress == 0  && hold == 0 )
		{
			PORTC |= (1<<2);
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
			PORTC &=~ (1<<2);
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
*/



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
