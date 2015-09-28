/***************************************************************************//**
 * @file dma_api.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#include <stdint.h>
#include "dma_api_HAL.h"
#include "em_dma.h"
#include "em_cmu.h"

/** DMA control block array, requires proper alignment. */
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

uint32_t channels = 0; // Bit vector of taken channels
bool enabled = false;

void dma_init(void)
{
    if (enabled) return;
    DMA_Init_TypeDef   dmaInit;

    CMU_ClockEnable(cmuClock_DMA, true);
    CMU_ClockEnable(cmuClock_HFPER, true);

    /* Configure general DMA issues */
    dmaInit.hprot        = 0;
    dmaInit.controlBlock = dmaControlBlock;
    DMA_Init(&dmaInit);
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
    channels &= ~(1 << channelid);
    return 0;
}

