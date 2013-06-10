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

static const PinMap PinMap_PWM[] = {
    // LEDs
    {LED_RED  , PWM_9 , 3}, // PTB18, TPM2 CH0
    {LED_GREEN, PWM_10, 3}, // PTB19, TPM2 CH1
    {LED_BLUE , PWM_2 , 4}, // PTD1 , TPM0 CH1

    // Arduino digital pinout
    {D0,  PWM_9 , 3}, // PTA1 , TPM2 CH0
    {D1,  PWM_10, 3}, // PTA2 , TPM2 CH1
    {D2,  PWM_5 , 4}, // PTD4 , TPM0 CH4
    {D3,  PWM_7 , 3}, // PTA12, TPM1 CH0
    {D4,  PWM_2 , 3}, // PTA4 , TPM0 CH1
    {D5,  PWM_3 , 3}, // PTA5 , TPM0 CH2
    {D6,  PWM_5 , 3}, // PTC8 , TPM0 CH4
    {D7,  PWM_6 , 3}, // PTC9 , TPM0 CH5
    {D8,  PWM_8 , 3}, // PTA13, TPM1 CH1
    {D9,  PWM_6 , 4}, // PTD5 , TPM0 CH5
    {D10, PWM_1 , 4}, // PTD0 , TPM0 CH0
    {D11, PWM_3 , 4}, // PTD2 , TPM0 CH2
    {D12, PWM_4 , 4}, // PTD3 , TPM0 CH3
    {D13, PWM_2 , 4}, // PTD1 , TPM0 CH1
    {NC , NC    , 0}
};

#define PWM_CLOCK_MHZ       (0.75) // (48)MHz / 64 = (0.75)MHz

void pwmout_init(pwmout_t* obj, PinName pin) {
    // determine the channel
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    if (pwm == (uint32_t)NC)
        error("PwmOut pin mapping failed");

    unsigned int port = (unsigned int)pin >> PORT_SHIFT;
    unsigned int tpm_n = (pwm >> TPM_SHIFT);
    unsigned int ch_n = (pwm & 0xFF);

    SIM->SCGC5 |= 1 << (SIM_SCGC5_PORTA_SHIFT + port);
    SIM->SCGC6 |= 1 << (SIM_SCGC6_TPM0_SHIFT + tpm_n);
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // Clock source: MCGFLLCLK or MCGPLLCLK

    TPM_Type *tpm = (TPM_Type *)(TPM0_BASE + 0x1000 * tpm_n);
    tpm->SC = TPM_SC_CMOD(1) | TPM_SC_PS(6); // (48)MHz / 64 = (0.75)MHz
    tpm->CONTROLS[ch_n].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK); /* No Interrupts; High True pulses on Edge Aligned PWM */

    obj->CnV = &tpm->CONTROLS[ch_n].CnV;
    obj->MOD = &tpm->MOD;
    obj->CNT = &tpm->CNT;

    // default to 20ms: standard for servos, and fine for e.g. brightness control
    pwmout_period_ms(obj, 20);
    pwmout_write    (obj, 0);

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

    *obj->CnV = (uint32_t)((float)(*obj->MOD) * value);
    *obj->CNT = 0;
}

float pwmout_read(pwmout_t* obj) {
    float v = (float)(*obj->CnV) / (float)(*obj->MOD);
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
    *obj->MOD = PWM_CLOCK_MHZ * us;
    pwmout_write(obj, dc);
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
    *obj->CnV = PWM_CLOCK_MHZ * us;
}
