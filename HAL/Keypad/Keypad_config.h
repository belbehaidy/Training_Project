/*
 * Keypad_config.h
 *
 *  Created on: Aug 13, 2022
 *      Author: Ahmed El-Gaafrawy
 */

#ifndef HAL_KEYPAD_KEYPAD_CONFIG_H_
#define HAL_KEYPAD_KEYPAD_CONFIG_H_

#include "..\..\Libraries\stdTypes.h"

typedef struct
{
	u8 OutputGrp;
	u8 OutputPin;
}KP_Output_t;

typedef struct
{
	u8 InputGrp;
	u8 InputPin;
	u8 InputState;
}KP_Input_t;

#define INPUT_STATE			DIO_u8PULL_UP

#define MAX_SIDE_KEYS		4

// max must not exceed MAX_SIDE_kEYS
#define ROW_NUM				4
#define COL_NUM				4

//columns pins

#define C1_GRP				DIO_u8GROUP_C
#define C1_PIN				DIO_u8PIN3

#define C2_GRP				DIO_u8GROUP_C
#define C2_PIN				DIO_u8PIN4

#define C3_GRP				DIO_u8GROUP_C
#define C3_PIN				DIO_u8PIN5

#define C4_GRP				DIO_u8GROUP_C
#define C4_PIN				DIO_u8PIN6



//rows pins

#define R1_GRP				DIO_u8GROUP_D
#define R1_PIN				DIO_u8PIN0

#define R2_GRP				DIO_u8GROUP_D
#define R2_PIN				DIO_u8PIN1

#define R3_GRP				DIO_u8GROUP_D
#define R3_PIN				DIO_u8PIN4

#define R4_GRP				DIO_u8GROUP_D
#define R4_PIN				DIO_u8PIN5

#define KEY_VALUES			{ 						\
								'7','8','9','/',	\
								'4','5','6','x',	\
								'1','2','3','-',	\
								'C','0','=','+' 	\
							}

#endif /* HAL_KEYPAD_KEYPAD_CONFIG_H_ */

/*
 * #define KEY_VALUES
							{ 	\
								{'7','8','9','/'},\
								{'4','5','6','x'},\
								{'1','2','3','-'},\
								{'C','0','=','+'} \
							}

							{	\
								{'1','2','3','A'},\
								{'4','5','6','B'},\
								{'7','8','9','C'},\
								{'*','0','#','D'} \
							}


 */
