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

#include "cyhal_utils.h"
#include "cyhal_system_impl.h"
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
static cyhal_event_callback_data_t *cyhal_tcpwm_callback_data_structs[TCPWM_CHANNELS];

void cyhal_tcpwm_init_callback_data(cyhal_resource_inst_t *resource, cyhal_event_callback_data_t *callback_data)
{
    uint8_t index = GET_ARRAY_INDEX(resource->block_num, resource->channel_num);
    cyhal_tcpwm_callback_data_structs[index] = callback_data;
    callback_data->callback = NULL;
    callback_data->callback_arg = NULL;
}

void cyhal_tcpwm_irq_handler()
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
        uint32_t index = GET_ARRAY_INDEX(block, channel);

        cyhal_event_callback_data_t *callback_data = cyhal_tcpwm_callback_data_structs[index];
        if (callback_data->callback != NULL)
        {
            cyhal_tcpwm_event_callback_t callback = (cyhal_tcpwm_event_callback_t) callback_data->callback;
            /* Call registered callbacks here */
            (void) (callback) (callback_data->callback_arg, Cy_TCPWM_GetInterruptStatus(blockAddr, channel));
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

void cyhal_tcpwm_register_callback(cyhal_resource_inst_t *resource, cy_israddress callback, void *callback_arg)
{
    uint8_t index = GET_ARRAY_INDEX(resource->block_num, resource->channel_num);
    uint32_t savedIntrStatus = cyhal_system_critical_section_enter();
    cyhal_tcpwm_callback_data_structs[index]->callback = callback;
    cyhal_tcpwm_callback_data_structs[index]->callback_arg = callback_arg;
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

void cyhal_tcpwm_enable_event(TCPWM_Type *type, cyhal_resource_inst_t *resource, uint32_t event, uint8_t intrPriority, bool enable)
{
    uint32_t old_mask = Cy_TCPWM_GetInterruptMask(type, resource->channel_num);
    if (enable)
    {
        // Clear any newly enabled events so that old IRQs don't trigger ISRs
        Cy_TCPWM_ClearInterrupt(type, resource->channel_num, ~old_mask & event);
    }
    Cy_TCPWM_SetInterruptMask(type, resource->channel_num, enable ? (old_mask | event) : (old_mask & ~event));

    IRQn_Type irqn = (IRQn_Type) (CYHAL_TCPWM_DATA[resource->block_num].isr_offset + resource->channel_num);
    NVIC_SetPriority(irqn, intrPriority);
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXTCPWM_INSTANCES) */
