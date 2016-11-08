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
#include "gpio_api.h"
#include "gpio_irq_api.h"
#include "pinmap.h"
#include "nrf_drv_gpiote.h"


#if defined(TARGET_MCU_NRF51822)
    #define GPIO_PIN_COUNT 31
#else
    #define GPIO_PIN_COUNT 32
#endif

typedef struct {
    bool         used_as_gpio : 1;
    PinDirection direction    : 1;
    bool         init_high    : 1;
    PinMode      pull         : 2;
    bool         used_as_irq  : 1;
    bool         irq_fall     : 1;
    bool         irq_rise     : 1;
} gpio_cfg_t;

uint32_t m_gpio_initialized;
gpio_cfg_t m_gpio_cfg[GPIO_PIN_COUNT];


/***********
  GPIO IRQ
***********/

static gpio_irq_handler m_irq_handler;
static uint32_t m_channel_ids[GPIO_PIN_COUNT] = {0};
uint32_t m_gpio_irq_enabled;


static void gpiote_irq_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    nrf_gpio_pin_sense_t sense =  nrf_gpio_pin_sense_get(pin);
    gpio_irq_event event = (sense == NRF_GPIO_PIN_SENSE_LOW) ? IRQ_RISE : IRQ_FALL;
    
    if (m_gpio_irq_enabled & (1UL << pin)) {
        if (((event == IRQ_RISE) && m_gpio_cfg[pin].irq_rise)
            || ((event == IRQ_FALL) && m_gpio_cfg[pin].irq_fall)) {
                m_irq_handler(m_channel_ids[pin], event);
            }
    }
}

void GPIOTE_IRQHandler(void);// exported from nrf_drv_gpiote.c

void gpio_init(gpio_t *obj, PinName pin)
{
    obj->pin = pin;
    if (pin == (PinName)NC) {
        return;
    }
    MBED_ASSERT((uint32_t)pin < GPIO_PIN_COUNT);
    
    NVIC_SetVector(GPIOTE_IRQn, (uint32_t) GPIOTE_IRQHandler);
    
    (void) nrf_drv_gpiote_init();

    m_gpio_cfg[obj->pin].used_as_gpio = true;
}


int gpio_read(gpio_t *obj)
{
    MBED_ASSERT(obj->pin != (PinName)NC);
    if (m_gpio_cfg[obj->pin].direction == PIN_OUTPUT) {
        return ((NRF_GPIO->OUTSET & (1UL << obj->pin)) ? 1 : 0);
    } else {
        return nrf_gpio_pin_read(obj->pin);
    }
}

static void gpiote_pin_uninit(uint8_t pin)
{
    if (m_gpio_initialized & (1UL << pin)) {
        if ((m_gpio_cfg[pin].direction == PIN_OUTPUT) && (!m_gpio_cfg[pin].used_as_irq)) {
            nrf_drv_gpiote_out_uninit(pin);
        }
        else {
            nrf_drv_gpiote_in_uninit(pin);
        }
    }
}

static void gpio_apply_config(uint8_t pin)
{
    if (m_gpio_cfg[pin].used_as_gpio || m_gpio_cfg[pin].used_as_irq) {
        if ((m_gpio_cfg[pin].direction == PIN_INPUT)
            || (m_gpio_cfg[pin].used_as_irq)) {
            //Configure as input.
            nrf_drv_gpiote_in_config_t cfg;

            cfg.hi_accuracy = false;
            cfg.is_watcher = false;
            cfg.sense = NRF_GPIOTE_POLARITY_TOGGLE;
            if (m_gpio_cfg[pin].used_as_irq) {
                cfg.pull = NRF_GPIO_PIN_PULLUP;
                nrf_drv_gpiote_in_init(pin, &cfg, gpiote_irq_handler);
                if ((m_gpio_irq_enabled & (1 << pin))
                    && (m_gpio_cfg[pin].irq_rise || m_gpio_cfg[pin].irq_fall))
                {
                    nrf_drv_gpiote_in_event_enable(pin, true);
                }
            }
            else {
                switch(m_gpio_cfg[pin].pull) {
                    case PullUp:
                        cfg.pull = NRF_GPIO_PIN_PULLUP;
                    break;
                    case PullDown:
                        cfg.pull = NRF_GPIO_PIN_PULLDOWN;
                    break;
                    default:
                        cfg.pull = NRF_GPIO_PIN_NOPULL;
                    break;
                }
                nrf_drv_gpiote_in_init(pin, &cfg, NULL);
            }
        }
        else {
            // Configure as output.
            nrf_drv_gpiote_out_config_t cfg = GPIOTE_CONFIG_OUT_SIMPLE(m_gpio_cfg[pin].init_high);
            nrf_drv_gpiote_out_init(pin, &cfg);
        }
        m_gpio_initialized |= (1UL << pin);
    }
    else {
        m_gpio_initialized &= ~(1UL << pin);
    }
}


void gpio_mode(gpio_t *obj, PinMode mode)
{
    MBED_ASSERT(obj->pin <= GPIO_PIN_COUNT);
    
    gpiote_pin_uninit(obj->pin); // try to uninitialize gpio before a change.
    
    m_gpio_cfg[obj->pin].pull = mode;
    gpio_apply_config(obj->pin);
}


void gpio_dir(gpio_t *obj, PinDirection direction)
{
    MBED_ASSERT(obj->pin <= GPIO_PIN_COUNT);
    
    gpiote_pin_uninit(obj->pin); // try to uninitialize gpio before a change.
    
    m_gpio_cfg[obj->pin].direction = direction;
    gpio_apply_config(obj->pin);
}


/***********
  GPIO IRQ
***********/

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    if (pin == NC) {
        return -1;
    }
    MBED_ASSERT((uint32_t)pin < GPIO_PIN_COUNT);
    (void) nrf_drv_gpiote_init();

    gpiote_pin_uninit(pin); // try to uninitialize gpio before a change.
    
    m_gpio_cfg[pin].used_as_irq = true;
    m_channel_ids[pin] = id;
    obj->ch            = pin;
    m_irq_handler      = handler;
    m_channel_ids[pin] = id;

    gpio_apply_config(pin);
    return 1;
}


void gpio_irq_free(gpio_irq_t *obj)
{
    nrf_drv_gpiote_in_uninit(obj->ch);
    m_gpio_cfg[obj->ch].used_as_irq = false;
    m_channel_ids[obj->ch] = 0;

    gpio_apply_config(obj->ch);
}


void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    gpio_cfg_t* cfg = &m_gpio_cfg[obj->ch];
    bool irq_enabled_before =
        (m_gpio_irq_enabled & (1 << obj->ch)) &&
        (cfg->irq_rise || cfg->irq_fall);

    if (event == IRQ_RISE) {
        cfg->irq_rise = enable ? true : false;
    }
    else if (event == IRQ_FALL) {
        cfg->irq_fall = enable ? true : false;
    }

    bool irq_enabled_after = cfg->irq_rise || cfg->irq_fall;

    if (irq_enabled_before != irq_enabled_after) {
        if (irq_enabled_after) {
            gpio_irq_enable(obj);
        } else {
            gpio_irq_disable(obj);
        }
    }
}


void gpio_irq_enable(gpio_irq_t *obj)
{
    m_gpio_irq_enabled |= (1 << obj->ch);
    if (m_gpio_cfg[obj->ch].irq_rise || m_gpio_cfg[obj->ch].irq_fall) {
        nrf_drv_gpiote_in_event_enable(obj->ch, true);
    }
}


void gpio_irq_disable(gpio_irq_t *obj)
{
    m_gpio_irq_enabled &= ~(1 << obj->ch);
    nrf_drv_gpiote_in_event_disable(obj->ch);
}
