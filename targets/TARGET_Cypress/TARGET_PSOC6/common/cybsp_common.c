/***************************************************************************//**
* \file cybsp_utils.c
*
* \brief
* Provides utility functions that are used by board support packages.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
*******************************************************************************/

#include "cybsp_api_core.h"
#include "cyhal.h"
#include "cyhal_implementation.h"

#if defined(__cplusplus)
extern "C" {
#endif

static void (*btn_interrupt_call_back) (void);

cy_rslt_t cybsp_led_init(cybsp_led_t which)
{
    return cyhal_gpio_init((cyhal_gpio_t)which, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
}

void cybsp_led_set_state(cybsp_led_t which, bool on)
{
    cyhal_gpio_write((cyhal_gpio_t)which, on);
}

void cybsp_led_toggle(cybsp_led_t which)
{
    cyhal_gpio_toggle((cyhal_gpio_t)which);
}

cy_rslt_t cybsp_btn_init(cybsp_btn_t which)
{
    return cyhal_gpio_init((cyhal_gpio_t)which, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);
}

bool cybsp_btn_get_state(cybsp_btn_t which)
{
    return cyhal_gpio_read((cyhal_gpio_t)which);
}

static void gpio_call_back_wrapper(void *handler_arg, cyhal_gpio_irq_event_t event)
{
    if (btn_interrupt_call_back != NULL)
    {
        btn_interrupt_call_back();
    }
}

void cybsp_btn_set_interrupt(cybsp_btn_t which, cyhal_gpio_irq_event_t type, void (*callback)(void))
{
    btn_interrupt_call_back = callback;
    cyhal_gpio_register_irq((cyhal_gpio_t)which, 7, &gpio_call_back_wrapper, NULL);
    cyhal_gpio_irq_enable((cyhal_gpio_t)which, type, 1);
}


#if defined(__cplusplus)
}
#endif
