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
 * This header contains constants and defines specific to processors with the v2 DMA IP.
 *
 * The v2 DMA IP has DMA controllers with multiple channels, where each channel has a request source
 * that determines what triggers DMA transactions
 */

#ifndef MBED_OS_STM_DMA_IP_V2_H
#define MBED_OS_STM_DMA_IP_V2_H

#ifdef TARGET_MCU_STM32F0

// STM32F0 is weird and does its own thing.
// Only 5 channels usable, the other 2 lack interrupts
#define MAX_DMA_CHANNELS_PER_CONTROLLER 5

#else

// Devices with DMA IP v2 have at most 7 channels per controller.
#define MAX_DMA_CHANNELS_PER_CONTROLLER 7

#endif

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

// On some smaller devices, e.g. STM32L1 family, DMA channels are simply logically ORed rather than
// muxed, so we don't need the "sourceNumber" field.
// We can check if this is the case by the absence of specific peripherals/registers.
#if defined(DMA1_CSELR) || defined(DMAMUX1_BASE)
#define STM_DEVICE_HAS_DMA_SOURCE_SELECTION 1
#else
#define STM_DEVICE_HAS_DMA_SOURCE_SELECTION 0
#endif

#endif //MBED_OS_STM_DMA_IP_V2_H
