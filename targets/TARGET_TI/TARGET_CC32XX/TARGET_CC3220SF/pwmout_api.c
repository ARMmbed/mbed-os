/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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
#include "pinmap.h"
#include "PeripheralPins.h"

#include "ti/drivers/pwm/PWMTimerCC32XX.h"
#include "ti/drivers/PWM.h"
#include "CC3220SF_LAUNCHXL.h"

extern const PWM_Config PWM_config[];

void pwmout_init(pwmout_t *obj, PinName pin)
{
    PWM_Params pwmParams;
    int        pwmIndex = CC3220SF_LAUNCHXL_PWMCOUNT;

    PWM_init();

    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(pwm != (PWMName)NC);

    obj->pwm = pwm;

    switch (pin) {
        case PIN_01:
            pwmIndex = CC3220SF_LAUNCHXL_PWM6;
            break;

        case PIN_02:
            pwmIndex = CC3220SF_LAUNCHXL_PWM7;
            break;

        case PIN_17:
            pwmIndex = CC3220SF_LAUNCHXL_PWM0;
            break;

        case PIN_19:
            pwmIndex = CC3220SF_LAUNCHXL_PWM3;
            break;

        case PIN_21:
            pwmIndex = CC3220SF_LAUNCHXL_PWM2;
            break;

        case PIN_64:
            pwmIndex = CC3220SF_LAUNCHXL_PWM5;
            break;

        default:
            while (1);
    }

    obj->handle = (void *)&PWM_config[pwmIndex];

    // Initialize the PWM parameters
    PWM_Params_init(&pwmParams);

    obj->duty_percent = PWM_DEFAULT_DUTY_PERCENT;
    obj->period_us = PWM_DEFAULT_PERIOD_US;

    if (PWM_open(pwmIndex, &pwmParams)) {
        PWM_start((PWM_Handle)obj->handle);
    } else {
        while (1);
    }
}

void pwmout_free(pwmout_t *obj)
{
    PWM_stop((PWM_Handle)obj->handle);
    PWM_close((PWM_Handle)obj->handle);
}

void pwmout_write(pwmout_t *obj, float value)
{
    PWM_setDuty((PWM_Handle)obj->handle, value * 100);
    obj->duty_percent = value;
}

float pwmout_read(pwmout_t *obj)
{
    return (obj->duty_percent);
}

void pwmout_period(pwmout_t *obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000 * 1000);
}

void pwmout_period_ms(pwmout_t *obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t *obj, int us)
{
    PWM_setPeriod((PWM_Handle)obj->handle, us);
    obj->period_us = us;
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
    if (obj->period_us) {
        float value = (float)us / (float)obj->period_us;
        pwmout_write(obj, value);
    }
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}
