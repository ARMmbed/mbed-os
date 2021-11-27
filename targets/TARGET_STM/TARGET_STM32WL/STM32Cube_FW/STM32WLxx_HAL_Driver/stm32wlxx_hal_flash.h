/**
  ******************************************************************************
  * @file    stm32wlxx_hal_flash.h
  * @author  MCD Application Team
  * @brief   Header file of FLASH HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32WLxx_HAL_FLASH_H
#define STM32WLxx_HAL_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wlxx_hal_def.h"

/** @addtogroup STM32WLxx_HAL_Driver
  * @{
  */

/** @addtogroup FLASH
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup FLASH_Exported_Types FLASH Exported Types
  * @{
  */

/**
  * @brief  FLASH Erase structure definition
  */
typedef struct
{
  uint32_t TypeErase;   /*!< Mass erase or page erase.
                             This parameter can be a value of @ref FLASH_TYPE_ERASE */
  uint32_t Page;        /*!< Initial Flash page to erase when page erase is enabled
                             This parameter must be a value between 0 and (FLASH_PAGE_NB - 1) */
  uint32_t NbPages;     /*!< Number of pages to be erased.
                             This parameter must be a value between 1 and (FLASH_PAGE_NB - value of initial page)*/
} FLASH_EraseInitTypeDef;

/**
  * @brief  FLASH Option Bytes Program structure definition
  */
typedef struct
{
  uint32_t OptionType;             /*!< Option byte to be configured.
                                        This parameter can be a combination of the values of @ref FLASH_OB_TYPE */
  uint32_t WRPArea;                 /*!< Write protection area to be programmed (used for OPTIONBYTE_WRP).
                                        Only one WRP area could be programmed at the same time.
                                        This parameter can be value of @ref FLASH_OB_WRP_AREA */
  uint32_t WRPStartOffset;         /*!< Write protection start offset (used for OPTIONBYTE_WRP).
                                        This parameter must be a value between 0 and (max number of pages - 1) */
  uint32_t WRPEndOffset;           /*!< Write protection end offset (used for OPTIONBYTE_WRP).
                                        This parameter must be a value between WRPStartOffset and (max number of pages - 1) */
  uint32_t RDPLevel;               /*!< Set the read protection level (used for OPTIONBYTE_RDP).
                                        This parameter can be a value of @ref FLASH_OB_READ_PROTECTION */
  uint32_t UserType;               /*!< User option byte(s) to be configured (used for OPTIONBYTE_USER).
                                        This parameter can be a combination of @ref FLASH_OB_USER_TYPE */
#if defined(DUAL_CORE) /* Comment duplicated for Document generation */
  uint32_t UserConfig;             /*!< Value of the user option byte (used for OPTIONBYTE_USER).
                                        This parameter can be a combination of the values of
                                            @ref FLASH_OB_USER_BOR_LEVEL
                                            @ref FLASH_OB_USER_nRST_STOP, @ref FLASH_OB_USER_nRST_STANDBY,
                                            @ref FLASH_OB_USER_nRST_SHUTDOWN, @ref FLASH_OB_USER_IWDG_SW,
                                            @ref FLASH_OB_USER_IWDG_STOP, @ref FLASH_OB_USER_IWDG_STANDBY,
                                            @ref FLASH_OB_USER_WWDG_SW, @ref FLASH_OB_USER_nBOOT1,
                                            @ref FLASH_OB_USER_SRAM2_PE, @ref FLASH_OB_USER_SRAM_RST,
                                            @ref FLASH_OB_USER_nSWBOOT0, @ref FLASH_OB_USER_nBOOT0,
                                            @ref FLASH_OB_USER_BOOT_LOCK, @ref FLASH_OB_USER_C2BOOT_LOCK */
#else
  uint32_t UserConfig;             /*!< Value of the user option byte (used for OPTIONBYTE_USER).
                                        This parameter can be a combination of the values of
                                            @ref FLASH_OB_USER_BOR_LEVEL
                                            @ref FLASH_OB_USER_nRST_STOP, @ref FLASH_OB_USER_nRST_STANDBY,
                                            @ref FLASH_OB_USER_nRST_SHUTDOWN, @ref FLASH_OB_USER_IWDG_SW,
                                            @ref FLASH_OB_USER_IWDG_STOP, @ref FLASH_OB_USER_IWDG_STANDBY,
                                            @ref FLASH_OB_USER_WWDG_SW, @ref FLASH_OB_USER_nBOOT1,
                                            @ref FLASH_OB_USER_SRAM2_PE, @ref FLASH_OB_USER_SRAM_RST,
                                            @ref FLASH_OB_USER_nSWBOOT0, @ref FLASH_OB_USER_nBOOT0,
                                            @ref FLASH_OB_USER_BOOT_LOCK */
#endif
  uint32_t PCROPConfig;            /*!< Configuration of the PCROP (used for OPTIONBYTE_PCROP).
                                        This parameter must be a combination of values of @ref FLASH_OB_PCROP_ZONE
                                        and @ref FLASH_OB_PCROP_RDP */
  uint32_t PCROP1AStartAddr;       /*!< PCROP Zone A Start address (used for OPTIONBYTE_PCROP). It represents first address of start block
                                        to protect. Make sure this parameter is multiple of PCROP granularity */
  uint32_t PCROP1AEndAddr;         /*!< PCROP Zone A End address (used for OPTIONBYTE_PCROP). It represents first address of end block
                                        to protect. Make sure this parameter is multiple of PCROP granularity */
  uint32_t PCROP1BStartAddr;       /*!< PCROP Zone B Start address (used for OPTIONBYTE_PCROP). It represents first address of start block
                                        to protect. Make sure this parameter is multiple of PCROP granularity */
  uint32_t PCROP1BEndAddr;         /*!< PCROP Zone B End address (used for OPTIONBYTE_PCROP). It represents first address of end block
                                        to protect. Make sure this parameter is multiple of PCROP granularity */
#if defined(DUAL_CORE)
  uint32_t SecureFlashStartAddr;   /*!< Secure Flash start address (used for OPTIONBYTE_SECURE_MODE).
                                        This parameter must be a value between begin and end of Flash bank
                                        => Contains the start address of the first 2kB page of the secure Flash area */
  uint32_t SecureSRAM2StartAddr;   /*!< Secure Backup SRAM2 start address (used for OPTIONBYTE_SECURE_MODE).
                                        This parameter must be aligned on 1kB boundary */
  uint32_t SecureSRAM1StartAddr;   /*!< Secure non-Backup SRAM1 start address (used for OPTIONBYTE_SECURE_MODE)
                                        This parameter must be aligned on 1kB boundary */
  uint32_t HideProtectionStartAddr;/*!< Hide Protection area start address (used for OPTIONBYTE_SECURE_MODE)
                                        This parameter must be aligned on 2kB boundary */
  uint32_t SecureMode;             /*!< Secure mode activated or deactivated.
                                        This parameter can be a value of @ref FLASH_OB_SECURITY_MODE */
  uint32_t SUBGHZSPISecureAccess;  /*!< Sub-GHz radio SPI security access enabled or disabled (used for OPTIONBYTE_SUBGHZSPI_SECURE_ACCESS).
                                        This parameter can be a value of @ref FLASH_OB_SUBGHZSPI_SECURE_ACCESS */
  uint32_t C2DebugAccessMode;      /*!< CPU2 debug access enabled or disabled (used for OPTIONBYTE_C2_DEBUG_ACCESS).
                                        This parameter can be a value of @ref FLASH_OB_C2_DEBUG_ACCESS */
  uint32_t C2BootRegion;           /*!< CPU2 Secure Boot memory region(used for OPTIONBYTE_C2_BOOT_VECT).
                                        This parameter can be a value of @ref FLASH_OB_C2_BOOT_REGION */
  uint32_t C2SecureBootVectAddr;   /*!< CPU2 Secure Boot reset vector (used for OPTIONBYTE_C2_BOOT_VECT).
                                        This parameter contains the CPU2 boot reset start address within
                                        the selected memory region. Make sure this parameter is word aligned. */
  uint32_t IPCCdataBufAddr;        /*!< IPCC mailbox data buffer base address (used for OPTIONBYTE_IPCC_BUF_ADDR).
                                        This parameter contains the IPCC mailbox data buffer start address area
                                        in SRAM1 or SRAM2. Make sure this parameter is double-word aligned. */
#endif /* DUAL_CORE */
} FLASH_OBProgramInitTypeDef;

/**
* @brief  FLASH handle Structure definition
*/
typedef struct
{
  HAL_LockTypeDef   Lock;              /* FLASH locking object */
  uint32_t          ErrorCode;         /* FLASH error code */
  uint32_t          ProcedureOnGoing;  /* Internal variable to indicate which procedure is ongoing or not in IT context */
  uint32_t          Address;           /* Internal variable to save address selected for program in IT context */
  uint32_t          Page;              /* Internal variable to define the current page which is erasing in IT context */
  uint32_t          NbPagesToErase;    /* Internal variable to save the remaining pages to erase in IT context */
} FLASH_ProcessTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup FLASH_Exported_Constants FLASH Exported Constants
  * @{
  */

/** @defgroup FLASH_KEYS FLASH Keys
  * @{
  */
#define FLASH_KEY1                      0x45670123U  /*!< Flash key1 */
#define FLASH_KEY2                      0xCDEF89ABU  /*!< Flash key2: used with FLASH_KEY1
                                                          to unlock the FLASH registers access */

#define FLASH_OPTKEY1                   0x08192A3BU  /*!< Flash option byte key1 */
#define FLASH_OPTKEY2                   0x4C5D6E7FU  /*!< Flash option byte key2: used with FLASH_OPTKEY1
                                                          to allow option bytes operations */
/**
  * @}
  */

/** @defgroup FLASH_LATENCY FLASH Latency
  * @{
  */
#define FLASH_LATENCY_0                 0x00000000U         /*!< FLASH Zero wait state */
#define FLASH_LATENCY_1                 FLASH_ACR_LATENCY_0 /*!< FLASH One wait state */
#define FLASH_LATENCY_2                 FLASH_ACR_LATENCY_1 /*!< FLASH Two wait states */
/**
  * @}
  */

/** @defgroup FLASH_FLAGS FLASH Flags Definition
  * @{
  */
#ifdef CORE_CM0PLUS
#define FLASH_FLAG_EOP                  FLASH_C2SR_EOP      /*!< CPU2 FLASH End of operation flag */
#define FLASH_FLAG_OPERR                FLASH_C2SR_OPERR    /*!< CPU2 FLASH Operation error flag */
#define FLASH_FLAG_PROGERR              FLASH_C2SR_PROGERR  /*!< CPU2 FLASH Programming error flag */
#define FLASH_FLAG_WRPERR               FLASH_C2SR_WRPERR   /*!< CPU2 FLASH Write protection error flag */
#define FLASH_FLAG_PGAERR               FLASH_C2SR_PGAERR   /*!< CPU2 FLASH Programming alignment error flag */
#define FLASH_FLAG_SIZERR               FLASH_C2SR_SIZERR   /*!< CPU2 FLASH Size error flag  */
#define FLASH_FLAG_PGSERR               FLASH_C2SR_PGSERR   /*!< CPU2 FLASH Programming sequence error flag */
#define FLASH_FLAG_MISERR               FLASH_C2SR_MISERR   /*!< CPU2 FLASH Fast programming data miss error flag */
#define FLASH_FLAG_FASTERR              FLASH_C2SR_FASTERR  /*!< CPU2 FLASH Fast programming error flag */
#define FLASH_FLAG_OPTNV                FLASH_C2SR_OPTNV    /*!< CPU2 FLASH User Option OPTVAL indication */
#define FLASH_FLAG_RDERR                FLASH_C2SR_RDERR    /*!< CPU2 FLASH PCROP read error flag */
#define FLASH_FLAG_OPTVERR              FLASH_SR_OPTVERR    /*!< FLASH Option validity error flag  */
#define FLASH_FLAG_BSY                  FLASH_C2SR_BSY      /*!< CPU2 FLASH Busy flag */
#define FLASH_FLAG_CFGBSY               FLASH_C2SR_CFGBSY   /*!< CPU2 FLASH Programming/erase configuration busy */
#define FLASH_FLAG_PESD                 FLASH_C2SR_PESD     /*!< CPU2 FLASH Programming/erase operation suspended */
#else
#define FLASH_FLAG_EOP                  FLASH_SR_EOP      /*!< FLASH End of operation flag */
#define FLASH_FLAG_OPERR                FLASH_SR_OPERR    /*!< FLASH Operation error flag */
#define FLASH_FLAG_PROGERR              FLASH_SR_PROGERR  /*!< FLASH Programming error flag */
#define FLASH_FLAG_WRPERR               FLASH_SR_WRPERR   /*!< FLASH Write protection error flag */
#define FLASH_FLAG_PGAERR               FLASH_SR_PGAERR   /*!< FLASH Programming alignment error flag */
#define FLASH_FLAG_SIZERR               FLASH_SR_SIZERR   /*!< FLASH Size error flag  */
#define FLASH_FLAG_PGSERR               FLASH_SR_PGSERR   /*!< FLASH Programming sequence error flag */
#define FLASH_FLAG_MISERR               FLASH_SR_MISERR   /*!< FLASH Fast programming data miss error flag */
#define FLASH_FLAG_FASTERR              FLASH_SR_FASTERR  /*!< FLASH Fast programming error flag */
#define FLASH_FLAG_OPTNV                FLASH_SR_OPTNV    /*!< FLASH User Option OPTVAL indication */
#define FLASH_FLAG_RDERR                FLASH_SR_RDERR    /*!< FLASH PCROP read error flag */
#define FLASH_FLAG_OPTVERR              FLASH_SR_OPTVERR  /*!< FLASH Option validity error flag  */
#define FLASH_FLAG_BSY                  FLASH_SR_BSY      /*!< FLASH Busy flag */
#define FLASH_FLAG_CFGBSY               FLASH_SR_CFGBSY   /*!< FLASH Programming/erase configuration busy */
#define FLASH_FLAG_PESD                 FLASH_SR_PESD     /*!< FLASH Programming/erase operation suspended */
#endif

#define FLASH_FLAG_SR_ERRORS            (FLASH_FLAG_OPERR   | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR | \
                                         FLASH_FLAG_PGAERR  | FLASH_FLAG_SIZERR  | FLASH_FLAG_PGSERR | \
                                         FLASH_FLAG_MISERR  | FLASH_FLAG_FASTERR | FLASH_FLAG_RDERR  | \
                                         FLASH_FLAG_OPTVERR)  /*!< All SR error flags */

#define FLASH_FLAG_ECCC                 FLASH_ECCR_ECCC   /*!< FLASH ECC correction */
#define FLASH_FLAG_ECCD                 FLASH_ECCR_ECCD   /*!< FLASH ECC detection */

#define FLASH_FLAG_ECCR_ERRORS          (FLASH_FLAG_ECCC | FLASH_FLAG_ECCD)

#define FLASH_FLAG_ALL_ERRORS           (FLASH_FLAG_SR_ERRORS | FLASH_FLAG_ECCR_ERRORS)

/** @defgroup FLASH_INTERRUPT_DEFINITION FLASH Interrupts Definition
  * @brief FLASH Interrupt definition
  * @{
  */
#define FLASH_IT_EOP                    FLASH_CR_EOPIE     /*!< End of FLASH Operation Interrupt source */
#define FLASH_IT_OPERR                  FLASH_CR_ERRIE     /*!< Error Interrupt source */
#define FLASH_IT_RDERR                  FLASH_CR_RDERRIE   /*!< PCROP Read Error Interrupt source */
#define FLASH_IT_ECCC                   (FLASH_ECCR_ECCCIE >> FLASH_ECCR_ECCCIE_Pos)  /*!< ECC Correction Interrupt source */
/**
  * @}
  */

/** @defgroup FLASH_ERROR FLASH Error
  * @{
  */
#define HAL_FLASH_ERROR_NONE            0x00000000U
#define HAL_FLASH_ERROR_OP              FLASH_FLAG_OPERR
#define HAL_FLASH_ERROR_PROG            FLASH_FLAG_PROGERR
#define HAL_FLASH_ERROR_WRP             FLASH_FLAG_WRPERR
#define HAL_FLASH_ERROR_PGA             FLASH_FLAG_PGAERR
#define HAL_FLASH_ERROR_SIZ             FLASH_FLAG_SIZERR
#define HAL_FLASH_ERROR_PGS             FLASH_FLAG_PGSERR
#define HAL_FLASH_ERROR_MIS             FLASH_FLAG_MISERR
#define HAL_FLASH_ERROR_FAST            FLASH_FLAG_FASTERR
#define HAL_FLASH_ERROR_RD              FLASH_FLAG_RDERR
#define HAL_FLASH_ERROR_OPTV            FLASH_FLAG_OPTVERR
/**
  * @}
  */

/** @defgroup FLASH_TYPE_ERASE FLASH Erase Type
  * @{
  */
#define FLASH_TYPEERASE_PAGES           FLASH_CR_PER  /*!< Pages erase only*/
#define FLASH_TYPEERASE_MASSERASE       FLASH_CR_MER  /*!< Flash mass erase activation*/
/**
  * @}
  */

/** @defgroup FLASH_TYPE_PROGRAM FLASH Program Type
  * @{
  */
#define FLASH_TYPEPROGRAM_DOUBLEWORD    FLASH_CR_PG     /*!< Program a double-word (64-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_FAST          FLASH_CR_FSTPG  /*!< Fast program a 32 double-word (64-bit) row at a specified address.
                                                             And another 32 double-word (64-bit) row will be programmed */
/**
  * @}
  */

/** @defgroup FLASH_OB_TYPE FLASH Option Bytes Type
  * @{
  */
#define OPTIONBYTE_WRP                  0x00000001U  /*!< WRP option byte configuration             */
#define OPTIONBYTE_RDP                  0x00000002U  /*!< RDP option byte configuration             */
#define OPTIONBYTE_USER                 0x00000004U  /*!< User option byte configuration            */
#define OPTIONBYTE_PCROP                0x00000008U  /*!< PCROP option byte configuration           */
#if defined(DUAL_CORE)
#define OPTIONBYTE_IPCC_BUF_ADDR        0x00000010U  /*!< IPCC mailbox buffer address configuration */
#define OPTIONBYTE_C2_BOOT_VECT         0x00000100U  /*!< CPU2 Secure Boot reset vector             */
#define OPTIONBYTE_SECURE_MODE          0x00000200U  /*!< Secure mode on activated or not           */
#define OPTIONBYTE_C2_DEBUG_ACCESS      0x00000400U  /*!< CPU2 debug access enabled or disabled     */
#define OPTIONBYTE_SUBGHZSPI_SECURE_ACCESS  0x00000800U  /*!< Sub-GHz radio SPI security access enabled or disabled  */

#define OPTIONBYTE_ALL                  (OPTIONBYTE_WRP   | OPTIONBYTE_RDP           | OPTIONBYTE_USER         | \
                                         OPTIONBYTE_PCROP | OPTIONBYTE_IPCC_BUF_ADDR | OPTIONBYTE_C2_BOOT_VECT | \
                                         OPTIONBYTE_SECURE_MODE | OPTIONBYTE_C2_DEBUG_ACCESS | OPTIONBYTE_SUBGHZSPI_SECURE_ACCESS) /*!< All option byte configuration */
#else

#define OPTIONBYTE_ALL                  (OPTIONBYTE_WRP   | OPTIONBYTE_RDP           | OPTIONBYTE_USER         | \
                                         OPTIONBYTE_PCROP) /*!< All option byte configuration */
#endif /* DUAL_CORE */

/**
  * @}
  */

/** @defgroup FLASH_OB_WRP_AREA FLASH WRP Area
  * @{
  */
#define OB_WRPAREA_BANK1_AREAA          0x00000000U  /*!< Flash Area A */
#define OB_WRPAREA_BANK1_AREAB          0x00000001U  /*!< Flash Area B */
/**
  * @}
  */

/** @defgroup FLASH_OB_READ_PROTECTION FLASH Option Bytes Read Protection
  * @{
  */
#define OB_RDP_LEVEL_0                  0x000000AAU
#define OB_RDP_LEVEL_1                  0x000000BBU
#define OB_RDP_LEVEL_2                  0x000000CCU  /*!< Warning: When enabling read protection level 2
                                                          it's no more possible to go back to level 1 or 0 */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_TYPE FLASH Option Bytes User Type
  * @{
  */
#define OB_USER_BOR_LEV                 FLASH_OPTR_BOR_LEV    /*!< BOR reset Level */
#define OB_USER_nRST_STOP               FLASH_OPTR_nRST_STOP  /*!< Reset generated when entering the stop mode */
#define OB_USER_nRST_STDBY              FLASH_OPTR_nRST_STDBY /*!< Reset generated when entering the standby mode */
#define OB_USER_nRST_SHDW               FLASH_OPTR_nRST_SHDW  /*!< Reset generated when entering the shutdown mode */
#define OB_USER_IWDG_SW                 FLASH_OPTR_IWDG_SW    /*!< Independent watchdog selection */
#define OB_USER_IWDG_STOP               FLASH_OPTR_IWDG_STOP  /*!< Independent watchdog counter freeze in stop mode */
#define OB_USER_IWDG_STDBY              FLASH_OPTR_IWDG_STDBY /*!< Independent watchdog counter freeze in standby mode */
#define OB_USER_WWDG_SW                 FLASH_OPTR_WWDG_SW    /*!< Window watchdog selection */
#define OB_USER_nBOOT1                  FLASH_OPTR_nBOOT1     /*!< Boot configuration */
#define OB_USER_SRAM2_PE                FLASH_OPTR_SRAM2_PE   /*!< SRAM2 parity check enable     */
#define OB_USER_SRAM_RST                FLASH_OPTR_SRAM_RST   /*!< SRAM1 and SRAM2 erase when system reset */
#define OB_USER_nSWBOOT0                FLASH_OPTR_nSWBOOT0   /*!< Software BOOT0 */
#define OB_USER_nBOOT0                  FLASH_OPTR_nBOOT0     /*!< nBOOT0 option bit */
#define OB_USER_BOOT_LOCK               FLASH_OPTR_BOOT_LOCK  /*!< CPU1 Boot Lock enable option bit */
#if defined(DUAL_CORE)
#define OB_USER_C2BOOT_LOCK             FLASH_OPTR_C2BOOT_LOCK /*!< CPU2 Boot Lock enable option bit */
#endif

#if defined(DUAL_CORE)
#define OB_USER_ALL                     (OB_USER_BOR_LEV    | OB_USER_nRST_STOP | OB_USER_nRST_STDBY | \
                                         OB_USER_nRST_SHDW  | OB_USER_IWDG_SW   | OB_USER_IWDG_STOP  | \
                                         OB_USER_IWDG_STDBY | OB_USER_WWDG_SW   | OB_USER_nBOOT1     | \
                                         OB_USER_SRAM2_PE   | OB_USER_SRAM_RST  | OB_USER_nSWBOOT0   | \
                                         OB_USER_nBOOT0     | OB_USER_BOOT_LOCK | OB_USER_C2BOOT_LOCK)   /*!< all option bits */
#else
#define OB_USER_ALL                     (OB_USER_BOR_LEV    | OB_USER_nRST_STOP | OB_USER_nRST_STDBY | \
                                         OB_USER_nRST_SHDW  | OB_USER_IWDG_SW   | OB_USER_IWDG_STOP  | \
                                         OB_USER_IWDG_STDBY | OB_USER_WWDG_SW   | OB_USER_nBOOT1     | \
                                         OB_USER_SRAM2_PE   | OB_USER_SRAM_RST  | OB_USER_nSWBOOT0   | \
                                         OB_USER_nBOOT0     | OB_USER_BOOT_LOCK)   /*!< all option bits */
#endif

/**
  * @}
  */

/** @defgroup FLASH_OB_USER_BOR_LEVEL FLASH Option Bytes User BOR Level
  * @{
  */
#define OB_BOR_LEVEL_0                  0x00000000U                                   /*!< Reset level threshold is around 1.7V */
#define OB_BOR_LEVEL_1                  FLASH_OPTR_BOR_LEV_0                          /*!< Reset level threshold is around 2.0V */
#define OB_BOR_LEVEL_2                  FLASH_OPTR_BOR_LEV_1                          /*!< Reset level threshold is around 2.2V */
#define OB_BOR_LEVEL_3                  (FLASH_OPTR_BOR_LEV_0 | FLASH_OPTR_BOR_LEV_1) /*!< Reset level threshold is around 2.5V */
#define OB_BOR_LEVEL_4                  FLASH_OPTR_BOR_LEV_2                          /*!< Reset level threshold is around 2.8V */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_nRST_STOP FLASH Option Bytes User Reset On Stop
  * @{
  */
#define OB_STOP_RST                     0x00000000U           /*!< Reset generated when entering the stop mode    */
#define OB_STOP_NORST                   FLASH_OPTR_nRST_STOP  /*!< No reset generated when entering the stop mode */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_nRST_STANDBY FLASH Option Bytes User Reset On Standby
  * @{
  */
#define OB_STANDBY_RST                  0x00000000U           /*!< Reset generated when entering the standby mode    */
#define OB_STANDBY_NORST                FLASH_OPTR_nRST_STDBY /*!< No reset generated when entering the standby mode */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_nRST_SHUTDOWN FLASH Option Bytes User Reset On Shutdown
  * @{
  */
#define OB_SHUTDOWN_RST                 0x00000000U           /*!< Reset generated when entering the shutdown mode    */
#define OB_SHUTDOWN_NORST               FLASH_OPTR_nRST_SHDW  /*!< No reset generated when entering the shutdown mode */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_IWDG_SW FLASH Option Bytes User IWDG Type
  * @{
  */
#define OB_IWDG_HW                      0x00000000U         /*!< Hardware independent watchdog */
#define OB_IWDG_SW                      FLASH_OPTR_IWDG_SW  /*!< Software independent watchdog */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_IWDG_STOP FLASH Option Bytes User IWDG Mode On Stop
  * @{
  */
#define OB_IWDG_STOP_FREEZE             0x00000000U           /*!< Independent watchdog counter is frozen in Stop mode  */
#define OB_IWDG_STOP_RUN                FLASH_OPTR_IWDG_STOP  /*!< Independent watchdog counter is running in Stop mode */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_IWDG_STANDBY FLASH Option Bytes User IWDG Mode On Standby
  * @{
  */
#define OB_IWDG_STDBY_FREEZE            0x00000000U            /*!< Independent watchdog counter is frozen in Standby mode  */
#define OB_IWDG_STDBY_RUN               FLASH_OPTR_IWDG_STDBY  /*!< Independent watchdog counter is running in Standby mode */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_WWDG_SW FLASH Option Bytes User WWDG Type
  * @{
  */
#define OB_WWDG_HW                      0x00000000U         /*!< Hardware window watchdog */
#define OB_WWDG_SW                      FLASH_OPTR_WWDG_SW  /*!< Software window watchdog */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_SRAM2_PE FLASH Option Bytes SRAM2 parity check
  * @{
  */
#define OB_SRAM2_PARITY_ENABLE          0x00000000U          /*!< SRAM2 parity check enable  */
#define OB_SRAM2_PARITY_DISABLE         FLASH_OPTR_SRAM2_PE  /*!< SRAM2 parity check disable */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_SRAM_RST FLASH Option Bytes SRAM1 and SRAM2 erase when system reset
  * @{
  */
#define OB_SRAM_RST_ERASE               0x00000000U         /*!< SRAM2 and SRAM1 erased when a system reset        */
#define OB_SRAM_RST_NOT_ERASE           FLASH_OPTR_SRAM_RST /*!< SRAM2 and SRAM1 is not erased when a system reset */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_nBOOT1 FLASH Option Bytes User BOOT1 Type
  * @{
  */
#define OB_BOOT1_RESET                  0x00000000U        /*!< nBOOT1 = 0 */
#define OB_BOOT1_SET                    FLASH_OPTR_nBOOT1  /*!< nBOOT1 = 1 */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_nSWBOOT0 FLASH Option Bytes User Software BOOT0
  * @{
  */
#define OB_BOOT0_FROM_OB                0x00000000U          /*!< BOOT0 taken from the option bit nBOOT0 */
#define OB_BOOT0_FROM_PIN               FLASH_OPTR_nSWBOOT0  /*!< BOOT0 taken from PH3/BOOT0 pin         */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_nBOOT0 FLASH Option Bytes User nBOOT0 option bit
  * @{
  */
#define OB_BOOT0_RESET                  0x00000000U        /*!< nBOOT0 = 0 */
#define OB_BOOT0_SET                    FLASH_OPTR_nBOOT0  /*!< nBOOT0 = 1 */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_BOOT_LOCK FLASH Option Bytes CPU1 Boot Lock option bit
  * @{
  */
#define OB_BOOT_LOCK_DISABLE            0x00000000U           /*!< BOOT_LOCK = 0 */
#define OB_BOOT_LOCK_ENABLE             FLASH_OPTR_BOOT_LOCK  /*!< BOOT_LOCK = 1 */
/**
  * @}
  */

#if defined(DUAL_CORE)
/** @defgroup FLASH_OB_USER_C2BOOT_LOCK FLASH Option Bytes CPU2 Boot Lock option bit
  * @{
  */
#define OB_C2BOOT_LOCK_DISABLE          0x00000000U             /*!< C2BOOT_LOCK = 0 */
#define OB_C2BOOT_LOCK_ENABLE           FLASH_OPTR_C2BOOT_LOCK  /*!< C2BOOT_LOCK = 1 */
/**
  * @}
  */
#endif /* DUAL_CORE */

/** @defgroup FLASH_OB_PCROP_ZONE FLASH PCROP ZONE
  * @{
  */
#define OB_PCROP_ZONE_A                 0x00000001U  /*!< PCROP Zone A */
#define OB_PCROP_ZONE_B                 0x00000002U  /*!< PCROP Zone B */
/**
  * @}
  */

/** @defgroup FLASH_OB_PCROP_RDP FLASH Option Bytes PCROP On RDP Level Type
  * @{
  */
#define OB_PCROP_RDP_NOT_ERASE          0x00000000U                /*!< PCROP area is not erased when the RDP level
                                                                        is decreased from Level 1 to Level 0 */
#define OB_PCROP_RDP_ERASE              FLASH_PCROP1AER_PCROP_RDP  /*!< PCROP area is erased when the RDP level is
                                                                        decreased from Level 1 to Level 0 (full mass erase) */
/**
  * @}
  */

#if defined(DUAL_CORE)
/** @defgroup FLASH_OB_SECURITY_MODE Option Bytes FLASH Secure mode
  * @{
  */
#define OB_SECURE_SYSTEM_AND_FLASH_ENABLE       0x00000001U     /*!< Flash and System secure area enabled  */
#define OB_SECURE_HIDE_PROTECTION_ENABLE        0x00000002U     /*!< Hide Protection area enabled          */
#define OB_SECURE_SRAM1_ENABLE                  0x00000004U     /*!< SRAM1 area enabled                    */
#define OB_SECURE_SRAM2_ENABLE                  0x00000008U     /*!< SRAM2 area enabled                    */
#define OB_SECURE_SYSTEM_AND_FLASH_DISABLE      0x00000010U     /*!< Flash and System secure area disabled */
#define OB_SECURE_HIDE_PROTECTION_DISABLE       0x00000020U     /*!< Hide Protection area disabled         */
#define OB_SECURE_SRAM1_DISABLE                 0x00000040U     /*!< SRAM1 area disabled                   */
#define OB_SECURE_SRAM2_DISABLE                 0x00000080U     /*!< SRAM2 area disabled                   */
#define OB_SECURE_SYSTEM_AND_ALL_AREAS_ENABLE   (OB_SECURE_SYSTEM_AND_FLASH_ENABLE | OB_SECURE_HIDE_PROTECTION_ENABLE | \
                                                 OB_SECURE_SRAM1_ENABLE            | OB_SECURE_SRAM2_ENABLE)               /*!< All System in Secure mode : Security enabled on all areas */
#define OB_SECURE_SYSTEM_AND_ALL_AREAS_DISABLE  (OB_SECURE_SYSTEM_AND_FLASH_DISABLE | OB_SECURE_HIDE_PROTECTION_DISABLE | \
                                                 OB_SECURE_SRAM1_DISABLE            | OB_SECURE_SRAM2_DISABLE)               /*!< Unsecure mode: Security disabled      */
/**
  * @}
  */

/** @defgroup FLASH_OB_SUBGHZSPI_SECURE_ACCESS Option Bytes Sub-GHz radio SPI Secure Access
  * @{
  */
#define OB_SUBGHZSPI_SECURE_ACCESS_DISABLE      FLASH_SFR_SUBGHZSPISD  /*!< Sub-GHz radio SPI Secure access disabled  */
#define OB_SUBGHZSPI_SECURE_ACCESS_ENABLE       0x00000000U       /*!< Sub-GHz radio SPI Secure access enabled   */
/**
  * @}
  */

/** @defgroup FLASH_OB_C2_DEBUG_ACCESS Option Bytes CPU2 Debug Access
  * @{
  */
#define OB_C2_DEBUG_ACCESS_DISABLE         FLASH_SFR_DDS /*!< CPU2 debug access disabled   */
#define OB_C2_DEBUG_ACCESS_ENABLE          0x00000000U   /*!< CPU2 debug access enabled (when also enabled by FLASH_ACR2_C2SWDBGEN) */
/**
  * @}
  */

/** @defgroup FLASH_OB_C2_BOOT_REGION CPU2 Option Bytes Reset Boot Vector
  * @{
  */
#define OB_C2_BOOT_FROM_SRAM               0x00000000U        /*!< CPU2 boot from SRAM1 or SRAM2 */
#define OB_C2_BOOT_FROM_FLASH              FLASH_SRRVR_C2OPT  /*!< CPU2 boot from Flash          */
/**
  * @}
  */
#endif /* DUAL_CORE */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup FLASH_Exported_Macros FLASH Exported Macros
  *  @brief macros to control FLASH features
  *  @{
  */

/**
  * @brief  Set the FLASH Latency.
  * @param __LATENCY__ FLASH Latency
  *         This parameter can be one of the following values :
  *     @arg @ref FLASH_LATENCY_0 FLASH Zero wait state
  *     @arg @ref FLASH_LATENCY_1 FLASH One wait state
  *     @arg @ref FLASH_LATENCY_2 FLASH Two wait states
  * @retval None
  */
#define __HAL_FLASH_SET_LATENCY(__LATENCY__)    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, (__LATENCY__))

/**
  * @brief  Get the FLASH Latency.
  * @retval FLASH Latency
  *         Returned value can be one of the following values :
  *     @arg @ref FLASH_LATENCY_0 FLASH Zero wait state
  *     @arg @ref FLASH_LATENCY_1 FLASH One wait state
  *     @arg @ref FLASH_LATENCY_2 FLASH Two wait states
  */
#define __HAL_FLASH_GET_LATENCY()               READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY)

/**
  * @brief  Enable the FLASH prefetch buffer.
  * @retval None
  */
#ifdef CORE_CM0PLUS
#define __HAL_FLASH_PREFETCH_BUFFER_ENABLE()    SET_BIT(FLASH->C2ACR, FLASH_C2ACR_PRFTEN)
#else
#define __HAL_FLASH_PREFETCH_BUFFER_ENABLE()    SET_BIT(FLASH->ACR, FLASH_ACR_PRFTEN)
#endif

/**
  * @brief  Disable the FLASH prefetch buffer.
  * @retval None
  */
#ifdef CORE_CM0PLUS
#define __HAL_FLASH_PREFETCH_BUFFER_DISABLE()   CLEAR_BIT(FLASH->C2ACR, FLASH_C2ACR_PRFTEN)
#else
#define __HAL_FLASH_PREFETCH_BUFFER_DISABLE()   CLEAR_BIT(FLASH->ACR, FLASH_ACR_PRFTEN)
#endif

/**
  * @brief  Enable the FLASH instruction cache.
  * @retval none
  */
#ifdef CORE_CM0PLUS
#define __HAL_FLASH_INSTRUCTION_CACHE_ENABLE()  SET_BIT(FLASH->C2ACR, FLASH_C2ACR_ICEN)
#else
#define __HAL_FLASH_INSTRUCTION_CACHE_ENABLE()  SET_BIT(FLASH->ACR, FLASH_ACR_ICEN)
#endif

/**
  * @brief  Disable the FLASH instruction cache.
  * @retval none
  */
#ifdef CORE_CM0PLUS
#define __HAL_FLASH_INSTRUCTION_CACHE_DISABLE() CLEAR_BIT(FLASH->C2ACR, FLASH_C2ACR_ICEN)
#else
#define __HAL_FLASH_INSTRUCTION_CACHE_DISABLE() CLEAR_BIT(FLASH->ACR, FLASH_ACR_ICEN)
#endif

/**
  * @brief  Enable the FLASH data cache.
  * @retval none
  */
#ifdef CORE_CM0PLUS
#else
#define __HAL_FLASH_DATA_CACHE_ENABLE()         SET_BIT(FLASH->ACR, FLASH_ACR_DCEN)
#endif

/**
  * @brief  Disable the FLASH data cache.
  * @retval none
  */
#ifdef CORE_CM0PLUS
#else
#define __HAL_FLASH_DATA_CACHE_DISABLE()        CLEAR_BIT(FLASH->ACR, FLASH_ACR_DCEN)
#endif

/**
  * @brief  Reset the FLASH instruction Cache.
  * @note   This function must be used only when the Instruction Cache is disabled.
  * @retval None
  */
#ifdef CORE_CM0PLUS
#define __HAL_FLASH_INSTRUCTION_CACHE_RESET()   do { SET_BIT(FLASH->C2ACR, FLASH_C2ACR_ICRST);   \
                                                     CLEAR_BIT(FLASH->C2ACR, FLASH_C2ACR_ICRST); \
                                                   } while (0)
#else
#define __HAL_FLASH_INSTRUCTION_CACHE_RESET()   do { SET_BIT(FLASH->ACR, FLASH_ACR_ICRST);   \
                                                     CLEAR_BIT(FLASH->ACR, FLASH_ACR_ICRST); \
                                                   } while (0)
#endif

/**
  * @brief  Reset the FLASH data Cache.
  * @note   This function must be used only when the data Cache is disabled.
  * @retval None
  */
#ifdef CORE_CM0PLUS
#else
#define __HAL_FLASH_DATA_CACHE_RESET()          do { SET_BIT(FLASH->ACR, FLASH_ACR_DCRST);   \
                                                     CLEAR_BIT(FLASH->ACR, FLASH_ACR_DCRST); \
                                                   } while (0)
#endif

/**
  * @}
  */

/** @defgroup FLASH_Interrupt FLASH Interrupts Macros
 *  @brief macros to handle FLASH interrupts
 * @{
 */

/**
  * @brief  Enable the specified FLASH interrupt.
  * @param __INTERRUPT__ FLASH interrupt
  *         This parameter can be any combination of the following values:
  *     @arg @ref FLASH_IT_EOP End of FLASH Operation Interrupt
  *     @arg @ref FLASH_IT_OPERR Error Interrupt
  *     @arg @ref FLASH_IT_RDERR PCROP Read Error Interrupt
  *     @arg @ref FLASH_IT_ECCC ECC Correction Interrupt
  * @retval none
  */
#ifdef CORE_CM0PLUS
#define __HAL_FLASH_ENABLE_IT(__INTERRUPT__)    do { if(((__INTERRUPT__) & FLASH_IT_ECCC) != 0U) { SET_BIT(FLASH->ECCR, FLASH_ECCR_ECCCIE); }\
                                                     if(((__INTERRUPT__) & (~FLASH_IT_ECCC)) != 0U) { SET_BIT(FLASH->C2CR, ((__INTERRUPT__) & (~FLASH_IT_ECCC))); }\
                                                   } while(0)
#else
#define __HAL_FLASH_ENABLE_IT(__INTERRUPT__)    do { if(((__INTERRUPT__) & FLASH_IT_ECCC) != 0U) { SET_BIT(FLASH->ECCR, FLASH_ECCR_ECCCIE); }\
                                                     if(((__INTERRUPT__) & (~FLASH_IT_ECCC)) != 0U) { SET_BIT(FLASH->CR, ((__INTERRUPT__) & (~FLASH_IT_ECCC))); }\
                                                   } while(0)
#endif

/**
  * @brief  Disable the specified FLASH interrupt.
  * @param __INTERRUPT__ FLASH interrupt
  *         This parameter can be any combination of the following values:
  *     @arg @ref FLASH_IT_EOP End of FLASH Operation Interrupt
  *     @arg @ref FLASH_IT_OPERR Error Interrupt
  *     @arg @ref FLASH_IT_RDERR PCROP Read Error Interrupt
  *     @arg @ref FLASH_IT_ECCC ECC Correction Interrupt
  * @retval none
  */
#ifdef CORE_CM0PLUS
#define __HAL_FLASH_DISABLE_IT(__INTERRUPT__)   do { if(((__INTERRUPT__) & FLASH_IT_ECCC) != 0U) { CLEAR_BIT(FLASH->ECCR, FLASH_ECCR_ECCCIE); }\
                                                     if(((__INTERRUPT__) & (~FLASH_IT_ECCC)) != 0U) { CLEAR_BIT(FLASH->C2CR, ((__INTERRUPT__) & (~FLASH_IT_ECCC))); }\
                                                   } while(0)
#else
#define __HAL_FLASH_DISABLE_IT(__INTERRUPT__)   do { if(((__INTERRUPT__) & FLASH_IT_ECCC) != 0U) { CLEAR_BIT(FLASH->ECCR, FLASH_ECCR_ECCCIE); }\
                                                     if(((__INTERRUPT__) & (~FLASH_IT_ECCC)) != 0U) { CLEAR_BIT(FLASH->CR, ((__INTERRUPT__) & (~FLASH_IT_ECCC))); }\
                                                   } while(0)
#endif

/**
  * @brief  Check whether the specified FLASH flag is set or not.
  * @param __FLAG__ specifies the FLASH flag to check.
  *   This parameter can be one of the following values:
  *     @arg @ref FLASH_FLAG_EOP FLASH End of Operation flag
  *     @arg @ref FLASH_FLAG_OPERR FLASH Operation error flag
  *     @arg @ref FLASH_FLAG_PROGERR FLASH Programming error flag
  *     @arg @ref FLASH_FLAG_WRPERR FLASH Write protection error flag
  *     @arg @ref FLASH_FLAG_PGAERR FLASH Programming alignment error flag
  *     @arg @ref FLASH_FLAG_SIZERR FLASH Size error flag
  *     @arg @ref FLASH_FLAG_PGSERR FLASH Programming sequence error flag
  *     @arg @ref FLASH_FLAG_MISERR FLASH Fast programming data miss error flag
  *     @arg @ref FLASH_FLAG_FASTERR FLASH Fast programming error flag
  *     @arg @ref FLASH_FLAG_OPTNV FLASH User Option OPTVAL indication
  *     @arg @ref FLASH_FLAG_RDERR FLASH PCROP read  error flag
  *     @arg @ref FLASH_FLAG_OPTVERR FLASH Option validity error flag
  *     @arg @ref FLASH_FLAG_BSY FLASH write/erase operations in progress flag
  *     @arg @ref FLASH_FLAG_CFGBSY Programming/erase configuration busy
  *     @arg @ref FLASH_FLAG_PESD FLASH Programming/erase operation suspended
  *     @arg @ref FLASH_FLAG_ECCC FLASH one ECC error has been detected and corrected
  *     @arg @ref FLASH_FLAG_ECCD FLASH two ECC errors have been detected
  * @retval The new state of FLASH_FLAG (SET or RESET).
  */
#ifdef CORE_CM0PLUS
#define __HAL_FLASH_GET_FLAG(__FLAG__)          ((((__FLAG__) & (FLASH_FLAG_ECCR_ERRORS)) != 0U) ? \
                                                 (READ_BIT(FLASH->ECCR, (__FLAG__)) == (__FLAG__))  : \
                                                 ((((__FLAG__) & FLASH_FLAG_OPTVERR) != 0U) ? \
                                                  (READ_BIT(FLASH->SR,   (__FLAG__)) == (__FLAG__))  : \
                                                  (READ_BIT(FLASH->C2SR,   (__FLAG__)) == (__FLAG__))))
#else
#define __HAL_FLASH_GET_FLAG(__FLAG__)          ((((__FLAG__) & (FLASH_FLAG_ECCR_ERRORS)) != 0U) ? \
                                                 (READ_BIT(FLASH->ECCR, (__FLAG__)) == (__FLAG__))  : \
                                                 (READ_BIT(FLASH->SR,   (__FLAG__)) == (__FLAG__)))
#endif
/**
  * @brief  Clear the FLASH's pending flags.
  * @param __FLAG__ specifies the FLASH flags to clear.
  *   This parameter can be any combination of the following values:
  *     @arg @ref FLASH_FLAG_EOP FLASH End of Operation flag
  *     @arg @ref FLASH_FLAG_OPERR FLASH Operation error flag
  *     @arg @ref FLASH_FLAG_PROGERR FLASH Programming error flag
  *     @arg @ref FLASH_FLAG_WRPERR FLASH Write protection error flag
  *     @arg @ref FLASH_FLAG_PGAERR FLASH Programming alignment error flag
  *     @arg @ref FLASH_FLAG_SIZERR FLASH Size error flag
  *     @arg @ref FLASH_FLAG_PGSERR FLASH Programming sequence error flag
  *     @arg @ref FLASH_FLAG_MISERR FLASH Fast programming data miss error flag
  *     @arg @ref FLASH_FLAG_FASTERR FLASH Fast programming error flag
  *     @arg @ref FLASH_FLAG_RDERR FLASH PCROP read  error flag
  *     @arg @ref FLASH_FLAG_OPTVERR FLASH Option validity error flag
  *     @arg @ref FLASH_FLAG_ECCC FLASH one ECC error has been detected and corrected
  *     @arg @ref FLASH_FLAG_ECCD FLASH two ECC errors have been detected
  *     @arg @ref FLASH_FLAG_SR_ERRORS FLASH All SR errors flags
  *     @arg @ref FLASH_FLAG_ECCR_ERRORS FLASH All ECCR errors flags
  *     @arg @ref FLASH_FLAG_ALL_ERRORS FLASH All errors flags
  * @retval None
  */
#ifdef CORE_CM0PLUS
#define __HAL_FLASH_CLEAR_FLAG(__FLAG__)        do { if(((__FLAG__) & (FLASH_FLAG_ECCR_ERRORS)) != 0U) { SET_BIT(FLASH->ECCR, ((__FLAG__) & (FLASH_FLAG_ECCR_ERRORS))); }\
                                                     if(((__FLAG__) & FLASH_FLAG_OPTVERR) != 0U) { SET_BIT(FLASH->SR, FLASH_FLAG_OPTVERR); }\
                                                     if(((__FLAG__) & ~(FLASH_FLAG_ECCR_ERRORS)) != 0U) { WRITE_REG(FLASH->C2SR, ((__FLAG__) & ~(FLASH_FLAG_ECCR_ERRORS))); }\
                                                   } while(0)
#else
#define __HAL_FLASH_CLEAR_FLAG(__FLAG__)        do { if(((__FLAG__) & (FLASH_FLAG_ECCR_ERRORS)) != 0U) { SET_BIT(FLASH->ECCR, ((__FLAG__) & (FLASH_FLAG_ECCR_ERRORS))); }\
                                                     if(((__FLAG__) & ~(FLASH_FLAG_ECCR_ERRORS)) != 0U) { WRITE_REG(FLASH->SR, ((__FLAG__) & ~(FLASH_FLAG_ECCR_ERRORS))); }\
                                                   } while(0)
#endif
/**
  * @}
  */

/* Include FLASH HAL Extended module */
#include "stm32wlxx_hal_flash_ex.h"
/* Exported variables --------------------------------------------------------*/
/** @defgroup FLASH_Exported_Variables FLASH Exported Variables
  * @{
  */
extern FLASH_ProcessTypeDef pFlash;
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup FLASH_Exported_Functions
  * @{
  */

/* Program operation functions  ***********************************************/
/** @addtogroup FLASH_Exported_Functions_Group1
  * @{
  */
HAL_StatusTypeDef  HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);
HAL_StatusTypeDef  HAL_FLASH_Program_IT(uint32_t TypeProgram, uint32_t Address, uint64_t Data);
/* FLASH IRQ handler method */
void               HAL_FLASH_IRQHandler(void);
/* Callbacks in non blocking modes */
void               HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue);
void               HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue);
/**
  * @}
  */

/* Peripheral Control functions  **********************************************/
/** @addtogroup FLASH_Exported_Functions_Group2
  * @{
  */
HAL_StatusTypeDef  HAL_FLASH_Unlock(void);
HAL_StatusTypeDef  HAL_FLASH_Lock(void);
/* Option bytes control */
HAL_StatusTypeDef  HAL_FLASH_OB_Unlock(void);
HAL_StatusTypeDef  HAL_FLASH_OB_Lock(void);
HAL_StatusTypeDef  HAL_FLASH_OB_Launch(void);
/**
  * @}
  */

/* Peripheral State functions  ************************************************/
/** @addtogroup FLASH_Exported_Functions_Group3
  * @{
  */
uint32_t HAL_FLASH_GetError(void);
/**
  * @}
  */

/**
  * @}
  */

/* Private types --------------------------------------------------------*/
/** @defgroup FLASH_Private_types FLASH Private Types
  * @{
  */
HAL_StatusTypeDef  FLASH_WaitForLastOperation(uint32_t Timeout);
/**
  * @}
  */

/* Private constants --------------------------------------------------------*/
/** @defgroup FLASH_Private_Constants FLASH Private Constants
  * @{
  */
#define FLASH_END_ADDR                          (FLASH_BASE + FLASH_SIZE - 1U)

#define FLASH_BANK_SIZE                         FLASH_SIZE   /*!< FLASH Bank Size */
#define FLASH_PAGE_SIZE                         0x00000800U  /*!< FLASH Page Size, 2 KBytes */
#define FLASH_PAGE_NB                           128U
#define FLASH_TIMEOUT_VALUE                     1000U        /*!< FLASH Execution Timeout, 1 s */

#define FLASH_PCROP_GRANULARITY_OFFSET          10U                                        /*!< FLASH Code Readout Protection granularity offset */
#define FLASH_PCROP_GRANULARITY                 (1UL << FLASH_PCROP_GRANULARITY_OFFSET)    /*!< FLASH Code Readout Protection granularity, 1 KBytes */

#define FLASH_TYPENONE                          0x00000000U                                /*!< No Programmation Procedure On Going */

/** @defgroup SRAM_MEMORY_SIZE  SRAM memory size
  * @{
  */
#define SRAM_SECURE_PAGE_GRANULARITY_OFFSET     10U                                      /*!< Secure SRAM1 and SRAM2 Protection granularity offset */
#define SRAM_SECURE_PAGE_GRANULARITY            (1UL << FLASH_PCROP_GRANULARITY_OFFSET)  /*!< Secure SRAM1 and SRAM2 Protection granularity, 1KBytes */
/**
  * @}
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup FLASH_Private_Macros FLASH Private Macros
 *  @{
 */
#define IS_FLASH_MAIN_MEM_ADDRESS(__VALUE__)        (((__VALUE__) >= FLASH_BASE) && ((__VALUE__) <= (FLASH_BASE + FLASH_SIZE - 1UL)))

#define IS_FLASH_FAST_PROGRAM_ADDRESS(__VALUE__)    (((__VALUE__) >= FLASH_BASE) && ((__VALUE__) <= (FLASH_BASE + FLASH_SIZE - 256UL)) && (((__VALUE__) % 256UL) == 0UL))

#define IS_FLASH_PROGRAM_MAIN_MEM_ADDRESS(__VALUE__)   (((__VALUE__) >= FLASH_BASE) && ((__VALUE__) <= (FLASH_BASE + FLASH_SIZE - 8UL)) && (((__VALUE__) % 8UL) == 0UL))

#define IS_FLASH_PROGRAM_OTP_ADDRESS(__VALUE__)     (((__VALUE__) >= OTP_AREA_BASE) && ((__VALUE__) <= (OTP_AREA_END_ADDR + 1UL - 8UL)) && (((__VALUE__) % 8UL) == 0UL))

#define IS_FLASH_PROGRAM_ADDRESS(__VALUE__)         (IS_FLASH_PROGRAM_MAIN_MEM_ADDRESS(__VALUE__) || IS_FLASH_PROGRAM_OTP_ADDRESS(__VALUE__))

#define IS_FLASH_PAGE(__VALUE__)                    ((__VALUE__) < FLASH_PAGE_NB)

#define IS_ADDR_ALIGNED_64BITS(__VALUE__)           (((__VALUE__) & 0x7U) == (0x00UL))

#define IS_FLASH_TYPEERASE(__VALUE__)               (((__VALUE__) == FLASH_TYPEERASE_PAGES) || \
                                                     ((__VALUE__) == FLASH_TYPEERASE_MASSERASE))

#define IS_FLASH_TYPEPROGRAM(__VALUE__)             (((__VALUE__) == FLASH_TYPEPROGRAM_DOUBLEWORD) || \
                                                     ((__VALUE__) == FLASH_TYPEPROGRAM_FAST))

#if defined(DUAL_CORE)
#define IS_OB_SFSA_START_ADDR(__VALUE__)            (((__VALUE__) >= FLASH_BASE) && ((__VALUE__) <= FLASH_END_ADDR) && (((__VALUE__) & ~0x7FFU) == (__VALUE__)))
#define IS_OB_HDPSA_START_ADDR(__VALUE__)           IS_OB_SFSA_START_ADDR(__VALUE__)
#define IS_OB_SBRSA_START_ADDR(__VALUE__)           (((__VALUE__) >= SRAM2_BASE) && ((__VALUE__) <= (SRAM2_BASE + SRAM2_SIZE)) && (((__VALUE__) & ~0x3FFU) == (__VALUE__)))
#define IS_OB_SNBRSA_START_ADDR(__VALUE__)          (((__VALUE__) >= SRAM1_BASE) && ((__VALUE__) <= (SRAM1_BASE + SRAM1_SIZE)) && (((__VALUE__) & ~0x3FFU) == (__VALUE__)))

#define IS_OB_SECURE_MODE(__VALUE__)                ( (((__VALUE__) &  (OB_SECURE_SYSTEM_AND_ALL_AREAS_ENABLE | OB_SECURE_SYSTEM_AND_ALL_AREAS_DISABLE)) != 0U)  && \
                                                      (((__VALUE__) & ~(OB_SECURE_SYSTEM_AND_ALL_AREAS_ENABLE | OB_SECURE_SYSTEM_AND_ALL_AREAS_DISABLE)) == 0U)  && \
                                                      (((__VALUE__) &  (OB_SECURE_SYSTEM_AND_FLASH_ENABLE     | OB_SECURE_SYSTEM_AND_FLASH_DISABLE))     != (OB_SECURE_SYSTEM_AND_FLASH_ENABLE | OB_SECURE_SYSTEM_AND_FLASH_DISABLE)) && \
                                                      (((__VALUE__) &  (OB_SECURE_HIDE_PROTECTION_ENABLE      | OB_SECURE_HIDE_PROTECTION_DISABLE))      != (OB_SECURE_HIDE_PROTECTION_ENABLE  | OB_SECURE_HIDE_PROTECTION_DISABLE))  && \
                                                      (((__VALUE__) &  (OB_SECURE_SRAM1_ENABLE                | OB_SECURE_SRAM1_DISABLE))                != (OB_SECURE_SRAM1_ENABLE            | OB_SECURE_SRAM1_DISABLE))            && \
                                                      (((__VALUE__) &  (OB_SECURE_SRAM2_ENABLE                | OB_SECURE_SRAM2_DISABLE))                != (OB_SECURE_SRAM2_ENABLE            | OB_SECURE_SRAM2_DISABLE)) )
#endif /* DUAL_CORE */

#if defined(DUAL_CORE)
#define IS_OPTIONBYTE(__VALUE__)                    ((__VALUE__) <= (OPTIONBYTE_WRP | OPTIONBYTE_RDP | OPTIONBYTE_USER | OPTIONBYTE_PCROP | \
                                                              OPTIONBYTE_IPCC_BUF_ADDR | OPTIONBYTE_C2_BOOT_VECT | OPTIONBYTE_SECURE_MODE | \
                                                              OPTIONBYTE_C2_DEBUG_ACCESS | OPTIONBYTE_SUBGHZSPI_SECURE_ACCESS))

#else

#define IS_OPTIONBYTE(__VALUE__)                    ((__VALUE__) <= (OPTIONBYTE_WRP | OPTIONBYTE_RDP | OPTIONBYTE_USER | OPTIONBYTE_PCROP))
#endif /* DUAL_CORE */

#define IS_OB_WRPAREA(__VALUE__)                    (((__VALUE__) == OB_WRPAREA_BANK1_AREAA) || ((__VALUE__) == OB_WRPAREA_BANK1_AREAB))

#define IS_OB_RDP_LEVEL(__VALUE__)                  (((__VALUE__) == OB_RDP_LEVEL_0)   ||\
                                                     ((__VALUE__) == OB_RDP_LEVEL_1)   ||\
                                                     ((__VALUE__) == OB_RDP_LEVEL_2))

#define IS_OB_USER_TYPE(__VALUE__)                  ((((__VALUE__) & OB_USER_ALL) != 0U) && \
                                                     (((__VALUE__) & ~OB_USER_ALL) == 0U))

#define IS_OB_USER_CONFIG(__TYPE__, __VALUE__)      ((((__TYPE__) & OB_USER_BOR_LEV) == OB_USER_BOR_LEV) \
                                                      ? ((((__VALUE__) & ~(OB_USER_ALL & ~OB_USER_BOR_LEV)) == OB_BOR_LEVEL_0) || \
                                                         (((__VALUE__) & ~(OB_USER_ALL & ~OB_USER_BOR_LEV)) == OB_BOR_LEVEL_1) || \
                                                         (((__VALUE__) & ~(OB_USER_ALL & ~OB_USER_BOR_LEV)) == OB_BOR_LEVEL_2) || \
                                                         (((__VALUE__) & ~(OB_USER_ALL & ~OB_USER_BOR_LEV)) == OB_BOR_LEVEL_3) || \
                                                         (((__VALUE__) & ~(OB_USER_ALL & ~OB_USER_BOR_LEV)) == OB_BOR_LEVEL_4)) \
                                                      : ((~(__TYPE__) & (__VALUE__)) == 0U))

#define IS_OB_USER_BOR_LEVEL(__VALUE__)             (((__VALUE__) == OB_BOR_LEVEL_0) || ((__VALUE__) == OB_BOR_LEVEL_1) || \
                                                     ((__VALUE__) == OB_BOR_LEVEL_2) || ((__VALUE__) == OB_BOR_LEVEL_3) || \
                                                     ((__VALUE__) == OB_BOR_LEVEL_4))

#define IS_OB_PCROP_CONFIG(__VALUE__)               (((__VALUE__) & ~(OB_PCROP_ZONE_A | OB_PCROP_ZONE_B | OB_PCROP_RDP_ERASE)) == 0U)

#if defined(DUAL_CORE)
#define IS_OB_IPCC_BUF_ADDR(__VALUE__)              (IS_OB_SBRSA_START_ADDR(__VALUE__) || IS_OB_SNBRSA_START_ADDR(__VALUE__))

#define IS_OB_BOOT_VECTOR_ADDR(__VALUE__)           (IS_OB_SFSA_START_ADDR(__VALUE__) || IS_OB_SBRSA_START_ADDR(__VALUE__) || IS_OB_SNBRSA_START_ADDR(__VALUE__))
#define IS_OB_BOOT_REGION(__VALUE__)                (((__VALUE__) == OB_C2_BOOT_FROM_FLASH) || ((__VALUE__) == OB_C2_BOOT_FROM_SRAM))

#define IS_OB_SUBGHZSPI_SECURE_ACCESS(__VALUE__)         (((__VALUE__) == OB_SUBGHZSPI_SECURE_ACCESS_ENABLE) || ((__VALUE__) == OB_SUBGHZSPI_SECURE_ACCESS_DISABLE))

#define IS_OB_C2_DEBUG_MODE(__VALUE__)              (((__VALUE__) == OB_C2_DEBUG_ACCESS_ENABLE) || ((__VALUE__) == OB_C2_DEBUG_ACCESS_DISABLE))
#endif /* DUAL_CORE */

#define IS_FLASH_LATENCY(__VALUE__)                 (((__VALUE__) == FLASH_LATENCY_0) || \
                                                     ((__VALUE__) == FLASH_LATENCY_1) || \
                                                     ((__VALUE__) == FLASH_LATENCY_2))

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

#endif /* STM32WLxx_HAL_FLASH_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
