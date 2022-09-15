/*
 * WDT_priv.h
 *
 *  Created on: Sep 15, 2022
 *      Author: basse
 */

#ifndef MCAL_WDT_WDT_PRIV_H_
#define MCAL_WDT_WDT_PRIV_H_


#define BIT_MASK			0x01
#define THREE_BITS_MASK		0x07

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


/************************************/
/*	WatchDog Timer Control Register	*/
/************************************/
#define WDTCR			*((u8*)0x41)
#define WDTOE				4			/*	Watch Dog Turn OFF Enable	*/
#define WDE					3			/*	Watch Dog Enable			*/
#define WDP2				2			/***Watch Dog Prescale Select****/
#define WDP1				1			/*		Bits: WDP2|WDP1|WDP0	*/	//	Check Options above
#define WDP0				0			/********************************/

#endif /* MCAL_WDT_WDT_PRIV_H_ */
