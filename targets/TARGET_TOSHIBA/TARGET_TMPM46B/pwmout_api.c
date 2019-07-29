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
#include "PeripheralNames.h"
#include "pinmap.h"
#include "tmpm46b_tmrb.h"

#define MAX_COUNTER_16B         0xFFFF

static TMRB_InitTypeDef m_tmrb;
static TMRB_FFOutputTypeDef FFStruct;

static const PinMap PinMap_PWM[] = {
    {PE4, PWM_0, PIN_DATA(5, 1)},
    {PB6, PWM_1, PIN_DATA(4, 1)},
    {PH1, PWM_2, PIN_DATA(2, 1)},
    {PH0, PWM_3, PIN_DATA(2, 1)},
    {PK1, PWM_4, PIN_DATA(4, 1)},
    {PA7, PWM_5, PIN_DATA(5, 1)},
    {NC,  NC,    0}
};

static const uint32_t prescale_tbl[] = {
    2, 8, 32, 64, 128, 256, 512
};

#define CLOCK_FREQUENCY                         (SystemCoreClock)  // Input source clock

void pwmout_init(pwmout_t *obj, PinName pin)
{
    // Determine the pwm channel
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    //Assert input is valid
    MBED_ASSERT(pwm != (PWMName)NC);
    switch (pwm) {
        case PWM_0:
            obj->channel = TSB_TB2;
            break;
        case PWM_1:
            obj->channel = TSB_TB3;
            break;
        case PWM_2:
            obj->channel = TSB_TB4;
            break;
        case PWM_3:
            obj->channel = TSB_TB5;
            break;
        case PWM_4:
            obj->channel = TSB_TB6;
            break;
        case PWM_5:
            obj->channel = TSB_TB7;
            break;
        default:
            obj->channel = NULL;
            break;
    }
    CG_SetFcPeriphA((0x01U << (15U + pwm)), ENABLE);
    TMRB_SetIdleMode(TSB_TB0, DISABLE);
    // Set pin function as PWM
    pinmap_pinout(pin, PinMap_PWM);
    // Default to 20ms, 0% duty cycle
    pwmout_period_ms(obj, 20);
}

void pwmout_free(pwmout_t *obj)
{
    // Stops and clear count operation
    TMRB_SetRunState(obj->channel, TMRB_STOP);
    pwmout_write(obj,0);
    obj->channel = NULL;
    obj->trailing_timing = 0;
    obj->leading_timing = 0;
    obj->divisor = 0;
    TMRB_SetIdleMode(TSB_TB0, ENABLE);
}

void pwmout_write(pwmout_t *obj, float value)
{
    // Stop timer for setting clock again
    TMRB_SetRunState(obj->channel, TMRB_STOP);
    // values outside this range will be saturated to 0.0f or 1.0f
    // Disable flip-flop reverse trigger when leading_timing and trailing_timing are duplicated
    if (value <= 0.0f) {
        value = 0;
        FFStruct.FlipflopCtrl = TMRB_FLIPFLOP_CLEAR;
        FFStruct.FlipflopReverseTrg = TMRB_DISABLE_FLIPFLOP;
    } else if (value >= 1.0f) {
        value = 1;
        FFStruct.FlipflopCtrl = TMRB_FLIPFLOP_SET;
        FFStruct.FlipflopReverseTrg = TMRB_DISABLE_FLIPFLOP;
    } else {
        FFStruct.FlipflopCtrl = TMRB_FLIPFLOP_CLEAR;
        FFStruct.FlipflopReverseTrg = (TMRB_FLIPFLOP_MATCH_TRAILING | TMRB_FLIPFLOP_MATCH_LEADING);
    }
    TMRB_SetFlipFlop(obj->channel, &FFStruct);

    if (obj->period > 0.560) {
        value = 1; // TMPM46B duty cycle should be < 560ms, above 560ms fixed 50% duty cycle
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
    pwmout_period_us(obj, (ms * 1000));
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t *obj, int us)
{
    float seconds       = 0;
    uint32_t cycles     = 0;
    int ClkDiv          = 0;
    int i               = 0;
    float duty_cycle    = 0;
    uint32_t clk_freq   = 0;

    seconds = (float)((us) / 1000000.0f);
    obj->period = seconds;

    if (obj->period > 0.560) {
        clk_freq = (CLOCK_FREQUENCY / 2);
    } else {
        clk_freq = CLOCK_FREQUENCY;
    }
    // Select highest timer resolution
    for (i = 0; i < 7; ++i) {
        cycles = (int)((clk_freq / prescale_tbl[i]) * seconds);
        if (cycles <= MAX_COUNTER_16B) {
            ClkDiv = i + 1; // range 1:6
            break;
        } else {
            cycles = MAX_COUNTER_16B;
            ClkDiv = 7;
        }
    }
    // Stops and clear count operation
    TMRB_SetRunState(obj->channel, TMRB_STOP);
    // Restore the duty-cycle
    duty_cycle = (float)((obj->trailing_timing - obj->leading_timing) / obj->trailing_timing);
    obj->trailing_timing = cycles;
    obj->leading_timing = ((cycles)- (uint16_t)(cycles * duty_cycle));

    // Change the source clock division and period
    m_tmrb.Mode = TMRB_INTERVAL_TIMER;
    m_tmrb.ClkDiv = ClkDiv;
    m_tmrb.UpCntCtrl = TMRB_AUTO_CLEAR;
    m_tmrb.TrailingTiming = obj->trailing_timing;
    m_tmrb.LeadingTiming = obj->leading_timing;
    FFStruct.FlipflopCtrl = TMRB_FLIPFLOP_SET;
    FFStruct.FlipflopReverseTrg = (TMRB_FLIPFLOP_MATCH_TRAILING | TMRB_FLIPFLOP_MATCH_LEADING);
    // Enable channel
    TMRB_Enable(obj->channel);
    // Disable double buffering
    TMRB_SetDoubleBuf(obj->channel, DISABLE, TMRB_WRITE_REG_SEPARATE);
    // Init timer function
    TMRB_Init(obj->channel, &m_tmrb);
    // Enable double buffering
    TMRB_SetDoubleBuf(obj->channel, ENABLE, TMRB_WRITE_REG_SEPARATE);
    TMRB_SetFlipFlop(obj->channel, &FFStruct);
    // Start timer function
    TMRB_SetRunState(obj->channel, TMRB_RUN);
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
    float seconds   = 0;
    float value     = 0;

    seconds = (float)(us / 1000000.0f);
    value = (((seconds / obj->period) * 100.0f) / 100.0f);
    pwmout_write(obj, value);
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}
