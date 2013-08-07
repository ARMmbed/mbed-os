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
#include "pwmout_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "error.h"

#define TCR_CNT_EN       0x00000001
#define TCR_RESET        0x00000002

/* To have a PWM where we can change both the period and the duty cycle,
 * we need an entire timer. With the following conventions:
 *   * MR3 is used for the PWM period
 *   * MR0, MR1, MR2 are used for the duty cycle
 */
static const PinMap PinMap_PWM[] = {
    /* CT16B0 */
    {P0_8 , PWM_1, 0x02},   /* MR0 */
    {P0_9 , PWM_2, 0x02},   /* MR1 */
    {P0_10, PWM_3, 0x03},   /* MR2 */

    /* CT16B1 */
    {P1_9 , PWM_4, 0x01},   /* MR0 */
    {P1_10, PWM_5, 0x02},   /* MR1 */

    /* CT32B0 */
    {P1_6 , PWM_6, 0x02},   /* MR0 */
    {P1_7 , PWM_7, 0x02},   /* MR1 */
    {P0_1 , PWM_8, 0x02},   /* MR2 */

    /* CT32B1 */
    {P1_1 , PWM_9 ,0x03},   /* MR0 */
    {P1_2 , PWM_10,0x03},   /* MR1 */
    {P1_3 , PWM_11,0x03},   /* MR2 */

    {NC   , NC    ,0x00}
};

typedef struct {
    uint8_t timer;
    uint8_t mr;
} timer_mr;

static timer_mr pwm_timer_map[11] = {
    {0, 0}, /* CT16B0, MR0 */
    {0, 1}, /* CT16B0, MR1 */
    {0, 2}, /* CT16B0, MR2 */

    {1, 0}, /* CT16B1, MR0 */
    {1, 1}, /* CT16B1, MR1 */

    {2, 0}, /* CT32B0, MR0 */
    {2, 1}, /* CT32B0, MR1 */
    {2, 2}, /* CT32B0, MR2 */

    {3, 0}, /* CT32B1, MR0 */
    {3, 1}, /* CT32B1, MR1 */
    {3, 2}  /* CT32B1, MR2 */
};

static LPC_TMR_TypeDef *Timers[4] = {
    LPC_TMR16B0, LPC_TMR16B1,
    LPC_TMR32B0, LPC_TMR32B1
};

static unsigned int pwm_clock_mhz;

void pwmout_init(pwmout_t* obj, PinName pin) {
    // determine the channel
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    if (pwm == (uint32_t)NC)
        error("PwmOut pin mapping failed");
    
    obj->pwm = pwm;
    
    // Timer registers
    timer_mr tid = pwm_timer_map[pwm];
    LPC_TMR_TypeDef *timer = Timers[tid.timer];
    
    // Disable timer
    timer->TCR = 0;
    
    // Power the correspondent timer
    LPC_SYSCON->SYSAHBCLKCTRL |= 1 << (tid.timer + 7);
    
    /* Enable PWM function */
    timer->PWMC = (1 << 3)|(1 << 2)|(1 << 1)|(1 << 0);
    
    /* Reset Functionality on MR3 controlling the PWM period */
    timer->MCR = 1 << 10;
    
    pwm_clock_mhz = SystemCoreClock / 1000000;
    
    // default to 20ms: standard for servos, and fine for e.g. brightness control
    pwmout_period_ms(obj, 20);
    pwmout_write    (obj, 0);
    
    // Wire pinout
    pinmap_pinout(pin, PinMap_PWM);
}

void pwmout_free(pwmout_t* obj) {
    // [TODO]
}

void pwmout_write(pwmout_t* obj, float value) {
    if (value < 0.0f) {
        value = 0.0;
    } else if (value > 1.0f) {
        value = 1.0;
    }
    
    timer_mr tid = pwm_timer_map[obj->pwm];
    LPC_TMR_TypeDef *timer = Timers[tid.timer];
    uint32_t t_off = timer->MR3 - (uint32_t)((float)(timer->MR3) * value);
    
    timer->TCR = TCR_RESET;
    timer->MR[tid.mr] = t_off;
    timer->TCR = TCR_CNT_EN;
}

float pwmout_read(pwmout_t* obj) {
    timer_mr tid = pwm_timer_map[obj->pwm];
    LPC_TMR_TypeDef *timer = Timers[tid.timer];
    
    float v = (float)(timer->MR3 - timer->MR[tid.mr]) / (float)(timer->MR3);
    return (v > 1.0f) ? (1.0f) : (v);
}

void pwmout_period(pwmout_t* obj, float seconds) {
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t* obj, int ms) {
    pwmout_period_us(obj, ms * 1000);
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t* obj, int us) {
    int i = 0;
    uint32_t period_ticks = pwm_clock_mhz * us;
    
    timer_mr tid = pwm_timer_map[obj->pwm];
    LPC_TMR_TypeDef *timer = Timers[tid.timer];
    uint32_t old_period_ticks = timer->MR3;
    
    timer->TCR = TCR_RESET;
    timer->MR3 = period_ticks;
    
    // Scale the pulse width to preserve the duty ratio
    if (old_period_ticks > 0) {
        for (i=0; i<3; i++) {
            uint32_t t_off = period_ticks - (uint32_t)(((uint64_t)timer->MR[i] * (uint64_t)period_ticks) / (uint64_t)old_period_ticks);
            timer->MR[i] = t_off;
        }
    }
    timer->TCR = TCR_CNT_EN;
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
    uint32_t t_on = (uint32_t)(((uint64_t)SystemCoreClock * (uint64_t)us) / (uint64_t)1000000);
    timer_mr tid = pwm_timer_map[obj->pwm];
    LPC_TMR_TypeDef *timer = Timers[tid.timer];
    
    timer->TCR = TCR_RESET;
    if (t_on > timer->MR3) {
        pwmout_period_us(obj, us);
    }
    uint32_t t_off = timer->MR3 - t_on;
    timer->MR[tid.mr] = t_off;
    timer->TCR = TCR_CNT_EN;
}
