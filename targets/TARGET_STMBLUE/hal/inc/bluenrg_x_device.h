/*
 * Copyright (c) 2019 STMicroelectronics. All Rights Reserved.
 *
 * The information contained herein is property of STMicroelectronics.
 * Terms and conditions of usage are described in detail in SOFTWARE
 * LICENSE AGREEMENT:
 *
 * (SLA0068 - PRODUCTION LIMITED LICENSE AGREEMENT FOR ST MATERIALS)
 *
 * You may obtain a copy of the License here:
 * LICENSE-SLA0068.txt and at STMicroelectronics website.
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
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
