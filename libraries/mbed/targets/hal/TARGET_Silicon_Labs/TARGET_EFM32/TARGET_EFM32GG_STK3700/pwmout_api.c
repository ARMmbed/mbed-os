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

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_timer.h"

#define PWM_TIMER TIMER2
#define PWM_TIMER_CLOCK cmuClock_TIMER2
#define PWM_ROUTE TIMER_ROUTE_LOCATION_LOC1


static int clockfreq;
static int prescaler_div;

void pwmout_init(pwmout_t* obj, PinName pin) {
    obj->channel = (PWMName) pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(obj->channel != (PWMName)NC);

    /* Enable clock for GPIO module */
    CMU_ClockEnable(cmuClock_GPIO, true);

    /* Enable clock for PWM_TIMER module */
    CMU_ClockEnable(PWM_TIMER_CLOCK, true);

    pin_mode(pin, PushPull);

    /* Start with default CC (Compare/Capture) channel parameters */
    TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;

    /* Set mode to PWM */
    timerCCInit.mode = timerCCModePWM;

    /* Configure CC channel */
    TIMER_InitCC(PWM_TIMER, obj->channel, &timerCCInit);

    /* Enable correct channel */
    switch(obj->channel){
        case PWM_CH0:
            PWM_TIMER->ROUTE |= TIMER_ROUTE_CC0PEN;
            break;
        case PWM_CH1:
            PWM_TIMER->ROUTE |= TIMER_ROUTE_CC1PEN;
            break;
        case PWM_CH2:
            PWM_TIMER->ROUTE |= TIMER_ROUTE_CC2PEN;
            break;

    }
    /* Route correct channel to location 1 */
    PWM_TIMER->ROUTE |= PWM_ROUTE;

    /* Select timer parameters */
    TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;

    /*HFPER is the default clock we will use. It has a frequency just north of 14MHz*/
    clockfreq = CMU_ClockFreqGet(cmuClock_HFPER);

    /* Configure timer */
    TIMER_Init(PWM_TIMER, &timerInit);

    /* Set default 2ms frequency and 0ms pulse width */
    pwmout_period(obj,0.02);
}

void pwmout_free(pwmout_t* obj) {}

void pwmout_write(pwmout_t* obj, float value) {
    if(value < 0.0f){
        value = 0;
    }else if (value > 1.0f){
        value = 1;
    }

    float pulse_period_in_s = obj->period_cycles / (float) clockfreq;
    pwmout_pulsewidth(obj, value * pulse_period_in_s);
}

float pwmout_read(pwmout_t* obj) {
    return obj->width_cycles / obj->period_cycles;
}

// Set the PWM period, keeping the absolute pulse width the same.
void pwmout_period(pwmout_t* obj, float seconds) {
    // Find the lowest prescaler divider possible.
    // This gives us max resolution for a given period
    
    //The value of the top register if prescaler is set to 0
    int cycles = clockfreq * seconds; 
    prescaler_div = 0;

    //The top register is only 16 bits, so we keep dividing till we are below 0xFFFF
    while(cycles > 0xFFFF){
        cycles /= 2;
        prescaler_div++;

        //Max prescaler_div supported is 10
        if(prescaler_div > 10){
            prescaler_div = 10;
            cycles = 0xFFFF; //Set it to max possible value;
            break; 
        }
    }



    obj->period_cycles = cycles;

    //Set prescaler
    PWM_TIMER->CTRL |= prescaler_div << _TIMER_CTRL_PRESC_SHIFT;
    PWM_TIMER->CTRL &= ~(~prescaler_div << _TIMER_CTRL_PRESC_SHIFT);

    /* Set Top Value, which controls the PWM period */
    TIMER_TopSet(PWM_TIMER, obj->period_cycles);
}

void pwmout_period_ms(pwmout_t* obj, int ms) {
    pwmout_period(obj, ms / 1000.0f);
}

void pwmout_period_us(pwmout_t* obj, int us) {
    pwmout_period_ms(obj, us / 1000.0f);
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    obj->width_cycles = clockfreq * seconds;
    TIMER_CompareBufSet(PWM_TIMER, obj->channel, obj->width_cycles);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth(obj, ms / 1000.0f);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
    pwmout_pulsewidth_ms(obj, us / 1000.0f);
}
