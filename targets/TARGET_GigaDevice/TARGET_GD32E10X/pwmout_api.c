/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
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
#include "pwmout_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"

#define DEV_PWMOUT_APB_MASK     0x00010000U
#define DEV_PWMOUT_APB1         0U
#define DEV_PWMOUT_APB2         1U

static uint32_t timer_get_clock(uint32_t timer_periph);

static void dev_pwmout_init(pwmout_t *obj)
{
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    MBED_ASSERT(obj);
    uint32_t periph = obj->pwm;

    switch (periph) {
        case TIMER0:
            rcu_periph_clock_enable(RCU_TIMER0);
            break;

        case TIMER1:
            rcu_periph_clock_enable(RCU_TIMER1);
            break;

        case TIMER2:
            rcu_periph_clock_enable(RCU_TIMER2);
            break;

        case TIMER3:
            rcu_periph_clock_enable(RCU_TIMER3);
            break;

        case TIMER4:
            rcu_periph_clock_enable(RCU_TIMER4);
            break;

        case TIMER7:
            rcu_periph_clock_enable(RCU_TIMER7);
            break;
        case TIMER8:
            rcu_periph_clock_enable(RCU_TIMER8);
            break;

        case TIMER9:
            rcu_periph_clock_enable(RCU_TIMER9);
            break;

        case TIMER10:
            rcu_periph_clock_enable(RCU_TIMER10);
            break;

        case TIMER11:
            rcu_periph_clock_enable(RCU_TIMER11);
            break;

        case TIMER12:
            rcu_periph_clock_enable(RCU_TIMER12);
            break;

        case TIMER13:
            rcu_periph_clock_enable(RCU_TIMER13);
            break;
    }
    /* configure TIMER base function */
    timer_initpara.prescaler             = 119;
    timer_initpara.period                = 9999;
    timer_initpara.clockdivision         = 0;
    timer_initpara.counterdirection      = TIMER_COUNTER_UP;
    timer_initpara.alignedmode           = TIMER_COUNTER_EDGE;

    timer_init(obj->pwm, &timer_initpara);

    /* configure TIMER channel output function */
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_HIGH;
    timer_channel_output_config(obj->pwm, obj->ch, &timer_ocintpara);
    timer_channel_output_mode_config(obj->pwm, obj->ch, TIMER_OC_MODE_PWM0);
    timer_channel_output_fast_config(obj->pwm, obj->ch, TIMER_OC_FAST_DISABLE);

    timer_primary_output_config(obj->pwm, ENABLE);
}

static uint8_t dev_pwmout_apb_check(uint32_t periph)
{
    uint8_t reval = DEV_PWMOUT_APB1;

    /* check peripherals belongs to APB1 or APB2 */
    if (DEV_PWMOUT_APB_MASK == (periph & DEV_PWMOUT_APB_MASK)) {
        reval = DEV_PWMOUT_APB2;
    }

    return reval;
}

void pwmout_init(pwmout_t *obj, PinName pin)
{
    MBED_ASSERT(obj);

    obj->pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(obj->pwm != (PWMName)NC);

    uint32_t function = pinmap_function(pin, PinMap_PWM);
    MBED_ASSERT(function != (uint32_t)NC);
    obj->ch = GD_PIN_CHANNEL_GET(function);
    /* Peripheral initialization */
    dev_pwmout_init(obj);
    /* pin function initialization */
    pinmap_pinout(pin, PinMap_PWM);
}

void pwmout_free(pwmout_t *obj)
{
    timer_channel_output_state_config(obj->pwm, obj->ch, TIMER_CCX_DISABLE);
}

void pwmout_write(pwmout_t *obj, float value)
{
    uint16_t period;
    uint16_t pulse;

    timer_disable(obj->pwm);
    /* overflow protection */
    if (value < (float)0.0) {
        value = 0.0;
    } else if (value > (float)1.0) {
        value = 1.0;
    }

    period = TIMER_CAR(obj->pwm);
    pulse = (uint16_t)(period * value);

    timer_channel_output_pulse_value_config(obj->pwm, obj->ch, pulse);

    timer_enable(obj->pwm);
}

float pwmout_read(pwmout_t *obj)
{
    float value = 0;
    uint16_t period;
    uint16_t pulse;

    period = TIMER_CAR(obj->pwm);

    switch (obj->ch) {
        case TIMER_CH_0:
            pulse = TIMER_CH0CV(obj->pwm);
            break;

        case TIMER_CH_1:
            pulse = TIMER_CH1CV(obj->pwm);
            break;

        case TIMER_CH_2:
            pulse = TIMER_CH2CV(obj->pwm);
            break;

        case TIMER_CH_3:
            pulse = TIMER_CH3CV(obj->pwm);
            break;

        default:
            error("Error: pwm channel error! \r\n");
    }

    /* calculated waveform duty ratio */
    value = (float)(pulse) / (float)(period);

    if (value > (float)1.0) {
        value = (float)1.0;
    }

    return value;
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

    uint32_t ultemp = 0;
    uint32_t timer_clk = 0;
    uint32_t period = us - 1;
    uint32_t prescaler;
    float duty_ratio;

    duty_ratio = pwmout_read(obj);

    timer_disable(obj->pwm);

    timer_clk = timer_get_clock(obj->pwm);

    ultemp = (timer_clk / 1000000);
    prescaler = ultemp;
    obj->cnt_unit = 1;

    while (period > 0xFFFF) {
        obj->cnt_unit = obj->cnt_unit << 1;
        period = period >> 1;
        prescaler = ultemp * obj->cnt_unit;
    }

    if (prescaler > 0xFFFF) {
        error("Error: TIMER prescaler value is overflow \r\n");
    }

    timer_autoreload_value_config(obj->pwm, period);
    timer_prescaler_config(obj->pwm, prescaler - 1, TIMER_PSC_RELOAD_NOW);

    ultemp = duty_ratio * us;

    pwmout_pulsewidth_us(obj, ultemp);

    timer_enable(obj->pwm);
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
    uint32_t pulse;
    uint32_t period;

    period = TIMER_CAR(obj->pwm);
    pulse = us / obj->cnt_unit;

    if (pulse > period) {
        pulse = period;
    }

    timer_channel_output_pulse_value_config(obj->pwm, obj->ch, pulse);
}

static uint32_t timer_get_clock(uint32_t timer_periph)
{
    uint32_t timerclk;

    if ((TIMER0 == timer_periph) || (TIMER7 == timer_periph) ||
            (TIMER8 == timer_periph) || (TIMER9 == timer_periph) || (TIMER10 == timer_periph)) {
        /* get the current APB2 TIMER clock source */
        if (RCU_APB2_CKAHB_DIV1 == (RCU_CFG0 & RCU_CFG0_APB2PSC)) {
            timerclk = rcu_clock_freq_get(CK_APB2);
        } else {
            timerclk = rcu_clock_freq_get(CK_APB2) * 2;
        }
    } else {
        /* get the current APB1 TIMER clock source */
        if (RCU_APB1_CKAHB_DIV1 == (RCU_CFG0 & RCU_CFG0_APB1PSC)) {
            timerclk = rcu_clock_freq_get(CK_APB1);
        } else {
            timerclk = rcu_clock_freq_get(CK_APB1) * 2;
        }
    }

    return timerclk;
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}
