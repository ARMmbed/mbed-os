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

#ifndef _DMA_API_HAL_H
#define _DMA_API_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler.h>
#include "dma.h"

enum dma_status_flags {
    DMA_NOT_USED		= (uint32_t)1,
    DMA_ALLOCATED		= (DMA_NOT_USED << 1),
    DMA_TEMPORARY		= (DMA_NOT_USED << 2),
    DMA_ERROR			= (DMA_NOT_USED << 3),
};

/* No other capabilities supported now */
#define DMA_CAP_NONE				0

#define DMA_ADDRESS_INC_DISABLE		0
#define DMA_ADDRESS_INC_ENABLE		1

#define DMA_TRANSFER_ERROR		((uint32_t)1 << 1)
#define DMA_TRANSFER_COMPLETE	((uint32_t)1 << 2)

#define DMA_EVENT_ALL (DMA_TRANSFER_ERROR | DMA_TRANSFER_COMPLETE)


COMPILER_ALIGNED(16)
struct dma_instance_s {
    struct dma_resource resource;
    DmacDescriptor descriptor;
    uint8_t status;
    uint32_t events;
    uint32_t handler;
};

/** Setup a DMA descriptor for specified resource
 *
 * @param[in] channel_index		DMA channel id
 * @param[in] src				source address
 * @param[in] src_inc_enable	source address auto increment enable flag
 * @param[in] desc				destination address
 * @param[in] desc_inc_enable	destination address auto increment enable flag
 * @param[in] length			length of data to be transferred
 * @param[in] beat_size			beat size to be set
 * @return void
 */
void dma_setup_transfer(uint8_t channelid, uint32_t src, bool src_inc_enable, uint32_t desc, bool desc_inc_enable, uint32_t length, uint8_t beat_size);

/** Start DMA transfer
 *
 * Kick starts transfer in DMA channel with specified channel id
 * @param[in] channelid    Channel id of DMA channel
 * @return non zero if success otherwise zero
 */
bool dma_start_transfer(int channelid);

/** DMA channel busy check
 *
 * To check whether DMA channel is busy with a job or not
 * @param[in] channelid    Channel id of DMA channel
 * @return non zero if busy otherwise zero
 */
bool dma_busy(int channelid);

/** DMA channel transfer completion check
 *
 * To check whether DMA channel job is completed or not
 * @param[in] channelid    Channel id of DMA channel
 * @return non zero if busy otherwise zero
 */
bool dma_is_transfer_complete(int channelid);

/** Registers callback function for DMA
 *
 * Registers callback function for DMA for specified events
 * @param[in] channelid    Channel id of DMA channel
 * @param[in] handler      Callback function pointer
 * @param[in] event        Events mask
 * @return void
 */
void dma_set_handler(int channelid, uint32_t handler, uint32_t event);

#ifdef __cplusplus
}
#endif

#endif /* _DMA_API_HAL_H */
