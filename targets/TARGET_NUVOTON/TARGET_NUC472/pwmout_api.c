/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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
#include "PeripheralPins.h"
#include "nu_modutil.h"
#include "nu_miscutil.h"
#include "nu_bitutil.h"

struct nu_pwm_var {
    uint32_t    en_msk;
};

static struct nu_pwm_var pwm0_var = {
    .en_msk = 0
};

static struct nu_pwm_var pwm1_var = {
    .en_msk = 0
};

static uint32_t pwm_modinit_mask = 0;

// FIXME: PWM1 2/3 channels fail. PWM registers cannot write after their respective clocks are enabled.
static const struct nu_modinit_s pwm_modinit_tab[] = {
    {PWM_0_0, PWM0CH01_MODULE, CLK_CLKSEL2_PWM0CH01SEL_HIRC, 0, PWM0_RST, PWM0CH0_IRQn, &pwm0_var},
    {PWM_0_1, PWM0CH01_MODULE, CLK_CLKSEL2_PWM0CH01SEL_HIRC, 0, PWM0_RST, PWM0CH1_IRQn, &pwm0_var},
    {PWM_0_2, PWM0CH23_MODULE, CLK_CLKSEL2_PWM0CH23SEL_HIRC, 0, PWM0_RST, PWM0CH2_IRQn, &pwm0_var},
    {PWM_0_3, PWM0CH23_MODULE, CLK_CLKSEL2_PWM0CH23SEL_HIRC, 0, PWM0_RST, PWM0CH3_IRQn, &pwm0_var},
    {PWM_0_4, PWM0CH45_MODULE, CLK_CLKSEL2_PWM0CH45SEL_HIRC, 0, PWM0_RST, PWM0CH4_IRQn, &pwm0_var},
    {PWM_0_5, PWM0CH45_MODULE, CLK_CLKSEL2_PWM0CH45SEL_HIRC, 0, PWM0_RST, PWM0CH5_IRQn, &pwm0_var},

    {PWM_1_0, PWM1CH01_MODULE, CLK_CLKSEL2_PWM1CH01SEL_HIRC, 0, PWM1_RST, PWM1CH0_IRQn, &pwm1_var},
    {PWM_1_1, PWM1CH01_MODULE, CLK_CLKSEL2_PWM1CH01SEL_HIRC, 0, PWM1_RST, PWM1CH1_IRQn, &pwm1_var},
    {PWM_1_2, PWM1CH23_MODULE, CLK_CLKSEL2_PWM1CH23SEL_HIRC, 0, PWM1_RST, PWM1CH2_IRQn, &pwm1_var},
    {PWM_1_3, PWM1CH23_MODULE, CLK_CLKSEL2_PWM1CH23SEL_HIRC, 0, PWM1_RST, PWM1CH3_IRQn, &pwm1_var},
    {PWM_1_4, PWM1CH45_MODULE, CLK_CLKSEL2_PWM1CH45SEL_HIRC, 0, PWM1_RST, PWM1CH4_IRQn, &pwm1_var},
    {PWM_1_5, PWM1CH45_MODULE, CLK_CLKSEL2_PWM1CH45SEL_HIRC, 0, PWM1_RST, PWM1CH5_IRQn, &pwm1_var},

    {NC, 0, 0, 0, 0, (IRQn_Type) 0, NULL}
};

static void pwmout_config(pwmout_t *obj);

void pwmout_init(pwmout_t *obj, PinName pin)
{
    obj->pwm = (PWMName) pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT((int) obj->pwm != NC);

    const struct nu_modinit_s *modinit = get_modinit(obj->pwm, pwm_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == obj->pwm);

    obj->pin = pin;

    // Wire pinout
    pinmap_pinout(pin, PinMap_PWM);

    uint32_t chn =  NU_MODSUBINDEX(obj->pwm);

    // NOTE: Channels 0/1, 2/3, and 4/5 share a clock source.
    if ((((struct nu_pwm_var *) modinit->var)->en_msk & (0x3 << (chn / 2 * 2))) == 0) {
        // Select clock source of paired channels
        CLK_SetModuleClock(modinit->clkidx, modinit->clksrc, modinit->clkdiv);

        // Enable clock of paired channels
        CLK_EnableModuleClock(modinit->clkidx);

        // FIXME: PWM_1_2/3 design bug. PWM_1_2/3 also require PWM_1_0/1 clock enabled.
        if (obj->pwm == PWM_1_2 || obj->pwm == PWM_1_3) {
            CLK_EnableModuleClock(PWM1CH01_MODULE);
        }
    }

    // NOTE: All channels (identified by PWMName) share a PWM module. This reset will also affect other channels of the same PWM module.
    if (!((struct nu_pwm_var *) modinit->var)->en_msk) {
        // Reset this module if no channel enabled
        SYS_ResetModule(modinit->rsetidx);
    }

    PWM_T *pwm_base = (PWM_T *) NU_MODBASE(obj->pwm);

    // Default: period = 10 ms, pulse width = 0 ms
    obj->period_us = 1000 * 10;
    obj->pulsewidth_us = 0;
    pwmout_config(obj);

    // Enable output of the specified PWM channel
    PWM_EnableOutput(pwm_base, 1 << chn);
    PWM_Start(pwm_base, 1 << chn);

    ((struct nu_pwm_var *) modinit->var)->en_msk |= 1 << chn;

    // Mark this module to be inited.
    int i = modinit - pwm_modinit_tab;
    pwm_modinit_mask |= 1 << i;
}

void pwmout_free(pwmout_t *obj)
{
    PWM_T *pwm_base = (PWM_T *) NU_MODBASE(obj->pwm);
    uint32_t chn =  NU_MODSUBINDEX(obj->pwm);
    PWM_ForceStop(pwm_base, 1 << chn);

    const struct nu_modinit_s *modinit = get_modinit(obj->pwm, pwm_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == obj->pwm);
    ((struct nu_pwm_var *) modinit->var)->en_msk &= ~(1 << chn);


    if ((((struct nu_pwm_var *) modinit->var)->en_msk & (0x3 << (chn / 2 * 2))) == 0) {
        // FIXME: PWM_1_2/3 design bug. PWM_1_2/3 also require PWM_1_0/1 clock enabled.
        switch (obj->pwm) {
            case PWM_1_0:
            case PWM_1_1:
                if (pwm1_var.en_msk & 0xC) {
                    break;
                }

            case PWM_1_2:
            case PWM_1_3:
                if (!(pwm1_var.en_msk & 0x3)) {
                    CLK_DisableModuleClock(PWM1CH01_MODULE);
                }

            default:
                // Disable clock of paired channels
                CLK_DisableModuleClock(modinit->clkidx);
        }
    }

    // Mark this module to be deinited.
    int i = modinit - pwm_modinit_tab;
    pwm_modinit_mask &= ~(1 << i);

    // Free up pins
    gpio_set(obj->pin);
    obj->pin = NC;
}

void pwmout_write(pwmout_t *obj, float value)
{
    obj->pulsewidth_us = NU_CLAMP((uint32_t)(value * obj->period_us), 0, obj->period_us);
    pwmout_config(obj);
}

float pwmout_read(pwmout_t *obj)
{
    return NU_CLAMP((((float) obj->pulsewidth_us) / obj->period_us), 0.0f, 1.0f);
}

void pwmout_period(pwmout_t *obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t *obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t *obj, int us)
{
    uint32_t period_us_old = obj->period_us;
    uint32_t pulsewidth_us_old = obj->pulsewidth_us;
    obj->period_us = us;
    obj->pulsewidth_us = NU_CLAMP(obj->period_us * pulsewidth_us_old / period_us_old, 0, obj->period_us);
    pwmout_config(obj);
}

int pwmout_read_period_us(pwmout_t *obj)
{
    return obj->period_us;
}

void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    obj->pulsewidth_us = NU_CLAMP(us, 0, obj->period_us);
    pwmout_config(obj);
}

int pwmout_read_pulsewidth_us(pwmout_t *obj)
{
    return obj->pulsewidth_us;
}

static void pwmout_config(pwmout_t *obj)
{
    PWM_T *pwm_base = (PWM_T *) NU_MODBASE(obj->pwm);
    uint32_t chn = NU_MODSUBINDEX(obj->pwm);
    // NOTE: Support period < 1s
    //PWM_ConfigOutputChannel(pwm_base, chn, 1000 * 1000 / obj->period_us, obj->pulsewidth_us * 100 / obj->period_us);
    PWM_ConfigOutputChannel2(pwm_base, chn, 1000 * 1000, obj->pulsewidth_us * 10000 / obj->period_us, obj->period_us);
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}

#endif
