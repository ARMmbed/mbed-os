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

/*
 * This header contains constants and defines specific to processors with the v1 DMA IP.
 * The v1 IP has DMA controllers with multiple streams, where each "stream" has a "channel selection"
 * to determine what triggers DMA requests.
 */

#ifndef MBED_OS_STM_DMA_IP_V1_H
#define MBED_OS_STM_DMA_IP_V1_H

// Devices with DMA IP v1 have at most 8 channels per controller.
#define MAX_DMA_CHANNELS_PER_CONTROLLER 8

// Count DMA controllers
#ifdef DMA1
#ifdef DMA2
#define NUM_DMA_CONTROLLERS 2
#else
#define NUM_DMA_CONTROLLERS 1
#endif
#else
#define NUM_DMA_CONTROLLERS 0
#endif

// Provide an alias so that code can always use the v2 name for this structure
#define DMA_Channel_TypeDef DMA_Stream_TypeDef

// On some smaller devices, e.g. STM32L1 family, DMA channels are simply logically ORed rather than
// muxed, so we don't need the "sourceNumber" field.
// We can check if this is the case by the absence of specific peripherals/registers.
#if defined(DMAMUX1_BASE) || defined(DMA_SxCR_CHSEL_Msk)
#define STM_DEVICE_HAS_DMA_SOURCE_SELECTION 1
#else
#define STM_DEVICE_HAS_DMA_SOURCE_SELECTION 0
#endif

#endif //MBED_OS_STM_DMA_IP_V1_H
