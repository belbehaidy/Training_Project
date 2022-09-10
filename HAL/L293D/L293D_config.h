/*
 * L293D_config.h
 *
 *  Created on: Sep 2, 2022
 *      Author: basse
 */

#ifndef HAL_L293D_L293D_CONFIG_H_
#define HAL_L293D_L293D_CONFIG_H_

#include "..\..\Libraries\stdTypes.h"

#include "..\..\MCAL\DIO\DIO_int.h"
#include "..\..\MCAL\PWM\PWM_int.h"

typedef struct
{
	u8 MotorNum;
	u8 PWM ;
	Pin_t EN;
	Pin_t A;
	Pin_t B;
}Motor_t;

#define MOTORS_MAX_NUM	2
#define MOTOR1			105
#define MOTOR2			112


/****************************************/
/*			MOTOR1 Configuration		*/
/****************************************/
#define H_EN1_GRP		DIO_u8GROUP_D
#define H_EN1_PIN		DIO_u8PIN4

#define H_A1_GRP		DIO_u8GROUP_C
#define H_A1_PIN		DIO_u8PIN3

#define H_B1_GRP		DIO_u8GROUP_C
#define H_B1_PIN		DIO_u8PIN4

#define MOTOR1_PWM		TIMER1B

/****************************************/
/*			MOTOR1 Configuration		*/
/****************************************/
#define H_EN2_GRP		DIO_u8GROUP_D
#define H_EN2_PIN		DIO_u8PIN5

#define H_A2_GRP		DIO_u8GROUP_C
#define H_A2_PIN		DIO_u8PIN5

#define H_B2_GRP		DIO_u8GROUP_C
#define H_B2_PIN		DIO_u8PIN6

#define MOTOR2_PWM		TIMER1A

#endif /* HAL_L293D_L293D_CONFIG_H_ */
