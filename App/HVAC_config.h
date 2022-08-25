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
#define POWER_OFF_DELAY				500

#define POWER_SWITCH				SW_TWO
#define POWER_SW_PRESSED			DIO_u8HIGH

#define INCREMENT_SWITCH			SW_ONE
#define INC_SW_PRESSED				DIO_u8HIGH

#define DECREMENT_SWITCH			SW_ZERO
#define DEC_SW_PRESSED				DIO_u8HIGH
/****************************************************/
/*				LEDs Configuration					*/
/****************************************************/
#define LD_ON						DIO_u8HIGH
#define LD_OFF						DIO_u8LOW
#define HOT_LD						LD_ZERO
#define NORM_LD						LD_ONE
#define COLD_LD						LD_TWO

/****************************************************/
/*			Temperature Configuration				*/
/****************************************************/
#define TEMP_COEFFICENT				2
#define TEMP_TOLERANCE				2
#define ADC_GAIN_FACTOR				10
#define TEMP_AVG_READINGS			5

#endif /* HVAC_CONFIG_H_ */
