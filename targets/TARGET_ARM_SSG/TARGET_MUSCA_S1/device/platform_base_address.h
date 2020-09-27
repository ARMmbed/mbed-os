/*
 * Copyright (c) 2017-2020 Arm Limited. All rights reserved.
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

#ifndef __PLATFORM_BASE_ADDRESS_H__
#define __PLATFORM_BASE_ADDRESS_H__

#ifdef __cplusplus
extern "C" {
#endif


/* =========================================================================================================================== */
/* ================                          Device Specific Peripheral Address Map                           ================ */
/* =========================================================================================================================== */


/** @addtogroup Device_Peripheral_peripheralAddr
  * @{
  */

/* Non-Secure Peripheral and SRAM base address */
#define MUSCA_S1_CODE_SRAM_NS_BASE       (0x00000000UL)                              /*!< (Non-Secure Code SRAM         ) Base Address */
#define MUSCA_S1_QSPI_FLASH_NS_BASE      (0x00200000UL)                              /*!< (Non-Secure QSPI FLASH        ) Base Address */
#define MUSCA_S1_MRAM_NS_BASE            (0x0A000000UL)                              /*!< (Non-Secure MRAM              ) Base Address */
#define MUSCA_S1_OTP_NS_BASE             (0x0E000000UL)                              /*!< (Non-Secure OTP               ) Base Address */
#define MUSCA_S1_SRAM_NS_BASE            (0x20000000UL)                              /*!< (Non-Secure Internal SRAM     ) Base Address */
#define MUSCA_S1_BASE_ELEMENT_NS_BASE    (0x40000000UL)                              /*!< (Non-Secure Base Peripherals  ) Base Address */
#define MUSCA_S1_CMSDK_TIMER0_NS_BASE    (0x40000000UL)                              /*!< (Non-Secure CMSDK Timer0      ) Base Address */
#define MUSCA_S1_CMSDK_TIMER1_NS_BASE    (0x40001000UL)                              /*!< (Non-Secure CMSDK Timer1      ) Base Address */
#define MUSCA_S1_CMSDK_DUALTIMER_NS_BASE (0x40002000UL)                              /*!< (Non-Secure CMSDK Dual Timer  ) Base Address */
#define MUSCA_S1_MHU0_NS_BASE            (0x40003000UL)                              /*!< (Non-Secure MHU0              ) Base Address */
#define MUSCA_S1_MHU1_NS_BASE            (0x40004000UL)                              /*!< (Non-Secure MHU1              ) Base Address */
#define MUSCA_S1_CPU_ELEMENT_NS_BASE     (0x40010000UL)                              /*!< (Non-Secure CPU Peripherals   ) Base Address */
#define MUSCA_S1_SYSTEM_INFO_NS_BASE     (0x40020000UL)                              /*!< (Non-Secure System Info       ) Base Address */
#define MUSCA_S1_CMSDK_S32KTIMER_NS_BASE (0x4002F000UL)                              /*!< (Non-Secure CMSDK S32K Timer  ) Base Address */
#define MUSCA_S1_NSPCTRL_NS_BASE         (0x40080000UL)                              /*!< (Non-Secure Privilege Ctrl Blk) Base Address */
#define MUSCA_S1_CMSDK_WATCHDOG_NS_BASE  (0x40081000UL)                              /*!< (Non-Secure CMSDK Watchdog    ) Base Address */
#define MUSCA_S1_UART0_NS_BASE           (0x40101000UL)                              /*!< (Non-Secure UART0             ) Base Address */
#define MUSCA_S1_UART1_NS_BASE           (0x40102000UL)                              /*!< (Non-Secure UART1             ) Base Address */
#define MUSCA_S1_SPI0_NS_BASE            (0x40103000UL)                              /*!< (Non-Secure SPI0              ) Base Address */
#define MUSCA_S1_I2C0_NS_BASE            (0x40104000UL)                              /*!< (Non-Secure I2C0              ) Base Address */
#define MUSCA_S1_I2C1_NS_BASE            (0x40105000UL)                              /*!< (Non-Secure I2C1              ) Base Address */
#define MUSCA_S1_I2S_NS_BASE             (0x40106000UL)                              /*!< (Non-Secure I2S               ) Base Address */
#define MUSCA_S1_PWM0_NS_BASE            (0x40107000UL)                              /*!< (Non-Secure PWM0              ) Base Address */
#define MUSCA_S1_RTC_NS_BASE             (0x40108000UL)                              /*!< (Non-Secure RTC               ) Base Address */
#define MUSCA_S1_PVT_NS_BASE             (0x40109000UL)                              /*!< (Non-Secure PVT sensors       ) Base Address */
#define MUSCA_S1_QSPI_REG_NS_BASE        (0x4010A000UL)                              /*!< (Non-Secure QSPI registers    ) Base Address */
#define MUSCA_S1_TIMER_NS_BASE           (0x4010B000UL)                              /*!< (Non-Secure Timer             ) Base Address */
#define MUSCA_S1_SCC_NS_BASE             (0x4010C000UL)                              /*!< (Non-Secure SCC               ) Base Address */
#define MUSCA_S1_PWM1_NS_BASE            (0x4010E000UL)                              /*!< (Non-Secure PWM1              ) Base Address */
#define MUSCA_S1_PWM2_NS_BASE            (0x4010F000UL)                              /*!< (Non-Secure PWM2              ) Base Address */
#define MUSCA_S1_GPIO_NS_BASE            (0x40110000UL)                              /*!< (Non-Secure GPIO              ) Base Address */
#define MUSCA_S1_QSPI_MPC_NS_BASE        (0x40120000UL)                              /*!< (Non-Secure QSPI MPC          ) Base Address */
#define MUSCA_S1_CODE_SRAM_MPC_NS_BASE   (0x40130000UL)                              /*!< (Non-Secure Code SRAM MPC     ) Base Address */
#define MUSCA_S1_MRAM_MPC_NS_BASE        (0x40140000UL)                              /*!< (Non-Secure MRAM MPC          ) Base Address */
#define MUSCA_S1_DEFAULT_SLAVE_NS_BASE   (0x60000000UL)                              /*!< (Non-Secure Unused AHB        ) Base Address */
/* Secure Peripheral and SRAM base address */
#define MUSCA_S1_CODE_SRAM_S_BASE        (0x10000000UL)                              /*!< (Secure Code SRAM         ) Base Address */
#define MUSCA_S1_QSPI_FLASH_S_BASE       (0x10200000UL)                              /*!< (Secure QSPI FLASH        ) Base Address */
#define MUSCA_S1_MRAM_S_BASE             (0x1A000000UL)                              /*!< (Secure MRAM              ) Base Address */
#define MUSCA_S1_OTP_S_BASE              (0x1E000000UL)                              /*!< (Secure OTP               ) Base Address */
#define MUSCA_S1_SRAM_S_BASE             (0x30000000UL)                              /*!< (Secure Internal SRAM     ) Base Address */
#define MUSCA_S1_BASE_ELEMENT_S_BASE     (0x50000000UL)                              /*!< (Secure Base Peripherals  ) Base Address */
#define MUSCA_S1_MHU0_S_BASE             (0x50003000UL)                              /*!< (Secure MHU0              ) Base Address */
#define MUSCA_S1_MHU1_S_BASE             (0x50004000UL)                              /*!< (Secure MHU1              ) Base Address */
#define MUSCA_S1_CPU_ELEMENT_S_BASE      (0x50010000UL)                              /*!< (Secure CPU Peripherals   ) Base Address */
#define MUSCA_S1_SYSTEM_INFO_S_BASE      (0x50020000UL)                              /*!< (Secure System Info       ) Base Address */
#define MUSCA_S1_SYSTEM_CTRL_S_BASE      (0x50021000UL)                              /*!< (Secure System Control    ) Base Address */
#define MUSCA_S1_CMSDK_S32KTIMER_S_BASE  (0x5002F000UL)                              /*!< (Secure CMSDK S32K Timer  ) Base Address */
#define MUSCA_S1_CMSDK_TIMER0_S_BASE     (0x50000000UL)                              /*!< (Secure CMSDK Timer0      ) Base Address */
#define MUSCA_S1_CMSDK_TIMER1_S_BASE     (0x50001000UL)                              /*!< (Secure CMSDK Timer1      ) Base Address */
#define MUSCA_S1_CMSDK_DUALTIMER_S_BASE  (0x50002000UL)                              /*!< (Secure CMSDK Dual Timer  ) Base Address */
#define MUSCA_S1_SPCTRL_S_BASE           (0x50080000UL)                              /*!< (Secure Privilege Ctrl Blk) Base Address */
#define MUSCA_S1_CMSDK_WATCHDOG_S_BASE   (0x50081000UL)                              /*!< (Secure CMSDK Watchdog    ) Base Address */
#define MUSCA_S1_MPC_SRAM0_S_BASE        (0x50083000UL)                              /*!< (Secure MPC SRAM Bank 0   ) Base Address */
#define MUSCA_S1_MPC_SRAM1_S_BASE        (0x50084000UL)                              /*!< (Secure MPC SRAM Bank 1   ) Base Address */
#define MUSCA_S1_MPC_SRAM2_S_BASE        (0x50085000UL)                              /*!< (Secure MPC SRAM Bank 2   ) Base Address */
#define MUSCA_S1_MPC_SRAM3_S_BASE        (0x50086000UL)                              /*!< (Secure MPC SRAM Bank 3   ) Base Address */
#define MUSCA_S1_UART0_S_BASE            (0x50101000UL)                              /*!< (Secure UART0             ) Base Address */
#define MUSCA_S1_UART1_S_BASE            (0x50102000UL)                              /*!< (Secure UART1             ) Base Address */
#define MUSCA_S1_SPI0_S_BASE             (0x50103000UL)                              /*!< (Secure SPI0              ) Base Address */
#define MUSCA_S1_I2C0_S_BASE             (0x50104000UL)                              /*!< (Secure I2C0              ) Base Address */
#define MUSCA_S1_I2C1_S_BASE             (0x50105000UL)                              /*!< (Secure I2C1              ) Base Address */
#define MUSCA_S1_I2S_S_BASE              (0x50106000UL)                              /*!< (Secure I2S               ) Base Address */
#define MUSCA_S1_PWM0_S_BASE             (0x50107000UL)                              /*!< (Secure PWM0              ) Base Address */
#define MUSCA_S1_RTC_S_BASE              (0x50108000UL)                              /*!< (Secure RTC               ) Base Address */
#define MUSCA_S1_PVT_S_BASE              (0x50109000UL)                              /*!< (Secure PVT sensors       ) Base Address */
#define MUSCA_S1_QSPI_REG_S_BASE         (0x5010A000UL)                              /*!< (Secure QSPI registers    ) Base Address */
#define MUSCA_S1_TIMER_S_BASE            (0x5010B000UL)                              /*!< (Secure Timer             ) Base Address */
#define MUSCA_S1_SCC_S_BASE              (0x5010C000UL)                              /*!< (Secure SCC               ) Base Address */
#define MUSCA_S1_PWM1_S_BASE             (0x5010E000UL)                              /*!< (Secure PWM1              ) Base Address */
#define MUSCA_S1_PWM2_S_BASE             (0x5010F000UL)                              /*!< (Secure PWM2              ) Base Address */
#define MUSCA_S1_GPIO_S_BASE             (0x50110000UL)                              /*!< (Secure GPIO              ) Base Address */
#define MUSCA_S1_QSPI_MPC_S_BASE         (0x50120000UL)                              /*!< (Secure QSPI MPC          ) Base Address */
#define MUSCA_S1_CODE_SRAM_MPC_S_BASE    (0x50130000UL)                              /*!< (Secure Code SRAM MPC     ) Base Address */
#define MUSCA_S1_MRAM_MPC_S_BASE         (0x50140000UL)                              /*!< (Secure MRAM MPC          ) Base Address */
#define MUSCA_S1_DEFAULT_SLAVE_S_BASE    (0x70000000UL)                              /*!< (Secure Unused AHB        ) Base Address */

/* SRAM MPC ranges and limits */
/* Internal memory */
#define MPC_ISRAM0_RANGE_BASE_NS     0x20000000
#define MPC_ISRAM0_RANGE_LIMIT_NS    0x2001FFFF
#define MPC_ISRAM0_RANGE_OFFSET_NS   0x00000000
#define MPC_ISRAM0_RANGE_BASE_S      0x30000000
#define MPC_ISRAM0_RANGE_LIMIT_S     0x3001FFFF
#define MPC_ISRAM0_RANGE_OFFSET_S    0x00000000

#define MPC_ISRAM1_RANGE_BASE_NS     0x20020000
#define MPC_ISRAM1_RANGE_LIMIT_NS    0x2003FFFF
#define MPC_ISRAM1_RANGE_OFFSET_NS   0x00000000
#define MPC_ISRAM1_RANGE_BASE_S      0x30020000
#define MPC_ISRAM1_RANGE_LIMIT_S     0x3003FFFF
#define MPC_ISRAM1_RANGE_OFFSET_S    0x00000000

#define MPC_ISRAM2_RANGE_BASE_NS     0x20040000
#define MPC_ISRAM2_RANGE_LIMIT_NS    0x2005FFFF
#define MPC_ISRAM2_RANGE_OFFSET_NS   0x00000000
#define MPC_ISRAM2_RANGE_BASE_S      0x30040000
#define MPC_ISRAM2_RANGE_LIMIT_S     0x3005FFFF
#define MPC_ISRAM2_RANGE_OFFSET_S    0x00000000

#define MPC_ISRAM3_RANGE_BASE_NS     0x20060000
#define MPC_ISRAM3_RANGE_LIMIT_NS    0x2007FFFF
#define MPC_ISRAM3_RANGE_OFFSET_NS   0x00000000
#define MPC_ISRAM3_RANGE_BASE_S      0x30060000
#define MPC_ISRAM3_RANGE_LIMIT_S     0x3007FFFF
#define MPC_ISRAM3_RANGE_OFFSET_S    0x00000000

/* Code SRAM memory */
#define MPC_CODE_SRAM_RANGE_BASE_NS   (0x00000000)
#define MPC_CODE_SRAM_RANGE_LIMIT_NS  (0x001FFFFF)
#define MPC_CODE_SRAM_RANGE_OFFSET_NS (0x00000000)
#define MPC_CODE_SRAM_RANGE_BASE_S    (0x10000000)
#define MPC_CODE_SRAM_RANGE_LIMIT_S   (0x101FFFFF)
#define MPC_CODE_SRAM_RANGE_OFFSET_S  (0x00000000)

/* QSPI Flash memory */
#define MPC_QSPI_RANGE_BASE_NS        (0x00200000)
#define MPC_QSPI_RANGE_LIMIT_NS       (0x011FFFFF)
#define MPC_QSPI_RANGE_OFFSET_NS      (0x00200000)
#define MPC_QSPI_RANGE_BASE_S         (0x10200000)
#define MPC_QSPI_RANGE_LIMIT_S        (0x111FFFFF)
#define MPC_QSPI_RANGE_OFFSET_S       (0x00200000)

/* MRAM memory */
#define MPC_MRAM_RANGE_BASE_NS        (0x0A000000)
#define MPC_MRAM_RANGE_LIMIT_NS       (0x0A1FFFFF)
#define MPC_MRAM_RANGE_OFFSET_NS      (0x00000000)
#define MPC_MRAM_RANGE_BASE_S         (0x1A000000)
#define MPC_MRAM_RANGE_LIMIT_S        (0x1A1FFFFF)
#define MPC_MRAM_RANGE_OFFSET_S       (0x00000000)

/** @} */ /* End of group Device_Peripheral_peripheralAddr */


#ifdef __cplusplus
}
#endif

#endif  /* __PLATFORM_BASE_ADDRESS_H__ */
