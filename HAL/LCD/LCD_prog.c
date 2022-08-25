/*
 * LCD_prog.c
 *
 *  Created on: Aug 12, 2022
 *      Author: Ahmed El-Gaafrawy
 */
#include "../../Libraries/stdTypes.h"
#include "../../Libraries/errorState.h"

#include "../../MCAL/DIO/DIO_int.h"

#include "LCD_config.h"
#include "LCD_priv.h"

#include <util/delay.h>

ES_t LCD_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	_delay_ms(30);

	DIO_enuSetPinDirection(RS_GRP , RS_PIN , DIO_u8OUTPUT);
#if RW_GRP != NC && RW_PIN != NC
	DIO_enuSetPinDirection(RW_GRP , RW_PIN , DIO_u8OUTPUT);
#endif
	DIO_enuSetPinDirection(EN_GRP , EN_PIN , DIO_u8OUTPUT);

	DIO_enuSetPinDirection(D7_GRP , D7_PIN , DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D6_GRP , D6_PIN , DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D5_GRP , D5_PIN , DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D4_GRP , D4_PIN , DIO_u8OUTPUT);

#if LCD_MODE == EIGHT_BIT

	DIO_enuSetPinDirection(D3_GRP , D3_PIN , DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D2_GRP , D2_PIN , DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D1_GRP , D1_PIN , DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D0_GRP , D0_PIN , DIO_u8OUTPUT);

	DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	LCD_enuWriteNLatch( 0x38 );

#elif LCD_MODE == FOUR_BIT

	DIO_enuSetPinValue(D7_GRP , D7_PIN , DIO_u8LOW);
	DIO_enuSetPinValue(D6_GRP , D6_PIN , DIO_u8LOW);
	DIO_enuSetPinValue(D5_GRP , D5_PIN , DIO_u8HIGH);
	DIO_enuSetPinValue(D4_GRP , D4_PIN , DIO_u8LOW);

	DIO_enuSetPinValue(EN_GRP , EN_PIN , DIO_u8HIGH);
	_delay_ms(1);
	DIO_enuSetPinValue(EN_GRP , EN_PIN , DIO_u8LOW);
	_delay_ms(4);

	DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	LCD_enuWriteNLatch( 0x28 );

#endif

	DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	LCD_enuWriteNLatch( 0x08 );//Display still OFF

	DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	LCD_enuWriteNLatch( 0x01 );//Clears Display Data

	DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	LCD_enuWriteNLatch( 0x06 );



	return Local_enuErrorState = ES_OK; //////////////////////////////////////////////////NEEDS ADJUSTMENT for error handling
}

ES_t LCD_enuWriteData(u8 Copy_u8Data)
{
	ES_t Local_enuErrorState = ES_NOK;

	Local_enuErrorState = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
	LCD_enuWriteNLatch( Copy_u8Data );

	return Local_enuErrorState;
}

ES_t LCD_enuWriteCommand(u8 Copy_u8Cmnd)
{
	ES_t Local_enuErrorState = ES_NOK;

	Local_enuErrorState = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	LCD_enuWriteNLatch( Copy_u8Cmnd );


	return Local_enuErrorState;
}

ES_t LCD_enuWriteString(const char * Copy_pcString)
{
	ES_t Local_enuErrorState = ES_NOK;

	while (*Copy_pcString)
	{
		Local_enuErrorState = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
		LCD_enuWriteNLatch( *Copy_pcString++ );
	}

	return Local_enuErrorState;
}


ES_t LCD_enuGoToPosition(u8 Copy_u8Row , u8 Copy_u8Column)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_u8Row >= 1 && Copy_u8Row <=2 &&
			Copy_u8Column >=1 && Copy_u8Column <=16)
	{
		Local_enuErrorState = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
		LCD_enuWriteNLatch( 0x80 + ( Copy_u8Row - 1 )*0x40 + ( Copy_u8Column - 1 ) );
	}
	return Local_enuErrorState;
}

static ES_t LCD_enuWriteNLatch(u8 Copy_u8Byte)
{
#if RW_GRP != NC  && RW_PIN != NC
	DIO_enuSetPinValue(RW_GRP , RW_PIN , DIO_u8LOW);
#endif
	DIO_enuSetPinValue(EN_GRP , EN_PIN , DIO_u8LOW);

#if LCD_MODE == EIGHT_BIT
	DIO_enuSetPinValue(D7_GRP , D7_PIN , ((Copy_u8Byte >> 7 )&1 ));
	DIO_enuSetPinValue(D6_GRP , D6_PIN , ((Copy_u8Byte >> 6 )&1 ));
	DIO_enuSetPinValue(D5_GRP , D5_PIN , ((Copy_u8Byte >> 5 )&1 ));
	DIO_enuSetPinValue(D4_GRP , D4_PIN , ((Copy_u8Byte >> 4 )&1 ));

	DIO_enuSetPinValue(D3_GRP , D3_PIN , ((Copy_u8Byte >> 3 )&1 ));
	DIO_enuSetPinValue(D2_GRP , D2_PIN , ((Copy_u8Byte >> 2 )&1 ));
	DIO_enuSetPinValue(D1_GRP , D1_PIN , ((Copy_u8Byte >> 1 )&1 ));
	DIO_enuSetPinValue(D0_GRP , D0_PIN , ((Copy_u8Byte >> 0 )&1 ));

	DIO_enuSetPinValue(EN_GRP , EN_PIN , DIO_u8HIGH);
	_delay_ms(1);
	DIO_enuSetPinValue(EN_GRP , EN_PIN , DIO_u8LOW);
	_delay_ms(4);


#elif LCD_MODE == FOUR_BIT
	DIO_enuSetPinValue(D7_GRP , D7_PIN , ((Copy_u8Byte >> 7 )&1 ));
	DIO_enuSetPinValue(D6_GRP , D6_PIN , ((Copy_u8Byte >> 6 )&1 ));
	DIO_enuSetPinValue(D5_GRP , D5_PIN , ((Copy_u8Byte >> 5 )&1 ));
	DIO_enuSetPinValue(D4_GRP , D4_PIN , ((Copy_u8Byte >> 4 )&1 ));

	DIO_enuSetPinValue(EN_GRP , EN_PIN , DIO_u8HIGH);
	_delay_ms(1);
	DIO_enuSetPinValue(EN_GRP , EN_PIN , DIO_u8LOW);
	_delay_ms(4);

	DIO_enuSetPinValue(D7_GRP , D7_PIN , ((Copy_u8Byte >> 3 )&1 ));
	DIO_enuSetPinValue(D6_GRP , D6_PIN , ((Copy_u8Byte >> 2 )&1 ));
	DIO_enuSetPinValue(D5_GRP , D5_PIN , ((Copy_u8Byte >> 1 )&1 ));
	DIO_enuSetPinValue(D4_GRP , D4_PIN , ((Copy_u8Byte >> 0 )&1 ));

	DIO_enuSetPinValue(EN_GRP , EN_PIN , DIO_u8HIGH);
	_delay_ms(1);
	DIO_enuSetPinValue(EN_GRP , EN_PIN , DIO_u8LOW);
	_delay_ms(4);
#else
#error "LCD mode is wrong ya 7aywan"
#endif

	return ES_OK;
}

/*		ES_t LCD_enuWriteIntegerNum (s32 Copy_s32Num)		*/
ES_t LCD_enuWriteIntegerNum (s32 Copy_s32Num)
{
	ES_t Local_enuErrorState = ES_NOK , Local_enuStatus[2];

	u8 Local_u8DigitCounter = 1 ;

	Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	Local_enuStatus[1] = LCD_enuWriteNLatch(0x06);

	if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
	{
		if( Copy_s32Num < 0 )
		{
			Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
			Local_enuStatus[1] = LCD_enuWriteNLatch( MINUS_SIGN );
			Copy_s32Num = - Copy_s32Num;
		}

		if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
		{
			u32 Local_u32Number = Copy_s32Num;

			while(Local_u32Number>10 && Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK)
			{
				Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
				Local_enuStatus[1] = LCD_enuWriteNLatch(0x14);
				Local_u32Number /=10;
			}

			if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
			{
				Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
				Local_enuStatus[1] = LCD_enuWriteNLatch(0x04);

				if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
				{
					while(Copy_s32Num && Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK)
					{
						Local_u8DigitCounter++;
						Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
						Local_enuStatus[1] = LCD_enuWriteNLatch(ZERO_ASCII + (Copy_s32Num % 10));
						Copy_s32Num /= 10;
					}

					if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
					{
						while(Local_u8DigitCounter-- && Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
						{
							Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
							Local_enuStatus[1] = LCD_enuWriteNLatch(0x14);
						}

						if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
						{
							Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
							Local_enuStatus[1] = LCD_enuWriteNLatch(0x06);
						}
					}

				}

			}

		}
	}
	if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK)
			 	 Local_enuErrorState = ES_OK;

	return (Local_enuErrorState = ES_OK);
}

/*		ES_t LCD_enuWriteFloatNum(f32 Copy_f32Num)		*/
ES_t LCD_enuWriteFloatNum(f32 Copy_f32Num)
{
	ES_t Local_enuErrorState = ES_NOK , Local_enuStatus[2];

	u8 Local_u8DigitCounter = 0 ;
	u32 Local_u32Integer = 0 , Local_u32Number = 0;
	f32 Local_f32_Fraction = 0.0 ;

	Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
	Local_enuStatus[1] = LCD_enuWriteNLatch(0x06);

	if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
	{
		if( Copy_f32Num < 0 )
		{
			Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
			Local_enuStatus[1] = LCD_enuWriteNLatch( MINUS_SIGN );
			Copy_f32Num = - Copy_f32Num;
		}

		Local_u32Integer = Local_u32Number =(u32)Copy_f32Num;
		Local_f32_Fraction = Copy_f32Num - Local_u32Integer;

		if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
		{
			while(Local_u32Number>10 && Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
			{
				Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
				Local_enuStatus[0] = LCD_enuWriteNLatch(0x14);
				Local_u32Number /=10;
			}

			if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
			{
				Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
				Local_enuStatus[1] = LCD_enuWriteNLatch(0x04);

				if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
				{
					do
					{
						Local_u8DigitCounter++;
						DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
						LCD_enuWriteNLatch(ZERO_ASCII + (Local_u32Integer % 10));
						Local_u32Integer /= 10;
					}while(Local_u32Integer && Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK);

					if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
					{
						do
						{
							Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
							Local_enuStatus[1] = LCD_enuWriteNLatch(0x14);
						}while(Local_u8DigitCounter-- && Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK );

						if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
						{
							Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
							Local_enuStatus[1] = LCD_enuWriteNLatch(0x06);

							if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
							{
								if(Local_f32_Fraction > 0.0f)
								{
									Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
									Local_enuStatus[1] = LCD_enuWriteNLatch(DOT);
								}

								if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
								{
									Local_u8DigitCounter =0;
									while(Local_f32_Fraction > 0.0f  && Local_u8DigitCounter<6  && Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK )
									{
										Local_u8DigitCounter++;
										Local_f32_Fraction *= 10.0;
										Local_u32Integer = (u32) Local_f32_Fraction;
										Local_f32_Fraction -= Local_u32Integer;
										Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
										Local_enuStatus[1] = LCD_enuWriteNLatch((ZERO_ASCII + Local_u32Integer));
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if( Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK)
		 	 Local_enuErrorState = ES_OK;

	return Local_enuErrorState ;
}

/*		ES_t LCD_enuWriteExtraPattern(u8 * Copy_pu8Pattern , u8 Copy_u8NumOfPatterns , u8 Copy_u8Start , u8 Copy_u8DisplayDirection)		*/
ES_t LCD_enuWriteExtraPattern(u8 * Copy_pu8Pattern , u8 Copy_u8NumOfPatterns , u8 Copy_u8Start , u8 Copy_u8DisplayDirection)
{

	ES_t Local_enuErrorState = ES_NOK , Local_enuStatus[2];

	if( Copy_u8Start >= PATTERN_1  && Copy_u8Start <= PATTERN_8 && (Copy_u8Start+Copy_u8NumOfPatterns-1)<= PATTERN_8 )
	{
		Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);
		Local_enuStatus[1] = LCD_enuWriteNLatch(CGROM_BASE_ADD + PATTERN_BYTES*(Copy_u8Start-PATTERN_1));

		for (u8 Local_u8Counter = 0 ; Local_u8Counter < PATTERN_BYTES*Copy_u8NumOfPatterns && Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK ; Local_u8Counter++)
		{
			Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8HIGH);
			Local_enuStatus[1] = LCD_enuWriteNLatch(Copy_pu8Pattern[Local_u8Counter]);
		}
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	if( Local_enuErrorState != ES_OUT_RANGE && Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK)
	{
		if(Copy_u8DisplayDirection == DISPLAY_DIR_RIGHT || Copy_u8DisplayDirection == DISPLAY_DIR_RIGHT )
		{
			Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);

			switch(Copy_u8DisplayDirection)
			{
				case DISPLAY_DIR_RIGHT :	Local_enuStatus[1] = LCD_enuWriteNLatch(0x06);
												break;
				case DISPLAY_DIR_LEFT  :	Local_enuStatus[1] = LCD_enuWriteNLatch(0x04);
												break;
			}
		}
		else Local_enuErrorState = ES_OUT_RANGE;
	}

	if( Local_enuErrorState != ES_OUT_RANGE && Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK)
	 	 Local_enuErrorState = ES_OK;

	return Local_enuErrorState;
}

ES_t LCD_enuSetTextDirection(u8 Copy_u8Direction)
{
	ES_t Local_enuErrorState = ES_NOK , Local_enuStatus[2];

	if(Copy_u8Direction == DISPLAY_DIR_RIGHT || Copy_u8Direction == DISPLAY_DIR_RIGHT )
	{
		Local_enuStatus[0] = DIO_enuSetPinValue(RS_GRP , RS_PIN , DIO_u8LOW);

		switch(Copy_u8Direction)
		{
			case DISPLAY_DIR_RIGHT :	Local_enuStatus[1] = LCD_enuWriteNLatch(0x06);
											break;
			case DISPLAY_DIR_LEFT  :	Local_enuStatus[1] = LCD_enuWriteNLatch(0x04);
											break;
		}

	}
	else Local_enuErrorState = ES_OUT_RANGE;

	if( Local_enuErrorState != ES_OUT_RANGE && Local_enuStatus[0] == ES_OK && Local_enuStatus[1]== ES_OK)
		 	 Local_enuErrorState = ES_OK;

	return Local_enuErrorState;
}
