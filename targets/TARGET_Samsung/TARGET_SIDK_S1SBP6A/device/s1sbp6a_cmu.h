/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __S1SBP6A_CMU_H
#define __S1SBP6A_CMU_H

#define LSOSC_CLK_FREQ                                     (4096000)
#define HSOSC_CLK_FREQ                                     (LSOSC_CLK_FREQ * 6)
#define EXT_CLK_FREQ                                       (4096000)
#define CMU_PLL_AMP_GAIN                                   (25)

#define CMU_MCU_CLK_CTRL_OFFSET                            (0x0100)
#define CMU_MCU_CLK_GATE_OFFSET                            (0x0104)
#define CMU_PERI_CLK_CTRL_OFFSET                           (0x0110)
#define CMU_PERI_CLK_GATE_OFFSET                           (0x0114)
#define CMU_PERI_CLK_MUX_OFFSET                            (0x0118)
#define CMU_AFE_CLK_CTRL_OFFSET                            (0x0120)
#define CMU_HOSC_CTRL_OFFSET                               (0x060C)
#define CMU_WAIT_PERI_CLK_GATE_OFFSET                      (0x0918)

#define CMU_MCU_CLK_CTRL_SEL_MCU_SRC_SHIFT                 (0)
#define CMU_MCU_CLK_CTRL_AHBCLK_DIV_SHIFT                  (4)
#define CMU_MCU_CLK_CTRL_APBCLK_DIV_SHIFT                  (12)
#define CMU_MCU_CLK_CTRL_SRPCLK_DIV_SHIFT                  (20)

#define CMU_MCU_CLK_CTRL_SEL_MCU_SRC_MASK                  (0x03 << CMU_MCU_CLK_CTRL_SEL_MCU_SRC_SHIFT)
#define CMU_MCU_CLK_CTRL_AHBCLK_DIV_MASK                   (0xFF << CMU_MCU_CLK_CTRL_AHBCLK_DIV_SHIFT)
#define CMU_MCU_CLK_CTRL_APBCLK_DIV_MASK                   (0xFF << CMU_MCU_CLK_CTRL_APBCLK_DIV_SHIFT)
#define CMU_MCU_CLK_CTRL_SRPCLK_DIV_MASK                   (0xFF << CMU_MCU_CLK_CTRL_SRPCLK_DIV_SHIFT)

#define CMU_MCU_CLK_CTRL_SEL_MCU_SRC(c)                    ((c) << CMU_MCU_CLK_CTRL_SEL_MCU_SRC_SHIFT)
#define CMU_MCU_CLK_CTRL_AHBCLK_DIV(c)                     ((c) << CMU_MCU_CLK_CTRL_AHBCLK_DIV_SHIFT)
#define CMU_MCU_CLK_CTRL_APBCLK_DIV(c)                     ((c) << CMU_MCU_CLK_CTRL_APBCLK_DIV_SHIFT)
#define CMU_MCU_CLK_CTRL_SRPCLK_DIV(c)                     ((c) << CMU_MCU_CLK_CTRL_SRPCLK_DIV_SHIFT)

#define CMU_MCU_CLK_GATE_FCLK_SHIFT                        (0)
#define CMU_MCU_CLK_GATE_SRP_SHIFT                         (1)
#define CMU_MCU_CLK_GATE_GPIO_SHIFT                        (2)
#define CMU_MCU_CLK_GATE_UDMAC_SHIFT                       (3)
#define CMU_MCU_CLK_GATE_DTRNG_SHIFT                       (4)
#define CMU_MCU_CLK_GATE_AES_SHIFT                         (5)
#define CMU_MCU_CLK_GATE_SRC_SHIFT                         (6)
#define CMU_MCU_CLK_GATE_QSPI_SHIFT                        (7)
#define CMU_MCU_CLK_GATE_TIMER_SHIFT                       (8)
#define CMU_MCU_CLK_GATE_PTIMER_SHIFT                      (14)
#define CMU_MCU_CLK_GATE_DTIMER_SHIFT                      (16)
#define CMU_MCU_CLK_GATE_WATCHDOG_SHIFT                    (17)
#define CMU_MCU_CLK_GATE_UDMAC_ACG_SHIFT                   (18)
#define CMU_MCU_CLK_GATE_BYPASS_SHIFT                      (19)

#define CMU_MCU_CLK_GATE_FCLK_MASK                         (0x01 << CMU_MCU_CLK_GATE_FCLK_SHIFT)
#define CMU_MCU_CLK_GATE_SRP_MASK                          (0x01 << CMU_MCU_CLK_GATE_SRP_SHIFT)
#define CMU_MCU_CLK_GATE_GPIO_MASK                         (0x01 << CMU_MCU_CLK_GATE_GPIO_SHIFT)
#define CMU_MCU_CLK_GATE_UDMAC_MASK                        (0x01 << CMU_MCU_CLK_GATE_UDMAC_SHIFT)
#define CMU_MCU_CLK_GATE_DTRNG_MASK                        (0x01 << CMU_MCU_CLK_GATE_DTRNG_SHIFT)
#define CMU_MCU_CLK_GATE_AES_MASK                          (0x01 << CMU_MCU_CLK_GATE_AES_SHIFT)
#define CMU_MCU_CLK_GATE_SRC_MASK                          (0x01 << CMU_MCU_CLK_GATE_SRC_SHIFT)
#define CMU_MCU_CLK_GATE_QSPI_MASK                         (0x01 << CMU_MCU_CLK_GATE_QSPI_SHIFT)
#define CMU_MCU_CLK_GATE_TIMER_MASK                        (0x3F << CMU_MCU_CLK_GATE_TIMER_SHIFT)
#define CMU_MCU_CLK_GATE_PTIMER_MASK                       (0x03 << CMU_MCU_CLK_GATE_PTIMER_SHIFT)
#define CMU_MCU_CLK_GATE_DTIMER_MASK                       (0x01 << CMU_MCU_CLK_GATE_DTIMER_SHIFT)
#define CMU_MCU_CLK_GATE_WATCHDOG_MASK                     (0x01 << CMU_MCU_CLK_GATE_WATCHDOG_SHIFT)
#define CMU_MCU_CLK_GATE_UDMAC_ACG_MASK                    (0x01 << CMU_MCU_CLK_GATE_UDMAC_ACG_SHIFT)
#define CMU_MCU_CLK_GATE_BYPASS_MASK                       (0x01 << CMU_MCU_CLK_GATE_BYPASS_SHIFT)

#define CMU_MCU_CLK_GATE_FCLK(c)                           ((c) << CMU_MCU_CLK_GATE_FCLK_SHIFT)
#define CMU_MCU_CLK_GATE_SRP(c)                            ((c) << CMU_MCU_CLK_GATE_SRP_SHIFT)
#define CMU_MCU_CLK_GATE_GPIO(c)                           ((c) << CMU_MCU_CLK_GATE_GPIO_SHIFT)
#define CMU_MCU_CLK_GATE_UDMAC(c)                          ((c) << CMU_MCU_CLK_GATE_UDMAC_SHIFT)
#define CMU_MCU_CLK_GATE_DTRNG(c)                          ((c) << CMU_MCU_CLK_GATE_DTRNG_SHIFT)
#define CMU_MCU_CLK_GATE_AES(c)                            ((c) << CMU_MCU_CLK_GATE_AES_SHIFT)
#define CMU_MCU_CLK_GATE_SRC(c)                            ((c) << CMU_MCU_CLK_GATE_SRC_SHIFT)
#define CMU_MCU_CLK_GATE_QSPI(c)                           ((c) << CMU_MCU_CLK_GATE_QSPI_SHIFT)
#define CMU_MCU_CLK_GATE_TIMER(c)                          ((c) << CMU_MCU_CLK_GATE_TIMER_SHIFT)
#define CMU_MCU_CLK_GATE_PTIMER(c)                         ((c) << CMU_MCU_CLK_GATE_PTIMER_SHIFT)
#define CMU_MCU_CLK_GATE_DTIMER(c)                         ((c) << CMU_MCU_CLK_GATE_DTIMER_SHIFT)
#define CMU_MCU_CLK_GATE_WATCHDOG(c)                       ((c) << CMU_MCU_CLK_GATE_WATCHDOG_SHIFT)
#define CMU_MCU_CLK_GATE_UDMAC_ACG(c)                      ((c) << CMU_MCU_CLK_GATE_UDMAC_ACG_SHIFT)
#define CMU_MCU_CLK_GATE_BYPASS(c)                         ((c) << CMU_MCU_CLK_GATE_BYPASS_SHIFT)

#define CMU_AFE_CLK_CTRL_SEL_SRC_SHIFT                     (0)
#define CMU_AFE_CLK_CTRL_AFE_DIV_ON_SHIFT                  (2)
#define CMU_AFE_CLK_CTRL_SEL_AFECON32_SHIFT                (3)
#define CMU_AFE_CLK_CTRL_SMP_CLK_DIV_SHIFT                 (4)

#define CMU_AFE_CLK_CTRL_SEL_SRC_MASK                      (0x03 << CMU_AFE_CLK_CTRL_SEL_SRC_SHIFT)
#define CMU_AFE_CLK_CTRL_AFE_DIV_ON_MASK                   (0x01 << CMU_AFE_CLK_CTRL_AFE_DIV_ON_SHIFT)
#define CMU_AFE_CLK_CTRL_SEL_AFECON32_MASK                 (0x01 << CMU_AFE_CLK_CTRL_SEL_AFECON32_SHIFT)
#define CMU_AFE_CLK_CTRL_SMP_CLK_DIV_MASK                  (0xFFFF << CMU_AFE_CLK_CTRL_SMP_CLK_DIV_SHIFT)

#define CMU_AFE_CLK_CTRL_SEL_SRC(c)                         ((c) << CMU_AFE_CLK_CTRL_SEL_SRC_SHIFT)
#define CMU_AFE_CLK_CTRL_AFE_DIV_ON(c)                      ((c) << CMU_AFE_CLK_CTRL_AFE_DIV_ON_SHIFT)
#define CMU_AFE_CLK_CTRL_SEL_AFECON32(c)                    ((c) << CMU_AFE_CLK_CTRL_SEL_AFECON32_SHIFT)
#define CMU_AFE_CLK_CTRL_SMP_CLK_DIV(c)                     ((c) << CMU_AFE_CLK_CTRL_SMP_CLK_DIV_SHIFT)

#define CMU_PERI_CLK_MUX_UARTCLK_MUX_SHIFT                  (0)
#define CMU_PERI_CLK_MUX_SPICLK_MUX_SHIFT                   (3)
#define CMU_PERI_CLK_MUX_I2CCLK_MUX_SHIFT                   (8)


#define CMU_PERI_CLK_MUX_UARTCLK_MUX_MASK                   (0x07 << CMU_PERI_CLK_MUX_UARTCLK_MUX_SHIFT)
#define CMU_PERI_CLK_MUX_SPICLK_MUX_MASK                    (0x0F << CMU_PERI_CLK_MUX_SPICLK_MUX_SHIFT)
#define CMU_PERI_CLK_MUX_I2CCLK_MUX_MASK                    (0x1F << CMU_PERI_CLK_MUX_I2CCLK_MUX_SHIFT)

#define CMU_PERI_CLK_MUX_UARTCLK_MUX(c)                     ((c) << CMU_PERI_CLK_MUX_UARTCLK_MUX_SHIFT)
#define CMU_PERI_CLK_MUX_SPICLK_MUX(c)                      ((c) << CMU_PERI_CLK_MUX_SPICLK_MUX_SHIFT)

#define CMU_PERI_CLK_MUX_UARTCLK_MUX_SHIFT                  (0)
#define CMU_PERI_CLK_MUX_SPICLK_MUX_SHIFT                   (3)
#define CMU_PERI_CLK_MUX_I2CCLK_MUX_SHIFT                   (8)

#define CMU_PERI_CLK_MUX_UARTCLK_MUX_MASK                   (0x07 << CMU_PERI_CLK_MUX_UARTCLK_MUX_SHIFT)
#define CMU_PERI_CLK_MUX_SPICLK_MUX_MASK                    (0x0F << CMU_PERI_CLK_MUX_SPICLK_MUX_SHIFT)
#define CMU_PERI_CLK_MUX_I2CCLK_MUX_MASK                    (0x1F << CMU_PERI_CLK_MUX_I2CCLK_MUX_SHIFT)

#define CMU_PERI_CLK_MUX_UARTCLK_MUX(c)                     ((c) << CMU_PERI_CLK_MUX_UARTCLK_MUX_SHIFT)
#define CMU_PERI_CLK_MUX_SPICLK_MUX(c)                      ((c) << CMU_PERI_CLK_MUX_SPICLK_MUX_SHIFT)

#define CMU_PERI_CLK_CTRL_SEL_SRC_SHIFT                     (0)
#define CMU_PERI_CLK_CTRL_CLK_DIV_SHIFT                     (4)

#define CMU_PERI_CLK_CTRL_SEL_SRC_MASK                      (0x03 << CMU_PERI_CLK_CTRL_SEL_SRC_SHIFT)
#define CMU_PERI_CLK_CTRL_CLK_DIV_MASK                      (0xFF << CMU_PERI_CLK_CTRL_CLK_DIV_SHIFT)

#define CMU_PERI_CLK_CTRL_SEL_SRC(c)                        ((c) << CMU_PERI_CLK_CTRL_SEL_SRC_SHIFT)
#define CMU_PERI_CLK_CTRL_CLK_DIV(c)                        ((c) << CMU_PERI_CLK_CTRL_CLK_DIV_SHIFT)

#define CMU_HOSC_CTRL_MODE_SHIFT                            (1)
#define CMU_HOSC_CTRL_CAL_SHIFT                             (2)
#define CMU_HOSC_CTRL_TOL_SHIFT                             (10)

#define CMU_HOSC_CTRL_MODE_MASK                             (0x01 << CMU_HOSC_CTRL_MODE_SHIFT)
#define CMU_HOSC_CTRL_CAL_MASK                              (0xFF << CMU_HOSC_CTRL_CAL_SHIFT)
#define CMU_HOSC_CTRL_TOL_MASK                              (0x03 << CMU_HOSC_CTRL_TOL_SHIFT)

#define CMU_HOSC_CTRL_MODE(c)                               ((c) << CMU_HOSC_CTRL_MODE_SHIFT)
#define CMU_HOSC_CTRL_CAL(c)                                ((c) << CMU_HOSC_CTRL_CAL_SHIFT)
#define CMU_HOSC_CTRL_TOL(c)                                ((c) << CMU_HOSC_CTRL_TOL_SHIFT)

#define ENABLE                                  1
#define DISABLE                                 0

#define CONFIG_CMU_FCLK_AHBCLK                  (ENABLE  << 0)
#define CONFIG_CMU_SRP_SRPCLK                   (DISABLE << 1)
#define CONFIG_CMU_GPIO_AHBCLK                  (ENABLE << 2)
#define CONFIG_CMU_UDMAC_AHBCLK                 (ENABLE << 3)
#define CONFIG_CMU_DTRNG_AHBCLK                 (DISABLE << 4)
#define CONFIG_CMU_AES_AHBCLK                   (DISABLE << 5)
#define CONFIG_CMU_SRC_AHBCLK                   (DISABLE << 6)
#define CONFIG_CMU_QSPI_AHBCLK                  (DISABLE << 7)
#define CONFIG_CMU_TIMER0_APBCLK                (DISABLE << 8)
#define CONFIG_CMU_TIMER1_APBCLK                (DISABLE << 9)
#define CONFIG_CMU_TIMER2_APBCLK                (DISABLE << 10)
#define CONFIG_CMU_TIMER3_APBCLK                (DISABLE << 11)
#define CONFIG_CMU_TIMER4_APBCLK                (DISABLE << 12)
#define CONFIG_CMU_TIMER5_APBCLK                (DISABLE << 13)
#define CONFIG_CMU_TIMER6_APBCLK                (DISABLE << 14)
#define CONFIG_CMU_PWMTIMER_APBCLK              (DISABLE << 15)
#define CONFIG_CMU_DUALTIMER_APBCLK             (ENABLE << 16)
#define CONFIG_CMU_WATCHDOG_APBCLK              (ENABLE << 17)
#define CONFIG_CMU_UART0_CLK                    (DISABLE  << 18)
#define CONFIG_CMU_UART1_CLK                    (DISABLE << 19)
#define CONFIG_CMU_UART2_CLK                    (DISABLE << 20)
#define CONFIG_CMU_SPI0_CLK                     (DISABLE  << 21)
#define CONFIG_CMU_SPI1_CLK                     (DISABLE << 22)
#define CONFIG_CMU_SPI2_CLK                     (DISABLE << 23)
#define CONFIG_CMU_SPI3_CLK                     (DISABLE << 24)
#define CONFIG_CMU_SPI4_CLK                     (DISABLE << 25)
#define CONFIG_CMU_I2C0_CLK                     (DISABLE << 26)
#define CONFIG_CMU_I2C1_CLK                     (DISABLE << 27)
#define CONFIG_CMU_I2C2_CLK                     (DISABLE << 28)
#define CONFIG_CMU_I2C3_CLK                     (DISABLE << 29)
#define CONFIG_CMU_I2C4_CLK                     (DISABLE << 30)

#define INIT_CLOCK_CONFIG  (CONFIG_CMU_FCLK_AHBCLK | CONFIG_CMU_SRP_SRPCLK |  \
                CONFIG_CMU_GPIO_AHBCLK | CONFIG_CMU_UDMAC_AHBCLK |         \
                CONFIG_CMU_DTRNG_AHBCLK |CONFIG_CMU_AES_AHBCLK |           \
                CONFIG_CMU_SRC_AHBCLK | CONFIG_CMU_QSPI_AHBCLK |           \
                CONFIG_CMU_TIMER0_APBCLK | CONFIG_CMU_TIMER1_APBCLK |      \
                CONFIG_CMU_TIMER2_APBCLK | CONFIG_CMU_TIMER3_APBCLK |      \
                CONFIG_CMU_TIMER4_APBCLK | CONFIG_CMU_TIMER5_APBCLK |      \
                CONFIG_CMU_TIMER6_APBCLK | CONFIG_CMU_PWMTIMER_APBCLK |    \
                CONFIG_CMU_DUALTIMER_APBCLK | CONFIG_CMU_WATCHDOG_APBCLK | \
                CONFIG_CMU_UART0_CLK | CONFIG_CMU_UART1_CLK |              \
                CONFIG_CMU_UART2_CLK | CONFIG_CMU_SPI0_CLK |               \
                CONFIG_CMU_SPI1_CLK | CONFIG_CMU_SPI2_CLK |                \
                CONFIG_CMU_SPI3_CLK | CONFIG_CMU_SPI4_CLK |                \
                CONFIG_CMU_I2C0_CLK | CONFIG_CMU_I2C1_CLK |                \
                CONFIG_CMU_I2C2_CLK | CONFIG_CMU_I2C3_CLK |                \
                CONFIG_CMU_I2C4_CLK )

typedef enum _cmu_sel_sample_clk_t {
    CMU_SELECT_INTERNAL_CLK = 0,
    CMU_SELECT_EXTERNAL_XTAL_32768_CLK,

    CMU_SELECT_INVALID
} cmu_sel_smp_clk_t;

typedef enum {
    CMU_SRC_CLK_LSOSC = 0,          // Internal LOSC 4.096MHz
    CMU_SRC_CLK_EXT_4M,             // External XTAL 4.096MHz
    CMU_SRC_CLK_HSOSC,              // Internal  HOSC 24.576MHz
    CMU_SRC_CLK_PLL,                // Internal PLL 102.4MHz
    CMU_SRC_32768Hz,                // External XTAL  32.768KHz

    CMU_SOURCE_CLK_INVALID
} cmu_src_clk_t;

typedef enum {
    CMU_SAMPLE_CLK_AFTER_DIV = 0,
    CMU_SRPCLK_AFTER_DIV,
    CMU_AHBCLK_AFTER_DIV,
    CMU_APBCLK_AFTER_DIV,
    CMU_PERICLK_AFTER_DIV,

    CMU_RTC_CLK,
    CMU_AFECON_CLK,
    CMU_SAMPLE_CLK,

    CMU_FCLK_AHBCLK,   //  eFlash, MCU core clock
    CMU_SRP_SRPCLK,
    CMU_GPIO_AHBCLK,
    CMU_UDMAC_AHBCLK,
    CMU_DTRNG_AHBCLK,
    CMU_AES_AHBCLK,
    CMU_SRC_AHBCLK,
    CMU_QSPI_AHBCLK,
    CMU_TIMER0_APBCLK,
    CMU_TIMER1_APBCLK,
    CMU_TIMER2_APBCLK,
    CMU_TIMER3_APBCLK,
    CMU_TIMER4_APBCLK,
    CMU_TIMER5_APBCLK,
    CMU_TIMER6_APBCLK,
    CMU_PWMTIMER_APBCLK,
    CMU_DUALTIMER_APBCLK,
    CMU_WATCHDOG_APBCLK,

    CMU_UART0_CLK,
    CMU_UART1_CLK,
    CMU_UART2_CLK,

    CMU_SPI0_CLK,
    CMU_SPI1_CLK,
    CMU_SPI2_CLK,
    CMU_SPI3_CLK,
    CMU_SPI4_CLK,

    CMU_I2C0_CLK,
    CMU_I2C1_CLK,
    CMU_I2C2_CLK,
    CMU_I2C3_CLK,
    CMU_I2C4_CLK,

    CMU_CLK_INVALID
} cmu_clock_t;

typedef enum {
    CMU_MUX_MCU_CLK = 0,
    CMU_MUX_PERI_CLK,

    CMU_MUX_INVALID
} cmu_mux_peri_clock_t;

void bp6a_cmu_init(cmu_src_clk_t mcu_clk, cmu_src_clk_t peri_clk);
bool bp6a_cmu_get_clock_enabled(cmu_clock_t clock);
int bp6a_cmu_enable_clock(cmu_clock_t clock, bool en);
uint32_t bp6a_get_clock_src_freq(cmu_clock_t clock);
uint32_t bp6a_cmu_get_clock_freq(cmu_clock_t clock);

#endif  /*__S1SBP6A_CMU_H */
