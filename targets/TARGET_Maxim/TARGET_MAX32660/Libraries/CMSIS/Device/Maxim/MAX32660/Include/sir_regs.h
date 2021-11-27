/**
 * @file    sir_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the SIR Peripheral Module.
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

#ifndef _SIR_REGS_H_
#define _SIR_REGS_H_

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
 * @ingroup     sir
 * @defgroup    sir_registers SIR_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the SIR Peripheral Module.
 * @details System Initialization Registers.
 */

/**
 * @ingroup sir_registers
 * Structure type to access the SIR Registers.
 */
typedef struct {
    __I  uint32_t status;               /**< <tt>\b 0x00:</tt> SIR STATUS Register */
    __I  uint32_t addr;                 /**< <tt>\b 0x04:</tt> SIR ADDR Register */
} mxc_sir_regs_t;

/* Register offsets for module SIR */
/**
 * @ingroup    sir_registers
 * @defgroup   SIR_Register_Offsets Register Offsets
 * @brief      SIR Peripheral Register Offsets from the SIR Base Peripheral Address. 
 * @{
 */
 #define MXC_R_SIR_STATUS                   ((uint32_t)0x00000000UL) /**< Offset from SIR Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_SIR_ADDR                     ((uint32_t)0x00000004UL) /**< Offset from SIR Base Address: <tt> 0x0004</tt> */ 
/**@} end of group sir_registers */

/**
 * @ingroup  sir_registers
 * @defgroup SIR_STATUS SIR_STATUS
 * @brief    System Initialization Status Register.
 * @{
 */
 #define MXC_F_SIR_STATUS_CFG_VALID_POS                 0 /**< STATUS_CFG_VALID Position */
 #define MXC_F_SIR_STATUS_CFG_VALID                     ((uint32_t)(0x1UL << MXC_F_SIR_STATUS_CFG_VALID_POS)) /**< STATUS_CFG_VALID Mask */

 #define MXC_F_SIR_STATUS_CFG_ERR_POS                   1 /**< STATUS_CFG_ERR Position */
 #define MXC_F_SIR_STATUS_CFG_ERR                       ((uint32_t)(0x1UL << MXC_F_SIR_STATUS_CFG_ERR_POS)) /**< STATUS_CFG_ERR Mask */

/**@} end of group SIR_STATUS_Register */

/**
 * @ingroup  sir_registers
 * @defgroup SIR_ADDR SIR_ADDR
 * @brief    Read-only field set by the SIB block if a CRC error occurs during the read of
 *           the OTP memory. Contains the failing address in OTP memory (when CRCERR equals
 *           1).
 * @{
 */
 #define MXC_F_SIR_ADDR_ADDR_POS                        0 /**< ADDR_ADDR Position */
 #define MXC_F_SIR_ADDR_ADDR                            ((uint32_t)(0xFFFFFFFFUL << MXC_F_SIR_ADDR_ADDR_POS)) /**< ADDR_ADDR Mask */

/**@} end of group SIR_ADDR_Register */

#ifdef __cplusplus
}
#endif

#endif /* _SIR_REGS_H_ */
