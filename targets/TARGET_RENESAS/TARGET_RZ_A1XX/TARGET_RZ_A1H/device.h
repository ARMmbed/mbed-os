// The 'features' section in 'target.json' is now used to create the device's hardware preprocessor switches.
// Check the 'features' section of the target description in 'targets.json' for more details.
/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_DEVICE_H
#define MBED_DEVICE_H

/* ->Take measures about optimization problems of web compiler */
   /* Web compiler has problem that inlining code may not be generated correctly */
   /* when "-O3 -Otime" was specified. */
#if defined(__CC_ARM) && (__ARMCC_VERSION <= 5040027)
#pragma Ospace
#endif
/* <-Take measures about optimization problems of web compiler */






#define TRANSACTION_QUEUE_SIZE_SPI 16





#define DEVICE_ID_LENGTH            32
#define DEVICE_MAC_OFFSET           20





#include "objects.h"
#include "dma_api.h"

#endif
