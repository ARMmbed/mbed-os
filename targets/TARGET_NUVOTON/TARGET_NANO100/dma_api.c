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

#define NU_PDMA_CH_MAX      6   /* Specify maximum channels of PDMA */
#define NU_PDMA_CH_Pos      1   /* Specify first channel number of PDMA */
#define NU_PDMA_CH_Msk      (((1 << NU_PDMA_CH_MAX) - 1) << NU_PDMA_CH_Pos)

struct nu_dma_chn_s {
    void        (*handler)(uint32_t, uint32_t);
    uint32_t    id;
    uint32_t    event;
};

static int dma_inited = 0;
static uint32_t dma_chn_mask = 0;
static struct nu_dma_chn_s dma_chn_arr[NU_PDMA_CH_MAX];
static const DMAName dmaname_chn_arr[NU_PDMA_CH_MAX] = {
    // NOTE: DMA_0_0 for VDMA
    DMA_1_0, DMA_2_0, DMA_3_0, DMA_4_0, DMA_5_0, DMA_6_0
};

void PDMA_IRQHandler(void);
static const struct nu_modinit_s dma_modinit = {DMAGCR_0, DMA_MODULE, 0, 0, DMA_RST, PDMA_IRQn, (void *) PDMA_IRQHandler};


void dma_init(void)
{
    if (dma_inited) {
        return;
    }
    
    dma_inited = 1;
    dma_chn_mask = ~NU_PDMA_CH_Msk;
    memset(dma_chn_arr, 0x00, sizeof (dma_chn_arr));
    
    // Enable IP clock
    CLK_EnableModuleClock(dma_modinit.clkidx);

    // Reset this module
    SYS_ResetModule(dma_modinit.rsetidx);

    PDMA_Open(0);
    
    NVIC_SetVector(dma_modinit.irq_n, (uint32_t) dma_modinit.var);
    NVIC_EnableIRQ(dma_modinit.irq_n);
}

int dma_channel_allocate(uint32_t capabilities)
{
    if (! dma_inited) {
        dma_init();
    }
    
    int i = nu_cto(dma_chn_mask);
    if (i != 32) {
         dma_chn_mask |= 1 << i;
         memset(dma_chn_arr + i - NU_PDMA_CH_Pos, 0x00, sizeof (struct nu_dma_chn_s));
         return i;
    }

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
    
    dma_chn_arr[channelid - NU_PDMA_CH_Pos].handler = (void (*)(uint32_t, uint32_t)) handler;
    dma_chn_arr[channelid - NU_PDMA_CH_Pos].id = id;
    dma_chn_arr[channelid - NU_PDMA_CH_Pos].event = event;
    
    // Set interrupt vector if someone has removed it.
    NVIC_SetVector(dma_modinit.irq_n, (uint32_t) dma_modinit.var);
    NVIC_EnableIRQ(dma_modinit.irq_n);
}

PDMA_T *dma_modbase(int channelid)
{
    DMAName dma_name = dmaname_chn_arr[channelid - NU_PDMA_CH_Pos];
    return (PDMA_T *) NU_MODBASE(dma_name);
}

void dma_enable(int channelid, int enable)
{
    DMA_GCR_T *dmagcr_base = (DMA_GCR_T *) NU_MODBASE(dma_modinit.modname);
    PDMA_T *pdma_base = dma_modbase(channelid);
    uint32_t pos = channelid - NU_PDMA_CH_Pos + DMA_GCR_GCRCSR_CLK1_EN_Pos;
    
    if (enable) {
        dmagcr_base->GCRCSR |= 1 << pos;                                    // Enable channel clock
        pdma_base->CSR |= (PDMA_CSR_PDMACEN_Msk);                           // Enable channel
    }
    else {
        dmagcr_base->GCRCSR &= ~(1 << pos);                                 // Disable channel clock
        pdma_base->CSR &= ~(PDMA_CSR_PDMACEN_Msk);                          // Disable channel
    }
}

void PDMA_IRQHandler(void)
{
    uint32_t intsts = PDMA_GET_INT_STATUS();
    // Just interested in INTR1-INTR6
    intsts &= ((NU_PDMA_CH_Msk >> NU_PDMA_CH_Pos) << DMA_GCR_GCRISR_INTR1_Pos);
    
    while (intsts) {
        int chn_id = nu_ctz(intsts) - DMA_GCR_GCRISR_INTR1_Pos + NU_PDMA_CH_Pos;
        uint32_t intsts_chn = PDMA_GET_CH_INT_STS(chn_id);
        
        if (dma_chn_mask & (1 << chn_id)) {
            struct nu_dma_chn_s *dma_chn = dma_chn_arr + chn_id - NU_PDMA_CH_Pos;
            
            // Abort
            if (intsts_chn & PDMA_ISR_TABORT_IS_Msk) {
                // Clear ABORT IF of the channel
                PDMA_CLR_CH_INT_FLAG(chn_id, PDMA_ISR_TABORT_IS_Msk);
            
                if (dma_chn->handler && (dma_chn->event & DMA_EVENT_ABORT)) {
                    dma_chn->handler(dma_chn->id, DMA_EVENT_ABORT);
                }
            }
    
            // Transfer done
            if (intsts_chn & PDMA_ISR_TD_IS_Msk) {
                // Clear TD IF of the channel
                PDMA_CLR_CH_INT_FLAG(chn_id, PDMA_ISR_TD_IS_Msk);
            
                if (dma_chn->handler && (dma_chn->event & DMA_EVENT_TRANSFER_DONE)) {
                    dma_chn->handler(dma_chn->id, DMA_EVENT_TRANSFER_DONE);
                }
            }
    
            // Timeout
            if (intsts_chn & PDMA_ISR_TO_IS_Msk) {
                // Clear TIMEOUT IF of the channel
                PDMA_CLR_CH_INT_FLAG(chn_id, PDMA_ISR_TO_IS_Msk);
            
                if (dma_chn->handler && (dma_chn->event & DMA_EVENT_TIMEOUT)) {
                    dma_chn->handler(dma_chn->id, DMA_EVENT_TIMEOUT);
                }
            }
        }
        
        intsts &= ~(1 << (chn_id - NU_PDMA_CH_Pos + DMA_GCR_GCRISR_INTR1_Pos));
    }
}
