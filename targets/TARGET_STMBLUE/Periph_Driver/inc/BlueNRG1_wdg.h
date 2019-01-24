/**
  ******************************************************************************
  * @file    BlueNRG1_WDG.h
  * @author  AMS RF Application Team
  * @version V2.2.0
  * @date    27-April-2018
  * @brief   This file contains all the functions prototypes for the WDG 
  *          firmware library.
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
  * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/**
 * @file  BlueNRG1_WDG.h
 * @brief BlueNRG-1,2 Watchdog header file
 * <!-- Copyright 2018 by STMicroelectronics.  All rights reserved.       *80*-->

* \section WDG How tho use the Watchdog functionality? 

   In order to use this functionality the best way is to add  the watchdog reload in the main loop (and not on the WDG IRQ handler: WDG_Handler()). 
   User should follow these steps:

   1: Add a Watchdog initialization function to be called before entering on main loop: (WATCHDOG_TIME is a timeout defined based on the application scenario):

         void WDG_Init(void)
         {
           SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_WDG, ENABLE);
           WDG_SetReload(RELOAD_TIME(WATCHDOG_TIME));
           WDG_Enable();
         }

   2: Just call the reload function inside the main loop:

         while(1)
         {
            .....
            WDG_SetReload(RELOAD_TIME(WATCHDOG_TIME));
	 }

   - NOTE: The option to use the WDG_Handler() it is not mandatory at all, and it is just an implementation choice. If user wants to use this method (not mandatory), then he  must clear the pending bit associated to the watchdog (WDG_ClearITPendingBit()).

**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLUENRG1_WDG_H
#define BLUENRG1_WDG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG_x_device.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup WDG_Peripheral  WDG Peripheral
  * @{
  */

/** @defgroup WDG_Exported_Types Exported Types
  * @{
  */

/**
  * @}
  */

/** @defgroup WDG_Exported_Constants Exported Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup WDG_Exported_Macros Exported Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup WDG_Exported_Functions Exported Functions
  * @{
  */
void WDG_SetWriteAccess(FunctionalState NewState);
FunctionalState WDG_GetWriteAccess(void);
void WDG_SetReload(uint32_t WDG_Reload);
uint32_t WDG_GetCounter(void);
void WDG_Enable(void);
void WDG_DisableReset(void);
void WDG_ITConfig(FunctionalState NewState);
ITStatus WDG_GetITStatus(void);
void WDG_ClearITPendingBit(void);

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

#endif /* BLUENRG1_WDG_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
