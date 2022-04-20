/**
 * @file    icc_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the ICC Peripheral Module.
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

#ifndef _ICC_REGS_H_
#define _ICC_REGS_H_

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
 * @ingroup     icc
 * @defgroup    icc_registers ICC_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the ICC Peripheral Module.
 * @details Instruction Cache Controller Registers
 */

/**
 * @ingroup icc_registers
 * Structure type to access the ICC Registers.
 */
typedef struct {
    __I  uint32_t info;                 /**< <tt>\b 0x0000:</tt> ICC INFO Register */
    __I  uint32_t sz;                   /**< <tt>\b 0x0004:</tt> ICC SZ Register */
    __I  uint32_t rsv_0x8_0xff[62];
    __IO uint32_t ctrl;                 /**< <tt>\b 0x0100:</tt> ICC CTRL Register */
    __I  uint32_t rsv_0x104_0x6ff[383];
    __IO uint32_t invalidate;           /**< <tt>\b 0x0700:</tt> ICC INVALIDATE Register */
} mxc_icc_regs_t;

/* Register offsets for module ICC */
/**
 * @ingroup    icc_registers
 * @defgroup   ICC_Register_Offsets Register Offsets
 * @brief      ICC Peripheral Register Offsets from the ICC Base Peripheral Address. 
 * @{
 */
 #define MXC_R_ICC_INFO                     ((uint32_t)0x00000000UL) /**< Offset from ICC Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_ICC_SZ                       ((uint32_t)0x00000004UL) /**< Offset from ICC Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_ICC_CTRL                     ((uint32_t)0x00000100UL) /**< Offset from ICC Base Address: <tt> 0x0100</tt> */ 
 #define MXC_R_ICC_INVALIDATE               ((uint32_t)0x00000700UL) /**< Offset from ICC Base Address: <tt> 0x0700</tt> */ 
/**@} end of group icc_registers */

/**
 * @ingroup  icc_registers
 * @defgroup ICC_INFO ICC_INFO
 * @brief    Cache ID Register.
 * @{
 */
 #define MXC_F_ICC_INFO_RELNUM_POS                      0 /**< INFO_RELNUM Position */
 #define MXC_F_ICC_INFO_RELNUM                          ((uint32_t)(0x3FUL << MXC_F_ICC_INFO_RELNUM_POS)) /**< INFO_RELNUM Mask */

 #define MXC_F_ICC_INFO_PARTNUM_POS                     6 /**< INFO_PARTNUM Position */
 #define MXC_F_ICC_INFO_PARTNUM                         ((uint32_t)(0xFUL << MXC_F_ICC_INFO_PARTNUM_POS)) /**< INFO_PARTNUM Mask */

 #define MXC_F_ICC_INFO_ID_POS                          10 /**< INFO_ID Position */
 #define MXC_F_ICC_INFO_ID                              ((uint32_t)(0x3FUL << MXC_F_ICC_INFO_ID_POS)) /**< INFO_ID Mask */

/**@} end of group ICC_INFO_Register */

/**
 * @ingroup  icc_registers
 * @defgroup ICC_SZ ICC_SZ
 * @brief    Memory Configuration Register.
 * @{
 */
 #define MXC_F_ICC_SZ_CCH_POS                           0 /**< SZ_CCH Position */
 #define MXC_F_ICC_SZ_CCH                               ((uint32_t)(0xFFFFUL << MXC_F_ICC_SZ_CCH_POS)) /**< SZ_CCH Mask */

 #define MXC_F_ICC_SZ_MEM_POS                           16 /**< SZ_MEM Position */
 #define MXC_F_ICC_SZ_MEM                               ((uint32_t)(0xFFFFUL << MXC_F_ICC_SZ_MEM_POS)) /**< SZ_MEM Mask */

/**@} end of group ICC_SZ_Register */

/**
 * @ingroup  icc_registers
 * @defgroup ICC_CTRL ICC_CTRL
 * @brief    Cache Control and Status Register.
 * @{
 */
 #define MXC_F_ICC_CTRL_EN_POS                          0 /**< CTRL_EN Position */
 #define MXC_F_ICC_CTRL_EN                              ((uint32_t)(0x1UL << MXC_F_ICC_CTRL_EN_POS)) /**< CTRL_EN Mask */

 #define MXC_F_ICC_CTRL_RDY_POS                         16 /**< CTRL_RDY Position */
 #define MXC_F_ICC_CTRL_RDY                             ((uint32_t)(0x1UL << MXC_F_ICC_CTRL_RDY_POS)) /**< CTRL_RDY Mask */

/**@} end of group ICC_CTRL_Register */

/**
 * @ingroup  icc_registers
 * @defgroup ICC_INVALIDATE ICC_INVALIDATE
 * @brief    Invalidate All Registers.
 * @{
 */
 #define MXC_F_ICC_INVALIDATE_INVALID_POS               0 /**< INVALIDATE_INVALID Position */
 #define MXC_F_ICC_INVALIDATE_INVALID                   ((uint32_t)(0xFFFFFFFFUL << MXC_F_ICC_INVALIDATE_INVALID_POS)) /**< INVALIDATE_INVALID Mask */

/**@} end of group ICC_INVALIDATE_Register */

#ifdef __cplusplus
}
#endif

#endif /* _ICC_REGS_H_ */
