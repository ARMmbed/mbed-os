/**
 ******************************************************************************
 * @file gpio_api.c
 * @brief Implementation of a GPIO driver
 * @internal
 * @author ON Semiconductor
 * $Rev:
 * $Date: 2015-11-04 $
 ******************************************************************************
 * @copyright (c) 2012 ON Semiconductor. All rights reserved.
 * ON Semiconductor is supplying this software for use with ON Semiconductor
 * processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup gpio
 *
 * @details
 *
 * @internal
 * <h1> Reference document(s) </h1>
 * <p>
 * Reference document: IPC7203 APB GPIO Design Specification v1.2</a>
 * </p>
 * @endinternal
 *
 * <h1> Functional description (internal) </h1>
 * <p>
 * Each GPIO line can be independently programmed as an input or an output. Separate Set
 * and Clear registers are provided since it is likely that different software tasks may be
 * servicing different I/O signals. Inputs are synchronized to the system clock
 * through a pair of flip-flops.  Each input can be programmed
 * to cause an interrupt to be generated.  The interrupt can be programmed to be level-sensitive
 * or edge-sensitive and the level (high or low) or edge (rising, falling or either) that causes
 * the interrupt can be selected.  Interrupts can be individually enabled or disabled.
 * Level-sensitive interrupts stay asserted until the interrupting condition is cleared.
 * Edge-triggered interrupts are cleared by writing to the GPIO interrupt clear register.
 * </p>
 *
 * <h1> Use of GPIO driver in SW </h1>
 * <p>
 * The user of the GPIO driver should set the pin as GPIO, using crossbar.
 * Init the GPIO and configure the mode and direction.This will return a device pointer.  One device controls all GPIO's.  It is not
 * needed nor supported to create a device per GPIO.
 * Next, the user should call the fGpioOpen function with the device and options as paramter.
 * </p>
 * <p>
 * Use the device driver fGpioIoctl function to change the behavior of the GPIO's and to register an
 * interrupt handler for each IO that has an interrupt enabled.  There is one interrupt for all GPIO's.
 * The GPIO driver will look up what IO caused the interrupt and call the respective interrupt handler.
 * </p>
 */

#include "gpio.h"

/* Include from the mbed-hal layer */
#include "gpio_api.h"
#include "pinmap.h"


/** Set the given pin as GPIO
 *
 * @param pin The pin to be set as GPIO
 * @return The GPIO port mask for this pin
 **/
uint32_t gpio_set(PinName pin)
{
    if (pin != NC) {
        /* Configure to GPIO using pin function API*/
        pin_function(pin, CONFIGURE_AS_GPIO);

        return ((uint32_t) 0x1 << pin);

    }

    return(0x00000000);

}

/** Initialize the GPIO pin
 *
 * @param obj The GPIO object to initialize
 * @param pin The GPIO pin to initialize
 */
void gpio_init(gpio_t *obj, PinName pin)
{
    /* Initialize the GPIO membase */
    obj->GPIOMEMBASE = GPIOREG;

    /* Initialize the pin to be GPIO */
    obj->gpioPin = pin;
    obj->gpioMask = gpio_set(pin);

    /* Enable the GPIO clock */
    CLOCK_ENABLE(CLOCK_GPIO);

    /* Set the drive strength of the pin to 1 by default */
    /** - Get PAD IO register address for the PAD number */
    PadReg_t *PadRegOffset = (PadReg_t*)(PADREG_BASE + (pin * PAD_REG_ADRS_BYTE_SIZE));

    /* - Disable the GPIO clock */
    CLOCK_DISABLE(CLOCK_GPIO);

    /** - Enable the clock for PAD peripheral device */
    CLOCK_ENABLE(CLOCK_PAD);

    /** - Set drive type, pulltype & drive strength */
    PadRegOffset->PADIO0.BITS.POWER = 1;

    /** - Disable the clock for PAD peripheral device */
    CLOCK_DISABLE(CLOCK_PAD);
}

/** Set the input pin mode
 *
 * @param obj  The GPIO object
 * @param mode The pin mode to be set
 */
void gpio_mode(gpio_t *obj, PinMode mode)
{
    uint32_t pin = obj->gpioPin;

    /* Set the mode for the pin */
    pin_mode((PinName)pin, mode);
}

/** Set the pin direction
 *
 * @param obj       The GPIO object
 * @param direction The pin direction to be set
 */
void gpio_dir(gpio_t *obj, PinDirection direction)
{
    /* Enable the GPIO clock */
    CLOCK_ENABLE(CLOCK_GPIO);

    if (direction == PIN_INPUT) {
        obj->GPIOMEMBASE->W_IN = obj->gpioMask;
    } else if (direction == PIN_OUTPUT) {
        obj->GPIOMEMBASE->W_OUT = obj->gpioMask;
    }

    /* - Disable the GPIO clock */
    CLOCK_DISABLE(CLOCK_GPIO);
}

/** Set the output value
 *
 * @param obj   The GPIO object
 * @param value The value to be set
 */
void gpio_write(gpio_t *obj, int value)
{
    /* Enable the GPIO clock */
    CLOCK_ENABLE(CLOCK_GPIO);

    /* Set the GPIO based on value */
    if (value) {
        obj->GPIOMEMBASE->R_STATE_W_SET = obj->gpioMask;
    } else {
        obj->GPIOMEMBASE->R_IRQ_W_CLEAR = obj->gpioMask;
    }

    /* - Disable the GPIO clock */
    CLOCK_DISABLE(CLOCK_GPIO);
}

/** Read the input value
 *
 * @param obj The GPIO object
 * @return An integer value 1 or 0
 */
int gpio_read(gpio_t *obj)
{
    int ret;

    /* Enable the GPIO clock */
    CLOCK_ENABLE(CLOCK_GPIO);

    ret = (obj->GPIOMEMBASE->R_STATE_W_SET & obj->gpioMask) ? 1: 0;

    /* - Disable the GPIO clock */
    CLOCK_DISABLE(CLOCK_GPIO);

    return ret;
}
