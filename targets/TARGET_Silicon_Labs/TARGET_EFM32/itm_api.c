/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if DEVICE_ITM

#include "hal/itm_api.h"
#include "cmsis.h"
#include "em_cmu.h"
#include "device_peripherals.h"

#include <stdbool.h>

/* SWO frequency: 875 kHz */
static void setupSWOForPrint(void)
{
#if defined( _GPIO_ROUTE_SWOPEN_MASK ) || defined( _GPIO_ROUTEPEN_SWVPEN_MASK )
    // Enable GPIO clock.
#if defined( _CMU_HFPERCLKEN0_GPIO_MASK )
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;
#elif defined( _CMU_HFBUSCLKEN0_GPIO_MASK )
    CMU->HFBUSCLKEN0 |= CMU_HFBUSCLKEN0_GPIO;
#endif
  
    // Enable Serial wire output pin
#if defined( _GPIO_ROUTE_SWOPEN_MASK )
    GPIO->ROUTE |= GPIO_ROUTE_SWOPEN;
#elif defined( _GPIO_ROUTEPEN_SWVPEN_MASK )
    GPIO->ROUTEPEN |= GPIO_ROUTEPEN_SWVPEN;
#endif
#endif

#if defined(_GPIO_ROUTE_MASK)
    #define SWO_ROUTE ROUTE
#endif
#if defined(_GPIO_ROUTELOC0_MASK)
    #define SWO_ROUTE ROUTELOC0
#endif

    // Set SWO location
    GPIO->SWO_ROUTE = SWO_LOCATION;
    // Enable output on pin
    GPIO->P[GPIO_PORT].SWO_MODE = SWO_ENABLE_OUTPUT_PIN;
  
    // Enable debug clock AUXHFRCO
    CMU->OSCENCMD = CMU_OSCENCMD_AUXHFRCOEN;
  
    // Wait until clock is ready
    while (!(CMU->STATUS & CMU_STATUS_AUXHFRCORDY));
  
    // Enable trace in core debug
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    /* Set TPIU prescaler for the current debug clock frequency. Target frequency
       is 875 kHz so we choose a divider that gives us the closest match.
       Actual divider is TPI->ACPR + 1. */
    uint32_t freq = CMU_ClockFreqGet(cmuClock_DBG) + (875000 / 2);
    uint32_t div  = freq / 875000;
    TPI->ACPR = div - 1;
}

static bool swoIsInitd()
{
#if defined( _CMU_HFPERCLKEN0_GPIO_MASK )
    return ((CMU->HFPERCLKEN0 & CMU_HFPERCLKEN0_GPIO) &&
            (GPIO->ROUTE & GPIO_ROUTE_SWOPEN) &&
            (CMU->STATUS & CMU_STATUS_AUXHFRCORDY) &&
            (CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk));
#elif defined( _CMU_HFBUSCLKEN0_GPIO_MASK )
    return ((CMU->HFBUSCLKEN0 & CMU_HFBUSCLKEN0_GPIO) &&
            (GPIO->ROUTEPEN |= GPIO_ROUTEPEN_SWVPEN) &&
            (CMU->STATUS & CMU_STATUS_AUXHFRCORDY) &&
            (CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk));
#endif
}

// As SWO has to be accessible everywhere, including ISRs, we can't easily
// communicate the dependency on clocks etc. to other components - so this
// function checks that things appear to be set up, and if not re-configures
// everything
void itm_init(void)
{
    if(!swoIsInitd()) {
        setupSWOForPrint();
    }
}

#endif
