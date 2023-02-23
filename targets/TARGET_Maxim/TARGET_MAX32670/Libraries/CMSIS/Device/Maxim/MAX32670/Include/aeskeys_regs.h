/**
 * @file    aeskeys_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the AESKEYS Peripheral Module.
 * @note    This file is @generated.
 */

/******************************************************************************
 * Copyright (C) 2023 Maxim Integrated Products, Inc., All Rights Reserved.
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
 ******************************************************************************/

#ifndef LIBRARIES_CMSIS_DEVICE_MAXIM_MAX32670_INCLUDE_AESKEYS_REGS_H_
#define LIBRARIES_CMSIS_DEVICE_MAXIM_MAX32670_INCLUDE_AESKEYS_REGS_H_

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
 * @ingroup     aeskeys
 * @defgroup    aeskeys_registers AESKEYS_Registers
 * @brief       Registers, Bit Masks and Bit Positions for the AESKEYS Peripheral Module.
 * @details     AES Key Registers.
 */

/**
 * @ingroup aeskeys_registers
 * Structure type to access the AESKEYS Registers.
 */
typedef struct {
    __IO uint32_t key0;                 /**< <tt>\b 0x00:</tt> AESKEYS KEY0 Register */
    __IO uint32_t key1;                 /**< <tt>\b 0x04:</tt> AESKEYS KEY1 Register */
    __IO uint32_t key2;                 /**< <tt>\b 0x08:</tt> AESKEYS KEY2 Register */
    __IO uint32_t key3;                 /**< <tt>\b 0x0C:</tt> AESKEYS KEY3 Register */
    __IO uint32_t key4;                 /**< <tt>\b 0x10:</tt> AESKEYS KEY4 Register */
    __IO uint32_t key5;                 /**< <tt>\b 0x14:</tt> AESKEYS KEY5 Register */
    __IO uint32_t key6;                 /**< <tt>\b 0x18:</tt> AESKEYS KEY6 Register */
    __IO uint32_t key7;                 /**< <tt>\b 0x1C:</tt> AESKEYS KEY7 Register */
} mxc_aeskeys_regs_t;

/* Register offsets for module AESKEYS */
/**
 * @ingroup    aeskeys_registers
 * @defgroup   AESKEYS_Register_Offsets Register Offsets
 * @brief      AESKEYS Peripheral Register Offsets from the AESKEYS Base Peripheral Address.
 * @{
 */
#define MXC_R_AESKEYS_KEY0                 ((uint32_t)0x00000000UL) /**< Offset from AESKEYS Base Address: <tt> 0x0000</tt> */
#define MXC_R_AESKEYS_KEY1                 ((uint32_t)0x00000004UL) /**< Offset from AESKEYS Base Address: <tt> 0x0004</tt> */
#define MXC_R_AESKEYS_KEY2                 ((uint32_t)0x00000008UL) /**< Offset from AESKEYS Base Address: <tt> 0x0008</tt> */
#define MXC_R_AESKEYS_KEY3                 ((uint32_t)0x0000000CUL) /**< Offset from AESKEYS Base Address: <tt> 0x000C</tt> */
#define MXC_R_AESKEYS_KEY4                 ((uint32_t)0x00000010UL) /**< Offset from AESKEYS Base Address: <tt> 0x0010</tt> */
#define MXC_R_AESKEYS_KEY5                 ((uint32_t)0x00000014UL) /**< Offset from AESKEYS Base Address: <tt> 0x0014</tt> */
#define MXC_R_AESKEYS_KEY6                 ((uint32_t)0x00000018UL) /**< Offset from AESKEYS Base Address: <tt> 0x0018</tt> */
#define MXC_R_AESKEYS_KEY7                 ((uint32_t)0x0000001CUL) /**< Offset from AESKEYS Base Address: <tt> 0x001C</tt> */
/**@} end of group aeskeys_registers */

#ifdef __cplusplus
}
#endif

#endif // LIBRARIES_CMSIS_DEVICE_MAXIM_MAX32670_INCLUDE_AESKEYS_REGS_H_
