/* mbed Microcontroller Library
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#include "mbed_error.h"
#include "cyhal_pwm.h"

#if DEVICE_PWMOUT

#ifdef __cplusplus
extern "C" {
#endif

static const int CY_US_PER_SECOND = 1000000;
static const int CY_US_PER_MS = 1000;

void pwmout_init(pwmout_t *obj, PinName pin)
{
    if (CY_RSLT_SUCCESS != cyhal_pwm_init(&(obj->hal_pwm), pin, NULL)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_PWM, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_pwm_init");
    }
    obj->period_us = 100;
    obj->width_us = 0;
}

void pwmout_free(pwmout_t *obj)
{
    cyhal_pwm_free(&(obj->hal_pwm));
}

void pwmout_write(pwmout_t *obj, float percent)
{
    MBED_ASSERT(percent >= 0.0f && percent <= 1.0f);
    pwmout_pulsewidth_us(obj, (int)(percent * obj->period_us));
}

float pwmout_read(pwmout_t *obj)
{
    return ((float)(obj->width_us) / obj->period_us);
}

void pwmout_period(pwmout_t *obj, float seconds)
{
    pwmout_period_us(obj, (int)(seconds * CY_US_PER_SECOND));
}

void pwmout_period_ms(pwmout_t *obj, int ms)
{
    pwmout_period_us(obj, ms * CY_US_PER_MS);
}

void pwmout_period_us(pwmout_t *obj, int us)
{
    obj->period_us = (uint32_t)us;
    if (CY_RSLT_SUCCESS != cyhal_pwm_set_period(&(obj->hal_pwm), obj->period_us, obj->width_us)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_PWM, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_pwm_set_period");
    }
    if (CY_RSLT_SUCCESS != cyhal_pwm_start(&(obj->hal_pwm))) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_PWM, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_pwm_start");
    }
}

void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
    pwmout_pulsewidth_us(obj, (int)(seconds * CY_US_PER_SECOND));
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    pwmout_pulsewidth_us(obj, ms * CY_US_PER_MS);
}

void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    obj->width_us = (uint32_t)us;
    if (CY_RSLT_SUCCESS != cyhal_pwm_set_period(&(obj->hal_pwm), obj->period_us, obj->width_us)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_PWM, MBED_ERROR_CODE_FAILED_OPERATION), "cyhal_pwm_set_period");
    }
}

const PinMap *pwmout_pinmap(void)
{
    return PinMap_PWM_OUT;
}

#ifdef __cplusplus
}
#endif

#endif
