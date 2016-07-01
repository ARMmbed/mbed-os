/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "nrf_drv_saadc.h"
#include "nrf_assert.h"
#include "nordic_common.h"
#include "nrf_drv_common.h"
#include "app_util_platform.h"


typedef enum
{
    NRF_SAADC_STATE_IDLE = 0,
    NRF_SAADC_STATE_BUSY = 1
} nrf_saadc_state_t;


typedef struct
{
    nrf_saadc_input_t pselp;
    nrf_saadc_input_t pseln;
} nrf_saadc_psel_buffer;

static const nrf_drv_saadc_config_t m_default_config = NRF_DRV_SAADC_DEFAULT_CONFIG;

/** @brief SAADC control block.*/
typedef struct
{
    nrf_drv_saadc_event_handler_t event_handler;                 ///< Event handler function pointer.
    volatile nrf_saadc_value_t *  p_buffer;                        ///< Sample buffer.
    volatile uint16_t             buffer_size;                   ///< Size of the sample buffer.
#ifdef NRF52_PAN_28
    volatile uint16_t             buffer_pos;                    ///< Current sample buffer position.
#endif
    volatile nrf_saadc_value_t *  p_secondary_buffer;            ///< Secondary sample buffer.
    uint32_t                      limits_enabled_flags;          ///< Enabled limits flags.
    uint16_t                      secondary_buffer_size;         ///< Size of the secondary buffer.
    nrf_saadc_psel_buffer         psel[NRF_SAADC_CHANNEL_COUNT]; ///< Pin configurations of SAADC channels.
    nrf_drv_state_t               state;                         ///< Driver initialization state.
    nrf_saadc_state_t             adc_state;                     ///< State of the SAADC.
#ifdef NRF52_PAN_28
    uint8_t                       scan_pos;                      ///< Current channel scanning position.
#endif
    uint8_t                       active_channels;               ///< Number of enabled SAADC channels.
} nrf_drv_saadc_cb_t;

static nrf_drv_saadc_cb_t m_cb;

#define LOW_LIMIT_TO_FLAG(channel)  ((2*channel+1))
#define HIGH_LIMIT_TO_FLAG(channel) ((2*channel))
#define FLAG_IDX_TO_EVENT(idx) ((nrf_saadc_event_t)((uint32_t)NRF_SAADC_EVENT_CH0_LIMITH+4*idx))
#define LIMIT_EVENT_TO_CHANNEL(event)(uint8_t)(((uint32_t)event-(uint32_t)NRF_SAADC_EVENT_CH0_LIMITH)/8)
#define LIMIT_EVENT_TO_LIMIT_TYPE(event)((((uint32_t)event-(uint32_t)NRF_SAADC_EVENT_CH0_LIMITH) & 4) ? \
                                                          NRF_SAADC_LIMIT_LOW : NRF_SAADC_LIMIT_HIGH)
#define HW_TIMEOUT 10000


void SAADC_IRQHandler(void)
{
    if (nrf_saadc_event_check(NRF_SAADC_EVENT_END))
    {
        nrf_saadc_event_clear(NRF_SAADC_EVENT_END);
#ifdef NRF52_PAN_28
        if (m_cb.active_channels == 1)
        {
#endif
            nrf_drv_saadc_evt_t evt;
            evt.type = NRF_DRV_SAADC_EVT_DONE;
            evt.data.done.p_buffer = (nrf_saadc_value_t *)m_cb.p_buffer;
            evt.data.done.size = nrf_saadc_amount_get();

            if (m_cb.p_secondary_buffer == NULL)
            {
                m_cb.adc_state = NRF_SAADC_STATE_IDLE;
            }
            else
            {
                m_cb.p_buffer = m_cb.p_secondary_buffer;
                m_cb.buffer_size = m_cb.secondary_buffer_size;
                m_cb.p_secondary_buffer = NULL;
                nrf_saadc_task_trigger(NRF_SAADC_TASK_START);
            }

            m_cb.event_handler(&evt);
#ifdef NRF52_PAN_28
        }
        else
        {
            //PAN-28: scan mode is not working correctly, emulated by interrupts
            ++(m_cb.buffer_pos);
            uint16_t buffer_pos = m_cb.buffer_pos;
            if (buffer_pos == m_cb.buffer_size)
            {
                nrf_drv_saadc_evt_t evt;
                evt.type = NRF_DRV_SAADC_EVT_DONE;
                evt.data.done.p_buffer = (nrf_saadc_value_t *)(m_cb.p_buffer);
                evt.data.done.size = m_cb.buffer_size;

                m_cb.adc_state = NRF_SAADC_STATE_IDLE;
                if (m_cb.p_secondary_buffer == NULL)
                {
                    m_cb.adc_state = NRF_SAADC_STATE_IDLE;
                }
                else
                {
                    (void)nrf_drv_saadc_buffer_convert((nrf_saadc_value_t *)m_cb.p_secondary_buffer, (uint16_t)m_cb.secondary_buffer_size);
                }
                m_cb.event_handler(&evt);
            }
            else
            {
                uint8_t current_scan_pos = m_cb.scan_pos;

                nrf_saadc_channel_input_set(current_scan_pos,
                                            NRF_SAADC_INPUT_DISABLED, NRF_SAADC_INPUT_DISABLED);

                nrf_saadc_buffer_init((nrf_saadc_value_t *)(m_cb.p_buffer + m_cb.buffer_pos), 1);
                // Find the next enabled channel.
                for (++m_cb.scan_pos; m_cb.scan_pos < NRF_SAADC_CHANNEL_COUNT; ++m_cb.scan_pos)
                {
                    if (m_cb.psel[m_cb.scan_pos].pselp)
                    {
                        nrf_saadc_channel_input_set(m_cb.scan_pos,
                                    m_cb.psel[m_cb.scan_pos].pselp, m_cb.psel[m_cb.scan_pos].pseln);
                        nrf_saadc_task_trigger(NRF_SAADC_TASK_START);
                        nrf_saadc_task_trigger(NRF_SAADC_TASK_SAMPLE);
                        return;
                    }
                }
                //if scanning is done prepare for next round.
                for (uint8_t i = 0; i < NRF_SAADC_CHANNEL_COUNT; ++i)
                {
                    if (m_cb.psel[i].pselp)
                    {
                        m_cb.scan_pos = i;
                        break;
                    }
                }
                nrf_saadc_channel_input_set(m_cb.scan_pos,
                                    m_cb.psel[m_cb.scan_pos].pselp, m_cb.psel[m_cb.scan_pos].pseln);
                nrf_saadc_task_trigger(NRF_SAADC_TASK_START);
            }
        }
#endif
    }
    if (nrf_saadc_event_check(NRF_SAADC_EVENT_STOPPED))
    {
        nrf_saadc_event_clear(NRF_SAADC_EVENT_STOPPED);
        m_cb.adc_state = NRF_SAADC_STATE_IDLE;
    }
    else
    {
        uint32_t limit_flags = m_cb.limits_enabled_flags;
        uint32_t flag_idx;
        nrf_saadc_event_t event;
        while (limit_flags)
        {
            flag_idx = __CLZ(limit_flags);
            limit_flags &= ~((1UL<<31) >> flag_idx);
            event = FLAG_IDX_TO_EVENT(flag_idx);
            if (nrf_saadc_event_check(event))
            {
                nrf_saadc_event_clear(event);
                nrf_drv_saadc_evt_t evt;
                evt.type = NRF_DRV_SAADC_EVT_LIMIT;
                evt.data.limit.channel = LIMIT_EVENT_TO_CHANNEL(event);
                evt.data.limit.limit_type = LIMIT_EVENT_TO_LIMIT_TYPE(event);
                m_cb.event_handler(&evt);
            }
        }
    }
}


ret_code_t nrf_drv_saadc_init(nrf_drv_saadc_config_t const * p_config,
                              nrf_drv_saadc_event_handler_t event_handler)
{
    if (m_cb.state != NRF_DRV_STATE_UNINITIALIZED)
    {
        return NRF_ERROR_INVALID_STATE;
    }
    if (event_handler == NULL)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    if (p_config == NULL)
    {
        p_config = &m_default_config;
    }

    m_cb.event_handler = event_handler;
    nrf_saadc_resolution_set(p_config->resolution);
    nrf_saadc_oversample_set(p_config->oversample);
    m_cb.state = NRF_DRV_STATE_INITIALIZED;
    m_cb.adc_state = NRF_SAADC_STATE_IDLE;
    m_cb.active_channels = 0;
    m_cb.limits_enabled_flags = 0;
#ifdef NRF52_PAN_28
    m_cb.buffer_pos = 0;
#endif

    nrf_saadc_int_disable(NRF_SAADC_INT_ALL);
    nrf_saadc_event_clear(NRF_SAADC_EVENT_END);
    nrf_drv_common_irq_enable(SAADC_IRQn, p_config->interrupt_priority);
    nrf_saadc_int_enable(NRF_SAADC_INT_END);

    nrf_saadc_enable();

    return NRF_SUCCESS;
}


void nrf_drv_saadc_uninit(void)
{
    ASSERT(m_cb.state != NRF_DRV_STATE_UNINITIALIZED);
    nrf_drv_common_irq_disable(SAADC_IRQn);
    nrf_saadc_task_trigger(NRF_SAADC_TASK_STOP);

    // Wait for ADC being stopped.
    uint32_t timeout = HW_TIMEOUT;
    while (nrf_saadc_event_check(NRF_SAADC_EVENT_STOPPED) == 0 && timeout > 0)
    {
        --timeout;
    }
    ASSERT(timeout > 0);
    
    nrf_saadc_disable();
    nrf_saadc_int_disable(NRF_SAADC_INT_ALL);

    m_cb.adc_state = NRF_SAADC_STATE_IDLE;

    for (uint8_t channel = 0; channel < NRF_SAADC_CHANNEL_COUNT; ++channel)
    {
        if (m_cb.psel[channel].pselp != NRF_SAADC_INPUT_DISABLED)
        {
            (void)nrf_drv_saadc_channel_uninit(channel);
        }
    }

    m_cb.state = NRF_DRV_STATE_UNINITIALIZED;
}


ret_code_t nrf_drv_saadc_channel_init(uint8_t channel,
                                      nrf_saadc_channel_config_t const * const p_config)
{
    ASSERT(m_cb.state != NRF_DRV_STATE_UNINITIALIZED);
    ASSERT(channel < NRF_SAADC_CHANNEL_COUNT);
    //Oversampling can be used only with one channel.
    ASSERT((nrf_saadc_oversample_get()==NRF_SAADC_OVERSAMPLE_DISABLED) || (m_cb.active_channels == 0));
    ASSERT((p_config->pin_p <= NRF_SAADC_INPUT_VDD) && (p_config->pin_p > NRF_SAADC_INPUT_DISABLED));
    ASSERT(p_config->pin_n <= NRF_SAADC_INPUT_VDD);

    // A channel can only be initialized if the driver is in the idle state.
    if (m_cb.adc_state == NRF_SAADC_STATE_BUSY)
    {
        return NRF_ERROR_BUSY;
    }

    if (!m_cb.psel[channel].pselp)
    {
        ++m_cb.active_channels;
    }
    m_cb.psel[channel].pselp = p_config->pin_p;
    m_cb.psel[channel].pseln = p_config->pin_n;
    nrf_saadc_channel_init(channel, p_config);

#ifdef NRF52_PAN_28
    nrf_saadc_channel_input_set(channel, NRF_SAADC_INPUT_DISABLED, NRF_SAADC_INPUT_DISABLED);
#else
    nrf_saadc_channel_input_set(channel, p_config->pin_p, p_config->pin_n);
#endif
    return NRF_SUCCESS;
}


ret_code_t nrf_drv_saadc_channel_uninit(uint8_t channel)
{
    ASSERT(channel <= NRF_SAADC_CHANNEL_COUNT)
    ASSERT(m_cb.state != NRF_DRV_STATE_UNINITIALIZED);

    // A channel can only be uninitialized if the driver is in the idle state.
    if (m_cb.adc_state == NRF_SAADC_STATE_BUSY)
    {
        return NRF_ERROR_BUSY;
    }

    if (m_cb.psel[channel].pselp)
    {
        --m_cb.active_channels;
    }
    m_cb.psel[channel].pselp = NRF_SAADC_INPUT_DISABLED;
    m_cb.psel[channel].pseln = NRF_SAADC_INPUT_DISABLED;
    nrf_saadc_channel_input_set(channel, NRF_SAADC_INPUT_DISABLED, NRF_SAADC_INPUT_DISABLED);
    nrf_drv_saadc_limits_set(channel, NRF_DRV_SAADC_LIMITL_DISABLED, NRF_DRV_SAADC_LIMITH_DISABLED);

    return NRF_SUCCESS;
}

ret_code_t nrf_drv_saadc_sample_convert(uint8_t channel, nrf_saadc_value_t * p_value)
{
    if (m_cb.adc_state != NRF_SAADC_STATE_IDLE)
    {
        return NRF_ERROR_BUSY;
    }
    m_cb.adc_state = NRF_SAADC_STATE_BUSY;
    nrf_saadc_int_disable(NRF_SAADC_INT_END);
    nrf_saadc_buffer_init(p_value, 1);
#ifndef NRF52_PAN_28
    if (m_cb.active_channels > 1)
    {
        for (uint8_t i = 0; i < NRF_SAADC_CHANNEL_COUNT; ++i)
        {
            nrf_saadc_channel_input_set(i, NRF_SAADC_INPUT_DISABLED, NRF_SAADC_INPUT_DISABLED);
        }
    }
#endif
    nrf_saadc_channel_input_set(channel,
                            m_cb.psel[channel].pselp, m_cb.psel[channel].pseln);
    nrf_saadc_task_trigger(NRF_SAADC_TASK_START);
    nrf_saadc_task_trigger(NRF_SAADC_TASK_SAMPLE);

    uint32_t timeout = HW_TIMEOUT;
    while (0 == nrf_saadc_event_check(NRF_SAADC_EVENT_END) && timeout > 0)
    {
        timeout--;
    }
    nrf_saadc_event_clear(NRF_SAADC_EVENT_END);

#ifdef NRF52_PAN_28
    nrf_saadc_channel_input_set(channel, NRF_SAADC_INPUT_DISABLED, NRF_SAADC_INPUT_DISABLED);
#else
    if (m_cb.active_channels > 1)
    {
        for (uint8_t i = 0; i < NRF_SAADC_CHANNEL_COUNT; ++i)
        {
            nrf_saadc_channel_input_set(i, m_cb.psel[i].pselp, m_cb.psel[i].pseln);
        }
    }
#endif

    nrf_saadc_int_enable(NRF_SAADC_INT_END);
    m_cb.adc_state = NRF_SAADC_STATE_IDLE;

    return NRF_SUCCESS;
}

ret_code_t nrf_drv_saadc_buffer_convert(nrf_saadc_value_t * p_buffer, uint16_t size)
{
    ASSERT(m_cb.state != NRF_DRV_STATE_UNINITIALIZED);

    nrf_saadc_int_disable(NRF_SAADC_INT_END);
    if (m_cb.adc_state == NRF_SAADC_STATE_BUSY)
    {
        if ( m_cb.p_secondary_buffer)
        {
            nrf_saadc_int_enable(NRF_SAADC_INT_END);
            return NRF_ERROR_BUSY;
        }
        else
        {
            m_cb.p_secondary_buffer = p_buffer;
            m_cb.secondary_buffer_size = size;
#ifdef NRF52_PAN_28
            if (m_cb.active_channels == 1)
#endif
            {
                while (nrf_saadc_event_check(NRF_SAADC_EVENT_STARTED) == 0);
                nrf_saadc_event_clear(NRF_SAADC_EVENT_STARTED);
                nrf_saadc_buffer_init(p_buffer, size);
            }
            nrf_saadc_int_enable(NRF_SAADC_INT_END);
            return NRF_SUCCESS;
        }
    }
    nrf_saadc_int_enable(NRF_SAADC_INT_END);
    m_cb.adc_state = NRF_SAADC_STATE_BUSY;

#ifdef NRF52_PAN_28
    m_cb.scan_pos = NRF_SAADC_CHANNEL_COUNT;
    for (uint8_t i = 0; i < NRF_SAADC_CHANNEL_COUNT; ++i)
    {
        if (m_cb.psel[i].pselp)
        {
            m_cb.scan_pos = i;
            break;
        }
    }

    // Find the first enabled channel.
    if (m_cb.scan_pos >= NRF_SAADC_CHANNEL_COUNT)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    m_cb.buffer_pos = 0;
#endif
    m_cb.p_buffer = p_buffer;
    m_cb.buffer_size = size;
    m_cb.p_secondary_buffer = NULL;

#ifdef NRF52_PAN_28
    nrf_saadc_channel_input_set(m_cb.scan_pos,
                                m_cb.psel[m_cb.scan_pos].pselp, m_cb.psel[m_cb.scan_pos].pseln);

    if (m_cb.active_channels == 1)
    {
        nrf_saadc_buffer_init(p_buffer, size);
    }
    else
    {
        nrf_saadc_buffer_init(p_buffer, 1);
    }
#else
    nrf_saadc_buffer_init(p_buffer, size);
#endif

    nrf_saadc_event_clear(NRF_SAADC_EVENT_STARTED);
    nrf_saadc_task_trigger(NRF_SAADC_TASK_START);

    return NRF_SUCCESS;
}

ret_code_t nrf_drv_saadc_sample()
{
    ASSERT(m_cb.state != NRF_DRV_STATE_UNINITIALIZED);

    ret_code_t err_code = NRF_SUCCESS;
    if (m_cb.adc_state == NRF_SAADC_STATE_IDLE)
    {
        err_code = NRF_ERROR_BUSY;
    }
    else
    {
        nrf_saadc_task_trigger(NRF_SAADC_TASK_SAMPLE);
    }
    
    return err_code;
}


bool nrf_drv_saadc_is_busy(void)
{
    return (m_cb.adc_state == NRF_SAADC_STATE_BUSY);
}

void nrf_drv_saadc_abort(void)
{
    if (nrf_drv_saadc_is_busy())
    {
        nrf_saadc_event_clear(NRF_SAADC_EVENT_STOPPED);
        nrf_saadc_task_trigger(NRF_SAADC_TASK_STOP);
        
        // Wait for ADC being stopped.
        uint32_t timeout = HW_TIMEOUT;
        while ((m_cb.adc_state != NRF_SAADC_STATE_IDLE) && (timeout > 0))
        {
            --timeout;
        }
        ASSERT(timeout > 0);
        
        m_cb.p_buffer = 0;
        m_cb.p_secondary_buffer = 0;
    }
}

void nrf_drv_saadc_limits_set(uint8_t channel, int16_t limit_low, int16_t limit_high)
{
    ASSERT(m_cb.state != NRF_DRV_STATE_UNINITIALIZED);
    ASSERT(m_cb.event_handler); // only non blocking mode supported
    ASSERT(limit_low>=NRF_DRV_SAADC_LIMITL_DISABLED);
    ASSERT(limit_high<=NRF_DRV_SAADC_LIMITH_DISABLED);
    ASSERT(limit_low<limit_high);
    nrf_saadc_channel_limits_set(channel, limit_low, limit_high);

    uint32_t int_mask = nrf_saadc_limit_int_get(channel, NRF_SAADC_LIMIT_LOW);
    if (limit_low == NRF_DRV_SAADC_LIMITL_DISABLED)
    {
        m_cb.limits_enabled_flags &= ~(0x80000000 >> LOW_LIMIT_TO_FLAG(channel));
        nrf_saadc_int_disable(int_mask);
    }
    else
    {
        m_cb.limits_enabled_flags |= (0x80000000 >> LOW_LIMIT_TO_FLAG(channel));
        nrf_saadc_int_enable(int_mask);
    }

    int_mask = nrf_saadc_limit_int_get(channel, NRF_SAADC_LIMIT_HIGH);
    if (limit_high == NRF_DRV_SAADC_LIMITH_DISABLED)
    {
        m_cb.limits_enabled_flags &= ~(0x80000000 >> HIGH_LIMIT_TO_FLAG(channel));
        nrf_saadc_int_disable(int_mask);
    }
    else
    {
        m_cb.limits_enabled_flags |= (0x80000000 >> HIGH_LIMIT_TO_FLAG(channel));
        nrf_saadc_int_enable(int_mask);
    }
}
