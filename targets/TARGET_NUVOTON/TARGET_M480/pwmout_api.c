/*
 * Copyright (c) 2015-2016, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
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
#include "gpio_api.h"
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

static const struct nu_modinit_s pwm_modinit_tab[] = {
    {PWM_0_0, EPWM0_MODULE, CLK_CLKSEL2_EPWM0SEL_PCLK0, 0, EPWM0_RST, EPWM0P0_IRQn, &pwm0_var},
    {PWM_0_1, EPWM0_MODULE, CLK_CLKSEL2_EPWM0SEL_PCLK0, 0, EPWM0_RST, EPWM0P0_IRQn, &pwm0_var},
    {PWM_0_2, EPWM0_MODULE, CLK_CLKSEL2_EPWM0SEL_PCLK0, 0, EPWM0_RST, EPWM0P1_IRQn, &pwm0_var},
    {PWM_0_3, EPWM0_MODULE, CLK_CLKSEL2_EPWM0SEL_PCLK0, 0, EPWM0_RST, EPWM0P1_IRQn, &pwm0_var},
    {PWM_0_4, EPWM0_MODULE, CLK_CLKSEL2_EPWM0SEL_PCLK0, 0, EPWM0_RST, EPWM0P2_IRQn, &pwm0_var},
    {PWM_0_5, EPWM0_MODULE, CLK_CLKSEL2_EPWM0SEL_PCLK0, 0, EPWM0_RST, EPWM0P2_IRQn, &pwm0_var},

    {PWM_1_0, EPWM1_MODULE, CLK_CLKSEL2_EPWM1SEL_PCLK1, 0, EPWM1_RST, EPWM1P0_IRQn, &pwm1_var},
    {PWM_1_1, EPWM1_MODULE, CLK_CLKSEL2_EPWM1SEL_PCLK1, 0, EPWM1_RST, EPWM1P0_IRQn, &pwm1_var},
    {PWM_1_2, EPWM1_MODULE, CLK_CLKSEL2_EPWM1SEL_PCLK1, 0, EPWM1_RST, EPWM1P1_IRQn, &pwm1_var},
    {PWM_1_3, EPWM1_MODULE, CLK_CLKSEL2_EPWM1SEL_PCLK1, 0, EPWM1_RST, EPWM1P1_IRQn, &pwm1_var},
    {PWM_1_4, EPWM1_MODULE, CLK_CLKSEL2_EPWM1SEL_PCLK1, 0, EPWM1_RST, EPWM1P2_IRQn, &pwm1_var},
    {PWM_1_5, EPWM1_MODULE, CLK_CLKSEL2_EPWM1SEL_PCLK1, 0, EPWM1_RST, EPWM1P2_IRQn, &pwm1_var},

    {NC, 0, 0, 0, 0, (IRQn_Type) 0, NULL}
};

static void pwmout_config(pwmout_t* obj, int start);

void pwmout_init(pwmout_t* obj, PinName pin)
{
    obj->pwm = (PWMName) pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT((int) obj->pwm != NC);

    const struct nu_modinit_s *modinit = get_modinit(obj->pwm, pwm_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->pwm);

    obj->pin = pin;

    // Wire pinout
    pinmap_pinout(pin, PinMap_PWM);

    // NOTE: Channels 0/1/2/3/4/5 share a clock source.
    if ((((struct nu_pwm_var *) modinit->var)->en_msk & 0x3F) == 0) {
        // Select clock source of paired channels
        CLK_SetModuleClock(modinit->clkidx, modinit->clksrc, modinit->clkdiv);

        // Enable clock of paired channels
        CLK_EnableModuleClock(modinit->clkidx);
    }

    // NOTE: All channels (identified by PWMName) share a PWM module. This reset will also affect other channels of the same PWM module.
    if (! ((struct nu_pwm_var *) modinit->var)->en_msk) {
        // Reset this module if no channel enabled
        SYS_ResetModule(modinit->rsetidx);
    }

    uint32_t chn =  NU_MODSUBINDEX(obj->pwm);

    // Default: period = 10 ms, pulse width = 0 ms
    obj->period_us = 1000 * 10;
    obj->pulsewidth_us = 0;
    pwmout_config(obj, 0);

    ((struct nu_pwm_var *) modinit->var)->en_msk |= 1 << chn;

    // Mark this module to be inited.
    int i = modinit - pwm_modinit_tab;
    pwm_modinit_mask |= 1 << i;
}

void pwmout_free(pwmout_t* obj)
{
    EPWM_T *pwm_base = (EPWM_T *) NU_MODBASE(obj->pwm);
    uint32_t chn =  NU_MODSUBINDEX(obj->pwm);
    EPWM_ForceStop(pwm_base, 1 << chn);

    const struct nu_modinit_s *modinit = get_modinit(obj->pwm, pwm_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->pwm);
    ((struct nu_pwm_var *) modinit->var)->en_msk &= ~(1 << chn);


    if ((((struct nu_pwm_var *) modinit->var)->en_msk & 0x3F) == 0) {
        CLK_DisableModuleClock(modinit->clkidx);
    }

    // Mark this module to be deinited.
    int i = modinit - pwm_modinit_tab;
    pwm_modinit_mask &= ~(1 << i);

    // Free up pins
    gpio_set(obj->pin);
    obj->pin = NC;
}

void pwmout_write(pwmout_t* obj, float value)
{
    obj->pulsewidth_us = NU_CLAMP((uint32_t) (value * obj->period_us), 0, obj->period_us);
    pwmout_config(obj, 1);
}

float pwmout_read(pwmout_t* obj)
{
    return NU_CLAMP((((float) obj->pulsewidth_us) / obj->period_us), 0.0f, 1.0f);
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
    uint32_t period_us_old = obj->period_us;
    uint32_t pulsewidth_us_old = obj->pulsewidth_us;
    obj->period_us = us;
    obj->pulsewidth_us = NU_CLAMP(obj->period_us * pulsewidth_us_old / period_us_old, 0, obj->period_us);
    pwmout_config(obj, 1);
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
    obj->pulsewidth_us = NU_CLAMP(us, 0, obj->period_us);
    pwmout_config(obj, 1);
}

static void pwmout_config(pwmout_t* obj, int start)
{
    EPWM_T *pwm_base = (EPWM_T *) NU_MODBASE(obj->pwm);
    uint32_t chn = NU_MODSUBINDEX(obj->pwm);

    // To avoid abnormal pulse on (re-)configuration, follow the sequence: stop/configure(/re-start).
    // NOTE: The issue is met in ARM mbed CI test tests-api-pwm on M487.
    EPWM_ForceStop(pwm_base, 1 << chn);

    // NOTE: Support period < 1s
    // NOTE: ARM mbed CI test fails due to first PWM pulse error. Workaround by:
    //       1. Inverse duty cycle (100 - duty)
    //       2. Inverse PWM output polarity
    //       This trick is here to pass ARM mbed CI test. First PWM pulse error still remains.
    EPWM_ConfigOutputChannel2(pwm_base, chn, 1000 * 1000, 100 - obj->pulsewidth_us * 100 / obj->period_us, obj->period_us);
    pwm_base->POLCTL |= 1 << (EPWM_POLCTL_PINV0_Pos + chn);

    if (start) {
        // Enable output of the specified PWM channel
        EPWM_EnableOutput(pwm_base, 1 << chn);
        EPWM_Start(pwm_base, 1 << chn);
    }
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}

#endif
