/*
 * GIE_prog.c
 *
 *  Created on: Aug 19, 2022
 *      Author: basse
 */

#include "..\..\Libraries\stdTypes.h"
#include "..\..\Libraries\errorState.h"

ES_t GIE_enuInit(void)
{
	asm("CLI");

	return ES_OK;
}

ES_t GIE_enuEnable(void)
{

	asm("SEI");
	return ES_OK;
}

ES_t GIE_enuDisable(void)
{
	asm("CLI");
	return ES_OK;
}

