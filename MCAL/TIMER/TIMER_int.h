/*
 * TIMER_int.h
 *
 *  Created on: Aug 26, 2022
 *      Author: basse
 */

#ifndef MCAL_TIMER_TIMER_INT_H_
#define MCAL_TIMER_TIMER_INT_H_

#define CPU_CLOCK					16000000UL
#define EXT_CLOCK					0xFFFFFFFF


/****************************************/
/*			Timers/Counters				*/
/****************************************/
#define	TIMER0				200			/*	Timer/Counter0	*/
#define	TIMER1				210			/*	Timer/Counter1	*/
#define	TIMER2				203			/*	Timer/Counter2	*/
#define TIMER1A				201			/*	Counter1A		*/
#define TIMER1B				202			/*	Counter1B		*/

/****************************************/
/* 		Clock selection Options			*/
/****************************************/
#define NO_CLK				10
#define PRES_1				11
#define PRES_8				12
#define PRES_32				13			//	Timer2 ONLY
#define PRES_64				14
#define PRES_128			15			//	Timer2 ONLY
#define PRES_256			16
#define PRES_1024			17
#define EXT_CLK_FALL		18			//	Timer0 & Timer1 ONLY
#define EXT_CLK_RISE		19			//	Timer0 & Timer1 ONLY

/****************************************/
/*		Compare Output Mode Options		*/
/****************************************/
#define COMP_NORMAL			20
#define COMP_TOG_ON_MATCH	21
#define COMP_NON_INVERTED	22
#define COMP_INVERTED		23

/********************************************************************************************/
/*							 OCR DDR PINS ACCESS INFORMATION								*/
/********************************************************************************************/
#define OC0_DDR				*((volatile u8*)0x37)		/*	OC0 DDR register Address		*/
#define OC0_PIN				3							/*	OC0 bit number on DDR Register	*/

#define OC1B_DDR			*((volatile u8*)0x31)		/*	OC1B DDR register Address		*/
#define OC1B_PIN			4							/*	OC1B bit number on DDR Register	*/

#define OC1A_DDR			*((volatile u8*)0x31)		/*	OC1A DDR register Address		*/
#define OC1A_PIN			5							/*	OC1A bit number on DDR Register	*/

#define OC2_DDR				*((volatile u8*)0x31)		/*	OC2 DDR register Address		*/
#define OC2_PIN				7							/*	OC2 bit number on DDR Register	*/

/****************************************/
/*	 Waveform Generation Mode Options	*/
/****************************************/
#define WGM_NORMAL_MODE		30
#define WGM_CTC_MODE		32
#define WGM_CTC_IRC1_MODE	33

/****************************************/
/*		 Timer Interrupt Options		*/
/****************************************/
#define TC_POLLING			40
#define TC_OVERFLOW_INT		41
#define TC_OUT_COMP_INT		42
#define TC1_A_OUT_COMP_INT	43
#define TC1_B_OUT_COMP_INT	44
#define TC1_INPUT_CAPT_INT	45

/********************************************************************************************/
/*								 Timer Interrupt Names										*/
/********************************************************************************************/
#define TOIE0				60			/* Timer/Counter0 Overflow Interrupt				*/
#define OCIE0				61			/* Timer/Counter0 Output Compare Match Interrupt	*/
#define TOIE1				62			/* Timer/Counter1 Overflow Interrupt				*/
#define OCIE1B				63			/* Timer/Counter1 Output Compare Match B Interrupt	*/
#define OCIE1A				64			/* Timer/Counter1 Output Compare Match A Interrupt	*/
#define TICIE1				65			/* Timer/Counter1 I/P Capture Interrupt				*/
#define TOIE2				66			/* Timer/Counter2 Overflow Interrupt				*/
#define OCIE2				67			/* Timer/Counter2 Output Compare Match Interrupt 	*/



void Timer_enuInit( void );

ES_t Timer_enuGetClock( u8 Copy_u8TimerNum , u32 *Copy_pu32TimerClk);

ES_t Timer_enuSetClkPrescaler( u8 Copy_u8TimerNum , u8 Copy_u8PrescalerValue );

ES_t Timer_enuGetClkSelect( u8 Copy_u8TimerNum , u16 *Copy_pu8TimerClkSelect );

ES_t Timer_enuSetOCn_Mode( u8 Copy_u8TimerNum , u8 Copy_u8COM_Mode );

ES_t Timer_enuGetOCn_Mode( u8 Copy_u8TimerNum , u8 *Copy_pu8TimerCOM_Mode);

ES_t Timer_enuSetTimer_Mode( u8 Copy_u8TimerNum , u8 Copy_u8WGM_Mode );

ES_t Timer_enuGetTimer_Mode( u8 Copy_u8TimerNum , u8 *Copy_pu8TimerWGM_Mode);

ES_t Timer_enuReset( u8 Copy_u8TimerNum );

ES_t Timer_enuPreLoad( u8 Copy_u8TimerNum , u16 Copy_u16PreLoad);

ES_t Timer_enuReadCounterValue( u8 Copy_u8TimerNum , void *Copy_pCounterValue );

ES_t Timer_enuSetOCRnValue( u8 Copy_u8TimerNum , u16 Copy_u16OCRnValue );

ES_t Timer_enuReadOCRnValue( u8 Copy_u8TimerNum , void *Copy_pCounterValue );

ES_t Timer_enuSetICR1Value( u16 Copy_u16ICR1Value );

ES_t Timer_enuReadICR1Value( u16 *Copy_pu16ICR1Value );

ES_t Timer_enuInterruptEnable( u8 Copy_u8TimerIntName);

ES_t Timer_enuInterruptDisable( u8 Copy_u8TimerIntName);

bool Timer_IsInterruptEnabled( u8 Copy_u8TimerIntName );

ES_t Timer_enuCallBack( u8 Copy_u8TimerIntName , void (*Copy_pAppFun)(void*) , void *Copy_pAppVar);





ES_t Timer_PollingDelay(u8 Copy_u8TimerNum ,u16 Copy_u16Delay_ms ); // POLLING Delay				in main///////////////////

ES_t Timer_ISR_Delay(u8 Copy_u8TimerNum ,u16 Copy_u16Delay_ms , void (*Copy_pFun)(void*) ); // ISR Delay				in main///////////////////////

#endif /* MCAL_TIMER_TIMER_INT_H_ */
