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

#include <stddef.h>
#include "cmsis.h"
#include "gpio_irq_api.h"
#include "pinmap.h"
#include "mbed_error.h"

#define EDGE_NONE (0)
#define EDGE_RISE (1)
#define EDGE_FALL (2)
#define EDGE_BOTH (3)

static gpio_irq_handler irq_handler;

static uint32_t channel_ids[4][16];


#ifdef __cplusplus
extern "C"{
#endif
void port_generic_handler(GPIO_TypeDef* GPIOx, uint32_t port_num);
 
void PORT0_Handler(void)
{
    NVIC_ClearPendingIRQ(PORT0_IRQn);
    port_generic_handler(GPIOA, 0);
}

void PORT1_Handler(void)
{
    port_generic_handler(GPIOB, 1);
}

void PORT2_Handler(void)
{
    port_generic_handler(GPIOC, 2);
}

void PORT3_Handler(void)
{
    port_generic_handler(GPIOD, 3);
}

void port_generic_handler(GPIO_TypeDef* GPIOx, uint32_t port_num)
{
    int i = 0;
    int loop = 16;

    if(GPIOx == GPIOD) loop = 5;
    
    for(i=0; i<loop; i++)
    {
        if(GPIOx->Interrupt.INTSTATUS & (1 << i))
        {
            GPIOx->Interrupt.INTCLEAR |= (1 << i);
            if(GPIOx->INTPOLSET >> i) //rising
                irq_handler(channel_ids[port_num][i], IRQ_RISE);
            else //falling
                irq_handler(channel_ids[port_num][i], IRQ_FALL);
        }
    }
}

#ifdef __cplusplus
}
#endif


int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{   
    obj->port_num = WIZ_PORT(pin);
    obj->pin_num = WIZ_PIN_NUM(pin);
    obj->pin_index  = WIZ_PIN_INDEX(pin);
    
    //gpio_irq_disable(obj);

    if (pin == NC) return -1;
    
    if(obj->port_num == 0)
        obj->irq_n = PORT0_IRQn;
    else if(obj->port_num == 1)
        obj->irq_n = PORT1_IRQn;
    else if(obj->port_num == 2)
        obj->irq_n = PORT2_IRQn;
    else
        obj->irq_n = PORT3_IRQn;
    
    obj->pin = pin;
    obj->event = EDGE_NONE;

    // Enable EXTI interrupt    
    NVIC_ClearPendingIRQ(obj->irq_n);
    NVIC_EnableIRQ(obj->irq_n);

    channel_ids[obj->port_num][obj->pin_num] = id;
    
    irq_handler = handler;
    
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    channel_ids[obj->port_num][obj->pin_num] = 0;
        
    obj->event = EDGE_NONE;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    GPIO_TypeDef *gpio = (GPIO_TypeDef *)Get_GPIO_BaseAddress(obj->port_num);
    
    if (enable) {
        if (event == IRQ_RISE) {
            gpio->INTPOLSET |= obj->pin_index;
            obj->event = EDGE_RISE;
            obj->rise_null = 0;
        }
        else if (event == IRQ_FALL) {
            gpio->INTPOLCLR |= obj->pin_index;
            obj->event = EDGE_FALL;
            obj->fall_null = 0;
        }

    
        gpio->INTENCLR |= obj->pin_index;
        gpio->INTTYPESET |= obj->pin_index;
        gpio->INTENSET |= obj->pin_index;
        
        
    } else {
        if (event == IRQ_RISE) {
            obj->rise_null = 1;
            if(obj->fall_null)
                gpio->INTENCLR |= obj->pin_index;
        }
        else if (event == IRQ_FALL) {
            obj->fall_null = 1;
            if(obj->rise_null)
                gpio->INTENCLR |= obj->pin_index;
        }
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    NVIC_EnableIRQ(obj->irq_n);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    NVIC_DisableIRQ(obj->irq_n);
    obj->event = EDGE_NONE;
}
