/***************************************************************************//**
 * @file dma_api_HAL.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef MBED_DMA_API_HAL_H
#define MBED_DMA_API_HAL_H

#include <stdint.h>
#include "dma_api.h"
#include "em_device.h"

#ifdef DMA_PRESENT
#include "em_dma.h"
#endif

#ifdef LDMA_PRESENT
#include "em_ldma.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Purpose of this file: extend dma_api.h to include EFM-specific DMA attributes */

#define DMA_CAP_2DCOPY (1 << 0)
#define DMA_CAP_NONE   (0 << 0)

#if ( DMA_CHAN_COUNT <= 4 )
#define DMACTRL_CH_CNT      4
#define DMACTRL_ALIGNMENT   256

#elif ( ( DMA_CHAN_COUNT > 4 ) && ( DMA_CHAN_COUNT <= 8 ) )
#define DMACTRL_CH_CNT      8
#define DMACTRL_ALIGNMENT   256

#elif ( ( DMA_CHAN_COUNT > 8 ) && ( DMA_CHAN_COUNT <= 16 ) )
#define DMACTRL_CH_CNT      16
#define DMACTRL_ALIGNMENT   512

#else
#error "Unsupported DMA channel count (dma_api.c)."
#endif

#ifdef LDMA_PRESENT
typedef void (*LDMAx_CBFunc_t)(unsigned int channel, bool primary, void *user);

typedef struct
{
    LDMAx_CBFunc_t cbFunc;
    void *userPtr;
} LDMAx_Callback_t;

void LDMAx_StartTransfer(  int ch,
                           LDMA_TransferCfg_t *transfer,
                           LDMA_Descriptor_t  *descriptor,
                           LDMAx_CBFunc_t cbFunc,
                           void *userData );
bool LDMAx_ChannelEnabled( int ch );
#endif

typedef struct {
    DMAUsage dmaUsageState;
    int dmaChannel;
#ifndef LDMA_PRESENT
    DMA_CB_TypeDef dmaCallback;
#else
    LDMAx_Callback_t dmaCallback;
#endif
} DMA_OPTIONS_t;

typedef void (*DMACallback)(void);

extern DMA_DESCRIPTOR_TypeDef dmaControlBlock[];

#ifdef __cplusplus
}
#endif

#endif
