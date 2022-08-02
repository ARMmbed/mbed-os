/**
 * @file    flc_revb_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the FLC_REVB Peripheral Module.
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

#ifndef _FLC_REVB_REGS_H_
#define _FLC_REVB_REGS_H_

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
#ifndef __R
#define __R  volatile const
#endif
/// @endcond

/* **** Definitions **** */

/**
 * @ingroup     flc_revb
 * @defgroup    flc_revb_registers FLC_REVB_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the FLC_REVB Peripheral Module.
 * @details Flash Memory Control.
 */

/**
 * @ingroup flc_revb_registers
 * Structure type to access the FLC_REVB Registers.
 */
typedef struct {
    __IO uint32_t addr;                 /**< <tt>\b 0x00:</tt> FLC_REVB ADDR Register */
    __IO uint32_t clkdiv;               /**< <tt>\b 0x04:</tt> FLC_REVB CLKDIV Register */
    __IO uint32_t ctrl;                 /**< <tt>\b 0x08:</tt> FLC_REVB CTRL Register */
    __R  uint32_t rsv_0xc_0x23[6];
    __IO uint32_t intr;                 /**< <tt>\b 0x024:</tt> FLC_REVB INTR Register */
    __IO uint32_t eccdata;              /**< <tt>\b 0x028:</tt> FLC_REVB ECCDATA Register */
    __R  uint32_t rsv_0x2c;
    __IO uint32_t data[4];              /**< <tt>\b 0x30:</tt> FLC_REVB DATA Register */
    __O  uint32_t actrl;                /**< <tt>\b 0x40:</tt> FLC_REVB ACTRL Register */
    __R  uint32_t rsv_0x44_0x7f[15];
    __IO uint32_t welr0;                /**< <tt>\b 0x80:</tt> FLC_REVB WELR0 Register */
    __R  uint32_t rsv_0x84;
    __IO uint32_t welr1;                /**< <tt>\b 0x88:</tt> FLC_REVB WELR1 Register */
    __R  uint32_t rsv_0x8c;
    __IO uint32_t rlr0;                 /**< <tt>\b 0x90:</tt> FLC_REVB RLR0 Register */
    __R  uint32_t rsv_0x94;
    __IO uint32_t rlr1;                 /**< <tt>\b 0x98:</tt> FLC_REVB RLR1 Register */
} mxc_flc_revb_regs_t;

/* Register offsets for module FLC_REVB */
/**
 * @ingroup    flc_revb_registers
 * @defgroup   FLC_REVB_Register_Offsets Register Offsets
 * @brief      FLC_REVB Peripheral Register Offsets from the FLC_REVB Base Peripheral Address. 
 * @{
 */
 #define MXC_R_FLC_REVB_ADDR                ((uint32_t)0x00000000UL) /**< Offset from FLC_REVB Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_FLC_REVB_CLKDIV              ((uint32_t)0x00000004UL) /**< Offset from FLC_REVB Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_FLC_REVB_CTRL                ((uint32_t)0x00000008UL) /**< Offset from FLC_REVB Base Address: <tt> 0x0008</tt> */ 
 #define MXC_R_FLC_REVB_INTR                ((uint32_t)0x00000024UL) /**< Offset from FLC_REVB Base Address: <tt> 0x0024</tt> */ 
 #define MXC_R_FLC_REVB_ECCDATA             ((uint32_t)0x00000028UL) /**< Offset from FLC_REVB Base Address: <tt> 0x0028</tt> */ 
 #define MXC_R_FLC_REVB_DATA                ((uint32_t)0x00000030UL) /**< Offset from FLC_REVB Base Address: <tt> 0x0030</tt> */ 
 #define MXC_R_FLC_REVB_ACTRL               ((uint32_t)0x00000040UL) /**< Offset from FLC_REVB Base Address: <tt> 0x0040</tt> */ 
 #define MXC_R_FLC_REVB_WELR0               ((uint32_t)0x00000080UL) /**< Offset from FLC_REVB Base Address: <tt> 0x0080</tt> */ 
 #define MXC_R_FLC_REVB_WELR1               ((uint32_t)0x00000088UL) /**< Offset from FLC_REVB Base Address: <tt> 0x0088</tt> */ 
 #define MXC_R_FLC_REVB_RLR0                ((uint32_t)0x00000090UL) /**< Offset from FLC_REVB Base Address: <tt> 0x0090</tt> */ 
 #define MXC_R_FLC_REVB_RLR1                ((uint32_t)0x00000098UL) /**< Offset from FLC_REVB Base Address: <tt> 0x0098</tt> */ 
/**@} end of group flc_revb_registers */

/**
 * @ingroup  flc_revb_registers
 * @defgroup FLC_REVB_ADDR FLC_REVB_ADDR
 * @brief    Flash Write Address.
 * @{
 */
 #define MXC_F_FLC_REVB_ADDR_ADDR_POS                   0 /**< ADDR_ADDR Position */
 #define MXC_F_FLC_REVB_ADDR_ADDR                       ((uint32_t)(0xFFFFFFFFUL << MXC_F_FLC_REVB_ADDR_ADDR_POS)) /**< ADDR_ADDR Mask */

/**@} end of group FLC_REVB_ADDR_Register */

/**
 * @ingroup  flc_revb_registers
 * @defgroup FLC_REVB_CLKDIV FLC_REVB_CLKDIV
 * @brief    Flash Clock Divide. The clock (PLL0) is divided by this value to generate a 1
 *           MHz clock for Flash controller.
 * @{
 */
 #define MXC_F_FLC_REVB_CLKDIV_CLKDIV_POS               0 /**< CLKDIV_CLKDIV Position */
 #define MXC_F_FLC_REVB_CLKDIV_CLKDIV                   ((uint32_t)(0xFFUL << MXC_F_FLC_REVB_CLKDIV_CLKDIV_POS)) /**< CLKDIV_CLKDIV Mask */

/**@} end of group FLC_REVB_CLKDIV_Register */

/**
 * @ingroup  flc_revb_registers
 * @defgroup FLC_REVB_CTRL FLC_REVB_CTRL
 * @brief    Flash Control Register.
 * @{
 */
 #define MXC_F_FLC_REVB_CTRL_WR_POS                     0 /**< CTRL_WR Position */
 #define MXC_F_FLC_REVB_CTRL_WR                         ((uint32_t)(0x1UL << MXC_F_FLC_REVB_CTRL_WR_POS)) /**< CTRL_WR Mask */

 #define MXC_F_FLC_REVB_CTRL_ME_POS                     1 /**< CTRL_ME Position */
 #define MXC_F_FLC_REVB_CTRL_ME                         ((uint32_t)(0x1UL << MXC_F_FLC_REVB_CTRL_ME_POS)) /**< CTRL_ME Mask */

 #define MXC_F_FLC_REVB_CTRL_PGE_POS                    2 /**< CTRL_PGE Position */
 #define MXC_F_FLC_REVB_CTRL_PGE                        ((uint32_t)(0x1UL << MXC_F_FLC_REVB_CTRL_PGE_POS)) /**< CTRL_PGE Mask */

 #define MXC_F_FLC_REVB_CTRL_WDTH_POS                   4 /**< CTRL_WDTH Position */
 #define MXC_F_FLC_REVB_CTRL_WDTH                       ((uint32_t)(0x1UL << MXC_F_FLC_REVB_CTRL_WDTH_POS)) /**< CTRL_WDTH Mask */

 #define MXC_F_FLC_REVB_CTRL_ERASE_CODE_POS             8 /**< CTRL_ERASE_CODE Position */
 #define MXC_F_FLC_REVB_CTRL_ERASE_CODE                 ((uint32_t)(0xFFUL << MXC_F_FLC_REVB_CTRL_ERASE_CODE_POS)) /**< CTRL_ERASE_CODE Mask */
 #define MXC_V_FLC_REVB_CTRL_ERASE_CODE_NOP             ((uint32_t)0x0UL) /**< CTRL_ERASE_CODE_NOP Value */
 #define MXC_S_FLC_REVB_CTRL_ERASE_CODE_NOP             (MXC_V_FLC_REVB_CTRL_ERASE_CODE_NOP << MXC_F_FLC_REVB_CTRL_ERASE_CODE_POS) /**< CTRL_ERASE_CODE_NOP Setting */
 #define MXC_V_FLC_REVB_CTRL_ERASE_CODE_ERASEPAGE       ((uint32_t)0x55UL) /**< CTRL_ERASE_CODE_ERASEPAGE Value */
 #define MXC_S_FLC_REVB_CTRL_ERASE_CODE_ERASEPAGE       (MXC_V_FLC_REVB_CTRL_ERASE_CODE_ERASEPAGE << MXC_F_FLC_REVB_CTRL_ERASE_CODE_POS) /**< CTRL_ERASE_CODE_ERASEPAGE Setting */
 #define MXC_V_FLC_REVB_CTRL_ERASE_CODE_ERASEALL        ((uint32_t)0xAAUL) /**< CTRL_ERASE_CODE_ERASEALL Value */
 #define MXC_S_FLC_REVB_CTRL_ERASE_CODE_ERASEALL        (MXC_V_FLC_REVB_CTRL_ERASE_CODE_ERASEALL << MXC_F_FLC_REVB_CTRL_ERASE_CODE_POS) /**< CTRL_ERASE_CODE_ERASEALL Setting */

 #define MXC_F_FLC_REVB_CTRL_PEND_POS                   24 /**< CTRL_PEND Position */
 #define MXC_F_FLC_REVB_CTRL_PEND                       ((uint32_t)(0x1UL << MXC_F_FLC_REVB_CTRL_PEND_POS)) /**< CTRL_PEND Mask */

 #define MXC_F_FLC_REVB_CTRL_LVE_POS                    25 /**< CTRL_LVE Position */
 #define MXC_F_FLC_REVB_CTRL_LVE                        ((uint32_t)(0x1UL << MXC_F_FLC_REVB_CTRL_LVE_POS)) /**< CTRL_LVE Mask */

 #define MXC_F_FLC_REVB_CTRL_UNLOCK_POS                 28 /**< CTRL_UNLOCK Position */
 #define MXC_F_FLC_REVB_CTRL_UNLOCK                     ((uint32_t)(0xFUL << MXC_F_FLC_REVB_CTRL_UNLOCK_POS)) /**< CTRL_UNLOCK Mask */
 #define MXC_V_FLC_REVB_CTRL_UNLOCK_UNLOCKED            ((uint32_t)0x2UL) /**< CTRL_UNLOCK_UNLOCKED Value */
 #define MXC_S_FLC_REVB_CTRL_UNLOCK_UNLOCKED            (MXC_V_FLC_REVB_CTRL_UNLOCK_UNLOCKED << MXC_F_FLC_REVB_CTRL_UNLOCK_POS) /**< CTRL_UNLOCK_UNLOCKED Setting */
 #define MXC_V_FLC_REVB_CTRL_UNLOCK_LOCKED              ((uint32_t)0x3UL) /**< CTRL_UNLOCK_LOCKED Value */
 #define MXC_S_FLC_REVB_CTRL_UNLOCK_LOCKED              (MXC_V_FLC_REVB_CTRL_UNLOCK_LOCKED << MXC_F_FLC_REVB_CTRL_UNLOCK_POS) /**< CTRL_UNLOCK_LOCKED Setting */

/**@} end of group FLC_REVB_CTRL_Register */

/**
 * @ingroup  flc_revb_registers
 * @defgroup FLC_REVB_INTR FLC_REVB_INTR
 * @brief    Flash Interrupt Register.
 * @{
 */
 #define MXC_F_FLC_REVB_INTR_DONE_POS                   0 /**< INTR_DONE Position */
 #define MXC_F_FLC_REVB_INTR_DONE                       ((uint32_t)(0x1UL << MXC_F_FLC_REVB_INTR_DONE_POS)) /**< INTR_DONE Mask */

 #define MXC_F_FLC_REVB_INTR_AF_POS                     1 /**< INTR_AF Position */
 #define MXC_F_FLC_REVB_INTR_AF                         ((uint32_t)(0x1UL << MXC_F_FLC_REVB_INTR_AF_POS)) /**< INTR_AF Mask */

 #define MXC_F_FLC_REVB_INTR_DONEIE_POS                 8 /**< INTR_DONEIE Position */
 #define MXC_F_FLC_REVB_INTR_DONEIE                     ((uint32_t)(0x1UL << MXC_F_FLC_REVB_INTR_DONEIE_POS)) /**< INTR_DONEIE Mask */

 #define MXC_F_FLC_REVB_INTR_AFIE_POS                   9 /**< INTR_AFIE Position */
 #define MXC_F_FLC_REVB_INTR_AFIE                       ((uint32_t)(0x1UL << MXC_F_FLC_REVB_INTR_AFIE_POS)) /**< INTR_AFIE Mask */

/**@} end of group FLC_REVB_INTR_Register */

/**
 * @ingroup  flc_revb_registers
 * @defgroup FLC_REVB_ECCDATA FLC_REVB_ECCDATA
 * @brief    ECC Data Register.
 * @{
 */
 #define MXC_F_FLC_REVB_ECCDATA_EVEN_POS                0 /**< ECCDATA_EVEN Position */
 #define MXC_F_FLC_REVB_ECCDATA_EVEN                    ((uint32_t)(0x1FFUL << MXC_F_FLC_REVB_ECCDATA_EVEN_POS)) /**< ECCDATA_EVEN Mask */

 #define MXC_F_FLC_REVB_ECCDATA_ODD_POS                 16 /**< ECCDATA_ODD Position */
 #define MXC_F_FLC_REVB_ECCDATA_ODD                     ((uint32_t)(0x1FFUL << MXC_F_FLC_REVB_ECCDATA_ODD_POS)) /**< ECCDATA_ODD Mask */

/**@} end of group FLC_REVB_ECCDATA_Register */

/**
 * @ingroup  flc_revb_registers
 * @defgroup FLC_REVB_DATA FLC_REVB_DATA
 * @brief    Flash Write Data.
 * @{
 */
 #define MXC_F_FLC_REVB_DATA_DATA_POS                   0 /**< DATA_DATA Position */
 #define MXC_F_FLC_REVB_DATA_DATA                       ((uint32_t)(0xFFFFFFFFUL << MXC_F_FLC_REVB_DATA_DATA_POS)) /**< DATA_DATA Mask */

/**@} end of group FLC_REVB_DATA_Register */

/**
 * @ingroup  flc_revb_registers
 * @defgroup FLC_REVB_ACTRL FLC_REVB_ACTRL
 * @brief    Access Control Register. Writing the ACTRL register with the following values in
 *           the order shown, allows read and write access to the system and user Information
 *           block:     pflc-actrl = 0x3a7f5ca3;     pflc-actrl = 0xa1e34f20;     pflc-actrl
 *           = 0x9608b2c1. When unlocked, a write of any word will disable access to system
 *           and user information block. Readback of this register is always zero.
 * @{
 */
 #define MXC_F_FLC_REVB_ACTRL_ACTRL_POS                 0 /**< ACTRL_ACTRL Position */
 #define MXC_F_FLC_REVB_ACTRL_ACTRL                     ((uint32_t)(0xFFFFFFFFUL << MXC_F_FLC_REVB_ACTRL_ACTRL_POS)) /**< ACTRL_ACTRL Mask */

/**@} end of group FLC_REVB_ACTRL_Register */

/**
 * @ingroup  flc_revb_registers
 * @defgroup FLC_REVB_WELR0 FLC_REVB_WELR0
 * @brief    WELR0
 * @{
 */
 #define MXC_F_FLC_REVB_WELR0_WELR0_POS                 0 /**< WELR0_WELR0 Position */
 #define MXC_F_FLC_REVB_WELR0_WELR0                     ((uint32_t)(0xFFFFFFFFUL << MXC_F_FLC_REVB_WELR0_WELR0_POS)) /**< WELR0_WELR0 Mask */

/**@} end of group FLC_REVB_WELR0_Register */

/**
 * @ingroup  flc_revb_registers
 * @defgroup FLC_REVB_WELR1 FLC_REVB_WELR1
 * @brief    WELR1
 * @{
 */
 #define MXC_F_FLC_REVB_WELR1_WELR1_POS                 0 /**< WELR1_WELR1 Position */
 #define MXC_F_FLC_REVB_WELR1_WELR1                     ((uint32_t)(0xFFFFFFFFUL << MXC_F_FLC_REVB_WELR1_WELR1_POS)) /**< WELR1_WELR1 Mask */

/**@} end of group FLC_REVB_WELR1_Register */

/**
 * @ingroup  flc_revb_registers
 * @defgroup FLC_REVB_RLR0 FLC_REVB_RLR0
 * @brief    RLR0
 * @{
 */
 #define MXC_F_FLC_REVB_RLR0_RLR0_POS                   0 /**< RLR0_RLR0 Position */
 #define MXC_F_FLC_REVB_RLR0_RLR0                       ((uint32_t)(0xFFFFFFFFUL << MXC_F_FLC_REVB_RLR0_RLR0_POS)) /**< RLR0_RLR0 Mask */

/**@} end of group FLC_REVB_RLR0_Register */

/**
 * @ingroup  flc_revb_registers
 * @defgroup FLC_REVB_RLR1 FLC_REVB_RLR1
 * @brief    RLR1
 * @{
 */
 #define MXC_F_FLC_REVB_RLR1_RLR1_POS                   0 /**< RLR1_RLR1 Position */
 #define MXC_F_FLC_REVB_RLR1_RLR1                       ((uint32_t)(0xFFFFFFFFUL << MXC_F_FLC_REVB_RLR1_RLR1_POS)) /**< RLR1_RLR1 Mask */

/**@} end of group FLC_REVB_RLR1_Register */

#ifdef __cplusplus
}
#endif

#endif /* _FLC_REVB_REGS_H_ */
