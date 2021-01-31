/**
  ******************************************************************************
  * @file    stm32f2xx_hal_flash_ex.h
  * @author  MCD Application Team
  * @brief   Header file of FLASH HAL Extension module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
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
#ifndef __STM32F2xx_HAL_FLASH_EX_H
#define __STM32F2xx_HAL_FLASH_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal_def.h"

/** @addtogroup STM32F2xx_HAL_Driver
  * @{
  */

/** @addtogroup FLASHEx
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/ 
/** @defgroup FLASHEx_Exported_Types FLASH Exported Types
  * @{
  */

/**
  * @brief  FLASH Erase structure definition
  */
typedef struct
{
  uint32_t TypeErase;   /*!< Mass erase or sector Erase.
                             This parameter can be a value of @ref FLASHEx_Type_Erase */

  uint32_t Banks;       /*!< Select banks to erase when Mass erase is enabled.
                             This parameter must be a value of @ref FLASHEx_Banks */

  uint32_t Sector;      /*!< Initial FLASH sector to erase when Mass erase is disabled
                             This parameter must be a value of @ref FLASHEx_Sectors */

  uint32_t NbSectors;   /*!< Number of sectors to be erased.
                             This parameter must be a value between 1 and (max number of sectors - value of Initial sector)*/

  uint32_t VoltageRange;/*!< The device voltage range which defines the erase parallelism
                             This parameter must be a value of @ref FLASHEx_Voltage_Range */

} FLASH_EraseInitTypeDef;

/**
  * @brief  FLASH Option Bytes Program structure definition
  */
typedef struct
{
  uint32_t OptionType;   /*!< Option byte to be configured.
                              This parameter can be a value of @ref FLASHEx_Option_Type */

  uint32_t WRPState;     /*!< Write protection activation or deactivation.
                              This parameter can be a value of @ref FLASHEx_WRP_State */

  uint32_t WRPSector;         /*!< Specifies the sector(s) to be write protected.
                              The value of this parameter depend on device used within the same series */

  uint32_t Banks;        /*!< Select banks for WRP activation/deactivation of all sectors.
                              This parameter must be a value of @ref FLASHEx_Banks */        

  uint32_t RDPLevel;     /*!< Set the read protection level.
                              This parameter can be a value of @ref FLASHEx_Option_Bytes_Read_Protection */

  uint32_t BORLevel;     /*!< Set the BOR Level.
                              This parameter can be a value of @ref FLASHEx_BOR_Reset_Level */

  uint8_t  USERConfig;   /*!< Program the FLASH User Option Byte: IWDG_SW / RST_STOP / RST_STDBY. */

} FLASH_OBProgramInitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup FLASHEx_Exported_Constants FLASH Exported Constants
  * @{
  */

/** @defgroup FLASHEx_Type_Erase FLASH Type Erase
  * @{
  */ 
#define FLASH_TYPEERASE_SECTORS         0x00000000U  /*!< Sectors erase only          */
#define FLASH_TYPEERASE_MASSERASE       0x00000001U  /*!< Flash Mass erase activation */
/**
  * @}
  */
  
/** @defgroup FLASHEx_Voltage_Range FLASH Voltage Range
  * @{
  */ 
#define FLASH_VOLTAGE_RANGE_1        0x00000000U  /*!< Device operating range: 1.8V to 2.1V                */
#define FLASH_VOLTAGE_RANGE_2        0x00000001U  /*!< Device operating range: 2.1V to 2.7V                */
#define FLASH_VOLTAGE_RANGE_3        0x00000002U  /*!< Device operating range: 2.7V to 3.6V                */
#define FLASH_VOLTAGE_RANGE_4        0x00000003U  /*!< Device operating range: 2.7V to 3.6V + External Vpp */
/**
  * @}
  */
  
/** @defgroup FLASHEx_WRP_State FLASH WRP State
  * @{
  */ 
#define OB_WRPSTATE_DISABLE       0x00000000U  /*!< Disable the write protection of the desired bank 1 sectors */
#define OB_WRPSTATE_ENABLE        0x00000001U  /*!< Enable the write protection of the desired bank 1 sectors  */
/**
  * @}
  */
  
/** @defgroup FLASHEx_Option_Type FLASH Option Type
  * @{
  */ 
#define OPTIONBYTE_WRP        0x00000001U  /*!< WRP option byte configuration  */
#define OPTIONBYTE_RDP        0x00000002U  /*!< RDP option byte configuration  */
#define OPTIONBYTE_USER       0x00000004U  /*!< USER option byte configuration */
#define OPTIONBYTE_BOR        0x00000008U  /*!< BOR option byte configuration  */
/**
  * @}
  */
  
/** @defgroup FLASHEx_Option_Bytes_Read_Protection FLASH Option Bytes Read Protection
  * @{
  */
#define OB_RDP_LEVEL_0   ((uint8_t)0xAA)
#define OB_RDP_LEVEL_1   ((uint8_t)0x55)
#define OB_RDP_LEVEL_2   ((uint8_t)0xCC) /*!< Warning: When enabling read protection level 2 
                                              it s no more possible to go back to level 1 or 0 */
/**
  * @}
  */ 
  
/** @defgroup FLASHEx_Option_Bytes_IWatchdog FLASH Option Bytes IWatchdog
  * @{
  */ 
#define OB_IWDG_SW                     ((uint8_t)0x20)  /*!< Software IWDG selected */
#define OB_IWDG_HW                     ((uint8_t)0x00)  /*!< Hardware IWDG selected */
/**
  * @}
  */ 
  
/** @defgroup FLASHEx_Option_Bytes_nRST_STOP FLASH Option Bytes nRST_STOP
  * @{
  */ 
#define OB_STOP_NO_RST                 ((uint8_t)0x40) /*!< No reset generated when entering in STOP */
#define OB_STOP_RST                    ((uint8_t)0x00) /*!< Reset generated when entering in STOP    */
/**
  * @}
  */ 


/** @defgroup FLASHEx_Option_Bytes_nRST_STDBY FLASH Option Bytes nRST_STDBY
  * @{
  */ 
#define OB_STDBY_NO_RST                ((uint8_t)0x80) /*!< No reset generated when entering in STANDBY */
#define OB_STDBY_RST                   ((uint8_t)0x00) /*!< Reset generated when entering in STANDBY    */
/**
  * @}
  */    

/** @defgroup FLASHEx_BOR_Reset_Level FLASH BOR Reset Level
  * @{
  */  
#define OB_BOR_LEVEL3          ((uint8_t)0x00)  /*!< Supply voltage ranges from 2.70 to 3.60 V */
#define OB_BOR_LEVEL2          ((uint8_t)0x04)  /*!< Supply voltage ranges from 2.40 to 2.70 V */
#define OB_BOR_LEVEL1          ((uint8_t)0x08)  /*!< Supply voltage ranges from 2.10 to 2.40 V */
#define OB_BOR_OFF             ((uint8_t)0x0C)  /*!< Supply voltage ranges from 1.62 to 2.10 V */
/**
  * @}
  */


/**
  * @}
  */

/** @defgroup FLASH_Latency FLASH Latency
  * @{
  */
#define FLASH_LATENCY_0                FLASH_ACR_LATENCY_0WS   /*!< FLASH Zero Latency cycle      */
#define FLASH_LATENCY_1                FLASH_ACR_LATENCY_1WS   /*!< FLASH One Latency cycle       */
#define FLASH_LATENCY_2                FLASH_ACR_LATENCY_2WS   /*!< FLASH Two Latency cycles      */
#define FLASH_LATENCY_3                FLASH_ACR_LATENCY_3WS   /*!< FLASH Three Latency cycles    */
#define FLASH_LATENCY_4                FLASH_ACR_LATENCY_4WS   /*!< FLASH Four Latency cycles     */
#define FLASH_LATENCY_5                FLASH_ACR_LATENCY_5WS   /*!< FLASH Five Latency cycles     */
#define FLASH_LATENCY_6                FLASH_ACR_LATENCY_6WS   /*!< FLASH Six Latency cycles      */
#define FLASH_LATENCY_7                FLASH_ACR_LATENCY_7WS   /*!< FLASH Seven Latency cycles    */

/**
  * @}
  */ 
  

/** @defgroup FLASHEx_Banks FLASH Banks
  * @{
  */
#define FLASH_BANK_1     1U /*!< Bank 1   */
/**
  * @}
  */ 
    
/** @defgroup FLASHEx_MassErase_bit FLASH Mass Erase bit
  * @{
  */
#define FLASH_MER_BIT     FLASH_CR_MER /*!< only 1 MER Bit */
/**
  * @}
  */ 

/** @defgroup FLASHEx_Sectors FLASH Sectors
  * @{
  */
#define FLASH_SECTOR_0     0U  /*!< Sector Number 0   */
#define FLASH_SECTOR_1     1U  /*!< Sector Number 1   */
#define FLASH_SECTOR_2     2U  /*!< Sector Number 2   */
#define FLASH_SECTOR_3     3U  /*!< Sector Number 3   */
#define FLASH_SECTOR_4     4U  /*!< Sector Number 4   */
#define FLASH_SECTOR_5     5U  /*!< Sector Number 5   */
#define FLASH_SECTOR_6     6U  /*!< Sector Number 6   */
#define FLASH_SECTOR_7     7U  /*!< Sector Number 7   */
#define FLASH_SECTOR_8     8U  /*!< Sector Number 8   */
#define FLASH_SECTOR_9     9U  /*!< Sector Number 9   */
#define FLASH_SECTOR_10    10U /*!< Sector Number 10  */
#define FLASH_SECTOR_11    11U /*!< Sector Number 11  */



/**
  * @}
  */ 

/** @defgroup FLASHEx_Option_Bytes_Write_Protection FLASH Option Bytes Write Protection
  * @{
  */
#define OB_WRP_SECTOR_0       0x00000001U /*!< Write protection of Sector0 */
#define OB_WRP_SECTOR_1       0x00000002U /*!< Write protection of Sector1 */
#define OB_WRP_SECTOR_2       0x00000004U /*!< Write protection of Sector2 */
#define OB_WRP_SECTOR_3       0x00000008U /*!< Write protection of Sector3 */
#define OB_WRP_SECTOR_4       0x00000010U /*!< Write protection of Sector4 */
#define OB_WRP_SECTOR_5       0x00000020U /*!< Write protection of Sector5 */
#define OB_WRP_SECTOR_6       0x00000040U /*!< Write protection of Sector6 */
#define OB_WRP_SECTOR_7       0x00000080U /*!< Write protection of Sector7 */
#define OB_WRP_SECTOR_8       0x00000100U /*!< Write protection of Sector8 */
#define OB_WRP_SECTOR_9       0x00000200U /*!< Write protection of Sector9 */
#define OB_WRP_SECTOR_10      0x00000400U /*!< Write protection of Sector10 */
#define OB_WRP_SECTOR_11      0x00000800U /*!< Write protection of Sector11 */
#define OB_WRP_SECTOR_All     0x00000FFFU /*!< Write protection of all Sectors */


/**
  * @}
  */

/**
  * @}
  */ 
  
/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup FLASHEx_Exported_Functions
  * @{
  */

/** @addtogroup FLASHEx_Exported_Functions_Group1
  * @{
  */
/* Extension Program operation functions  *************************************/
HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *SectorError);
HAL_StatusTypeDef HAL_FLASHEx_Erase_IT(FLASH_EraseInitTypeDef *pEraseInit);
HAL_StatusTypeDef HAL_FLASHEx_OBProgram(FLASH_OBProgramInitTypeDef *pOBInit);
void              HAL_FLASHEx_OBGetConfig(FLASH_OBProgramInitTypeDef *pOBInit);

/**
  * @}
  */

/**
  * @}
  */
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @defgroup FLASHEx_Private_Variables FLASH Private Variables
  * @{
  */

/**
  * @}
  */
/* Private constants ---------------------------------------------------------*/
/** @defgroup FLASHEx_Private_Constants FLASH Private Constants
  * @{
  */

#define FLASH_SECTOR_TOTAL  12U

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup FLASHEx_Private_Macros FLASH Private Macros
  * @{
  */

/** @defgroup FLASHEx_IS_FLASH_Definitions FLASH Private macros to check input parameters
  * @{
  */

#define IS_FLASH_TYPEERASE(VALUE)(((VALUE) == FLASH_TYPEERASE_SECTORS) || \
                                  ((VALUE) == FLASH_TYPEERASE_MASSERASE))  

#define IS_VOLTAGERANGE(RANGE)(((RANGE) == FLASH_VOLTAGE_RANGE_1) || \
                               ((RANGE) == FLASH_VOLTAGE_RANGE_2) || \
                               ((RANGE) == FLASH_VOLTAGE_RANGE_3) || \
                               ((RANGE) == FLASH_VOLTAGE_RANGE_4))  

#define IS_WRPSTATE(VALUE)(((VALUE) == OB_WRPSTATE_DISABLE) || \
                           ((VALUE) == OB_WRPSTATE_ENABLE))  

#define IS_OPTIONBYTE(VALUE)(((VALUE) <= (OPTIONBYTE_WRP|OPTIONBYTE_RDP|OPTIONBYTE_USER|OPTIONBYTE_BOR)))

#define IS_OB_RDP_LEVEL(LEVEL) (((LEVEL) == OB_RDP_LEVEL_0)   ||\
                                ((LEVEL) == OB_RDP_LEVEL_1)   ||\
                                ((LEVEL) == OB_RDP_LEVEL_2))

#define IS_OB_IWDG_SOURCE(SOURCE) (((SOURCE) == OB_IWDG_SW) || ((SOURCE) == OB_IWDG_HW))

#define IS_OB_STOP_SOURCE(SOURCE) (((SOURCE) == OB_STOP_NO_RST) || ((SOURCE) == OB_STOP_RST))

#define IS_OB_STDBY_SOURCE(SOURCE) (((SOURCE) == OB_STDBY_NO_RST) || ((SOURCE) == OB_STDBY_RST))

#define IS_OB_BOR_LEVEL(LEVEL) (((LEVEL) == OB_BOR_LEVEL1) || ((LEVEL) == OB_BOR_LEVEL2) ||\
                                ((LEVEL) == OB_BOR_LEVEL3) || ((LEVEL) == OB_BOR_OFF))


#define IS_FLASH_LATENCY(LATENCY) (((LATENCY) == FLASH_LATENCY_0)  || \
                                   ((LATENCY) == FLASH_LATENCY_1)  || \
                                   ((LATENCY) == FLASH_LATENCY_2)  || \
                                   ((LATENCY) == FLASH_LATENCY_3)  || \
                                   ((LATENCY) == FLASH_LATENCY_4)  || \
                                   ((LATENCY) == FLASH_LATENCY_5)  || \
                                   ((LATENCY) == FLASH_LATENCY_6)  || \
                                   ((LATENCY) == FLASH_LATENCY_7))
#define IS_FLASH_BANK(BANK) (((BANK) == FLASH_BANK_1))
#define IS_FLASH_SECTOR(SECTOR) (((SECTOR) == FLASH_SECTOR_0)   || ((SECTOR) == FLASH_SECTOR_1)   ||\
                                 ((SECTOR) == FLASH_SECTOR_2)   || ((SECTOR) == FLASH_SECTOR_3)   ||\
                                 ((SECTOR) == FLASH_SECTOR_4)   || ((SECTOR) == FLASH_SECTOR_5)   ||\
                                 ((SECTOR) == FLASH_SECTOR_6)   || ((SECTOR) == FLASH_SECTOR_7)   ||\
                                 ((SECTOR) == FLASH_SECTOR_8)   || ((SECTOR) == FLASH_SECTOR_9)   ||\
                                 ((SECTOR) == FLASH_SECTOR_10)  || ((SECTOR) == FLASH_SECTOR_11))



#define IS_FLASH_ADDRESS(ADDRESS) ((((ADDRESS) >= FLASH_BASE) && ((ADDRESS) <= FLASH_END)) || \
                                   (((ADDRESS) >= FLASH_OTP_BASE) && ((ADDRESS) <= FLASH_OTP_END)))
#define IS_FLASH_NBSECTORS(NBSECTORS) (((NBSECTORS) != 0U) && ((NBSECTORS) <= FLASH_SECTOR_TOTAL))
#define IS_OB_WRP_SECTOR(SECTOR)((((SECTOR) & 0xFFFFF000U) == 0x00000000U) && ((SECTOR) != 0x00000000U))

/**
  * @}
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup FLASHEx_Private_Functions FLASH Private Functions
  * @{
  */
void FLASH_Erase_Sector(uint32_t Sector, uint8_t VoltageRange);
void FLASH_FlushCaches(void);
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

#endif /* __STM32F2xx_HAL_FLASH_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
