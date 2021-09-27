/**
 * @file    mxc_pins.h
 * @brief   This file contains constant pin configurations for the peripherals.
 */

/* *****************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
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
 *
 **************************************************************************** */


#ifndef _MXC_PINS_H_
#define _MXC_PINS_H_

#include "gpio.h"

/***** Global Variables *****/

typedef enum {
	MAP_A,
	MAP_B,
	MAP_C
} sys_map_t;

// Predefined GPIO Configurations
extern const mxc_gpio_cfg_t gpio_cfg_swda;
extern const mxc_gpio_cfg_t gpio_cfg_swdb;

extern const mxc_gpio_cfg_t gpio_cfg_i2c0;
extern const mxc_gpio_cfg_t gpio_cfg_i2c1;


extern const mxc_gpio_cfg_t gpio_cfg_uart0;
extern const mxc_gpio_cfg_t gpio_cfg_uart0_flow;
extern const mxc_gpio_cfg_t gpio_cfg_uart1a;
extern const mxc_gpio_cfg_t gpio_cfg_uart1b;
extern const mxc_gpio_cfg_t gpio_cfg_uart1c;
extern const mxc_gpio_cfg_t gpio_cfg_uart1_flow;


extern const mxc_gpio_cfg_t gpio_cfg_spi0;
extern const mxc_gpio_cfg_t gpio_cfg_spi0_ss;
extern const mxc_gpio_cfg_t gpio_cfg_spi1a;
extern const mxc_gpio_cfg_t gpio_cfg_spi1a_ss;
extern const mxc_gpio_cfg_t gpio_cfg_spi1b;
extern const mxc_gpio_cfg_t gpio_cfg_spi1b_ss;

// Timers are only defined once, depending on package, each timer could be mapped to other pins
extern const mxc_gpio_cfg_t gpio_cfg_tmr0;
extern const mxc_gpio_cfg_t gpio_cfg_32kcal;

extern const mxc_gpio_cfg_t gpio_cfg_i2s0a;
extern const mxc_gpio_cfg_t gpio_cfg_i2s0b;


#endif /* _MXC_PINS_H_ */

