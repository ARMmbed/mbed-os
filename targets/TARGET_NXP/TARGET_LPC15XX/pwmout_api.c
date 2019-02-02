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
#include "mbed_error.h"

static LPC_SCT0_Type *SCTs[4] = {
    (LPC_SCT0_Type*)LPC_SCT0,
    (LPC_SCT0_Type*)LPC_SCT1,
    (LPC_SCT0_Type*)LPC_SCT2,
    (LPC_SCT0_Type*)LPC_SCT3,
};

// Pinmap used for testing only
static const PinMap PinMap_PWM_testing[] = {
    {P0_0,  0, 0},
    {P0_1,  0, 0},
    {P0_2,  0, 0},
    {P0_3,  0, 0},
    {P0_4,  0, 0},
    {P0_5,  0, 0},
    {P0_6,  0, 0},
    {P0_7,  0, 0},
    {P0_8,  0, 0},
    {P0_9,  0, 0},
    {P0_10, 0, 0},
    {P0_11, 0, 0},
    {P0_12, 0, 0},
    {P0_13, 0, 0},
    {P0_14, 0, 0},
    {P0_15, 0, 0},
    {P0_16, 0, 0},
    {P0_17, 0, 0},
    {P0_18, 0, 0},
    {P0_19, 0, 0},
    {P0_20, 0, 0},
    {P0_21, 0, 0},
    {P0_22, 0, 0},
    {P0_23, 0, 0},
    {P0_24, 0, 0},
    {P0_25, 0, 0},
    {P0_26, 0, 0},
    {P0_27, 0, 0},
    {P0_28, 0, 0},
    {P0_29, 0, 0},
    {P0_30, 0, 0},
    {P0_31, 0, 0},
    {P1_0,  0, 0},
    {P1_1,  0, 0},
    {P1_2,  0, 0},
    {P1_3,  0, 0},
    {P1_4,  0, 0},
    {P1_5,  0, 0},
    {P1_6,  0, 0},
    {P1_7,  0, 0},
    {P1_8,  0, 0},
    {P1_9,  0, 0},
    {P1_10, 0, 0},
    {P1_11, 0, 0},
    {P1_12, 0, 0},
    {P1_13, 0, 0},
    {P1_14, 0, 0},
    {P1_15, 0, 0},
    {P1_16, 0, 0},
    {P1_17, 0, 0},
    {P1_18, 0, 0},
    {P1_19, 0, 0},
    {P1_20, 0, 0},
    {P1_21, 0, 0},
    {P1_22, 0, 0},
    {P1_23, 0, 0},
    {P1_24, 0, 0},
    {P1_25, 0, 0},
    {P1_26, 0, 0},
    {P1_27, 0, 0},
    {P1_28, 0, 0},
    {P1_29, 0, 0},
    {P1_30, 0, 0},
    {P1_31, 0, 0},
    {P2_0,  0, 0},
    {P2_1,  0, 0},
    {P2_2,  0, 0},
    {P2_3,  0, 0},
    {P2_4,  0, 0},
    {P2_5,  0, 0},
    {P2_6,  0, 0},
    {P2_7,  0, 0},
    {P2_8,  0, 0},
    {P2_9,  0, 0},
    {P2_10, 0, 0},
    {P2_11, 0, 0},
    {P2_12, 0, 0},

    {NC, NC, 0}
};

// bit flags for used SCTs
static unsigned char sct_used = (1 << 3);
static int get_available_sct(void) {
    int i;
    for (i=0; i<4; i++) {
        if ((sct_used & (1 << i)) == 0)
            return i;
    }
    return -1;
}

void pwmout_init(pwmout_t* obj, PinName pin) {
    MBED_ASSERT(pin != (uint32_t)NC);

    int sct_n = get_available_sct();
    if (sct_n == -1) {
        error("No available SCT");
    }
    
    sct_used |= (1 << sct_n);
    obj->pwm =  SCTs[sct_n];
    obj->pwm_ch = sct_n;

    LPC_SCT0_Type* pwm = obj->pwm;

    // Enable the SCT clock
    LPC_SYSCON->SYSAHBCLKCTRL1 |= (1 << (obj->pwm_ch + 2));

    // Clear peripheral reset the SCT:
    LPC_SYSCON->PRESETCTRL1 |=  (1 << (obj->pwm_ch + 2));
    LPC_SYSCON->PRESETCTRL1 &= ~(1 << (obj->pwm_ch + 2));
    
    switch(obj->pwm_ch) {
        case 0:
            // SCT0_OUT0
            LPC_SWM->PINASSIGN[7] &= ~0x0000FF00;
            LPC_SWM->PINASSIGN[7] |= (pin << 8);
            break;
        case 1:
            // SCT1_OUT0
            LPC_SWM->PINASSIGN[8] &= ~0x000000FF;
            LPC_SWM->PINASSIGN[8] |= (pin);
            break;
        case 2:
            // SCT2_OUT0
            LPC_SWM->PINASSIGN[8] &= ~0xFF000000;
            LPC_SWM->PINASSIGN[8] |= (pin << 24);
            break;
        case 3:
            // SCT3_OUT0
            LPC_SWM->PINASSIGN[9] &= ~0x00FF0000;
            LPC_SWM->PINASSIGN[9] |= (pin << 16);
            break;
        default:
            break;
    }
    
    // Unified 32-bit counter, autolimit
    pwm->CONFIG |= ((0x3 << 17) | 0x01);
    
    // halt and clear the counter
    pwm->CTRL |= (1 << 2) | (1 << 3);
    
    pwm->OUT0_SET = (1 << 0); // event 0
    pwm->OUT0_CLR = (1 << 1); // event 1
    // Resolve conflicts on output 0 to set output
    // This allows duty cycle = 1.0 to work, where the MATCH registers for set and clear are equal
    pwm->RES = 0x01;

    pwm->EV0_CTRL  = (1 << 12);
    pwm->EV0_STATE = 0xFFFFFFFF;
    pwm->EV1_CTRL  = (1 << 12) | (1 << 0);
    pwm->EV1_STATE = 0xFFFFFFFF;

    // default to 20ms: standard for servos, and fine for e.g. brightness control
    pwmout_period_ms(obj, 20);
    pwmout_write    (obj, 0);
}

void pwmout_free(pwmout_t* obj) {
    // Disable the SCT clock
    LPC_SYSCON->SYSAHBCLKCTRL1 &= ~(1 << (obj->pwm_ch + 2));
    sct_used &= ~(1 << obj->pwm_ch);
}

void pwmout_write(pwmout_t* obj, float value) {
    LPC_SCT0_Type* pwm = obj->pwm;
    if (value < 0.0f) {
        value = 0.0;
    } else if (value > 1.0f) {
        value = 1.0;
    }
    uint32_t t_on = (uint32_t)((float)(pwm->MATCHREL0 + 1) * value);
    if (t_on > 0) {
        pwm->MATCHREL1 = t_on - 1;
        
        // Un-halt the timer and ensure the new pulse-width takes immediate effect if necessary
        if (pwm->CTRL & (1 << 2)) {
            pwm->MATCH1 = pwm->MATCHREL1;
            pwm->CTRL &= ~(1 << 2);
        }
    } else {
        // Halt the timer and force the output low
        pwm->CTRL |= (1 << 2) | (1 << 3);
        pwm->OUTPUT = 0x00000000;
    }
}

float pwmout_read(pwmout_t* obj) {
    LPC_SCT0_Type* pwm = obj->pwm;
    uint32_t t_off = pwm->MATCHREL0 + 1;
    uint32_t t_on  = (!(pwm->CTRL & (1 << 2))) ? pwm->MATCHREL1 + 1 : 0;
    float v = (float)t_on/(float)t_off;
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
    LPC_SCT0_Type* pwm = obj->pwm;
    uint32_t t_off = pwm->MATCHREL0 + 1;
    uint32_t t_on  = (!(pwm->CTRL & (1 << 2))) ? pwm->MATCHREL1 + 1 : 0;
    float v = (float)t_on/(float)t_off;
    uint32_t period_ticks = (uint32_t)(((uint64_t)SystemCoreClock * (uint64_t)us) / (uint64_t)1000000);
    uint32_t pulsewidth_ticks = period_ticks * v;
    pwm->MATCHREL0 = period_ticks - 1;
    if (pulsewidth_ticks > 0) {
        pwm->MATCHREL1 = pulsewidth_ticks - 1;
        
        // Un-halt the timer and ensure the new period & pulse-width take immediate effect if necessary
        if (pwm->CTRL & (1 << 2)) {
            pwm->MATCH0 = pwm->MATCHREL0;
            pwm->MATCH1 = pwm->MATCHREL1;
            pwm->CTRL &= ~(1 << 2);
        }
    } else {
        // Halt the timer and force the output low
        pwm->CTRL |= (1 << 2) | (1 << 3);
        pwm->OUTPUT = 0x00000000;

        // Ensure the new period will take immediate effect when the timer is un-halted
        pwm->MATCH0 = pwm->MATCHREL0;
    }
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
    LPC_SCT0_Type* pwm = obj->pwm;
    if (us > 0) {
        pwm->MATCHREL1 = (uint32_t)(((uint64_t)SystemCoreClock * (uint64_t)us) / (uint64_t)1000000) - 1;
        
        // Un-halt the timer and ensure the new pulse-width takes immediate effect if necessary
        if (pwm->CTRL & (1 << 2)) {
            pwm->MATCH1 = pwm->MATCHREL1;
            pwm->CTRL &= ~(1 << 2);
        }
    } else {
        // Halt the timer and force the output low
        pwm->CTRL |= (1 << 2) | (1 << 3);
        pwm->OUTPUT = 0x00000000;
    }
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM_testing;
}

