/*
 * EXTI_int.h

 *
 *  Created on: Aug 19, 2022
 *      Author: Bassem EL-Behaidy
 */

#ifndef MCAL_EXTI_EXTI_INT_H_
#define MCAL_EXTI_EXTI_INT_H_

//////////////////////////////////////////////////
//		Interrupt Sense Level Choices			\\
//		-----------------------------			//
//	Choice					Description			\\
//////////////////////////////////////////////////
//	LOW_LEVEL		|	Low Level Sense			//
//	ANY_LGC_LVL		|	Any Logic Level Sense	//
//	FALLING_EDGE	|	Falling Edge Sense		//
//	RISING_EDGE		|	Rising Edge Sense		//
//////////////////////////////////////////////////

//////////////////////////////////////
//			Interrupt Numbers		\\
//////////////////////////////////////
//	INT0	|	INT1	|	INT2	//
//////////////////////////////////////


ES_t EXTI_enuInit(void);

ES_t EXTI_enuEnableInterrupt(u8 Copy_u8IntNum);

ES_t EXTI_enuDisableInterrupt(u8 Copy_u8IntNum);

ES_t EXTI_enuSetSenseLevel(u8 Copy_u8IntNum , u8 Copy_u8SenseLevel);

#endif /* MCAL_EXTI_EXTI_INT_H_ */
