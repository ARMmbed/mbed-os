/* mbed Microcontroller Library
 * Copyright (c) 2021 ARM Limited
 * Copyright (c) 2021 Embedded Planet, Inc.
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

#include <stdint.h>

#include "subtarget_init.h"

#include "nrf.h"

/**
 * Override the subtarget sdk init startup hook (specific to nRF2)
 * This will configure the internal regulator to operate at 3.3V
 */
void subtarget_sdk_init(void)
{

    if (NRF_UICR->REGOUT0 != UICR_REGOUT0_VOUT_3V3) {
        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}
        NRF_UICR->REGOUT0 = UICR_REGOUT0_VOUT_3V3;
        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}

        // Trigger a soft reset so that the settings take effect
        NVIC_SystemReset();
    }
}


