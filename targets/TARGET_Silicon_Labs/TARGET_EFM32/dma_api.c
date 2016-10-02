/***************************************************************************//**
 * @file dma_api.c
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

#include <stdint.h>
#include "dma_api_HAL.h"
#include "em_device.h"
#include "em_cmu.h"

#ifdef DMA_PRESENT
#include "em_dma.h"
#endif

#ifdef LDMA_PRESENT
#include "em_ldma.h"
#endif

/** DMA control block array, requires proper alignment. */
#ifdef DMA_PRESENT
#if defined (__ICCARM__)
#pragma data_alignment=DMACTRL_ALIGNMENT
DMA_DESCRIPTOR_TypeDef dmaControlBlock[DMACTRL_CH_CNT * 2];

#elif defined (__CC_ARM)
DMA_DESCRIPTOR_TypeDef dmaControlBlock[DMACTRL_CH_CNT * 2] __attribute__ ((aligned(DMACTRL_ALIGNMENT)));

#elif defined (__GNUC__)
DMA_DESCRIPTOR_TypeDef dmaControlBlock[DMACTRL_CH_CNT * 2] __attribute__ ((aligned(DMACTRL_ALIGNMENT), section("dma")));

#else
#error Undefined toolkit, need to define alignment
#endif
#endif /* DMA_PRESENT */

uint32_t channels = 0; // Bit vector of taken channels
bool enabled = false;

void dma_init(void)
{
    if (enabled) return;

#if defined DMA_PRESENT
    CMU_ClockEnable(cmuClock_DMA, true);
    CMU_ClockEnable(cmuClock_HFPER, true); // FIXME: DMA is clocked via HFCORECLK, why HFPERCLK?

    DMA_Init_TypeDef   dmaInit;

    dmaInit.hprot        = 0;
    dmaInit.controlBlock = dmaControlBlock;
    DMA_Init(&dmaInit);

#elif defined LDMA_PRESENT
    CMU_ClockEnable(cmuClock_LDMA, true);

    LDMA_Init_t ldmaInit;

    ldmaInit.ldmaInitCtrlNumFixed = 0;     /* All channels round-robin */
    ldmaInit.ldmaInitCtrlSyncPrsClrEn = 0; /* Do not allow PRS to clear SYNCTRIG */
    ldmaInit.ldmaInitCtrlSyncPrsSetEn = 0; /* Do not allow PRS to set SYNCTRIG */
    ldmaInit.ldmaInitIrqPriority = 2;      /* IRQ Priority */

    LDMA_Init(&ldmaInit);
#else
#error "Unrecognized DMA peripheral"
#endif

    enabled = true;
}

int dma_channel_allocate(uint32_t capabilities)
{
    int i;
    // Check if 2d copy is required
    if (DMA_CAP_2DCOPY & capabilities) {
        if (channels & 1) {
            // Channel already in use
            return DMA_ERROR_OUT_OF_CHANNELS;
        } else {
            channels |= 1 << 0;
            return 0;
        }
    }
    for (i = 1; i < DMA_CHAN_COUNT; i++) {
        if ((channels & (1 << i)) == 0) {
            // Channel available
            channels |= 1 << i;
            return i;
        }
    }
    // Check if channel 0 is available
    if ((channels & 1 ) == 0) {
        channels |= 1 << 0;
        return 0;
    }
    // Couldn't find a channel.
    return DMA_ERROR_OUT_OF_CHANNELS;
}

int dma_channel_free(int channelid)
{
    if( channelid >= 0 ) {
        channels &= ~(1 << channelid);
    }

    return 0;
}

#ifdef LDMA_PRESENT

/* LDMA emlib API extensions */

typedef struct {
    LDMAx_CBFunc_t callback;
    void *userdata;
} LDMA_InternCallback_t;

static LDMA_InternCallback_t ldmaCallback[DMA_CHAN_COUNT];

void LDMAx_StartTransfer(  int ch,
                           LDMA_TransferCfg_t *transfer,
                           LDMA_Descriptor_t  *descriptor,
                           LDMAx_CBFunc_t cbFunc,
                           void *userData )
{
    ldmaCallback[ch].callback = cbFunc;
    ldmaCallback[ch].userdata = userData;

    LDMA_StartTransfer(ch, transfer, descriptor);
}

void LDMA_IRQHandler( void )
{
    uint32_t pending, chnum, chmask;

    /* Get all pending and enabled interrupts */
    pending  = LDMA->IF;
    pending &= LDMA->IEN;

    /* Check for LDMA error */
    if ( pending & LDMA_IF_ERROR )
    {
        /* Loop here to enable the debugger to see what has happened */
        while (1)
            ;
    }

    /* Iterate over all LDMA channels. */
    for ( chnum = 0,                chmask = 1;
          chnum < DMA_CHAN_COUNT;
          chnum++,                  chmask <<= 1 )
    {
        if ( pending & chmask )
        {
            /* Clear interrupt flag. */
            LDMA->IFC = chmask;

            /* Do more stuff here, execute callbacks etc. */
            if ( ldmaCallback[chnum].callback )
            {
                ldmaCallback[chnum].callback(chnum, false, ldmaCallback[chnum].userdata);
            }
        }
    }
}

/***************************************************************************//**
 * @brief
 *   Check if LDMA channel is enabled.
 *
 * @param[in] ch
 *   LDMA channel to check.
 *
 * @return
 *   true if channel is enabled, false if not.
 ******************************************************************************/
bool LDMAx_ChannelEnabled( int ch )
{
    EFM_ASSERT(ch < DMA_CHAN_COUNT);
    uint32_t chMask = 1 << ch;
    return (bool)(LDMA->CHEN & chMask);
}

#endif /* LDMA_PRESENT */
