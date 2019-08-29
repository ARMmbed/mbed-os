/*******************************************************************************
* File Name: cycfg_dmas.c
*
* Description:
* DMA configuration
* This file was automatically generated and should not be modified.
* 
********************************************************************************
* Copyright 2017-2019 Cypress Semiconductor Corporation
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
********************************************************************************/

#include "cycfg_dmas.h"

const cy_stc_dma_descriptor_config_t cpuss_0_dw0_0_chan_0_Descriptor_0_config = 
{
	.retrigger = CY_DMA_RETRIG_IM,
	.interruptType = CY_DMA_1ELEMENT,
	.triggerOutType = CY_DMA_1ELEMENT,
	.channelState = CY_DMA_CHANNEL_DISABLED,
	.triggerInType = CY_DMA_1ELEMENT,
	.dataSize = CY_DMA_BYTE,
	.srcTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
	.dstTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
	.descriptorType = CY_DMA_1D_TRANSFER,
	.srcAddress = NULL,
	.dstAddress = NULL,
	.srcXincrement = 0,
	.dstXincrement = 1,
	.xCount = 6,
	.srcYincrement = 0,
	.dstYincrement = 0,
	.yCount = 1,
	.nextDescriptor = NULL,
};
cy_stc_dma_descriptor_t cpuss_0_dw0_0_chan_0_Descriptor_0 = 
{
	.ctl = 0UL,
	.src = 0UL,
	.dst = 0UL,
	.xCtl = 0UL,
	.yCtl = 0UL,
	.nextPtr = 0UL,
};
const cy_stc_dma_channel_config_t cpuss_0_dw0_0_chan_0_channelConfig = 
{
	.descriptor = &cpuss_0_dw0_0_chan_0_Descriptor_0,
	.preemptable = true,
	.priority = 1,
	.enable = false,
	.bufferable = false,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t cpuss_0_dw0_0_chan_0_obj = 
	{
		.type = CYHAL_RSC_DMA,
		.block_num = 0U,
		.channel_num = cpuss_0_dw0_0_chan_0_CHANNEL,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_dma_descriptor_config_t cpuss_0_dw0_0_chan_1_Descriptor_0_config = 
{
	.retrigger = CY_DMA_RETRIG_16CYC,
	.interruptType = CY_DMA_1ELEMENT,
	.triggerOutType = CY_DMA_1ELEMENT,
	.channelState = CY_DMA_CHANNEL_DISABLED,
	.triggerInType = CY_DMA_1ELEMENT,
	.dataSize = CY_DMA_BYTE,
	.srcTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
	.dstTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
	.descriptorType = CY_DMA_1D_TRANSFER,
	.srcAddress = NULL,
	.dstAddress = NULL,
	.srcXincrement = 1,
	.dstXincrement = 0,
	.xCount = 5,
	.srcYincrement = 0,
	.dstYincrement = 0,
	.yCount = 1,
	.nextDescriptor = NULL,
};
cy_stc_dma_descriptor_t cpuss_0_dw0_0_chan_1_Descriptor_0 = 
{
	.ctl = 0UL,
	.src = 0UL,
	.dst = 0UL,
	.xCtl = 0UL,
	.yCtl = 0UL,
	.nextPtr = 0UL,
};
const cy_stc_dma_channel_config_t cpuss_0_dw0_0_chan_1_channelConfig = 
{
	.descriptor = &cpuss_0_dw0_0_chan_1_Descriptor_0,
	.preemptable = true,
	.priority = 1,
	.enable = false,
	.bufferable = false,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t cpuss_0_dw0_0_chan_1_obj = 
	{
		.type = CYHAL_RSC_DMA,
		.block_num = 0U,
		.channel_num = cpuss_0_dw0_0_chan_1_CHANNEL,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_dma_descriptor_config_t cpuss_0_dw1_0_chan_1_Descriptor_0_config = 
{
	.retrigger = CY_DMA_RETRIG_4CYC,
	.interruptType = CY_DMA_DESCR,
	.triggerOutType = CY_DMA_1ELEMENT,
	.channelState = CY_DMA_CHANNEL_DISABLED,
	.triggerInType = CY_DMA_X_LOOP,
	.dataSize = CY_DMA_HALFWORD,
	.srcTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
	.dstTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
	.descriptorType = CY_DMA_2D_TRANSFER,
	.srcAddress = NULL,
	.dstAddress = NULL,
	.srcXincrement = 2,
	.dstXincrement = 0,
	.xCount = 10,
	.srcYincrement = 10,
	.dstYincrement = 0,
	.yCount = 2,
	.nextDescriptor = NULL,
};
cy_stc_dma_descriptor_t cpuss_0_dw1_0_chan_1_Descriptor_0 = 
{
	.ctl = 0UL,
	.src = 0UL,
	.dst = 0UL,
	.xCtl = 0UL,
	.yCtl = 0UL,
	.nextPtr = 0UL,
};
const cy_stc_dma_channel_config_t cpuss_0_dw1_0_chan_1_channelConfig = 
{
	.descriptor = &cpuss_0_dw1_0_chan_1_Descriptor_0,
	.preemptable = false,
	.priority = 0,
	.enable = false,
	.bufferable = false,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t cpuss_0_dw1_0_chan_1_obj = 
	{
		.type = CYHAL_RSC_DMA,
		.block_num = 1U,
		.channel_num = cpuss_0_dw1_0_chan_1_CHANNEL,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_dma_descriptor_config_t cpuss_0_dw1_0_chan_3_Descriptor_0_config = 
{
	.retrigger = CY_DMA_RETRIG_IM,
	.interruptType = CY_DMA_DESCR,
	.triggerOutType = CY_DMA_1ELEMENT,
	.channelState = CY_DMA_CHANNEL_DISABLED,
	.triggerInType = CY_DMA_X_LOOP,
	.dataSize = CY_DMA_HALFWORD,
	.srcTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
	.dstTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
	.descriptorType = CY_DMA_2D_TRANSFER,
	.srcAddress = NULL,
	.dstAddress = NULL,
	.srcXincrement = 0,
	.dstXincrement = 2,
	.xCount = 10,
	.srcYincrement = 0,
	.dstYincrement = 10,
	.yCount = 2,
	.nextDescriptor = NULL,
};
cy_stc_dma_descriptor_t cpuss_0_dw1_0_chan_3_Descriptor_0 = 
{
	.ctl = 0UL,
	.src = 0UL,
	.dst = 0UL,
	.xCtl = 0UL,
	.yCtl = 0UL,
	.nextPtr = 0UL,
};
const cy_stc_dma_channel_config_t cpuss_0_dw1_0_chan_3_channelConfig = 
{
	.descriptor = &cpuss_0_dw1_0_chan_3_Descriptor_0,
	.preemptable = false,
	.priority = 0,
	.enable = false,
	.bufferable = false,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t cpuss_0_dw1_0_chan_3_obj = 
	{
		.type = CYHAL_RSC_DMA,
		.block_num = 1U,
		.channel_num = cpuss_0_dw1_0_chan_3_CHANNEL,
	};
#endif //defined (CY_USING_HAL)


void init_cycfg_dmas(void)
{
#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&cpuss_0_dw0_0_chan_0_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&cpuss_0_dw0_0_chan_1_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&cpuss_0_dw1_0_chan_1_obj);
#endif //defined (CY_USING_HAL)

#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&cpuss_0_dw1_0_chan_3_obj);
#endif //defined (CY_USING_HAL)
}
