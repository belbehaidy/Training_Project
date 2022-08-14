/*
 * main.c
 *
 *  Created on: Aug 5, 2022
 *      Author: Ahmed El-Gaafrawy
 */
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
