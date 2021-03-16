/*
 * Copyright 2020 Arduino SA
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

/** @file
 *  Provides cyhal porting to generic mbed APIs
 */
#include "cyhal.h"
#include "mbed_thread.h"
#include "mbed_wait_api.h"

static cyhal_sdio_t sdio_obj;

/* Edit  Pin configuration */
const pinconfig_t PinConfig[] = {

    [CYBSP_WIFI_WL_REG_ON] = WIFI_WL_REG_ON,
#ifdef CYBSP_WIFI_32K_CLK
    [CYBSP_WIFI_32K_CLK]  =  WIFI_32K_CLK,
#endif /* CYBSP_WIFI_32K_CLK */
    [CYBSP_LED1    ]      =  BSP_LED1,
    [CYBSP_LED2    ]      =  BSP_LED2,
    [CYBSP_WIFI_SDIO_CMD] =  WIFI_SDIO_CMD,
    [CYBSP_WIFI_SDIO_CLK] =  WIFI_SDIO_CLK,
    [CYBSP_WIFI_SDIO_D0 ] =  WIFI_SDIO_D0,
    [CYBSP_WIFI_SDIO_D1 ] =  WIFI_SDIO_D1,
    [CYBSP_WIFI_SDIO_D2 ] =  WIFI_SDIO_D2,
    [CYBSP_WIFI_SDIO_D3 ] =  WIFI_SDIO_D3,
    [CYBSP_SDIO_OOB_IRQ ] =  WIFI_SDIO_OOB_IRQ //VIKR
};

void Cy_SysLib_Delay(uint32_t milliseconds)
{
    thread_sleep_for(milliseconds);
}

void Cy_SysLib_DelayUs(uint16_t microseconds)
{
    wait_us(microseconds);
}

void cyhal_system_delay_ms(uint32_t milliseconds)
{
    Cy_SysLib_Delay(milliseconds);
}

cyhal_sdio_t *cybsp_get_wifi_sdio_obj(void)
{
    return &sdio_obj;
}
