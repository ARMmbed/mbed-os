/**
  ******************************************************************************
  * @file    stm32l0xx_hal_cryp.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   Header file of CRYP HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L0xx_HAL_CRYP_H
#define __STM32L0xx_HAL_CRYP_H

#ifdef __cplusplus
 extern "C" {
#endif
   
#if !defined (STM32L051xx) && !defined (STM32L052xx) && !defined (STM32L053xx)

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal_def.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @addtogroup CRYP
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/ 

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
  HAL_CRYP_STATE_RESET             = 0x00,  /*!< CRYP not yet initialized or disabled  */
  HAL_CRYP_STATE_READY             = 0x01,  /*!< CRYP initialized and ready for use    */
  HAL_CRYP_STATE_BUSY              = 0x02,  /*!< CRYP internal processing is ongoing   */
  HAL_CRYP_STATE_TIMEOUT           = 0x03,  /*!< CRYP timeout state                    */
  HAL_CRYP_STATE_ERROR             = 0x04   /*!< CRYP error state                      */ 
    
}HAL_CRYP_STATETypeDef;

/** 
  * @brief HAL CRYP phase structures definition  
  */ 
typedef enum
{
  HAL_CRYP_PHASE_READY             = 0x01,    /*!< CRYP peripheral is ready for initialization. */
  HAL_CRYP_PHASE_PROCESS           = 0x02,    /*!< CRYP peripheral is in processing phase */
}HAL_PhaseTypeDef;

/** 
  * @brief  CRYP handle Structure definition  
  */ 
typedef struct
{   
      CRYP_InitTypeDef         Init;             /*!< CRYP required parameters */
  
      uint8_t                  *pCrypInBuffPtr;  /*!< Pointer to CRYP processing (encryption, decryption,...) buffer */
  
      uint8_t                  *pCrypOutBuffPtr; /*!< Pointer to CRYP processing (encryption, decryption,...) buffer */
  
      __IO uint16_t            CrypInCount;      /*!< Counter of inputed data */
                          
      __IO uint16_t            CrypOutCount;     /*!< Counter of outputed data */
  
      HAL_StatusTypeDef        Status;           /*!< CRYP peripheral status */
  
      HAL_PhaseTypeDef         Phase;            /*!< CRYP peripheral phase */
  
      DMA_HandleTypeDef        *hdmain;          /*!< CRYP In DMA handle parameters */
  
      DMA_HandleTypeDef        *hdmaout;         /*!< CRYP Out DMA handle parameters */
  
      HAL_LockTypeDef          Lock;             /*!< CRYP locking object */
  
   __IO  HAL_CRYP_STATETypeDef State;            /*!< CRYP peripheral state */
  
}CRYP_HandleTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup CRYP_Exported_Constants
  * @{
  */

/** @defgroup CRYP_Data_Type 
  * @{
  */
#define CRYP_DATATYPE_32B         ((uint32_t)0x00000000)
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

/** @defgroup CRYP_AlgoModeDirection
  * @{
  */ 
#define CRYP_CR_ALGOMODE_DIRECTION              (uint32_t)(AES_CR_MODE|AES_CR_CHMOD)

#define CRYP_CR_ALGOMODE_AES_ECB_ENCRYPT        ((uint32_t)0x00000000)
#define CRYP_CR_ALGOMODE_AES_ECB_KEYDERDECRYPT  (AES_CR_MODE)
#define CRYP_CR_ALGOMODE_AES_CBC_ENCRYPT        (AES_CR_CHMOD_0)
#define CRYP_CR_ALGOMODE_AES_CBC_KEYDERDECRYPT  ((uint32_t)(AES_CR_CHMOD_0|AES_CR_MODE))
#define CRYP_CR_ALGOMODE_AES_CTR_ENCRYPT        (AES_CR_CHMOD_1)
#define CRYP_CR_ALGOMODE_AES_CTR_DECRYPT        ((uint32_t)(AES_CR_CHMOD_1 | AES_CR_MODE_1))
/**
  * @}
  */
  
/** @defgroup AES_Interrupts
  * @{
  */ 
#define AES_IT_CC                          AES_CR_CCIE  /*!< Computation Complete interrupt */
#define AES_IT_ERR                         AES_CR_ERRIE /*!< Error interrupt                */

#define IS_AES_IT(IT) ((((IT) & (uint32_t)0xFFFFF9FF) == 0x00000000) && ((IT) != 0x00000000))
#define IS_AES_GET_IT(IT) (((IT) == AES_IT_CC) || ((IT) == AES_IT_ERR))

/**
  * @}
  */


/** @defgroup AES_Flags
  * @{
  */ 
#define AES_FLAG_CCF                       AES_SR_CCF    /*!< Computation Complete Flag */
#define AES_FLAG_RDERR                     AES_SR_RDERR  /*!< Read Error Flag           */
#define AES_FLAG_WRERR                     AES_SR_WRERR  /*!< Write Error Flag          */

#define IS_AES_FLAG(FLAG) (((FLAG) == AES_FLAG_CCF)    || \
                           ((FLAG) == AES_FLAG_RDERR)  || \
                           ((FLAG) == AES_FLAG_WRERR))
/**
  * @}
  */ 

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/** @brief Reset CRYP handle state
  * @param  __HANDLE__: specifies the CRYP Handle.
  * @retval None
  */
#define __HAL_CRYP_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_CRYP_STATE_RESET)

/**
  * @brief  Enable/Disable the CRYP peripheral.
  * @param  None
  * @retval None
  */
#define __HAL_CRYP_ENABLE()  (AES->CR |=  AES_CR_EN)
#define __HAL_CRYP_DISABLE() (AES->CR &=  ~AES_CR_EN)

/**
  * @brief  Set the algorithm mode: AES-ECB, AES-CBC, AES-CTR, DES-ECB, DES-CBC,...
  * @param  MODE: The algorithm mode.
  * @retval None
  */
#define __HAL_CRYP_SET_MODE(MODE)  (AES->CR |= (uint32_t)(MODE))


/** @brief  Check whether the specified CRYP flag is set or not.
  * @param  __FLAG__: specifies the flag to check.
  *         This parameter can be one of the following values:
  *            @arg AES_FLAG_CCF   : Computation Complete Flag
  *            @arg AES_FLAG_RDERR : Read Error Flag
  *            @arg AES_FLAG_WRERR : Write Error Flag
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_CRYP_GET_FLAG(__FLAG__) ((AES->SR & (__FLAG__)) == (__FLAG__))
   
/**
  * @brief  Enable the CRYP interrupt.
  * @param  __INTERRUPT__: CRYP Interrupt.
  * @retval None
  */
#define __HAL_CRYP_ENABLE_IT(__INTERRUPT__) ((AES->CR) |= (__INTERRUPT__))

/**
  * @brief  Disable the CRYP interrupt.
  * @param  __INTERRUPT__: CRYP interrupt.
  * @retval None
  */
#define __HAL_CRYP_DISABLE_IT(__INTERRUPT__) ((AES->CR) &= ~(__INTERRUPT__))

/* Include CRYP HAL Extension module */
#include "stm32l0xx_hal_cryp_ex.h"

/* Exported functions --------------------------------------------------------*/

/* Initialization/de-initialization functions *********************************/
HAL_StatusTypeDef HAL_CRYP_Init(CRYP_HandleTypeDef *hcryp);
HAL_StatusTypeDef HAL_CRYP_DeInit(CRYP_HandleTypeDef *hcryp);

/* AES encryption/decryption using polling  ***********************************/
HAL_StatusTypeDef HAL_CRYP_AESECB_Encrypt(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData, uint32_t Timeout);
HAL_StatusTypeDef HAL_CRYP_AESECB_Decrypt(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData, uint32_t Timeout);
HAL_StatusTypeDef HAL_CRYP_AESCBC_Encrypt(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData, uint32_t Timeout);
HAL_StatusTypeDef HAL_CRYP_AESCBC_Decrypt(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData, uint32_t Timeout);
HAL_StatusTypeDef HAL_CRYP_AESCTR_Encrypt(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData, uint32_t Timeout);
HAL_StatusTypeDef HAL_CRYP_AESCTR_Decrypt(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData, uint32_t Timeout);

/* AES encryption/decryption using interrupt  *********************************/
HAL_StatusTypeDef HAL_CRYP_AESECB_Encrypt_IT(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData);
HAL_StatusTypeDef HAL_CRYP_AESCBC_Encrypt_IT(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData);
HAL_StatusTypeDef HAL_CRYP_AESCTR_Encrypt_IT(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData);
HAL_StatusTypeDef HAL_CRYP_AESECB_Decrypt_IT(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData);
HAL_StatusTypeDef HAL_CRYP_AESCTR_Decrypt_IT(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData);
HAL_StatusTypeDef HAL_CRYP_AESCBC_Decrypt_IT(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData);

/* AES encryption/decryption using DMA  ***************************************/
HAL_StatusTypeDef HAL_CRYP_AESECB_Encrypt_DMA(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData);
HAL_StatusTypeDef HAL_CRYP_AESECB_Decrypt_DMA(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData);
HAL_StatusTypeDef HAL_CRYP_AESCBC_Encrypt_DMA(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData);
HAL_StatusTypeDef HAL_CRYP_AESCBC_Decrypt_DMA(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData);
HAL_StatusTypeDef HAL_CRYP_AESCTR_Encrypt_DMA(CRYP_HandleTypeDef *hcryp, uint8_t *pPlainData, uint16_t Size, uint8_t *pCypherData);
HAL_StatusTypeDef HAL_CRYP_AESCTR_Decrypt_DMA(CRYP_HandleTypeDef *hcryp, uint8_t *pCypherData, uint16_t Size, uint8_t *pPlainData);

/* Processing functions  ********************************************************/
void HAL_CRYP_IRQHandler(CRYP_HandleTypeDef *hcryp);

/* Peripheral State functions  **************************************************/
HAL_CRYP_STATETypeDef HAL_CRYP_GetState(CRYP_HandleTypeDef *hcryp);

/* MSP functions  *************************************************************/
void HAL_CRYP_MspInit(CRYP_HandleTypeDef *hcryp);
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef *hcryp);

/* CallBack functions  ********************************************************/
void HAL_CRYP_ComputationCpltCallback(CRYP_HandleTypeDef *hcryp);
void HAL_CRYP_InCpltCallback(CRYP_HandleTypeDef *hcryp);
void HAL_CRYP_OutCpltCallback(CRYP_HandleTypeDef *hcryp);
void HAL_CRYP_ErrorCallback(CRYP_HandleTypeDef *hcryp);

/* Aliases for inter STM32 series compatibility */
#define HAL_CRYP_ComputationCpltCallback         HAL_CRYPEx_ComputationCpltCallback

#endif /* STM32L051xx && STM32L052xx && STM32L053xx*/

/**
  * @}
  */ 

/**
  * @}
  */ 
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32L0xx_HAL_CRYP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
