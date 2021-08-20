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
#include "whd_config.h"
#include "cyhal_hw_types.h"
#include "cyhal_gpio.h"
#include "cyhal_sdio.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum
{
  CYBSP_WIFI_WL_REG_ON,
  CYBSP_WIFI_32K_CLK,
  CYBSP_LED1,
  CYBSP_LED2,
  CYBSP_WIFI_SDIO_CMD,
  CYBSP_WIFI_SDIO_CLK,
  CYBSP_WIFI_SDIO_D0,
  CYBSP_WIFI_SDIO_D1,
  CYBSP_WIFI_SDIO_D2,
  CYBSP_WIFI_SDIO_D3,
  CYBSP_SDIO_OOB_IRQ,
  CYBSP_WIFI_MAX,
} wwd_sdio_pin_t;

void cyhal_system_delay_ms(uint32_t milliseconds);
cy_rslt_t sdio_enable_high_speed(void);

#if defined(__cplusplus)
}
#endif

