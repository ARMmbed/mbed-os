/***************************************************************************//**
 * @file pwmout_api.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#include "device.h"
#include "clocking.h"
#if DEVICE_PWMOUT

#include "mbed_assert.h"
#include "pwmout_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "device_peripherals.h"
#include "sleepmodes.h"

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_timer.h"

static int pwm_prescaler_div;

uint32_t pwmout_get_channel_route(pwmout_t *obj)
{
    MBED_ASSERT(obj->channel != (PWMName) NC);

    switch (obj->channel) {
        case PWM_CH0:
            return TIMER_ROUTE_CC0PEN;
            break;
        case PWM_CH1:
            return TIMER_ROUTE_CC1PEN;
            break;
        case PWM_CH2:
            return TIMER_ROUTE_CC2PEN;
            break;
        default:
            return 0;
    }
}

void pwmout_enable_pins(pwmout_t *obj, uint8_t enable)
{
    if (enable) {
        pin_mode(obj->pin, PushPull);
    } else {
        // TODO_LP return PinMode to the previous state
        pin_mode(obj->pin, Disabled);
    }
}

void pwmout_enable(pwmout_t *obj, uint8_t enable)
{
    /* Start with default CC (Compare/Capture) channel parameters */
    TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;
    if (enable) {
        /* Set mode to PWM */
        timerCCInit.mode = timerCCModePWM;
    }

    /* Configure CC channel */
    TIMER_InitCC(PWM_TIMER, obj->channel, &timerCCInit);
}

void pwmout_init(pwmout_t *obj, PinName pin)
{
    obj->channel = (PWMName) pinmap_peripheral(pin, PinMap_PWM);
    obj->pin = pin;
    MBED_ASSERT(obj->channel != (PWMName) NC);

    /* Turn on clock */
    CMU_ClockEnable(PWM_TIMER_CLOCK, true);

    /* Turn on timer */
    if(!(PWM_TIMER->STATUS & TIMER_STATUS_RUNNING)) {
        TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
        TIMER_Init(PWM_TIMER, &timerInit);
    }

    /* Enable correct channel */
    uint32_t routeloc = pwmout_get_channel_route(obj);
    if(PWM_TIMER->ROUTE & routeloc) {
        //This channel was already in use
        //TODO: gracefully handle this case
    } else {
        //This channel was unused up to now
        PWM_TIMER->ROUTE |= routeloc;
        blockSleepMode(EM1);

        //TODO: check if any channel was up already, then don't re-init timer
        pwmout_enable(obj, true);
        pwmout_enable_pins(obj, true);
    }

    /* Route correct channel to location 1 */
    PWM_TIMER->ROUTE &= ~_TIMER_ROUTE_LOCATION_MASK;
    PWM_TIMER->ROUTE |= PWM_ROUTE;

    /* Set default 20ms frequency and 0ms pulse width */
    pwmout_period(obj, 0.02);
}

void pwmout_free(pwmout_t *obj)
{
    uint32_t routeloc = pwmout_get_channel_route(obj);
    if(PWM_TIMER->ROUTE & routeloc) {
        //This channel was in use, so disable
        PWM_TIMER->ROUTE &= ~routeloc;
        pwmout_enable_pins(obj, false);
        unblockSleepMode(EM1);

        //TODO: check if all channels are down, then switch off timer
    } else {
        //This channel was disabled already
    }
}

void pwmout_write(pwmout_t *obj, float value)
{
    if (value < 0.0f) {
        value = 0;
    } else if (value > 1.0f) {
        value = 1;
    }

    float pulse_period_in_s = obj->period_cycles / ((float) (REFERENCE_FREQUENCY >> pwm_prescaler_div));
    pwmout_pulsewidth(obj, value * pulse_period_in_s);
}

float pwmout_read(pwmout_t *obj)
{
    return obj->width_cycles / (float) obj->period_cycles;
}

// Set the PWM period, keeping the absolute pulse width the same.
void pwmout_period(pwmout_t *obj, float seconds)
{
    // Find the lowest prescaler divider possible.
    // This gives us max resolution for a given period

    //The value of the top register if prescaler is set to 0
    int cycles = REFERENCE_FREQUENCY * seconds;
    pwm_prescaler_div = 0;

    //The top register is only 16 bits, so we keep dividing till we are below 0xFFFF
    while (cycles > 0xFFFF) {
        cycles /= 2;
        pwm_prescaler_div++;

        //Max pwm_prescaler_div supported is 10
        if (pwm_prescaler_div > 10) {
            pwm_prescaler_div = 10;
            cycles = 0xFFFF; //Set it to max possible value;
            break;
        }
    }

    obj->period_cycles = cycles;

    //Set prescaler
    PWM_TIMER->CTRL = (PWM_TIMER->CTRL & ~_TIMER_CTRL_PRESC_MASK) | (pwm_prescaler_div << _TIMER_CTRL_PRESC_SHIFT);

    /* Set Top Value, which controls the PWM period */
    TIMER_TopSet(PWM_TIMER, obj->period_cycles);
}

void pwmout_period_ms(pwmout_t *obj, int ms)
{
    pwmout_period(obj, ms / 1000.0f);
}

void pwmout_period_us(pwmout_t *obj, int us)
{
    pwmout_period(obj, us / 1000000.0f);
}

void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
    obj->width_cycles = (uint32_t) (((float) (REFERENCE_FREQUENCY >> pwm_prescaler_div)) * seconds);
    TIMER_CompareBufSet(PWM_TIMER, obj->channel, obj->width_cycles);
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    obj->width_cycles = (uint32_t) ((REFERENCE_FREQUENCY >> pwm_prescaler_div) * ms) / 1000;
    TIMER_CompareBufSet(PWM_TIMER, obj->channel, obj->width_cycles);
}

void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    obj->width_cycles = (uint32_t) ((REFERENCE_FREQUENCY >> pwm_prescaler_div) * us) / 1000000;
    TIMER_CompareBufSet(PWM_TIMER, obj->channel, obj->width_cycles);
}

#endif
