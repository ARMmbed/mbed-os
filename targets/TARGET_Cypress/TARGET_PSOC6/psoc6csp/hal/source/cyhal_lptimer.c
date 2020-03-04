/***************************************************************************//**
* \file cyhal_lptimer.c
*
* \brief
* Provides a high level interface for interacting with the Cypress Low-Power Timer.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
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

#include "cmsis_compiler.h"
#include "cy_mcwdt.h"
#include "cy_syslib.h"
#include "cy_sysint.h"
#include "cyhal_lptimer.h"
#include "cyhal_hwmgr.h"
#include "cyhal_system_impl.h"
#include "cyhal_utils.h"

#ifdef CY_IP_MXS40SRSS_MCWDT_INSTANCES

#if defined(__cplusplus)
extern "C" {
#endif

static MCWDT_STRUCT_Type * const CYHAL_LPTIMER_BASE_ADDRESSES[] = {
#if CY_IP_MXS40SRSS_MCWDT_INSTANCES >= 1
    MCWDT_STRUCT0,
#endif
#if CY_IP_MXS40SRSS_MCWDT_INSTANCES >= 2
    MCWDT_STRUCT1,
#endif
#if CY_IP_MXS40SRSS_MCWDT_INSTANCES >= 3
#error "CY_IP_MXS40SRSS_MCWDT_INSTANCES > 2 not supported"
#endif
};

#define CY_MCWDT_MAX_DELAY_TICKS    (0xfff0ffffUL) /* ~36hours, Not set to 0xffffffff to avoid C0 and C1 both overflowing */
#define CY_MCWDT_LPTIMER_CTRL       (CY_MCWDT_CTR0 | CY_MCWDT_CTR1 | CY_MCWDT_CTR2)

#define CY_MCWDT_MIN_DELAY          3 /* minimum amount of lfclk cycles of that LPTIMER can delay for. */

#define CY_DEFAULT_MCWDT_PRIORITY   3

static const uint16_t CY_MCWDT_RESET_TIME_US = 62;
static const uint16_t CY_MCWDT_SETMATCH_NOWAIT_TIME_US = 0;

static cyhal_lptimer_t *cyhal_lptimer_config_structs[CY_IP_MXS40SRSS_MCWDT_INSTANCES];

static void cyhal_lptimer_irq_handler(void)
{
    uint32_t instance = CYHAL_GET_CURRENT_IRQN() - (uint32_t) srss_interrupt_mcwdt_0_IRQn;
    cyhal_lptimer_t *obj = cyhal_lptimer_config_structs[instance];

    Cy_MCWDT_ClearInterrupt(obj->base, CY_MCWDT_CTR0 | CY_MCWDT_CTR1 | CY_MCWDT_CTR2);

    if (NULL != obj->callback_data.callback)
    {
        cyhal_lptimer_event_callback_t callback = (cyhal_lptimer_event_callback_t) obj->callback_data.callback;
        (callback)(obj->callback_data.callback_arg, CYHAL_LPTIMER_COMPARE_MATCH);
    }
}

cy_rslt_t cyhal_lptimer_init(cyhal_lptimer_t *obj)
{
    CY_ASSERT(NULL != obj);

    obj->resource.type = CYHAL_RSC_INVALID;

    cy_rslt_t rslt = cyhal_hwmgr_allocate(CYHAL_RSC_LPTIMER, &(obj->resource));
    if (CY_RSLT_SUCCESS == rslt)
    {
        obj->base = CYHAL_LPTIMER_BASE_ADDRESSES[obj->resource.block_num];

        const cy_stc_mcwdt_config_t cfg = {
                .c0Match = 0xFFFF,
                .c1Match = 0xFFFF,
                .c0Mode = CY_MCWDT_MODE_INT,
                .c1Mode = CY_MCWDT_MODE_INT,
                .c2Mode = CY_MCWDT_MODE_NONE,
                .c2ToggleBit = 0,
                .c0ClearOnMatch = false,
                .c1ClearOnMatch = false,
                .c0c1Cascade = true,
                .c1c2Cascade = false
        };
        rslt = (cy_rslt_t) Cy_MCWDT_Init(obj->base, &cfg);
    }

    if (CY_RSLT_SUCCESS == rslt)
    {
        obj->callback_data.callback = NULL;
        obj->callback_data.callback_arg = NULL;
        cyhal_lptimer_config_structs[obj->resource.block_num] = obj;
    }

    if (CY_RSLT_SUCCESS == rslt)
    {
        IRQn_Type irqn = (IRQn_Type) (srss_interrupt_mcwdt_0_IRQn + obj->resource.block_num);
        cy_stc_sysint_t irqCfg = { irqn, CY_DEFAULT_MCWDT_PRIORITY };
        rslt = (cy_rslt_t) Cy_SysInt_Init(&irqCfg, &cyhal_lptimer_irq_handler);
        if (CY_RSLT_SUCCESS == rslt)
        {
            NVIC_EnableIRQ(irqn);
            Cy_MCWDT_Enable(obj->base, CY_MCWDT_LPTIMER_CTRL, CY_MCWDT_RESET_TIME_US);
        }
    }


    if (CY_RSLT_SUCCESS != rslt)
    {
        cyhal_lptimer_free(obj);
    }

    return rslt;
}

void cyhal_lptimer_free(cyhal_lptimer_t *obj)
{
    if (CYHAL_RSC_INVALID != obj->resource.type)
    {
        IRQn_Type irqn = (IRQn_Type)(srss_interrupt_mcwdt_0_IRQn + obj->resource.block_num);
        NVIC_DisableIRQ(irqn);

        cyhal_hwmgr_free(&(obj->resource));
        obj->resource.type = CYHAL_RSC_INVALID;
    }
    if (NULL != obj->base)
    {
        Cy_MCWDT_Disable(obj->base, CY_MCWDT_LPTIMER_CTRL, CY_MCWDT_RESET_TIME_US);
        Cy_MCWDT_DeInit(obj->base);
        obj->base = NULL;
    }
}

cy_rslt_t cyhal_lptimer_reload(cyhal_lptimer_t *obj)
{
    Cy_MCWDT_ResetCounters(obj->base, CY_MCWDT_CTR2, CY_MCWDT_RESET_TIME_US);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_lptimer_set_match(cyhal_lptimer_t *obj, uint32_t ticks)
{
    return cyhal_lptimer_set_delay(obj, ticks - cyhal_lptimer_read(obj));
}

cy_rslt_t cyhal_lptimer_set_delay(cyhal_lptimer_t *obj, uint32_t delay)
{
    /**
     * 16 bit C0/C1 are cascaded to generated a 32 bit counter.
     * Counter0 continues counting after reaching its match value
     * Interrupt is generated on Counter1 match.
     *
     * Supposed T=C0=C1=0, and we need to trigger an interrupt at T=0x28000.
     * We set C0_match to 0x8000 and C1 match to 1.
     * At T = 0x8000, C0_value matches C0_match so C1 get incremented. C1/C0=0x18000.
     * At T = 0x18000, C0_value matches C0_match again so C1 get incremented from 1 to 2.
     * When C1 get incremented from 1 to 2 theinterrupt is generated.
     * At T = 0x18000, C1/C0 = 0x28000.
     */

    if (delay <= CY_MCWDT_MIN_DELAY)
    {
        delay = CY_MCWDT_MIN_DELAY;
    }
    if (delay > CY_MCWDT_MAX_DELAY_TICKS)
    {
        delay = CY_MCWDT_MAX_DELAY_TICKS;
    }

    uint16_t c0_increment = (uint16_t)delay;
    uint16_t c1_increment = (uint16_t)(delay >> 16);

    Cy_MCWDT_ClearInterrupt(obj->base, CY_MCWDT_CTR1);

    uint16_t c0_old_match = Cy_MCWDT_GetMatch(obj->base, CY_MCWDT_COUNTER0);

    uint32_t critical_section = cyhal_system_critical_section_enter();

    /* Cascading from C0 match into C1 is queued and can take 1 full LF clk cycle.
     * There are 3 cases:
     * Case 1: if c0 = match0 then the cascade into C1 will happen 1 cycle from now. The value c1_current_ticks is 1 lower than expected.
     * Case 2: if c0 = match0 -1 then cascade may or not happen before new match value would occur. Match occurs on rising clock edge.
     *          Synching match value occurs on falling edge. Wait until c0 = match0 to ensure cascade occurs.
     * Case 3: everything works as expected.
     */
    uint16_t c0_current_ticks;
    while ((c0_current_ticks = (Cy_MCWDT_GetCount(obj->base, CY_MCWDT_COUNTER0))) == c0_old_match) {}

    uint16_t c1_current_ticks = Cy_MCWDT_GetCount(obj->base, CY_MCWDT_COUNTER1);
    if (c0_current_ticks == c0_old_match + 1)
    {
        c1_current_ticks++;
    }
    if (Cy_MCWDT_GetCount(obj->base, CY_MCWDT_COUNTER0) != c0_current_ticks)
    {
        // Just in the very unlikely case that an increment occurred while previous instruction was running.
        c1_current_ticks = Cy_MCWDT_GetCount(obj->base, CY_MCWDT_COUNTER1);
    }
    Cy_MCWDT_SetMatch(obj->base, CY_MCWDT_COUNTER0, c0_current_ticks + c0_increment, CY_MCWDT_SETMATCH_NOWAIT_TIME_US);
    Cy_MCWDT_SetMatch(obj->base, CY_MCWDT_COUNTER1, c1_current_ticks + c1_increment, CY_MCWDT_SETMATCH_NOWAIT_TIME_US);

    cyhal_system_critical_section_exit(critical_section);

    Cy_MCWDT_SetInterruptMask(obj->base, CY_MCWDT_CTR1);

    return CY_RSLT_SUCCESS;
}

uint32_t cyhal_lptimer_read(const cyhal_lptimer_t *obj)
{
    return Cy_MCWDT_GetCount(obj->base, CY_MCWDT_COUNTER2);
}

void cyhal_lptimer_register_callback(cyhal_lptimer_t *obj, cyhal_lptimer_event_callback_t callback, void *callback_arg)
{
    CY_ASSERT_L2(CYHAL_RSC_INVALID != obj->resource.block_num);

    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress) callback;
    obj->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);
}

void cyhal_lptimer_enable_event(cyhal_lptimer_t *obj, cyhal_lptimer_event_t event, uint8_t intrPriority, bool enable)
{
    CY_ASSERT(event == CYHAL_LPTIMER_COMPARE_MATCH);
    Cy_MCWDT_ClearInterrupt(obj->base, CY_MCWDT_CTR1);
    Cy_MCWDT_SetInterruptMask(obj->base, enable ? CY_MCWDT_CTR1 : 0);

    IRQn_Type irqn = (IRQn_Type)(srss_interrupt_mcwdt_0_IRQn + obj->resource.block_num);
    NVIC_SetPriority(irqn, intrPriority);
}

void cyhal_lptimer_irq_trigger(cyhal_lptimer_t *obj)
{
    CY_ASSERT_L2(CYHAL_RSC_INVALID != obj->resource.block_num);
    IRQn_Type irqn = (IRQn_Type)(srss_interrupt_mcwdt_0_IRQn + obj->resource.block_num);
    NVIC_SetPendingIRQ(irqn);
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXS40SRSS_MCWDT_INSTANCES */
