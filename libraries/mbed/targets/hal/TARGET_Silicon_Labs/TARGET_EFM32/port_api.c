/***************************************************************************//**
 * @file port_api.c
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
#if DEVICE_PORTOUT

#include "port_api.h"
#include "pinmap.h"
#include "gpio_api.h"
#include "em_gpio.h"
#include "em_cmu.h"

#define PORT_NUM_PINS 16

PinName port_pin(PortName port, int pin_n)
{
    return (PinName) (pin_n | port << 4); // Encode pin and port number in one uint32
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    obj->mask = mask;
    obj->port = port;
    obj->dir = dir;
    
    port_dir(obj, obj->dir);
}

void port_mode(port_t *obj, PinMode mode)
{
    /* Set mode for pins given by mask */
    uint32_t pin;
    for (pin = 0; pin < PORT_NUM_PINS; pin++) {
        if (obj->mask & (1 << pin)) {
            pin_mode(port_pin(obj->port, pin), mode);
        }
    }
}

void port_dir(port_t *obj, PinDirection dir)
{
    obj->dir = dir;

    /* Set default pin mode for pins given by mask */
    switch (dir) {
        case PIN_INPUT:
            port_mode(obj, Input);
            break;
        case PIN_OUTPUT:
            port_mode(obj, PushPull);
            break;
    }
}

void port_write(port_t *obj, int value)
{
    GPIO_PortOutSetVal(obj->port, value, obj->mask);
}

int port_read(port_t *obj)
{
    if (obj->dir == PIN_INPUT) {
        return (int) (GPIO_PortInGet(obj->port) & obj->mask);
    } else {
        return (int) (GPIO_PortOutGet(obj->port) & obj->mask);
    }
}

#endif
