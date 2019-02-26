/*
 * Copyright (c) 2017-2019 Arm Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This file is derivative of CMSIS V5.01 Device\_Template_Vendor\Vendor\Device\Include\Device.h
 */

#ifndef SYSTEM_CMSDK_MUSCA_H
#define SYSTEM_CMSDK_MUSCA_H

#ifdef __cplusplus
extern "C" {
#endif

/* =========================================================================================================================== */
/* ================                                Interrupt Number Definition                                ================ */
/* =========================================================================================================================== */

typedef enum IRQn
{
/* ===========================================  Core Specific Interrupt Numbers  ============================================= */
  NonMaskableInt_IRQn       = -14,              /*  -14 Non Maskable Interrupt                                                 */
  HardFault_IRQn            = -13,              /*  -13 HardFault Interrupt                                                    */
  MemoryManagement_IRQn     = -12,              /*  -12 Memory Management Interrupt                                            */
  BusFault_IRQn             = -11,              /*  -11 Bus Fault Interrupt                                                    */
  UsageFault_IRQn           = -10,              /*  -10 Usage Fault Interrupt                                                  */
  SecureFault_IRQn          =  -9,              /*  -9 Secure Fault Interrupt                                                  */
  SVCall_IRQn               =  -5,              /*  -5 SV Call Interrupt                                                       */
  DebugMonitor_IRQn         =  -4,              /*  -4 Debug Monitor Interrupt                                                 */
  PendSV_IRQn               =  -2,              /*  -2 Pend SV Interrupt                                                       */
  SysTick_IRQn              =  -1,              /*  -1 System Tick Interrupt                                                   */

/* ========================================  Musca Specific SSE-200 Interrupt Numbers  ====================================== */
  NS_WATCHDOG_RESET_IRQn    = 0,                 /*   Non-Secure Watchdog Reset Request Interrupt                              */
  NS_WATCHDOG_IRQn          = 1,                 /*   Non-Secure Watchdog Interrupt                                            */
  S32K_TIMER_IRQn           = 2,                 /*   S32K Timer Interrupt                                                     */
  TIMER0_IRQn               = 3,                 /*   CMSDK Timer 0 Interrupt                                                  */
  TIMER1_IRQn               = 4,                 /*   CMSDK Timer 1 Interrupt                                                  */
  DUALTIMER_IRQn            = 5,                 /*   CMSDK Dual Timer Interrupt                                               */
  MHU0_IRQn                 = 6,                 /*   Message Handling Unit 0 Interrupt                                        */
  MHU1_IRQn                 = 7,                 /*   Message Handling Unit 1 Interrupt                                        */
  CRYPTOCELL_IRQn           = 8,                 /*   CryptoCell-312 Interrupt                                                 */
  S_MPC_COMBINED_IRQn       = 9,                 /*   Secure Combined MPC Interrupt                                            */
  S_PPC_COMBINED_IRQn       = 10,                /*   Secure Combined PPC Interrupt                                            */
  S_MSC_COMBINED_IRQn       = 11,                /*   Secure Combined MSC Interrupt                                            */
  S_BRIDGE_ERR_IRQn         = 12,                /*   Secure Bridge Error Combined Interrupt                                   */
  I_CACHE_INV_ERR_IRQn      = 13,                /*   Instruction Cache Invalidation Interrupt                                 */
  /* Reserved               = 14,                     Reserved                                                                 */
  SYS_PPU_IRQn              = 15,                /*   System PPU Interrupt                                                     */
  CPU0_PPU_IRQn             = 16,                /*   CPU0 PPU Interrupt                                                       */
  CPU1_PPU_IRQn             = 17,                /*   CPU1 PPU Interrupt                                                       */
  CPU0_DGB_PPU_IRQn         = 18,                /*   CPU0 Debug PPU Interrupt                                                 */
  CPU1_DGB_PPU_IRQn         = 19,                /*   CPU1 Debug PPU Interrupt                                                 */
  CRYPTOCELL_PPU_IRQn       = 20,                /*   CryptoCell PPU Interrupt                                                 */
  /* Reserved               = 21,                     Reserved                                                                 */
  RAM0_PPU_IRQn             = 22,                /*   RAM 0 PPU Interrupt                                                      */
  RAM1_PPU_IRQn             = 23,                /*   RAM 1 PPU Interrupt                                                      */
  RAM2_PPU_IRQn             = 24,                /*   RAM 2 PPU Interrupt                                                      */
  RAM3_PPU_IRQn             = 25,                /*   RAM 3 PPU Interrupt                                                      */
  DEBUG_PPU_IRQn            = 26,                /*   Debug PPU Interrupt                                                      */
  /* Reserved               = 27,                     Reserved                                                                 */
  CPU0_CTI_IRQn             = 28,                /*   CPU0 CTI Interrupt                                                       */
  CPU1_CTI_IRQn             = 29,                /*   CPU1 CTI Interrupt                                                       */
  /* Reserved               = 30,                     Reserved                                                                 */
  /* Reserved               = 31,                     Reserved                                                                 */
/* ==========================================  Musca Specific Expansion Interrupt Numbers  =================================== */
  /* None                   = 32,                     Not used. Tied to 0                                                      */
  GpTimer_IRQn              = 33,                /*   General Purpose Timer Interrupt                                          */
  I2C0_IRQn                 = 34,                /*   I2C0 Interrupt                                                           */
  I2C1_IRQn                 = 35,                /*   I2C1 Interrupt                                                           */
  I2S_IRQn                  = 36,                /*   I2S Interrupt                                                            */
  SPI_IRQn                  = 37,                /*   SPI Interrupt                                                            */
  QSPI_IRQn                 = 38,                /*   QSPI Interrupt                                                           */
  UART0_Rx_IRQn             = 39,                /*   UART0 receive FIFO interrupt                                             */
  UART0_Tx_IRQn             = 40,                /*   UART0 transmit FIFO interrupt                                            */
  UART0_RxTimeout_IRQn      = 41,                /*   UART0 receive timeout interrupt                                          */
  UART0_ModemStatus_IRQn    = 42,                /*   UART0 modem status interrupt                                             */
  UART0_Error_IRQn          = 43,                /*   UART0 error interrupt                                                    */
  UART0_IRQn                = 44,                /*   UART0 interrupt                                                          */
  UART1_Rx_IRQn             = 45,                /*   UART1 receive FIFO interrupt                                             */
  UART1_Tx_IRQn             = 46,                /*   UART1 transmit FIFO interrupt                                            */
  UART1_RxTimeout_IRQn      = 47,                /*   UART1 receive timeout interrupt                                          */
  UART1_ModemStatus_IRQn    = 48,                /*   UART1 modem status interrupt                                             */
  UART1_Error_IRQn          = 49,                /*   UART1 error interrupt                                                    */
  UART1_IRQn                = 50,                /*   UART1 interrupt                                                          */
  GPIO_0_IRQn               = 51,                /*   GPIO 0 interrupt                                                         */
  GPIO_1_IRQn               = 52,                /*   GPIO 1 interrupt                                                         */
  GPIO_2_IRQn               = 53,                /*   GPIO 2 interrupt                                                         */
  GPIO_3_IRQn               = 54,                /*   GPIO 3 interrupt                                                         */
  GPIO_4_IRQn               = 55,                /*   GPIO 4 interrupt                                                         */
  GPIO_5_IRQn               = 56,                /*   GPIO 5 interrupt                                                         */
  GPIO_6_IRQn               = 57,                /*   GPIO 6 interrupt                                                         */
  GPIO_7_IRQn               = 58,                /*   GPIO 7 interrupt                                                         */
  GPIO_8_IRQn               = 59,                /*   GPIO 8 interrupt                                                         */
  GPIO_9_IRQn               = 60,                /*   GPIO 9 interrupt                                                         */
  GPIO_10_IRQn              = 61,                /*   GPIO 10 interrupt                                                        */
  GPIO_11_IRQn              = 62,                /*   GPIO 11 interrupt                                                        */
  GPIO_12_IRQn              = 63,                /*   GPIO 12 interrupt                                                        */
  GPIO_13_IRQn              = 64,                /*   GPIO 13 interrupt                                                        */
  GPIO_14_IRQn              = 65,                /*   GPIO 14 interrupt                                                        */
  GPIO_15_IRQn              = 66,                /*   GPIO 15 interrupt                                                        */
  Combined_IRQn             = 67,                /*   Combined interrupt                                                       */
  PVT_IRQn                  = 68,                /*   PVT sensor interrupt                                                     */
  /* Reserved               = 69,                     Reserved                                                                 */
  PWM_0_IRQn                = 70,                /*   PWM0 interrupt                                                           */
  RTC_IRQn                  = 71,                /*   RTC interrupt                                                            */
  GpTimer1_IRQn             = 72,                /*   General Purpose Timer1 Interrupt                                         */
  GpTimer0_IRQn             = 73,                /*   General Purpose Timer0 Interrupt                                         */
  PWM_1_IRQn                = 74,                /*   PWM1 interrupt                                                           */
  PWM_2_IRQn                = 75,                /*   PWM2 interrupt                                                           */
  IOMUX_IRQn                = 76,                /*   IOMUX interrupt                                                          */
} IRQn_Type;



/* =========================================================================================================================== */
/* ================                           Processor and Core Peripheral Section                           ================ */
/* =========================================================================================================================== */

#include "system_core_musca.h"

/** @} */ /* End of group Configuration_of_CMSIS */


#include <core_cm33.h>                           /*!< ARM Cortex-M33 processor and core peripherals */
#include "system_musca.h"                        /*!< Musca System */



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


/** @addtogroup Device_Peripheral_peripherals
  * @{
  */

/*@}*/ /* end of group <Device>_Peripherals */

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


/* =========================================================================================================================== */
/* ================                          Device Specific Peripheral Address Map                           ================ */
/* =========================================================================================================================== */


/** @addtogroup Device_Peripheral_peripheralAddr
  * @{
  */

/* Non-Secure Peripheral and SRAM base address */
#define MUSCA_CODE_SRAM_NS_BASE       (0x00000000UL)                              /*!< (Non-Secure Code SRAM         ) Base Address */
#define MUSCA_QSPI_FLASH_NS_BASE      (0x00200000UL)                              /*!< (Non-Secure QSPI FLASH        ) Base Address */
#define MUSCA_OTP_NS_BASE             (0x0E000000UL)                              /*!< (Non-Secure OTP               ) Base Address */
#define MUSCA_SRAM_NS_BASE            (0x20000000UL)                              /*!< (Non-Secure Internal SRAM     ) Base Address */
#define MUSCA_BASE_ELEMENT_NS_BASE    (0x40000000UL)                              /*!< (Non-Secure Base Peripherals  ) Base Address */
#define MUSCA_CMSDK_TIMER0_NS_BASE    (0x40000000UL)                              /*!< (Non-Secure CMSDK Timer0      ) Base Address */
#define MUSCA_CMSDK_TIMER1_NS_BASE    (0x40001000UL)                              /*!< (Non-Secure CMSDK Timer1      ) Base Address */
#define MUSCA_CMSDK_DUALTIMER_NS_BASE (0x40002000UL)                              /*!< (Non-Secure CMSDK Dual Timer  ) Base Address */
#define MUSCA_MHU0_NS_BASE            (0x40003000UL)                              /*!< (Non-Secure MHU0              ) Base Address */
#define MUSCA_MHU1_NS_BASE            (0x40004000UL)                              /*!< (Non-Secure MHU1              ) Base Address */
#define MUSCA_CPU_ELEMENT_NS_BASE     (0x40010000UL)                              /*!< (Non-Secure CPU Peripherals   ) Base Address */
#define MUSCA_SYSTEM_INFO_NS_BASE     (0x40020000UL)                              /*!< (Non-Secure System Info       ) Base Address */
#define MUSCA_CMSDK_S32KTIMER_NS_BASE (0x4002F000UL)                              /*!< (Non-Secure CMSDK S32K Timer  ) Base Address */
#define MUSCA_NSPCTRL_NS_BASE         (0x40080000UL)                              /*!< (Non-Secure Privilege Ctrl Blk) Base Address */
#define MUSCA_CMSDK_WATCHDOG_NS_BASE  (0x40081000UL)                              /*!< (Non-Secure CMSDK Watchdog    ) Base Address */
#define MUSCA_UART0_NS_BASE           (0x40101000UL)                              /*!< (Non-Secure UART0             ) Base Address */
#define MUSCA_UART1_NS_BASE           (0x40102000UL)                              /*!< (Non-Secure UART1             ) Base Address */
#define MUSCA_SPI0_NS_BASE            (0x40103000UL)                              /*!< (Non-Secure SPI0              ) Base Address */
#define MUSCA_I2C0_NS_BASE            (0x40104000UL)                              /*!< (Non-Secure I2C0              ) Base Address */
#define MUSCA_I2C1_NS_BASE            (0x40105000UL)                              /*!< (Non-Secure I2C1              ) Base Address */
#define MUSCA_I2S_NS_BASE             (0x40106000UL)                              /*!< (Non-Secure I2S               ) Base Address */
#define MUSCA_PWM0_NS_BASE            (0x40107000UL)                              /*!< (Non-Secure PWM0              ) Base Address */
#define MUSCA_RTC_NS_BASE             (0x40108000UL)                              /*!< (Non-Secure RTC               ) Base Address */
#define MUSCA_PVT_NS_BASE             (0x40109000UL)                              /*!< (Non-Secure PVT sensors       ) Base Address */
#define MUSCA_QSPI_REG_NS_BASE        (0x4010A000UL)                              /*!< (Non-Secure QSPI registers    ) Base Address */
#define MUSCA_TIMER_NS_BASE           (0x4010B000UL)                              /*!< (Non-Secure Timer             ) Base Address */
#define MUSCA_SCC_NS_BASE             (0x4010C000UL)                              /*!< (Non-Secure SCC               ) Base Address */
#define MUSCA_PWM1_NS_BASE            (0x4010E000UL)                              /*!< (Non-Secure PWM1              ) Base Address */
#define MUSCA_PWM2_NS_BASE            (0x4010F000UL)                              /*!< (Non-Secure PWM2              ) Base Address */
#define MUSCA_GPIO_NS_BASE            (0x40110000UL)                              /*!< (Non-Secure GPIO              ) Base Address */
#define MUSCA_QSPI_MPC_NS_BASE        (0x40120000UL)                              /*!< (Non-Secure QSPI MPC          ) Base Address */
#define MUSCA_CODE_SRAM_MPC_NS_BASE   (0x40130000UL)                              /*!< (Non-Secure Code SRAM MPC     ) Base Address */
#define MUSCA_DEFAULT_SLAVE_NS_BASE   (0x60000000UL)                              /*!< (Non-Secure Unused AHB        ) Base Address */
/* Secure Peripheral and SRAM base address */
#define MUSCA_CODE_SRAM_S_BASE        (0x10000000UL)                              /*!< (Secure Code SRAM         ) Base Address */
#define MUSCA_QSPI_FLASH_S_BASE       (0x10200000UL)                              /*!< (Secure QSPI FLASH        ) Base Address */
#define MUSCA_OTP_S_BASE              (0x1E000000UL)                              /*!< (Secure OTP               ) Base Address */
#define MUSCA_SRAM_S_BASE             (0x30000000UL)                              /*!< (Secure Internal SRAM     ) Base Address */
#define MUSCA_BASE_ELEMENT_S_BASE     (0x50000000UL)                              /*!< (Secure Base Peripherals  ) Base Address */
#define MUSCA_MHU0_S_BASE             (0x50003000UL)                              /*!< (Secure MHU0              ) Base Address */
#define MUSCA_MHU1_S_BASE             (0x50004000UL)                              /*!< (Secure MHU1              ) Base Address */
#define MUSCA_CPU_ELEMENT_S_BASE      (0x50010000UL)                              /*!< (Secure CPU Peripherals   ) Base Address */
#define MUSCA_SYSTEM_INFO_S_BASE      (0x50020000UL)                              /*!< (Secure System Info       ) Base Address */
#define MUSCA_SYSTEM_CTRL_S_BASE      (0x50021000UL)                              /*!< (Secure System Control    ) Base Address */
#define MUSCA_CMSDK_S32KTIMER_S_BASE  (0x5002F000UL)                              /*!< (Secure CMSDK S32K Timer  ) Base Address */
#define MUSCA_CMSDK_TIMER0_S_BASE     (0x50000000UL)                              /*!< (Secure CMSDK Timer0      ) Base Address */
#define MUSCA_CMSDK_TIMER1_S_BASE     (0x50001000UL)                              /*!< (Secure CMSDK Timer1      ) Base Address */
#define MUSCA_CMSDK_DUALTIMER_S_BASE  (0x50002000UL)                              /*!< (Secure CMSDK Dual Timer  ) Base Address */
#define MUSCA_SPCTRL_S_BASE           (0x50080000UL)                              /*!< (Secure Privilege Ctrl Blk) Base Address */
#define MUSCA_CMSDK_WATCHDOG_S_BASE   (0x50081000UL)                              /*!< (Secure CMSDK Watchdog    ) Base Address */
#define MUSCA_MPC_SRAM0_S_BASE        (0x50083000UL)                              /*!< (Secure MPC SRAM Bank 0   ) Base Address */
#define MUSCA_MPC_SRAM1_S_BASE        (0x50084000UL)                              /*!< (Secure MPC SRAM Bank 1   ) Base Address */
#define MUSCA_MPC_SRAM2_S_BASE        (0x50085000UL)                              /*!< (Secure MPC SRAM Bank 2   ) Base Address */
#define MUSCA_MPC_SRAM3_S_BASE        (0x50086000UL)                              /*!< (Secure MPC SRAM Bank 3   ) Base Address */
#define MUSCA_UART0_S_BASE            (0x50101000UL)                              /*!< (Secure UART0             ) Base Address */
#define MUSCA_UART1_S_BASE            (0x50102000UL)                              /*!< (Secure UART1             ) Base Address */
#define MUSCA_SPI0_S_BASE             (0x50103000UL)                              /*!< (Secure SPI0              ) Base Address */
#define MUSCA_I2C0_S_BASE             (0x50104000UL)                              /*!< (Secure I2C0              ) Base Address */
#define MUSCA_I2C1_S_BASE             (0x50105000UL)                              /*!< (Secure I2C1              ) Base Address */
#define MUSCA_I2S_S_BASE              (0x50106000UL)                              /*!< (Secure I2S               ) Base Address */
#define MUSCA_PWM0_S_BASE             (0x50107000UL)                              /*!< (Secure PWM0              ) Base Address */
#define MUSCA_RTC_S_BASE              (0x50108000UL)                              /*!< (Secure RTC               ) Base Address */
#define MUSCA_PVT_S_BASE              (0x50109000UL)                              /*!< (Secure PVT sensors       ) Base Address */
#define MUSCA_QSPI_REG_S_BASE         (0x5010A000UL)                              /*!< (Secure QSPI registers    ) Base Address */
#define MUSCA_TIMER_S_BASE            (0x5010B000UL)                              /*!< (Secure Timer             ) Base Address */
#define MUSCA_SCC_S_BASE              (0x5010C000UL)                              /*!< (Secure SCC               ) Base Address */
#define MUSCA_PWM1_S_BASE             (0x5010E000UL)                              /*!< (Secure PWM1              ) Base Address */
#define MUSCA_PWM2_S_BASE             (0x5010F000UL)                              /*!< (Secure PWM2              ) Base Address */
#define MUSCA_GPIO_S_BASE             (0x50110000UL)                              /*!< (Secure GPIO              ) Base Address */
#define MUSCA_QSPI_MPC_S_BASE         (0x50120000UL)                              /*!< (Secure QSPI MPC          ) Base Address */
#define MUSCA_CODE_SRAM_MPC_S_BASE    (0x50130000UL)                              /*!< (Secure Code SRAM MPC     ) Base Address */
#define MUSCA_DEFAULT_SLAVE_S_BASE    (0x70000000UL)                              /*!< (Secure Unused AHB        ) Base Address */


/* Peripheral memory map */

/** @} */ /* End of group Device_Peripheral_peripheralAddr */


/* =========================================================================================================================== */
/* ================                                  Peripheral declaration                                   ================ */
/* =========================================================================================================================== */


/** @addtogroup Device_Peripheral_declaration
  * @{
  */


#ifdef __cplusplus
}
#endif

#endif  /* SYSTEM_CMSDK_MUSCA_H */
