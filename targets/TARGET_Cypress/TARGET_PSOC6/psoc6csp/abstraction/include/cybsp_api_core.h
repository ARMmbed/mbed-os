/***************************************************************************//**
* \file cybsp_api_core.h
*
* \brief
* Basic abstraction layer for dealing with boards containing a Cypress MCU. This
* API provides convenience methods for initializing and manipulating different
* hardware found on the board.
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
* \addtogroup group_abstraction_board Board abstraction
* \ingroup group_abstraction
* \{
* Basic abstraction layer for dealing with boards containing a Cypress MCU. This
* API provides convenience methods for initializing and manipulating different
* hardware found on the board.
*
* \defgroup group_abstraction_board_macros Macros
* \defgroup group_abstraction_board_functions Functions
*/

#pragma once

#include <stdbool.h>
#include "cy_result.h"
#include "cybsp_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_abstraction_board_macros
* \{
*/


/** \} group_abstraction_board_macros */

/**
* \addtogroup group_abstraction_board_functions
* \{
*/

/**
 * \brief Initialize all hardware on the board
 * \returns CY_RSLT_SUCCESS if the board is sucessfully initialized, if there is
 *          a problem initializing any hardware it returns an error code specific
 *          to the hardware module that had a problem.
 */
cy_rslt_t cybsp_init(void);

/**
 * \brief Init and allocate the specified LED, setting the GPIO drive mode as necessary
 * \param which The specific LED number to enable, see BSP header file for available LEDs
 * \returns CY_RSLT_SUCCESS if the LED was enabled successfully
 */
cy_rslt_t cybsp_led_init(cybsp_led_t which);

/**
 * \brief Toggle the specified LED
 * \param which The specific LED number to enable, see BSP header file for available LEDs
 */
void cybsp_led_toggle(cybsp_led_t which);

/**
 * \brief Sets the state of the LED.
 * \param which The specific LED number to set state, see BSP header file for available LEDs
 * \param on Whether the LED should be turned on (true) or off (false)
 */
void cybsp_led_set_state(cybsp_led_t which, bool on);

/**
 * \brief Turns the LED on.
 * \param which The specific LED number to set state, see BSP header file for available LEDs
 * \returns CY_RSLT_SUCCESS if the LED was turned on
 */
static inline void cybsp_led_on(cybsp_led_t which)
{
    cybsp_led_set_state(which, CYBSP_LED_STATE_ON);
}

/**
 * \brief Turns the LED off.
 * \param which The specific LED number to set state, see BSP header file for available LEDs
 * \returns CY_RSLT_SUCCESS if the LED was turned off
 */
static inline void cybsp_led_off(cybsp_led_t which)
{
    cybsp_led_set_state(which, CYBSP_LED_STATE_OFF);
}

/**
 * \brief Init and allocate the specified button, setting the GPIO drive mode as necessary
 * \param which The specific button number to enable, see BSP header file for available buttones
 * \returns CY_RSLT_SUCCESS if the button was enabled successfully
 */
cy_rslt_t cybsp_btn_init(cybsp_btn_t which);

/**
 * \brief Sets the state of the button.
 * \param which The specific button number to get state from, see BSP header file for available buttones
 * \returns State of the button
 */
bool cybsp_btn_get_state(cybsp_btn_t which);

/**
 * \brief Sets the interrupt to trigger when the button state is changed.
 * \param which The specific button number to get state from, see BSP header file for available buttones
 * \param type The type sets level vs edge and active high vs active low
 * \param callback The function pointer to call when the button state changes
 */
void cybsp_btn_set_interrupt(cybsp_btn_t which, cyhal_gpio_irq_event_t type, void (*callback)(void));


/** \} group_abstraction_board_functions */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** \} group_abstraction_board */
