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

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#include "pinmap_function.h"


#define PWMOUT_CTRL_CHANNEL     3

const uint32_t tcc_prescaler[] = {
    TCC_CLOCK_PRESCALER_DIV1,
    TCC_CLOCK_PRESCALER_DIV2,
    TCC_CLOCK_PRESCALER_DIV4,
    TCC_CLOCK_PRESCALER_DIV8,
    TCC_CLOCK_PRESCALER_DIV16,
    TCC_CLOCK_PRESCALER_DIV64,
    TCC_CLOCK_PRESCALER_DIV256,
    TCC_CLOCK_PRESCALER_DIV1024
};

extern const uint32_t _tcc_maxs[TCC_INST_NUM];

static void pwmout_set_period(pwmout_t* obj, int period_us)
{
    uint32_t i;
    uint32_t freq_hz;
    double us_per_cycle;
    uint64_t max_period = 0;
    uint32_t us_period = period_us;

    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* TCC instance index */
    uint8_t module_index = _tcc_get_inst_index(obj->tcc.hw);

    uint32_t count_max  = _tcc_maxs[module_index];

    freq_hz = system_gclk_gen_get_hz(obj->clock_source);

    for (i=0; i<sizeof(tcc_prescaler); i++) {
        us_per_cycle = 1000000.00 / (freq_hz >> tcc_prescaler[i]);
        max_period = us_per_cycle * count_max;
        if (max_period >= us_period) {
            obj->clock_prescaler = tcc_prescaler[i];
            obj->period = us_period / us_per_cycle;
            break;
        }
    }
}

void pwmout_init_hw(pwmout_t* obj)
{
    uint32_t mux_func = NC;
    uint32_t pwm = NC;
    PinName pin;
    uint32_t ch_index = NC;
    struct tcc_config config_tcc;

    /* Sanity check arguments */
    MBED_ASSERT(obj);

    pin = obj->pin;
    pwm = pinmap_peripheral(pin, PinMap_PWM);
    if (pwm == (uint32_t)NC) return; /* Pin not supported */

    mux_func = pinmap_function(pin, PinMap_PWM);
    ch_index = pinmap_channel_pwm(pin, pwm);
    if ((mux_func == (uint32_t)NC) || (ch_index == (uint32_t)NC)) {
        /* Pin not supported */
        return;
    }

    tcc_get_config_defaults(&config_tcc, (Tcc*)pwm);

    config_tcc.counter.clock_source = obj->clock_source;
    config_tcc.counter.clock_prescaler = obj->clock_prescaler;

    config_tcc.counter.period = obj->period;
    config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
    config_tcc.compare.match[PWMOUT_CTRL_CHANNEL] = obj->period * obj->duty_cycle;

    config_tcc.pins.enable_wave_out_pin[ch_index] = true;
    config_tcc.pins.wave_out_pin[ch_index]        = pin;
    config_tcc.pins.wave_out_pin_mux[ch_index]    = mux_func;

    tcc_init(&obj->tcc, (Tcc*)pwm, &config_tcc);

}

void pwmout_init(pwmout_t* obj, PinName pin)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    obj->pin = pin;
    obj->period = 0xFFFF;
    obj->duty_cycle = 1;
    obj->clock_source = GCLK_GENERATOR_0; /* 8Mhz input clock */
    obj->clock_prescaler = TCC_CLOCK_PRESCALER_DIV8; /* Default to 1MHz for 8Mhz input clock */

    pwmout_init_hw(obj);

    tcc_enable(&obj->tcc);
}

void pwmout_free(pwmout_t* obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    tcc_disable(&obj->tcc);
}

void pwmout_write(pwmout_t* obj, float value)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    if (value < 0.0f) {
        value = 0;
    } else if (value > 1.0f) {
        value = 1;
    }

    /* Modify the pulse width keeping period same */
    obj->duty_cycle = value;

    /* Disable PWM Module */
    tcc_disable(&obj->tcc);

    /* Update the changes */
    pwmout_init_hw(obj);

    /* Enable PWM Module */
    tcc_enable(&obj->tcc);

}

float pwmout_read(pwmout_t* obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    return obj->duty_cycle;
}

void pwmout_period(pwmout_t* obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t* obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

void pwmout_period_us(pwmout_t* obj, int us)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Disable PWM Module */
    tcc_disable(&obj->tcc);

    /* TODO: Find and set the period */
    pwmout_set_period(obj, us);

    /* Update the changes */
    pwmout_init_hw(obj);

    /* Enable PWM Module */
    tcc_enable(&obj->tcc);
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
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Find the new duty cycle */
    double duty_cycle = us / (double)obj->period;

    /* This call updates pulse width as well as period */
    pwmout_write(obj, duty_cycle);
}
