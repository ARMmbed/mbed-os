/**
  ******************************************************************************
  * @file    BlueNRG1_RNG.h
  * @author  VMA Application Team
  * @version V2.0.0
  * @date    21-March-2016
  * @brief   This file contains all the functions prototypes for the RNG 
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
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLUENRG1_RNG_H
#define BLUENRG1_RNG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG_x_device.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup RNG_Peripheral  RNG Peripheral
  * @{
  */

/** @defgroup RNG_Exported_Types Exported Types
  * @{
  */

/**
  * @}
  */

/** @defgroup RNG_Exported_Constants Exported Constants
  * @{
  */
    
/**
  * @}
  */


/** @defgroup RNG_Exported_Macros Exported Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup RNG_Exported_Functions Exported Functions
  * @{
  */

void RNG_Cmd(FunctionalState NewState);
FlagStatus RNG_GetFlagStatus(void);

/**
* @brief  Get the RNG value.
* @param  None
* @retval RNG value
*/
#define RNG_GetValue()                                    READ_REG(RNG->VAL)

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

#endif /* BLUENRG1_RNG_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
