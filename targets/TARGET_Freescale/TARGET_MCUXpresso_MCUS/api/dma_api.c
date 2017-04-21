/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_dmamux.h"
#include "dma_api.h"

uint32_t channels = 0; // Bit vector of taken channels

void dma_init(void)
{
    /* DMA MUX init */
    DMAMUX_Init(DMAMUX0);
}

int dma_channel_allocate(uint32_t capabilities)
{
    int i;

    for (i = 0; i < FSL_FEATURE_DMAMUX_DMAMUX_CHANNELS; i++) {
        if ((channels & (1 << i)) == 0) {
            // Channel available
            channels |= 1 << i;
            /* Check if we need to set the source and enable the MUX for this channel */
            if (capabilities != kDmaRequestMux0Disable) {
                DMAMUX_SetSource(DMAMUX0, i, capabilities);
                DMAMUX_EnableChannel(DMAMUX0, i);
            }

            return i;
        }
    }

    // Couldn't find a channel.
    return DMA_ERROR_OUT_OF_CHANNELS;
}

int dma_channel_free(int channelid)
{
    channels &= ~(1 << channelid);
    DMAMUX_DisableChannel(DMAMUX0, channelid);

    return 0;
}
