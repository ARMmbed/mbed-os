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

#if DEVICE_PWMOUT

#include "cmsis.h"
#include "pinmap.h"
#include "error.h"

#define TCR_CNT_EN       0x00000001
#define TCR_RESET        0x00000002

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
//  PORT ID, PWM ID, Pin function
static const PinMap PinMap_PWM[] = {
    {P1_18, PWM_1, 2},
    {P1_20, PWM_2, 2},
    {P1_21, PWM_3, 2},
    {P1_23, PWM_4, 2},
    {P1_24, PWM_5, 2},
    {P1_26, PWM_6, 2},
    {P2_0 , PWM_1, 1},
    {P2_1 , PWM_2, 1},
    {P2_2 , PWM_3, 1},
    {P2_3 , PWM_4, 1},
    {P2_4 , PWM_5, 1},
    {P2_5 , PWM_6, 1},
    {P3_25, PWM_2, 3},
    {P3_26, PWM_3, 3},
    {NC, NC, 0}
};

__IO uint32_t *PWM_MATCH[] = {
    &(LPC_PWM1->MR0),
    &(LPC_PWM1->MR1),
    &(LPC_PWM1->MR2),
    &(LPC_PWM1->MR3),
    &(LPC_PWM1->MR4),
    &(LPC_PWM1->MR5),
    &(LPC_PWM1->MR6)
};

#define TCR_PWM_EN       0x00000008

static unsigned int pwm_clock_mhz;

#elif defined(TARGET_LPC11U24)
/* To have a PWM where we can change both the period and the duty cycle,
 * we need an entire timer. With the following conventions:
 *   * MR3 is used for the PWM period
 *   * MR0, MR1, MR2 are used for the duty cycle
 */
static const PinMap PinMap_PWM[] = {
    /* CT16B0 */
    {P0_8 , PWM_1, 2}, {P1_13, PWM_1, 2},    /* MR0 */
    {P0_9 , PWM_2, 2}, {P1_14, PWM_2, 2},   /* MR1 */
    {P0_10, PWM_3, 3}, {P1_15, PWM_3, 2},   /* MR2 */

    /* CT16B1 */
    {P0_21, PWM_4, 1},                      /* MR0 */
    {P0_22, PWM_5, 2}, {P1_23, PWM_5, 1},   /* MR1 */

    /* CT32B0 */
    {P0_18, PWM_6, 2}, {P1_24, PWM_6, 1},   /* MR0 */
    {P0_19, PWM_7, 2}, {P1_25, PWM_7, 1},   /* MR1 */
    {P0_1 , PWM_8, 2}, {P1_26, PWM_8, 1},   /* MR2 */

    /* CT32B1 */
    {P0_13, PWM_9 , 3}, {P1_0, PWM_9 , 1},  /* MR0 */
    {P0_14, PWM_10, 3}, {P1_1, PWM_10, 1},  /* MR1 */
    {P0_15, PWM_11, 3}, {P1_2, PWM_11, 1},  /* MR2 */

    {NC, NC, 0}
};

typedef struct {
    uint8_t timer;
    uint8_t mr;
} timer_mr;

static timer_mr pwm_timer_map[11] = {
    {0, 0}, {0, 1}, {0, 2},
    {1, 0}, {1, 1},
    {2, 0}, {2, 1}, {2, 2},
    {3, 0}, {3, 1}, {3, 2},
};

static LPC_CTxxBx_Type *Timers[4] = {
    LPC_CT16B0, LPC_CT16B1,
    LPC_CT32B0, LPC_CT32B1
};

static unsigned int pwm_clock_mhz;

#elif defined(TARGET_LPC4088)
//  PORT ID, PWM ID, Pin function
static const PinMap PinMap_PWM[] = {
    {P1_2,  PWM0_1, 3},
    {P1_3,  PWM0_2, 3},
    {P1_5,  PWM0_3, 3},
    {P1_6,  PWM0_4, 3},
    {P1_7,  PWM0_5, 3},
    {P1_11, PWM0_6, 3},
    {P1_18, PWM1_1, 2},
    {P1_20, PWM1_2, 2},
    {P1_21, PWM1_3, 2},
    {P1_23, PWM1_4, 2},
    {P1_24, PWM1_5, 2},
    {P1_26, PWM1_6, 2},
    {P2_0,  PWM1_1, 1},
    {P2_1,  PWM1_2, 1},
    {P2_2,  PWM1_3, 1},
    {P2_3,  PWM1_4, 1},
    {P2_4,  PWM1_5, 1},
    {P2_5,  PWM1_6, 1},
    {P3_16, PWM0_1, 2},
    {P3_17, PWM0_2, 2},
    {P3_18, PWM0_3, 2},
    {P3_19, PWM0_4, 2},
    {P3_20, PWM0_5, 2},
    {P3_21, PWM0_6, 2},
    {P3_24, PWM1_1, 2},
    {P3_25, PWM1_2, 2},
    {P3_26, PWM1_3, 2},
    {P3_27, PWM1_4, 2},
    {P3_28, PWM1_5, 2},
    {P3_29, PWM1_6, 2},
    {NC, NC, 0}
};

static const uint32_t PWM_mr_offset[7] = {
    0x18, 0x1C, 0x20, 0x24, 0x40, 0x44, 0x48
};

#define TCR_PWM_EN       0x00000008
static unsigned int pwm_clock_mhz;

#endif

void pwmout_init(pwmout_t* obj, PinName pin) {
    // determine the channel
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    if (pwm == (uint32_t)NC)
        error("PwmOut pin mapping failed");

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
    obj->pwm = pwm;
    obj->MR = PWM_MATCH[pwm];

    // ensure the power is on
    LPC_SC->PCONP |= 1 << 6;

    // ensure clock to /4
    LPC_SC->PCLKSEL0 &= ~(0x3 << 12);     // pclk = /4
    LPC_PWM1->PR = 0;                     // no pre-scale

    // ensure single PWM mode
    LPC_PWM1->MCR = 1 << 1; // reset TC on match 0

    // enable the specific PWM output
    LPC_PWM1->PCR |= 1 << (8 + pwm);

    pwm_clock_mhz = SystemCoreClock / 4000000;

#elif defined(TARGET_LPC11U24)
    obj->pwm = pwm;

    // Timer registers
    timer_mr tid = pwm_timer_map[pwm];
    LPC_CTxxBx_Type *timer = Timers[tid.timer];

    // Disable timer
    timer->TCR = 0;

    // Power the correspondent timer
    LPC_SYSCON->SYSAHBCLKCTRL |= 1 << (tid.timer + 7);

    /* Enable PWM function */
    timer->PWMC = (1 << 3)|(1 << 2)|(1 << 1)|(1 << 0);

    /* Reset Functionality on MR3 controlling the PWM period */
    timer->MCR = 1 << 10;

    pwm_clock_mhz = SystemCoreClock / 1000000;

#elif defined(TARGET_LPC4088)
    obj->channel = pwm;
    obj->pwm = LPC_PWM0;

    if (obj->channel > 6) { // PWM1 is used if pwm > 6
      obj->channel -= 6;
      obj->pwm = LPC_PWM1;
    }

    obj->MR = (__IO uint32_t *)((uint32_t)obj->pwm + PWM_mr_offset[obj->channel]);

    // ensure the power is on
    if (obj->pwm == LPC_PWM0) {
        LPC_SC->PCONP |= 1 << 5;
    } else {
        LPC_SC->PCONP |= 1 << 6;
    }

    obj->pwm->PR = 0;                     // no pre-scale

    // ensure single PWM mode
    obj->pwm->MCR = 1 << 1; // reset TC on match 0

    // enable the specific PWM output
    obj->pwm->PCR |= 1 << (8 + obj->channel);

    pwm_clock_mhz = PeripheralClock / 1000000;

#endif
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

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
    // set channel match to percentage
    uint32_t v = (uint32_t)((float)(LPC_PWM1->MR0) * value);

    // workaround for PWM1[1] - Never make it equal MR0, else we get 1 cycle dropout
    if (v == LPC_PWM1->MR0) {
        v++;
    }

    *obj->MR = v;

    // accept on next period start
    LPC_PWM1->LER |= 1 << obj->pwm;

#elif defined(TARGET_LPC11U24)
    timer_mr tid = pwm_timer_map[obj->pwm];
    LPC_CTxxBx_Type *timer = Timers[tid.timer];
    uint32_t t_off = timer->MR3 - (uint32_t)((float)(timer->MR3) * value);

    timer->TCR = TCR_RESET;
    timer->MR[tid.mr] = t_off;
    timer->TCR = TCR_CNT_EN;

#elif defined(TARGET_LPC4088)
    // set channel match to percentage
    uint32_t v = (uint32_t)((float)(obj->pwm->MR0) * value);

    // workaround for PWM1[1] - Never make it equal MR0, else we get 1 cycle dropout
    if (v == obj->pwm->MR0) {
        v++;
    }

    *obj->MR = v;

    // accept on next period start
    obj->pwm->LER |= 1 << obj->channel;

#endif
}

float pwmout_read(pwmout_t* obj) {
    float v;
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
    v = (float)(*obj->MR) / (float)(LPC_PWM1->MR0);

#elif defined(TARGET_LPC11U24)
    timer_mr tid = pwm_timer_map[obj->pwm];
    LPC_CTxxBx_Type *timer = Timers[tid.timer];
    v = (float)(timer->MR3 - timer->MR[tid.mr]) / (float)(timer->MR3);

#elif defined(TARGET_LPC4088)
    v = (float)(*obj->MR) / (float)(obj->pwm->MR0);
#endif

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
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
    // calculate number of ticks
    uint32_t ticks = pwm_clock_mhz * us;

    // set reset
    LPC_PWM1->TCR = TCR_RESET;

    // set the global match register
    LPC_PWM1->MR0 = ticks;

    // Scale the pulse width to preserve the duty ratio
    if (LPC_PWM1->MR0 > 0) {
        *obj->MR = (*obj->MR * ticks) / LPC_PWM1->MR0;
    }

    // set the channel latch to update value at next period start
    LPC_PWM1->LER |= 1 << 0;

    // enable counter and pwm, clear reset
    LPC_PWM1->TCR = TCR_CNT_EN | TCR_PWM_EN;

#elif defined(TARGET_LPC11U24)
    int i = 0;
    uint32_t period_ticks = pwm_clock_mhz * us;

    timer_mr tid = pwm_timer_map[obj->pwm];
    LPC_CTxxBx_Type *timer = Timers[tid.timer];
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

#elif defined(TARGET_LPC4088)

    // calculate number of ticks
    uint32_t ticks = pwm_clock_mhz * us;

    // set reset
    obj->pwm->TCR = TCR_RESET;

    // set the global match register
    obj->pwm->MR0 = ticks;

    // Scale the pulse width to preserve the duty ratio
    if (obj->pwm->MR0 > 0) {
        *obj->MR = (*obj->MR * ticks) / obj->pwm->MR0;
    }

    // set the channel latch to update value at next period start
    obj->pwm->LER |= 1 << 0;

    // enable counter and pwm, clear reset
    obj->pwm->TCR = TCR_CNT_EN | TCR_PWM_EN;

#endif
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
    // calculate number of ticks
    uint32_t v = pwm_clock_mhz * us;

    // workaround for PWM1[1] - Never make it equal MR0, else we get 1 cycle dropout
    if (v == LPC_PWM1->MR0) {
        v++;
    }

    // set the match register value
    *obj->MR = v;

    // set the channel latch to update value at next period start
    LPC_PWM1->LER |= 1 << obj->pwm;

#elif defined(TARGET_LPC11U24)
    uint32_t t_on = (uint32_t)(((uint64_t)SystemCoreClock * (uint64_t)us) / (uint64_t)1000000);
    timer_mr tid = pwm_timer_map[obj->pwm];
    LPC_CTxxBx_Type *timer = Timers[tid.timer];

    timer->TCR = TCR_RESET;
    if (t_on > timer->MR3) {
        pwmout_period_us(obj, us);
    }
    uint32_t t_off = timer->MR3 - t_on;
    timer->MR[tid.mr] = t_off;
    timer->TCR = TCR_CNT_EN;

#elif defined(TARGET_LPC4088)
    // calculate number of ticks
    uint32_t v = pwm_clock_mhz * us;

    // workaround for PWM1[1] - Never make it equal MR0, else we get 1 cycle dropout
    if (v == obj->pwm->MR0) {
        v++;
    }

    // set the match register value
    *obj->MR = v;

    // set the channel latch to update value at next period start
    obj->pwm->LER |= 1 << obj->channel;


#endif
}

#endif
