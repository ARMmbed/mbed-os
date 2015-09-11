/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   This file contains all the functions prototypes for the ADC 
  *          firmware library.
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __W7500X_ADC_H
#define __W7500X_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "W7500x.h"

typedef enum { ADC_CH0 = 0, 
	       ADC_CH1 = 1,
	       ADC_CH2 = 2, 
	       ADC_CH3 = 3,
	       ADC_CH4 = 4,
	       ADC_CH5 = 5,
	       ADC_CH6 = 6,
	       ADC_CH7 = 7,
	       ADC_CH15 = 15} ADC_CH;

#define IS_ADC_CH_NUM(NUM)                (((NUM) == ADC_CH0) || \
		                           ((NUM) == ADC_CH1) || \
		                           ((NUM) == ADC_CH2) || \
		                           ((NUM) == ADC_CH3) || \
		                           ((NUM) == ADC_CH4) || \
		                           ((NUM) == ADC_CH5) || \
		                           ((NUM) == ADC_CH6) || \
		                           ((NUM) == ADC_CH7) || \
		                           ((NUM) == ADC_CH15))

void ADC_Init(void); 
void ADC_DeInit(void);
void ADC_PowerDownEnable (FunctionalState NewState);
void ADC_ChannelSelect (ADC_CH num);
void ADC_Start (void);
uint16_t ADC_ReadData (void);
void ADC_InterruptMask (FunctionalState NewState);
uint8_t ADC_IsInterrupt (void);
void ADC_InterruptClear (void);

#ifdef __cplusplus
 }
#endif

#endif  //__W7500X_ADC_H
