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
#include "pwmout_api.h"

#include "cmsis.h"
#include "pinmap.h"
#include "error.h"
#include "fsl_ftm_hal.h"
#include "fsl_mcg_hal.h"
#include "fsl_clock_manager.h"

static const PinMap PinMap_PWM[] = {
    {PTB18, PWM_17, 3},
    {NC   , NC    , 0}
};

static float pwm_clock;

void pwmout_init(pwmout_t* obj, PinName pin) {
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    if (pwm == (PWMName)NC) {
        error("PwmOut pin mapping failed");
    }
    obj->pwm_name = pwm;

    float clkval = clock_hal_get_fllclk() / 1000000.0f;
    uint32_t clkdiv = 0;
    while (clkval > 1) {
        clkdiv++;
        clkval /= 2.0f;
        if (clkdiv == 7) {
            break;
        }
    }
    uint32_t channel = pwm & 0xF;
    uint32_t instance = pwm >> TPM_SHIFT;
    clock_manager_set_gate(kClockModuleFTM, instance, true);
    ftm_hal_set_clock_ps(instance, (ftm_clock_ps_t)clkdiv);
    ftm_hal_set_clock_source(instance, kClock_source_FTM_SystemClk);
    ftm_hal_set_channel_edge_level(instance, channel, 2);
    // default to 20ms: standard for servos, and fine for e.g. brightness control
    pwmout_period_ms(obj, 20);
    pwmout_write    (obj, 0);

    // Wire pinout
    pinmap_pinout(pin, PinMap_PWM);
}

void pwmout_free(pwmout_t* obj) {
}

void pwmout_write(pwmout_t* obj, float value) {
    if (value < 0.0f) {
        value = 0.0f;
    } else if (value > 1.0f) {
        value = 1.0f;
    }
    uint16_t mod = ftm_hal_get_mod(obj->pwm_name >> TPM_SHIFT);
    uint32_t new_count = (uint32_t)((float)(mod) * value);
    ftm_hal_set_channel_count_value(obj->pwm_name >> TPM_SHIFT, obj->pwm_name & 0xF, new_count);
}

float pwmout_read(pwmout_t* obj) {
    uint16_t count = ftm_hal_get_channel_count_value(obj->pwm_name >> TPM_SHIFT, obj->pwm_name & 0xF, 0);
    uint16_t mod = ftm_hal_get_mod(obj->pwm_name >> TPM_SHIFT);
    float v = (float)(count) / (float)(mod);
    return (v > 1.0f) ? (1.0f) : (v);
}

void pwmout_period(pwmout_t* obj, float seconds) {
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t* obj, int ms) {
    pwmout_period_us(obj, ms * 1000);
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t* obj, int us) {
    float dc = pwmout_read(obj);
    ftm_hal_set_mod(obj->pwm_name >> TPM_SHIFT, (uint32_t)(pwm_clock * (float)us));
    pwmout_write(obj, dc);

}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
    uint32_t value = (uint32_t)(pwm_clock * (float)us);
    ftm_hal_set_channel_count_value(obj->pwm_name >> TPM_SHIFT, obj->pwm_name & 0xF, value);
}
