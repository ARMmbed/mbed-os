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
 * This header contains constants and defines specific to processors with the v3 DMA IP.
 *
 * The v3 DMA IP has one DMA controller with multiple channels, where each channel has a request source
 * that determines what triggers DMA transactions.  Any DMA channel can connect to any request source,
 * unlike many other STM32 chips.
 */

#ifndef MBED_OS_STM_DMA_IP_V3_H
#define MBED_OS_STM_DMA_IP_V3_H

// Devices with DMA IP v3 have at most 16 channels per controller.
#define MAX_DMA_CHANNELS_PER_CONTROLLER 16

#define NUM_DMA_CONTROLLERS 1

// Currently all known IPv3 devices have source selection
#define STM_DEVICE_HAS_DMA_SOURCE_SELECTION 1

#endif //MBED_OS_STM_DMA_IP_V3_H
