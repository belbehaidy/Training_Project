/*
 * UART_priv.h
 *
 *  Created on: Sep 9, 2022
 *      Author: basse
 */

#ifndef MCAL_UART_UART_PRIV_H_
#define MCAL_UART_UART_PRIV_H_

typedef struct
{
	u8 InterruptName;
	void *ptrVar;
	void (*ptrFun)(void *);
}UART_Int_t;

#define CPU_CLOCK			16000000UL

#define UART_INTERRUPTS		3
#define BIT_MASK			0x01
#define TWO_BITS_MASK		0x03
#define NIBBLE_MASK			0x0F
#define BYTE_SHIFT			8

/********************************/
/*	UBRR VALUE CALCULATOR MACRO	*/
/********************************/
#define UBRR_VALUE(BAUD_RATE)		(u16)( ( ( CPU_CLOCK * OPERATING_MODE) / ( 16 * BAUD_RATE ) ) - 1 )

/********************************/
/*	OPERATING MODE  OPTIONs		*/
/********************************/
#define NORMAL_SPEED		1
#define DOUBLE_SPEED		2
#define MASTER_SPEED		8

/********************************/
/*			UART MODEs			*/
/********************************/
#define TRANSMITTER			10
#define RECEIVER			11
#define TRANSCEIVER			12

/********************************/
/*		DATA BITs OPTIONs		*/
/********************************/
#define	FIVE_BITS			20
#define SIX_BITS			21
#define SEVEN_BITS			22
#define EIGHT_BITS			23
#define NINE_BITS			24

/********************************/
/*		PARITY BITs OPTIONs		*/
/********************************/
#define NO_PARITY			25
#define ODD_PARITY			26
#define EVEN_PARITY			27

/********************************/
/*		STOP BIT OPTIONs		*/
/********************************/
#define ONE_STOP_BIT		30
#define TWO_STOP_BITS		31

/********************************/
/*		SYNC MODE OPTIONS		*/
/********************************/
#define ASYNCHRONOUS		40
#define SYNCHRONOUS			41

/********************************/
/*		USART INTERRUPTS		*/
/********************************/
#define UDRIE				50
#define TXCIE				53
#define RXCIE				56

/********************************/
/*	CLOCK POLARITY  OPTIONs		*/
/********************************/
#define TX_RISING_EDGE		60
#define RX_RISING_EDGE		61

/********************************************/
/*	MULTI PROCESSOR COMMUNICATION  OPTIONs	*/
/********************************************/
#define MPCM_OFF			70
#define MPCM_ON				71

/********************************************************************************/
/*						USART I/O DATA REGISTER									*/
/********************************************************************************/
#define UDR			*((u8*)0x2C)
#define TXB				UDR				/*	UDR Register in Transmit Mode		*/
#define RXB				UDR				/*	UDR Register in Receive Mode		*/


/********************************************************************************/
/*						USART CONTROL & STATUS  REGISTER A						*/
/********************************************************************************/
#define UCSRA		*((u8*)0x2B)
#define MPCM_BIT		0				/*	Multi-processor Communication Mode	*/	//	This bit enables the Multi-processor Communication mode
#define U2X_BIT			1				/*	Double the USART Transmission Speed	*/	//	This bit only has effect for the asynchronous operation
#define PE_BIT			2				/*	Parity Error Flag					*/
#define	DOR_BIT			3				/*	Data OverRun Flag					*/
#define	FE_BIT			4				/*	Frame Error Flag					*/
#define	UDRE_BIT		5				/*	USART Data Register(UDR) Empty Flag	*/
#define	TXC_BIT			6				/*	USART Transmit Complete  Flag		*/
#define	RXC_BIT			7				/*	USART Receive Complete  Flag		*/


/********************************************************************************/
/*						USART CONTROL & STATUS  REGISTER B						*/
/********************************************************************************/
#define UCSRB		*((u8*)0x2A)
#define TXB8_BIT		0				/*	Transmit Data Bit 8	(ninth data bit)*/	//	Must be written first before low bits to UDR
#define RXB8_BIT		1				/*	Receive Data Bit 8	(ninth data bit)*/	//	Must be read before reading low bits from UDR
#define UCSZ2_BIT		2				/*	Character Size (sets #of Data bits)	*/
#define	TXEN_BIT		3				/*	Transmitter Enable					*/
#define	RXEN_BIT		4				/*	Receiver Enable						*/
#define	UDRIE_BIT		5				/*	UDR Empty Interrupt Enable			*/
#define	TXCIE_BIT		6				/*	TX Complete Interrupt Enable		*/
#define	RXCIE_BIT		7				/*	RX Complete Interrupt Enable		*/

/********************************************************************************/
/*						USART CONTROL & STATUS  REGISTER C						*/
/********************************************************************************/
#define UCSRC			UBRRH
#define UCPOL_BIT		0				/*	Clock Polarity (used For Sync ONLY) */	//	This bit is ZERO in ASYNC mode	( 0: TxD@Rising Edge&RxD@Falling Edge / 1:TxD@Falling Edge&RxD@Rising Edge )
#define UCSZ0_BIT		1				/*********	Character Size	*************/	//	UCSZ2|UCSZ1|UCSZ0	Combinations
#define UCSZ1_BIT		2				/***	along with UCSZ2 in UCSRB 	*****/	//	000: 5-bit	/ 001: 6-bit / 010: 7-bit / 011: 8-bit / 111: 9-bit
#define	USBS_BIT		3				/*	STOP Bit Select	(0:1-bit / 1:2-bit)	*/
#define	UPM0_BIT		4				/*******	Parity Mode Select	*********/	//UPM1_BIT & UPM0_BIT Combination
#define	UPM1_BIT		5				/* 	00:Disabled	/	10:EVEN	/	11:ODD	*/	//	BITS	5|4: Selection
#define	UMSEL_BIT		6				/*	USART Mode Select (0:Async / 1:Sync)*/
#define	URSEL_BIT		7				/*	Register Select ( UCSRC / UBRRH )	*/	//Initially is 1 for UCSRC


/********************************************************************************/
/*							USART BAUD RATE REGISTERs							*/
/********************************************************************************/
#define UBRRH		*((u8*)0x40)		/*	Lower 4-bits along with UBRRL is a	*/	//	Has to be Written First
#define UBRRL		*((u8*)0x29)		/*	12-bit Register for Baud rate Value	*/	// Once written will trigger an immediate update of Baud rate


#define SREG		*((u8*)0x5F)		/*	AVR Status Register					*/

#endif /* MCAL_UART_UART_PRIV_H_ */
