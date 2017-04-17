/* mbed Microcontroller Library
 * Copyright (c) 2015-2017 Nuvoton
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
 */

#include "dma_api.h"
#include "string.h"
#include "cmsis.h"
#include "mbed_assert.h"
#include "PeripheralNames.h"
#include "nu_modutil.h"
#include "nu_bitutil.h"
#include "dma.h"

#define PDMA_CH_MAX    6   /* Specify Maximum Channels of PDMA */

struct nu_dma_chn_s {
    void        (*handler)(uint32_t, uint32_t);
    uint32_t    id;
    uint32_t    event;
};

static int dma_inited = 0;
static uint32_t dma_chn_mask = 0;
static struct nu_dma_chn_s dma_chn_arr[PDMA_CH_MAX];

void PDMA_IRQHandler(void);
static const struct nu_modinit_s dma_modinit = {DMA_0, DMA_MODULE, 0, 0, DMA_RST, PDMA_IRQn, (void *) PDMA_IRQHandler};


void dma_init(void)
{
    if (dma_inited) {
        return;
    }
    
    dma_inited = 1;
    dma_chn_mask = 0;
    memset(dma_chn_arr, 0x00, sizeof (dma_chn_arr));
    
    // Reset this module
    SYS_ResetModule(dma_modinit.rsetidx);
    
    // Enable IP clock
    CLK_EnableModuleClock(dma_modinit.clkidx);
    
    PDMA_Open(0);
    
    NVIC_SetVector(dma_modinit.irq_n, (uint32_t) dma_modinit.var);
    NVIC_EnableIRQ(dma_modinit.irq_n);
}

int dma_channel_allocate(uint32_t capabilities)
{
    if (! dma_inited) {
        dma_init();
    }
    
#if 1
    int i = nu_cto(dma_chn_mask);
    if (i != 32) {
         dma_chn_mask |= 1 << i;
         memset(dma_chn_arr + i, 0x00, sizeof (struct nu_dma_chn_s));
         return i;
    }
#else
    int i;
    
    for (i = 0; i < PDMA_CH_MAX; i ++) {
        if ((dma_chn_mask & (1 << i)) == 0) {
            // Channel available
            dma_chn_mask |= 1 << i;
            memset(dma_chn_arr + i, 0x00, sizeof (struct nu_dma_chn_s));
            return i;
        }
    }
#endif

    // No channel available
    return DMA_ERROR_OUT_OF_CHANNELS;
}

int dma_channel_free(int channelid)
{
    if (channelid != DMA_ERROR_OUT_OF_CHANNELS) {
        dma_chn_mask &= ~(1 << channelid);
    }
    
    return 0;
}

void dma_set_handler(int channelid, uint32_t handler, uint32_t id, uint32_t event)
{
    MBED_ASSERT(dma_chn_mask & (1 << channelid));
    
    dma_chn_arr[channelid].handler = (void (*)(uint32_t, uint32_t)) handler;
    dma_chn_arr[channelid].id = id;
    dma_chn_arr[channelid].event = event;
    
    // Set interrupt vector if someone has removed it.
    NVIC_SetVector(dma_modinit.irq_n, (uint32_t) dma_modinit.var);
    NVIC_EnableIRQ(dma_modinit.irq_n);
}

PDMA_T *dma_modbase(void)
{
    return (PDMA_T *) NU_MODBASE(dma_modinit.modname);
}

void PDMA_IRQHandler(void)
{
    uint32_t intsts = PDMA_GET_INT_STATUS();
    
    while (intsts) {
        int chn_id = nu_ctz(intsts);
        if (dma_chn_mask & (1 << chn_id)) {
            struct nu_dma_chn_s *dma_chn = dma_chn_arr + chn_id;
            uint32_t ch_intsts = PDMA_GET_CH_INT_STS(chn_id);
            if (dma_chn->handler) {
                // Abort
                if ((ch_intsts & PDMA_ISR_TABORT_IS_Msk) && (dma_chn->event & DMA_EVENT_ABORT)) {
                    dma_chn->handler(dma_chn->id, DMA_EVENT_ABORT);
                }
                // Transfer done
                if ((ch_intsts & PDMA_ISR_TD_IS_Msk) && (dma_chn->event & DMA_EVENT_TRANSFER_DONE)) {
                    dma_chn->handler(dma_chn->id, DMA_EVENT_TRANSFER_DONE);
                }
                // Timeout
                if ((ch_intsts & PDMA_ISR_TO_IS_Msk) && (dma_chn->event & DMA_EVENT_TIMEOUT)) {
                    dma_chn->handler(dma_chn->id, DMA_EVENT_TIMEOUT);
                }
            }
            // Clear all interrupt flags
            PDMA_CLR_CH_INT_FLAG(chn_id, ch_intsts);
        }
        intsts &= ~(1 << chn_id);
    }
}
