/***************************************************************************//**
* \file cyhal_system.c
*
* \brief
* Provides a high level interface for interacting with the Cypress power
* management and system clock configuration. This interface abstracts out the
* chip specific details. If any chip specific functionality is necessary, or
* performance is critical the low level functions can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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

#include "cyhal_system.h"
#include "cyhal_hwmgr.h"
#ifdef CY_RTOS_AWARE
#include "cyabs_rtos.h"
#endif

#ifdef CY_IP_MXS40SRSS

#if defined(__cplusplus)
extern "C"
{
#endif

#define HZ_PER_MHZ 1000000

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

cy_rslt_t cyhal_system_delay_ms(uint32_t milliseconds)
{
#ifdef CY_RTOS_AWARE
    return cy_rtos_delay_milliseconds(milliseconds);
#else
    Cy_SysLib_Delay(milliseconds);
    return CY_RSLT_SUCCESS;
#endif
}

uint32_t get_src_freq(cy_en_clkpath_in_sources_t source)
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

static uint32_t get_clkpath_freq(cy_en_clkhf_in_sources_t path, uint32_t freq, uint8_t *fll_pll_used)
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

static cy_rslt_t try_set_hf_divider(uint8_t clock, uint32_t input_freq, uint32_t target_freq)
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
        Cy_SysClk_ClkHfSetDivider(clock, divider);
        Cy_SysClk_ClkHfEnable(clock);
        return CY_RSLT_SUCCESS;
    }
    else
    {
        return CYHAL_SYSTEM_RSLT_NO_VALID_DIVIDER;
    }
}

static cy_rslt_t try_set_fll(uint8_t clock, uint32_t target_freq)
{
    Cy_SysClk_FllDisable();
    Cy_SysClk_ClkHfSetSource(clock, CY_SYSCLK_CLKHF_IN_CLKPATH0);
    Cy_SysClk_ClkPathSetSource(0, CY_SYSCLK_CLKPATH_IN_IMO);
    cy_rslt_t rslt = Cy_SysClk_FllConfigure(CY_SYSCLK_IMO_FREQ, target_freq, CY_SYSCLK_FLLPLL_OUTPUT_AUTO);
    if (rslt == CY_RSLT_SUCCESS)
    {
        // Wait up to 1 seconds for FLL to lock
        rslt = Cy_SysClk_FllEnable(1000000);
    }
    if (rslt == CY_RSLT_SUCCESS)
    {
        Cy_SysClk_ClkHfSetDivider(clock, CY_SYSCLK_CLKHF_NO_DIVIDE);
        SystemCoreClockUpdate();
    }
    return rslt;
}

static cy_rslt_t try_set_pll(uint8_t clock, uint8_t pll, uint32_t target_freq)
{
    Cy_SysClk_PllDisable(pll);
    Cy_SysClk_ClkHfSetSource(clock, (cy_en_clkhf_in_sources_t)(pll));

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
        Cy_SysClk_ClkHfSetDivider(clock, CY_SYSCLK_CLKHF_NO_DIVIDE);
        SystemCoreClockUpdate();
    }
    return rslt;
}

/* This should be part of the PDL */
static inline bool cyhal_sysclk_clkhfisenabled(uint32_t clkHf)
{
    bool retVal = false;
    if (clkHf < CY_SRSS_NUM_HFROOT)
    {
        retVal = _FLD2BOOL(SRSS_CLK_ROOT_SELECT_ENABLE, SRSS_CLK_ROOT_SELECT[clkHf]);
    }
    return (retVal);
}

cy_rslt_t cyhal_system_clock_get_frequency(uint8_t clock, uint32_t *frequency_hz)
{
    *frequency_hz = Cy_SysClk_ClkHfGetFrequency(clock);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_system_clock_set_frequency(uint8_t clock, uint32_t frequency_hz)
{
    cy_en_clkhf_in_sources_t path = Cy_SysClk_ClkHfGetSource((uint32_t)clock);
    cy_en_clkpath_in_sources_t source = Cy_SysClk_ClkPathGetSource((uint32_t)path);

    uint32_t src_freq = get_src_freq(source);
    if (src_freq == 0)
    {
        return CYHAL_SYSTEM_RSLT_SRC_CLK_DISABLED;
    }
    uint8_t fll_pll_used;
    uint32_t clkpath_freq = get_clkpath_freq(path, src_freq, &fll_pll_used);

    cy_rslt_t rslt = try_set_hf_divider(clock, clkpath_freq, frequency_hz);
    if (rslt == CY_RSLT_SUCCESS)
    {
        SystemCoreClockUpdate();
        return rslt;
    }

    bool enabled = cyhal_sysclk_clkhfisenabled(clock);
    if (enabled && fll_pll_used == 0)
    {
        return try_set_fll(clock, frequency_hz);
    }
    else if (enabled && fll_pll_used <= SRSS_NUM_PLL)
    {
        return try_set_pll(clock, fll_pll_used, frequency_hz);
    }
    else
    {
        // Cannot get the correct frequency. Try to allocate an FLL or PLL
        cyhal_resource_inst_t inst;
        rslt = cyhal_hwmgr_allocate(CYHAL_RSC_CLKPATH, &inst);
        if (rslt == CY_RSLT_SUCCESS)
        {
            if (inst.block_num == 0)
            {
                rslt = try_set_fll(clock, frequency_hz);
            }
            else if (inst.block_num <= SRSS_NUM_PLL)
            {
                rslt = try_set_pll(clock, inst.block_num, frequency_hz);
            }
            else
            {
                // No FLL or PLL available.
                rslt = CYHAL_SYSTEM_RSLT_UNABLE_TO_SET_CLK_FREQ;
            }

            if (!enabled && rslt == CY_RSLT_SUCCESS)
            {
                rslt = Cy_SysClk_ClkHfEnable(clock);
            }

            if (rslt != CY_RSLT_SUCCESS)
            {
                cyhal_hwmgr_free(&inst);
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
            Cy_SysClk_ClkFastSetDivider(divider - 1);
            break;
        }
        case CYHAL_SYSTEM_CLOCK_CM0:
        {
            Cy_SysClk_ClkSlowSetDivider(divider - 1);
            break;
        }
        case CYHAL_SYSTEM_CLOCK_PERI:
        {
            Cy_SysClk_ClkPeriSetDivider(divider - 1);
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

cyhal_reset_reason_t cyhal_system_get_reset_reason(void)
{
    uint32_t pdl_reason = Cy_SysLib_GetResetReason();
    cyhal_reset_reason_t reason = CYHAL_SYSTEM_RESET_NONE;

    if (CY_SYSLIB_RESET_ACT_FAULT & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_ACTIVE_FAULT;
    if (CY_SYSLIB_RESET_DPSLP_FAULT & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_DEEPSLEEP_FAULT;
    if (CY_SYSLIB_RESET_SOFT & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_SOFT;
    if (CY_SYSLIB_RESET_HIB_WAKEUP & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_HIB_WAKEUP;
    if ((CY_SYSLIB_RESET_HWWDT | CY_SYSLIB_RESET_SWWDT0 | CY_SYSLIB_RESET_SWWDT1 |
        CY_SYSLIB_RESET_SWWDT2 | CY_SYSLIB_RESET_SWWDT3) & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_WDT;
#if (SRSS_WCOCSV_PRESENT != 0U)
    if (CY_SYSLIB_RESET_CSV_WCO_LOSS & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_WCO_ERR;
#endif
#if (SRSS_MASK_HFCSV != 0)
    if ((CY_SYSLIB_RESET_HFCLK_LOSS | CY_SYSLIB_RESET_HFCLK_ERR) & pdl_reason)
        reason |= CYHAL_SYSTEM_RESET_SYS_CLK_ERR;
#endif

    return reason;
}

void cyhal_system_clear_reset_reason(void)
{
    Cy_SysLib_ClearResetReason();
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXS40SRSS */
