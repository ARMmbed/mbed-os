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

#ifndef _SERCOM_DMA_H
#define _SERCOM_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler.h>
#include "dma.h"


struct sercom_dma {
    uint8_t tx_channel_id;
    uint8_t rx_channel_id;
    enum dma_status_flags tx_status;
    enum dma_status_flags rx_status;
};

/** Allocate a channel for TX
 *
 * @param[in] sercom_index	index of sercom instance
 * @param[out] tx_id		pointer to channel id
 * @return allocated channel id
 */
uint8_t sercom_setup_tx_channel(uint8_t sercom_index, uint8_t *tx_id);

/** Allocate a channel for RX
 *
 * @param[in] sercom_index	index of sercom instance
 * @param[out] rx_id		pointer to channel id
 * @return allocated channel id
 */
uint8_t sercom_setup_rx_channel(uint8_t sercom_index, uint8_t *rx_id);

/** Release DMA channels if allocated
 *
 * @param[in] sercom_index	index of sercom instance
 * @return void
 */
void sercom_release_channel(uint8_t sercom_index);

#ifdef __cplusplus
}
#endif

#endif /* _SERCOM_DMA_H */
