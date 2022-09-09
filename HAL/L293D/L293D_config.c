/*
 * L293D_config.c
 *
 *  Created on: Sep 5, 2022
 *      Author: basse
 */

#include "L293D_config.h"
#include "L293D_priv.h"


u8 Motors_u8MaxNum = MOTORS_MAX_NUM ;

Motor_t Motors[MOTORS_MAX_NUM] =	{
										{ MOTOR1 , MOTOR1_PWM , { H_EN1_GRP , H_EN1_PIN } , { H_A1_GRP , H_A1_GRP } , { H_B1_GRP , H_B1_GRP } },
										{ MOTOR2 , MOTOR2_PWM , { H_EN2_GRP , H_EN2_PIN } , { H_A2_GRP , H_A2_GRP } , { H_B2_GRP , H_B2_GRP } }
									};
