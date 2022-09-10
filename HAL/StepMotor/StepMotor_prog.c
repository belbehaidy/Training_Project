/*
 * StepMotor_prog.c
 *
 *  Created on: Sep 10, 2022
 *      Author: basse
 */

#include "..\..\Libraries\stdTypes.h"
#include "..\..\Libraries\errorState.h"

#include "..\..\MCAL\DIO\DIO_int.h"
#include "..\..\MCAL\TIMER\TIMER_int.h"

#include "StepMotor_priv.h"
#include "StepMotor_config.h"

u16 Global_u16FullCycleSteps;


ES_t StepMotor_enuInit( void )
{
	ES_t Local_enuErrorState = ES_NOK , Local_AenuErrorStates[8];
	u8 Local_u8Iter= 0 ;

	if( STRIDE_ANGLE == 0.0F)
	{
		Global_u16FullCycleSteps = (u16)( (f64)360.0 /( (f64)STRIDE_ANGLE * 2 ) ) ;
	}
	else if( RESOLUTION > 0.0F )
	{
		Global_u16FullCycleSteps = (u16)( (f64)360.0 / (f64)RESOLUTION ) ;
	}
	else
	{
		#warning " Stepper Motor Stride Angle / Resolution has to be Specified. "
	}

	Timer_enuInit();

	Local_AenuErrorStates[0] = DIO_enuSetPinDirection( COIL1_GRP, COIL1_PIN, DIO_u8OUTPUT);
	Local_AenuErrorStates[1] = DIO_enuSetPinValue( COIL1_GRP, COIL1_PIN, COIL_IDLE);
	Local_AenuErrorStates[2] = DIO_enuSetPinDirection( COIL2_GRP, COIL2_PIN, DIO_u8OUTPUT);
	Local_AenuErrorStates[3] = DIO_enuSetPinValue( COIL2_GRP, COIL2_PIN, COIL_IDLE);
	Local_AenuErrorStates[4] = DIO_enuSetPinDirection( COIL3_GRP, COIL3_PIN, DIO_u8OUTPUT);
	Local_AenuErrorStates[5] = DIO_enuSetPinValue( COIL3_GRP, COIL3_PIN, COIL_IDLE);
	Local_AenuErrorStates[6] = DIO_enuSetPinDirection( COIL4_GRP, COIL4_PIN, DIO_u8OUTPUT);
	Local_AenuErrorStates[7] = DIO_enuSetPinValue( COIL4_GRP, COIL4_PIN, COIL_IDLE);

	for( ; (Local_u8Iter < 8) && ( Local_AenuErrorStates[Local_u8Iter] == ES_OK ) ; Local_u8Iter++ );

	return ( ( Local_u8Iter == 8 )? ES_OK : Local_AenuErrorStates[Local_u8Iter] ) ;
}

ES_t StepMotor_enuFullCycle( u8 Copy_u8Direction , u16 Copy_u16StepSpeed)
{
	ES_t Local_enuErrorState = ES_NOK , Local_AenuErrorStates[4];

	u8 Local_u8StepDiff , Local_u8Output , Local_u8Iter = 0 , Local_u8ErrorFlag = 0 ;
	u16 Local_u16Iter;

	switch( Copy_u8Direction )
	{
		case CLOCKWISE			:	Local_u16Iter =  Global_u16FullCycleSteps-1 ;
									Local_u8StepDiff = -1 ;
									break;
		case COUNTER_CLOCKWISE	:	Local_u16Iter =  0 ;
									Local_u8StepDiff = 1 ;
									break;
		default	: 	Local_u16Iter =  Global_u16FullCycleSteps ;
					Local_u8StepDiff = 1 ;
					Local_u8ErrorFlag =1 ;
					Local_enuErrorState = ES_OUT_RANGE;
					#warning " StepMotor_enuFullCycle(): Un-identified Direction, No motion "
	}



	for( ; ( Local_u16Iter >= 0 ) && ( Local_u16Iter <  Global_u16FullCycleSteps ) ; Local_u16Iter += Local_u8StepDiff )
	{
#if( COIL_ACTIVE == DIO_u8LOW )
		Local_u8Output = (u8) ( OUTPUT_LOW_PATTERN & ~( BIT_MASK << ( Local_u16Iter % COIL_NUM ) ) );
#elif ( COIL_ACTIVE == DIO_u8HIGH )
		Local_u8Output = (u8) ( OUTPUT_HIGH_PATTERN  | ( BIT_MASK << ( Local_u16Iter % COIL_NUM ) ) ) ;
#endif
		Local_AenuErrorStates[0] = DIO_enuSetPinValue( COIL1_GRP, COIL1_PIN, (( Local_u8Output >> COIL1_SHIFT ) & BIT_MASK ));
		Local_AenuErrorStates[1] = DIO_enuSetPinValue( COIL2_GRP, COIL2_PIN, (( Local_u8Output >> COIL2_SHIFT ) & BIT_MASK ));
		Local_AenuErrorStates[2] = DIO_enuSetPinValue( COIL3_GRP, COIL3_PIN, (( Local_u8Output >> COIL3_SHIFT ) & BIT_MASK ));
		Local_AenuErrorStates[3] = DIO_enuSetPinValue( COIL4_GRP, COIL4_PIN, (( Local_u8Output >> COIL4_SHIFT ) & BIT_MASK ));
		for( Local_u8Iter = 0 ; (Local_u8Iter < COIL_NUM) && ( Local_AenuErrorStates[Local_u8Iter] == ES_OK ) ; Local_u8Iter++ );
		if( Local_u8Iter != COIL_NUM )
		{
			Local_u8ErrorFlag = 1 ;
			break;
		}
	}
	if( !Local_u8ErrorFlag)
	{
		Local_enuErrorState = Timer_PollingDelay( TIMER2 , (u16)( 1000UL / (u32)Copy_u16StepSpeed ) );
	}

	return Local_enuErrorState ;
}

ES_t StepMotor_enuGoToAngle( u8 Copy_u8Direction , u8 Copy_u8Angle , u16 Copy_u16StepSpeed )
{
	ES_t Local_enuErrorState = ES_NOK , Local_AenuErrorStates[4];

	u8 Local_u8StepDiff , Local_u8Output , Local_u8Iter = 0 , Local_u8ErrorFlag = 0 ;
	u16 Local_u16Iter , Local_u8AngleSteps;

	Local_u8AngleSteps = (u8) ( ( Copy_u8Angle * (u32) Global_u16FullCycleSteps ) / 360UL ) ;

	switch( Copy_u8Direction )
	{
		case CLOCKWISE			:	Local_u16Iter =  Local_u8AngleSteps-1 ;
									Local_u8StepDiff = -1 ;
									break;
		case COUNTER_CLOCKWISE	:	Local_u16Iter =  0 ;
									Local_u8StepDiff = 1 ;
									break;
		default	: 	Local_u16Iter =  Global_u16FullCycleSteps ;
					Local_u8StepDiff = 1 ;
					Local_u8ErrorFlag =1 ;
					Local_enuErrorState = ES_OUT_RANGE;
					#warning " StepMotor_enuFullCycle(): Un-identified Direction, No motion "
	}
	if( Local_enuErrorState == ES_NOK)
	{
		for( ; ( Local_u16Iter >= 0 ) && ( Local_u16Iter <  Local_u8AngleSteps ) ; Local_u16Iter += Local_u8StepDiff )
		{
#if( COIL_ACTIVE == DIO_u8LOW )
		Local_u8Output = (u8) ( OUTPUT_LOW_PATTERN & ~( BIT_MASK << ( Local_u16Iter % COIL_NUM ) ) );
#elif ( COIL_ACTIVE == DIO_u8HIGH )
		Local_u8Output = (u8) ( OUTPUT_HIGH_PATTERN  | ( BIT_MASK << ( Local_u16Iter % COIL_NUM ) ) ) ;
#endif

			Local_AenuErrorStates[0] = DIO_enuSetPinValue( COIL1_GRP, COIL1_PIN, (( Local_u8Output >> COIL1_SHIFT ) & BIT_MASK ));
			Local_AenuErrorStates[1] = DIO_enuSetPinValue( COIL2_GRP, COIL2_PIN, (( Local_u8Output >> COIL2_SHIFT ) & BIT_MASK ));
			Local_AenuErrorStates[2] = DIO_enuSetPinValue( COIL3_GRP, COIL3_PIN, (( Local_u8Output >> COIL3_SHIFT ) & BIT_MASK ));
			Local_AenuErrorStates[3] = DIO_enuSetPinValue( COIL4_GRP, COIL4_PIN, (( Local_u8Output >> COIL4_SHIFT ) & BIT_MASK ));
			for( Local_u8Iter = 0 ; (Local_u8Iter < COIL_NUM) && ( Local_AenuErrorStates[Local_u8Iter] == ES_OK ) ; Local_u8Iter++ );
			if( Local_u8Iter != COIL_NUM )
			{
				Local_u8ErrorFlag = 1 ;
				break;
			}
		}
	}

	if( !Local_u8ErrorFlag)
	{
		Local_enuErrorState = Timer_PollingDelay( TIMER2 , (u16)( 1000UL / (u32)Copy_u16StepSpeed ) );
	}

	return Local_enuErrorState ;
}
