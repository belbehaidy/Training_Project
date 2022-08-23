/*
 * HVAC_config.h


 *
 *  Created on: Aug 18, 2022
 *      Author: Bassem EL-Behaidy
 */

#ifndef _HVAC_CONFIG_H_
#define _HVAC_CONFIG_H_


/****************************************************/
/*			Switches Configuration					*/
/****************************************************/
#define SWITCH_BOUNCE_DELAY			10
#define SWITCH_UNPRESSED			111

#define POWER_SWITCH				SW_TWO
#define POWER_SWITCH_PRESSED		DIO_u8HIGH

#define INCREMENT_SWITCH			SW_ZERO
#define INCREMENT_SWITCH_PRESSED	DIO_u8HIGH

#define DECREMENT_SWITCH			SW_ONE
#define DECREMENT_SWITCH_PRESSED	DIO_u8HIGH


/****************************************************/
/*				LEDs Configuration					*/
/****************************************************/
#define LD_ON						DIO_u8HIGH
#define LD_OFF						DIO_u8LOW
#define HOT_LD						0
#define NORM_LD						1
#define COLD_LD						2

/****************************************************/
/*			Temperature Configuration				*/
/****************************************************/
#define TEMP_COEFFICENT				10
#define TEMP_TOLERANCE				2
#define ADC_GAIN_FACTOR				1


#endif /* HVAC_CONFIG_H_ */
