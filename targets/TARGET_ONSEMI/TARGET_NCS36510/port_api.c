/**
 ******************************************************************************
 * @file port_api.c
 * @brief Implementation of a port API
 * @internal
 * @author ON Semiconductor
 * $Rev:
 * $Date:
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 */
#include "gpio.h"
#include "gpio_api.h"
#include "port_api.h"
#include "pinmap.h"

#if DEVICE_PORTIN || DEVICE_PORTOUT

/** Get the pin name from the port's pin number
 *
 * @param port  The port name
 * @param pin_n The pin number within the specified port
 * @return The pin name for the port's pin number
 */
PinName port_pin(PortName port, int pin_n)
{
    return((PinName)(pin_n));
}

/** Initilize the port
 *
 * @param obj  The port object to initialize
 * @param port The port name
 * @param mask The bitmask to identify which bits in the port should be included (0 - ignore)
 * @param dir  The port direction
 */
void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    uint8_t i;
    PinName pin;

    /* Store the port mask in obj */
    obj->mask = mask;

    /* Store the port name in obj */
    obj->port = port;

    /* Store GPIO base address */
    obj->GPIOMEMBASE = GPIOREG;

    for (i=0; i<NUMBER_OF_GPIO; i++) {
        /* check for valid pin */
        if (obj->mask & (1<<i)) {

            /* Gpio numbers start from DIO#0 to #17, so can pass in "i" */
            pin = port_pin(obj->port, i);

            /* Set the pin as GPIO */
            gpio_set(pin);
        }
    }

    /* Call function to set pin direction */
    port_dir(obj, dir);
}

/** Set the input port mode
 *
 * @param obj  The port object
 * @param mode THe port mode to be set
 */
void port_mode(port_t *obj, PinMode mode)
{
    uint8_t i = 0;
    PinName pin;

    /* For each pin in the mask, set the mode to that defined in "mode" parameter */
    for (i=0; i < NUMBER_OF_GPIO; i++) {
        /* check for valid pin */
        if (obj->mask & (1<<i)) {

            /* get the pin name */
            pin = port_pin(obj->port, i);

            /* Set the mode for the pin */
            pin_mode(pin, mode);
        }
    }
}

/** Set port direction (in/out)
 *
 * @param obj The port object
 * @param dir The port direction to be set
 */
void port_dir(port_t *obj, PinDirection dir)
{
    /* Enable the GPIO clock */
    CLOCK_ENABLE(CLOCK_GPIO);

    if (dir == PIN_INPUT) {
        obj->GPIOMEMBASE->W_IN = obj->mask;
    } else if (dir == PIN_OUTPUT) {
        obj->GPIOMEMBASE->W_OUT = obj->mask;
    }

    /* Disable the GPIO clock */
    CLOCK_DISABLE(CLOCK_GPIO);
}

/** Write value to the port
 *
 * @param obj   The port object
 * @param value The value to be set
 */
void port_write(port_t *obj, int value)
{
    /* Enable the GPIO clock */
    CLOCK_ENABLE(CLOCK_GPIO);

    obj->GPIOMEMBASE->R_STATE_W_SET = value;//(obj->mask & value);
    obj->GPIOMEMBASE->R_IRQ_W_CLEAR = ~value;//(obj->mask ^ value);

    /* Disable the GPIO clock */
    CLOCK_DISABLE(CLOCK_GPIO);
}

/** Read the current value on the port
 *
 * @param obj The port object
 * @return An integer with each bit corresponding to an associated port pin setting
 */
int port_read(port_t *obj)
{
    int gpio_level = 0;

    /* Enable the GPIO clock */
    CLOCK_ENABLE(CLOCK_GPIO);

    gpio_level = obj->GPIOMEMBASE->R_STATE_W_SET;

    /* Disable the GPIO clock */
    CLOCK_DISABLE(CLOCK_GPIO);

    return(gpio_level);
}

#endif
