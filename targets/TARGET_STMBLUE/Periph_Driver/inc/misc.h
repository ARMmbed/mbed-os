/**
  ******************************************************************************
  * @file    misc.h
  * @author  VMA Application Team
  * @version V2.0.0
  * @date    21-March-2016
  * @brief   This file contains all the functions prototypes for the miscellaneous
  *          firmware library functions (add-on to CMSIS functions).
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLUENRG1_MISC_H
#define BLUENRG1_MISC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bluenrg_x_device.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup MISC_Peripheral   MISC Peripheral
  * @{
  */

/** @defgroup MISC_Exported_Types Exported Types
  * @{
  */


/** 
  * @brief  Structure definition of NVIC initialization
  */

typedef struct
{
  uint8_t NVIC_IRQChannel;                    /*!< Specifies the IRQ channel to be enabled or disabled.
                                                   This parameter can be a value of @ref IRQn_Type 
                                                   (For the complete BlueNRG1 Device IRQ Channels list, please
                                                    refer to BlueNRG1.h file) */

  uint8_t NVIC_IRQChannelPreemptionPriority;  /*!< Specifies the pre-emption priority for the IRQ channel
                                                   specified in NVIC_IRQChannel. This parameter can be a value
                                                   between 0 and 3 */

  FunctionalState NVIC_IRQChannelCmd;         /*!< Specifies whether the IRQ channel defined in NVIC_IRQChannel
                                                   will be enabled or disabled. 
                                                   This parameter can be set either to ENABLE or DISABLE */   
} NVIC_InitType;
 
/**
  * @}
  */

/** @defgroup MISC_Exported_Constants Exported Constants
  * @{
  */

/** @defgroup Preemption_Priority_Definitions Preemption Priority Group Definitions
  * @{
  */

/* IRQ priority high */ 
#define HIGH_PRIORITY     1

/* IRQ priority medium */ 
#define MED_PRIORITY      2

/* IRQ priority low */ 
#define LOW_PRIORITY      3

/* This macro checks if PRIORITY is a valid WriteAccess value */
#define IS_NVIC_PREEMPTION_PRIORITY(PRIORITY)  ((PRIORITY) < 0x04)


/**
  * @}
  */


/**
  * @}
  */

/** @defgroup MISC_Exported_Macros Exported Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup MISC_Exported_Functions Exported Functions
  * @{
  */

void NVIC_Init(NVIC_InitType* NVIC_InitStruct);
void SystemSleepCmd(FunctionalState NewState);
void SysTick_State(FunctionalState NewState);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* BLUENRG1_MISC_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
