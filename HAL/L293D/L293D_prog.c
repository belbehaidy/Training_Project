/*
 * L293D_prog.c
 *
 *  Created on: Sep 2, 2022
 *      Author: basse
 */

#include "..\..\Libraries\stdTypes.h"
#include "..\..\Libraries\errorState.h"

#include "..\..\MCAL\DIO\DIO_int.h"
#include "..\..\MCAL\PWM\PWM_int.h"

#include "L293D_priv.h"
#include "L293D_config.h"

extern u8 Motors_u8MaxNum ;
extern Motor_t Motors[];

ES_t L293D_enuInit( void )
{
	ES_t Local_enuErrorState = ES_NOK , Local_AenuErrorStates[2];

	/* 		Setting H-Bridge EN1 Pin 		*/
	Local_AenuErrorStates[0] = DIO_enuSetPinDirection( H_EN1_GRP , H_EN1_PIN , DIO_u8OUTPUT);
	Local_AenuErrorStates[1] = DIO_enuSetPinValue( H_EN1_GRP , H_EN1_PIN , DIO_u8LOW);

	if( Local_AenuErrorStates[0] == ES_OK && Local_AenuErrorStates[1] == ES_OK)
	{
		/* 		Setting H-Bridge EN2 Pin 		*/
		Local_AenuErrorStates[0] = DIO_enuSetPinDirection( H_EN2_GRP , H_EN2_PIN , DIO_u8OUTPUT);
		Local_AenuErrorStates[1] = DIO_enuSetPinValue( H_EN2_GRP , H_EN2_PIN , DIO_u8LOW);

		if( Local_AenuErrorStates[0] == ES_OK && Local_AenuErrorStates[1] == ES_OK)
		{
			/* 		Setting H-Bridge A1 Pin 		*/
			Local_AenuErrorStates[0] = DIO_enuSetPinDirection( H_A1_GRP , H_A1_PIN , DIO_u8OUTPUT);
			Local_AenuErrorStates[1] = DIO_enuSetPinValue( H_A1_GRP , H_A1_PIN , DIO_u8LOW);

			if( Local_AenuErrorStates[0] == ES_OK && Local_AenuErrorStates[1] == ES_OK)
			{
				/* 		Setting H-Bridge A2 Pin 		*/
				Local_AenuErrorStates[0] = DIO_enuSetPinDirection( H_A2_GRP , H_A2_PIN , DIO_u8OUTPUT);
				Local_AenuErrorStates[1] = DIO_enuSetPinValue( H_A2_GRP , H_A2_PIN , DIO_u8LOW);

				if( Local_AenuErrorStates[0] == ES_OK && Local_AenuErrorStates[1] == ES_OK)
				{
					/* 		Setting H-Bridge A3 Pin 		*/
					Local_AenuErrorStates[0] = DIO_enuSetPinDirection( H_A3_GRP , H_A3_PIN , DIO_u8OUTPUT);
					Local_AenuErrorStates[1] = DIO_enuSetPinValue( H_A3_GRP , H_A3_PIN , DIO_u8LOW);

					if( Local_AenuErrorStates[0] == ES_OK && Local_AenuErrorStates[1] == ES_OK)
					{
						/* 		Setting H-Bridge A4 Pin 		*/
						Local_AenuErrorStates[0] = DIO_enuSetPinDirection( H_A4_GRP , H_A4_PIN , DIO_u8OUTPUT);
						Local_AenuErrorStates[1] = DIO_enuSetPinValue( H_A4_GRP , H_A4_PIN , DIO_u8LOW);
					}
				}
			}
		}
	}

	if( Local_AenuErrorStates[0] == ES_OK && Local_AenuErrorStates[1] == ES_OK)
		Local_enuErrorState = ES_OK ;

	//////////////////////////////////////////we need to set the PWM module settings as well

	return Local_enuErrorState ;
}

ES_t Le93D_enuSetDirectio( u8 Copy_u8MotorNum , u8 Copy_u8MotorDirection)
{
	ES_t Local_enuErrorState = ES_NOK , Local_AenuErrorStates[2];

	if( Copy_u8MotorDirection == CLOCK_WISE_DIRECTION || Copy_u8MotorDirection == COUNTER_CLOCK_WISE_DIRECTION )
	{
		if( Copy_u8MotorNum == MOTOR1 )
			{

				if( PWM_1_enuDisable( ) == ES_OK )
				{
					switch( Copy_u8MotorDirection )
					{
						case 		CLOCK_WISE_DIRECTION	:	Local_AenuErrorStates[0] = DIO_enuSetPinValue( H_A1_GRP , H_A1_PIN , DIO_u8LOW);
																Local_AenuErrorStates[1] = DIO_enuSetPinValue( H_A2_GRP , H_A2_PIN , DIO_u8LOW);
																Local_AenuErrorStates[2] = DIO_enuSetPinValue( H_A1_GRP , H_A1_PIN , DIO_u8HIGH);
									break;

						case COUNTER_CLOCK_WISE_DIRECTION	:	Local_AenuErrorStates[0] = DIO_enuSetPinValue( H_A2_GRP , H_A2_PIN , DIO_u8LOW);
																Local_AenuErrorStates[1] = DIO_enuSetPinValue( H_A1_GRP , H_A1_PIN , DIO_u8LOW);
																Local_AenuErrorStates[2] = DIO_enuSetPinValue( H_A2_GRP , H_A2_PIN , DIO_u8HIGH);
									break;
					}
					if( Local_AenuErrorStates[0] == ES_OK && Local_AenuErrorStates[1] == ES_OK && Local_AenuErrorStates[2] == ES_OK )
						Local_enuErrorState = PWM_1_enuEnable();
				}
			}
			else if(  Copy_u8MotorNum == MOTOR2 )
			{
				if( PWM_2_enuDisable( ) == ES_OK )
				{
					switch( Copy_u8MotorDirection )
					{
						case 		CLOCK_WISE_DIRECTION	:	Local_AenuErrorStates[0] = DIO_enuSetPinValue( H_A3_GRP , H_A3_PIN , DIO_u8LOW);
																Local_AenuErrorStates[1] = DIO_enuSetPinValue( H_A4_GRP , H_A4_PIN , DIO_u8LOW);
																Local_AenuErrorStates[2] = DIO_enuSetPinValue( H_A3_GRP , H_A3_PIN , DIO_u8HIGH);
									break;

						case COUNTER_CLOCK_WISE_DIRECTION	:	Local_AenuErrorStates[0] = DIO_enuSetPinValue( H_A4_GRP , H_A4_PIN , DIO_u8LOW);
																Local_AenuErrorStates[1] = DIO_enuSetPinValue( H_A3_GRP , H_A3_PIN , DIO_u8LOW);
																Local_AenuErrorStates[2] = DIO_enuSetPinValue( H_A4_GRP , H_A4_PIN , DIO_u8HIGH);
									break;
					}
				}
				if( Local_AenuErrorStates[0] == ES_OK && Local_AenuErrorStates[1] == ES_OK && Local_AenuErrorStates[2] == ES_OK )
					Local_enuErrorState = PWM_2_enuEnable();
			}
			else Local_enuErrorState = ES_OUT_RANGE;
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	return Local_enuErrorState ;
}

ES_t Le93D_enuSetSpeed( u8 Copy_u8MotorNum , u8 Copy_u8MotorMaxSpeedPercentage) // three cases 0 -> EN = LOW , 100 -> EN = HIGH , Others Calc Duty from RMS value law
{
	ES_t Local_enuErrorState = ES_NOK;

	switch( Copy_u8MotorMaxSpeedPercentage )
	{
	case 0	:	if( Copy_u8MotorNum == MOTOR1 )
				{
					Local_enuErrorState = DIO_enuSetPinValue( H_EN1_GRP , H_EN1_PIN , DIO_u8LOW);
				}
				else if ( Copy_u8MotorNum == MOTOR2 )
				{
					Local_enuErrorState = DIO_enuSetPinValue( H_EN2_GRP , H_EN2_PIN , DIO_u8LOW);
				}
				break;
	case 100:	if( Copy_u8MotorNum == MOTOR1 )
				{
					Local_enuErrorState = DIO_enuSetPinValue( H_EN1_GRP , H_EN1_PIN , DIO_u8HIGH);
				}
				else if ( Copy_u8MotorNum == MOTOR2 )
				{
					Local_enuErrorState = DIO_enuSetPinValue( H_EN2_GRP , H_EN2_PIN , DIO_u8HIGH);
				}
				break;
	default	:	f32 Local_f32Sqrt = (f32) Copy_u8MotorMaxSpeedPercentage;
				u32 DutyCycle = (u32) ( *( (f32 *) &( *( (u64 *) &Local_f32Sqrt ) >> 1 ) ) ) ;


				break;
	}

	return Local_enuErrorState ;
}
