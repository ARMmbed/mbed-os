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

#if DEVICE_PWMOUT

#include "cmsis.h"
#include "pinmap.h"
#include "fsl_tpm.h"
#include "PeripheralPins.h"

static float pwm_clock_mhz;
/* Array of TPM peripheral base address. */
static TPM_Type *const tpm_addrs[] = TPM_BASE_PTRS;

void pwmout_init(pwmout_t* obj, PinName pin)
{
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(pwm != (PWMName)NC);

    obj->pwm_name = pwm;

    uint32_t pwm_base_clock;

    /* TPM clock source is set to OSCERCLK during init */
    pwm_base_clock = CLOCK_GetFreq(kCLOCK_Osc0ErClk);
    float clkval = (float)pwm_base_clock / 1000000.0f;
    uint32_t clkdiv = 0;
    while (clkval > 1) {
        clkdiv++;
        clkval /= 2.0f;
        if (clkdiv == 7) {
            break;
        }
    }

    pwm_clock_mhz = clkval;
    uint32_t channel = pwm & 0xF;
    uint32_t instance = pwm >> TPM_SHIFT;
    tpm_config_t tpmInfo;

    TPM_GetDefaultConfig(&tpmInfo);
    tpmInfo.prescale = (tpm_clock_prescale_t)clkdiv;
    /* Initialize TPM module */
    TPM_Init(tpm_addrs[instance], &tpmInfo);

    tpm_chnl_pwm_signal_param_t config = {
        .chnlNumber = (tpm_chnl_t)channel,
        .level = kTPM_HighTrue,
        .dutyCyclePercent = 0,
    };
    // default to 20ms: standard for servos, and fine for e.g. brightness control
    TPM_SetupPwm(tpm_addrs[instance], &config, 1, kTPM_EdgeAlignedPwm, 50, pwm_base_clock);

    TPM_StartTimer(tpm_addrs[instance], kTPM_SystemClock);

    // Wire pinout
    pinmap_pinout(pin, PinMap_PWM);
}

void pwmout_free(pwmout_t* obj)
{
    TPM_Deinit(tpm_addrs[obj->pwm_name >> TPM_SHIFT]);
}

void pwmout_write(pwmout_t* obj, float value)
{
    if (value < 0.0f) {
        value = 0.0f;
    } else if (value > 1.0f) {
        value = 1.0f;
    }

    TPM_Type *base = tpm_addrs[obj->pwm_name >> TPM_SHIFT];
    uint16_t mod = base->MOD & TPM_MOD_MOD_MASK;
    uint32_t new_count = (uint32_t)((float)(mod) * value);
    // Update of CnV register
    base->CONTROLS[obj->pwm_name & 0xF].CnV = new_count;
    base->CNT = 0;
}

float pwmout_read(pwmout_t* obj)
{
    TPM_Type *base = tpm_addrs[obj->pwm_name >> TPM_SHIFT];
    uint16_t count = (base->CONTROLS[obj->pwm_name & 0xF].CnV) & TPM_CnV_VAL_MASK;
    uint16_t mod = base->MOD & TPM_MOD_MOD_MASK;

    if (mod == 0)
        return 0.0;
    float v = (float)(count) / (float)(mod);
    return (v > 1.0f) ? (1.0f) : (v);
}

void pwmout_period(pwmout_t* obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t* obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t* obj, int us)
{
    TPM_Type *base = tpm_addrs[obj->pwm_name >> TPM_SHIFT];
    float dc = pwmout_read(obj);

    // Stop TPM clock to ensure instant update of MOD register
    base->MOD = TPM_MOD_MOD((pwm_clock_mhz * (float)us) - 1);
    pwmout_write(obj, dc);
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds)
{
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms)
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us)
{
    TPM_Type *base = tpm_addrs[obj->pwm_name >> TPM_SHIFT];
    uint32_t value = (uint32_t)(pwm_clock_mhz * (float)us);

    // Update of CnV register
    base->CONTROLS[obj->pwm_name & 0xF].CnV = value;
}

#endif
