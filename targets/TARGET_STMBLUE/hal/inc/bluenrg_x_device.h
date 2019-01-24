/**
  ******************************************************************************
  * @file    BlueNRG_x_device.h
  * @author  VMA Application Team
  * @version V1.0.0
  * @date    23-January-2017
  * @brief   This file is used to select the BlueNRG-1 or BlueNRG-2 device.
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
  * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLUENRG_X_DEVICE_H
#define BLUENRG_X_DEVICE_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup BlueNRG_x_device BlueNRG_x_device
  * @{
  */

#ifdef BLUENRG2_DEVICE /* Reserved for Future Use: Do Not Enable it */
#include "BlueNRG2.h"   
#else
#include "BlueNRG1.h"
#endif

/** @addtogroup BlueNRG_x_device_configuration Configuration
  * @{
  */

/**
  * @}
  */

/** @addtogroup BlueNRG_x_device_Exported_Constants Exported Constants
  * @{
  */



   
/**
  * @}
  */

/** @addtogroup BlueNRG_x_device_Exported_Macros Exported Macros
  * @{
  */

  
  
/**
  * @}
  */
  
/** @addtogroup BlueNRG_x_device_Exported_Typedefs Exported Typedefs
  * @{
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

#endif /* BLUENRG_X_DEVICE_H */

/**
  * @}
  */
  
/**
  * @}
  */
