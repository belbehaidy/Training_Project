/*
 * EXTI_config.c
 *
 *  Created on: Aug 19, 2022
 *      Author: Bassem EL-Behaidy
 */
#include "..\..\Libraries\stdTypes.h"
#include "..\..\Libraries\errorState.h"

#include "EXTI_config.h"
#include "EXTI_priv.h"

u8 EXTI_u8EXTI_Num	=	EXTI_NUM;

EXTI_t EXTI_AstrEXTI_Config[EXTI_NUM] =
{
		{RISING_EDGE	, INTERRUPT	, INT0	},
		{RISING_EDGE	, POLLING	, INT1	},
		{RISING_EDGE	, POLLING	, INT2	}
};
