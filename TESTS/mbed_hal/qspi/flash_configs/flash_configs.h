/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#ifndef MBED_FLASH_CONFIGS_H
#define MBED_FLASH_CONFIGS_H

#if defined(TARGET_DISCO_L475VG_IOT01A)
#include "STM/DISCO_L475VG_IOT01A/flash_config.h"
#elif defined(TARGET_NRF52840)
#include "NORDIC/NRF52840_DK/flash_config.h"
#elif defined(TARGET_DISCO_F413ZH)
#include "STM/DISCO_F413ZH/flash_config.h"
#elif defined(TARGET_EFM32GG11_STK3701)
#include "SiliconLabs/EFM32GG11_STK3701/flash_config.h"
#elif defined(TARGET_K82F)
#include "Freescale/K82F/flash_config.h"
#elif defined(TARGET_KL82Z)
#include "Freescale/KL82Z/flash_config.h"
#endif

#endif // MBED_FLASH_CONFIGS_H
