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

#define GPIO_NUM  4

CMSDK_GPIO_TypeDef* GPIO_MAP[GPIO_NUM] = {
    CMSDK_GPIO0,
    CMSDK_GPIO1,
    CMSDK_GPIO2,
    CMSDK_GPIO3
};

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
    /* TODO */
    return 1;
}

void gpio_init(gpio_t *obj, PinName pin)
{
    uint8_t pin_position = 0;

    if (pin == NC) {
       return;
    }

    if(pin <= 15){
        pin_position = pin;
        obj->reg_data   = &CMSDK_GPIO0->DATAOUT;
        obj->reg_in     = &CMSDK_GPIO0->DATA;
        obj->reg_dir    = &CMSDK_GPIO0->OUTENABLESET;
        obj->reg_dirclr = &CMSDK_GPIO0->OUTENABLECLR;
    } else if (pin >= 16 && pin <= 31) {
        pin_position = (pin - 16);
        obj->reg_data   = &CMSDK_GPIO1->DATAOUT;
        obj->reg_in     = &CMSDK_GPIO1->DATA;
        obj->reg_dir    = &CMSDK_GPIO1->OUTENABLESET;
        obj->reg_dirclr = &CMSDK_GPIO1->OUTENABLECLR;
    } else if (pin >= 32 && pin <= 47) {
        pin_position = (pin - 32);
        obj->reg_data   = &CMSDK_GPIO2->DATAOUT;
        obj->reg_in     = &CMSDK_GPIO2->DATA;
        obj->reg_dir    = &CMSDK_GPIO2->OUTENABLESET;
        obj->reg_dirclr = &CMSDK_GPIO2->OUTENABLECLR;
    } else if (pin >= 48 && pin <= 51) {
        pin_position = (pin - 48 );
        obj->reg_data   = &CMSDK_GPIO3->DATAOUT;
        obj->reg_in     = &CMSDK_GPIO3->DATA;
        obj->reg_dir    = &CMSDK_GPIO3->OUTENABLESET;
        obj->reg_dirclr = &CMSDK_GPIO3->OUTENABLECLR;
    } else if (pin == 100 || pin == 101) {
        /* User LEDs */
        pin_position    = (pin - 100);
        obj->reg_data   = &MPS2_FPGAIO->LED;
        obj->reg_in     = &MPS2_FPGAIO->LED;
        obj->reg_dir    = NULL;
        obj->reg_dirclr = NULL;
    } else if (pin == 110 || pin == 111) {
        /* User buttons */
        pin_position    = pin-110;
        obj->reg_data   = &MPS2_FPGAIO->BUTTON;
        obj->reg_in     = &MPS2_FPGAIO->BUTTON;
        obj->reg_dir    = NULL;
        obj->reg_dirclr = NULL;
    } else if (pin >= 200 && pin <= 207) {
        /* MCC LEDs */
        pin_position    = pin-200;
        obj->reg_data   = &MPS2_SCC->LEDS;
        obj->reg_in     = &MPS2_SCC->LEDS;
        obj->reg_dir    = NULL;
        obj->reg_dirclr = NULL;
    } else if (pin >= 210 && pin <= 217) {
        /* MCC switches */
        pin_position    = (pin - 210);
        obj->reg_in     = &MPS2_SCC->SWITCHES;
        obj->reg_data   = NULL;
        obj->reg_dir    = NULL;
        obj->reg_dirclr = NULL;
    } else if (pin == SHIELD_0_SPI_nCS) {
        pin_position = 8;
        GPIO_MAP[CMSDK_GPIO_SH0_CS_SPI_GPIO_NUM]->ALTFUNCSET |=
                                (1 << CMSDK_GPIO_ALTFUNC_SH0_CS_SPI_SET);
    } else if (pin == SHIELD_1_SPI_nCS) {
        pin_position = 9;
        GPIO_MAP[CMSDK_GPIO_SH1_CS_SPI_GPIO_NUM]->ALTFUNCSET |=
                                (1 << CMSDK_GPIO_ALTFUNC_SH1_CS_SPI_SET);
    } else if (pin == ADC_SSEL) {
        pin_position = 7;
        GPIO_MAP[CMSDK_GPIO_ADC_CS_SPI_GPIO_NUM]->ALTFUNCSET |=
                                (1 << CMSDK_GPIO_ALTFUNC_ADC_CS_SPI_SET);
    } else {
        pin_position = get_fpga_misc_pin_pos(pin);
        if (pin_position != RESERVED_MISC_PIN) {
            obj->reg_data = &MPS2_FPGAIO->MISC;
        } else {
            pin_position = 0;
        }
    }

    obj->pin  = pin;
    obj->mask = (0x1 << pin_position);
    obj->pin_number = pin;
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

