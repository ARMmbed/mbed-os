/**
  ******************************************************************************
  * @file    stm32l0xx_hal_flash.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   This file contains all the functions prototypes for the FLASH 
  *          firmware library.
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
#ifndef __STM32L0XX_HAL_FLASH_H
#define __STM32L0XX_HAL_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal_def.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @addtogroup FLASH
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  FLASH Error structure definition
  */
typedef enum
{ 
  FLASH_ERROR_RD      = 0x01,
  FLASH_ERROR_ENDHV   = 0x02,
  FLASH_ERROR_SIZE    = 0x04,
  FLASH_ERROR_PGA     = 0x08,
  FLASH_ERROR_WRP     = 0x10,
  FLASH_ERROR_OPTV    = 0x20,
  FLASH_ERROR_NOTZERO = 0x40
}FLASH_ErrorTypeDef;

/**
  * @brief  FLASH Procedure structure definition
  */
typedef enum 
{
  FLASH_PROC_NONE = 0, 
  FLASH_PROC_PAGEERASE,
  FLASH_PROC_PROGRAM
} FLASH_ProcedureTypeDef;

/**
  * @brief  FLASH Erase structure definition
  */
typedef struct
{
  uint32_t TypeErase;   /*!< TypeErase: Mass erase or sector Erase.
                              This parameter can be a value of @ref FLASH_Type_Erase */

  uint32_t Page;        /*!< Sector: Initial FLASH sector to erase when Mass erase is disabled
                              This parameter must be an address value between 0x08000000 and 0x0800FFFF */
  
  uint32_t NbPages;    /*!< NbSectors: Number of sectors to be erased.
                              This parameter must be a value between 1 and (max number of sectors - initial sector value) */
  
} FLASH_EraseInitTypeDef;

/**
  * @brief  FLASH Option Bytes PROGRAM structure definition
  */
typedef struct
{
  uint32_t OptionType;     /*!< OptionType: Option byte to be configured.
                              This parameter can be a value of @ref FLASH_Option_Type */

  uint32_t WRPState;      /*!< WRPState: Write protection activation or deactivation.
                              This parameter can be a value of @ref FLASH_WRP_State */

  uint32_t WRPSector;    /*!< WRPSector: specifies the sector(s) to be write protected.
                              The value of this parameter depends on device used within the same series */

  uint32_t RDPLevel;     /*!< RDPLevel: Set the read protection level..
                              This parameter can be a value of @ref FLASH_Option_Bytes_Read_Protection */

  uint32_t BORLevel;     /*!< BORLevel: Set the BOR Level.
                              This parameter can be a value of @ref FLASH_Option_Bytes_BOR_Level */
  uint8_t  USERConfig;     /*!< USERConfig: Program the FLASH User Option Byte: IWDG_SW / RST_STOP / RST_STDBY.
                              This parameter can be a combination of @ref FLASH_Option_Bytes_IWatchdog, @ref FLASH_Option_Bytes_nRST_STOP and @ref FLASH_Option_Bytes_nRST_STDBY*/
} FLASH_OBProgramInitTypeDef;

/** 
  * @brief  FLASH handle Structure definition  
  */
typedef struct
{
  __IO FLASH_ProcedureTypeDef ProcedureOnGoing;  /*Internal variable to indicate which procedure is ongoing or not in IT context*/
  
  __IO uint32_t               NbPagesToErase;    /*Internal variable to save the remaining sectors to erase in IT context*/
  
  __IO uint32_t               Page;              /*Internal variable to define the current sector which is erasing*/
  
  __IO uint32_t               Address;           /*Internal variable to save address selected for program*/
  
  HAL_LockTypeDef             Lock;              /* FLASH locking object */

  __IO FLASH_ErrorTypeDef     ErrorCode;         /* FLASH error code */

}FLASH_ProcessTypeDef;

/*Variables used for Erase sectors under interruption*/
extern FLASH_ProcessTypeDef pFlash;   

/* Exported constants --------------------------------------------------------*/

/** @defgroup FLASH_Exported_Constants FLASH Exported Constants
  * @{
  */

/** @defgroup FLASH_Type_Erase
  * @{
  */
#define TYPEERASE_PAGEERASE       ((uint32_t)0x00)  /*!<Page erase only*/
#define TYPEERASE_WORD            ((uint32_t)0x01)  /*!<Data erase word activation*/
 
#define IS_TYPEERASE(VALUE)(((VALUE) == TYPEERASE_PAGEERASE) || \
                              ((VALUE) == TYPEERASE_WORD)) 
/**
  * @}
  */
  
/** @defgroup FLASH_Type_Program
  * @{
  */
#define TYPEPROGRAM_BYTE            ((uint32_t)0x00)  /*!<Program byte (8-bit) at a specified address.*/
#define TYPEPROGRAM_HALFWORD        ((uint32_t)0x01)  /*!<Program a half-word (16-bit) at a specified address.*/
#define TYPEPROGRAM_WORD            ((uint32_t)0x02)  /*!<Program a word (32-bit) at a specified address.*/
#define TYPEPROGRAM_FASTBYTE        ((uint32_t)0x04)  /*!<Fast Program byte (8-bit) at a specified address.*/
#define TYPEPROGRAM_FASTHALFWORD    ((uint32_t)0x08)  /*!<Fast Program a half-word (16-bit) at a specified address.*/
#define TYPEPROGRAM_FASTWORD        ((uint32_t)0x10)  /*!<Fast Program a word (32-bit) at a specified address.*/

#define IS_TYPEPROGRAM(VALUE)(((VALUE) == TYPEPROGRAM_BYTE) || \
                              ((VALUE) == TYPEPROGRAM_HALFWORD) || \
                              ((VALUE) == TYPEPROGRAM_WORD) || \
                              ((VALUE) == TYPEPROGRAM_FASTBYTE) || \
                              ((VALUE) == TYPEPROGRAM_FASTHALFWORD) || \
                              ((VALUE) == TYPEPROGRAM_FASTWORD))  
/**
  * @}
  */

/** @defgroup FLASH_WRP_State
  * @{
  */
#define WRPSTATE_DISABLE       ((uint32_t)0x00)  /*!<Disable the write protection of the desired bank 1 sectors*/
#define WRPSTATE_ENABLE        ((uint32_t)0x01)  /*!<Enable the write protection of the desired bank 1 sectors*/

#define IS_WRPSTATE(VALUE)(((VALUE) == WRPSTATE_DISABLE) || \
                           ((VALUE) == WRPSTATE_ENABLE))
/**
  * @}
  */

/** @defgroup FLASH_Option_Type
  * @{
  */
#define OPTIONBYTE_WRP        ((uint32_t)0x01)  /*!<WRP option byte configuration*/
#define OPTIONBYTE_RDP        ((uint32_t)0x02)  /*!<RDP option byte configuration*/
#define OPTIONBYTE_USER       ((uint32_t)0x04)  /*!<USER option byte configuration*/
#define OPTIONBYTE_BOR        ((uint32_t)0x08)  /*!<BOR option byte configuration*/

#define IS_OPTIONBYTE(VALUE)(((VALUE) < (OPTIONBYTE_WRP|OPTIONBYTE_RDP|OPTIONBYTE_USER|OPTIONBYTE_BOR)))
/**
  * @}
  */

/** @defgroup FLASH_Interrupts
  * @{
  */
#define FLASH_IT_EOP               FLASH_PECR_EOPIE  /*!< End of programming interrupt source */
#define FLASH_IT_ERR               FLASH_PECR_ERRIE  /*!< Error interrupt source */
#define IS_FLASH_IT(IT) ((((IT) & (uint32_t)0xFFFCFFFF) == 0x00000000) && (((IT) != 0x00000000)))
/**
  * @}
  */ 

/** @defgroup FLASH_Address
  * @{
  */
#define IS_FLASH_DATA_ADDRESS(ADDRESS) (((ADDRESS) >= DATA_EEPROM_BASE) && ((ADDRESS) <= DATA_EEPROM_END)) /* 2K */
#define IS_FLASH_PROGRAM_ADDRESS(ADDRESS) (((ADDRESS) >= FLASH_BASE) && ((ADDRESS) <= FLASH_END)) /* 64K */ 
#define IS_NBPAGES(PAGES) (((PAGES) >= 1) && ((PAGES) <= 512)) /* 512 pages from page0 to page 511 */  
/**
  * @}
  */ 

/** @defgroup FLASH_Option_Bytes_Write_Protection
  * @{
  */
#define OB_WRP_Pages0to31              ((uint32_t)0x00000001) /* Write protection of Sector0 */
#define OB_WRP_Pages32to63             ((uint32_t)0x00000002) /* Write protection of Sector1 */
#define OB_WRP_Pages64to95             ((uint32_t)0x00000004) /* Write protection of Sector2 */
#define OB_WRP_Pages96to127            ((uint32_t)0x00000008) /* Write protection of Sector3 */
#define OB_WRP_Pages128to159           ((uint32_t)0x00000010) /* Write protection of Sector4 */
#define OB_WRP_Pages160to191           ((uint32_t)0x00000020) /* Write protection of Sector5 */
#define OB_WRP_Pages192to223           ((uint32_t)0x00000040) /* Write protection of Sector6 */
#define OB_WRP_Pages224to255           ((uint32_t)0x00000080) /* Write protection of Sector7 */
#define OB_WRP_Pages256to287           ((uint32_t)0x00000100) /* Write protection of Sector8 */
#define OB_WRP_Pages288to319           ((uint32_t)0x00000200) /* Write protection of Sector9 */
#define OB_WRP_Pages320to351           ((uint32_t)0x00000400) /* Write protection of Sector10 */
#define OB_WRP_Pages352to383           ((uint32_t)0x00000800) /* Write protection of Sector11 */
#define OB_WRP_Pages384to415           ((uint32_t)0x00001000) /* Write protection of Sector12 */
#define OB_WRP_Pages416to447           ((uint32_t)0x00002000) /* Write protection of Sector13 */
#define OB_WRP_Pages448to479           ((uint32_t)0x00004000) /* Write protection of Sector14 */
#define OB_WRP_Pages480to511           ((uint32_t)0x00008000) /* Write protection of Sector15 */

#define OB_WRP_AllPages                ((uint32_t)0x0000FFFF) /*!< Write protection of all Sectors */

#define IS_OB_WRP(PAGE) (((PAGE) != 0x0000000))
/**
  * @}
  */

/** @defgroup FLASH_Option_Bytes_Read_Protection
  * @{
  */ 

/** 
  * @brief FLASH_Option_Bytes_Read_Protection  
  */ 
#define OB_RDP_Level_0   ((uint8_t)0xAA)
#define OB_RDP_Level_1   ((uint8_t)0xBB)
/*#define OB_RDP_Level_2   ((uint8_t)0xCC)*/ /* Warning: When enabling read protection level 2 
                                                it's no more possible to go back to level 1 or 0 */

#define IS_OB_RDP(LEVEL) (((LEVEL) == OB_RDP_Level_0)||\
                          ((LEVEL) == OB_RDP_Level_1))/*||\
                          ((LEVEL) == OB_RDP_Level_2))*/
/**
  * @}
  */ 

/** @defgroup FLASH_Option_Bytes_IWatchdog
  * @{
  */
#define OB_IWDG_SW                     ((uint8_t)0x10)  /*!< Software WDG selected */
#define OB_IWDG_HW                     ((uint8_t)0x00)  /*!< Hardware WDG selected */
#define IS_OB_IWDG_SOURCE(SOURCE) (((SOURCE) == OB_IWDG_SW) || ((SOURCE) == OB_IWDG_HW))
/**
  * @}
  */

/** @defgroup FLASH_Option_Bytes_nRST_STOP
  * @{
  */
#define OB_STOP_NoRST                  ((uint8_t)0x20) /*!< No reset generated when entering in STOP */
#define OB_STOP_RST                    ((uint8_t)0x00) /*!< Reset generated when entering in STOP */
#define IS_OB_STOP_SOURCE(SOURCE) (((SOURCE) == OB_STOP_NoRST) || ((SOURCE) == OB_STOP_RST))
/**
  * @}
  */

/** @defgroup FLASH_Option_Bytes_nRST_STDBY
  * @{
  */
#define OB_STDBY_NoRST                 ((uint8_t)0x40) /*!< No reset generated when entering in STANDBY */
#define OB_STDBY_RST                   ((uint8_t)0x00) /*!< Reset generated when entering in STANDBY */
#define IS_OB_STDBY_SOURCE(SOURCE) (((SOURCE) == OB_STDBY_NoRST) || ((SOURCE) == OB_STDBY_RST))
/**
  * @}
  */

/** @defgroup FLASH_Option_Bytes_BOR_Level
  * @{
  */

#define OB_BOR_OFF       ((uint8_t)0x00) /*!< BOR is disabled at power down, the reset is asserted when the VDD 
                                              power supply reaches the PDR(Power Down Reset) threshold (1.5V) */
#define OB_BOR_LEVEL1    ((uint8_t)0x08) /*!< BOR Reset threshold levels for 1.7V - 1.8V VDD power supply    */
#define OB_BOR_LEVEL2    ((uint8_t)0x09) /*!< BOR Reset threshold levels for 1.9V - 2.0V VDD power supply    */
#define OB_BOR_LEVEL3    ((uint8_t)0x0A) /*!< BOR Reset threshold levels for 2.3V - 2.4V VDD power supply    */
#define OB_BOR_LEVEL4    ((uint8_t)0x0B) /*!< BOR Reset threshold levels for 2.55V - 2.65V VDD power supply  */
#define OB_BOR_LEVEL5    ((uint8_t)0x0C) /*!< BOR Reset threshold levels for 2.8V - 2.9V VDD power supply    */

#define IS_OB_BOR_LEVEL(LEVEL)  (((LEVEL) == OB_BOR_OFF) || \
                                 ((LEVEL) == OB_BOR_LEVEL1) || \
                                 ((LEVEL) == OB_BOR_LEVEL2) || \
                                 ((LEVEL) == OB_BOR_LEVEL3) || \
                                 ((LEVEL) == OB_BOR_LEVEL4) || \
                                 ((LEVEL) == OB_BOR_LEVEL5))

/**
  * @}
  */
  
/** @defgroup FLASH_Flags
  * @{
  */ 

#define FLASH_FLAG_BSY                 FLASH_SR_BSY             /*!< FLASH Busy flag */
#define FLASH_FLAG_EOP                 FLASH_SR_EOP             /*!< FLASH End of Programming flag */
#define FLASH_FLAG_ENDHV               FLASH_SR_ENHV            /*!< FLASH End of High Voltage flag */
#define FLASH_FLAG_READY               FLASH_SR_READY           /*!< FLASH Ready flag after low power mode */
#define FLASH_FLAG_WRPERR              FLASH_SR_WRPERR          /*!< FLASH Write protected error flag */
#define FLASH_FLAG_PGAERR              FLASH_SR_PGAERR          /*!< FLASH Programming Alignment error flag */
#define FLASH_FLAG_SIZERR              FLASH_SR_SIZERR          /*!< FLASH Size error flag  */
#define FLASH_FLAG_OPTVERR             FLASH_SR_OPTVERR         /*!< FLASH Option Validity error flag  */
#define FLASH_FLAG_RDERR               FLASH_SR_RDERR           /*!< FLASH Read protected error flag */
#define FLASH_FLAG_NOTZEROERR          FLASH_SR_NOTZEROERR      /*!< FLASH Read protected error flag */
    
#define IS_FLASH_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0xFFFED0FF) == 0x00000000) && ((FLAG) != 0x00000000))

#define IS_FLASH_GET_FLAG(FLAG)  (((FLAG) == FLASH_FLAG_BSY) || ((FLAG) == FLASH_FLAG_EOP) || \
                                  ((FLAG) == FLASH_FLAG_ENDHV) || ((FLAG) == FLASH_FLAG_READY ) || \
                                  ((FLAG) ==  FLASH_FLAG_WRPERR) || ((FLAG) == FLASH_FLAG_PGAERR ) || \
                                  ((FLAG) ==  FLASH_FLAG_SIZERR) || ((FLAG) == FLASH_FLAG_OPTVERR) || \
                                  ((FLAG) ==  FLASH_FLAG_RDERR) || ((FLAG) ==  FLASH_FLAG_NOTZEROERR))
/**
  * @}
  */ 

/** @defgroup FLASH_Keys
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
  
/* CMSIS_Legacy */ 
  
#if defined ( __ICCARM__ )
#define InterruptType_ACTLR_DISMCYCINT_Msk         IntType_ACTLR_DISMCYCINT_Msk
#endif

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup FLASH_Interrupt FLASH Interrupt
 *  @brief macros to handle FLASH interrupts
 * @{
 */

/**
  * @brief  Enables or disables the specified FLASH interrupts.
  * @param  __INTERRUPT__: specifies the FLASH interrupt sources to be enabled or 
  *         disabled.
  *   This parameter can be any combination of the following values:
  *     @arg FLASH_IT_EOP: FLASH end of programming Interrupt
  *     @arg FLASH_IT_ERR: FLASH Error Interrupt
  * @retval None 
  */ 
#define __HAL_FLASH_ENABLE_IT(__INTERRUPT__)  (FLASH->PECR |= (__INTERRUPT__))

/**
  * @brief  Disable the specified FLASH interrupt.
  * @param  __INTERRUPT__ : FLASH interrupt 
  *         This parameter can be any combination of the following values:
  *     @arg FLASH_IT_EOP: End of FLASH Operation Interrupt
  *     @arg FLASH_IT_ERR: Error Interrupt    
  * @retval none
  */
#define __HAL_FLASH_DISABLE_IT(__INTERRUPT__)  (FLASH->PECR &= ~(uint32_t)(__INTERRUPT__))

/**
  * @brief  Checks whether the specified FLASH flag is set or not.
  * @param  __FLAG__: specifies the FLASH flag to check.
  *   This parameter can be one of the following values:
  *     @arg FLASH_FLAG_BSY: FLASH write/erase operations in progress flag 
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag
  *     @arg FLASH_FLAG_READY: FLASH Ready flag after low power mode
  *     @arg FLASH_FLAG_ENDHV: FLASH End of high voltage flag
  *     @arg FLASH_FLAG_WRPERR: FLASH Write protected error flag 
  *     @arg FLASH_FLAG_PGAERR: FLASH Programming Alignment error flag
  *     @arg FLASH_FLAG_SIZERR: FLASH size error flag
  *     @arg FLASH_FLAG_OPTVERR: FLASH Option validity error flag
  *     @arg FLASH_FLAG_OPTVERRUSR: FLASH Option User validity error flag
  *     @arg FLASH_FLAG_RDERR: FLASH Read protected error flag
  *     @arg FLASH_FLAG_NOTZEROERR:  Not Zero area error flag  
  * @retval The new state of FLASH_FLAG (SET or RESET).
  */
#define __HAL_FLASH_GET_FLAG(__FLAG__)   ((FLASH->SR & (__FLAG__)) == (__FLAG__))

/**
  * @brief  Clears the FLASH's pending flags.
  * @param  __FLAG__: specifies the FLASH flags to clear.
  *   This parameter can be any combination of the following values:
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag
  *     @arg FLASH_FLAG_WRPERR: FLASH Write protected error flag 
  *     @arg FLASH_FLAG_PGAERR: FLASH Programming Alignment error flag 
  *     @arg FLASH_FLAG_SIZERR: FLASH size error flag    
  *     @arg FLASH_FLAG_OPTVERR: FLASH Option validity error flag
  *     @arg FLASH_FLAG_OPTVERRUSR: FLASH Option User validity error flag
  *     @arg FLASH_FLAG_RDERR: FLASH Read protected error flag
  *     @arg FLASH_FLAG_NOTZEROERR:  Not Zero area error flag  
  * @retval None
  */
#define __HAL_FLASH_CLEAR_FLAG(__FLAG__)   (FLASH->SR = (__FLAG__)) 

/**
  * @}
  */

/* Include FLASH HAL Extension module */
#include "stm32l0xx_hal_flash_ex.h"
#include "stm32l0xx_hal_flash_ramfunc.h"

/* Exported functions ------------------------------------------------------- */

/** 
  * @brief  FLASH memory functions that can be executed from FLASH.  
  */
/* Program operation functions  ***********************************************/
HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint32_t Data);
HAL_StatusTypeDef HAL_FLASH_Program_IT(uint32_t TypeProgram, uint32_t Address, uint32_t Data);

/* FLASH IRQ handler function */
void HAL_FLASH_IRQHandler(void);

/* Callbacks in non blocking modes */
void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue);
void HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue); 

/* Peripheral Control functions  **********************************************/
HAL_StatusTypeDef HAL_FLASH_Unlock(void);
HAL_StatusTypeDef HAL_FLASH_Lock(void);
HAL_StatusTypeDef HAL_FLASH_OB_Unlock(void);
HAL_StatusTypeDef HAL_FLASH_OB_Lock(void);

/* Option bytes control */
HAL_StatusTypeDef HAL_FLASH_OB_Launch(void);

/* Peripheral State functions  ************************************************/
FLASH_ErrorTypeDef HAL_FLASH_GetError(void);

/* Non-User functions  ********************************************************/
HAL_StatusTypeDef FLASH_WaitForLastOperation(uint32_t Timeout);
void FLASH_Erase_Page(uint32_t Page_Address);

#ifdef __cplusplus
}
#endif

#endif /* __STM32L0XX_HAL_FLASH_H */

/**
  * @}
  */

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
