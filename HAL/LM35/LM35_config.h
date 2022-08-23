/*
 * LM35_config.h
 *
 *  Created on: Aug 21, 2022
 *      Author: basse
 */

#ifndef APP_LM35_CONFIG_H_
#define APP_LM35_CONFIG_H_


#define VSS_GRP					DIO_u8GROUP_C
#define VSS_PIN					DIO_u8PIN1

#define GROUND_GRP				DIO_u8GROUP_C
#define GROUND_PIN				DIO_u8PIN0

#define TEMP_VALUE_GRP			DIO_u8GROUP_A
#define TEMP_VALUE_PIN			DIO_u8PIN0
#define TEMP_VALUE_STATE		DIO_u8FLOAT

#endif /* APP_LM35_CONFIG_H_ */
