/*
 * SevSeg_config.h
 *
 *  Created on: Aug 9, 2022
 *      Author: Bassem El-Behaidy
 */

#ifndef SEVSEG_CONFIG_H_
#define SEVSEG_CONFIG_H_

#include "..\..\Libraries\stdTypes.h"

#define SEV_SEG_MAX_MODULES		2
#define MODULE_VALUE_PINS		4
#define MODULE_CONTROL_PINS		2
#define DIGIT_BASE				10


typedef struct
{
	u8 InputGrp;
	u8 InputPin;
}SS_Input_t;

typedef struct
{
	SS_Input_t Value_Conf[MODULE_VALUE_PINS];
	SS_Input_t DIP_Conf;
	SS_Input_t Control_Conf[MODULE_CONTROL_PINS];
}SSegModule_t;

/************************************************************/
/*			Seven Segment Module 1 Configuration			*/
/************************************************************/
#define SevSeg_u8MOD_1_A_GROUP		DIO_u8GROUP_A	//A PORT
#define SevSeg_u8MOD_1_A_PIN		DIO_u8PIN4		//A PIN

#define SevSeg_u8MOD_1_B_GROUP		DIO_u8GROUP_A	//B PORT
#define SevSeg_u8MOD_1_B_PIN		DIO_u8PIN5		//B PIN

#define SevSeg_u8MOD_1_C_GROUP		DIO_u8GROUP_A	//C PORT
#define SevSeg_u8MOD_1_C_PIN		DIO_u8PIN6		//C PIN

#define SevSeg_u8MOD_1_D_GROUP		DIO_u8GROUP_A	//D PORT
#define SevSeg_u8MOD_1_D_PIN		DIO_u8PIN7		//D PIN

#define SevSeg_u8MOD_1_DIP_GROUP	DIO_u8GROUP_A	//DIP PORT
#define SevSeg_u8MOD_1_DIP_PIN		DIO_u8PIN3		//DIP PIN

#define SevSeg_u8MOD_1_EN1_GROUP	DIO_u8GROUP_B	//EN1 PORT
#define SevSeg_u8MOD_1_EN1_PIN		DIO_u8PIN2		//EN1 PIN

#define SevSeg_u8MOD_1_EN2_GROUP	DIO_u8GROUP_B	//EN2 PORT
#define SevSeg_u8MOD_1_EN2_PIN		DIO_u8PIN1		//EN2 PIN
/************************************************************/



/************************************************************/
/*			Seven Segment Module 2 Configuration			*/
/************************************************************/
#define SevSeg_u8MOD_2_A_GROUP		DIO_u8GROUP_A	//A PORT
#define SevSeg_u8MOD_2_A_PIN		DIO_u8PIN4		//A PIN

#define SevSeg_u8MOD_2_B_GROUP		DIO_u8GROUP_A	//B PORT
#define SevSeg_u8MOD_2_B_PIN		DIO_u8PIN5		//B PIN

#define SevSeg_u8MOD_2_C_GROUP		DIO_u8GROUP_A	//C PORT
#define SevSeg_u8MOD_2_C_PIN		DIO_u8PIN6		//C PIN

#define SevSeg_u8MOD_2_D_GROUP		DIO_u8GROUP_A	//D PORT
#define SevSeg_u8MOD_2_D_PIN		DIO_u8PIN7		//D PIN

#define SevSeg_u8MOD_2_DIP_GROUP	DIO_u8GROUP_A	//DIP PORT
#define SevSeg_u8MOD_2_DIP_PIN		DIO_u8PIN3		//DIP PIN

#define SevSeg_u8MOD_2_EN1_GROUP	DIO_u8GROUP_B	//EN1 PORT
#define SevSeg_u8MOD_2_EN1_PIN		DIO_u8PIN2		//EN1 PIN

#define SevSeg_u8MOD_2_EN2_GROUP	DIO_u8GROUP_B	//EN2 PORT
#define SevSeg_u8MOD_2_EN2_PIN		DIO_u8PIN1		//EN2 PIN
/************************************************************/


#endif /* SEVSEG_CONFIG_H_ */
