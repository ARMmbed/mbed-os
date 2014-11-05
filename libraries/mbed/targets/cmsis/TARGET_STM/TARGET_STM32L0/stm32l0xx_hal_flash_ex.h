/**
  ******************************************************************************
  * @file    stm32l0xx_hal_flash_ex.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   Header file of FLASH HAL Extension module.
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
#ifndef __STM32L0xx_HAL_FLASH_EX_H
#define __STM32L0xx_HAL_FLASH_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal_def.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @addtogroup FLASHEx
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/ 
/**
  * @brief  FLASH Advanced Option Bytes Program structure definition
  */
typedef struct
{
  uint32_t OptionType;     /*!< OptionType: Option byte to be configured for extension .
                              This parameter can be a value of @ref FLASHEx_Option_Type */

  uint32_t PCROPState;    /*!< PCROPState: PCROP activation or deactivation.
                              This parameter can be a value of @ref FLASHEx_PCROP_State */

  uint16_t Pages;       /*!< Sectors: specifies the sector(s) set for PCROP
                            This parameter can be a value of @ref FLASHEx_Option_Bytes_PC_ReadWrite_Protection */
 
  uint16_t BootConfig;         /*!< BootConfig: specifies Option bytes for boot config
                                This parameter can be a value of @ref FLASHEx_Option_Bytes_BOOT1 */
} FLASH_AdvOBProgramInitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup FLASHEx_Exported_Constants
  * @{
  */  
/** @defgroup FLASHEx_PCROP_State
  * @{
  */ 
#define PCROPSTATE_DISABLE       ((uint32_t)0x00)  /*!<Disable PCROP */
#define PCROPSTATE_ENABLE        ((uint32_t)0x01)  /*!<Enable PCROP  */
  
#define IS_PCROPSTATE(VALUE)(((VALUE) == PCROPSTATE_DISABLE) || \
                             ((VALUE) == PCROPSTATE_ENABLE))  
  
/**
  * @}
  */

/** @defgroup FLASHEx_Option_Type
  * @{
  */ 
#define OBEX_PCROP        ((uint32_t)0x01)  /*!<PCROP option byte configuration*/
#define OBEX_BOOTCONFIG   ((uint32_t)0x02)  /*!<BOOTConfig option byte configuration*/

#define IS_OBEX(VALUE)(((VALUE) == OBEX_PCROP) || \
                       ((VALUE) == OBEX_BOOTCONFIG))  
/**
  * @}
  */

/** @defgroup FLASHEx_Latency
  * @{
  */ 
#define FLASH_LATENCY_0                ((uint8_t)0x00)  /*!< FLASH Zero Latency cycle */
#define FLASH_LATENCY_1                FLASH_ACR_LATENCY  /*!< FLASH One Latency cycle */

#define IS_FLASH_LATENCY(LATENCY) (((LATENCY) == FLASH_LATENCY_0) || \
                                   ((LATENCY) == FLASH_LATENCY_1))

/**
  * @}
  */ 
/** @defgroup FLASHEx_Option_Bytes_PC_ReadWrite_Protection
  * @{
  */
#define OB_PCROP_Pages0to31              ((uint32_t)0x00000001) /* PC Read/Write  protection of Sector0 */
#define OB_PCROP_Pages32to63             ((uint32_t)0x00000002) /* PC Read/Write  protection of Sector1 */
#define OB_PCROP_Pages64to95             ((uint32_t)0x00000004) /* PC Read/Write  protection of Sector2 */
#define OB_PCROP_Pages96to127            ((uint32_t)0x00000008) /* PC Read/Write  protection of Sector3 */
#define OB_PCROP_Pages128to159           ((uint32_t)0x00000010) /* PC Read/Write  protection of Sector4 */
#define OB_PCROP_Pages160to191           ((uint32_t)0x00000020) /* PC Read/Write  protection of Sector5 */
#define OB_PCROP_Pages192to223           ((uint32_t)0x00000040) /* PC Read/Write  protection of Sector6 */
#define OB_PCROP_Pages224to255           ((uint32_t)0x00000080) /* PC Read/Write  protection of Sector7 */
#define OB_PCROP_Pages256to287           ((uint32_t)0x00000100) /* PC Read/Write  protection of Sector8 */
#define OB_PCROP_Pages288to319           ((uint32_t)0x00000200) /* PC Read/Write  protection of Sector9 */
#define OB_PCROP_Pages320to351           ((uint32_t)0x00000400) /* PC Read/Write  protection of Sector10 */
#define OB_PCROP_Pages352to383           ((uint32_t)0x00000800) /* PC Read/Write  protection of Sector11 */
#define OB_PCROP_Pages384to415           ((uint32_t)0x00001000) /* PC Read/Write  protection of Sector12 */
#define OB_PCROP_Pages416to447           ((uint32_t)0x00002000) /* PC Read/Write  protection of Sector13 */
#define OB_PCROP_Pages448to479           ((uint32_t)0x00004000) /* PC Read/Write  protection of Sector14 */
#define OB_PCROP_Pages480to511           ((uint32_t)0x00008000) /* PC Read/Write  protection of Sector15 */
#define OB_PCROP_AllPages                ((uint32_t)0x0000FFFF) /*!< PC Read/Write  protection of all Sectors */

#define IS_OB_PCROP(PAGE) (((PAGE) != 0x0000000))

/**
  * @}
  */
  
/** @defgroup FLASHEx_Option_Bytes_BOOT1
  * @{
  */

#define OB_BOOT1_RESET                 ((uint16_t)0x0000) /*!< BOOT1 Reset */
#define OB_BOOT1_SET                   ((uint16_t)0x8000) /*!< BOOT1 Set */
#define IS_OB_BOOT1(BOOT1) (((BOOT1) == OB_BOOT1_RESET) || ((BOOT1) == OB_BOOT1_SET))

/**
  * @}
  */

/** @defgroup  FLASHEx_Selection_Protection_Mode
  * @{
  */
#define OB_PCROP_DESELECTED     ((uint16_t)0x0000) /*!< Disabled PcROP, nWPRi bits used for Write Protection on sector i */
#define OB_PCROP_SELECTED       ((uint16_t)0x0100) /*!< Enable PcROP, nWPRi bits used for PCRoP Protection on sector i   */
#define IS_OB_PCROP_SELECT(PCROP) (((PCROP) == OB_PCROP_SELECTED) || ((PCROP) == OB_PCROP_DESELECTED))
/**
  * @}
  */
    
/**
  * @}
  */
    
    
/* Exported macro ------------------------------------------------------------*/
/** @defgroup FLASHEx_Macros
 *  @brief macros to control FLASH features 
 *  @{
 */
 
/**
  * @brief  Set the FLASH Latency.
  * @param  __LATENCY__: FLASH Latency                   
  *         The value of this parameter depend on device used within the same series
  * @retval none
  */ 
#define __HAL_FLASH_SET_LATENCY(__LATENCY__) \
                  MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, (uint32_t)(__LATENCY__))

/**
  * @brief  Enable the FLASH prefetch buffer.
  * @retval none
  */ 
#define __HAL_FLASH_PREFETCH_BUFFER_ENABLE()  (FLASH->ACR |= FLASH_ACR_PRFTEN)

/**
  * @brief  Disable the FLASH prefetch buffer.
  * @retval none
  */ 
#define __HAL_FLASH_PREFETCH_BUFFER_DISABLE()   (FLASH->ACR &= (~FLASH_ACR_PRFTEN))

/**
  * @brief  Enable the FLASH Buffer cache.
  * @retval none
  */ 
#define __HAL_FLASH_BUFFER_CACHE_ENABLE()  (FLASH->ACR &= (~FLASH_ACR_DISAB_BUF))

/**
  * @brief  Disable the FLASH Buffer cache.
  * @retval none
  */ 
#define __HAL_FLASH_BUFFER_CACHE_DISABLE()   (FLASH->ACR |= FLASH_ACR_DISAB_BUF)

/**
  * @brief  Enable the FLASH preread buffer
  * @retval none
  */ 
#define __HAL_FLASH_PREREAD_BUFFER_ENABLE()  (FLASH->ACR |= FLASH_ACR_PRE_READ)

/**
  * @brief  Disable the FLASH preread buffer
  * @retval none
  */ 
#define __HAL_FLASH_PREREAD_BUFFER_DISABLE()   (FLASH->ACR &= (~FLASH_ACR_PRE_READ))

/**
  * @brief  Enable the FLASH power down during Sleep mode
  * @retval none
  */ 
#define __HAL_FLASH_SLEEP_POWERDOWN_ENABLE()   SET_BIT(FLASH->ACR, FLASH_ACR_SLEEP_PD)

/**
  * @brief  Disable the FLASH power down during Sleep mode
  * @retval none
  */ 
#define __HAL_FLASH_SLEEP_POWERDOWN_DISABLE()   CLEAR_BIT(FLASH->ACR, FLASH_ACR_SLEEP_PD)

/**
  * @brief  Macro to enable or disable the Flash Run power down mode.
  * @note   Writing this bit  to 0 this bit, automatically the keys are
  *         loss and a new unlock sequence is necessary to re-write it to 1.
  */

#define __HAL_FLASH_POWER_DOWN_ENABLE() do { FLASH->PDKEYR = FLASH_PDKEY1;    \
                                             FLASH->PDKEYR = FLASH_PDKEY2;    \
                                             SET_BIT(FLASH->ACR, FLASH_ACR_RUN_PD);  \
                                           } while (0)

#define __HAL_FLASH_POWER_DOWN_DISABLE() do { FLASH->PDKEYR = FLASH_PDKEY1;    \
                                              FLASH->PDKEYR = FLASH_PDKEY2;    \
                                             CLEAR_BIT(FLASH->ACR, FLASH_ACR_RUN_PD);  \
                                            } while (0)
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/* I/O operation functions  *****************************************************/
/* Peripheral Control functions  ************************************************/
HAL_StatusTypeDef HAL_FLASHEx_DATAEEPROM_Unlock(void);
HAL_StatusTypeDef HAL_FLASHEx_DATAEEPROM_Lock(void);
HAL_StatusTypeDef HAL_FLASHEx_DATAEEPROM_Erase(uint32_t Address);
HAL_StatusTypeDef HAL_FLASHEx_DATAEEPROM_Program(uint32_t TypeProgram, uint32_t Address, uint32_t Data);

/* Aliases for legacy HAL versions compatibility */
#define  HAL_DATA_EEPROMEx_Unlock   HAL_FLASHEx_DATAEEPROM_Unlock
#define  HAL_DATA_EEPROMEx_Lock     HAL_FLASHEx_DATAEEPROM_Lock
#define  HAL_DATA_EEPROMEx_Erase    HAL_FLASHEx_DATAEEPROM_Erase
#define  HAL_DATA_EEPROMEx_Program  HAL_FLASHEx_DATAEEPROM_Program

HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError);
HAL_StatusTypeDef HAL_FLASHEx_Erase_IT(FLASH_EraseInitTypeDef *pEraseInit);

HAL_StatusTypeDef HAL_FLASHEx_OB_SelectPCROP(void);
HAL_StatusTypeDef HAL_FLASHEx_OB_DeSelectPCROP(void);

HAL_StatusTypeDef HAL_FLASHEx_OBProgram(FLASH_OBProgramInitTypeDef *pOBInit);
void HAL_FLASHEx_OBGetConfig(FLASH_OBProgramInitTypeDef *pOBInit);

HAL_StatusTypeDef HAL_FLASHEx_AdvOBProgram (FLASH_AdvOBProgramInitTypeDef *pAdvOBInit);
void  HAL_FLASHEx_AdvOBGetConfig(FLASH_AdvOBProgramInitTypeDef *pAdvOBInit);



/**
  * @}
  */ 

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32L0xx_HAL_FLASH_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
