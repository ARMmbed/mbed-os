/**
  ******************************************************************************
  * @file    system_stm32wbxx.h
  * @author  MCD Application Team
  * @brief   CMSIS Cortex Device System Source File for STM32WBxx devices.  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Apache License, Version 2.0,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/Apache-2.0
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32wbxx_system
  * @{
  */  
  
/**
  * @brief Define to prevent recursive inclusion
  */
#ifndef __SYSTEM_STM32WBXX_H
#define __SYSTEM_STM32WBXX_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>

/** @addtogroup STM32WBxx_System_Includes
  * @{
  */

/**
  * @}
  */


/** @addtogroup STM32WBxx_System_Exported_types
  * @{
  */
  /* The SystemCoreClock variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */

extern uint32_t SystemCoreClock;            /*!< System Clock Frequency */

extern const uint32_t AHBPrescTable[16];    /*!< AHB prescalers table values */
extern const uint32_t APBPrescTable[8];     /*!< APB prescalers table values */
extern const uint32_t MSIRangeTable[16];    /*!< MSI ranges table values     */

#if defined(STM32WB55xx) || defined(STM32WB5Mxx) || defined(STM32WB35xx) || defined (STM32WB15xx) || defined (STM32WB10xx)
extern const uint32_t SmpsPrescalerTable[4][6];  /*!< SMPS factor ranges table values     */
#endif
/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32WBxx_System_Exported_Functions
  * @{
  */
  
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_STM32WBXX_H */

/**
  * @}
  */
  
/**
  * @}
  */  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
