/**
 ******************************************************************************
 * @file aes_map.h
 * @brief AES HW register map
 * @internal
 * @author ON Semiconductor.
 * $Rev: 2110 $
 * $Date: 2013-07-16 20:13:03 +0530 (Tue, 16 Jul 2013) $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup aes
 *
 * @details
 * <p>
 * AES HW register map description
 * </p>
 */

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#ifndef AES_MAP_H_
#define AES_MAP_H_

#include "architecture.h"

/** AES Encryption HW Structure Overlay */
typedef struct {
    __IO uint32_t KEY0; /**< Bits[31:00] of the 128-bit key */
    __IO uint32_t KEY1; /**< Bits[63:32] of the 128-bit key */
    __IO uint32_t KEY2; /**< Bits[95:64] of the 128-bit key */
    __IO uint32_t KEY3; /**< Bits[127:96] of the 128-bit key  */
    __IO uint32_t KEY4; /**< Bits[159:128] of the 256-bit key */
    __IO uint32_t KEY5; /**< Bits[191:160] of the 256-bit key */
    __IO uint32_t KEY6; /**< Bits[223:192] of the 256-bit key */
    __IO uint32_t KEY7; /**< Bits[255:224] of the 256-bit key  */

    __IO uint32_t CNTi0; /**< Bits[31:00] of the 128-bit counter value used in counter mode */
    __IO uint32_t CNTi1; /**< Bits[63:32] of the 128-bit counter value used in counter mode  */
    __IO uint32_t CNTi2; /**< Bits[95:64] of the 128-bit counter value used in counter mode */
    __IO uint32_t CNTi3; /**< Bits[127:96] of the 128-bit counter value used in counter mode */
    __I uint32_t CNTo0; /**< Bits[31:00] of the 128-bit counter result */
    __I uint32_t CNTo1; /**< Bits[63:32] of the 128-bit counter result */
    __I uint32_t CNTo2; /**< Bits[95:64] of the 128-bit counter result */
    __I uint32_t CNTo3; /**< Bits[127:96] of the 128-bit counter result */

    __I uint32_t CBCo0; /**< Bits[31:00] of the 128-bit CBC result */
    __I uint32_t CBCo1; /**< Bits[63:32] of the 128-bit CBC result */
    __I uint32_t CBCo2; /**< Bits[95:64] of the 128-bit CBC result */
    __I uint32_t CBCo3; /**< Bits[127:96] of the 128-bit CBC result */
    union {
        struct {
            __IO uint32_t START:1; /**< start the encryption : 0 = no-effect , 1 = enable  */
            __IO uint32_t ACC_CLR:1; /**< Clear the CBC accumulator : 0 = no-effect 1 = clears the CBC accumulator */
            __IO uint32_t INT_CLEAR:1; /**<  interrupt clear : 0 = no-effect 1 = clear the interrupt  */
        } BITS;
        __IO uint32_t WORD;
    } CTL;
    union {
        struct {
            __IO uint32_t CBC_MODE:1; /**< counter mode : 0 = counter mode , 1 = CBC mode */
            __IO uint32_t BYPASS:1; /**< encryption : 0 = Normal Mode , 1 = Bypasss any encryption */
            __IO uint32_t INT_EN:1; /**< interrupt mask : 0 = disabled 1 = enabled  */
            __IO uint32_t KEY_LENGTH:1; /**<  Key Length: 0 = 128 Bit Encryption 1 = 256 Bit Encryption  */

        } BITS;
        __IO uint32_t WORD;
    } MODE;
    union {
        struct {
            __I uint32_t COMPLETE:1;/**< status : 0 = not complete  , 1 = complete  */
        } BITS;
        __IO uint32_t WORD;
    } STAT;

    __O uint32_t MAC_INIT0; /**< Bits[31:00] of the CBC Initialization Vector */
    __O uint32_t MAC_INIT1; /**< Bits[63:32] of the CBC Initialization Vector */
    __O uint32_t MAC_INIT2; /**< Bits[95:64] of the CBC Initialization Vector */
    __O uint32_t MAC_INIT3; /**< Bits[127:96] of the CBC Initialization Vector */

    __IO uint32_t RESERVED;
    __O uint32_t DATA0; /**< Bits[31:00] of the 128-bit data to encrypt */
    __O uint32_t DATA1; /**< Bits[63:32] of the 128-bit data to encrypt */
    __O uint32_t DATA2; /**< Bits[95:64] of the 128-bit data to encrypt */
    __O uint32_t DATA3; /**< Bits[127:96] of the 128-bit data to encrypt */
} AesReg_t, *AesReg_pt;

#endif /* AES_MAP_H_ */
