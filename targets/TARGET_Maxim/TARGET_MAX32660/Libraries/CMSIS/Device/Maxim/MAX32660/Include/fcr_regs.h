/**
 * @file    fcr_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the FCR Peripheral Module.
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

#ifndef _FCR_REGS_H_
#define _FCR_REGS_H_

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
 * @ingroup     fcr
 * @defgroup    fcr_registers FCR_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the FCR Peripheral Module.
 * @details Function Control.
 */

/**
 * @ingroup fcr_registers
 * Structure type to access the FCR Registers.
 */
typedef struct {
    __IO uint32_t reg0;                 /**< <tt>\b 0x00:</tt> FCR REG0 Register */
} mxc_fcr_regs_t;

/* Register offsets for module FCR */
/**
 * @ingroup    fcr_registers
 * @defgroup   FCR_Register_Offsets Register Offsets
 * @brief      FCR Peripheral Register Offsets from the FCR Base Peripheral Address. 
 * @{
 */
 #define MXC_R_FCR_REG0                     ((uint32_t)0x00000000UL) /**< Offset from FCR Base Address: <tt> 0x0000</tt> */ 
/**@} end of group fcr_registers */

/**
 * @ingroup  fcr_registers
 * @defgroup FCR_REG0 FCR_REG0
 * @brief    Register 0.
 * @{
 */
 #define MXC_F_FCR_REG0_I2C0_SDA_FILTER_EN_POS          20 /**< REG0_I2C0_SDA_FILTER_EN Position */
 #define MXC_F_FCR_REG0_I2C0_SDA_FILTER_EN              ((uint32_t)(0x1UL << MXC_F_FCR_REG0_I2C0_SDA_FILTER_EN_POS)) /**< REG0_I2C0_SDA_FILTER_EN Mask */

 #define MXC_F_FCR_REG0_I2C0_SCL_FILTER_EN_POS          21 /**< REG0_I2C0_SCL_FILTER_EN Position */
 #define MXC_F_FCR_REG0_I2C0_SCL_FILTER_EN              ((uint32_t)(0x1UL << MXC_F_FCR_REG0_I2C0_SCL_FILTER_EN_POS)) /**< REG0_I2C0_SCL_FILTER_EN Mask */

 #define MXC_F_FCR_REG0_I2C1_SDA_FILTER_EN_POS          22 /**< REG0_I2C1_SDA_FILTER_EN Position */
 #define MXC_F_FCR_REG0_I2C1_SDA_FILTER_EN              ((uint32_t)(0x1UL << MXC_F_FCR_REG0_I2C1_SDA_FILTER_EN_POS)) /**< REG0_I2C1_SDA_FILTER_EN Mask */

 #define MXC_F_FCR_REG0_I2C1_SCL_FILTER_EN_POS          23 /**< REG0_I2C1_SCL_FILTER_EN Position */
 #define MXC_F_FCR_REG0_I2C1_SCL_FILTER_EN              ((uint32_t)(0x1UL << MXC_F_FCR_REG0_I2C1_SCL_FILTER_EN_POS)) /**< REG0_I2C1_SCL_FILTER_EN Mask */

/**@} end of group FCR_REG0_Register */

#ifdef __cplusplus
}
#endif

#endif /* _FCR_REGS_H_ */
