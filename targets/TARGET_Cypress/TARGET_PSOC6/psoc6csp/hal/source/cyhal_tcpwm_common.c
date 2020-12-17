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
#include "cyhal_clock.h"

#if defined(CY_IP_MXTCPWM_INSTANCES) || defined(CY_IP_M0S8TCPWM_INSTANCES)

#if defined(__cplusplus)
extern "C" {
#endif

const _cyhal_tcpwm_data_t _CYHAL_TCPWM_DATA[] = {
#if defined(CY_IP_MXTCPWM_INSTANCES)
    #if (CY_IP_MXTCPWM_VERSION == 1)
        #if (CY_IP_MXTCPWM_INSTANCES > 0)
            {TCPWM0, PCLK_TCPWM0_CLOCKS0, TCPWM0_CNT_CNT_WIDTH, TCPWM0_CNT_NR, 0, tcpwm_0_interrupts_0_IRQn },
        #endif
        #if (CY_IP_MXTCPWM_INSTANCES > 1)
            {TCPWM1, PCLK_TCPWM1_CLOCKS0, TCPWM1_CNT_CNT_WIDTH, TCPWM1_CNT_NR, TCPWM0_CNT_NR, tcpwm_1_interrupts_0_IRQn },
        #endif
        #if (CY_IP_MXTCPWM_INSTANCES > 2)
            #warning Unhandled TCPWM instance count
        #endif
    #else // (CY_IP_MXTCPWM_VERSION >= 2)
        #if (CY_IP_MXTCPWM_INSTANCES == 1)
            #if (TCPWM_GRP_NR > 0)
                {TCPWM0, PCLK_TCPWM0_CLOCKS0, TCPWM_GRP_NR0_CNT_GRP_CNT_WIDTH, TCPWM_GRP_NR0_GRP_GRP_CNT_NR, 0, tcpwm_0_interrupts_0_IRQn },
            #endif
            #if (TCPWM_GRP_NR > 1)
                {TCPWM0, PCLK_TCPWM0_CLOCKS256, TCPWM_GRP_NR1_CNT_GRP_CNT_WIDTH, TCPWM_GRP_NR1_GRP_GRP_CNT_NR, TCPWM_GRP_NR0_GRP_GRP_CNT_NR, tcpwm_0_interrupts_256_IRQn },
            #endif
            #if (TCPWM_GRP_NR > 2)
                {TCPWM0, PCLK_TCPWM0_CLOCKS512, TCPWM_GRP_NR2_CNT_GRP_CNT_WIDTH, TCPWM_GRP_NR2_GRP_GRP_CNT_NR, TCPWM_GRP_NR0_GRP_GRP_CNT_NR + TCPWM_GRP_NR1_GRP_GRP_CNT_NR, tcpwm_0_interrupts_512_IRQn },
            #endif
            #if (TCPWM_GRP_NR > 3)
                {TCPWM0, PCLK_TCPWM0_CLOCKS768, TCPWM_GRP_NR3_CNT_GRP_CNT_WIDTH, TCPWM_GRP_NR3_GRP_GRP_CNT_NR, TCPWM_GRP_NR0_GRP_GRP_CNT_NR + TCPWM_GRP_NR1_GRP_GRP_CNT_NR + TCPWM_GRP_NR2_GRP_GRP_CNT_NR, tcpwm_0_interrupts_768_IRQn },
            #endif
        #endif
        #if (CY_IP_MXTCPWM_INSTANCES > 1)
            #warning Unhandled TCPWM instance count
        #endif
    #endif
#elif defined(CY_IP_M0S8TCPWM_INSTANCES)
    #if (CY_IP_M0S8TCPWM_INSTANCES == 1)
        {TCPWM, PCLK_TCPWM_CLOCKS0, 16, TCPWM_CNT_NR, 0, tcpwm_interrupts_0_IRQn },
    #endif
    #if (CY_IP_M0S8TCPWM_INSTANCES > 1)
        #warning Unhandled TCPWM instance count
    #endif
#endif
};

#if defined(CY_IP_MXTCPWM_INSTANCES)
#if (CY_IP_MXTCPWM_VERSION == 1)
    #if (CY_IP_MXTCPWM_INSTANCES == 0)
        #define _CYHAL_TCPWM_CHANNELS (0u)
    #elif (CY_IP_MXTCPWM_INSTANCES == 1)
        #define _CYHAL_TCPWM_CHANNELS (TCPWM0_CNT_NR)
    #elif (CY_IP_MXTCPWM_INSTANCES == 2)
        #define _CYHAL_TCPWM_CHANNELS (TCPWM0_CNT_NR + TCPWM1_CNT_NR)
    #else
        #warning Unhandled TCPWM instance count
    #endif
#else // (CY_IP_MXTCPWM_VERSION >= 2)
    #if (TCPWM_GRP_NR == 0)
        #define _CYHAL_TCPWM_CHANNELS 0
    #elif (TCPWM_GRP_NR == 1)
        #define _CYHAL_TCPWM_CHANNELS TCPWM_GRP_NR0_GRP_GRP_CNT_NR
    #elif (TCPWM_GRP_NR == 2)
        #define _CYHAL_TCPWM_CHANNELS TCPWM_GRP_NR0_GRP_GRP_CNT_NR + TCPWM_GRP_NR1_GRP_GRP_CNT_NR
    #elif (TCPWM_GRP_NR == 3)
        #define _CYHAL_TCPWM_CHANNELS TCPWM_GRP_NR0_GRP_GRP_CNT_NR + TCPWM_GRP_NR1_GRP_GRP_CNT_NR + TCPWM_GRP_NR2_GRP_GRP_CNT_NR
    #elif (TCPWM_GRP_NR == 4)
        #define _CYHAL_TCPWM_CHANNELS TCPWM_GRP_NR0_GRP_GRP_CNT_NR + TCPWM_GRP_NR1_GRP_GRP_CNT_NR + TCPWM_GRP_NR2_GRP_GRP_CNT_NR + TCPWM_GRP_NR3_GRP_GRP_CNT_NR
    #endif
#endif
#elif defined(CY_IP_M0S8TCPWM_INSTANCES)
    #if (CY_IP_M0S8TCPWM_INSTANCES == 1)
        #define _CYHAL_TCPWM_CHANNELS (TCPWM_CNT_NR)
    #else
        #warning Unhandled TCPWM instance count
    #endif
#endif

#define _CYHAL_TCPWM_GET_ARRAY_INDEX(block, channel)  (_CYHAL_TCPWM_DATA[block].channel_offset + channel)

/** Callback array for TCPWM interrupts */
static cyhal_tcpwm_t *_cyhal_tcpwm_data_structs[_CYHAL_TCPWM_CHANNELS];

bool _cyhal_tcpwm_pm_has_enabled()
{
    for (uint8_t i = 0; i < _CYHAL_TCPWM_CHANNELS; i++)
    {
        if (_cyhal_tcpwm_data_structs[i])
        {
            return true;
        }
    }
    return false;
}

static bool _cyhal_tcpwm_pm_transition_pending_value = false;

bool _cyhal_tcpwm_pm_transition_pending(void)
{
    return _cyhal_tcpwm_pm_transition_pending_value;
}

bool _cyhal_tcpwm_pm_callback(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void* callback_arg)
{
    CY_UNUSED_PARAMETER(state);
    CY_UNUSED_PARAMETER(callback_arg);
    switch(mode)
    {
        case CYHAL_SYSPM_CHECK_FAIL:
        {
            _cyhal_tcpwm_pm_transition_pending_value = false;
            break;
        }
        case CYHAL_SYSPM_AFTER_TRANSITION:
        {
            for (uint8_t i = 0; i < _CYHAL_TCPWM_INSTANCES; i++)
            {
                #if defined(CY_IP_M0S8TCPWM) || (CY_IP_MXTCPWM_VERSION == 1)
                uint32_t enable_flag = 0;
                #endif
                TCPWM_Type* base = _CYHAL_TCPWM_DATA[i].base;
                for (uint8_t j = 0; j < _CYHAL_TCPWM_DATA[i].num_channels; j++)
                {
                    if (_cyhal_tcpwm_data_structs[_CYHAL_TCPWM_GET_ARRAY_INDEX(i, j)])
                    {
                        #if defined(CY_IP_M0S8TCPWM) || (CY_IP_MXTCPWM_VERSION == 1)
                        enable_flag |= 1u << j;
                        #else
                        Cy_TCPWM_Enable_Single(base, j);
                        #endif
                    }
                }
                #if defined(CY_IP_M0S8TCPWM) || (CY_IP_MXTCPWM_VERSION == 1)
                if (0 != enable_flag)
                {
                    // This only enables the counter. This does not start the timer/counter or the pwm.
                    Cy_TCPWM_Enable_Multiple(base, enable_flag);
                }
                #endif
            }
            _cyhal_tcpwm_pm_transition_pending_value = false;
            break;
        }
        case CYHAL_SYSPM_CHECK_READY:
        {
            for (uint8_t i = 0; i < _CYHAL_TCPWM_INSTANCES; i++)
            {
                for (uint8_t j = 0; j < _CYHAL_TCPWM_DATA[i].num_channels; j++)
                {
                    cyhal_tcpwm_t* obj = _cyhal_tcpwm_data_structs[_CYHAL_TCPWM_GET_ARRAY_INDEX(i, j)];
                    if (obj && (CY_TCPWM_PWM_STATUS_COUNTER_RUNNING & Cy_TCPWM_PWM_GetStatus(obj->base, _CYHAL_TCPWM_CNT_NUMBER(obj->resource))))
                    {
                        return false;
                    }
                }
            }
            _cyhal_tcpwm_pm_transition_pending_value = true;
            break;
        }
        default:
        {
            break;
        }
    }
    return true;
}

static cyhal_syspm_callback_data_t _cyhal_tcpwm_syspm_callback_data =
{
    .callback = &_cyhal_tcpwm_pm_callback,
    .states = (cyhal_syspm_callback_state_t)(CYHAL_SYSPM_CB_CPU_DEEPSLEEP | CYHAL_SYSPM_CB_SYSTEM_HIBERNATE),
    .next = NULL,
    .args = NULL,
    .ignore_modes = CYHAL_SYSPM_BEFORE_TRANSITION,
};

void _cyhal_tcpwm_init_data(cyhal_tcpwm_t *tcpwm)
{
    if (!_cyhal_tcpwm_pm_has_enabled())
    {
        _cyhal_syspm_register_peripheral_callback(&_cyhal_tcpwm_syspm_callback_data);
    }
    _cyhal_tcpwm_data_structs[_CYHAL_TCPWM_GET_ARRAY_INDEX(tcpwm->resource.block_num, tcpwm->resource.channel_num)] = tcpwm;
}

void _cyhal_tcpwm_irq_handler(void)
{
    IRQn_Type irqn = _CYHAL_UTILS_GET_CURRENT_IRQN();
    uint8_t block, channel = 0;
    // Determine TCPWM block and channel from IRQn
    for (block = 0; block < _CYHAL_TCPWM_INSTANCES; block++)
    {
        if ((irqn >= _CYHAL_TCPWM_DATA[block].isr_offset) && (irqn < _CYHAL_TCPWM_DATA[block].isr_offset + _CYHAL_TCPWM_DATA[block].num_channels))
        {
            channel = (uint8_t)(irqn - _CYHAL_TCPWM_DATA[block].isr_offset);
            break;
        }
    }

    if (block < _CYHAL_TCPWM_INSTANCES)
    {
        TCPWM_Type *blockAddr = _CYHAL_TCPWM_DATA[block].base;
        uint32_t intrCause = Cy_TCPWM_GetInterruptStatusMasked(blockAddr, channel);
        cyhal_tcpwm_t *tcpwm = _cyhal_tcpwm_data_structs[_CYHAL_TCPWM_GET_ARRAY_INDEX(block, channel)];
        if (tcpwm->callback_data.callback != NULL)
        {
            _cyhal_tcpwm_event_callback_t callback = (_cyhal_tcpwm_event_callback_t) tcpwm->callback_data.callback;
            /* Call registered callbacks here */
            (void) (callback) (tcpwm->callback_data.callback_arg, intrCause);
        }

        Cy_TCPWM_ClearInterrupt(blockAddr, channel, intrCause);
    }
    else
    {
        CY_HALT(); // Could not determine the block/channel for IRQn
    }
}

/*******************************************************************************
*       TCPWM Shared HAL Functions
*******************************************************************************/

void _cyhal_tcpwm_free(cyhal_tcpwm_t *obj)
{
    CY_ASSERT(NULL != obj);

    IRQn_Type irqn = (IRQn_Type)(_CYHAL_TCPWM_DATA[obj->resource.block_num].isr_offset + obj->resource.channel_num);
    NVIC_DisableIRQ(irqn);

    if (NULL != obj->base)
    {
        _cyhal_tcpwm_data_structs[_CYHAL_TCPWM_GET_ARRAY_INDEX(obj->resource.block_num, obj->resource.channel_num)] = NULL;
        if (!_cyhal_tcpwm_pm_has_enabled())
        {
            _cyhal_syspm_unregister_peripheral_callback(&_cyhal_tcpwm_syspm_callback_data);
        }

        #if defined(CY_IP_MXTCPWM) && (CY_IP_MXTCPWM_VERSION >= 2)
        Cy_TCPWM_Disable_Single(obj->base, _CYHAL_TCPWM_CNT_NUMBER(obj->resource));
        #else
        Cy_TCPWM_PWM_Disable(obj->base, _CYHAL_TCPWM_CNT_NUMBER(obj->resource));
        #endif

        cyhal_hwmgr_free(&(obj->resource));
        obj->base = NULL;
        obj->resource.type = CYHAL_RSC_INVALID;
    }

    if (obj->dedicated_clock)
    {
        cy_en_sysclk_status_t rslt = Cy_SysClk_PeriphDisableDivider((cy_en_divider_types_t)obj->clock.block, obj->clock.channel);
        CY_UNUSED_PARAMETER(rslt); /* CY_ASSERT only processes in DEBUG, ignores for others */
        CY_ASSERT(CY_SYSCLK_SUCCESS == rslt);
        cyhal_clock_free(&(obj->clock));
        obj->dedicated_clock = false;
    }
}

void _cyhal_tcpwm_register_callback(cyhal_resource_inst_t *resource, cy_israddress callback, void *callback_arg)
{
    uint8_t index = _CYHAL_TCPWM_GET_ARRAY_INDEX(resource->block_num, resource->channel_num);
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    _cyhal_tcpwm_data_structs[index]->callback_data.callback = callback;
    _cyhal_tcpwm_data_structs[index]->callback_data.callback_arg = callback_arg;
    cyhal_system_critical_section_exit(savedIntrStatus);

    IRQn_Type irqn = (IRQn_Type)(_CYHAL_TCPWM_DATA[resource->block_num].isr_offset + resource->channel_num);
    /* Only enable if it's not already enabled */
    if (NVIC_GetEnableIRQ(irqn) == 0)
    {
        cy_stc_sysint_t irqCfg = { irqn, CYHAL_ISR_PRIORITY_DEFAULT };
        Cy_SysInt_Init(&irqCfg, _cyhal_tcpwm_irq_handler);
        NVIC_EnableIRQ(irqn);
    }
}

void _cyhal_tcpwm_enable_event(TCPWM_Type *type, cyhal_resource_inst_t *resource, uint32_t event, uint8_t intr_priority, bool enable)
{
    uint32_t old_mask = Cy_TCPWM_GetInterruptMask(type, _CYHAL_TCPWM_CNT_NUMBER(*resource));
    if (enable)
    {
        // Clear any newly enabled events so that old IRQs don't trigger ISRs
        Cy_TCPWM_ClearInterrupt(type, _CYHAL_TCPWM_CNT_NUMBER(*resource), ~old_mask & event);
    }
    Cy_TCPWM_SetInterruptMask(type, _CYHAL_TCPWM_CNT_NUMBER(*resource), enable ? (old_mask | event) : (old_mask & ~event));

    IRQn_Type irqn = (IRQn_Type) (_CYHAL_TCPWM_DATA[resource->block_num].isr_offset + resource->channel_num);
    NVIC_SetPriority(irqn, intr_priority);
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXTCPWM_INSTANCES) */
