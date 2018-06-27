/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
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
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
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
#include "gpio_irq_device.h"

#define EDGE_NONE (0)
#define EDGE_RISE (1)
#define EDGE_FALL (2)
#define EDGE_BOTH (3)


typedef struct gpio_channel {
    uint32_t pin_mask;                   // bitmask representing which pins are configured for receiving interrupts
    uint32_t channel_ids[MAX_PIN_LINE];  // mbed "gpio_irq_t gpio_irq" field of instance
    GPIO_TypeDef *channel_gpio[MAX_PIN_LINE]; // base address of gpio port group
    uint32_t channel_pin[MAX_PIN_LINE];  // pin number in port group
} gpio_channel_t;

static gpio_irq_handler irq_handler;

static gpio_channel_t channels[CHANNEL_NUM] = {
#ifdef EXTI_IRQ0_NUM_LINES
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ1_NUM_LINES
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ2_NUM_LINES
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ3_NUM_LINES
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ4_NUM_LINES
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ5_NUM_LINES
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ6_NUM_LINES
    {.pin_mask = 0}
#endif
};

static void handle_interrupt_in(uint32_t irq_index, uint32_t max_num_pin_line)
{
    gpio_channel_t *gpio_channel = &channels[irq_index];
    uint32_t gpio_idx;

    for (gpio_idx = 0; gpio_idx < max_num_pin_line; gpio_idx++) {
        uint32_t current_mask = (1 << gpio_idx);

        if (gpio_channel->pin_mask & current_mask) {
            // Retrieve the gpio and pin that generate the irq
            GPIO_TypeDef *gpio = (GPIO_TypeDef *)(gpio_channel->channel_gpio[gpio_idx]);
            uint32_t pin = (uint32_t)(1 << (gpio_channel->channel_pin[gpio_idx]));

            // Clear interrupt flag
            if (__HAL_GPIO_EXTI_GET_FLAG(pin) != RESET) {
                __HAL_GPIO_EXTI_CLEAR_FLAG(pin);

                if (gpio_channel->channel_ids[gpio_idx] == 0) {
                    continue;
                }

                // Check which edge has generated the irq
                if ((gpio->IDR & pin) == 0) {
                    irq_handler(gpio_channel->channel_ids[gpio_idx], IRQ_FALL);
                } else {
                    irq_handler(gpio_channel->channel_ids[gpio_idx], IRQ_RISE);
                }
                return;
            }
        }
    }
    error("Unexpected Spurious interrupt, index %d\r\n", irq_index);
}


#ifdef EXTI_IRQ0_NUM_LINES
// EXTI line 0
static void gpio_irq0(void)
{
    handle_interrupt_in(0, EXTI_IRQ0_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ1_NUM_LINES
// EXTI line 1
static void gpio_irq1(void)
{
    handle_interrupt_in(1, EXTI_IRQ1_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ2_NUM_LINES
// EXTI line 2
static void gpio_irq2(void)
{
    handle_interrupt_in(2, EXTI_IRQ2_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ3_NUM_LINES
// EXTI line 3
static void gpio_irq3(void)
{
    handle_interrupt_in(3, EXTI_IRQ3_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ4_NUM_LINES
// EXTI line 4
static void gpio_irq4(void)
{
    handle_interrupt_in(4, EXTI_IRQ4_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ5_NUM_LINES
// EXTI lines 5 to 9
static void gpio_irq5(void)
{
    handle_interrupt_in(5, EXTI_IRQ5_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ6_NUM_LINES
// EXTI lines 10 to 15
static void gpio_irq6(void)
{
    handle_interrupt_in(6, EXTI_IRQ6_NUM_LINES);
}
#endif

extern GPIO_TypeDef *Set_GPIO_Clock(uint32_t port_idx);
extern void pin_function_gpiomode(PinName pin, uint32_t gpiomode);

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    uint32_t vector = 0;
    uint32_t irq_index;
    gpio_channel_t *gpio_channel;
    uint32_t gpio_idx;

    if (pin == NC) {
        return -1;
    }

    /* Enable SYSCFG Clock */
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    uint32_t port_index  = STM_PORT(pin);
    uint32_t pin_index  = STM_PIN(pin);
    irq_index =  pin_lines_desc[pin_index].irq_index;

    switch (irq_index) {
#ifdef EXTI_IRQ0_NUM_LINES
        case 0:
            vector = (uint32_t)&gpio_irq0;
            break;
#endif
#ifdef EXTI_IRQ1_NUM_LINES
        case 1:
            vector = (uint32_t)&gpio_irq1;
            break;
#endif
#ifdef EXTI_IRQ2_NUM_LINES
        case 2:
            vector = (uint32_t)&gpio_irq2;
            break;
#endif
#ifdef EXTI_IRQ3_NUM_LINES
        case 3:
            vector = (uint32_t)&gpio_irq3;
            break;
#endif
#ifdef EXTI_IRQ4_NUM_LINES
        case 4:
            vector = (uint32_t)&gpio_irq4;
            break;
#endif
#ifdef EXTI_IRQ5_NUM_LINES
        case 5:
            vector = (uint32_t)&gpio_irq5;
            break;
#endif
#ifdef EXTI_IRQ6_NUM_LINES
        case 6:
            vector = (uint32_t)&gpio_irq6;
            break;
#endif
        default:
            error("InterruptIn error: pin not supported.\n");
            return -1;
    }

    // Enable GPIO clock
    GPIO_TypeDef *gpio_add = Set_GPIO_Clock(port_index);

    // Save informations for future use
    obj->irq_n = pin_lines_desc[pin_index].irq_n;
    obj->irq_index =  pin_lines_desc[pin_index].irq_index;
    obj->event = EDGE_NONE;
    obj->pin = pin;

    gpio_channel = &channels[irq_index];
    gpio_idx = pin_lines_desc[pin_index].gpio_idx;
    gpio_channel->pin_mask |= (1 << gpio_idx);
    gpio_channel->channel_ids[gpio_idx] = id;
    gpio_channel->channel_gpio[gpio_idx] = gpio_add;
    gpio_channel->channel_pin[gpio_idx] = pin_index;

    irq_handler = handler;

    // Enable EXTI interrupt
    NVIC_SetVector(obj->irq_n, vector);
    gpio_irq_enable(obj);

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    uint32_t gpio_idx = pin_lines_desc[STM_PIN(obj->pin)].gpio_idx;
    gpio_channel_t *gpio_channel = &channels[obj->irq_index];

    gpio_irq_disable(obj);
    gpio_channel->pin_mask &= ~(1 << gpio_idx);
    gpio_channel->channel_ids[gpio_idx] = 0;
    gpio_channel->channel_gpio[gpio_idx] = 0;
    gpio_channel->channel_pin[gpio_idx] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    /*  Enable / Disable Edge triggered interrupt and store event */
    if (event == IRQ_RISE) {
        if (enable) {
            LL_EXTI_EnableRisingTrig_0_31(1 << STM_PIN(obj->pin));
            obj->event |= IRQ_RISE;
        } else {
            LL_EXTI_DisableRisingTrig_0_31(1 << STM_PIN(obj->pin));
            obj->event &= ~IRQ_RISE;
        }
    }
    if (event == IRQ_FALL) {
        if (enable) {
            LL_EXTI_EnableFallingTrig_0_31(1 << STM_PIN(obj->pin));
            obj->event |= IRQ_FALL;
        } else {
            LL_EXTI_DisableFallingTrig_0_31(1 << STM_PIN(obj->pin));
            obj->event &= ~IRQ_FALL;
        }
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    uint32_t temp = 0;
    uint32_t port_index = STM_PORT(obj->pin);
    uint32_t pin_index  = STM_PIN(obj->pin);

    /*  Select Source  */
    temp = SYSCFG->EXTICR[pin_index >> 2];
    CLEAR_BIT(temp, (0x0FU) << (4U * (pin_index & 0x03U)));
    SET_BIT(temp, port_index << (4U * (pin_index & 0x03U)));
    SYSCFG->EXTICR[pin_index >> 2] = temp;

    LL_EXTI_EnableIT_0_31(1 << pin_index);

    /* Restore previous edge interrupt configuration if applicable */
    if (obj->event & IRQ_RISE) {
        LL_EXTI_EnableRisingTrig_0_31(1 << STM_PIN(obj->pin));
    }
    if (obj->event & IRQ_FALL) {
        LL_EXTI_EnableFallingTrig_0_31(1 << STM_PIN(obj->pin));
    }

    NVIC_EnableIRQ(obj->irq_n);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    /* Clear EXTI line configuration */
    LL_EXTI_DisableRisingTrig_0_31(1 << STM_PIN(obj->pin));
    LL_EXTI_DisableFallingTrig_0_31(1 << STM_PIN(obj->pin));
    LL_EXTI_DisableIT_0_31(1 << STM_PIN(obj->pin));
    NVIC_DisableIRQ(obj->irq_n);
    NVIC_ClearPendingIRQ(obj->irq_n);
}
