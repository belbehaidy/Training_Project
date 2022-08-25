/*
 * Switch_int.h
 *
 *  Created on: Aug 6, 2022
 *      Author: Ahmed El-Gaafrawy
 */

#ifndef SWITCH_INT_H_
#define SWITCH_INT_H_

#define SWITCH_UNPRESSED		15
#define SWITCH_IS_PRESSED		25

#define SW_ZERO					201
#define SW_ONE					202
#define SW_TWO					203



ES_t Switch_enuInit(void);

/*Bouncing effect of switch is considered in Function , return Value in Variable Pointer is SWITCH_UNPRESSED or SWITCH_IS_PRESSED*/
ES_t Switch_enuGetPressed (u8 Copy_u8SwitchNum ,u8 *Copy_pu8SwitchValue );

#endif /* SWITCH_INT_H_ */
