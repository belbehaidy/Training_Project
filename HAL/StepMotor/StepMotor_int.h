/*
 * StepMotor_int.h
 *
 *  Created on: Sep 10, 2022
 *      Author: basse
 */

#ifndef HAL_STEPMOTOR_STEPMOTOR_INT_H_
#define HAL_STEPMOTOR_STEPMOTOR_INT_H_

#define CLOCKWISE			65
#define COUNTER_CLOCKWISE	95


ES_t StepMotor_enuInit( void );

ES_t StepMotor_enuFullCycle( u8 Copy_u8Direction , u16 Copy_u16StepSpeed);

ES_t StepMotor_enuGoToAngle( u8 Copy_u8Direction , u8 Copy_u8Angle , u16 Copy_u16StepSpeed ); // Angle in Degrees , Step Speed in Hz

#endif /* HAL_STEPMOTOR_STEPMOTOR_INT_H_ */
