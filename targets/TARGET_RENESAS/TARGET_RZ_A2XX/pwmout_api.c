/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
#if DEVICE_PWMOUT
#include "mbed_assert.h"
#include "pwmout_api.h"
#include "cmsis.h"
#include "PeripheralPins.h"
#include "RZ_A2_Init.h"
#include "iodefine.h"
#include "gpio_addrdefine.h"
#include "mbed_drv_cfg.h"

static const volatile struct st_gpt32e *GPT32E[] = {
    &GPT32E0, &GPT32E1, &GPT32E2, &GPT32E3,
    &GPT32E4, &GPT32E5, &GPT32E6, &GPT32E7
};

void pwmout_init(pwmout_t *obj, PinName pin)
{
    // determine the channel
    PWMName index = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(index != (PWMName)NC);

    CPG.STBCR3.BIT.MSTP30 = 0;

    pinmap_pinout(pin, PinMap_PWM);

    obj->index = index;
    obj->type  = (index & 0x01);
    obj->pwm   = (volatile struct st_gpt32e *)GPT32E[(index >> 1)];
    obj->duty  = 0.0f;

    /* Set operating mode : saw-wave PWM mode */
    obj->pwm->GTCR.BIT.MD = 0x0;

    /* Set count direction : up */
    obj->pwm->GTUDDTYC.BIT.UDF = 0x1;
    obj->pwm->GTUDDTYC.BIT.UD  = 0x1;
    obj->pwm->GTUDDTYC.BIT.UDF = 0x0;

    pwmout_period_us(obj, 1000);

    if (obj->type == 0) {
        obj->pwm->GTIOR.BIT.GTIOA = 0x19;  /* Set GTIOC pin function */
        obj->pwm->GTBER.BIT.CCRA  = 0x01;  /* Set buffer operation : Single buffer operation */
        obj->pwm->GTCCRA.LONG     = 0;     /* Set compare match value */
        obj->pwm->GTIOR.BIT.OAE   = 0x1;   /* Enable GTIOC pin output */
    } else {
        obj->pwm->GTIOR.BIT.GTIOB = 0x19;  /* Set GTIOC pin function */
        obj->pwm->GTBER.BIT.CCRB  = 0x01;  /* Set buffer operation : Single buffer operation */
        obj->pwm->GTCCRB.LONG     = 0;     /* Set compare match value */
        obj->pwm->GTIOR.BIT.OBE   = 0x1;   /* Enable GTIOC pin output */
    }

    /* Start count operation */
    obj->pwm->GTCR.BIT.CST = 0x01;
}

void pwmout_free(pwmout_t *obj)
{
    pwmout_write(obj, 0);
}

void pwmout_write(pwmout_t *obj, float value)
{
    if (value < 0.0f) {
        value = 0.0f;
    } else if (value > 1.0f) {
        value = 1.0f;
    } else {
        // Do Nothing
    }

    obj->duty = value;

    if (obj->type == 0) {
        obj->pwm->GTCCRC.LONG = obj->pwm->GTPR.LONG * value; /* Set buffer value */
        obj->pwm->GTUDDTYC.BIT.OADTYF = 0x1;
        if (value == 0.0f) {
            obj->pwm->GTUDDTYC.BIT.OADTY = 0x2;
        } else if (value == 1.0f) {
            obj->pwm->GTUDDTYC.BIT.OADTY = 0x3;
        } else {
            obj->pwm->GTUDDTYC.BIT.OADTY = 0x0;
        }
        obj->pwm->GTUDDTYC.BIT.OADTYF = 0x0;
    } else {
        obj->pwm->GTCCRE.LONG = obj->pwm->GTPR.LONG * value; /* Set buffer value */
        obj->pwm->GTUDDTYC.BIT.OBDTYF = 0x1;
        if (value == 0.0f) {
            obj->pwm->GTUDDTYC.BIT.OBDTY = 0x2;
        } else if (value == 1.0f) {
            obj->pwm->GTUDDTYC.BIT.OBDTY = 0x3;
        } else {
            obj->pwm->GTUDDTYC.BIT.OBDTY = 0x0;
        }
        obj->pwm->GTUDDTYC.BIT.OBDTYF = 0x0;
    }
}

float pwmout_read(pwmout_t *obj)
{
    float value;

    if (obj->type == 0) {
        value = (float)obj->pwm->GTCCRC.LONG / (float)obj->pwm->GTPR.LONG;
    } else {
        value = (float)obj->pwm->GTCCRE.LONG / (float)obj->pwm->GTPR.LONG;
    }

    return (value > 1.0f) ? (1.0f) : (value);
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
    uint32_t pclk_base;
    uint32_t wk_cycle;

    if (RZ_A2_IsClockMode0() == false) {
        pclk_base = (uint32_t)CM1_RENESAS_RZ_A2_P1_CLK / 1000000;
    } else {
        pclk_base = (uint32_t)CM0_RENESAS_RZ_A2_P1_CLK / 1000000;
    }

    uint32_t us_max = 0xFFFFFFFF / pclk_base;

    if ((uint32_t)us > us_max) {
        us = us_max;
    } else if (us < 1) {
        us = 1;
    } else {
        // Do Nothing
    }

    wk_cycle = pclk_base * us;
    obj->pwm->GTCR.BIT.TPCS = 0x0;        /* Select count clock */
    obj->pwm->GTPR.LONG = wk_cycle - 1;   /* Set cycle */
    obj->pwm->GTCNT.LONG = 0;             /* Set initial value for counter */

    /* set duty again */
    pwmout_write(obj, obj->duty);
}

int pwmout_read_period_us(pwmout_t *obj)
{
    return obj->pwm->GTPR.LONG;
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
    float value = 0;

    if (RZ_A2_IsClockMode0() == false) {
        value = (float)(us * ((uint32_t)CM0_RENESAS_RZ_A2_P0_CLK / 1000000)) / (float)(obj->pwm->GTPR.LONG + 1);
    } else {
        value = (float)(us * ((uint32_t)CM1_RENESAS_RZ_A2_P0_CLK / 1000000)) / (float)(obj->pwm->GTPR.LONG + 1);
    }
    pwmout_write(obj, value);
}

int pwmout_read_pulsewidth_us(pwmout_t *obj)
{
    if (obj->type == 0) {
        return (float)obj->pwm->GTCCRC.LONG;
    }
    return (float)obj->pwm->GTCCRE.LONG ;
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}
#endif
