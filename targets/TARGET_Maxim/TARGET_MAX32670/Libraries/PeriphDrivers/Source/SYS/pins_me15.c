/**
 * @file mxc_pins.c
 * @brief      This file contains constant pin configurations for the peripherals.
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
 *
 **************************************************************************** */

#include "gpio.h"
#include "mxc_device.h"

/***** Definitions *****/

/***** Global Variables *****/
const mxc_gpio_cfg_t gpio_cfg_extclk             =   { MXC_GPIO0, (MXC_GPIO_PIN_12 | MXC_GPIO_PIN_13), MXC_GPIO_FUNC_ALT2, MXC_GPIO_PAD_NONE };

const mxc_gpio_cfg_t gpio_cfg_i2c0               =   { MXC_GPIO0, (MXC_GPIO_PIN_6  | MXC_GPIO_PIN_7),  MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_PULL_UP, MXC_GPIO_VSSEL_VDDIO};
const mxc_gpio_cfg_t gpio_cfg_i2c1               =   { MXC_GPIO0, (MXC_GPIO_PIN_12 | MXC_GPIO_PIN_13), MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_PULL_UP, MXC_GPIO_VSSEL_VDDIO};
const mxc_gpio_cfg_t gpio_cfg_i2c2               =   { MXC_GPIO0, (MXC_GPIO_PIN_18 | MXC_GPIO_PIN_19), MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_PULL_UP, MXC_GPIO_VSSEL_VDDIO};

const mxc_gpio_cfg_t gpio_cfg_uart0a              =   { MXC_GPIO0, (MXC_GPIO_PIN_8  | MXC_GPIO_PIN_9),  MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_uart0a_flow         =   { MXC_GPIO0, (MXC_GPIO_PIN_10 | MXC_GPIO_PIN_11), MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_uart0a_flow_disable =   { MXC_GPIO0, (MXC_GPIO_PIN_10 | MXC_GPIO_PIN_11), MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE };

const mxc_gpio_cfg_t gpio_cfg_uart0b              =   { MXC_GPIO0, (MXC_GPIO_PIN_24 | MXC_GPIO_PIN_25), MXC_GPIO_FUNC_ALT2, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_uart0b_flow         =   { MXC_GPIO0, (MXC_GPIO_PIN_26 | MXC_GPIO_PIN_27), MXC_GPIO_FUNC_ALT2, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_uart0b_flow_disable =   { MXC_GPIO0, (MXC_GPIO_PIN_26 | MXC_GPIO_PIN_27), MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE };

const mxc_gpio_cfg_t gpio_cfg_uart1a              =   { MXC_GPIO0, (MXC_GPIO_PIN_28 | MXC_GPIO_PIN_29), MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_uart1a_flow         =   { MXC_GPIO0, (MXC_GPIO_PIN_30 | MXC_GPIO_PIN_31), MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_uart1a_flow_disable =   { MXC_GPIO0, (MXC_GPIO_PIN_30 | MXC_GPIO_PIN_31), MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE };

const mxc_gpio_cfg_t gpio_cfg_uart1b              =   { MXC_GPIO0, (MXC_GPIO_PIN_2 | MXC_GPIO_PIN_3), MXC_GPIO_FUNC_ALT2, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_uart1b_flow         =   { MXC_GPIO0, (MXC_GPIO_PIN_4 | MXC_GPIO_PIN_5), MXC_GPIO_FUNC_ALT2, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_uart1b_flow_disable =   { MXC_GPIO0, (MXC_GPIO_PIN_4 | MXC_GPIO_PIN_5), MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE };

const mxc_gpio_cfg_t gpio_cfg_uart2b              =   { MXC_GPIO0, (MXC_GPIO_PIN_14 | MXC_GPIO_PIN_15), MXC_GPIO_FUNC_ALT2, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_uart2b_flow         =   { MXC_GPIO0, (MXC_GPIO_PIN_16 | MXC_GPIO_PIN_17), MXC_GPIO_FUNC_ALT2, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_uart2b_flow_disable =   { MXC_GPIO0, (MXC_GPIO_PIN_16 | MXC_GPIO_PIN_17), MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_uart3               =   { MXC_GPIO0, (MXC_GPIO_PIN_26 | MXC_GPIO_PIN_27), MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_uart3_flow          =   { MXC_GPIO0, (MXC_GPIO_PIN_24 | MXC_GPIO_PIN_25), MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_uart3_flow_disable  =   { MXC_GPIO0, (MXC_GPIO_PIN_24 | MXC_GPIO_PIN_25), MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE };

const mxc_gpio_cfg_t gpio_cfg_i2s0               =   { MXC_GPIO0, (MXC_GPIO_PIN_8 | MXC_GPIO_PIN_9 | MXC_GPIO_PIN_10 | MXC_GPIO_PIN_11), MXC_GPIO_FUNC_ALT2, MXC_GPIO_PAD_NONE };

const mxc_gpio_cfg_t gpio_cfg_spi0    =  { MXC_GPIO0, (MXC_GPIO_PIN_2 | MXC_GPIO_PIN_3 | MXC_GPIO_PIN_4), MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_spi0_ss =  { MXC_GPIO0, (MXC_GPIO_PIN_5), MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE };
// NOTE: SPI1 definied here with SS0 only
const mxc_gpio_cfg_t gpio_cfg_spi1    =  { MXC_GPIO0, (MXC_GPIO_PIN_14 | MXC_GPIO_PIN_15 | MXC_GPIO_PIN_16), MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_spi1_ss =  { MXC_GPIO0, (MXC_GPIO_PIN_17), MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE };
// NOTE: SPI2 defined here with SS0 only, and NOT SS1 and SS2
const mxc_gpio_cfg_t gpio_cfg_spi2    =  { MXC_GPIO1, (MXC_GPIO_PIN_1 | MXC_GPIO_PIN_2 | MXC_GPIO_PIN_3), MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_spi2_ss =  { MXC_GPIO1, (MXC_GPIO_PIN_4), MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE };

// Timers are only defined once, depending on package, each timer could be mapped to other pins
const mxc_gpio_cfg_t gpio_cfg_tmr0  =  { MXC_GPIO0, (MXC_GPIO_PIN_16 | MXC_GPIO_PIN_17), MXC_GPIO_FUNC_ALT3, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO};
const mxc_gpio_cfg_t gpio_cfg_tmr1  =  { MXC_GPIO0, (MXC_GPIO_PIN_2  |  MXC_GPIO_PIN_3), MXC_GPIO_FUNC_ALT3, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO};
const mxc_gpio_cfg_t gpio_cfg_tmr2  =  { MXC_GPIO0, (MXC_GPIO_PIN_4  |  MXC_GPIO_PIN_5), MXC_GPIO_FUNC_ALT3, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO};
const mxc_gpio_cfg_t gpio_cfg_tmr3  =  { MXC_GPIO0, (MXC_GPIO_PIN_14 | MXC_GPIO_PIN_15), MXC_GPIO_FUNC_ALT3, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO};
const mxc_gpio_cfg_t gpio_cfg_tmr4  =  { MXC_GPIO0, (MXC_GPIO_PIN_6  |  MXC_GPIO_PIN_7), MXC_GPIO_FUNC_ALT2, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO};
const mxc_gpio_cfg_t gpio_cfg_tmr5  =  { MXC_GPIO0, (MXC_GPIO_PIN_22 | MXC_GPIO_PIN_23), MXC_GPIO_FUNC_ALT1, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO};

const mxc_gpio_cfg_t gpio_cfg_rtcsqw  =    { MXC_GPIO0, MXC_GPIO_PIN_8,  MXC_GPIO_FUNC_ALT4, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_rtcsqwb =    { MXC_GPIO1, MXC_GPIO_PIN_11, MXC_GPIO_FUNC_ALT2, MXC_GPIO_PAD_NONE };


const mxc_gpio_cfg_t gpio_cfg_lc1           =    { MXC_GPIO1, MXC_GPIO_PIN_9,  MXC_GPIO_FUNC_ALT4, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_mon_lc1       =    { MXC_GPIO1, MXC_GPIO_PIN_10, MXC_GPIO_FUNC_ALT4, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_cmd_rs_lc1    =    { MXC_GPIO1, MXC_GPIO_PIN_11, MXC_GPIO_FUNC_ALT4, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_chrg_lc1      =    { MXC_GPIO1, MXC_GPIO_PIN_12, MXC_GPIO_FUNC_ALT4, MXC_GPIO_PAD_NONE };

const mxc_gpio_cfg_t gpio_cfg_lc2           =    { MXC_GPIO0, MXC_GPIO_PIN_31, MXC_GPIO_FUNC_ALT4, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_mon_lc2       =    { MXC_GPIO0, MXC_GPIO_PIN_19, MXC_GPIO_FUNC_ALT4, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_cmd_rs_lc2    =    { MXC_GPIO0, MXC_GPIO_PIN_18, MXC_GPIO_FUNC_ALT4, MXC_GPIO_PAD_NONE };
const mxc_gpio_cfg_t gpio_cfg_chrg_lc2      =    { MXC_GPIO0, MXC_GPIO_PIN_17, MXC_GPIO_FUNC_ALT4, MXC_GPIO_PAD_NONE };
