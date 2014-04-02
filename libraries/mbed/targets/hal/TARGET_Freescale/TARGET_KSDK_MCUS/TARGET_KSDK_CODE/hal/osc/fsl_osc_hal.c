/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_osc_hal.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : osc_hal_enable_external_reference_clock
 * Description   : Enable the external reference clock for oscillator
 * This function will enable the external reference clock output 
 * for oscillator - that is the OSCERCLK. This clock will be used
 * by many peripherals. It should be enabled at early system init
 * stage to ensure the peripherals could select it and use it.
 * 
 *END**************************************************************************/
void osc_hal_enable_external_reference_clock(osc_instance_t instance)
{
    /* check instance range */
    assert(instance < FSL_FEATURE_OSC_COUNT);

    /* branch according to instance */
    switch (instance)
    {
#if FSL_FEATURE_OSC_HAS_OSC0
    case 0:
        OSC0->CR |= OSC_CR_ERCLKEN_MASK;
        break;
#endif
#if FSL_FEATURE_OSC_HAS_OSC1
    case 1:
        OSC1->CR |= OSC_CR_ERCLKEN_MASK;
        break;
#endif
#if FSL_FEATURE_OSC_HAS_OSC
    case 0:
        OSC->CR |= OSC_CR_ERCLKEN_MASK;
        break;
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : osc_hal_disable_external_reference_clock
 * Description   : Disable the external reference clock for oscillator
 * This function will disable the external reference clock output 
 * for oscillator - that is the OSCERCLK. This clock will be used
 * by many peripherals. It should be enabled at early system init
 * stage to ensure the peripherals could select it and use it.
 * 
 *END**************************************************************************/
void osc_hal_disable_external_reference_clock(osc_instance_t instance)
{
    /* check instance range */
    assert(instance < FSL_FEATURE_OSC_COUNT);

    /* branch according to instance */
    switch (instance)
    {
#if FSL_FEATURE_OSC_HAS_OSC0
    case 0:
        OSC0->CR &= ~OSC_CR_ERCLKEN_MASK;
        break;
#endif
#if FSL_FEATURE_OSC_HAS_OSC1
    case 1:
        OSC1->CR &= ~OSC_CR_ERCLKEN_MASK;
        break;
#endif
#if FSL_FEATURE_OSC_HAS_OSC
    case 0:
        OSC->CR &= ~OSC_CR_ERCLKEN_MASK;
        break;
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : osc_hal_enable_external_reference_clock_in_stop_mode
 * Description   : Enable the external reference clock in stop mode
 * This function will enable the external reference clock (OSCERCLK) when
 * MCU enters Stop mode. 
 * 
 *END**************************************************************************/
void osc_hal_enable_external_reference_clock_in_stop_mode(osc_instance_t instance)
{
    /* check instance range */
    assert(instance < FSL_FEATURE_OSC_COUNT);

    /* branch according to instance */
    switch (instance)
    {
#if FSL_FEATURE_OSC_HAS_OSC0
    case 0:
        OSC0->CR |= OSC_CR_EREFSTEN_MASK;
        break;
#endif
#if FSL_FEATURE_OSC_HAS_OSC1
    case 1:
        OSC1->CR |= OSC_CR_EREFSTEN_MASK;
        break;
#endif
#if FSL_FEATURE_OSC_HAS_OSC
    case 0:
        OSC->CR |= OSC_CR_EREFSTEN_MASK;
        break;
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : osc_hal_disable_external_reference_clock_in_stop_mode
 * Description   : Disable the external reference clock for in stop mode
 * This function will disable the external reference clock (OSCERCLK) when 
 * MCU enters Stop mode. 
 * 
 *END**************************************************************************/
void osc_hal_disable_external_reference_clock_in_stop_mode(osc_instance_t instance)
{
    /* check instance range */
    assert(instance < FSL_FEATURE_OSC_COUNT);

    /* branch according to instance */
    switch (instance)
    {
#if FSL_FEATURE_OSC_HAS_OSC0
    case 0:
        OSC0->CR &= ~OSC_CR_EREFSTEN_MASK;
        break;
#endif
#if FSL_FEATURE_OSC_HAS_OSC1
    case 1:
        OSC1->CR &= ~OSC_CR_EREFSTEN_MASK;
        break;
#endif
#if FSL_FEATURE_OSC_HAS_OSC
    case 0:
        OSC->CR &= ~OSC_CR_EREFSTEN_MASK;
        break;
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : osc_hal_enable_capacitor_config
 * Description   : Enable the capacitor configuration for oscillator
 * This function will enable the specified capacitors configuration for the 
 * oscillator. This should be done in early system level init function call
 * based on system configuration.
 * 
 *END**************************************************************************/
void osc_hal_enable_capacitor_config(osc_instance_t instance, uint32_t capacitorConfigs)
{
    /* check instance range */
    assert(instance < FSL_FEATURE_OSC_COUNT);

    /* branch according to instance */
    switch (instance)
    {
#if FSL_FEATURE_OSC_HAS_OSC0
    case 0:
        OSC0->CR |= (capacitorConfigs & kOscCapacitorMask);
        break;
#endif
#if FSL_FEATURE_OSC_HAS_OSC1
    case 1:
        OSC1->CR |= (capacitorConfigs & kOscCapacitorMask);
        break;
#endif
#if FSL_FEATURE_OSC_HAS_OSC
    case 0:
        OSC->CR |= (capacitorConfigs & kOscCapacitorMask);
        break;
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : osc_hal_disable_capacitor_config
 * Description   : Disable the capacitor configuration for specific oscillator
 * This function will enable the specified capacitors configuration for the 
 * oscillator.
 * 
 *END**************************************************************************/
void osc_hal_disable_capacitor_config(osc_instance_t instance, uint32_t capacitorConfigs)
{
    /* check instance range */
    assert(instance < FSL_FEATURE_OSC_COUNT);

    /* branch according to instance */
    switch (instance)
    {
#if FSL_FEATURE_OSC_HAS_OSC0
    case 0:
        OSC0->CR &= ~(capacitorConfigs & kOscCapacitorMask);
        break;
#endif
#if FSL_FEATURE_OSC_HAS_OSC1
    case 1:
        OSC1->CR &= ~(capacitorConfigs & kOscCapacitorMask);
        break;
#endif
#if FSL_FEATURE_OSC_HAS_OSC
    case 0:
        OSC->CR &= ~(capacitorConfigs & kOscCapacitorMask);
        break;
#endif
    default:
        break;
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

