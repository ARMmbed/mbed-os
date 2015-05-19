/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "mbed_assert.h"
#include "pinmap.h"
#include "PortNames.h"


// GPIO mode look-up table
// It have to same with PinMode index in "PinNames.h"
static const uint32_t gpio_pupd[4] = {
    GPIO_NO_PUPD,           //  PullNone
    GPIO_PuPd_DOWN,         //  PullDown
    GPIO_PuPd_UP,           //  PullUp
    GPIO_OD                 //  OpenDrain
};

uint32_t Get_GPIO_BaseAddress(uint32_t port_idx)
{
    uint32_t gpio_add = 0;
    switch(port_idx)    {
        case PortA:
            gpio_add = GPIOA_BASE;
            break;
        case PortB:
            gpio_add = GPIOB_BASE;
            break;
        case PortC:
            gpio_add = GPIOC_BASE;
            break;
        case PortD:
            gpio_add = GPIOD_BASE;
            break;
        default:
            error("Pinmap error: wrong port number.");
            break;
    }
    return gpio_add;
}


/**
 * Configure pin (input, output, alternate function or analog) + output speed + AF
 */

void pin_function(PinName pin, int data) {
    MBED_ASSERT(pin != (PinName)NC);
    // Get the pin informations
    uint32_t mode  = WIZ_PIN_MODE(data);
    uint32_t pupd  = WIZ_PIN_PUPD(data);
    uint32_t afnum;

    if( mode == WIZ_MODE_AF )
        afnum = WIZ_PIN_AFNUM(data);
    else
        afnum = WIZ_AFNUM(pin);

    uint32_t port_index = WIZ_PORT(pin);
    uint32_t pin_index  = WIZ_PIN(pin);


    uint32_t gpio_add = Get_GPIO_BaseAddress(port_index);
    GPIO_TypeDef *gpio = (GPIO_TypeDef *)gpio_add;

    // Configure Alternate Function
    // Warning: Must be done before the GPIO is initialized
    switch (afnum) {
        case 0:
            HAL_PAD_AFConfig(port_index,(uint32_t)(1 << pin_index),Px_AFSR_AF0);
            break;
        case 1:
            HAL_PAD_AFConfig(port_index,(uint32_t)(1 << pin_index),Px_AFSR_AF1);
            break;
        case 2:
            HAL_PAD_AFConfig(port_index,(uint32_t)(1 << pin_index),Px_AFSR_AF2);
            break;
        case 3:
            HAL_PAD_AFConfig(port_index,(uint32_t)(1 << pin_index),Px_AFSR_AF3);
            break;
        default:
            break;
    }

    if(mode == WIZ_MODE_AF)
        return;

    // Configure GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin       = (uint32_t)(1 << pin_index);
    GPIO_InitStructure.GPIO_Mode      = mode;
    GPIO_InitStructure.GPIO_Pad       = gpio_pupd[pupd];
    HAL_GPIO_Init(gpio, &GPIO_InitStructure);
}

/**
 * Configure pin pull-up/pull-down
 */
void pin_mode(PinName pin, PinMode pupd)
{
    MBED_ASSERT(pin != (PinName)NC);

    P_Port_Def *px_pcr;

    uint32_t port_index = WIZ_PORT(pin);

    switch(port_index)    {
        case PortA:
            px_pcr = PA_PCR;
            break;
        case PortB:
            px_pcr = PB_PCR;
            break;
        case PortC:
            px_pcr = PC_PCR;
            break;
        case PortD:
            px_pcr = (P_Port_Def*)PD_PCR;
            break;
        default:
            error("Pinmap error: wrong port number.");
            return;
    }

    px_pcr->Port[port_index] &= ~(Px_PCR_PUPD_DOWN|Px_PCR_PUPD_UP|Px_PCR_DS_HIGH| \
                                  Px_PCR_OD | Px_PCR_IE | Px_PCR_CS_SUMMIT);
    px_pcr->Port[port_index] |= gpio_pupd[pupd];
}
