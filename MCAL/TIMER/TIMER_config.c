/*
 * TIMER_config.c
 *
 *  Created on: Aug 26, 2022
 *      Author: basse
 */

#include "../../Libraries/stdTypes.h"
#include "../../Libraries/errorState.h"

#include "TIMER_config.h"
#include "TIMER_priv.h"

u8 Timers_u8MaxNum = TIMERS_MAX_NUM , Timer0Max = TIMER0_MAX , Timer2Max = TIMER2_MAX ;
//u16 Timer1Max = TIMER1_MAX;

Timer_t	Timers[TIMERS_MAX_NUM] = {
										{	TIMER0	, TC0_CS	, TC0_COM	, TC0_WGM 	, TC0_INT	},
#if MASK==0
										{	TIMER1A	, TC1_CS	, TC1A_COM	, TC1_WGM	, TC1A_INT	},
										{	TIMER1B	, TC1_CS	, TC1B_COM	, TC1_WGM	, TC1B_INT	},
#endif
										{	TIMER2	, TC2_CS	, TC0_COM	, TC0_WGM 	, TC2_INT	}
								};
