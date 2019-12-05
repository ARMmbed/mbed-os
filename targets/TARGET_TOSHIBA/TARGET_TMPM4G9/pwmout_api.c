/* mbed Microcontroller Library
 *
 * Copyright (C) 2019, Toshiba Electronic Device Solutions Corporation
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
#include "PeripheralNames.h"
#include "pinmap.h"

#define GPIO_CLK_OFFSET                 2
// Because Timer operating frequency is 2.5 MhZ
#define CALCULATE_RGC1_VAL              2.5

static const PinMap PinMap_PWM[] = {
    {PA5, PWM_0,  PIN_DATA(5, 1)},
    {PB2, PWM_1,  PIN_DATA(5, 1)},
    {PB4, PWM_2,  PIN_DATA(5, 1)},
    {PD2, PWM_3,  PIN_DATA(5, 1)},
    {PD4, PWM_4,  PIN_DATA(5, 1)},
    {PE1, PWM_5,  PIN_DATA(5, 1)},
    {PE6, PWM_6,  PIN_DATA(5, 1)},
    {PC2, PWM_7,  PIN_DATA(5, 1)},
    {PL6, PWM_8,  PIN_DATA(3, 1)},
    {PC4, PWM_9,  PIN_DATA(5, 1)},
    {PM2, PWM_10, PIN_DATA(3, 1)},
    {PU0, PWM_11, PIN_DATA(3, 1)},
    {PU6, PWM_12, PIN_DATA(3, 1)},
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
            TSB_CG_FSYSMENA_IPMENA07 = TXZ_ENABLE;
            break;
        case PWM_1:
            obj->p_t32a.p_instance   = TSB_T32A2;
            // Clock enable of T32A ch02
            TSB_CG_FSYSMENA_IPMENA08 = TXZ_ENABLE;
            break;
        case PWM_2:
            obj->p_t32a.p_instance   = TSB_T32A3;
            // Clock enable of T32A ch03
            TSB_CG_FSYSMENA_IPMENA09 = TXZ_ENABLE;
            break;
        case PWM_3:
            obj->p_t32a.p_instance   = TSB_T32A4;
            // Clock enable of T32A ch04
            TSB_CG_FSYSMENA_IPMENA10 = TXZ_ENABLE;
            break;
        case PWM_4:
            obj->p_t32a.p_instance   = TSB_T32A5;
            // Clock enable of T32A ch05
            TSB_CG_FSYSMENA_IPMENA11 = TXZ_ENABLE;
            break;
        case PWM_5:
            obj->p_t32a.p_instance   = TSB_T32A6;
            // Clock enable of T32A ch06
            TSB_CG_FSYSMENA_IPMENA12 = TXZ_ENABLE;
            break;
        case PWM_6:
            obj->p_t32a.p_instance   = TSB_T32A7;
            // Clock enable of T32A ch07
            TSB_CG_FSYSMENA_IPMENA13 = TXZ_ENABLE;
            break;
        case PWM_7:
            obj->p_t32a.p_instance   = TSB_T32A8;
            // Clock enable of T32A ch08
            TSB_CG_FSYSMENA_IPMENA14 = TXZ_ENABLE;
            break;
        case PWM_8:
            obj->p_t32a.p_instance   = TSB_T32A9;
            // Clock enable of T32A ch09
            TSB_CG_FSYSMENA_IPMENA15 = TXZ_ENABLE;
            break;
        case PWM_9:
            obj->p_t32a.p_instance   = TSB_T32A10;
            // Clock enable of T32A ch10
            TSB_CG_FSYSMENA_IPMENA16 = TXZ_ENABLE;
            break;
        case PWM_10:
            obj->p_t32a.p_instance   = TSB_T32A11;
            // Clock enable of T32A ch11
            TSB_CG_FSYSMENA_IPMENA17 = TXZ_ENABLE;
            break;
        case PWM_11:
            obj->p_t32a.p_instance   = TSB_T32A12;
            // Clock enable of T32A ch12
            TSB_CG_FSYSMENA_IPMENA18 = TXZ_ENABLE;
            break;
        case PWM_12:
            obj->p_t32a.p_instance   = TSB_T32A13;
            // Clock enable of T32A ch13
            TSB_CG_FSYSMENA_IPMENA19 = TXZ_ENABLE;
            break;
        default:
            obj->p_t32a.p_instance = NULL;
            break;
    }

    if (obj->p_t32a.p_instance == NULL) {
        return;
    }

    // Enable clock for GPIO port.
    TSB_CG->FSYSMENB |= (TXZ_ENABLE << ((PIN_PORT(pin)) + GPIO_CLK_OFFSET));

    // Set pin function as PWM
    pinmap_pinout(pin, PinMap_PWM);

    // Default to 20ms, 0% duty cycle
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
    obj->p_t32a.p_instance->RUNC = (T32A_RUN_ENABLE | T32A_COUNT_STOP);

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
    obj->leading_timing  = ((obj->trailing_timing)- (obj->trailing_timing * duty_cycle));

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

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}
