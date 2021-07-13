/*******************************************************************************
* File Name: cyhal_clock.c
*
* Description:
* Provides an implementation for high level interface for interacting with the
* Cypress Clocks. This is a wrapper around the lower level PDL API.
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

#include <stdlib.h>
#include "cy_sysclk.h"
#include "cy_utils.h"
#include "cyhal_clock.h"
#include "cyhal_utils.h"
#include "cyhal_hwmgr.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#define _CYHAL_CLOCK_FLL_LOCK_TIME (200000UL)
#define _CYHAL_CLOCK_PLL_LOCK_TIME (10000UL)

/******************************************************************************
 ****************************** Clock Resources *******************************
 *****************************************************************************/
const cyhal_resource_inst_t CYHAL_CLOCK_IMO = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_IMO, 0 };
const cyhal_resource_inst_t CYHAL_CLOCK_EXT = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_EXT, 0 };
const cyhal_resource_inst_t CYHAL_CLOCK_ILO = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_ILO, 0 };

const cyhal_clock_tolerance_t CYHAL_CLOCK_TOLERANCE_0_P = {CYHAL_TOLERANCE_PERCENT, 0};
const cyhal_clock_tolerance_t CYHAL_CLOCK_TOLERANCE_1_P = {CYHAL_TOLERANCE_PERCENT, 1};
const cyhal_clock_tolerance_t CYHAL_CLOCK_TOLERANCE_5_P = {CYHAL_TOLERANCE_PERCENT, 5};

#if SRSS_ECO_PRESENT
const cyhal_resource_inst_t CYHAL_CLOCK_ECO = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_ECO, 0 };
#endif
#if SRSS_ALTHF_PRESENT
const cyhal_resource_inst_t CYHAL_CLOCK_ALTHF = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_ALTHF, 0 };
#endif
#if SRSS_ALTLF_PRESENT
const cyhal_resource_inst_t CYHAL_CLOCK_ALTLF = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_ALTLF, 0 };
#endif
#if SRSS_PILO_PRESENT
const cyhal_resource_inst_t CYHAL_CLOCK_PILO = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PILO, 0 };
#endif
#if SRSS_BACKUP_PRESENT
const cyhal_resource_inst_t CYHAL_CLOCK_WCO = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_WCO, 0 };
#endif
#if SRSS_MFO_PRESENT
const cyhal_resource_inst_t CYHAL_CLOCK_MFO = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_MFO, 0 };
const cyhal_resource_inst_t CYHAL_CLOCK_MF = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_MF, 0 };
#endif

const cyhal_resource_inst_t CYHAL_CLOCK_LF = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_LF, 0 };
const cyhal_resource_inst_t CYHAL_CLOCK_PUMP = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PUMP, 0 };
const cyhal_resource_inst_t CYHAL_CLOCK_BAK = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_BAK, 0 };
const cyhal_resource_inst_t CYHAL_CLOCK_FAST = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_FAST, 0 };
const cyhal_resource_inst_t CYHAL_CLOCK_PERI = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PERI, 0 };
const cyhal_resource_inst_t CYHAL_CLOCK_TIMER = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_TIMER, 0 };
const cyhal_resource_inst_t CYHAL_CLOCK_SLOW = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_SLOW, 0 };
const cyhal_resource_inst_t CYHAL_CLOCK_ALT_SYS_TICK = { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_ALT_SYS_TICK, 0 };

const cyhal_resource_inst_t CYHAL_CLOCK_PATHMUX[SRSS_NUM_CLKPATH] =
{
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 0 },
#if (SRSS_NUM_CLKPATH > 1)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 1 },
#endif
#if (SRSS_NUM_CLKPATH > 2)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 2 },
#endif
#if (SRSS_NUM_CLKPATH > 3)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 3 },
#endif
#if (SRSS_NUM_CLKPATH > 4)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 4 },
#endif
#if (SRSS_NUM_CLKPATH > 5)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 5 },
#endif
#if (SRSS_NUM_CLKPATH > 6)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 6 },
#endif
#if (SRSS_NUM_CLKPATH > 7)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 7 },
#endif
#if (SRSS_NUM_CLKPATH > 8)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 8 },
#endif
#if (SRSS_NUM_CLKPATH > 9)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 9 },
#endif
#if (SRSS_NUM_CLKPATH > 10)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 10 },
#endif
#if (SRSS_NUM_CLKPATH > 11)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 11 },
#endif
#if (SRSS_NUM_CLKPATH > 12)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 12 },
#endif
#if (SRSS_NUM_CLKPATH > 13)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 13 },
#endif
#if (SRSS_NUM_CLKPATH > 14)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 14 },
#endif
#if (SRSS_NUM_CLKPATH > 15)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PATHMUX, 15 },
#endif
};

const cyhal_resource_inst_t CYHAL_CLOCK_FLL = { CYHAL_RSC_CLOCK, CYHAL_CLOCK_BLOCK_FLL, 0 };
#if (SRSS_NUM_PLL > 0)
const cyhal_resource_inst_t CYHAL_CLOCK_PLL[SRSS_NUM_PLL] =
{
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 0 },
#if (SRSS_NUM_PLL > 1)
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 1 },
#endif
#if (SRSS_NUM_PLL > 2)
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 2 },
#endif
#if (SRSS_NUM_PLL > 3)
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 3 },
#endif
#if (SRSS_NUM_PLL > 4)
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 4 },
#endif
#if (SRSS_NUM_PLL > 5)
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 5 },
#endif
#if (SRSS_NUM_PLL > 6)
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 6 },
#endif
#if (SRSS_NUM_PLL > 7)
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 7 },
#endif
#if (SRSS_NUM_PLL > 8)
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 8 },
#endif
#if (SRSS_NUM_PLL > 9)
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 9 },
#endif
#if (SRSS_NUM_PLL > 10)
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 10 },
#endif
#if (SRSS_NUM_PLL > 11)
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 11 },
#endif
#if (SRSS_NUM_PLL > 12)
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 12 },
#endif
#if (SRSS_NUM_PLL > 13)
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 13 },
#endif
#if (SRSS_NUM_PLL > 14)
   { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_PLL, 14 },
#endif
};
#endif

const cyhal_resource_inst_t CYHAL_CLOCK_HF[SRSS_NUM_HFROOT] =
{
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 0 },
#if (SRSS_NUM_HFROOT > 1)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 1 },
#endif
#if (SRSS_NUM_HFROOT > 2)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 2 },
#endif
#if (SRSS_NUM_HFROOT > 3)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 3 },
#endif
#if (SRSS_NUM_HFROOT > 4)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 4 },
#endif
#if (SRSS_NUM_HFROOT > 5)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 5 },
#endif
#if (SRSS_NUM_HFROOT > 6)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 6 },
#endif
#if (SRSS_NUM_HFROOT > 7)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 7 },
#endif
#if (SRSS_NUM_HFROOT > 8)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 8 },
#endif
#if (SRSS_NUM_HFROOT > 9)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 9 },
#endif
#if (SRSS_NUM_HFROOT > 10)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 10 },
#endif
#if (SRSS_NUM_HFROOT > 11)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 11 },
#endif
#if (SRSS_NUM_HFROOT > 12)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 12 },
#endif
#if (SRSS_NUM_HFROOT > 13)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 13 },
#endif
#if (SRSS_NUM_HFROOT > 14)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 14 },
#endif
#if (SRSS_NUM_HFROOT > 15)
    { CYHAL_RSC_CLOCK, (uint8_t)CYHAL_CLOCK_BLOCK_HF, 15 },
#endif
};

/******************************************************************************
 ***************************** Support Functions*******************************
 *****************************************************************************/
static void _cyhal_clock_setup_inst(cyhal_clock_t *clock, const cyhal_resource_inst_t* resource, bool reserved)
{
    clock->div_type = (cy_en_divider_types_t)resource->block_num;
    clock->div_num = resource->channel_num;
    clock->block = (cyhal_clock_block_t)resource->block_num;
    clock->channel = resource->channel_num;
    clock->reserved = reserved;
}

static cy_rslt_t _cyhal_clock_compute_div(uint32_t input_hz, uint32_t desired_hz, uint32_t divider_bits, const cyhal_clock_tolerance_t *tolerance, uint32_t *div)
{
    uint32_t max_div = (1 << divider_bits);
    *div = (input_hz + (desired_hz / 2)) / desired_hz;
    if (*div > max_div)
        *div = max_div;

    uint32_t diff = (tolerance != NULL)
                    ? (uint32_t)abs(_cyhal_utils_calculate_tolerance(tolerance->type, desired_hz, input_hz / *div))
                    : 0;

    return (tolerance != NULL && diff > tolerance->value)
        ? CYHAL_CLOCK_RSLT_ERR_FREQ
        : CY_RSLT_SUCCESS;
}

static uint32_t _cyhal_clock_get_lf_frequency(void)
{
    cy_en_clklf_in_sources_t source = Cy_SysClk_ClkLfGetSource();
    switch (source)
    {
        case CY_SYSCLK_CLKLF_IN_ILO:
            return CY_SYSCLK_ILO_FREQ;
#if SRSS_PILO_PRESENT
        case CY_SYSCLK_CLKLF_IN_PILO:
            return CY_SYSCLK_PILO_FREQ;
#endif
#if SRSS_BACKUP_PRESENT
        case CY_SYSCLK_CLKLF_IN_WCO:
            return CY_SYSCLK_WCO_FREQ;
#endif
#if SRSS_ALTLF_PRESENT
        case CY_SYSCLK_CLKLF_IN_ALTLF:
            return Cy_SysClk_AltLfGetFrequency();
#endif
        default:
            CY_ASSERT(false);
            return 0;
    }
}

static uint32_t _cyhal_clock_get_channel_count(cyhal_clock_block_t block)
{
    switch (block)
    {
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_8BIT:
            return PERI_DIV_8_NR;
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT:
            return PERI_DIV_16_NR;
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16_5BIT:
            return PERI_DIV_16_5_NR;
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_24_5BIT:
            return PERI_DIV_24_5_NR;
        case CYHAL_CLOCK_BLOCK_PATHMUX:
            return SRSS_NUM_CLKPATH;
        case CYHAL_CLOCK_BLOCK_PLL:
            return SRSS_NUM_PLL;
        case CYHAL_CLOCK_BLOCK_HF:
            return SRSS_NUM_HFROOT;
        default:
            return 1;
    }
}

static void _cyhal_clock_update_system_state(bool before_change, uint32_t old_sysclk_freq_hz, uint32_t new_sysclk_freq_hz)
{
    // If increasing the clock frequency we need to update the speeds
    // before the change. If decreasing the frequency we need to update
    // after the change.
    if ((before_change == (bool)(new_sysclk_freq_hz > old_sysclk_freq_hz)) ||
        (!before_change == (new_sysclk_freq_hz < old_sysclk_freq_hz)))
    {
        bool is_ulp = Cy_SysPm_IsSystemUlp();
        Cy_SysLib_SetWaitStates(is_ulp, new_sysclk_freq_hz / 1000000);
    }

    // If after the change, update the clock
    if (!before_change)
        SystemCoreClockUpdate();
}

static inline cy_rslt_t _cyhal_clock_set_hfclk_div(uint8_t clk, uint32_t div_val)
{
    cy_en_clkhf_dividers_t new_div;
    switch (div_val)
    {
        case 1:
            new_div = CY_SYSCLK_CLKHF_NO_DIVIDE;
            break;
        case 2:
            new_div = CY_SYSCLK_CLKHF_DIVIDE_BY_2;
            break;
        case 4:
            new_div = CY_SYSCLK_CLKHF_DIVIDE_BY_4;
            break;
        case 8:
            new_div = CY_SYSCLK_CLKHF_DIVIDE_BY_8;
            break;
        default:
            return CYHAL_CLOCK_RSLT_ERR_FREQ;
    }

    /* Only used if updating HFClk 0 */
    uint32_t old_div = (uint32_t)Cy_SysClk_ClkHfGetDivider(0);
    uint32_t src = (uint32_t)Cy_SysClk_ClkHfGetSource(0);
    uint32_t path_freq = Cy_SysClk_ClkPathGetFrequency(src);
    uint32_t old_freq = path_freq >> old_div;
    uint32_t new_freq = path_freq >> ((uint32_t)new_div);

    if (0 == clk)
        _cyhal_clock_update_system_state(true, old_freq, new_freq);

    cy_rslt_t rslt = (cy_rslt_t)Cy_SysClk_ClkHfSetDivider(clk, new_div);

    if (0 == clk)
    {
        if (CY_RSLT_SUCCESS == rslt)
            _cyhal_clock_update_system_state(false, old_freq, new_freq);
        else // revert the change if there was one
            _cyhal_clock_update_system_state(false, new_freq, old_freq);
    }

    return rslt;
}

static inline cy_rslt_t _cyhal_clock_set_hfclk_source(uint8_t clk, const cyhal_clock_t *source)
{
    uint32_t new_src;
    if (source->block == CYHAL_CLOCK_BLOCK_PATHMUX || source->block == CYHAL_CLOCK_BLOCK_FLL)
        new_src = source->channel;
    else if (source->block == CYHAL_CLOCK_BLOCK_PLL)
        new_src = source->channel + 1;
    else
        return CYHAL_CLOCK_RSLT_ERR_SOURCE;

    /* Only used if updating HFClk 0 */
    uint32_t div = (uint32_t)Cy_SysClk_ClkHfGetDivider(0);
    uint32_t old_src = (uint32_t)Cy_SysClk_ClkHfGetSource(0);
    uint32_t old_freq = Cy_SysClk_ClkPathGetFrequency(old_src) >> div;
    uint32_t new_freq = Cy_SysClk_ClkPathGetFrequency(new_src) >> div;

    if (0 == clk)
        _cyhal_clock_update_system_state(true, old_freq, new_freq);

    cy_rslt_t rslt = Cy_SysClk_ClkHfSetSource(clk, (cy_en_clkhf_in_sources_t)new_src);

    if (0 == clk)
    {
        if (CY_RSLT_SUCCESS == rslt)
            _cyhal_clock_update_system_state(false, old_freq, new_freq);
        else // revert the change if there was one
            _cyhal_clock_update_system_state(false, new_freq, old_freq);
    }

    return rslt;
}

static inline cy_rslt_t _cyhal_clock_set_pathmux_source(uint8_t mux, cyhal_clock_block_t source)
{
    uint32_t new_freq;
    cy_en_clkpath_in_sources_t clkpath_src;
    switch (source)
    {
        case CYHAL_CLOCK_BLOCK_IMO:
            clkpath_src = CY_SYSCLK_CLKPATH_IN_IMO;
            new_freq = CY_SYSCLK_IMO_FREQ;
            break;
        case CYHAL_CLOCK_BLOCK_EXT:
            clkpath_src = CY_SYSCLK_CLKPATH_IN_EXT;
            new_freq = Cy_SysClk_ExtClkGetFrequency();
            break;
#if SRSS_ECO_PRESENT
        case CYHAL_CLOCK_BLOCK_ECO:
            clkpath_src = CY_SYSCLK_CLKPATH_IN_ECO;
            new_freq = Cy_SysClk_EcoGetFrequency();
            break;
#endif
#if SRSS_ALTHF_PRESENT
        case CYHAL_CLOCK_BLOCK_ALTHF:
            clkpath_src = CY_SYSCLK_CLKPATH_IN_ALTHF;
            new_freq = Cy_SysClk_AltHfGetFrequency();
            break;
#endif
        case CYHAL_CLOCK_BLOCK_ILO:
            clkpath_src = CY_SYSCLK_CLKPATH_IN_ILO;
            new_freq = CY_SYSCLK_ILO_FREQ;
            break;
#if SRSS_BACKUP_PRESENT
        case CYHAL_CLOCK_BLOCK_WCO:
            clkpath_src = CY_SYSCLK_CLKPATH_IN_WCO;
            new_freq = CY_SYSCLK_WCO_FREQ;
            break;
#endif
#if SRSS_ALTLF_PRESENT
        case CYHAL_CLOCK_BLOCK_ALTLF:
            clkpath_src = CY_SYSCLK_CLKPATH_IN_ALTLF;
            new_freq = Cy_SysClk_AltLfGetFrequency();
            break;
#endif
#if SRSS_PILO_PRESENT
        case CYHAL_CLOCK_BLOCK_PILO:
            clkpath_src = CY_SYSCLK_CLKPATH_IN_PILO;
            new_freq = CY_SYSCLK_PILO_FREQ;
            break;
#endif
        default:
            CY_ASSERT(false); //Unhandled clock
            return CYHAL_CLOCK_RSLT_ERR_SOURCE;
    }

    uint32_t old_hf_freq = Cy_SysClk_ClkHfGetFrequency(0);
    uint32_t new_hf_freq = new_freq >> ((uint8_t)Cy_SysClk_ClkHfGetDivider(0));
    bool is_sysclk_path = (mux == (uint32_t)Cy_SysClk_ClkHfGetSource(0));

    if (is_sysclk_path)
        _cyhal_clock_update_system_state(true, old_hf_freq, new_hf_freq);

    cy_rslt_t rslt = Cy_SysClk_ClkPathSetSource(mux, clkpath_src);

    if (is_sysclk_path)
    {
        if (CY_RSLT_SUCCESS == rslt)
            _cyhal_clock_update_system_state(false, old_hf_freq, new_hf_freq);
        else // revert the change if there was one
            _cyhal_clock_update_system_state(false, new_hf_freq, old_hf_freq);
    }

    return rslt;
}

static inline cy_rslt_t _cyhal_clock_change_fll_enablement(bool enable, bool wait_for_lock)
{
    cy_stc_fll_manual_config_t cfg;
    Cy_SysClk_FllGetConfiguration(&cfg);
    uint32_t new_freq, old_freq;
    uint32_t div = (uint32_t)Cy_SysClk_ClkHfGetDivider(0);
    uint32_t src_freq = Cy_SysClk_ClkPathMuxGetFrequency(0);
    uint32_t fll_freq = CY_SYSLIB_DIV_ROUND((uint64_t)src_freq * (uint64_t)cfg.fllMult, (uint32_t)cfg.refDiv * ((cfg.enableOutputDiv) ? 2UL : 1UL));
    if (enable)
    {
        new_freq = fll_freq >> div;
        old_freq = src_freq >> div;
    }
    else
    {
        new_freq = src_freq >> div;
        old_freq = fll_freq >> div;
    }

    bool fll_sources_hf0 = (0 == (uint32_t)Cy_SysClk_ClkHfGetSource(0));
    if (fll_sources_hf0)
        _cyhal_clock_update_system_state(true, old_freq, new_freq);

    cy_rslt_t rslt = (enable)
        ? Cy_SysClk_FllEnable(wait_for_lock ? _CYHAL_CLOCK_FLL_LOCK_TIME : 0UL)
        : Cy_SysClk_FllDisable();

    if (fll_sources_hf0)
    {
        if (CY_RSLT_SUCCESS == rslt)
            _cyhal_clock_update_system_state(false, old_freq, new_freq);
        else // revert the change if there was one
            _cyhal_clock_update_system_state(false, new_freq, old_freq);
    }

    return rslt;
}

static inline cy_rslt_t _cyhal_clock_set_fll_freq(uint32_t new_freq)
{
    cy_rslt_t rslt = CY_RSLT_SUCCESS;
    cy_stc_fll_manual_config_t cfg;
    Cy_SysClk_FllGetConfiguration(&cfg);
    uint32_t src_freq = Cy_SysClk_ClkPathMuxGetFrequency(0);

    if (0 == src_freq)
        rslt = CYHAL_CLOCK_RSLT_ERR_SOURCE;
    else
    {
        uint32_t old_freq = CY_SYSLIB_DIV_ROUND((uint64_t)src_freq * (uint64_t)cfg.fllMult, (uint32_t)cfg.refDiv * ((cfg.enableOutputDiv) ? 2UL : 1UL));
        uint32_t div = (uint32_t)Cy_SysClk_ClkHfGetDivider(0);
        uint32_t old_hf_freq = old_freq >> div;
        uint32_t new_hf_freq = new_freq >> div;

        bool fll_sources_hf0 = (0 == (uint32_t)Cy_SysClk_ClkHfGetSource(0));
        if (fll_sources_hf0)
            _cyhal_clock_update_system_state(true, old_hf_freq, new_hf_freq);

        bool enabled = Cy_SysClk_FllIsEnabled();
        if (enabled)
            rslt = Cy_SysClk_FllDisable();
        if (CY_RSLT_SUCCESS == rslt)
        {
            rslt = Cy_SysClk_FllConfigure(src_freq, new_freq, CY_SYSCLK_FLLPLL_OUTPUT_AUTO);

            if (enabled)
            {
                cy_rslt_t rslt2 = Cy_SysClk_FllEnable(_CYHAL_CLOCK_FLL_LOCK_TIME);
                if (CY_RSLT_SUCCESS == rslt)
                    rslt = rslt2;
            }
        }

        if (fll_sources_hf0)
        {
            if (CY_RSLT_SUCCESS == rslt)
                _cyhal_clock_update_system_state(false, old_hf_freq, new_hf_freq);
            else // revert the change if there was one
                _cyhal_clock_update_system_state(false, new_hf_freq, old_hf_freq);
        }
    }

    return rslt;
}

#if (SRSS_NUM_PLL > 0)
//pll_idx is the path mux index (eg PLL number + 1) as used by PDL APIs
static inline cy_rslt_t _cyhal_clock_change_pll_enablement(uint8_t pll_idx, bool enable, bool wait_for_lock)
{
    cy_stc_pll_manual_config_t cfg;
    cy_rslt_t rslt = Cy_SysClk_PllGetConfiguration(pll_idx, &cfg);
    if (CY_RSLT_SUCCESS == rslt)
    {
        uint32_t new_freq, old_freq;
        uint32_t div = (uint32_t)Cy_SysClk_ClkHfGetDivider(0);
        uint32_t src_freq = Cy_SysClk_ClkPathMuxGetFrequency(pll_idx);
        uint32_t pll_freq = CY_SYSLIB_DIV_ROUND((uint64_t)src_freq * (uint64_t)cfg.feedbackDiv, (uint32_t)cfg.referenceDiv * (uint32_t)cfg.outputDiv);
        if (enable)
        {
            new_freq = pll_freq >> div;
            old_freq = src_freq >> div;
        }
        else
        {
            new_freq = src_freq >> div;
            old_freq = pll_freq >> div;
        }

        bool pll_sources_hf0 = (pll_idx == (uint32_t)Cy_SysClk_ClkHfGetSource(0));
        if (pll_sources_hf0)
            _cyhal_clock_update_system_state(true, old_freq, new_freq);

        rslt = (enable)
            ? Cy_SysClk_PllEnable(pll_idx, wait_for_lock ? _CYHAL_CLOCK_PLL_LOCK_TIME : 0UL)
            : Cy_SysClk_PllDisable(pll_idx);

        if (pll_sources_hf0)
        {
            if (CY_RSLT_SUCCESS == rslt)
                _cyhal_clock_update_system_state(false, old_freq, new_freq);
            else // revert the change if there was one
                _cyhal_clock_update_system_state(false, new_freq, old_freq);
        }
    }

    return rslt;
}

static inline cy_rslt_t _cyhal_clock_set_pll_freq(uint8_t pll_idx, uint32_t new_freq)
{
    cy_stc_pll_manual_config_t cfg;
    cy_rslt_t rslt = Cy_SysClk_PllGetConfiguration(pll_idx, &cfg);
    if (CY_RSLT_SUCCESS == rslt)
    {
        bool enabled = Cy_SysClk_PllIsEnabled(pll_idx);
        if (enabled)
            rslt = Cy_SysClk_PllDisable(pll_idx);
        if (CY_RSLT_SUCCESS == rslt)
        {
            uint32_t src_freq = Cy_SysClk_ClkPathMuxGetFrequency(pll_idx);
            uint32_t old_freq = CY_SYSLIB_DIV_ROUND((uint64_t)src_freq * (uint64_t)cfg.feedbackDiv, (uint32_t)cfg.referenceDiv * (uint32_t)cfg.outputDiv);

            uint32_t div = (uint32_t)Cy_SysClk_ClkHfGetDivider(0);
            uint32_t old_hf_freq = old_freq >> div;
            uint32_t new_hf_freq = new_freq >> div;

            bool pll_sources_hf0 = (pll_idx == (uint32_t)Cy_SysClk_ClkHfGetSource(0));
            if (pll_sources_hf0)
                _cyhal_clock_update_system_state(true, old_hf_freq, new_hf_freq);

            uint32_t input_hz = Cy_SysClk_ClkPathMuxGetFrequency(pll_idx);
            cy_stc_pll_config_t cfg =
            {
                .inputFreq = input_hz,
                .outputFreq = new_freq,
                .lfMode = false,
                .outputMode = CY_SYSCLK_FLLPLL_OUTPUT_AUTO,
            };
            rslt = Cy_SysClk_PllConfigure(pll_idx, &cfg);

            if (enabled)
            {
                cy_rslt_t rslt2 = Cy_SysClk_PllEnable(pll_idx, _CYHAL_CLOCK_PLL_LOCK_TIME);
                if (CY_RSLT_SUCCESS == rslt)
                    rslt = rslt2;
            }

            if (pll_sources_hf0)
            {
                if (CY_RSLT_SUCCESS == rslt)
                    _cyhal_clock_update_system_state(false, old_hf_freq, new_hf_freq);
                else // revert the change if there was one
                    _cyhal_clock_update_system_state(false, new_hf_freq, old_hf_freq);
            }
        }
    }

    return rslt;
}
#endif

/******************************************************************************
 **************************** Public API (clocks) *****************************
 *****************************************************************************/
cy_rslt_t cyhal_clock_get(cyhal_clock_t *clock, const cyhal_resource_inst_t *resource)
{
    CY_ASSERT(NULL != clock);
    CY_ASSERT(NULL != resource);
    CY_ASSERT(CYHAL_RSC_CLOCK == resource->type);

    _cyhal_clock_setup_inst(clock, resource, false);

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_clock_allocate(cyhal_clock_t *clock, cyhal_clock_block_t block)
{
    CY_ASSERT(NULL != clock);

    uint8_t maxChannels = (uint8_t)_cyhal_clock_get_channel_count(block);
    for (uint8_t i = 0; i < maxChannels; i++)
    {
        cyhal_resource_inst_t clock_resource = { CYHAL_RSC_CLOCK, block, i };
        if (CY_RSLT_SUCCESS == cyhal_hwmgr_reserve(&clock_resource))
        {
            _cyhal_clock_setup_inst(clock, &clock_resource, true);
            return CY_RSLT_SUCCESS;
        }
    }
    return CYHAL_HWMGR_RSLT_ERR_NONE_FREE;
}

cy_rslt_t cyhal_clock_init(cyhal_clock_t *clock)
{
    CY_ASSERT(NULL != clock);
    CY_ASSERT(_cyhal_utils_is_new_clock_format(clock));

    cyhal_resource_inst_t resource = { CYHAL_RSC_CLOCK, clock->block, clock->channel };
    cy_rslt_t rslt = cyhal_hwmgr_reserve(&resource);
    if (CY_RSLT_SUCCESS == rslt)
    {
        clock->reserved = true;
    }

    return rslt;
}

cyhal_clock_feature_t cyhal_clock_get_features(const cyhal_clock_t *clock)
{
    CY_ASSERT(NULL != clock);
    CY_ASSERT(_cyhal_utils_is_new_clock_format(clock));

    switch (clock->block)
    {
        case CYHAL_CLOCK_BLOCK_IMO:
#if SRSS_ALTHF_PRESENT
        case CYHAL_CLOCK_BLOCK_ALTHF:
#endif
#if SRSS_ALTLF_PRESENT
        case CYHAL_CLOCK_BLOCK_ALTLF:
#endif
            return CYHAL_CLOCK_FEATURE_NONE;

        case CYHAL_CLOCK_BLOCK_ILO:
#if SRSS_ECO_PRESENT
        case CYHAL_CLOCK_BLOCK_ECO: //We don't allow setting frequency because we don't have the necessary args
#endif
#if SRSS_PILO_PRESENT
        case CYHAL_CLOCK_BLOCK_PILO:
#endif
#if SRSS_BACKUP_PRESENT
        case CYHAL_CLOCK_BLOCK_WCO:
#endif
#if SRSS_MFO_PRESENT
        case CYHAL_CLOCK_BLOCK_MFO:
#endif
            return CYHAL_CLOCK_FEATURE_ENABLE;

        case CYHAL_CLOCK_BLOCK_EXT:
            return CYHAL_CLOCK_FEATURE_FREQUENCY;

        case CYHAL_CLOCK_BLOCK_FLL:
#if (SRSS_NUM_PLL > 0)
        case CYHAL_CLOCK_BLOCK_PLL:
#endif
            return (cyhal_clock_feature_t)(CYHAL_CLOCK_FEATURE_ENABLE | CYHAL_CLOCK_FEATURE_FREQUENCY);

        case CYHAL_CLOCK_BLOCK_FAST:
        case CYHAL_CLOCK_BLOCK_PERI:
        case CYHAL_CLOCK_BLOCK_SLOW:
            return (cyhal_clock_feature_t)(CYHAL_CLOCK_FEATURE_DIVIDER | CYHAL_CLOCK_FEATURE_FREQUENCY);

#if SRSS_MFO_PRESENT
        case CYHAL_CLOCK_BLOCK_MF:
#endif
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_8BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16_5BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_24_5BIT:
            return (cyhal_clock_feature_t)(CYHAL_CLOCK_FEATURE_ENABLE | CYHAL_CLOCK_FEATURE_DIVIDER | CYHAL_CLOCK_FEATURE_FREQUENCY);

        case CYHAL_CLOCK_BLOCK_HF:
            if (clock->channel == 0)    // HF0 cannot be disabled
                return (cyhal_clock_feature_t)(CYHAL_CLOCK_FEATURE_SOURCE | CYHAL_CLOCK_FEATURE_DIVIDER);
            return (cyhal_clock_feature_t)(CYHAL_CLOCK_FEATURE_ENABLE | CYHAL_CLOCK_FEATURE_SOURCE | CYHAL_CLOCK_FEATURE_DIVIDER);
        case CYHAL_CLOCK_BLOCK_PUMP:
        case CYHAL_CLOCK_BLOCK_TIMER:
            return (cyhal_clock_feature_t)(CYHAL_CLOCK_FEATURE_ENABLE | CYHAL_CLOCK_FEATURE_SOURCE | CYHAL_CLOCK_FEATURE_DIVIDER);

        case CYHAL_CLOCK_BLOCK_PATHMUX:
        case CYHAL_CLOCK_BLOCK_LF:
        case CYHAL_CLOCK_BLOCK_BAK:
        case CYHAL_CLOCK_BLOCK_ALT_SYS_TICK:
            return CYHAL_CLOCK_FEATURE_SOURCE;

        default:
            CY_ASSERT(false); //Unhandled clock
            return CYHAL_CLOCK_FEATURE_NONE;
    }
}

bool cyhal_clock_is_enabled(const cyhal_clock_t *clock)
{
    CY_ASSERT(NULL != clock);
    CY_ASSERT(_cyhal_utils_is_new_clock_format(clock));

    switch (clock->block)
    {
#if SRSS_ECO_PRESENT
        case CYHAL_CLOCK_BLOCK_ECO:
            return 0u != (SRSS_CLK_ECO_CONFIG & SRSS_CLK_ECO_CONFIG_ECO_EN_Msk);
#endif
        case CYHAL_CLOCK_BLOCK_EXT:
            return (Cy_SysClk_ExtClkGetFrequency() > 0);
#if SRSS_ALTHF_PRESENT
        case CYHAL_CLOCK_BLOCK_ALTHF:
            return (Cy_SysClk_AltHfGetFrequency() > 0);
#endif
#if SRSS_ALTLF_PRESENT
        case CYHAL_CLOCK_BLOCK_ALTLF:
            return Cy_SysClk_AltLfIsEnabled();
#endif
        case CYHAL_CLOCK_BLOCK_ILO:
            return Cy_SysClk_IloIsEnabled();
#if SRSS_PILO_PRESENT
        case CYHAL_CLOCK_BLOCK_PILO:
            return Cy_SysClk_PiloIsEnabled();
#endif
#if SRSS_BACKUP_PRESENT
        case CYHAL_CLOCK_BLOCK_WCO:
            return 0u != (BACKUP_CTL & BACKUP_CTL_WCO_EN_Msk);
#endif
#if SRSS_MFO_PRESENT
        case CYHAL_CLOCK_BLOCK_MFO:
            return Cy_SysClk_MfoIsEnabled();
#endif
        case CYHAL_CLOCK_BLOCK_FLL:
            return Cy_SysClk_FllIsEnabled();
#if (SRSS_NUM_PLL > 0)
        case CYHAL_CLOCK_BLOCK_PLL:
            return Cy_SysClk_PllIsEnabled(clock->channel + 1);
#endif
#if SRSS_MFO_PRESENT
        case CYHAL_CLOCK_BLOCK_MF:
            return Cy_SysClk_ClkMfIsEnabled();
#endif
        case CYHAL_CLOCK_BLOCK_HF:
            return Cy_SysClk_ClkHfIsEnabled(clock->channel);
        case CYHAL_CLOCK_BLOCK_PUMP:
            return Cy_SysClk_ClkPumpIsEnabled();
        case CYHAL_CLOCK_BLOCK_TIMER:
            return Cy_SysClk_ClkTimerIsEnabled();
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_8BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16_5BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_24_5BIT:
            return Cy_SysClk_PeriphGetDividerEnabled((cy_en_divider_types_t)clock->block, clock->channel);
        case CYHAL_CLOCK_BLOCK_IMO:
        case CYHAL_CLOCK_BLOCK_PATHMUX:
        case CYHAL_CLOCK_BLOCK_LF:
        case CYHAL_CLOCK_BLOCK_BAK:
        case CYHAL_CLOCK_BLOCK_ALT_SYS_TICK:
        case CYHAL_CLOCK_BLOCK_FAST:
        case CYHAL_CLOCK_BLOCK_PERI:
        case CYHAL_CLOCK_BLOCK_SLOW:
            return true;
        default:
            CY_ASSERT(false); //Unhandled clock
            return false;
    }
}

cy_rslt_t cyhal_clock_set_enabled(cyhal_clock_t *clock, bool enabled, bool wait_for_lock)
{
    CY_ASSERT(NULL != clock);
    CY_ASSERT(_cyhal_utils_is_new_clock_format(clock));

    /* Timeout values are from the device datasheet. */

    switch (clock->block)
    {
#if SRSS_ECO_PRESENT
        case CYHAL_CLOCK_BLOCK_ECO:
            if (enabled)
            {
                if (0u != (SRSS_CLK_ECO_CONFIG & SRSS_CLK_ECO_CONFIG_ECO_EN_Msk))
                {
                    // Already enabled
                    if (wait_for_lock)
                    {
                        for (int t = 0; t < 3 && Cy_SysClk_EcoGetStatus() != CY_SYSCLK_ECOSTAT_STABLE; ++t)
                        {
                            Cy_SysLib_DelayUs(1000UL);
                        }
                        return Cy_SysClk_EcoGetStatus() == CY_SYSCLK_ECOSTAT_STABLE
                            ? CY_RSLT_SUCCESS
                            : CY_SYSCLK_TIMEOUT;
                    }
                    return CY_RSLT_SUCCESS;
                }
                else
                {
                    return Cy_SysClk_EcoEnable(wait_for_lock ? 3000UL : 0UL);
                }
            }
            else
            {
                Cy_SysClk_EcoDisable();
                return CY_RSLT_SUCCESS;
            }
#endif
        case CYHAL_CLOCK_BLOCK_ILO:
            if (enabled)
                Cy_SysClk_IloEnable();
            else
                Cy_SysClk_IloDisable();
            return CY_RSLT_SUCCESS;
#if SRSS_PILO_PRESENT
        case CYHAL_CLOCK_BLOCK_PILO:
            if (enabled)
                Cy_SysClk_PiloEnable();
            else
                Cy_SysClk_PiloDisable();
            return CY_RSLT_SUCCESS;
#endif
#if SRSS_BACKUP_PRESENT
        case CYHAL_CLOCK_BLOCK_WCO:
            if (enabled)
            {
                cy_rslt_t rslt = Cy_SysClk_WcoEnable(wait_for_lock ? 1000000UL : 0UL);
                // Ignore CY_SYSCLK_TIMEOUT unless wait_for_lock is true
                return wait_for_lock ? rslt : CY_RSLT_SUCCESS;
            }
            else
            {
                Cy_SysClk_WcoDisable();
                return CY_RSLT_SUCCESS;
            }
#endif
#if SRSS_MFO_PRESENT
        case CYHAL_CLOCK_BLOCK_MFO:
            if (enabled)
                Cy_SysClk_MfoEnable(true);
            else
                Cy_SysClk_MfoDisable();
            return CY_RSLT_SUCCESS;
#endif
        case CYHAL_CLOCK_BLOCK_FLL:
            return _cyhal_clock_change_fll_enablement(enabled, wait_for_lock);

#if (SRSS_NUM_PLL > 0)
        case CYHAL_CLOCK_BLOCK_PLL:
            return _cyhal_clock_change_pll_enablement(clock->channel + 1, enabled, wait_for_lock);
#endif
#if SRSS_MFO_PRESENT
        case CYHAL_CLOCK_BLOCK_MF:
            if (enabled)
                Cy_SysClk_ClkMfEnable();
            else
                Cy_SysClk_ClkMfDisable();
            return CY_RSLT_SUCCESS;
#endif
        case CYHAL_CLOCK_BLOCK_HF:
            return (enabled)
                ? Cy_SysClk_ClkHfEnable(clock->channel)
                : Cy_SysClk_ClkHfDisable(clock->channel);
        case CYHAL_CLOCK_BLOCK_PUMP:
            if (enabled)
                Cy_SysClk_ClkPumpEnable();
            else
                Cy_SysClk_ClkPumpDisable();
            return CY_RSLT_SUCCESS;
        case CYHAL_CLOCK_BLOCK_TIMER:
            if (enabled)
                Cy_SysClk_ClkTimerEnable();
            else
                Cy_SysClk_ClkTimerDisable();
            return CY_RSLT_SUCCESS;
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_8BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16_5BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_24_5BIT:
            return (enabled)
                ? Cy_SysClk_PeriphEnableDivider((cy_en_divider_types_t)clock->block, clock->channel)
                : Cy_SysClk_PeriphDisableDivider((cy_en_divider_types_t)clock->block, clock->channel);
        default:
            CY_ASSERT(false); //Unhandled clock
            return CYHAL_CLOCK_RSLT_ERR_RESOURCE;
    }
}

uint32_t cyhal_clock_get_frequency(const cyhal_clock_t *clock)
{
    CY_ASSERT(NULL != clock);
    CY_ASSERT(_cyhal_utils_is_new_clock_format(clock));

    if (!cyhal_clock_is_enabled(clock))
    {
        return 0;
    }

    switch (clock->block)
    {
        case CYHAL_CLOCK_BLOCK_IMO:
            return CY_SYSCLK_IMO_FREQ;
#if SRSS_ECO_PRESENT
        case CYHAL_CLOCK_BLOCK_ECO:
            return Cy_SysClk_EcoGetFrequency();
#endif
        case CYHAL_CLOCK_BLOCK_EXT:
            return Cy_SysClk_ExtClkGetFrequency();
#if SRSS_ALTHF_PRESENT
        case CYHAL_CLOCK_BLOCK_ALTHF:
            return Cy_SysClk_AltHfGetFrequency();
#endif
#if SRSS_ALTLF_PRESENT
        case CYHAL_CLOCK_BLOCK_ALTLF:
            return Cy_SysClk_AltLfGetFrequency();
#endif
#if SRSS_MFO_PRESENT
        case CYHAL_CLOCK_BLOCK_MFO:
            return CY_SYSCLK_MFO_FREQ;
#endif
        case CYHAL_CLOCK_BLOCK_ILO:
            return CY_SYSCLK_ILO_FREQ;
#if SRSS_PILO_PRESENT
        case CYHAL_CLOCK_BLOCK_PILO:
            return CY_SYSCLK_PILO_FREQ;
#endif
#if SRSS_BACKUP_PRESENT
        case CYHAL_CLOCK_BLOCK_WCO:
            return CY_SYSCLK_WCO_FREQ;
#endif
        case CYHAL_CLOCK_BLOCK_PATHMUX:
            return Cy_SysClk_ClkPathMuxGetFrequency(clock->channel);
        case CYHAL_CLOCK_BLOCK_FLL:
            return Cy_SysClk_FllIsEnabled()
                ? Cy_SysClk_ClkPathGetFrequency(0)
                : 0;
#if (SRSS_NUM_PLL > 0)
        case CYHAL_CLOCK_BLOCK_PLL:
            return Cy_SysClk_PllIsEnabled(clock->channel + 1)
                ? Cy_SysClk_ClkPathGetFrequency(clock->channel + 1)
                : 0;
#endif
#if SRSS_MFO_PRESENT
        case CYHAL_CLOCK_BLOCK_MF:
            return Cy_SysClk_ClkMfGetFrequency();
#endif
        case CYHAL_CLOCK_BLOCK_HF:
            return Cy_SysClk_ClkHfGetFrequency(clock->channel);
        case CYHAL_CLOCK_BLOCK_PUMP:
            return Cy_SysClk_ClkPumpGetFrequency();
        case CYHAL_CLOCK_BLOCK_TIMER:
            return Cy_SysClk_ClkTimerGetFrequency();
        case CYHAL_CLOCK_BLOCK_LF:
            return _cyhal_clock_get_lf_frequency();
        case CYHAL_CLOCK_BLOCK_BAK:
        {
            cy_en_clkbak_in_sources_t src = Cy_SysClk_ClkBakGetSource();
#if SRSS_BACKUP_PRESENT
            if (src == CY_SYSCLK_BAK_IN_WCO)
                return CY_SYSCLK_WCO_FREQ;
            else
#endif
            return _cyhal_clock_get_lf_frequency();
        }
        case CYHAL_CLOCK_BLOCK_FAST:
            return Cy_SysClk_ClkFastGetFrequency();
        case CYHAL_CLOCK_BLOCK_PERI:
            return Cy_SysClk_ClkPeriGetFrequency();
        case CYHAL_CLOCK_BLOCK_SLOW:
            return Cy_SysClk_ClkSlowGetFrequency();
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_8BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16_5BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_24_5BIT:
            return Cy_SysClk_PeriphGetFrequency((cy_en_divider_types_t)clock->block, clock->channel);
        case CYHAL_CLOCK_BLOCK_ALT_SYS_TICK:
        default:
            CY_ASSERT(false); //Unhandled clock
            return 0;
    }
}

cy_rslt_t cyhal_clock_set_frequency(cyhal_clock_t *clock, uint32_t hz, const cyhal_clock_tolerance_t *tolerance)
{
    CY_ASSERT(NULL != clock);
    CY_ASSERT(_cyhal_utils_is_new_clock_format(clock));

    switch (clock->block)
    {
        case CYHAL_CLOCK_BLOCK_EXT:
            Cy_SysClk_ExtClkSetFrequency(hz);
            return CY_RSLT_SUCCESS;
        case CYHAL_CLOCK_BLOCK_FLL:
            return _cyhal_clock_set_fll_freq(hz);
#if (SRSS_NUM_PLL > 0)
        case CYHAL_CLOCK_BLOCK_PLL:
            return _cyhal_clock_set_pll_freq(clock->channel + 1, hz);
#endif
#if SRSS_MFO_PRESENT
        case CYHAL_CLOCK_BLOCK_MF:
        {
            uint32_t div;
            cy_rslt_t rslt = _cyhal_clock_compute_div(CY_SYSCLK_MFO_FREQ, hz, 8, tolerance, &div);

            if (CY_RSLT_SUCCESS == rslt)
                Cy_SysClk_ClkMfSetDivider(div);

            return rslt;
        }
#endif
        case CYHAL_CLOCK_BLOCK_FAST:
        case CYHAL_CLOCK_BLOCK_PERI:
        {
            uint32_t div;
            uint32_t input_hz = Cy_SysClk_ClkHfGetFrequency(0);
            cy_rslt_t rslt = _cyhal_clock_compute_div(input_hz, hz, 8, tolerance, &div);

            if (CY_RSLT_SUCCESS == rslt)
            {
                if (CYHAL_CLOCK_BLOCK_PERI == clock->block)
                    Cy_SysClk_ClkPeriSetDivider((uint8_t)(div - 1));
                else
                    Cy_SysClk_ClkFastSetDivider((uint8_t)(div - 1));

                SystemCoreClockUpdate();
            }

            return rslt;
        }
        case CYHAL_CLOCK_BLOCK_SLOW:
        {
            uint32_t div;
            uint32_t input_hz = Cy_SysClk_ClkPeriGetFrequency();
            cy_rslt_t rslt = _cyhal_clock_compute_div(input_hz, hz, 8, tolerance, &div);

            if (CY_RSLT_SUCCESS == rslt)
            {
                Cy_SysClk_ClkSlowSetDivider((uint8_t)(div - 1));
                SystemCoreClockUpdate();
            }

            return rslt;
        }
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_8BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT:
        {
            uint32_t div;
            uint32_t input_hz = Cy_SysClk_ClkPeriGetFrequency();
            uint32_t bits = (clock->block == CYHAL_CLOCK_BLOCK_PERIPHERAL_8BIT) ? 8 : 16;
            cy_rslt_t rslt = _cyhal_clock_compute_div(input_hz, hz, bits, tolerance, &div);
            return (CY_RSLT_SUCCESS == rslt)
                ? Cy_SysClk_PeriphSetDivider((cy_en_divider_types_t)clock->block, clock->channel, (div - 1))
                : rslt;
        }
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16_5BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_24_5BIT:
        {
            uint32_t div;
            uint32_t input_hz = Cy_SysClk_ClkPeriGetFrequency();
            // Multiply input by 32 so we can treat the 5 fractional bits as though they are extentions of the integer divider
            // Leave the the desired frequency alone, so we can just strip out the integer & fractional bits at the end.
            uint32_t bits = (clock->block == CYHAL_CLOCK_BLOCK_PERIPHERAL_16_5BIT) ? 21 : 29; // Integer bits + 5
            cy_rslt_t rslt = _cyhal_clock_compute_div(input_hz << 5, hz, bits, tolerance, &div);
            if (CY_RSLT_SUCCESS == rslt)
            {
                uint32_t div_int = (div >> 5) - 1;
                uint32_t div_frac = div & 0x1F;
                return Cy_SysClk_PeriphSetFracDivider((cy_en_divider_types_t)clock->block, clock->channel, div_int, div_frac);
            }
            else
                return rslt;
        }
        default:
            CY_ASSERT(false); //Unhandled clock
            return CYHAL_CLOCK_RSLT_ERR_NOT_SUPPORTED;
    }
}

cy_rslt_t cyhal_clock_set_divider(cyhal_clock_t *clock, uint32_t divider)
{
    CY_ASSERT(NULL != clock);
    CY_ASSERT(_cyhal_utils_is_new_clock_format(clock));

    switch (clock->block)
    {
#if SRSS_MFO_PRESENT
        case CYHAL_CLOCK_BLOCK_MF:
#endif
        case CYHAL_CLOCK_BLOCK_FAST:
        case CYHAL_CLOCK_BLOCK_PERI:
        case CYHAL_CLOCK_BLOCK_SLOW:
        case CYHAL_CLOCK_BLOCK_TIMER:
            if (divider <= 256)
            {
                uint32_t divVal = divider - 1;
                switch ((uint8_t)clock->block)
                {
#if SRSS_MFO_PRESENT
                    case CYHAL_CLOCK_BLOCK_MF:
                        Cy_SysClk_ClkMfSetDivider(divVal);
                        return CY_RSLT_SUCCESS;
#endif
                    case CYHAL_CLOCK_BLOCK_FAST:
                        Cy_SysClk_ClkFastSetDivider((uint8_t)divVal);
                        SystemCoreClockUpdate();
                        return CY_RSLT_SUCCESS;
                    case CYHAL_CLOCK_BLOCK_SLOW:
                        Cy_SysClk_ClkSlowSetDivider((uint8_t)divVal);
                        SystemCoreClockUpdate();
                        return CY_RSLT_SUCCESS;
                    case CYHAL_CLOCK_BLOCK_PERI:
                        Cy_SysClk_ClkPeriSetDivider((uint8_t)divVal);
                        SystemCoreClockUpdate();
                        return CY_RSLT_SUCCESS;
                    case CYHAL_CLOCK_BLOCK_TIMER:
                        Cy_SysClk_ClkTimerSetDivider((uint8_t)divVal);
                        return CY_RSLT_SUCCESS;
                }
            }
            return CYHAL_CLOCK_RSLT_ERR_FREQ;

        case CYHAL_CLOCK_BLOCK_HF:
            return _cyhal_clock_set_hfclk_div(clock->channel, divider);
        case CYHAL_CLOCK_BLOCK_PUMP:
        {
            cy_en_clkpump_divide_t divVal;
            switch (divider)
            {
                case 1:
                    divVal = CY_SYSCLK_PUMP_NO_DIV;
                    break;
                case 2:
                    divVal = CY_SYSCLK_PUMP_DIV_2;
                    break;
                case 4:
                    divVal = CY_SYSCLK_PUMP_DIV_4;
                    break;
                case 8:
                    divVal = CY_SYSCLK_PUMP_DIV_8;
                    break;
                case 16:
                    divVal = CY_SYSCLK_PUMP_DIV_16;
                    break;
                default:
                    return CYHAL_CLOCK_RSLT_ERR_FREQ;
            }
            Cy_SysClk_ClkPumpSetDivider(divVal);
            return CY_RSLT_SUCCESS;
        }
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_8BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT:
            return Cy_SysClk_PeriphSetDivider((cy_en_divider_types_t)clock->block, clock->channel, divider - 1);
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16_5BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_24_5BIT:
            return Cy_SysClk_PeriphSetFracDivider((cy_en_divider_types_t)clock->block, clock->channel, divider - 1, 0);
        default:
            CY_ASSERT(false); //Unhandled clock
            return CYHAL_CLOCK_RSLT_ERR_NOT_SUPPORTED;
    }
}

cy_rslt_t cyhal_clock_get_sources(const cyhal_clock_t *clock, const cyhal_resource_inst_t **sources[], uint32_t *count)
{
    static const cyhal_resource_inst_t *_CYHAL_CLOCK_SOURCE_PATHMUX[] =
    {
        &CYHAL_CLOCK_IMO,
    #if SRSS_ECO_PRESENT
        &CYHAL_CLOCK_ECO,
    #endif
        &CYHAL_CLOCK_EXT,
    #if SRSS_ALTHF_PRESENT
        &CYHAL_CLOCK_ALTHF,
    #endif
        &CYHAL_CLOCK_ILO,
    #if SRSS_PILO_PRESENT
        &CYHAL_CLOCK_PILO,
    #endif
    #if SRSS_BACKUP_PRESENT
        &CYHAL_CLOCK_WCO,
    #endif
    #if SRSS_ALTLF_PRESENT
        &CYHAL_CLOCK_ALTLF,
    #endif
    };
    #if SRSS_MFO_PRESENT
    static const cyhal_resource_inst_t *_CYHAL_CLOCK_SOURCE_MFO[] =
    {
        &CYHAL_CLOCK_IMO,
    };
    static const cyhal_resource_inst_t *_CYHAL_CLOCK_SOURCE_MF[] =
    {
        &CYHAL_CLOCK_MFO,
    };
    #endif
    static const cyhal_resource_inst_t *_CYHAL_CLOCK_SOURCE_SLOW[] =
    {
        &CYHAL_CLOCK_PERI,
    };
    static const cyhal_resource_inst_t *_CYHAL_CLOCK_SOURCE_FAST[] =
    {
        &CYHAL_CLOCK_HF[0],
    };
    static const cyhal_resource_inst_t *_CYHAL_CLOCK_SOURCE_TIMER[] =
    {
        &CYHAL_CLOCK_IMO,
        &CYHAL_CLOCK_HF[0],
    };
    static const cyhal_resource_inst_t *_CYHAL_CLOCK_SOURCE_ALT_SYS_TICK[] =
    {
        &CYHAL_CLOCK_IMO,
    #if SRSS_ECO_PRESENT
        &CYHAL_CLOCK_ECO,
    #endif
        &CYHAL_CLOCK_TIMER,
        &CYHAL_CLOCK_LF,
        &CYHAL_CLOCK_FAST,
        &CYHAL_CLOCK_SLOW,
    };
    static const cyhal_resource_inst_t *_CYHAL_CLOCK_SOURCE_BAK[] =
    {
        &CYHAL_CLOCK_LF,
    #if SRSS_BACKUP_PRESENT
        &CYHAL_CLOCK_WCO,
    #endif
    };
    static const cyhal_resource_inst_t *_CYHAL_CLOCK_SOURCE_LF[] =
    {
        &CYHAL_CLOCK_ILO,
    #if SRSS_PILO_PRESENT
        &CYHAL_CLOCK_PILO,
    #endif
    #if SRSS_BACKUP_PRESENT
        &CYHAL_CLOCK_WCO,
    #endif
    #if SRSS_ALTLF_PRESENT
        &CYHAL_CLOCK_ALTLF,
    #endif
    };
    static const cyhal_resource_inst_t *_CYHAL_CLOCK_SOURCE_HF[] =
    {
        &CYHAL_CLOCK_FLL,
    #if (SRSS_NUM_PLL > 0)
        &CYHAL_CLOCK_PLL[0],
    #endif
    #if (SRSS_NUM_PLL > 1)
        &CYHAL_CLOCK_PLL[1],
    #endif
    #if (SRSS_NUM_PLL > 2)
        &CYHAL_CLOCK_PLL[2],
    #endif
    #if (SRSS_NUM_PLL > 3)
        &CYHAL_CLOCK_PLL[3],
    #endif
    #if (SRSS_NUM_PLL > 4)
        &CYHAL_CLOCK_PLL[4],
    #endif
    #if (SRSS_NUM_PLL > 5)
        &CYHAL_CLOCK_PLL[5],
    #endif
    #if (SRSS_NUM_PLL > 6)
        &CYHAL_CLOCK_PLL[6],
    #endif
    #if (SRSS_NUM_PLL > 7)
        &CYHAL_CLOCK_PLL[7],
    #endif
    #if (SRSS_NUM_PLL > 8)
        &CYHAL_CLOCK_PLL[8],
    #endif
    #if (SRSS_NUM_PLL > 9)
        &CYHAL_CLOCK_PLL[9],
    #endif
    #if (SRSS_NUM_PLL > 10)
        &CYHAL_CLOCK_PLL[10],
    #endif
    #if (SRSS_NUM_PLL > 11)
        &CYHAL_CLOCK_PLL[11],
    #endif
    #if (SRSS_NUM_PLL > 12)
        &CYHAL_CLOCK_PLL[12],
    #endif
    #if (SRSS_NUM_PLL > 13)
        &CYHAL_CLOCK_PLL[13],
    #endif
    #if (SRSS_NUM_PLL > 14)
        &CYHAL_CLOCK_PLL[14],
    #endif
        &CYHAL_CLOCK_PATHMUX[0],
    #if (SRSS_NUM_CLKPATH > 1)
        &CYHAL_CLOCK_PATHMUX[1],
    #endif
    #if (SRSS_NUM_CLKPATH > 2)
        &CYHAL_CLOCK_PATHMUX[2],
    #endif
    #if (SRSS_NUM_CLKPATH > 3)
        &CYHAL_CLOCK_PATHMUX[3],
    #endif
    #if (SRSS_NUM_CLKPATH > 4)
        &CYHAL_CLOCK_PATHMUX[4],
    #endif
    #if (SRSS_NUM_CLKPATH > 5)
        &CYHAL_CLOCK_PATHMUX[5],
    #endif
    #if (SRSS_NUM_CLKPATH > 6)
        &CYHAL_CLOCK_PATHMUX[6],
    #endif
    #if (SRSS_NUM_CLKPATH > 7)
        &CYHAL_CLOCK_PATHMUX[7],
    #endif
    #if (SRSS_NUM_CLKPATH > 8)
        &CYHAL_CLOCK_PATHMUX[8],
    #endif
    #if (SRSS_NUM_CLKPATH > 9)
        &CYHAL_CLOCK_PATHMUX[9],
    #endif
    #if (SRSS_NUM_CLKPATH > 10)
        &CYHAL_CLOCK_PATHMUX[10],
    #endif
    #if (SRSS_NUM_CLKPATH > 11)
        &CYHAL_CLOCK_PATHMUX[11],
    #endif
    #if (SRSS_NUM_CLKPATH > 12)
        &CYHAL_CLOCK_PATHMUX[12],
    #endif
    #if (SRSS_NUM_CLKPATH > 13)
        &CYHAL_CLOCK_PATHMUX[13],
    #endif
    #if (SRSS_NUM_CLKPATH > 14)
        &CYHAL_CLOCK_PATHMUX[14],
    #endif
    #if (SRSS_NUM_CLKPATH > 15)
        &CYHAL_CLOCK_PATHMUX[15],
    #endif
    };

    CY_ASSERT(NULL != clock);
    CY_ASSERT(_cyhal_utils_is_new_clock_format(clock));

    switch (clock->block)
    {
        case CYHAL_CLOCK_BLOCK_IMO:
#if SRSS_ECO_PRESENT
        case CYHAL_CLOCK_BLOCK_ECO:
#endif
        case CYHAL_CLOCK_BLOCK_EXT:
#if SRSS_ALTHF_PRESENT
        case CYHAL_CLOCK_BLOCK_ALTHF:
#endif
#if SRSS_ALTLF_PRESENT
        case CYHAL_CLOCK_BLOCK_ALTLF:
#endif
        case CYHAL_CLOCK_BLOCK_ILO:
#if SRSS_PILO_PRESENT
        case CYHAL_CLOCK_BLOCK_PILO:
#endif
#if SRSS_BACKUP_PRESENT
        case CYHAL_CLOCK_BLOCK_WCO:
            *count = 0;
            break;
#endif
#if SRSS_MFO_PRESENT
        case CYHAL_CLOCK_BLOCK_MFO:
            *sources = _CYHAL_CLOCK_SOURCE_MFO;
            *count = sizeof(_CYHAL_CLOCK_SOURCE_MFO) / sizeof(_CYHAL_CLOCK_SOURCE_MFO[0]);
            break;
        case CYHAL_CLOCK_BLOCK_MF:
            *sources = _CYHAL_CLOCK_SOURCE_MF;
            *count = sizeof(_CYHAL_CLOCK_SOURCE_MF) / sizeof(_CYHAL_CLOCK_SOURCE_MF[0]);
            break;
#endif
        case CYHAL_CLOCK_BLOCK_PATHMUX:
            *sources = _CYHAL_CLOCK_SOURCE_PATHMUX;
            *count = sizeof(_CYHAL_CLOCK_SOURCE_PATHMUX) / sizeof(_CYHAL_CLOCK_SOURCE_PATHMUX[0]);
            break;
        case CYHAL_CLOCK_BLOCK_FLL:
            *sources = &(_CYHAL_CLOCK_SOURCE_HF[1 + SRSS_NUM_PLL]); /* PATHMUX[0] entry is after the FLL/PLLs */
            *count = 1;
            break;
#if (SRSS_NUM_PLL > 0)
        case CYHAL_CLOCK_BLOCK_PLL:
            *sources = &(_CYHAL_CLOCK_SOURCE_HF[2 + SRSS_NUM_PLL + clock->channel]); /* PATHMUX[n] entry is after the FLL/PLLs + 1 for FLL path */
            *count = 1;
            break;
#endif
        case CYHAL_CLOCK_BLOCK_LF:
            *sources = _CYHAL_CLOCK_SOURCE_LF;
            *count = sizeof(_CYHAL_CLOCK_SOURCE_LF) / sizeof(_CYHAL_CLOCK_SOURCE_LF[0]);
            break;
        case CYHAL_CLOCK_BLOCK_HF:
        case CYHAL_CLOCK_BLOCK_PUMP:
            *sources = _CYHAL_CLOCK_SOURCE_HF;
            *count = sizeof(_CYHAL_CLOCK_SOURCE_HF) / sizeof(_CYHAL_CLOCK_SOURCE_HF[0]);
            break;
        case CYHAL_CLOCK_BLOCK_BAK:
            *sources = _CYHAL_CLOCK_SOURCE_BAK;
            *count = sizeof(_CYHAL_CLOCK_SOURCE_BAK) / sizeof(_CYHAL_CLOCK_SOURCE_BAK[0]);
            break;
        case CYHAL_CLOCK_BLOCK_FAST:
        case CYHAL_CLOCK_BLOCK_PERI:
            *sources = _CYHAL_CLOCK_SOURCE_FAST;
            *count = sizeof(_CYHAL_CLOCK_SOURCE_FAST) / sizeof(_CYHAL_CLOCK_SOURCE_FAST[0]);
            break;
        case CYHAL_CLOCK_BLOCK_TIMER:
            *sources = _CYHAL_CLOCK_SOURCE_TIMER;
            *count = sizeof(_CYHAL_CLOCK_SOURCE_TIMER) / sizeof(_CYHAL_CLOCK_SOURCE_TIMER[0]);
            break;
        case CYHAL_CLOCK_BLOCK_SLOW:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_8BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_16_5BIT:
        case CYHAL_CLOCK_BLOCK_PERIPHERAL_24_5BIT:
            *sources = _CYHAL_CLOCK_SOURCE_SLOW;
            *count = sizeof(_CYHAL_CLOCK_SOURCE_SLOW) / sizeof(_CYHAL_CLOCK_SOURCE_SLOW[0]);
            break;
        case CYHAL_CLOCK_BLOCK_ALT_SYS_TICK:
            *sources = _CYHAL_CLOCK_SOURCE_ALT_SYS_TICK;
            *count = sizeof(_CYHAL_CLOCK_SOURCE_ALT_SYS_TICK) / sizeof(_CYHAL_CLOCK_SOURCE_ALT_SYS_TICK[0]);
            break;
        default:
            CY_ASSERT(false); //Unhandled clock
            *count = 0;
            return CYHAL_CLOCK_RSLT_ERR_RESOURCE;
    }
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_clock_set_source(cyhal_clock_t *clock, const cyhal_clock_t *source)
{
    CY_ASSERT(NULL != clock && NULL != source);
    CY_ASSERT(_cyhal_utils_is_new_clock_format(clock));

    switch (clock->block)
    {
        case CYHAL_CLOCK_BLOCK_HF:
            return _cyhal_clock_set_hfclk_source(clock->channel, source);
        case CYHAL_CLOCK_BLOCK_PUMP:
            if (source->block == CYHAL_CLOCK_BLOCK_PATHMUX || source->block == CYHAL_CLOCK_BLOCK_FLL)
            {
                Cy_SysClk_ClkPumpSetSource((cy_en_clkpump_in_sources_t)source->channel);
                return CY_RSLT_SUCCESS;
            }
            else if (source->block == CYHAL_CLOCK_BLOCK_PLL)
            {
                Cy_SysClk_ClkPumpSetSource((cy_en_clkpump_in_sources_t)(source->channel + 1));
                return CY_RSLT_SUCCESS;
            }
            else
                return CYHAL_CLOCK_RSLT_ERR_SOURCE;
        case CYHAL_CLOCK_BLOCK_TIMER:
            if (source->block == CYHAL_CLOCK_BLOCK_IMO)
            {
                Cy_SysClk_ClkTimerSetSource(CY_SYSCLK_CLKTIMER_IN_IMO);
                return CY_RSLT_SUCCESS;
            }
            else if (source->block == CYHAL_CLOCK_BLOCK_HF && source->channel == 0)
            {
                Cy_SysClk_ClkTimerSetSource(CY_SYSCLK_CLKTIMER_IN_HF0_NODIV);
                return CY_RSLT_SUCCESS;
            }
            return CYHAL_CLOCK_RSLT_ERR_SOURCE;
        case CYHAL_CLOCK_BLOCK_PATHMUX:
            return _cyhal_clock_set_pathmux_source(clock->channel, source->block);
        case CYHAL_CLOCK_BLOCK_LF:
            switch (source->block)
            {
                case CYHAL_CLOCK_BLOCK_ILO:
                    Cy_SysClk_ClkLfSetSource(CY_SYSCLK_CLKLF_IN_ILO);
                    return CY_RSLT_SUCCESS;
#if SRSS_BACKUP_PRESENT
                case CYHAL_CLOCK_BLOCK_WCO:
                    Cy_SysClk_ClkLfSetSource(CY_SYSCLK_CLKLF_IN_WCO);
                    return CY_RSLT_SUCCESS;
#endif
#if SRSS_ALTLF_PRESENT
                case CYHAL_CLOCK_BLOCK_ALTLF:
                    Cy_SysClk_ClkLfSetSource(CY_SYSCLK_CLKLF_IN_ALTLF);
                    return CY_RSLT_SUCCESS;
#endif
#if SRSS_PILO_PRESENT
                case CYHAL_CLOCK_BLOCK_PILO:
                    Cy_SysClk_ClkLfSetSource(CY_SYSCLK_CLKLF_IN_PILO);
                    return CY_RSLT_SUCCESS;
#endif
                default:
                    CY_ASSERT(false); //Unhandled clock
                    return CYHAL_CLOCK_RSLT_ERR_SOURCE;
            }
        case CYHAL_CLOCK_BLOCK_BAK:
            if (source->block == CYHAL_CLOCK_BLOCK_LF)
            {
                Cy_SysClk_ClkBakSetSource(CY_SYSCLK_BAK_IN_CLKLF);
                return CY_RSLT_SUCCESS;
            }
#if SRSS_BACKUP_PRESENT
            else if (source->block == CYHAL_CLOCK_BLOCK_WCO)
            {
                Cy_SysClk_ClkBakSetSource(CY_SYSCLK_BAK_IN_WCO);
                return CY_RSLT_SUCCESS;
            }
#endif
            return CYHAL_CLOCK_RSLT_ERR_SOURCE;
        case CYHAL_CLOCK_BLOCK_ALT_SYS_TICK:
            switch (source->block)
            {
                case CYHAL_CLOCK_BLOCK_LF:
                    Cy_SysTick_SetClockSource(CY_SYSTICK_CLOCK_SOURCE_CLK_LF);
                    return CY_RSLT_SUCCESS;
                case CYHAL_CLOCK_BLOCK_IMO:
                    Cy_SysTick_SetClockSource(CY_SYSTICK_CLOCK_SOURCE_CLK_IMO);
                    return CY_RSLT_SUCCESS;
#if SRSS_ECO_PRESENT
                case CYHAL_CLOCK_BLOCK_ECO:
                    Cy_SysTick_SetClockSource(CY_SYSTICK_CLOCK_SOURCE_CLK_ECO);
                    return CY_RSLT_SUCCESS;
#endif
                case CYHAL_CLOCK_BLOCK_TIMER:
                    Cy_SysTick_SetClockSource(CY_SYSTICK_CLOCK_SOURCE_CLK_TIMER);
                    return CY_RSLT_SUCCESS;
                case CYHAL_CLOCK_BLOCK_FAST:
                case CYHAL_CLOCK_BLOCK_SLOW:
                    Cy_SysTick_SetClockSource(CY_SYSTICK_CLOCK_SOURCE_CLK_CPU);
                    return CY_RSLT_SUCCESS;
                default:
                    CY_ASSERT(false); //Unhandled clock
                    return CYHAL_CLOCK_RSLT_ERR_SOURCE;
            }
        default:
            CY_ASSERT(false); //Unhandled clock
            return CYHAL_CLOCK_RSLT_ERR_NOT_SUPPORTED;
    }
}

void cyhal_clock_free(cyhal_clock_t *clock)
{
    CY_ASSERT(NULL != clock);
    CY_ASSERT(_cyhal_utils_is_new_clock_format(clock));

    cyhal_resource_inst_t rsc = { CYHAL_RSC_CLOCK, clock->block, clock->channel };
    cyhal_hwmgr_free(&rsc);
    clock->reserved = false;
}

#if defined(__cplusplus)
}
#endif
