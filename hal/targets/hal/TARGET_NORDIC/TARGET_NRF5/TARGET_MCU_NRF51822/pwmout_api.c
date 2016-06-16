/* mbed Microcontroller Library
 * Copyright (c) 2013 Nordic Semiconductor
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
#include "mbed_error.h"
#include "pwmout_api.h"
#include "cmsis.h"
#include "pinmap.h"

#if DEVICE_PWMOUT

#include "nrf.h"
#include "nrf_drv_timer.h"
#include "app_pwm.h"

#define PWM_INSTANCE_COUNT          2
#define PWM_CHANNELS_PER_INSTANCE   2
#define PWM_DEFAULT_PERIOD_US       20000
#define PWM_PERIOD_MIN              3

typedef struct
{
    const app_pwm_t * const instance;
    NRF_TIMER_Type * const  timer_reg;
    uint8_t                 channels_allocated;
    uint32_t                pins[PWM_CHANNELS_PER_INSTANCE];
    uint16_t                period_us;
    uint16_t                duty_ticks[PWM_CHANNELS_PER_INSTANCE];
} pwm_t;


APP_PWM_INSTANCE(m_pwm_instance_0, 1);   //PWM0: Timer 1
APP_PWM_INSTANCE(m_pwm_instance_1, 2);   //PWM1: Timer 2


static pwm_t m_pwm[] = {
                    {.instance = &m_pwm_instance_0, .timer_reg = NRF_TIMER1, .channels_allocated = 0},
                    {.instance = &m_pwm_instance_1, .timer_reg = NRF_TIMER2, .channels_allocated = 0}
                };

static inline void pwm_ticks_set(pwm_t* pwm, uint8_t channel, uint16_t ticks)
{
    pwm->duty_ticks[channel] = ticks;
    while (app_pwm_channel_duty_ticks_set(pwm->instance, channel, ticks) != NRF_SUCCESS);
}

static void pwm_reinit(pwm_t * pwm)
{
    app_pwm_uninit(pwm->instance);
    app_pwm_config_t pwm_cfg = APP_PWM_DEFAULT_CONFIG_2CH(pwm->period_us,
                                                          pwm->pins[0],
                                                          pwm->pins[1]);
    app_pwm_init(pwm->instance, &pwm_cfg, NULL);
    app_pwm_enable(pwm->instance);
    
    for (uint8_t channel = 0; channel < PWM_CHANNELS_PER_INSTANCE; ++channel) {
        if ((pwm->channels_allocated & (1 << channel)) && (pwm->pins[channel] != APP_PWM_NOPIN)) {
            app_pwm_channel_duty_ticks_set(pwm->instance, channel, pwm->duty_ticks[channel]);
        }
    }
}

void pwmout_init(pwmout_t *obj, PinName pin)
{
    if (pin == NC) {
        error("PwmOut init failed. Invalid pin name.");
    }
    
    // Check if pin is already initialized and find the next free channel.
    uint8_t free_instance = 0xFF;
    uint8_t free_channel  = 0xFF;

    for (uint8_t inst = 0; inst < PWM_INSTANCE_COUNT; ++inst) {
        if (m_pwm[inst].channels_allocated) {
            for (uint8_t channel = 0; channel < PWM_CHANNELS_PER_INSTANCE; ++channel) {
                if (m_pwm[inst].channels_allocated & (1 << channel)) {
                    if (m_pwm[inst].pins[channel] == (uint32_t)pin) {
                        error("PwmOut init failed. Pin is already in use.");
                        return;
                    }
                }
                else {
                    if (free_instance == 0xFF) {
                        free_instance = inst;
                        free_channel  = channel;
                    }
                }
            }
        }
        else {
            if (free_instance == 0xFF) {
                free_instance = inst;
                free_channel  = 0;
            }
        }
    }

    if (free_instance == 0xFF)
    {
        error("PwmOut init failed. All available PWM channels are in use.");
        return;
    }

    // Init / reinit PWM instance.
    m_pwm[free_instance].pins[free_channel] = (uint32_t) pin;
    m_pwm[free_instance].duty_ticks[free_channel] = 0;
    if (!m_pwm[free_instance].channels_allocated) {
        m_pwm[free_instance].period_us = PWM_DEFAULT_PERIOD_US;
        for (uint8_t channel = 1; channel < PWM_CHANNELS_PER_INSTANCE; ++channel) {
            m_pwm[free_instance].pins[channel] = APP_PWM_NOPIN;
            m_pwm[free_instance].duty_ticks[channel] = 0;
        }
        app_pwm_config_t pwm_cfg = APP_PWM_DEFAULT_CONFIG_1CH(PWM_DEFAULT_PERIOD_US, pin);
        app_pwm_init(m_pwm[free_instance].instance, &pwm_cfg, NULL);
        app_pwm_enable(m_pwm[free_instance].instance);
    }
    else {
        pwm_reinit(&m_pwm[free_instance]);
    }
    m_pwm[free_instance].channels_allocated |= (1 << free_channel);

    obj->pin         = pin;
    obj->pwm_struct  = (void *) &m_pwm[free_instance];
    obj->pwm_channel = free_channel;
}

void pwmout_free(pwmout_t *obj)
{
    MBED_ASSERT(obj->pwm_name != (PWMName)NC);
    MBED_ASSERT(obj->pwm_channel < PWM_CHANNELS_PER_INSTANCE);

    pwm_t * pwm = (pwm_t *) obj->pwm_struct;
    pwm->channels_allocated &= ~(1 << obj->pwm_channel);
    pwm->pins[obj->pwm_channel] = APP_PWM_NOPIN;
    pwm->duty_ticks[obj->pwm_channel] = 0;

    app_pwm_uninit(pwm->instance);
    if (pwm->channels_allocated) {
        pwm_reinit(pwm);
    }

    obj->pwm_struct = NULL;    
}

void pwmout_write(pwmout_t *obj, float value)
{
    pwm_t * pwm = (pwm_t *) obj->pwm_struct;
    if (value > 1.0f) {
        value = 1.0f;
    }
    uint16_t ticks = (uint16_t)((float)app_pwm_cycle_ticks_get(pwm->instance) * value);
}

float pwmout_read(pwmout_t *obj)
{
    pwm_t * pwm = (pwm_t *) obj->pwm_struct;
    MBED_ASSERT(pwm != NULL);
    return ((float)pwm->duty_ticks[obj->pwm_channel] / (float)app_pwm_cycle_ticks_get(pwm->instance));
}

void pwmout_period(pwmout_t *obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t *obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

void pwmout_period_us(pwmout_t *obj, int us)
{
    pwm_t * pwm = (pwm_t *) obj->pwm_struct;
    MBED_ASSERT(pwm != NULL);
    if (us < PWM_PERIOD_MIN) {
        us = PWM_PERIOD_MIN;
    }
    pwm->period_us = (uint32_t)us;
    pwm_reinit(pwm);
}

void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    pwm_t * pwm = (pwm_t *) obj->pwm_struct;
    MBED_ASSERT(pwm);
    
    uint16_t ticks = nrf_timer_us_to_ticks((uint32_t)us, nrf_timer_frequency_get(pwm->timer_reg));
    pwm_ticks_set(pwm, obj->pwm_channel, ticks);
}

#endif // DEVICE_PWMOUT
