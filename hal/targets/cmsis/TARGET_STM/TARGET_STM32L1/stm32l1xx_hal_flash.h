/**
  ******************************************************************************
  * @file    stm32l1xx_hal_flash.h
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    04-March-2016
  * @brief   Header file of Flash HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
#ifndef __STM32L1xx_HAL_FLASH_H
#define __STM32L1xx_HAL_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal_def.h"
   
/** @addtogroup STM32L1xx_HAL_Driver
  * @{
  */

/** @addtogroup FLASH
  * @{
  */
  
/** @addtogroup FLASH_Private_Constants
  * @{
  */
#define FLASH_TIMEOUT_VALUE   ((uint32_t)50000)/* 50 s */
/**
  * @}
  */

/** @addtogroup FLASH_Private_Macros
  * @{
  */

#define IS_FLASH_TYPEPROGRAM(_VALUE_)   (((_VALUE_) == FLASH_TYPEPROGRAM_WORD))

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
  FLASH_PROC_NONE              = 0, 
  FLASH_PROC_PAGEERASE         = 1,
  FLASH_PROC_PROGRAM           = 2,
} FLASH_ProcedureTypeDef;

/** 
  * @brief  FLASH handle Structure definition  
  */
typedef struct
{
  __IO FLASH_ProcedureTypeDef ProcedureOnGoing; /*!< Internal variable to indicate which procedure is ongoing or not in IT context */
  
  __IO uint32_t               NbPagesToErase;   /*!< Internal variable to save the remaining sectors to erase in IT context*/

  __IO uint32_t               Address;          /*!< Internal variable to save address selected for program or erase */

  __IO uint32_t               Page;             /*!< Internal variable to define the current page which is erasing */

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

#define HAL_FLASH_ERROR_NONE      ((uint32_t)0x00)  /*!< No error */
#define HAL_FLASH_ERROR_PGA       ((uint32_t)0x01)  /*!< Programming alignment error */
#define HAL_FLASH_ERROR_WRP       ((uint32_t)0x02)  /*!< Write protection error */
#define HAL_FLASH_ERROR_OPTV      ((uint32_t)0x04)  /*!< Option validity error */
#define HAL_FLASH_ERROR_SIZE      ((uint32_t)0x08)  /*!<  */
#define HAL_FLASH_ERROR_OPTVUSR   ((uint32_t)0x10)  /*!< Option UserValidity Error. */
#define HAL_FLASH_ERROR_RD        ((uint32_t)0x20)  /*!< Read protected error */
#define HAL_FLASH_ERROR_OPERATION ((uint32_t)0x40)  /*!< Not used */

/**
  * @}
  */

/** @defgroup FLASH_Page_Size FLASH Page Size
  * @{
  */ 

#define FLASH_PAGE_SIZE           ((uint32_t)256)

/**
  * @}
  */

/** @defgroup FLASH_Type_Program FLASH Type Program
  * @{
  */ 
#define FLASH_TYPEPROGRAM_WORD       ((uint32_t)0x02)  /*!<Program a word (32-bit) at a specified address.*/

/**
  * @}
  */

/** @defgroup FLASH_Latency FLASH Latency
  * @{
  */ 
#define FLASH_LATENCY_0            ((uint32_t)0x00000000)    /*!< FLASH Zero Latency cycle */
#define FLASH_LATENCY_1            FLASH_ACR_LATENCY         /*!< FLASH One Latency cycle */

/**
  * @}
  */

/** @defgroup FLASH_Interrupts FLASH Interrupts 
  * @{
  */
   
#define FLASH_IT_EOP               FLASH_PECR_EOPIE  /*!< End of programming interrupt source */
#define FLASH_IT_ERR               FLASH_PECR_ERRIE  /*!< Error interrupt source */
/**
  * @}
  */ 

/** @defgroup FLASH_Flags FLASH Flags 
  * @{
  */ 

#define FLASH_FLAG_BSY             FLASH_SR_BSY        /*!< FLASH Busy flag */
#define FLASH_FLAG_EOP             FLASH_SR_EOP        /*!< FLASH End of Programming flag */
#define FLASH_FLAG_ENDHV           FLASH_SR_ENDHV      /*!< FLASH End of High Voltage flag */
#define FLASH_FLAG_READY           FLASH_SR_READY      /*!< FLASH Ready flag after low power mode */
#define FLASH_FLAG_WRPERR          FLASH_SR_WRPERR     /*!< FLASH Write protected error flag */
#define FLASH_FLAG_PGAERR          FLASH_SR_PGAERR     /*!< FLASH Programming Alignment error flag */
#define FLASH_FLAG_SIZERR          FLASH_SR_SIZERR     /*!< FLASH Size error flag  */
#define FLASH_FLAG_OPTVERR         FLASH_SR_OPTVERR    /*!< FLASH Option Validity error flag  */

/**
  * @}
  */ 

/** @defgroup FLASH_Keys FLASH Keys 
  * @{
  */ 

#define FLASH_PDKEY1               ((uint32_t)0x04152637) /*!< Flash power down key1 */
#define FLASH_PDKEY2               ((uint32_t)0xFAFBFCFD) /*!< Flash power down key2: used with FLASH_PDKEY1 
                                                              to unlock the RUN_PD bit in FLASH_ACR */

#define FLASH_PEKEY1               ((uint32_t)0x89ABCDEF) /*!< Flash program erase key1 */
#define FLASH_PEKEY2               ((uint32_t)0x02030405) /*!< Flash program erase key: used with FLASH_PEKEY2
                                                               to unlock the write access to the FLASH_PECR register and
                                                               data EEPROM */

#define FLASH_PRGKEY1              ((uint32_t)0x8C9DAEBF) /*!< Flash program memory key1 */
#define FLASH_PRGKEY2              ((uint32_t)0x13141516) /*!< Flash program memory key2: used with FLASH_PRGKEY2
                                                               to unlock the program memory */

#define FLASH_OPTKEY1              ((uint32_t)0xFBEAD9C8) /*!< Flash option key1 */
#define FLASH_OPTKEY2              ((uint32_t)0x24252627) /*!< Flash option key2: used with FLASH_OPTKEY1 to
                                                              unlock the write access to the option byte block */
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
#define __HAL_FLASH_ENABLE_IT(__INTERRUPT__)  SET_BIT((FLASH->PECR), (__INTERRUPT__))

/**
  * @brief  Disable the specified FLASH interrupt.
  * @param  __INTERRUPT__  FLASH interrupt 
  *         This parameter can be any combination of the following values:
  *     @arg @ref FLASH_IT_EOP End of FLASH Operation Interrupt
  *     @arg @ref FLASH_IT_ERR Error Interrupt    
  * @retval none
  */  
#define __HAL_FLASH_DISABLE_IT(__INTERRUPT__)  CLEAR_BIT((FLASH->PECR), (uint32_t)(__INTERRUPT__))

/**
  * @brief  Get the specified FLASH flag status. 
  * @param  __FLAG__ specifies the FLASH flag to check.
  *          This parameter can be one of the following values:
  *            @arg @ref FLASH_FLAG_BSY         FLASH Busy flag
  *            @arg @ref FLASH_FLAG_EOP         FLASH End of Operation flag 
  *            @arg @ref FLASH_FLAG_ENDHV       FLASH End of High Voltage flag
  *            @arg @ref FLASH_FLAG_READY       FLASH Ready flag after low power mode
  *            @arg @ref FLASH_FLAG_PGAERR      FLASH Programming Alignment error flag
  *            @arg @ref FLASH_FLAG_SIZERR      FLASH Size error flag
  *            @arg @ref FLASH_FLAG_OPTVERR     FLASH Option validity error error flag
  *            @arg @ref FLASH_FLAG_OPTVERRUSR  FLASH Option UserValidity (available only Cat.3, Cat.4 and Cat.5 devices)
  *            @arg @ref FLASH_FLAG_RDERR       FLASH Read Protection error flag (PCROP) (available only Cat.2 and Cat.3 devices)
  *            @arg @ref FLASH_FLAG_WRPERR      FLASH Write protected error flag 
  * @retval The new state of __FLAG__ (SET or RESET).
  */
#define __HAL_FLASH_GET_FLAG(__FLAG__)   (((FLASH->SR) & (__FLAG__)) == (__FLAG__))

/**
  * @brief  Clear the specified FLASH flag.
  * @param  __FLAG__ specifies the FLASH flags to clear.
  *          This parameter can be any combination of the following values:
  *            @arg @ref FLASH_FLAG_EOP         FLASH End of Operation flag 
  *            @arg @ref FLASH_FLAG_ENDHV       FLASH End of High Voltage flag
  *            @arg @ref FLASH_FLAG_READY       FLASH Ready flag after low power mode
  *            @arg @ref FLASH_FLAG_PGAERR      FLASH Programming Alignment error flag
  *            @arg @ref FLASH_FLAG_SIZERR      FLASH Size error flag
  *            @arg @ref FLASH_FLAG_OPTVERR     FLASH Option validity error error flag
  *            @arg @ref FLASH_FLAG_OPTVERRUSR  FLASH Option UserValidity (available only Cat.3, Cat.4 and Cat.5 devices)
  *            @arg @ref FLASH_FLAG_RDERR       FLASH Read Protection error flag (PCROP) (available only Cat.2 and Cat.3 devices)
  *            @arg @ref FLASH_FLAG_WRPERR      FLASH Write protected error flag 
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
#include "stm32l1xx_hal_flash_ex.h"  
#include "stm32l1xx_hal_flash_ramfunc.h"  

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
void                    FLASH_PageErase(uint32_t PageAddress);
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

#endif /* __STM32L1xx_HAL_FLASH_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

