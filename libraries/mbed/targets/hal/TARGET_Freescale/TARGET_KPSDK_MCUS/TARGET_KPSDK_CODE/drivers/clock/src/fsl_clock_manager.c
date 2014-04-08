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

#include "fsl_device_registers.h"
#include "fsl_sim_hal.h"
#include "fsl_clock_manager.h"
#include "fsl_clock_manager_private.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* internal function for clock manager. see function header for details */
static clock_manager_error_code_t clock_manager_get_frequency_by_name(sim_clock_names_t clockName,
                                                                      uint32_t *frequency);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_manager_get_system_clock
 * Description   : Internal function to get the system clock frequency
 * This function will check the clock name configuration table for specific
 * chip family and find out the supported clock name for that chip family
 * then it will call the mcg hal function to get the basic system clock,
 * calculate the clock frequency for specified clock name.
 * 
 *END**************************************************************************/
clock_manager_error_code_t clock_manager_get_system_clock(sim_clock_names_t clockName, 
                                                          uint32_t *frequency)
{
    /* system clock out divider*/
    uint32_t divider;
    uint32_t i = 0;
    const sim_clock_name_config_t *table = NULL;

    /* search through whole name table*/
    while (kSimClockNameConfigTable[i].clockName != kSimClockNameCount)
    {
        /* find only the match with name*/
        if (kSimClockNameConfigTable[i].clockName == clockName)
        {
            /* return the table pointer*/
            table = &kSimClockNameConfigTable[i];
            break;
        }
        i++;
    }

    /* if the configuration table doesn't exist, return error */
    if (table == NULL)
    {
        return kClockManagerNoSuchClockName;
    }
        
    /* check if we need to use a reference clock*/
    if (table->useOtherRefClock)
    {
        /* get other specified ref clock*/
        if ( kClockManagerSuccess != clock_manager_get_frequency_by_name(table->otherRefClockName, 
                                                                    frequency) )
        {
            return kClockManagerNoSuchClockName;
        }
    }
    else
    {
        /* get default ref clock */
        *frequency = clock_hal_get_outclk();
    }
        
    /* get system clock divider*/
    if ( clock_hal_get_clock_divider(table->dividerName, &divider) == kSimHalSuccess)
    {
        /* get the frequency for the specified clock*/
        *frequency = (*frequency) / (divider + 1);
        return kClockManagerSuccess;
    }
    else
    {
        return kClockManagerNoSuchDivider;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_manager_set_gate
 * Description   : Enable or disable the clock for specified clock module  
 * This function will enable/disable the clock for specified clock module and
 * instance. Refer to clock_gate_module_names_t for supported clock module name
 * by this function and refer to reference manual for supported clock moulde
 * name for a specified chip family. Most module driver will call this function
 * to gate(disable)/ungate(enable) the clock for that module, but application 
 * can also call this function as needed. Disable the clock will make the module
 * stop working. Refer to the reference maunal for proper procedure of enalbing
 * and disabling the clock for the device module.
 * 
 *END**************************************************************************/
clock_manager_error_code_t clock_manager_set_gate(clock_gate_module_names_t moduleName,
                                                  uint8_t instance, bool enable)
{
    /* calling hal api, to set(Enable/Disable) the clock module gate */
    if (clock_hal_set_gate(kClockModuleNameSimMap[moduleName], instance, enable) == kSimHalSuccess)
    {
        return kClockManagerSuccess;
    }
    else
    {
        return kClockManagerNoSuchClockModule;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_manager_get_gate
 * Description   : Get the current clock gate status for specified clock module
 * This function will return the current clock gate status for specified clock
 * module. Refer to clock_gate_module_names_t for supported clock module name.
 * 
 *END**************************************************************************/
clock_manager_error_code_t clock_manager_get_gate(clock_gate_module_names_t moduleName,
                                                  uint8_t instance, bool *isEnabled)
{
    /* calling the hal function to get the clock module gate status */
    if (clock_hal_get_gate(kClockModuleNameSimMap[moduleName], instance, isEnabled) 
                                                                 == kSimHalSuccess)
    {
        return kClockManagerSuccess;
    }
    else
    {
        return kClockManagerNoSuchClockModule;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_manager_get_frequency
 * Description   : Get the clock frequency for specified clock name 
 * This function will check the current clock configurations and then calculate
 * the clock frequency for specified clock name defined in clock_names_t.
 * The MCG must be properly configured before using this function. Also check
 * the reference manual for supported clock names on different chip falmily.
 * The returned value will be in herz. And if it cannot find the clock name
 * or the name is not supported on specific chip family, it will return an
 * error.
 * 
 *END**************************************************************************/
clock_manager_error_code_t clock_manager_get_frequency(clock_names_t clockName, 
                                                       uint32_t *frequency)
{
    /* convert clock manager clock name to sim clock name */
    sim_clock_names_t simClockName = kClockNameSimMap[clockName];
    
    /* calling internal get frequency by name function */
    return clock_manager_get_frequency_by_name(simClockName, frequency);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_manager_get_frequency_by_name
 * Description   : Internal function to get the frequency by clock name
 * This function will get/calculate the clock frequency based on clock name
 * and current configuration of clock generator.
 * 
 *END**************************************************************************/
clock_manager_error_code_t static clock_manager_get_frequency_by_name(sim_clock_names_t clockName,
                                                                      uint32_t *frequency)
{
    /* branch according to clock name */
    switch(clockName)
    {
    /* osc clock*/
    case kSimOsc32kClock:
/*            *frequency = CPU_XTAL32k_CLK_HZ;    // or should provided by osc module??*/
        *frequency = 32768;   
        break;
    case kSimOsc0ErClock:
/*            *frequency = CPU_XTAL_CLK_HZ;       // or should provided by osc module??*/
        *frequency = 80000;                     /* for testing purpose only*/
        break;
    case kSimOsc1ErClock:
/*            *frequency = CPU_XTAL_CLK_HZ;       // or should provided by osc module??*/
        *frequency = 80001;                     /* for testing purpose only*/
        break;

    /* irc clock*/
    case kSimIrc48mClock:
/*            *frequency = CPU_INT_IRC_CLK_HZ;    // or should provided by irc module??*/
        *frequency = 48000;   
        break;
        
    /* rtc clock*/
    case kSimRtc32kClock:
/*            *frequency = CPU_XTAL32k_CLK_HZ;    // or should provided by rtc module??*/
        *frequency = 32768;   
        break;

    case kSimRtc1hzClock:
/*            *frequency = CPU_XTAL1hz_CLK_HZ;    // or should provided by rtc module??*/
        *frequency = 1;   
        break;

    /* lpo clcok*/
    case kSimLpoClock:
/*            *frequency = 1000;                  // where should we define this constant??*/
        *frequency = 1000;    
        break;

    /* mcg clocks, calling mcg clock functions */
    case kSimMcgFllClock:
        *frequency = clock_hal_get_fllclk();
        break;
    case kSimMcgPll0Clock:
        *frequency = clock_hal_get_pll0clk();
        break;
    case kSimMcgOutClock:
        *frequency = clock_hal_get_outclk();
        break;
    case kSimMcgIrClock:
        *frequency = clock_hal_get_irclk();
        break;
    
    /* constant clocks (provided in other header files - TBD)*/
    case kSimSDHC0_CLKIN:
        *frequency = 11111;                     /* for testing use purpose*/
        break;
    case kSimENET_1588_CLKIN:
        *frequency = 22222;                     /* for testing use purpose*/
        break;
    case kSimEXTAL_Clock:
/*            *frequency = CPU_XTAL_CLK_HZ;                    // is this correct??*/
        *frequency = 33333;                     /* for testing use purpose*/
        break;
    case kSimEXTAL1_Clock:
/*            *frequency = CPU_XTAL_CLK_HZ;                    // is this correct??*/
        *frequency = 33334;                     /* for testing use purpose*/
        break;
    case kSimUSB_CLKIN:
        *frequency = 44444;                     /* for testing use purpose*/
        break;
    
    /* system clocks */
    case kSimCoreClock:
    case kSimSystemClock:
    case kSimPlatformClock:
    case kSimBusClock:
    case kSimFlexBusClock:
    case kSimFlashClock:
        return clock_manager_get_system_clock(clockName, frequency);


        /* reserved value*/
    case kSimReserved:
    default:
        *frequency = 55555;                     /* for testing use purpose*/
        return kClockManagerNoSuchClockName;
    }

    return kClockManagerSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_manager_get_frequency_by_source
 * Description   : Get the clock frequency for specified clock source
 * This function will get the specified clock source setting and convert it 
 * into a clock name, then calling the internal function to find out the value
 * for that clock name. Also the returned value is in herz.
 * If it cannot find the clock source or the source is not supported on the
 * specific chip family, it will return an error.
 * 
 *END**************************************************************************/
clock_manager_error_code_t clock_manager_get_frequency_by_source(clock_source_names_t clockSource,
                                                                 uint32_t *frequency)
{
    uint8_t setting;
    uint8_t subsetting1;
    uint8_t subsetting2;
    uint32_t frac = 0;
    uint32_t divider = 0;
    sim_clock_names_t simClockName;
    sim_clock_source_names_t simClockSource;
    sim_clock_divider_names_t dividerName;
    const sim_clock_source_value_t *valueTable = NULL;
    const sim_clock_source_value_t *subValueTable1 = NULL;
    const sim_clock_source_value_t *subValueTable2 = NULL;

    assert (clockSource < kClockSourceMax);

    /* convert clock manager clock source to sim clock source*/
    simClockSource = kClockSourceNameSimMap[clockSource];

    /* check to see if we need to use clock sel for specified clock source (see if CLKSEL exists)*/
    switch (simClockSource)
    {
    case kSimClockNfcSrc:                   /* NFCSRC*/
        simClockSource = kSimClockNfcSel;
        break;
    case kSimClockLcdcSrc:                  /* LCDCSRC*/
        simClockSource = kSimClockLcdcSel;
        break;
    case kSimClockUsbfSrc:                  /* USBFSRC  K70*/
        simClockSource = kSimClockUsbfSel;
        break;
    default:
        break;
    }

    /* get the sim clock source setting*/
    if (clock_hal_get_clock_source(simClockSource, &setting) != kSimHalSuccess)
    {
        return kClockManagerNoSuchClockSource;
    }

    /* get the value index table for the clock source*/
    valueTable = kSimClockSourceValueTable[simClockSource];

    /* if the source exists*/
    if (valueTable == NULL)
    {
        return kClockManagerNoSuchClockSource;
    }

    /* check if it is a clock name or selection*/
    if (valueTable[setting].isSel)
    {

        /* if it is a selection, then convert the selection into a clock name*/
        /* get the config table for the clock source*/
        if (clock_hal_get_clock_source(valueTable[setting].clockSelName, &subsetting1)
                                        != kSimHalSuccess)
        {
            return kClockManagerNoSuchClockSource;
        }

        /* get the value index table for the clock source*/
        subValueTable1 = kSimClockSourceValueTable[valueTable[setting].clockSelName];

        /* check if there's bad data*/
        if (subValueTable1 == NULL)
        {
            return kClockManagerNoSuchClockSource;
        }

        /* check if there's further clock select*/
        if (subValueTable1[subsetting1].isSel)
        {
            /* if it is a selection, then convert the selection into a clock name*/
            /* get the config table for the clock source*/
            if (clock_hal_get_clock_source(subValueTable1[subsetting1].clockSelName, &subsetting2)
                                            != kSimHalSuccess)
            {
                return kClockManagerNoSuchClockSource;
            }

            /* get the value index table for the clock source*/
            subValueTable2 = kSimClockSourceValueTable[subValueTable1[subsetting1].clockSelName];

            /* check if there's bad data*/
            if (subValueTable2 == NULL)
            {
                return kClockManagerNoSuchClockSource;
            }

            /* further convert the subvalue to clock name */
            simClockName = subValueTable2[subsetting2].clockName;
        }
        else
        {
            /* further convert the subvalue to clock name */
            simClockName = subValueTable1[subsetting1].clockName;
        }
    }
    else
    {
        /* convert the value to clock name */
        simClockName = valueTable[setting].clockName;
    }

    /* get/calculate the frequency for the specified clock name*/
    if ( clock_manager_get_frequency_by_name(simClockName, frequency) != kClockManagerSuccess)
    {
        return kClockManagerNoSuchClockName;
    }

    /* find the first available divider*/
    if (valueTable[setting].hasDivider)
    {
        /* get the diviver name */
        dividerName = valueTable[setting].dividerName;
    }
    else if ((subValueTable1 != NULL) && (subValueTable1[subsetting1].hasDivider))
    {
        /* get the diviver name */
        dividerName = subValueTable1[subsetting1].dividerName;
    }
    else if ((subValueTable2 != NULL) && (subValueTable2[subsetting2].hasDivider))
    {
        /* get the diviver name */
        dividerName = subValueTable2[subsetting2].dividerName;
    }
    else
    {
        return kClockManagerSuccess;
    }

    /* get the divider value*/
    switch (dividerName)
    {
    case kSimClockDividerNfcDiv:                   /* NFCSRC*/
        clock_hal_get_clock_divider(kSimClockDividerNfcDiv, &divider);
        clock_hal_get_clock_divider(kSimClockDividerNfcFrac, &frac);
        break;
    case kSimClockDividerLcdcDiv:                  /* LCDCSRC*/
        clock_hal_get_clock_divider(kSimClockDividerLcdcDiv, &divider);
        clock_hal_get_clock_divider(kSimClockDividerLcdcFrac, &frac);
        break;
    case kSimClockDividerUsbfsDiv:                 /* USBFSRC  K70*/
        clock_hal_get_clock_divider(kSimClockDividerUsbfsDiv, &divider);
        clock_hal_get_clock_divider(kSimClockDividerUsbfsFrac, &frac);
        break;
    case kSimClockDividerUsbDiv:                   /* USBSRC   K64, KL25, KV31, and K22*/
        clock_hal_get_clock_divider(kSimClockDividerUsbDiv, &divider);
        clock_hal_get_clock_divider(kSimClockDividerUsbFrac, &frac);
        break;
    case kSimClockDividerUsbhsDiv:                 /* USBHSRC  K70*/
        clock_hal_get_clock_divider(kSimClockDividerUsbhsDiv, &divider);
        clock_hal_get_clock_divider(kSimClockDividerUsbhsFrac, &frac);
        break;
    case kSimClockDividerSpecial1:                 /* Special  KL25 divider 2*/
        clock_hal_get_clock_divider(kSimClockDividerSpecial1, &divider);
        break;
    default:
        break;
    }

    /* Divider output clock = Divider input clock × [ (USBFRAC+1) / (USBDIV+1) ]*/
    *frequency = (*frequency) * (frac + 1) / (divider + 1);

    return kClockManagerSuccess;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

