/*******************************************************************************
 * Copyright (c) Maxim Integrated Products, Inc., All Rights Reserved.
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
#include "pinmap.h"
#include "objects.h"
#include "gpio_regs.h"

void pin_function(PinName name, int function)
{
    MBED_ASSERT(name != (PinName)NC);

    if ((function >= 0) && (function <= 0xF)) {

        // Set GPIO Function
        unsigned int pin_val = (1 << PINNAME_TO_PIN(name));
        switch (function) {
            case 0: // GPIO_FUNC_IN
                MXC_GPIO0->en0		|= 	pin_val;
                MXC_GPIO0->en1    	&= ~pin_val;
                break;
            case 1: // GPIO_FUNC_OUT
                MXC_GPIO0->en0		|= 	pin_val;
                MXC_GPIO0->en1    	&= ~pin_val;
                MXC_GPIO0->out_en	|= 	pin_val;
                break;
            case 2: // GPIO_FUNC_ALT1
            	MXC_GPIO0->en0		&= ~pin_val;
            	MXC_GPIO0->en1    	&= ~pin_val;
                break;
            case 3: // GPIO_FUNC_ALT2
            	MXC_GPIO0->en0    	&= ~pin_val;
            	MXC_GPIO0->en1		|= 	pin_val;
                break;
            case 4: // GPIO_FUNC_ALT3
            	MXC_GPIO0->en0		|= 	pin_val;
				MXC_GPIO0->en1		|= 	pin_val;
                break;
            default:
                break;
        }
     }
}

void pin_mode(PinName name, PinMode mode)
{
    MBED_ASSERT(name != (PinName)NC);

    unsigned int pin_val = (1 << PINNAME_TO_PIN(name));
    switch (mode) {
        case PullUp:
            MXC_GPIO0->pad_cfg1 |=  pin_val;
            MXC_GPIO0->ps 		|=  pin_val;
            break;
        case PullDown:
        	MXC_GPIO0->pad_cfg1 |=  pin_val;
        	MXC_GPIO0->ps 		&= ~pin_val;
            break;
        case PullNone:
            MXC_GPIO0->pad_cfg1 &= ~pin_val;
            MXC_GPIO0->ps 		&= ~pin_val;
            break;
        default:
            break;
    }
}
