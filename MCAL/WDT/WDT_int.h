/*
 * WDT_int.h
 *
 *  Created on: Sep 15, 2022
 *      Author: basse
 */

#ifndef MCAL_WDT_WDT_INT_H_
#define MCAL_WDT_WDT_INT_H_

/************************************/
/*	WatchDog Timer Prescale Options	*/
/************************************/
#define WDT_PRES_16K		20			/*	WDP2|WDP1|WDP0 = 000	*/
#define WDT_PRES_32K		21			/*	WDP2|WDP1|WDP0 = 001	*/
#define WDT_PRES_64K		22			/*	WDP2|WDP1|WDP0 = 010	*/
#define WDT_PRES_128K		23			/*	WDP2|WDP1|WDP0 = 011	*/
#define WDT_PRES_256K		24			/*	WDP2|WDP1|WDP0 = 100	*/
#define WDT_PRES_512K		25			/*	WDP2|WDP1|WDP0 = 101	*/
#define WDT_PRES_1024K		26			/*	WDP2|WDP1|WDP0 = 110	*/
#define WDT_PRES_2048K		27			/*	WDP2|WDP1|WDP0 = 111	*/


ES_t WDT_Init(void);

ES_t WDT_SetPrescale( u8 Copy_u8PrescaleValue );

ES_t WDT_Enable(void);

ES_t WDT_Disable(void);

void WDT_Reset(void);

#endif /* MCAL_WDT_WDT_INT_H_ */
