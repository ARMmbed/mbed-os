/**
 ******************************************************************************
 * @file aes_map.h
 * @brief AES HW register map
 * @internal
 * @author Industrial ASSP Application Team.
 * $Rev: 2110 $
 * $Date: 2013-07-16 20:13:03 +0530 (Tue, 16 Jul 2013) $
 ******************************************************************************
 * @copyright (c) 2012 ON Semiconductor. All rights reserved.
 * ON Semiconductor is supplying this software for use with ON Semiconductor
 * processor based microcontrollers only.
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
			__O uint32_t START:1; /**< start the encryption : 0 = no-effect , 1 = enable  */
			__O uint32_t ACC_CLR:1; /**< Clear the CBC accumulator : 0 = no-effect 1 = clears the CBC accumulator */
			__O uint32_t INT_CLEAR:1; /**<  interrupt clear : 0 = no-effect 1 = clear the interrupt  */
			__O uint32_t KEY_LENGTH:1; /**<  Key Length: 0 = 128 Bit Encryption 1 = 256 Bit Encryption  */
		} BITS;
		__O uint32_t WORD;
	} CTL;
	union {
		struct {
			__IO uint32_t CBC_MODE:1; /**< counter mode : 0 = counter mode , 1 = CBC mode */
			__IO uint32_t BYPASS:1; /**< encryption : 0 = Normal Mode , 1 = Bypasss any encryption */
			__IO uint32_t INT_EN:1; /**< interrupt mask : 0 = disabled 1 = enabled  */
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
