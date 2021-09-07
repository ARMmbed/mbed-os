/***************************************************************************/ /**
* \file cyhal_deprecated.c
*
* \brief
* Provides access to items that are device specific and no longer part of the
* common HAL API.
*
********************************************************************************
* \copyright
* Copyright 2018-2021 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
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
*******************************************************************************/

#include "cyhal_adc.h"
#include "cyhal_hwmgr.h"
#include "cyhal_clock.h"
#include "cyhal_deprecated.h"

#define HZ_PER_MHZ 1000000

uint32_t _cyhal_deprecated_get_src_freq(cy_en_clkpath_in_sources_t source)
{
    /* get the frequency of the source, i.e., the path mux input */
    switch(source)
    {
        case CY_SYSCLK_CLKPATH_IN_IMO: /* IMO frequency is fixed at 8 MHz */
            return CY_SYSCLK_IMO_FREQ;
        case CY_SYSCLK_CLKPATH_IN_ILO: /* ILO, WCO and PILO frequencies are nominally 32.768 kHz */
        case CY_SYSCLK_CLKPATH_IN_WCO:
        case CY_SYSCLK_CLKPATH_IN_PILO:
            return CY_SYSCLK_ILO_FREQ;
        default:
            return 0;
    }
}

static uint32_t _cyhal_deprecated_get_clkpath_freq(cy_en_clkhf_in_sources_t path, uint32_t freq, uint8_t *fll_pll_used)
{
    *fll_pll_used = 0xff;
    if (path == CY_SYSCLK_CLKHF_IN_CLKPATH0)
    {
        cy_stc_fll_manual_config_t fll_config;
        Cy_SysClk_FllGetConfiguration(&fll_config);
        if (fll_config.outputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT)
        {
            freq *= fll_config.fllMult;
            freq /= fll_config.refDiv;
            freq /= (fll_config.enableOutputDiv ? 2U : 1U);
            *fll_pll_used = 0;
        }
    }
    else if((uint32_t)path <= CY_SRSS_NUM_PLL)
    {
        cy_stc_pll_manual_config_t pll_config;
        Cy_SysClk_PllGetConfiguration(path, &pll_config);
        if (pll_config.outputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT)
        {
            freq *= pll_config.feedbackDiv;
            freq /= pll_config.referenceDiv;
            freq /= pll_config.outputDiv;
            *fll_pll_used = (uint8_t)path;
        }
    }
    return freq;
}

static cy_rslt_t _cyhal_deprecated_try_set_hf_divider(uint8_t hf_clock, uint32_t input_freq, uint32_t target_freq)
{
    bool divider_found = false;
    cy_en_clkhf_dividers_t divider;
    if (target_freq == input_freq)
    {
        divider_found = true;
        divider = CY_SYSCLK_CLKHF_NO_DIVIDE;
    }
    else if (target_freq * 2 == input_freq)
    {
        divider_found = true;
        divider = CY_SYSCLK_CLKHF_DIVIDE_BY_2;
    }
    else if (target_freq * 4 == input_freq)
    {
        divider_found = true;
        divider = CY_SYSCLK_CLKHF_DIVIDE_BY_4;
    }
    else if (target_freq * 8 == input_freq)
    {
        divider_found = true;
        divider = CY_SYSCLK_CLKHF_DIVIDE_BY_8;
    }

    if (divider_found)
    {
        Cy_SysClk_ClkHfSetDivider(hf_clock, divider);
        Cy_SysClk_ClkHfEnable(hf_clock);
        return CY_RSLT_SUCCESS;
    }
    else
    {
        return CYHAL_SYSTEM_RSLT_NO_VALID_DIVIDER;
    }
}

static cy_rslt_t _cyhal_deprecated_try_set_fll(uint8_t hf_clock, uint32_t target_freq)
{
    Cy_SysClk_FllDisable();
    Cy_SysClk_ClkHfSetSource(hf_clock, CY_SYSCLK_CLKHF_IN_CLKPATH0);
    Cy_SysClk_ClkPathSetSource(0, CY_SYSCLK_CLKPATH_IN_IMO);
    cy_rslt_t rslt = Cy_SysClk_FllConfigure(CY_SYSCLK_IMO_FREQ, target_freq, CY_SYSCLK_FLLPLL_OUTPUT_AUTO);
    if (rslt == CY_RSLT_SUCCESS)
    {
        // Wait up to 1 seconds for FLL to lock
        rslt = Cy_SysClk_FllEnable(1000000);
    }
    if (rslt == CY_RSLT_SUCCESS)
    {
        Cy_SysClk_ClkHfSetDivider(hf_clock, CY_SYSCLK_CLKHF_NO_DIVIDE);
        SystemCoreClockUpdate();
    }
    return rslt;
}

static cy_rslt_t _cyhal_deprecated_try_set_pll(uint8_t hf_clock, uint8_t pll, uint32_t target_freq)
{
    Cy_SysClk_PllDisable(pll);
    Cy_SysClk_ClkHfSetSource(hf_clock, (cy_en_clkhf_in_sources_t)(pll));

    cy_stc_pll_config_t cfg;
    cfg.inputFreq = CY_SYSCLK_IMO_FREQ;
    cfg.outputFreq = target_freq;
    cfg.lfMode = false;
    cfg.outputMode = CY_SYSCLK_FLLPLL_OUTPUT_AUTO;

    Cy_SysClk_ClkPathSetSource(pll, CY_SYSCLK_CLKPATH_IN_IMO);
    cy_rslt_t rslt = Cy_SysClk_PllConfigure(pll, &cfg);
    if (rslt == CY_RSLT_SUCCESS)
    {
        // Wait up to 1 seconds for PLL to lock
        rslt = Cy_SysClk_PllEnable(pll, 1000000);
    }
    if (rslt == CY_RSLT_SUCCESS)
    {
        Cy_SysClk_ClkHfSetDivider(hf_clock, CY_SYSCLK_CLKHF_NO_DIVIDE);
        SystemCoreClockUpdate();
    }
    return rslt;
}

cy_rslt_t cyhal_system_clock_get_frequency(uint8_t hf_clock, uint32_t *frequency_hz)
{
    *frequency_hz = Cy_SysClk_ClkHfGetFrequency(hf_clock);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_system_clock_set_frequency(uint8_t hf_clock, uint32_t frequency_hz)
{
    cy_en_clkhf_in_sources_t path = Cy_SysClk_ClkHfGetSource((uint32_t)hf_clock);
    cy_en_clkpath_in_sources_t source = Cy_SysClk_ClkPathGetSource((uint32_t)path);

    uint32_t src_freq = _cyhal_deprecated_get_src_freq(source);
    if (src_freq == 0)
    {
        return CYHAL_SYSTEM_RSLT_SRC_CLK_DISABLED;
    }
    uint8_t fll_pll_used;
    uint32_t clkpath_freq = _cyhal_deprecated_get_clkpath_freq(path, src_freq, &fll_pll_used);

    cy_rslt_t rslt = _cyhal_deprecated_try_set_hf_divider(hf_clock, clkpath_freq, frequency_hz);
    if (rslt == CY_RSLT_SUCCESS)
    {
        SystemCoreClockUpdate();
        return rslt;
    }

    bool enabled = Cy_SysClk_ClkHfIsEnabled(hf_clock);
    if (enabled && fll_pll_used == 0)
    {
        return _cyhal_deprecated_try_set_fll(hf_clock, frequency_hz);
    }
    else if (enabled && fll_pll_used <= SRSS_NUM_PLL)
    {
        return _cyhal_deprecated_try_set_pll(hf_clock, fll_pll_used, frequency_hz);
    }
    else
    {
        // Cannot get the correct frequency. Try to allocate an FLL or PLL
        cyhal_clock_t inst;
        rslt = cyhal_clock_allocate(&inst, CYHAL_CLOCK_BLOCK_PATHMUX);
        if (rslt == CY_RSLT_SUCCESS)
        {
            if (inst.channel < 1)
            {
                rslt = _cyhal_deprecated_try_set_fll(hf_clock, frequency_hz);
            }
            else if (inst.channel <= SRSS_NUM_PLL)
            {
                rslt = _cyhal_deprecated_try_set_pll(hf_clock, inst.channel, frequency_hz);
            }
            else
            {
                // No FLL or PLL available.
                rslt = CYHAL_SYSTEM_RSLT_UNABLE_TO_SET_CLK_FREQ;
            }

            if (!enabled && rslt == CY_RSLT_SUCCESS)
            {
                rslt = Cy_SysClk_ClkHfEnable(hf_clock);
            }

            if (rslt != CY_RSLT_SUCCESS)
            {
                cyhal_clock_free(&inst);
            }
        }
    }
    return rslt;
}

cy_rslt_t cyhal_system_clock_set_divider(cyhal_system_clock_t clock, cyhal_system_divider_t divider)
{
    if (divider < 1 || divider > 0x100)
    {
        return CYHAL_SYSTEM_RSLT_INVALID_CLK_DIVIDER;
    }
    switch(clock)
    {
        case CYHAL_SYSTEM_CLOCK_CM4:
        {
            Cy_SysClk_ClkFastSetDivider((uint8_t)(divider - 1));
            break;
        }
        case CYHAL_SYSTEM_CLOCK_CM0:
        {
            Cy_SysClk_ClkSlowSetDivider((uint8_t)(divider - 1));
            break;
        }
        case CYHAL_SYSTEM_CLOCK_PERI:
        {
            Cy_SysClk_ClkPeriSetDivider((uint8_t)(divider - 1));
            break;
        }
        default:
        {
            return CYHAL_SYSTEM_RSLT_INVALID_CLK_DIVIDER;
        }
    }
    SystemCoreClockUpdate();
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_system_register_callback(cyhal_system_callback_t *handler)
{
    return Cy_SysPm_RegisterCallback(handler)
        ? CY_RSLT_SUCCESS
        : CYHAL_SYSTEM_RSLT_ERROR;
}

cy_rslt_t cyhal_system_unregister_callback(cyhal_system_callback_t const *handler)
{
    return Cy_SysPm_UnregisterCallback(handler)
        ? CY_RSLT_SUCCESS
        : CYHAL_SYSTEM_RSLT_ERROR;
}

cy_rslt_t cyhal_hwmgr_allocate_clock(cyhal_clock_divider_t* obj, cyhal_clock_divider_types_t div, bool accept_larger)
{
    static uint8_t counts[] = { PERI_DIV_8_NR, PERI_DIV_16_NR, PERI_DIV_16_5_NR, PERI_DIV_24_5_NR };

    cyhal_clock_divider_types_t max_div_type = (accept_larger) ? (cyhal_clock_divider_types_t)(sizeof(counts) - 1) : div;
    cy_rslt_t rslt = CYHAL_HWMGR_RSLT_ERR_NONE_FREE;
    for(cyhal_clock_divider_types_t current_div = div; rslt != CY_RSLT_SUCCESS && current_div <= max_div_type; ++current_div)
    {
        uint8_t block = (uint8_t)current_div;
        uint8_t count = counts[block];

        for (uint8_t i = 0; rslt != CY_RSLT_SUCCESS && i < count; i++)
        {
            cyhal_resource_inst_t res = { CYHAL_RSC_CLOCK, block, i };
            bool reserved = (CY_RSLT_SUCCESS == cyhal_hwmgr_reserve(&res));
            if (reserved)
            {
                obj->div_type = current_div;
                obj->div_num = i;
                rslt = CY_RSLT_SUCCESS;
            }
        }
    }

    return rslt;
}

void cyhal_hwmgr_free_clock(cyhal_clock_divider_t* obj)
{
    cyhal_resource_inst_t res = { CYHAL_RSC_CLOCK, obj->div_type, obj->div_num };
    cyhal_hwmgr_free(&res);
}

cy_rslt_t cyhal_adc_channel_init(cyhal_adc_channel_t *obj, cyhal_adc_t* adc, cyhal_gpio_t pin)
{
    const cyhal_adc_channel_config_t DEFAULT_CHAN_CONFIG = { .enable_averaging = false, .min_acquisition_ns = 10u, .enabled = true};

    return cyhal_adc_channel_init_diff(obj, adc, pin, NC, &DEFAULT_CHAN_CONFIG);
}

