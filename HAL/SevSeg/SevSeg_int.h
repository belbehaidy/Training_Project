/*
 * SevSeg_int.h
 *
 *  Created on: Aug 9, 2022
 *      Author: basse
 */

#ifndef SEVSEG_INT_H_
#define SEVSEG_INT_H_

#define SevSeg_u8MODULE_1			MODULE_1
#define SevSeg_u8MODULE_2			MODULE_2

#define SevSeg_u8MODULE_DISABLE		MODULE_DISABLE
#define SevSeg_u8MODULE_ENABLE		MODULE_ENABLE


#define SevSeg_u8DIGIT_BASE			DIGIT_BASE
#define SevSeg_u8DIP_OFF			DIP_OFF
#define SevSeg_u8DIP_ON				DIP_ON


ES_t SevSeg_enuInit(void);
ES_t SevSeg_enuSetDigitValue(u8 Copy_u8ModuleNum , u8 Copy_u8SevSegDigitValue);
ES_t SevSeg_enuSetDIPValue(u8 Copy_u8ModuleNum ,u8 Copy_u8SevSegDIPValue);
ES_t SevSeg_enuModuleControl(u8 Copy_u8ModuleNum , u8 Copy_u8SevSegModuleStatus);
ES_t SevSeg_enuFrameDelay(u8 Copy_u8SevSegTotalModules, u8 * Copy_pu8SevSegModuleDelay);


#endif /* SEVSEG_INT_H_ */
