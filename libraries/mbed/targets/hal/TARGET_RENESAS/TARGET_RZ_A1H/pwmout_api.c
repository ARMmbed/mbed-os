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

#include "cpg_iodefine.h"
#include "pwm_iodefine.h"

#define TCR_CNT_EN       0x00000001
#define TCR_RESET        0x00000002

//  PORT ID, PWM ID, Pin function
static const PinMap PinMap_PWM[] = {
    {LED_RED  , 0, 4},
    {LED_GREEN, 1, 4},
    {LED_BLUE , 2, 4},
    {P4_7     , 3, 4},
    {P8_14    , 4, 6},
    {P8_15    , 5, 6},
    {P8_13    , 6, 6},
    {P8_11    , 7, 6},
    {NC, NC, 0}
};

static __IO uint16_t PORT[] = {
     PWM2E,
     PWM2F,
     PWM2G,
     PWM2H,
     PWM1G,
     PWM1H,
     PWM1F,
     PWM1D,
};
static __IO uint16_t *PWM_MATCH[] = {
    &PWMPWBFR_2E,
    &PWMPWBFR_2E,
    &PWMPWBFR_2G,
    &PWMPWBFR_2G,
    &PWMPWBFR_1G,
    &PWMPWBFR_1G,
    &PWMPWBFR_1E,
    &PWMPWBFR_1C,
};

#define TCR_PWM_EN       0x00000008

static unsigned int pwm_clock_mhz;

void pwmout_init(pwmout_t* obj, PinName pin) {
    // determine the channel
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(pwm != (PWMName)NC);

    obj->pwm = pwm;
    obj->MR = PWM_MATCH[pwm];
    obj->flag = (PORT[pwm]&1)<<12;
    
    // power on
    CPGSTBCR3 &= ~(1<<0);
    
    // clk mode settings PWM mode
    PWMPWCR_1_BYTE_L = 0xc4;
    PWMPWCR_2_BYTE_L = 0xc4;
    
    // output settings
    PWMPWPR_1_BYTE_L = 0x00;
    PWMPWPR_2_BYTE_L = 0x00;

    // cycle reg.
    PWMPWCYR_1 = 0x3ff;
    PWMPWCYR_2 = 0x3ff;
    
    //pwm_clock_mhz = SystemCoreClock / 4000000;
    
    PWMPWCR_1_BYTE_L = 0xcc;
    PWMPWCR_2_BYTE_L = 0xcc;
    // default to 20ms: standard for servos, and fine for e.g. brightness control
    //pwmout_period_ms(obj, 20);
    //pwmout_write    (obj, 0);
    
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
    
    // set channel match to percentage
    uint16_t v = (uint32_t)((float)0x3ff* value);

    v |= (obj->flag);
    
    // workaround for PWM1[1] - Never make it equal MR0, else we get 1 cycle dropout
    *obj->MR = v;
    
    // accept on next period start
    //LPC_PWM1->LER |= 1 << obj->pwm;
}

float pwmout_read(pwmout_t* obj) {
    float v = (float)((*obj->MR&0x3ff)) / 0x3ff;
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
    // calculate number of ticks
    uint16_t ticks = 0x3ff * us;

    // stop timer
    *obj->MR = ticks;

    // Scale the pulse width to preserve the duty ratio

    // set the channel latch to update value at next period start
//    LPC_PWM1->LER |= 1 << 0;

    // enable counter and pwm, clear reset
 //   LPC_PWM1->TCR = TCR_CNT_EN | TCR_PWM_EN;
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
    // calculate number of ticks
    uint32_t v = pwm_clock_mhz * us;
    
    // workaround for PWM1[1] - Never make it equal MR0, else we get 1 cycle dropout
    
    // set the match register value
    *obj->MR = v;
    
    // set the channel latch to update value at next period start
    //LPC_PWM1->LER |= 1 << obj->pwm;
}
