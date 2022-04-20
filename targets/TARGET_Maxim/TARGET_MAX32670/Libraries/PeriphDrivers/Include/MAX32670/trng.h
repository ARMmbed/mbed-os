/**
 * @file    trng.h
 * @brief   Random number generator driver.
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
 *************************************************************************** */

#ifndef _TRNG_H_
#define _TRNG_H_

/***** Includes *****/
#include "trng_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup trng TRNG
 * @ingroup periphlibs
 * @{
 */


/***** Function Prototypes *****/
typedef void (*mxc_trng_complete_t) (void* req, int result);

/* ************************************************************************* */
/* Global Control/Configuration functions                                    */
/* ************************************************************************* */

/**
 * @brief   Enable portions of the TRNG
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_TRNG_Init (void);

/**
 * @brief   Enable TRNG Interrupts
 *
 */
void MXC_TRNG_EnableInt ();

/**
 * @brief   Disable TRNG Interrupts
 *
 */
void MXC_TRNG_DisableInt ();

/**
 * @brief   Disable and reset portions of the TRNG
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_TRNG_Shutdown (void);

/**
 * @brief   This function should be called from the TRNG ISR Handler
 *          when using Async functions
 */
void MXC_TRNG_Handler (void);

/* ************************************************************************* */
/* True Random Number Generator (TRNG) functions                             */
/* ************************************************************************* */

/**
 * @brief   Get a random number
 *
 * @return  A random 32-bit number
 */
int MXC_TRNG_RandomInt (void);

/**
 * @brief   Get a random number of length len
 *
 * @param   data    Pointer to a location to store the number
 * @param   len     Length of random number in bytes
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_TRNG_Random (uint8_t* data, uint32_t len);

/**
 * @brief   Get a random number of length len, do not block while generating data
 * @note    The user must call MXC_TRNG_Handler() in the ISR
 *
 * @param   data      Pointer to a location to store the number
 * @param   len       Length of random number in bytes
 * @param   callback  Function that will be called when all data has been generated
 *
 */
void MXC_TRNG_RandomAsync (uint8_t* data, uint32_t len, mxc_trng_complete_t callback);

/**
 * @brief   Generate an AES key and transfer to the AES block
 */
void MXC_TRNG_GenerateKey(void);

#ifdef __cplusplus
}
#endif
/**@} end of group trng */

#endif  /* _TRNG_H_ */
