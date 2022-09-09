/*
 * PWM_config.h
 *
 *  Created on: Sep 5, 2022
 *      Author: basse
 */

#ifndef MCAL_PWM_PWM_CONFIG_H_
#define MCAL_PWM_PWM_CONFIG_H_

#include "PWM_priv.h"

typedef struct
{
	u8 TimerNum;
	u8 ClkSelect;
	u8 CompOutMode;
	u8 WaveGenMode;
	u8 InterruptMode;
}Timer_t;

/************************************/
/*	Timer/Counter0	Configuration	*/
/************************************/
#define TC0_CS			PRES_1
#define TC0_COM			COMP_NON_INVERTED
#define	TC0_WGM			WGM_PC_8_bit
#define TC0_INT			TC_POLLING
#define TIMER0_BOTTOM	0x00
#define TIMER0_MAX		0xFF


/************************************/
/*	Timer/Counter1	Configuration	*/
/************************************/
#define TC1_CS			PRES_1
#define	TC1_WGM			WGM_PC_8_bit
#define TIMER1_BOTTOM	0x0000
#define TIMER1_MAX		0xFFFF
/************************************/
/*	Timer/Counter1A	Configuration	*/
/************************************/
#define TC1A_COM		COMP_NON_INVERTED
#define TC1A_INT		TC_POLLING
/************************************/
/*	Timer/Counter1B	Configuration	*/
/************************************/
#define TC1B_COM		COMP_NORMAL
#define TC1B_INT		TC_POLLING

/************************************/
/*		Timer2	Configuration		*/
/************************************/
#define TC2_CS			PRES_1
#define TC2_COM			COMP_NON_INVERTED
#define	TC2_WGM			WGM_PC_8_bit
#define TC2_INT			TC_POLLING
#define TIMER2_BOTTOM	0x00
#define TIMER2_MAX		0xFF


#endif /* MCAL_PWM_PWM_CONFIG_H_ */
