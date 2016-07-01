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


#include "nrf_drv_adc.h"
#include "nrf_drv_common.h"
#include "nrf_assert.h"
#include "app_util_platform.h"
#include "app_util.h"

typedef struct
{
    nrf_drv_adc_event_handler_t event_handler;
    nrf_drv_adc_channel_t     * p_head;
    nrf_drv_adc_channel_t     * p_current_conv;
    nrf_adc_value_t           * p_buffer;
    uint8_t                     size;
    uint8_t                     idx;
    nrf_drv_state_t             state;
} adc_cb_t;

static adc_cb_t m_cb;
static const nrf_drv_adc_config_t m_default_config = NRF_DRV_ADC_DEFAULT_CONFIG;

ret_code_t nrf_drv_adc_init(nrf_drv_adc_config_t const * p_config,
                            nrf_drv_adc_event_handler_t  event_handler)
{
    if (m_cb.state != NRF_DRV_STATE_UNINITIALIZED)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    nrf_adc_event_clear(NRF_ADC_EVENT_END);
    if (event_handler)
    {
        if (!p_config)
        {
            p_config = (nrf_drv_adc_config_t *)&m_default_config;
        }
        nrf_drv_common_irq_enable(ADC_IRQn, p_config->interrupt_priority);
    }
    m_cb.event_handler = event_handler;
    m_cb.state = NRF_DRV_STATE_INITIALIZED;

    return NRF_SUCCESS;
}

void nrf_drv_adc_uninit(void)
{
    m_cb.p_head = NULL;
    nrf_drv_common_irq_disable(ADC_IRQn);
    nrf_adc_int_disable(NRF_ADC_INT_END_MASK);
    nrf_adc_task_trigger(NRF_ADC_TASK_STOP);

    m_cb.state = NRF_DRV_STATE_UNINITIALIZED;
}

void nrf_drv_adc_channel_enable(nrf_drv_adc_channel_t * const p_channel)
{
    ASSERT(mp_state == NRF_DRV_STATE_INITIALIZED);
    ASSERT(!is_address_from_stack(p_channel));

    p_channel->p_next = NULL;
    if (m_cb.p_head == NULL)
    {
        m_cb.p_head = p_channel;
    }
    else
    {
        nrf_drv_adc_channel_t * p_curr_channel = m_cb.p_head;
        while (p_curr_channel->p_next != NULL)
        {
            ASSERT(p_channel != p_curr_channel);
            p_curr_channel = p_curr_channel->p_next;
        }
        p_curr_channel->p_next = p_channel;
    }
}

void nrf_drv_adc_channel_disable(nrf_drv_adc_channel_t * const p_channel)
{
    ASSERT(mp_state == NRF_DRV_STATE_INITIALIZED);
    ASSERT(m_cb.p_head);

    nrf_drv_adc_channel_t * p_curr_channel = m_cb.p_head;
    nrf_drv_adc_channel_t * p_prev_channel = NULL;
    while(p_curr_channel != p_channel)
    {
        p_prev_channel = p_curr_channel;
        p_curr_channel = p_curr_channel->p_next;
        ASSERT(p_curr_channel == NULL);
    }
    if (p_prev_channel)
    {
        p_prev_channel->p_next = p_curr_channel->p_next;
    }
    else
    {
        m_cb.p_head = p_curr_channel->p_next;
    }
}

void nrf_drv_adc_sample(void)
{
    ASSERT(mp_state != NRF_DRV_STATE_UNINITIALIZED);
    ASSERT(!nrf_adc_is_busy());
    nrf_adc_start();
}

ret_code_t nrf_drv_adc_sample_convert(nrf_drv_adc_channel_t const * const p_channel, 
                                      nrf_adc_value_t * p_value)
{
    ASSERT(mp_state != NRF_DRV_STATE_UNINITIALIZED);
    if(m_cb.state == NRF_DRV_STATE_POWERED_ON)
    {
        return NRF_ERROR_BUSY;
    }
    else
    {
        m_cb.state = NRF_DRV_STATE_POWERED_ON;

        nrf_adc_config_set(p_channel->config.data);
        nrf_adc_enable();
        nrf_adc_int_disable(NRF_ADC_INT_END_MASK);
        nrf_adc_start();
        if (p_value)
        {
            while(!nrf_adc_event_check(NRF_ADC_EVENT_END)) {}
            nrf_adc_event_clear(NRF_ADC_EVENT_END);
            *p_value = (nrf_adc_value_t)nrf_adc_result_get();
            nrf_adc_disable();

            m_cb.state = NRF_DRV_STATE_INITIALIZED;
        }
        else
        {
            ASSERT(m_cb.event_handler);
            m_cb.p_buffer = NULL;
            nrf_adc_int_enable(NRF_ADC_INT_END_MASK);
        }
        return NRF_SUCCESS;
    }
}

static bool adc_sample_process()
{
    nrf_adc_event_clear(NRF_ADC_EVENT_END);
    nrf_adc_disable();
    m_cb.p_buffer[m_cb.idx] = (nrf_adc_value_t)nrf_adc_result_get();
    m_cb.idx++;
    if (m_cb.idx < m_cb.size)
    {
        bool task_trigger = false;
        if (m_cb.p_current_conv->p_next == NULL)
        {
            m_cb.p_current_conv = m_cb.p_head;
        }
        else
        {
            m_cb.p_current_conv = m_cb.p_current_conv->p_next;
            task_trigger = true;
        }
        nrf_adc_config_set(m_cb.p_current_conv->config.data);
        nrf_adc_enable();
        if (task_trigger)
        {
            //nrf_adc_start();
            nrf_adc_task_trigger(NRF_ADC_TASK_START);
        }
        return false;
    }
    else
    {
        return true;
    }
}

ret_code_t nrf_drv_adc_buffer_convert(nrf_adc_value_t * buffer, uint16_t size)
{
    ASSERT(mp_state != NRF_DRV_STATE_UNINITIALIZED);
    if(m_cb.state == NRF_DRV_STATE_POWERED_ON)
    {
        return NRF_ERROR_BUSY;
    }
    else
    {
        m_cb.state          = NRF_DRV_STATE_POWERED_ON;
        m_cb.p_current_conv = m_cb.p_head;
        m_cb.size           = size;
        m_cb.idx            = 0;
        m_cb.p_buffer       = buffer;
        nrf_adc_config_set(m_cb.p_current_conv->config.data);
        nrf_adc_event_clear(NRF_ADC_EVENT_END);
        nrf_adc_enable();
        if (m_cb.event_handler)
        {
            nrf_adc_int_enable(NRF_ADC_INT_END_MASK);
        }
        else
        {
            while(1)
            {
                while(!nrf_adc_event_check(NRF_ADC_EVENT_END)){}

                if (adc_sample_process())
                {
                    m_cb.state = NRF_DRV_STATE_INITIALIZED;
                    break;
                }
            }
        }
        return NRF_SUCCESS;
    }
}

bool nrf_drv_adc_is_busy(void)
{
    ASSERT(mp_state != NRF_DRV_STATE_UNINITIALIZED);
    return (m_cb.state == NRF_DRV_STATE_POWERED_ON) ? true : false;
}

void ADC_IRQHandler(void)
{
    if (m_cb.p_buffer == NULL)
    {
        nrf_adc_event_clear(NRF_ADC_EVENT_END);
        nrf_adc_int_disable(NRF_ADC_INT_END_MASK);
        nrf_adc_disable();
        nrf_drv_adc_evt_t evt;
        evt.type = NRF_DRV_ADC_EVT_SAMPLE;
        evt.data.sample.sample = (nrf_adc_value_t)nrf_adc_result_get();
        m_cb.state = NRF_DRV_STATE_INITIALIZED;
        m_cb.event_handler(&evt);
    }
    else if (adc_sample_process())
    {
        nrf_adc_int_disable(NRF_ADC_INT_END_MASK);
        nrf_drv_adc_evt_t evt;
        evt.type = NRF_DRV_ADC_EVT_DONE;
        evt.data.done.p_buffer = m_cb.p_buffer;
        evt.data.done.size     = m_cb.size;
        m_cb.state = NRF_DRV_STATE_INITIALIZED;
        m_cb.event_handler(&evt);
    }
}
