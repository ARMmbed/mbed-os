/***************************************************************************//**
* \file cy_hal_lptimer.c
*
* \brief
* Provides a high level interface for interacting with the Cypress Low-Power Timer.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
#include "cy_wdt.h"
#include "cy_syslib.h"
#include "cy_sysint.h"
#include "cyhal_lptimer.h"
#include "cyhal_hwmgr.h"

#ifdef CY_IP_MXS40SRSS_MCWDT_INSTANCES

#if defined(__cplusplus)
extern "C" {
#endif

static MCWDT_STRUCT_Type * const cyhal_lptimer_base[] = {
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

static const uint16_t CY_MCWDT_RESET_TIME_US = 62;
static const uint16_t CY_MCWDT_SETMATCH_TIME_US = 93;

typedef struct {
    cyhal_lptimer_irq_handler_t handler;
    void *handler_arg;
} cyhal_lptimer_irq_info_t;

static cyhal_lptimer_irq_info_t cyhal_lptimer_handlers[CY_IP_MXS40SRSS_MCWDT_INSTANCES];

static void cyhal_lptimer_dispatch(void)
{
    // IPSR is numbered from 0 (0-15 are exceptions); IRQn_Type is numbered from -16
    uint32_t instance = __get_IPSR() - 0x10 - (uint32_t)srss_interrupt_mcwdt_0_IRQn;
    if (instance < CY_IP_MXS40SRSS_MCWDT_INSTANCES)
    {
        Cy_MCWDT_ClearInterrupt(cyhal_lptimer_base[instance], CY_MCWDT_CTR0 | CY_MCWDT_CTR1 | CY_MCWDT_CTR2);
        cyhal_lptimer_irq_info_t *info = &cyhal_lptimer_handlers[instance];
        if (NULL != info->handler)
            (*info->handler)(info->handler_arg, CYHAL_LPTIMER_COMPARE_MATCH);
    }
}

cy_rslt_t cyhal_lptimer_init(cyhal_lptimer_t *obj)
{
    cy_rslt_t rslt;
    obj->resource.type = CYHAL_RSC_INVALID;
    if (CY_RSLT_SUCCESS == (rslt = cyhal_hwmgr_allocate(CYHAL_RSC_LPTIMER, &(obj->resource))))
    {
        obj->base = cyhal_lptimer_base[obj->resource.block_num];
        cyhal_lptimer_handlers[obj->resource.block_num].handler = NULL;
        IRQn_Type irq = (IRQn_Type)(srss_interrupt_mcwdt_0_IRQn + obj->resource.block_num);
        static const uint32_t CY_DEFAULT_WDT_PRIORITY = 3;
        cy_stc_sysint_t irqCfg = { irq, CY_DEFAULT_WDT_PRIORITY };
        if (NVIC_GetEnableIRQ(irq) ||
            CY_RSLT_SUCCESS == (rslt = (cy_rslt_t)Cy_SysInt_Init(&irqCfg, &cyhal_lptimer_dispatch)))
        {
            NVIC_EnableIRQ(irq);

            bool configured = cyhal_hwmgr_is_configured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
            if (!configured)
            {
                static const cy_stc_mcwdt_config_t cfg = {
                /* .c0Match = */ 0,
                /* .c1Match = */ 0,
                /* .c0Mode = */ CY_MCWDT_MODE_INT,
                /* .c1Mode = */ CY_MCWDT_MODE_NONE,
                /* .c2ToggleBit = */ 0,
                /* .c2Mode = */ CY_MCWDT_MODE_NONE,
                /* .c0ClearOnMatch = */ false,
                /* .c1ClearOnMatch = */ false,
                /* .c0c1Cascade = */ false,
                /* .c1c2Cascade = */ false
                };
                if (CY_RSLT_SUCCESS == (rslt = (cy_rslt_t)Cy_MCWDT_Init(obj->base, &cfg)))
                {
                    Cy_MCWDT_Enable(obj->base, CY_MCWDT_CTR0, CY_MCWDT_RESET_TIME_US);
                    rslt = cyhal_hwmgr_set_configured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
                }
            }
        }
    }
    
    if (CY_RSLT_SUCCESS != rslt)
        cyhal_lptimer_free(obj);

    return rslt;
}

void cyhal_lptimer_free(cyhal_lptimer_t *obj)
{
    if (CYHAL_RSC_INVALID != obj->resource.type)
    {
        cyhal_hwmgr_set_unconfigured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
        cyhal_lptimer_handlers[obj->resource.block_num].handler = NULL;
        obj->resource.type = CYHAL_RSC_INVALID;
    }
    if (NULL != obj->base)
    {
        Cy_MCWDT_DeInit(obj->base);
        obj->base = NULL;
    }
}

cy_rslt_t cyhal_lptimer_reload(cyhal_lptimer_t *obj)
{
    Cy_MCWDT_ResetCounters(obj->base, CY_MCWDT_CTR0, CY_MCWDT_RESET_TIME_US);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_lptimer_set_time(cyhal_lptimer_t *obj, uint32_t time)
{
    Cy_MCWDT_SetMatch(obj->base, CY_MCWDT_COUNTER0, time, CY_MCWDT_SETMATCH_TIME_US);
    Cy_MCWDT_SetClearOnMatch(obj->base, CY_MCWDT_COUNTER0, 1);
    Cy_MCWDT_ResetCounters(obj->base, CY_MCWDT_CTR0,  CY_MCWDT_RESET_TIME_US);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_lptimer_set_match(cyhal_lptimer_t *obj, uint32_t value)
{
    Cy_MCWDT_SetMatch(obj->base, CY_MCWDT_COUNTER0, value, 0);
    return CY_RSLT_SUCCESS;
}

uint32_t cyhal_lptimer_read(const cyhal_lptimer_t *obj)
{
    return Cy_MCWDT_GetCount(obj->base, CY_MCWDT_COUNTER0);
}

void cyhal_lptimer_register_irq(cyhal_lptimer_t *obj, cyhal_lptimer_irq_handler_t handler, void *handler_arg)
{
    CY_ASSERT_L2(CYHAL_RSC_INVALID != obj->resource.block_num);
    cyhal_lptimer_handlers[obj->resource.block_num].handler = NULL;
    __DSB();
    __ISB();
    cyhal_lptimer_handlers[obj->resource.block_num].handler_arg = handler_arg;
    __DSB();
    __ISB();
    cyhal_lptimer_handlers[obj->resource.block_num].handler = handler;
}

void cyhal_lptimer_irq_enable(cyhal_lptimer_t *obj, cyhal_lptimer_irq_event_t event, bool enable)
{
    Cy_MCWDT_SetInterruptMask(obj->base, enable ? CY_MCWDT_CTR0 : 0);
}

void cyhal_lptimer_irq_trigger(cyhal_lptimer_t *obj)
{
    CY_ASSERT_L2(CYHAL_RSC_INVALID != obj->resource.block_num);
    IRQn_Type irq = (IRQn_Type)(srss_interrupt_mcwdt_0_IRQn + obj->resource.block_num);
    NVIC_SetPendingIRQ(irq);
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXS40SRSS_MCWDT_INSTANCES */
