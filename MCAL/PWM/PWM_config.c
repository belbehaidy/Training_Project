/*
 * PWM_config.c
 *
 *  Created on: Sep 5, 2022
 *      Author: basse
 */

#include "PWM_config.h"
#include "PWM_priv.h"

u8 PWM_u8MaxNum = PWM_MAX_NUM ;
u16 Timer1Max = TIMER1_MAX;

PWM_t PWMs[PWM_MAX_NUM] = {
#if PWM ==0
										{	TIMER0	, TC0_CS	, TC0_COM	, TC0_WGM 	, TC0_INT	},
#endif
										{	TIMER1A	, TC1_CS	, TC1A_COM	, TC1_WGM	, TC1A_INT	},
										{	TIMER1B	, TC1_CS	, TC1B_COM	, TC1_WGM	, TC1B_INT	},
#if PWM==0
										{	TIMER2	, TC2_CS	, TC0_COM	, TC0_WGM 	, TC2_INT	}
#endif
								};

