/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
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
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
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

#include "cmsis.h"
#include "pinmap.h"
#include "error.h"

static const PinMap PinMap_PWM[] = {
    // TIM2 full remap
    {PB_3,  PWM_2, STM_PIN_DATA(GPIO_Mode_AF_PP, 5)}, // TIM2fr_CH2 - ARDUINO D3
    // TIM3 partial remap
    {PB_4,  PWM_3, STM_PIN_DATA(GPIO_Mode_AF_PP, 7)}, // TIM3pr_CH1 - ARDUINO D5
    // TIM4 default
    {PB_6,  PWM_4, STM_PIN_DATA(GPIO_Mode_AF_PP, 0)}, // TIM4_CH1 - ARDUINO D10    
    {NC,    NC,    0}
};

void pwmout_init(pwmout_t* obj, PinName pin) {  
    // Get the peripheral name from the pin and assign it to the object
    obj->pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
  
    if (obj->pwm == (PWMName)NC) {
        error("PWM pinout mapping failed");
    }
    
    // Enable TIM clock
    if (obj->pwm == PWM_2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    if (obj->pwm == PWM_3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    if (obj->pwm == PWM_4) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    // Configure GPIO
    pinmap_pinout(pin, PinMap_PWM);
    
    obj->pin = pin;
    obj->period = 0;
    obj->pulse = 0;
    
    pwmout_period_us(obj, 20000); // 20 ms per default
}

void pwmout_free(pwmout_t* obj) {
    TIM_TypeDef *tim = (TIM_TypeDef *)(obj->pwm);
    TIM_DeInit(tim);
}

void pwmout_write(pwmout_t* obj, float value) {
    TIM_TypeDef *tim = (TIM_TypeDef *)(obj->pwm);
    TIM_OCInitTypeDef TIM_OCInitStructure;
  
    if (value < 0.0) {
        value = 0.0;
    } else if (value > 1.0) {
        value = 1.0;
    }
   
    obj->pulse = (uint32_t)((float)obj->period * value);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = obj->pulse;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // Configure channel 1
    if ((obj->pin == PB_4) || (obj->pin == PB_6)) {
        TIM_OC1PreloadConfig(tim, TIM_OCPreload_Enable);
        TIM_OC1Init(tim, &TIM_OCInitStructure);
    }

    // Configure channel 2
    if (obj->pin == PB_3) {
        TIM_OC2PreloadConfig(tim, TIM_OCPreload_Enable);
        TIM_OC2Init(tim, &TIM_OCInitStructure);
    }
}

float pwmout_read(pwmout_t* obj) {
    float value = 0;
    if (obj->period > 0) {
        value = (float)(obj->pulse) / (float)(obj->period);
    }
    return ((value > 1.0) ? (1.0) : (value));
}

void pwmout_period(pwmout_t* obj, float seconds) {
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t* obj, int ms) {
    pwmout_period_us(obj, ms * 1000);
}

void pwmout_period_us(pwmout_t* obj, int us) {
    TIM_TypeDef *tim = (TIM_TypeDef *)(obj->pwm);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    float dc = pwmout_read(obj);

    TIM_Cmd(tim, DISABLE);  
    
    obj->period = us;
  
    TIM_TimeBaseStructure.TIM_Period = obj->period - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(SystemCoreClock / 1000000) - 1; // 1 µs tick
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(tim, &TIM_TimeBaseStructure);

    // Set duty cycle again
    pwmout_write(obj, dc);
  
    TIM_ARRPreloadConfig(tim, ENABLE);    
    TIM_Cmd(tim, ENABLE);
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
    float value = (float)us / (float)obj->period;
    pwmout_write(obj, value);
}
