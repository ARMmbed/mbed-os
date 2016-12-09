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

#include <string.h>
#include "nrf_drv_pwm.h"
#include "nrf_drv_common.h"
#include "nrf_gpio.h"
#include "app_util_platform.h"

#if (PWM_COUNT == 0)
    #error "No PWM instances enabled in the driver configuration file."
#endif


// Control block - driver instance local data.
typedef struct
{
    nrf_drv_pwm_handler_t    handler;
    nrf_drv_state_t volatile state;
} pwm_control_block_t;
static pwm_control_block_t m_cb[PWM_COUNT];

static nrf_drv_pwm_config_t const m_default_config[PWM_COUNT] = {
#if PWM0_ENABLED
    NRF_DRV_PWM_DEFAULT_CONFIG(0),
#endif
#if PWM1_ENABLED
    NRF_DRV_PWM_DEFAULT_CONFIG(1),
#endif
#if PWM2_ENABLED
    NRF_DRV_PWM_DEFAULT_CONFIG(2),
#endif
};


static void configure_pins(nrf_drv_pwm_t const * const p_instance,
                           nrf_drv_pwm_config_t const * p_config)
{
    uint32_t out_pins[NRF_PWM_CHANNEL_COUNT];
    uint8_t i;

    for (i = 0; i < NRF_PWM_CHANNEL_COUNT; ++i)
    {
        uint8_t output_pin = p_config->output_pins[i];
        if (output_pin != NRF_DRV_PWM_PIN_NOT_USED)
        {
            bool inverted = output_pin &  NRF_DRV_PWM_PIN_INVERTED;
            out_pins[i]   = output_pin & ~NRF_DRV_PWM_PIN_INVERTED;

            if (inverted)
            {
                nrf_gpio_pin_set(out_pins[i]);
            }
            else
            {
                nrf_gpio_pin_clear(out_pins[i]);
            }

            nrf_gpio_cfg_output(out_pins[i]);
        }
        else
        {
            out_pins[i] = NRF_PWM_PIN_NOT_CONNECTED;
        }
    }

    nrf_pwm_pins_set(p_instance->p_registers, out_pins);
}


ret_code_t nrf_drv_pwm_init(nrf_drv_pwm_t const * const p_instance,
                            nrf_drv_pwm_config_t const * p_config,
                            nrf_drv_pwm_handler_t        handler)
{
    pwm_control_block_t * p_cb  = &m_cb[p_instance->drv_inst_idx];

    if (p_cb->state != NRF_DRV_STATE_UNINITIALIZED)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (p_config == NULL)
    {
        p_config = &m_default_config[p_instance->drv_inst_idx];
    }

    p_cb->handler = handler;

    configure_pins(p_instance, p_config);

    nrf_pwm_enable(p_instance->p_registers);
    nrf_pwm_configure(p_instance->p_registers,
        p_config->base_clock, p_config->count_mode, p_config->top_value);
    nrf_pwm_decoder_set(p_instance->p_registers,
        p_config->load_mode, p_config->step_mode);

    nrf_pwm_shorts_set(p_instance->p_registers, 0);
    nrf_pwm_int_set(p_instance->p_registers, 0);
    nrf_pwm_event_clear(p_instance->p_registers, NRF_PWM_EVENT_LOOPSDONE);
    nrf_pwm_event_clear(p_instance->p_registers, NRF_PWM_EVENT_SEQEND0);
    nrf_pwm_event_clear(p_instance->p_registers, NRF_PWM_EVENT_SEQEND1);
    nrf_pwm_event_clear(p_instance->p_registers, NRF_PWM_EVENT_STOPPED);

    if (p_cb->handler)
    {
        nrf_drv_common_irq_enable(nrf_drv_get_IRQn(p_instance->p_registers),
            p_config->irq_priority);
    }

    p_cb->state = NRF_DRV_STATE_INITIALIZED;

    return NRF_SUCCESS;
}


void nrf_drv_pwm_uninit(nrf_drv_pwm_t const * const p_instance)
{
    pwm_control_block_t * p_cb  = &m_cb[p_instance->drv_inst_idx];
    ASSERT(p_cb->state != NRF_DRV_STATE_UNINITIALIZED);

    nrf_drv_common_irq_disable(nrf_drv_get_IRQn(p_instance->p_registers));

    nrf_pwm_disable(p_instance->p_registers);

    p_cb->state = NRF_DRV_STATE_UNINITIALIZED;
}


static void start_playback(nrf_drv_pwm_t const * const p_instance,
                           pwm_control_block_t * p_cb,
                           uint8_t               flags,
                           nrf_pwm_task_t        starting_task)
{
    p_cb->state = NRF_DRV_STATE_POWERED_ON;

    if (p_cb->handler)
    {
        // The notification about finished playback is by default enabled, but
        // this can be suppressed. The notification that the peripheral has been
        // stopped is always enable.
        uint32_t int_mask = NRF_PWM_INT_LOOPSDONE_MASK |
                            NRF_PWM_INT_STOPPED_MASK;

        if (flags & NRF_DRV_PWM_FLAG_SIGNAL_END_SEQ0)
        {
            int_mask |= NRF_PWM_INT_SEQEND0_MASK;
        }
        if (flags & NRF_DRV_PWM_FLAG_SIGNAL_END_SEQ1)
        {
            int_mask |= NRF_PWM_INT_SEQEND1_MASK;
        }
        if (flags & NRF_DRV_PWM_FLAG_NO_EVT_FINISHED)
        {
            int_mask &= ~NRF_PWM_INT_LOOPSDONE_MASK;
        }

        nrf_pwm_int_set(p_instance->p_registers, int_mask);
    }

    nrf_pwm_event_clear(p_instance->p_registers, NRF_PWM_EVENT_STOPPED);

    nrf_pwm_task_trigger(p_instance->p_registers, starting_task);
}


void nrf_drv_pwm_simple_playback(nrf_drv_pwm_t const * const p_instance,
                                 nrf_pwm_sequence_t const * p_sequence,
                                 uint16_t                   playback_count,
                                 uint32_t                   flags)
{
    pwm_control_block_t * p_cb  = &m_cb[p_instance->drv_inst_idx];
    ASSERT(p_cb->state != NRF_DRV_STATE_UNINITIALIZED);
    ASSERT(playback_count > 0);
    ASSERT(nrf_drv_is_in_RAM(p_sequence->values.p_raw));

    // To take advantage of the looping mechanism, we need to use both sequences
    // (single sequence can be played back only once).
    nrf_pwm_sequence_set(p_instance->p_registers, 0, p_sequence);
    nrf_pwm_sequence_set(p_instance->p_registers, 1, p_sequence);
    bool odd = (playback_count & 1);
    nrf_pwm_loop_set(p_instance->p_registers, playback_count/2 + (odd ? 1 : 0));

    uint32_t shorts_mask;
    if (flags & NRF_DRV_PWM_FLAG_STOP)
    {
        shorts_mask = NRF_PWM_SHORT_LOOPSDONE_STOP_MASK;
    }
    else if (flags & NRF_DRV_PWM_FLAG_LOOP)
    {
        shorts_mask = odd ? NRF_PWM_SHORT_LOOPSDONE_SEQSTART1_MASK
                          : NRF_PWM_SHORT_LOOPSDONE_SEQSTART0_MASK;
    }
    else
    {
        shorts_mask = 0;
    }
    nrf_pwm_shorts_set(p_instance->p_registers, shorts_mask);

    start_playback(p_instance, p_cb, flags, odd ? NRF_PWM_TASK_SEQSTART1
                                                : NRF_PWM_TASK_SEQSTART0);
}


void nrf_drv_pwm_complex_playback(nrf_drv_pwm_t const * const p_instance,
                                  nrf_pwm_sequence_t const * p_sequence_0,
                                  nrf_pwm_sequence_t const * p_sequence_1,
                                  uint16_t                   playback_count,
                                  uint32_t                   flags)
{
    pwm_control_block_t * p_cb  = &m_cb[p_instance->drv_inst_idx];
    ASSERT(p_cb->state != NRF_DRV_STATE_UNINITIALIZED);
    ASSERT(playback_count > 0);
    ASSERT(nrf_drv_is_in_RAM(p_sequence_0->values.p_raw));
    ASSERT(nrf_drv_is_in_RAM(p_sequence_1->values.p_raw));

    nrf_pwm_sequence_set(p_instance->p_registers, 0, p_sequence_0);
    nrf_pwm_sequence_set(p_instance->p_registers, 1, p_sequence_1);
    nrf_pwm_loop_set(p_instance->p_registers, playback_count);

    uint32_t shorts_mask;
    if (flags & NRF_DRV_PWM_FLAG_STOP)
    {
        shorts_mask = NRF_PWM_SHORT_LOOPSDONE_STOP_MASK;
    }
    else if (flags & NRF_DRV_PWM_FLAG_LOOP)
    {
        shorts_mask = NRF_PWM_SHORT_LOOPSDONE_SEQSTART0_MASK;
    }
    else
    {
        shorts_mask = 0;
    }
    nrf_pwm_shorts_set(p_instance->p_registers, shorts_mask);

    start_playback(p_instance, p_cb, flags, NRF_PWM_TASK_SEQSTART0);
}


bool nrf_drv_pwm_stop(nrf_drv_pwm_t const * const p_instance,
                      bool wait_until_stopped)
{
    ASSERT(m_cb[p_instance->drv_inst_idx].state != NRF_DRV_STATE_UNINITIALIZED);

    if (nrf_drv_pwm_is_stopped(p_instance))
    {
        return true;
    }

    nrf_pwm_task_trigger(p_instance->p_registers, NRF_PWM_TASK_STOP);

    do {
        if (nrf_drv_pwm_is_stopped(p_instance))
        {
            return true;
        }
    } while (wait_until_stopped);

    return false;
}


bool nrf_drv_pwm_is_stopped(nrf_drv_pwm_t const * const p_instance)
{
    pwm_control_block_t * p_cb  = &m_cb[p_instance->drv_inst_idx];
    ASSERT(p_cb->state != NRF_DRV_STATE_UNINITIALIZED);

    // If the event handler is used (interrupts are enabled), the state will
    // be changed in interrupt handler when the STOPPED event occurs.
    if (p_cb->state != NRF_DRV_STATE_POWERED_ON)
    {
        return true;
    }
    // If interrupts are disabled, we must check the STOPPED event here.
    if (nrf_pwm_event_check(p_instance->p_registers, NRF_PWM_EVENT_STOPPED))
    {
        p_cb->state = NRF_DRV_STATE_INITIALIZED;
        return true;
    }

    return false;
}


static void irq_handler(NRF_PWM_Type * p_pwm, pwm_control_block_t * p_cb)
{
    ASSERT(p_cb->handler);

    // The SEQEND0 and SEQEND1 events are only handled when the user asked for
    // it (by setting proper flags when starting the playback).
    if (nrf_pwm_int_enable_check(p_pwm, NRF_PWM_INT_SEQEND0_MASK) &&
        nrf_pwm_event_check(p_pwm, NRF_PWM_EVENT_SEQEND0))
    {
        nrf_pwm_event_clear(p_pwm, NRF_PWM_EVENT_SEQEND0);
        p_cb->handler(NRF_DRV_PWM_EVT_END_SEQ0);
    }
    if (nrf_pwm_int_enable_check(p_pwm, NRF_PWM_INT_SEQEND1_MASK) &&
        nrf_pwm_event_check(p_pwm, NRF_PWM_EVENT_SEQEND1))
    {
        nrf_pwm_event_clear(p_pwm, NRF_PWM_EVENT_SEQEND1);
        p_cb->handler(NRF_DRV_PWM_EVT_END_SEQ1);
    }

    // The LOOPSDONE event is handled by default, but this can be disabled.
    if (nrf_pwm_int_enable_check(p_pwm, NRF_PWM_INT_LOOPSDONE_MASK) &&
        nrf_pwm_event_check(p_pwm, NRF_PWM_EVENT_LOOPSDONE))
    {
        nrf_pwm_event_clear(p_pwm, NRF_PWM_EVENT_LOOPSDONE);
        p_cb->handler(NRF_DRV_PWM_EVT_FINISHED);
    }

    if (nrf_pwm_event_check(p_pwm, NRF_PWM_EVENT_STOPPED))
    {
        nrf_pwm_event_clear(p_pwm, NRF_PWM_EVENT_STOPPED);

        p_cb->state = NRF_DRV_STATE_INITIALIZED;

        p_cb->handler(NRF_DRV_PWM_EVT_STOPPED);
    }
}


#if PWM0_ENABLED
void PWM0_IRQHandler(void)
{
    irq_handler(NRF_PWM0, &m_cb[PWM0_INSTANCE_INDEX]);
}
#endif

#if PWM1_ENABLED
void PWM1_IRQHandler(void)
{
    irq_handler(NRF_PWM1, &m_cb[PWM1_INSTANCE_INDEX]);
}
#endif

#if PWM2_ENABLED
void PWM2_IRQHandler(void)
{
    irq_handler(NRF_PWM2, &m_cb[PWM2_INSTANCE_INDEX]);
}
#endif
