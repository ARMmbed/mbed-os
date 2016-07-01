/* 
 * Copyright (c) 2013 Nordic Semiconductor ASA
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

#include "mbed_assert.h"
#include "mbed_error.h"
#include "pwmout_api.h"
#include "cmsis.h"
#include "pinmap.h"

#if DEVICE_PWMOUT

#include "app_util_platform.h"
#include "nrf_drv_pwm.h"

#define PWM_INSTANCE_COUNT  3

static nrf_drv_pwm_t m_pwm_driver[PWM_INSTANCE_COUNT] =
    {
        NRF_DRV_PWM_INSTANCE(0),
        NRF_DRV_PWM_INSTANCE(1),
        NRF_DRV_PWM_INSTANCE(2)
    };

typedef struct
{
    uint32_t period_us;
    uint32_t duty_us;
} pwm_signal_t;

typedef struct
{
    nrf_drv_pwm_t * p_pwm_driver;
    pwm_signal_t signal;
} pwm_t;

static pwm_t m_pwm[PWM_INSTANCE_COUNT] =
{
    {.p_pwm_driver = NULL},
    {.p_pwm_driver = NULL},
    {.p_pwm_driver = NULL}
};

typedef struct
{
    uint16_t period;
    uint16_t duty;
    nrf_pwm_clk_t  pwm_clk;
} pulsewidth_set_t;    
    
    
static void internal_pwmout_exe(pwmout_t *obj);
    
void pwmout_init(pwmout_t *obj, PinName pin)
{
    uint32_t i;
    
    for (i = 0; PWM_INSTANCE_COUNT; i++)
    {
        if (m_pwm[i].p_pwm_driver == NULL) // a driver instance not assigned to the obj?
        {
            obj->pin         = pin;
            /// @todo obj->pwm_name = 
            obj->pwm_channel = i;
            
            m_pwm[i].p_pwm_driver = &m_pwm_driver[i];
            m_pwm[i].signal.period_us = 200000; // 0.02 s
            m_pwm[i].signal.duty_us   = 100000;
            
            obj->pwm_struct  = &m_pwm[i];

            internal_pwmout_exe(obj);
            break;
        }
    }
    
    MBED_ASSERT(i != PWM_INSTANCE_COUNT); // assert if free instance was not found.
}

void pwmout_free(pwmout_t *obj)
{
    nrf_drv_pwm_uninit( (nrf_drv_pwm_t*) obj->pwm_struct );

    m_pwm[obj->pwm_channel].p_pwm_driver = NULL;
    /// @todo release gpio
}

void pwmout_write(pwmout_t *obj, float percent)
{
    
    if (percent < 0)
    {
        percent = 0;
    }
    else if (percent > 100)
    {
        percent = 100;
    }
    
    pwm_signal_t * p_pwm_signal = &(((pwm_t*)obj->pwm_struct)->signal);
    
    int us  = (((int)p_pwm_signal->period_us) * percent) / 100;
    
    pwmout_pulsewidth_us(obj, us);
}

float pwmout_read(pwmout_t *obj)
{
    pwm_signal_t * p_pwm_signal = &(((pwm_t*)obj->pwm_struct)->signal);
    
    return (float)p_pwm_signal->duty_us / (float)p_pwm_signal->period_us * 100;
}

void pwmout_period(pwmout_t *obj, float seconds)
{
    // @todo saturation
    int us = seconds * 1000000;
    
    pwmout_period_us(obj, us);
}

void pwmout_period_ms(pwmout_t *obj, int ms)
{
    int us = ms * 1000;
    
    pwmout_period_us(obj, us);
}

void pwmout_period_us(pwmout_t *obj, int us)
{
    pwm_signal_t * p_pwm_signal = &(((pwm_t*)obj->pwm_struct)->signal);
    
    p_pwm_signal->period_us  = us;
    
    internal_pwmout_exe(obj);
}

void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
    // @todo saturation
    int us = seconds * 1000000;
    
    pwmout_pulsewidth_us(obj,us);
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    // @todo saturation
    int us = ms * 1000;
    
    pwmout_pulsewidth_us(obj,us);
}

void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    // @todo saturation
    pwm_signal_t * p_pwm_signal = &(((pwm_t*)obj->pwm_struct)->signal);
    
    p_pwm_signal->duty_us  = us;
    
    internal_pwmout_exe(obj);
}






static ret_code_t pulsewidth_us_set_get(int period_us, int duty_us, pulsewidth_set_t * const p_settings)
{
    uint16_t div;
    nrf_pwm_clk_t pwm_clk = NRF_PWM_CLK_16MHz;
    
    for(div = 1; div <= 128 ; div <<= 1) 
    {
        if (0xFFFF >= period_us)
        {
            p_settings->period  = period_us; // unit [us * div]
            p_settings->duty = duty_us;       // unit [us * div]
            p_settings->pwm_clk   = pwm_clk;
    
            return NRF_SUCCESS;
        }
        
        period_us >>= 1;
        duty_us >>= 1;
        pwm_clk++;
    }
    
    return NRF_ERROR_INVALID_PARAM;
}

        static nrf_pwm_values_common_t seq_values[1];
        
        static nrf_pwm_sequence_t const seq =
        {
            .values.p_common = seq_values,
            .length          = NRF_PWM_VALUES_LENGTH(seq_values),
            .repeats         = 0,
            .end_delay       = 0
        };

static void internal_pwmout_exe(pwmout_t *obj)
{
    pulsewidth_set_t pulsewidth_set;
    pwm_signal_t   * p_pwm_signal;
    nrf_drv_pwm_t *p_pwm_driver;
    ret_code_t ret_code;
    
    p_pwm_signal = &(((pwm_t*)obj->pwm_struct)->signal);
    
    if (NRF_SUCCESS == pulsewidth_us_set_get(p_pwm_signal->period_us,
                                             p_pwm_signal->duty_us,
                                             &pulsewidth_set))
    {
        //@todo apply pulsewidth_set
        p_pwm_driver = (((pwm_t*)obj->pwm_struct)->p_pwm_driver);
        
        nrf_drv_pwm_config_t config0 =
        {
            .output_pins =
            {
                obj->pin,                 // channel 0
                NRF_DRV_PWM_PIN_NOT_USED, // channel 1
                NRF_DRV_PWM_PIN_NOT_USED, // channel 2
                NRF_DRV_PWM_PIN_NOT_USED, // channel 3
            },
            .irq_priority = APP_IRQ_PRIORITY_LOW,
            .base_clock   = pulsewidth_set.pwm_clk,
            .count_mode   = NRF_PWM_MODE_UP,
            .top_value    = pulsewidth_set.period,
            .load_mode    = NRF_PWM_LOAD_COMMON,
            .step_mode    = NRF_PWM_STEP_AUTO
        };
        
        
        //printf("clock = %d, top = %d\r\n", pulsewidth_set.pwm_clk, pulsewidth_set.period);
        
        nrf_drv_pwm_uninit(p_pwm_driver);
        
        ret_code = nrf_drv_pwm_init( p_pwm_driver, &config0, NULL);
        
        MBED_ASSERT(ret_code == NRF_SUCCESS); // assert if free instance was not found.
        
        seq_values[0] = pulsewidth_set.duty;
        
        nrf_drv_pwm_simple_playback(p_pwm_driver, &seq, 3, NRF_DRV_PWM_FLAG_LOOP);

        
    }
    else
    {
        MBED_ASSERT(0); // force assertion
    }
    
}

#endif // DEVICE_PWMOUT
