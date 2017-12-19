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
#include "mbed_assert.h"
#include "pwmout_api.h"

#if DEVICE_PWMOUT

#include "cmsis.h"
#include "pinmap.h"
#include "fsl_pwm.h"
#include "PeripheralPins.h"

static float pwm_clock_mhz;

/* Array of PWM peripheral base address. */
static PWM_Type *const pwm_addrs[] = PWM_BASE_PTRS;

extern void pwm_setup_clock();
extern uint32_t pwm_get_clock();

void pwmout_init(pwmout_t* obj, PinName pin)
{
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(pwm != (PWMName)NC);

    pwm_setup_clock();

    obj->pwm_name = pwm;

    uint32_t pwm_base_clock;
    pwm_base_clock = pwm_get_clock();
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
    uint32_t instance = (pwm >> PWM_SHIFT) & 0x3;
    uint32_t module = pwm >> PWM_MODULE_SHIFT;
    uint8_t pwmchannel = pwm & 0x1;
    pwm_config_t pwmInfo;

    PWM_GetDefaultConfig(&pwmInfo);
    pwmInfo.prescale = (pwm_clock_prescale_t)clkdiv;
    /* Initialize PWM module */
    PWM_Init(pwm_addrs[instance], (pwm_submodule_t)module, &pwmInfo);

    pwm_signal_param_t config = {
        .level = kPWM_HighTrue,
        .dutyCyclePercent = 0,
        .deadtimeValue = 0
    };
    if (pwmchannel == 0) {
        config.pwmChannel = kPWM_PwmA;
    } else {
        config.pwmChannel = kPWM_PwmB;
    }

    // default to 20ms: standard for servos, and fine for e.g. brightness control
    PWM_SetupPwm(pwm_addrs[instance], (pwm_submodule_t)module, &config, 1, kPWM_EdgeAligned, 50, pwm_base_clock);

    PWM_StartTimer(pwm_addrs[instance], module);

    // Wire pinout
    pinmap_pinout(pin, PinMap_PWM);
}

void pwmout_free(pwmout_t* obj)
{
    uint32_t instance = (obj->pwm_name >> PWM_SHIFT) & 0x3;
    uint32_t module = obj->pwm_name >> PWM_MODULE_SHIFT;

    PWM_Deinit(pwm_addrs[instance], (pwm_submodule_t)module);
}

void pwmout_write(pwmout_t* obj, float value)
{
    if (value < 0.0f) {
        value = 0.0f;
    } else if (value > 1.0f) {
        value = 1.0f;
    }

    PWM_Type *base = pwm_addrs[(obj->pwm_name >> PWM_SHIFT) & 0x3];
    uint32_t module = obj->pwm_name >> PWM_MODULE_SHIFT;
    uint32_t pwmchannel = obj->pwm_name & 0xF;
    uint16_t pulseCnt = 0;

    pulseCnt = base->SM[module].VAL1;
    uint32_t new_count = (uint32_t)((float)(pulseCnt) * value);

    /* Setup the PWM dutycycle */
    if (pwmchannel == 0) {
        base->SM[module].VAL2 = 0;
        base->SM[module].VAL3 = new_count;
    } else {
        base->SM[module].VAL4 = 0;
        base->SM[module].VAL5 = new_count;
    }

    /* Set the load okay bit */
    PWM_SetPwmLdok(base, module, true);

}

float pwmout_read(pwmout_t* obj)
{
    PWM_Type *base = pwm_addrs[(obj->pwm_name >> PWM_SHIFT) & 0x3];
    uint32_t module = obj->pwm_name >> PWM_MODULE_SHIFT;
    uint32_t pwmchannel = obj->pwm_name & 0xF;
    uint16_t count;
    uint16_t mod = (base->SM[module].VAL1) & PWM_VAL1_VAL1_MASK;

    if (pwmchannel == 0) {
        count = (base->SM[module].VAL3) & PWM_VAL3_VAL3_MASK;
    } else {
        count = (base->SM[module].VAL5) & PWM_VAL5_VAL5_MASK;
    }


    if (mod == 0) {
        return 0.0;
    }

    float v = (float)(count) / (float)(mod);
    return (v > 1.0f) ? (1.0f) : (v);
}

void pwmout_period(pwmout_t* obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t* obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t* obj, int us)
{
    PWM_Type *base = pwm_addrs[(obj->pwm_name >> PWM_SHIFT) & 0x3];
    uint32_t module = obj->pwm_name >> PWM_MODULE_SHIFT;
    float dc = pwmout_read(obj);

    /* Indicates the end of the PWM period */
    base->SM[module].VAL1 = PWM_VAL1_VAL1((pwm_clock_mhz * (float)us) - 1);

    pwmout_write(obj, dc);
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds)
{
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms)
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us)
{
    PWM_Type *base = pwm_addrs[(obj->pwm_name >> PWM_SHIFT) & 0x3];
    uint32_t module = obj->pwm_name >> PWM_MODULE_SHIFT;
    uint32_t pwmchannel = obj->pwm_name & 0xF;
    uint32_t value = (uint32_t)(pwm_clock_mhz * (float)us);

    /* Setup the PWM dutycycle */
    if (pwmchannel == 0) {
        base->SM[module].VAL2 = 0;
        base->SM[module].VAL3 = value;
    } else {
        base->SM[module].VAL4 = 0;
        base->SM[module].VAL5 = value;
    }
    /* Set the load okay bit */
    PWM_SetPwmLdok(base, module, true);
}

#endif
