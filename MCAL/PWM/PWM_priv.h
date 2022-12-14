/*
 * PWM_priv.h
 *
 *  Created on: Sep 5, 2022
 *      Author: basse
 */

#ifndef MCAL_PWM_PWM_PRIV_H_
#define MCAL_PWM_PWM_PRIV_H_

#include "..\..\Libraries\stdTypes.h"

#define PWM		1

typedef struct
{
	u8 InterruptName ;
	void *ptrVar;
	void (*ptrFun)(void *);
}PWM_Int_Pointers_t;



#define PWM_MAX_NUM				4
#define PWM_INTERRUPTS			8
#define CPU_CLOCK					16000000UL
#define EXT_CLOCK					0xFFFFFFFF

#define WGM_MODE_01			0x01
#define WGM_MODE_03			0x03


#define BIT0_MASK			1
#define BIT1_MASK			2
#define TWO_BITS_MASK		3


/****************************************/
/*			Timers/Counters				*/
/****************************************/
//#define	TIMER0				200			/*	Timer/Counter0	*/
//#define	TIMER1				210			/*	Timer/Counter1	*/
//#define	TIMER2				203			/*		Timer2		*/
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
#define COMP_NORMAL			20		/* TIMERS ONLY	*/
//#define COMP_TOG_ON_MATCH	21		/* TIMERS ONLY	*/
#define COMP_NON_INVERTED	22
#define COMP_INVERTED		23

/****************************************/
/*	 Waveform Generation Mode Options	*/
/****************************************/
#define WGM_MODE_00			100		/* TIMERS ONLY	*/
#define WGM_PC_8_bit		101
#define WGM_PC_9_bit		102
#define WGM_PC_10_bit		103
#define WGM_MODE_04			104		/* TIMERS ONLY	*/
#define WGM_FAST_8_bit		105
#define WGM_FAST_9_bit		106
#define WGM_FAST_10_bit		107
#define WGM_PFC_ICR1		108
#define WGM_PFC_OCR1A		109
#define WGM_PC_ICR1			110
#define WGM_PC_OCR1A		111
#define WGM_MODE_12			112		/* TIMERS ONLY	*/
#define WGM_MODE_13			113		/*	Reserved	*/
#define WGM_FAST_ICR1		114
#define WGM_FAST_OCR1A		115


/****************************************/
/*		 Timer Interrupt Options		*/
/****************************************/
#define TC_POLLING			40
#define TC_OVERFLOW_INT		41
#define TC_OUT_COMP_INT		42
#define TC1_INPUT_CAPT_INT	43

/********************************************************************************************/
/*								 Timer Interrupt Names										*/
/********************************************************************************************/
#if PWM==0
#define TOIE0				60			/* Timer/Counter0 Overflow Interrupt				*/
#define OCIE0				61			/* Timer/Counter0 Output Compare Match Interrupt	*/
#define TOIE2				66			/* Timer/Counter2 Overflow Interrupt				*/
#define OCIE2				67			/* Timer/Counter2 Output Compare Match Interrupt 	*/
#endif
#define TOIE1				62			/* Timer/Counter1 Overflow Interrupt				*/
#define OCIE1B				63			/* Timer/Counter1 Output Compare Match B Interrupt	*/
#define OCIE1A				64			/* Timer/Counter1 Output Compare Match A Interrupt	*/
#define TICIE1				65			/* Timer/Counter1 I/P Capture Interrupt				*/

#if PWM == 0
/************************************************************************************************************/
/* 										TIMER/Counter0 Registers											*/
/************************************************************************************************************/
#define TCCR0_ADDRESS		0x33
#define TCCR0				*((volatile u8*)0x53)		/*		Timer/Counter0 Control Register				*/
#define CS00				0							/*******Timer/Counter0 Clock Select Bits*************/
#define CS01				1							/*				CS02:CS01:CS00						*/
#define CS02				2							/**********	 Bits  2:  1 :  0		*****************/
#define WGM01_BIT			3							/*	Timer/Counter0 Waveform Generation Mode (CTC)	*/
#define COM00				4							/********Timer/Counter0 Compare Output Mode Bits*****/
#define COM01				5							/********	COM01:COM00				Bits 5:4	*****/
#define WGM00_BIT			6							/*	Timer/Counter0 Waveform Generation Mode (PWM)	*/
#define FOC0_BIT			7							/*		Timer/Counter0 Force Output Compare			*/

#define CLK0_SEL_BITS_MASK					0x07
#define CLK0_SEL_BITS						0

#define COMP0_MAT_OUT_MODE_BITS_MASK		0x30
#define COMP0_MAT_OUT_MODE_BITS				4

#define TCNT0_ADDRESS		0x32
#define TCNT0				*((volatile u8*)0x52)		/*		Timer/Counter0 Counter Register				*/
#define OCR0_ADDRESS		0x3C
#define OCR0				*((volatile u8*)0x5C)		/*		Timer/Counter0 Output Compare Register		*/

#endif

/************************************************************************************************************/
/* 										TIMER/Counter1 Registers											*/
/************************************************************************************************************/
#define TCCR1A_ADDRESS		0x2F
#define TCCR1A				*((volatile u8*)0x4F)		/*	Timer/Counter1 Control Register A				*/
#define WGM10				0							/*	Timer/Counter1 Waveform Generation Mode (WGM10)	*/
#define WGM11				1							/*	Timer/Counter1 Waveform Generation Mode (WGM11)	*/
#define FOC1B_BIT			2							/*	Timer/Counter1 Force Output Compare	B			*/
#define FOC1A_BIT			3							/*	Timer/Counter1 Force Output Compare A			*/
#define COM1B0				4							/*******Timer/Counter1 B Compare Output Mode Bits****/
#define COM1B1				5							/***********COM1B1:COM1B0			Bits 5:4*********/
#define COM1A0				6							/*******Timer/Counter1 A Compare Output Mode Bits****/
#define COM1A1				7							/***********COM1A1:COM1A0			Bits 7:6*********/

#define WGM1A_SEL_BITS_MASK					0x03
#define WGM1A_SEL_BITS						0

#define COM1B_SEL_BITS_MASK					0x30
#define COM1B_SEL_BITS						4

#define COM1A_SEL_BITS_MASK					0xC0
#define COM1A_SEL_BITS						6
					/***********************************************************/
#define TCCR1B_ADDRESS		0x2E
#define TCCR1B				*((volatile u8*)0x4E)		/*	Timer/Counter1 Control Register B				*/
#define CS10				0							/*******Timer/Counter1 Clock Select Bits*************/
#define CS11				1							/*				CS12:CS11:CS10						*/
#define CS12				2							/**********	 Bits  2:  1 :  0		*****************/
#define WGM12_BIT			3							/*	Timer/Counter1 Waveform Generation Mode (WGM12)	*/
#define WGM13_BIT			4							/*	Timer/Counter1 Waveform Generation Mode (WGM13) */
/*	NOT USED	NOT USED	NOT USED	NOT USED	NOT USED	NOT USED	NOT USED	NOT USED	NOT USED*/
#define ICES1				6							/* 		Timer/Counter1 I/P Capture Edge Select	 	*/		//1 for Positive/Rising Edge & 0 for Negative/Falling Edge
#define ICNC1				7							/* Timer/Counter1 I/P Capture Noise Canceler Enable */

#define CLK1_SEL_BITS_MASK					0x07
#define CLK1_SEL_BITS						0
#define WGM1B_SEL_BITS_MASK					0x18
#define WGM1B_SEL_BITS						3

					/***********************************************************/
#define TCNT1H_ADDRESS		0x2D
#define TCNT1H				*((volatile u8*)0x4D)		/*	Timer/Counter1 Counter Register High Byte		*/
#define TCNT1L_ADDRESS		0x2C
#define TCNT1L				*((volatile u8*)0x4C)		/*	Timer/Counter1 Counter Register Low Byte		*/
#define OCR1AH_ADDRESS		0x2B
#define OCR1AH				*((volatile u8*)0x4B)		/*	Timer/Counter1 O/P Compare Register A High Byte	*/
#define OCR1AL_ADDRESS		0x2A
#define OCR1AL				*((volatile u8*)0x4A)		/*	Timer/Counter1 O/P Compare Register A Low Byte	*/
#define OCR1BH_ADDRESS		0x29
#define OCR1BH				*((volatile u8*)0x49)		/*	Timer/Counter1 O/P Compare Register B High Byte	*/
#define OCR1BL_ADDRESS		0x28
#define OCR1BL				*((volatile u8*)0x48)		/*	Timer/Counter1 O/P Compare Register B Low Byte	*/
#define ICR1H_ADDRESS		0x27
#define ICR1H				*((volatile u8*)0x47)		/*	Timer/Counter1 I/P Capture Register High Byte	*/
#define ICR1L_ADDRESS		0x26
#define ICR1L				*((volatile u8*)0x46)		/*	Timer/Counter1 I/P Capture Register Low Byte	*/

#if PWM ==0

/************************************************************************************************************/
/* 										TIMER/Counter2 Registers											*/
/************************************************************************************************************/
#define TCCR2_ADDRESS		0x25
#define TCCR2				*((volatile u8*)0x45)		/*				Timer2 Control Register				*/
#define CS20								0			/*************Timer2 Clock Select Bits***************/
#define CS21								1			/*					CS22:CS21:CS20					*/
#define CS22								2			/************	 Bits  2:  1 :  0		*************/
#define WGM21_BIT							3			/*		Timer2 Waveform Generation Mode (CTC)		*/
#define COM20								4			/***********Timer2 Compare Output Mode Bits**********/
#define COM21								5			/***********COM21:COM20				Bits 5:4*********/
#define WGM20_BIT							6			/*		Timer2 Waveform Generation Mode (PWM)		*/
#define FOC2_BIT							7			/*		Timer2 Force Output Compare					*/

#define CLK2_SEL_BITS_MASK					0x07
#define CLK2_SEL_BITS						0

#define COMP2_MAT_OUT_MODE_BITS_MASK		0x30
#define COMP2_MAT_OUT_MODE_BITS				4

#define TCNT2_ADDRESS		0x24
#define TCNT2				*((volatile u8*)0x44)		/*				Timer2 Counter Register				*/
#define OCR2_ADDRESS		0x23
#define OCR2				*((volatile u8*)0x43)		/*			Timer2 Output Compare Register			*/

/************************************************************************************************************/
/* 									Asynchronous Status Register											*/
/************************************************************************************************************/
#define ASSR				*((volatile u8*)0x42)		/*			Asynchronous Status Register			*/
#define AS2									3			/*			Asynchronous Timer2	Enable Bit			*/
#define TCN2UB								2			/*				Timer2	Update Busy Flag			*/
#define OCR2UB								1			/*	Timer2 Output Compare Register Update Busy Flag	*/
#define TCR2UB								0			/*		Timer2 Control Register Update Busy Flag	*/

#endif


/********************************************************************************************************************/
/*										TIMER/Counter Interrupt Mask  Register										*/
/********************************************************************************************************************/
#define TIMSK_ADDRESS		0x39
#define TIMSK				*((volatile u8*)0x59)		/* Timer/Counter Interrupt Mask Register					*/
#if PWM==0
#define TOIE0_BIT							0			/* Timer/Counter0 Overflow Interrupt Enable bit				*/
#define OCIE0_BIT							1			/* Timer/Counter0 Output Compare Match Interrupt Enable bit	*/
#endif
#define TOIE1_BIT							2			/* Timer/Counter1 Overflow Interrupt Enable bit				*/
#define OCIE1B_BIT							3			/* Timer/Counter1B Output Compare Match Interrupt Enable bit*/
#define OCIE1A_BIT							4			/* Timer/Counter1A Output Compare Match Interrupt Enable bit*/
#define TICIE1_BIT							5			/* Timer/Counter1 I/P Capture Interrupt Enable bit			*/
#if PWM==0
#define TOIE2_BIT							6			/* Timer/Counter2 Overflow Interrupt Enable bit				*/
#define OCIE2_BIT							7			/* Timer/Counter2 Output Compare Match Interrupt Enable bit	*/
#endif
//#define TC0_INT_EN_MASK						0x03
#define TC1_INT_EN_MASK						0x3C
//#define TC2_INT_EN_MASK						0xC0

/****************************************************************************************************/
/*								TIMER/Counter Interrupt Flag  Register								*/
/****************************************************************************************************/
#define TIFR_ADDRESS		0x38
#define TIFR				*((volatile u8*)0x58)		/* TIMER/Counter Interrupt Flag  Register	*/
#if PWM==0
#define TOV0_BIT							0			/* Timer/Counter0 Overflow Flag bit			*/
#define OCF0_BIT							1			/* Timer/Counter0 Output Compare Flag bit	*/
#endif
#define TOV1_BIT							2			/* Timer/Counter1 Overflow Flag bit			*/
#define OCF1B_BIT							3			/* Timer/Counter1 Output Compare 1B Flag bit*/
#define OCF1A_BIT							4			/* Timer/Counter1 Output Compare 1A Flag bit*/
#define ICF1_BIT							5			/* Timer/Counter1 I/P Capture Flag bit		*/
#if PWM==0
#define TOV2_BIT							6			/* Timer/Counter2 Overflow Flag bit			*/
#define OCF2_BIT							7			/* Timer/Counter2 Output Compare Flag bit	*/
#endif
/****************************************************************************************************/
/*								Special Function IO  Register										*/
/****************************************************************************************************/
#define SFIOR_ADDRESS		0x30
#define SFIOR				*((volatile u8*)0x50)		/*	Special Function IO  Register			*/
#define PSR10_BIT							0			/* Prescaler RESET Bit for Timer/Counter0&1	*/
//#define PSR2_BIT							1			/* Prescaler RESET Bit for Timer/Counter2	*/

#define SREG_ADDRESS		0x3F
#define SREG				*((volatile u8*)0x5F)		/*			AVR Status Register				*/


static u32 PWM_u32TimerClock( u8 Copy_u8TimerNum ,u8 Copy_u8ClkSelectNum ,u8 *Copy_pu8ClkSelectPrescalar );


#endif /* MCAL_PWM_PWM_PRIV_H_ */
