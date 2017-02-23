/**
 ******************************************************************************
 * @file trng_api.c
 * @brief Implementation of TRNG functionality.
 * @internal
 * @author  ON Semiconductor.
 * $Rev: $
 * $Date: $
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
 * @ingroup TRNG
 *
 */
#if DEVICE_TRNG

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/
#include "trng_api.h"
#include "memory_map.h"
#include "ncs36510_trng.h"
#include "clock.h"
#include "mbed_wait_api.h"

/*************************************************************************************************
*                                                                                                *
*  Functions                                                                                     *
*                                                                                                *
*************************************************************************************************/


void trng_init(trng_t *obj)
{
    /* Enable TRNG */
    CLOCK_ENABLE(CLOCK_RAND);

    /* Initialize TRNG */
    RANDREG->CONTROL.WORD = False;

    return;
}
void trng_free(trng_t *obj)
{
    /* Stop TRNG  */
    RANDREG->CONTROL.WORD = False;

    /* Disable TRNG */
    CLOCK_DISABLE(CLOCK_RAND);

    return;
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    uint32_t MSLRandom = 0, Index, TempLen, *TempPtr = (uint32_t*)output;
        
    RANDREG->CONTROL.BITS.METASTABLE_LATCH_EN = TRNG_ENABLE;        /* ENable MSL TRNG */
    RANDREG->CONTROL.BITS.MEATSTABLE_SPEED    = TRNG_FAST_MODE;     /* Meta-stable Latch TRNG Speed Control */
    RANDREG->CONTROL.BITS.MODE                = TRNG_ON_READ_EVENT; /* TRNG is only updated on a read event of the TRNG register */

    wait_us(1); /* Wait till MSL generates random number after enable for the first time */
    
    TempLen = length / 4;

    for(Index = 0; Index < TempLen; Index++)
    {
        MSLRandom = RANDREG->METASTABLE_LATCH_VAL;
        *TempPtr++ = MSLRandom ;
    }

    TempLen = length % 4;
    Index *= 4;

    if(TempLen-- > 0)
    {
        MSLRandom = RANDREG->METASTABLE_LATCH_VAL;
        *(output + Index++) = (MSLRandom >> 0) & 0xFF;
        if(TempLen-- > 0)
        {
            *(output + Index++) = (MSLRandom >> 8) & 0xFF;
            if(TempLen-- > 0)
            {
                *(output + Index++) = (MSLRandom >> 16) & 0xFF;
            }
        }
    }

    RANDREG->CONTROL.BITS.METASTABLE_LATCH_EN = TRNG_DISABLE;        /* Disable MSL */

    *output_length = Index;

    return 0; /* Success */
}

#endif /* DEVICE_TRNG */