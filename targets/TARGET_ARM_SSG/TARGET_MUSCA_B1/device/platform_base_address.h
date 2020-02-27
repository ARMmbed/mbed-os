/*
 * Copyright (c) 2009-2018 Arm Limited
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
 *
 * This file is derivative of CMSIS V5.01 Device\_Template_Vendor\Vendor\Device\Include\Device.h
 * Git SHA: 8a1d9d6ee18b143ae5befefa14d89fb5b3f99c75
 */

#ifndef __MUSCA_B1_BASE_ADDRESS_H__
#define __MUSCA_B1_BASE_ADDRESS_H__

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
#define MUSCA_B1_QSPI_FLASH_NS_BASE      (0x00000000UL)           /*!< (Non-Secure QSPI FLASH             ) Base Address */
#define MUSCA_B1_EFLASH0_NS_BASE         (0x0A000000UL)           /*!< (Non-Secure Embedded FLASH 0       ) Base Address */
#define MUSCA_B1_EFLASH1_NS_BASE         (0x0A200000UL)           /*!< (Non-Secure Embedded FLASH 1       ) Base Address */
#define MUSCA_B1_CODE_SRAM_NS_BASE       (0x0A400000UL)           /*!< (Non-Secure Code SRAM              ) Base Address */
#define MUSCA_B1_OTP_NS_BASE             (0x0E000000UL)           /*!< (Non-Secure OTP                    ) Base Address */
#define MUSCA_B1_SRAM_NS_BASE            (0x20000000UL)           /*!< (Non-Secure Internal SRAM          ) Base Address */
#define MUSCA_B1_BASE_ELEMENT_NS_BASE    (0x40000000UL)           /*!< (Non-Secure Base Peripherals       ) Base Address */
#define MUSCA_B1_CMSDK_TIMER0_NS_BASE    (0x40000000UL)           /*!< (Non-Secure CMSDK Timer0           ) Base Address */
#define MUSCA_B1_CMSDK_TIMER1_NS_BASE    (0x40001000UL)           /*!< (Non-Secure CMSDK Timer1           ) Base Address */
#define MUSCA_B1_CMSDK_DUALTIMER_NS_BASE (0x40002000UL)           /*!< (Non-Secure CMSDK Dual Timer       ) Base Address */
#define MUSCA_B1_MHU0_NS_BASE            (0x40003000UL)           /*!< (Non-Secure MHU0                   ) Base Address */
#define MUSCA_B1_MHU1_NS_BASE            (0x40004000UL)           /*!< (Non-Secure MHU1                   ) Base Address */
#define MUSCA_B1_CPU_ELEMENT_NS_BASE     (0x40010000UL)           /*!< (Non-Secure CPU Peripherals        ) Base Address */
#define MUSCA_B1_SYSTEM_INFO_NS_BASE     (0x40020000UL)           /*!< (Non-Secure System Info            ) Base Address */
#define MUSCA_B1_CMSDK_S32KTIMER_NS_BASE (0x4002F000UL)           /*!< (Non-Secure CMSDK S32K Timer       ) Base Address */
#define MUSCA_B1_NSPCTRL_NS_BASE         (0x40080000UL)           /*!< (Non-Secure Privilege Ctrl Blk     ) Base Address */
#define MUSCA_B1_CMSDK_WATCHDOG_NS_BASE  (0x40081000UL)           /*!< (Non-Secure CMSDK Watchdog         ) Base Address */
#define MUSCA_B1_PWM0_NS_BASE            (0x40101000UL)           /*!< (Non-Secure PWM0                   ) Base Address */
#define MUSCA_B1_PWM1_NS_BASE            (0x40102000UL)           /*!< (Non-Secure PWM1                   ) Base Address */
#define MUSCA_B1_PWM2_NS_BASE            (0x40103000UL)           /*!< (Non-Secure PWM2                   ) Base Address */
#define MUSCA_B1_I2S_NS_BASE             (0x40104000UL)           /*!< (Non-Secure I2S                    ) Base Address */
#define MUSCA_B1_UART0_NS_BASE           (0x40105000UL)           /*!< (Non-Secure UART0                  ) Base Address */
#define MUSCA_B1_UART1_NS_BASE           (0x40106000UL)           /*!< (Non-Secure UART1                  ) Base Address */
#define MUSCA_B1_I2C0_NS_BASE            (0x40108000UL)           /*!< (Non-Secure I2C0                   ) Base Address */
#define MUSCA_B1_I2C1_NS_BASE            (0x40109000UL)           /*!< (Non-Secure I2C1                   ) Base Address */
#define MUSCA_B1_SPI0_NS_BASE            (0x4010A000UL)           /*!< (Non-Secure SPI0                   ) Base Address */
#define MUSCA_B1_TIMER_NS_BASE           (0x4010C000UL)           /*!< (Non-Secure Timer                  ) Base Address */
#define MUSCA_B1_RTC_NS_BASE             (0x4010D000UL)           /*!< (Non-Secure RTC                    ) Base Address */
#define MUSCA_B1_PVT_NS_BASE             (0x4010E000UL)           /*!< (Non-Secure PVT sensors            ) Base Address */
#define MUSCA_B1_SDIO_NS_BASE            (0x4010F000UL)           /*!< (Non-Secure SDIO                   ) Base Address */
#define MUSCA_B1_GPIO_NS_BASE            (0x41000000UL)           /*!< (Non-Secure GPIO                   ) Base Address */
#define MUSCA_B1_CI_MHU_SND_NS_BASE      (0x42600000UL)           /*!< (Non-Secure CryptoIsland MHU Send  ) Base Address */
#define MUSCA_B1_CI_MHU_RCV_NS_BASE      (0x42700000UL)           /*!< (Non-Secure CryptoIsland MHU Rcv   ) Base Address */
#define MUSCA_B1_QSPI_REG_NS_BASE        (0x42800000UL)           /*!< (Non-Secure QSPI registers         ) Base Address */

/* Secure Peripheral and SRAM base address */
#define MUSCA_B1_QSPI_FLASH_S_BASE       (0x10000000UL)           /*!< (Secure QSPI FLASH                 ) Base Address */
#define MUSCA_B1_EFLASH0_S_BASE          (0x1A000000UL)           /*!< (Secure Embedded FLASH 0           ) Base Address */
#define MUSCA_B1_EFLASH1_S_BASE          (0x1A200000UL)           /*!< (Secure Embedded FLASH 1           ) Base Address */
#define MUSCA_B1_CODE_SRAM_S_BASE        (0x1A400000UL)           /*!< (Secure Code SRAM                  ) Base Address */
#define MUSCA_B1_OTP_S_BASE              (0x1E000000UL)           /*!< (Secure OTP                        ) Base Address */
#define MUSCA_B1_SRAM_S_BASE             (0x30000000UL)           /*!< (Secure Internal SRAM              ) Base Address */
#define MUSCA_B1_BASE_ELEMENT_S_BASE     (0x50000000UL)           /*!< (Secure Base Peripherals           ) Base Address */
#define MUSCA_B1_CMSDK_TIMER0_S_BASE     (0x50000000UL)           /*!< (Secure CMSDK Timer0               ) Base Address */
#define MUSCA_B1_CMSDK_TIMER1_S_BASE     (0x50001000UL)           /*!< (Secure CMSDK Timer1               ) Base Address */
#define MUSCA_B1_CMSDK_DUALTIMER_S_BASE  (0x50002000UL)           /*!< (Secure CMSDK Dual Timer           ) Base Address */
#define MUSCA_B1_MHU0_S_BASE             (0x50003000UL)           /*!< (Secure MHU0                       ) Base Address */
#define MUSCA_B1_MHU1_S_BASE             (0x50004000UL)           /*!< (Secure MHU1                       ) Base Address */
#define MUSCA_B1_CPU_ELEMENT_S_BASE      (0x50010000UL)           /*!< (Secure CPU Peripherals            ) Base Address */
#define MUSCA_B1_SYSTEM_INFO_S_BASE      (0x50020000UL)           /*!< (Secure System Info                ) Base Address */
#define MUSCA_B1_SYSTEM_CTRL_S_BASE      (0x50021000UL)           /*!< (Secure System Control             ) Base Address */
#define MUSCA_B1_CMSDK_S32K_WDOG_S_BASE  (0x5002E000UL)           /*!< (Secure CMSDK S32K Watchdog        ) Base Address */
#define MUSCA_B1_CMSDK_S32KTIMER_S_BASE  (0x5002F000UL)           /*!< (Secure CMSDK S32K Timer           ) Base Address */
#define MUSCA_B1_SPCTRL_S_BASE           (0x50080000UL)           /*!< (Secure Privilege Ctrl Blk         ) Base Address */
#define MUSCA_B1_CMSDK_WATCHDOG_S_BASE   (0x50081000UL)           /*!< (Secure CMSDK Watchdog             ) Base Address */
#define MUSCA_B1_MPC_SRAM0_S_BASE        (0x50083000UL)           /*!< (Secure MPC SRAM Bank 0            ) Base Address */
#define MUSCA_B1_MPC_SRAM1_S_BASE        (0x50084000UL)           /*!< (Secure MPC SRAM Bank 1            ) Base Address */
#define MUSCA_B1_MPC_SRAM2_S_BASE        (0x50085000UL)           /*!< (Secure MPC SRAM Bank 2            ) Base Address */
#define MUSCA_B1_MPC_SRAM3_S_BASE        (0x50086000UL)           /*!< (Secure MPC SRAM Bank 3            ) Base Address */
#define MUSCA_B1_PWM0_S_BASE             (0x50101000UL)           /*!< (Secure PWM0                       ) Base Address */
#define MUSCA_B1_PWM1_S_BASE             (0x50102000UL)           /*!< (Secure PWM1                       ) Base Address */
#define MUSCA_B1_PWM2_S_BASE             (0x50103000UL)           /*!< (Secure PWM2                       ) Base Address */
#define MUSCA_B1_I2S_S_BASE              (0x50104000UL)           /*!< (Secure I2S                        ) Base Address */
#define MUSCA_B1_UART0_S_BASE            (0x50105000UL)           /*!< (Secure UART0                      ) Base Address */
#define MUSCA_B1_UART1_S_BASE            (0x50106000UL)           /*!< (Secure UART1                      ) Base Address */
#define MUSCA_B1_I2C0_S_BASE             (0x50108000UL)           /*!< (Secure I2C0                       ) Base Address */
#define MUSCA_B1_I2C1_S_BASE             (0x50109000UL)           /*!< (Secure I2C1                       ) Base Address */
#define MUSCA_B1_SPI0_S_BASE             (0x5010A000UL)           /*!< (Secure SPI0                       ) Base Address */
#define MUSCA_B1_SCC_S_BASE              (0x5010B000UL)           /*!< (Secure SCC                        ) Base Address */
#define MUSCA_B1_TIMER_S_BASE            (0x5010C000UL)           /*!< (Secure Timer                      ) Base Address */
#define MUSCA_B1_RTC_S_BASE              (0x5010D000UL)           /*!< (Secure RTC                        ) Base Address */
#define MUSCA_B1_PVT_S_BASE              (0x5010E000UL)           /*!< (Secure PVT sensors                ) Base Address */
#define MUSCA_B1_SDIO_S_BASE             (0x5010F000UL)           /*!< (Secure SDIO                       ) Base Address */
#define MUSCA_B1_GPIO_S_BASE             (0x51000000UL)           /*!< (Secure GPIO                       ) Base Address */
#define MUSCA_B1_QSPI_MPC_S_BASE         (0x52000000UL)           /*!< (Secure QSPI MPC                   ) Base Address */
#define MUSCA_B1_CODE_SRAM_MPC_S_BASE    (0x52100000UL)           /*!< (Secure Code SRAM MPC              ) Base Address */
#define MUSCA_B1_EFLASH0_MPC_S_BASE      (0x52200000UL)           /*!< (Secure Embedded Flash 0 MPC       ) Base Address */
#define MUSCA_B1_EFLASH1_MPC_S_BASE      (0x52300000UL)           /*!< (Secure Embedded Flash 1 MPC       ) Base Address */
#define MUSCA_B1_EFLASH0_REG_MAP_S_BASE  (0x52400000UL)           /*!< (Secure GFC100 EFlash0 controller  ) Base Address */
#define MUSCA_B1_EFLASH1_REG_MAP_S_BASE  (0x52500000UL)           /*!< (Secure GFC100 EFlash1 controller  ) Base Address */
#define MUSCA_B1_CI_MHU_SND_S_BASE       (0x52600000UL)           /*!< (Secure CryptoIsland MHU Sender    ) Base Address */
#define MUSCA_B1_CI_MHU_RCV_S_BASE       (0x52700000UL)           /*!< (Secure CryptoIsland MHU Receiver  ) Base Address */
#define MUSCA_B1_QSPI_REG_S_BASE         (0x52800000UL)           /*!< (Secure QSPI registers             ) Base Address */
#define MUSCA_B1_CRYPTO_MPC_S_BASE       (0x52E00000UL)           /*!< (Secure CryptoIsland MPC           ) Base Address */

/* MPC memory range bases and limits */
/* Internal SRAM */
#define MPC_ISRAM0_RANGE_BASE_NS         (0x20000000UL)
#define MPC_ISRAM0_RANGE_LIMIT_NS        (0x2001FFFFUL)
#define MPC_ISRAM0_RANGE_BASE_S          (0x30000000UL)
#define MPC_ISRAM0_RANGE_LIMIT_S         (0x3001FFFFUL)

#define MPC_ISRAM1_RANGE_BASE_NS         (0x20020000UL)
#define MPC_ISRAM1_RANGE_LIMIT_NS        (0x2003FFFFUL)
#define MPC_ISRAM1_RANGE_BASE_S          (0x30020000UL)
#define MPC_ISRAM1_RANGE_LIMIT_S         (0x3003FFFFUL)

#define MPC_ISRAM2_RANGE_BASE_NS         (0x20040000UL)
#define MPC_ISRAM2_RANGE_LIMIT_NS        (0x2005FFFFUL)
#define MPC_ISRAM2_RANGE_BASE_S          (0x30040000UL)
#define MPC_ISRAM2_RANGE_LIMIT_S         (0x3005FFFFUL)

#define MPC_ISRAM3_RANGE_BASE_NS         (0x20060000UL)
#define MPC_ISRAM3_RANGE_LIMIT_NS        (0x2007FFFFUL)
#define MPC_ISRAM3_RANGE_BASE_S          (0x30060000UL)
#define MPC_ISRAM3_RANGE_LIMIT_S         (0x3007FFFFUL)

/* Code SRAM */
#define MPC_CODE_SRAM_RANGE_BASE_NS      (0x0A400000UL)
#define MPC_CODE_SRAM_RANGE_LIMIT_NS     (0x0A47FFFFUL)
#define MPC_CODE_SRAM_RANGE_BASE_S       (0x1A400000UL)
#define MPC_CODE_SRAM_RANGE_LIMIT_S      (0x1A47FFFFUL)

/* QSPI Flash */
#define MPC_QSPI_RANGE_BASE_NS           (0x00000000UL)
#define MPC_QSPI_RANGE_LIMIT_NS          (0x007FFFFFUL)
#define MPC_QSPI_RANGE_BASE_S            (0x10000000UL)
#define MPC_QSPI_RANGE_LIMIT_S           (0x107FFFFFUL)

/* EFlash */
#define MPC_EFLASH0_RANGE_BASE_NS        (0x0A000000UL)
#define MPC_EFLASH0_RANGE_LIMIT_NS       (0x0A1FFFFFUL)
#define MPC_EFLASH0_RANGE_BASE_S         (0x1A000000UL)
#define MPC_EFLASH0_RANGE_LIMIT_S        (0x1A1FFFFFUL)

#define MPC_EFLASH1_RANGE_BASE_NS        (0x0A200000UL)
#define MPC_EFLASH1_RANGE_LIMIT_NS       (0x0A3FFFFFUL)
#define MPC_EFLASH1_RANGE_BASE_S         (0x1A200000UL)
#define MPC_EFLASH1_RANGE_LIMIT_S        (0x1A3FFFFFUL)

/** @} */ /* End of group Device_Peripheral_peripheralAddr */


#ifdef __cplusplus
}
#endif

#endif  /* __MUSCA_B1_BASE_ADDRESS_H__ */
