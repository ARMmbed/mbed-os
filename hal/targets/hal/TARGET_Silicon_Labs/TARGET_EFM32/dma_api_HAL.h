/***************************************************************************//**
 * @file dma_api_HAL.h
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
