/*
 * Sw_One_config.h
 *
 *  Created on: Aug 6, 2022
 *      Author: Ahmed El-Gaafrawy
 */

#ifndef SWITCH_CONFIG_H_
#define SWITCH_CONFIG_H_

#include "..\..\Libraries\stdTypes.h"


#define SWITCHES_MAX_NUM	3


typedef struct
{
	u8 sw_Grp;
	u8 sw_Pin;
	u8 sw_State;
	u8 sw_Press;
}Switch_t;

#define SW_ZERO_u8GROUP		DIO_u8GROUP_B
#define SW_ZERO_u8PIN		DIO_u8PIN0
#define SW_ZERO_u8STATE		DIO_u8FLOAT

#define SW_ONE_u8GROUP		DIO_u8GROUP_D
#define SW_ONE_u8PIN		DIO_u8PIN6
#define SW_ONE_u8STATE		DIO_u8FLOAT

#define SW_TWO_u8GROUP		DIO_u8GROUP_D
#define SW_TWO_u8PIN		DIO_u8PIN7
#define SW_TWO_u8STATE		DIO_u8FLOAT

#endif /* SWITCH_CONFIG_H_ */
