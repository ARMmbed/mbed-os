/*
 *  Portable interface to the CPU cycle counter
 *
 *  Copyright (C) 2016, Silicon Labs, http://www.silabs.com
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SELF_TEST) && defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf     printf
#endif

#if defined(MBEDTLS_TIMING_C)

#include "em_device.h"
#include "em_cmu.h"

#include "mbedtls/timing.h"

#if defined(MBEDTLS_TIMING_ALT)

void mbedtls_timing_init( void )
{
    if ((CMU->STATUS & _CMU_STATUS_AUXHFRCOENS_MASK) == 0)
    {
        /* Enable debug clock AUXHFRCO */
        CMU_OscillatorEnable(cmuOsc_AUXHFRCO, true, true);
    }
  
    if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) == 0)
    {
        /* Enable trace in core debug */
        CoreDebug->DHCSR |= CoreDebug_DHCSR_C_DEBUGEN_Msk;
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    }    
    if ((ITM->TCR & ITM_TCR_DWTENA_Msk) == 0)
    {
        /* Unlock ITM and output data */
        ITM->LAR    = 0xC5ACCE55;
        ITM->TCR   |= ITM_TCR_DWTENA_Msk;
    }
    if ((DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) == 0)
    {
        /* Enable DWT bits */
        DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;
        DWT->CYCCNT = 0;
    }
}

void mbedtls_timing_free( void )
{
    /* Disable the DWT and debug clock */
    DWT->CTRL = 0;
    CMU_OscillatorEnable( cmuOsc_AUXHFRCO, false, true );
}

#endif /* !MBEDTLS_TIMING_ALT */

#endif /* MBEDTLS_TIMING_C */
