/**
 * @file    mxc_sys.h
 * @brief   System level header file.
 */

/*******************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 ******************************************************************************/

#ifndef _MXC_MXC_SYS_H_
#define _MXC_MXC_SYS_H_

#include "mxc_device.h"
#include "gcr_regs.h"
#include "mcr_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief System reset0 and reset1 enumeration. Used in MXC_SYS_PeriphReset0 function */
typedef enum {
    MXC_SYS_RESET0_DMA       = MXC_F_GCR_RST0_DMA_POS,             /**< Reset DMA */
    MXC_SYS_RESET0_WDT0      = MXC_F_GCR_RST0_WDT0_POS,            /**< Reset WDT */
    MXC_SYS_RESET0_GPIO0     = MXC_F_GCR_RST0_GPIO0_POS,           /**< Reset GPIO0 */
    MXC_SYS_RESET0_GPIO1     = MXC_F_GCR_RST0_GPIO1_POS,           /**< Reset GPIO1 */
    MXC_SYS_RESET0_TMR0      = MXC_F_GCR_RST0_TMR0_POS,            /**< Reset TIMER0 */
    MXC_SYS_RESET0_TMR1      = MXC_F_GCR_RST0_TMR1_POS,            /**< Reset TIMER1 */
    MXC_SYS_RESET0_TMR2      = MXC_F_GCR_RST0_TMR2_POS,            /**< Reset TIMER2 */
    MXC_SYS_RESET0_TMR3      = MXC_F_GCR_RST0_TMR3_POS,            /**< Reset TIMER3 */
    MXC_SYS_RESET0_UART0     = MXC_F_GCR_RST0_UART0_POS,           /**< Reset UART0 */
    MXC_SYS_RESET0_UART1     = MXC_F_GCR_RST0_UART1_POS,           /**< Reset UART1 */
    MXC_SYS_RESET0_SPI0      = MXC_F_GCR_RST0_SPI0_POS,            /**< Reset SPI0 */
    MXC_SYS_RESET0_SPI1      = MXC_F_GCR_RST0_SPI1_POS,            /**< Reset SPI1 */
    MXC_SYS_RESET0_SPI2      = MXC_F_GCR_RST0_SPI2_POS,            /**< Reset SPI2 */
    MXC_SYS_RESET0_I2C0      = MXC_F_GCR_RST0_I2C0_POS,            /**< Reset I2C0 */
    MXC_SYS_RESET0_RTC       = MXC_F_GCR_RST0_RTC_POS,             /**< Reset RTC */
    MXC_SYS_RESET0_TRNG      = MXC_F_GCR_RST0_TRNG_POS,            /**< Reset TRNG */
    MXC_SYS_RESET0_UART2     = MXC_F_GCR_RST0_UART2_POS,           /**< Reset UART2 */
    MXC_SYS_RESET0_SRST      = MXC_F_GCR_RST0_SOFT_POS,            /**< Soft reset */
    MXC_SYS_RESET0_PRST      = MXC_F_GCR_RST0_PERIPH_POS,          /**< Peripheral reset */
    MXC_SYS_RESET0_SYS       = MXC_F_GCR_RST0_SYS_POS,             /**< System reset */
    /* RESET1 Below this line we add 32 to separate RESET0 and RESET1 */
    MXC_SYS_RESET1_I2C1      = (MXC_F_GCR_RST1_I2C1_POS + 32),     /**< Reset I2C1 */
    MXC_SYS_RESET1_WDT1      = (MXC_F_GCR_RST1_WDT1_POS + 32),     /**< Reset WDT1 */
    MXC_SYS_RESET1_AES       = (MXC_F_GCR_RST1_AES_POS + 32),      /**< Reset WDT1 */
    MXC_SYS_RESET1_CRC       = (MXC_F_GCR_RST1_CRC_POS + 32),      /**< Reset WDT1 */
    MXC_SYS_RESET1_I2C2      = (MXC_F_GCR_RST1_I2C2_POS + 32),     /**< Reset */
    MXC_SYS_RESET1_I2S       = (MXC_F_GCR_RST1_I2S_POS + 32),      /**< Reset */
    /* LPGCR RESET Below this line we add 64 to separate LPGCR and GCR */
    MXC_SYS_RESET_TMR4       = (MXC_F_MCR_RST_LPTMR0_POS + 64),        /**< Reset TMR4 */
    MXC_SYS_RESET_TMR5       = (MXC_F_MCR_RST_LPTMR1_POS + 64),        /**< Reset TMR5 */
    MXC_SYS_RESET_UART3      = (MXC_F_MCR_RST_LPUART0_POS + 64),       /**< Reset UART3 */
} mxc_sys_reset_t;

/** @brief System clock disable enumeration. Used in MXC_SYS_ClockDisable and MXC_SYS_ClockEnable functions */
typedef enum {
    MXC_SYS_PERIPH_CLOCK_GPIO0       = MXC_F_GCR_PCLKDIS0_GPIO0_POS,                    /**< Disable MXC_F_GCR_PCLKDIS0_GPIO0 clock */
    MXC_SYS_PERIPH_CLOCK_GPIO1       = MXC_F_GCR_PCLKDIS0_GPIO1_POS,                    /**< Disable MXC_F_GCR_PCLKDIS0_GPIO1 clock */
    MXC_SYS_PERIPH_CLOCK_DMA         = MXC_F_GCR_PCLKDIS0_DMA_POS,                      /**< Disable MXC_F_GCR_PCLKDIS0_DMA clock */
    MXC_SYS_PERIPH_CLOCK_SPI0        = MXC_F_GCR_PCLKDIS0_SPI0_POS,                     /**< Disable MXC_F_GCR_PCLKDIS0_SPI0 clock */
    MXC_SYS_PERIPH_CLOCK_SPI1        = MXC_F_GCR_PCLKDIS0_SPI1_POS,                     /**< Disable MXC_F_GCR_PCLKDIS0_SPI1 clock */
    MXC_SYS_PERIPH_CLOCK_SPI2        = MXC_F_GCR_PCLKDIS0_SPI2_POS,                     /**< Disable MXC_F_GCR_PCLKDIS0_SPI2 clock */
    MXC_SYS_PERIPH_CLOCK_UART0       = MXC_F_GCR_PCLKDIS0_UART0_POS,                    /**< Disable MXC_F_GCR_PCLKDIS0_UART0 clock */
    MXC_SYS_PERIPH_CLOCK_UART1       = MXC_F_GCR_PCLKDIS0_UART1_POS,                    /**< Disable MXC_F_GCR_PCLKDIS0_UART1 clock */
    MXC_SYS_PERIPH_CLOCK_I2C0        = MXC_F_GCR_PCLKDIS0_I2C0_POS,                     /**< Disable MXC_F_GCR_PCLKDIS0_I2C0 clock */
    MXC_SYS_PERIPH_CLOCK_TMR0        = MXC_F_GCR_PCLKDIS0_TMR0_POS,                     /**< Disable MXC_F_GCR_PCLKDIS0_T0 clock */
    MXC_SYS_PERIPH_CLOCK_TMR1        = MXC_F_GCR_PCLKDIS0_TMR1_POS,                     /**< Disable MXC_F_GCR_PCLKDIS0_T1 clock */
    MXC_SYS_PERIPH_CLOCK_TMR2        = MXC_F_GCR_PCLKDIS0_TMR2_POS,                     /**< Disable MXC_F_GCR_PCLKDIS0_T2 clock */
    MXC_SYS_PERIPH_CLOCK_TMR3        = MXC_F_GCR_PCLKDIS0_TMR3_POS,                     /**< Disable MXC_F_GCR_PCLKDIS0_T3 clock */
    MXC_SYS_PERIPH_CLOCK_I2C1        = MXC_F_GCR_PCLKDIS0_I2C1_POS,                     /**< Disable MXC_F_GCR_PCLKDIS0_I2C1 clock */
    /* PCLKDIS1 Below this line we add 32 to separate PCLKDIS0 and PCLKDIS1 */
    MXC_SYS_PERIPH_CLOCK_UART2       = (MXC_F_GCR_PCLKDIS1_UART2_POS + 32),             /**< Disable MXC_F_GCR_PCLKDIS1_UART2 clock */
    MXC_SYS_PERIPH_CLOCK_TRNG        = (MXC_F_GCR_PCLKDIS1_TRNG_POS + 32),              /**< Disable MXC_F_GCR_PCLKDIS1_TRNG clock */
    MXC_SYS_PERIPH_CLOCK_WDT0        = (MXC_F_GCR_PCLKDIS1_WWDT0_POS + 32),             /**< Disable MXC_F_GCR_PCLKDIS1_WDT0 clock */
    MXC_SYS_PERIPH_CLOCK_WDT1        = (MXC_F_GCR_PCLKDIS1_WWDT1_POS + 32),             /**< Disable MXC_F_GCR_PCLKDIS1_WDT1 clock */
    MXC_SYS_PERIPH_CLOCK_ICACHE      = (MXC_F_GCR_PCLKDIS1_ICC0_POS + 32),              /**< Disable MXC_F_GCR_PCLKDIS1_ICACHE clock */
    MXC_SYS_PERIPH_CLOCK_CRC         = (MXC_F_GCR_PCLKDIS1_CRC_POS + 32),               /**< Disable MXC_F_GCR_PCLKDIS1_CRC clock */
    MXC_SYS_PERIPH_CLOCK_AES         = (MXC_F_GCR_PCLKDIS1_AES_POS + 32),               /**< Disable MXC_F_GCR_PCLKDIS1_AES clock */
    MXC_SYS_PERIPH_CLOCK_I2C2        = (MXC_F_GCR_PCLKDIS1_I2C2_POS + 32),              /**< Disable MXC_F_GCR_PCLKDIS1_I2C2 clock */
    MXC_SYS_PERIPH_CLOCK_I2S         = (MXC_F_GCR_PCLKDIS1_I2S_POS + 32),               /**< Disable MXC_F_GCR_PCLKDIS1_I2S clock */
    /* LPGCR PCLKDIS Below this line we add 64 to seperate GCR and LPGCR registers */
    MXC_SYS_PERIPH_CLOCK_TMR4        = (MXC_F_MCR_CLKDIS_LPTMR0_POS + 64),                  /**< Disable MXC_F_LPGCR_PCLKDIS_TMR4 clock */
    MXC_SYS_PERIPH_CLOCK_TMR5        = (MXC_F_MCR_CLKDIS_LPTMR1_POS + 64),                  /**< Disable MXC_F_LPGCR_PCLKDIS_TMR5 clock */
    MXC_SYS_PERIPH_CLOCK_UART3       = (MXC_F_MCR_CLKDIS_LPUART0_POS + 64),                 /**< Disable MXC_F_LPGCR_PCLKDIS_UART3 clock */
} mxc_sys_periph_clock_t;

/** @brief Enumeration to select System Clock source */
typedef enum {
    MXC_SYS_CLOCK_IPO     = MXC_V_GCR_CLKCTRL_SYSCLK_SEL_IPO,
    MXC_SYS_CLOCK_IBRO    = MXC_V_GCR_CLKCTRL_SYSCLK_SEL_IBRO,
    MXC_SYS_CLOCK_ERFO    = MXC_V_GCR_CLKCTRL_SYSCLK_SEL_ERFO,
    MXC_SYS_CLOCK_INRO    = MXC_V_GCR_CLKCTRL_SYSCLK_SEL_INRO,
    MXC_SYS_CLOCK_ERTCO   = MXC_V_GCR_CLKCTRL_SYSCLK_SEL_ERTCO,
    MXC_SYS_CLOCK_EXTCLK  = MXC_V_GCR_CLKCTRL_SYSCLK_SEL_EXTCLK
} mxc_sys_system_clock_t;

#define MXC_SYS_USN_CHECKSUM_LEN    16

/***** Function Prototypes *****/

/**
 * @brief Reads the device USN.
 * @param usn       Pointer to store the USN.
 * @param checksum  Optional pointer to store the AES checksum.
 * @returns       E_NO_ERROR if everything is successful.
 */
int MXC_SYS_GetUSN(uint8_t *usn, uint8_t *checksum);

/**
 * @brief Determines if the selected peripheral clock is enabled.
 * @param clock   Enumeration for desired clock.
 * @returns       0 is the clock is disabled, non 0 if the clock is enabled.
 */
int MXC_SYS_IsClockEnabled (mxc_sys_periph_clock_t clock);

/**
 * @brief Disables the selected peripheral clock.
 * @param clock   Enumeration for desired clock.
 */
void MXC_SYS_ClockDisable (mxc_sys_periph_clock_t clock);

/**
 * @brief Enables the selected peripheral clock.
 * @param clock   Enumeration for desired clock.
 */
void MXC_SYS_ClockEnable (mxc_sys_periph_clock_t clock);

/**
 * @brief Enables the 32kHz oscillator
 * @param mxc_sys_cfg   Not used, may be NULL.
 */
void MXC_SYS_RTCClockEnable (void);

/**
 * @brief Disables the 32kHz oscillator
 * @returns         E_NO_ERROR if everything is successful
 */
int MXC_SYS_RTCClockDisable();

/**
 * @brief Enable System Clock Source without switching to it
 * @param      clock The clock to enable
 * @return     E_NO_ERROR if everything is successful
 */
int MXC_SYS_ClockSourceEnable (mxc_sys_system_clock_t clock);

/**
 * @brief Disable System Clock Source
 * @param      clock The clock to disable
 * @return     E_NO_ERROR if everything is successful
 */
int MXC_SYS_ClockSourceDisable (mxc_sys_system_clock_t clock);

/**
 * @brief Select the system clock.
 * @param clock     Enumeration for desired clock.
 * @param tmr       Optional tmr pointer for timeout. NULL if undesired.
 * @returns         E_NO_ERROR if everything is successful.
 */
int MXC_SYS_Clock_Select (mxc_sys_system_clock_t clock);

/**
 * @brief Wait for a clock to enable with timeout
 * @param      ready The clock to wait for
 * @return     E_NO_ERROR if ready, E_TIME_OUT if timeout
 */
int MXC_SYS_Clock_Timeout (uint32_t ready);
/**
 * @brief Reset the peripherals and/or CPU in the rstr0 or rstr1 register.
 * @param           Enumeration for what to reset. Can reset multiple items at once.
 */
void MXC_SYS_Reset_Periph (mxc_sys_reset_t reset);

#ifdef __cplusplus
}
#endif

#endif /* _MXC_MXC_SYS_H_*/
