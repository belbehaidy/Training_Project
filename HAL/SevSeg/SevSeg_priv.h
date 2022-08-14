/*
 * SevSeg_priv.h
 *
 *  Created on: Aug 9, 2022
 *      Author: basse
 */

#ifndef SEVSEG_PRIV_H_
#define SEVSEG_PRIV_H_

#define DIP_OFF						DIO_u8LOW
#define DIP_ON						DIO_u8HIGH

#define SevSeg_u8BIT_MASK			0x01

#define SevSeg_u8FRAME_DELAY		20




#define MODULE_1					11
#define MODULE_2					12

#define MODULE_DISABLE				13
#define MODULE_ENABLE				14

#define SevSeg_u8MODULE_DELAY( Copy_u8SevSegTotalModules )	 ( (SevSeg_u8FRAME_DELAY + Copy_u8SevSegTotalModules - 1) / Copy_u8SevSegTotalModules )

#endif /* SEVSEG_PRIV_H_ */
