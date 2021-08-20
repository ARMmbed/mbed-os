/**
  ******************************************************************************
  * @file    stm32l0xx_hal_cryp.h
  * @author  MCD Application Team
  * @brief   Header file of CRYP HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright(c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L0xx_HAL_CRYP_H
#define __STM32L0xx_HAL_CRYP_H

#ifdef __cplusplus
 extern "C" {
#endif
   
#if defined (STM32L021xx) || defined (STM32L041xx) || defined (STM32L062xx) || defined (STM32L063xx) || defined (STM32L081xx) || defined (STM32L082xx) || defined (STM32L083xx)

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal_def.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @defgroup CRYP CRYP
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/ 

/** @defgroup CRYP_Exported_Types CRYP Exported Types
  * @{
  */

/** 
  * @brief  CRYP Configuration Structure definition  
  */
typedef struct
{  
  uint32_t DataType;    /*!< 32-bit data, 16-bit data, 8-bit data or 1-bit string.
                             This parameter can be a value of @ref CRYP_Data_Type */
  
  uint8_t* pKey;        /*!< The key used for encryption/decryption */
  
  uint8_t* pInitVect;   /*!< The initialization vector used also as initialization
                             counter in CTR mode */
  
}CRYP_InitTypeDef;

/** 
  * @brief HAL CRYP State structures definition  
  */ 
typedef enum
{
  HAL_CRYP_STATE_RESET             = 0x00U,  /*!< CRYP not yet initialized or disabled  */
  HAL_CRYP_STATE_READY             = 0x01U,  /*!< CRYP initialized and ready for use    */
  HAL_CRYP_STATE_BUSY              = 0x02U,  /*!< CRYP internal processing is ongoing   */
  HAL_CRYP_STATE_TIMEOUT           = 0x03U,  /*!< CRYP timeout state                    */
  HAL_CRYP_STATE_ERROR             = 0x04U   /*!< CRYP error state                      */ 
    
}HAL_CRYP_STATETypeDef;

/** 
  * @brief HAL CRYP phase structures definition  
  */ 
typedef enum
{
  HAL_CRYP_PHASE_READY             = 0x01U,    /*!< CRYP peripheral is ready for initialization. */
  HAL_CRYP_PHASE_PROCESS           = 0x02U,    /*!< CRYP peripheral is in processing phase */
}HAL_PhaseTypeDef;

/** 
  * @brief  CRYP handle Structure definition  
  */ 
typedef struct
{
  AES_TypeDef                 *Instance;        /*!< Register base address        */

  CRYP_InitTypeDef            Init;             /*!< CRYP required parameters */

  uint8_t                     *pCrypInBuffPtr;  /*!< Pointer to CRYP processing (encryption, decryption,...) buffer */

  uint8_t                     *pCrypOutBuffPtr; /*!< Pointer to CRYP processing (encryption, decryption,...) buffer */

  __IO uint16_t               CrypInCount;      /*!< Counter of inputed data */

  __IO uint16_t               CrypOutCount;     /*!< Counter of outputed data */

  HAL_StatusTypeDef           Status;           /*!< CRYP peripheral status */

  HAL_PhaseTypeDef            Phase;            /*!< CRYP peripheral phase */

  DMA_HandleTypeDef           *hdmain;          /*!< CRYP In DMA handle parameters */

  DMA_HandleTypeDef           *hdmaout;         /*!< CRYP Out DMA handle parameters */

  HAL_LockTypeDef             Lock;             /*!< CRYP locking object */

  __IO  HAL_CRYP_STATETypeDef State;            /*!< CRYP peripheral state */

}CRYP_HandleTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup CRYP_Exported_Constants CRYP Exported Constants
  * @{
  */

/** @defgroup CRYP_Data_Type CRYP Data Type
  * @{
  */
#define CRYP_DATATYPE_32B         ((uint32_t)0x00000000U)
#define CRYP_DATATYPE_16B         AES_CR_DATATYPE_0
#define CRYP_DATATYPE_8B          AES_CR_DATATYPE_1
#define CRYP_DATATYPE_1B          AES_CR_DATATYPE

#define IS_CRYP_DATATYPE(DATATYPE) (((DATATYPE) == CRYP_DATATYPE_32B) || \
                                    ((DATATYPE) == CRYP_DATATYPE_16B) || \
                                    ((DATATYPE) == CRYP_DATATYPE_8B)  || \
                                    ((DATATYPE) == CRYP_DATATYPE_1B))  
/**
  * @}
  */

/** @defgroup CRYP_AlgoModeDirection CRYP Algo Mode Direction
  * @{
  */ 
#define CRYP_CR_ALGOMODE_DIRECTION              (uint32_t)(AES_CR_MODE|AES_CR_CHMOD)

#define CRYP_CR_ALGOMODE_AES_ECB_ENCRYPT        ((uint32_t)0x00000000U)
#define CRYP_CR_ALGOMODE_AES_ECB_KEYDERDECRYPT  (AES_CR_MODE)
#define CRYP_CR_ALGOMODE_AES_CBC_ENCRYPT        (AES_CR_CHMOD_0)
#define CRYP_CR_ALGOMODE_AES_CBC_KEYDERDECRYPT  ((uint32_t)(AES_CR_CHMOD_0|AES_CR_MODE))
#define CRYP_CR_ALGOMODE_AES_CTR_ENCRYPT        (AES_CR_CHMOD_1)
#define CRYP_CR_ALGOMODE_AES_CTR_DECRYPT        ((uint32_t)(AES_CR_CHMOD_1 | AES_CR_MODE_1))
/**
  * @}
  */
  
/** @defgroup CRYP_AES_Interrupts AES Interrupts
  * @{
  */ 
#define CRYP_IT_CC                          AES_CR_CCIE  /*!< Computation Complete interrupt */
#define CRYP_IT_ERR                         AES_CR_ERRIE /*!< Error interrupt                */

/**
  * @}
  */


/** @defgroup CRYP_AES_Flags AES Flags
  * @{
  */ 
#define CRYP_FLAG_CCF                       AES_SR_CCF    /*!< Computation Complete Flag */
#define CRYP_FLAG_RDERR                     AES_SR_RDERR  /*!< Read Error Flag           */
#define CRYP_FLAG_WRERR                     AES_SR_WRERR  /*!< Write Error Flag          */

/**
  * @}
  */ 

/** @defgroup CRYP_AES_Clear_Flags AES Clear Flags
  * @{
  */ 
#define CRYP_CLEARFLAG_CCF                       AES_CR_CCFC   /*!< Computation Complete Flag Clear */
#define CRYP_CLEARFLAG_RDERR                     AES_CR_ERRC   /*!< Read Error Clear           */
#define CRYP_CLEARFLAG_WRERR                     AES_CR_ERRC   /*!< Write Error Clear          */

/**
  * @}
  */ 

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/** @defgroup CRYP_Exported_Macros CRYP Exported Macros
  * @{
  */

/** @brief Reset CRYP handle state
  * @param  __HANDLE__ specifies the CRYP handle.
  * @retval None
  */
#define __HAL_CRYP_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_CRYP_STATE_RESET)

/**
  * @brief  Enable/Disable the CRYP peripheral.
  * @param  __HANDLE__ specifies the CRYP handle.
  * @retval None
  */
#define __HAL_CRYP_ENABLE(__HANDLE__)                   SET_BIT((__HANDLE__)->Instance->CR, AES_CR_EN)
#define __HAL_CRYP_DISABLE(__HANDLE__)                  CLEAR_BIT((__HANDLE__)->Instance->CR, AES_CR_EN)

/**
  * @brief  Set the algorithm mode: AES-ECB, AES-CBC, AES-CTR, DES-ECB, DES-CBC,...
  * @param  __HANDLE__ specifies the CRYP handle.
  * @param  __MODE__ The algorithm mode.
  * @retval None
  */
#define __HAL_CRYP_SET_MODE(__HANDLE__,__MODE__)             SET_BIT((__HANDLE__)->Instance->CR, (__MODE__))


/** @brief  Check whether the specified CRYP flag is set or not.
  * @param  __HANDLE__ specifies the CRYP handle.
  * @param  __FLAG__ specifies the flag to check.
  *         This parameter can be one of the following values:
  *            @arg CRYP_FLAG_CCF   : Computation Complete Flag
  *            @arg CRYP_FLAG_RDERR : Read Error Flag
  *            @arg CRYP_FLAG_WRERR : Write Error Flag
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_CRYP_GET_FLAG(__HANDLE__,__FLAG__)         (((__HANDLE__)->Instance->SR & (__FLAG__)) == (__FLAG__))

/** @brief  Clear the CRYP pending flag.
  * @param  __HANDLE__ specifies the CRYP handle.
  * @param  __FLAG__ specifies the flag to clear.
  *         This parameter can be one of the following values:
  *            @arg CRYP_CLEARFLAG_CCF   : Computation Complete Clear Flag
  *            @arg CRYP_CLEARFLAG_RDERR : Read Error Clear
  *            @arg CRYP_CLEARFLAG_WRERR : Write Error Clear
  * @retval None
  */
#define __HAL_CRYP_CLEAR_FLAG(__HANDLE__, __FLAG__)   SET_BIT((__HANDLE__)->Instance->CR, (__FLAG__))

/**
  * @brief  Enable the CRYP interrupt.
  * @param  __HANDLE__ specifies the CRYP handle.
  * @param  __INTERRUPT__ CRYP Interrupt.
  * @retval None
  */
#define __HAL_CRYP_ENABLE_IT(__HANDLE__,__INTERRUPT__)   SET_BIT((__HANDLE__)->Instance->CR, (__INTERRUPT__))

/**
  * @brief  Disable the CRYP interrupt.
  * @param  __HANDLE__ specifies the CRYP handle.
  * @param  __INTERRUPT__ CRYP interrupt.
  * @retval None
  */
#define __HAL_CRYP_DISABLE_IT(__HANDLE__,__INTERRUPT__)  CLEAR_BIT((__HANDLE__)->Instance->CR, (__INTERRUPT__))

/** @brief  Checks if the specified CRYP interrupt source is enabled or disabled.
  * @param  __HANDLE__ specifies the CRYP handle.
  * @param __INTERRUPT__ CRYP interrupt source to check
  *         This parameter can be one of the following values:
  *            @arg CRYP_IT_CC   : Computation Complete interrupt
  *            @arg CRYP_IT_ERR : Error interrupt (used for RDERR and WRERR)
  * @retval State of interruption (SET or RESET)
  */
#define __HAL_CRYP_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) \
    (( ((__HANDLE__)->Instance->CR & (__INTERRUPT__)) == (__INTERRUPT__)       \
     )? SET : RESET                                         \
    )
         
/** @brief  Clear the CRYP pending IT.
  * @param  __HANDLE__ specifies the CRYP handle.
  * @param  __IT__ specifies the IT to clear.
  *         This parameter can be one of the following values:
  *            @arg CRYP_CLEARFLAG_CCF   : Computation Complete Clear Flag
  *            @arg CRYP_CLEARFLAG_RDERR : Read Error Clear
  *            @arg CRYP_CLEARFLAG_WRERR : Write Error Clear
  * @retval None
  */
#define __HAL_CRYP_CLEAR_IT(__HANDLE__, __IT__) SET_BIT((__HANDLE__)->Instance->CR, (__IT__))

/**
  * @}
  */

/* Include CRYP HAL Extension module */
#include "stm32l0xx_hal_cryp_ex.h"

/* Exported functions --------------------------------------------------------*/

/** @defgroup CRYP_Exported_Functions CRYP Exported Functions
  * @{
  */

/** @defgroup CRYP_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */

/* Initialization/de-initialization functions *********************************/
HAL_StatusTypeDef     HAL_CRYP_Init(CRYP_HandleTypeDef *hcryp);
HAL_StatusTypeDef     HAL_CRYP_DeInit(CRYP_HandleTypeDef *hcryp);

/* MSP functions  *************************************************************/
void                  HAL_CRYP_MspInit(CRYP_HandleTypeDef *hcryp);
void                  HAL_CRYP_MspDeInit(CRYP_HandleTypeDef *hcryp);

/**
  * @}
  */ 

/** @defgroup CRYP_Exported_Functions_Group2  AES processing functions
  * @{
  */

/* AES encryption/decryption using polling  ***********************************/
HAL_StatusTypeDef     HAL_CRYP_AESECB_Encrypt(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData, uint32_t Timeout);
HAL_StatusTypeDef     HAL_CRYP_AESECB_Decrypt(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData, uint32_t Timeout);
HAL_StatusTypeDef     HAL_CRYP_AESCBC_Encrypt(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData, uint32_t Timeout);
HAL_StatusTypeDef     HAL_CRYP_AESCBC_Decrypt(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData, uint32_t Timeout);
HAL_StatusTypeDef     HAL_CRYP_AESCTR_Encrypt(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData, uint32_t Timeout);
HAL_StatusTypeDef     HAL_CRYP_AESCTR_Decrypt(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData, uint32_t Timeout);

/* AES encryption/decryption using interrupt  *********************************/
HAL_StatusTypeDef     HAL_CRYP_AESECB_Encrypt_IT(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData);
HAL_StatusTypeDef     HAL_CRYP_AESCBC_Encrypt_IT(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData);
HAL_StatusTypeDef     HAL_CRYP_AESCTR_Encrypt_IT(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData);
HAL_StatusTypeDef     HAL_CRYP_AESECB_Decrypt_IT(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData);
HAL_StatusTypeDef     HAL_CRYP_AESCTR_Decrypt_IT(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData);
HAL_StatusTypeDef     HAL_CRYP_AESCBC_Decrypt_IT(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData);

/* AES encryption/decryption using DMA  ***************************************/
HAL_StatusTypeDef     HAL_CRYP_AESECB_Encrypt_DMA(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData);
HAL_StatusTypeDef     HAL_CRYP_AESECB_Decrypt_DMA(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData);
HAL_StatusTypeDef     HAL_CRYP_AESCBC_Encrypt_DMA(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData);
HAL_StatusTypeDef     HAL_CRYP_AESCBC_Decrypt_DMA(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData);
HAL_StatusTypeDef     HAL_CRYP_AESCTR_Encrypt_DMA(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData);
HAL_StatusTypeDef     HAL_CRYP_AESCTR_Decrypt_DMA(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData);

/**
  * @}
  */ 

/** @addtogroup CRYP_Exported_Functions_Group3 DMA callback functions
  * @{
  */

/* CallBack functions  ********************************************************/
void                  HAL_CRYP_InCpltCallback(CRYP_HandleTypeDef *hcryp);
void                  HAL_CRYP_OutCpltCallback(CRYP_HandleTypeDef *hcryp);
void                  HAL_CRYP_ErrorCallback(CRYP_HandleTypeDef *hcryp);

/**
  * @}
  */ 

/** @defgroup CRYP_Exported_Functions_Group4 CRYP IRQ handler
  * @{
  */

/* Processing functions  ********************************************************/
void                  HAL_CRYP_IRQHandler(CRYP_HandleTypeDef *hcryp);

/**
  * @}
  */ 

/** @defgroup CRYP_Exported_Functions_Group5 Peripheral State functions
  * @{
  */

/* Peripheral State functions  **************************************************/
HAL_CRYP_STATETypeDef HAL_CRYP_GetState(CRYP_HandleTypeDef *hcryp);

/**
  * @}
  */ 

/**
  * @}
  */ 

/* Define the private group ***********************************/
/**************************************************************/
/** @defgroup CRYP_Private CRYP Private
  * @{
  */
/**
  * @}
  */
/**************************************************************/


/**
  * @}
  */ 

/**
  * @}
  */ 
  
#endif /* STM32L021xx || STM32L041xx || STM32L062xx || STM32L063xx || STM32L081xx || STM32L082xx || STM32L083xx */
#ifdef __cplusplus
}
#endif

#endif /* __STM32L0xx_HAL_CRYP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

