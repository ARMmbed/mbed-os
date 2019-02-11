/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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
#include "fsl_ftm.h"
#include "PeripheralPins.h"

static float pwm_clock_mhz;
/* Array of FTM peripheral base address. */
static FTM_Type *const ftm_addrs[] = FTM_BASE_PTRS;

void pwmout_init(pwmout_t* obj, PinName pin)
{
    PWMName pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(pwm != (PWMName)NC);

    obj->pwm_name = pwm;

    uint32_t pwm_base_clock;
    pwm_base_clock = CLOCK_GetFreq(kCLOCK_BusClk);
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
    ftm_config_t ftmInfo;

    FTM_GetDefaultConfig(&ftmInfo);
    ftmInfo.prescale = (ftm_clock_prescale_t)clkdiv;
    /* Initialize FTM module */
    FTM_Init(ftm_addrs[instance], &ftmInfo);

    ftm_addrs[instance]->CONF |= FTM_CONF_NUMTOF(3);

    ftm_chnl_pwm_signal_param_t config = {
        .chnlNumber = (ftm_chnl_t)channel,
        .level = kFTM_HighTrue,
        .dutyCyclePercent = 0,
        .firstEdgeDelayPercent = 0
    };
    // default to 20ms: standard for servos, and fine for e.g. brightness control
    FTM_SetupPwm(ftm_addrs[instance], &config, 1, kFTM_EdgeAlignedPwm, 50, pwm_base_clock);

    FTM_StartTimer(ftm_addrs[instance], kFTM_SystemClock);

    // Wire pinout
    pinmap_pinout(pin, PinMap_PWM);
}

void pwmout_free(pwmout_t* obj)
{
    FTM_Deinit(ftm_addrs[obj->pwm_name >> TPM_SHIFT]);
}

void pwmout_write(pwmout_t* obj, float value)
{
    if (value < 0.0f) {
        value = 0.0f;
    } else if (value > 1.0f) {
        value = 1.0f;
    }

    FTM_Type *base = ftm_addrs[obj->pwm_name >> TPM_SHIFT];
    uint16_t mod = base->MOD & FTM_MOD_MOD_MASK;
    uint32_t new_count = (uint32_t)((float)(mod) * value);
    // Update of CnV register
    base->CONTROLS[obj->pwm_name & 0xF].CnV = new_count;
    base->CNT = 0;
    /* Software trigger to update registers */
    FTM_SetSoftwareTrigger(base, true);
}

float pwmout_read(pwmout_t* obj)
{
    FTM_Type *base = ftm_addrs[obj->pwm_name >> TPM_SHIFT];
    uint16_t count = (base->CONTROLS[obj->pwm_name & 0xF].CnV) & FTM_CnV_VAL_MASK;
    uint16_t mod = base->MOD & FTM_MOD_MOD_MASK;

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
    FTM_Type *base = ftm_addrs[obj->pwm_name >> TPM_SHIFT];
    float dc = pwmout_read(obj);

    // Stop FTM clock to ensure instant update of MOD register
    base->MOD = FTM_MOD_MOD((pwm_clock_mhz * (float)us) - 1);
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
    FTM_Type *base = ftm_addrs[obj->pwm_name >> TPM_SHIFT];
    uint32_t value = (uint32_t)(pwm_clock_mhz * (float)us);

    // Update of CnV register
    base->CONTROLS[obj->pwm_name & 0xF].CnV = value;
    /* Software trigger to update registers */
    FTM_SetSoftwareTrigger(base, true);
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}

#endif
