/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
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

#include "apm32f4xx_tmr.h"
#include "pwmout_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"

static uint32_t get_timer_clock(uint32_t timer_periph);
static void pwmout_dev_init(pwmout_t *obj);

 /*!
 * @brief     Initialize the pwm out peripheral and configure the pin
 *
 * @param     obj : The pwmout object to initialize
 *
 * @param     pin : The pwmout pin to initialize
 *
 * @retval    None
 */
void pwmout_init(pwmout_t *obj, PinName pin)
{

    MBED_ASSERT(obj);

    obj->pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
    MBED_ASSERT(obj->pwm != (PWMName)NC);

    uint32_t function = pinmap_function(pin, PinMap_PWM);
    MBED_ASSERT(function != (uint32_t)NC);
    obj->ch = APM_PIN_CHANNEL_GET(function);
    /* Peripheral initialization */
    pwmout_dev_init(obj);
    /* pin function initialization */
    pinmap_pinout(pin, PinMap_PWM);
}

 /*!
 * @brief     Deinitialize the pwmout object
 *
 * @param     obj : The pwmout object to initialize
 *
 * @retval    None
 */
void pwmout_free(pwmout_t *obj)
{
    TMR_T *tmr = (TMR_T *)obj->pwm;
    TMR_DisableCCxChannel(tmr, obj->ch);
}

 /*!
 * @brief     Set the output duty-cycle in range <0.0f, 1.0f>,
 *
 * @param     obj : The pwmout object
 *
 * @param     percent : The floating-point percentage number
 *
 * @retval    None
 */
void pwmout_write(pwmout_t *obj, float percent)
{
    TMR_T *tmr = (TMR_T *)obj->pwm;
    uint16_t period;
    uint16_t pulse;

    TMR_Disable(tmr);
    /* overflow protection */
    if (percent < (float)0.0)
    {
        percent = 0.0;
    }
    else if (percent > (float)1.0)
    {
        percent = 1.0;
    }
    period = (tmr)->AUTORLD;
    pulse = (uint16_t)((period + 1) * percent);

    switch (obj->ch)
    {
        case 0:
            TMR_ConfigCompare1(tmr, pulse);
            break;
        case 1:
            TMR_ConfigCompare2(tmr, pulse);
            break;
        case 2:
            TMR_ConfigCompare3(tmr, pulse);
            break;
        case 3:
            TMR_ConfigCompare4(tmr, pulse);
            break;
        default:
            error("error channel");
            break;
    }

    TMR_Enable(tmr);
}

 /*!
 * @brief     Read the current float-point output duty-cycle
 *
 * @param     obj : The pwmout object
 *
 * @retval    A floating-point output duty-cycle
 */
float pwmout_read(pwmout_t *obj)
{
    float value = 0;
    uint16_t period;
    uint16_t pulse;
    TMR_T *tmr = (TMR_T *)obj->pwm;

    period = tmr->AUTORLD;

    switch (obj->ch)
    {
        case 0:
            pulse = tmr->CC1;
            break;

        case 1:
            pulse = tmr->CC2;
            break;

        case 2:
            pulse = tmr->CC3;
            break;

        case 3:
            pulse = tmr->CC4;
            break;

        default:
            error("Error: pwm channel error! \r\n");
    }

    /* calculated waveform duty ratio */
    value = (float)(pulse) / (float)(period + 1);

    if (value > (float)1.0) 
    {
        value = (float)1.0;
    }

    return value;
}

 /*!
 * @brief     Set the PWM period specified in seconds, keeping the duty cycle the same
 *
 * @param     obj : The pwmout object
 *
 * @param     seconds : The floating-point seconds period
 *
 * @retval    None
 */
void pwmout_period(pwmout_t *obj, float seconds)
{
    pwmout_period_us(obj, seconds * 1000000.0f);
}

 /*!
 * @brief     Set the PWM period specified in miliseconds, keeping the duty cycle the same
 *
 * @param     obj : The pwmout object
 *
 * @param     ms : The milisecond period
 *
 * @retval    None
 */
void pwmout_period_ms(pwmout_t *obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

 /*!
 * @brief     Set the PWM period specified in miliseconds, keeping the duty cycle the same
 *
 * @param     obj : The pwmout object
 *
 * @param     us : The milisecond period
 *
 * @retval    None
 */
void pwmout_period_us(pwmout_t *obj, int us)
{
    uint32_t ultemp = 0;
    uint32_t timer_clk = 0;
    uint32_t period = us - 1;
    uint32_t prescaler;
    float duty_ratio;
    TMR_T *tmr = (TMR_T *)obj->pwm;

    duty_ratio = pwmout_read(obj);

    TMR_Disable(tmr);

    timer_clk = get_timer_clock(obj->pwm);

    ultemp = (timer_clk / 1000000);
    prescaler = ultemp;
    obj->cnt_unit = 1;

    while (period > 0xFFFF) 
    {
        obj->cnt_unit = obj->cnt_unit << 1;
        period = period >> 1;
        prescaler = ultemp * obj->cnt_unit;
    }

    if (prescaler > 0xFFFF) 
    {
        error("Error: TIMER prescaler value is overflow \r\n");
    }

    TMR_ConfigAutoreload(tmr, period);
    TMR_ConfigPrescaler(tmr, prescaler - 1, TMR_PSC_RELOAD_IMMEDIATE);

    ultemp = duty_ratio * us;

    pwmout_pulsewidth_us(obj, ultemp);
}

 /*!
 * @brief     Return the Autorld
 *
 * @param     obj : The pwmout object
 *
 * @retval    Autorld
 */
int pwmout_read_period_us(pwmout_t *obj)
{
    TMR_T *tmr = (TMR_T *)obj->pwm;
    return tmr->AUTORLD;
}

 /*!
 * @brief     Set the PWM pulsewidth specified in seconds, keeping the period the same.
 *
 * @param     obj : The pwmout object
 *
 * @param     seconds : The floating-point pulsewidth in seconds
 *
 * @retval    None
 */
void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

 /*!
 * @brief     Set the PWM pulsewidth specified in miliseconds, keeping the period the same.
 *
 * @param     obj : The pwmout object
 *
 * @param     ms  : The floating-point pulsewidth in miliseconds
 *
 * @retval    None
 */
void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

 /*!
 * @brief     Set the PWM pulsewidth specified in microseconds, keeping the period the same.
 *
 * @param     obj : The pwmout object
 *
 * @param     us  : The floating-point pulsewidth in microseconds
 *
 * @retval    None
 */
void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    uint32_t pulse;
    uint32_t period;
    TMR_T *tmr = (TMR_T *)obj->pwm;

    period = ((TMR_T *)obj->pwm)->AUTORLD;
    pulse = us / obj->cnt_unit;

    if (pulse > period)
    {
        pulse = period;
    }
    
    switch (obj->ch) 
    {
        case 0:
            TMR_ConfigCompare1(tmr, pulse);
            break;

        case 1:
            TMR_ConfigCompare2(tmr, pulse);
            break;

        case 2:
            TMR_ConfigCompare3(tmr, pulse);
            break;

        case 3:
            TMR_ConfigCompare4(tmr, pulse);
            break;

        default:
            error("Error: pwm channel error! \r\n");
    }
}

 /*!
 * @brief     Read the PWM pulsewidth in microseconds.
 *
 * @param     obj : The pwmout object
 *
 * @retval    Pulse
 */
int pwmout_read_pulsewidth_us(pwmout_t *obj)
{
    uint16_t pulse = 0;
    TMR_T *tmr = (TMR_T *)obj->pwm;

    switch (obj->ch) 
    {
        case 0:
            pulse = tmr->CC1;
            break;

        case 1:
            pulse = tmr->CC2;
            break;

        case 2:
            pulse = tmr->CC3;
            break;

        case 3:
            pulse = tmr->CC4;
            break;

        default:
            error("Error: pwm channel error! \r\n");
    }
    return pulse;
}

 /*!
 * @brief     Get the timer clock frequent.
 *
 * @param     timer_periph : tmr
 *
 * @retval    timerclk
 */
static uint32_t get_timer_clock(uint32_t timer_periph)
{
    uint32_t timerclk;
    uint32_t NOGET = 0;
    uint32_t GET = 1;

    if (((uint32_t)TMR1 == timer_periph) || ((uint32_t)TMR8 == timer_periph) ||
         ((uint32_t)TMR9 == timer_periph) || ((uint32_t)TMR10 == timer_periph) ||
         ((uint32_t)TMR11 == timer_periph)) 
    {
        /* get the current APB2 TIMER clock source */
        if (RCM_APB_DIV_1 == (RCM->CFG_B.APB2PSC))
        {
            RCM_ReadPCLKFreq(&NOGET, &GET);
            timerclk = GET;
        }
        else
        {
            RCM_ReadPCLKFreq(&NOGET, &GET);
            timerclk = GET * 2;
        }
    }
    else
    {
        /* get the current APB1 TIMER clock source */
        if (RCM_APB_DIV_1 == (RCM->CFG_B.APB1PSC))
        {
            RCM_ReadPCLKFreq(&GET, &NOGET);
            timerclk = GET;
        }
        else
        {
            RCM_ReadPCLKFreq(&GET, &NOGET);
            timerclk = GET * 2;
        }
    }

    return timerclk;
}

 /*!
 * @brief     Init the timer clock.
 *
 * @param     obj : The pwmout object
 *
 * @retval    None
 */
static void pwmout_dev_init(pwmout_t *obj)
{
    TMR_T *tmr = (TMR_T *)obj->pwm;

    MBED_ASSERT(obj);
    switch (obj->pwm)
    {
        case PWM_1:
            RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR1);
            break;

        case PWM_2:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2);
            break;

        case PWM_3:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR3);
            break;

        case PWM_4:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR4);
            break;

        case PWM_5:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR5);
            break;

        case PWM_6:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR6);
            break;

        case PWM_7:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR7);
            break;

        case PWM_8:
            RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR8);
            break;
        case PWM_9:
            RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR9);
            break;

        case PWM_10:
            RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR10);
            break;

        case PWM_11:
            RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR11);
            break;

        case PWM_12:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR12);
            break;

        case PWM_13:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR13);
            break;

        case PWM_14:
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR14);
            break;
    }
    /* configure TIMER base function */
    TMR_BaseConfig_T TMR_BaseConfigStruct;
    
    TMR_BaseConfigStruct.division             = 167;
    TMR_BaseConfigStruct.period               = 9999;
    TMR_BaseConfigStruct.clockDivision        = TMR_CLOCK_DIV_1;
    TMR_BaseConfigStruct.repetitionCounter    = 0;
    TMR_BaseConfigStruct.countMode            = TMR_COUNTER_MODE_UP;

    TMR_ConfigTimeBase(tmr, &TMR_BaseConfigStruct);

    /* configure TIMER channel output function */
    TMR_OCConfig_T TMR_OCConfigStruct;

    TMR_OCConfigStruct.mode = TMR_OC_MODE_PWM1;
    TMR_OCConfigStruct.polarity = TMR_OC_POLARITY_HIGH;
    TMR_OCConfigStruct.nPolarity = TMR_OC_NPOLARITY_LOW;
    TMR_OCConfigStruct.outputState = TMR_OC_STATE_ENABLE;
    TMR_OCConfigStruct.outputNState = TMR_OC_NSTATE_ENABLE;
    TMR_OCConfigStruct.idleState = TMR_OC_IDLE_STATE_RESET;
    TMR_OCConfigStruct.nIdleState = TMR_OC_NIDLE_STATE_RESET;
    TMR_OCConfigStruct.pulse = 5000;
    switch(obj->ch)
    {
        case 0:
            TMR_ConfigOC1(tmr, &TMR_OCConfigStruct);
        break;
        
        case 1:
            TMR_ConfigOC2(tmr, &TMR_OCConfigStruct);
        break;
        
        case 2:
            TMR_ConfigOC3(tmr, &TMR_OCConfigStruct);
        break;
        
        case 3:
            TMR_ConfigOC4(tmr, &TMR_OCConfigStruct);
        break;
        
        default:
            error("error");
    }
    TMR_ConfigOC1Fast(tmr, TMR_OC_FAST_DISABLE);
    TMR_EnablePWMOutputs(tmr);
}

const PinMap *pwmout_pinmap()
{
    return PinMap_PWM;
}
