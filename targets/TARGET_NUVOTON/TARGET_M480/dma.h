/*
 * Copyright (c) 2015-2016, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef MBED_DMA_H
#define MBED_DMA_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DMA_CAP_NONE    (0 << 0)

#define DMA_EVENT_ABORT             (1 << 0)
#define DMA_EVENT_TRANSFER_DONE     (1 << 1)
#define DMA_EVENT_TIMEOUT           (1 << 2)
#define DMA_EVENT_ALL               (DMA_EVENT_ABORT | DMA_EVENT_TRANSFER_DONE | DMA_EVENT_TIMEOUT)
#define DMA_EVENT_MASK              DMA_EVENT_ALL

void dma_set_handler(int channelid, uint32_t handler, uint32_t id, uint32_t event);
PDMA_T *dma_modbase(void);

#ifdef __cplusplus
}
#endif

#endif
