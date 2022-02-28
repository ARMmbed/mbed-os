/**
 * @file    trng_revb_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the TRNG_REVB Peripheral Module.
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

#ifndef _TRNG_REVB_REGS_H_
#define _TRNG_REVB_REGS_H_

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
 * @ingroup     trng_revb
 * @defgroup    trng_revb_registers TRNG_REVB_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the TRNG_REVB Peripheral Module.
 * @details Random Number Generator.
 */

/**
 * @ingroup trng_revb_registers
 * Structure type to access the TRNG_REVB Registers.
 */
typedef struct {
    __IO uint32_t ctrl;                 /**< <tt>\b 0x00:</tt> TRNG_REVB CTRL Register */
    __I  uint32_t status;               /**< <tt>\b 0x04:</tt> TRNG_REVB STATUS Register */
    __I  uint32_t data;                 /**< <tt>\b 0x08:</tt> TRNG_REVB DATA Register */
} mxc_trng_revb_regs_t;

/* Register offsets for module TRNG_REVB */
/**
 * @ingroup    trng_revb_registers
 * @defgroup   TRNG_REVB_Register_Offsets Register Offsets
 * @brief      TRNG_REVB Peripheral Register Offsets from the TRNG_REVB Base Peripheral Address. 
 * @{
 */
 #define MXC_R_TRNG_REVB_CTRL               ((uint32_t)0x00000000UL) /**< Offset from TRNG_REVB Base Address: <tt> 0x0000</tt> */ 
 #define MXC_R_TRNG_REVB_STATUS             ((uint32_t)0x00000004UL) /**< Offset from TRNG_REVB Base Address: <tt> 0x0004</tt> */ 
 #define MXC_R_TRNG_REVB_DATA               ((uint32_t)0x00000008UL) /**< Offset from TRNG_REVB Base Address: <tt> 0x0008</tt> */ 
/**@} end of group trng_revb_registers */

/**
 * @ingroup  trng_revb_registers
 * @defgroup TRNG_REVB_CTRL TRNG_REVB_CTRL
 * @brief    TRNG Control Register.
 * @{
 */
 #define MXC_F_TRNG_REVB_CTRL_RND_IE_POS                1 /**< CTRL_RND_IE Position */
 #define MXC_F_TRNG_REVB_CTRL_RND_IE                    ((uint32_t)(0x1UL << MXC_F_TRNG_REVB_CTRL_RND_IE_POS)) /**< CTRL_RND_IE Mask */

 #define MXC_F_TRNG_REVB_CTRL_KEYGEN_POS                3 /**< CTRL_KEYGEN Position */
 #define MXC_F_TRNG_REVB_CTRL_KEYGEN                    ((uint32_t)(0x1UL << MXC_F_TRNG_REVB_CTRL_KEYGEN_POS)) /**< CTRL_KEYGEN Mask */

 #define MXC_F_TRNG_REVB_CTRL_KEYWIPE_POS               15 /**< CTRL_KEYWIPE Position */
 #define MXC_F_TRNG_REVB_CTRL_KEYWIPE                   ((uint32_t)(0x1UL << MXC_F_TRNG_REVB_CTRL_KEYWIPE_POS)) /**< CTRL_KEYWIPE Mask */

/**@} end of group TRNG_REVB_CTRL_Register */

/**
 * @ingroup  trng_revb_registers
 * @defgroup TRNG_REVB_STATUS TRNG_REVB_STATUS
 * @brief    Data. The content of this register is valid only when RNG_IS = 1. When TRNG is
 *           disabled, read returns 0x0000 0000.
 * @{
 */
 #define MXC_F_TRNG_REVB_STATUS_RDY_POS                 0 /**< STATUS_RDY Position */
 #define MXC_F_TRNG_REVB_STATUS_RDY                     ((uint32_t)(0x1UL << MXC_F_TRNG_REVB_STATUS_RDY_POS)) /**< STATUS_RDY Mask */

/**@} end of group TRNG_REVB_STATUS_Register */

/**
 * @ingroup  trng_revb_registers
 * @defgroup TRNG_REVB_DATA TRNG_REVB_DATA
 * @brief    Data. The content of this register is valid only when RNG_IS = 1. When TRNG is
 *           disabled, read returns 0x0000 0000.
 * @{
 */
 #define MXC_F_TRNG_REVB_DATA_DATA_POS                  0 /**< DATA_DATA Position */
 #define MXC_F_TRNG_REVB_DATA_DATA                      ((uint32_t)(0xFFFFFFFFUL << MXC_F_TRNG_REVB_DATA_DATA_POS)) /**< DATA_DATA Mask */

/**@} end of group TRNG_REVB_DATA_Register */

#ifdef __cplusplus
}
#endif

#endif /* _TRNG_REVB_REGS_H_ */
