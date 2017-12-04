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
 
#ifndef TARGET_MCU_NRF51822
 
#include "mbed_assert.h"
#include "mbed_error.h"
#include "pwmout_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "sdk_config.h"

#if DEVICE_PWMOUT

#include "app_util_platform.h"
#include "nrf_drv_pwm.h"

#define MAX_PWM_COUNTERTOP  (0x7FFF)                 // 0x7FFF is the max of COUNTERTOP value for the PWM peripherial of the nRF52.
#define MAX_PWM_PERIOD_US   (MAX_PWM_COUNTERTOP * 8) // PWM hw is driven by 16 MHz clock, hence the tick is 1_us/16,
                                                     // and 128 is the max prescaler value.
#define MAX_PWM_PERIOD_MS   ((MAX_PWM_PERIOD_US / 1000) + 1)    // approximations advance
#define MAX_PWM_PERIOD_S    ((MAX_PWM_PERIOD_US / 1000000) + 1) // approximations advance


#define PWM_INSTANCE_COUNT  (PWM_COUNT) // import from the nrf_drv_config.h file

///> instances of nRF52 PWM driver
static const nrf_drv_pwm_t m_pwm_driver[PWM_INSTANCE_COUNT] =
{
#if PWM0_ENABLED
    NRF_DRV_PWM_INSTANCE(0),
#endif
#if PWM1_ENABLED
    NRF_DRV_PWM_INSTANCE(1),
#endif
#if PWM2_ENABLED
    NRF_DRV_PWM_INSTANCE(2)
#endif
};

typedef struct
{
    uint32_t period_us;
    uint32_t duty_us;
    float    duty;
} pwm_signal_t; /// PWM signal description type

typedef struct
{
    nrf_drv_pwm_t * p_pwm_driver;
    pwm_signal_t signal;
    volatile nrf_pwm_values_common_t seq_values[1];
} pwm_t; /// internal PWM instance support type

static pwm_t m_pwm[PWM_INSTANCE_COUNT] =
{
#if PWM0_ENABLED
    {.p_pwm_driver = NULL},
#endif
#if PWM1_ENABLED
    {.p_pwm_driver = NULL},
#endif
#if PWM2_ENABLED
    {.p_pwm_driver = NULL}
#endif
};  /// Array of internal PWM instances.

typedef struct
{
    uint16_t       period_hwu; // unit related to pwm_clk
    uint16_t       duty_hwu;   // unit related to pwm_clk
    nrf_pwm_clk_t  pwm_clk;
} pulsewidth_set_t; /// helper type for timing calculations
    
    
static void internal_pwmout_exe(pwmout_t *obj, bool new_period, bool initialization);
    
// extern PWM nIRQ handler implementations
void PWM0_IRQHandler(void);
void PWM1_IRQHandler(void);
void PWM2_IRQHandler(void);

static const peripheral_handler_desc_t pwm_handlers[PWM_INSTANCE_COUNT] =
{
    {
        PWM0_IRQn,
        (uint32_t)PWM0_IRQHandler
    },
    {
        PWM1_IRQn,
        (uint32_t)PWM1_IRQHandler
    },
    {
        PWM2_IRQn,
        (uint32_t)PWM2_IRQHandler
    }
};
 
void pwmout_init(pwmout_t *obj, PinName pin)
{
    uint32_t i;
    
    for (i = 0; PWM_INSTANCE_COUNT; i++)
    {
        if (m_pwm[i].p_pwm_driver == NULL) // a driver instance not assigned to the obj?
        {
            NVIC_SetVector(pwm_handlers[i].IRQn, pwm_handlers[i].vector);
            
            obj->pin         = pin;
            
            obj->pwm_channel = i;
            
            m_pwm[i].p_pwm_driver = (nrf_drv_pwm_t *) &m_pwm_driver[i];
            m_pwm[i].signal.period_us = 200000; // 0.02 s
            m_pwm[i].signal.duty_us   = 100000;
            m_pwm[i].signal.duty      = 0.5f;
            
            obj->pwm_struct  = &m_pwm[i];

            internal_pwmout_exe(obj, true, true);

            break;
        }
    }
    
    MBED_ASSERT(i != PWM_INSTANCE_COUNT); // assert if free instance was not found.
}

void pwmout_free(pwmout_t *obj)
{
    nrf_drv_pwm_uninit( (nrf_drv_pwm_t*) obj->pwm_struct );

    m_pwm[obj->pwm_channel].p_pwm_driver = NULL;
}

void pwmout_write(pwmout_t *obj, float percent)
{
    
    if (percent < 0)
    {
        percent = 0;
    }
    else if (percent > 1)
    {
        percent = 1;
    }
    
    pwm_signal_t * p_pwm_signal = &(((pwm_t*)obj->pwm_struct)->signal);
    
    p_pwm_signal->duty = percent;
    
    int us  = (((int)p_pwm_signal->period_us) * percent);
    
    pwmout_pulsewidth_us(obj, us);
}

float pwmout_read(pwmout_t *obj)
{
    pwm_signal_t * p_pwm_signal = &(((pwm_t*)obj->pwm_struct)->signal);
    
    return (float)p_pwm_signal->duty_us / (float)p_pwm_signal->period_us;
}

void pwmout_period(pwmout_t *obj, float seconds)
{
    // raught saturation < 0, quasi-max>
    if (seconds > MAX_PWM_PERIOD_S)
    {
        seconds = MAX_PWM_PERIOD_S;
    }
    else if (seconds < 0)
    {
        seconds = 0; // f. pwmout_period_us will set period to min. value
    }
    
    int us = seconds * 1000000;
    
    pwmout_period_us(obj, us);
}

void pwmout_period_ms(pwmout_t *obj, int ms)
{
    // reught saturation < 0, quasi-max>
    if (ms > MAX_PWM_PERIOD_MS)
    {
        ms = MAX_PWM_PERIOD_MS;
    }
    else if (ms < 0)
    {
        ms = 0; // f. pwmout_period_us will set period to min. value
    }
    
    int us = ms * 1000;
    
    pwmout_period_us(obj, us);
}


void pwmout_period_us(pwmout_t *obj, int us)
{
    pwm_signal_t * p_pwm_signal = &(((pwm_t*)obj->pwm_struct)->signal);
    
    // saturation <1, real-max>
    if (us > MAX_PWM_PERIOD_US)
    {
        us = MAX_PWM_PERIOD_US;
    }
    else if (us < 1)
    {
        us = 1;
    }
    
    p_pwm_signal->duty_us = (int)((float)us * p_pwm_signal->duty);
    
    p_pwm_signal->period_us  = us;
    
    internal_pwmout_exe(obj, true, false);
}

void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
    // raught saturation < 0, quasi-max>
    if (seconds > MAX_PWM_PERIOD_S)
    {
        seconds = MAX_PWM_PERIOD_S;
    }
    else if (seconds < 0)
    {
        seconds = 0;
    }
    
    int us = seconds * 1000000;
    
    pwmout_pulsewidth_us(obj,us);
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    // raught saturation < 0, quasi-max>
    if (ms > MAX_PWM_PERIOD_MS)
    {
        ms = MAX_PWM_PERIOD_MS;
    }
    else if (ms < 0)
    {
        ms = 0;
    }
    
    int us = ms * 1000;
    
    pwmout_pulsewidth_us(obj, us);
}

void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    // saturation <0, real-max>
    if (us > MAX_PWM_PERIOD_US)
    {
        us = MAX_PWM_PERIOD_US;
    }
    else if (us < 0)
    {
        us = 0;
    }
    
    pwm_signal_t * p_pwm_signal = &(((pwm_t*)obj->pwm_struct)->signal);
    
    p_pwm_signal->duty_us  = us;
    p_pwm_signal->duty     = us / p_pwm_signal->period_us;
    
    internal_pwmout_exe(obj, false, false);
}






static ret_code_t pulsewidth_us_set_get(int period_hwu, int duty_hwu, pulsewidth_set_t * p_settings)
{
    uint16_t      div;
    nrf_pwm_clk_t pwm_clk = NRF_PWM_CLK_16MHz;
    
    for(div = 1; div <= 128 ; div <<= 1) // 128 is the maximum of clock prescaler for PWM peripherial
    {
        if (MAX_PWM_COUNTERTOP >= period_hwu)
        {
            p_settings->period_hwu  = period_hwu; // unit [us/16 * div]
            p_settings->duty_hwu = duty_hwu;       // unit [us/16 * div]
            p_settings->pwm_clk   = pwm_clk;
    
            return NRF_SUCCESS;
        }
        
        period_hwu >>= 1;
        duty_hwu >>= 1;
        pwm_clk++;
    }
    
    return NRF_ERROR_INVALID_PARAM;
}


static void internal_pwmout_exe(pwmout_t *obj, bool new_period, bool initialization)
{
    pulsewidth_set_t          pulsewidth_set;
    pwm_signal_t            * p_pwm_signal;
    nrf_drv_pwm_t           * p_pwm_driver;
    ret_code_t                ret_code;
    
    p_pwm_signal = &(((pwm_t*)obj->pwm_struct)->signal);
    
    if (NRF_SUCCESS == pulsewidth_us_set_get(p_pwm_signal->period_us * 16, // base clk for PWM is 16 MHz
                                             p_pwm_signal->duty_us * 16,   // base clk for PWM is 16 MHz
                                             &pulsewidth_set))
    {
        p_pwm_driver = (((pwm_t*)obj->pwm_struct)->p_pwm_driver);
        
        const nrf_pwm_sequence_t seq =
        {
            .values.p_common = (nrf_pwm_values_common_t*) (((pwm_t*)obj->pwm_struct)->seq_values),
            .length          = 1,
            .repeats         = 0,
            .end_delay       = 0
        };
        
        (((pwm_t*)obj->pwm_struct)->seq_values)[0] = pulsewidth_set.duty_hwu | 0x8000;
        
        if (new_period)
        {
            nrf_drv_pwm_config_t config0 =
            {
                .output_pins =
                {
                    obj->pin | NRF_DRV_PWM_PIN_INVERTED, // channel 0
                    NRF_DRV_PWM_PIN_NOT_USED,            // channel 1
                    NRF_DRV_PWM_PIN_NOT_USED,            // channel 2
                    NRF_DRV_PWM_PIN_NOT_USED,            // channel 3
                },
                .irq_priority = PWM_DEFAULT_CONFIG_IRQ_PRIORITY,
                .base_clock   = pulsewidth_set.pwm_clk,
                .count_mode   = NRF_PWM_MODE_UP,
                .top_value    = pulsewidth_set.period_hwu,
                .load_mode    = NRF_PWM_LOAD_COMMON,
                .step_mode    = NRF_PWM_STEP_AUTO
            };
            
            if (!initialization)
            {
                nrf_drv_pwm_uninit(p_pwm_driver);
            }
        
            ret_code = nrf_drv_pwm_init( p_pwm_driver, &config0, NULL);
        
            MBED_ASSERT(ret_code == NRF_SUCCESS); // assert if free instance was not found.
        }

        nrf_drv_pwm_simple_playback(p_pwm_driver, &seq, 0, NRF_DRV_PWM_FLAG_LOOP);
    }
    else
    {
        MBED_ASSERT(0); // force assertion
    }
    
}

#endif // DEVICE_PWMOUT

#endif