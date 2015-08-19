/* mbed Microcontroller Library 
 *******************************************************************************
 * Copyright (c) 2015 WIZnet Co.,Ltd. All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of ARM Limited nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#include "pwmout_api.h"

#if DEVICE_PWMOUT

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"
#include "W7500x_pwm.h"

static PWM_TimerModeInitTypeDef TimerModeStructure;

void pwmout_init(pwmout_t* obj, PinName pin)
{
    // Get the peripheral name from the pin and assign it to the object
    obj->PWM_CHx = (PWM_CHn_TypeDef *)pinmap_peripheral(pin, PinMap_PWM);

    if (obj->PWM_CHx == (PWM_CHn_TypeDef *)NC) {
        error("PWM error: pinout mapping failed.");
    }

    // Configure GPIO
    pinmap_pinout(pin, PinMap_PWM);
    
    GetSystemClock();

    obj->pin = pin;
    
    pwmout_period_us(obj, 20000); // 20 ms per default
}

void pwmout_free(pwmout_t* obj)
{
    // Configure GPIO
    pin_function(obj->pin, WIZ_PIN_DATA(WIZ_MODE_AF, WIZ_GPIO_NOPULL, Px_AFSR_AF0));
}

void pwmout_write(pwmout_t* obj, float value)
{
    if (value < (float)0.0) {
        value = 0.0;
    } else if (value > (float)1.0) {
        value = 1.0;
    }

    obj->pulse = (uint32_t)((float)obj->period * value);
    
    PWM_CHn_Stop(obj->PWM_CHx);

    TimerModeStructure.PWM_CHn_PR = obj->PrescalerValue - 1;
    TimerModeStructure.PWM_CHn_MR = obj->pulse;
    TimerModeStructure.PWM_CHn_LR = obj->period; 
    TimerModeStructure.PWM_CHn_UDMR = PWM_CHn_UDMR_UpCount;
    TimerModeStructure.PWM_CHn_PDMR = PWM_CHn_PDMR_Periodic;
    
    PWM_TimerModeInit(obj->PWM_CHx, &TimerModeStructure);
    
    PWM_CHn_Start(obj->PWM_CHx);
}

float pwmout_read(pwmout_t* obj)
{
    float value = 0;
    if (obj->period > 0) {
        value = (float)(obj->pulse) / (float)(obj->period);
    }
    return ((value > (float)1.0) ? (float)(1.0) : (value));
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
    PWM_CHn_Stop(obj->PWM_CHx);
    // Update the SystemCoreClock variable
    SystemCoreClockUpdate();

    obj->period = (us * 2) - 1;
    obj->pulse = us / 2;
    
    obj->PrescalerValue = (SystemCoreClock / 1000000) / 2;
    TimerModeStructure.PWM_CHn_PR = obj->PrescalerValue - 1;
    TimerModeStructure.PWM_CHn_MR = obj->pulse;
    TimerModeStructure.PWM_CHn_LR = obj->period; 
    TimerModeStructure.PWM_CHn_UDMR = PWM_CHn_UDMR_UpCount;
    TimerModeStructure.PWM_CHn_PDMR = PWM_CHn_PDMR_Periodic;
    
    PWM_TimerModeInit(obj->PWM_CHx, &TimerModeStructure);
    PWM_CtrlPWMOutputEnable(obj->PWM_CHx);
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
    float value = (float)(2 * us) / (float)obj->period;
    pwmout_write(obj, value);
}

#endif
