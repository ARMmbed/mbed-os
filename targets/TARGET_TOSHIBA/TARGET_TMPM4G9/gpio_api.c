/* mbed Microcontroller Library
 *
 * Copyright (C) 2019, Toshiba Electronic Device Solutions Corporation
 *
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
#include "gpio_api.h"
#include "PeripheralNames.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "txz_gpio.h"

#define GPIO_DATA                       PIN_DATA(0, 2)
#define ALT_FUNC_GPIO                   0

_gpio_t gpio_port_add = {
    .p_pa_instance = TSB_PA,
    .p_pb_instance = TSB_PB,
    .p_pc_instance = TSB_PC,
    .p_pd_instance = TSB_PD,
    .p_pe_instance = TSB_PE,
    .p_pf_instance = TSB_PF,
    .p_pg_instance = TSB_PG,
    .p_ph_instance = TSB_PH,
    .p_pj_instance = TSB_PJ,
    .p_pk_instance = TSB_PK,
    .p_pl_instance = TSB_PL,
    .p_pm_instance = TSB_PM,
    .p_pn_instance = TSB_PN,
    .p_pp_instance = TSB_PP,
    .p_pr_instance = TSB_PR,
    .p_pt_instance = TSB_PT,
    .p_pu_instance = TSB_PU,
    .p_pv_instance = TSB_PV,
    .p_pw_instance = TSB_PW,
    .p_py_instance = TSB_PY
};

uint32_t gpio_set(PinName pin)
{
    // Check that pin is valid
    MBED_ASSERT(pin != (PinName)NC);

    // Set pin function as GPIO pin
    pin_function(pin, GPIO_DATA);

    // Return pin mask
    return (1 << (pin & 0x07));
}

void gpio_init(gpio_t *obj, PinName pin)
{
    // Store above pin mask, pin name into GPIO object
    obj->pin      = pin;
    if (pin == (PinName)NC) {
        return;
    }
    obj->pin_num  = PIN_POS(pin);
    obj->mask     = gpio_set(pin);
    obj->port     = (PortName) PIN_PORT(pin);
    // Enable clock for particular port
    TSB_CG->FSYSMENB |= (1 << ((obj->port) + 2));
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    // Set pin mode
    pin_mode(obj->pin, mode);
}

// Set gpio object pin direction
void gpio_dir(gpio_t *obj, PinDirection direction)
{
    // Set direction
    switch (direction) {
        case PIN_INPUT:
            // Set pin input
            gpio_func(&gpio_port_add,
                      (gpio_gr_t)obj->port,
                      (gpio_num_t)obj->pin_num,
                      (uint32_t)ALT_FUNC_GPIO,
                      GPIO_PIN_INPUT);
            break;
        case PIN_OUTPUT:
            // Set pin output
            gpio_func(&gpio_port_add,
                      (gpio_gr_t)obj->port,
                      (gpio_num_t)obj->pin_num,
                      (uint32_t)ALT_FUNC_GPIO,
                      GPIO_PIN_OUTPUT);
            break;
        case PIN_INOUT:
            // Set pin both input and output
            gpio_func(&gpio_port_add,
                      (gpio_gr_t)obj->port,
                      (gpio_num_t)obj->pin_num,
                      (uint32_t)ALT_FUNC_GPIO,
                      GPIO_PIN_INOUT);
            break;
        default:
            break;
    }
}

void gpio_write(gpio_t *obj, int value)
{
    // Write gpio object pin data
    gpio_write_bit(&gpio_port_add, (gpio_gr_t)obj->port, (gpio_num_t)obj->pin_num, GPIO_Mode_DATA, value);
}

int gpio_read(gpio_t *obj)
{
    // Read gpio object pin data
    gpio_pinstate_t val = GPIO_PIN_SET; // To initialize local variable
    gpio_read_bit(&gpio_port_add, (gpio_gr_t)obj->port, (gpio_num_t)obj->pin_num, GPIO_Mode_DATA, &val);
    return val;
}
