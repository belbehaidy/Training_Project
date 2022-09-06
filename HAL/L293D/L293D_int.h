/*
 * L293d_int.h
 *
 *  Created on: Sep 2, 2022
 *      Author: basse
 */

#ifndef HAL_L293D_L293D_INT_H_
#define HAL_L293D_L293D_INT_H_

#define CLOCK_WISE_DIRECTION			32
#define COUNTER_CLOCK_WISE_DIRECTION	33


ES_t L293D_enuInit( void );

ES_t Le93D_enuSetDirectio( u8 Copy_u8MotorNum , u8 Copy_u8MotorDirection);

ES_t Le93D_enuSetSpeed( u8 Copy_u8MotorNum , u8 Copy_u8MotorMaxSpeedPercentage); // three cases 0 -> EN = LOW , 100 -> EN = HIGH , Others Calc Duty from RMS value law


#endif /* HAL_L293D_L293D_INT_H_ */
