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

static int clockfreq;

void pwmout_init(pwmout_t* obj, PinName pin) {
    obj->channel = (PWMName) pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(obj->channel != (PWMName)NC);

    /* Enable clock for GPIO module */
    CMU_ClockEnable(cmuClock_GPIO, true);

    /* Enable clock for TIMER2 module */
    CMU_ClockEnable(cmuClock_TIMER2, true);

    switch(obj->channel){
        case PWM_CH0:
            /* Set TIMER2 CC0 location 1 pin (PA12) as output */
            GPIO_PinModeSet(gpioPortA, 12, gpioModePushPull, 0);
            break;
        case PWM_CH1:
            /* Set TIMER2 CC1 location 1 pin (PA13) as output */
            GPIO_PinModeSet(gpioPortA, 13, gpioModePushPull, 0);
            break;
        case PWM_CH2:
            /* Set TIMER2 CC2 location 1 pin (PA14) as output */
            GPIO_PinModeSet(gpioPortA, 14, gpioModePushPull, 0);
            break;

    }

    /* Start with default CC channel parameters */
    TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;

    /* Set mode to PWM */
    timerCCInit.mode = timerCCModePWM;

    /* Configure CC channel */
    TIMER_InitCC(TIMER2, obj->channel, &timerCCInit);

    /* Enable correct channel */
    switch(obj->channel){
        case PWM_CH0:
            TIMER2->ROUTE |= TIMER_ROUTE_CC0PEN;
            break;
        case PWM_CH1:
            TIMER2->ROUTE |= TIMER_ROUTE_CC1PEN;
            break;
        case PWM_CH2:
            TIMER2->ROUTE |= TIMER_ROUTE_CC2PEN;
            break;

    }
    /* Route correct channel to location 1 */
    TIMER2->ROUTE |= TIMER_ROUTE_LOCATION_LOC1;

    /* Select timer parameters */
    TIMER_Init_TypeDef timerInit =
    {
        .enable     = true,
        .debugRun   = false,
        .prescale   = timerPrescale64,
        .clkSel     = timerClkSelHFPerClk,
        .fallAction = timerInputActionNone,
        .riseAction = timerInputActionNone,
        .mode       = timerModeUp,
        .dmaClrAct  = false,
        .quadModeX4 = false,
        .oneShot    = false,
        .sync       = false,
    };

    clockfreq = (CMU_ClockFreqGet(cmuClock_HFPER)/64);

    /* Configure timer */
    TIMER_Init(TIMER2, &timerInit);

    /* Set default 2ms frequency and 0ms pulse width */
    pwmout_period(obj,0.002);


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
    obj->period_cycles = clockfreq*seconds;

    /* Set Top Value, which controls the PWM period */
    TIMER_TopSet(TIMER2, obj->period_cycles);
}

void pwmout_period_ms(pwmout_t* obj, int ms) {
    pwmout_period(obj, ms / 1000.0f);
}

void pwmout_period_us(pwmout_t* obj, int us) {
    pwmout_period_ms(obj, us / 1000.0f);
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    obj->width_cycles = clockfreq * seconds;
    TIMER_CompareBufSet(TIMER2, obj->channel, obj->width_cycles);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth(obj, ms / 1000.0f);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
    pwmout_pulsewidth_ms(obj, us / 1000.0f);
}
