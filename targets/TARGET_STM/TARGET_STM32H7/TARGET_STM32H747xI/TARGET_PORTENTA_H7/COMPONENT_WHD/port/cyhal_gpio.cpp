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
 *  Provides the porting layer for STM32 GPIOs on WHD driver
 */

#include <stdio.h>
#include "cyhal.h"
#include "cybsp.h"
#include "mbed.h"
#include "wiced_filesystem.h"

extern pinconfig_t      PinConfig[];

/*******************************************************************************
*       Internal
*******************************************************************************/
static mbed::InterruptIn *oob_irq;
static cyhal_gpio_event_t oob_event = CYHAL_GPIO_IRQ_FALL;
static cyhal_gpio_event_callback_t oob_handler;
static void *oob_handler_arg;

/*******************************************************************************
*       Internal Interrrupt Service Routine
*******************************************************************************/
static void cb()
{
    oob_handler(oob_handler_arg, oob_event);
}

/*******************************************************************************
*       HAL Implementation
*******************************************************************************/
cy_rslt_t cyhal_gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drvMode, bool initVal)
{
    cy_rslt_t     ret = CY_RSLT_SUCCESS;

    // Workaround to enable GPIOJ clock
    if (pin == CYBSP_WIFI_WL_REG_ON) {
        __HAL_RCC_GPIOJ_CLK_ENABLE();
        // Ensure FS and BlockDevice are initialized on time if needed
        wiced_filesystem_init();
    }
    // Ignore the parameter and take the pin config directly from a static array defintions
    HAL_GPIO_Init(PinConfig[pin].port, &PinConfig[pin].config);
    if (direction == CYHAL_GPIO_DIR_OUTPUT) {
        HAL_GPIO_WritePin(PinConfig[pin].port, PinConfig[pin].config.Pin, (initVal) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
    // Workaround to initialize sdio interface without cypress bsp init
    if (pin == CYBSP_WIFI_WL_REG_ON) {
        cyhal_sdio_t *sdio_p = cybsp_get_wifi_sdio_obj();
        ret = cyhal_sdio_init(sdio_p, CYBSP_WIFI_SDIO_CMD, CYBSP_WIFI_SDIO_CLK, CYBSP_WIFI_SDIO_D0, CYBSP_WIFI_SDIO_D1, CYBSP_WIFI_SDIO_D2, CYBSP_WIFI_SDIO_D3);
    }
    return ret;
}

void cyhal_gpio_write(cyhal_gpio_t pin, bool value)
{
    HAL_GPIO_WritePin(PinConfig[pin].port, PinConfig[pin].config.Pin, (value) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void cyhal_gpio_register_callback(cyhal_gpio_t pin, cyhal_gpio_event_callback_t handler, void *handler_arg)
{
    if (handler && handler_arg && (oob_irq == NULL)) {
        oob_irq = new mbed::InterruptIn(PJ_5);
        oob_handler = handler;
        oob_handler_arg = handler_arg;
    }
}

void cyhal_gpio_enable_event(cyhal_gpio_t pin, cyhal_gpio_event_t event, uint8_t intr_priority, bool enable)
{
    oob_event = event;
    if (enable) {
        if (event == CYHAL_GPIO_IRQ_RISE) {
            oob_irq->rise(cb);
        }
        if (event == CYHAL_GPIO_IRQ_FALL) {
            oob_irq->fall(cb);
        }
    } else if (oob_irq != NULL) {
        delete oob_irq;
    }
}

void cyhal_gpio_free(cyhal_gpio_t pin)
{
    // Do nothing
}
