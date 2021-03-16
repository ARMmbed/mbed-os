/***************************************************************************//**
* \file cyhal_gpio.h
*
* \brief
* Provides a high level interface for interacting with the GPIO on Cypress devices.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/**
* \addtogroup group_hal_gpio GPIO (General Purpose Input Output)
* \ingroup group_hal
* \{
* High level interface for configuring and interacting with general purpose input/outputs (GPIO).
*
* The GPIO driver provides functions to configure and initialize GPIO, and to read and write data to the pin.
* The driver also supports interrupt generation on GPIO signals with rising, falling or both edges.
*
* \note The APIs in this driver need not be used if a GPIO is to be used as an input or output of peripherals like I2C or PWM.
* The respective peripheral's driver will utilize the GPIO interface to configure and initialize its GPIO pins.
*
* \section subsection_gpio_features Features
* * Configurable GPIO pin direction - \ref cyhal_gpio_direction_t
* * Configurable GPIO pin drive modes - \ref cyhal_gpio_drive_mode_t
* * Configurable analog and digital characteristics
* * Configurable edge-triggered interrupts and callback assignment on GPIO events - \ref cyhal_gpio_event_t
*
* \section subsection_gpio_quickstart Quick Start
* \ref cyhal_gpio_init can be used for a simple GPIO initialization by providing the pin number (<b>pin</b>), pin direction (<b>direction</b>),
* pin drive mode (<b>drive_mode</b>) and the initial value on the pin (<b>init_val</b>).
*
* \section subsection_gpio_sample_snippets Code Snippets
*
* \subsection subsection_gpio_snippet_1 Snippet 1: Reading value from GPIO
* The following snippet initializes GPIO pin \ref P0_0 as an input with high impedance digital drive mode and initial value = <b>false</b> (low). A value is read
* from the pin and stored to a uint8_t variable (<b>read_val</b>).

* \snippet gpio.c snippet_cyhal_gpio_read

* \subsection subsection_gpio_snippet_2 Snippet 2: Writing value to a GPIO
* The following snippet initializes GPIO pin \ref P0_0 as an output pin with strong drive mode and initial value = <b>false</b> (low).
* A value = <b>true</b> (high) is written to the output driver.

* \snippet gpio.c snippet_cyhal_gpio_write

* \subsection subsection_gpio_snippet_3 Snippet 3: Reconfiguring a GPIO
* The following snippet shows how to reconfigure a GPIO pin during run-time using the firmware. The GPIO pin \ref P0_0
* is first initialized as an output pin with strong drive mode. The pin is then reconfigured as an input with high impedance digital drive mode.
* \note \ref cyhal_gpio_configure only changes the <b>direction</b> and the <b>drive_mode</b>
* of the pin. Previously set pin value is retained.
*
* \snippet gpio.c snippet_cyhal_gpio_reconfigure

* \subsection subsection_gpio_snippet_4 Snippet 4: Interrupts on GPIO events
* GPIO events can be mapped to an interrupt and assigned to a callback function. The callback function needs to be first registered and
* then the event needs to be enabled.
** The following snippet initializes GPIO pin \ref P0_0 as an input pin. It registers a callback function and enables detection
* of a falling edge event to trigger the callback.
* \note If no argument needs to be passed to the callback function then a NULL can be passed during registering. <br>
*
* \snippet gpio.c snippet_cyhal_gpio_interrupt
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


/*******************************************************************************
*       Defines
*******************************************************************************/

/** Integer representation of no connect pin (required to exist in all BSPs) */
#define CYHAL_NC_PIN_VALUE (NC)

/*******************************************************************************
*       Enumerations
*******************************************************************************/

/** Pin events */
typedef enum {
    CYHAL_GPIO_IRQ_NONE = 0,                                            /**< No interrupt */
    CYHAL_GPIO_IRQ_RISE = 1 << 0,                                       /**< Interrupt on rising edge */
    CYHAL_GPIO_IRQ_FALL = 1 << 1,                                       /**< Interrupt on falling edge */
    CYHAL_GPIO_IRQ_BOTH = (CYHAL_GPIO_IRQ_RISE | CYHAL_GPIO_IRQ_FALL),  /**< Interrupt on both rising and falling edges */
} cyhal_gpio_event_t;

/** Pin direction */
typedef enum {
    CYHAL_GPIO_DIR_INPUT,         /**< Input pin */
    CYHAL_GPIO_DIR_OUTPUT,        /**< Output pin */
    CYHAL_GPIO_DIR_BIDIRECTIONAL, /**< Input and output pin */
} cyhal_gpio_direction_t;

/** Pin drive mode */

/** \note When the <b> drive_mode </b> of the <b> pin </b> is set to <b> CYHAL_GPIO_DRIVE_PULL_NONE </b>,
 * it is set to <b> CYHAL_GPIO_DRIVE_STRONG </b> if the <b> direction </b>
 * of the <b> pin </b> is <b> CYHAL_GPIO_DIR_OUTPUT </b> or <b> CYHAL_GPIO_DIR_BIDIRECTIONAL</b>.
 * If not, the <b> drive_mode </b> of the <b> pin </b> is set to <b> CYHAL_GPIO_DRIVE_NONE</b>.
 */
typedef enum {
    CYHAL_GPIO_DRIVE_NONE,                /**< Digital Hi-Z. Input only. Input init value(s): 0 or 1 */
    CYHAL_GPIO_DRIVE_ANALOG,              /**< Analog Hi-Z. Use only for analog purpose */
    CYHAL_GPIO_DRIVE_PULLUP,              /**< Pull-up resistor. Input and output. Input init value(s): 1, output value(s): 0 */
    CYHAL_GPIO_DRIVE_PULLDOWN,            /**< Pull-down resistor. Input and output. Input init value(s): 0, output value(s): 1 */
    CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW,  /**< Open-drain, Drives Low. Input and output. Input init value(s): 1, output value(s): 0 */
    CYHAL_GPIO_DRIVE_OPENDRAINDRIVESHIGH, /**< Open-drain, Drives High. Input and output. Input init value(s): 0, output value(s): 1 */
    CYHAL_GPIO_DRIVE_STRONG,              /**< Strong output. Output only. Output init value(s): 0 or 1 */
    CYHAL_GPIO_DRIVE_PULLUPDOWN,          /**< Pull-up and pull-down resistors. Input and output. Input init value(s): 0 or 1, output value(s): 0 or 1 */
    CYHAL_GPIO_DRIVE_PULL_NONE,           /**< No Pull-up or pull-down resistors. Input and output. Input init value(s): 0 or 1, output value(s): 0 or 1 */
} cyhal_gpio_drive_mode_t;

/** GPIO callback function type */
typedef void (*cyhal_gpio_event_callback_t)(void *callback_arg, cyhal_gpio_event_t event);

/*******************************************************************************
*       Functions
*******************************************************************************/

/** Initialize the GPIO pin <br>
 * See \ref subsection_gpio_snippet_1.
 *
 * @param[in] pin         The GPIO pin to initialize
 * @param[in] direction   The pin direction
 * @param[in] drive_mode  The pin drive mode
 * @param[in] init_val    Initial value on the pin
 *
 * @return The status of the init request
 *
 * Guidance for using gpio drive modes ( \ref cyhal_gpio_drive_mode_t for details).
 * For default use drive modes:
 * Input GPIO direction - \ref CYHAL_GPIO_DRIVE_NONE
 * Output GPIO direction - \ref CYHAL_GPIO_DRIVE_STRONG
 * Bidirectional GPIO - \ref CYHAL_GPIO_DRIVE_PULLUPDOWN
 * \warning Don't use \ref CYHAL_GPIO_DRIVE_STRONG for input GPIO direction. It may cause an overcurrent issue.
 */
cy_rslt_t cyhal_gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drive_mode, bool init_val);

/** Uninitialize the gpio peripheral and the cyhal_gpio_t object
 *
 * @param[in] pin Pin number
 */
void cyhal_gpio_free(cyhal_gpio_t pin);

/** Configure the GPIO pin <br>
 * See \ref subsection_gpio_snippet_3.
 *
 * @param[in] pin          The GPIO pin
 * @param[in] direction    The pin direction
 * @param[in] drive_mode   The pin drive mode
 *
 * @return The status of the configure request
 */
cy_rslt_t cyhal_gpio_configure(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drive_mode);

/** Set the output value for the pin. This only works for output & in_out pins. <br>
 * See \ref subsection_gpio_snippet_2.
 *
 * @param[in] pin   The GPIO object
 * @param[in] value The value to be set (high = true, low = false)
 */
void cyhal_gpio_write(cyhal_gpio_t pin, bool value);

/** Read the input value.  This only works for \ref CYHAL_GPIO_DIR_INPUT & \ref CYHAL_GPIO_DIR_BIDIRECTIONAL pins. <br>
 * See \ref subsection_gpio_snippet_1.
 *
 * @param[in]  pin   The GPIO object
 * @return The value of the IO (true = high, false = low)
 */
bool cyhal_gpio_read(cyhal_gpio_t pin);

/** Toggle the output value <br>
 * See \ref subsection_gpio_snippet_4.
 * @param[in]  pin   The GPIO object
 */
void cyhal_gpio_toggle(cyhal_gpio_t pin);

/** Register/clear a callback handler for pin events <br>
 *
 * This function will be called when one of the events enabled by \ref cyhal_gpio_enable_event occurs.
 *
 * See \ref subsection_gpio_snippet_4.
 *
 * @param[in] pin            The pin number
 * @param[in] callback       The function to call when the specified event happens. Pass NULL to unregister the handler.
 * @param[in] callback_arg   Generic argument that will be provided to the callback when called, can be NULL
 */
void cyhal_gpio_register_callback(cyhal_gpio_t pin, cyhal_gpio_event_callback_t callback, void *callback_arg);

/** Enable or Disable the specified GPIO event <br>
 *
 * When an enabled event occurs, the function specified by \ref cyhal_gpio_register_callback will be called.
 *
 * See \ref subsection_gpio_snippet_4.
 *
 * @param[in] pin           The GPIO object
 * @param[in] event         The GPIO event
 * @param[in] intr_priority The priority for NVIC interrupt events
 * @param[in] enable        True to turn on interrupts, False to turn off
 */
void cyhal_gpio_enable_event(cyhal_gpio_t pin, cyhal_gpio_event_t event, uint8_t intr_priority, bool enable);

/*******************************************************************************
* Backward compatibility macro. The following code is DEPRECATED and must
* not be used in new projects
*******************************************************************************/
/** \cond INTERNAL */
#define cyhal_gpio_register_irq(pin, priority, handler, handler_arg)        cyhal_gpio_register_callback(pin, handler, handler_arg)
#define cyhal_gpio_irq_enable(pin, event, enable)          cyhal_gpio_enable_event(pin, event, CYHAL_ISR_PRIORITY_DEFAULT, enable)
typedef cyhal_gpio_event_t             cyhal_gpio_irq_event_t;
/** \endcond */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifdef CYHAL_GPIO_IMPL_HEADER
#include CYHAL_GPIO_IMPL_HEADER
#endif /* CYHAL_GPIO_IMPL_HEADER */

/** \} group_hal_gpio */
