/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
#include "objects.h"
#include "pinmap.h"

#if CONFIG_GPIO_EN
#include "gpio_irq_api.h"

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id) 
{
    uint32_t pin_name;

    if (pin == NC) return -1;

    obj->pin = pin;
    pin_name = HAL_GPIO_GetPinName((u32)pin);; // get the IP pin name
    obj->hal_pin.pin_name = pin_name;
    obj->hal_pin.pin_mode = INT_FALLING;    // default use Falling trigger
    obj->hal_port_num = HAL_GPIO_GET_PORT_BY_NAME(pin_name);
    obj->hal_pin_num = HAL_GPIO_GET_PIN_BY_NAME(pin_name);
    HAL_GPIO_Irq_Init(&obj->hal_pin);
    HAL_GPIO_UserRegIrq(&obj->hal_pin, (VOID*) handler, (VOID*) id);

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj) 
{
    HAL_GPIO_UserUnRegIrq(&obj->hal_pin);
    HAL_GPIO_DeInit(&obj->hal_pin);
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable) 
{
    HAL_GPIO_MaskIrq(&obj->hal_pin);
    switch((uint32_t)event) {
        case IRQ_RISE:
            obj->hal_pin.pin_mode = INT_RISING;
            break;

        case IRQ_FALL:
            obj->hal_pin.pin_mode = INT_FALLING;
            break;

        case IRQ_NONE:
            break;
            
        default:
            break;
    }
    HAL_GPIO_Init_8195a(&obj->hal_pin);

    HAL_GPIO_IntCtrl(&obj->hal_pin, enable);
    if(enable){
        HAL_GPIO_UnMaskIrq(&obj->hal_pin);
    } else{
        HAL_GPIO_MaskIrq(&obj->hal_pin);
    }
}

void gpio_irq_enable(gpio_irq_t *obj) 
{
    HAL_GPIO_UnMaskIrq(&obj->hal_pin);
}

void gpio_irq_disable(gpio_irq_t *obj) 
{
    HAL_GPIO_MaskIrq(&obj->hal_pin);
}
#endif

