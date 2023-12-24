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

#include <stddef.h>
#include "cmsis.h"
#include "gpio_irq_api.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "apm32f4xx_eint.h"

extern uint32_t gpio_clock_enable(uint32_t port_idx);
static gpio_irq_handler irq_handler;

enum
{
    EDGE_NONE = 0,
    EDGE_RISE = 1,
    EDGE_FALL = 2
};

typedef struct
{
    uintptr_t eint_context;
    GPIO_T *eint_gpio; /* base address of gpio */
    uint32_t eint_pin;  /* pin number */
} gpio_eint_struct;

/* EXTI0...EXTI15 */
static gpio_eint_struct eint_arr[16] = {0};

 /*!
 * @brief     handle EXTI interrupt in EXTI0 to EXTI15
 *
 * @param     irq_index : the line of EXTI(0~15)
 *
 * @retval    None
 */
static void handle_interrupt(uint32_t irq_index)
{
    gpio_eint_struct *gpio_eint = &eint_arr[irq_index];

    GPIO_T* gpio = (GPIO_T*)(gpio_eint->eint_gpio);
    uint32_t pin = (uint32_t)(1 << gpio_eint->eint_pin);

    if (EINT_ReadIntFlag((EINT_LINE_T)pin) != RESET)
    {
        EINT_ClearIntFlag((EINT_LINE_T)pin);

        if ((gpio->IDATA & pin) == 0)
        {
            irq_handler(gpio_eint->eint_context, IRQ_FALL);
        }
        else
        {
            irq_handler(gpio_eint->eint_context, IRQ_RISE);
        }
    }

}

 /*!
 * @brief     EXTI0 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint0(void)
{
    handle_interrupt(0);
}

 /*!
 * @brief     EXTI1 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint1(void)
{
    handle_interrupt(1);
}

 /*!
 * @brief     EXTI2 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint2(void)
{
    handle_interrupt(2);
}

 /*!
 * @brief     EXTI3 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint3(void)
{
    handle_interrupt(3);
}

 /*!
 * @brief     EXTI4 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint4(void)
{
    handle_interrupt(4);
}

 /*!
 * @brief     EXTI5 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint5(void)
{
    handle_interrupt(5);
}

 /*!
 * @brief     EXTI6 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint6(void)
{
    handle_interrupt(6);
}

 /*!
 * @brief     EXTI7 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint7(void)
{
    handle_interrupt(7);
}

 /*!
 * @brief     EXTI8 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint8(void)
{
    handle_interrupt(8);
}

 /*!
 * @brief     EXTI9 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint9(void)
{
    handle_interrupt(9);
}

 /*!
 * @brief     EXTI10 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint10(void)
{
    handle_interrupt(10);
}

 /*!
 * @brief     EXTI11 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint11(void)
{
    handle_interrupt(11);
}

 /*!
 * @brief     EXTI12 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint12(void)
{
    handle_interrupt(12);
}

 /*!
 * @brief     EXTI13 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint13(void)
{
    handle_interrupt(13);
}

 /*!
 * @brief     EXTI14 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint14(void)
{
    handle_interrupt(14);
}

 /*!
 * @brief     EXTI15 interrupt handler
 *
 * @param     None
 *
 * @retval    None
 */
static void gpio_irq_eint15(void)
{
    handle_interrupt(15);
}

 /*!
 * @brief     Initialize the GPIO IRQ pin
 *
 * @param     obj : The GPIO object to initialize
 *
 * @param     pin : The GPIO pin name
 *
 * @param     handler : The handler to be attached to GPIO IRQ
 *
 * @param     context : The context to be passed back to the handler (context != 0, 0 is reserved)
 *
 * @retval    -1 if pin is NC, 0 otherwise
 */
int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uintptr_t context)
{
    uint32_t vector = 0;
    gpio_eint_struct *gpio_eint;

    if(pin == NC)
    {
        return -1;
    }

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);

    uint32_t port_num = APM_PORT_GET(pin);
    uint32_t pin_num = APM_PIN_GET(pin);

    switch (pin_num)
    {
        case 0:
            vector = (uint32_t)&gpio_irq_eint0;
            obj->irq_index = 0;
            obj->irq_n = EINT0_IRQn;
            break;
        case 1:
            vector = (uint32_t)&gpio_irq_eint1;
            obj->irq_index = 1;
            obj->irq_n = EINT1_IRQn;
            break;
        case 2:
            vector = (uint32_t)&gpio_irq_eint2;
            obj->irq_index = 2;
            obj->irq_n = EINT2_IRQn;
            break;
        case 3:
            vector = (uint32_t)&gpio_irq_eint3;
            obj->irq_index = 3;
            obj->irq_n = EINT3_IRQn;
            break;
        case 4:
            vector = (uint32_t)&gpio_irq_eint4;
            obj->irq_index = 4;
            obj->irq_n = EINT4_IRQn;
            break;
        case 5:
            vector = (uint32_t)&gpio_irq_eint5;
            obj->irq_index = 5;
            obj->irq_n = EINT9_5_IRQn;
            break;
        case 6:
            vector = (uint32_t)&gpio_irq_eint6;
            obj->irq_index = 6;
            obj->irq_n = EINT9_5_IRQn;
            break;
        case 7:
            vector = (uint32_t)&gpio_irq_eint7;
            obj->irq_index = 7;
            obj->irq_n = EINT9_5_IRQn;
            break;
        case 8:
            vector = (uint32_t)&gpio_irq_eint8;
            obj->irq_index = 8;
            obj->irq_n = EINT9_5_IRQn;
            break;
        case 9:
            vector = (uint32_t)&gpio_irq_eint9;
            obj->irq_index = 9;
            obj->irq_n = EINT9_5_IRQn;
            break;
        case 10:
            vector = (uint32_t)&gpio_irq_eint10;
            obj->irq_index = 10;
            obj->irq_n = EINT15_10_IRQn;
            break;
        case 11:
            vector = (uint32_t)&gpio_irq_eint11;
            obj->irq_index = 11;
            obj->irq_n = EINT15_10_IRQn;
            break;
        case 12:
            vector = (uint32_t)&gpio_irq_eint12;
            obj->irq_index = 12;
            obj->irq_n = EINT15_10_IRQn;
            break;
        case 13:
            vector = (uint32_t)&gpio_irq_eint13;
            obj->irq_index = 13;
            obj->irq_n = EINT15_10_IRQn;
            break;
        case 14:
            vector = (uint32_t)&gpio_irq_eint14;
            obj->irq_index = 14;
            obj->irq_n = EINT15_10_IRQn;
            break;
        case 15:
            vector = (uint32_t)&gpio_irq_eint15;
            obj->irq_index = 15;
            obj->irq_n = EINT15_10_IRQn;
            break;
        default:
            error("the pin is not support for interrupt. \n");
            return -1;
    }

    /* Enable GPIO Clock */
    GPIO_T* gpio_port = (GPIO_T*)gpio_clock_enable(port_num);

    obj->event = EDGE_NONE;
    obj->pin = pin;

    gpio_eint = &eint_arr[obj->irq_index];
    gpio_eint->eint_context = context;
    gpio_eint->eint_gpio = gpio_port;
    gpio_eint->eint_pin = pin;

    irq_handler = handler;

    /* Enable EXTI Interrupt */
    NVIC_SetVector(obj->irq_n, vector);
    gpio_irq_enable(obj);

    return 0;
}

 /*!
 * @brief     Release the GPIO IRQ PIN
 *
 * @param     obj : The gpio object
 *
 * @retval    None
 */
void gpio_irq_free(gpio_irq_t *obj)
{
    gpio_eint_struct *gpio_eint = &eint_arr[obj->irq_index];

    /* Disable EXTI Interrupt */
    gpio_irq_disable(obj);

    gpio_eint->eint_context = 0;
    gpio_eint->eint_gpio = 0;
    gpio_eint->eint_pin = 0;
}

 /*!
 * @brief     Enable/disable pin IRQ event
 *
 * @param     obj : The gpio object
 *
 * @param     event : The GPIO IRQ event
 *
 * @param     enable : The enable flag
 *
 * @retval    None
 */
void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    if (event == IRQ_FALL)
    {
        if (enable)
        {
            /* Config EXTI Line and Enable EXTI*/
            EINT_Config_T eintConfig;
            eintConfig.line = (EINT_LINE_T)(1 << APM_PIN_GET(obj->pin));
            eintConfig.lineCmd = ENABLE;
            eintConfig.mode = EINT_MODE_INTERRUPT;
            eintConfig.trigger = EINT_TRIGGER_FALLING;
            EINT_Config(&eintConfig);
        }
        else
        {
            /* Disable EXTI Line */
            *(uint32_t*)0x40013C00 &= ~(uint32_t)(EINT_LINE_T)(1 << APM_PIN_GET(obj->pin));
            *(uint32_t*)0x40013C08 &= ~(uint32_t)(EINT_LINE_T)(1 << APM_PIN_GET(obj->pin));
            *(uint32_t*)0x40013C0C &= ~(uint32_t)(EINT_LINE_T)(1 << APM_PIN_GET(obj->pin));
        }
    }
    if (event == IRQ_RISE)
    {
        if (enable)
        {
            /* Config EXTI Line and Enable EXTI*/
            EINT_Config_T eintConfig;
            eintConfig.line = (EINT_LINE_T)(1 << APM_PIN_GET(obj->pin));
            eintConfig.lineCmd = ENABLE;
            eintConfig.mode = EINT_MODE_INTERRUPT;
            eintConfig.trigger = EINT_TRIGGER_RISING;
            EINT_Config(&eintConfig);
        }
        else
        {
            /* Disable EXTI Line */
            *(uint32_t*)0x40013C00 &= ~(uint32_t)(EINT_LINE_T)(1 << APM_PIN_GET(obj->pin));
            *(uint32_t*)0x40013C08 &= ~(uint32_t)(EINT_LINE_T)(1 << APM_PIN_GET(obj->pin));
            *(uint32_t*)0x40013C0C &= ~(uint32_t)(EINT_LINE_T)(1 << APM_PIN_GET(obj->pin));
        }
    }
}

 /*!
 * @brief     Enable GPIO IRQ.This is target dependent, as it might enable the entire port or just a pin
 *
 * @param     obj : The gpio object
 *
 * @retval    None
 */
void gpio_irq_enable(gpio_irq_t *obj)
{
    EINT_Config_T eintConfig;
    SYSCFG_ConfigEINTLine(APM_PORT_GET(obj->pin),APM_PIN_GET(obj->pin));

    eintConfig.line = (EINT_LINE_T)(1 << APM_PIN_GET(obj->pin));
    eintConfig.lineCmd = ENABLE;
    eintConfig.mode = EINT_MODE_INTERRUPT;
    eintConfig.trigger = EINT_TRIGGER_FALLING;
    EINT_Config(&eintConfig);

    NVIC_EnableIRQ(obj->irq_n);
}

 /*!
 * @brief     Disable GPIO IRQ.This is target dependent, as it might disable the entire port or just a pin.
 *
 * @param     obj : The gpio object
 *
 * @retval    None
 */
void gpio_irq_disable(gpio_irq_t *obj)
{
    EINT_Config_T eintConfig;
    eintConfig.line = (EINT_LINE_T)(1 << APM_PIN_GET(obj->pin));
    eintConfig.lineCmd = DISABLE;
    eintConfig.mode = EINT_MODE_INTERRUPT;
    EINT_Config(&eintConfig);

    NVIC_DisableIRQ(obj->irq_n);
    NVIC_ClearPendingIRQ(obj->irq_n);
    obj->event = EDGE_NONE;
}
