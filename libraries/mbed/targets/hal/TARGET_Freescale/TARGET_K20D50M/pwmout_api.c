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

static const PinMap PinMap_PWM[] = {
    // LEDs
    {LED_RED  , PWM_3 , 4}, // PTC3, FTM0 CH2
    {LED_GREEN, PWM_5,  4}, // PTD4, FTM0 CH4
    {LED_BLUE , PWM_8 , 3}, // PTA2, FTM0 CH7

    // Arduino digital pinout
    {D3,  PWM_5 , 4}, // PTD4, FTM0 CH4
    {D5,  PWM_7 , 3}, // PTA1, FTM0 CH6
    {D6,  PWM_3 , 4}, // PTC3, FTM0 CH2
    {D9,  PWM_6 , 4}, // PTD5, FTM0 CH6
    {D10, PWM_2 , 4}, // PTC2, FTM0 CH1

    {PTA0,  PWM_6 , 3}, // PTA0, FTM0 CH5
    {PTA3,  PWM_1 , 3}, // PTA3, FTM0 CH0
    {PTA4,  PWM_2 , 3}, // PTA4, FTM0 CH1
    {PTA5,  PWM_3 , 3}, // PTA5, FTM0 CH2
    {PTA12, PWM_9 , 3}, // PTA12, FTM1 CH0
    {PTA13, PWM_10, 3}, // PTA13, FTM1 CH1
    {PTB0,  PWM_9 , 3}, // PTB0, FTM1 CH0
    {PTB1,  PWM_10, 3}, // PTB1, FTM1 CH1
    {PTC1,  PWM_1 , 4}, // PTC1, FTM0 CH0
    {PTD4,  PWM_4 , 4}, // PTD4, FTM0 CH3
    {PTD6,  PWM_7 , 4}, // PTD6, FTM0 CH6
    {PTD7,  PWM_8 , 4}, // PTD7, FTM0 CH7

    {NC , NC    , 0}
};

static float pwm_clock = 0;

void pwmout_init(pwmout_t* obj, PinName pin) {
    // determine the channel
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(pwm != (PWMName)NC);

    uint32_t clkdiv = 0;
    float clkval = SystemCoreClock / 1000000.0f;

    while (clkval > 1) {
        clkdiv++;
        clkval /= 2.0;
        if (clkdiv == 7)
            break;
    }

    pwm_clock = clkval;
    unsigned int port = (unsigned int)pin >> PORT_SHIFT;
    unsigned int ftm_n = (pwm >> TPM_SHIFT);
    unsigned int ch_n = (pwm & 0xFF);

    SIM->SCGC5 |= 1 << (SIM_SCGC5_PORTA_SHIFT + port);
    SIM->SCGC6 |= 1 << (SIM_SCGC6_FTM0_SHIFT + ftm_n);

    FTM_Type *ftm = (FTM_Type *)(FTM0_BASE + 0x1000 * ftm_n);
    ftm->CONF |= FTM_CONF_BDMMODE(3);
    ftm->SC = FTM_SC_CLKS(1) | FTM_SC_PS(clkdiv); // (clock)MHz / clkdiv ~= (0.75)MHz
    ftm->CONTROLS[ch_n].CnSC = (FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK); /* No Interrupts; High True pulses on Edge Aligned PWM */

    obj->CnV = &ftm->CONTROLS[ch_n].CnV;
    obj->MOD = &ftm->MOD;
    obj->CNT = &ftm->CNT;

    // default to 20ms: standard for servos, and fine for e.g. brightness control
    pwmout_period_ms(obj, 20);
    pwmout_write(obj, 0);

    // Wire pinout
    pinmap_pinout(pin, PinMap_PWM);
}

void pwmout_free(pwmout_t* obj) {}

void pwmout_write(pwmout_t* obj, float value) {
    if (value < 0.0) {
        value = 0.0;
    } else if (value > 1.0) {
        value = 1.0;
    }

    *obj->CnV = (uint32_t)((float)(*obj->MOD + 1) * value);
}

float pwmout_read(pwmout_t* obj) {
    float v = (float)(*obj->CnV) / (float)(*obj->MOD + 1);
    return (v > 1.0) ? (1.0) : (v);
}

void pwmout_period(pwmout_t* obj, float seconds) {
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t* obj, int ms) {
    pwmout_period_us(obj, ms * 1000);
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t* obj, int us) {
    float dc = pwmout_read(obj);
    *obj->MOD = (uint32_t)(pwm_clock * (float)us) - 1;
    pwmout_write(obj, dc);
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
    *obj->CnV = (uint32_t)(pwm_clock * (float)us);
}
