/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "mbed_assert.h"

#include "cmsis.h"
#include "sercom.h"
#include "dma_api.h"
#include "dma_api_HAL.h"
#include "sercom_dma.h"

static struct sercom_dma sercom_channels[SERCOM_INST_NUM] = {{0}};

/** Allocate a channel for TX
 *
 * @param[in] sercom_index	index of sercom instance
 * @param[out] tx_id		pointer to channel id
 * @return allocated channel id
 */
uint8_t sercom_setup_tx_channel(uint8_t sercom_index, uint8_t *tx_id)
{
    if (sercom_channels[sercom_index].tx_status & DMA_ALLOCATED) {
        *tx_id = sercom_channels[sercom_index].tx_channel_id;
    } else {
        /* Try to allocate a channel */
        sercom_channels[sercom_index].tx_channel_id = dma_channel_allocate(DMA_CAP_NONE);
        if (sercom_channels[sercom_index].tx_channel_id != (uint8_t)DMA_ERROR_OUT_OF_CHANNELS) {
            *tx_id = sercom_channels[sercom_index].tx_channel_id;
            sercom_channels[sercom_index].tx_status = DMA_ALLOCATED;
        } else {
            /* Couldn't find a channel */
            return DMA_ERROR_OUT_OF_CHANNELS;
        }
    }
    return *tx_id;
}

/** Allocate a channel for RX
 *
 * @param[in] sercom_index	index of sercom instance
 * @param[out] rx_id		pointer to channel id
 * @return allocated channel id
 */
uint8_t sercom_setup_rx_channel(uint8_t sercom_index, uint8_t *rx_id)
{
    if (sercom_channels[sercom_index].rx_status & DMA_ALLOCATED) {
        *rx_id = sercom_channels[sercom_index].rx_channel_id;
    } else {
        /* Try to allocate a channel */
        sercom_channels[sercom_index].rx_channel_id = dma_channel_allocate(DMA_CAP_NONE);
        if (sercom_channels[sercom_index].rx_channel_id != (uint8_t)DMA_ERROR_OUT_OF_CHANNELS) {
            *rx_id = sercom_channels[sercom_index].rx_channel_id;
            sercom_channels[sercom_index].rx_status = DMA_ALLOCATED;
        } else {
            /* Couldn't find a channel */
            return DMA_ERROR_OUT_OF_CHANNELS;
        }
    }
    return *rx_id;
}

/** Release DMA channels if allocated
 *
 * @param[in] sercom_index	index of sercom instance
 * @return void
 */
void sercom_release_channel(uint8_t sercom_index)
{
    if (sercom_channels[sercom_index].rx_status & DMA_ALLOCATED) {
        if (0 == dma_channel_free(sercom_channels[sercom_index].rx_channel_id)) {
            sercom_channels[sercom_index].rx_status = DMA_NOT_USED;
        }
    }
    if (sercom_channels[sercom_index].tx_status & DMA_ALLOCATED) {
        if (0 == dma_channel_free(sercom_channels[sercom_index].tx_channel_id)) {
            sercom_channels[sercom_index].tx_status = DMA_NOT_USED;
        }
    }
}
