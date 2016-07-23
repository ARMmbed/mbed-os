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


#include "nrf_drv_timer.h"
#include "nrf_drv_common.h"
#include "app_util_platform.h"

#if (TIMER_COUNT == 0)
    #error "No TIMER instances enabled in the driver configuration file."
#endif


/**@brief Timer control block. */
typedef struct
{
    nrf_timer_event_handler_t handler;
    void *                    context;
    nrf_drv_state_t           state;
} timer_control_block_t;

static timer_control_block_t m_cb[TIMER_COUNT];

static const nrf_drv_timer_config_t m_default_config[TIMER_COUNT] = {
#if TIMER0_ENABLED
    NRF_DRV_TIMER_DEFAULT_CONFIG(0),
#endif
#if TIMER1_ENABLED
    NRF_DRV_TIMER_DEFAULT_CONFIG(1),
#endif
#if TIMER2_ENABLED
    NRF_DRV_TIMER_DEFAULT_CONFIG(2),
#endif
#if TIMER3_ENABLED
    NRF_DRV_TIMER_DEFAULT_CONFIG(3),
#endif
#if TIMER4_ENABLED
    NRF_DRV_TIMER_DEFAULT_CONFIG(4),
#endif
};


ret_code_t nrf_drv_timer_init(nrf_drv_timer_t const * const p_instance,
                              nrf_drv_timer_config_t const * p_config,
                              nrf_timer_event_handler_t timer_event_handler)
{
    timer_control_block_t * p_cb = &m_cb[p_instance->instance_id];

#ifdef SOFTDEVICE_PRESENT
    ASSERT(p_instance->p_reg != NRF_TIMER0);
#endif
    ASSERT(NRF_TIMER_IS_BIT_WIDTH_VALID(p_instance->p_reg, p_config->bit_width));

    if (p_cb->state != NRF_DRV_STATE_UNINITIALIZED)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (timer_event_handler == NULL)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    if (p_config == NULL)
    {
        p_config = &m_default_config[p_instance->instance_id];
    }

    p_cb->handler = timer_event_handler;
    p_cb->context = p_config->p_context;

    uint8_t i;
    for (i = 0; i < p_instance->cc_channel_count; ++i)
    {
        nrf_timer_event_clear(p_instance->p_reg,
            nrf_timer_compare_event_get(i));
    }

    nrf_drv_common_irq_enable(nrf_drv_get_IRQn(p_instance->p_reg),
        p_config->interrupt_priority);

    nrf_timer_mode_set(p_instance->p_reg, p_config->mode);
    nrf_timer_bit_width_set(p_instance->p_reg, p_config->bit_width);
    nrf_timer_frequency_set(p_instance->p_reg, p_config->frequency);

    p_cb->state = NRF_DRV_STATE_INITIALIZED;

    return NRF_SUCCESS;
}

void nrf_drv_timer_uninit(nrf_drv_timer_t const * const p_instance)
{
    nrf_drv_common_irq_disable(nrf_drv_get_IRQn(p_instance->p_reg));

    #define DISABLE_ALL UINT32_MAX
    nrf_timer_shorts_disable(p_instance->p_reg, DISABLE_ALL);
    nrf_timer_int_disable(p_instance->p_reg, DISABLE_ALL);
    #undef DISABLE_ALL

    nrf_drv_timer_disable(p_instance);

    m_cb[p_instance->instance_id].state = NRF_DRV_STATE_UNINITIALIZED;
}

void nrf_drv_timer_enable(nrf_drv_timer_t const * const p_instance)
{
    ASSERT(m_cb[p_instance->instance_id].state == NRF_DRV_STATE_INITIALIZED);
    nrf_timer_task_trigger(p_instance->p_reg, NRF_TIMER_TASK_START);
    m_cb[p_instance->instance_id].state = NRF_DRV_STATE_POWERED_ON;
}

void nrf_drv_timer_disable(nrf_drv_timer_t const * const p_instance)
{
    ASSERT(m_cb[p_instance->instance_id].state == NRF_DRV_STATE_POWERED_ON);
    nrf_timer_task_trigger(p_instance->p_reg, NRF_TIMER_TASK_SHUTDOWN);
    m_cb[p_instance->instance_id].state = NRF_DRV_STATE_INITIALIZED;
}

void nrf_drv_timer_resume(nrf_drv_timer_t const * const p_instance)
{
    ASSERT(m_cb[p_instance->instance_id].state == NRF_DRV_STATE_POWERED_ON);
    nrf_timer_task_trigger(p_instance->p_reg, NRF_TIMER_TASK_START);
}

void nrf_drv_timer_pause(nrf_drv_timer_t const * const p_instance)
{
    ASSERT(m_cb[p_instance->instance_id].state == NRF_DRV_STATE_POWERED_ON);
    nrf_timer_task_trigger(p_instance->p_reg, NRF_TIMER_TASK_STOP);
}

void nrf_drv_timer_clear(nrf_drv_timer_t const * const p_instance)
{
    ASSERT(m_cb[p_instance->instance_id].state != NRF_DRV_STATE_UNINITIALIZED);
    nrf_timer_task_trigger(p_instance->p_reg, NRF_TIMER_TASK_CLEAR);
}

void nrf_drv_timer_increment(nrf_drv_timer_t const * const p_instance)
{
    ASSERT(m_cb[p_instance->instance_id].state == NRF_DRV_STATE_POWERED_ON);
    ASSERT(nrf_timer_mode_get(p_instance->p_reg) != NRF_TIMER_MODE_TIMER);

    nrf_timer_task_trigger(p_instance->p_reg, NRF_TIMER_TASK_COUNT);
}

uint32_t nrf_drv_timer_capture(nrf_drv_timer_t const * const p_instance,
                               nrf_timer_cc_channel_t cc_channel)
{
    ASSERT(m_cb[p_instance->instance_id].state == NRF_DRV_STATE_POWERED_ON);
    ASSERT(cc_channel < p_instance->cc_channel_count);

    nrf_timer_task_trigger(p_instance->p_reg,
        nrf_timer_capture_task_get(cc_channel));
    return nrf_timer_cc_read(p_instance->p_reg, cc_channel);
}

void nrf_drv_timer_compare(nrf_drv_timer_t const * const p_instance,
                           nrf_timer_cc_channel_t cc_channel,
                           uint32_t               cc_value,
                           bool                   enable_int)
{
    nrf_timer_int_mask_t timer_int = nrf_timer_compare_int_get(cc_channel);

    if (enable_int)
    {
        nrf_timer_int_enable(p_instance->p_reg, timer_int);
    }
    else
    {
        nrf_timer_int_disable(p_instance->p_reg, timer_int);
    }

    nrf_timer_cc_write(p_instance->p_reg, cc_channel, cc_value);
}

void nrf_drv_timer_extended_compare(nrf_drv_timer_t const * const p_instance,
                                    nrf_timer_cc_channel_t cc_channel,
                                    uint32_t               cc_value,
                                    nrf_timer_short_mask_t timer_short_mask,
                                    bool                   enable_int)
{
    nrf_timer_shorts_disable(p_instance->p_reg,
        (TIMER_SHORTS_COMPARE0_STOP_Msk  << cc_channel) |
        (TIMER_SHORTS_COMPARE0_CLEAR_Msk << cc_channel));

    nrf_timer_shorts_enable(p_instance->p_reg, timer_short_mask);

    (void)nrf_drv_timer_compare(p_instance,
                                cc_channel,
                                cc_value,
                                enable_int);
}

void nrf_drv_timer_compare_int_enable(nrf_drv_timer_t const * const p_instance,
                                      uint32_t channel)
{
    ASSERT(m_cb[p_instance->instance_id].state != NRF_DRV_STATE_UNINITIALIZED);
    ASSERT(channel < p_instance->cc_channel_count);

    nrf_timer_event_clear(p_instance->p_reg,
        nrf_timer_compare_event_get(channel));
    nrf_timer_int_enable(p_instance->p_reg,
        nrf_timer_compare_int_get(channel));
}

void nrf_drv_timer_compare_int_disable(nrf_drv_timer_t const * const p_instance,
                                       uint32_t channel)
{
    ASSERT(m_cb[p_instance->instance_id].state != NRF_DRV_STATE_UNINITIALIZED);
    ASSERT(channel < p_instance->cc_channel_count);

    nrf_timer_int_disable(p_instance->p_reg,
        nrf_timer_compare_int_get(channel));
}

static void irq_handler(NRF_TIMER_Type * p_reg,
                        timer_control_block_t * p_cb,
                        uint8_t channel_count)
{
    uint8_t i;
    for (i = 0; i < channel_count; ++i)
    {
        nrf_timer_event_t event = nrf_timer_compare_event_get(i);
        nrf_timer_int_mask_t int_mask = nrf_timer_compare_int_get(i);

        if (nrf_timer_event_check(p_reg, event) &&
            nrf_timer_int_enable_check(p_reg, int_mask))
        {
            nrf_timer_event_clear(p_reg, event);
            p_cb->handler(event, p_cb->context);
        }
    }
}

#if TIMER0_ENABLED
void TIMER0_IRQHandler(void)
{
    irq_handler(NRF_TIMER0, &m_cb[TIMER0_INSTANCE_INDEX],
        NRF_TIMER_CC_CHANNEL_COUNT(0));
}
#endif

#if TIMER1_ENABLED
void TIMER1_IRQHandler(void)
{
    irq_handler(NRF_TIMER1, &m_cb[TIMER1_INSTANCE_INDEX],
        NRF_TIMER_CC_CHANNEL_COUNT(1));
}
#endif

#if TIMER2_ENABLED
void TIMER2_IRQHandler(void)
{
    irq_handler(NRF_TIMER2, &m_cb[TIMER2_INSTANCE_INDEX],
        NRF_TIMER_CC_CHANNEL_COUNT(2));
}
#endif

#if TIMER3_ENABLED
void TIMER3_IRQHandler(void)
{
    irq_handler(NRF_TIMER3, &m_cb[TIMER3_INSTANCE_INDEX],
        NRF_TIMER_CC_CHANNEL_COUNT(3));
}
#endif

#if TIMER4_ENABLED
void TIMER4_IRQHandler(void)
{
    irq_handler(NRF_TIMER4, &m_cb[TIMER4_INSTANCE_INDEX],
        NRF_TIMER_CC_CHANNEL_COUNT(4));
}
#endif
