/**
 * @file    gcr_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the GCR Peripheral Module.
 */

/* ****************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
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
 *
 *************************************************************************** */

#ifndef _GCR_REGS_H_
#define _GCR_REGS_H_

/* **** Includes **** */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
 
#if defined (__ICCARM__)
  #pragma system_include
#endif
 
#if defined (__CC_ARM)
  #pragma anon_unions
#endif
/// @cond
/*
    If types are not defined elsewhere (CMSIS) define them here
*/
#ifndef __IO
#define __IO volatile
#endif
#ifndef __I
#define __I  volatile const
#endif
#ifndef __O
#define __O  volatile
#endif

/// @endcond

/* **** Definitions **** */

/**
 * @ingroup     gcr
 * @defgroup    gcr_registers GCR_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the GCR Peripheral Module.
 * @details Global Control Registers.
 */

/**
 * @ingroup gcr_registers
 * Structure type to access the GCR Registers.
 */
typedef struct {
    __IO uint32_t scon;                 /**< <tt>\b 0x00:</tt> GCR SCON Register */
    __IO uint32_t rst0;                 /**< <tt>\b 0x04:</tt> GCR RST0 Register */
    __IO uint32_t clk_ctrl;             /**< <tt>\b 0x08:</tt> GCR CLK_CTRL Register */
    __IO uint32_t pm;                   /**< <tt>\b 0x0C:</tt> GCR PM Register */
    __I  uint32_t rsv_0x10_0x23[5];
    __IO uint32_t pclk_dis0;            /**< <tt>\b 0x24:</tt> GCR PCLK_DIS0 Register */
    __IO uint32_t mem_ctrl;             /**< <tt>\b 0x28:</tt> GCR MEM_CTRL Register */
    __IO uint32_t mem_zctrl;            /**< <tt>\b 0x2C:</tt> GCR MEM_ZCTRL Register */
    __I  uint32_t rsv_0x30_0x3f[4];
    __IO uint32_t sys_stat;             /**< <tt>\b 0x40:</tt> GCR SYS_STAT Register */
    __IO uint32_t rst1;                 /**< <tt>\b 0x44:</tt> GCR RST1 Register */
    __IO uint32_t pclk_dis1;            /**< <tt>\b 0x48:</tt> GCR PCLK_DIS1 Register */
    __IO uint32_t evten;                /**< <tt>\b 0x4C:</tt> GCR EVTEN Register */
    __I  uint32_t rev;                  /**< <tt>\b 0x50:</tt> GCR REV Register */
    __IO uint32_t sys_ie;               /**< <tt>\b 0x54:</tt> GCR SYS_IE Register */
} mxc_gcr_regs_t;

/* Register offsets for module GCR */
/**
 * @ingroup    gcr_registers
 * @defgroup   GCR_Register_Offsets Register Offsets
 * @brief      GCR Peripheral Register Offsets from the GCR Base Peripheral Address. 
 * @{
 */
 #define MXC_R_GCR_SCON                     ((uint32_t)0x00000000UL) /**< Offset from GCR Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_GCR_RST0                     ((uint32_t)0x00000004UL) /**< Offset from GCR Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_GCR_CLK_CTRL                 ((uint32_t)0x00000008UL) /**< Offset from GCR Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_GCR_PM                       ((uint32_t)0x0000000CUL) /**< Offset from GCR Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_GCR_PCLK_DIS0                ((uint32_t)0x00000024UL) /**< Offset from GCR Base Address: <tt> 0x0024</tt> */ 
 #define MXC_R_GCR_MEM_CTRL                 ((uint32_t)0x00000028UL) /**< Offset from GCR Base Address: <tt> 0x0028</tt> */ 
 #define MXC_R_GCR_MEM_ZCTRL                ((uint32_t)0x0000002CUL) /**< Offset from GCR Base Address: <tt> 0x002C</tt> */ 
 #define MXC_R_GCR_SYS_STAT                 ((uint32_t)0x00000040UL) /**< Offset from GCR Base Address: <tt> 0x0040</tt> */ 
 #define MXC_R_GCR_RST1                     ((uint32_t)0x00000044UL) /**< Offset from GCR Base Address: <tt> 0x0044</tt> */ 
 #define MXC_R_GCR_PCLK_DIS1                ((uint32_t)0x00000048UL) /**< Offset from GCR Base Address: <tt> 0x0048</tt> */ 
 #define MXC_R_GCR_EVTEN                    ((uint32_t)0x0000004CUL) /**< Offset from GCR Base Address: <tt> 0x004C</tt> */ 
 #define MXC_R_GCR_REV                      ((uint32_t)0x00000050UL) /**< Offset from GCR Base Address: <tt> 0x0050</tt> */ 
 #define MXC_R_GCR_SYS_IE                   ((uint32_t)0x00000054UL) /**< Offset from GCR Base Address: <tt> 0x0054</tt> */ 
/**@} end of group gcr_registers */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_SCON GCR_SCON
 * @brief    System Control.
 * @{
 */
 #define MXC_F_GCR_SCON_FLASH_PAGE_FLIP_POS             4 /**< SCON_FLASH_PAGE_FLIP Position */
 #define MXC_F_GCR_SCON_FLASH_PAGE_FLIP                 ((uint32_t)(0x1UL << MXC_F_GCR_SCON_FLASH_PAGE_FLIP_POS)) /**< SCON_FLASH_PAGE_FLIP Mask */

 #define MXC_F_GCR_SCON_FPU_DIS_POS                     5 /**< SCON_FPU_DIS Position */
 #define MXC_F_GCR_SCON_FPU_DIS                         ((uint32_t)(0x1UL << MXC_F_GCR_SCON_FPU_DIS_POS)) /**< SCON_FPU_DIS Mask */

 #define MXC_F_GCR_SCON_ICC0_FLUSH_POS                  6 /**< SCON_ICC0_FLUSH Position */
 #define MXC_F_GCR_SCON_ICC0_FLUSH                      ((uint32_t)(0x1UL << MXC_F_GCR_SCON_ICC0_FLUSH_POS)) /**< SCON_ICC0_FLUSH Mask */

 #define MXC_F_GCR_SCON_SWD_DIS_POS                     14 /**< SCON_SWD_DIS Position */
 #define MXC_F_GCR_SCON_SWD_DIS                         ((uint32_t)(0x1UL << MXC_F_GCR_SCON_SWD_DIS_POS)) /**< SCON_SWD_DIS Mask */

/**@} end of group GCR_SCON_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_RST0 GCR_RST0
 * @brief    Reset.
 * @{
 */
 #define MXC_F_GCR_RST0_DMA_POS                         0 /**< RST0_DMA Position */
 #define MXC_F_GCR_RST0_DMA                             ((uint32_t)(0x1UL << MXC_F_GCR_RST0_DMA_POS)) /**< RST0_DMA Mask */

 #define MXC_F_GCR_RST0_WDT0_POS                        1 /**< RST0_WDT0 Position */
 #define MXC_F_GCR_RST0_WDT0                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_WDT0_POS)) /**< RST0_WDT0 Mask */

 #define MXC_F_GCR_RST0_GPIO0_POS                       2 /**< RST0_GPIO0 Position */
 #define MXC_F_GCR_RST0_GPIO0                           ((uint32_t)(0x1UL << MXC_F_GCR_RST0_GPIO0_POS)) /**< RST0_GPIO0 Mask */

 #define MXC_F_GCR_RST0_TIMER0_POS                      5 /**< RST0_TIMER0 Position */
 #define MXC_F_GCR_RST0_TIMER0                          ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TIMER0_POS)) /**< RST0_TIMER0 Mask */

 #define MXC_F_GCR_RST0_TIMER1_POS                      6 /**< RST0_TIMER1 Position */
 #define MXC_F_GCR_RST0_TIMER1                          ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TIMER1_POS)) /**< RST0_TIMER1 Mask */

 #define MXC_F_GCR_RST0_TIMER2_POS                      7 /**< RST0_TIMER2 Position */
 #define MXC_F_GCR_RST0_TIMER2                          ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TIMER2_POS)) /**< RST0_TIMER2 Mask */

 #define MXC_F_GCR_RST0_UART0_POS                       11 /**< RST0_UART0 Position */
 #define MXC_F_GCR_RST0_UART0                           ((uint32_t)(0x1UL << MXC_F_GCR_RST0_UART0_POS)) /**< RST0_UART0 Mask */

 #define MXC_F_GCR_RST0_UART1_POS                       12 /**< RST0_UART1 Position */
 #define MXC_F_GCR_RST0_UART1                           ((uint32_t)(0x1UL << MXC_F_GCR_RST0_UART1_POS)) /**< RST0_UART1 Mask */

 #define MXC_F_GCR_RST0_SPI0_POS                        13 /**< RST0_SPI0 Position */
 #define MXC_F_GCR_RST0_SPI0                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_SPI0_POS)) /**< RST0_SPI0 Mask */

 #define MXC_F_GCR_RST0_SPI1_POS                        14 /**< RST0_SPI1 Position */
 #define MXC_F_GCR_RST0_SPI1                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_SPI1_POS)) /**< RST0_SPI1 Mask */

 #define MXC_F_GCR_RST0_I2C0_POS                        16 /**< RST0_I2C0 Position */
 #define MXC_F_GCR_RST0_I2C0                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_I2C0_POS)) /**< RST0_I2C0 Mask */

 #define MXC_F_GCR_RST0_RTC_POS                         17 /**< RST0_RTC Position */
 #define MXC_F_GCR_RST0_RTC                             ((uint32_t)(0x1UL << MXC_F_GCR_RST0_RTC_POS)) /**< RST0_RTC Mask */

 #define MXC_F_GCR_RST0_SOFT_POS                        29 /**< RST0_SOFT Position */
 #define MXC_F_GCR_RST0_SOFT                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_SOFT_POS)) /**< RST0_SOFT Mask */

 #define MXC_F_GCR_RST0_PERIPH_POS                      30 /**< RST0_PERIPH Position */
 #define MXC_F_GCR_RST0_PERIPH                          ((uint32_t)(0x1UL << MXC_F_GCR_RST0_PERIPH_POS)) /**< RST0_PERIPH Mask */

 #define MXC_F_GCR_RST0_SYSTEM_POS                      31 /**< RST0_SYSTEM Position */
 #define MXC_F_GCR_RST0_SYSTEM                          ((uint32_t)(0x1UL << MXC_F_GCR_RST0_SYSTEM_POS)) /**< RST0_SYSTEM Mask */

/**@} end of group GCR_RST0_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_CLK_CTRL GCR_CLK_CTRL
 * @brief    Clock Control.
 * @{
 */
 #define MXC_F_GCR_CLK_CTRL_PSC_POS                     6 /**< CLK_CTRL_PSC Position */
 #define MXC_F_GCR_CLK_CTRL_PSC                         ((uint32_t)(0x7UL << MXC_F_GCR_CLK_CTRL_PSC_POS)) /**< CLK_CTRL_PSC Mask */
 #define MXC_V_GCR_CLK_CTRL_PSC_DIV1                    ((uint32_t)0x0UL) /**< CLK_CTRL_PSC_DIV1 Value */
 #define MXC_S_GCR_CLK_CTRL_PSC_DIV1                    (MXC_V_GCR_CLK_CTRL_PSC_DIV1 << MXC_F_GCR_CLK_CTRL_PSC_POS) /**< CLK_CTRL_PSC_DIV1 Setting */
 #define MXC_V_GCR_CLK_CTRL_PSC_DIV2                    ((uint32_t)0x1UL) /**< CLK_CTRL_PSC_DIV2 Value */
 #define MXC_S_GCR_CLK_CTRL_PSC_DIV2                    (MXC_V_GCR_CLK_CTRL_PSC_DIV2 << MXC_F_GCR_CLK_CTRL_PSC_POS) /**< CLK_CTRL_PSC_DIV2 Setting */
 #define MXC_V_GCR_CLK_CTRL_PSC_DIV4                    ((uint32_t)0x2UL) /**< CLK_CTRL_PSC_DIV4 Value */
 #define MXC_S_GCR_CLK_CTRL_PSC_DIV4                    (MXC_V_GCR_CLK_CTRL_PSC_DIV4 << MXC_F_GCR_CLK_CTRL_PSC_POS) /**< CLK_CTRL_PSC_DIV4 Setting */
 #define MXC_V_GCR_CLK_CTRL_PSC_DIV8                    ((uint32_t)0x3UL) /**< CLK_CTRL_PSC_DIV8 Value */
 #define MXC_S_GCR_CLK_CTRL_PSC_DIV8                    (MXC_V_GCR_CLK_CTRL_PSC_DIV8 << MXC_F_GCR_CLK_CTRL_PSC_POS) /**< CLK_CTRL_PSC_DIV8 Setting */
 #define MXC_V_GCR_CLK_CTRL_PSC_DIV16                   ((uint32_t)0x4UL) /**< CLK_CTRL_PSC_DIV16 Value */
 #define MXC_S_GCR_CLK_CTRL_PSC_DIV16                   (MXC_V_GCR_CLK_CTRL_PSC_DIV16 << MXC_F_GCR_CLK_CTRL_PSC_POS) /**< CLK_CTRL_PSC_DIV16 Setting */
 #define MXC_V_GCR_CLK_CTRL_PSC_DIV32                   ((uint32_t)0x5UL) /**< CLK_CTRL_PSC_DIV32 Value */
 #define MXC_S_GCR_CLK_CTRL_PSC_DIV32                   (MXC_V_GCR_CLK_CTRL_PSC_DIV32 << MXC_F_GCR_CLK_CTRL_PSC_POS) /**< CLK_CTRL_PSC_DIV32 Setting */
 #define MXC_V_GCR_CLK_CTRL_PSC_DIV64                   ((uint32_t)0x6UL) /**< CLK_CTRL_PSC_DIV64 Value */
 #define MXC_S_GCR_CLK_CTRL_PSC_DIV64                   (MXC_V_GCR_CLK_CTRL_PSC_DIV64 << MXC_F_GCR_CLK_CTRL_PSC_POS) /**< CLK_CTRL_PSC_DIV64 Setting */
 #define MXC_V_GCR_CLK_CTRL_PSC_DIV128                  ((uint32_t)0x7UL) /**< CLK_CTRL_PSC_DIV128 Value */
 #define MXC_S_GCR_CLK_CTRL_PSC_DIV128                  (MXC_V_GCR_CLK_CTRL_PSC_DIV128 << MXC_F_GCR_CLK_CTRL_PSC_POS) /**< CLK_CTRL_PSC_DIV128 Setting */

 #define MXC_F_GCR_CLK_CTRL_CLKSEL_POS                  9 /**< CLK_CTRL_CLKSEL Position */
 #define MXC_F_GCR_CLK_CTRL_CLKSEL                      ((uint32_t)(0x7UL << MXC_F_GCR_CLK_CTRL_CLKSEL_POS)) /**< CLK_CTRL_CLKSEL Mask */
 #define MXC_V_GCR_CLK_CTRL_CLKSEL_HIRC                 ((uint32_t)0x0UL) /**< CLK_CTRL_CLKSEL_HIRC Value */
 #define MXC_S_GCR_CLK_CTRL_CLKSEL_HIRC                 (MXC_V_GCR_CLK_CTRL_CLKSEL_HIRC << MXC_F_GCR_CLK_CTRL_CLKSEL_POS) /**< CLK_CTRL_CLKSEL_HIRC Setting */
 #define MXC_V_GCR_CLK_CTRL_CLKSEL_NANORING             ((uint32_t)0x3UL) /**< CLK_CTRL_CLKSEL_NANORING Value */
 #define MXC_S_GCR_CLK_CTRL_CLKSEL_NANORING             (MXC_V_GCR_CLK_CTRL_CLKSEL_NANORING << MXC_F_GCR_CLK_CTRL_CLKSEL_POS) /**< CLK_CTRL_CLKSEL_NANORING Setting */
 #define MXC_V_GCR_CLK_CTRL_CLKSEL_HFXIN                ((uint32_t)0x6UL) /**< CLK_CTRL_CLKSEL_HFXIN Value */
 #define MXC_S_GCR_CLK_CTRL_CLKSEL_HFXIN                (MXC_V_GCR_CLK_CTRL_CLKSEL_HFXIN << MXC_F_GCR_CLK_CTRL_CLKSEL_POS) /**< CLK_CTRL_CLKSEL_HFXIN Setting */

 #define MXC_F_GCR_CLK_CTRL_CLKRDY_POS                  13 /**< CLK_CTRL_CLKRDY Position */
 #define MXC_F_GCR_CLK_CTRL_CLKRDY                      ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_CLKRDY_POS)) /**< CLK_CTRL_CLKRDY Mask */

 #define MXC_F_GCR_CLK_CTRL_X32K_EN_POS                 17 /**< CLK_CTRL_X32K_EN Position */
 #define MXC_F_GCR_CLK_CTRL_X32K_EN                     ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_X32K_EN_POS)) /**< CLK_CTRL_X32K_EN Mask */

 #define MXC_F_GCR_CLK_CTRL_HIRC_EN_POS                 18 /**< CLK_CTRL_HIRC_EN Position */
 #define MXC_F_GCR_CLK_CTRL_HIRC_EN                     ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_HIRC_EN_POS)) /**< CLK_CTRL_HIRC_EN Mask */

 #define MXC_F_GCR_CLK_CTRL_X32K_RDY_POS                25 /**< CLK_CTRL_X32K_RDY Position */
 #define MXC_F_GCR_CLK_CTRL_X32K_RDY                    ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_X32K_RDY_POS)) /**< CLK_CTRL_X32K_RDY Mask */

 #define MXC_F_GCR_CLK_CTRL_HIRC_RDY_POS                26 /**< CLK_CTRL_HIRC_RDY Position */
 #define MXC_F_GCR_CLK_CTRL_HIRC_RDY                    ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_HIRC_RDY_POS)) /**< CLK_CTRL_HIRC_RDY Mask */

 #define MXC_F_GCR_CLK_CTRL_LIRC8K_RDY_POS              29 /**< CLK_CTRL_LIRC8K_RDY Position */
 #define MXC_F_GCR_CLK_CTRL_LIRC8K_RDY                  ((uint32_t)(0x1UL << MXC_F_GCR_CLK_CTRL_LIRC8K_RDY_POS)) /**< CLK_CTRL_LIRC8K_RDY Mask */

/**@} end of group GCR_CLK_CTRL_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_PM GCR_PM
 * @brief    Power Management.
 * @{
 */
 #define MXC_F_GCR_PM_MODE_POS                          0 /**< PM_MODE Position */
 #define MXC_F_GCR_PM_MODE                              ((uint32_t)(0x7UL << MXC_F_GCR_PM_MODE_POS)) /**< PM_MODE Mask */
 #define MXC_V_GCR_PM_MODE_ACTIVE                       ((uint32_t)0x0UL) /**< PM_MODE_ACTIVE Value */
 #define MXC_S_GCR_PM_MODE_ACTIVE                       (MXC_V_GCR_PM_MODE_ACTIVE << MXC_F_GCR_PM_MODE_POS) /**< PM_MODE_ACTIVE Setting */
 #define MXC_V_GCR_PM_MODE_SHUTDOWN                     ((uint32_t)0x3UL) /**< PM_MODE_SHUTDOWN Value */
 #define MXC_S_GCR_PM_MODE_SHUTDOWN                     (MXC_V_GCR_PM_MODE_SHUTDOWN << MXC_F_GCR_PM_MODE_POS) /**< PM_MODE_SHUTDOWN Setting */
 #define MXC_V_GCR_PM_MODE_BACKUP                       ((uint32_t)0x4UL) /**< PM_MODE_BACKUP Value */
 #define MXC_S_GCR_PM_MODE_BACKUP                       (MXC_V_GCR_PM_MODE_BACKUP << MXC_F_GCR_PM_MODE_POS) /**< PM_MODE_BACKUP Setting */

 #define MXC_F_GCR_PM_GPIOWK_EN_POS                     4 /**< PM_GPIOWK_EN Position */
 #define MXC_F_GCR_PM_GPIOWK_EN                         ((uint32_t)(0x1UL << MXC_F_GCR_PM_GPIOWK_EN_POS)) /**< PM_GPIOWK_EN Mask */

 #define MXC_F_GCR_PM_RTCWK_EN_POS                      5 /**< PM_RTCWK_EN Position */
 #define MXC_F_GCR_PM_RTCWK_EN                          ((uint32_t)(0x1UL << MXC_F_GCR_PM_RTCWK_EN_POS)) /**< PM_RTCWK_EN Mask */

 #define MXC_F_GCR_PM_HFIOPD_POS                        15 /**< PM_HFIOPD Position */
 #define MXC_F_GCR_PM_HFIOPD                            ((uint32_t)(0x1UL << MXC_F_GCR_PM_HFIOPD_POS)) /**< PM_HFIOPD Mask */

/**@} end of group GCR_PM_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_PCLK_DIS0 GCR_PCLK_DIS0
 * @brief    Peripheral Clock Disable.
 * @{
 */
 #define MXC_F_GCR_PCLK_DIS0_GPIO0D_POS                 0 /**< PCLK_DIS0_GPIO0D Position */
 #define MXC_F_GCR_PCLK_DIS0_GPIO0D                     ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_GPIO0D_POS)) /**< PCLK_DIS0_GPIO0D Mask */

 #define MXC_F_GCR_PCLK_DIS0_DMAD_POS                   5 /**< PCLK_DIS0_DMAD Position */
 #define MXC_F_GCR_PCLK_DIS0_DMAD                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_DMAD_POS)) /**< PCLK_DIS0_DMAD Mask */

 #define MXC_F_GCR_PCLK_DIS0_SPI0D_POS                  6 /**< PCLK_DIS0_SPI0D Position */
 #define MXC_F_GCR_PCLK_DIS0_SPI0D                      ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_SPI0D_POS)) /**< PCLK_DIS0_SPI0D Mask */

 #define MXC_F_GCR_PCLK_DIS0_SPI1D_POS                  7 /**< PCLK_DIS0_SPI1D Position */
 #define MXC_F_GCR_PCLK_DIS0_SPI1D                      ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_SPI1D_POS)) /**< PCLK_DIS0_SPI1D Mask */

 #define MXC_F_GCR_PCLK_DIS0_UART0D_POS                 9 /**< PCLK_DIS0_UART0D Position */
 #define MXC_F_GCR_PCLK_DIS0_UART0D                     ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_UART0D_POS)) /**< PCLK_DIS0_UART0D Mask */

 #define MXC_F_GCR_PCLK_DIS0_UART1D_POS                 10 /**< PCLK_DIS0_UART1D Position */
 #define MXC_F_GCR_PCLK_DIS0_UART1D                     ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_UART1D_POS)) /**< PCLK_DIS0_UART1D Mask */

 #define MXC_F_GCR_PCLK_DIS0_I2C0D_POS                  13 /**< PCLK_DIS0_I2C0D Position */
 #define MXC_F_GCR_PCLK_DIS0_I2C0D                      ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_I2C0D_POS)) /**< PCLK_DIS0_I2C0D Mask */

 #define MXC_F_GCR_PCLK_DIS0_TIMER0D_POS                15 /**< PCLK_DIS0_TIMER0D Position */
 #define MXC_F_GCR_PCLK_DIS0_TIMER0D                    ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_TIMER0D_POS)) /**< PCLK_DIS0_TIMER0D Mask */

 #define MXC_F_GCR_PCLK_DIS0_TIMER1D_POS                16 /**< PCLK_DIS0_TIMER1D Position */
 #define MXC_F_GCR_PCLK_DIS0_TIMER1D                    ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_TIMER1D_POS)) /**< PCLK_DIS0_TIMER1D Mask */

 #define MXC_F_GCR_PCLK_DIS0_TIMER2D_POS                17 /**< PCLK_DIS0_TIMER2D Position */
 #define MXC_F_GCR_PCLK_DIS0_TIMER2D                    ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_TIMER2D_POS)) /**< PCLK_DIS0_TIMER2D Mask */

 #define MXC_F_GCR_PCLK_DIS0_I2C1D_POS                  28 /**< PCLK_DIS0_I2C1D Position */
 #define MXC_F_GCR_PCLK_DIS0_I2C1D                      ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS0_I2C1D_POS)) /**< PCLK_DIS0_I2C1D Mask */

/**@} end of group GCR_PCLK_DIS0_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_MEM_CTRL GCR_MEM_CTRL
 * @brief    Memory Clock Control Register.
 * @{
 */
 #define MXC_F_GCR_MEM_CTRL_FWS_POS                     0 /**< MEM_CTRL_FWS Position */
 #define MXC_F_GCR_MEM_CTRL_FWS                         ((uint32_t)(0x7UL << MXC_F_GCR_MEM_CTRL_FWS_POS)) /**< MEM_CTRL_FWS Mask */

 #define MXC_F_GCR_MEM_CTRL_RAM0_LS_POS                 8 /**< MEM_CTRL_RAM0_LS Position */
 #define MXC_F_GCR_MEM_CTRL_RAM0_LS                     ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CTRL_RAM0_LS_POS)) /**< MEM_CTRL_RAM0_LS Mask */

 #define MXC_F_GCR_MEM_CTRL_RAM1_LS_POS                 9 /**< MEM_CTRL_RAM1_LS Position */
 #define MXC_F_GCR_MEM_CTRL_RAM1_LS                     ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CTRL_RAM1_LS_POS)) /**< MEM_CTRL_RAM1_LS Mask */

 #define MXC_F_GCR_MEM_CTRL_RAM2_LS_POS                 10 /**< MEM_CTRL_RAM2_LS Position */
 #define MXC_F_GCR_MEM_CTRL_RAM2_LS                     ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CTRL_RAM2_LS_POS)) /**< MEM_CTRL_RAM2_LS Mask */

 #define MXC_F_GCR_MEM_CTRL_RAM3_LS_POS                 11 /**< MEM_CTRL_RAM3_LS Position */
 #define MXC_F_GCR_MEM_CTRL_RAM3_LS                     ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CTRL_RAM3_LS_POS)) /**< MEM_CTRL_RAM3_LS Mask */

 #define MXC_F_GCR_MEM_CTRL_ICACHE_RET_POS              12 /**< MEM_CTRL_ICACHE_RET Position */
 #define MXC_F_GCR_MEM_CTRL_ICACHE_RET                  ((uint32_t)(0x1UL << MXC_F_GCR_MEM_CTRL_ICACHE_RET_POS)) /**< MEM_CTRL_ICACHE_RET Mask */

/**@} end of group GCR_MEM_CTRL_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_MEM_ZCTRL GCR_MEM_ZCTRL
 * @brief    Memory Zeroize Control.
 * @{
 */
 #define MXC_F_GCR_MEM_ZCTRL_SRAM_ZERO_POS              0 /**< MEM_ZCTRL_SRAM_ZERO Position */
 #define MXC_F_GCR_MEM_ZCTRL_SRAM_ZERO                  ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZCTRL_SRAM_ZERO_POS)) /**< MEM_ZCTRL_SRAM_ZERO Mask */

 #define MXC_F_GCR_MEM_ZCTRL_ICACHE_ZERO_POS            1 /**< MEM_ZCTRL_ICACHE_ZERO Position */
 #define MXC_F_GCR_MEM_ZCTRL_ICACHE_ZERO                ((uint32_t)(0x1UL << MXC_F_GCR_MEM_ZCTRL_ICACHE_ZERO_POS)) /**< MEM_ZCTRL_ICACHE_ZERO Mask */

/**@} end of group GCR_MEM_ZCTRL_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_SYS_STAT GCR_SYS_STAT
 * @brief    System Status Register.
 * @{
 */
 #define MXC_F_GCR_SYS_STAT_ICECLOCK_POS                0 /**< SYS_STAT_ICECLOCK Position */
 #define MXC_F_GCR_SYS_STAT_ICECLOCK                    ((uint32_t)(0x1UL << MXC_F_GCR_SYS_STAT_ICECLOCK_POS)) /**< SYS_STAT_ICECLOCK Mask */

/**@} end of group GCR_SYS_STAT_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_RST1 GCR_RST1
 * @brief    Reset 1.
 * @{
 */
 #define MXC_F_GCR_RST1_I2C1_POS                        0 /**< RST1_I2C1 Position */
 #define MXC_F_GCR_RST1_I2C1                            ((uint32_t)(0x1UL << MXC_F_GCR_RST1_I2C1_POS)) /**< RST1_I2C1 Mask */

/**@} end of group GCR_RST1_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_PCLK_DIS1 GCR_PCLK_DIS1
 * @brief    Peripheral Clock Disable.
 * @{
 */
 #define MXC_F_GCR_PCLK_DIS1_FLCD_POS                   3 /**< PCLK_DIS1_FLCD Position */
 #define MXC_F_GCR_PCLK_DIS1_FLCD                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_FLCD_POS)) /**< PCLK_DIS1_FLCD Mask */

 #define MXC_F_GCR_PCLK_DIS1_ICCD_POS                   11 /**< PCLK_DIS1_ICCD Position */
 #define MXC_F_GCR_PCLK_DIS1_ICCD                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLK_DIS1_ICCD_POS)) /**< PCLK_DIS1_ICCD Mask */

/**@} end of group GCR_PCLK_DIS1_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_EVTEN GCR_EVTEN
 * @brief    Event Enable Register.
 * @{
 */
 #define MXC_F_GCR_EVTEN_DMAEVENT_POS                   0 /**< EVTEN_DMAEVENT Position */
 #define MXC_F_GCR_EVTEN_DMAEVENT                       ((uint32_t)(0x1UL << MXC_F_GCR_EVTEN_DMAEVENT_POS)) /**< EVTEN_DMAEVENT Mask */

 #define MXC_F_GCR_EVTEN_RX_EVT_POS                     1 /**< EVTEN_RX_EVT Position */
 #define MXC_F_GCR_EVTEN_RX_EVT                         ((uint32_t)(0x1UL << MXC_F_GCR_EVTEN_RX_EVT_POS)) /**< EVTEN_RX_EVT Mask */

/**@} end of group GCR_EVTEN_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_REV GCR_REV
 * @brief    Revision Register.
 * @{
 */
 #define MXC_F_GCR_REV_REVISION_POS                     0 /**< REV_REVISION Position */
 #define MXC_F_GCR_REV_REVISION                         ((uint32_t)(0xFFFFUL << MXC_F_GCR_REV_REVISION_POS)) /**< REV_REVISION Mask */

/**@} end of group GCR_REV_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_SYS_IE GCR_SYS_IE
 * @brief    System Status Interrupt Enable
 * @{
 */
 #define MXC_F_GCR_SYS_IE_ICEULIE_POS                   0 /**< SYS_IE_ICEULIE Position */
 #define MXC_F_GCR_SYS_IE_ICEULIE                       ((uint32_t)(0x1UL << MXC_F_GCR_SYS_IE_ICEULIE_POS)) /**< SYS_IE_ICEULIE Mask */

/**@} end of group GCR_SYS_IE_Register */

#ifdef __cplusplus
}
#endif

#endif /* _GCR_REGS_H_ */
