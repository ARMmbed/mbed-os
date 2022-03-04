/**
 * @file    gcr_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the GCR Peripheral Module.
 */

/* ****************************************************************************
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
    __IO uint32_t sysctrl;              /**< <tt>\b 0x00:</tt> GCR SYSCTRL Register */
    __IO uint32_t rst0;                 /**< <tt>\b 0x04:</tt> GCR RST0 Register */
    __IO uint32_t clkctrl;              /**< <tt>\b 0x08:</tt> GCR CLKCTRL Register */
    __IO uint32_t pm;                   /**< <tt>\b 0x0C:</tt> GCR PM Register */
    __I  uint32_t rsv_0x10_0x17[2];
    __IO uint32_t pclkdiv;              /**< <tt>\b 0x18:</tt> GCR PCLKDIV Register */
    __I  uint32_t rsv_0x1c_0x23[2];
    __IO uint32_t pclkdis0;             /**< <tt>\b 0x24:</tt> GCR PCLKDIS0 Register */
    __IO uint32_t memctrl;              /**< <tt>\b 0x28:</tt> GCR MEMCTRL Register */
    __IO uint32_t memz;                 /**< <tt>\b 0x2C:</tt> GCR MEMZ Register */
    __I  uint32_t rsv_0x30_0x3f[4];
    __IO uint32_t sysst;                /**< <tt>\b 0x40:</tt> GCR SYSST Register */
    __IO uint32_t rst1;                 /**< <tt>\b 0x44:</tt> GCR RST1 Register */
    __IO uint32_t pclkdis1;             /**< <tt>\b 0x48:</tt> GCR PCLKDIS1 Register */
    __IO uint32_t eventen;              /**< <tt>\b 0x4C:</tt> GCR EVENTEN Register */
    __I  uint32_t revision;             /**< <tt>\b 0x50:</tt> GCR REVISION Register */
    __IO uint32_t sysie;                /**< <tt>\b 0x54:</tt> GCR SYSIE Register */
    __I  uint32_t rsv_0x58_0x63[3];
    __IO uint32_t eccerr;               /**< <tt>\b 0x64:</tt> GCR ECCERR Register */
    __IO uint32_t eccced;               /**< <tt>\b 0x68:</tt> GCR ECCCED Register */
    __IO uint32_t eccie;                /**< <tt>\b 0x6C:</tt> GCR ECCIE Register */
    __IO uint32_t eccaddr;              /**< <tt>\b 0x70:</tt> GCR ECCADDR Register */
} mxc_gcr_regs_t;

/* Register offsets for module GCR */
/**
 * @ingroup    gcr_registers
 * @defgroup   GCR_Register_Offsets Register Offsets
 * @brief      GCR Peripheral Register Offsets from the GCR Base Peripheral Address. 
 * @{
 */
 #define MXC_R_GCR_SYSCTRL                  ((uint32_t)0x00000000UL) /**< Offset from GCR Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_GCR_RST0                     ((uint32_t)0x00000004UL) /**< Offset from GCR Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_GCR_CLKCTRL                  ((uint32_t)0x00000008UL) /**< Offset from GCR Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_GCR_PM                       ((uint32_t)0x0000000CUL) /**< Offset from GCR Base Address: <tt> 0x000C</tt> */ 
 #define MXC_R_GCR_PCLKDIV                  ((uint32_t)0x00000018UL) /**< Offset from GCR Base Address: <tt> 0x0018</tt> */ 
 #define MXC_R_GCR_PCLKDIS0                 ((uint32_t)0x00000024UL) /**< Offset from GCR Base Address: <tt> 0x0024</tt> */ 
 #define MXC_R_GCR_MEMCTRL                  ((uint32_t)0x00000028UL) /**< Offset from GCR Base Address: <tt> 0x0028</tt> */ 
 #define MXC_R_GCR_MEMZ                     ((uint32_t)0x0000002CUL) /**< Offset from GCR Base Address: <tt> 0x002C</tt> */ 
 #define MXC_R_GCR_SYSST                    ((uint32_t)0x00000040UL) /**< Offset from GCR Base Address: <tt> 0x0040</tt> */ 
 #define MXC_R_GCR_RST1                     ((uint32_t)0x00000044UL) /**< Offset from GCR Base Address: <tt> 0x0044</tt> */ 
 #define MXC_R_GCR_PCLKDIS1                 ((uint32_t)0x00000048UL) /**< Offset from GCR Base Address: <tt> 0x0048</tt> */ 
 #define MXC_R_GCR_EVENTEN                  ((uint32_t)0x0000004CUL) /**< Offset from GCR Base Address: <tt> 0x004C</tt> */ 
 #define MXC_R_GCR_REVISION                 ((uint32_t)0x00000050UL) /**< Offset from GCR Base Address: <tt> 0x0050</tt> */ 
 #define MXC_R_GCR_SYSIE                    ((uint32_t)0x00000054UL) /**< Offset from GCR Base Address: <tt> 0x0054</tt> */ 
 #define MXC_R_GCR_ECCERR                   ((uint32_t)0x00000064UL) /**< Offset from GCR Base Address: <tt> 0x0064</tt> */ 
 #define MXC_R_GCR_ECCCED                   ((uint32_t)0x00000068UL) /**< Offset from GCR Base Address: <tt> 0x0068</tt> */ 
 #define MXC_R_GCR_ECCIE                    ((uint32_t)0x0000006CUL) /**< Offset from GCR Base Address: <tt> 0x006C</tt> */ 
 #define MXC_R_GCR_ECCADDR                  ((uint32_t)0x00000070UL) /**< Offset from GCR Base Address: <tt> 0x0070</tt> */ 
/**@} end of group gcr_registers */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_SYSCTRL GCR_SYSCTRL
 * @brief    System Control.
 * @{
 */
 #define MXC_F_GCR_SYSCTRL_SBUSARB_POS                  1 /**< SYSCTRL_SBUSARB Position */
 #define MXC_F_GCR_SYSCTRL_SBUSARB                      ((uint32_t)(0x3UL << MXC_F_GCR_SYSCTRL_SBUSARB_POS)) /**< SYSCTRL_SBUSARB Mask */
 #define MXC_V_GCR_SYSCTRL_SBUSARB_FIX                  ((uint32_t)0x0UL) /**< SYSCTRL_SBUSARB_FIX Value */
 #define MXC_S_GCR_SYSCTRL_SBUSARB_FIX                  (MXC_V_GCR_SYSCTRL_SBUSARB_FIX << MXC_F_GCR_SYSCTRL_SBUSARB_POS) /**< SYSCTRL_SBUSARB_FIX Setting */
 #define MXC_V_GCR_SYSCTRL_SBUSARB_ROUND                ((uint32_t)0x1UL) /**< SYSCTRL_SBUSARB_ROUND Value */
 #define MXC_S_GCR_SYSCTRL_SBUSARB_ROUND                (MXC_V_GCR_SYSCTRL_SBUSARB_ROUND << MXC_F_GCR_SYSCTRL_SBUSARB_POS) /**< SYSCTRL_SBUSARB_ROUND Setting */

 #define MXC_F_GCR_SYSCTRL_FPU_DIS_POS                  5 /**< SYSCTRL_FPU_DIS Position */
 #define MXC_F_GCR_SYSCTRL_FPU_DIS                      ((uint32_t)(0x1UL << MXC_F_GCR_SYSCTRL_FPU_DIS_POS)) /**< SYSCTRL_FPU_DIS Mask */

 #define MXC_F_GCR_SYSCTRL_ICC0_FLUSH_POS               6 /**< SYSCTRL_ICC0_FLUSH Position */
 #define MXC_F_GCR_SYSCTRL_ICC0_FLUSH                   ((uint32_t)(0x1UL << MXC_F_GCR_SYSCTRL_ICC0_FLUSH_POS)) /**< SYSCTRL_ICC0_FLUSH Mask */

 #define MXC_F_GCR_SYSCTRL_ROMDONE_POS                  12 /**< SYSCTRL_ROMDONE Position */
 #define MXC_F_GCR_SYSCTRL_ROMDONE                      ((uint32_t)(0x1UL << MXC_F_GCR_SYSCTRL_ROMDONE_POS)) /**< SYSCTRL_ROMDONE Mask */

 #define MXC_F_GCR_SYSCTRL_CCHK_POS                     13 /**< SYSCTRL_CCHK Position */
 #define MXC_F_GCR_SYSCTRL_CCHK                         ((uint32_t)(0x1UL << MXC_F_GCR_SYSCTRL_CCHK_POS)) /**< SYSCTRL_CCHK Mask */

 #define MXC_F_GCR_SYSCTRL_SWD_DIS_POS                  14 /**< SYSCTRL_SWD_DIS Position */
 #define MXC_F_GCR_SYSCTRL_SWD_DIS                      ((uint32_t)(0x1UL << MXC_F_GCR_SYSCTRL_SWD_DIS_POS)) /**< SYSCTRL_SWD_DIS Mask */

 #define MXC_F_GCR_SYSCTRL_CHKRES_POS                   15 /**< SYSCTRL_CHKRES Position */
 #define MXC_F_GCR_SYSCTRL_CHKRES                       ((uint32_t)(0x1UL << MXC_F_GCR_SYSCTRL_CHKRES_POS)) /**< SYSCTRL_CHKRES Mask */

/**@} end of group GCR_SYSCTRL_Register */

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

 #define MXC_F_GCR_RST0_GPIO1_POS                       3 /**< RST0_GPIO1 Position */
 #define MXC_F_GCR_RST0_GPIO1                           ((uint32_t)(0x1UL << MXC_F_GCR_RST0_GPIO1_POS)) /**< RST0_GPIO1 Mask */

 #define MXC_F_GCR_RST0_TMR0_POS                        5 /**< RST0_TMR0 Position */
 #define MXC_F_GCR_RST0_TMR0                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TMR0_POS)) /**< RST0_TMR0 Mask */

 #define MXC_F_GCR_RST0_TMR1_POS                        6 /**< RST0_TMR1 Position */
 #define MXC_F_GCR_RST0_TMR1                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TMR1_POS)) /**< RST0_TMR1 Mask */

 #define MXC_F_GCR_RST0_TMR2_POS                        7 /**< RST0_TMR2 Position */
 #define MXC_F_GCR_RST0_TMR2                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TMR2_POS)) /**< RST0_TMR2 Mask */

 #define MXC_F_GCR_RST0_TMR3_POS                        8 /**< RST0_TMR3 Position */
 #define MXC_F_GCR_RST0_TMR3                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TMR3_POS)) /**< RST0_TMR3 Mask */

 #define MXC_F_GCR_RST0_UART0_POS                       11 /**< RST0_UART0 Position */
 #define MXC_F_GCR_RST0_UART0                           ((uint32_t)(0x1UL << MXC_F_GCR_RST0_UART0_POS)) /**< RST0_UART0 Mask */

 #define MXC_F_GCR_RST0_UART1_POS                       12 /**< RST0_UART1 Position */
 #define MXC_F_GCR_RST0_UART1                           ((uint32_t)(0x1UL << MXC_F_GCR_RST0_UART1_POS)) /**< RST0_UART1 Mask */

 #define MXC_F_GCR_RST0_SPI0_POS                        13 /**< RST0_SPI0 Position */
 #define MXC_F_GCR_RST0_SPI0                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_SPI0_POS)) /**< RST0_SPI0 Mask */

 #define MXC_F_GCR_RST0_SPI1_POS                        14 /**< RST0_SPI1 Position */
 #define MXC_F_GCR_RST0_SPI1                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_SPI1_POS)) /**< RST0_SPI1 Mask */

 #define MXC_F_GCR_RST0_SPI2_POS                        15 /**< RST0_SPI2 Position */
 #define MXC_F_GCR_RST0_SPI2                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_SPI2_POS)) /**< RST0_SPI2 Mask */

 #define MXC_F_GCR_RST0_I2C0_POS                        16 /**< RST0_I2C0 Position */
 #define MXC_F_GCR_RST0_I2C0                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_I2C0_POS)) /**< RST0_I2C0 Mask */

 #define MXC_F_GCR_RST0_RTC_POS                         17 /**< RST0_RTC Position */
 #define MXC_F_GCR_RST0_RTC                             ((uint32_t)(0x1UL << MXC_F_GCR_RST0_RTC_POS)) /**< RST0_RTC Mask */

 #define MXC_F_GCR_RST0_TRNG_POS                        24 /**< RST0_TRNG Position */
 #define MXC_F_GCR_RST0_TRNG                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_TRNG_POS)) /**< RST0_TRNG Mask */

 #define MXC_F_GCR_RST0_UART2_POS                       28 /**< RST0_UART2 Position */
 #define MXC_F_GCR_RST0_UART2                           ((uint32_t)(0x1UL << MXC_F_GCR_RST0_UART2_POS)) /**< RST0_UART2 Mask */

 #define MXC_F_GCR_RST0_SOFT_POS                        29 /**< RST0_SOFT Position */
 #define MXC_F_GCR_RST0_SOFT                            ((uint32_t)(0x1UL << MXC_F_GCR_RST0_SOFT_POS)) /**< RST0_SOFT Mask */

 #define MXC_F_GCR_RST0_PERIPH_POS                      30 /**< RST0_PERIPH Position */
 #define MXC_F_GCR_RST0_PERIPH                          ((uint32_t)(0x1UL << MXC_F_GCR_RST0_PERIPH_POS)) /**< RST0_PERIPH Mask */

 #define MXC_F_GCR_RST0_SYS_POS                         31 /**< RST0_SYS Position */
 #define MXC_F_GCR_RST0_SYS                             ((uint32_t)(0x1UL << MXC_F_GCR_RST0_SYS_POS)) /**< RST0_SYS Mask */

/**@} end of group GCR_RST0_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_CLKCTRL GCR_CLKCTRL
 * @brief    Clock Control.
 * @{
 */
 #define MXC_F_GCR_CLKCTRL_SYSCLK_DIV_POS               6 /**< CLKCTRL_SYSCLK_DIV Position */
 #define MXC_F_GCR_CLKCTRL_SYSCLK_DIV                   ((uint32_t)(0x7UL << MXC_F_GCR_CLKCTRL_SYSCLK_DIV_POS)) /**< CLKCTRL_SYSCLK_DIV Mask */
 #define MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV1              ((uint32_t)0x0UL) /**< CLKCTRL_SYSCLK_DIV_DIV1 Value */
 #define MXC_S_GCR_CLKCTRL_SYSCLK_DIV_DIV1              (MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV1 << MXC_F_GCR_CLKCTRL_SYSCLK_DIV_POS) /**< CLKCTRL_SYSCLK_DIV_DIV1 Setting */
 #define MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV2              ((uint32_t)0x1UL) /**< CLKCTRL_SYSCLK_DIV_DIV2 Value */
 #define MXC_S_GCR_CLKCTRL_SYSCLK_DIV_DIV2              (MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV2 << MXC_F_GCR_CLKCTRL_SYSCLK_DIV_POS) /**< CLKCTRL_SYSCLK_DIV_DIV2 Setting */
 #define MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV4              ((uint32_t)0x2UL) /**< CLKCTRL_SYSCLK_DIV_DIV4 Value */
 #define MXC_S_GCR_CLKCTRL_SYSCLK_DIV_DIV4              (MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV4 << MXC_F_GCR_CLKCTRL_SYSCLK_DIV_POS) /**< CLKCTRL_SYSCLK_DIV_DIV4 Setting */
 #define MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV8              ((uint32_t)0x3UL) /**< CLKCTRL_SYSCLK_DIV_DIV8 Value */
 #define MXC_S_GCR_CLKCTRL_SYSCLK_DIV_DIV8              (MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV8 << MXC_F_GCR_CLKCTRL_SYSCLK_DIV_POS) /**< CLKCTRL_SYSCLK_DIV_DIV8 Setting */
 #define MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV16             ((uint32_t)0x4UL) /**< CLKCTRL_SYSCLK_DIV_DIV16 Value */
 #define MXC_S_GCR_CLKCTRL_SYSCLK_DIV_DIV16             (MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV16 << MXC_F_GCR_CLKCTRL_SYSCLK_DIV_POS) /**< CLKCTRL_SYSCLK_DIV_DIV16 Setting */
 #define MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV32             ((uint32_t)0x5UL) /**< CLKCTRL_SYSCLK_DIV_DIV32 Value */
 #define MXC_S_GCR_CLKCTRL_SYSCLK_DIV_DIV32             (MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV32 << MXC_F_GCR_CLKCTRL_SYSCLK_DIV_POS) /**< CLKCTRL_SYSCLK_DIV_DIV32 Setting */
 #define MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV64             ((uint32_t)0x6UL) /**< CLKCTRL_SYSCLK_DIV_DIV64 Value */
 #define MXC_S_GCR_CLKCTRL_SYSCLK_DIV_DIV64             (MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV64 << MXC_F_GCR_CLKCTRL_SYSCLK_DIV_POS) /**< CLKCTRL_SYSCLK_DIV_DIV64 Setting */
 #define MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV128            ((uint32_t)0x7UL) /**< CLKCTRL_SYSCLK_DIV_DIV128 Value */
 #define MXC_S_GCR_CLKCTRL_SYSCLK_DIV_DIV128            (MXC_V_GCR_CLKCTRL_SYSCLK_DIV_DIV128 << MXC_F_GCR_CLKCTRL_SYSCLK_DIV_POS) /**< CLKCTRL_SYSCLK_DIV_DIV128 Setting */

 #define MXC_F_GCR_CLKCTRL_SYSCLK_SEL_POS               9 /**< CLKCTRL_SYSCLK_SEL Position */
 #define MXC_F_GCR_CLKCTRL_SYSCLK_SEL                   ((uint32_t)(0x7UL << MXC_F_GCR_CLKCTRL_SYSCLK_SEL_POS)) /**< CLKCTRL_SYSCLK_SEL Mask */
 #define MXC_V_GCR_CLKCTRL_SYSCLK_SEL_ERFO              ((uint32_t)0x2UL) /**< CLKCTRL_SYSCLK_SEL_ERFO Value */
 #define MXC_S_GCR_CLKCTRL_SYSCLK_SEL_ERFO              (MXC_V_GCR_CLKCTRL_SYSCLK_SEL_ERFO << MXC_F_GCR_CLKCTRL_SYSCLK_SEL_POS) /**< CLKCTRL_SYSCLK_SEL_ERFO Setting */
 #define MXC_V_GCR_CLKCTRL_SYSCLK_SEL_INRO              ((uint32_t)0x3UL) /**< CLKCTRL_SYSCLK_SEL_INRO Value */
 #define MXC_S_GCR_CLKCTRL_SYSCLK_SEL_INRO              (MXC_V_GCR_CLKCTRL_SYSCLK_SEL_INRO << MXC_F_GCR_CLKCTRL_SYSCLK_SEL_POS) /**< CLKCTRL_SYSCLK_SEL_INRO Setting */
 #define MXC_V_GCR_CLKCTRL_SYSCLK_SEL_IPO               ((uint32_t)0x4UL) /**< CLKCTRL_SYSCLK_SEL_IPO Value */
 #define MXC_S_GCR_CLKCTRL_SYSCLK_SEL_IPO               (MXC_V_GCR_CLKCTRL_SYSCLK_SEL_IPO << MXC_F_GCR_CLKCTRL_SYSCLK_SEL_POS) /**< CLKCTRL_SYSCLK_SEL_IPO Setting */
 #define MXC_V_GCR_CLKCTRL_SYSCLK_SEL_IBRO              ((uint32_t)0x5UL) /**< CLKCTRL_SYSCLK_SEL_IBRO Value */
 #define MXC_S_GCR_CLKCTRL_SYSCLK_SEL_IBRO              (MXC_V_GCR_CLKCTRL_SYSCLK_SEL_IBRO << MXC_F_GCR_CLKCTRL_SYSCLK_SEL_POS) /**< CLKCTRL_SYSCLK_SEL_IBRO Setting */
 #define MXC_V_GCR_CLKCTRL_SYSCLK_SEL_ERTCO             ((uint32_t)0x6UL) /**< CLKCTRL_SYSCLK_SEL_ERTCO Value */
 #define MXC_S_GCR_CLKCTRL_SYSCLK_SEL_ERTCO             (MXC_V_GCR_CLKCTRL_SYSCLK_SEL_ERTCO << MXC_F_GCR_CLKCTRL_SYSCLK_SEL_POS) /**< CLKCTRL_SYSCLK_SEL_ERTCO Setting */
 #define MXC_V_GCR_CLKCTRL_SYSCLK_SEL_EXTCLK            ((uint32_t)0x7UL) /**< CLKCTRL_SYSCLK_SEL_EXTCLK Value */
 #define MXC_S_GCR_CLKCTRL_SYSCLK_SEL_EXTCLK            (MXC_V_GCR_CLKCTRL_SYSCLK_SEL_EXTCLK << MXC_F_GCR_CLKCTRL_SYSCLK_SEL_POS) /**< CLKCTRL_SYSCLK_SEL_EXTCLK Setting */

 #define MXC_F_GCR_CLKCTRL_SYSCLK_RDY_POS               13 /**< CLKCTRL_SYSCLK_RDY Position */
 #define MXC_F_GCR_CLKCTRL_SYSCLK_RDY                   ((uint32_t)(0x1UL << MXC_F_GCR_CLKCTRL_SYSCLK_RDY_POS)) /**< CLKCTRL_SYSCLK_RDY Mask */

 #define MXC_F_GCR_CLKCTRL_IPO_DIV_POS                  14 /**< CLKCTRL_IPO_DIV Position */
 #define MXC_F_GCR_CLKCTRL_IPO_DIV                      ((uint32_t)(0x3UL << MXC_F_GCR_CLKCTRL_IPO_DIV_POS)) /**< CLKCTRL_IPO_DIV Mask */
 #define MXC_V_GCR_CLKCTRL_IPO_DIV_DIV1                 ((uint32_t)0x0UL) /**< CLKCTRL_IPO_DIV_DIV1 Value */
 #define MXC_S_GCR_CLKCTRL_IPO_DIV_DIV1                 (MXC_V_GCR_CLKCTRL_IPO_DIV_DIV1 << MXC_F_GCR_CLKCTRL_IPO_DIV_POS) /**< CLKCTRL_IPO_DIV_DIV1 Setting */
 #define MXC_V_GCR_CLKCTRL_IPO_DIV_DIV2                 ((uint32_t)0x1UL) /**< CLKCTRL_IPO_DIV_DIV2 Value */
 #define MXC_S_GCR_CLKCTRL_IPO_DIV_DIV2                 (MXC_V_GCR_CLKCTRL_IPO_DIV_DIV2 << MXC_F_GCR_CLKCTRL_IPO_DIV_POS) /**< CLKCTRL_IPO_DIV_DIV2 Setting */
 #define MXC_V_GCR_CLKCTRL_IPO_DIV_DIV4                 ((uint32_t)0x2UL) /**< CLKCTRL_IPO_DIV_DIV4 Value */
 #define MXC_S_GCR_CLKCTRL_IPO_DIV_DIV4                 (MXC_V_GCR_CLKCTRL_IPO_DIV_DIV4 << MXC_F_GCR_CLKCTRL_IPO_DIV_POS) /**< CLKCTRL_IPO_DIV_DIV4 Setting */
 #define MXC_V_GCR_CLKCTRL_IPO_DIV_DIV8                 ((uint32_t)0x3UL) /**< CLKCTRL_IPO_DIV_DIV8 Value */
 #define MXC_S_GCR_CLKCTRL_IPO_DIV_DIV8                 (MXC_V_GCR_CLKCTRL_IPO_DIV_DIV8 << MXC_F_GCR_CLKCTRL_IPO_DIV_POS) /**< CLKCTRL_IPO_DIV_DIV8 Setting */

 #define MXC_F_GCR_CLKCTRL_ERFO_EN_POS                  16 /**< CLKCTRL_ERFO_EN Position */
 #define MXC_F_GCR_CLKCTRL_ERFO_EN                      ((uint32_t)(0x1UL << MXC_F_GCR_CLKCTRL_ERFO_EN_POS)) /**< CLKCTRL_ERFO_EN Mask */

 #define MXC_F_GCR_CLKCTRL_ERTCO_EN_POS                 17 /**< CLKCTRL_ERTCO_EN Position */
 #define MXC_F_GCR_CLKCTRL_ERTCO_EN                     ((uint32_t)(0x1UL << MXC_F_GCR_CLKCTRL_ERTCO_EN_POS)) /**< CLKCTRL_ERTCO_EN Mask */

 #define MXC_F_GCR_CLKCTRL_IPO_EN_POS                   19 /**< CLKCTRL_IPO_EN Position */
 #define MXC_F_GCR_CLKCTRL_IPO_EN                       ((uint32_t)(0x1UL << MXC_F_GCR_CLKCTRL_IPO_EN_POS)) /**< CLKCTRL_IPO_EN Mask */

 #define MXC_F_GCR_CLKCTRL_IBRO_EN_POS                  20 /**< CLKCTRL_IBRO_EN Position */
 #define MXC_F_GCR_CLKCTRL_IBRO_EN                      ((uint32_t)(0x1UL << MXC_F_GCR_CLKCTRL_IBRO_EN_POS)) /**< CLKCTRL_IBRO_EN Mask */

 #define MXC_F_GCR_CLKCTRL_IBRO_VS_POS                  21 /**< CLKCTRL_IBRO_VS Position */
 #define MXC_F_GCR_CLKCTRL_IBRO_VS                      ((uint32_t)(0x1UL << MXC_F_GCR_CLKCTRL_IBRO_VS_POS)) /**< CLKCTRL_IBRO_VS Mask */

 #define MXC_F_GCR_CLKCTRL_ERFO_RDY_POS                 24 /**< CLKCTRL_ERFO_RDY Position */
 #define MXC_F_GCR_CLKCTRL_ERFO_RDY                     ((uint32_t)(0x1UL << MXC_F_GCR_CLKCTRL_ERFO_RDY_POS)) /**< CLKCTRL_ERFO_RDY Mask */

 #define MXC_F_GCR_CLKCTRL_ERTCO_RDY_POS                25 /**< CLKCTRL_ERTCO_RDY Position */
 #define MXC_F_GCR_CLKCTRL_ERTCO_RDY                    ((uint32_t)(0x1UL << MXC_F_GCR_CLKCTRL_ERTCO_RDY_POS)) /**< CLKCTRL_ERTCO_RDY Mask */

 #define MXC_F_GCR_CLKCTRL_IPO_RDY_POS                  27 /**< CLKCTRL_IPO_RDY Position */
 #define MXC_F_GCR_CLKCTRL_IPO_RDY                      ((uint32_t)(0x1UL << MXC_F_GCR_CLKCTRL_IPO_RDY_POS)) /**< CLKCTRL_IPO_RDY Mask */

 #define MXC_F_GCR_CLKCTRL_IBRO_RDY_POS                 28 /**< CLKCTRL_IBRO_RDY Position */
 #define MXC_F_GCR_CLKCTRL_IBRO_RDY                     ((uint32_t)(0x1UL << MXC_F_GCR_CLKCTRL_IBRO_RDY_POS)) /**< CLKCTRL_IBRO_RDY Mask */

 #define MXC_F_GCR_CLKCTRL_INRO_RDY_POS                 29 /**< CLKCTRL_INRO_RDY Position */
 #define MXC_F_GCR_CLKCTRL_INRO_RDY                     ((uint32_t)(0x1UL << MXC_F_GCR_CLKCTRL_INRO_RDY_POS)) /**< CLKCTRL_INRO_RDY Mask */

 #define MXC_F_GCR_CLKCTRL_EXTCLK_RDY_POS               31 /**< CLKCTRL_EXTCLK_RDY Position */
 #define MXC_F_GCR_CLKCTRL_EXTCLK_RDY                   ((uint32_t)(0x1UL << MXC_F_GCR_CLKCTRL_EXTCLK_RDY_POS)) /**< CLKCTRL_EXTCLK_RDY Mask */

/**@} end of group GCR_CLKCTRL_Register */

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

 #define MXC_F_GCR_PM_GPIO_WE_POS                       4 /**< PM_GPIO_WE Position */
 #define MXC_F_GCR_PM_GPIO_WE                           ((uint32_t)(0x1UL << MXC_F_GCR_PM_GPIO_WE_POS)) /**< PM_GPIO_WE Mask */

 #define MXC_F_GCR_PM_RTC_WE_POS                        5 /**< PM_RTC_WE Position */
 #define MXC_F_GCR_PM_RTC_WE                            ((uint32_t)(0x1UL << MXC_F_GCR_PM_RTC_WE_POS)) /**< PM_RTC_WE Mask */

 #define MXC_F_GCR_PM_LPTMR0_WE_POS                     6 /**< PM_LPTMR0_WE Position */
 #define MXC_F_GCR_PM_LPTMR0_WE                         ((uint32_t)(0x1UL << MXC_F_GCR_PM_LPTMR0_WE_POS)) /**< PM_LPTMR0_WE Mask */

 #define MXC_F_GCR_PM_LPTMR1_WE_POS                     7 /**< PM_LPTMR1_WE Position */
 #define MXC_F_GCR_PM_LPTMR1_WE                         ((uint32_t)(0x1UL << MXC_F_GCR_PM_LPTMR1_WE_POS)) /**< PM_LPTMR1_WE Mask */

 #define MXC_F_GCR_PM_LPUART0_WE_POS                    8 /**< PM_LPUART0_WE Position */
 #define MXC_F_GCR_PM_LPUART0_WE                        ((uint32_t)(0x1UL << MXC_F_GCR_PM_LPUART0_WE_POS)) /**< PM_LPUART0_WE Mask */

 #define MXC_F_GCR_PM_ERFO_PD_POS                       14 /**< PM_ERFO_PD Position */
 #define MXC_F_GCR_PM_ERFO_PD                           ((uint32_t)(0x1UL << MXC_F_GCR_PM_ERFO_PD_POS)) /**< PM_ERFO_PD Mask */

 #define MXC_F_GCR_PM_IPO_PD_POS                        16 /**< PM_IPO_PD Position */
 #define MXC_F_GCR_PM_IPO_PD                            ((uint32_t)(0x1UL << MXC_F_GCR_PM_IPO_PD_POS)) /**< PM_IPO_PD Mask */

 #define MXC_F_GCR_PM_IBRO_PD_POS                       17 /**< PM_IBRO_PD Position */
 #define MXC_F_GCR_PM_IBRO_PD                           ((uint32_t)(0x1UL << MXC_F_GCR_PM_IBRO_PD_POS)) /**< PM_IBRO_PD Mask */

 #define MXC_F_GCR_PM_ERFO_BP_POS                       20 /**< PM_ERFO_BP Position */
 #define MXC_F_GCR_PM_ERFO_BP                           ((uint32_t)(0x1UL << MXC_F_GCR_PM_ERFO_BP_POS)) /**< PM_ERFO_BP Mask */

/**@} end of group GCR_PM_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_PCLKDIV GCR_PCLKDIV
 * @brief    Peripheral Clock Divider.
 * @{
 */
 #define MXC_F_GCR_PCLKDIV_AON_CLKDIV_POS               0 /**< PCLKDIV_AON_CLKDIV Position */
 #define MXC_F_GCR_PCLKDIV_AON_CLKDIV                   ((uint32_t)(0x3UL << MXC_F_GCR_PCLKDIV_AON_CLKDIV_POS)) /**< PCLKDIV_AON_CLKDIV Mask */
 #define MXC_V_GCR_PCLKDIV_AON_CLKDIV_DIV4              ((uint32_t)0x0UL) /**< PCLKDIV_AON_CLKDIV_DIV4 Value */
 #define MXC_S_GCR_PCLKDIV_AON_CLKDIV_DIV4              (MXC_V_GCR_PCLKDIV_AON_CLKDIV_DIV4 << MXC_F_GCR_PCLKDIV_AON_CLKDIV_POS) /**< PCLKDIV_AON_CLKDIV_DIV4 Setting */
 #define MXC_V_GCR_PCLKDIV_AON_CLKDIV_DIV8              ((uint32_t)0x1UL) /**< PCLKDIV_AON_CLKDIV_DIV8 Value */
 #define MXC_S_GCR_PCLKDIV_AON_CLKDIV_DIV8              (MXC_V_GCR_PCLKDIV_AON_CLKDIV_DIV8 << MXC_F_GCR_PCLKDIV_AON_CLKDIV_POS) /**< PCLKDIV_AON_CLKDIV_DIV8 Setting */
 #define MXC_V_GCR_PCLKDIV_AON_CLKDIV_DIV16             ((uint32_t)0x2UL) /**< PCLKDIV_AON_CLKDIV_DIV16 Value */
 #define MXC_S_GCR_PCLKDIV_AON_CLKDIV_DIV16             (MXC_V_GCR_PCLKDIV_AON_CLKDIV_DIV16 << MXC_F_GCR_PCLKDIV_AON_CLKDIV_POS) /**< PCLKDIV_AON_CLKDIV_DIV16 Setting */
 #define MXC_V_GCR_PCLKDIV_AON_CLKDIV_DIV32             ((uint32_t)0x3UL) /**< PCLKDIV_AON_CLKDIV_DIV32 Value */
 #define MXC_S_GCR_PCLKDIV_AON_CLKDIV_DIV32             (MXC_V_GCR_PCLKDIV_AON_CLKDIV_DIV32 << MXC_F_GCR_PCLKDIV_AON_CLKDIV_POS) /**< PCLKDIV_AON_CLKDIV_DIV32 Setting */

 #define MXC_F_GCR_PCLKDIV_DIV_CLK_OUT_CTRL_POS         14 /**< PCLKDIV_DIV_CLK_OUT_CTRL Position */
 #define MXC_F_GCR_PCLKDIV_DIV_CLK_OUT_CTRL             ((uint32_t)(0x3UL << MXC_F_GCR_PCLKDIV_DIV_CLK_OUT_CTRL_POS)) /**< PCLKDIV_DIV_CLK_OUT_CTRL Mask */

 #define MXC_F_GCR_PCLKDIV_DIV_CLK_OUT_EN_POS           16 /**< PCLKDIV_DIV_CLK_OUT_EN Position */
 #define MXC_F_GCR_PCLKDIV_DIV_CLK_OUT_EN               ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIV_DIV_CLK_OUT_EN_POS)) /**< PCLKDIV_DIV_CLK_OUT_EN Mask */

/**@} end of group GCR_PCLKDIV_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_PCLKDIS0 GCR_PCLKDIS0
 * @brief    Peripheral Clock Disable.
 * @{
 */
 #define MXC_F_GCR_PCLKDIS0_GPIO0_POS                   0 /**< PCLKDIS0_GPIO0 Position */
 #define MXC_F_GCR_PCLKDIS0_GPIO0                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS0_GPIO0_POS)) /**< PCLKDIS0_GPIO0 Mask */

 #define MXC_F_GCR_PCLKDIS0_GPIO1_POS                   1 /**< PCLKDIS0_GPIO1 Position */
 #define MXC_F_GCR_PCLKDIS0_GPIO1                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS0_GPIO1_POS)) /**< PCLKDIS0_GPIO1 Mask */

 #define MXC_F_GCR_PCLKDIS0_DMA_POS                     5 /**< PCLKDIS0_DMA Position */
 #define MXC_F_GCR_PCLKDIS0_DMA                         ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS0_DMA_POS)) /**< PCLKDIS0_DMA Mask */

 #define MXC_F_GCR_PCLKDIS0_SPI0_POS                    6 /**< PCLKDIS0_SPI0 Position */
 #define MXC_F_GCR_PCLKDIS0_SPI0                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS0_SPI0_POS)) /**< PCLKDIS0_SPI0 Mask */

 #define MXC_F_GCR_PCLKDIS0_SPI1_POS                    7 /**< PCLKDIS0_SPI1 Position */
 #define MXC_F_GCR_PCLKDIS0_SPI1                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS0_SPI1_POS)) /**< PCLKDIS0_SPI1 Mask */

 #define MXC_F_GCR_PCLKDIS0_SPI2_POS                    8 /**< PCLKDIS0_SPI2 Position */
 #define MXC_F_GCR_PCLKDIS0_SPI2                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS0_SPI2_POS)) /**< PCLKDIS0_SPI2 Mask */

 #define MXC_F_GCR_PCLKDIS0_UART0_POS                   9 /**< PCLKDIS0_UART0 Position */
 #define MXC_F_GCR_PCLKDIS0_UART0                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS0_UART0_POS)) /**< PCLKDIS0_UART0 Mask */

 #define MXC_F_GCR_PCLKDIS0_UART1_POS                   10 /**< PCLKDIS0_UART1 Position */
 #define MXC_F_GCR_PCLKDIS0_UART1                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS0_UART1_POS)) /**< PCLKDIS0_UART1 Mask */

 #define MXC_F_GCR_PCLKDIS0_I2C0_POS                    13 /**< PCLKDIS0_I2C0 Position */
 #define MXC_F_GCR_PCLKDIS0_I2C0                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS0_I2C0_POS)) /**< PCLKDIS0_I2C0 Mask */

 #define MXC_F_GCR_PCLKDIS0_TMR0_POS                    15 /**< PCLKDIS0_TMR0 Position */
 #define MXC_F_GCR_PCLKDIS0_TMR0                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS0_TMR0_POS)) /**< PCLKDIS0_TMR0 Mask */

 #define MXC_F_GCR_PCLKDIS0_TMR1_POS                    16 /**< PCLKDIS0_TMR1 Position */
 #define MXC_F_GCR_PCLKDIS0_TMR1                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS0_TMR1_POS)) /**< PCLKDIS0_TMR1 Mask */

 #define MXC_F_GCR_PCLKDIS0_TMR2_POS                    17 /**< PCLKDIS0_TMR2 Position */
 #define MXC_F_GCR_PCLKDIS0_TMR2                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS0_TMR2_POS)) /**< PCLKDIS0_TMR2 Mask */

 #define MXC_F_GCR_PCLKDIS0_TMR3_POS                    18 /**< PCLKDIS0_TMR3 Position */
 #define MXC_F_GCR_PCLKDIS0_TMR3                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS0_TMR3_POS)) /**< PCLKDIS0_TMR3 Mask */

 #define MXC_F_GCR_PCLKDIS0_I2C1_POS                    28 /**< PCLKDIS0_I2C1 Position */
 #define MXC_F_GCR_PCLKDIS0_I2C1                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS0_I2C1_POS)) /**< PCLKDIS0_I2C1 Mask */

/**@} end of group GCR_PCLKDIS0_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_MEMCTRL GCR_MEMCTRL
 * @brief    Memory Clock Control Register.
 * @{
 */
 #define MXC_F_GCR_MEMCTRL_FWS_POS                      0 /**< MEMCTRL_FWS Position */
 #define MXC_F_GCR_MEMCTRL_FWS                          ((uint32_t)(0x7UL << MXC_F_GCR_MEMCTRL_FWS_POS)) /**< MEMCTRL_FWS Mask */

 #define MXC_F_GCR_MEMCTRL_RAMWS_EN_POS                 4 /**< MEMCTRL_RAMWS_EN Position */
 #define MXC_F_GCR_MEMCTRL_RAMWS_EN                     ((uint32_t)(0x1UL << MXC_F_GCR_MEMCTRL_RAMWS_EN_POS)) /**< MEMCTRL_RAMWS_EN Mask */

 #define MXC_F_GCR_MEMCTRL_RAM0LS_EN_POS                8 /**< MEMCTRL_RAM0LS_EN Position */
 #define MXC_F_GCR_MEMCTRL_RAM0LS_EN                    ((uint32_t)(0x1UL << MXC_F_GCR_MEMCTRL_RAM0LS_EN_POS)) /**< MEMCTRL_RAM0LS_EN Mask */

 #define MXC_F_GCR_MEMCTRL_RAM1LS_EN_POS                9 /**< MEMCTRL_RAM1LS_EN Position */
 #define MXC_F_GCR_MEMCTRL_RAM1LS_EN                    ((uint32_t)(0x1UL << MXC_F_GCR_MEMCTRL_RAM1LS_EN_POS)) /**< MEMCTRL_RAM1LS_EN Mask */

 #define MXC_F_GCR_MEMCTRL_RAM2LS_EN_POS                10 /**< MEMCTRL_RAM2LS_EN Position */
 #define MXC_F_GCR_MEMCTRL_RAM2LS_EN                    ((uint32_t)(0x1UL << MXC_F_GCR_MEMCTRL_RAM2LS_EN_POS)) /**< MEMCTRL_RAM2LS_EN Mask */

 #define MXC_F_GCR_MEMCTRL_RAM3LS_EN_POS                11 /**< MEMCTRL_RAM3LS_EN Position */
 #define MXC_F_GCR_MEMCTRL_RAM3LS_EN                    ((uint32_t)(0x1UL << MXC_F_GCR_MEMCTRL_RAM3LS_EN_POS)) /**< MEMCTRL_RAM3LS_EN Mask */

 #define MXC_F_GCR_MEMCTRL_ICC0LS_EN_POS                12 /**< MEMCTRL_ICC0LS_EN Position */
 #define MXC_F_GCR_MEMCTRL_ICC0LS_EN                    ((uint32_t)(0x1UL << MXC_F_GCR_MEMCTRL_ICC0LS_EN_POS)) /**< MEMCTRL_ICC0LS_EN Mask */

 #define MXC_F_GCR_MEMCTRL_ROMLS_EN_POS                 13 /**< MEMCTRL_ROMLS_EN Position */
 #define MXC_F_GCR_MEMCTRL_ROMLS_EN                     ((uint32_t)(0x1UL << MXC_F_GCR_MEMCTRL_ROMLS_EN_POS)) /**< MEMCTRL_ROMLS_EN Mask */

/**@} end of group GCR_MEMCTRL_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_MEMZ GCR_MEMZ
 * @brief    Memory Zeroize Control.
 * @{
 */
 #define MXC_F_GCR_MEMZ_RAM_POS                         0 /**< MEMZ_RAM Position */
 #define MXC_F_GCR_MEMZ_RAM                             ((uint32_t)(0x1UL << MXC_F_GCR_MEMZ_RAM_POS)) /**< MEMZ_RAM Mask */

 #define MXC_F_GCR_MEMZ_RAMCB_POS                       1 /**< MEMZ_RAMCB Position */
 #define MXC_F_GCR_MEMZ_RAMCB                           ((uint32_t)(0x1UL << MXC_F_GCR_MEMZ_RAMCB_POS)) /**< MEMZ_RAMCB Mask */

 #define MXC_F_GCR_MEMZ_ICC0_POS                        2 /**< MEMZ_ICC0 Position */
 #define MXC_F_GCR_MEMZ_ICC0                            ((uint32_t)(0x1UL << MXC_F_GCR_MEMZ_ICC0_POS)) /**< MEMZ_ICC0 Mask */

/**@} end of group GCR_MEMZ_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_SYSST GCR_SYSST
 * @brief    System Status Register.
 * @{
 */
 #define MXC_F_GCR_SYSST_ICELOCK_POS                    0 /**< SYSST_ICELOCK Position */
 #define MXC_F_GCR_SYSST_ICELOCK                        ((uint32_t)(0x1UL << MXC_F_GCR_SYSST_ICELOCK_POS)) /**< SYSST_ICELOCK Mask */

/**@} end of group GCR_SYSST_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_RST1 GCR_RST1
 * @brief    Reset 1.
 * @{
 */
 #define MXC_F_GCR_RST1_I2C1_POS                        0 /**< RST1_I2C1 Position */
 #define MXC_F_GCR_RST1_I2C1                            ((uint32_t)(0x1UL << MXC_F_GCR_RST1_I2C1_POS)) /**< RST1_I2C1 Mask */

 #define MXC_F_GCR_RST1_WDT1_POS                        8 /**< RST1_WDT1 Position */
 #define MXC_F_GCR_RST1_WDT1                            ((uint32_t)(0x1UL << MXC_F_GCR_RST1_WDT1_POS)) /**< RST1_WDT1 Mask */

 #define MXC_F_GCR_RST1_CRC_POS                         9 /**< RST1_CRC Position */
 #define MXC_F_GCR_RST1_CRC                             ((uint32_t)(0x1UL << MXC_F_GCR_RST1_CRC_POS)) /**< RST1_CRC Mask */

 #define MXC_F_GCR_RST1_AES_POS                         10 /**< RST1_AES Position */
 #define MXC_F_GCR_RST1_AES                             ((uint32_t)(0x1UL << MXC_F_GCR_RST1_AES_POS)) /**< RST1_AES Mask */

 #define MXC_F_GCR_RST1_AC_POS                          14 /**< RST1_AC Position */
 #define MXC_F_GCR_RST1_AC                              ((uint32_t)(0x1UL << MXC_F_GCR_RST1_AC_POS)) /**< RST1_AC Mask */

 #define MXC_F_GCR_RST1_I2C2_POS                        17 /**< RST1_I2C2 Position */
 #define MXC_F_GCR_RST1_I2C2                            ((uint32_t)(0x1UL << MXC_F_GCR_RST1_I2C2_POS)) /**< RST1_I2C2 Mask */

 #define MXC_F_GCR_RST1_I2S_POS                         23 /**< RST1_I2S Position */
 #define MXC_F_GCR_RST1_I2S                             ((uint32_t)(0x1UL << MXC_F_GCR_RST1_I2S_POS)) /**< RST1_I2S Mask */

/**@} end of group GCR_RST1_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_PCLKDIS1 GCR_PCLKDIS1
 * @brief    Peripheral Clock Disable.
 * @{
 */
 #define MXC_F_GCR_PCLKDIS1_UART2_POS                   1 /**< PCLKDIS1_UART2 Position */
 #define MXC_F_GCR_PCLKDIS1_UART2                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS1_UART2_POS)) /**< PCLKDIS1_UART2 Mask */

 #define MXC_F_GCR_PCLKDIS1_TRNG_POS                    2 /**< PCLKDIS1_TRNG Position */
 #define MXC_F_GCR_PCLKDIS1_TRNG                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS1_TRNG_POS)) /**< PCLKDIS1_TRNG Mask */

 #define MXC_F_GCR_PCLKDIS1_WWDT0_POS                   4 /**< PCLKDIS1_WWDT0 Position */
 #define MXC_F_GCR_PCLKDIS1_WWDT0                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS1_WWDT0_POS)) /**< PCLKDIS1_WWDT0 Mask */

 #define MXC_F_GCR_PCLKDIS1_WWDT1_POS                   5 /**< PCLKDIS1_WWDT1 Position */
 #define MXC_F_GCR_PCLKDIS1_WWDT1                       ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS1_WWDT1_POS)) /**< PCLKDIS1_WWDT1 Mask */

 #define MXC_F_GCR_PCLKDIS1_ICC0_POS                    11 /**< PCLKDIS1_ICC0 Position */
 #define MXC_F_GCR_PCLKDIS1_ICC0                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS1_ICC0_POS)) /**< PCLKDIS1_ICC0 Mask */

 #define MXC_F_GCR_PCLKDIS1_CRC_POS                     14 /**< PCLKDIS1_CRC Position */
 #define MXC_F_GCR_PCLKDIS1_CRC                         ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS1_CRC_POS)) /**< PCLKDIS1_CRC Mask */

 #define MXC_F_GCR_PCLKDIS1_AES_POS                     15 /**< PCLKDIS1_AES Position */
 #define MXC_F_GCR_PCLKDIS1_AES                         ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS1_AES_POS)) /**< PCLKDIS1_AES Mask */

 #define MXC_F_GCR_PCLKDIS1_I2C2_POS                    21 /**< PCLKDIS1_I2C2 Position */
 #define MXC_F_GCR_PCLKDIS1_I2C2                        ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS1_I2C2_POS)) /**< PCLKDIS1_I2C2 Mask */

 #define MXC_F_GCR_PCLKDIS1_I2S_POS                     23 /**< PCLKDIS1_I2S Position */
 #define MXC_F_GCR_PCLKDIS1_I2S                         ((uint32_t)(0x1UL << MXC_F_GCR_PCLKDIS1_I2S_POS)) /**< PCLKDIS1_I2S Mask */

/**@} end of group GCR_PCLKDIS1_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_EVENTEN GCR_EVENTEN
 * @brief    Event Enable Register.
 * @{
 */
 #define MXC_F_GCR_EVENTEN_DMA_POS                      0 /**< EVENTEN_DMA Position */
 #define MXC_F_GCR_EVENTEN_DMA                          ((uint32_t)(0x1UL << MXC_F_GCR_EVENTEN_DMA_POS)) /**< EVENTEN_DMA Mask */

 #define MXC_F_GCR_EVENTEN_RX_POS                       1 /**< EVENTEN_RX Position */
 #define MXC_F_GCR_EVENTEN_RX                           ((uint32_t)(0x1UL << MXC_F_GCR_EVENTEN_RX_POS)) /**< EVENTEN_RX Mask */

 #define MXC_F_GCR_EVENTEN_TX_POS                       2 /**< EVENTEN_TX Position */
 #define MXC_F_GCR_EVENTEN_TX                           ((uint32_t)(0x1UL << MXC_F_GCR_EVENTEN_TX_POS)) /**< EVENTEN_TX Mask */

/**@} end of group GCR_EVENTEN_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_REVISION GCR_REVISION
 * @brief    Revision Register.
 * @{
 */
 #define MXC_F_GCR_REVISION_REVISION_POS                0 /**< REVISION_REVISION Position */
 #define MXC_F_GCR_REVISION_REVISION                    ((uint32_t)(0xFFFFUL << MXC_F_GCR_REVISION_REVISION_POS)) /**< REVISION_REVISION Mask */

/**@} end of group GCR_REVISION_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_SYSIE GCR_SYSIE
 * @brief    System Status Interrupt Enable Register.
 * @{
 */
 #define MXC_F_GCR_SYSIE_ICEUNLOCK_POS                  0 /**< SYSIE_ICEUNLOCK Position */
 #define MXC_F_GCR_SYSIE_ICEUNLOCK                      ((uint32_t)(0x1UL << MXC_F_GCR_SYSIE_ICEUNLOCK_POS)) /**< SYSIE_ICEUNLOCK Mask */

/**@} end of group GCR_SYSIE_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_ECCERR GCR_ECCERR
 * @brief    ECC Error Register
 * @{
 */
 #define MXC_F_GCR_ECCERR_RAM_POS                       0 /**< ECCERR_RAM Position */
 #define MXC_F_GCR_ECCERR_RAM                           ((uint32_t)(0x1UL << MXC_F_GCR_ECCERR_RAM_POS)) /**< ECCERR_RAM Mask */

 #define MXC_F_GCR_ECCERR_ICC0_POS                      1 /**< ECCERR_ICC0 Position */
 #define MXC_F_GCR_ECCERR_ICC0                          ((uint32_t)(0x1UL << MXC_F_GCR_ECCERR_ICC0_POS)) /**< ECCERR_ICC0 Mask */

 #define MXC_F_GCR_ECCERR_FLASH_POS                     2 /**< ECCERR_FLASH Position */
 #define MXC_F_GCR_ECCERR_FLASH                         ((uint32_t)(0x1UL << MXC_F_GCR_ECCERR_FLASH_POS)) /**< ECCERR_FLASH Mask */

/**@} end of group GCR_ECCERR_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_ECCCED GCR_ECCCED
 * @brief    ECC Not Double Error Detect Register
 * @{
 */
 #define MXC_F_GCR_ECCCED_RAM_POS                       0 /**< ECCCED_RAM Position */
 #define MXC_F_GCR_ECCCED_RAM                           ((uint32_t)(0x1UL << MXC_F_GCR_ECCCED_RAM_POS)) /**< ECCCED_RAM Mask */

 #define MXC_F_GCR_ECCCED_ICC0_POS                      1 /**< ECCCED_ICC0 Position */
 #define MXC_F_GCR_ECCCED_ICC0                          ((uint32_t)(0x1UL << MXC_F_GCR_ECCCED_ICC0_POS)) /**< ECCCED_ICC0 Mask */

 #define MXC_F_GCR_ECCCED_FLASH_POS                     2 /**< ECCCED_FLASH Position */
 #define MXC_F_GCR_ECCCED_FLASH                         ((uint32_t)(0x1UL << MXC_F_GCR_ECCCED_FLASH_POS)) /**< ECCCED_FLASH Mask */

/**@} end of group GCR_ECCCED_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_ECCIE GCR_ECCIE
 * @brief    ECC IRQ Enable Register
 * @{
 */
 #define MXC_F_GCR_ECCIE_RAM_POS                        0 /**< ECCIE_RAM Position */
 #define MXC_F_GCR_ECCIE_RAM                            ((uint32_t)(0x1UL << MXC_F_GCR_ECCIE_RAM_POS)) /**< ECCIE_RAM Mask */

 #define MXC_F_GCR_ECCIE_ICC0_POS                       1 /**< ECCIE_ICC0 Position */
 #define MXC_F_GCR_ECCIE_ICC0                           ((uint32_t)(0x1UL << MXC_F_GCR_ECCIE_ICC0_POS)) /**< ECCIE_ICC0 Mask */

 #define MXC_F_GCR_ECCIE_FLASH_POS                      2 /**< ECCIE_FLASH Position */
 #define MXC_F_GCR_ECCIE_FLASH                          ((uint32_t)(0x1UL << MXC_F_GCR_ECCIE_FLASH_POS)) /**< ECCIE_FLASH Mask */

/**@} end of group GCR_ECCIE_Register */

/**
 * @ingroup  gcr_registers
 * @defgroup GCR_ECCADDR GCR_ECCADDR
 * @brief    ECC Error Address Register
 * @{
 */
 #define MXC_F_GCR_ECCADDR_DATARAMADDR_POS              0 /**< ECCADDR_DATARAMADDR Position */
 #define MXC_F_GCR_ECCADDR_DATARAMADDR                  ((uint32_t)(0x3FFFUL << MXC_F_GCR_ECCADDR_DATARAMADDR_POS)) /**< ECCADDR_DATARAMADDR Mask */

 #define MXC_F_GCR_ECCADDR_DATARAMBANK_POS              14 /**< ECCADDR_DATARAMBANK Position */
 #define MXC_F_GCR_ECCADDR_DATARAMBANK                  ((uint32_t)(0x1UL << MXC_F_GCR_ECCADDR_DATARAMBANK_POS)) /**< ECCADDR_DATARAMBANK Mask */

 #define MXC_F_GCR_ECCADDR_DATARAMERR_POS               15 /**< ECCADDR_DATARAMERR Position */
 #define MXC_F_GCR_ECCADDR_DATARAMERR                   ((uint32_t)(0x1UL << MXC_F_GCR_ECCADDR_DATARAMERR_POS)) /**< ECCADDR_DATARAMERR Mask */

 #define MXC_F_GCR_ECCADDR_TAGRAMADDR_POS               16 /**< ECCADDR_TAGRAMADDR Position */
 #define MXC_F_GCR_ECCADDR_TAGRAMADDR                   ((uint32_t)(0x3FFFUL << MXC_F_GCR_ECCADDR_TAGRAMADDR_POS)) /**< ECCADDR_TAGRAMADDR Mask */

 #define MXC_F_GCR_ECCADDR_TAGRAMBANK_POS               30 /**< ECCADDR_TAGRAMBANK Position */
 #define MXC_F_GCR_ECCADDR_TAGRAMBANK                   ((uint32_t)(0x1UL << MXC_F_GCR_ECCADDR_TAGRAMBANK_POS)) /**< ECCADDR_TAGRAMBANK Mask */

 #define MXC_F_GCR_ECCADDR_TAGRAMERR_POS                31 /**< ECCADDR_TAGRAMERR Position */
 #define MXC_F_GCR_ECCADDR_TAGRAMERR                    ((uint32_t)(0x1UL << MXC_F_GCR_ECCADDR_TAGRAMERR_POS)) /**< ECCADDR_TAGRAMERR Mask */

/**@} end of group GCR_ECCADDR_Register */

#ifdef __cplusplus
}
#endif

#endif /* _GCR_REGS_H_ */
