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

// TIM5 cannot be used because already used by the us_ticker
static const PinMap PinMap_PWM[] = {
  //{PA_0,  PWM_5,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM5)},  // TIM5_CH1
    {PA_1,  PWM_2,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM2)},  // TIM2_CH2
  //{PA_1,  PWM_5,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM5)},  // TIM5_CH1
    {PA_2,  PWM_2,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM2)},  // TIM2_CH3
  //{PA_2,  PWM_5,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM5)},  // TIM5_CH3
  //{PA_2,  PWM_9,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM9)},  // TIM9_CH1
    {PA_3,  PWM_2,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM2)},  // TIM2_CH4
  //{PA_3,  PWM_5,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM5)},  // TIM5_CH4
  //{PA_3,  PWM_9,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM9)},  // TIM9_CH2
    {PA_6,  PWM_3,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM3)},  // TIM3_CH1
  //{PA_6,  PWM_10, STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM10)}, // TIM10_CH1
    {PA_7,  PWM_3,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM3)},  // TIM3_CH2
  //{PA_7,  PWM_11, STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM11)}, // TIM11_CH1    
    {PB_0,  PWM_3,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM3)},  // TIM3_CH3
    {PB_1,  PWM_3,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM3)},  // TIM3_CH4
    {PB_3,  PWM_2,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM2)},  // TIM2_CH2
    {PB_4,  PWM_3,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM3)},  // TIM3_CH1
    {PB_5,  PWM_3,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM3)},  // TIM3_CH2
    {PB_6,  PWM_4,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM4)},  // TIM4_CH1
    {PB_7,  PWM_4,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM4)},  // TIM4_CH2
    {PB_8,  PWM_4,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM4)},  // TIM4_CH3
  //{PB_8,  PWM_10, STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM10)}, // TIM10_CH1
    {PB_9,  PWM_4,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM4)},  // TIM4_CH4
  //{PB_9,  PWM_11, STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM11)}, // TIM11_CH1
    {PB_10, PWM_2,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM2)},  // TIM2_CH3
    {PB_11, PWM_2,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM2)},  // TIM2_CH4
    {PB_12, PWM_10, STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM10)}, // TIM10_CH1
    {PB_13, PWM_9,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM9)},  // TIM9_CH1
    {PB_14, PWM_9,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM9)},  // TIM9_CH2
    {PB_15, PWM_11, STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM11)}, // TIM11_CH1    
    {PC_6,  PWM_3,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM3)},  // TIM3_CH1
    {PC_7,  PWM_3,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM3)},  // TIM3_CH2
    {PC_8,  PWM_3,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM3)},  // TIM3_CH3
    {PC_9,  PWM_3,  STM_PIN_DATA(GPIO_Mode_AF, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_TIM3)},  // TIM3_CH4    
    {NC,    NC,     0}
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
    if (obj->pwm == PWM_5) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    if (obj->pwm == PWM_9) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
    if (obj->pwm == PWM_10) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
    if (obj->pwm == PWM_11) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
    
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
    
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = obj->pulse;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;

    // Configure the channels
    switch (obj->pin) {
        // Channels 1
      //case PA_0:
      //case PA_1:
      //case PA_2:
        case PA_6:
      //case PA_7:
        case PB_4:
        case PB_6:
      //case PB_8:
      //case PB_9:
        case PB_12:
        case PB_13:
        case PB_15:
        case PC_6:
            TIM_OC1PreloadConfig(tim, TIM_OCPreload_Enable);
            TIM_OC1Init(tim, &TIM_OCInitStructure);
            break;
        // Channels 2
        case PA_1:
      //case PA_3:
        case PA_7:
        case PB_3:
        case PB_5:
        case PB_7:
        case PB_14:
        case PC_7:
            TIM_OC2PreloadConfig(tim, TIM_OCPreload_Enable);
            TIM_OC2Init(tim, &TIM_OCInitStructure);
            break;
        // Channels 3
        case PA_2:
        case PB_0:
        case PB_8:
        case PB_10:
        case PC_8:
            TIM_OC3PreloadConfig(tim, TIM_OCPreload_Enable);
            TIM_OC3Init(tim, &TIM_OCInitStructure);
            break;
        // Channels 4
        case PA_3:
        case PB_1:
        case PB_9:
        case PB_11:
        case PC_9:
            TIM_OC4PreloadConfig(tim, TIM_OCPreload_Enable);
            TIM_OC4Init(tim, &TIM_OCInitStructure);
            break;        
        default:
            return;
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
  
    TIM_TimeBaseStructure.TIM_Period        = obj->period - 1;
    TIM_TimeBaseStructure.TIM_Prescaler     = (uint16_t)(SystemCoreClock / 1000000) - 1; // 1 µs tick
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
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
