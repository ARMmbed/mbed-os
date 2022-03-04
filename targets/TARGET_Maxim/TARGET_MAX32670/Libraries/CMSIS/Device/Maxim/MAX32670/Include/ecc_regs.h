/**
 * @file    ecc_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the ECC Peripheral Module.
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

#ifndef _ECC_REGS_H_
#define _ECC_REGS_H_

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
 * @ingroup     ecc
 * @defgroup    ecc_registers ECC_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the ECC Peripheral Module.
 * @details Error Correction Code
 */

/**
 * @ingroup ecc_registers
 * Structure type to access the ECC Registers.
 */
typedef struct {
    __I  uint32_t rsv_0x0_0x7[2];
    __IO uint32_t en;                   /**< <tt>\b 0x08:</tt> ECC EN Register */
} mxc_ecc_regs_t;

/* Register offsets for module ECC */
/**
 * @ingroup    ecc_registers
 * @defgroup   ECC_Register_Offsets Register Offsets
 * @brief      ECC Peripheral Register Offsets from the ECC Base Peripheral Address. 
 * @{
 */
 #define MXC_R_ECC_EN                       ((uint32_t)0x00000008UL) /**< Offset from ECC Base Address: <tt> 0x0008</tt> */ 
/**@} end of group ecc_registers */

/**
 * @ingroup  ecc_registers
 * @defgroup ECC_EN ECC_EN
 * @brief    ECC Enable Register
 * @{
 */
 #define MXC_F_ECC_EN_RAM_POS                           8 /**< EN_RAM Position */
 #define MXC_F_ECC_EN_RAM                               ((uint32_t)(0x1UL << MXC_F_ECC_EN_RAM_POS)) /**< EN_RAM Mask */

 #define MXC_F_ECC_EN_ICC0_POS                          9 /**< EN_ICC0 Position */
 #define MXC_F_ECC_EN_ICC0                              ((uint32_t)(0x1UL << MXC_F_ECC_EN_ICC0_POS)) /**< EN_ICC0 Mask */

 #define MXC_F_ECC_EN_FLASH_POS                         10 /**< EN_FLASH Position */
 #define MXC_F_ECC_EN_FLASH                             ((uint32_t)(0x1UL << MXC_F_ECC_EN_FLASH_POS)) /**< EN_FLASH Mask */

/**@} end of group ECC_EN_Register */

#ifdef __cplusplus
}
#endif

#endif /* _ECC_REGS_H_ */
