/* mbed Microcontroller Library
 * Copyright (c) 2024, Arm Limited and affiliates.
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


#if DEVICE_PWMOUT

#include "hal/pwmout_api.h"
#include "PeripheralPins.h"
#include "pinmap.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "mbed_assert.h"

#include <math.h>

// Change to 1 to enable debug prints of what's being calculated.
// Must comment out the critical section calls in PwmOut to use.
#define RP2040_PWMOUT_DEBUG 0

#if RP2040_PWMOUT_DEBUG
#include <stdio.h>
#include <inttypes.h>
#endif

/// Largest top count value supported by hardware.  Using this value will provide the highest duty cycle resolution,
/// but will limit the period to a maximum of (1 / (125 MHz / (65534 + 1)) =) 524 us
const uint16_t MAX_TOP_COUNT = 65534;

/// Value for PWM_CHn_DIV register that produces a division of 1
const uint16_t PWM_CHn_DIV_1 = 0x010;

/// Calculate the effective PWM period (in floating point seconds) based on a divider and top_count value
static float calc_effective_pwm_period(float divider, uint16_t top_count)
{
    // Note: The hardware counts to top_count *inclusively*, so we have to add 1
    // to get the number of clock cycles that a given top_count value will produce
    return 1.0f / ((clock_get_hz(clk_sys) / divider) / (top_count + 1));
}

/// Calculate the best possible top_count value (rounding up) for a divider and a desired pwm period
static uint16_t calc_top_count_for_period(float divider, float desired_pwm_period)
{
    // Derivation:
    // desired_pwm_period = 1.0f / ((clock_get_hz(clk_sys) / divider) / (top_count + 1))
    // desired_pwm_period = (top_count + 1) / (clock_get_hz(clk_sys) / divider)
    // desired_pwm_period * (clock_get_hz(clk_sys) / divider) - 1 = top_count

    long top_count_float = lroundf(desired_pwm_period * (clock_get_hz(clk_sys) / divider) - 1);
    MBED_ASSERT(top_count_float <= MAX_TOP_COUNT);
    return (uint16_t)top_count_float;
}

/// Calculate the best possible floating point divider value for a desired pwm period.
/// This function assumes that top_count is set to MAX_TOP_COUNT.
static float calc_divider_for_period(float desired_pwm_period)
{
    // Derivation:
    // (desired_pwm_period * clock_get_hz(clk_sys)) / divider - 1 = top_count
    // (desired_pwm_period * clock_get_hz(clk_sys)) / divider = top_count + 1
    // divider = (desired_pwm_period * clock_get_hz(clk_sys)) / (top_count + 1)

    return (desired_pwm_period * clock_get_hz(clk_sys)) / (MAX_TOP_COUNT + 1);
}

/// Convert PWM divider from floating point to a fixed point number (rounding up).
/// The divider is returned as an 8.4 bit fixed point number, which is what the Pico registers use.
static uint16_t pwm_divider_float_to_fixed(float divider_float)
{
    // To convert to a fixed point number, multiply by 16 and then round up
    uint16_t divider_exact = ceil(divider_float * 16);

    // Largest supported divider is 255 and 15/16
    if(divider_exact > 0xFFF)
    {
        divider_exact = 0xFFF;
    }
    return divider_exact;
}

/// Convert PWM divider from the fixed point hardware value (8.4 bits) to a float.
static float pwm_divider_fixed_to_float(uint16_t divider_fixed)
{
    return divider_fixed / 16.0f;
}

/** Initialize the pwm out peripheral and configure the pin
 *
 * Parameter obj The pwmout object to initialize
 * Parameter pin The pwmout pin to initialize
 */
void pwmout_init(pwmout_t *obj, PinName pin)
{
    MBED_ASSERT(obj != NULL);

    /* Populate PWM object with default values. */
    obj->slice = pwm_gpio_to_slice_num(pin);
    obj->channel = pwm_gpio_to_channel(pin);
    obj->pin = pin;
    obj->top_count = MAX_TOP_COUNT;
    obj->percent = 0.5f;
    obj->cfg = pwm_get_default_config();
    pwm_config_set_wrap(&(obj->cfg), obj->top_count);

    pwm_init(obj->slice, &(obj->cfg), false);
    gpio_set_function(pin, GPIO_FUNC_PWM);
}

/** Deinitialize the pwmout object
 *
 * Parameter obj The pwmout object
 */
void pwmout_free(pwmout_t *obj)
{
    MBED_ASSERT(obj != NULL);
    pwm_set_enabled(obj->slice, false);
}

/** Set the output duty-cycle in range <0.0f, 1.0f>
 *
 * pulse 0.0f represents 0 percentage, 1.0f represents 100 percent.
 * Parameter obj     The pwmout object
 * Parameter percent The floating-point percentage number
 */
void pwmout_write(pwmout_t *obj, float percent)
{
    obj->percent = percent;

    // Per datasheet section 4.5.2.2, a period value of top_count + 1 produces 100% duty cycle
    int32_t new_reset_counts = lroundf((obj->top_count + 1) * percent);

    // Clamp to valid values
    if(new_reset_counts > obj->top_count + 1)
    {
        new_reset_counts = obj->top_count + 1;
    }
    else if(new_reset_counts < 0)
    {
        new_reset_counts = 0;
    }

#if RP2040_PWMOUT_DEBUG
    printf("new_reset_counts: %" PRIu32 "\n", new_reset_counts);
#endif

    pwm_set_chan_level(obj->slice, obj->channel, new_reset_counts);
    pwm_set_enabled(obj->slice, true);
}

/** Read the current float-point output duty-cycle
 *
 * Parameter obj The pwmout object
 * Return A floating-point output duty-cycle
 */
float pwmout_read(pwmout_t *obj)
{
    /* Return percentage stored in object instead of calculating the value.
     * This prevents floating point rounding errors.
     */
    return obj->percent;
}

/** Set the PWM period specified in seconds, keeping the duty cycle the same
 *
 * Periods smaller than microseconds (the lowest resolution) are set to zero.
 * Parameter obj     The pwmout object
 * Parameter seconds The floating-point seconds period
 */
void pwmout_period(pwmout_t *obj, float period)
{
    // Two possibilities here:
    // - If the period is relatively short (< about 524 us), we want to keep the clock divider at 1
    //    and reduce top_count to match the period
    // - If the period is larger than what we can achieve with a clock divider of 1, we need to
    //    use a higher clock divider, then recalculate the top_count to match

    // Note: For math this complex, I wasn't able to avoid using floating point values.
    // This function won't be too efficient, but for now I just want something that works and
    // can access the full PWM range.

    if(period <= calc_effective_pwm_period(1, MAX_TOP_COUNT))
    {
        // Short period.  Leave divider at 1 and reduce top_count to match the expected period
        obj->clock_divider = 1.0f;
        obj->cfg.div = PWM_CHn_DIV_1;
        obj->top_count = calc_top_count_for_period(obj->clock_divider, period);
    }
    else
    {
        // Long period, need to use divider.

        // Step 1: Calculate exact desired divider such that top_count would equal MAX_TOP_COUNT
        float desired_divider = calc_divider_for_period(period);

        // Step 2: Round desired divider upwards to the next value the hardware can do.
        // We go upwards so that the top_count value can be trimmed downwards for the best period accuracy.
        uint16_t divider_fixed_point = pwm_divider_float_to_fixed(desired_divider);
        obj->cfg.div = divider_fixed_point;

        // Step 3: Get the divider we'll actually be using as a float
        obj->clock_divider = pwm_divider_fixed_to_float(divider_fixed_point);

        // Step 4: For best accuracy, recalculate the top_count value using the divider.
        obj->top_count = calc_top_count_for_period(obj->clock_divider, period);

#if RP2040_PWMOUT_DEBUG
        printf("period = %f, desired_divider = %f\n",
               period,
               desired_divider);
#endif
    }

    // Save period for later
    obj->period = period;

#if RP2040_PWMOUT_DEBUG
    printf("obj->clock_divider = %f, obj->cfg.div = %" PRIu32 ", obj->top_count = %" PRIu16 "\n",
           obj->clock_divider,
           obj->cfg.div,
           obj->top_count);
#endif

    // Set the new divider and top_count values.
    pwm_config_set_wrap(&(obj->cfg), obj->top_count);
    pwm_init(obj->slice, &(obj->cfg), false);
}

/** Set the PWM period specified in miliseconds, keeping the duty cycle the same
 *
 * Parameter obj The pwmout object
 * Parameter ms  The milisecond period
 */
void pwmout_period_ms(pwmout_t *obj, int period)
{
    /* Set new period. */
    pwmout_period(obj, period / 1000.0f);
}

/** Set the PWM period specified in microseconds, keeping the duty cycle the same
 *
 * Parameter obj The pwmout object
 * Parameter us  The microsecond period
 */
void pwmout_period_us(pwmout_t *obj, int period)
{
    /* Set new period. */
    pwmout_period(obj, period / 1000000.0f);
}

/** Read the PWM period specified in microseconds
 *
 * @param obj The pwmout object
 * @return A int output period
 */
int pwmout_read_period_us(pwmout_t *obj)
{
    return lroundf(1000000 * calc_effective_pwm_period(obj->clock_divider, obj->top_count));
}

/** Set the PWM pulsewidth specified in seconds, keeping the period the same.
 *
 * Parameter obj     The pwmout object
 * Parameter seconds The floating-point pulsewidth in seconds
 */
void pwmout_pulsewidth(pwmout_t *obj, float pulse)
{
    pwmout_write(obj, pulse / obj->period);
}

/** Set the PWM pulsewidth specified in miliseconds, keeping the period the same.
 *
 * Parameter obj The pwmout object
 * Parameter ms  The floating-point pulsewidth in miliseconds
 */
void pwmout_pulsewidth_ms(pwmout_t *obj, int pulse)
{
    pwmout_write(obj, (pulse * .001f) / obj->period);
}

/** Set the PWM pulsewidth specified in microseconds, keeping the period the same.
 *
 * Parameter obj The pwmout object
 * Parameter us  The floating-point pulsewidth in microseconds
 */
void pwmout_pulsewidth_us(pwmout_t *obj, int pulse)
{
    pwmout_write(obj, (pulse * .000001f) / obj->period);
}

int pwmout_read_pulsewidth_us(pwmout_t *obj) {
    return lroundf(obj->period * obj->percent * 1000000);
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM_OUT;
}

#endif // DEVICE_PWMOUT
