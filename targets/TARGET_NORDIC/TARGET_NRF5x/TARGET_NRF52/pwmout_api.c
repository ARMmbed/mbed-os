/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA
 *      integrated circuit in a product or a software update for such product, must reproduce
 *      the above copyright notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be
 *      used to endorse or promote products derived from this software without specific prior
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse
 *      engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#if DEVICE_PWMOUT

#include "hal/pwmout_api.h"

#include "pinmap_ex.h"
#include "nrf_drv_pwm.h"

#if 0
#define DEBUG_PRINTF(...) do { printf(__VA_ARGS__); } while(0)
#else
#define DEBUG_PRINTF(...) {}
#endif

/* 0x7FFF is the max of COUNTERTOP pulse for the PWM peripherial of the nRF52. */
#define MAX_PWM_COUNTERTOP  (0x7FFF)

/* The PWM is driven by a 1 MHz clock to fit the 1 us resolution expected by the API. */
#define MAX_PWM_PERIOD_US   (MAX_PWM_COUNTERTOP)
#define MAX_PWM_PERIOD_MS   (MAX_PWM_PERIOD_US / 1000)
#define MAX_PWM_PERIOD_S    ((float) MAX_PWM_PERIOD_US / 1000000.0f)

/* Allocate PWM instances. */
static nrf_drv_pwm_t nordic_nrf5_pwm_instance[] = {
#if PWM0_ENABLED
    NRF_DRV_PWM_INSTANCE(0),
#endif
#if PWM1_ENABLED
    NRF_DRV_PWM_INSTANCE(1),
#endif
#if PWM2_ENABLED
    NRF_DRV_PWM_INSTANCE(2),
#endif
#if PWM3_ENABLED
    NRF_DRV_PWM_INSTANCE(3),
#endif
};

/* Helper function for (re)initializing the PWM instance.
 */
static void nordic_pwm_init(pwmout_t *obj)
{
    MBED_ASSERT(obj);

    /* Default configuration:
     * 1 pin per instance, otherwise they would share base count.
     * 1 MHz clock source to match the 1 us resolution.
     */
    nrf_drv_pwm_config_t config = {
        .output_pins  = {
            obj->pin,
            NRF_DRV_PWM_PIN_NOT_USED,
            NRF_DRV_PWM_PIN_NOT_USED,
            NRF_DRV_PWM_PIN_NOT_USED,
        },
        .irq_priority = PWM_DEFAULT_CONFIG_IRQ_PRIORITY,
        .base_clock   = NRF_PWM_CLK_1MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = obj->period,
        .load_mode    = NRF_PWM_LOAD_COMMON,
        .step_mode    = NRF_PWM_STEP_AUTO,
    };

    /* Make sure PWM instance is not running before making changes. */
    nrf_drv_pwm_uninit(&nordic_nrf5_pwm_instance[obj->instance]);

    /* Initialize instance with new configuration. */
    ret_code_t result = nrf_drv_pwm_init(&nordic_nrf5_pwm_instance[obj->instance],
                                         &config,
                                         NULL);

    MBED_ASSERT(result == NRF_SUCCESS);
}

/* Helper function for reinitializing the PWM instance and setting the duty-cycle. */
static void nordic_pwm_restart(pwmout_t *obj)
{
    MBED_ASSERT(obj);

    /* (Re)initialize PWM instance. */
    nordic_pwm_init(obj);

    /* Set duty-cycle from object. */
    ret_code_t result = nrf_drv_pwm_simple_playback(&nordic_nrf5_pwm_instance[obj->instance],
                                                    &obj->sequence,
                                                    1,
                                                    NRF_DRV_PWM_FLAG_LOOP);

    MBED_ASSERT(result == NRF_SUCCESS);
}

/** Initialize the pwm out peripheral and configure the pin
 *
 * Parameter obj The pwmout object to initialize
 * Parameter pin The pwmout pin to initialize
 */
void pwmout_init(pwmout_t *obj, PinName pin)
{
    DEBUG_PRINTF("pwmout_init: %d\r\n", pin);

    MBED_ASSERT(obj);

    /* Get hardware instance from pinmap. */
    int instance = pin_instance_pwm(pin);

    MBED_ASSERT(instance < (int)(sizeof(nordic_nrf5_pwm_instance) / sizeof(nrf_drv_pwm_t)));

    /* Populate PWM object with default values. */
    obj->instance = instance;
    obj->pin = pin;
    obj->pulse = 0;
    obj->period = MAX_PWM_COUNTERTOP;
    obj->percent = 0;
    obj->sequence.values.p_common = &obj->pulse;
    obj->sequence.length = NRF_PWM_VALUES_LENGTH(obj->pulse);
    obj->sequence.repeats = 0;
    obj->sequence.end_delay = 0;

    /* Initialize PWM instance. */
    nordic_pwm_init(obj);
}

/** Deinitialize the pwmout object
 *
 * Parameter obj The pwmout object
 */
void pwmout_free(pwmout_t *obj)
{
    DEBUG_PRINTF("pwmout_free\r\n");

    MBED_ASSERT(obj);

    /* Uninitialize PWM instance. */
    nrf_drv_pwm_uninit(&nordic_nrf5_pwm_instance[obj->instance]);
}

/** Set the output duty-cycle in range <0.0f, 1.0f>
 *
 * pulse 0.0f represents 0 percentage, 1.0f represents 100 percent.
 * Parameter obj     The pwmout object
 * Parameter percent The floating-point percentage number
 */
void pwmout_write(pwmout_t *obj, float percent)
{
    DEBUG_PRINTF("pwmout_write: %f\r\n", percent);

    /* Find counts based on period. */
    uint16_t pulse = obj->period * percent;

    /* Ensure we don't overcount. */
    obj->pulse = (pulse > MAX_PWM_COUNTERTOP) ? MAX_PWM_COUNTERTOP : pulse;

    /* Store actual percentage passed as parameter to avoid floating point rounding errors. */
    obj->percent = percent;

    /* Set new duty-cycle. */
    ret_code_t result = nrf_drv_pwm_simple_playback(&nordic_nrf5_pwm_instance[obj->instance],
                                                    &obj->sequence,
                                                    1,
                                                    NRF_DRV_PWM_FLAG_LOOP);

    MBED_ASSERT(result == NRF_SUCCESS);
}

/** Read the current float-point output duty-cycle
 *
 * Parameter obj The pwmout object
 * Return A floating-point output duty-cycle
 */
float pwmout_read(pwmout_t *obj)
{
    DEBUG_PRINTF("pwmout_read: %f\r\n", obj->percent);

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
    DEBUG_PRINTF("pwmout_period: %f\r\n", period);

    /* Cap period if too large. */
    if (period > MAX_PWM_PERIOD_S) {
        period = MAX_PWM_PERIOD_S;
    }

    /* Set new period. */
    pwmout_period_us(obj, period * 1000000);
}

/** Set the PWM period specified in miliseconds, keeping the duty cycle the same
 *
 * Parameter obj The pwmout object
 * Parameter ms  The milisecond period
 */
void pwmout_period_ms(pwmout_t *obj, int period)
{
    DEBUG_PRINTF("pwmout_period_ms: %d\r\n", period);

    /* Cap period if too large. */
    if (period > MAX_PWM_PERIOD_MS) {
        period = MAX_PWM_PERIOD_MS;
    }

    /* Set new period. */
    pwmout_period_us(obj, period * 1000);
}

/** Set the PWM period specified in microseconds, keeping the duty cycle the same
 *
 * Parameter obj The pwmout object
 * Parameter us  The microsecond period
 */
void pwmout_period_us(pwmout_t *obj, int period)
{
    DEBUG_PRINTF("pwmout_period_us: %d\r\n", period);

    /* Cap period if too large. */
    if (period > MAX_PWM_PERIOD_US) {
        period = MAX_PWM_PERIOD_US;
    }

    /* Scale new count based on stored duty-cycle and new period. */
    uint32_t pulse = (period * obj->pulse) / obj->period;

    /* Store new values in object. */
    obj->pulse = pulse;
    obj->period = period;
    obj->percent = (float) pulse / (float) period;

    /* Restart instance with new values. */
    nordic_pwm_restart(obj);
}

/** Set the PWM pulsewidth specified in seconds, keeping the period the same.
 *
 * Parameter obj     The pwmout object
 * Parameter seconds The floating-point pulsewidth in seconds
 */
void pwmout_pulsewidth(pwmout_t *obj, float pulse)
{
    DEBUG_PRINTF("pwmout_pulsewidt: %f\r\n", pulse);

    /* Cap pulsewidth to period before setting it. */
    if ((pulse * 1000000) > (float) obj->pulse) {
        obj->pulse = obj->period;
        pwmout_pulsewidth_us(obj, obj->pulse);
    } else {
        pwmout_pulsewidth_us(obj, pulse * 1000000);
    }
}

/** Set the PWM pulsewidth specified in miliseconds, keeping the period the same.
 *
 * Parameter obj The pwmout object
 * Parameter ms  The floating-point pulsewidth in miliseconds
 */
void pwmout_pulsewidth_ms(pwmout_t *obj, int pulse)
{
    DEBUG_PRINTF("pwmout_pulsewidth_ms: %d\r\n", ms);

    /* Cap pulsewidth to period before setting it. */
    if ((pulse * 1000) > (int) obj->period) {
        obj->pulse = obj->period;
        pwmout_pulsewidth_us(obj, obj->pulse);
    } else {
        pwmout_pulsewidth_us(obj, pulse * 1000);
    }
}

/** Set the PWM pulsewidth specified in microseconds, keeping the period the same.
 *
 * Parameter obj The pwmout object
 * Parameter us  The floating-point pulsewidth in microseconds
 */
void pwmout_pulsewidth_us(pwmout_t *obj, int pulse)
{
    DEBUG_PRINTF("pwmout_pulsewidth_us: %d\r\n", pulse);

    /* Cap pulsewidth to period. */
    if (pulse > obj->period) {
        pulse = obj->period;
    }

    /* Store new values in object. */
    obj->pulse = pulse;
    obj->percent = (float) pulse / (float) obj->period;

    /* Restart instance with new values. */
    nordic_pwm_restart(obj);
}

#endif // DEVICE_PWMOUT
