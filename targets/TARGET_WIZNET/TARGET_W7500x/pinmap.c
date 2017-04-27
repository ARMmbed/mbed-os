/* mbed Microcontroller Library 
 *******************************************************************************
 * Copyright (c) 2015 WIZnet Co.,Ltd. All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of ARM Limited nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
 
#include "mbed_assert.h"
#include "pinmap.h"
#include "PortNames.h"
#include "mbed_error.h"
#include "W7500x.h"
#include "W7500x_gpio.h"


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
    uint32_t afnum = WIZ_PIN_AFNUM(data);
    
    uint32_t port_num = WIZ_PORT(pin);
    uint32_t pin_index  = WIZ_PIN_INDEX(pin);
    
    GPIO_TypeDef *gpio;

    // Configure Alternate Function
    // Warning: Must be done before the GPIO is initialized
    switch (afnum) {
        case 0:
            HAL_PAD_AFConfig((PAD_Type)port_num, (uint16_t)pin_index, (PAD_AF_TypeDef)Px_AFSR_AF0);
            break;
        case 1:
            HAL_PAD_AFConfig((PAD_Type)port_num, (uint16_t)pin_index, (PAD_AF_TypeDef)Px_AFSR_AF1);
            break;
        case 2:
            HAL_PAD_AFConfig((PAD_Type)port_num, (uint16_t)pin_index, (PAD_AF_TypeDef)Px_AFSR_AF2);
            break;
        case 3:
            HAL_PAD_AFConfig((PAD_Type)port_num, (uint16_t)pin_index, (PAD_AF_TypeDef)Px_AFSR_AF3);
            break;
        default:
            break;
    }

    if(mode == WIZ_MODE_AF)
        return;

    // Configure GPIO
    gpio = (GPIO_TypeDef *)Get_GPIO_BaseAddress(port_num);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin       = pin_index;
    GPIO_InitStructure.GPIO_Mode      = (GPIOMode_TypeDef)mode;
    GPIO_InitStructure.GPIO_Pad       = (GPIOPad_TypeDef)pupd;
    HAL_GPIO_Init(gpio, &GPIO_InitStructure);
}

/**
 * Configure pin pull-up/pull-down
 */
void pin_mode(PinName pin, PinMode pupd)
{
    MBED_ASSERT(pin != (PinName)NC);
   
    uint32_t port_num = WIZ_PORT(pin);
    uint32_t pin_num = WIZ_PIN_NUM(pin);

    switch(port_num) {
        case PortA:
            if(pupd != 0) {
                PA_PCR->Port[pin_num] &= 0xFFFFFFFC;
            }
            PA_PCR->Port[pin_num] |= pupd;
            break;
        case PortB:
            if(pupd != 0) {
                PB_PCR->Port[pin_num] &= 0xFFFFFFFC;
            }
            PB_PCR->Port[pin_num] |= pupd;
            break;
        case PortC:
            if(pupd != 0) {
                PC_PCR->Port[pin_num] &= 0xFFFFFFFC;
            }
            PC_PCR->Port[pin_num] |= pupd;
            break;
        case PortD:
            if(pupd != 0) {
                PD_PCR->Port[pin_num] &= 0xFFFFFFFC;
            }
            PD_PCR->Port[pin_num] |= pupd;
            break;
        default:
            error("Pinmap error: wrong port number.");            
            return;
    }
}
