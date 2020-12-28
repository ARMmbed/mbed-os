/**
  ******************************************************************************
  * @file    stm32f0xx_hal_flash.h
  * @author  MCD Application Team
  * @brief   Header file of Flash HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
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
#ifndef __STM32F0xx_HAL_FLASH_H
#define __STM32F0xx_HAL_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal_def.h"
   
/** @addtogroup STM32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup FLASH
  * @{
  */
  
/** @addtogroup FLASH_Private_Constants
  * @{
  */
#define FLASH_TIMEOUT_VALUE      (50000U) /* 50 s */
/**
  * @}
  */

/** @addtogroup FLASH_Private_Macros
  * @{
  */

#define IS_FLASH_TYPEPROGRAM(VALUE)  (((VALUE) == FLASH_TYPEPROGRAM_HALFWORD) || \
                                      ((VALUE) == FLASH_TYPEPROGRAM_WORD)     || \
                                      ((VALUE) == FLASH_TYPEPROGRAM_DOUBLEWORD))  

#define IS_FLASH_LATENCY(__LATENCY__) (((__LATENCY__) == FLASH_LATENCY_0) || \
                                       ((__LATENCY__) == FLASH_LATENCY_1))

/**
  * @}
  */  

/* Exported types ------------------------------------------------------------*/ 
/** @defgroup FLASH_Exported_Types FLASH Exported Types
  * @{
  */  

/**
  * @brief  FLASH Procedure structure definition
  */
typedef enum 
{
  FLASH_PROC_NONE              = 0U, 
  FLASH_PROC_PAGEERASE         = 1U,
  FLASH_PROC_MASSERASE         = 2U,
  FLASH_PROC_PROGRAMHALFWORD   = 3U,
  FLASH_PROC_PROGRAMWORD       = 4U,
  FLASH_PROC_PROGRAMDOUBLEWORD = 5U
} FLASH_ProcedureTypeDef;

/** 
  * @brief  FLASH handle Structure definition  
  */
typedef struct
{
  __IO FLASH_ProcedureTypeDef ProcedureOnGoing; /*!< Internal variable to indicate which procedure is ongoing or not in IT context */
  
  __IO uint32_t               DataRemaining;    /*!< Internal variable to save the remaining pages to erase or half-word to program in IT context */

  __IO uint32_t               Address;          /*!< Internal variable to save address selected for program or erase */

  __IO uint64_t               Data;             /*!< Internal variable to save data to be programmed */

  HAL_LockTypeDef             Lock;             /*!< FLASH locking object                */

  __IO uint32_t               ErrorCode;        /*!< FLASH error code                    
                                                     This parameter can be a value of @ref FLASH_Error_Codes  */
} FLASH_ProcessTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup FLASH_Exported_Constants FLASH Exported Constants
  * @{
  */  

/** @defgroup FLASH_Error_Codes FLASH Error Codes
  * @{
  */

#define HAL_FLASH_ERROR_NONE      0x00U  /*!< No error */
#define HAL_FLASH_ERROR_PROG      0x01U  /*!< Programming error */
#define HAL_FLASH_ERROR_WRP       0x02U  /*!< Write protection error */

/**
  * @}
  */

/** @defgroup FLASH_Type_Program FLASH Type Program
  * @{
  */ 
#define FLASH_TYPEPROGRAM_HALFWORD   (0x01U)  /*!<Program a half-word (16-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_WORD       (0x02U)  /*!<Program a word (32-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_DOUBLEWORD (0x03U)  /*!<Program a double word (64-bit) at a specified address*/

/**
  * @}
  */

/** @defgroup FLASH_Latency FLASH Latency
  * @{
  */ 
#define FLASH_LATENCY_0            (0x00000000U)    /*!< FLASH Zero Latency cycle */
#define FLASH_LATENCY_1            FLASH_ACR_LATENCY         /*!< FLASH One Latency cycle */

/**
  * @}
  */


/** @defgroup FLASH_Flag_definition FLASH Flag definition
  * @{
  */ 
#define FLASH_FLAG_BSY             FLASH_SR_BSY            /*!< FLASH Busy flag                           */ 
#define FLASH_FLAG_PGERR           FLASH_SR_PGERR          /*!< FLASH Programming error flag    */
#define FLASH_FLAG_WRPERR          FLASH_SR_WRPERR         /*!< FLASH Write protected error flag          */
#define FLASH_FLAG_EOP             FLASH_SR_EOP            /*!< FLASH End of Operation flag               */
/**
  * @}
  */
  
/** @defgroup FLASH_Interrupt_definition FLASH Interrupt definition
  * @{
  */ 
#define FLASH_IT_EOP               FLASH_CR_EOPIE          /*!< End of FLASH Operation Interrupt source */
#define FLASH_IT_ERR               FLASH_CR_ERRIE  /*!< Error Interrupt source */
/**
  * @}
  */  

/**
  * @}
  */  
  
/* Exported macro ------------------------------------------------------------*/

/** @defgroup FLASH_Exported_Macros FLASH Exported Macros
 *  @brief macros to control FLASH features 
 *  @{
 */
 

/** @defgroup FLASH_EM_Latency FLASH Latency
 *  @brief macros to handle FLASH Latency
 * @{
 */ 
  
/**
  * @brief  Set the FLASH Latency.
  * @param  __LATENCY__ FLASH Latency                   
  *         The value of this parameter depend on device used within the same series
  * @retval None
  */ 
#define __HAL_FLASH_SET_LATENCY(__LATENCY__)    (FLASH->ACR = (FLASH->ACR&(~FLASH_ACR_LATENCY)) | (__LATENCY__))


/**
  * @brief  Get the FLASH Latency.
  * @retval FLASH Latency                   
  *         The value of this parameter depend on device used within the same series
  */ 
#define __HAL_FLASH_GET_LATENCY()     (READ_BIT((FLASH->ACR), FLASH_ACR_LATENCY))

/**
  * @}
  */

/** @defgroup FLASH_Prefetch FLASH Prefetch
 *  @brief macros to handle FLASH Prefetch buffer
 * @{
 */   
/**
  * @brief  Enable the FLASH prefetch buffer.
  * @retval None
  */ 
#define __HAL_FLASH_PREFETCH_BUFFER_ENABLE()    (FLASH->ACR |= FLASH_ACR_PRFTBE)

/**
  * @brief  Disable the FLASH prefetch buffer.
  * @retval None
  */
#define __HAL_FLASH_PREFETCH_BUFFER_DISABLE()   (FLASH->ACR &= (~FLASH_ACR_PRFTBE))

/**
  * @}
  */
  
/** @defgroup FLASH_Interrupt FLASH Interrupts
 *  @brief macros to handle FLASH interrupts
 * @{
 */ 

/**
  * @brief  Enable the specified FLASH interrupt.
  * @param  __INTERRUPT__  FLASH interrupt 
  *         This parameter can be any combination of the following values:
  *     @arg @ref FLASH_IT_EOP End of FLASH Operation Interrupt
  *     @arg @ref FLASH_IT_ERR Error Interrupt    
  * @retval none
  */  
#define __HAL_FLASH_ENABLE_IT(__INTERRUPT__)  SET_BIT((FLASH->CR), (__INTERRUPT__))

/**
  * @brief  Disable the specified FLASH interrupt.
  * @param  __INTERRUPT__  FLASH interrupt 
  *         This parameter can be any combination of the following values:
  *     @arg @ref FLASH_IT_EOP End of FLASH Operation Interrupt
  *     @arg @ref FLASH_IT_ERR Error Interrupt    
  * @retval none
  */  
#define __HAL_FLASH_DISABLE_IT(__INTERRUPT__)  CLEAR_BIT((FLASH->CR), (uint32_t)(__INTERRUPT__))

/**
  * @brief  Get the specified FLASH flag status. 
  * @param  __FLAG__ specifies the FLASH flag to check.
  *          This parameter can be one of the following values:
  *            @arg @ref FLASH_FLAG_BSY         FLASH Busy flag
  *            @arg @ref FLASH_FLAG_EOP         FLASH End of Operation flag 
  *            @arg @ref FLASH_FLAG_WRPERR      FLASH Write protected error flag 
  *            @arg @ref FLASH_FLAG_PGERR       FLASH Programming error flag
  * @retval The new state of __FLAG__ (SET or RESET).
  */
#define __HAL_FLASH_GET_FLAG(__FLAG__)   (((FLASH->SR) & (__FLAG__)) == (__FLAG__))

/**
  * @brief  Clear the specified FLASH flag.
  * @param  __FLAG__ specifies the FLASH flags to clear.
  *          This parameter can be any combination of the following values:
  *            @arg @ref FLASH_FLAG_EOP         FLASH End of Operation flag 
  *            @arg @ref FLASH_FLAG_WRPERR      FLASH Write protected error flag 
  *            @arg @ref FLASH_FLAG_PGERR       FLASH Programming error flag
  * @retval none
  */
#define __HAL_FLASH_CLEAR_FLAG(__FLAG__)   ((FLASH->SR) = (__FLAG__))

/**
  * @}
  */ 

/**
  * @}
  */ 

/* Include FLASH HAL Extended module */
#include "stm32f0xx_hal_flash_ex.h"  

/* Exported functions --------------------------------------------------------*/
/** @addtogroup FLASH_Exported_Functions
  * @{
  */
  
/** @addtogroup FLASH_Exported_Functions_Group1
  * @{
  */
/* IO operation functions *****************************************************/
HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);
HAL_StatusTypeDef HAL_FLASH_Program_IT(uint32_t TypeProgram, uint32_t Address, uint64_t Data);

/* FLASH IRQ handler function */
void       HAL_FLASH_IRQHandler(void);
/* Callbacks in non blocking modes */ 
void       HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue);
void       HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue);

/**
  * @}
  */

/** @addtogroup FLASH_Exported_Functions_Group2
  * @{
  */
/* Peripheral Control functions ***********************************************/
HAL_StatusTypeDef HAL_FLASH_Unlock(void);
HAL_StatusTypeDef HAL_FLASH_Lock(void);
HAL_StatusTypeDef HAL_FLASH_OB_Unlock(void);
HAL_StatusTypeDef HAL_FLASH_OB_Lock(void);
HAL_StatusTypeDef HAL_FLASH_OB_Launch(void);

/**
  * @}
  */

/** @addtogroup FLASH_Exported_Functions_Group3
  * @{
  */
/* Peripheral State and Error functions ***************************************/
uint32_t HAL_FLASH_GetError(void);

/**
  * @}
  */

/**
  * @}
  */

/* Private function -------------------------------------------------*/
/** @addtogroup FLASH_Private_Functions
 * @{
 */
HAL_StatusTypeDef       FLASH_WaitForLastOperation(uint32_t Timeout);

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

#endif /* __STM32F0xx_HAL_FLASH_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

