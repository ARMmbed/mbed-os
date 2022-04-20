/**
 * @file    mcr_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the MCR Peripheral Module.
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

#ifndef _MCR_REGS_H_
#define _MCR_REGS_H_

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
 * @ingroup     mcr
 * @defgroup    mcr_registers MCR_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the MCR Peripheral Module.
 * @details Misc Control.
 */

/**
 * @ingroup mcr_registers
 * Structure type to access the MCR Registers.
 */
typedef struct {
    __I  uint32_t rsv_0x0;
    __IO uint32_t rst;                  /**< <tt>\b 0x04:</tt> MCR RST Register */
    __I  uint32_t rsv_0x8_0x23[7];
    __IO uint32_t clkdis;               /**< <tt>\b 0x24:</tt> MCR CLKDIS Register */
} mxc_mcr_regs_t;

/* Register offsets for module MCR */
/**
 * @ingroup    mcr_registers
 * @defgroup   MCR_Register_Offsets Register Offsets
 * @brief      MCR Peripheral Register Offsets from the MCR Base Peripheral Address. 
 * @{
 */
 #define MXC_R_MCR_RST                      ((uint32_t)0x00000004UL) /**< Offset from MCR Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_MCR_CLKDIS                   ((uint32_t)0x00000024UL) /**< Offset from MCR Base Address: <tt> 0x0024</tt> */ 
/**@} end of group mcr_registers */

/**
 * @ingroup  mcr_registers
 * @defgroup MCR_RST MCR_RST
 * @brief    Reset Control Register
 * @{
 */
 #define MXC_F_MCR_RST_LPTMR0_POS                       0 /**< RST_LPTMR0 Position */
 #define MXC_F_MCR_RST_LPTMR0                           ((uint32_t)(0x1UL << MXC_F_MCR_RST_LPTMR0_POS)) /**< RST_LPTMR0 Mask */

 #define MXC_F_MCR_RST_LPTMR1_POS                       1 /**< RST_LPTMR1 Position */
 #define MXC_F_MCR_RST_LPTMR1                           ((uint32_t)(0x1UL << MXC_F_MCR_RST_LPTMR1_POS)) /**< RST_LPTMR1 Mask */

 #define MXC_F_MCR_RST_LPUART0_POS                      2 /**< RST_LPUART0 Position */
 #define MXC_F_MCR_RST_LPUART0                          ((uint32_t)(0x1UL << MXC_F_MCR_RST_LPUART0_POS)) /**< RST_LPUART0 Mask */

/**@} end of group MCR_RST_Register */

/**
 * @ingroup  mcr_registers
 * @defgroup MCR_CLKDIS MCR_CLKDIS
 * @brief    Low Power Peripheral Clock Disable.
 * @{
 */
 #define MXC_F_MCR_CLKDIS_LPTMR0_POS                    0 /**< CLKDIS_LPTMR0 Position */
 #define MXC_F_MCR_CLKDIS_LPTMR0                        ((uint32_t)(0x1UL << MXC_F_MCR_CLKDIS_LPTMR0_POS)) /**< CLKDIS_LPTMR0 Mask */

 #define MXC_F_MCR_CLKDIS_LPTMR1_POS                    1 /**< CLKDIS_LPTMR1 Position */
 #define MXC_F_MCR_CLKDIS_LPTMR1                        ((uint32_t)(0x1UL << MXC_F_MCR_CLKDIS_LPTMR1_POS)) /**< CLKDIS_LPTMR1 Mask */

 #define MXC_F_MCR_CLKDIS_LPUART0_POS                   2 /**< CLKDIS_LPUART0 Position */
 #define MXC_F_MCR_CLKDIS_LPUART0                       ((uint32_t)(0x1UL << MXC_F_MCR_CLKDIS_LPUART0_POS)) /**< CLKDIS_LPUART0 Mask */

/**@} end of group MCR_CLKDIS_Register */

#ifdef __cplusplus
}
#endif

#endif /* _MCR_REGS_H_ */
