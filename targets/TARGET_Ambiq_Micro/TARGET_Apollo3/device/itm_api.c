/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#if DEVICE_ITM

#include "hal/itm_api.h"
#include "cmsis.h"
#include "am_bsp.h"

#include <stdbool.h>

/* SWO frequency: 1000 kHz */

#ifndef AM_BSP_GPIO_ITM_SWO
#define AM_GPIO_ITM_SWO             	22

const am_hal_gpio_pincfg_t g_AM_GPIO_ITM_SWO =
{
    .uFuncSel            = AM_HAL_PIN_22_SWO,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
};
#endif 


void itm_init(void)
{
    #ifdef AM_BSP_GPIO_ITM_SWO
    am_bsp_itm_printf_enable();
    #else
    am_bsp_itm_printf_enable(AM_GPIO_ITM_SWO,g_AM_GPIO_ITM_SWO);
    #endif
}

#endif
