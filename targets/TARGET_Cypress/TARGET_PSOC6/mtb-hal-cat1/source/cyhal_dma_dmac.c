/***************************************************************************//**
* \file cyhal_dma_dmac.c
*
* \brief
* Implements a high level interface for interacting with the Cypress DMAC.
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

#include "cyhal_dma.h"
#include "cyhal_dma_dmac.h"
#include "cyhal_dma_impl.h"
#include "cyhal_hwmgr.h"
#include "cyhal_hwmgr_impl.h"
#include "cyhal_interconnect.h"
#include "cyhal_syspm.h"
#include "cyhal_utils.h"
#include "cyhal_triggers.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(CY_IP_M4CPUSS_DMAC) || defined(CY_IP_M0S8CPUSSV3_DMAC)

#if defined(CY_IP_M4CPUSS_DMAC)
#define DMAC_IRQ_NUM            (cpuss_interrupts_dmac_0_IRQn)
#define GET_RESOURCE_DATA(x)    (x.dmac)
#elif defined(CY_IP_M0S8CPUSSV3_DMAC)
#define DMAC_IRQ_NUM            (cpuss_interrupt_dma_IRQn)
#define GET_RESOURCE_DATA(x)    (x)
#define CY_TRIGGER_TWO_CYCLES   (2)
#endif

static cyhal_dma_t* _cyhal_dma_dmac_config_structs[CPUSS_DMAC_CH_NR];

/** Default dmac descriptor config */
static const cy_stc_dmac_descriptor_config_t _cyhal_dma_dmac_default_descriptor_config =
{
    .srcAddress = 0,
    .dstAddress = 0,
    .dataSize = CY_DMAC_WORD,
    .dstTransferSize = CY_DMAC_TRANSFER_SIZE_DATA,
    .srcTransferSize = CY_DMAC_TRANSFER_SIZE_DATA,
    .retrigger = CY_DMAC_RETRIG_IM,
#if defined(CY_IP_M4CPUSS_DMAC)
    .interruptType = CY_DMAC_DESCR,
    .triggerOutType = CY_DMAC_DESCR_CHAIN,
    .channelState = CY_DMAC_CHANNEL_ENABLED,
    .triggerInType = CY_DMAC_DESCR,
    .dataPrefetch = false,
    .descriptorType = CY_DMAC_1D_TRANSFER,
    .srcXincrement = 1U,
    .dstXincrement = 1U,
    .xCount = 1UL,
    .srcYincrement = 0U,
    .dstYincrement = 0U,
    .yCount = 1UL,
    .nextDescriptor = 0,
#elif defined(CY_IP_M0S8CPUSSV3_DMAC)
    .triggerType = CY_DMAC_SINGLE_DESCR,
    .dataCount = 1,
    .dstAddrIncrement = true,
    .srcAddrIncrement = true,
    .interrupt = true,
    .preemptable = true,
    .flipping = false,
#endif
};

/** Default dmac channel config */
static const cy_stc_dmac_channel_config_t _cyhal_dma_dmac_default_channel_config =
{
    .priority = 1,
    .enable = false,
#if defined(CY_IP_M4CPUSS_DMAC)
    .bufferable = false,
    .descriptor = 0,
#elif defined(CY_IP_M0S8CPUSSV3_DMAC)
    .descriptor = CY_DMAC_DESCRIPTOR_PING,
#endif
};

static bool _cyhal_dma_dmac_pm_callback(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void* callback_arg);

static cyhal_syspm_callback_data_t _cyhal_dma_dmac_pm_callback_args = {
    .callback = &_cyhal_dma_dmac_pm_callback,
    .states = (cyhal_syspm_callback_state_t)(CYHAL_SYSPM_CB_CPU_DEEPSLEEP | CYHAL_SYSPM_CB_SYSTEM_HIBERNATE),
    .next = NULL,
    .args = NULL,
    .ignore_modes = CYHAL_SYSPM_BEFORE_TRANSITION,
};
static bool _cyhal_dma_dmac_pm_transition_pending = false;
static bool _cyhal_dma_dmac_has_enabled(void)
{
    for (uint8_t i = 0; i <  CPUSS_DMAC_CH_NR; i++)
        if (_cyhal_dma_dmac_config_structs[i])
            return true;
    return false;
}

static bool _cyhal_dma_dmac_pm_callback(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void* callback_arg)
{
    CY_UNUSED_PARAMETER(state);
    CY_UNUSED_PARAMETER(callback_arg);
    bool block_transition = false;
    switch(mode)
    {
        case CYHAL_SYSPM_CHECK_READY:
            for (uint8_t i = 0; i <  CPUSS_DMAC_CH_NR && !block_transition; i++)
            {
                block_transition |= (_cyhal_dma_dmac_config_structs[i] != NULL) && _cyhal_dma_dmac_is_busy(_cyhal_dma_dmac_config_structs[i]);
            }
            _cyhal_dma_dmac_pm_transition_pending = !block_transition;
            break;
        case CYHAL_SYSPM_CHECK_FAIL:
        case CYHAL_SYSPM_AFTER_TRANSITION:
            _cyhal_dma_dmac_pm_transition_pending = false;
            break;
        default:
            break;
    }
    return _cyhal_dma_dmac_pm_transition_pending;
}

/** Sets the dmac configuration struct */
static inline void _cyhal_dma_dmac_set_obj(cyhal_dma_t *obj)
{
    _cyhal_dma_dmac_config_structs[obj->resource.block_num * CPUSS_DMAC_CH_NR + obj->resource.channel_num] = obj;
}

/** Zeros the dmac configuration struct */
static inline void _cyhal_dma_dmac_free_obj(cyhal_dma_t *obj)
{
    _cyhal_dma_dmac_config_structs[obj->resource.block_num * CPUSS_DMAC_CH_NR + obj->resource.channel_num] = NULL;
}

/** Gets the dmac configuration struct from block and channel */
static inline cyhal_dma_t* _cyhal_dma_dmac_get_obj(uint8_t block, uint8_t channel)
{
    return _cyhal_dma_dmac_config_structs[block * CPUSS_DMAC_CH_NR + channel];
}

/** Gets the dmac block number from irq number */
/** This should never be called from a non-dma IRQn */
static inline uint8_t _cyhal_dma_dmac_get_block_from_irqn(IRQn_Type irqn)
{
    CY_UNUSED_PARAMETER(irqn);
    /* Since there is only one dmac block this function always returns 0. diff
     * is calculated here only to verify that this was called from a valid
     * IRQn. */

#if defined(CY_IP_M4CPUSS_DMAC)
    CY_ASSERT(irqn >= DMAC_IRQ_NUM && irqn < DMAC_IRQ_NUM + (IRQn_Type)CPUSS_DMAC_CH_NR);
#elif defined(CY_IP_M0S8CPUSSV3_DMAC)
    CY_ASSERT(irqn >= DMAC_IRQ_NUM && irqn < DMAC_IRQ_NUM + ((int)CY_IP_M0S8CPUSSV3_DMAC_INSTANCES));
#endif
    return 0;
}

#if defined(CY_IP_M4CPUSS_DMAC)
/** Gets the dmac channel number from irq number */
/** This should never be called from a non-dma IRQn */
static inline uint8_t _cyhal_dma_dmac_get_channel_from_irqn(IRQn_Type irqn)
{
    uint8_t diff = irqn - DMAC_IRQ_NUM;

    CY_ASSERT(diff < CPUSS_DMAC_CH_NR);

    return diff;
}
#endif

/** Gets the irqn corresponding to a particular cyhal_dma_t config struct */
static inline IRQn_Type _cyhal_dma_dmac_get_irqn(cyhal_dma_t *obj)
{
    return (IRQn_Type)((uint8_t)DMAC_IRQ_NUM + (obj->resource.block_num * CPUSS_DMAC_CH_NR + obj->resource.channel_num));
}

/** Gets the dmac base pointer from block number */
static inline DMAC_Type* _cyhal_dma_dmac_get_base(uint8_t block_num)
{
    CY_UNUSED_PARAMETER(block_num);
    return DMAC;
}

/** Uses tables provided as part of the hal interconnect driver to determine mux
 * trigger group and mux trigger index and then construct the trigger line
 * input parameter to Cy_TrigMux_SwTrigger. */
static inline uint32_t _cyhal_dma_dmac_get_trigger_line(uint8_t block_num, uint8_t channel_num)
{
    /* cyhal_dest_t triggers are guaranteed to be sorted by trigger type, block
     * num, then channel num, therefore, we can just directly find the proper
     * trigger by calculating an offset. */
    cyhal_dest_t trigger = (cyhal_dest_t)(CYHAL_TRIGGER_CPUSS_DMAC_TR_IN0 + (block_num * CPUSS_DMAC_CH_NR) + channel_num);

    /* One to one triggers have bit 8 set in cyhal_dest_to_mux but
     * Cy_TrigMux_SwTrigger wants the trigger group field to have bit 5 set to
     * denote one to one triggers. */
    uint8_t trig_group = cyhal_dest_to_mux[trigger];
    /* If hal one to one triggers bit is set: mask it out and set pdl one to
     * one bit */
    if(trig_group & _CYHAL_DMA_TRIGGERS_1TO1_MASK)
        trig_group = (trig_group & ~_CYHAL_DMA_TRIGGERS_1TO1_MASK) | _CYHAL_DMA_PDL_TRIGGERS_1TO1_MASK;

    /* Construct trigger line which consists of three fields packed into a
     * uint32_t:
     * Bits   30: Input/output bit. Set to 1 for output.
     * Bits 12-8: Trigger group selection.
     * Bits  7-0: Select the output trigger number in the trigger group. */
#if defined(CY_IP_M4CPUSS_DMAC)
    return PERI_TR_CMD_OUT_SEL_Msk | trig_group << 8 | cyhal_mux_dest_index[trigger];
#elif defined(CY_IP_M0S8CPUSSV3_DMAC)
    return PERI_TR_CTL_TR_OUT_Msk | trig_group << 8 | cyhal_mux_dest_index[trigger];
#endif
}

#if defined(CY_IP_M4CPUSS_DMAC)
/** Convert PDL interrupt cause to hal dma event */
static inline cyhal_dma_event_t _cyhal_dma_dmac_convert_interrupt_cause(uint32_t cause)
{
    switch(cause)
    {
        case CY_DMAC_INTR_COMPLETION:
            return CYHAL_DMA_TRANSFER_COMPLETE;
        case CY_DMAC_INTR_SRC_BUS_ERROR:
            return CYHAL_DMA_SRC_BUS_ERROR;
        case CY_DMAC_INTR_DST_BUS_ERROR:
            return CYHAL_DMA_DST_BUS_ERROR;
        case CY_DMAC_INTR_SRC_MISAL:
            return CYHAL_DMA_SRC_MISAL;
        case CY_DMAC_INTR_DST_MISAL:
            return CYHAL_DMA_DST_MISAL;
        case CY_DMAC_INTR_CURR_PTR_NULL:
            return CYHAL_DMA_CURR_PTR_NULL;
        case CY_DMAC_INTR_ACTIVE_CH_DISABLED:
            return CYHAL_DMA_ACTIVE_CH_DISABLED;
        case CY_DMAC_INTR_DESCR_BUS_ERROR:
            return CYHAL_DMA_DESCR_BUS_ERROR;
        default:
            return CYHAL_DMA_NO_INTR;
    }
}
#endif

/** DMAC irq handler */
static void _cyhal_dma_dmac_irq_handler(void)
{
    /* Use irqn to get appropriate config structure */
    uint8_t block = _cyhal_dma_dmac_get_block_from_irqn(_CYHAL_UTILS_GET_CURRENT_IRQN());
    DMAC_Type* base = _cyhal_dma_dmac_get_base(block);
#if defined(CY_IP_M4CPUSS_DMAC)
    uint8_t channel = _cyhal_dma_dmac_get_channel_from_irqn(_CYHAL_UTILS_GET_CURRENT_IRQN());
    cyhal_dma_t *obj = _cyhal_dma_dmac_get_obj(block, channel);

    /* Get interrupt type and call users event callback if they have enabled that event */
    uint32_t cause = Cy_DMAC_Channel_GetInterruptStatusMasked(base, channel);
    cyhal_dma_event_t event_type = _cyhal_dma_dmac_convert_interrupt_cause(cause);
    uint32_t events_to_callback = event_type & obj->irq_cause;
    if(obj->callback_data.callback != NULL && events_to_callback)
    {
        ((cyhal_dma_event_callback_t)obj->callback_data.callback)(obj->callback_data.callback_arg, (cyhal_dma_event_t)events_to_callback);
    }

    /* Clear all interrupts */
    Cy_DMAC_Channel_ClearInterrupt(base, channel, CY_DMAC_INTR_MASK);
#elif defined(CY_IP_M0S8CPUSSV3_DMAC)
    uint32_t channels = Cy_DMAC_GetInterruptStatusMasked(base);
    for(int i = 0 ; ((uint32_t)(1 << i)) <= channels ; i++)
    {
        cyhal_dma_t *obj = _cyhal_dma_dmac_get_obj(block, i);
        if (obj != NULL)
        {
            if ((channels & (1 << i)) != 0 && obj->callback_data.callback != NULL)
            {
                ((cyhal_dma_event_callback_t)obj->callback_data.callback)(obj->callback_data.callback_arg, CYHAL_DMA_TRANSFER_COMPLETE);
            }
        }
    }
    Cy_DMAC_ClearInterrupt(_cyhal_dma_dmac_get_base(block), channels);
#endif
}

static cyhal_source_t _cyhal_dma_dmac_get_src(uint8_t block_num, uint8_t channel_num)
{
    return (cyhal_source_t)(CYHAL_TRIGGER_CPUSS_DMAC_TR_OUT0 + (block_num * CPUSS_DMAC_CH_NR) + channel_num);
}

static cyhal_dest_t _cyhal_dma_dmac_get_dest(uint8_t block_num, uint8_t channel_num)
{
    return (cyhal_dest_t)(CYHAL_TRIGGER_CPUSS_DMAC_TR_IN0 + (block_num * CPUSS_DMAC_CH_NR) + channel_num);
}

cy_rslt_t _cyhal_dma_dmac_init(cyhal_dma_t *obj, cyhal_source_t *src, cyhal_dest_t *dest, uint8_t priority)
{
    if(!CY_DMAC_IS_PRIORITY_VALID(priority))
        return CYHAL_DMA_RSLT_ERR_INVALID_PRIORITY;

    if (_cyhal_dma_dmac_pm_transition_pending)
    {
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;
    }

    cy_rslt_t rslt = _cyhal_hwmgr_allocate_with_connection(
        CYHAL_RSC_DMA, src, dest, _cyhal_dma_dmac_get_src, _cyhal_dma_dmac_get_dest, &obj->resource);
    if(rslt != CY_RSLT_SUCCESS)
        return rslt;

    /* Setup descriptor and channel configs */
    GET_RESOURCE_DATA(obj->descriptor_config) = _cyhal_dma_dmac_default_descriptor_config;
    GET_RESOURCE_DATA(obj->channel_config) = _cyhal_dma_dmac_default_channel_config;
#if defined(CY_IP_M4CPUSS_DMAC)
    GET_RESOURCE_DATA(obj->channel_config).descriptor = GET_RESOURCE_DATA(&obj->descriptor);
#elif defined(CY_IP_M0S8CPUSSV3_DMAC)
    obj->descriptor = obj->channel_config.descriptor;
#endif
    GET_RESOURCE_DATA(obj->channel_config).priority = priority;

    if (!_cyhal_dma_dmac_has_enabled())
    {
        _cyhal_syspm_register_peripheral_callback(&_cyhal_dma_dmac_pm_callback_args);
    }

    _cyhal_dma_dmac_set_obj(obj);

    return CY_RSLT_SUCCESS;
}

void _cyhal_dma_dmac_free(cyhal_dma_t *obj)
{
    DMAC_Type* base = _cyhal_dma_dmac_get_base(obj->resource.block_num);
#if defined(CY_IP_M4CPUSS_DMAC)
    Cy_DMAC_Descriptor_DeInit(GET_RESOURCE_DATA(&obj->descriptor));
#elif defined(CY_IP_M0S8CPUSSV3_DMAC)
    Cy_DMAC_Descriptor_DeInit(base, obj->resource.channel_num, obj->descriptor);
#endif

    Cy_DMAC_Channel_DeInit(base, obj->resource.channel_num);

    NVIC_DisableIRQ(_cyhal_dma_dmac_get_irqn(obj));

    _cyhal_dma_dmac_free_obj(obj);

    if (!_cyhal_dma_dmac_has_enabled())
    {
        _cyhal_syspm_unregister_peripheral_callback(&_cyhal_dma_dmac_pm_callback_args);
        _cyhal_dma_dmac_pm_transition_pending = false;
    }
}

/* Initialize descriptor, initialize channel, enable channel, enable channel
 * interrupt, and enable DMAC controller */
cy_rslt_t _cyhal_dma_dmac_configure(cyhal_dma_t *obj, const cyhal_dma_cfg_t *cfg)
{
    /* Do not reconfigure if transfer is pending/active already */
    if(_cyhal_dma_dmac_is_busy(obj))
        return CYHAL_DMA_RSLT_ERR_CHANNEL_BUSY;

    GET_RESOURCE_DATA(obj->descriptor_config).srcAddress = (void*)cfg->src_addr;
    GET_RESOURCE_DATA(obj->descriptor_config).dstAddress = (void*)cfg->dst_addr;

    if(cfg->transfer_width == 8)
        GET_RESOURCE_DATA(obj->descriptor_config).dataSize = CY_DMAC_BYTE;
    else if(cfg->transfer_width == 16)
        GET_RESOURCE_DATA(obj->descriptor_config).dataSize = CY_DMAC_HALFWORD;
    else if(cfg->transfer_width == 32)
        GET_RESOURCE_DATA(obj->descriptor_config).dataSize = CY_DMAC_WORD;
    else
        return CYHAL_DMA_RSLT_ERR_INVALID_TRANSFER_WIDTH;

    /* By default, transfer what the user set for dataSize. However, if transfering between memory
     * and a peripheral, make sure the peripheral access is using words. */
    GET_RESOURCE_DATA(obj->descriptor_config).srcTransferSize =
        GET_RESOURCE_DATA(obj->descriptor_config).dstTransferSize = CY_DMAC_TRANSFER_SIZE_DATA;
    if (obj->direction == CYHAL_DMA_DIRECTION_PERIPH2MEM)
        GET_RESOURCE_DATA(obj->descriptor_config).srcTransferSize = CY_DMAC_TRANSFER_SIZE_WORD;
    else if (obj->direction == CYHAL_DMA_DIRECTION_MEM2PERIPH)
        GET_RESOURCE_DATA(obj->descriptor_config).dstTransferSize = CY_DMAC_TRANSFER_SIZE_WORD;

#if defined(CY_IP_M4CPUSS_DMAC)
    GET_RESOURCE_DATA(obj->descriptor_config).nextDescriptor = GET_RESOURCE_DATA(&obj->descriptor);

    /* Setup 2D transfer if burst_size is being used otherwise set up 1D
     * transfer */
    if(cfg->burst_size != 0)
    {
        /* Length must be a multiple of burst_size */
        if(cfg->length % cfg->burst_size != 0)
            return CYHAL_DMA_RSLT_ERR_INVALID_BURST_SIZE;

        GET_RESOURCE_DATA(obj->descriptor_config).descriptorType = CY_DMAC_2D_TRANSFER;
        GET_RESOURCE_DATA(obj->descriptor_config).xCount = cfg->burst_size;
        GET_RESOURCE_DATA(obj->descriptor_config).yCount = cfg->length / cfg->burst_size;
        GET_RESOURCE_DATA(obj->descriptor_config).srcXincrement = cfg->src_increment;
        GET_RESOURCE_DATA(obj->descriptor_config).dstXincrement = cfg->dst_increment;
        GET_RESOURCE_DATA(obj->descriptor_config).srcYincrement = cfg->src_increment * cfg->burst_size;
        GET_RESOURCE_DATA(obj->descriptor_config).dstYincrement = cfg->dst_increment * cfg->burst_size;

        /* If burst action, configure trigger and interrupt actions */
        if(cfg->action == CYHAL_DMA_TRANSFER_BURST)
        {
            GET_RESOURCE_DATA(obj->descriptor_config).interruptType = CY_DMAC_X_LOOP;
            GET_RESOURCE_DATA(obj->descriptor_config).triggerInType = CY_DMAC_X_LOOP;
        }
    }
    else
    {
        GET_RESOURCE_DATA(obj->descriptor_config).descriptorType = CY_DMAC_1D_TRANSFER;
        GET_RESOURCE_DATA(obj->descriptor_config).xCount = cfg->length;
        GET_RESOURCE_DATA(obj->descriptor_config).srcXincrement = cfg->src_increment;
        GET_RESOURCE_DATA(obj->descriptor_config).dstXincrement = cfg->dst_increment;

        GET_RESOURCE_DATA(obj->descriptor_config).interruptType = CY_DMAC_DESCR;
        GET_RESOURCE_DATA(obj->descriptor_config).triggerInType = CY_DMAC_DESCR;
    }
#elif defined(CY_IP_M0S8CPUSSV3_DMAC)
    if(cfg->burst_size != 0)
    {
        return CYHAL_DMA_RSLT_ERR_INVALID_BURST_SIZE;
    }
    else
    {
        GET_RESOURCE_DATA(obj->descriptor_config).dataCount = cfg->length;
        GET_RESOURCE_DATA(obj->descriptor_config).srcAddrIncrement = cfg->src_increment;
        GET_RESOURCE_DATA(obj->descriptor_config).dstAddrIncrement = cfg->dst_increment;
    }
#endif

    DMAC_Type* base = _cyhal_dma_dmac_get_base(obj->resource.block_num);
#if defined(CY_IP_M4CPUSS_DMAC)
    cy_rslt_t rslt = Cy_DMAC_Descriptor_Init(GET_RESOURCE_DATA(&obj->descriptor), GET_RESOURCE_DATA(&obj->descriptor_config));
#elif defined(CY_IP_M0S8CPUSSV3_DMAC)
    cy_rslt_t rslt = Cy_DMAC_Descriptor_Init(base, obj->resource.channel_num, obj->descriptor, GET_RESOURCE_DATA(&obj->descriptor_config));
#endif
    if(CY_DMAC_SUCCESS != rslt)
        return CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER;

    /* Setup channel and enable */
    if(CY_DMAC_SUCCESS != Cy_DMAC_Channel_Init(base, obj->resource.channel_num, GET_RESOURCE_DATA(&obj->channel_config)))
        return CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER;
#if defined(CY_IP_M4CPUSS_DMAC)
    Cy_DMAC_Channel_SetDescriptor(base, obj->resource.channel_num, GET_RESOURCE_DATA(&obj->descriptor));
#elif defined(CY_IP_M0S8CPUSSV3_DMAC)
    Cy_DMAC_Channel_SetCurrentDescriptor(base, obj->resource.channel_num, obj->descriptor);
#endif
    Cy_DMAC_Channel_SetPriority(base, obj->resource.channel_num, GET_RESOURCE_DATA(obj->channel_config).priority);
    Cy_DMAC_Channel_Enable(base, obj->resource.channel_num);
#if defined(CY_IP_M4CPUSS_DMAC)
    Cy_DMAC_Channel_SetInterruptMask(base, obj->resource.channel_num, CY_DMAC_INTR_MASK);
#endif

    Cy_DMAC_Enable(base);

#if defined(CY_IP_M4CPUSS_DMAC)
    /* src_misal and dst_misal interrupts are triggered immediately on enable
     * so return those errors here */
    uint32_t status = Cy_DMAC_Channel_GetInterruptStatus(base, obj->resource.channel_num);
    if((status & CY_DMAC_INTR_SRC_MISAL) ||
       (status & CY_DMAC_INTR_DST_MISAL))
    {
        /* Clear all interrupts and return error */
        Cy_DMAC_Channel_ClearInterrupt(base, obj->resource.channel_num, CY_DMAC_INTR_MASK);
        return CYHAL_DMA_RSLT_ERR_INVALID_ALIGNMENT;
    }
#endif

    /* Enable interrupt for this channel */
    cy_stc_sysint_t irqCfg = { _cyhal_dma_dmac_get_irqn(obj), CYHAL_ISR_PRIORITY_DEFAULT };
    if(CY_SYSINT_SUCCESS != Cy_SysInt_Init(&irqCfg, _cyhal_dma_dmac_irq_handler))
        return CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER;
    NVIC_EnableIRQ(irqCfg.intrSrc);

    return CY_RSLT_SUCCESS;
}

cy_rslt_t _cyhal_dma_dmac_start_transfer(cyhal_dma_t *obj)
{
    /* Return warning if channel is busy */
    if(_cyhal_dma_dmac_is_busy(obj))
        return CYHAL_DMA_RSLT_WARN_TRANSFER_ALREADY_STARTED;

    if (_cyhal_dma_dmac_pm_transition_pending)
        return CYHAL_SYSPM_RSLT_ERR_PM_PENDING;

    uint32_t trigline = _cyhal_dma_dmac_get_trigger_line(obj->resource.block_num, obj->resource.channel_num);
    cy_en_trigmux_status_t trig_status = Cy_TrigMux_SwTrigger(trigline, CY_TRIGGER_TWO_CYCLES);

    /* Also return warning if SW trigger is already initated but DMA hardware
     * has not seen it yet */
    if(trig_status == CY_TRIGMUX_INVALID_STATE)
        return CYHAL_DMA_RSLT_WARN_TRANSFER_ALREADY_STARTED;
    else
        return CY_RSLT_SUCCESS;
}

void _cyhal_dma_dmac_enable_event(cyhal_dma_t *obj, cyhal_dma_event_t event, uint8_t intr_priority, bool enable)
{
#if defined (CY_IP_M0S8CPUSSV3_DMAC)
    DMAC_Type *base = _cyhal_dma_dmac_get_base(obj->resource.block_num);
    uint32_t mask = Cy_DMAC_GetInterruptMask(base);
#endif
    if(enable)
    {
#if defined (CY_IP_M0S8CPUSSV3_DMAC)
        Cy_DMAC_SetInterruptMask(base, mask | (1 << obj->resource.channel_num));
#endif
        obj->irq_cause |= event;
    }
    else
    {
#if defined (CY_IP_M0S8CPUSSV3_DMAC)
        Cy_DMAC_SetInterruptMask(base, mask & ~(1 << obj->resource.channel_num));
#endif
        obj->irq_cause &= ~event;
    }

    NVIC_SetPriority(_cyhal_dma_dmac_get_irqn(obj), intr_priority);
}

bool _cyhal_dma_dmac_is_busy(cyhal_dma_t *obj)
{
    /* The value is a bit field of all pending or active channels */
    return Cy_DMAC_GetActiveChannel(_cyhal_dma_dmac_get_base(obj->resource.block_num)) & (1 << obj->resource.channel_num);
}

#if defined(CY_IP_M4CPUSS_DMAC)
static cy_en_dma_trigger_type_t _cyhal_convert_input_t(cyhal_dma_input_t input)
{
    switch(input)
    {
        case CYHAL_DMA_INPUT_TRIGGER_SINGLE_ELEMENT:
            return CY_DMA_1ELEMENT;
        case CYHAL_DMA_INPUT_TRIGGER_SINGLE_BURST:
            return CY_DMA_X_LOOP;
        case CYHAL_DMA_INPUT_TRIGGER_ALL_ELEMENTS:
            return CY_DMA_DESCR;
    }
    // Should never reach here. Just silencing compiler warnings.
    CY_ASSERT(false);
    return CY_DMA_DESCR;
}

static cy_en_dma_trigger_type_t _cyhal_convert_output_t(cyhal_dma_output_t output)
{
    switch(output)
    {
        case CYHAL_DMA_OUTPUT_TRIGGER_SINGLE_ELEMENT:
            return CY_DMA_1ELEMENT;
        case CYHAL_DMA_OUTPUT_TRIGGER_SINGLE_BURST:
            return CY_DMA_X_LOOP;
        case CYHAL_DMA_OUTPUT_TRIGGER_ALL_ELEMENTS:
            return CY_DMA_DESCR;
    }
    // Should never reach here. Just silencing compiler warnings.
    CY_ASSERT(false);
    return CY_DMA_DESCR;
}

cy_rslt_t _cyhal_dma_dmac_connect_digital(cyhal_dma_t *obj, cyhal_source_t source, cyhal_dma_input_t input)
{
    if(input != CYHAL_DMA_INPUT_TRIGGER_SINGLE_ELEMENT &&
       input != CYHAL_DMA_INPUT_TRIGGER_SINGLE_BURST &&
       input != CYHAL_DMA_INPUT_TRIGGER_ALL_ELEMENTS)
        return CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER;

    Cy_DMAC_Channel_Disable(_cyhal_dma_dmac_get_base(obj->resource.block_num), obj->resource.channel_num);
    obj->descriptor.dmac.ctl &= ~DMAC_CH_V2_DESCR_CTL_TR_IN_TYPE_Msk;
    obj->descriptor.dmac.ctl |= _VAL2FLD(DMAC_CH_V2_DESCR_CTL_TR_IN_TYPE, _cyhal_convert_input_t(input));
    Cy_DMAC_Channel_Enable(_cyhal_dma_dmac_get_base(obj->resource.block_num), obj->resource.channel_num);

    cyhal_dest_t dest = _cyhal_dma_dmac_get_dest(obj->resource.block_num, obj->resource.channel_num);

    return _cyhal_connect_signal(source, dest, CYHAL_SIGNAL_TYPE_EDGE);
}

cy_rslt_t _cyhal_dma_dmac_enable_output(cyhal_dma_t *obj, cyhal_dma_output_t output, cyhal_source_t *source)
{
    if(output != CYHAL_DMA_OUTPUT_TRIGGER_SINGLE_ELEMENT &&
       output != CYHAL_DMA_OUTPUT_TRIGGER_SINGLE_BURST &&
       output != CYHAL_DMA_OUTPUT_TRIGGER_ALL_ELEMENTS)
        return CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER;

    Cy_DMAC_Channel_Disable(_cyhal_dma_dmac_get_base(obj->resource.block_num), obj->resource.channel_num);
    obj->descriptor.dmac.ctl &= ~DMAC_CH_V2_DESCR_CTL_TR_OUT_TYPE_Msk;
    obj->descriptor.dmac.ctl |= _VAL2FLD(DMAC_CH_V2_DESCR_CTL_TR_OUT_TYPE, _cyhal_convert_output_t(output));
    Cy_DMAC_Channel_Enable(_cyhal_dma_dmac_get_base(obj->resource.block_num), obj->resource.channel_num);

    *source = _cyhal_dma_dmac_get_src(obj->resource.block_num, obj->resource.channel_num);

    return CY_RSLT_SUCCESS;
}

cy_rslt_t _cyhal_dma_dmac_disconnect_digital(cyhal_dma_t *obj, cyhal_source_t source, cyhal_dma_input_t input)
{
    if(input != CYHAL_DMA_INPUT_TRIGGER_SINGLE_ELEMENT &&
       input != CYHAL_DMA_INPUT_TRIGGER_SINGLE_BURST &&
       input != CYHAL_DMA_INPUT_TRIGGER_ALL_ELEMENTS)
        return CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER;

    Cy_DMAC_Channel_Disable(_cyhal_dma_dmac_get_base(obj->resource.block_num), obj->resource.channel_num);

    // There is no option to totally disable. Just reset to default.
    obj->descriptor.dmac.ctl &= ~DMAC_CH_V2_DESCR_CTL_TR_IN_TYPE_Msk;
    obj->descriptor.dmac.ctl |= _VAL2FLD(DMAC_CH_V2_DESCR_CTL_TR_IN_TYPE, _cyhal_dma_dmac_default_descriptor_config.triggerInType);

    Cy_DMAC_Channel_Enable(_cyhal_dma_dmac_get_base(obj->resource.block_num), obj->resource.channel_num);

    cyhal_dest_t dest = _cyhal_dma_dmac_get_dest(obj->resource.block_num, obj->resource.channel_num);

    return _cyhal_disconnect_signal(source, dest);
}

cy_rslt_t _cyhal_dma_dmac_disable_output(cyhal_dma_t *obj, cyhal_dma_output_t output)
{
    if(output != CYHAL_DMA_OUTPUT_TRIGGER_SINGLE_ELEMENT &&
       output != CYHAL_DMA_OUTPUT_TRIGGER_SINGLE_BURST &&
       output != CYHAL_DMA_OUTPUT_TRIGGER_ALL_ELEMENTS)
        return CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER;

    Cy_DMAC_Channel_Disable(_cyhal_dma_dmac_get_base(obj->resource.block_num), obj->resource.channel_num);

    // There is no option to totally disable. Just reset to default.
    obj->descriptor.dmac.ctl &= ~DMAC_CH_V2_DESCR_CTL_TR_OUT_TYPE_Msk;
    obj->descriptor.dmac.ctl |= _VAL2FLD(DMAC_CH_V2_DESCR_CTL_TR_OUT_TYPE, _cyhal_dma_dmac_default_descriptor_config.triggerOutType);

    Cy_DMAC_Channel_Enable(_cyhal_dma_dmac_get_base(obj->resource.block_num), obj->resource.channel_num);

    return CY_RSLT_SUCCESS;

}
#elif defined(CY_IP_M0S8CPUSSV3_DMAC)

cy_rslt_t _cyhal_dma_dmac_connect_digital(cyhal_dma_t *obj, cyhal_source_t source, cyhal_dma_input_t input)
{
    if(input != CYHAL_DMA_INPUT_TRIGGER_SINGLE_ELEMENT &&
       input != CYHAL_DMA_INPUT_TRIGGER_ALL_ELEMENTS)
        return CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER;

    Cy_DMAC_Channel_SetCurrentDescriptor(_cyhal_dma_dmac_get_base(obj->resource.block_num), obj->resource.channel_num, obj->descriptor);

    cyhal_dest_t dest = (cyhal_dest_t)(CYHAL_TRIGGER_CPUSS_DMAC_TR_IN0 + obj->resource.channel_num);

    return _cyhal_connect_signal(source, dest, CYHAL_SIGNAL_TYPE_EDGE);
}

// M0S8 output triggers are always active. This is a noop except to return the source.
cy_rslt_t _cyhal_dma_dmac_enable_output(cyhal_dma_t *obj, cyhal_dma_output_t output, cyhal_source_t *source)
{
    if(output != CYHAL_DMA_OUTPUT_TRIGGER_SINGLE_ELEMENT &&
       output != CYHAL_DMA_OUTPUT_TRIGGER_ALL_ELEMENTS)
        return CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER;

    *source = (cyhal_source_t)(CYHAL_TRIGGER_CPUSS_DMAC_TR_OUT0 + (obj->resource.block_num * CPUSS_DMAC_CH_NR) + obj->resource.channel_num);

    return CY_RSLT_SUCCESS;
}

cy_rslt_t _cyhal_dma_dmac_disconnect_digital(cyhal_dma_t *obj, cyhal_source_t source, cyhal_dma_input_t input)
{
    if(input != CYHAL_DMA_INPUT_TRIGGER_SINGLE_ELEMENT &&
       input != CYHAL_DMA_INPUT_TRIGGER_ALL_ELEMENTS)
        return CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER;

    // Reset to default
    Cy_DMAC_Channel_SetCurrentDescriptor(_cyhal_dma_dmac_get_base(obj->resource.block_num), obj->resource.channel_num, obj->descriptor);

    cyhal_dest_t dest = (cyhal_dest_t)(CYHAL_TRIGGER_CPUSS_DMAC_TR_IN0 + obj->resource.channel_num);

    return _cyhal_disconnect_signal(source, dest);
}

// M0S8 output triggers are always active. This is a noop.
cy_rslt_t _cyhal_dma_dmac_disable_output(cyhal_dma_t *obj, cyhal_dma_output_t output)
{
    CY_UNUSED_PARAMETER(obj);

    if(output != CYHAL_DMA_OUTPUT_TRIGGER_SINGLE_ELEMENT &&
       output != CYHAL_DMA_OUTPUT_TRIGGER_ALL_ELEMENTS)
        return CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER;

    return CY_RSLT_SUCCESS;
}

#endif

#endif /* defined(CY_IP_M4CPUSS_DMAC) || defined(CY_IP_M0S8CPUSSV3_DMAC) */

#if defined(__cplusplus)
}
#endif
