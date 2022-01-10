/* mbed Microcontroller Library
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
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
#include "PeripheralNames.h"
#include "pinmap.h"

#define DUTY_CYCLE_INIT_VALUE           1
#define CALCULATE_RGC1_VAL              2.4

static const PinMap PinMap_PWM[] = {
    {PF4, PWM_0,  PIN_DATA(5, 1)},
    {PU2, PWM_1,  PIN_DATA(5, 1)},
    {PC2, PWM_2,  PIN_DATA(6, 1)},
    {PN1, PWM_3,  PIN_DATA(5, 1)},
    {NC,  NC,     0}
};

void pwmout_init(pwmout_t *obj, PinName pin)
{
    // Determine the pwm channel
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);

    // Assert input is valid
    MBED_ASSERT(pwm != (PWMName)NC);

    switch (pwm) {
        case PWM_0:
            obj->p_t32a.p_instance   = TSB_T32A1;
            // Clock enable of T32A ch01
            TSB_CG_FSYSMENA_IPMENA29 = TXZ_ENABLE;
            break;
        case PWM_1:
            obj->p_t32a.p_instance   = TSB_T32A2;
            // Clock enable of T32A ch02
            TSB_CG_FSYSMENA_IPMENA30 = TXZ_ENABLE;
            break;
        case PWM_2:
            obj->p_t32a.p_instance   = TSB_T32A3;
            // Clock enable of T32A ch03
            TSB_CG_FSYSMENA_IPMENA31 = TXZ_ENABLE;
            break;
        case PWM_3:
            obj->p_t32a.p_instance   = TSB_T32A5;
            // Clock enable of T32A ch05
            TSB_CG_FSYSMENB_IPMENB01 = TXZ_ENABLE;
            break;
        default:
            obj->p_t32a.p_instance = NULL;
            break;
    }

    if (obj->p_t32a.p_instance == NULL) {
        return;
    }

    // Enable clock for GPIO port.
    if (pwm == PWM_1) {
        TSB_CG->FSYSMENA |= (TXZ_ENABLE << (PIN_PORT(pin) + 3));
    } else {
        TSB_CG->FSYSMENA |= (TXZ_ENABLE << (PIN_PORT(pin)));
    }

    // Set pin function as PWM
    pinmap_pinout(pin, PinMap_PWM);

    // Default to 20ms, 0% duty cycle
    // Assign same init value to trailing and leading timing duty cycle is zero.
    pwmout_period_ms(obj, 20);
}

void pwmout_free(pwmout_t *obj)
{
    // Stop PWM
    obj->p_t32a.p_instance->RUNC    = (T32A_RUN_ENABLE | T32A_COUNT_STOP);
    obj->trailing_timing = TXZ_DISABLE;
    obj->leading_timing = TXZ_DISABLE;
    obj->p_t32a.p_instance = NULL;
}

void pwmout_write(pwmout_t *obj, float value)
{
    // Stop PWM
    obj->p_t32a.p_instance->RUNC = (T32A_RUN_DISABLE | T32A_COUNT_STOP);

    if (value <= 0.0f) {
        value = TXZ_DISABLE;
    } else if (value >= 1.0f) {
        value = TXZ_ENABLE;
    }

    // Store the new leading_timing value
    obj->leading_timing = obj->trailing_timing - (obj->trailing_timing * value);

    // Setting T32A_RGA0 register
    obj->p_t32a.p_instance->RGC0 = obj->leading_timing;

    // Start PWM
    obj->p_t32a.p_instance->RUNC = (T32A_RUN_ENABLE | T32A_COUNT_START);
}

float pwmout_read(pwmout_t *obj)
{
    float duty_cycle = (float)(obj->trailing_timing - obj->leading_timing) / obj->trailing_timing;
    return duty_cycle;
}

void pwmout_period(pwmout_t *obj, float seconds)
{
    pwmout_period_us(obj, (int)(seconds * 1000000.0f));
}

void pwmout_period_ms(pwmout_t *obj, int ms)
{
    pwmout_period_us(obj, (ms * 1000));
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t *obj, int us)
{
    uint32_t prscl   = 0;
    float duty_cycle = 0;
    float seconds    = (float)((us) / 1000000.0f);

    obj->period = seconds;
    // Restore the duty-cycle
    duty_cycle  = ((float)(obj->trailing_timing - obj->leading_timing) / obj->trailing_timing);
    prscl       = T32A_PRSCLx_32;

    obj->trailing_timing = (us * CALCULATE_RGC1_VAL);
    obj->leading_timing  = ((obj->trailing_timing) - (obj->trailing_timing * duty_cycle));

    obj->p_t32a.p_instance->MOD     =  T32A_MODE_32;
    obj->p_t32a.p_instance->RUNC    = (T32A_RUN_DISABLE | T32A_COUNT_STOP);
    obj->p_t32a.p_instance->CRC     = (prscl | T32A_RELOAD_TREGx);
    obj->p_t32a.p_instance->IMC     = (T32A_IMUFx_MASK_REQ | T32A_IMOFx_MASK_REQ |
                                       T32A_IMx1_MASK_REQ  | T32A_IMx0_MASK_REQ);
    obj->p_t32a.p_instance->RGC0    = obj->leading_timing;
    obj->p_t32a.p_instance->RGC1    = obj->trailing_timing;
    obj->p_t32a.p_instance->OUTCRC0 = T32A_OCR_DISABLE;
    obj->p_t32a.p_instance->OUTCRC1 = (T32A_OCRCMPx1_CLR | T32A_OCRCMPx0_SET);
    obj->p_t32a.p_instance->RUNC    = (T32A_RUN_ENABLE   | T32A_COUNT_START);
}

int pwmout_read_period_us(pwmout_t *obj)
{
    return obj->trailing_timing;
}

void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
    pwmout_pulsewidth_us(obj, (seconds * 1000000.0f));
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    pwmout_pulsewidth_us(obj, (ms * 1000));
}

void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    float seconds = 0;
    float value   = 0;

    seconds = (float)(us / 1000000.0f);
    value   = (((seconds / obj->period) * 100.0f) / 100.0f);
    pwmout_write(obj, value);
}

int pwmout_read_pulsewidth_us(pwmout_t *obj)
{
    return obj->trailing_timing - obj->leading_timing;
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}
