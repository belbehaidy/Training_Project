/*
 * ADC_priv.h
 *
 *  Created on: Aug 23, 2022
 *      Author: basse
 */

#ifndef MCAL_ADC_ADC_PRIV_H_
#define MCAL_ADC_ADC_PRIV_H_

#define	BIT_MASK		1

/****************************/
/*	Preset Scalar Selection */
/****************************/
#define PRES_0			120
#define PRES_2 			121
#define PRES_4			122
#define PRES_8			123
#define PRES_16			124
#define PRES_32			125
#define PRES_64			126
#define PRES_128		127

/************************************/
/*	Voltage Reference Selections	*/
/************************************/
#define AREF_REF		128
#define AVCC_REF		129
#define INTERNAL_REF	130

/****************************/
/*	 Adjust Selections		*/
/****************************/
#define RIGHT_ADJUST	131
#define LEFT_ADJUST		132

/****************************/
/*	 Adjust Selections		*/
/****************************/
#define CH_00			133
#define CH_01			134
#define CH_02			135
#define CH_03			136
#define CH_04			137
#define CH_05			138
#define CH_06			139
#define CH_07			140
#define CH_08			141
#define CH_09			142
#define CH_10			143
#define CH_11			144
#define CH_12			145
#define CH_13			146
#define CH_14			147
#define CH_15			148
#define CH_16			149
#define CH_17			150
#define CH_18			151
#define CH_19			152
#define CH_20			153
#define CH_21			154
#define CH_22			155
#define CH_23			156
#define CH_24			157
#define CH_25			158
#define CH_26			159
#define CH_27			160
#define CH_28			161
#define CH_29			162
#define CH_30			163
#define CH_31			164

/****************************/
/*	 Interrupt Modes		*/
/****************************/
#define ADC_INTERRUPT		165
#define ADC_POLLING			166

/****************************/
/*	 Auto Trigger Source	*/
/****************************/
#define FREE_RUNNING		167
#define ANALOG_COMPARATOR	168
#define EXT_INT0			169
#define TIMER0_CMP_MATCH	170
#define TIMER0_OVERFLOW		171
#define TIMER_CMP_MATCH_B	172
#define TIMER1_OVERFLOW		173
#define TIMER1_CAPT_EVENT	174

/****************************/
/*		Trigger Mode		*/
/****************************/
#define SINGLE_TRIGGER		175
#define AUTO_TRIGGER		176

/****************************************************/
/*	 		ADC Related Registers & Bits			*/
/****************************************************/
#define ADMUX					*((volatile u8 *)0x27)
#define ADC_CH_SEL_BITS					0
#define ADC_CH_SEL_BITS_MASK			0x1F
#define	ADLAR_BIT						5
#define ADC_REF_SEL_BITS				6
#define ADC_REF_SEL_BITS_MASK			0xC0


#define ADCSRA					*((volatile u8 *)0x26)
#define ADC_PRE_SCALAR_BITS				0
#define ADC_PRE_SCALAR_BITS_MASK		0x07
#define ADC_INT_ENABLE_BIT				3
#define ADC_INT_FLAG_BIT				4
#define ADC_AUTO_TRIGGER_EN_BIT			5
#define ADC_START_CONVERSION_BIT		6
#define ADC_ENABLE_BIT					7

#define ADCL					*((volatile u8 *)0x24)
#define ADCH					*((volatile u8 *)0x25)

#define SFIOR					*((volatile u8 *)0x50)
#define ADC_TRIGGER_SEL_BITS			0
#define ADC_TRIGGER_SEL_BITS_MASK		0x07



#endif /* MCAL_ADC_ADC_PRIV_H_ */
