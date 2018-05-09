/*******************************************************************************
 * Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
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
#include "gpio_api.h"
#include "pinmap.h"
#include "gpio_regs.h"
#include "clkman_regs.h"

uint32_t gpio_set(PinName name)
{
    MBED_ASSERT(name != (PinName)NC);
    pin_function(name, 0);
    return 1 << PINNAME_TO_PIN(name);
}

void gpio_init(gpio_t *obj, PinName name)
{
    obj->name = name;
    if (name == (PinName)NC) {
        return;
    }

    unsigned int port = PINNAME_TO_PORT(name);
    unsigned int pin = PINNAME_TO_PIN(name);

    obj->reg_out = (uint32_t*)BITBAND(&MXC_GPIO->out_val[port], pin);
    obj->reg_in = (uint32_t*)BITBAND(&MXC_GPIO->in_val[port], pin);
    obj->mode = PullDefault;

    /* Ensure that the GPIO clock is enabled */
    MXC_CLKMAN->clk_gate_ctrl1 |= MXC_F_CLKMAN_CLK_GATE_CTRL1_GPIO_CLK_GATER;

    /* Ensure that the GPIO clock is enabled */
    MXC_CLKMAN->sys_clk_ctrl_6_gpio = MXC_S_CLKMAN_CLK_SCALE_DIV_1;
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
#ifdef OPEN_DRAIN_LEDS
    if ((obj->name == LED1) || (obj->name == LED2) ||
        (obj->name == LED3) || (obj->name == LED4)) {
        mode = OpenDrain;
#ifdef TARGET_MAX32620FTHR
        unsigned int port = PINNAME_TO_PORT(obj->name);
        unsigned int pin = PINNAME_TO_PIN(obj->name);
        if (port < 4) {
            MXC_IOMAN->use_vddioh_0 |= (1 << ((port * 8) + pin));
        } else if (port < 7) {
            MXC_IOMAN->use_vddioh_1 |= (1 << (((port - 4) * 8) + pin));
        }
#endif
    }
#endif

    obj->mode = mode;
    pin_mode(obj->name, mode);
}

void pin_dir_mode(PinName name, PinDirection direction, PinMode mode)
{
    MBED_ASSERT(name != (PinName)NC);

    unsigned int port = PINNAME_TO_PORT(name);
    unsigned int pin = PINNAME_TO_PIN(name);

    /* Set function; Firmware Control (GPIO mode) */
    MXC_GPIO->func_sel[port] &= ~(0xF << (4 * pin));

    /* Normal input is always enabled */
    MXC_GPIO->in_mode[port] &= ~(0xF << (4 * pin));

    uint32_t new_mode;
    if (direction == PIN_OUTPUT) {
        // PullUp      = not valid,
        // PullDown    = not valid,
        // OpenDrain   = MXC_V_GPIO_OUT_MODE_OD,
        // PullNone    = MXC_V_GPIO_OUT_MODE_NORMAL,
        if (mode == OpenDrain) {
            new_mode = MXC_V_GPIO_OUT_MODE_OPEN_DRAIN;
        } else {
            new_mode = MXC_V_GPIO_OUT_MODE_NORMAL;
        }
    } else {
        // PullUp      = MXC_V_GPIO_OUT_MODE_HIGH_Z_WEAK_PULLUP
        // PullDown    = MXC_V_GPIO_OUT_MODE_HIGH_Z_WEAK_PULLDOWN
        // OpenDrain   = MXC_V_GPIO_OUT_MODE_OD
        // PullNone    = MXC_V_GPIO_OUT_MODE_NORMAL_HIGH_Z
        if (mode == PullUp) {
            new_mode = MXC_V_GPIO_OUT_MODE_HIGH_Z_WEAK_PULLUP;
            MXC_GPIO->out_val[port] |= 1 << pin;
        } else if (mode == PullDown) {
            new_mode = MXC_V_GPIO_OUT_MODE_HIGH_Z_WEAK_PULLDOWN;
            MXC_GPIO->out_val[port] &= ~(1 << pin);
        } else if (mode == OpenDrain) {
            new_mode = MXC_V_GPIO_OUT_MODE_OPEN_DRAIN;
            MXC_GPIO->out_val[port] |= 1 << pin;
        } else {
            new_mode = MXC_V_GPIO_OUT_MODE_NORMAL_HIGH_Z;
            MXC_GPIO->out_val[port] &= ~(1 << pin);
        }
    }

    /* Set new mode */
    uint32_t out_mode = MXC_GPIO->out_mode[port];
    out_mode &= ~(0xF << (pin * 4));
    out_mode |= (new_mode << (pin * 4));
    MXC_GPIO->out_mode[port] = out_mode;
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    pin_dir_mode(obj->name, direction, obj->mode);
}
