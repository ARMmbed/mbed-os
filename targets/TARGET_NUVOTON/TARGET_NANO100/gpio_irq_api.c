/* mbed Microcontroller Library
 * Copyright (c) 2015-2017 Nuvoton
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

#include "gpio_irq_api.h"

#if DEVICE_INTERRUPTIN

#include "gpio_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "mbed_error.h"
#include "nu_bitutil.h"

#define NU_MAX_PIN_PER_PORT     16

struct nu_gpio_irq_var {
    gpio_irq_t *    obj_arr;
    uint32_t       gpio_n;
    void            (*vec)(void);
};

void GPABC_IRQHandler(void);
void GPDEF_IRQHandler(void);
static void gpio_irq(struct nu_gpio_irq_var *var);

//EINT0_IRQn
static struct nu_gpio_irq_var gpio_irq_var_arr[] = {
    {NULL, 0, GPABC_IRQHandler},
    {NULL, 1, GPABC_IRQHandler},
    {NULL, 2, GPABC_IRQHandler},
    {NULL, 3, GPDEF_IRQHandler},
    {NULL, 4, GPDEF_IRQHandler},
    {NULL, 5, GPDEF_IRQHandler}
};

#define NU_MAX_PORT     (sizeof (gpio_irq_var_arr) / sizeof (gpio_irq_var_arr[0]))

#ifndef MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE
#define MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE 0
#endif

#ifndef MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE_LIST
#define MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE_LIST NC
#endif
static PinName gpio_irq_debounce_arr[] = {
    MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE_LIST
};

#ifndef MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_CLOCK_SOURCE
#define MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_CLOCK_SOURCE GPIO_DBCLKSRC_IRC10K
#endif

#ifndef MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_SAMPLE_RATE
#define MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_SAMPLE_RATE GPIO_DBCLKSEL_16
#endif

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    if (pin == NC) {
        return -1;
    }
    
    uint32_t pin_index = NU_PINNAME_TO_PIN(pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(pin);
    if (pin_index >= NU_MAX_PIN_PER_PORT || port_index >= NU_MAX_PORT) {
        return -1;
    }
    
    obj->pin = pin;
    obj->irq_handler = (uint32_t) handler;
    obj->irq_id = id;
    obj->next = NULL;

    GPIO_T *gpio_base = NU_PORT_BASE(port_index);
    // NOTE: In InterruptIn constructor, gpio_irq_init() is called with gpio_init_in() which is responsible for multi-function pin setting.
    //       There is no need to call gpio_set() redundantly.
    
    {
#if MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE
        // Suppress compiler warning
        (void) gpio_irq_debounce_arr;

        // Configure de-bounce clock source and sampling cycle time
        GPIO_SET_DEBOUNCE_TIME(MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_CLOCK_SOURCE, MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_SAMPLE_RATE);
        GPIO_ENABLE_DEBOUNCE(gpio_base, 1 << pin_index);
#else
        // Enable de-bounce if the pin is in the de-bounce enable list
    
        // De-bounce defaults to disabled.
        GPIO_DISABLE_DEBOUNCE(gpio_base, 1 << pin_index);
        
        PinName *debounce_pos = gpio_irq_debounce_arr;
        PinName *debounce_end = gpio_irq_debounce_arr + sizeof (gpio_irq_debounce_arr) / sizeof (gpio_irq_debounce_arr[0]);
        for (; debounce_pos != debounce_end && *debounce_pos != NC; debounce_pos ++) {
            uint32_t pin_index_debunce = NU_PINNAME_TO_PIN(*debounce_pos);
            uint32_t port_index_debounce = NU_PINNAME_TO_PORT(*debounce_pos);
            
            if (pin_index == pin_index_debunce &&
                port_index == port_index_debounce) {
                // Configure de-bounce clock source and sampling cycle time
                GPIO_SET_DEBOUNCE_TIME(MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_CLOCK_SOURCE, MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_SAMPLE_RATE);
                GPIO_ENABLE_DEBOUNCE(gpio_base, 1 << pin_index);
                break;
            }
        }
#endif
    }
    
    struct nu_gpio_irq_var *var = gpio_irq_var_arr + port_index;
    
    // Add obj to linked list
    gpio_irq_t *cur_obj = var->obj_arr;
    if (cur_obj == NULL) {
        var->obj_arr = obj;
    } else {
        while (cur_obj->next != NULL)
            cur_obj = cur_obj->next;
        cur_obj->next = obj;
    }
    
    // NOTE: InterruptIn requires IRQ enabled by default.
    gpio_irq_enable(obj);
    
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    uint32_t pin_index = NU_PINNAME_TO_PIN(obj->pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(obj->pin);
    struct nu_gpio_irq_var *var = gpio_irq_var_arr + port_index;
    
    NVIC_DisableIRQ((var->gpio_n < 3) ? GPABC_IRQn : GPDEF_IRQn);
    NU_PORT_BASE(port_index)->IER = 0;
    
    MBED_ASSERT(pin_index < NU_MAX_PIN_PER_PORT);
    gpio_irq_t *pre_obj = var->obj_arr;
    if (pre_obj->pin == obj->pin)
        var->obj_arr = pre_obj->next;
    else {
        int error_flag = 1;
        while (pre_obj->next) {
            gpio_irq_t *cur_obj = pre_obj->next;
            if (cur_obj->pin == obj->pin) {
                pre_obj->next = cur_obj->next;
                error_flag = 0;
                break;
            }
            pre_obj = pre_obj->next;
        }
        if (error_flag)
            error("cannot find obj in gpio_irq_free()");
    }
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    uint32_t pin_index = NU_PINNAME_TO_PIN(obj->pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(obj->pin);
    GPIO_T *gpio_base = NU_PORT_BASE(port_index);
    
    switch (event) {
        case IRQ_RISE:
            if (enable) {
                GPIO_EnableInt(gpio_base, pin_index, GPIO_INT_RISING);
            }
            else {
                gpio_base->IER &= ~(GPIO_INT_RISING << pin_index);
            }
            break;
        
        case IRQ_FALL:
            if (enable) {
                GPIO_EnableInt(gpio_base, pin_index, GPIO_INT_FALLING);
            }
            else {
                gpio_base->IER &= ~(GPIO_INT_FALLING << pin_index);
            }
            break;
        
        default:
            break;
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    uint32_t port_index = NU_PINNAME_TO_PORT(obj->pin);
    struct nu_gpio_irq_var *var = gpio_irq_var_arr + port_index;
    
    NVIC_SetVector((var->gpio_n < 3) ? GPABC_IRQn : GPDEF_IRQn, (uint32_t) var->vec);
    NVIC_EnableIRQ((var->gpio_n < 3) ? GPABC_IRQn : GPDEF_IRQn);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    uint32_t port_index = NU_PINNAME_TO_PORT(obj->pin);
    struct nu_gpio_irq_var *var = gpio_irq_var_arr + port_index;
    
    NVIC_DisableIRQ((var->gpio_n < 3) ? GPABC_IRQn : GPDEF_IRQn);
}

void GPABC_IRQHandler(void)
{
    if (PA->ISRC)
        gpio_irq(gpio_irq_var_arr + 0);
    if (PB->ISRC)
        gpio_irq(gpio_irq_var_arr + 1);
    if (PC->ISRC)
        gpio_irq(gpio_irq_var_arr + 2);
}
void GPDEF_IRQHandler(void)
{
    if (PD->ISRC)
        gpio_irq(gpio_irq_var_arr + 3);
    if (PE->ISRC)
        gpio_irq(gpio_irq_var_arr + 4);
    if (PF->ISRC)
        gpio_irq(gpio_irq_var_arr + 5);
}

static void gpio_irq(struct nu_gpio_irq_var *var)
{
    uint32_t port_index = var->gpio_n;
    GPIO_T *gpio_base = NU_PORT_BASE(port_index);
    
    uint32_t isrc = gpio_base->ISRC;
    uint32_t ier = gpio_base->IER;
    while (isrc) {
        int pin_index = nu_ctz(isrc);
        PinName pin = (PinName) NU_PINNAME(port_index, pin_index);
        gpio_irq_t *obj = var->obj_arr;
        while (obj) {
            if (obj->pin == pin)
                break;
            obj = obj->next;
        }
        if (ier & (GPIO_INT_RISING << pin_index)) {
            if (GPIO_PIN_ADDR(port_index, pin_index)) {
                if (obj && obj->irq_handler) {
                    ((gpio_irq_handler) obj->irq_handler)(obj->irq_id, IRQ_RISE);
                }
            }
        }
        
        if (ier & (GPIO_INT_FALLING << pin_index)) {   
            if (! GPIO_PIN_ADDR(port_index, pin_index)) {
                if (obj && obj->irq_handler) {
                    ((gpio_irq_handler) obj->irq_handler)(obj->irq_id, IRQ_FALL);
                }
            }
        }
        
        isrc &= ~(1 << pin_index);
    }
    // Clear all interrupt flags
    gpio_base->ISRC = gpio_base->ISRC;
}

#endif
