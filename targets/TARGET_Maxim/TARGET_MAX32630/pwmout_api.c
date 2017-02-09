/*******************************************************************************
 * Copyright (c) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#include "mbed_assert.h"
#include "cmsis.h"
#include "pwmout_api.h"
#include "pinmap.h"
#include "clkman_regs.h"
#include "PeripheralPins.h"

#define MXC_GPIO_OUT_MODE_FIELD_WIDTH 4
#define MXC_GPIO_OUT_MODE_FIELD_MASK  ((uint32_t)0xFFFFFFFF >> (32 - MXC_GPIO_OUT_MODE_FIELD_WIDTH))
#define MXC_GPIO_FUNC_SEL_FIELD_WIDTH 4
#define MXC_GPIO_FUNC_SEL_FIELD_MASK  ((uint32_t)0xFFFFFFFF >> (32 - MXC_GPIO_FUNC_SEL_FIELD_WIDTH))


//******************************************************************************
void pwmout_init(pwmout_t* obj, PinName pin)
{
    // Make sure the pin is free for GPIO use
    unsigned int port = (unsigned int)pin >> PORT_SHIFT;
    unsigned int port_pin = (unsigned int)pin & ~(0xFFFFFFFF << PORT_SHIFT);
    MBED_ASSERT(MXC_GPIO->free[port] & (0x1 << port_pin));

    int i = 0;
    PinMap pwm = PinMap_PWM[0];

    // Check if there is a pulse train already active on this port
    int pin_func = (MXC_GPIO->func_sel[port] & (MXC_GPIO_FUNC_SEL_FIELD_MASK << (port_pin * MXC_GPIO_FUNC_SEL_FIELD_WIDTH))) >>
                   (port_pin * MXC_GPIO_FUNC_SEL_FIELD_WIDTH);
    MBED_ASSERT((pin_func < 1) || (pin_func > 3));

    // Search through PinMap_PWM to find the pin
    while (pwm.pin != pin) {
        pwm = PinMap_PWM[++i];
    }

    // Find a free PT instance on this pin
    while (pwm.pin == pin) {

        // Check to see if this PT instance is free
        if (((mxc_pt_regs_t*)pwm.peripheral)->rate_length & MXC_F_PT_RATE_LENGTH_MODE) {
            break;
        }

        pwm = PinMap_PWM[++i];

        // Raise an assertion if we can not allocate another PT instance.
        MBED_ASSERT(pwm.pin == pin);
    }

    // Enable the clock
    MXC_CLKMAN->sys_clk_ctrl_7_pt = MXC_S_CLKMAN_CLK_SCALE_DIV_1;

    // Set the obj pointer to the propper PWM instance
    obj->pwm = (mxc_pt_regs_t*)pwm.peripheral;

    // Initialize object period and pulse width
    obj->period = -1;
    obj->pulse_width = -1;

    // Disable the output
    obj->pwm->train = 0x0;
    obj->pwm->rate_length = 0x0;

    // Configure the pin
    pin_mode(pin, (PinMode)PullNone);
    pin_function(pin, pwm.function);

    // default to 20ms: standard for servos, and fine for e.g. brightness control
    pwmout_period_us(obj, 20000);
    pwmout_write    (obj, 0);

    // Set the drive mode to normal
    MXC_SET_FIELD(&MXC_GPIO->out_mode[port],
                  (MXC_GPIO_OUT_MODE_FIELD_MASK << (port_pin * MXC_GPIO_OUT_MODE_FIELD_WIDTH)),
                  (MXC_V_GPIO_OUT_MODE_NORMAL << (port_pin * MXC_GPIO_OUT_MODE_FIELD_WIDTH)));

    // Enable this PWM channel
    MXC_PTG->enable |= (1 << MXC_PT_GET_IDX(obj->pwm));
}

//******************************************************************************
void pwmout_free(pwmout_t* obj)
{
    // Set the registers to the reset value
    obj->pwm->train = 0;
    obj->pwm->rate_length = 0x08000000;
}

//******************************************************************************
static void pwmout_update(pwmout_t* obj)
{
    // Calculate and set the divider ratio
    int div = (obj->period * (SystemCoreClock / 1000000))/32;
    if (div < 2) {
        div = 2;
    }
    MXC_SET_FIELD(&obj->pwm->rate_length, MXC_F_PT_RATE_LENGTH_RATE_CONTROL, div);

    // Change the duty cycle to adjust the pulse width
    obj->pwm->train = (0xFFFFFFFF << (32 - ((32 * obj->pulse_width) / obj->period)));
}


//******************************************************************************
void pwmout_write(pwmout_t* obj, float percent)
{
    // Saturate percent if outside of range
    if(percent < 0.0f) {
        percent = 0.0f;
    } else if(percent > 1.0f) {
        percent = 1.0f;
    }

    // Resize the pulse width to set the duty cycle
    pwmout_pulsewidth_us(obj, (int)(percent*obj->period));
}

//******************************************************************************
float pwmout_read(pwmout_t* obj)
{
    // Check for when pulsewidth or period equals 0
    if((obj->pulse_width == 0) || (obj->period == 0)) {
        return 0;
    }

    // Return the duty cycle
    return ((float)obj->pulse_width / (float)obj->period);
}

//******************************************************************************
void pwmout_period(pwmout_t* obj, float seconds)
{
    pwmout_period_us(obj, (int)(seconds * 1000000.0f));
}

//******************************************************************************
void pwmout_period_ms(pwmout_t* obj, int ms)
{
    pwmout_period_us(obj, ms * 1000);
}

//******************************************************************************
void pwmout_period_us(pwmout_t* obj, int us)
{
    // Check the range of the period
    MBED_ASSERT((us >= 0) && (us <= (int)(SystemCoreClock / 32)));

    // Set pulse width to half the period if uninitialized
    if (obj->pulse_width == -1) {
        obj->pulse_width = us / 2;
    }

    // Save the period
    obj->period = us;

    // Update the registers
    pwmout_update(obj);
}

//******************************************************************************
void pwmout_pulsewidth(pwmout_t* obj, float seconds)
{
    pwmout_pulsewidth_us(obj, (int)(seconds * 1000000.0f));
}

//******************************************************************************
void pwmout_pulsewidth_ms(pwmout_t* obj, int ms)
{
    pwmout_pulsewidth_us(obj, ms * 1000);
}

//******************************************************************************
void pwmout_pulsewidth_us(pwmout_t* obj, int us)
{
    // Check the range of the pulsewidth
    MBED_ASSERT((us >= 0) && (us <= (int)(SystemCoreClock / 32)));

    // Initialize period to double the pulsewidth if uninitialized
    if (obj->period == -1) {
        obj->period = 2 * us;
    }

    // Save the pulsewidth
    obj->pulse_width = us;

    // Update the register
    pwmout_update(obj);
}

