/**
 * @file    mxc_pins.h
 * @brief   This file contains constant pin configurations for the peripherals.
 */

/* *****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
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

typedef enum {
    MAP_A,
    MAP_B,
    MAP_C
} sys_map_t;

/***** Global Variables *****/
// Predefined GPIO Configurations
extern const mxc_gpio_cfg_t gpio_cfg_extclk;
extern const mxc_gpio_cfg_t gpio_cfg_i2c0;
extern const mxc_gpio_cfg_t gpio_cfg_i2c1;
extern const mxc_gpio_cfg_t gpio_cfg_i2c2;
extern const mxc_gpio_cfg_t gpio_cfg_i2c2b;
extern const mxc_gpio_cfg_t gpio_cfg_i2c2c;


extern const mxc_gpio_cfg_t gpio_cfg_uart0a;
extern const mxc_gpio_cfg_t gpio_cfg_uart0a_flow;
extern const mxc_gpio_cfg_t gpio_cfg_uart0a_flow_disable;
extern const mxc_gpio_cfg_t gpio_cfg_uart0b;
extern const mxc_gpio_cfg_t gpio_cfg_uart0b_flow;
extern const mxc_gpio_cfg_t gpio_cfg_uart0b_flow_disable;

extern const mxc_gpio_cfg_t gpio_cfg_uart1a;
extern const mxc_gpio_cfg_t gpio_cfg_uart1a_flow;
extern const mxc_gpio_cfg_t gpio_cfg_uart1a_flow_disable;
extern const mxc_gpio_cfg_t gpio_cfg_uart1b;
extern const mxc_gpio_cfg_t gpio_cfg_uart1b_flow;
extern const mxc_gpio_cfg_t gpio_cfg_uart1b_flow_disable;

extern const mxc_gpio_cfg_t gpio_cfg_uart2b;
extern const mxc_gpio_cfg_t gpio_cfg_uart2b_flow;
extern const mxc_gpio_cfg_t gpio_cfg_uart2b_flow_disable;

extern const mxc_gpio_cfg_t gpio_cfg_uart3;
extern const mxc_gpio_cfg_t gpio_cfg_uart3_flow;
extern const mxc_gpio_cfg_t gpio_cfg_uart3_flow_disable;


extern const mxc_gpio_cfg_t gpio_cfg_spi0;
// NOTE: SPI1 definied here with SS1 only, SS0 is on port0 by itself.
extern const mxc_gpio_cfg_t gpio_cfg_spi1;
// NOTE: SPI2 defined here with SS0 only, and NOT SS1 and SS2
extern const mxc_gpio_cfg_t gpio_cfg_spi2;
extern const mxc_gpio_cfg_t gpio_cfg_spi2b;
// NOTE: SPI3 defined here with SS0 only, and NOT SS1, SS2, or SS3
extern const mxc_gpio_cfg_t gpio_cfg_spi3;

// Timers are only defined once, depending on package, each timer could be mapped to other pins
extern const mxc_gpio_cfg_t gpio_cfg_tmr0;
extern const mxc_gpio_cfg_t gpio_cfg_tmr1;
extern const mxc_gpio_cfg_t gpio_cfg_tmr2;
extern const mxc_gpio_cfg_t gpio_cfg_tmr3;
extern const mxc_gpio_cfg_t gpio_cfg_tmr4;
extern const mxc_gpio_cfg_t gpio_cfg_tmr5;

extern const mxc_gpio_cfg_t gpio_cfg_i2s0;

extern const mxc_gpio_cfg_t gpio_cfg_rtcsqw;
extern const mxc_gpio_cfg_t gpio_cfg_rtcsqwb;

extern const mxc_gpio_cfg_t gpio_cfg_lc1;
extern const mxc_gpio_cfg_t gpio_cfg_mon_lc1;
extern const mxc_gpio_cfg_t gpio_cfg_cmd_rs_lc1;
extern const mxc_gpio_cfg_t gpio_cfg_chrg_lc1;
extern const mxc_gpio_cfg_t gpio_cfg_lc2;
extern const mxc_gpio_cfg_t gpio_cfg_mon_lc2;
extern const mxc_gpio_cfg_t gpio_cfg_cmd_rs_lc2;
extern const mxc_gpio_cfg_t gpio_cfg_chrg_lc2;

#endif /* _MXC_PINS_H_ */

