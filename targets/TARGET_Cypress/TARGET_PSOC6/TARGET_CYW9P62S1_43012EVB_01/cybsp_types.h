/***********************************************************************************************//**
 * \copyright
 * Copyright 2018-2021 Cypress Semiconductor Corporation
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
 **************************************************************************************************/

#pragma once

#include "cybsp_doc.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * \addtogroup group_bsp_pin_state Pin States
 * \{
 * Macros to abstract out whether the LEDs & Buttons are wired high or active low.
 */
/** Pin state for the LED on. */
#ifndef CYBSP_LED_STATE_ON
#define CYBSP_LED_STATE_ON          (0U)
#endif
/** Pin state for the LED off. */
#ifndef CYBSP_LED_STATE_OFF
#define CYBSP_LED_STATE_OFF         (1U)
#endif
/** Pin state for when a button is pressed. */
#ifndef CYBSP_BTN_PRESSED
#define CYBSP_BTN_PRESSED           (0U)
#endif
/** Pin state for when a button is released. */
#ifndef CYBSP_BTN_OFF
#define CYBSP_BTN_OFF               (1U)
#endif
/** \} group_bsp_pin_state */

#define CYBSP_D8                    (NC)
#define CYBSP_D9                    (NC)

#if defined(__cplusplus)
}
#endif
