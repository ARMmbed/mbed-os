/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
     
#include "device.h"
#include "objects.h"
#include "pinmap.h"

#ifdef CONFIG_MBED_ENABLED
#include "platform_stdlib.h"
#endif

#if DEVICE_PWMOUT

#ifdef CONFIG_PWM_EN
#include "pwmout_api.h"

#ifdef CONFIG_MBED_ENABLED
#include "PeripheralPins.h"
#endif

void pwmout_init(pwmout_t* obj, PinName pin) 
{
    int peripheral;
    u32 pwm_idx;
    u32 pin_sel;

    DBG_PWM_INFO("%s: Init PWM for pin(0x%x)\n", __FUNCTION__, pin);
    
    // Get the peripheral name from the pin and assign it to the object
    peripheral = pinmap_peripheral(pin, PinMap_PWM);

    if (unlikely(peripheral == NC)) {
        DBG_PWM_ERR("%s: Cannot find matched pwm for this pin(0x%x)\n", __FUNCTION__, pin);
        return;
    }

    pwm_idx = RTL_GET_PERI_IDX(peripheral);
    pin_sel = RTL_GET_PERI_SEL(peripheral);

    obj->pwm_idx = pwm_idx;
    obj->pin_sel = pin_sel;
    obj->period = 0;
    obj->pulse = 0;
    _memset((void *)&obj->pwm_hal_adp, 0, sizeof(HAL_PWM_ADAPTER));
    if (HAL_OK != HAL_Pwm_Init(&obj->pwm_hal_adp, pwm_idx, pin_sel)) {
        DBG_PWM_ERR("pwmout_init Err!\n");
        return;
    }
    pwmout_period_us(obj, 20000); // 20 ms per default
    HAL_Pwm_Enable(&obj->pwm_hal_adp);
}

void pwmout_free(pwmout_t* obj) 
{
    HAL_Pwm_Disable(&obj->pwm_hal_adp);
}

void pwmout_write(pwmout_t* obj, float value) 
{
    if (value < (float)0.0) {
        value = 0.0;
    } else if (value > (float)1.0) {
        value = 1.0;
    }

    obj->pulse = (uint32_t)((float)obj->period * value);
    HAL_Pwm_SetDuty(&obj->pwm_hal_adp, obj->period, obj->pulse);
}

float pwmout_read(pwmout_t* obj) 
{
    float value = 0;
    if (obj->period > 0) {
        value = (float)(obj->pulse) / (float)(obj->period);
    }
    return ((value > (float)1.0) ? (float)(1.0) : (value));
}

void pwmout_period(pwmout_t* obj, float seconds) 
{
    pwmout_period_us(obj, (int)(seconds * 1000000.0f));
}

void pwmout_period_ms(pwmout_t* obj, int ms) 
{
    pwmout_period_us(obj, (int)(ms * 1000));
}

void pwmout_period_us(pwmout_t* obj, int us) 
{
    float dc = pwmout_read(obj);

    obj->period = us;
    // Set duty cycle again
    pwmout_write(obj, dc);
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) 
{
    pwmout_pulsewidth_us(obj, (int)(seconds * 1000000.0f));
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) 
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) 
{
    float value = (float)us / (float)obj->period;
    pwmout_write(obj, value);
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}

#endif // #ifdef CONFIG_PWM_EN
#endif // #if DEVICE_PWMOUT
