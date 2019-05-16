/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#include "pinmap_function.h"

/* Prescaler values for TCC Module */
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

/* Max count limits of TCC Modules */
extern const uint32_t _tcc_maxs[TCC_INST_NUM];

/** Set the period of PWM object (will not update the waveform)
 *
 * @param[in] obj        The PWM object whose period is to be updated
 * @param[in] period_us  Period in microseconds
 * @return    void
 */
static void pwmout_set_period(pwmout_t* obj, int period_us)
{
    uint8_t i;
    uint32_t freq_hz;
    uint32_t div_freq;
    double us_per_cycle;
    uint64_t max_period = 0;
    uint32_t us_period = period_us;

    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* TCC instance index */
    uint8_t module_index = _tcc_get_inst_index(obj->tcc.hw);

    uint32_t count_max  = _tcc_maxs[module_index];

    freq_hz = system_gclk_gen_get_hz(obj->clock_source);

    for (i=0; i<sizeof(tcc_prescaler) / sizeof(tcc_prescaler[0]); i++) {
        div_freq = freq_hz >> tcc_prescaler[i];
        if (!div_freq) break;
        us_per_cycle = 1000000.00 / div_freq;
        max_period = us_per_cycle * count_max;
        if (max_period >= us_period) {
            obj->clock_prescaler = (enum tc_clock_prescaler)tcc_prescaler[i];
            obj->period = us_period / us_per_cycle;
            obj->us_per_cycle = us_per_cycle;
            break;
        }
    }
}

/** Initialize PWM Module with updated values
 *
 * @param[in][out] obj  The PWM object to initialize
 * @return         non-zero if success
 */
bool pwmout_init_hw(pwmout_t* obj)
{
    uint32_t mux_func = (uint32_t)NC;
    uint32_t pwm = (uint32_t)NC;
    PinName pin;
    uint32_t ch_index = (uint32_t)NC;
    struct tcc_config config_tcc;
    uint32_t tcc_channel = (uint32_t)NC;

    /* Sanity check arguments */
    MBED_ASSERT(obj);

    pin = obj->pin;
    pwm = pinmap_peripheral(pin, PinMap_PWM);
    if (pwm == (uint32_t)NC) return 0; /* Pin not supported */

    mux_func = pinmap_function(pin, PinMap_PWM);
    ch_index = pinmap_channel_pwm(pin, (PWMName) pwm);
    if ((mux_func == (uint32_t)NC) || (ch_index == (uint32_t)NC)) {
        /* Pin not supported */
        return 0;
    }
    if ((ch_index == 0) || (ch_index == 4)) {
        tcc_channel = 0;
    } else if ((ch_index == 1) || (ch_index == 5)) {
        tcc_channel = 1;
    } else if ((ch_index == 2) || (ch_index == 6)) {
        tcc_channel = 2;
    } else if ((ch_index == 3) || (ch_index == 7)) {
        tcc_channel = 3;
    }

    tcc_get_config_defaults(&config_tcc, (Tcc*)pwm);

    config_tcc.counter.clock_source = obj->clock_source;
    config_tcc.counter.clock_prescaler = (enum tcc_clock_prescaler)obj->clock_prescaler;

    config_tcc.counter.period = obj->period;
    config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
    config_tcc.compare.match[tcc_channel] = obj->period * obj->duty_cycle;

    config_tcc.pins.enable_wave_out_pin[ch_index] = true;
    config_tcc.pins.wave_out_pin[ch_index]        = pin;
    config_tcc.pins.wave_out_pin_mux[ch_index]    = mux_func;

    return (STATUS_OK == tcc_init(&obj->tcc, (Tcc*)pwm, &config_tcc));

}

/** Initialize PWM Module
 *
 * @param[in][out] obj  The PWM object to initialize
 * @return         void
 */
void pwmout_init(pwmout_t* obj, PinName pin)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    if ((uint32_t)NC == pinmap_peripheral(pin, PinMap_PWM)) {
        /* Pin not supported */
        return;
    }

    obj->pin = pin;
    obj->period = 0xFFFF;
    obj->duty_cycle = 1;
    obj->clock_source = GCLK_GENERATOR_0; /* 8Mhz input clock */
    obj->clock_prescaler = (enum tc_clock_prescaler)TCC_CLOCK_PRESCALER_DIV8; /* Default to 1MHz for 8Mhz input clock */

    /* Update the changes */
    if (pwmout_init_hw(obj)) {
        /* Enable PWM Module */
        tcc_enable(&obj->tcc);
    }

}

/** Free the PWM Module
 *
 * @param[in] obj  The PWM object to free
 * @return    void
 */
void pwmout_free(pwmout_t* obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    tcc_disable(&obj->tcc);
}

/** Set the duty cycle of PWM Waveform
 *
 * @param[in] obj    The PWM object
 * @param[in] value  New duty cycle to be set
 * @return    void
 */
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
    if (pwmout_init_hw(obj)) {
        /* Enable PWM Module */
        tcc_enable(&obj->tcc);
    }
}

/** Get the duty cycle of PWM Waveform
 *
 * @param[in] obj  The PWM object
 * @return    Current duty cycle
 */
float pwmout_read(pwmout_t* obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    return obj->duty_cycle;
}

/** Set the period of PWM Waveform
 *
 * @param[in] obj      The PWM object
 * @param[in] seconds  New period in seconds
 * @return           void
 */
void pwmout_period(pwmout_t* obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000000.0f);
}

/** Set the period of PWM Waveform
 *
 * @param[in] obj    The PWM object
 * @param[in] value  New period in milliseconds
 * @return           void
 */
void pwmout_period_ms(pwmout_t* obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

/** Set the period of PWM Waveform
 *
 * @param[in] obj  The PWM object
 * @param[in] us   New period in microseconds
 * @return    void
 */
void pwmout_period_us(pwmout_t* obj, int us)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Disable PWM Module */
    tcc_disable(&obj->tcc);

    /* TODO: Find and set the period */
    pwmout_set_period(obj, us);

    /* Update the changes */
    if (pwmout_init_hw(obj)) {
        /* Enable PWM Module */
        tcc_enable(&obj->tcc);
    }
}

/** Set the pulse width of PWM Waveform
 *
 * @param[in] obj      The PWM object
 * @param[in] seconds  New pulse width in seconds
 * @return    void
 */
void pwmout_pulsewidth(pwmout_t* obj, float seconds)
{
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

/** Set the pulse width of PWM Waveform
 *
 * @param[in] obj  The PWM object
 * @param[in] ms   New pulse width in milliseconds
 * @return    void
 */
void pwmout_pulsewidth_ms(pwmout_t* obj, int ms)
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

/** Set the pulse width of PWM Waveform
 *
 * @param[in] obj  The PWM object
 * @param[in] us   New pulse width in microseconds
 * @return    void
 */
void pwmout_pulsewidth_us(pwmout_t* obj, int us)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    uint32_t us_pulse = us;

    /* Find the new duty cycle */
    double duty_cycle = us_pulse / ((double)obj->period * obj->us_per_cycle);

    /* This call updates pulse width as well as period */
    pwmout_write(obj, duty_cycle);
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}
