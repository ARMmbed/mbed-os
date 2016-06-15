/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
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

#include "nrf_drv_rtc.h"
#include "nrf_rtc.h"
#include "nrf_assert.h"
#include "app_util_platform.h"

/**@brief RTC driver instance control block structure. */
typedef struct
{
    nrf_drv_state_t state;        /**< Instance state. */
    bool            reliable;     /**< Reliable mode flag. */
    uint8_t         tick_latency; /**< Maximum length of interrupt handler in ticks (max 7.7 ms). */
} nrf_drv_rtc_cb_t;

// User callbacks local storage.
static nrf_drv_rtc_handler_t m_handlers[RTC_COUNT];
static nrf_drv_rtc_cb_t      m_cb[RTC_COUNT];

static const nrf_drv_rtc_config_t m_default_config[] = {
#if RTC0_ENABLED
    NRF_DRV_RTC_DEFAULT_CONFIG(0),
#endif
#if RTC1_ENABLED
    NRF_DRV_RTC_DEFAULT_CONFIG(1),
#endif
#if RTC2_ENABLED
    NRF_DRV_RTC_DEFAULT_CONFIG(2)
#endif
};

ret_code_t nrf_drv_rtc_init(nrf_drv_rtc_t const * const p_instance,
                            nrf_drv_rtc_config_t const * p_config,
                            nrf_drv_rtc_handler_t handler)
{
    if (handler)
    {
        m_handlers[p_instance->instance_id] = handler;
    }
    else
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    if (p_config == NULL)
    {
        p_config = &m_default_config[p_instance->instance_id];
    }

    if (m_cb[p_instance->instance_id].state != NRF_DRV_STATE_UNINITIALIZED)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    nrf_drv_common_irq_enable(p_instance->irq, p_config->interrupt_priority);
    nrf_rtc_prescaler_set(p_instance->p_reg, p_config->prescaler);
    m_cb[p_instance->instance_id].reliable     = p_config->reliable;
    m_cb[p_instance->instance_id].tick_latency = p_config->tick_latency;
    m_cb[p_instance->instance_id].state        = NRF_DRV_STATE_INITIALIZED;

    return NRF_SUCCESS;
}

void nrf_drv_rtc_uninit(nrf_drv_rtc_t const * const p_instance)
{
    uint32_t mask = NRF_RTC_INT_TICK_MASK     |
                    NRF_RTC_INT_OVERFLOW_MASK |
                    NRF_RTC_INT_COMPARE0_MASK |
                    NRF_RTC_INT_COMPARE1_MASK |
                    NRF_RTC_INT_COMPARE2_MASK |
                    NRF_RTC_INT_COMPARE3_MASK;
    ASSERT(m_cb[p_instance->instance_id].state != NRF_DRV_STATE_UNINITIALIZED);

    nrf_drv_common_irq_disable(p_instance->irq);

    nrf_rtc_task_trigger(p_instance->p_reg, NRF_RTC_TASK_STOP);
    nrf_rtc_event_disable(p_instance->p_reg, mask);
    nrf_rtc_int_disable(p_instance->p_reg, mask);

    m_cb[p_instance->instance_id].state = NRF_DRV_STATE_UNINITIALIZED;
}

void nrf_drv_rtc_enable(nrf_drv_rtc_t const * const p_instance)
{
    ASSERT(m_cb[p_instance->instance_id].state == NRF_DRV_STATE_INITIALIZED);

    nrf_rtc_task_trigger(p_instance->p_reg, NRF_RTC_TASK_START);
    m_cb[p_instance->instance_id].state = NRF_DRV_STATE_POWERED_ON;
}

void nrf_drv_rtc_disable(nrf_drv_rtc_t const * const p_instance)
{
    ASSERT(m_cb[p_instance->instance_id].state == NRF_DRV_STATE_POWERED_ON);

    nrf_rtc_task_trigger(p_instance->p_reg, NRF_RTC_TASK_STOP);
    m_cb[p_instance->instance_id].state = NRF_DRV_STATE_INITIALIZED;
}

ret_code_t nrf_drv_rtc_cc_disable(nrf_drv_rtc_t const * const p_instance, uint32_t channel)
{
    ASSERT(m_cb[p_instance->instance_id].state != NRF_DRV_STATE_UNINITIALIZED);
    ASSERT(channel<p_instance->cc_channel_count);

    uint32_t int_mask = RTC_CHANNEL_INT_MASK(channel);
    nrf_rtc_event_t event    = RTC_CHANNEL_EVENT_ADDR(channel);

    nrf_rtc_event_disable(p_instance->p_reg,int_mask);
    if (nrf_rtc_int_is_enabled(p_instance->p_reg,int_mask))
    {
        nrf_rtc_int_disable(p_instance->p_reg,int_mask);
        if (nrf_rtc_event_pending(p_instance->p_reg,event))
        {
            nrf_rtc_event_clear(p_instance->p_reg,event);
            return NRF_ERROR_TIMEOUT;
        }
    }
    return NRF_SUCCESS;
}

ret_code_t nrf_drv_rtc_cc_set(nrf_drv_rtc_t const * const p_instance,
                              uint32_t channel,
                              uint32_t val,
                              bool enable_irq)
{
    ASSERT(m_cb[p_instance->instance_id].state != NRF_DRV_STATE_UNINITIALIZED);
    ASSERT(channel<p_instance->cc_channel_count);

    uint32_t int_mask = RTC_CHANNEL_INT_MASK(channel);
    nrf_rtc_event_t event    = RTC_CHANNEL_EVENT_ADDR(channel);

    nrf_rtc_event_disable(p_instance->p_reg, int_mask);
    nrf_rtc_int_disable(p_instance->p_reg, int_mask);

    val = RTC_WRAP(val);
    if (m_cb[p_instance->instance_id].reliable)
    {
        nrf_rtc_cc_set(p_instance->p_reg,channel,val);
        uint32_t cnt = nrf_rtc_counter_get(p_instance->p_reg);
        int32_t diff = cnt - val;
        if (cnt < val)
        {
            diff += RTC_COUNTER_COUNTER_Msk;
        }
        if (diff < m_cb[p_instance->instance_id].tick_latency)
        {
            return NRF_ERROR_TIMEOUT;
        }
    }
    else
    {
        nrf_rtc_cc_set(p_instance->p_reg,channel,val);
    }

    if (enable_irq)
    {
        nrf_rtc_event_clear(p_instance->p_reg,event);
        nrf_rtc_int_enable(p_instance->p_reg, int_mask);
    }
    nrf_rtc_event_enable(p_instance->p_reg,int_mask);

    return NRF_SUCCESS;
}

void nrf_drv_rtc_tick_enable(nrf_drv_rtc_t const * const p_instance, bool enable_irq)
{
    nrf_rtc_event_t event = NRF_RTC_EVENT_TICK;
    uint32_t mask = NRF_RTC_INT_TICK_MASK;

    nrf_rtc_event_clear(p_instance->p_reg, event);
    nrf_rtc_event_enable(p_instance->p_reg, mask);
    if (enable_irq)
    {
        nrf_rtc_int_enable(p_instance->p_reg, mask);
    }
}

void nrf_drv_rtc_tick_disable(nrf_drv_rtc_t const * const p_instance)
{
    uint32_t mask = NRF_RTC_INT_TICK_MASK;

    nrf_rtc_event_disable(p_instance->p_reg, mask);
    nrf_rtc_int_disable(p_instance->p_reg, mask);
}

void nrf_drv_rtc_overflow_enable(nrf_drv_rtc_t const * const p_instance, bool enable_irq)
{
    nrf_rtc_event_t event = NRF_RTC_EVENT_OVERFLOW;
    uint32_t mask = NRF_RTC_INT_OVERFLOW_MASK;

    nrf_rtc_event_clear(p_instance->p_reg, event);
    nrf_rtc_event_enable(p_instance->p_reg, mask);
    if (enable_irq)
    {
        nrf_rtc_int_enable(p_instance->p_reg, mask);
    }
}
void nrf_drv_rtc_overflow_disable(nrf_drv_rtc_t const * const p_instance)
{
    uint32_t mask = NRF_RTC_INT_OVERFLOW_MASK;
    nrf_rtc_event_disable(p_instance->p_reg, mask);
    nrf_rtc_int_disable(p_instance->p_reg, mask);
}

uint32_t nrf_drv_rtc_max_ticks_get(nrf_drv_rtc_t const * const p_instance)
{
    ASSERT(m_cb[p_instance->instance_id].reliable);
    uint32_t ticks;
    if (m_cb[p_instance->instance_id].reliable)
    {
        ticks = RTC_COUNTER_COUNTER_Msk - m_cb[p_instance->instance_id].tick_latency;
    }
    else
    {
        ticks = RTC_COUNTER_COUNTER_Msk;
    }
    return ticks;
}

/**@brief Generic function for handling RTC interrupt
 *
 * @param[in]  p_reg         Pointer to instance register structure.
 * @param[in]  instance_id   Index of instance.
 */
__STATIC_INLINE void nrf_drv_rtc_int_handler(NRF_RTC_Type * p_reg,
                                             uint32_t instance_id,
                                             uint32_t channel_count)
{
    uint32_t i;
    uint32_t int_mask = (uint32_t)NRF_RTC_INT_COMPARE0_MASK;
    nrf_rtc_event_t event = NRF_RTC_EVENT_COMPARE_0;

    for (i = 0; i < channel_count; i++)
    {
        if (nrf_rtc_int_is_enabled(p_reg,int_mask) && nrf_rtc_event_pending(p_reg,event))
        {
            nrf_rtc_event_disable(p_reg,int_mask);
            nrf_rtc_int_disable(p_reg,int_mask);
            nrf_rtc_event_clear(p_reg,event);
            m_handlers[instance_id]((nrf_drv_rtc_int_type_t)i);
        }
        int_mask <<= 1;
        event    = (nrf_rtc_event_t)((uint32_t)event + sizeof(uint32_t));
    }
    event = NRF_RTC_EVENT_TICK;
    if (nrf_rtc_int_is_enabled(p_reg,NRF_RTC_INT_TICK_MASK) &&
        nrf_rtc_event_pending(p_reg, event))
    {
        nrf_rtc_event_clear(p_reg, event);
        m_handlers[instance_id](NRF_DRV_RTC_INT_TICK);
    }

    event = NRF_RTC_EVENT_OVERFLOW;
    if (nrf_rtc_int_is_enabled(p_reg,NRF_RTC_INT_OVERFLOW_MASK) &&
        nrf_rtc_event_pending(p_reg, event))
    {
        nrf_rtc_event_clear(p_reg,event);
        m_handlers[instance_id](NRF_DRV_RTC_INT_OVERFLOW);
    }
}

#if RTC0_ENABLED
void RTC0_IRQHandler(void)
{
    nrf_drv_rtc_int_handler(NRF_RTC0,RTC0_INSTANCE_INDEX, NRF_RTC_CC_CHANNEL_COUNT(0));
}
#endif

#if RTC1_ENABLED
void RTC1_IRQHandler(void)
{
    nrf_drv_rtc_int_handler(NRF_RTC1,RTC1_INSTANCE_INDEX, NRF_RTC_CC_CHANNEL_COUNT(1));
}
#endif

#if RTC2_ENABLED
void RTC2_IRQHandler(void)
{
    nrf_drv_rtc_int_handler(NRF_RTC2,RTC2_INSTANCE_INDEX, NRF_RTC_CC_CHANNEL_COUNT(2));
}
#endif
