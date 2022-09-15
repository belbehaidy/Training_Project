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
#include "..\..\MCAL\GIE\GIE_int.h"


#include "L293D_priv.h"
#include "L293D_config.h"

extern u8 Motors_u8MaxNum ;
extern Motor_t Motors[];

ES_t L293D_enuInit( void )
{
	ES_t Local_enuErrorState = ES_NOK , Local_AenuErrorStates[4];

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < Motors_u8MaxNum ; Local_u8Iter++)
	{
		/****************************************/
		/* 			Setting Motor EN Pin 		*/
		/****************************************/
		Local_AenuErrorStates[0] = DIO_enuSetPinDirection(	Motors[Local_u8Iter].EN.PinGrp , Motors[Local_u8Iter].EN.PinNum , DIO_u8OUTPUT);
		Local_AenuErrorStates[1] = DIO_enuSetPinValue(	Motors[Local_u8Iter].EN.PinGrp , Motors[Local_u8Iter].EN.PinNum , DIO_u8LOW);

		if( Local_AenuErrorStates[0] == ES_OK && Local_AenuErrorStates[1] == ES_OK)
		{
			/****************************************/
			/* 		Setting Motor A Pin 			*/
			/****************************************/
			Local_AenuErrorStates[0] = DIO_enuSetPinDirection(	Motors[Local_u8Iter].A.PinGrp , Motors[Local_u8Iter].A.PinNum , DIO_u8OUTPUT);
			Local_AenuErrorStates[1] = DIO_enuSetPinValue(	Motors[Local_u8Iter].A.PinGrp , Motors[Local_u8Iter].A.PinNum , DIO_u8LOW);

			/****************************************/
			/* 		Setting Motor B Pin 			*/
			/****************************************/
			Local_AenuErrorStates[2] = DIO_enuSetPinDirection(	Motors[Local_u8Iter].B.PinGrp , Motors[Local_u8Iter].B.PinNum , DIO_u8OUTPUT);
			Local_AenuErrorStates[3] = DIO_enuSetPinValue(	Motors[Local_u8Iter].B.PinGrp , Motors[Local_u8Iter].B.PinNum , DIO_u8LOW);

			if( Local_AenuErrorStates[0] == ES_OK && Local_AenuErrorStates[1] == ES_OK &&
				Local_AenuErrorStates[2] == ES_OK && Local_AenuErrorStates[3] == ES_OK)
			{
				/****************************************/
				/* 		Setting Motor PWM Source		*/
				/****************************************/
				Local_AenuErrorStates[0] = PWM_enuInit() ;
				Local_AenuErrorStates[1] = GIE_enuInit() ;

				if( Local_AenuErrorStates[0] == ES_OK && Local_AenuErrorStates[1] == ES_OK )
				{
					Local_AenuErrorStates[0] = PWM_enuSetWGM_Mode( Motors[Local_u8Iter].PWM , WGM_PC_8_bit );
					Local_AenuErrorStates[1] = PWM_enuSetClkPrescaler( Motors[Local_u8Iter].PWM , PRES_8 );
					Local_AenuErrorStates[2] = PWM_enuSetCOM_Mode( Motors[Local_u8Iter].PWM , COMP_NON_INVERTED );
					Local_AenuErrorStates[3] = PWM_enuSetInterruptMode( Motors[Local_u8Iter].PWM , TC_OUT_COMP_INT );

					if( Local_AenuErrorStates[0] == ES_OK && Local_AenuErrorStates[1] == ES_OK &&
						Local_AenuErrorStates[2] == ES_OK && Local_AenuErrorStates[3] == ES_OK)
					{
						Local_AenuErrorStates[0] = PWM_enuSetDutyCycle( Motors[Local_u8Iter].PWM , 0 );
						Local_AenuErrorStates[1] = GIE_enuEnable();
					}
				 }
			 }
		 }
	}

	if( Local_AenuErrorStates[0] == ES_OK && Local_AenuErrorStates[1] == ES_OK &&
		Local_AenuErrorStates[2] == ES_OK && Local_AenuErrorStates[3] == ES_OK)
		Local_enuErrorState = ES_OK ;

	return Local_enuErrorState ;
}

ES_t Le93D_enuSetDirectio( u8 Copy_u8MotorNum , u8 Copy_u8MotorDirection)
{
	ES_t Local_enuErrorState = ES_NOK , Local_AenuErrorStates[2];

	if( Copy_u8MotorDirection == CLOCK_WISE_DIRECTION || Copy_u8MotorDirection == COUNTER_CLOCK_WISE_DIRECTION )
	{

		for( u8 Local_u8Iter = 0 ; Local_u8Iter < Motors_u8MaxNum ; Local_u8Iter++ )
		{
			if( Motors[Local_u8Iter].MotorNum == Copy_u8MotorNum )
			{
				switch( Copy_u8MotorDirection )
				{
					case 		CLOCK_WISE_DIRECTION	:	Local_AenuErrorStates[0] = DIO_enuSetPinValue(	Motors[Local_u8Iter].A.PinGrp ,
																											Motors[Local_u8Iter].A.PinNum ,	DIO_u8LOW);
															Local_AenuErrorStates[1] = DIO_enuSetPinValue( 	Motors[Local_u8Iter].B.PinGrp ,
																											Motors[Local_u8Iter].B.PinNum , DIO_u8LOW);
															Local_AenuErrorStates[2] = DIO_enuSetPinValue( 	Motors[Local_u8Iter].A.PinGrp ,
																											Motors[Local_u8Iter].A.PinNum , DIO_u8HIGH);
															break;

					case COUNTER_CLOCK_WISE_DIRECTION	:	Local_AenuErrorStates[0] = DIO_enuSetPinValue(	Motors[Local_u8Iter].B.PinGrp ,
																											Motors[Local_u8Iter].B.PinNum ,	DIO_u8LOW);
															Local_AenuErrorStates[1] = DIO_enuSetPinValue( 	Motors[Local_u8Iter].A.PinGrp ,
																											Motors[Local_u8Iter].A.PinNum , DIO_u8LOW);
															Local_AenuErrorStates[2] = DIO_enuSetPinValue( 	Motors[Local_u8Iter].B.PinGrp ,
																											Motors[Local_u8Iter].B.PinNum , DIO_u8HIGH);
															break;
				}
				if( Local_AenuErrorStates[0] == ES_OK && Local_AenuErrorStates[1] == ES_OK && Local_AenuErrorStates[2] == ES_OK )
					Local_enuErrorState = ES_OK;
			}
		}
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	return Local_enuErrorState ;
}

ES_t Le93D_enuSetSpeed( u8 Copy_u8MotorNum , u8 Copy_u8MotorMaxSpeedPercentage) // three cases 0 -> EN = LOW , 100 -> EN = HIGH , Other Calculate Duty Cycle from RMS value law
{
	ES_t Local_enuErrorState = ES_NOK;

	if( Copy_u8MotorMaxSpeedPercentage >=0 && Copy_u8MotorMaxSpeedPercentage <= 100 )
	{
		for( u8 Local_u8Iter = 0 ; Local_u8Iter < Motors_u8MaxNum ; Local_u8Iter++ )
		{
			if( Motors[Local_u8Iter].MotorNum == Copy_u8MotorNum )
			{
				switch( Copy_u8MotorMaxSpeedPercentage )
				{
					case 0	:
					case 100:	Local_enuErrorState = PWM_enuSetDutyCycle( Motors[Local_u8Iter].PWM , Copy_u8MotorMaxSpeedPercentage );
								break;
					default	:	{	/*	RMS = Amplitude * SQRT(Duty Cycle)	*/
									f32 DutyCycle = ( Copy_u8MotorMaxSpeedPercentage * Copy_u8MotorMaxSpeedPercentage ) / 100.0 ;
									Local_enuErrorState = PWM_enuSetDutyCycle( Motors[Local_u8Iter].PWM , DutyCycle );
								}
								break;
				}
			}
		}
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	return Local_enuErrorState ;
}
