/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
  if (DMA_CAP_2DCOPY & capabilities)
  {
    if (channels & 1) 
    {
      // Channel already in use
      return DMA_ERROR_OUT_OF_CHANNELS;
    } else {
      channels |= 1 << 0;
      return 0;
    }
  }
  for (i = 1; i < DMA_CHAN_COUNT; i++)
  {
    if ((channels & (1 << i)) == 0) 
    {
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

