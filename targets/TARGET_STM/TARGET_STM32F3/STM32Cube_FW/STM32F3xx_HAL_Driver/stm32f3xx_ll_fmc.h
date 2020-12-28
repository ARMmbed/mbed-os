/**
  ******************************************************************************
  * @file    stm32f3xx_ll_fmc.h
  * @author  MCD Application Team
  * @brief   Header file of FMC HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
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
#ifndef STM32F3xx_LL_FMC_H
#define STM32F3xx_LL_FMC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal_def.h"

/** @addtogroup STM32F3xx_HAL_Driver
  * @{
  */

/** @addtogroup FMC_LL
  * @{
  */

/** @addtogroup FMC_LL_Private_Macros
  * @{
  */
#if defined FMC_BANK1

#define IS_FMC_NORSRAM_BANK(__BANK__) (((__BANK__) == FMC_NORSRAM_BANK1) || \
                                       ((__BANK__) == FMC_NORSRAM_BANK2) || \
                                       ((__BANK__) == FMC_NORSRAM_BANK3) || \
                                       ((__BANK__) == FMC_NORSRAM_BANK4))
#define IS_FMC_MUX(__MUX__) (((__MUX__) == FMC_DATA_ADDRESS_MUX_DISABLE) || \
                             ((__MUX__) == FMC_DATA_ADDRESS_MUX_ENABLE))
#define IS_FMC_MEMORY(__MEMORY__) (((__MEMORY__) == FMC_MEMORY_TYPE_SRAM) || \
                                   ((__MEMORY__) == FMC_MEMORY_TYPE_PSRAM)|| \
                                   ((__MEMORY__) == FMC_MEMORY_TYPE_NOR))
#define IS_FMC_NORSRAM_MEMORY_WIDTH(__WIDTH__) (((__WIDTH__) == FMC_NORSRAM_MEM_BUS_WIDTH_8)  || \
                                                ((__WIDTH__) == FMC_NORSRAM_MEM_BUS_WIDTH_16) || \
                                                ((__WIDTH__) == FMC_NORSRAM_MEM_BUS_WIDTH_32))
#define IS_FMC_ACCESS_MODE(__MODE__) (((__MODE__) == FMC_ACCESS_MODE_A) || \
                                      ((__MODE__) == FMC_ACCESS_MODE_B) || \
                                      ((__MODE__) == FMC_ACCESS_MODE_C) || \
                                      ((__MODE__) == FMC_ACCESS_MODE_D))
#define IS_FMC_BURSTMODE(__STATE__) (((__STATE__) == FMC_BURST_ACCESS_MODE_DISABLE) || \
                                     ((__STATE__) == FMC_BURST_ACCESS_MODE_ENABLE))
#define IS_FMC_WAIT_POLARITY(__POLARITY__) (((__POLARITY__) == FMC_WAIT_SIGNAL_POLARITY_LOW) || \
                                            ((__POLARITY__) == FMC_WAIT_SIGNAL_POLARITY_HIGH))
#define IS_FMC_WRAP_MODE(__MODE__) (((__MODE__) == FMC_WRAP_MODE_DISABLE) || \
                                     ((__MODE__) == FMC_WRAP_MODE_ENABLE))     
#define IS_FMC_WAIT_SIGNAL_ACTIVE(__ACTIVE__) (((__ACTIVE__) == FMC_WAIT_TIMING_BEFORE_WS) || \
                                               ((__ACTIVE__) == FMC_WAIT_TIMING_DURING_WS))
#define IS_FMC_WRITE_OPERATION(__OPERATION__) (((__OPERATION__) == FMC_WRITE_OPERATION_DISABLE) || \
                                               ((__OPERATION__) == FMC_WRITE_OPERATION_ENABLE))
#define IS_FMC_WAITE_SIGNAL(__SIGNAL__) (((__SIGNAL__) == FMC_WAIT_SIGNAL_DISABLE) || \
                                         ((__SIGNAL__) == FMC_WAIT_SIGNAL_ENABLE))
#define IS_FMC_EXTENDED_MODE(__MODE__) (((__MODE__) == FMC_EXTENDED_MODE_DISABLE) || \
                                        ((__MODE__) == FMC_EXTENDED_MODE_ENABLE))
#define IS_FMC_ASYNWAIT(__STATE__) (((__STATE__) == FMC_ASYNCHRONOUS_WAIT_DISABLE) || \
                                    ((__STATE__) == FMC_ASYNCHRONOUS_WAIT_ENABLE))
#define IS_FMC_DATA_LATENCY(__LATENCY__) (((__LATENCY__) > 1U) && ((__LATENCY__) <= 17U))
#define IS_FMC_WRITE_BURST(__BURST__) (((__BURST__) == FMC_WRITE_BURST_DISABLE) || \
                                       ((__BURST__) == FMC_WRITE_BURST_ENABLE))
#define IS_FMC_CONTINOUS_CLOCK(__CCLOCK__) (((__CCLOCK__) == FMC_CONTINUOUS_CLOCK_SYNC_ONLY) || \
                                            ((__CCLOCK__) == FMC_CONTINUOUS_CLOCK_SYNC_ASYNC))
#define IS_FMC_ADDRESS_SETUP_TIME(__TIME__) ((__TIME__) <= 15U)
#define IS_FMC_ADDRESS_HOLD_TIME(__TIME__) (((__TIME__) > 0U) && ((__TIME__) <= 15U))
#define IS_FMC_DATASETUP_TIME(__TIME__) (((__TIME__) > 0U) && ((__TIME__) <= 255U))
#define IS_FMC_DATAHOLD_DURATION(__DATAHOLD__) ((__DATAHOLD__) <= 3U)
#define IS_FMC_TURNAROUND_TIME(__TIME__) ((__TIME__) <= 15U)
#define IS_FMC_CLK_DIV(__DIV__) (((__DIV__) > 1U) && ((__DIV__) <= 16U))
#define IS_FMC_NORSRAM_DEVICE(__INSTANCE__) ((__INSTANCE__) == FMC_NORSRAM_DEVICE)
#define IS_FMC_NORSRAM_EXTENDED_DEVICE(__INSTANCE__) ((__INSTANCE__) == FMC_NORSRAM_EXTENDED_DEVICE)

#endif /* FMC_BANK1 */
#if defined(FMC_BANK3)

#define IS_FMC_NAND_BANK(__BANK__) ((__BANK__) == FMC_NAND_BANK3)
#define IS_FMC_WAIT_FEATURE(__FEATURE__) (((__FEATURE__) == FMC_NAND_PCC_WAIT_FEATURE_DISABLE) || \
                                          ((__FEATURE__) == FMC_NAND_PCC_WAIT_FEATURE_ENABLE))
#define IS_FMC_NAND_MEMORY_WIDTH(__WIDTH__) (((__WIDTH__) == FMC_NAND_PCC_MEM_BUS_WIDTH_8) || \
                                             ((__WIDTH__) == FMC_NAND_PCC_MEM_BUS_WIDTH_16))
#define IS_FMC_ECC_STATE(__STATE__) (((__STATE__) == FMC_NAND_ECC_DISABLE) || \
                                     ((__STATE__) == FMC_NAND_ECC_ENABLE))

#define IS_FMC_ECCPAGE_SIZE(__SIZE__) (((__SIZE__) == FMC_NAND_ECC_PAGE_SIZE_256BYTE)  || \
                                       ((__SIZE__) == FMC_NAND_ECC_PAGE_SIZE_512BYTE)  || \
                                       ((__SIZE__) == FMC_NAND_ECC_PAGE_SIZE_1024BYTE) || \
                                       ((__SIZE__) == FMC_NAND_ECC_PAGE_SIZE_2048BYTE) || \
                                       ((__SIZE__) == FMC_NAND_ECC_PAGE_SIZE_4096BYTE) || \
                                       ((__SIZE__) == FMC_NAND_ECC_PAGE_SIZE_8192BYTE))
#define IS_FMC_TCLR_TIME(__TIME__) ((__TIME__) <= 255U)
#define IS_FMC_TAR_TIME(__TIME__) ((__TIME__) <= 255U)
#define IS_FMC_SETUP_TIME(__TIME__) ((__TIME__) <= 254U)
#define IS_FMC_WAIT_TIME(__TIME__) ((__TIME__) <= 254U)
#define IS_FMC_HOLD_TIME(__TIME__) ((__TIME__) <= 254U)
#define IS_FMC_HIZ_TIME(__TIME__) ((__TIME__) <= 254U)
#define IS_FMC_NAND_DEVICE(__INSTANCE__) ((__INSTANCE__) == FMC_NAND_DEVICE)

#endif /* FMC_BANK3 */
#if defined(FMC_BANK4)
#define IS_FMC_PCCARD_DEVICE(__INSTANCE__) ((__INSTANCE__) == FMC_PCCARD_DEVICE)

#endif /* FMC_BANK4 */

/**
  * @}
  */

/* Exported typedef ----------------------------------------------------------*/

/** @defgroup FMC_LL_Exported_typedef FMC Low Layer Exported Types
  * @{
  */

#if defined FMC_BANK1
#define FMC_NORSRAM_TypeDef            FMC_Bank1_TypeDef
#define FMC_NORSRAM_EXTENDED_TypeDef   FMC_Bank1E_TypeDef
#endif /* FMC_BANK1 */
#if defined(FMC_BANK3)
#define FMC_NAND_TypeDef               FMC_Bank2_3_TypeDef
#endif /* FMC_BANK3 */
#if defined(FMC_BANK4)
#define FMC_PCCARD_TypeDef             FMC_Bank4_TypeDef
#endif /* FMC_BANK4 */

#if defined FMC_BANK1
#define FMC_NORSRAM_DEVICE             FMC_Bank1
#define FMC_NORSRAM_EXTENDED_DEVICE    FMC_Bank1E
#endif /* FMC_BANK1 */
#if defined(FMC_BANK3)
#define FMC_NAND_DEVICE                FMC_Bank2_3
#endif /* FMC_BANK3 */
#if defined(FMC_BANK4)
#define FMC_PCCARD_DEVICE              FMC_Bank4
#endif /* FMC_BANK4 */

#if defined FMC_BANK1
/**
  * @brief  FMC NORSRAM Configuration Structure definition
  */
typedef struct
{
  uint32_t NSBank;                       /*!< Specifies the NORSRAM memory device that will be used.
                                              This parameter can be a value of @ref FMC_NORSRAM_Bank                     */

  uint32_t DataAddressMux;               /*!< Specifies whether the address and data values are
                                              multiplexed on the data bus or not.
                                              This parameter can be a value of @ref FMC_Data_Address_Bus_Multiplexing    */

  uint32_t MemoryType;                   /*!< Specifies the type of external memory attached to
                                              the corresponding memory device.
                                              This parameter can be a value of @ref FMC_Memory_Type                      */

  uint32_t MemoryDataWidth;              /*!< Specifies the external memory device width.
                                              This parameter can be a value of @ref FMC_NORSRAM_Data_Width               */

  uint32_t BurstAccessMode;              /*!< Enables or disables the burst access mode for Flash memory,
                                              valid only with synchronous burst Flash memories.
                                              This parameter can be a value of @ref FMC_Burst_Access_Mode                */

  uint32_t WaitSignalPolarity;           /*!< Specifies the wait signal polarity, valid only when accessing
                                              the Flash memory in burst mode.
                                              This parameter can be a value of @ref FMC_Wait_Signal_Polarity             */

  uint32_t WrapMode;                     /*!< Enables or disables the Wrapped burst access mode for Flash
                                              memory, valid only when accessing Flash memories in burst mode.
                                              This parameter can be a value of @ref FMC_Wrap_Mode                        */

  uint32_t WaitSignalActive;             /*!< Specifies if the wait signal is asserted by the memory one
                                              clock cycle before the wait state or during the wait state,
                                              valid only when accessing memories in burst mode.
                                              This parameter can be a value of @ref FMC_Wait_Timing                      */

  uint32_t WriteOperation;               /*!< Enables or disables the write operation in the selected device by the FMC.
                                              This parameter can be a value of @ref FMC_Write_Operation                  */

  uint32_t WaitSignal;                   /*!< Enables or disables the wait state insertion via wait
                                              signal, valid for Flash memory access in burst mode.
                                              This parameter can be a value of @ref FMC_Wait_Signal                      */

  uint32_t ExtendedMode;                 /*!< Enables or disables the extended mode.
                                              This parameter can be a value of @ref FMC_Extended_Mode                    */

  uint32_t AsynchronousWait;             /*!< Enables or disables wait signal during asynchronous transfers,
                                              valid only with asynchronous Flash memories.
                                              This parameter can be a value of @ref FMC_AsynchronousWait                 */

  uint32_t WriteBurst;                   /*!< Enables or disables the write burst operation.
                                              This parameter can be a value of @ref FMC_Write_Burst                      */

  uint32_t ContinuousClock;              /*!< Enables or disables the FMC clock output to external memory devices.
                                              This parameter is only enabled through the FMC_BCR1 register, and don't care
                                              through FMC_BCR2..4 registers.
                                              This parameter can be a value of @ref FMC_Continous_Clock                  */
}FMC_NORSRAM_InitTypeDef;

/**
  * @brief  FMC NORSRAM Timing parameters structure definition
  */
typedef struct
{
  uint32_t AddressSetupTime;             /*!< Defines the number of HCLK cycles to configure
                                              the duration of the address setup time.
                                              This parameter can be a value between Min_Data = 0 and Max_Data = 15.
                                              @note This parameter is not used with synchronous NOR Flash memories.      */

  uint32_t AddressHoldTime;              /*!< Defines the number of HCLK cycles to configure
                                              the duration of the address hold time.
                                              This parameter can be a value between Min_Data = 1 and Max_Data = 15.
                                              @note This parameter is not used with synchronous NOR Flash memories.      */

  uint32_t DataSetupTime;                /*!< Defines the number of HCLK cycles to configure
                                              the duration of the data setup time.
                                              This parameter can be a value between Min_Data = 1 and Max_Data = 255.
                                              @note This parameter is used for SRAMs, ROMs and asynchronous multiplexed
                                              NOR Flash memories.                                                        */

  uint32_t BusTurnAroundDuration;        /*!< Defines the number of HCLK cycles to configure
                                              the duration of the bus turnaround.
                                              This parameter can be a value between Min_Data = 0 and Max_Data = 15.
                                              @note This parameter is only used for multiplexed NOR Flash memories.      */

  uint32_t CLKDivision;                  /*!< Defines the period of CLK clock output signal, expressed in number of
                                              HCLK cycles. This parameter can be a value between Min_Data = 2 and Max_Data = 16.
                                              @note This parameter is not used for asynchronous NOR Flash, SRAM or ROM
                                              accesses.                                                                  */

  uint32_t DataLatency;                  /*!< Defines the number of memory clock cycles to issue
                                              to the memory before getting the first data.
                                              The parameter value depends on the memory type as shown below:
                                              - It must be set to 0 in case of a CRAM
                                              - It is don't care in asynchronous NOR, SRAM or ROM accesses
                                              - It may assume a value between Min_Data = 2 and Max_Data = 17 in NOR Flash memories
                                                with synchronous burst mode enable                                       */

  uint32_t AccessMode;                   /*!< Specifies the asynchronous access mode.
                                              This parameter can be a value of @ref FMC_Access_Mode                      */
}FMC_NORSRAM_TimingTypeDef;
#endif /* FMC_BANK1 */

#if defined(FMC_BANK3)
/**
  * @brief  FMC NAND Configuration Structure definition
  */
typedef struct
{
  uint32_t NandBank;               /*!< Specifies the NAND memory device that will be used.
                                        This parameter can be a value of @ref FMC_NAND_Bank                    */

  uint32_t Waitfeature;            /*!< Enables or disables the Wait feature for the NAND Memory device.
                                        This parameter can be any value of @ref FMC_Wait_feature               */

  uint32_t MemoryDataWidth;        /*!< Specifies the external memory device width.
                                        This parameter can be any value of @ref FMC_NAND_Data_Width            */

  uint32_t EccComputation;         /*!< Enables or disables the ECC computation.
                                        This parameter can be any value of @ref FMC_ECC                        */

  uint32_t ECCPageSize;            /*!< Defines the page size for the extended ECC.
                                        This parameter can be any value of @ref FMC_ECC_Page_Size              */

  uint32_t TCLRSetupTime;          /*!< Defines the number of HCLK cycles to configure the
                                        delay between CLE low and RE low.
                                        This parameter can be a value between Min_Data = 0 and Max_Data = 255  */

  uint32_t TARSetupTime;           /*!< Defines the number of HCLK cycles to configure the
                                        delay between ALE low and RE low.
                                        This parameter can be a number between Min_Data = 0 and Max_Data = 255 */
}FMC_NAND_InitTypeDef;
#endif

#if defined(FMC_BANK3)||defined(FMC_BANK4)
/**
  * @brief  FMC NAND Timing parameters structure definition
  */
typedef struct
{
  uint32_t SetupTime;            /*!< Defines the number of HCLK cycles to setup address before
                                      the command assertion for NAND-Flash read or write access
                                      to common/Attribute or I/O memory space (depending on
                                      the memory space timing to be configured).
                                      This parameter can be a value between Min_Data = 0 and Max_Data = 254    */

  uint32_t WaitSetupTime;        /*!< Defines the minimum number of HCLK cycles to assert the
                                      command for NAND-Flash read or write access to
                                      common/Attribute or I/O memory space (depending on the
                                      memory space timing to be configured).
                                      This parameter can be a number between Min_Data = 0 and Max_Data = 254   */

  uint32_t HoldSetupTime;        /*!< Defines the number of HCLK clock cycles to hold address
                                      (and data for write access) after the command de-assertion
                                      for NAND-Flash read or write access to common/Attribute
                                      or I/O memory space (depending on the memory space timing
                                      to be configured).
                                      This parameter can be a number between Min_Data = 0 and Max_Data = 254   */

  uint32_t HiZSetupTime;         /*!< Defines the number of HCLK clock cycles during which the
                                      data bus is kept in HiZ after the start of a NAND-Flash
                                      write access to common/Attribute or I/O memory space (depending
                                      on the memory space timing to be configured).
                                      This parameter can be a number between Min_Data = 0 and Max_Data = 254   */
}FMC_NAND_PCC_TimingTypeDef;
#endif /* FMC_BANK3 */

#if defined(FMC_BANK4)
/** 
  * @brief FMC PCCARD Configuration Structure definition
  */ 
typedef struct
{
  uint32_t Waitfeature;            /*!< Enables or disables the Wait feature for the PCCARD Memory device.
                                        This parameter can be any value of @ref FMC_Wait_feature      */

  uint32_t TCLRSetupTime;          /*!< Defines the number of HCLK cycles to configure the
                                        delay between CLE low and RE low.
                                        This parameter can be a value between Min_Data = 0 and Max_Data = 255  */

  uint32_t TARSetupTime;           /*!< Defines the number of HCLK cycles to configure the
                                        delay between ALE low and RE low.
                                        This parameter can be a number between Min_Data = 0 and Max_Data = 255 */
}FMC_PCCARD_InitTypeDef;
#endif

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @addtogroup FMC_LL_Exported_Constants FMC Low Layer Exported Constants
  * @{
  */
#if defined FMC_BANK1

/** @defgroup FMC_LL_NOR_SRAM_Controller FMC NOR/SRAM Controller
  * @{
  */

/** @defgroup FMC_NORSRAM_Bank FMC NOR/SRAM Bank
  * @{
  */
#define FMC_NORSRAM_BANK1                       ((uint32_t)0x00000000U)
#define FMC_NORSRAM_BANK2                       ((uint32_t)0x00000002U)
#define FMC_NORSRAM_BANK3                       ((uint32_t)0x00000004U)
#define FMC_NORSRAM_BANK4                       ((uint32_t)0x00000006U)
/**
  * @}
  */

/** @defgroup FMC_Data_Address_Bus_Multiplexing FMC Data Address Bus Multiplexing
  * @{
  */
#define FMC_DATA_ADDRESS_MUX_DISABLE            ((uint32_t)0x00000000U)
#define FMC_DATA_ADDRESS_MUX_ENABLE             ((uint32_t)0x00000002U)
/**
  * @}
  */

/** @defgroup FMC_Memory_Type FMC Memory Type
  * @{
  */
#define FMC_MEMORY_TYPE_SRAM                    ((uint32_t)0x00000000U)
#define FMC_MEMORY_TYPE_PSRAM                   ((uint32_t)0x00000004U)
#define FMC_MEMORY_TYPE_NOR                     ((uint32_t)0x00000008U)
/**
  * @}
  */

/** @defgroup FMC_NORSRAM_Data_Width FMC NORSRAM Data Width
  * @{
  */
#define FMC_NORSRAM_MEM_BUS_WIDTH_8             ((uint32_t)0x00000000U)
#define FMC_NORSRAM_MEM_BUS_WIDTH_16            ((uint32_t)0x00000010U)
#define FMC_NORSRAM_MEM_BUS_WIDTH_32            ((uint32_t)0x00000020U)
/**
  * @}
  */

/** @defgroup FMC_NORSRAM_Flash_Access FMC NOR/SRAM Flash Access
  * @{
  */
#define FMC_NORSRAM_FLASH_ACCESS_ENABLE         ((uint32_t)0x00000040U)
#define FMC_NORSRAM_FLASH_ACCESS_DISABLE        ((uint32_t)0x00000000U)
/**
  * @}
  */

/** @defgroup FMC_Burst_Access_Mode FMC Burst Access Mode
  * @{
  */
#define FMC_BURST_ACCESS_MODE_DISABLE           ((uint32_t)0x00000000U)
#define FMC_BURST_ACCESS_MODE_ENABLE            ((uint32_t)0x00000100U)
/**
  * @}
  */

/** @defgroup FMC_Wait_Signal_Polarity FMC Wait Signal Polarity
  * @{
  */
#define FMC_WAIT_SIGNAL_POLARITY_LOW            ((uint32_t)0x00000000U)
#define FMC_WAIT_SIGNAL_POLARITY_HIGH           ((uint32_t)0x00000200U)
/**
  * @}
  */

/** @defgroup FMC_Wrap_Mode FMC Wrap Mode 
  * @{
  */
#define FMC_WRAP_MODE_DISABLE                   ((uint32_t)0x00000000U)
#define FMC_WRAP_MODE_ENABLE                    ((uint32_t)0x00000400U)
/**
  * @}
  */

/** @defgroup FMC_Wait_Timing FMC Wait Timing
  * @{
  */
#define FMC_WAIT_TIMING_BEFORE_WS               ((uint32_t)0x00000000U)
#define FMC_WAIT_TIMING_DURING_WS               ((uint32_t)0x00000800U)
/**
  * @}
  */

/** @defgroup FMC_Write_Operation FMC Write Operation
  * @{
  */
#define FMC_WRITE_OPERATION_DISABLE             ((uint32_t)0x00000000U)
#define FMC_WRITE_OPERATION_ENABLE              ((uint32_t)0x00001000U)
/**
  * @}
  */

/** @defgroup FMC_Wait_Signal FMC Wait Signal
  * @{
  */
#define FMC_WAIT_SIGNAL_DISABLE                 ((uint32_t)0x00000000U)
#define FMC_WAIT_SIGNAL_ENABLE                  ((uint32_t)0x00002000U)
/**
  * @}
  */

/** @defgroup FMC_Extended_Mode FMC Extended Mode
  * @{
  */
#define FMC_EXTENDED_MODE_DISABLE               ((uint32_t)0x00000000U)
#define FMC_EXTENDED_MODE_ENABLE                ((uint32_t)0x00004000U)
/**
  * @}
  */

/** @defgroup FMC_AsynchronousWait FMC Asynchronous Wait
  * @{
  */
#define FMC_ASYNCHRONOUS_WAIT_DISABLE           ((uint32_t)0x00000000U)
#define FMC_ASYNCHRONOUS_WAIT_ENABLE            ((uint32_t)0x00008000U)
/**
  * @}
  */

/** @defgroup FMC_Write_Burst FMC Write Burst
  * @{
  */
#define FMC_WRITE_BURST_DISABLE                 ((uint32_t)0x00000000U)
#define FMC_WRITE_BURST_ENABLE                  ((uint32_t)0x00080000U)
/**
  * @}
  */

/** @defgroup FMC_Continous_Clock FMC Continuous Clock
  * @{
  */
#define FMC_CONTINUOUS_CLOCK_SYNC_ONLY          ((uint32_t)0x00000000U)
#define FMC_CONTINUOUS_CLOCK_SYNC_ASYNC         ((uint32_t)0x00100000U)
/**
  * @}
  */

  /** @defgroup FMC_Access_Mode FMC Access Mode
  * @{
  */
#define FMC_ACCESS_MODE_A                       ((uint32_t)0x00000000U)
#define FMC_ACCESS_MODE_B                       ((uint32_t)0x10000000U)
#define FMC_ACCESS_MODE_C                       ((uint32_t)0x20000000U)
#define FMC_ACCESS_MODE_D                       ((uint32_t)0x30000000U)
/**
  * @}
  */

/**
  * @}
  */
#endif /* FMC_BANK1 */

#if defined(FMC_BANK3)||defined(FMC_BANK4)

/** @defgroup FMC_LL_NAND_Controller FMC NAND Controller
  * @{
  */
/** @defgroup FMC_NAND_Bank FMC NAND Bank
  * @{
  */
#define FMC_NAND_BANK2                          ((uint32_t)0x00000010U)
#define FMC_NAND_BANK3                          ((uint32_t)0x00000100U)
/**
  * @}
  */

/** @defgroup FMC_Wait_feature FMC Wait feature
  * @{
  */
#define FMC_NAND_PCC_WAIT_FEATURE_DISABLE       ((uint32_t)0x00000000U)
#define FMC_NAND_PCC_WAIT_FEATURE_ENABLE        ((uint32_t)0x00000002U)
/**
  * @}
  */

/** @defgroup FMC_PCR_Memory_Type FMC PCR Memory Type
  * @{
  */
#if defined(FMC_BANK4)
#define FMC_PCR_MEMORY_TYPE_PCCARD              ((uint32_t)0x00000000U)
#endif
#define FMC_PCR_MEMORY_TYPE_NAND                ((uint32_t)0x00000008U)
/**
  * @}
  */

/** @defgroup FMC_NAND_Data_Width FMC NAND Data Width
  * @{
  */
#define FMC_NAND_PCC_MEM_BUS_WIDTH_8                ((uint32_t)0x00000000U)
#define FMC_NAND_PCC_MEM_BUS_WIDTH_16               ((uint32_t)0x00000010U)
/**
  * @}
  */

/** @defgroup FMC_ECC FMC ECC
  * @{
  */
#define FMC_NAND_ECC_DISABLE                    ((uint32_t)0x00000000U)
#define FMC_NAND_ECC_ENABLE                     ((uint32_t)0x00000040U)
/**
  * @}
  */

/** @defgroup FMC_ECC_Page_Size FMC ECC Page Size
  * @{
  */
#define FMC_NAND_ECC_PAGE_SIZE_256BYTE          ((uint32_t)0x00000000U)
#define FMC_NAND_ECC_PAGE_SIZE_512BYTE          ((uint32_t)0x00020000U)
#define FMC_NAND_ECC_PAGE_SIZE_1024BYTE         ((uint32_t)0x00040000U)
#define FMC_NAND_ECC_PAGE_SIZE_2048BYTE         ((uint32_t)0x00060000U)
#define FMC_NAND_ECC_PAGE_SIZE_4096BYTE         ((uint32_t)0x00080000U)
#define FMC_NAND_ECC_PAGE_SIZE_8192BYTE         ((uint32_t)0x000A0000U)
/**
  * @}
  */

/**
  * @}
  */
#endif /* FMC_BANK3 */


/** @defgroup FMC_LL_Interrupt_definition FMC Low Layer Interrupt definition
  * @{
  */
#if defined(FMC_BANK3)||defined(FMC_BANK4)
#define FMC_IT_RISING_EDGE                      ((uint32_t)0x00000008U)
#define FMC_IT_LEVEL                            ((uint32_t)0x00000010U)
#define FMC_IT_FALLING_EDGE                     ((uint32_t)0x00000020U)
#endif /* FMC_BANK3 */
/**
  * @}
  */

/** @defgroup FMC_LL_Flag_definition FMC Low Layer Flag definition
  * @{
  */
#if defined(FMC_BANK3)||defined(FMC_BANK4)
#define FMC_FLAG_RISING_EDGE                    ((uint32_t)0x00000001U)
#define FMC_FLAG_LEVEL                          ((uint32_t)0x00000002U)
#define FMC_FLAG_FALLING_EDGE                   ((uint32_t)0x00000004U)
#define FMC_FLAG_FEMPT                          ((uint32_t)0x00000040U)
#endif /* FMC_BANK3 */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
  
/* Private macro -------------------------------------------------------------*/
/** @defgroup FMC_LL_Private_Macros FMC_LL  Private Macros
  * @{
  */
#if defined FMC_BANK1
/** @defgroup FMC_LL_NOR_Macros FMC NOR/SRAM Macros
  * @brief macros to handle NOR device enable/disable and read/write operations
  * @{
  */

/**
  * @brief  Enable the NORSRAM device access.
  * @param  __INSTANCE__ FMC_NORSRAM Instance
  * @param  __BANK__ FMC_NORSRAM Bank
  * @retval None
  */
#define __FMC_NORSRAM_ENABLE(__INSTANCE__, __BANK__)  ((__INSTANCE__)->BTCR[(__BANK__)] |= FMC_BCRx_MBKEN)

/**
  * @brief  Disable the NORSRAM device access.
  * @param  __INSTANCE__ FMC_NORSRAM Instance
  * @param  __BANK__ FMC_NORSRAM Bank
  * @retval None
  */
#define __FMC_NORSRAM_DISABLE(__INSTANCE__, __BANK__) ((__INSTANCE__)->BTCR[(__BANK__)] &= ~FMC_BCRx_MBKEN)

/**
  * @}
  */
#endif /* FMC_BANK1 */

#if defined(FMC_BANK3)
/** @defgroup FMC_LL_NAND_Macros FMC NAND Macros
 *  @brief macros to handle NAND device enable/disable
 *  @{
 */

/**
  * @brief  Enable the NAND device access.
  * @param  __INSTANCE__ FMC_NAND Instance
  * @param  __BANK__     FMC_NAND Bank
  * @retval None
  */
#define __FMC_NAND_ENABLE(__INSTANCE__, __BANK__)  (((__BANK__) == FMC_NAND_BANK2)? ((__INSTANCE__)->PCR2 |= FMC_PCRx_PBKEN): \
                                                             ((__INSTANCE__)->PCR3 |= FMC_PCRx_PBKEN))

/**
  * @brief  Disable the NAND device access.
  * @param  __INSTANCE__ FMC_NAND Instance
  * @param  __BANK__     FMC_NAND Bank
  * @retval None
  */
#define __FMC_NAND_DISABLE(__INSTANCE__, __BANK__) (((__BANK__) == FMC_NAND_BANK2)? CLEAR_BIT((__INSTANCE__)->PCR2, FMC_PCRx_PBKEN): \
                                                             CLEAR_BIT((__INSTANCE__)->PCR3, FMC_PCRx_PBKEN))

/**
  * @}
  */
#endif

#if defined(FMC_BANK4)
/** @defgroup FMC_LL_PCCARD_Macros FMC PCCARD Macros
 *  @brief macros to handle PCCARD read/write operations 
 *  @{
 */
/**
  * @brief  Enable the PCCARD device access.
  * @param  __INSTANCE__ FMC_PCCARD Instance  
  * @retval None
  */ 
#define __FMC_PCCARD_ENABLE(__INSTANCE__)  ((__INSTANCE__)->PCR4 |= FMC_PCR4_PBKEN)

/**
  * @brief  Disable the PCCARD device access.
  * @param  __INSTANCE__ FMC_PCCARD Instance     
  * @retval None
  */ 
#define __FMC_PCCARD_DISABLE(__INSTANCE__) ((__INSTANCE__)->PCR4 &= ~FMC_PCR4_PBKEN)
/**
  * @}
  */

#endif
#if defined(FMC_BANK3)
/** @defgroup FMC_LL_NAND_Interrupt FMC NAND Interrupt
  * @brief macros to handle NAND interrupts
  * @{
  */

/**
  * @brief  Enable the NAND device interrupt.
  * @param  __INSTANCE__  FMC_NAND instance
  * @param  __BANK__     FMC_NAND Bank
  * @param  __INTERRUPT__ FMC_NAND interrupt
  *         This parameter can be any combination of the following values:
  *            @arg FMC_IT_RISING_EDGE: Interrupt rising edge.
  *            @arg FMC_IT_LEVEL: Interrupt level.
  *            @arg FMC_IT_FALLING_EDGE: Interrupt falling edge.
  * @retval None
  */
#define __FMC_NAND_ENABLE_IT(__INSTANCE__, __BANK__, __INTERRUPT__)  (((__BANK__) == FMC_NAND_BANK2)? ((__INSTANCE__)->SR2 |= (__INTERRUPT__)): \
                                                                               ((__INSTANCE__)->SR3 |= (__INTERRUPT__)))

/**
  * @brief  Disable the NAND device interrupt.
  * @param  __INSTANCE__  FMC_NAND Instance
  * @param  __BANK__     FMC_NAND Bank
  * @param  __INTERRUPT__ FMC_NAND interrupt
  *         This parameter can be any combination of the following values:
  *            @arg FMC_IT_RISING_EDGE: Interrupt rising edge.
  *            @arg FMC_IT_LEVEL: Interrupt level.
  *            @arg FMC_IT_FALLING_EDGE: Interrupt falling edge.
  * @retval None
  */
#define __FMC_NAND_DISABLE_IT(__INSTANCE__, __BANK__, __INTERRUPT__)  (((__BANK__) == FMC_NAND_BANK2)? ((__INSTANCE__)->SR2 &= ~(__INTERRUPT__)): \
                                                                                ((__INSTANCE__)->SR3 &= ~(__INTERRUPT__))) 

/**
  * @brief  Get flag status of the NAND device.
  * @param  __INSTANCE__ FMC_NAND Instance
  * @param  __BANK__     FMC_NAND Bank
  * @param  __FLAG__     FMC_NAND flag
  *         This parameter can be any combination of the following values:
  *            @arg FMC_FLAG_RISING_EDGE: Interrupt rising edge flag.
  *            @arg FMC_FLAG_LEVEL: Interrupt level edge flag.
  *            @arg FMC_FLAG_FALLING_EDGE: Interrupt falling edge flag.
  *            @arg FMC_FLAG_FEMPT: FIFO empty flag.
  * @retval The state of FLAG (SET or RESET).
  */
#define __FMC_NAND_GET_FLAG(__INSTANCE__, __BANK__, __FLAG__)  (((__BANK__) == FMC_NAND_BANK2)? (((__INSTANCE__)->SR2 &(__FLAG__)) == (__FLAG__)): \
                                                                         (((__INSTANCE__)->SR3 &(__FLAG__)) == (__FLAG__)))

/**
  * @brief  Clear flag status of the NAND device.
  * @param  __INSTANCE__ FMC_NAND Instance
  * @param  __BANK__     FMC_NAND Bank
  * @param  __FLAG__     FMC_NAND flag
  *         This parameter can be any combination of the following values:
  *            @arg FMC_FLAG_RISING_EDGE: Interrupt rising edge flag.
  *            @arg FMC_FLAG_LEVEL: Interrupt level edge flag.
  *            @arg FMC_FLAG_FALLING_EDGE: Interrupt falling edge flag.
  *            @arg FMC_FLAG_FEMPT: FIFO empty flag.
  * @retval None
  */
#define __FMC_NAND_CLEAR_FLAG(__INSTANCE__, __BANK__, __FLAG__)  (((__BANK__) == FMC_NAND_BANK2)? ((__INSTANCE__)->SR2 &= ~(__FLAG__)): \
                                                                           ((__INSTANCE__)->SR3 &= ~(__FLAG__)))

/**
  * @}
  */
#endif /* FMC_BANK3 */

#if defined(FMC_BANK4)
/** @defgroup FMC_LL_PCCARD_Interrupt FMC PCCARD Interrupt
  * @brief macros to handle PCCARD interrupts
  * @{
  */

/**
  * @brief  Enable the PCCARD device interrupt.
  * @param  __INSTANCE__ FMC_PCCARD instance  
  * @param  __INTERRUPT__ FMC_PCCARD interrupt 
  *         This parameter can be any combination of the following values:
  *            @arg FMC_IT_RISING_EDGE: Interrupt rising edge.
  *            @arg FMC_IT_LEVEL: Interrupt level.
  *            @arg FMC_IT_FALLING_EDGE: Interrupt falling edge.       
  * @retval None
  */ 
#define __FMC_PCCARD_ENABLE_IT(__INSTANCE__, __INTERRUPT__)  ((__INSTANCE__)->SR4 |= (__INTERRUPT__))

/**
  * @brief  Disable the PCCARD device interrupt.
  * @param  __INSTANCE__ FMC_PCCARD instance  
  * @param  __INTERRUPT__ FMC_PCCARD interrupt 
  *         This parameter can be any combination of the following values:
  *            @arg FMC_IT_RISING_EDGE: Interrupt rising edge.
  *            @arg FMC_IT_LEVEL: Interrupt level.
  *            @arg FMC_IT_FALLING_EDGE: Interrupt falling edge.       
  * @retval None
  */ 
#define __FMC_PCCARD_DISABLE_IT(__INSTANCE__, __INTERRUPT__)  ((__INSTANCE__)->SR4 &= ~(__INTERRUPT__)) 

/**
  * @brief  Get flag status of the PCCARD device.
  * @param  __INSTANCE__ FMC_PCCARD instance  
  * @param  __FLAG__ FMC_PCCARD flag
  *         This parameter can be any combination of the following values:
  *            @arg  FMC_FLAG_RISING_EDGE: Interrupt rising edge flag.
  *            @arg  FMC_FLAG_LEVEL: Interrupt level edge flag.
  *            @arg  FMC_FLAG_FALLING_EDGE: Interrupt falling edge flag.
  *            @arg  FMC_FLAG_FEMPT: FIFO empty flag.   
  * @retval The state of FLAG (SET or RESET).
  */
#define __FMC_PCCARD_GET_FLAG(__INSTANCE__, __FLAG__)  (((__INSTANCE__)->SR4 &(__FLAG__)) == (__FLAG__))

/**
  * @brief  Clear flag status of the PCCARD device.
  * @param  __INSTANCE__ FMC_PCCARD instance  
  * @param  __FLAG__ FMC_PCCARD flag
  *         This parameter can be any combination of the following values:
  *            @arg  FMC_FLAG_RISING_EDGE: Interrupt rising edge flag.
  *            @arg  FMC_FLAG_LEVEL: Interrupt level edge flag.
  *            @arg  FMC_FLAG_FALLING_EDGE: Interrupt falling edge flag.
  *            @arg  FMC_FLAG_FEMPT: FIFO empty flag.   
  * @retval None
  */
#define __FMC_PCCARD_CLEAR_FLAG(__INSTANCE__, __FLAG__)  ((__INSTANCE__)->SR4 &= ~(__FLAG__))

/**
  * @}
  */
#endif

/**
  * @}
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup FMC_LL_Private_Functions FMC LL Private Functions
  *  @{
  */

#if defined FMC_BANK1
/** @defgroup FMC_LL_NORSRAM  NOR SRAM
  *  @{
  */
/** @defgroup FMC_LL_NORSRAM_Private_Functions_Group1 NOR SRAM Initialization/de-initialization functions
  *  @{
  */
HAL_StatusTypeDef  FMC_NORSRAM_Init(FMC_NORSRAM_TypeDef *Device, FMC_NORSRAM_InitTypeDef *Init);
HAL_StatusTypeDef  FMC_NORSRAM_Timing_Init(FMC_NORSRAM_TypeDef *Device, FMC_NORSRAM_TimingTypeDef *Timing, uint32_t Bank);
HAL_StatusTypeDef  FMC_NORSRAM_Extended_Timing_Init(FMC_NORSRAM_EXTENDED_TypeDef *Device, FMC_NORSRAM_TimingTypeDef *Timing, uint32_t Bank, uint32_t ExtendedMode);
HAL_StatusTypeDef  FMC_NORSRAM_DeInit(FMC_NORSRAM_TypeDef *Device, FMC_NORSRAM_EXTENDED_TypeDef *ExDevice, uint32_t Bank);
/**
  * @}
  */

/** @defgroup FMC_LL_NORSRAM_Private_Functions_Group2 NOR SRAM Control functions
  *  @{
  */
HAL_StatusTypeDef  FMC_NORSRAM_WriteOperation_Enable(FMC_NORSRAM_TypeDef *Device, uint32_t Bank);
HAL_StatusTypeDef  FMC_NORSRAM_WriteOperation_Disable(FMC_NORSRAM_TypeDef *Device, uint32_t Bank);
/**
  * @}
  */
/**
  * @}
  */
#endif /* FMC_BANK1 */

#if defined(FMC_BANK3)
/** @defgroup FMC_LL_NAND NAND
  *  @{
  */
/** @defgroup FMC_LL_NAND_Private_Functions_Group1 NAND Initialization/de-initialization functions
  *  @{
  */
HAL_StatusTypeDef  FMC_NAND_Init(FMC_NAND_TypeDef *Device, FMC_NAND_InitTypeDef *Init);
HAL_StatusTypeDef  FMC_NAND_CommonSpace_Timing_Init(FMC_NAND_TypeDef *Device, FMC_NAND_PCC_TimingTypeDef *Timing, uint32_t Bank);
HAL_StatusTypeDef  FMC_NAND_AttributeSpace_Timing_Init(FMC_NAND_TypeDef *Device, FMC_NAND_PCC_TimingTypeDef *Timing, uint32_t Bank);
HAL_StatusTypeDef  FMC_NAND_DeInit(FMC_NAND_TypeDef *Device, uint32_t Bank);
/**
  * @}
  */

/** @defgroup FMC_LL_NAND_Private_Functions_Group2 NAND Control functions
  *  @{
  */
HAL_StatusTypeDef  FMC_NAND_ECC_Enable(FMC_NAND_TypeDef *Device, uint32_t Bank);
HAL_StatusTypeDef  FMC_NAND_ECC_Disable(FMC_NAND_TypeDef *Device, uint32_t Bank);
HAL_StatusTypeDef  FMC_NAND_GetECC(FMC_NAND_TypeDef *Device, uint32_t *ECCval, uint32_t Bank, uint32_t Timeout);
/**
  * @}
  */
/**
  * @}
  */
#endif /* FMC_BANK3 */

#if defined(FMC_BANK4)
/** @defgroup FMC_LL_PCCARD PCCARD
  *  @{
  */
/** @defgroup FMC_LL_PCCARD_Private_Functions_Group1 PCCARD Initialization/de-initialization functions
  *  @{
  */
HAL_StatusTypeDef  FMC_PCCARD_Init(FMC_PCCARD_TypeDef *Device, FMC_PCCARD_InitTypeDef *Init);
HAL_StatusTypeDef  FMC_PCCARD_CommonSpace_Timing_Init(FMC_PCCARD_TypeDef *Device, FMC_NAND_PCC_TimingTypeDef *Timing);
HAL_StatusTypeDef  FMC_PCCARD_AttributeSpace_Timing_Init(FMC_PCCARD_TypeDef *Device, FMC_NAND_PCC_TimingTypeDef *Timing);
HAL_StatusTypeDef  FMC_PCCARD_IOSpace_Timing_Init(FMC_PCCARD_TypeDef *Device, FMC_NAND_PCC_TimingTypeDef *Timing);
HAL_StatusTypeDef  FMC_PCCARD_DeInit(FMC_PCCARD_TypeDef *Device);
/**
  * @}
  */
/**
  * @}
  */
#endif /* FMC_BANK4 */


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

#endif /* STM32F3xx_LL_FMC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
