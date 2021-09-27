/**
 * @file    flc_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the FLC Peripheral Module.
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

#ifndef _FLC_REGS_H_
#define _FLC_REGS_H_

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
 * @ingroup     flc
 * @defgroup    flc_registers FLC_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the FLC Peripheral Module.
 * @details Flash Memory Control.
 */

/**
 * @ingroup flc_registers
 * Structure type to access the FLC Registers.
 */
typedef struct {
    __IO uint32_t addr;                 /**< <tt>\b 0x00:</tt> FLC ADDR Register */
    __IO uint32_t clkdiv;               /**< <tt>\b 0x04:</tt> FLC CLKDIV Register */
    __IO uint32_t ctrl;                 /**< <tt>\b 0x08:</tt> FLC CTRL Register */
    __I  uint32_t rsv_0xc_0x23[6];
    __IO uint32_t intr;                 /**< <tt>\b 0x024:</tt> FLC INTR Register */
    __I  uint32_t rsv_0x28_0x2f[2];
    __IO uint32_t data[4];              /**< <tt>\b 0x30:</tt> FLC DATA Register */
    __O  uint32_t actrl;                /**< <tt>\b 0x40:</tt> FLC ACTRL Register */
} mxc_flc_regs_t;

/* Register offsets for module FLC */
/**
 * @ingroup    flc_registers
 * @defgroup   FLC_Register_Offsets Register Offsets
 * @brief      FLC Peripheral Register Offsets from the FLC Base Peripheral Address. 
 * @{
 */
 #define MXC_R_FLC_ADDR                     ((uint32_t)0x00000000UL) /**< Offset from FLC Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_FLC_CLKDIV                   ((uint32_t)0x00000004UL) /**< Offset from FLC Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_FLC_CTRL                     ((uint32_t)0x00000008UL) /**< Offset from FLC Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_FLC_INTR                     ((uint32_t)0x00000024UL) /**< Offset from FLC Base Address: <tt> 0x0024</tt> */ 
 #define MXC_R_FLC_DATA                     ((uint32_t)0x00000030UL) /**< Offset from FLC Base Address: <tt> 0x0030</tt> */ 
 #define MXC_R_FLC_ACTRL                    ((uint32_t)0x00000040UL) /**< Offset from FLC Base Address: <tt> 0x0040</tt> */ 
/**@} end of group flc_registers */

/**
 * @ingroup  flc_registers
 * @defgroup FLC_ADDR FLC_ADDR
 * @brief    Flash Write Address.
 * @{
 */
 #define MXC_F_FLC_ADDR_ADDR_POS                        0 /**< ADDR_ADDR Position */
 #define MXC_F_FLC_ADDR_ADDR                            ((uint32_t)(0xFFFFFFFFUL << MXC_F_FLC_ADDR_ADDR_POS)) /**< ADDR_ADDR Mask */

/**@} end of group FLC_ADDR_Register */

/**
 * @ingroup  flc_registers
 * @defgroup FLC_CLKDIV FLC_CLKDIV
 * @brief    Flash Clock Divide. The clock (PLL0) is divided by this value to generate a 1
 *           MHz clock for Flash controller.
 * @{
 */
 #define MXC_F_FLC_CLKDIV_CLKDIV_POS                    0 /**< CLKDIV_CLKDIV Position */
 #define MXC_F_FLC_CLKDIV_CLKDIV                        ((uint32_t)(0xFFUL << MXC_F_FLC_CLKDIV_CLKDIV_POS)) /**< CLKDIV_CLKDIV Mask */

/**@} end of group FLC_CLKDIV_Register */

/**
 * @ingroup  flc_registers
 * @defgroup FLC_CTRL FLC_CTRL
 * @brief    Flash Control Register.
 * @{
 */
 #define MXC_F_FLC_CTRL_WRITE_POS                       0 /**< CTRL_WRITE Position */
 #define MXC_F_FLC_CTRL_WRITE                           ((uint32_t)(0x1UL << MXC_F_FLC_CTRL_WRITE_POS)) /**< CTRL_WRITE Mask */

 #define MXC_F_FLC_CTRL_MASS_ERASE_POS                  1 /**< CTRL_MASS_ERASE Position */
 #define MXC_F_FLC_CTRL_MASS_ERASE                      ((uint32_t)(0x1UL << MXC_F_FLC_CTRL_MASS_ERASE_POS)) /**< CTRL_MASS_ERASE Mask */

 #define MXC_F_FLC_CTRL_PAGE_ERASE_POS                  2 /**< CTRL_PAGE_ERASE Position */
 #define MXC_F_FLC_CTRL_PAGE_ERASE                      ((uint32_t)(0x1UL << MXC_F_FLC_CTRL_PAGE_ERASE_POS)) /**< CTRL_PAGE_ERASE Mask */

 #define MXC_F_FLC_CTRL_WIDTH_POS                       4 /**< CTRL_WIDTH Position */
 #define MXC_F_FLC_CTRL_WIDTH                           ((uint32_t)(0x1UL << MXC_F_FLC_CTRL_WIDTH_POS)) /**< CTRL_WIDTH Mask */

 #define MXC_F_FLC_CTRL_ERASE_CODE_POS                  8 /**< CTRL_ERASE_CODE Position */
 #define MXC_F_FLC_CTRL_ERASE_CODE                      ((uint32_t)(0xFFUL << MXC_F_FLC_CTRL_ERASE_CODE_POS)) /**< CTRL_ERASE_CODE Mask */
 #define MXC_V_FLC_CTRL_ERASE_CODE_NOP                  ((uint32_t)0x0UL) /**< CTRL_ERASE_CODE_NOP Value */
 #define MXC_S_FLC_CTRL_ERASE_CODE_NOP                  (MXC_V_FLC_CTRL_ERASE_CODE_NOP << MXC_F_FLC_CTRL_ERASE_CODE_POS) /**< CTRL_ERASE_CODE_NOP Setting */
 #define MXC_V_FLC_CTRL_ERASE_CODE_ERASEPAGE            ((uint32_t)0x55UL) /**< CTRL_ERASE_CODE_ERASEPAGE Value */
 #define MXC_S_FLC_CTRL_ERASE_CODE_ERASEPAGE            (MXC_V_FLC_CTRL_ERASE_CODE_ERASEPAGE << MXC_F_FLC_CTRL_ERASE_CODE_POS) /**< CTRL_ERASE_CODE_ERASEPAGE Setting */
 #define MXC_V_FLC_CTRL_ERASE_CODE_ERASEALL             ((uint32_t)0xAAUL) /**< CTRL_ERASE_CODE_ERASEALL Value */
 #define MXC_S_FLC_CTRL_ERASE_CODE_ERASEALL             (MXC_V_FLC_CTRL_ERASE_CODE_ERASEALL << MXC_F_FLC_CTRL_ERASE_CODE_POS) /**< CTRL_ERASE_CODE_ERASEALL Setting */

 #define MXC_F_FLC_CTRL_BUSY_POS                        24 /**< CTRL_BUSY Position */
 #define MXC_F_FLC_CTRL_BUSY                            ((uint32_t)(0x1UL << MXC_F_FLC_CTRL_BUSY_POS)) /**< CTRL_BUSY Mask */

 #define MXC_F_FLC_CTRL_LVE_POS                         25 /**< CTRL_LVE Position */
 #define MXC_F_FLC_CTRL_LVE                             ((uint32_t)(0x1UL << MXC_F_FLC_CTRL_LVE_POS)) /**< CTRL_LVE Mask */

 #define MXC_F_FLC_CTRL_UNLOCK_CODE_POS                 28 /**< CTRL_UNLOCK_CODE Position */
 #define MXC_F_FLC_CTRL_UNLOCK_CODE                     ((uint32_t)(0xFUL << MXC_F_FLC_CTRL_UNLOCK_CODE_POS)) /**< CTRL_UNLOCK_CODE Mask */
 #define MXC_V_FLC_CTRL_UNLOCK_CODE_UNLOCKED            ((uint32_t)0x2UL) /**< CTRL_UNLOCK_CODE_UNLOCKED Value */
 #define MXC_S_FLC_CTRL_UNLOCK_CODE_UNLOCKED            (MXC_V_FLC_CTRL_UNLOCK_CODE_UNLOCKED << MXC_F_FLC_CTRL_UNLOCK_CODE_POS) /**< CTRL_UNLOCK_CODE_UNLOCKED Setting */
 #define MXC_V_FLC_CTRL_UNLOCK_CODE_LOCKED              ((uint32_t)0x3UL) /**< CTRL_UNLOCK_CODE_LOCKED Value */
 #define MXC_S_FLC_CTRL_UNLOCK_CODE_LOCKED              (MXC_V_FLC_CTRL_UNLOCK_CODE_LOCKED << MXC_F_FLC_CTRL_UNLOCK_CODE_POS) /**< CTRL_UNLOCK_CODE_LOCKED Setting */

/**@} end of group FLC_CTRL_Register */

/**
 * @ingroup  flc_registers
 * @defgroup FLC_INTR FLC_INTR
 * @brief    Flash Interrupt Register.
 * @{
 */
 #define MXC_F_FLC_INTR_DONE_POS                        0 /**< INTR_DONE Position */
 #define MXC_F_FLC_INTR_DONE                            ((uint32_t)(0x1UL << MXC_F_FLC_INTR_DONE_POS)) /**< INTR_DONE Mask */

 #define MXC_F_FLC_INTR_ACCESS_FAIL_POS                 1 /**< INTR_ACCESS_FAIL Position */
 #define MXC_F_FLC_INTR_ACCESS_FAIL                     ((uint32_t)(0x1UL << MXC_F_FLC_INTR_ACCESS_FAIL_POS)) /**< INTR_ACCESS_FAIL Mask */

 #define MXC_F_FLC_INTR_DONE_IE_POS                     8 /**< INTR_DONE_IE Position */
 #define MXC_F_FLC_INTR_DONE_IE                         ((uint32_t)(0x1UL << MXC_F_FLC_INTR_DONE_IE_POS)) /**< INTR_DONE_IE Mask */

 #define MXC_F_FLC_INTR_ACCESS_FAIL_IE_POS              9 /**< INTR_ACCESS_FAIL_IE Position */
 #define MXC_F_FLC_INTR_ACCESS_FAIL_IE                  ((uint32_t)(0x1UL << MXC_F_FLC_INTR_ACCESS_FAIL_IE_POS)) /**< INTR_ACCESS_FAIL_IE Mask */

/**@} end of group FLC_INTR_Register */

/**
 * @ingroup  flc_registers
 * @defgroup FLC_DATA FLC_DATA
 * @brief    Flash Write Data.
 * @{
 */
 #define MXC_F_FLC_DATA_DATA_POS                        0 /**< DATA_DATA Position */
 #define MXC_F_FLC_DATA_DATA                            ((uint32_t)(0xFFFFFFFFUL << MXC_F_FLC_DATA_DATA_POS)) /**< DATA_DATA Mask */

/**@} end of group FLC_DATA_Register */

/**
 * @ingroup  flc_registers
 * @defgroup FLC_ACTRL FLC_ACTRL
 * @brief    Access Control Register. Writing the ACTRL register with the following values in
 *           the order shown, allows read and write access to the system and user Information
 *           block:                 pflc-actrl = 0x3a7f5ca3;                 pflc-actrl =
 *           0xa1e34f20;                 pflc-actrl = 0x9608b2c1. When unlocked, a write of
 *           any word will disable access to system and user information block. Readback of
 *           this register is always zero.
 * @{
 */
 #define MXC_F_FLC_ACTRL_ACTRL_POS                      0 /**< ACTRL_ACTRL Position */
 #define MXC_F_FLC_ACTRL_ACTRL                          ((uint32_t)(0xFFFFFFFFUL << MXC_F_FLC_ACTRL_ACTRL_POS)) /**< ACTRL_ACTRL Mask */

/**@} end of group FLC_ACTRL_Register */

#ifdef __cplusplus
}
#endif

#endif /* _FLC_REGS_H_ */
