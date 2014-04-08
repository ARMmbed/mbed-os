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

#include "fsl_sim_hal.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_manager_find_module
 * Description   : An internal function to find the specified clock module 
 * table pointer for specified clock gate module name with instance. 
 * If the clock module with instance is supported, it will return an valid 
 * pointer. Otherwise it will return a null pointer. 
 * 
 *END**************************************************************************/
const sim_clock_gate_module_config_t *clock_manager_find_module(
                                                    sim_clock_gate_module_names_t clockModule,
                                                    uint8_t instance)
{
    uint32_t i = 0;

    /* search through whole name table*/
    while (kSimClockGateModuleConfigTable[i].clockGateModuleName != kSimClockModuleMax)
    {
        /* find only the match with both name and instance*/
        if ( (kSimClockGateModuleConfigTable[i].clockGateModuleName == clockModule) && 
             (kSimClockGateModuleConfigTable[i].deviceInstance == instance) )
        {
            /* return the table pointer*/
            return &kSimClockGateModuleConfigTable[i];
        }
        i++;
    }
    return NULL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_hal_set_gate
 * Description   : Enable or disable the clock for specified clock module
 * This function will enable/disable the clock for specified clock module and 
 * instance.
 * 
 *END**************************************************************************/
sim_hal_status_t clock_hal_set_gate(sim_clock_gate_module_names_t clockModule, 
                                    uint8_t instance, bool enable)
{
    const sim_clock_gate_module_config_t *table = NULL;
    /* check clock module name*/
    assert(clockModule < kSimClockModuleMax);

    /* find out the content matches with name and instance*/
    table = clock_manager_find_module(clockModule, instance);

    if (table != NULL)
    {
        if (enable)
        {
            /* if matches, execute the set(Enable/Disable) the clock gate*/
            *(uint32_t *)table->scgcRegAddress |= table->deviceMask;
        }
        else
        {
            /* if matches, execute the set(Enable/Disable) the clock gate*/
            *(uint32_t *)table->scgcRegAddress &= ~table->deviceMask;
        }
        return kSimHalSuccess;
    }
    else
    {
        return kSimHalNoSuchModule;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_hal_get_gate
 * Description   : Get the clock enable or disable state
 * This function will get the current clock gate status of the specified clock
 * moudle and instance.
 * 
 *END**************************************************************************/
sim_hal_status_t clock_hal_get_gate(sim_clock_gate_module_names_t clockModule, 
                                    uint8_t instance, bool *isEnabled)
{
    const sim_clock_gate_module_config_t *table = NULL;
    /* check clock module name*/
    assert(clockModule < kSimClockModuleMax);

    /* find out the content matches with name and instance*/
    table = clock_manager_find_module(clockModule, instance);

    if (table != NULL)
    {
        if ((*(uint32_t *)table->scgcRegAddress) & table->deviceMask)
        {
            *isEnabled = true;
        }
        else
        {
            *isEnabled = false;
        }
        return kSimHalSuccess;
    }
    else
    {
        return kSimHalNoSuchModule;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_hal_set_clock_source
 * Description   : Set clock source setting 
 * This function will set the settings for specified clock source. Each clock 
 * source has its clock selection settings. Refer to reference manual for 
 * details of settings for each clock source. Refer to sim_clock_source_names_t 
 * for clock sources.
 * 
 *END**************************************************************************/
sim_hal_status_t clock_hal_set_clock_source(sim_clock_source_names_t clockSource, uint8_t setting)
{
    sim_hal_status_t status = kSimHalSuccess;
    assert(clockSource < kSimClockSourceMax);

    switch (clockSource)
    {
#if FSL_FEATURE_SIM_OPT_HAS_NFCSRC
    case kSimClockNfcSrc:                   /* NFCSRC*/
        BW_SIM_SOPT2_NFCSRC(setting);
        break;
    case kSimClockNfcSel:                   /* NFC_CLKSEL*/
        BW_SIM_SOPT2_NFC_CLKSEL(setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_ESDHCSRC
    case kSimClockEsdhcSrc:                 /* ESDHCSRC*/
        BW_SIM_SOPT2_ESDHCSRC(setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_SDHCSRC
    case kSimClockSdhcSrc:                  /* SDHCSRC*/
        BW_SIM_SOPT2_SDHCSRC(setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_LCDCSRC
    case kSimClockLcdcSrc:                  /* LCDCSRC*/
        BW_SIM_SOPT2_LCDCSRC(setting);
        break;
    case kSimClockLcdcSel:                  /* LCDC_CLKSEL*/
        BW_SIM_SOPT2_LCDC_CLKSEL(setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_TIMESRC
    case kSimClockTimeSrc:                  /* TIMESRC*/
        BW_SIM_SOPT2_TIMESRC(setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_RMIISRC
    case kSimClockRmiiSrc:                  /* RMIISRC*/
        BW_SIM_SOPT2_RMIISRC(setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_USBSRC
    case kSimClockUsbSrc:                    /* USBSRC*/
        BW_SIM_SOPT2_USBSRC(setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_USBFSRC
    case kSimClockUsbfSrc:                   /* USBFSRC*/
        BW_SIM_SOPT2_USBFSRC(setting);
        break;
    case kSimClockUsbfSel:                  /* USBF_CLKSEL*/
        BW_SIM_SOPT2_USBF_CLKSEL(setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_USBHSRC
    case kSimClockUsbhSrc:                  /* USBHSRC*/
        BW_SIM_SOPT2_USBHSRC(setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_UART0SRC
    case kSimClockUart0Src:                 /* UART0SRC*/
        BW_SIM_SOPT2_UART0SRC(setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_TPMSRC
    case kSimClockTpmSrc:                   /* TPMSRC*/
        BW_SIM_SOPT2_TPMSRC(setting);
        break;
#endif

    case kSimClockOsc32kSel:                /* OSC32KSEL*/
        BW_SIM_SOPT1_OSC32KSEL(setting);
        break;

    case kSimClockPllfllSel:                /* PLLFLLSEL*/
        BW_SIM_SOPT2_PLLFLLSEL(setting);
        break;

#if FSL_FEATURE_SIM_OPT_HAS_TRACE_CLKSEL
    case kSimClockTraceSel:                 /* TRACE_CLKSEL*/
        BW_SIM_SOPT2_TRACECLKSEL(setting);
        break;
#endif

    case kSimClockClkoutSel:                /* CLKOUTSEL*/
        BW_SIM_SOPT2_CLKOUTSEL(setting);
        break;

    case kSimClockRtcClkoutSel:                /* RTCCLKOUTSEL*/
        BW_SIM_SOPT2_RTCCLKOUTSEL(setting);
        break;

    default:
        status = kSimHalNoSuchClockSrc;
        break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_hal_get_clock_source
 * Description   : Get clock source setting
 * This function will get the settings for specified clock source. Each clock 
 * source has its clock selection settings. Refer to reference manual for 
 * details of settings for each clock source. Refer to sim_clock_source_names_t
 * for clock sources.
 * 
 *END**************************************************************************/
sim_hal_status_t clock_hal_get_clock_source(sim_clock_source_names_t clockSource, uint8_t *setting)
{
    sim_hal_status_t status = kSimHalSuccess;
    assert(clockSource < kSimClockSourceMax);

    switch (clockSource)
    {
#if FSL_FEATURE_SIM_OPT_HAS_NFCSRC
    case kSimClockNfcSrc:                   /* NFCSRC*/
        *setting = BR_SIM_SOPT2_NFCSRC;
        break;
    case kSimClockNfcSel:                   /* NFC_CLKSEL*/
        *setting = BR_SIM_SOPT2_NFC_CLKSEL;
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_ESDHCSRC
    case kSimClockEsdhcSrc:                 /* ESDHCSRC*/
        *setting = BR_SIM_SOPT2_ESDHCSRC;
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_SDHCSRC
    case kSimClockSdhcSrc:                  /* SDHCSRC*/
        *setting = BR_SIM_SOPT2_SDHCSRC;
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_LCDCSRC
    case kSimClockLcdcSrc:                  /* LCDCSRC*/
        *setting = BR_SIM_SOPT2_LCDCSRC;
        break;
    case kSimClockLcdcSel:                  /* LCDC_CLKSEL*/
        *setting = BR_SIM_SOPT2_LCDC_CLKSEL;
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_TIMESRC
    case kSimClockTimeSrc:                  /* TIMESRC*/
        *setting = BR_SIM_SOPT2_TIMESRC;
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_RMIISRC
    case kSimClockRmiiSrc:                  /* RMIISRC*/
        *setting = BR_SIM_SOPT2_RMIISRC;
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_USBSRC
    case kSimClockUsbSrc:                    /* USBSRC*/
        *setting = BR_SIM_SOPT2_USBSRC;
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_USBFSRC
    case kSimClockUsbfSrc:                   /* USBFSRC*/
        *setting = BR_SIM_SOPT2_USBFSRC;
        break;
    case kSimClockUsbfSel:                  /* USBF_CLKSEL*/
        *setting = BR_SIM_SOPT2_USBF_CLKSEL;
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_USBHSRC
    case kSimClockUsbhSrc:                  /* USBHSRC*/
        *setting = BR_SIM_SOPT2_USBHSRC;
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_UART0SRC
    case kSimClockUart0Src:                 /* UART0SRC*/
        *setting = BR_SIM_SOPT2_UART0SRC;
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_TPMSRC
    case kSimClockTpmSrc:                   /* TPMSRC*/
        *setting = BR_SIM_SOPT2_TPMSRC;
        break;
#endif

    case kSimClockOsc32kSel:                /* OSC32KSEL*/
        *setting = BR_SIM_SOPT1_OSC32KSEL;
        break;

    case kSimClockPllfllSel:                /* PLLFLLSEL*/
        *setting = BR_SIM_SOPT2_PLLFLLSEL;
        break;

#if FSL_FEATURE_SIM_OPT_HAS_TRACE_CLKSEL
    case kSimClockTraceSel:                 /* TRACE_CLKSEL*/
        *setting = BR_SIM_SOPT2_TRACECLKSEL;
        break;
#endif

    case kSimClockClkoutSel:                /* CLKOUTSEL */
        *setting = BR_SIM_SOPT2_CLKOUTSEL;
        break;

    case kSimClockRtcClkoutSel:                /* RTCCLKOUTSEL */
        *setting = BR_SIM_SOPT2_RTCCLKOUTSEL;
        break;

    default:
        status = kSimHalNoSuchClockSrc;
        break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_hal_set_clock_divider
 * Description   : Set clock divider setting
 * This function will set the setting for specified clock divider. Refer to 
 * reference manual for supported clock divider and value range. Refer to 
 * sim_clock_divider_names_t for dividers.
 * 
 *END**************************************************************************/
sim_hal_status_t clock_hal_set_clock_divider(sim_clock_divider_names_t clockDivider, 
                                             uint32_t setting)
{
    sim_hal_status_t status = kSimHalSuccess;
    assert(clockDivider < kSimClockDividerMax);

    switch (clockDivider)
    {
    case kSimClockDividerOutdiv1:           /* OUTDIV1*/
        BW_SIM_CLKDIV1_OUTDIV1(setting);
        break;

#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV2
    case kSimClockDividerOutdiv2:           /* OUTDIV2*/
        BW_SIM_CLKDIV1_OUTDIV2(setting);
        break;
#endif

#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV3
    case kSimClockDividerOutdiv3:           /* OUTDIV3*/
        BW_SIM_CLKDIV1_OUTDIV3(setting);
        break;
#endif

    case kSimClockDividerOutdiv4:           /* OUTDIV4*/
        BW_SIM_CLKDIV1_OUTDIV4(setting);
        break;

#if FSL_FEATURE_SIM_DIVIDER_HAS_USBDIV
    case kSimClockDividerUsbFrac:           /* USBFRAC*/
        BW_SIM_CLKDIV2_USBFRAC(setting);
        break;
    case kSimClockDividerUsbDiv:            /* USBDIV*/
        BW_SIM_CLKDIV2_USBDIV(setting);
        break;
#endif

#if FSL_FEATURE_SIM_DIVIDER_HAS_USBFSDIV
    case kSimClockDividerUsbfsFrac:         /* USBFSFRAC*/
        BW_SIM_CLKDIV2_USBFSFRAC(setting);
        break;
    case kSimClockDividerUsbfsDiv:          /* USBFSDIV*/
        BW_SIM_CLKDIV2_USBFSDIV(setting);
        break;
#endif

#if FSL_FEATURE_SIM_DIVIDER_HAS_USBHSDIV
    case kSimClockDividerUsbhsFrac:         /* USBHSFRAC*/
        BW_SIM_CLKDIV2_USBHSFRAC(setting);
        break;
    case kSimClockDividerUsbhsDiv:          /* USBHSDIV*/
        BW_SIM_CLKDIV2_USBHSDIV(setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_LCDCSRC
    case kSimClockDividerLcdcFrac:          /* LCDCFRAC*/
        BW_SIM_CLKDIV3_LCDCFRAC(setting);
        break;
    case kSimClockDividerLcdcDiv:           /* LCDCDIV*/
        BW_SIM_CLKDIV3_LCDCDIV(setting);
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_NFCSRC
    case kSimClockDividerNfcFrac:           /* NFCFRAC*/
        BW_SIM_CLKDIV4_NFCFRAC(setting);
        break;
    case kSimClockDividerNfcDiv:            /* NFCDIV*/
        BW_SIM_CLKDIV4_NFCDIV(setting);
        break;
#endif

    case kSimClockDividerSpecial1:          /* special divider 1   */
        break;

    default:
        status = kSimHalNoSuchDivider;
        break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_hal_set_clock_out_dividers
 * Description   : Set all clock out dividers setting at the same time
 * This function will set the setting for all clock out dividers. Refer to 
 * reference manual for supported clock divider and value range. Refer to 
 * sim_clock_divider_names_t for dividers.
 * 
 *END**************************************************************************/
void clock_hal_set_clock_out_dividers(uint32_t outdiv1, uint32_t outdiv2, 
                                 uint32_t outdiv3, uint32_t outdiv4)
{
    uint32_t clkdiv1 = 0;
    
    clkdiv1 |= BF_SIM_CLKDIV1_OUTDIV1(outdiv1);
#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV2
    clkdiv1 |= BF_SIM_CLKDIV1_OUTDIV2(outdiv2);
#endif
#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV3
    clkdiv1 |= BF_SIM_CLKDIV1_OUTDIV3(outdiv3);
#endif
    clkdiv1 |= BF_SIM_CLKDIV1_OUTDIV4(outdiv4);
    
    HW_SIM_CLKDIV1_WR(clkdiv1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : clock_hal_get_clock_divider
 * Description   : Get clock divider setting
 * This function will get the setting for specified clock divider. Refer to 
 * reference manual for supported clock divider and value range. Refer to 
 * sim_clock_divider_names_t for dividers.
 * 
 *END**************************************************************************/
sim_hal_status_t clock_hal_get_clock_divider(sim_clock_divider_names_t clockDivider, uint32_t *setting)
{
    sim_hal_status_t status = kSimHalSuccess;
    assert(clockDivider < kSimClockDividerMax);

    *setting = 0;

    switch (clockDivider)
    {
    case kSimClockDividerOutdiv1:           /* OUTDIV1*/
        *setting = BR_SIM_CLKDIV1_OUTDIV1;
        break;

#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV2
    case kSimClockDividerOutdiv2:           /* OUTDIV2*/
        *setting = BR_SIM_CLKDIV1_OUTDIV2;
        break;
#endif

#if FSL_FEATURE_SIM_DIVIDER_HAS_OUTDIV3
    case kSimClockDividerOutdiv3:           /* OUTDIV3*/
        *setting = BR_SIM_CLKDIV1_OUTDIV3;
        break;
#endif

    case kSimClockDividerOutdiv4:           /* OUTDIV4*/
        *setting = BR_SIM_CLKDIV1_OUTDIV4;
        break;

#if FSL_FEATURE_SIM_DIVIDER_HAS_USBDIV
    case kSimClockDividerUsbFrac:           /* USBFRAC*/
        *setting = BR_SIM_CLKDIV2_USBFRAC;
        break;
    case kSimClockDividerUsbDiv:            /* USBDIV*/
        *setting = BR_SIM_CLKDIV2_USBDIV;
        break;
#endif

#if FSL_FEATURE_SIM_DIVIDER_HAS_USBFSDIV
    case kSimClockDividerUsbfsFrac:         /* USBFSFRAC*/
        *setting = BR_SIM_CLKDIV2_USBFSFRAC;
        break;
    case kSimClockDividerUsbfsDiv:          /* USBFSDIV*/
        *setting = BR_SIM_CLKDIV2_USBFSDIV;
        break;
#endif

#if FSL_FEATURE_SIM_DIVIDER_HAS_USBHSDIV
    case kSimClockDividerUsbhsFrac:         /* USBHSFRAC*/
        *setting = BR_SIM_CLKDIV2_USBHSFRAC;
        break;
    case kSimClockDividerUsbhsDiv:          /* USBHSDIV*/
        *setting = BR_SIM_CLKDIV2_USBHSDIV;
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_LCDCSRC
    case kSimClockDividerLcdcFrac:          /* LCDCFRAC*/
        *setting = BR_SIM_CLKDIV3_LCDCFRAC;
        break;
    case kSimClockDividerLcdcDiv:           /* LCDCDIV*/
        *setting = BR_SIM_CLKDIV3_LCDCDIV;
        break;
#endif

#if FSL_FEATURE_SIM_OPT_HAS_NFCSRC
    case kSimClockDividerNfcFrac:           /* NFCFRAC*/
        *setting = BR_SIM_CLKDIV4_NFCFRAC;
        break;
    case kSimClockDividerNfcDiv:            /* NFCDIV*/
        *setting = BR_SIM_CLKDIV4_NFCDIV;
        break;
#endif

    case kSimClockDividerSpecial1:          /* special divider 1    */
        *setting = 1;                   
        break;

    default:
        status = kSimHalNoSuchDivider;
        break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_set_alttrgen
 * Description   : Set ADCx alternate trigger enable setting
 * This function will enable/disable alternative conversion triggers for ADCx. 
 * 
 *END**************************************************************************/
void sim_set_alttrgen(uint8_t instance, bool enable)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT7_ADC0ALTTRGEN(enable ? 1 : 0);
        break;
#if (HW_ADC_INSTANCE_COUNT > 1)
    case 1:
        BW_SIM_SOPT7_ADC1ALTTRGEN(enable ? 1 : 0);
        break;
#if (HW_ADC_INSTANCE_COUNT > 2)
    case 2:
        BW_SIM_SOPT7_ADC2ALTTRGEN(enable ? 1 : 0);
        break;
    case 3:
        BW_SIM_SOPT7_ADC3ALTTRGEN(enable ? 1 : 0);
        break;
#endif
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_get_alttrgen
 * Description   : Get ADCx alternate trigger enable settingg
 * This function will get ADCx alternate trigger enable setting. 
 * 
 *END**************************************************************************/
bool sim_get_alttrgen(uint8_t instance)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        return BR_SIM_SOPT7_ADC0ALTTRGEN;
#if (HW_ADC_INSTANCE_COUNT > 1)
    case 1:
        return BR_SIM_SOPT7_ADC1ALTTRGEN;
#if (HW_ADC_INSTANCE_COUNT > 2)
    case 2:
        return BR_SIM_SOPT7_ADC2ALTTRGEN;
    case 3:
        return BR_SIM_SOPT7_ADC3ALTTRGEN;
#endif
#endif
    default:
        return false;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_set_pretrgsel
 * Description   : Set ADCx pre-trigger select setting
 * This function will select the ADCx pre-trigger source when alternative
 * triggers are enabled through ADCxALTTRGEN
 * 
 *END**************************************************************************/
void sim_set_pretrgsel(uint8_t instance, sim_pretrgsel_t select)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT7_ADC0PRETRGSEL(select);
        break;
#if (HW_ADC_INSTANCE_COUNT > 1)
    case 1:
        BW_SIM_SOPT7_ADC1PRETRGSEL(select);
        break;
#if (HW_ADC_INSTANCE_COUNT > 2)
    case 2:
        BW_SIM_SOPT7_ADC2PRETRGSEL(select);
        break;
    case 3:
        BW_SIM_SOPT7_ADC3PRETRGSEL(select);
        break;
#endif
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_get_pretrgsel
 * Description   : Get ADCx pre-trigger select setting
 * This function will get ADCx pre-trigger select setting.
 * 
 *END**************************************************************************/
sim_pretrgsel_t sim_get_pretrgsel(uint8_t instance)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        return (sim_pretrgsel_t)BR_SIM_SOPT7_ADC0PRETRGSEL;
#if (HW_ADC_INSTANCE_COUNT > 1)
    case 1:
        return (sim_pretrgsel_t)BR_SIM_SOPT7_ADC1PRETRGSEL;
#if (HW_ADC_INSTANCE_COUNT > 2)
    case 2:
        return (sim_pretrgsel_t)BR_SIM_SOPT7_ADC2PRETRGSEL;
    case 3:
        return (sim_pretrgsel_t)BR_SIM_SOPT7_ADC3PRETRGSEL;
#endif
#endif
    default:
        return (sim_pretrgsel_t)false;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_set_trgsel
 * Description   : Set ADCx trigger select setting
 * This function will select the ADCx trigger source when alternative triggers
 * are enabled through ADCxALTTRGEN
 * 
 *END**************************************************************************/
void sim_set_trgsel(uint8_t instance, sim_trgsel_t select)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT7_ADC0TRGSEL(select);
        break;
#if (HW_ADC_INSTANCE_COUNT > 1)
    case 1:
        BW_SIM_SOPT7_ADC1TRGSEL(select);
        break;
#if (HW_ADC_INSTANCE_COUNT > 2)
    case 2:
        BW_SIM_SOPT7_ADC2TRGSEL(select);
        break;
    case 3:
        BW_SIM_SOPT7_ADC3TRGSEL(select);
        break;
#endif
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_get_trgsel
 * Description   : Get ADCx trigger select setting 
 * This function will get ADCx trigger select setting.
 * 
 *END**************************************************************************/
sim_pretrgsel_t sim_get_trgsel(uint8_t instance)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        return (sim_pretrgsel_t)BR_SIM_SOPT7_ADC0TRGSEL;
#if (HW_ADC_INSTANCE_COUNT > 1)
    case 1:
        return (sim_pretrgsel_t)BR_SIM_SOPT7_ADC1TRGSEL;
#if (HW_ADC_INSTANCE_COUNT > 2)
    case 2:
        return (sim_pretrgsel_t)BR_SIM_SOPT7_ADC2TRGSEL;
    case 3:
        return (sim_pretrgsel_t)BR_SIM_SOPT7_ADC3TRGSEL;
#endif
#endif
    default:
        return (sim_pretrgsel_t)false;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_set_uart_rxsrc
 * Description   : Set UARTx receive data source select setting 
 * This function will select the source for the UART1 receive data.
 * 
 *END**************************************************************************/
void sim_set_uart_rxsrc(uint8_t instance, sim_uart_rxsrc_t select)
{
    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT5_UART0RXSRC(select);
        break;
    case 1:
        BW_SIM_SOPT5_UART1RXSRC(select);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_get_pretrgsel
 * Description   : Get UARTx receive data source select setting 
 * This function will get UARTx receive data source select setting.
 * 
 *END**************************************************************************/
sim_uart_rxsrc_t sim_get_uart_rxsrc(uint8_t instance)
{
    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        return (sim_uart_rxsrc_t)BR_SIM_SOPT5_UART0RXSRC;
    case 1:
        return (sim_uart_rxsrc_t)BR_SIM_SOPT5_UART1RXSRC;
    default:
        return (sim_uart_rxsrc_t)0;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_set_uart_txsrc
 * Description   : Set UARTx transmit data source select setting 
 * This function will select the source for the UARTx transmit data.
 * 
 *END**************************************************************************/
void sim_set_uart_txsrc(uint8_t instance, sim_uart_txsrc_t select)
{
    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT5_UART0TXSRC(select);
        break;
    case 1:
        BW_SIM_SOPT5_UART1TXSRC(select);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_get_uart_txsrc
 * Description   : Get UARTx transmit data source select setting 
 * This function will get UARTx transmit data source select setting.
 * 
 *END**************************************************************************/
sim_uart_txsrc_t sim_get_uart_txsrc(uint8_t instance)
{
    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        return (sim_uart_txsrc_t)BR_SIM_SOPT5_UART0TXSRC;
    case 1:
        return (sim_uart_txsrc_t)BR_SIM_SOPT5_UART1TXSRC;
    default:
        return (sim_uart_txsrc_t)0;
    }
}

#if FSL_FEATURE_SIM_OPT_HAS_ODE
/*FUNCTION**********************************************************************
 *
 * Function Name : sim_set_uart_ode
 * Description   : Set UARTx Open Drain Enable setting 
 * This function will enable/disable the UARTx Open Drain.
 * 
 *END**************************************************************************/
void sim_set_uart_ode(uint8_t instance, bool enable)
{
    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT5_UART0ODE(enable ? 1 : 0);
        break;
    case 1:
        BW_SIM_SOPT5_UART1ODE(enable ? 1 : 0);
        break;
    case 2:
        BW_SIM_SOPT5_UART2ODE(enable ? 1 : 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_get_uart_ode
 * Description   : Get UARTx Open Drain Enable setting 
 * This function will get UARTx Open Drain Enable setting.
 * 
 *END**************************************************************************/
bool sim_get_uart_ode(uint8_t instance)
{
    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        return BR_SIM_SOPT5_UART0ODE;
    case 1:
        return BR_SIM_SOPT5_UART1ODE;
    case 2:
        return BR_SIM_SOPT5_UART2ODE;
    default:
        return (sim_uart_txsrc_t)0;
    }
}
#endif

#if FSL_FEATURE_SIM_OPT_HAS_FTM
/*FUNCTION**********************************************************************
 *
 * Function Name : sim_set_ftm_trg_src
 * Description   : Set FlexTimer x hardware trigger y source select setting 
 * This function will select the source of FTMx hardware trigger y.
 * 
 *END**************************************************************************/
void sim_set_ftm_trg_src(uint8_t instance, uint8_t trigger, sim_ftm_trg_src_t select)
{
    assert (instance < HW_FTM_INSTANCE_COUNT);
    assert (trigger < FSL_FEATURE_SIM_OPT_FTM_TRIGGER_COUNT);

    switch (instance)
    {
#if FSL_FEATURE_SIM_OPT_HAS_FTM0_TRIGGER
    case 0:
        switch (trigger)
        {
        case 0:
            BW_SIM_SOPT4_FTM0TRG0SRC(select);
            break;
        case 1:
            BW_SIM_SOPT4_FTM0TRG1SRC(select);
            break;
        default:
            break;
        }
        break;
#endif
#if FSL_FEATURE_SIM_OPT_HAS_FTM3_TRIGGER
    case 3:
        switch (trigger)
        {
        case 0:
            BW_SIM_SOPT4_FTM3TRG0SRC(select);
            break;
        case 1:
            BW_SIM_SOPT4_FTM3TRG1SRC(select);
            break;
        default:
            break;
        }
        break;
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_get_ftm_trg_src
 * Description   : Get FlexTimer x hardware trigger y source select setting
 * This function will get FlexTimer x hardware trigger y source select setting.
 * 
 *END**************************************************************************/
sim_ftm_trg_src_t sim_get_ftm_trg_src(uint8_t instance, uint8_t trigger)
{
    assert (instance < HW_FTM_INSTANCE_COUNT);
    assert (trigger < FSL_FEATURE_SIM_OPT_FTM_TRIGGER_COUNT);

    switch (instance)
    {
#if FSL_FEATURE_SIM_OPT_HAS_FTM0_TRIGGER
    case 0:
        switch (trigger)
        {
        case 0:
            return (sim_ftm_trg_src_t)BR_SIM_SOPT4_FTM0TRG0SRC;
        case 1:
            return (sim_ftm_trg_src_t)BR_SIM_SOPT4_FTM0TRG1SRC;
        default:
            break;
        }
        break;
#endif
#if FSL_FEATURE_SIM_OPT_HAS_FTM3_TRIGGER
    case 3:
        switch (trigger)
        {
        case 0:
            return (sim_ftm_trg_src_t)BR_SIM_SOPT4_FTM3TRG0SRC;
        case 1:
            return (sim_ftm_trg_src_t)BR_SIM_SOPT4_FTM3TRG1SRC;
        default:
            break;
        }
        break;
#endif
    default:
        break;
    }

    return (sim_ftm_trg_src_t)0;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_set_ftm_clk_sel
 * Description   : Set FlexTimer x external clock pin select setting 
 * This function will select the source of FTMx external clock pin select
 * 
 *END**************************************************************************/
void sim_set_ftm_clk_sel(uint8_t instance, sim_ftm_clk_sel_t select)
{
    assert (instance < HW_FTM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT4_FTM0CLKSEL(select);
        break;
    case 1:
        BW_SIM_SOPT4_FTM1CLKSEL(select);
        break;
    case 2:
        BW_SIM_SOPT4_FTM2CLKSEL(select);
        break;
    case 3:
        BW_SIM_SOPT4_FTM3CLKSEL(select);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_get_ftm_clk_sel
 * Description   : Get FlexTimer x external clock pin select setting
 * This function will get FlexTimer x external clock pin select setting.
 * 
 *END**************************************************************************/
sim_ftm_clk_sel_t sim_get_ftm_clk_sel(uint8_t instance)
{
    assert (instance < HW_FTM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        return (sim_ftm_clk_sel_t)BR_SIM_SOPT4_FTM0CLKSEL;
    case 1:
        return (sim_ftm_clk_sel_t)BR_SIM_SOPT4_FTM1CLKSEL;
    case 2:
        return (sim_ftm_clk_sel_t)BR_SIM_SOPT4_FTM2CLKSEL;
    case 3:
        return (sim_ftm_clk_sel_t)BR_SIM_SOPT4_FTM3CLKSEL;
    default:
        break;
    }

    return (sim_ftm_clk_sel_t)0;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_set_ftm_ch_src
 * Description   : FlexTimer x channel y input capture source select setting 
 * This function will select FlexTimer x channel y input capture source
 * 
 *END**************************************************************************/
void sim_set_ftm_ch_src(uint8_t instance, uint8_t channel, sim_ftm_ch_src_t select)
{
    assert (instance < HW_FTM_INSTANCE_COUNT);

    switch (instance)
    {
#if FSL_FEATURE_SIM_OPT_HAS_FTM1_CHANNELS
    case 1:
        switch (channel)
        {
        case 0:
            BW_SIM_SOPT4_FTM1CH0SRC(select);
            break;
        default:
            break;
        }
        break;
#endif
#if FSL_FEATURE_SIM_OPT_HAS_FTM2_CHANNELS
    case 2:
        switch (channel)
        {
        case 0:
            BW_SIM_SOPT4_FTM2CH0SRC(select);
            break;
#if FSL_FEATURE_SIM_OPT_HAS_FTM2_CHANNEL1
        case 1:
            BW_SIM_SOPT4_FTM2CH1SRC(select);
            break;
#endif
        default:
            break;
        }
        break;
#endif
#if FSL_FEATURE_SIM_OPT_HAS_FTM3_CHANNELS
    case 3:
        switch (channel)
        {
        case 0:
            BW_SIM_SOPT4_FTM3CH0SRC(select);
            break;
        default:
            break;
        }
        break;
#endif
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_get_ftm_ch_src
 * Description   : Get FlexTimer x channel y input capture source select setting
 * This function will get FlexTimer x channel y input capture source select 
 * setting.
 * 
 *END**************************************************************************/
sim_ftm_ch_src_t sim_get_ftm_ch_src(uint8_t instance, uint8_t channel)
{
    assert (instance < HW_FTM_INSTANCE_COUNT);

    switch (instance)
    {
#if FSL_FEATURE_SIM_OPT_HAS_FTM1_CHANNELS
    case 1:
        switch (channel)
        {
        case 0:
            return (sim_ftm_ch_src_t)BR_SIM_SOPT4_FTM1CH0SRC;
        default:
            break;
        }
        break;
#endif
#if FSL_FEATURE_SIM_OPT_HAS_FTM2_CHANNELS
    case 2:
        switch (channel)
        {
        case 0:
            return (sim_ftm_ch_src_t)BR_SIM_SOPT4_FTM2CH0SRC;
#if FSL_FEATURE_SIM_OPT_HAS_FTM2_CHANNEL1
        case 1:
            return (sim_ftm_ch_src_t)BR_SIM_SOPT4_FTM2CH1SRC;
#endif
        default:
            break;
        }
        break;
#endif
#if FSL_FEATURE_SIM_OPT_HAS_FTM3_CHANNELS
    case 3:
        switch (channel)
        {
        case 0:
            return (sim_ftm_ch_src_t)BR_SIM_SOPT4_FTM3CH0SRC;
        default:
            break;
        }
        break;
#endif
    default:
        break;
    }

    return (sim_ftm_ch_src_t)0;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_set_ftm_flt_sel
 * Description   : Set FlexTimer x fault y select setting 
 * This function will set the FlexTimer x fault y select setting.
 * 
 *END**************************************************************************/
void sim_set_ftm_flt_sel(uint8_t instance, uint8_t fault, sim_ftm_flt_sel_t select)
{
    assert (instance < HW_FTM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        switch (fault)
        {
        case 0:
            BW_SIM_SOPT4_FTM0FLT0(select);
            break;
        case 1:
            BW_SIM_SOPT4_FTM0FLT1(select);
            break;
#if (FSL_FEATURE_SIM_OPT_FTM0_FAULT_COUNT > 2)
        case 2:
            BW_SIM_SOPT4_FTM0FLT2(select);
            break;
#if (FSL_FEATURE_SIM_OPT_FTM0_FAULT_COUNT > 3)
        case 3:
            BW_SIM_SOPT4_FTM0FLT3(select);
            break;
#endif
#endif
        default:
            break;
        }
        break;
    case 1:
        BW_SIM_SOPT4_FTM1FLT0(select);
        break;
    case 2:
        BW_SIM_SOPT4_FTM2FLT0(select);
        break;
    case 3:
        BW_SIM_SOPT4_FTM3FLT0(select);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_get_ftm_flt_sel
 * Description   : Get FlexTimer x fault y select setting
 * This function will get FlexTimer x fault y select setting.
 * 
 *END**************************************************************************/
sim_ftm_flt_sel_t sim_get_ftm_flt_sel(uint8_t instance, uint8_t fault)
{
    assert (instance < HW_FTM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        switch (fault)
        {
        case 0:
            return (sim_ftm_flt_sel_t)BR_SIM_SOPT4_FTM0FLT0;
        case 1:
            return (sim_ftm_flt_sel_t)BR_SIM_SOPT4_FTM0FLT1;
#if (FSL_FEATURE_SIM_OPT_FTM0_FAULT_COUNT > 2)
        case 2:
            return (sim_ftm_flt_sel_t)BR_SIM_SOPT4_FTM0FLT2;
#if (FSL_FEATURE_SIM_OPT_FTM0_FAULT_COUNT > 3)
        case 3:
            return (sim_ftm_flt_sel_t)BR_SIM_SOPT4_FTM0FLT3;
#endif
#endif
        default:
            break;
        }
        break;
    case 1:
        return (sim_ftm_flt_sel_t)BR_SIM_SOPT4_FTM1FLT0;
    case 2:
        return (sim_ftm_flt_sel_t)BR_SIM_SOPT4_FTM2FLT0;
    case 3:
        return (sim_ftm_flt_sel_t)BR_SIM_SOPT4_FTM3FLT0;
    default:
        break;
    }

    return (sim_ftm_flt_sel_t)0;
}
#endif

#if FSL_FEATURE_SIM_OPT_HAS_TPM
/*FUNCTION**********************************************************************
 *
 * Function Name : sim_set_tpm_clk_sel
 * Description   : Set Timer/PWM x external clock pin select setting 
 * This function will select the source of Timer/PWM x external clock pin select
 * 
 *END**************************************************************************/
void sim_set_tpm_clk_sel(uint8_t instance, sim_tpm_clk_sel_t select)
{
    assert (instance < HW_TPM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        BW_SIM_SOPT4_TPM0CLKSEL(select);
        break;
    case 1:
        BW_SIM_SOPT4_TPM1CLKSEL(select);
        break;
    case 2:
        BW_SIM_SOPT4_TPM2CLKSEL(select);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_get_tpm_clk_sel
 * Description   : Get Timer/PWM x external clock pin select setting
 * This function will get Timer/PWM x external clock pin select setting.
 * 
 *END**************************************************************************/
sim_tpm_clk_sel_t sim_get_tpm_clk_sel(uint8_t instance)
{
    assert (instance < HW_TPM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        return (sim_tpm_clk_sel_t)BR_SIM_SOPT4_TPM0CLKSEL;
    case 1:
        return (sim_tpm_clk_sel_t)BR_SIM_SOPT4_TPM1CLKSEL;
    case 2:
        return (sim_tpm_clk_sel_t)BR_SIM_SOPT4_TPM2CLKSEL;
    default:
        break;
    }

    return (sim_tpm_clk_sel_t)0;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_set_tpm_ch_src
 * Description   : Timer/PWM x channel y input capture source select setting 
 * This function will select Timer/PWM x channel y input capture source
 * 
 *END**************************************************************************/
void sim_set_tpm_ch_src(uint8_t instance, uint8_t channel, sim_tpm_ch_src_t select)
{
    assert (instance < HW_TPM_INSTANCE_COUNT);

    switch (instance)
    {
    case 1:
        switch (channel)
        {
        case 0:
            BW_SIM_SOPT4_TPM1CH0SRC(select);
            break;
        default:
            break;
        }
        break;
    case 2:
        switch (channel)
        {
        case 0:
            BW_SIM_SOPT4_TPM2CH0SRC(select);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sim_get_tpm_ch_src
 * Description   : Get Timer/PWM x channel y input capture source select setting
 * This function will get Timer/PWM x channel y input capture source select 
 * setting.
 * 
 *END**************************************************************************/
sim_tpm_ch_src_t sim_get_tpm_ch_src(uint8_t instance, uint8_t channel)
{
    assert (instance < HW_TPM_INSTANCE_COUNT);

    switch (instance)
    {
    case 1:
        switch (channel)
        {
        case 0:
            return (sim_tpm_ch_src_t)BR_SIM_SOPT4_TPM1CH0SRC;
        default:
            break;
        }
        break;
    case 2:
        switch (channel)
        {
        case 0:
            return (sim_tpm_ch_src_t)BR_SIM_SOPT4_TPM2CH0SRC;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return (sim_tpm_ch_src_t)0;
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/

