/*
 * StepMotor_config.h
 *
 *  Created on: Sep 10, 2022
 *      Author: basse
 */

#ifndef HAL_STEPMOTOR_STEPMOTOR_CONFIG_H_
#define HAL_STEPMOTOR_STEPMOTOR_CONFIG_H_


#define COIL_NUM			4
#define COIL_ACTIVE			DIO_u8LOW
#define COIL_IDLE			DIO_u8HIGH

#define STRIDE_ANGLE		((f64)5.625/(f64)64)
#define RESOLUTION			0.0F


/****************************************/
/*	Stepper Motor Coils Configuration	*/
/****************************************/
#define COIL1_GRP		DIO_u8GROUP_B
#define COIL1_PIN		DIO_u8PIN4

#define COIL2_GRP		DIO_u8GROUP_B
#define COIL2_PIN		DIO_u8PIN5

#define COIL3_GRP		DIO_u8GROUP_B
#define COIL3_PIN		DIO_u8PIN6

#define COIL4_GRP		DIO_u8GROUP_B
#define COIL4_PIN		DIO_u8PIN7

#endif /* HAL_STEPMOTOR_STEPMOTOR_CONFIG_H_ */
