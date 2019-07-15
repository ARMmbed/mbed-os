/***************************************************************************//**
* \file cyhal_gpio.h
*
* \brief
* Provides a high level interface for interacting with the Cypress GPIO.
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
*
* \defgroup group_hal_gpio_macros Macros
* \defgroup group_hal_gpio_functions Functions
* \defgroup group_hal_gpio_data_structures Data Structures
* \defgroup group_hal_gpio_enums Enumerated Types
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

/**
* \addtogroup group_hal_gpio_macros
* \{
*/

/** Integer representation of no connect pin (required to exist in all BSPs) */
#define CYHAL_NC_PIN_VALUE ((cyhal_gpio_t)0xFFFFFFFF)

/** \} group_hal_gpio_macros */


/**
* \addtogroup group_hal_gpio_enums
* \{
*/

/*******************************************************************************
*       Enumerations
*******************************************************************************/

/** Pin IRQ events */
typedef enum {
    CYHAL_GPIO_IRQ_NONE = 0,   /**< No interrupt */
    CYHAL_GPIO_IRQ_RISE = 1,   /**< Interrupt on rising edge */
    CYHAL_GPIO_IRQ_FALL = 2,   /**< Interrupt on falling edge */
    CYHAL_GPIO_IRQ_BOTH = 3,   /**< Interrupt on both rising and falling edges */
} cyhal_gpio_irq_event_t;

/** Pin direction */
typedef enum {
    CYHAL_GPIO_DIR_INPUT         = 0,  /**< Input pin */
    CYHAL_GPIO_DIR_OUTPUT        = 1,  /**< Output pin */
    CYHAL_GPIO_DIR_BIDIRECTIONAL = 2,   /**< Input and output pin */
} cyhal_gpio_direction_t;

/** Pin drive mode */
typedef enum {
    CYHAL_GPIO_DRIVE_ANALOG              = 0, /**< Analog Hi-Z */
    CYHAL_GPIO_DRIVE_PULLUP              = 2, /**< Pull-up resistor */
    CYHAL_GPIO_DRIVE_PULLDOWN            = 3, /**< Pull-down resistor */
    CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW  = 4, /**< Open-drain, Drives Low */
    CYHAL_GPIO_DRIVE_OPENDRAINDRIVESHIGH = 5, /**< Open-drain, Drives High */
    CYHAL_GPIO_DRIVE_STRONG              = 6, /**< Strong output */
    CYHAL_GPIO_DRIVE_PULLUPDOWN          = 7, /**< Pull-up and pull-down resistors */
} cyhal_gpio_drive_mode_t;

/** \} group_hal_gpio_enums */


/**
* \addtogroup group_hal_gpio_data_structures
* \{
*/

/** GPIO callback function type */
typedef void (*cyhal_gpio_irq_handler_t)(void *handler_arg, cyhal_gpio_irq_event_t event);

/** \} group_hal_gpio_data_structures */


/**
* \addtogroup group_hal_gpio_functions
* \{
*/

/*******************************************************************************
*       Functions
*******************************************************************************/

/** Initialize the GPIO pin
 *
 * @param[in]  pin The GPIO pin to initialize
 * @param[in]  direction The pin direction (input/output)
 * @param[in]  drvMode The pin drive mode
 * @param[in]  initVal Initial value on the pin
 *
 * @return The status of the init request
 */
cy_rslt_t cyhal_gpio_init(cyhal_gpio_t pin, cyhal_gpio_direction_t direction, cyhal_gpio_drive_mode_t drvMode, bool initVal);

/** Uninitialize the gpio peripheral and the cyhal_gpio_t object
 *
 * @param[in] pin Pin number
 */
void cyhal_gpio_free(cyhal_gpio_t pin);

/** Set the pin direction
 *
 * @param[in] pin       The pin number
 * @param[in] direction The pin direction to be set
 * @return The status of the dir request
 */
cy_rslt_t cyhal_gpio_direction(cyhal_gpio_t pin, cyhal_gpio_direction_t direction);

/** Set the input pin mode
 *
 * @param[in] pin  The GPIO object
 * @param[in] drvMode The pin mode to be set
 *
 * @return The status of the mode request
 */
cy_rslt_t cyhal_gpio_drivemode(cyhal_gpio_t pin, cyhal_gpio_drive_mode_t drvMode);

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

/** Register/clear an interrupt handler for the pin toggle pin IRQ event
 *
 * @param[in] pin           The pin number
 * @param[in] intrPriority  The NVIC interrupt channel priority
 * @param[in] handler       The function to call when the specified event happens
 * @param[in] handler_arg   Generic argument that will be provided to the handler when called
 */
void cyhal_gpio_register_irq(cyhal_gpio_t pin, uint8_t intrPriority, cyhal_gpio_irq_handler_t handler, void *handler_arg);

/** Enable or Disable the GPIO IRQ
 *
 * @param[in] pin    The GPIO object
 * @param[in] event  The GPIO IRQ event
 * @param[in] enable True to turn on interrupts, False to turn off
 */
void cyhal_gpio_irq_enable(cyhal_gpio_t pin, cyhal_gpio_irq_event_t event, bool enable);

/** \} group_hal_gpio_functions */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** \} group_hal_gpio */
