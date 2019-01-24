/**
  ******************************************************************************
  * @file    BlueNRG1_pka.h
  * @author  VMA Application Team
  * @version V1.0.0
  * @date    27-February-2017
  * @brief   This file contains all the functions prototypes for the PKA 
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
#ifndef BLUENRG1_PKA_H
#define BLUENRG1_PKA_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG_x_device.h"

     
/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup PKA_Peripheral  PKA Peripheral
  * @{
  */

/** @defgroup PKA_Exported_Types Exported Types
  * @{
  */

#define PKA_DATA_SK       0x01  /* PKA select data for K value */
#define PKA_DATA_PCX      0x02  /* PKA select data for Point X coordinate value */
#define PKA_DATA_PCY      0x03  /* PKA select data for Point Y coordinate value */

#define IS_PKA_CMD(CMD) ( ((CMD) == PKA_DATA_SK) || \
                          ((CMD) == PKA_DATA_PCX) || \
                          ((CMD) == PKA_DATA_PCY) )


/** 
  * @brief  PKA status enumeration
  */
  
#define PKA_PROCEND       0x01  /* PKA process end */
#define PKA_RAMERR        0x04  /* PKA RAM error */
#define PKA_ADDERR        0x08  /* PKA address invalid */

#define IS_PKA_FLAG(FLAG) ( ((FLAG) == PKA_PROCEND) || \
                            ((FLAG) == PKA_RAMERR) || \
                            ((FLAG) == PKA_ADDERR) )

/**
  * @}
  */

/** @defgroup PKA_Exported_Constants Exported Constants
  * @{
  */

/** @defgroup PKA_Size_Definitions Size Definitions
  * @{
  */

#define PKA_RAM_BASE                (PKA_BASE + 0x00000400)

#define PKA_RAM_START               (PKA_RAM_BASE)
#define PKA_RAM_END                 (PKA_RAM_BASE + 0x00001000)

#define PKA_RAM_ECC_ADDR_KP_ERROR   (PKA_RAM_BASE)
#define PKA_RAM_ECC_ADDR_K          (PKA_RAM_BASE + 0x6C)
#define PKA_RAM_ECC_ADDR_PX         (PKA_RAM_BASE + 0x90)
#define PKA_RAM_ECC_ADDR_PY         (PKA_RAM_BASE + 0xB4)

   
          
/* The initial Point X coordinate of the curve in hex format:
 * 6b17d1f2 e12c4247 f8bce6e5 63a440f2 77037d81 2deb33a0 f4a13945 d898c296
 */
#define INITIAL_START_POINT_X_W1        0xd898c296
#define INITIAL_START_POINT_X_W2        0xf4a13945
#define INITIAL_START_POINT_X_W3        0x2deb33a0
#define INITIAL_START_POINT_X_W4        0x77037d81
#define INITIAL_START_POINT_X_W5        0x63a440f2
#define INITIAL_START_POINT_X_W6        0xf8bce6e5
#define INITIAL_START_POINT_X_W7        0xe12c4247
#define INITIAL_START_POINT_X_W8        0x6b17d1f2

/* The initial Point Y coordinate of the curve in hex format:
 * 4fe342e2 fe1a7f9b 8ee7eb4a 7c0f9e16 2bce3357 6b315ece cbb64068 37bf51f5
 */
#define INITIAL_START_POINT_Y_W1        0x37bf51f5
#define INITIAL_START_POINT_Y_W2        0xcbb64068
#define INITIAL_START_POINT_Y_W3        0x6b315ece
#define INITIAL_START_POINT_Y_W4        0x2bce3357
#define INITIAL_START_POINT_Y_W5        0x7c0f9e16
#define INITIAL_START_POINT_Y_W6        0x8ee7eb4a
#define INITIAL_START_POINT_Y_W7        0xfe1a7f9b
#define INITIAL_START_POINT_Y_W8        0x4fe342e2

    
/**
  * @}
  */

  
/** @defgroup PKA_Commands_Definitions  Commands Definitions
  * @{
  */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup PKA_Exported_Macros Exported Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup PKA_Exported_Functions Exported Functions
  * @{
  */


ErrorStatus PKA_SetData(uint8_t dataType, uint32_t* srcData);
ErrorStatus PKA_GetData(uint8_t dataType, uint8_t* dataTarget);

void PKA_Reset(void);
void PKA_StartProcessing(void);
void PKA_WaitProcess(void);
FlagStatus PKA_GetProcessStatus(void);
ErrorStatus PKA_VerifyProcess(void);

void PKA_ClearITPendingBit(uint8_t PkaFlag);
FlagStatus PKA_GetFlagStatus(uint8_t PkaFlag);
void PKA_ITConfig(uint8_t PkaFlag, FunctionalState NewState);

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

#endif /* BLUENRG1_PKA_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
