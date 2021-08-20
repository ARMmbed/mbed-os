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

#if DEVICE_INTERRUPTIN

#include <stdbool.h>
#include <inttypes.h>
#include "cmsis.h"
#include "gpio_irq_api.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "gpio_irq_device.h"
#include "platform/mbed_critical.h"

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
static uint16_t irq_channel_used = 0x00;

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
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ7_NUM_LINES
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ8_NUM_LINES
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ9_NUM_LINES
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ10_NUM_LINES
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ11_NUM_LINES
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ12_NUM_LINES
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ13_NUM_LINES
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ14_NUM_LINES
    {.pin_mask = 0},
#endif
#ifdef EXTI_IRQ15_NUM_LINES
    {.pin_mask = 0},
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

#if defined(TARGET_STM32L5)
            // Clear interrupt flag
            if (LL_EXTI_IsActiveRisingFlag_0_31(pin) != RESET) {
                LL_EXTI_ClearRisingFlag_0_31(pin);

                if (gpio_channel->channel_ids[gpio_idx] == 0) {
                    continue;
                }

                gpio_irq_event event = IRQ_RISE;
                irq_handler(gpio_channel->channel_ids[gpio_idx], event);

                return;
            }

            if (LL_EXTI_IsActiveFallingFlag_0_31(pin) != RESET) {
                LL_EXTI_ClearFallingFlag_0_31(pin);

                if (gpio_channel->channel_ids[gpio_idx] == 0) {
                    continue;
                }

                gpio_irq_event event = IRQ_FALL;
                irq_handler(gpio_channel->channel_ids[gpio_idx], event);

                return;
            }

#else /* TARGET_STM32L5 */

            // Clear interrupt flag
#if defined(DUAL_CORE) && defined(CORE_CM4) && defined(TARGET_STM32H7)
            if (__HAL_GPIO_EXTID2_GET_FLAG(pin) != RESET) {
                __HAL_GPIO_EXTID2_CLEAR_FLAG(pin);
#else
            if (__HAL_GPIO_EXTI_GET_FLAG(pin) != RESET) {
                __HAL_GPIO_EXTI_CLEAR_FLAG(pin);
#endif
                if (gpio_channel->channel_ids[gpio_idx] == 0) {
                    continue;
                }

                // Trying to discern which edge caused the IRQ
                gpio_irq_event event = IRQ_NONE;
                if (LL_EXTI_IsEnabledFallingTrig_0_31(pin) && !LL_EXTI_IsEnabledRisingTrig_0_31(pin)) {
                    // Only the fall handler is active, so this must be a falling edge
                    event = IRQ_FALL;
                } else if (LL_EXTI_IsEnabledRisingTrig_0_31(pin) && !LL_EXTI_IsEnabledFallingTrig_0_31(pin)) {
                    // Only the rise handler is active, so this must be a rising edge
                    event = IRQ_RISE;
                } else {
                    // Ambiguous as to which edge caused the IRQ
                    //
                    // The state of the pin could/should indicate which edge
                    // has occurred but this can go wrong if the IRQ caused a
                    // transition from a low power mode. In some circumstances
                    // only the trailing edge callback will be called.
                    if ((gpio->IDR & pin) == 0) {
                        event = IRQ_FALL;
                    } else {
                        event = IRQ_RISE;
                    }
                }

                irq_handler(gpio_channel->channel_ids[gpio_idx], event);

                return;
            }
#endif /* TARGET_STM32L5 */
        }
    }
    error("Unexpected Spurious interrupt index %" PRIu32 "\n", irq_index);
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
// EXTI lines 5 to 9, except for STM32L5 line 5 only
static void gpio_irq5(void)
{
    handle_interrupt_in(5, EXTI_IRQ5_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ6_NUM_LINES
// EXTI lines 10 to 15, except for STM32L5 line 6 only
static void gpio_irq6(void)
{
    handle_interrupt_in(6, EXTI_IRQ6_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ7_NUM_LINES
// EXTI line 7 for STM32L5
static void gpio_irq7(void)
{
    handle_interrupt_in(7, EXTI_IRQ7_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ8_NUM_LINES
// EXTI line 8 for STM32L5
static void gpio_irq8(void)
{
    handle_interrupt_in(8, EXTI_IRQ8_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ9_NUM_LINES
// EXTI line 9 for STM32L5
static void gpio_irq9(void)
{
    handle_interrupt_in(9, EXTI_IRQ9_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ10_NUM_LINES
// EXTI line 10 for STM32L5
static void gpio_irq10(void)
{
    handle_interrupt_in(10, EXTI_IRQ10_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ11_NUM_LINES
// EXTI line 11 for STM32L5
static void gpio_irq11(void)
{
    handle_interrupt_in(11, EXTI_IRQ11_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ12_NUM_LINES
// EXTI line 12 for STM32L5
static void gpio_irq12(void)
{
    handle_interrupt_in(12, EXTI_IRQ12_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ13_NUM_LINES
// EXTI line 13 for STM32L5
static void gpio_irq13(void)
{
    handle_interrupt_in(13, EXTI_IRQ13_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ14_NUM_LINES
// EXTI line 14 for STM32L5
static void gpio_irq14(void)
{
    handle_interrupt_in(14, EXTI_IRQ14_NUM_LINES);
}
#endif
#ifdef EXTI_IRQ15_NUM_LINES
// EXTI line 15 for STM32L5
static void gpio_irq15(void)
{
    handle_interrupt_in(15, EXTI_IRQ15_NUM_LINES);
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

    core_util_critical_section_enter();

    /* Enable SYSCFG Clock */
#if (!defined(TARGET_STM32WB) && !defined(TARGET_STM32WL))
    __HAL_RCC_SYSCFG_CLK_ENABLE();
#endif

    uint32_t port_index  = STM_PORT(pin);
    uint32_t pin_index  = STM_PIN(pin);
    irq_index =  pin_lines_desc[pin_index].irq_index;

    if (irq_channel_used & (1<<pin_index)) {
        error("InterruptIn error: irq channel conflict\n");
        return -1;
    }
    else {
        irq_channel_used |= (1<<pin_index);
    }

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
#ifdef EXTI_IRQ7_NUM_LINES
        case 7:
            vector = (uint32_t)&gpio_irq7;
            break;
#endif
#ifdef EXTI_IRQ8_NUM_LINES
        case 8:
            vector = (uint32_t)&gpio_irq8;
            break;
#endif
#ifdef EXTI_IRQ9_NUM_LINES
        case 9:
            vector = (uint32_t)&gpio_irq9;
            break;
#endif
#ifdef EXTI_IRQ10_NUM_LINES
        case 10:
            vector = (uint32_t)&gpio_irq10;
            break;
#endif
#ifdef EXTI_IRQ11_NUM_LINES
        case 11:
            vector = (uint32_t)&gpio_irq11;
            break;
#endif
#ifdef EXTI_IRQ12_NUM_LINES
        case 12:
            vector = (uint32_t)&gpio_irq12;
            break;
#endif
#ifdef EXTI_IRQ13_NUM_LINES
        case 13:
            vector = (uint32_t)&gpio_irq13;
            break;
#endif
#ifdef EXTI_IRQ14_NUM_LINES
        case 14:
            vector = (uint32_t)&gpio_irq14;
            break;
#endif
#ifdef EXTI_IRQ15_NUM_LINES
        case 15:
            vector = (uint32_t)&gpio_irq15;
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

    core_util_critical_section_exit();
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    core_util_critical_section_enter();

    uint32_t gpio_idx = pin_lines_desc[STM_PIN(obj->pin)].gpio_idx;
    gpio_channel_t *gpio_channel = &channels[obj->irq_index];

    gpio_irq_disable(obj);

    gpio_channel->pin_mask &= ~(1 << gpio_idx);
    gpio_channel->channel_ids[gpio_idx] = 0;
    gpio_channel->channel_gpio[gpio_idx] = 0;
    gpio_channel->channel_pin[gpio_idx] = 0;

    irq_channel_used &= ~(1<<(STM_PIN(obj->pin)));

    core_util_critical_section_exit();
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

#if defined(STM32G0) || defined(STM32L5)
    temp = EXTI->EXTICR[pin_index >> 2];
    CLEAR_BIT(temp, (0x0FU) << (8U * (pin_index & 0x03U)));
    SET_BIT(temp, port_index << (8U * (pin_index & 0x03U)));
    EXTI->EXTICR[pin_index >> 2] = temp;
#else
    temp = SYSCFG->EXTICR[pin_index >> 2];
    CLEAR_BIT(temp, (0x0FU) << (4U * (pin_index & 0x03U)));
    SET_BIT(temp, port_index << (4U * (pin_index & 0x03U)));
    SYSCFG->EXTICR[pin_index >> 2] = temp;
#endif

#if defined(DUAL_CORE) && defined(CORE_CM4) && defined(TARGET_STM32H7)
    LL_C2_EXTI_EnableIT_0_31(1 << pin_index);
#else
    LL_EXTI_EnableIT_0_31(1 << pin_index);
#endif

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
    const uint32_t pin_index = STM_PIN(obj->pin);
    const uint32_t gpio_idx = pin_lines_desc[pin_index].gpio_idx;
    const uint32_t pin_mask = 1 << gpio_idx;
    const uint32_t irq_index = pin_lines_desc[pin_index].irq_index;
    const gpio_channel_t *const gpio_channel = &channels[irq_index];

    /* Clear EXTI line configuration */
    LL_EXTI_DisableRisingTrig_0_31(1 << pin_index);
    LL_EXTI_DisableFallingTrig_0_31(1 << pin_index);

#if defined(DUAL_CORE) && defined(CORE_CM4) && defined(TARGET_STM32H7)
    LL_C2_EXTI_DisableIT_0_31(1 << pin_index);
#else
    LL_EXTI_DisableIT_0_31(1 << pin_index);
#endif

    uint32_t pin = (uint32_t)(1 << (gpio_channel->channel_pin[gpio_idx]));

    // Clear interrupt flag
    if (__HAL_GPIO_EXTI_GET_FLAG(pin) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_FLAG(pin);
    }

    const bool no_more_pins_on_vector = (gpio_channel->pin_mask & ~pin_mask) == 0;
    if (no_more_pins_on_vector) {
        NVIC_DisableIRQ(obj->irq_n);
        NVIC_ClearPendingIRQ(obj->irq_n);
    }
}

#endif /* DEVICE_INTERRUPTIN */
