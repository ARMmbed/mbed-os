/***************************************************************************//**
* \file cyhal_tcpwm_common.c
*
* \brief
* Code shared between the Cypress Timer/Counter and PWM.
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

uint32_t _CYHAL_INPUT_TRIGGERS_USED[_CYHAL_TCPWM_INSTANCES][(_CYHAL_TCPWM_TRIGGER_INPUTS_PER_BLOCK / 32) + 1];

#if (CY_IP_MXTCPWM_VERSION == 2)
uint8_t _CYHAL_OUTPUT_TRIGGERS_USED[TCPWM_GRP_NR0_GRP_GRP_CNT_NR + TCPWM_GRP_NR1_GRP_GRP_CNT_NR];
#endif

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
#if defined(CY_IP_M0S8PERI_TR) || defined(CY_IP_MXPERI_TR)
    for (uint8_t i = 0; i < _CYHAL_TCPWM_INPUTS; i++)
    {
        tcpwm->inputs[i] = CYHAL_TRIGGER_CPUSS_ZERO;
    }
#endif

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

#if defined(CY_IP_M0S8PERI_TR) || defined(CY_IP_MXPERI_TR)
        for (uint8_t i = 0; i < _CYHAL_TCPWM_OUTPUTS; i++)
        {
            _cyhal_tcpwm_disable_output(obj, (cyhal_tcpwm_output_t)i);
        }
        for (uint8_t i = 0; i < _CYHAL_TCPWM_INPUTS; i++)
        {
            if (CYHAL_TRIGGER_CPUSS_ZERO != obj->inputs[i])
            {
                _cyhal_tcpwm_disconnect_digital(obj, obj->inputs[i], (cyhal_tcpwm_input_t)i);
            }
        }
#endif

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

#if (CY_IP_MXTCPWM_VERSION == 2U)
static uint8_t _cyhal_tcpwm_convert_output_t(cyhal_tcpwm_output_t signal)
{
    switch(signal)
    {
        case CYHAL_TCPWM_OUTPUT_OVERFLOW:
            return CY_TCPWM_CNT_TRIGGER_ON_OVERFLOW;
        case CYHAL_TCPWM_OUTPUT_UNDERFLOW:
            return CY_TCPWM_CNT_TRIGGER_ON_UNDEFLOW;
        case CYHAL_TCPWM_OUTPUT_TERMINAL_COUNT:
            return CY_TCPWM_CNT_TRIGGER_ON_TC;
        case CYHAL_TCPWM_OUTPUT_COMPARE_MATCH:
            return CY_TCPWM_CNT_TRIGGER_ON_CC0_MATCH;
        case CYHAL_TCPWM_OUTPUT_LINE_OUT:
            return CY_TCPWM_CNT_TRIGGER_ON_LINE_OUT;
        default:
            CY_ASSERT(false);
            return 0;
    }
}
#endif

#if defined(CY_IP_M0S8PERI_TR) || defined(CY_IP_MXPERI_TR)
#if (CY_IP_MXTCPWM_VERSION == 1U) ||(CY_IP_MXTCPWM_VERSION == 2U) || (CY_IP_M0S8TCPWM_VERSION == 2)
// Assumes trig_index is not offset by _CYHAL_TCPWM_TRIGGER_INPUTS_IDX_OFFSET
// (that is, it is 0 indexed).
static cyhal_dest_t _cyhal_tpwm_calculate_dest(uint8_t block, uint8_t trig_index)
{
#if (CY_IP_MXTCPWM_VERSION == 1U)
    if(block == 0)
    {
        return (cyhal_dest_t)(CYHAL_TRIGGER_TCPWM0_TR_IN0 + trig_index);
    }
    else
    {
        CY_ASSERT(block == 1);
        return (cyhal_dest_t)(CYHAL_TRIGGER_TCPWM1_TR_IN0 + trig_index);
    }
#elif (CY_IP_MXTCPWM_VERSION == 2U)
    return block == 0 ? (cyhal_dest_t)(CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN0 + trig_index) : (cyhal_dest_t)(CYHAL_TRIGGER_TCPWM0_TR_ALL_CNT_IN14 + trig_index);
#elif (CY_IP_M0S8TCPWM_VERSION == 2)
#if defined(CY_DEVICE_PSOC4AS1)
    CY_ASSERT(block == 0);
    return (cyhal_dest_t)(CYHAL_TRIGGER_TCPWM_TR_IN12 + trig_index);
#elif defined(CY_DEVICE_PMG1S3)
    CY_ASSERT(block == 0);
    return (cyhal_dest_t)(CYHAL_TRIGGER_TCPWM_TR_IN8 + trig_index);
#else
    CY_ASSERT(block == 0);
    return (cyhal_dest_t)(CYHAL_TRIGGER_TCPWM_TR_IN7 + trig_index);
#endif
#endif
}
#endif

#if (CY_IP_MXTCPWM_VERSION == 1U) || (CY_IP_M0S8TCPWM_VERSION == 2)
static cyhal_source_t _cyhal_tcpwm_calculate_source(uint8_t block, uint8_t chnl, cyhal_tcpwm_output_t signal)
{
#if (CY_IP_MXTCPWM_VERSION == 1U)
    switch(signal)
    {
        case CYHAL_TCPWM_OUTPUT_OVERFLOW:
            return block == 0 ? (cyhal_source_t)(CYHAL_TRIGGER_TCPWM0_TR_OVERFLOW0 + chnl) : (cyhal_source_t)(CYHAL_TRIGGER_TCPWM1_TR_OVERFLOW0 + chnl);
        case CYHAL_TCPWM_OUTPUT_UNDERFLOW:
            return block == 0 ? (cyhal_source_t)(CYHAL_TRIGGER_TCPWM0_TR_UNDERFLOW0 + chnl) : (cyhal_source_t)(CYHAL_TRIGGER_TCPWM1_TR_UNDERFLOW0 + chnl);
        case CYHAL_TCPWM_OUTPUT_COMPARE_MATCH:
            return block == 0 ? (cyhal_source_t)(CYHAL_TRIGGER_TCPWM0_TR_COMPARE_MATCH0 + chnl) : (cyhal_source_t)(CYHAL_TRIGGER_TCPWM1_TR_COMPARE_MATCH0 + chnl);
       default:
            // Should never reach here
            CY_ASSERT(false);
            return CYHAL_TRIGGER_CPUSS_ZERO;
    }
#elif (CY_IP_M0S8TCPWM_VERSION == 2)
    CY_UNUSED_PARAMETER(block);
    switch(signal)
    {
        case CYHAL_TCPWM_OUTPUT_OVERFLOW:
            return (cyhal_source_t)(CYHAL_TRIGGER_TCPWM_TR_OVERFLOW0 + chnl);
        case CYHAL_TCPWM_OUTPUT_UNDERFLOW:
            return (cyhal_source_t)(CYHAL_TRIGGER_TCPWM_TR_UNDERFLOW0 + chnl);
        case CYHAL_TCPWM_OUTPUT_COMPARE_MATCH:
            return (cyhal_source_t)(CYHAL_TRIGGER_TCPWM_TR_COMPARE_MATCH0 + chnl);
       default:
            // Should never reach here
            CY_ASSERT(false);
            return CYHAL_TRIGGER_CPUSS_ZERO;
    }
#endif
}
#elif (CY_IP_MXTCPWM_VERSION == 2U)
static cyhal_source_t _cyhal_tcpwm_calculate_source(uint8_t out_trig_idx, uint8_t free_trig)
{
    // Triggers are ordered by output trig idx, then block, then channel.
    // Offsetting by out_trig_idx is sufficient to get the correct trigger.
    if(free_trig == 0)
    {
        return (cyhal_source_t)(CYHAL_TRIGGER_TCPWM0_TR_OUT00 + out_trig_idx);
    }
    else if(free_trig == 1)
    {
        return (cyhal_source_t)(CYHAL_TRIGGER_TCPWM0_TR_OUT10 + out_trig_idx);
    }
    else
    {
        // Should never reach here
        CY_ASSERT(false);
        return CYHAL_TRIGGER_CPUSS_ZERO;
    }
}
#endif
#endif /* defined(CY_IP_M0S8PERI_TR) || defined(CY_IP_MXPERI_TR) */

cy_rslt_t _cyhal_tcpwm_connect_digital(cyhal_tcpwm_t *obj, cyhal_source_t source, cyhal_tcpwm_input_t signal, cyhal_edge_type_t type)
{
#if defined(CY_IP_M0S8PERI_TR) || defined(CY_IP_MXPERI_TR)
    cy_rslt_t rslt;
    const uint8_t chnl = obj->resource.channel_num;
    const uint8_t block = obj->resource.block_num;

    cyhal_signal_type_t signal_type = (type == CYHAL_EDGE_TYPE_LEVEL)
        ? CYHAL_SIGNAL_TYPE_LEVEL
        : CYHAL_SIGNAL_TYPE_EDGE;

    // Find free input trigger index
    uint32_t saved_intr_status = cyhal_system_critical_section_enter();
    uint8_t trig_index;
    for(trig_index = 0; trig_index < _CYHAL_TCPWM_TRIGGER_INPUTS_PER_BLOCK; trig_index++)
    {
        if(!(_CYHAL_INPUT_TRIGGERS_USED[obj->resource.block_num][trig_index / 32] & (1 << trig_index)))
            break;
    }
    cyhal_system_critical_section_exit(saved_intr_status);

    if(trig_index == _CYHAL_TCPWM_TRIGGER_INPUTS_PER_BLOCK)
        return CYHAL_TCPWM_RSLT_ERR_NO_FREE_INPUT_SIGNALS;

#if (CY_IP_MXTCPWM_VERSION == 1U) || (CY_IP_M0S8TCPWM_VERSION == 2)
    // Clear appropriate trigger reg field and set input index and edge trigger type
    // Note: Input trigger indices 0 and 1 are reserved for constant signals 0
    // and 1 respectively. The first actual trigger input has index 2.
    switch(signal)
    {
        case CYHAL_TCPWM_INPUT_START:
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL0_START_SEL_Msk;
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL0_START_SEL, trig_index + _CYHAL_TCPWM_TRIGGER_INPUTS_IDX_OFFSET);
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL1_START_EDGE_Msk;
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL1_START_EDGE, type);
            break;
        case CYHAL_TCPWM_INPUT_STOP:
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL0_STOP_SEL_Msk;
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL0_STOP_SEL, trig_index + _CYHAL_TCPWM_TRIGGER_INPUTS_IDX_OFFSET);
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL1_STOP_EDGE_Msk;
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL1_STOP_EDGE, type);
            break;
        case CYHAL_TCPWM_INPUT_RELOAD:
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL0_RELOAD_SEL_Msk;
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL0_RELOAD_SEL, trig_index + _CYHAL_TCPWM_TRIGGER_INPUTS_IDX_OFFSET);
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL1_RELOAD_EDGE_Msk;
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL1_RELOAD_EDGE, type);
            break;
        case CYHAL_TCPWM_INPUT_COUNT:
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL0_COUNT_SEL_Msk;
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL0_COUNT_SEL, trig_index + _CYHAL_TCPWM_TRIGGER_INPUTS_IDX_OFFSET);
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL1_COUNT_EDGE_Msk;
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL1_COUNT_EDGE, type);
            break;
        case CYHAL_TCPWM_INPUT_CAPTURE:
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL0_CAPTURE_SEL_Msk;
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL0_CAPTURE_SEL, trig_index + _CYHAL_TCPWM_TRIGGER_INPUTS_IDX_OFFSET);
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL1_CAPTURE_EDGE_Msk;
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL1_CAPTURE_EDGE, type);
            break;
       default:
            return CYHAL_TCPWM_RSLT_ERR_BAD_ARGUMENT;
    }

#elif (CY_IP_MXTCPWM_VERSION == 2U)
    // Clear appropriate trigger reg fields and set input index and edge
    // trigger type.
    // Note: Cy_TCPWM_InputTriggerSetup assumes channel indices for block 0 are
    // 0-255 and block 1 are 256-511.
    switch(signal)
    {
        case CYHAL_TCPWM_INPUT_START:
            TCPWM_GRP_CNT_TR_IN_SEL1(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_SEL1_START_SEL_Msk;
            TCPWM_GRP_CNT_TR_IN_EDGE_SEL(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_EDGE_SEL_START_EDGE_Msk;
            Cy_TCPWM_InputTriggerSetup(obj->base, block == 0 ? chnl : chnl + 256, CY_TCPWM_INPUT_TR_START, type, trig_index + _CYHAL_TCPWM_TRIGGER_INPUTS_IDX_OFFSET);
            break;
        case CYHAL_TCPWM_INPUT_STOP:
            TCPWM_GRP_CNT_TR_IN_SEL0(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_SEL0_STOP_SEL_Msk;
            TCPWM_GRP_CNT_TR_IN_EDGE_SEL(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_EDGE_SEL_STOP_EDGE_Msk;
            Cy_TCPWM_InputTriggerSetup(obj->base, block == 0 ? chnl : chnl + 256, CY_TCPWM_INPUT_TR_STOP_OR_KILL, type, trig_index + _CYHAL_TCPWM_TRIGGER_INPUTS_IDX_OFFSET);
            break;
        case CYHAL_TCPWM_INPUT_RELOAD:
            TCPWM_GRP_CNT_TR_IN_SEL0(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_SEL0_RELOAD_SEL_Msk;
            TCPWM_GRP_CNT_TR_IN_EDGE_SEL(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_EDGE_SEL_RELOAD_EDGE_Msk;
            Cy_TCPWM_InputTriggerSetup(obj->base, block == 0 ? chnl : chnl + 256, CY_TCPWM_INPUT_TR_RELOAD_OR_INDEX, type, trig_index + _CYHAL_TCPWM_TRIGGER_INPUTS_IDX_OFFSET);
            break;
        case CYHAL_TCPWM_INPUT_COUNT:
            TCPWM_GRP_CNT_TR_IN_SEL0(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_SEL0_COUNT_SEL_Msk;
            TCPWM_GRP_CNT_TR_IN_EDGE_SEL(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_EDGE_SEL_COUNT_EDGE_Msk;
            Cy_TCPWM_InputTriggerSetup(obj->base, block == 0 ? chnl : chnl + 256, CY_TCPWM_INPUT_TR_COUNT, type, trig_index + _CYHAL_TCPWM_TRIGGER_INPUTS_IDX_OFFSET);
            break;
        case CYHAL_TCPWM_INPUT_CAPTURE:
            TCPWM_GRP_CNT_TR_IN_SEL0(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_SEL0_CAPTURE0_SEL_Msk;
            TCPWM_GRP_CNT_TR_IN_EDGE_SEL(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_EDGE_SEL_CAPTURE0_EDGE_Msk;
            Cy_TCPWM_InputTriggerSetup(obj->base, block == 0 ? chnl : chnl + 256, CY_TCPWM_INPUT_TR_CAPTURE0, type, trig_index + _CYHAL_TCPWM_TRIGGER_INPUTS_IDX_OFFSET);
            break;
       default:
            return CYHAL_TCPWM_RSLT_ERR_BAD_ARGUMENT;
    }
#else
#error Unrecognized TCPWM version
#endif

    cyhal_dest_t dest = _cyhal_tpwm_calculate_dest(block, trig_index);

    rslt = _cyhal_connect_signal(source, dest, signal_type);

    if (rslt == CY_RSLT_SUCCESS)
    {
        _CYHAL_INPUT_TRIGGERS_USED[obj->resource.block_num][trig_index / 32] |= 1 << trig_index;
        obj->inputs[(uint32_t)signal] = source;
    }

    return rslt;
#else
    CY_UNUSED_PARAMETER(obj);
    CY_UNUSED_PARAMETER(source);
    CY_UNUSED_PARAMETER(signal);
    CY_UNUSED_PARAMETER(type);

    return CYHAL_TCPWM_RSLT_ERR_BAD_ARGUMENT;
#endif /* defined(CY_IP_M0S8PERI_TR) || defined(CY_IP_MXPERI_TR) */
}

cy_rslt_t _cyhal_tcpwm_enable_output(cyhal_tcpwm_t *obj, cyhal_tcpwm_output_t signal, cyhal_source_t *source)
{
#if defined(CY_IP_M0S8PERI_TR) || defined(CY_IP_MXPERI_TR)
    const uint8_t block = obj->resource.block_num;
    const uint8_t chnl = obj->resource.channel_num;

// Note that triggers are always generated for TCPWMv1 so this just returns the proper source signal
#if (CY_IP_MXTCPWM_VERSION == 1U) || (CY_IP_M0S8TCPWM_VERSION == 2)
    CY_ASSERT(block < 2);

    *source = _cyhal_tcpwm_calculate_source(block, chnl, signal);

    return CY_RSLT_SUCCESS;
#elif (CY_IP_MXTCPWM_VERSION == 2U)
    uint8_t event_type = _cyhal_tcpwm_convert_output_t(signal);

    // All channels in _CYHAL_OUTPUT_TRIGGERS_USED are just ordered
    // sequentially, not ordered by blocks.
    uint8_t out_trig_idx = block == 0 ? chnl : chnl + TCPWM_GRP_NR0_GRP_GRP_CNT_NR;

    // Find free trigger, if any
    int8_t free_trig = -1;
    uint32_t saved_intr_status = cyhal_system_critical_section_enter();
    if(!(_CYHAL_OUTPUT_TRIGGERS_USED[out_trig_idx] & 1))
    {
        // Output trigger idx 0 is free
        free_trig = 0;
        _CYHAL_OUTPUT_TRIGGERS_USED[out_trig_idx] |= 1;
    }
    else if(!(_CYHAL_OUTPUT_TRIGGERS_USED[out_trig_idx] & 2))
    {
        // Output trigger idx 1 is free
        free_trig = 1;
        _CYHAL_OUTPUT_TRIGGERS_USED[out_trig_idx] |= 2;
    }
    cyhal_system_critical_section_exit(saved_intr_status);

    // Configure trigger out registers
    if(free_trig == 0)
    {
        TCPWM_GRP_CNT_TR_OUT_SEL(obj->base, block, chnl) &= TCPWM_GRP_CNT_V2_TR_OUT_SEL_OUT0_Msk;
        TCPWM_GRP_CNT_TR_OUT_SEL(obj->base, block, chnl) |= _VAL2FLD(TCPWM_GRP_CNT_V2_TR_OUT_SEL_OUT0, event_type);
    }
    else if(free_trig == 1)
    {
        TCPWM_GRP_CNT_TR_OUT_SEL(obj->base, block, chnl) &= TCPWM_GRP_CNT_V2_TR_OUT_SEL_OUT1_Msk;
        TCPWM_GRP_CNT_TR_OUT_SEL(obj->base, block, chnl) |= _VAL2FLD(TCPWM_GRP_CNT_V2_TR_OUT_SEL_OUT1, event_type);
    }
    else
        return CYHAL_TCPWM_RSLT_ERR_NO_FREE_OUTPUT_SIGNALS;

    *source = _cyhal_tcpwm_calculate_source(out_trig_idx, free_trig);

    return CY_RSLT_SUCCESS;
#else
#error Unrecognized TCPWM version
#endif
#else
    CY_UNUSED_PARAMETER(obj);
    CY_UNUSED_PARAMETER(signal);
    CY_UNUSED_PARAMETER(source);
    return CYHAL_TCPWM_RSLT_ERR_NO_FREE_OUTPUT_SIGNALS;
#endif /* defined(CY_IP_M0S8PERI_TR) || defined(CY_IP_MXPERI_TR) */
}

cy_rslt_t _cyhal_tcpwm_disconnect_digital(cyhal_tcpwm_t *obj, cyhal_source_t source, cyhal_tcpwm_input_t signal)
{
#if defined(CY_IP_M0S8PERI_TR) || defined(CY_IP_MXPERI_TR)
    const uint8_t block = obj->resource.block_num;
    const uint8_t chnl = obj->resource.channel_num;
    uint8_t trig_index;

#if (CY_IP_MXTCPWM_VERSION == 1U) || (CY_IP_M0S8TCPWM_VERSION == 2)
    switch (signal)
    {
        // Grab trig_index then clear/reset to default (CY_TCPWM_INPUT_LEVEL is
        // default) appropriate ctrl reg fields
        case CYHAL_TCPWM_INPUT_START:
            trig_index = _FLD2VAL(TCPWM_CNT_TR_CTRL0_START_SEL, TCPWM_CNT_TR_CTRL0(obj->base, chnl));
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL0_START_SEL_Msk;
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL1_START_EDGE, CY_TCPWM_INPUT_LEVEL);
            break;
        case CYHAL_TCPWM_INPUT_STOP:
            trig_index = _FLD2VAL(TCPWM_CNT_TR_CTRL0_STOP_SEL, TCPWM_CNT_TR_CTRL0(obj->base, chnl));
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL0_STOP_SEL_Msk;
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL1_STOP_EDGE, CY_TCPWM_INPUT_LEVEL);
            break;
        case CYHAL_TCPWM_INPUT_RELOAD:
            trig_index = _FLD2VAL(TCPWM_CNT_TR_CTRL0_RELOAD_SEL, TCPWM_CNT_TR_CTRL0(obj->base, chnl));
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL0_RELOAD_SEL_Msk;
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL1_RELOAD_EDGE, CY_TCPWM_INPUT_LEVEL);
            break;
        case CYHAL_TCPWM_INPUT_COUNT:
            trig_index = _FLD2VAL(TCPWM_CNT_TR_CTRL0_COUNT_SEL, TCPWM_CNT_TR_CTRL0(obj->base, chnl));
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL0_COUNT_SEL_Msk;
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL1_COUNT_EDGE, CY_TCPWM_INPUT_LEVEL);
            break;
        case CYHAL_TCPWM_INPUT_CAPTURE:
            trig_index = _FLD2VAL(TCPWM_CNT_TR_CTRL0_CAPTURE_SEL, TCPWM_CNT_TR_CTRL0(obj->base, chnl));
            TCPWM_CNT_TR_CTRL0(obj->base, chnl) &= ~TCPWM_CNT_TR_CTRL0_CAPTURE_SEL_Msk;
            TCPWM_CNT_TR_CTRL1(obj->base, chnl) |= _VAL2FLD(TCPWM_CNT_TR_CTRL1_CAPTURE_EDGE, CY_TCPWM_INPUT_LEVEL);
            break;
       default:
            return CYHAL_TCPWM_RSLT_ERR_BAD_ARGUMENT;
    }
#elif (CY_IP_MXTCPWM_VERSION == 2U)
    uint32_t grp = TCPWM_GRP_CNT_GET_GRP(chnl);
    switch(signal)
    {
        case CYHAL_TCPWM_INPUT_START:
            trig_index = _FLD2VAL(TCPWM_GRP_CNT_V2_TR_IN_SEL1_START_SEL, TCPWM_GRP_CNT_TR_IN_SEL1(obj->base, grp, chnl));
            TCPWM_GRP_CNT_TR_IN_SEL1(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_SEL1_START_SEL_Msk;
            TCPWM_GRP_CNT_TR_IN_EDGE_SEL(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_EDGE_SEL_START_EDGE_Msk;
            break;
        case CYHAL_TCPWM_INPUT_STOP:
            trig_index = _FLD2VAL(TCPWM_GRP_CNT_V2_TR_IN_SEL0_STOP_SEL, TCPWM_GRP_CNT_TR_IN_SEL0(obj->base, grp, chnl));
            TCPWM_GRP_CNT_TR_IN_SEL0(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_SEL0_STOP_SEL_Msk;
            TCPWM_GRP_CNT_TR_IN_EDGE_SEL(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_EDGE_SEL_STOP_EDGE_Msk;
            break;
        case CYHAL_TCPWM_INPUT_RELOAD:
            trig_index = _FLD2VAL(TCPWM_GRP_CNT_V2_TR_IN_SEL0_RELOAD_SEL, TCPWM_GRP_CNT_TR_IN_SEL0(obj->base, grp, chnl));
            TCPWM_GRP_CNT_TR_IN_SEL0(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_SEL0_RELOAD_SEL_Msk;
            TCPWM_GRP_CNT_TR_IN_EDGE_SEL(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_EDGE_SEL_RELOAD_EDGE_Msk;
            break;
        case CYHAL_TCPWM_INPUT_COUNT:
            trig_index = _FLD2VAL(TCPWM_GRP_CNT_V2_TR_IN_SEL0_COUNT_SEL, TCPWM_GRP_CNT_TR_IN_SEL0(obj->base, grp, chnl));
            TCPWM_GRP_CNT_TR_IN_SEL0(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_SEL0_COUNT_SEL_Msk;
            TCPWM_GRP_CNT_TR_IN_EDGE_SEL(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_EDGE_SEL_COUNT_EDGE_Msk;
            break;
        case CYHAL_TCPWM_INPUT_CAPTURE:
            trig_index = _FLD2VAL(TCPWM_GRP_CNT_V2_TR_IN_SEL0_CAPTURE0_SEL, TCPWM_GRP_CNT_TR_IN_SEL0(obj->base, grp, chnl));
            TCPWM_GRP_CNT_TR_IN_SEL0(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_SEL0_CAPTURE0_SEL_Msk;
            TCPWM_GRP_CNT_TR_IN_EDGE_SEL(obj->base, block, chnl) &= ~TCPWM_GRP_CNT_V2_TR_IN_EDGE_SEL_CAPTURE0_EDGE_Msk;
            break;
       default:
            return CYHAL_TCPWM_RSLT_ERR_BAD_ARGUMENT;
    }
#else
#error Unrecognized TCPWM version
#endif

    trig_index -= _CYHAL_TCPWM_TRIGGER_INPUTS_IDX_OFFSET;

    cyhal_dest_t dest = _cyhal_tpwm_calculate_dest(block, trig_index);

    _CYHAL_INPUT_TRIGGERS_USED[block][trig_index / 32] &= ~(1 << trig_index);
    cy_rslt_t rslt = _cyhal_disconnect_signal(source, dest);
    if (CY_RSLT_SUCCESS == rslt)
    {
        obj->inputs[(uint32_t)signal] = CYHAL_TRIGGER_CPUSS_ZERO;
    }
    return rslt;
#else
    CY_UNUSED_PARAMETER(obj);
    CY_UNUSED_PARAMETER(signal);
    CY_UNUSED_PARAMETER(source);
    return CYHAL_TCPWM_RSLT_ERR_BAD_ARGUMENT;
#endif /* defined(CY_IP_M0S8PERI_TR) || defined(CY_IP_MXPERI_TR) */
}

cy_rslt_t _cyhal_tcpwm_disable_output(cyhal_tcpwm_t *obj, cyhal_tcpwm_output_t signal)
{
#if defined(CY_IP_M0S8PERI_TR) || defined(CY_IP_MXPERI_TR)
// Triggers are always generated for TCPWMv1 so this is a noop.
#if (CY_IP_MXTCPWM_VERSION == 1U) || (CY_IP_M0S8TCPWM_VERSION == 2)
    CY_UNUSED_PARAMETER(obj);
    CY_UNUSED_PARAMETER(signal);

    return CY_RSLT_SUCCESS;
#elif (CY_IP_MXTCPWM_VERSION == 2U)
    const uint8_t block = obj->resource.block_num;
    const uint8_t chnl = obj->resource.channel_num;

    uint8_t event_type = _cyhal_tcpwm_convert_output_t(signal);

    // All channels in _CYHAL_OUTPUT_TRIGGERS_USED are just ordered
    // sequentially, not ordered by blocks.
    uint8_t trig_index = block == 0 ? chnl : chnl + TCPWM_GRP_NR0_GRP_GRP_CNT_NR;

    if(_FLD2VAL(TCPWM_GRP_CNT_V2_TR_OUT_SEL_OUT0, TCPWM_GRP_CNT_TR_OUT_SEL(obj->base, block, chnl)) == event_type)
    {
        // Disable output trigger idx 0
        TCPWM_GRP_CNT_TR_OUT_SEL(obj->base, block, chnl) &= TCPWM_GRP_CNT_V2_TR_OUT_SEL_OUT0_Msk;
        _CYHAL_OUTPUT_TRIGGERS_USED[trig_index] &= ~1;
    }
    else if(_FLD2VAL(TCPWM_GRP_CNT_V2_TR_OUT_SEL_OUT1, TCPWM_GRP_CNT_TR_OUT_SEL(obj->base, block, chnl)) == event_type)
    {
        // Disable output trigger idx 1
        TCPWM_GRP_CNT_TR_OUT_SEL(obj->base, block, chnl) &= TCPWM_GRP_CNT_V2_TR_OUT_SEL_OUT1_Msk;
        _CYHAL_OUTPUT_TRIGGERS_USED[trig_index] &= ~2;
    }
    else
        return CYHAL_TCPWM_RSLT_ERR_BAD_ARGUMENT;

    return CY_RSLT_SUCCESS;
#else
#error Unrecognized TCPWM version
#endif
#else
    CY_UNUSED_PARAMETER(obj);
    CY_UNUSED_PARAMETER(signal);

    return CYHAL_TCPWM_RSLT_ERR_BAD_ARGUMENT;
#endif /* defined(CY_IP_M0S8PERI_TR) || defined(CY_IP_MXPERI_TR) */
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXTCPWM_INSTANCES) */
