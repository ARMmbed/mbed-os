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
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
* High level interface for interacting with the Cypress GPIO.
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
#define CYHAL_NC_PIN_VALUE ((cyhal_gpio_t)0xFFFFFFFF)

/*******************************************************************************
*       Enumerations
*******************************************************************************/

/** Pin events */
typedef enum {
    CYHAL_GPIO_IRQ_NONE,   /**< No interrupt */
    CYHAL_GPIO_IRQ_RISE,   /**< Interrupt on rising edge */
    CYHAL_GPIO_IRQ_FALL,   /**< Interrupt on falling edge */
    CYHAL_GPIO_IRQ_BOTH,   /**< Interrupt on both rising and falling edges */
} cyhal_gpio_event_t;

/** Pin direction */
typedef enum {
    CYHAL_GPIO_DIR_INPUT,         /**< Input pin */
    CYHAL_GPIO_DIR_OUTPUT,        /**< Output pin */
    CYHAL_GPIO_DIR_BIDIRECTIONAL, /**< Input and output pin */
} cyhal_gpio_direction_t;

/** Pin drive mode */
typedef enum {
    CYHAL_GPIO_DRIVE_NONE,                /**< No drive; Hi-Z */
    CYHAL_GPIO_DRIVE_ANALOG,              /**< Analog Hi-Z */
    CYHAL_GPIO_DRIVE_PULLUP,              /**< Pull-up resistor */
    CYHAL_GPIO_DRIVE_PULLDOWN,            /**< Pull-down resistor */
    CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW,  /**< Open-drain, Drives Low */
    CYHAL_GPIO_DRIVE_OPENDRAINDRIVESHIGH, /**< Open-drain, Drives High */
    CYHAL_GPIO_DRIVE_STRONG,              /**< Strong output */
    CYHAL_GPIO_DRIVE_PULLUPDOWN,          /**< Pull-up and pull-down resistors */
} cyhal_gpio_drive_mode_t;

/** GPIO callback function type */
typedef void (*cyhal_gpio_event_callback_t)(void *callback_arg, cyhal_gpio_event_t event);

/*******************************************************************************
*       Functions
*******************************************************************************/

/** Initialize the GPIO pin
 *
 * @param[in] pin       The GPIO pin to initialize
 * @param[in] direction The pin direction
 * @param[in] drvMode   The pin drive mode
 * @param[in] initVal   Initial value on the pin
 *
 * @return The status of the init request
 */
cy_rslt_t cyhal_gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drvMode, bool initVal);

/** Uninitialize the gpio peripheral and the cyhal_gpio_t object
 *
 * @param[in] pin Pin number
 */
void cyhal_gpio_free(cyhal_gpio_t pin);

/** Configure the GPIO pin
 *
 * @param[in] pin       The GPIO pin
 * @param[in] direction The pin direction
 * @param[in] drvMode   The pin drive mode
 *
 * @return The status of the configure request
 */
cy_rslt_t cyhal_gpio_configure(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drvMode);

/** Set the output value for the pin. This only works for output & in_out pins.
 *
 * @param[in] pin   The GPIO object
 * @param[in] value The value to be set (high = true, low = false)
 */
void cyhal_gpio_write(cyhal_gpio_t pin, bool value);

/** Read the input value.  This only works for input & in_out pins.
 *
 * @param[in]  pin   The GPIO object
 * @return The value of the IO (true = high, false = low)
 */
bool cyhal_gpio_read(cyhal_gpio_t pin);

/** Toggle the output value
 *
 * @param[in]  pin   The GPIO object
 */
void cyhal_gpio_toggle(cyhal_gpio_t pin);

/** Register/clear a callback handler for pin events
 *
 * @param[in] pin            The pin number
 * @param[in] callback       The function to call when the specified event happens. Pass NULL to unregister the handler.
 * @param[in] callback_arg   Generic argument that will be provided to the callback when called, can be NULL
 */
void cyhal_gpio_register_callback(cyhal_gpio_t pin, cyhal_gpio_event_callback_t callback, void *callback_arg);

/** Enable or Disable the specified GPIO event
 *
 * @param[in] pin           The GPIO object
 * @param[in] event         The GPIO event
 * @param[in] intrPriority  The priority for NVIC interrupt events
 * @param[in] enable        True to turn on interrupts, False to turn off
 */
void cyhal_gpio_enable_event(cyhal_gpio_t pin, cyhal_gpio_event_t event, uint8_t intrPriority, bool enable);

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
