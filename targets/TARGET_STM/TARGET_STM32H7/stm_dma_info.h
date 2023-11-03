/* mbed Microcontroller Library
 * Copyright (c) 2016-2023 STMicroelectronics
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

#ifndef MBED_OS_STM_DMA_INFO_H
#define MBED_OS_STM_DMA_INFO_H

#include "cmsis.h"
#include "stm_dma_utils.h"

// STM32H7 devices, with DMAMUX feature.
// On this device, the DMA channels may be chosen arbitrarily.

/// Mapping from SPI index to DMA link info for Tx
static const DMALinkInfo SPITxDMALinks[] = {
        {1, 1, DMA_REQUEST_SPI1_TX},
        {1, 3, DMA_REQUEST_SPI2_TX},
        {1, 5, DMA_REQUEST_SPI3_TX},
        {1, 7, DMA_REQUEST_SPI4_TX},
        {2, 1, DMA_REQUEST_SPI5_TX},
};

/// Mapping from SPI index to DMA link info for Rx
static const DMALinkInfo SPIRxDMALinks[] = {
        {1, 0, DMA_REQUEST_SPI1_RX},
        {1, 2, DMA_REQUEST_SPI2_RX},
        {1, 4, DMA_REQUEST_SPI3_RX},
        {1, 6, DMA_REQUEST_SPI4_RX},
        {2, 0, DMA_REQUEST_SPI5_RX},
};


#endif //MBED_OS_STM_DMA_INFO_H
