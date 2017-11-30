/*******************************************************************************
 * Copyright (c) 2010-2017 Analog Devices, Inc.
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

#if defined(DEVICE_TRNG)

#include <stdlib.h>
#include <drivers/rng/adi_rng.h>
#include <drivers/pwr/adi_pwr.h>
#include "adi_rng_def.h"
#include "cmsis.h"
#include "trng_api.h"
#include "objects.h"

// Sampling counter values
// Prescaler: 0 - 10
// LenReload: 0 - 4095
#define TRNG_CNT_VAL    4095
#define TRNG_PRESCALER  2

/*******************************************************************************
   RNG Device memory is the instance of ADI_RNG_DEV_DATA_TYPE that contains
   pointers and requires 4 byte alignment. The use of uint8_t may cause memory
   access fault for some compilers which are not configured to handle unaligned
   accesses to SRAM. The size in uint8_t for RngDevMem is ADI_RNG_MEMORY_SIZE.
   The size in uint32_t for RngDevMem is recalculated to be
   (ADI_RNG_MEMORY_SIZE + 3)/4.
 *******************************************************************************/
static uint32_t RngDevMem[(ADI_RNG_MEMORY_SIZE + 3)/4];

void trng_init(trng_t *obj)
{
    ADI_RNG_HANDLE RNGhDevice;

    // Open the device
    adi_rng_Open(0,RngDevMem,sizeof(RngDevMem),&RNGhDevice);

    // Set sample length for the H/W RN accumulator
    adi_rng_SetSampleLen(RNGhDevice, TRNG_PRESCALER, TRNG_CNT_VAL);

    // Disable buffering - single byte generation only
    adi_rng_EnableBuffering(RNGhDevice, false);

    // Enable the TRNG
    adi_rng_Enable(RNGhDevice, true);

    // Save device handle
    obj->RNGhDevice = RNGhDevice;
}

void trng_free(trng_t *obj)
{
    ADI_RNG_HANDLE RNGhDevice = obj->RNGhDevice;

    adi_rng_Enable(RNGhDevice, false);
    adi_rng_Close(RNGhDevice);
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    ADI_RNG_HANDLE RNGhDevice = obj->RNGhDevice;
    bool bRNGRdy, bStuck;
    uint32_t i;
    volatile uint32_t nRandomNum;
    ADI_RNG_RESULT result;
    ADI_RNG_DEV_TYPE *pDevice = (ADI_RNG_DEV_TYPE*)RNGhDevice;

    for (i = 0; i < length; i++) {
        // Loop until the device has data to be read
        do {
            result = adi_rng_GetRdyStatus(RNGhDevice, &bRNGRdy);
            if (result != ADI_RNG_SUCCESS) {
                return -1;
            }
        } while (!bRNGRdy);

        // Check the STUCK bit to make sure the oscillator output isn't stuck
        result = adi_rng_GetStuckStatus(RNGhDevice, &bStuck);

        // If the stuck bit is set, this means there may be a problem with RNG hardware,
        // exit with an error
        if ( (result != ADI_RNG_SUCCESS) || ((result == ADI_RNG_SUCCESS) && (bStuck)) ) {
            // Clear the STUCK bit by writing a 1 to it
            pDevice->pRNG->STAT |= BITM_RNG_STAT_STUCK;
            return -1;
        }

        // Read the RNG
        result = adi_rng_GetRngData(RNGhDevice, (uint32_t*)(&nRandomNum));

        if (result != ADI_RNG_SUCCESS) {
            return -1;
        }

        // Save the output
        output[i] = (uint8_t)(nRandomNum & 0xFF);
    }

    *output_length = length;

    // Clear nRandomNum on the stack before exiting
    nRandomNum = 0;

    return 0;
}

#endif
