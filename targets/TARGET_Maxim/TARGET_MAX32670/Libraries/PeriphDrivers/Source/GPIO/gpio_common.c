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

/* **** Includes **** */
#include "mxc_device.h"
#include "mxc_assert.h"
#include "gpio.h"
#include <stddef.h>

/* **** Globals **** */
static void (*callback[MXC_CFG_GPIO_INSTANCES][MXC_CFG_GPIO_PINS_PORT])(void*);
static void* cbparam[MXC_CFG_GPIO_INSTANCES][MXC_CFG_GPIO_PINS_PORT];
static uint8_t initialized = 0;

/* **** Functions **** */
int MXC_GPIO_Common_Init(uint32_t portmask)
{
    if (!initialized) {
        int i, j;
        
        for (i = 0; i < MXC_CFG_GPIO_INSTANCES; i++) {
            // Initialize call back arrays
            for (j = 0; j < MXC_CFG_GPIO_PINS_PORT; j++) {
                callback[i][j] = NULL;
            }
        }
        
        initialized = 1;
    }
    
    return E_NO_ERROR;
}

void MXC_GPIO_Common_RegisterCallback(const mxc_gpio_cfg_t* cfg, mxc_gpio_callback_fn func, void* cbdata)
{
    uint32_t mask;
    unsigned int pin;
    
    mask = cfg->mask;
    pin = 0;
    
    while (mask) {
        if (mask & 1) {
            callback[MXC_GPIO_GET_IDX(cfg->port)][pin] = func;
            cbparam[MXC_GPIO_GET_IDX(cfg->port)][pin] = cbdata;
        }
        
        pin++;
        mask >>= 1;
    }
}

void MXC_GPIO_Common_Handler(unsigned int port)
{
    uint32_t stat;
    unsigned int pin;
    
    MXC_ASSERT(port < MXC_CFG_GPIO_INSTANCES);
    
    mxc_gpio_regs_t* gpio = MXC_GPIO_GET_GPIO(port);
    
    stat = MXC_GPIO_GetFlags(gpio);
    MXC_GPIO_ClearFlags(gpio, stat);
    
    pin = 0;
    
    while (stat) {
        if (stat & 1) {
            if (callback[port][pin]) {
                callback[port][pin](cbparam[port][pin]);
            }
        }
        
        pin++;
        stat >>= 1;
    }
}
