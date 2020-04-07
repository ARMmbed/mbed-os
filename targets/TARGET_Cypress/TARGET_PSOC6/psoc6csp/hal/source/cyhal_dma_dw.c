/***************************************************************************//**
* \file cyhal_dma_dw.c
*
* \brief
* Implements a high level interface for interacting with the Cypress Datawire DMA.
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

#include "cyhal_dma.h"
#include "cyhal_dma_dw.h"
#include "cyhal_dma_impl.h"
#include "cyhal_hwmgr.h"
#include "cyhal_system.h"
#include "cyhal_utils.h"
#include "cyhal_triggers.h"

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef CY_IP_M4CPUSS_DMA

#if (CPUSS_DW0_PRESENT==1) && (CPUSS_DW1_PRESENT==1)
#define NUM_DW_CHANNELS (CPUSS_DW0_CH_NR + CPUSS_DW1_CH_NR)
#elif (CPUSS_DW0_PRESENT==1)
#define NUM_DW_CHANNELS (CPUSS_DW0_CH_NR)
#endif

static cyhal_dma_t* cyhal_dw_config_structs[NUM_DW_CHANNELS];

/** Default dw descriptor config */
static const cy_stc_dma_descriptor_config_t default_descriptor_config_dw =
{
    .retrigger = CY_DMA_RETRIG_IM,
    .interruptType = CY_DMA_DESCR,
    .triggerOutType = CY_DMA_DESCR_CHAIN,
    .channelState = CY_DMA_CHANNEL_ENABLED,
    .triggerInType = CY_DMA_DESCR,
    .dataSize = CY_DMA_WORD,
    .srcTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
    .dstTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
    .descriptorType = CY_DMA_1D_TRANSFER,
    .srcAddress = 0,
    .dstAddress = 0,
    .srcXincrement = 1U,
    .dstXincrement = 1U,
    .xCount = 1UL,
    .srcYincrement = 0U,
    .dstYincrement = 0U,
    .yCount = 1UL,
    .nextDescriptor = 0,
};

/** Default dw channel config */
static const cy_stc_dma_channel_config_t default_channel_config_dw =
{
    .descriptor = 0,
    .preemptable = false,
    .priority = 1,
    .enable = false,
    .bufferable = false,
};

/** Sets the dw configuration struct */
static inline void cyhal_dma_set_dw_obj(cyhal_dma_t *obj)
{
    cyhal_dw_config_structs[obj->resource.block_num * CPUSS_DW0_CH_NR + obj->resource.channel_num] = obj;
}

/** Zeros the dw configuration struct */
static inline void cyhal_dma_free_dw_obj(cyhal_dma_t *obj)
{
    cyhal_dw_config_structs[obj->resource.block_num * CPUSS_DW0_CH_NR + obj->resource.channel_num] = NULL;
}

/** Gets the dw configuration struct from block and channel */
static inline cyhal_dma_t* cyhal_dma_get_dw_obj(uint8_t block, uint8_t channel)
{
    return cyhal_dw_config_structs[block * CPUSS_DW0_CH_NR + channel];
}

/** Gets the dw block number from irq number */
/** This should never be called from a non-dma IRQn */
static inline uint8_t cyhal_dma_get_dw_block_from_irqn(IRQn_Type irqn)
{
    uint8_t diff = irqn - cpuss_interrupts_dw0_0_IRQn;
#if defined(CPUSS_DW0_CH_NR) && !defined(CPUSS_DW1_CH_NR)
    CY_ASSERT(diff < CPUSS_DW0_CH_NR);

    if(diff < CPUSS_DW0_CH_NR)
        return 0;
#elif defined(CPUSS_DW0_CH_NR) && defined(CPUSS_DW1_CH_NR)
    CY_ASSERT(diff < CPUSS_DW0_CH_NR + CPUSS_DW1_CH_NR);

    if(diff < CPUSS_DW0_CH_NR)
        return 0;
    if(diff < CPUSS_DW0_CH_NR + CPUSS_DW1_CH_NR)
        return 1;
#endif

    // Should never reach here. Just silencing compiler warnings.
    CY_ASSERT(false);
    return 255;
}

/** Gets the dw channel number from irq number */
/** This should never be called from a non-dma IRQn */
static inline uint8_t cyhal_dma_get_dw_channel_from_irqn(IRQn_Type irqn)
{
    uint8_t diff = irqn - cpuss_interrupts_dw0_0_IRQn;
#if defined(CPUSS_DW0_CH_NR) && !defined(CPUSS_DW1_CH_NR)
    CY_ASSERT(diff < CPUSS_DW0_CH_NR);

    if(diff < CPUSS_DW0_CH_NR)
        return diff;
#elif defined(CPUSS_DW0_CH_NR) && defined(CPUSS_DW1_CH_NR)
    CY_ASSERT(diff < CPUSS_DW0_CH_NR + CPUSS_DW1_CH_NR);

    if(diff < CPUSS_DW0_CH_NR)
        return diff;
    else
        return diff - CPUSS_DW0_CH_NR;
#endif
}

/** Gets the irqn corresponding to a particular cyhal_dma_t config struct */
static inline IRQn_Type cyhal_dma_get_dw_irqn(cyhal_dma_t *obj)
{
    return (IRQn_Type)((uint8_t)cpuss_interrupts_dw0_0_IRQn + (obj->resource.block_num * CPUSS_DW0_CH_NR + obj->resource.channel_num));
}

/** Gets the dw base pointer from block number */
static inline DW_Type* cyhal_dma_get_dw_base(uint8_t block_num)
{
    return block_num == 0 ? DW0 : DW1;
}

/** Uses tables provided as part of the hal interconnect driver to determine mux
 * trigger group and mux trigger index and then construct the trigger line
 * input parameter to Cy_TrigMux_SwTrigger. */
static inline uint32_t cyhal_dma_get_dw_trigger_line(uint8_t block_num, uint8_t channel_num)
{
    /* cyhal_dest_t triggers are guaranteed to be sorted by trigger type, block
     * num, then channel num, therefore, we can just directly find the proper
     * trigger by calculating an offset. */
    cyhal_dest_t trigger = (cyhal_dest_t)(TRIGGER_CPUSS_DW0_TR_IN0 + (block_num * CPUSS_DW0_CH_NR) + channel_num);

    /* One to one triggers have bit 8 set in cyhal_dest_to_mux but
     * Cy_TrigMux_SwTrigger wants the trigger group field to have bit 5 set to
     * denote one to one triggers. */
    uint8_t trig_group = cyhal_dest_to_mux[trigger];
    /* If hal one to one triggers bit is set: mask it out and set pdl one to
     * one bit */
    if(trig_group & HAL_TRIGGERS_1TO1_MASK)
        trig_group = (trig_group & ~HAL_TRIGGERS_1TO1_MASK) | PDL_TRIGGERS_1TO1_MASK;

    /* Construct trigger line which consists of three fields packed into a
     * uint32_t:
     * Bits   30: Input/output bit. Set to 1 for output.
     * Bits 12-8: Trigger group selection.
     * Bits  7-0: Select the output trigger number in the trigger group. */
    return PERI_TR_CMD_OUT_SEL_Msk | trig_group << 8 | cyhal_mux_dest_index[trigger];
}

/** Convert PDL interrupt cause to hal dma event */
static inline cyhal_dma_event_t cyhal_dma_convert_dw_interrupt_cause(cy_en_dma_intr_cause_t cause)
{
    switch(cause)
    {
        case CY_DMA_INTR_CAUSE_NO_INTR:
            return CYHAL_DMA_NO_INTR;
        case CY_DMA_INTR_CAUSE_COMPLETION:
            return CYHAL_DMA_TRANSFER_COMPLETE;
        case CY_DMA_INTR_CAUSE_SRC_BUS_ERROR:
            return CYHAL_DMA_SRC_BUS_ERROR;
        case CY_DMA_INTR_CAUSE_DST_BUS_ERROR:
            return CYHAL_DMA_DST_BUS_ERROR;
        case CY_DMA_INTR_CAUSE_SRC_MISAL:
            return CYHAL_DMA_SRC_MISAL;
        case CY_DMA_INTR_CAUSE_DST_MISAL:
            return CYHAL_DMA_DST_MISAL;
        case CY_DMA_INTR_CAUSE_CURR_PTR_NULL:
            return CYHAL_DMA_CURR_PTR_NULL;
        case CY_DMA_INTR_CAUSE_ACTIVE_CH_DISABLED:
            return CYHAL_DMA_ACTIVE_CH_DISABLED;
        case CY_DMA_INTR_CAUSE_DESCR_BUS_ERROR:
            return CYHAL_DMA_DESCR_BUS_ERROR;
        default:
            return CYHAL_DMA_NO_INTR;
    }
}

/** DW irq handler */
static void cyhal_dma_irq_handler_dw(void)
{
    /* Use irqn to get appropriate config structure */
    uint8_t block = cyhal_dma_get_dw_block_from_irqn(CYHAL_GET_CURRENT_IRQN());
    uint8_t channel = cyhal_dma_get_dw_channel_from_irqn(CYHAL_GET_CURRENT_IRQN());
    cyhal_dma_t *obj = cyhal_dma_get_dw_obj(block, channel);

    /* Get interrupt type and call users event callback if they have enabled that event */
    cy_en_dma_intr_cause_t cause = Cy_DMA_Channel_GetStatus(cyhal_dma_get_dw_base(block), channel);
    cyhal_dma_event_t event_type = cyhal_dma_convert_dw_interrupt_cause(cause);
    uint32_t events_to_callback = event_type && obj->irq_cause;
    if(obj->callback_data.callback != NULL && events_to_callback)
    {
        ((cyhal_dma_event_callback_t)obj->callback_data.callback)(obj->callback_data.callback_arg, (cyhal_dma_event_t)events_to_callback);
    }

    /* Clear all interrupts */
    Cy_DMA_Channel_ClearInterrupt(cyhal_dma_get_dw_base(block), channel);
}

cy_rslt_t cyhal_dma_init_dw(cyhal_dma_t *obj, uint8_t priority)
{
    if(!CY_DMA_IS_PRIORITY_VALID(priority))
        return CYHAL_DMA_RSLT_ERR_INVALID_PRIORITY;

    cy_rslt_t rslt = cyhal_hwmgr_allocate(CYHAL_RSC_DW, &obj->resource);
    if(rslt != CY_RSLT_SUCCESS)
        return rslt;

    /* Setup descriptor and channel configs */
    obj->descriptor_config.dw = default_descriptor_config_dw;
    obj->channel_config.dw = default_channel_config_dw;
    obj->channel_config.dw.descriptor = &obj->descriptor.dw;
    obj->channel_config.dw.priority = priority;

    obj->callback_data.callback = NULL;
    obj->callback_data.callback_arg = NULL;
    obj->irq_cause = 0;

    cyhal_dma_set_dw_obj(obj);

    return CY_RSLT_SUCCESS;
}

void cyhal_dma_free_dw(cyhal_dma_t *obj)
{
    Cy_DMA_Descriptor_DeInit(&obj->descriptor.dw);
    Cy_DMA_Channel_DeInit(cyhal_dma_get_dw_base(obj->resource.block_num), obj->resource.channel_num);

    NVIC_DisableIRQ(cyhal_dma_get_dw_irqn(obj));

    cyhal_dma_free_dw_obj(obj);
    cyhal_hwmgr_free(&obj->resource);
}

/* Initalize descriptor, initialize channel, enable channel, enable channel
 * interrupt, and enable DW controller */
cy_rslt_t cyhal_dma_configure_dw(cyhal_dma_t *obj, const cyhal_dma_cfg_t *cfg)
{
    /* Do not reconfigure if transfer is pending/active already */
    if(cyhal_dma_is_busy_dw(obj))
        return CYHAL_DMA_RSLT_ERR_CHANNEL_BUSY;

    obj->descriptor_config.dw.srcAddress = (void*)cfg->src_addr;
    obj->descriptor_config.dw.dstAddress = (void*)cfg->dst_addr;
    obj->descriptor_config.dw.nextDescriptor = &obj->descriptor.dw;

    if(cfg->transfer_width == 8)
        obj->descriptor_config.dw.dataSize = CY_DMA_BYTE;
    else if(cfg->transfer_width == 16)
        obj->descriptor_config.dw.dataSize = CY_DMA_HALFWORD;
    else if(cfg->transfer_width == 32)
        obj->descriptor_config.dw.dataSize = CY_DMA_WORD;
    else
        return CYHAL_DMA_RSLT_ERR_INVALID_TRANSFER_WIDTH;

    /* Length must be a multiple of burst_size */
    if(cfg->burst_size != 0 && cfg->length % cfg->burst_size != 0)
        return CYHAL_DMA_RSLT_ERR_INVALID_BURST_SIZE;

    /* Setup 2D transfer if burst_size is being used otherwise set up 1D
     * transfer */
    if(cfg->burst_size != 0)
    {
        obj->descriptor_config.dw.descriptorType = CY_DMA_2D_TRANSFER;
        obj->descriptor_config.dw.xCount = cfg->burst_size;
        obj->descriptor_config.dw.yCount = cfg->length / cfg->burst_size;
        obj->descriptor_config.dw.srcXincrement = cfg->src_increment;
        obj->descriptor_config.dw.dstXincrement = cfg->dst_increment;
        obj->descriptor_config.dw.srcYincrement = cfg->src_increment * cfg->burst_size;
        obj->descriptor_config.dw.dstYincrement = cfg->dst_increment * cfg->burst_size;

        /* If burst action, configure trigger and interrupt actions */
        if(cfg->action == CYHAL_DMA_TRANSFER_BURST)
        {
            obj->descriptor_config.dw.interruptType = CY_DMA_X_LOOP;
            obj->descriptor_config.dw.triggerInType = CY_DMA_X_LOOP;
        }
    }
    else
    {
        obj->descriptor_config.dw.descriptorType = CY_DMA_1D_TRANSFER;
        obj->descriptor_config.dw.xCount = cfg->length;
        obj->descriptor_config.dw.srcXincrement = cfg->src_increment;
        obj->descriptor_config.dw.dstXincrement = cfg->dst_increment;

        obj->descriptor_config.dw.interruptType = CY_DMA_DESCR;
        obj->descriptor_config.dw.triggerInType = CY_DMA_DESCR;
    }

    if(CY_DMA_SUCCESS != Cy_DMA_Descriptor_Init(&obj->descriptor.dw, &obj->descriptor_config.dw))
        return CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER;

    /* Setup channel and enable */
    DW_Type* base = cyhal_dma_get_dw_base(obj->resource.block_num);
    if(CY_DMA_SUCCESS != Cy_DMA_Channel_Init(base, obj->resource.channel_num, &obj->channel_config.dw))
        return CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER;
    Cy_DMA_Channel_SetDescriptor(base, obj->resource.channel_num, &obj->descriptor.dw);
    Cy_DMA_Channel_SetPriority(base, obj->resource.channel_num, obj->channel_config.dw.priority);
    Cy_DMA_Channel_Enable(base, obj->resource.channel_num);
    Cy_DMA_Channel_SetInterruptMask (base, obj->resource.channel_num, CY_DMA_INTR_MASK);

    Cy_DMA_Enable(base);

    /* src_misal and dst_misal interrupts are triggered immediately on enable
     * so return those errors here */
    uint32_t status = Cy_DMA_Channel_GetInterruptStatus(base, obj->resource.channel_num);
    if((status & CY_DMA_INTR_CAUSE_SRC_MISAL) ||
       (status & CY_DMA_INTR_CAUSE_DST_MISAL))
    {
        Cy_DMA_Channel_ClearInterrupt(base, obj->resource.channel_num);
        return CYHAL_DMA_RSLT_ERR_INVALID_ALIGNMENT;
    }

    /* Enable interrupt for this channel */
    cy_stc_sysint_t irqCfg = { cyhal_dma_get_dw_irqn(obj), CYHAL_ISR_PRIORITY_DEFAULT };
    if(CY_SYSINT_SUCCESS != Cy_SysInt_Init(&irqCfg, cyhal_dma_irq_handler_dw))
        return CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER;
    NVIC_EnableIRQ(irqCfg.intrSrc);

    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_dma_start_transfer_dw(cyhal_dma_t *obj)
{
    /* Return warning if channel is busy */
    if(cyhal_dma_is_busy_dw(obj))
        return CYHAL_DMA_RSLT_WARN_TRANSFER_ALREADY_STARTED;

    uint32_t trigline = cyhal_dma_get_dw_trigger_line(obj->resource.block_num, obj->resource.channel_num);
    cy_en_trigmux_status_t trig_status = Cy_TrigMux_SwTrigger(trigline, CY_TRIGGER_TWO_CYCLES);

    /* Also return warning if SW trigger is already initated but DMA hardware
     * has not seen it yet */
    if(trig_status == CY_TRIGMUX_INVALID_STATE)
        return CYHAL_DMA_RSLT_WARN_TRANSFER_ALREADY_STARTED;
    else
        return CY_RSLT_SUCCESS;
}

void cyhal_dma_enable_event_dw(cyhal_dma_t *obj, cyhal_dma_event_t event, uint8_t intrPriority, bool enable)
{
    if(enable)
        obj->irq_cause |= event;
    else
        obj->irq_cause &= ~event;

    NVIC_SetPriority(cyhal_dma_get_dw_irqn(obj), intrPriority);
}

bool cyhal_dma_is_busy_dw(cyhal_dma_t *obj)
{
#if CY_IP_M4CPUSS_DMA_VERSION == 1
    /* In DW_V1 the pending channel information is stored in the PENDING
     * register of the DW block and is a bit field of all pending or active
     * channels */
    return cyhal_dma_get_dw_base(obj->resource.block_num)->PENDING & (1 << obj->resource.channel_num);
#elif CY_IP_M4CPUSS_DMA_VERSION == 2
    /* In DW_V2 the pending channel information is stored in the STATUS
     * register of the channel itself */
    return DW_CH_STATUS(cyhal_dma_get_dw_base(obj->resource.block_num), obj->resource.channel_num) & (1UL << DW_CH_STRUCT_V2_CH_STATUS_PENDING_Pos);
#else
    // Should never reach here. Just silencing compiler warnings.
    CY_ASSERT(false);
    return false;
#endif
}

#endif /* CY_IP_M4CPUSS_DMA */

#if defined(__cplusplus)
}
#endif
