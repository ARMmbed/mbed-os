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

float   pwmout_calculate_duty(uint32_t width_cycles, uint32_t period_cycles);
void    pwmout_write_channel(uint32_t channel, float value);

uint32_t pwmout_get_channel_route(uint32_t channel)
{
    MBED_ASSERT(channel != (PWMName) NC);

    switch (channel) {
#ifdef TIMER_ROUTEPEN_CC0PEN
        case PWM_CH0:
            return TIMER_ROUTEPEN_CC0PEN;
            break;
        case PWM_CH1:
            return TIMER_ROUTEPEN_CC1PEN;
            break;
        case PWM_CH2:
            return TIMER_ROUTEPEN_CC2PEN;
            break;
        case PWM_CH3:
            return TIMER_ROUTEPEN_CC3PEN;
            break;
#else
        case PWM_CH0:
            return TIMER_ROUTE_CC0PEN;
            break;
        case PWM_CH1:
            return TIMER_ROUTE_CC1PEN;
            break;
        case PWM_CH2:
            return TIMER_ROUTE_CC2PEN;
            break;
#endif
        default:
            return 0;
    }
}

/*
* Disables the route location given. Returns true if it was enabled, false if it wasn't.
*/
bool pwmout_disable_channel_route(uint32_t routeloc) {
#ifdef TIMER_ROUTEPEN_CC0PEN
    if(PWM_TIMER->ROUTEPEN & routeloc) {
        //This channel was in use, so disable
        PWM_TIMER->ROUTEPEN &= ~routeloc;
        return true;
    }
#else
    if(PWM_TIMER->ROUTE & routeloc) {
        //This channel was in use, so disable
        PWM_TIMER->ROUTE &= ~routeloc;
        return true;
    }
#endif
    return false;
}

/*
* Check if a channel is active
*/
bool pwmout_channel_route_active(uint32_t routeloc) {
#ifdef TIMER_ROUTEPEN_CC0PEN
    if(PWM_TIMER->ROUTEPEN & routeloc) {
        return true;
    }
#else
    if(PWM_TIMER->ROUTE & routeloc) {
        return true;
    }
#endif
    return false;
}

/*
* Set the given route PEN flag
*/
void pwmout_set_channel_route(uint32_t routeloc) {
#ifdef TIMER_ROUTEPEN_CC0PEN
    PWM_TIMER->ROUTEPEN |= routeloc;
#else
    PWM_TIMER->ROUTE    |= routeloc;
#endif
}

/*
* Check if all routes are disabled
*/
bool pwmout_all_inactive(void) {
#ifdef TIMER_ROUTEPEN_CC0PEN
    if(PWM_TIMER->ROUTEPEN == _TIMER_ROUTEPEN_RESETVALUE) {
        return true;
    }
#else
    if(PWM_TIMER->ROUTE == PWM_ROUTE) {
        return true;
    }
#endif
    return false;
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

void pwmout_enable(pwmout_t *obj, uint8_t enable){
    if (enable) {
        // Set mode to PWM
        PWM_TIMER->CC[obj->channel].CTRL = TIMER_CC_CTRL_MODE_PWM;
    } else {
        // Set mode to default (== disabled)
        PWM_TIMER->CC[obj->channel].CTRL = _TIMER_CC_CTRL_MODE_DEFAULT;
    }
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

    // Set route enable
    if(pwmout_channel_route_active(pwmout_get_channel_route(obj->channel))) {
        //This channel was already in use
        //TODO: gracefully handle this case. mbed_error?
        return;
    } else {
        pwmout_set_channel_route(pwmout_get_channel_route(obj->channel));
        blockSleepMode(EM1);
        pwmout_enable(obj, true);
        pwmout_enable_pins(obj, true);
    }

    // Set route location
#ifdef _TIMER_ROUTELOC0_CC0LOC_LOC0
    switch (obj->channel) {
        case PWM_CH0:
            PWM_TIMER->ROUTELOC0 &= ~_TIMER_ROUTELOC0_CC0LOC_MASK;
            PWM_TIMER->ROUTELOC0 |= pinmap_find_function(pin,PinMap_PWM) << _TIMER_ROUTELOC0_CC0LOC_SHIFT;
            break;
        case PWM_CH1:
            PWM_TIMER->ROUTELOC0 &= ~_TIMER_ROUTELOC0_CC1LOC_MASK;
            PWM_TIMER->ROUTELOC0 |= pinmap_find_function(pin,PinMap_PWM)<< _TIMER_ROUTELOC0_CC1LOC_SHIFT;
            break;
        case PWM_CH2:
            PWM_TIMER->ROUTELOC0 &= ~_TIMER_ROUTELOC0_CC2LOC_MASK;
            PWM_TIMER->ROUTELOC0 |= pinmap_find_function(pin,PinMap_PWM) << _TIMER_ROUTELOC0_CC2LOC_SHIFT;
            break;
        case PWM_CH3:
            PWM_TIMER->ROUTELOC0 &= ~_TIMER_ROUTELOC0_CC3LOC_MASK;
            PWM_TIMER->ROUTELOC0 |= pinmap_find_function(pin,PinMap_PWM) << _TIMER_ROUTELOC0_CC3LOC_SHIFT;
            break;
        default:
            MBED_ASSERT(false);
    }
#else
    // On P1, the route location is statically defined for the entire timer.
    PWM_TIMER->ROUTE &= ~_TIMER_ROUTE_LOCATION_MASK;
    PWM_TIMER->ROUTE |= PWM_ROUTE;
#endif

    // Set default 20ms frequency and 0ms pulse width
    pwmout_period(obj, 0.02);
}

void pwmout_free(pwmout_t *obj)
{
    if(pwmout_disable_channel_route(pwmout_get_channel_route(obj->channel))) {
        //Channel was previously enabled, so do housekeeping
        unblockSleepMode(EM1);
    } else {
        //This channel was disabled already
    }
    
    pwmout_enable_pins(obj, false);
    
    if(pwmout_all_inactive()) {
        //Stop timer
        PWM_TIMER->CMD = TIMER_CMD_STOP;
        while(PWM_TIMER->STATUS & TIMER_STATUS_RUNNING);
        
        //Disable clock
        CMU_ClockEnable(PWM_TIMER_CLOCK, false);
    }
}

void pwmout_write(pwmout_t *obj, float value)
{
    pwmout_write_channel(obj->channel, value);
}

void pwmout_write_channel(uint32_t channel, float value) {
    uint32_t width_cycles = 0;
    if (value < 0.0f) {
        width_cycles = 0;
    } else if (value >= 1.0f) {
        width_cycles = PWM_TIMER->TOPB + 1;
    } else {
       width_cycles = (uint16_t)((float)PWM_TIMER->TOPB * value);
    }

    TIMER_CompareBufSet(PWM_TIMER, channel, width_cycles);
}

float pwmout_read(pwmout_t *obj)
{
    return pwmout_calculate_duty(TIMER_CaptureGet(PWM_TIMER, obj->channel), TIMER_TopGet(PWM_TIMER));
}

float pwmout_calculate_duty(uint32_t width_cycles, uint32_t period_cycles) {
    if(width_cycles > period_cycles) {
        return 1.0f;
    }
    else if (width_cycles == 0) {
        return 0.0f;
    }
    else {
        return (float) width_cycles / (float) period_cycles;
    }
}

// Set the PWM period, keeping the duty cycle the same.
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

    //Check if anything changed
    if(((PWM_TIMER->CTRL & ~_TIMER_CTRL_PRESC_MASK) == (pwm_prescaler_div << _TIMER_CTRL_PRESC_SHIFT)) && (TIMER_TopGet(PWM_TIMER) == cycles)) return;

    //Save previous period for recalculation of duty cycles
    uint32_t previous_period_cycles = PWM_TIMER->TOPB;

    //Set prescaler
    PWM_TIMER->CTRL = (PWM_TIMER->CTRL & ~_TIMER_CTRL_PRESC_MASK) | (pwm_prescaler_div << _TIMER_CTRL_PRESC_SHIFT);

    //Set Top Value, which controls the PWM period
    TIMER_TopBufSet(PWM_TIMER, cycles);

    //For each active channel, re-calculate the compare value
    uint32_t channel = 0;
    while(pwmout_get_channel_route(channel) != 0) {
        if(pwmout_channel_route_active(channel)) {
            //recalc and reset compare value
            pwmout_write_channel(channel, pwmout_calculate_duty(PWM_TIMER->CC[channel].CCVB, previous_period_cycles));
        }
        channel++;
    }
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
    uint16_t width_cycles = (uint16_t) (((float) (REFERENCE_FREQUENCY >> pwm_prescaler_div)) * seconds);
    TIMER_CompareBufSet(PWM_TIMER, obj->channel, width_cycles);
}

void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
    uint16_t width_cycles = (uint16_t) ((REFERENCE_FREQUENCY >> pwm_prescaler_div) * ms) / 1000;
    TIMER_CompareBufSet(PWM_TIMER, obj->channel, width_cycles);
}

void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
    uint16_t width_cycles = (uint16_t) ((REFERENCE_FREQUENCY >> pwm_prescaler_div) * us) / 1000000;
    TIMER_CompareBufSet(PWM_TIMER, obj->channel, width_cycles);
}

#endif
