/* 
 * Copyright (c) 2015 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */


#include "nrf_drv_gpiote.h"
#include "nrf_drv_common.h"
#include "nrf_drv_config.h"
#include "app_util_platform.h"
#include "nrf_assert.h"

#define FORBIDDEN_HANDLER_ADDRESS ((nrf_drv_gpiote_evt_handler_t)UINT32_MAX)
#define PIN_NOT_USED              (-1)
#define PIN_USED                  (-2)
#define NO_CHANNELS               (-1)
#define SENSE_FIELD_POS           (6)
#define SENSE_FIELD_MASK          (0xC0)

/**
 * @brief Macro for conveting task-event index to an address of an event register.
 *
 * Macro utilizes the fact that registers are grouped together in ascending order.
 */
#define TE_IDX_TO_EVENT_ADDR(idx)   (nrf_gpiote_events_t)((uint32_t)NRF_GPIOTE_EVENTS_IN_0+(sizeof(uint32_t)*(idx)))

/**
 * @brief Macro for conveting task-event index to an address of a task register.
 *
 * Macro utilizes the fact that registers are grouped together in ascending order.
 */
#define TE_IDX_TO_TASK_ADDR(idx)   (nrf_gpiote_tasks_t)((uint32_t)NRF_GPIOTE_TASKS_OUT_0+(sizeof(uint32_t)*(idx)))

//lint -save -e661
typedef struct
{
    nrf_drv_gpiote_evt_handler_t handlers[NUMBER_OF_GPIO_TE+GPIOTE_CONFIG_NUM_OF_LOW_POWER_EVENTS];
    int8_t                       pin_assignments[NUMBER_OF_PINS];
    int8_t                       port_handlers_pins[GPIOTE_CONFIG_NUM_OF_LOW_POWER_EVENTS];
    nrf_drv_state_t              state;
} gpiote_control_block_t;

static gpiote_control_block_t m_cb;

__STATIC_INLINE bool pin_in_use(uint32_t pin)
{
    return (m_cb.pin_assignments[pin] != PIN_NOT_USED);
}

__STATIC_INLINE bool pin_in_use_as_non_task_out(uint32_t pin)
{
    return (m_cb.pin_assignments[pin] == PIN_USED);
}

__STATIC_INLINE bool pin_in_use_by_te(uint32_t pin)
{
    return (m_cb.pin_assignments[pin] >= 0 && m_cb.pin_assignments[pin] < NUMBER_OF_GPIO_TE) ? true : false;
}

__STATIC_INLINE bool pin_in_use_by_port(uint32_t pin)
{
    return (m_cb.pin_assignments[pin] >= NUMBER_OF_GPIO_TE);
}

__STATIC_INLINE bool pin_in_use_by_gpiote(uint32_t pin)
{
    return (m_cb.pin_assignments[pin] >= 0);
}

__STATIC_INLINE void pin_in_use_by_te_set(uint32_t pin,
                                          uint32_t channel_id,
                                          nrf_drv_gpiote_evt_handler_t handler,
                                          bool is_channel)
{
    m_cb.pin_assignments[pin] = channel_id;
    m_cb.handlers[channel_id] = handler;
    if (!is_channel)
    {
        m_cb.port_handlers_pins[channel_id-NUMBER_OF_GPIO_TE] = (int8_t)pin;
    }
}

__STATIC_INLINE void pin_in_use_set(uint32_t pin)
{
    m_cb.pin_assignments[pin] = PIN_USED;
}

__STATIC_INLINE void pin_in_use_clear(uint32_t pin)
{
    m_cb.pin_assignments[pin] = PIN_NOT_USED;
}

__STATIC_INLINE int8_t channel_port_get(uint32_t pin)
{
    return m_cb.pin_assignments[pin];
}

__STATIC_INLINE nrf_drv_gpiote_evt_handler_t channel_handler_get(uint32_t channel)
{
    return m_cb.handlers[channel];
}

static int8_t channel_port_alloc(uint32_t pin,nrf_drv_gpiote_evt_handler_t handler, bool channel)
{
    int8_t channel_id = NO_CHANNELS;
    uint32_t i;

    uint32_t start_idx = channel ? 0 : NUMBER_OF_GPIO_TE;
    uint32_t end_idx = channel ? NUMBER_OF_GPIO_TE : (NUMBER_OF_GPIO_TE+GPIOTE_CONFIG_NUM_OF_LOW_POWER_EVENTS);
    //critical section

    for (i = start_idx; i < end_idx; i++)
    {
        if (m_cb.handlers[i] == FORBIDDEN_HANDLER_ADDRESS)
        {
            pin_in_use_by_te_set(pin, i, handler, channel);
            channel_id = i;
            break;
        }
    }
    //critical section
    return channel_id;
}

static void channel_free(uint8_t channel_id)
{
    m_cb.handlers[channel_id] = FORBIDDEN_HANDLER_ADDRESS;
    if (channel_id >= NUMBER_OF_GPIO_TE)
    {
        m_cb.port_handlers_pins[channel_id-NUMBER_OF_GPIO_TE] = (int8_t)PIN_NOT_USED;
    }
}

ret_code_t nrf_drv_gpiote_init(void)
{
    if (m_cb.state != NRF_DRV_STATE_UNINITIALIZED)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    uint8_t i;
    for (i = 0; i < NUMBER_OF_PINS; i++)
    {
        pin_in_use_clear(i);
    }
    for (i = 0; i < (NUMBER_OF_GPIO_TE+GPIOTE_CONFIG_NUM_OF_LOW_POWER_EVENTS); i++)
    {
        channel_free(i);
    }

    nrf_drv_common_irq_enable(GPIOTE_IRQn, GPIOTE_CONFIG_IRQ_PRIORITY);
    nrf_gpiote_int_enable(GPIOTE_INTENSET_PORT_Msk);
    m_cb.state = NRF_DRV_STATE_INITIALIZED;

    return NRF_SUCCESS;
}

bool nrf_drv_gpiote_is_init(void)
{
    return (m_cb.state != NRF_DRV_STATE_UNINITIALIZED) ? true : false;
}

void nrf_drv_gpiote_uninit(void)
{
    ASSERT(m_cb.state!=NRF_DRV_STATE_UNINITIALIZED);

    uint32_t i;
    for (i = 0; i < NUMBER_OF_PINS; i++)
    {
        if (pin_in_use_as_non_task_out(i))
        {
            nrf_drv_gpiote_out_uninit(i);
        }
        else if( pin_in_use_by_gpiote(i))
        {
            /* Disable gpiote_in is having the same effect on out pin as gpiote_out_uninit on
             * so it can be called on all pins used by GPIOTE.
             */
            nrf_drv_gpiote_in_uninit(i);
        }
    }
    m_cb.state = NRF_DRV_STATE_UNINITIALIZED;
}

ret_code_t nrf_drv_gpiote_out_init(nrf_drv_gpiote_pin_t pin, 
                                   nrf_drv_gpiote_out_config_t const * p_config)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ASSERT(m_cb.state == NRF_DRV_STATE_INITIALIZED);
    ASSERT(p_config);

    ret_code_t result = NRF_SUCCESS;

    if (pin_in_use(pin))
    {
        result = NRF_ERROR_INVALID_STATE;
    }
    else
    {
        if (p_config->task_pin)
        {
            int8_t channel = channel_port_alloc(pin, NULL, true);

            if (channel != NO_CHANNELS)
            {
                nrf_gpiote_task_configure(channel, pin, p_config->action, p_config->init_state);
            }
            else
            {
                result = NRF_ERROR_NO_MEM;
            }
        }
        else
        {
            pin_in_use_set(pin);
        }

        if (result == NRF_SUCCESS)
        {
            if (p_config->init_state == NRF_GPIOTE_INITIAL_VALUE_HIGH)
            {
                nrf_gpio_pin_set(pin);
            }
            else
            {
                nrf_gpio_pin_clear(pin);
            }
            
            nrf_gpio_cfg_output(pin);
        }
    }

    return result;
}

void nrf_drv_gpiote_out_uninit(nrf_drv_gpiote_pin_t pin)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ASSERT(pin_in_use(pin));

    if (pin_in_use_by_te(pin))
    {
        channel_free((uint8_t)channel_port_get(pin));
        nrf_gpiote_te_default(channel_port_get(pin));
    }
    pin_in_use_clear(pin);

    nrf_gpio_cfg_default(pin);
}

void nrf_drv_gpiote_out_set(nrf_drv_gpiote_pin_t pin)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ASSERT(pin_in_use(pin));
    ASSERT(!pin_in_use_by_te(pin))

    nrf_gpio_pin_set(pin);
}

void nrf_drv_gpiote_out_clear(nrf_drv_gpiote_pin_t pin)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ASSERT(pin_in_use(pin));
    ASSERT(!pin_in_use_by_te(pin))

    nrf_gpio_pin_clear(pin);
}

void nrf_drv_gpiote_out_toggle(nrf_drv_gpiote_pin_t pin)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ASSERT(pin_in_use(pin));
    ASSERT(!pin_in_use_by_te(pin))

    nrf_gpio_pin_toggle(pin);
}

void nrf_drv_gpiote_out_task_enable(nrf_drv_gpiote_pin_t pin)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ASSERT(pin_in_use(pin));
    ASSERT(pin_in_use_by_te(pin))

    nrf_gpiote_task_enable(m_cb.pin_assignments[pin]);
}

void nrf_drv_gpiote_out_task_disable(nrf_drv_gpiote_pin_t pin)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ASSERT(pin_in_use(pin));
    ASSERT(pin_in_use_by_te(pin))

    nrf_gpiote_task_disable(m_cb.pin_assignments[pin]);
}

uint32_t nrf_drv_gpiote_out_task_addr_get(nrf_drv_gpiote_pin_t pin)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ASSERT(pin_in_use_by_te(pin));
    
    nrf_gpiote_tasks_t task = TE_IDX_TO_TASK_ADDR(channel_port_get(pin));
    return nrf_gpiote_task_addr_get(task);
}

void nrf_drv_gpiote_out_task_force(nrf_drv_gpiote_pin_t pin, uint8_t state)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ASSERT(pin_in_use(pin));
    ASSERT(pin_in_use_by_te(pin));
    
    nrf_gpiote_outinit_t init_val = state ? NRF_GPIOTE_INITIAL_VALUE_HIGH : NRF_GPIOTE_INITIAL_VALUE_LOW;
    nrf_gpiote_task_force(m_cb.pin_assignments[pin], init_val);
}

void nrf_drv_gpiote_out_task_trigger(nrf_drv_gpiote_pin_t pin)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ASSERT(pin_in_use(pin));
    ASSERT(pin_in_use_by_te(pin));

    nrf_gpiote_tasks_t task = TE_IDX_TO_TASK_ADDR(channel_port_get(pin));;
    nrf_gpiote_task_set(task);
}

ret_code_t nrf_drv_gpiote_in_init(nrf_drv_gpiote_pin_t pin,
                                  nrf_drv_gpiote_in_config_t const * p_config,
                                  nrf_drv_gpiote_evt_handler_t evt_handler)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ret_code_t result = NRF_SUCCESS;
    /* Only one GPIOTE channel can be assigned to one physical pin. */
    if (pin_in_use_by_gpiote(pin))
    {
        result = NRF_ERROR_INVALID_STATE;
    }
    else
    {
        int8_t channel = channel_port_alloc(pin, evt_handler, p_config->hi_accuracy);
        if (channel != NO_CHANNELS)
        {
            if (p_config->is_watcher)
            {
                nrf_gpio_cfg_watcher(pin);
            }
            else
            {
                nrf_gpio_cfg_input(pin,p_config->pull);
            }

            if (p_config->hi_accuracy)
            {
                nrf_gpiote_event_configure(channel, pin,p_config->sense);
            }
            else
            {
                m_cb.port_handlers_pins[channel-NUMBER_OF_GPIO_TE] |= (p_config->sense)<< SENSE_FIELD_POS;
            }
        }
        else
        {
            result = NRF_ERROR_NO_MEM;
        }
    }
    return result;
}

void nrf_drv_gpiote_in_event_enable(nrf_drv_gpiote_pin_t pin, bool int_enable)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ASSERT(pin_in_use_by_gpiote(pin));
    if (pin_in_use_by_port(pin))
    {
        uint8_t pin_and_sense = m_cb.port_handlers_pins[channel_port_get(pin)-NUMBER_OF_GPIO_TE];
        nrf_gpiote_polarity_t polarity = (nrf_gpiote_polarity_t)(pin_and_sense >> SENSE_FIELD_POS);
        nrf_gpio_pin_sense_t sense;
        if (polarity == NRF_GPIOTE_POLARITY_TOGGLE)
        {
            /* read current pin state and set for next sense to oposit */
            sense = (nrf_gpio_pins_read() & (1 << pin)) ?
                    NRF_GPIO_PIN_SENSE_LOW : NRF_GPIO_PIN_SENSE_HIGH;
        }
        else
        {
            sense = (polarity == NRF_GPIOTE_POLARITY_LOTOHI) ?
                    NRF_GPIO_PIN_SENSE_HIGH : NRF_GPIO_PIN_SENSE_LOW;
        }
        nrf_gpio_cfg_sense_set(pin,sense);
    }
    else if(pin_in_use_by_te(pin))
    {
        int32_t channel = (int32_t)channel_port_get(pin);
        nrf_gpiote_events_t event = TE_IDX_TO_EVENT_ADDR(channel);
       
        nrf_gpiote_event_enable(channel);

        nrf_gpiote_event_clear(event);
        if (int_enable)
        {
            nrf_drv_gpiote_evt_handler_t handler = channel_handler_get(channel_port_get(pin));
            // Enable the interrupt only if event handler was provided.
            if (handler)
            {
                nrf_gpiote_int_enable(1 << channel);
            }
        }
    }
}

void nrf_drv_gpiote_in_event_disable(nrf_drv_gpiote_pin_t pin)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ASSERT(pin_in_use_by_gpiote(pin));
    if (pin_in_use_by_port(pin))
    {
        nrf_gpio_cfg_sense_set(pin,NRF_GPIO_PIN_NOSENSE);
    }
    else if(pin_in_use_by_te(pin))
    {
        int32_t channel = (int32_t)channel_port_get(pin);
        nrf_gpiote_event_disable(channel);
        nrf_gpiote_int_disable(1 << channel);
    }
}

void nrf_drv_gpiote_in_uninit(nrf_drv_gpiote_pin_t pin)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ASSERT(pin_in_use_by_gpiote(pin));
    nrf_drv_gpiote_in_event_disable(pin);
    if(pin_in_use_by_te(pin))
    {
        nrf_gpiote_te_default(channel_port_get(pin));
    }
    nrf_gpio_cfg_default(pin);
    channel_free((uint8_t)channel_port_get(pin));
    pin_in_use_clear(pin);
}

bool nrf_drv_gpiote_in_is_set(nrf_drv_gpiote_pin_t pin)
{
    ASSERT(pin < NUMBER_OF_PINS);
    return nrf_gpio_pin_read(pin) ? true : false;
}

uint32_t nrf_drv_gpiote_in_event_addr_get(nrf_drv_gpiote_pin_t pin)
{
    ASSERT(pin < NUMBER_OF_PINS);
    ASSERT(pin_in_use_by_te(pin));
    
    nrf_gpiote_events_t event = TE_IDX_TO_EVENT_ADDR(channel_port_get(pin));
    return nrf_gpiote_event_addr_get(event);
}

void GPIOTE_IRQHandler(void)
{
    uint32_t status = 0;
    uint32_t input = 0;

    /* collect status of all GPIOTE pin events. Processing is done once all are collected and cleared.*/
    uint32_t i;
    nrf_gpiote_events_t event = NRF_GPIOTE_EVENTS_IN_0;
    uint32_t mask = (uint32_t)NRF_GPIOTE_INT_IN0_MASK;
    for (i = 0; i < NUMBER_OF_GPIO_TE; i++)
    {
        if (nrf_gpiote_event_is_set(event) && nrf_gpiote_int_is_enabled(mask))
        {
            nrf_gpiote_event_clear(event);
            status |= mask;
        }
        mask <<= 1;
        /* Incrementing to next event, utilizing the fact that events are grouped together
         * in ascending order. */
        event = (nrf_gpiote_events_t)((uint32_t)event + sizeof(uint32_t));
    }

    /* collect PORT status event, if event is set read pins state. Processing is postponed to the
     * end of interrupt. */
    if (nrf_gpiote_event_is_set(NRF_GPIOTE_EVENTS_PORT))
    {
        nrf_gpiote_event_clear(NRF_GPIOTE_EVENTS_PORT);
        status |= (uint32_t)NRF_GPIOTE_INT_PORT_MASK;
        input = nrf_gpio_pins_read();
    }

    /* Process pin events. */
    if (status & NRF_GPIOTE_INT_IN_MASK)
    {
        mask = (uint32_t)NRF_GPIOTE_INT_IN0_MASK;
        for (i = 0; i < NUMBER_OF_GPIO_TE; i++)
        {
            if (mask & status)
            {
                nrf_drv_gpiote_pin_t pin = nrf_gpiote_event_pin_get(i);
                nrf_gpiote_polarity_t polarity = nrf_gpiote_event_polarity_get(i);
                nrf_drv_gpiote_evt_handler_t handler = channel_handler_get(i);
                handler(pin,polarity);
            }
            mask <<= 1;
        }
    }

    if (status & (uint32_t)NRF_GPIOTE_INT_PORT_MASK)
    {
        /* Process port event. */
        uint8_t repeat = 0;
        uint32_t toggle_mask = 0;
        uint32_t pins_to_check = 0xFFFFFFFFuL;

        do
        {
            repeat = 0;
            for (i = 0; i < GPIOTE_CONFIG_NUM_OF_LOW_POWER_EVENTS; i++)
            {
                uint8_t pin_and_sense = m_cb.port_handlers_pins[i];
                nrf_drv_gpiote_pin_t pin = (pin_and_sense & ~SENSE_FIELD_MASK);

                if ((m_cb.port_handlers_pins[i] != PIN_NOT_USED)
                    && ((1UL << pin) & pins_to_check))
                {
                    nrf_gpiote_polarity_t polarity =
                                (nrf_gpiote_polarity_t)((pin_and_sense & SENSE_FIELD_MASK) >> SENSE_FIELD_POS);
                    nrf_drv_gpiote_evt_handler_t handler = channel_handler_get(channel_port_get(pin));
                    if (handler || polarity == NRF_GPIOTE_POLARITY_TOGGLE)
                    {
                        mask = 1 << pin;
                        if (polarity == NRF_GPIOTE_POLARITY_TOGGLE)
                        {
                            toggle_mask |= mask;
                        }
                        nrf_gpio_pin_sense_t sense = nrf_gpio_pin_sense_get(pin);

                        if (((mask & input) && (sense==NRF_GPIO_PIN_SENSE_HIGH)) ||
                           (!(mask & input) && (sense==NRF_GPIO_PIN_SENSE_LOW))  )
                        {
                            if (polarity == NRF_GPIOTE_POLARITY_TOGGLE)
                            {
                                nrf_gpio_pin_sense_t next_sense = (sense == NRF_GPIO_PIN_SENSE_HIGH) ?
                                        NRF_GPIO_PIN_SENSE_LOW : NRF_GPIO_PIN_SENSE_HIGH;
                                nrf_gpio_cfg_sense_set(pin, next_sense);
                                ++repeat;
                            }
                            if (handler)
                            {
                                handler(pin, polarity);
                            }
                        }
                    }
                }
            }

            if (repeat)
            {
                // When one of the pins in low-accuracy and toggle mode becomes active,
                // it's sense mode is inverted to clear the internal SENSE signal.
                // State of any other enabled low-accuracy input in toggle mode must be checked
                // explicitly, because it does not trigger the interrput when SENSE signal is active.
                // For more information about SENSE functionality, refer to Product Specification.
                uint32_t new_input = nrf_gpio_pins_read();
                if (new_input == input)
                {
                    //No change.
                    repeat = 0;
                }
                else
                {
                    input = new_input;
                    pins_to_check = toggle_mask;
                }
            }
        }
        while (repeat);
    }
}
//lint -restore
