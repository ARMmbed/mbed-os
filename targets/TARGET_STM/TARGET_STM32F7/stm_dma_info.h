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

// See STM32F7 reference manual Tables 27 and 28.
// Note: For each SPI and each direction, there are two possible assignments to a DMA channel.
// We need to assign them here so that no combination would ever conflict and use the same DMA channel.

// Exception: SPI5 and SPI6 use the same DMA channels in hardware and there's no way to deconflict them.
// So, SPI5 and SPI6 cannot be used with DMA at the same time.

/// Mapping from SPI index to DMA link info for Tx
static const DMALinkInfo SPITxDMALinks[] = {
        {2, 3, 3}, // SPI1 Tx is DMA2 Stream 3 Channel 3
        {1, 4, 0}, // SPI2 Tx is DMA1 Stream 4 Channel 0
        {1, 5, 0}, // SPI3 Tx is DMA1 Stream 5 Channel 0
        {2, 1, 4}, // SPI4 Tx is DMA2 Stream 1 Channel 4
        {2, 6, 7}, // SPI5 Tx is DMA2 Stream 6 Channel 7
        {2, 5, 1}, // SPI6 Tx is DMA2 Stream 5 Channel 1
};

/// Mapping from SPI index to DMA link info for Rx
static const DMALinkInfo SPIRxDMALinks[] = {
        {2, 2, 3}, // SPI1 Rx is DMA2 Stream 2 Channel 3
        {1, 3, 0}, // SPI2 Rx is DMA1 Stream 3 Channel 0
        {1, 0, 0}, // SPI3 Rx is DMA1 Stream 0 Channel 0
        {2, 0, 4}, // SPI4 Rx is DMA2 Stream 0 Channel 4
        {2, 5, 7}, // SPI5 Rx is DMA2 Stream 5 Channel 7
        {2, 6, 1}, // SPI6 Rx is DMA2 Stream 6 Channel 1
};

#endif //MBED_OS_STM_DMA_INFO_H
