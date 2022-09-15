/*
 * TIMER_config.h
 *
 *  Created on: Aug 26, 2022
 *      Author: basse
 */

#ifndef MCAL_TIMER_TIMER_CONFIG_H_
#define MCAL_TIMER_TIMER_CONFIG_H_

#include "TIMER_priv.h"

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
#define TC0_CS			PRES_1024
#define TC0_COM			COMP_NORMAL
#define	TC0_WGM			WGM_NORMAL_MODE
#define TC0_INT			TC_POLLING
#define TIMER0_BOTTOM	0x00
#define TIMER0_MAX		0xFF

#if MASK==0
/************************************/
/*	Timer/Counter1	Configuration	*/
/************************************/
#define TC1_CS			PRES_1024
#define	TC1_WGM			WGM_NORMAL_MODE
#define TIMER1_BOTTOM	0x0000
#define TIMER1_MAX		0xFFFF
/************************************/
/*	Timer/Counter1A	Configuration	*/
/************************************/
#define TC1A_COM		COMP_NORMAL
#define TC1A_INT		TC_POLLING
/************************************/
/*	Timer/Counter1B	Configuration	*/
/************************************/
#define TC1B_COM		COMP_NORMAL
#define TC1B_INT		TC_POLLING

#endif
/************************************/
/*		Timer2	Configuration		*/
/************************************/
#define TC2_CS			PRES_1024
#define TC2_COM			COMP_NORMAL
#define	TC2_WGM			WGM_NORMAL_MODE
#define TC2_INT			TC_POLLING
#define TIMER2_BOTTOM	0x00
#define TIMER2_MAX		0xFF


#endif /* MCAL_TIMER_TIMER_CONFIG_H_ */
