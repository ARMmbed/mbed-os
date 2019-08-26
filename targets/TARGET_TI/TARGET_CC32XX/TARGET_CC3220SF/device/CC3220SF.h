/**************************************************************************//**
 * @file     CC3220SF.h
 * @brief    CMSIS Cortex-M4 Core Peripheral Access Layer Header File for
 *           Device CC3220SF
 * @version  V5.00
 * @date     10. January 2018
 ******************************************************************************/
/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CC3220SF_H
#define CC3220SF_H

#ifdef __cplusplus
extern "C" {
#endif

/* =========================================================================================================================== */
/* ================                                Interrupt Number Definition                                ================ */
/* =========================================================================================================================== */

// mbed-os core_cm4.h adds offset of 16. Deduct 16 from the IRQ enums
#define INT_IRQn_OFFSET 16

typedef enum IRQn
{
/* =======================================  ARM Cortex-M4 Specific Interrupt Numbers  ======================================== */

  Reset_IRQn                = -15,              /*!< -15  Reset Vector, invoked on Power up and warm reset                     */
  NonMaskableInt_IRQn       = -14,              /*!< -14  Non maskable Interrupt, cannot be stopped or preempted               */
  HardFault_IRQn            = -13,              /*!< -13  Hard Fault, all classes of Fault                                     */
  MemoryManagement_IRQn     = -12,              /*!< -12  Memory Management, MPU mismatch, including Access Violation
                                                          and No Match                                                         */
  BusFault_IRQn             = -11,              /*!< -11  Bus Fault, Pre-Fetch-, Memory Access Fault, other address/memory
                                                          related Fault                                                        */
  UsageFault_IRQn           = -10,              /*!< -10  Usage Fault, i.e. Undef Instruction, Illegal State Transition        */
  SVCall_IRQn               =  -5,              /*!< -5 System Service Call via SVC instruction                                */
  DebugMonitor_IRQn         =  -4,              /*!< -4 Debug Monitor                                                          */
  PendSV_IRQn               =  -2,              /*!< -2 Pendable request for system service                                    */
  SysTick_IRQn              =  -1,              /*!< -1 System Tick Timer                                                      */

/* ===========================================  CC3220SF Specific Interrupt Numbers  ========================================= */
  INT_GPIOA0_IRQn           = 16 - INT_IRQn_OFFSET,          // GPIO Port S0
  INT_GPIOA1_IRQn           = 17 - INT_IRQn_OFFSET,          // GPIO Port S1
  INT_GPIOA2_IRQn           = 18 - INT_IRQn_OFFSET,          // GPIO Port S2
  INT_GPIOA3_IRQn           = 19 - INT_IRQn_OFFSET,          // GPIO Port S3
  INT_UARTA0_IRQn           = 21 - INT_IRQn_OFFSET,          // UART0 Rx and Tx
  INT_UARTA1_IRQn           = 22 - INT_IRQn_OFFSET,          // UART1 Rx and Tx
  INT_I2CA0_IRQn            = 24 - INT_IRQn_OFFSET,          // I2C controller
  INT_ADCCH0_IRQn           = 30 - INT_IRQn_OFFSET,          // ADC Sequence 0
  INT_ADCCH1_IRQn           = 31 - INT_IRQn_OFFSET,          // ADC Sequence 1
  INT_ADCCH2_IRQn           = 32 - INT_IRQn_OFFSET,          // ADC Sequence 2
  INT_ADCCH3_IRQn           = 33 - INT_IRQn_OFFSET,          // ADC Sequence 3
  INT_WDT_IRQn              = 34 - INT_IRQn_OFFSET,          // Watchdog Timer0
  INT_TIMERA0A_IRQn         = 35 - INT_IRQn_OFFSET,          // Timer 0 subtimer A
  INT_TIMERA0B_IRQn         = 36 - INT_IRQn_OFFSET,          // Timer 0 subtimer B
  INT_TIMERA1A_IRQn         = 37 - INT_IRQn_OFFSET,          // Timer 1 subtimer A
  INT_TIMERA1B_IRQn         = 38 - INT_IRQn_OFFSET,          // Timer 1 subtimer B
  INT_TIMERA2A_IRQn         = 39 - INT_IRQn_OFFSET,          // Timer 2 subtimer A
  INT_TIMERA2B_IRQn         = 40 - INT_IRQn_OFFSET,          // Timer 2 subtimer B
  INT_FLASH_IRQn            = 45 - INT_IRQn_OFFSET,          // FLASH Control
  INT_TIMERA3A_IRQn         = 51 - INT_IRQn_OFFSET,          // Timer 3 subtimer A
  INT_TIMERA3B_IRQn         = 52 - INT_IRQn_OFFSET,          // Timer 3 subtimer B
  INT_UDMA_IRQn             = 62 - INT_IRQn_OFFSET,          // uDMA controller
  INT_UDMAERR_IRQn          = 63 - INT_IRQn_OFFSET,          // uDMA Error
  INT_SHA_IRQn              = 164 - INT_IRQn_OFFSET,         // SHA
  INT_AES_IRQn              = 167 - INT_IRQn_OFFSET,         // AES
  INT_DES_IRQn              = 169 - INT_IRQn_OFFSET,         // DES
  INT_MMCHS_IRQn            = 175 - INT_IRQn_OFFSET,         // SDIO
  INT_I2S_IRQn              = 177 - INT_IRQn_OFFSET,         // McAPS
  INT_CAMERA_IRQn           = 179 - INT_IRQn_OFFSET,         // Camera
  INT_NWPIC_IRQn            = 187 - INT_IRQn_OFFSET,         // Interprocessor communication
  INT_PRCM_IRQn             = 188 - INT_IRQn_OFFSET,         // Power, Reset and Clock Module
  INT_SSPI_IRQn             = 191 - INT_IRQn_OFFSET,         // Shared SPI
  INT_GSPI_IRQn             = 192 - INT_IRQn_OFFSET,         // Generic SPI
  INT_LSPI_IRQn             = 193 - INT_IRQn_OFFSET          // Link SPI
} IRQn_Type;



/* =========================================================================================================================== */
/* ================                           Processor and Core Peripheral Section                           ================ */
/* =========================================================================================================================== */

/* ===========================  Configuration of the Arm Cortex-M4 Processor and Core Peripherals  =========================== */
#define __CM4_REV                 0x0201    /*!< Core Revision r2p1 */
/* ToDo: define the correct core features for the CC3220SF */
#define __MPU_PRESENT             1         /*!< Set to 1 if MPU is present */
#define __VTOR_PRESENT            1         /*!< Set to 1 if VTOR is present */
#define __NVIC_PRIO_BITS          3         /*!< Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT             0         /*!< Set to 1 if FPU is present */
#define __FPU_DP                  0         /*!< Set to 1 if FPU is double precision FPU (default is single precision FPU) */
#define __ICACHE_PRESENT          0         /*!< Set to 1 if I-Cache is present */
#define __DCACHE_PRESENT          0         /*!< Set to 1 if D-Cache is present */
#define __DTCM_PRESENT            0         /*!< Set to 1 if DTCM is present */


/** @} */ /* End of group Configuration_of_CMSIS */

#include <core_cm4.h>                           /*!< Arm Cortex-M4 processor and core peripherals */
#include "system_CC3220SF.h"                    /*!< CC3220SF System */


/* ========================================  Start of section using anonymous unions  ======================================== */
#if   defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__ICCARM__)
  #pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning 586
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif


/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */

/* =========================================================================================================================== */
/* ================                                            GPIO                                           ================ */
/* =========================================================================================================================== */

/**
  * @brief GPIO
  */
typedef struct
{
  __IO uint32_t DATA;             /*!< Address offset : 0x00000000 */
  uint32_t RESERVED0[255];
  __IO uint32_t DIR;              /*!< Address offset : 0x00000400 */
  __IO uint32_t IS;               /*!< Address offset : 0x00000404 */
  __IO uint32_t IBE;              /*!< Address offset : 0x00000408 */
  __IO uint32_t IEV;              /*!< Address offset : 0x0000040C */
  __IO uint32_t IM;               /*!< Address offset : 0x00000410 */
  __IO uint32_t RIS;              /*!< Address offset : 0x00000414 */
  __IO uint32_t MIS;              /*!< Address offset : 0x00000418 */
  __O  uint32_t ICR;              /*!< Address offset : 0x0000041C */
  __IO uint32_t AFSEL;            /*!< Address offset : 0x00000420 */
  uint32_t RESERVED1[55];
  __IO uint32_t DR2R;             /*!< Address offset : 0x00000500 */
  __IO uint32_t DR4R;             /*!< Address offset : 0x00000504 */
  __IO uint32_t DR8R;             /*!< Address offset : 0x00000508 */
  __IO uint32_t ODR;              /*!< Address offset : 0x0000050C */
  __IO uint32_t PUR;              /*!< Address offset : 0x00000510 */
  __IO uint32_t PDR;              /*!< Address offset : 0x00000514 */
  __IO uint32_t SLR;              /*!< Address offset : 0x00000518 */
  __IO uint32_t DEN;              /*!< Address offset : 0x0000051C */
  __IO uint32_t LOCK;             /*!< Address offset : 0x00000520 */
  __IO uint32_t CR;               /*!< Address offset : 0x00000524 */
  __IO uint32_t AMSEL;            /*!< Address offset : 0x00000528 */
  __IO uint32_t PCTL;             /*!< Address offset : 0x0000052C ! This register is not used in cc3xx. ! */
  __IO uint32_t ADCCTL;           /*!< Address offset : 0x00000530 ! This register is not used in cc3xx. ! */
  __IO uint32_t DMACTL;           /*!< Address offset : 0x00000534 */
  __IO uint32_t SI;               /*!< Address offset : 0x00000538 */
  uint32_t RESERVED2[677];
  __IO uint32_t PERIPHID4;        /*!< Address offset : 0x00000FD0 */
  __IO uint32_t PERIPHID5;        /*!< Address offset : 0x00000FD4 */
  __IO uint32_t PERIPHID6;        /*!< Address offset : 0x00000FD8 */
  __IO uint32_t PERIPHID7;        /*!< Address offset : 0x00000FDC */
  __IO uint32_t PERIPHID0;        /*!< Address offset : 0x00000FE0 */
  __IO uint32_t PERIPHID1;        /*!< Address offset : 0x00000FE4 */
  __IO uint32_t PERIPHID2;        /*!< Address offset : 0x00000FE8 */
  __IO uint32_t PERIPHID3;        /*!< Address offset : 0x00000FEC */
  __IO uint32_t PCELLID0;         /*!< Address offset : 0x00000FF0 */
  __IO uint32_t PCELLID1;         /*!< Address offset : 0x00000FF4 */
  __IO uint32_t PCELLID2;         /*!< Address offset : 0x00000FF8 */
  __IO uint32_t PCELLID3;         /*!< Address offset : 0x00000FFC */
} CC3220SF_GPIO_TypeDef;

/* =========================================================================================================================== */
/* ================                                            UART                                           ================ */
/* =========================================================================================================================== */

/**
  * @brief UART
  */
typedef struct
{
  __IO uint32_t DR;             /*!< Data,                              Address offset : 0x00 */
  union {
  __I  uint32_t RSR;            /*!< Receive Status,                    Address offset : 0x04 */
  __O  uint32_t ECR;            /*!< Error Clear,                       Address offset : 0x04 */
  };
  uint32_t RESERVED0[4];
  __IO uint32_t FR;             /*!< Flags,                             Address offset : 0x18 */
  uint32_t RESERVED1[1];
  __IO uint32_t ILPR;           /*!< IrDA Low-power Counter,            Address offset : 0x20 */
  __IO uint32_t IBRD;           /*!< Integer Baud Rate,                 Address offset : 0x24 */
  __IO uint32_t FBRD;           /*!< Fractional Baud Rate,              Address offset : 0x28 */
  __IO uint32_t LCRH;           /*!< Line Control,                      Address offset : 0x2C */
  __IO uint32_t CTL;            /*!< Control,                           Address offset : 0x30 */
  __IO uint32_t IFLS;           /*!< Interrupt FIFO Level Select,       Address offset : 0x34 */
  __IO uint32_t IM;             /*!< Interrupt Mask Set / Clear,        Address offset : 0x38 */
  __IO uint32_t RIS;            /*!< Raw Interrupt Status ,             Address offset : 0x3C */
  __IO uint32_t MIS;            /*!< Masked Interrupt Status ,          Address offset : 0x40 */
  __O  uint32_t ICR;            /*!< Interrupt Clear,                   Address offset : 0x44 */
  __IO uint32_t DMACTL;         /*!< DMA Control,                       Address offset : 0x48 */
  __IO uint32_t LCTL;           /*!<                                    Address offset : 0x90 */
  __IO uint32_t LSS;            /*!<                                    Address offset : 0x94 */
  __IO uint32_t LTIM;           /*!<                                    Address offset : 0x98 */
  __IO uint32_t BITADDR;        /*!< 9BITADDR                           Address offset : 0xA4 */
  __IO uint32_t BITMASK;        /*!< 9BITMASK                           Address offset : 0xA8 */
  __IO uint32_t PP;             /*!<                                    Address offset : 0xFC0 */
  __IO uint32_t CC;             /*!<                                    Address offset : 0xFC8 */
} CC3220SF_UART_TypeDef;

/* =========================================================================================================================== */
/* ================                          Device Specific Peripheral Address Map                           ================ */
/* =========================================================================================================================== */


/** @addtogroup Peripheral_memory_map
  * @{
  */

/* Peripheral and SRAM base address */
#define CC3220SF_FLASH_BASE              0x01000000
#define CC3220SF_SRAM_BASE               0x20000000
#define CC3220SF_WDT_BASE                0x40000000
#define CC3220SF_GPIOA0_BASE             0x40004000
#define CC3220SF_GPIOA1_BASE             0x40005000
#define CC3220SF_GPIOA2_BASE             0x40006000
#define CC3220SF_GPIOA3_BASE             0x40007000
#define CC3220SF_GPIOA4_BASE             0x40024000
#define CC3220SF_UARTA0_BASE             0x4000C000
#define CC3220SF_UARTA1_BASE             0x4000D000
#define CC3220SF_I2CA0_BASE              0x40020000
#define CC3220SF_TIMERA0_BASE            0x40030000
#define CC3220SF_TIMERA1_BASE            0x40031000
#define CC3220SF_TIMERA2_BASE            0x40032000
#define CC3220SF_TIMERA3_BASE            0x40033000
#define CC3220SF_STACKDIE_CTRL_BASE      0x400F5000
#define CC3220SF_COMMON_REG_BASE         0x400F7000
#define CC3220SF_FLASH_CONTROL_BASE      0x400FD000
#define CC3220SF_SYSTEM_CONTROL_BASE     0x400FE000
#define CC3220SF_UDMA_BASE               0x400FF000
#define CC3220SF_SDHOST_BASE             0x44010000
#define CC3220SF_CAMERA_BASE             0x44018000
#define CC3220SF_I2S_BASE                0x4401C000
#define CC3220SF_SSPI_BASE               0x44020000
#define CC3220SF_GSPI_BASE               0x44021000
#define CC3220SF_LSPI_BASE               0x44022000
#define CC3220SF_ARCM_BASE               0x44025000
#define CC3220SF_APPS_CONFIG_BASE        0x44026000
#define CC3220SF_GPRCM_BASE              0x4402D000
#define CC3220SF_OCP_SHARED_BASE         0x4402E000
#define CC3220SF_ADC_BASE                0x4402E800
#define CC3220SF_HIB1P2_BASE             0x4402F000
#define CC3220SF_HIB3P3_BASE             0x4402F800
#define CC3220SF_DTHE_BASE               0x44030000
#define CC3220SF_SHAMD5_BASE             0x44035000
#define CC3220SF_AES_BASE                0x44037000
#define CC3220SF_DES_BASE                0x44039000

/**
  * @}
  */

/* =========================================================================================================================== */
/* ================                                  Peripheral declaration                                   ================ */
/* =========================================================================================================================== */

/** @addtogroup Peripheral_declaration
  * @{
  */

/******************************************************************************/
/*                                                                            */
/*                                  GPIO                                      */
/*                                                                            */
/******************************************************************************/

#define CC3220SF_GPIO0             ((CC3220SF_GPIO_TypeDef  *)   CC3220SF_GPIOA0_BASE)
#define CC3220SF_GPIO1             ((CC3220SF_GPIO_TypeDef  *)   CC3220SF_GPIOA1_BASE)
#define CC3220SF_GPIO2             ((CC3220SF_GPIO_TypeDef  *)   CC3220SF_GPIOA2_BASE)
#define CC3220SF_GPIO3             ((CC3220SF_GPIO_TypeDef  *)   CC3220SF_GPIOA3_BASE)
#define CC3220SF_GPIO4             ((CC3220SF_GPIO_TypeDef  *)   CC3220SF_GPIOA4_BASE)

/******************************************************************************/
/*                                                                            */
/*                                  UART                                      */
/*                                                                            */
/******************************************************************************/

#define CC3220SF_UART0             ((CC3220SF_UART_TypeDef  *)   CC3220SF_UARTA0_BASE)
#define CC3220SF_UART1             ((CC3220SF_UART_TypeDef  *)   CC3220SF_UARTA1_BASE)


/**
  * @}
  */


/* =========================================  End of section using anonymous unions  ========================================= */
#if   defined (__CC_ARM)
  #pragma pop
#elif defined (__ICCARM__)
  /* leave anonymous unions enabled */
#elif (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning restore
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif


#ifdef __cplusplus
}
#endif

#endif  /* CC3220SF_H */
