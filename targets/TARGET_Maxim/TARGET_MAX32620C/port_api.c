/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
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

#include "port_api.h"
#include "pinmap.h"
#include "gpio_api.h"
#include "gpio_regs.h"
#include "clkman_regs.h"

PinName port_pin(PortName port, int pin_n)
{
    return (PinName)((port << PORT_SHIFT) | pin_n);
}

void port_init(port_t *obj, PortName port, int mask, PinDirection direction)
{
    obj->port = port;
    obj->mask = mask;
    obj->reg_out = &MXC_GPIO->out_val[port];
    obj->reg_in  = &MXC_GPIO->in_val[port];
    obj->mode = PullDefault;

    /* Ensure that the GPIO clock is enabled */
    MXC_CLKMAN->sys_clk_ctrl_6_gpio = MXC_S_CLKMAN_CLK_SCALE_DIV_1;

    uint32_t i;
    // The function is set per pin: reuse gpio logic
    for (i = 0; i < MXC_GPIO_MAX_PINS_PER_PORT; i++) {
        if (obj->mask & (1 << i)) {
            gpio_set(port_pin(obj->port, i));
            pin_dir_mode(port_pin(obj->port, i), direction, obj->mode);
        }
    }
}

void port_mode(port_t *obj, PinMode mode)
{
    uint32_t i;
    obj->mode = mode;
    // The mode is set per pin: reuse pinmap logic
    for (i = 0; i < MXC_GPIO_MAX_PINS_PER_PORT; i++) {
        if (obj->mask & (1 << i)) {
            pin_mode(port_pin(obj->port, i), mode);
        }
    }
}

void port_dir(port_t *obj, PinDirection direction)
{
    uint32_t i;
    // The mode is set per pin: reuse gpio logic
    for (i = 0; i < MXC_GPIO_MAX_PINS_PER_PORT; i++) {
        if (obj->mask & (1 << i)) {
            pin_dir_mode(port_pin(obj->port, i), direction, obj->mode);
        }
    }
}

void port_write(port_t *obj, int value)
{
    *obj->reg_out = (*obj->reg_out & ~obj->mask) | (value & obj->mask);
}

int port_read(port_t *obj)
{
    return (*obj->reg_in & obj->mask);
}

