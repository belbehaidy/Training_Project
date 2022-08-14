/*
 * Keypad_config.c
 *
 *  Created on: Aug 14, 2022
 *      Author: basse
 */

#include "..\..\Libraries\stdTypes.h"

#include "..\..\MCAL\DIO\DIO_int.h"

#include "Keypad_config.h"
#include "Keypad_priv.h"


u8 Kpad_u8MaxSideKeys = MAX_SIDE_KEYS;
u8 Kpad_u8RowNum = ROW_NUM;
u8 Kpad_u8ColNum = COL_NUM;
u8 Kpad_u8KeyValues[ROW_NUM * COL_NUM] = KEY_VALUES;

KP_Output_t KeyPadCols[COL_NUM]=	{
										{ C1_GRP , C1_PIN },				/*	COLUMN1	*/
										{ C2_GRP , C2_PIN },				/*	COLUMN2	*/
										{ C3_GRP , C3_PIN },				/*	COLUMN3	*/
										{ C4_GRP , C4_PIN } 				/*	COLUMN4	*/
									};

KP_Input_t KeyPadRows[ROW_NUM]=	{
										{ R1_GRP , R1_PIN , INPUT_STATE },	/*	ROW1 	*/
										{ R2_GRP , R2_PIN , INPUT_STATE },	/* 	ROW2 	*/
										{ R3_GRP , R3_PIN , INPUT_STATE },	/* 	ROW3	*/
										{ R4_GRP , R4_PIN , INPUT_STATE },	/* 	ROW4 	*/
									};
