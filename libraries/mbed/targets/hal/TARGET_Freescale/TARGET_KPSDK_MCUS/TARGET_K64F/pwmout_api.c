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
    {PTA0 , PWM_6 , 3},
    {PTA1 , PWM_7 , 3},
    {PTA2 , PWM_8 , 3},
    {PTA3 , PWM_1 , 3},
    {PTA4 , PWM_2 , 3},
    {PTA5 , PWM_3 , 3},
    {PTA6 , PWM_4 , 3},
    {PTA7 , PWM_5 , 3},
    {PTA8 , PWM_9 , 3},
    {PTA9 , PWM_10, 3},
    {PTA10, PWM_17, 3},
    {PTA11, PWM_18, 3},
    {PTA12, PWM_9 , 3},
    {PTA13, PWM_10, 3},
    
    {PTB0 , PWM_9 , 3},
    {PTB1 , PWM_10, 3},
    {PTB18, PWM_17, 3},
    {PTB19, PWM_18, 3},
    
    {PTC1 , PWM_1 , 4},
    {PTC2 , PWM_2 , 4},
    {PTC3 , PWM_3 , 4},
    {PTC4 , PWM_4 , 4},
    {PTC5 , PWM_3 , 7},
    {PTC8 , PWM_29, 3},
    {PTC9 , PWM_30, 3},
    {PTC10, PWM_31, 3},
    {PTC11, PWM_32, 3},
    
    {PTD0 , PWM_25, 4},
    {PTD1 , PWM_26, 4},
    {PTD2 , PWM_27, 4},
    {PTD3 , PWM_28, 4},
    {PTD4 , PWM_5 , 4},
    {PTD5 , PWM_6 , 4},
    {PTD6 , PWM_7 , 4},
    {PTD4 , PWM_5 , 4},
    {PTD7 , PWM_8 , 4},
    
    {PTE5 , PWM_25, 6},
    {PTE6 , PWM_26, 6},
        
    {NC   , NC    , 0}
};

static float pwm_clock_mhz;

void pwmout_init(pwmout_t* obj, PinName pin) {
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    if (pwm == (PWMName)NC) {
        error("PwmOut pin mapping failed");
    }
    obj->pwm_name = pwm;

    uint32_t pwm_base_clock;
    clock_manager_get_frequency(kBusClock, &pwm_base_clock);
    float clkval = (float)pwm_base_clock / 1000000.0f;
    uint32_t clkdiv = 0;
    while (clkval > 1) {
        clkdiv++;
        clkval /= 2.0f;
        if (clkdiv == 7) {
            break;
        }
    }

    pwm_clock_mhz = clkval;
    uint32_t channel = pwm & 0xF;
    uint32_t instance = pwm >> TPM_SHIFT;
    clock_manager_set_gate(kClockModuleFTM, instance, true);
    ftm_hal_set_tof_frequency(instance, 3);
    ftm_hal_set_clock_source(instance, kClock_source_FTM_SystemClk);
    ftm_hal_set_clock_ps(instance, (ftm_clock_ps_t)clkdiv);
    ftm_hal_set_counter_init_val(instance, 0);
    // default to 20ms: standard for servos, and fine for e.g. brightness control
    pwmout_period_ms(obj, 20);
    pwmout_write    (obj, 0);
    ftm_config_t config = {
        .mode = kFtmEdgeAlignedPWM,
        .channel = channel,
        .edge_mode = {.ftm_pwm_edge_mode = kFtmHighTrue}
    };
    ftm_hal_enable_pwm_mode(instance, &config);

    // Wire pinout
    pinmap_pinout(pin, PinMap_PWM);
}

void pwmout_free(pwmout_t* obj) {
}

void pwmout_write(pwmout_t* obj, float value) {
    uint32_t instance = obj->pwm_name >> TPM_SHIFT;
    if (value < 0.0f) {
        value = 0.0f;
    } else if (value > 1.0f) {
        value = 1.0f;
    }
    uint16_t mod = ftm_hal_get_mod(instance);
    uint32_t new_count = (uint32_t)((float)(mod) * value);
    // Stop FTM clock to ensure instant update of MOD register
    ftm_hal_set_clock_source(instance, kClock_source_FTM_None);
    ftm_hal_set_channel_count_value(instance, obj->pwm_name & 0xF, new_count);
    ftm_hal_set_counter(instance, 0);
    ftm_hal_set_clock_source(instance, kClock_source_FTM_SystemClk);
}

float pwmout_read(pwmout_t* obj) {
    uint16_t count = ftm_hal_get_channel_count_value(obj->pwm_name >> TPM_SHIFT, obj->pwm_name & 0xF, 0);
    uint16_t mod = ftm_hal_get_mod(obj->pwm_name >> TPM_SHIFT);
    if (mod == 0)
        return 0.0;
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
    uint32_t instance = obj->pwm_name >> TPM_SHIFT;
    float dc = pwmout_read(obj);
    // Stop FTM clock to ensure instant update of MOD register
    ftm_hal_set_clock_source(instance, kClock_source_FTM_None);
    ftm_hal_set_mod(instance, (uint32_t)(pwm_clock_mhz * (float)us) - 1);
    pwmout_write(obj, dc);
    ftm_hal_set_clock_source(instance, kClock_source_FTM_SystemClk);
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
    uint32_t value = (uint32_t)(pwm_clock_mhz * (float)us);
    ftm_hal_set_channel_count_value(obj->pwm_name >> TPM_SHIFT, obj->pwm_name & 0xF, value);
}
