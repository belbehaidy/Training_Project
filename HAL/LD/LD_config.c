/*
 * LD_config.c
 *
 *  Created on: Aug 14, 2022
 *      Author: basse
 */

#include "..\..\Libraries\stdTypes.h"

#include "..\..\MCAL\DIO\DIO_int.h"

#include "LD_config.h"
#include "LD_priv.h"


u8 LD_u8LD_MaxNum = LD_MAX_NUM ;


LD_t LD[LD_MAX_NUM] =	{
							{ LD_ZERO_u8GROUP , LD_ZERO_u8PIN },
							{ LD_ONE_u8GROUP  , LD_ONE_u8PIN  },
							{ LD_TWO_u8GROUP  , LD_TWO_u8PIN  }
						};
