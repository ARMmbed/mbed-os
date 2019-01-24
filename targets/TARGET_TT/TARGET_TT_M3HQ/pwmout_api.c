/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
 * SPDX-License-Identifier: Apache-2.0
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
#include "pinmap.h"
#include "gpio_include.h"

static const PinMap PinMap_PWM[] = {
    {PB0, PWM_0, PIN_DATA(5, 1)},
    {PC0, PWM_1, PIN_DATA(4, 1)},
    {PJ0, PWM_2, PIN_DATA(4, 1)},
    {PK2, PWM_3, PIN_DATA(4, 1)},
    {PN0, PWM_4, PIN_DATA(4, 1)},
    {PL5, PWM_5, PIN_DATA(4, 1)},
    {PG2, PWM_6, PIN_DATA(4, 1)},
    {NC,  NC,    0}
};

static const uint32_t prescale_tbl[] = {
    2, 8, 32, 128, 256, 512, 1024
};

void pwmout_init(pwmout_t* obj, PinName pin)
{
    uint16_t counter = 0;

    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);  // Determine the pwm channel
    MBED_ASSERT(pwm != (PWMName)NC);

    switch (pwm) {
        case PWM_0:
            obj->channel = TSB_T32A1;
            TSB_CG_FSYSENA_IPENA25 = ENABLE;
            TSB_CG_FSYSENA_IPENA01 = ENABLE;
            break;
        case PWM_1:
            obj->channel = TSB_T32A2;
            TSB_CG_FSYSENA_IPENA26 = ENABLE;
            TSB_CG_FSYSENA_IPENA02 = ENABLE;
            break;
        case PWM_2:
            obj->channel = TSB_T32A3;
            TSB_CG_FSYSENA_IPENA27 = ENABLE;
            TSB_CG_FSYSENA_IPENA08 = ENABLE;
            break;
        case PWM_3:
            obj->channel = TSB_T32A4;
            TSB_CG_FSYSENA_IPENA28 = ENABLE;
            TSB_CG_FSYSENA_IPENA09 = ENABLE;
            break;
        case PWM_4:
            obj->channel = TSB_T32A5;
            TSB_CG_FSYSENA_IPENA29 = ENABLE;
            TSB_CG_FSYSENA_IPENA12 = ENABLE;
            break;
        case PWM_5:
            obj->channel = TSB_T32A6;
            TSB_CG_FSYSENA_IPENA30 = ENABLE;
            TSB_CG_FSYSENA_IPENA10 = ENABLE;
            break;
        case PWM_6:
            obj->channel = TSB_T32A7;
            TSB_CG_FSYSENA_IPENA31 = ENABLE;
            TSB_CG_FSYSENA_IPENA06 = ENABLE;
            break;
        default:
            obj->channel = NULL;
            break;
    }

    pinmap_pinout(pin, PinMap_PWM);  // Set pin function as PWM
    obj->pin = pin;
    obj->period = DEFAULT_PERIOD;
    obj->divisor = DEFAULT_CLOCK_DIVISION;
    obj->channel->MOD = (T32A_MODE_32 | T32A_DBG_HALT_STOP);
    obj->channel->RUNC = (T32A_COUNT_STOP | T32A_COUNT_DONT_START | T32A_RUN_DISABLE);
    obj->channel->CRC = (T32A_PRSCLx_32 | T32A_WBF_ENABLE | T32A_RELOAD_TREGx);
    obj->channel->OUTCRC0 = T32A_OCR_DISABLE;
    obj->channel->OUTCRC1 = (T32A_OCRCMPx0_SET | T32A_OCRCMPx1_CLR);
    counter = ((DEFAULT_PERIOD * (SystemCoreClock)) / obj->divisor);
    obj->channel->RGC0 = counter;
    obj->channel->RGC1 = counter;
    obj->trailing_timing = counter;
    obj->leading_timing = counter;
}

void pwmout_free(pwmout_t* obj)
{
    // Stops and clear count operation
    obj->channel->RUNC = (T32A_RUN_DISABLE | T32A_COUNT_STOP);
    pwmout_write(obj, 0);
    obj->pin = NC;
    obj->channel = NULL;
    obj->trailing_timing = 0;
    obj->leading_timing = 0;
    obj->divisor = 0;
}

void pwmout_write(pwmout_t* obj, float value)
{
    obj->channel->RUNC = (T32A_RUN_DISABLE | T32A_COUNT_STOP); // Stop timer for setting clock again
    obj->leading_timing = (obj->trailing_timing -
                           (obj->trailing_timing * value)); // leading_timing value
    obj->channel->RGC0 = obj->leading_timing;  // Setting TBxRG0 register
    obj->channel->RUNC = (T32A_RUN_ENABLE | T32A_COUNT_START);  // Start count operation
}

float pwmout_read(pwmout_t* obj)
{
    float duty_cycle = ((float)(obj->trailing_timing - obj->leading_timing)
                        / obj->trailing_timing);
    return duty_cycle;
}

void pwmout_period(pwmout_t* obj, float seconds)
{
    pwmout_period_us(obj, (int)(seconds * 1000000.0f));
}

void pwmout_period_ms(pwmout_t* obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

void pwmout_period_us(pwmout_t* obj, int us)
{
    float seconds = 0;
    int cycles = 0;
    uint32_t clkdiv = 0;
    int i = 0;
    float duty_cycle = 0;

    seconds = (float)(us / 1000000.0f);
    // Select highest timer resolution
    for (i = 0; i < 7; ++i) {
        cycles = (int)(((SystemCoreClock) / prescale_tbl[i]) * seconds);
        if (cycles <= MAX_COUNTER_16B) {
            clkdiv = i + 1; // range 1:7
            clkdiv <<= 28;
            break;
        } else {
            cycles = MAX_COUNTER_16B;
            clkdiv = 7;
            clkdiv <<= 28;
        }
    }
    // Stop timer for setting clock again
    obj->channel->RUNC = (T32A_RUN_DISABLE | T32A_COUNT_STOP);
    // Restore the duty-cycle
    duty_cycle = ((float)(obj->trailing_timing - obj->leading_timing)
                  / obj->trailing_timing);
    obj->trailing_timing = cycles;
    obj->leading_timing = (cycles - (cycles * duty_cycle));
    // Change the source clock division and period
    obj->channel->MOD = T32A_MODE_32;
    obj->channel->CRC = (clkdiv | T32A_WBF_ENABLE | T32A_RELOAD_TREGx);
    obj->channel->OUTCRC0 = T32A_OCR_DISABLE;
    obj->channel->OUTCRC1 = (T32A_OCRCMPx0_SET | T32A_OCRCMPx1_CLR);
    obj->channel->RGC0 = obj->leading_timing;
    obj->channel->RGC1 = obj->trailing_timing;
    obj->channel->RUNC = (T32A_RUN_ENABLE | T32A_COUNT_START);  // Start count operation
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
    float seconds = 0;
    float value = 0;

    seconds = (float)(us / 1000000.0f);
    value = (((seconds / obj->period) * 100.0f) / 100.0f);

    pwmout_write(obj, value);
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}
