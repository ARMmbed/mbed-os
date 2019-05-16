/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#include "device.h"
#include "pwmout_api.h"
#include "cy_tcpwm.h"
#include "cy_tcpwm_pwm.h"
#include "psoc6_utils.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "platform/mbed_error.h"
#include "cy_syspm.h"

#define PWMOUT_BASE_CLOCK_HZ        1000000UL
#define MAX_16_BIT_PERIOD           65536

static uint32_t pwm_clock_divider = CY_INVALID_DIVIDER;

static const cy_stc_tcpwm_pwm_config_t pwm_config = {
    .pwmMode            = CY_TCPWM_PWM_MODE_PWM,
    .clockPrescaler     = 0,    // will be configured separately
    .pwmAlignment       = CY_TCPWM_PWM_LEFT_ALIGN,
    .runMode            = CY_TCPWM_PWM_CONTINUOUS,
    .period0            = 0,    // will be configured separately
    .enablePeriodSwap   = 0,
    .compare0           = 0,    // will be configured separately
    .compare1           = 0,    // will be configured separately
    .enableCompareSwap  = 0,
    .interruptSources   = 0, //CY_TCPWM_INT_ON_CC,
    .invertPWMOut = CY_TCPWM_PWM_INVERT_DISABLE,
    .invertPWMOutN = CY_TCPWM_PWM_INVERT_ENABLE,
    .killMode = CY_TCPWM_PWM_ASYNC_KILL,
    .countInputMode     = CY_TCPWM_INPUT_LEVEL,
    .countInput         = CY_TCPWM_INPUT_1,
    .swapInputMode      = CY_TCPWM_INPUT_LEVEL,
    .swapInput          = CY_TCPWM_INPUT_1,
    .reloadInputMode    = CY_TCPWM_INPUT_LEVEL,
    .reloadInput        = CY_TCPWM_INPUT_0,
    .startInputMode     = CY_TCPWM_INPUT_LEVEL,
    .startInput         = CY_TCPWM_INPUT_0,
    .killInputMode      = CY_TCPWM_INPUT_LEVEL,
    .killInput          = CY_TCPWM_INPUT_0,
};


static void Cy_TCPWM_PWM_SetPrescaler(TCPWM_Type *base, uint32_t cntNum,  uint32_t prescaler)
{
    base->CNT[cntNum].CTRL = _CLR_SET_FLD32U(base->CNT[cntNum].CTRL, TCPWM_CNT_CTRL_GENERIC, prescaler);
}

static void pwm_start_32b(pwmout_t *obj, uint32_t new_period, uint32_t new_width)
{
    obj->period = new_period;
    obj->pulse_width = new_width;
    Cy_TCPWM_PWM_SetPeriod0(obj->base, obj->counter_id, obj->period - 1);
    Cy_TCPWM_PWM_SetCompare0(obj->base, obj->counter_id, obj->pulse_width);
    Cy_TCPWM_PWM_Enable(obj->base, obj->counter_id);
    Cy_TCPWM_TriggerStart(obj->base, 1UL << obj->counter_id);
}

static void pwm_start_16b(pwmout_t *obj, uint32_t period, uint32_t width)
{
    uint32_t prescaler = 0;

    obj->period = period;
    obj->pulse_width = width;

    // For 16-bit counters we need to configure prescaler appropriately.
    while ((period > MAX_16_BIT_PERIOD) && (prescaler < CY_TCPWM_PWM_PRESCALER_DIVBY_128)) {
        period /= 2;
        prescaler += 1;
    }
    if (period > MAX_16_BIT_PERIOD) {
        // We have reached the prescaler limit, set period to max value.
        error("Can't configure required PWM period.");
        period = MAX_16_BIT_PERIOD;
    }

    obj->prescaler = prescaler;
    width >>= prescaler;

    Cy_TCPWM_PWM_SetPeriod0(obj->base, obj->counter_id, period - 1);
    Cy_TCPWM_PWM_SetPrescaler(obj->base, obj->counter_id, prescaler);
    Cy_TCPWM_PWM_SetCompare0(obj->base, obj->counter_id, width);
    Cy_TCPWM_PWM_Enable(obj->base, obj->counter_id);
    Cy_TCPWM_TriggerStart(obj->base, 1UL << obj->counter_id);
}

static void pwm_start(pwmout_t *obj, uint32_t new_period, uint32_t new_pulse_width)
{
    obj->period = new_period;
    obj->pulse_width = new_pulse_width;
    Cy_TCPWM_PWM_Disable(obj->base, obj->counter_id);
    if (new_period > 0) {
        if (obj->base == TCPWM0) {
            pwm_start_32b(obj, new_period, new_pulse_width);
        } else {
            pwm_start_16b(obj, new_period, new_pulse_width);
        }
    }
}


/*
 * Callback handler to restart the timer after deep sleep.
 */
#if DEVICE_SLEEP && DEVICE_LPTICKER
static cy_en_syspm_status_t pwm_pm_callback(cy_stc_syspm_callback_params_t *callback_params)
{
    pwmout_t *obj = (pwmout_t *)callback_params->context;

    switch (callback_params->mode) {
        case CY_SYSPM_BEFORE_TRANSITION:
            /* Disable timer before transition */
            Cy_TCPWM_PWM_Disable(obj->base, obj->counter_id);
            break;

        case CY_SYSPM_AFTER_TRANSITION:
            /* Enable the timer to operate */
            if (obj->period > 0) {
                Cy_TCPWM_PWM_Enable(obj->base, obj->counter_id);
                Cy_TCPWM_TriggerStart(obj->base, 1UL << obj->counter_id);
            }
            break;

        default:
            break;
    }

    return CY_SYSPM_SUCCESS;
}
#endif // DEVICE_SLEEP && DEVICE_LPTICKER


void pwmout_init(pwmout_t *obj, PinName pin)
{
    uint32_t    pwm_cnt = 0;
    uint32_t    pwm_function = 0;
    uint32_t    abs_cnt_num = 0;

    MBED_ASSERT(obj);
    MBED_ASSERT(pin != (PinName)NC);
    // Allocate and setup clock.
    if (pwm_clock_divider == CY_INVALID_DIVIDER) {
        pwm_clock_divider = cy_clk_allocate_divider(CY_SYSCLK_DIV_8_BIT);
        if (pwm_clock_divider == CY_INVALID_DIVIDER) {
            error("PWM clock divider allocation failed.");
            return;
        }
        Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT,
                                   pwm_clock_divider,
                                   (CY_CLK_PERICLK_FREQ_HZ / PWMOUT_BASE_CLOCK_HZ) - 1);
        Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, pwm_clock_divider);
    }

    pwm_cnt = pinmap_peripheral(pin, PinMap_PWM_OUT);
    if (pwm_cnt != (uint32_t)NC) {
        if (cy_reserve_io_pin(pin)) {
            error("PWMOUT pin reservation conflict.");
        }
        obj->base = (TCPWM_Type*)CY_PERIPHERAL_BASE(pwm_cnt);
        obj->pin = pin;
        if (obj->base == TCPWM0) {
            obj->counter_id = ((PWMName)pwm_cnt - PWM_32b_0) / (PWM_32b_1 - PWM_32b_0);
            abs_cnt_num = obj->counter_id;
        } else {
            // TCPWM1 is used.
            obj->counter_id = ((PWMName)pwm_cnt - PWM_16b_0) / (PWM_16b_1 - PWM_16b_0);
            abs_cnt_num = obj->counter_id + 8;
        }
        if (cy_reserve_tcpwm(abs_cnt_num)) {
            error("PWMOUT Timer/Counter reservation conflict.");
        }

        // Configure clock.
        pwm_function = pinmap_function(pin, PinMap_PWM_OUT);
        obj->clock = CY_PIN_CLOCK(pwm_function);
        Cy_SysClk_PeriphAssignDivider(obj->clock, CY_SYSCLK_DIV_8_BIT, pwm_clock_divider);
        Cy_TCPWM_PWM_Init(obj->base, obj->counter_id, &pwm_config);
        pin_function(pin, pwm_function);
        // These will be properly configured later on.
        obj->period = 0;
        obj->pulse_width = 0;
        obj->prescaler = 0;
#if DEVICE_SLEEP && DEVICE_LPTICKER
        obj->pm_callback_handler.callback = pwm_pm_callback;
        obj->pm_callback_handler.type = CY_SYSPM_DEEPSLEEP;
        obj->pm_callback_handler.skipMode = 0;
        obj->pm_callback_handler.callbackParams = &obj->pm_callback_params;
        obj->pm_callback_params.base = obj->base;
        obj->pm_callback_params.context = obj;
        if (!Cy_SysPm_RegisterCallback(&obj->pm_callback_handler)) {
            error("PM callback registration failed!");
        }
#endif // DEVICE_SLEEP && DEVICE_LPTICKER

    } else {
        error("PWM OUT pinout mismatch.");
    }
}

void pwmout_free(pwmout_t *obj)
{
    // TODO: Not implemented yet.
}

void pwmout_write(pwmout_t *obj, float percent)
{
    uint32_t pulse_width;
    MBED_ASSERT(obj);

    if (percent < 0.0) {
        percent = 0.0;
    } else if (percent > 1.0) {
        percent = 1.0;
    }
    pulse_width = (uint32_t)(percent * obj->period + 0.5);
    pwm_start(obj, obj->period, pulse_width);
}

float pwmout_read(pwmout_t *obj)
{
    MBED_ASSERT(obj);

    return (float)(obj->pulse_width) / obj->period;
}

void pwmout_period(pwmout_t *obj, float seconds)
{
    uint32_t period;
    uint32_t pulse_width;

    MBED_ASSERT(obj);

    if (seconds < 0.0) {
        seconds = 0.0;
    }
    period = (uint32_t)(seconds * 1000000 + 0.5);
    pulse_width = (uint32_t)((uint64_t)period * obj->pulse_width / obj->period);
    pwm_start(obj, period, pulse_width);
}

void pwmout_period_ms(pwmout_t *obj, int ms)
{
    uint32_t period;
    uint32_t pulse_width;

    MBED_ASSERT(obj);

    if (ms < 0.0) {
        ms = 0.0;
    }
    period = (uint32_t)(ms * 1000 + 0.5);
    pulse_width = (uint32_t)((uint64_t)period * obj->pulse_width / obj->period);
    pwm_start(obj, period, pulse_width);
}

void pwmout_period_us(pwmout_t *obj, int us)
{
    uint32_t pulse_width;

    MBED_ASSERT(obj);

    if (us < 0) {
        us = 0;
    }
    pulse_width = (uint32_t)((uint64_t)us * obj->pulse_width / obj->period);
    pwm_start(obj, us, pulse_width);
}

void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
    uint32_t pulse_width;

    MBED_ASSERT(obj);

    if (seconds < 0.0) {
        seconds = 0.0;
    }
    pulse_width = (uint32_t)(seconds * 1000000 + 0.5);
    pwm_start(obj, obj->period, pulse_width);
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    uint32_t pulse_width;

    MBED_ASSERT(obj);

    if (ms < 0.0) {
        ms = 0.0;
    }
    pulse_width = (uint32_t)(ms * 1000 + 0.5);
    pwm_start(obj, obj->period, pulse_width);
}

void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    MBED_ASSERT(obj);

    if (us < 0) {
        us = 0;
    }
    pwm_start(obj, obj->period, us);
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM_OUT;
}
