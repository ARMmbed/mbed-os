/**
  ******************************************************************************
  * @file    stm32f0xx_hal_irda_ex.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    28-May-2014
  * @brief   Header file of IRDA HAL Extension module.
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
#ifndef __STM32F0xx_HAL_IRDA_EX_H
#define __STM32F0xx_HAL_IRDA_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

#if !defined(STM32F030x6) && !defined(STM32F030x8) 

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal_def.h"

/** @addtogroup STM32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup IRDAEx
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup IRDAEx_Exported_Constants
  * @{
  */
  
/** @defgroup IRDAEx_Word_Length IRDA Word Length
  * @{
  */
#if defined (STM32F042x6) || defined (STM32F048xx) || \
    defined (STM32F071xB) || defined (STM32F072xB) || defined (STM32F078xx)
#define IRDA_WORDLENGTH_7B                  ((uint32_t)USART_CR1_M1)
#define IRDA_WORDLENGTH_8B                  ((uint32_t)0x00000000)
#define IRDA_WORDLENGTH_9B                  ((uint32_t)USART_CR1_M0)
#define IS_IRDA_WORD_LENGTH(LENGTH) (((LENGTH) == IRDA_WORDLENGTH_7B) || \
                                     ((LENGTH) == IRDA_WORDLENGTH_8B) || \
                                     ((LENGTH) == IRDA_WORDLENGTH_9B))
#else
#define IRDA_WORDLENGTH_8B                  ((uint32_t)0x00000000)
#define IRDA_WORDLENGTH_9B                  ((uint32_t)USART_CR1_M)
#define IS_IRDA_WORD_LENGTH(LENGTH) (((LENGTH) == IRDA_WORDLENGTH_8B) || \
                                     ((LENGTH) == IRDA_WORDLENGTH_9B))
#endif /* defined (STM32F042x6) || defined (STM32F048xx) || \
          defined (STM32F071xB) || defined (STM32F072xB) || defined (STM32F078xx) */
/**
  * @}
  */
  
  
/**
  * @}
  */  
  
/* Exported macros -----------------------------------------------------------*/

/** @defgroup IRDAEx_Exported_Macros
  * @{
  */
  
/** @brief  Computes the mask to apply to retrieve the received data
  *         according to the word length and to the parity bits activation.
  * @param  __HANDLE__: specifies the IRDA Handle
  * @retval none
  */  
#if defined (STM32F042x6) || defined (STM32F048xx) || \
    defined (STM32F071xB) || defined (STM32F072xB) || defined (STM32F078xx)
#define __HAL_IRDA_MASK_COMPUTATION(__HANDLE__)                       \
  do {                                                                \
  if ((__HANDLE__)->Init.WordLength == IRDA_WORDLENGTH_9B)            \
  {                                                                   \
     if ((__HANDLE__)->Init.Parity == IRDA_PARITY_NONE)               \
     {                                                                \
        (__HANDLE__)->Mask = 0x01FF ;                                 \
     }                                                                \
     else                                                             \
     {                                                                \
        (__HANDLE__)->Mask = 0x00FF ;                                 \
     }                                                                \
  }                                                                   \
  else if ((__HANDLE__)->Init.WordLength == IRDA_WORDLENGTH_8B)       \
  {                                                                   \
     if ((__HANDLE__)->Init.Parity == IRDA_PARITY_NONE)               \
     {                                                                \
        (__HANDLE__)->Mask = 0x00FF ;                                 \
     }                                                                \
     else                                                             \
     {                                                                \
        (__HANDLE__)->Mask = 0x007F ;                                 \
     }                                                                \
  }                                                                   \
  else if ((__HANDLE__)->Init.WordLength == IRDA_WORDLENGTH_7B)       \
  {                                                                   \
     if ((__HANDLE__)->Init.Parity == IRDA_PARITY_NONE)               \
     {                                                                \
        (__HANDLE__)->Mask = 0x007F ;                                 \
     }                                                                \
     else                                                             \
     {                                                                \
        (__HANDLE__)->Mask = 0x003F ;                                 \
     }                                                                \
  }                                                                   \
} while(0) 
#else
#define __HAL_IRDA_MASK_COMPUTATION(__HANDLE__)                       \
  do {                                                                \
  if ((__HANDLE__)->Init.WordLength == IRDA_WORDLENGTH_9B)            \
  {                                                                   \
     if ((__HANDLE__)->Init.Parity == IRDA_PARITY_NONE)               \
     {                                                                \
        (__HANDLE__)->Mask = 0x01FF ;                                 \
     }                                                                \
     else                                                             \
     {                                                                \
        (__HANDLE__)->Mask = 0x00FF ;                                 \
     }                                                                \
  }                                                                   \
  else if ((__HANDLE__)->Init.WordLength == IRDA_WORDLENGTH_8B)       \
  {                                                                   \
     if ((__HANDLE__)->Init.Parity == IRDA_PARITY_NONE)               \
     {                                                                \
        (__HANDLE__)->Mask = 0x00FF ;                                 \
     }                                                                \
     else                                                             \
     {                                                                \
        (__HANDLE__)->Mask = 0x007F ;                                 \
     }                                                                \
  }                                                                   \
} while(0) 
#endif /* defined (STM32F042x6) || defined (STM32F048xx) || \
          defined (STM32F071xB) || defined (STM32F072xB) || defined (STM32F078xx) */
/**
  * @}
  */
  
/* Exported functions --------------------------------------------------------*/
/* Initialization and de-initialization functions  ****************************/
/* IO operation functions *****************************************************/
/* Peripheral Control functions ***********************************************/
/* Peripheral State and Error functions ***************************************/


/**
  * @}
  */ 

/**
  * @}
  */ 

#endif /* !defined(STM32F030x6) && !defined(STM32F030x8) */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32F0xx_HAL_IRDA_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
