/*******************************************************************************
 * Copyright (c) 2010-2018 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - Modified versions of the software must be conspicuously marked as such.
 *   - This software is licensed solely and exclusively for use with processors
 *     manufactured by or for Analog Devices, Inc.
 *   - This software may not be combined or merged with other code in any manner
 *     that would cause the software to become subject to terms and conditions
 *     which differ from those listed here.
 *   - Neither the name of Analog Devices, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
 * INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
 * CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

#include "cmsis.h"

#define NVIC_USER_IRQ_OFFSET 16
#define NVIC_USER_IRQ_NUMBER 72
#define NVIC_NUM_VECTORS     (NVIC_USER_IRQ_OFFSET + NVIC_USER_IRQ_NUMBER)

#define NVIC_RAM_VECTOR_ADDRESS   0x20000000

#ifdef __cplusplus
extern "C" {
#endif

/** Set the ISR for IRQn
 *
 * Sets an Interrupt Service Routine vector for IRQn; if the feature is available, the vector table is relocated to SRAM
 * the first time this function is called
 * @param[in] IRQn   The Interrupt Request number for which a vector will be registered
 * @param[in] vector The ISR vector to register for IRQn
 */
void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector);

/** Get the ISR registered for IRQn
 *
 * Reads the Interrupt Service Routine currently registered for IRQn
 * @param[in] IRQn   The Interrupt Request number the vector of which will be read
 * @return           Returns the ISR registered for IRQn
 */
uint32_t NVIC_GetVector(IRQn_Type IRQn);

#ifdef __cplusplus
}
#endif

#endif
