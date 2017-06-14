/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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

#include <stddef.h>
#include "gpio_api.h"
#include "pinmap.h"

#define GPIO_PIN_POS_MASK 0x0F /* pin % 16 */
#define RESERVED_MISC_PIN 7

/* \brief Gets the FPGA MISC (Miscellaneous control) bit position for the given
 *        pin name
 *
 *  FPGA MISC bit mapping:
 *    [31:7] Reserved
 *    [6] CLCD_BL_CTRL
 *    [5] CLCD_RD
 *    [4] CLCD_RS
 *    [3] CLCD_RESET
 *    [2] Reserved
 *    [1] SPI_nSS
 *    [0] CLCD_CS
 *
 *  \param[in] pin  MISC pin name
 *
 *  \return FPGA MISC bit position
 */
static uint8_t get_fpga_misc_pin_pos(PinName pin)
{
    uint8_t pin_position = RESERVED_MISC_PIN;

    if (pin == SPI_SCLK) {
        pin_position = 0;
    } else if (pin == CLCD_SSEL) {
        pin_position = 1;
    } else if (pin == CLCD_RESET) {
        pin_position = 3;
    } else if (pin == CLCD_RS) {
        pin_position = 4;
    } else if (pin == CLCD_RD) {
        pin_position = 5;
    } else if (pin == CLCD_BL_CTRL){
        pin_position = 6;
    }

    return pin_position;
}

uint32_t gpio_set(PinName pin)
{
    uint8_t pin_position;

    if (pin >=EXP0 && pin <= EXP51) {
        /* Set pin functinality as GPIO. pin_function asserts if pin == NC */
        pin_function(pin, GPIO_FUNC);
    } else {
        /* Check if pin is a MISC pin */
        pin_position = get_fpga_misc_pin_pos(pin);
        if (pin_position != RESERVED_MISC_PIN) {
            return (1 << pin_position);
        }
    }

    /* Return pin mask */
    return (1 << (pin & 0xFF));
}

void gpio_init(gpio_t *obj, PinName pin)
{
    uint8_t pin_position;

    if (pin == NC) {
       return;
    }

    obj->pin = pin;
    obj->pin_number = pin;

    if (pin <= EXP15) {
        obj->reg_data   = &CMSDK_GPIO0->DATAOUT;
        obj->reg_in     = &CMSDK_GPIO0->DATA;
        obj->reg_dir    = &CMSDK_GPIO0->OUTENABLESET;
        obj->reg_dirclr = &CMSDK_GPIO0->OUTENABLECLR;
        /* Set pin function as a GPIO */
        pin_function(pin, GPIO_FUNC);
        pin_position = pin;
    } else if (pin >= EXP16 && pin <= EXP31) {
        obj->reg_data   = &CMSDK_GPIO1->DATAOUT;
        obj->reg_in     = &CMSDK_GPIO1->DATA;
        obj->reg_dir    = &CMSDK_GPIO1->OUTENABLESET;
        obj->reg_dirclr = &CMSDK_GPIO1->OUTENABLECLR;
        /* Set pin function as a GPIO */
        pin_function(pin, GPIO_FUNC);
        pin_position = (pin & GPIO_PIN_POS_MASK);
    } else if (pin >= EXP32 && pin <= EXP47) {
        obj->reg_data   = &CMSDK_GPIO2->DATAOUT;
        obj->reg_in     = &CMSDK_GPIO2->DATA;
        obj->reg_dir    = &CMSDK_GPIO2->OUTENABLESET;
        obj->reg_dirclr = &CMSDK_GPIO2->OUTENABLECLR;
        /* Set pin function as a GPIO */
        pin_function(pin, GPIO_FUNC);
        pin_position = (pin & GPIO_PIN_POS_MASK);
    } else if (pin >= EXP48 && pin <= EXP51) {
        obj->reg_data   = &CMSDK_GPIO3->DATAOUT;
        obj->reg_in     = &CMSDK_GPIO3->DATA;
        obj->reg_dir    = &CMSDK_GPIO3->OUTENABLESET;
        obj->reg_dirclr = &CMSDK_GPIO3->OUTENABLECLR;
        /* Set pin function as a GPIO */
        pin_function(pin, GPIO_FUNC);
        pin_position = (pin & GPIO_PIN_POS_MASK);
    } else if (pin == 100 || pin == 101) {
        /* User LEDs */
        pin_position = (pin - 100);
        obj->reg_data   = &MPS2_FPGAIO->LED;
        obj->reg_in     = &MPS2_FPGAIO->LED;
        obj->reg_dir    = NULL;
        obj->reg_dirclr = NULL;
    } else if (pin == 110 || pin == 111) {
        /* User buttons */
        pin_position = (pin - 110);
        obj->reg_data   = &MPS2_FPGAIO->BUTTON;
        obj->reg_in     = &MPS2_FPGAIO->BUTTON;
        obj->reg_dir    = NULL;
        obj->reg_dirclr = NULL;
    } else if (pin >= 200 && pin <= 207) {
        /* MCC LEDs */
        pin_position = (pin - 200);
        obj->reg_data   = &MPS2_SCC->LEDS;
        obj->reg_in     = &MPS2_SCC->LEDS;
        obj->reg_dir    = NULL;
        obj->reg_dirclr = NULL;
    } else if (pin >= 210 && pin <= 217) {
        /* MCC switches */
        pin_position = (pin - 210);
        obj->reg_in     = &MPS2_SCC->SWITCHES;
        obj->reg_data   = NULL;
        obj->reg_dir    = NULL;
        obj->reg_dirclr = NULL;
    } else {
        /* Check if pin is a MISC pin */
        pin_position = get_fpga_misc_pin_pos(pin);
        if (pin_position != RESERVED_MISC_PIN) {
            obj->reg_data = &MPS2_FPGAIO->MISC;
        } else {
            pin_position = 0;
        }
    }

    /* Set pin mask */
    obj->mask = (1 << pin_position);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    if (obj->pin >= EXP0 && obj->pin <= EXP51) {
        switch (direction) {
            case PIN_INPUT :
                *obj->reg_dirclr = obj->mask;
                break;
            case PIN_OUTPUT:
                *obj->reg_dir |= obj->mask;
                break;
        }
    }
}

int gpio_is_connected(const gpio_t *obj)
{
    if (obj->pin != (PinName)NC) {
        return 1;
    }

    return 0;
}

