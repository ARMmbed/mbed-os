/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
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

#include "apm32f4xx_gpio.h"
#include "mbed_assert.h"
#include "gpio_api.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "apm32f4xx_rcm.h"

extern const int APM_GPIO_MODE[];
extern const int APM_GPIO_PULLUP_DOWN[];
extern const int APM_GPIO_OUT_PUT_MODE[];
extern const int APM_GPIO_SPEED[];
extern const int APM_GPIO_AF[];

 /*!
 * @brief     enable GPIO clock and return GPIO base address
 *
 * @param     port : GPIOA-GPIOI
 *
 * @retval    gpio_port
 */
GPIO_T* gpio_clock_enable(uint32_t port_idx)
{
    GPIO_T* gpio_port = GPIOA;
    switch (port_idx)
    {
        case PORTA:
            gpio_port = GPIOA;
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOA);
            break;
        case PORTB:
            gpio_port = GPIOB;
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOB);
            break;
        case PORTC:
            gpio_port = GPIOC;
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOC);
            break;
        case PORTD:
            gpio_port = GPIOD;
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOD);
            break;
        case PORTE:
            gpio_port = GPIOE;
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOE);
            break;
        case PORTF:
            gpio_port = GPIOF;
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOF);
            break;
        case PORTG:
            gpio_port = GPIOG;
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOG);
            break;
        case PORTH:
            gpio_port = GPIOH;
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOH);
            break;
        case PORTI:
            gpio_port = GPIOI;
            RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOI);
            break;
        default:
            error("port number not exist");
            break;
    }
    return gpio_port;
}

 /*!
 * @brief     Set the given pin as GPIO
 *
 * @param     pin : The pin to be set as GPIO
 *
 * @retval    The GPIO port mask for this pin
 */
uint32_t gpio_set(PinName pin)
{
    MBED_ASSERT(pin != (PinName)NC);
    pin_function(pin, SET_PIN_FUNCTION(PIN_MODE_INPUT, PIN_OTYPE_PP, PIN_PUPD_NONE, PIN_AF_0));
    /* return pin mask */
    return (uint32_t)(1 << ((uint32_t)pin & 0xF));
}

 /*!
 * @brief     Initialize the GPIO pin
 *
 * @param     obj : The GPIO object to initialize
 *
 * @param     pin : The pin to be set as GPIO
 *
 * @retval    None
 */
void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;
    if (pin == (PinName)NC)
    {
        return;
    }

    uint32_t port_index = APM_PORT_GET(pin);
    GPIO_T* gpio = gpio_clock_enable(port_index);
    obj->mask = gpio_set(pin);
    obj->gpio_periph = gpio;
}

 /*!
 * @brief     Set the input pin mode
 *
 * @param     obj : The GPIO object
 *
 * @param     mode : The pin mode to be set
 *
 * @retval    None
 */
void gpio_mode(gpio_t *obj, PinMode mode)
{
    pin_mode(obj->pin, mode);
}

 /*!
 * @brief     Set the output value
 *
 * @param     obj : The GPIO object
 *
 * @param     value : The value to be set
 *
 * @retval    None
 */
void gpio_write(gpio_t *obj, int value)
{
    /* set or reset GPIO pin */
    if (value)
    {
        GPIO_SetBit(obj->gpio_periph, 1 << (uint32_t)APM_PIN_GET(obj->pin));
    }
    else
    {
        GPIO_ResetBit(obj->gpio_periph, 1 << (uint32_t)APM_PIN_GET(obj->pin));
    }
}

 /*!
 * @brief     Read the input value
 *
 * @param     obj : The GPIO object
 *
 * @retval    An integer value 1 or 0
 */
int gpio_read(gpio_t *obj)
{
    /* return state of GPIO pin */
    if (obj->mode == GPIO_MODE_IN)
    {
        return GPIO_ReadInputBit(obj->gpio_periph,1 << (uint32_t)APM_PIN_GET(obj->pin)) ? 1 : 0;
    }
    else if (obj->mode == GPIO_MODE_OUT)
    {
        return GPIO_ReadOutputBit(obj->gpio_periph,1 << (uint32_t)APM_PIN_GET(obj->pin)) ? 1 : 0;
    }
    return -1;
}

 /*!
 * @brief     Checks if gpio object is connected (pin was not initialized with NC)
 *
 * @param     pin : The pin to be set as GPIO
 *
 * @retval    0 if port is initialized with NC
 */
int gpio_is_connected(const gpio_t *obj)
{
    return obj->pin != (PinName)NC;
}

 /*!
 * @brief     Set the pin direction
 *
 * @param     obj : The GPIO object
 *
 * @param     direction : The pin direction to be set
 *
 * @retval    None
 */
void gpio_dir(gpio_t *obj, PinDirection direction)
{
    uint32_t pin = (uint32_t)APM_PIN_GET(obj->pin);

    obj->gpio_periph->MODE &= ((uint32_t)~(0x03 << (pin * 2)));
    obj->gpio_periph->MODE |= (((uint32_t)APM_GPIO_MODE[direction]) << (pin * 2));

}

