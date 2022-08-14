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


#if( LD_SET_MODE == ACTIVE_HIGH )
#define LD_MODE 	ACTIVE_HIGH
#define LD_u8ON		DIO_u8HIGH
#define LD_u8OFF	DIO_u8LOW
#elif( LD_SET_MODE == ACTIVE_LOW )
#define LD_MODE		ACTIVE_LOW
#define LD_u8ON		DIO_u8LOW
#define LD_u8OFF	DIO_u8HIGH
#endif


u8 LD_u8LD_MaxNum = LD_MAX_NUM ;
u8 LD_Mode = LD_MODE;
u8 LD_u8On = LD_u8ON;
u8 LD_u8Off = LD_u8OFF;


LD_t LD[LD_MAX_NUM] =	{
							{ LD_ZERO_u8GROUP , LD_ZERO_u8PIN },
							{ LD_ONE_u8GROUP  , LD_ONE_u8PIN  },
							{ LD_TWO_u8GROUP  , LD_TWO_u8PIN  }
						};
