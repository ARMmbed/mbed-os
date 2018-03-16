/*******************************************************************************
 * Copyright (c) 2010-2017 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - Modified versions of the software must be conspicuously marked as such.
 *   - This software is licensed solely and exclusively for use with processors
 *     manufactured by or for Analog Devices, Inc.
 *   - This software may not be combined or merged with other code in any manner
 *     that would cause the software to become subject to terms and conditions
 *     which differ from those listed here.
 *   - Neither the name of Analog Devices, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
 * INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
 * CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include "mbed_assert.h"
#include "gpio_api.h"
#include "pinmap.h"
#include "adi_gpio.h"


#define MUX_FUNC_0 0x0
#define NUM_GPIO_PORTS 4
/*******************************************************************************
   ADI_GPIO_DEV_DATA Instance memory containing memory pointer should
   guarantee 4 byte alignmnet.
 *******************************************************************************/
extern uint32_t gpioMemory[(ADI_GPIO_MEMORY_SIZE + 3)/4];
extern uint8_t  gpio_initialized;

static uint16_t gpio_oen[NUM_GPIO_PORTS] = {0};
static uint16_t gpio_output_val[NUM_GPIO_PORTS] = {0};


/******************************************************************************
   Function definitions
 *****************************************************************************/
uint32_t gpio_set(PinName pin)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t pin_num = pin & 0xFF;

    pin_function(pin, MUX_FUNC_0);

    return (1 << pin_num);
}

void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;

    if (pin == (PinName)NC) {
        return;
    }

    // Initialize the GPIO driver. This function
    // initializes the GPIO driver only once globally.
    if (!gpio_initialized) {
        adi_gpio_Init(gpioMemory, ADI_GPIO_MEMORY_SIZE);
    }

    pin_function(pin, MUX_FUNC_0);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    uint32_t pin = obj->pin;

    pin_mode((PinName)pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    uint32_t port = obj->pin >> GPIO_PORT_SHIFT;
    uint32_t pin_num = obj->pin & 0xFF;

    if (direction ==  PIN_OUTPUT) {
        adi_gpio_OutputEnable((ADI_GPIO_PORT)port, 1 << pin_num, true);
        // save the input/output configuration
        gpio_oen[port] |= (1 << pin_num);
    } else {
        adi_gpio_InputEnable((ADI_GPIO_PORT)port, 1 << pin_num, true);
        // save the input/output configuration
        gpio_oen[port] &= (~(1 << pin_num));
    }
}

void gpio_write(gpio_t *obj, int value)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    uint32_t port = obj->pin >> GPIO_PORT_SHIFT;
    uint32_t pin_num = obj->pin & 0xFF;

    if (value & 1) {
        adi_gpio_SetHigh((ADI_GPIO_PORT)port, (1 << pin_num));

        // save the output port value
        gpio_output_val[port] |= ((value & 1) << pin_num);
    } else {
        adi_gpio_SetLow((ADI_GPIO_PORT)port, (1 << pin_num));

        // save the output port value
        gpio_output_val[port] &= (~(1 << pin_num));
    }
}


int gpio_read(gpio_t *obj)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    uint32_t port = obj->pin >> GPIO_PORT_SHIFT;
    uint32_t pin_num = obj->pin & 0xFF;
    uint16_t Data;

    // check whether the pin is configured as input or output
    if ((gpio_oen[port] >> pin_num) & 1) {
        Data = gpio_output_val[port] & (1 << pin_num);
    } else {
        // otherwise call GetData
        adi_gpio_GetData((ADI_GPIO_PORT)port, (1 << pin_num), &Data);
    }

    return ((((uint32_t)Data) >> pin_num) & 1);
}
