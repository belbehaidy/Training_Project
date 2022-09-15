/*
 * UART_int.h
 *
 *  Created on: Sep 9, 2022
 *      Author: basse
 */

#ifndef MCAL_UART_UART_INT_H_
#define MCAL_UART_UART_INT_H_

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
#define RECIEVER			11
#define TRANSCIEVER			12

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



ES_t UART_enuInit( void );

ES_t UART_ReceiveFrame( void *Copy_pReceivedData);

ES_t UART_SendFrame( void *Copy_pData);

ES_t UART_enuSendPacket( void *Copy_pcData );

ES_t UART_enuRecievePacket(void *Copy_pcData);

ES_t UART_enuCallBack(  u8 Copy_u8InterruptName , void ( *Copy_pAppFun(void *) ) , void *Copy_pAppVar );

#endif /* MCAL_UART_UART_INT_H_ */
