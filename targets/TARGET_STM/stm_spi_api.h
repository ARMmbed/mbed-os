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


#ifndef MBED_OS_STM_SPI_API_H
#define MBED_OS_STM_SPI_API_H

#include "spi_device.h"

#if STM32_SPI_CAPABILITY_DMA
#include "stm_dma_utils.h"
#endif

struct spi_s {
    SPI_HandleTypeDef handle;
    IRQn_Type spiIRQ;
    SPIName spi;
    PinName pin_miso;
    PinName pin_mosi;
    PinName pin_sclk;
    PinName pin_ssel;
#if DEVICE_SPI_ASYNCH
    uint32_t event;
    uint8_t transfer_type;
    bool curr_transfer_uses_dma;

    // Callback function for when we get an interrupt on an async transfer.
    // This will point, through a bit of indirection, to SPI::irq_handler_asynch()
    // for the correct SPI instance.
    void (*driverCallback)(void);

#endif
    uint8_t spiIndex; // Index of the SPI peripheral, from 1-6
#if STM32_SPI_CAPABILITY_DMA
    bool txDMAInitialized;
    bool rxDMAInitialized;
#endif
};

#endif //MBED_OS_STM_SPI_API_H
