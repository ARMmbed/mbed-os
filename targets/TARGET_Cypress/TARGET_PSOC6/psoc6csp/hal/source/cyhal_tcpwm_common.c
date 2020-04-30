/***************************************************************************//**
* \file cyhal_tcpwm_common.c
*
* \brief
* Code shared between the Cypress Timer/Counter and PWM.
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

#include "cyhal_hwmgr.h"
#include "cyhal_utils.h"
#include "cyhal_syspm.h"
#include "cyhal_system.h"
#include "cyhal_tcpwm_common.h"

#if defined(CY_IP_MXTCPWM_INSTANCES)

#if defined(__cplusplus)
extern "C" {
#endif

const cyhal_tcpwm_data_t CYHAL_TCPWM_DATA[] = {
#if (CY_IP_MXTCPWM_INSTANCES > 0)
    {TCPWM0, PCLK_TCPWM0_CLOCKS0, TCPWM0_CNT_CNT_WIDTH, TCPWM0_CNT_NR, 0, tcpwm_0_interrupts_0_IRQn },
#endif
#if (CY_IP_MXTCPWM_INSTANCES > 1)
    {TCPWM1, PCLK_TCPWM1_CLOCKS0, TCPWM1_CNT_CNT_WIDTH, TCPWM1_CNT_NR, TCPWM0_CNT_NR, tcpwm_1_interrupts_0_IRQn },
#endif
#if (CY_IP_MXTCPWM_INSTANCES > 2)
    #warning Unhandled TCPWM instance count
#endif
};

#define GET_ARRAY_INDEX(block, channel)  (CYHAL_TCPWM_DATA[block].channel_offset + channel)

#if (CY_IP_MXTCPWM_INSTANCES == 0)
#define TCPWM_CHANNELS (0u)
#elif (CY_IP_MXTCPWM_INSTANCES == 1)
#define TCPWM_CHANNELS (TCPWM0_CNT_NR)
#elif (CY_IP_MXTCPWM_INSTANCES == 2)
#define TCPWM_CHANNELS (TCPWM0_CNT_NR + TCPWM1_CNT_NR)
#else
#warning Unhandled TCPWM instance count
#endif

/** Callback array for TCPWM interrupts */
static cyhal_tcpwm_common_t *cyhal_tcpwm_data_structs[TCPWM_CHANNELS];

bool cyhal_tcpwm_pm_has_enabled()
{
    for (uint8_t i = 0; i < TCPWM_CHANNELS; i++)
    {
        if (cyhal_tcpwm_data_structs[i])
        {
            return true;
        }
    }
    return false;
}

static bool cyhal_tcpwm_pm_transition_pending_value = false;

bool cyhal_tcpwm_pm_transition_pending(void)
{
    return cyhal_tcpwm_pm_transition_pending_value;
}

bool cyhal_tcpwm_pm_callback(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void* callback_arg)
{
    switch(mode)
    {
        case CYHAL_SYSPM_CHECK_FAIL:
        {
            cyhal_tcpwm_pm_transition_pending_value = false;
            break;
        }
        case CYHAL_SYSPM_AFTER_TRANSITION:
        {
            for (uint8_t i = 0; i < CY_IP_MXTCPWM_INSTANCES; i++)
            {
                uint32_t enable_flag = 0;
                TCPWM_Type* base = CYHAL_TCPWM_DATA[i].base;
                for (uint8_t j = 0; j < CYHAL_TCPWM_DATA[i].num_channels; j++)
                {
                    if (cyhal_tcpwm_data_structs[GET_ARRAY_INDEX(i, j)])
                    {
                        enable_flag |= 1u << j;
                    }
                }
                if (0 != enable_flag)
                {
                    // This only enables the counter. This does not start the timer/counter or the pwm.
                    Cy_TCPWM_Enable_Multiple(base, enable_flag);
                }
            }
            cyhal_tcpwm_pm_transition_pending_value = false;
            break;
        }
        case CYHAL_SYSPM_CHECK_READY:
        {
            for (uint8_t i = 0; i < CY_IP_MXTCPWM_INSTANCES; i++)
            {
                for (uint8_t j = 0; j < CYHAL_TCPWM_DATA[i].num_channels; j++)
                {
                    cyhal_tcpwm_common_t* obj = cyhal_tcpwm_data_structs[GET_ARRAY_INDEX(i, j)];
                    if (obj && (CY_TCPWM_PWM_STATUS_COUNTER_RUNNING & Cy_TCPWM_PWM_GetStatus(obj->base, j)))
                    {
                        return false;
                    }
                }
            }
            cyhal_tcpwm_pm_transition_pending_value = true;
            break;
        }
        default:
        {
            break;
        }
    }
    return true;
}

static cyhal_syspm_callback_data_t cyhal_tcpwm_syspm_callback_data =
{
    .callback = &cyhal_tcpwm_pm_callback,
    .states = (cyhal_syspm_callback_state_t)(CYHAL_SYSPM_CB_CPU_DEEPSLEEP | CYHAL_SYSPM_CB_SYSTEM_HIBERNATE),
    .next = NULL,
    .args = NULL,
    .ignore_modes = CYHAL_SYSPM_BEFORE_TRANSITION,
};

void cyhal_tcpwm_init_data(cyhal_tcpwm_common_t *tcpwm)
{
    if (!cyhal_tcpwm_pm_has_enabled())
    {
        cyhal_syspm_register_peripheral_callback(&cyhal_tcpwm_syspm_callback_data);
    }
    cyhal_tcpwm_data_structs[GET_ARRAY_INDEX(tcpwm->resource.block_num, tcpwm->resource.channel_num)] = tcpwm;
}

void cyhal_tcpwm_irq_handler(void)
{
    IRQn_Type irqn = CYHAL_GET_CURRENT_IRQN();
    uint8_t block, channel = 0;
    // Determine TCPWM block and channel from IRQn
    for (block = 0; block < CY_IP_MXTCPWM_INSTANCES; block++)
    {
        if ((irqn >= CYHAL_TCPWM_DATA[block].isr_offset) && (irqn < CYHAL_TCPWM_DATA[block].isr_offset + CYHAL_TCPWM_DATA[block].num_channels))
        {
            channel = irqn - CYHAL_TCPWM_DATA[block].isr_offset;
            break;
        }
    }

    if (block < CY_IP_MXTCPWM_INSTANCES)
    {
        TCPWM_Type *blockAddr = CYHAL_TCPWM_DATA[block].base;
        cyhal_tcpwm_common_t *tcpwm = cyhal_tcpwm_data_structs[GET_ARRAY_INDEX(block, channel)];
        if (tcpwm->callback_data.callback != NULL)
        {
            cyhal_tcpwm_event_callback_t callback = (cyhal_tcpwm_event_callback_t) tcpwm->callback_data.callback;
            /* Call registered callbacks here */
            (void) (callback) (tcpwm->callback_data.callback_arg, Cy_TCPWM_GetInterruptStatus(blockAddr, channel));
        }

        Cy_TCPWM_ClearInterrupt(blockAddr, channel, CY_TCPWM_INT_ON_CC_OR_TC);
    }
    else
    {
        CY_HALT(); // Could not determine the block/channel for IRQn
    }
}

/*******************************************************************************
*       TCPWM Shared HAL Functions
*******************************************************************************/

void cyhal_tcpwm_free(cyhal_tcpwm_common_t *obj)
{
    CY_ASSERT(NULL != obj);

    IRQn_Type irqn = (IRQn_Type)(CYHAL_TCPWM_DATA[obj->resource.block_num].isr_offset + obj->resource.channel_num);
    NVIC_DisableIRQ(irqn);

    cyhal_utils_release_if_used(&(obj->pin));

    if (NULL != obj->base)
    {
        cyhal_tcpwm_data_structs[GET_ARRAY_INDEX(obj->resource.block_num, obj->resource.channel_num)] = NULL;
        if (!cyhal_tcpwm_pm_has_enabled())
        {
            cyhal_syspm_unregister_peripheral_callback(&cyhal_tcpwm_syspm_callback_data);
        }

        Cy_TCPWM_PWM_Disable(obj->base, obj->resource.channel_num);

        cyhal_hwmgr_free(&(obj->resource));
        obj->base = NULL;
        obj->resource.type = CYHAL_RSC_INVALID;
    }

    if (obj->dedicated_clock)
    {
        cy_en_sysclk_status_t rslt = Cy_SysClk_PeriphDisableDivider(obj->clock.div_type, obj->clock.div_num);
        CY_UNUSED_PARAMETER(rslt); /* CY_ASSERT only processes in DEBUG, ignores for others */
        CY_ASSERT(CY_SYSCLK_SUCCESS == rslt);
        cyhal_hwmgr_free_clock(&(obj->clock));
        obj->dedicated_clock = false;
    }
}

void cyhal_tcpwm_register_callback(cyhal_resource_inst_t *resource, cy_israddress callback, void *callback_arg)
{
    uint8_t index = GET_ARRAY_INDEX(resource->block_num, resource->channel_num);
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    cyhal_tcpwm_data_structs[index]->callback_data.callback = callback;
    cyhal_tcpwm_data_structs[index]->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);

    IRQn_Type irqn = (IRQn_Type)(CYHAL_TCPWM_DATA[resource->block_num].isr_offset + resource->channel_num);
    /* Only enable if it's not already enabled */
    if (NVIC_GetEnableIRQ(irqn) == 0)
    {
        cy_stc_sysint_t irqCfg = { irqn, CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, cyhal_tcpwm_irq_handler);
        NVIC_EnableIRQ(irqn);
    }
}

void cyhal_tcpwm_enable_event(TCPWM_Type *type, cyhal_resource_inst_t *resource, uint32_t event, uint8_t intr_priority, bool enable)
{
    uint32_t old_mask = Cy_TCPWM_GetInterruptMask(type, resource->channel_num);
    if (enable)
    {
        // Clear any newly enabled events so that old IRQs don't trigger ISRs
        Cy_TCPWM_ClearInterrupt(type, resource->channel_num, ~old_mask & event);
    }
    Cy_TCPWM_SetInterruptMask(type, resource->channel_num, enable ? (old_mask | event) : (old_mask & ~event));

    IRQn_Type irqn = (IRQn_Type) (CYHAL_TCPWM_DATA[resource->block_num].isr_offset + resource->channel_num);
    NVIC_SetPriority(irqn, intr_priority);
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXTCPWM_INSTANCES) */
