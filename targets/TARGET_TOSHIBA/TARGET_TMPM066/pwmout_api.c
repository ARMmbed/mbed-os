/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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
#include "tmpm066_tmrb.h"

#define CLOCK_FREQUENCY         SystemCoreClock // input source clock
#define MAX_COUNTER_16B         0xFFFF
#define DEFAULT_CLOCK_DIVISION  32
#define DEFAULT_PERIOD          0.02f // 20ms

static const PinMap PinMap_PWM[] = {
    {PD1, PWM_0, PIN_DATA(1, 1)},
    {PD2, PWM_1, PIN_DATA(1, 1)},
    {PD3, PWM_2, PIN_DATA(1, 1)},
    {PF4, PWM_3, PIN_DATA(1, 1)},
    {PF5, PWM_4, PIN_DATA(1, 1)},
    {PJ0, PWM_5, PIN_DATA(1, 1)},
    {PJ1, PWM_6, PIN_DATA(1, 1)},
    {NC,  NC,    0}
};

static const uint32_t prescale_tbl[] = {
    2, 8, 32, 64, 128, 256, 512
};

void pwmout_init(pwmout_t *obj, PinName pin)
{
    uint16_t counter = 0;
    TMRB_FFOutputTypeDef FFStruct;
    TMRB_InitTypeDef m_tmrb;

    // Determine the pwm channel
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    //Assert input is valid
    MBED_ASSERT(pwm != (PWMName)NC);

    // Enable clock supply to TB0
    CG_SetFcPeriphA(CG_FC_PERIPH_TMRB0_3, ENABLE);
    CG_SetFcPeriphA(CG_FC_PERIPH_TMRB4_6, ENABLE);

    switch (pwm) {
        case PWM_0:
            obj->channel = TSB_TB0;
            break;
        case PWM_1:
            obj->channel = TSB_TB1;
            break;
        case PWM_2:
            obj->channel = TSB_TB2;
            break;
        case PWM_3:
            obj->channel = TSB_TB3;
            break;
        case PWM_4:
            obj->channel = TSB_TB4;
            break;
        case PWM_5:
            obj->channel = TSB_TB5;
            CG_SetFcPeriphA(CG_FC_PERIPH_PORTJ, ENABLE);
            break;
        case PWM_6:
            obj->channel = TSB_TB6;
            CG_SetFcPeriphA(CG_FC_PERIPH_PORTJ, ENABLE);
            break;
        default:
            obj->channel = NULL;
            return;
    }
    // Set pin function as PWM
    pinmap_pinout(pin, PinMap_PWM);
    obj->pin = pin;
    obj->period = DEFAULT_PERIOD;
    // Enable channel
    TMRB_Enable(obj->channel);
    // Stops and clear count operation
    TMRB_SetRunState(obj->channel, TMRB_STOP);
    // Disables double buffering
    TMRB_SetDoubleBuf(obj->channel, DISABLE);
    // Set default period = 20ms, duty cycle = 0
    obj->divisor = DEFAULT_CLOCK_DIVISION;
    counter = (uint16_t)((DEFAULT_PERIOD * CLOCK_FREQUENCY) / obj->divisor);

    // Init timer variable for using PPG mode
    m_tmrb.Mode = TMRB_INTERVAL_TIMER;
    m_tmrb.ClkDiv = TMRB_CLK_DIV_32;
    m_tmrb.UpCntCtrl = TMRB_AUTO_CLEAR;     // clear UC when matching value
    m_tmrb.TrailingTiming = counter;        // period = 20ms
    m_tmrb.LeadingTiming = counter;         // duty cycle = 0%

    // Init timer function
    TMRB_Init(obj->channel, &m_tmrb);
    obj->trailing_timing = counter;
    obj->leading_timing = counter;

    // Enable double buffering
    TMRB_SetDoubleBuf(obj->channel, ENABLE);

    // Setting to TBxFF0 reverse trigger
    FFStruct.FlipflopCtrl = TMRB_FLIPFLOP_CLEAR;
    FFStruct.FlipflopReverseTrg = TMRB_FLIPFLOP_MATCH_TRAILINGTIMING | TMRB_FLIPFLOP_MATCH_LEADINGTIMING;
    TMRB_SetFlipFlop(obj->channel, &FFStruct);

    // Start count operation
    TMRB_SetRunState(obj->channel, TMRB_RUN);
}

void pwmout_free(pwmout_t *obj)
{
    // Stops and clear count operation
    TMRB_SetRunState(obj->channel, TMRB_STOP);
    pwmout_write(obj,0);
    obj->pin = NC;
    obj->channel = NULL;
    obj->trailing_timing = 0;
    obj->leading_timing = 0;
    obj->divisor = 0;
}

void pwmout_write(pwmout_t *obj, float value)
{
    TMRB_FFOutputTypeDef FFStruct;

    // Stop timer for setting clock again
    TMRB_SetRunState(obj->channel, TMRB_STOP);
    // values outside this range will be saturated to 0.0f or 1.0f
    // Disable flip-flop reverse trigger when leading_timing and trailing_timing are duplicated
    if (value <= 0.0f) {
        value = 0;
        FFStruct.FlipflopCtrl = TMRB_FLIPFLOP_CLEAR;
        FFStruct.FlipflopReverseTrg = TMRB_DISABLE_FLIPFLOP;
        TMRB_SetFlipFlop(obj->channel, &FFStruct);
    } else if (value >= 1.0f) {
        value = 1;
        FFStruct.FlipflopCtrl = TMRB_FLIPFLOP_SET;
        FFStruct.FlipflopReverseTrg = TMRB_DISABLE_FLIPFLOP;
        TMRB_SetFlipFlop(obj->channel, &FFStruct);
    } else {
        FFStruct.FlipflopCtrl = TMRB_FLIPFLOP_CLEAR;
        FFStruct.FlipflopReverseTrg = TMRB_FLIPFLOP_MATCH_TRAILINGTIMING | TMRB_FLIPFLOP_MATCH_LEADINGTIMING;
        TMRB_SetFlipFlop(obj->channel, &FFStruct);
    }

    // Store the new leading_timing value
    obj->leading_timing = obj->trailing_timing - (uint16_t)(obj->trailing_timing * value);

    // Setting TBxRG0 register
    TMRB_ChangeLeadingTiming(obj->channel, obj->leading_timing);
    TMRB_SetRunState(obj->channel, TMRB_RUN);
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
    pwmout_period_us(obj, ms * 1000);
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t *obj, int us)
{
    float seconds = 0;
    int cycles = 0;
    uint32_t clkdiv = 0;
    float duty_cycle = 0;
    TMRB_InitTypeDef m_tmrb;
    seconds = (float)((us) / 1000000.0f);
    obj->period = seconds;

    MBED_ASSERT(obj->channel != NULL);

    // Select highest timer resolution
    for (int i = 0; i < 7; ++i) {
        cycles = (int)((CLOCK_FREQUENCY / prescale_tbl[i]) * seconds);
        if (cycles <= MAX_COUNTER_16B) {
            clkdiv = i + 1; // range 1:7
            break;
        }
    }
    // Stop timer for setting clock again
    TMRB_SetRunState(obj->channel, TMRB_STOP);
    // Restore the duty-cycle
    duty_cycle = (float)(obj->trailing_timing - obj->leading_timing) / obj->trailing_timing;
    obj->trailing_timing = cycles;
    obj->leading_timing = (cycles - (uint16_t)(cycles * duty_cycle));

    // Change the source clock division and period
    m_tmrb.Mode = TMRB_INTERVAL_TIMER;
    m_tmrb.ClkDiv = clkdiv;
    m_tmrb.UpCntCtrl = TMRB_AUTO_CLEAR;
    m_tmrb.TrailingTiming = obj->trailing_timing;
    m_tmrb.LeadingTiming = obj->leading_timing;

    //Init timer function
    TMRB_Init(obj->channel, &m_tmrb);
    //Start timer function
    TMRB_SetRunState(obj->channel, TMRB_RUN);
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
    float seconds = 0;
    float value = 0;

    MBED_ASSERT(obj->channel != NULL);

    seconds = (float)(us / 1000000.0f);
    value = (((seconds / obj->period) * 100.0f) / 100.0f);
    pwmout_write(obj, value);
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}
