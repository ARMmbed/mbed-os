/***************************************************************************//**
* \file cyhal_pdmpcm.h
*
* \brief
* Provides a high level interface for interacting with the Cypress PDM/PCM.
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
* \addtogroup group_hal_pdmpcm PDM/PCM (PDM-PCM Converter)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress PDM/PCM.
*
* \defgroup group_hal_pdmpcm_macros Macros
* \defgroup group_hal_pdmpcm_functions Functions
* \defgroup group_hal_pdmpcm_data_structures Data Structures
* \defgroup group_hal_pdmpcm_enums Enumerated Types
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_hal_pdmpcm_enums
* \{
*/

/** PDM/PCM interrupt triggers */
typedef enum {
    /** TODO: Fill in */
    CY_PDM_PCM_TBD,
} cyhal_pdm_pcm_irq_event_t;

/** \} group_hal_pdmpcm_enums */

/**
* \addtogroup group_hal_pdmpcm_data_structures
* \{
*/

/** Describes the current configuration of a PDM/PCM */
typedef struct
{
    /** TODO: define */
    void * cfg;
} cyhal_pdm_pcm_cfg_t;

/** Handler for PDM/PCM interrupts */
typedef void (*cyhal_pdm_pcm_irq_handler_t)(void *handler_arg, cyhal_pdm_pcm_irq_event_t event);

/** \} group_hal_pdmpcm_data_structures */


/**
* \addtogroup group_hal_pdmpcm_functions
* \{
*/

/** Initialize the PDM/PCM peripheral
 *
 * Configures the pins used by PDM/PCM converter, sets a default format and frequency, and enables the peripheral
 * @param[out] obj The PDM/PCM object to initialize
 * @param[in]  in  The pin to use for PDM input
 * @param[in]  clk The pin to use for PDM clock output
 * @return The status of the init request
 */
cy_rslt_t cyhal_pdm_pcm_init(cyhal_pdm_pcm_t *obj, cyhal_gpio_t in, cyhal_gpio_t clk);

/** Release a PDM/PCM object
 *
 * Return the peripheral, pins and clock owned by the PDM/PCM object to their reset state
 * @param[in,out] obj The PDM/PCM object to deinitialize
 */
void cyhal_pdm_pcm_free(cyhal_pdm_pcm_t *obj);

/** Updates the configuration of the PDM/PCM object
 *
 * @param[inout] obj   The PDM/PCM object to configure
 * @param[in] cfg  The configuration of the PDM/PCM
 * @return The status of the format request
 */
cy_rslt_t cyhal_pdm_pcm_config(cyhal_pdm_pcm_t *obj, const cyhal_pdm_pcm_cfg_t *cfg);

/** Clears the FIFO
 *
 * @param[in] obj The PDM/PCM peripheral to use for sending
 * @return The status of the clear request
 */
cy_rslt_t cyhal_pdm_pcm_clear(cyhal_pdm_pcm_t *obj);

/** Reads a block out of the FIFO
 *
 * @param[in]     obj    The PDM/PCM peripheral to use for sending
 * @param[out]    data   Pointer to the byte-array of data to read from the device
 * @param[in,out] length Number of bytes to read, updated with the number actually read
 * @return The status of the read request
 */
cy_rslt_t cyhal_pdm_pcm_read(cyhal_pdm_pcm_t *obj, uint8_t *data, uint32_t *length);

/** Begin the PDM/PCM transfer
 *
 * @param[in]     obj     The PDM/PCM object that holds the transfer information
 * @param[out]    data    The receive buffer
 * @param[in,out] length  Number of bytes to read, updated with the number actually read
 * @return The status of the read_async request
 */
cy_rslt_t cyhal_pdm_pcm_read_async(cyhal_pdm_pcm_t *obj, void *data, size_t length);

/** Checks if the specified PDM/PCM peripheral is in use
 *
 * @param[in] obj  The PDM/PCM peripheral to check
 * @return Indication of whether the PDM/PCM is still transmitting
 */
bool cyhal_pdm_pcm_is_busy(cyhal_pdm_pcm_t *obj);

/** Abort an PDM/PCM transfer
 *
 * @param[in] obj The PDM/PCM peripheral to stop
 * @return The status of the abort_async request
 */
cy_rslt_t cyhal_pdm_pcm_abort_async(cyhal_pdm_pcm_t *obj);

/** The PDM/PCM interrupt handler registration
 *
 * @param[in] obj         The PDM/PCM object
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 */
void cyhal_pdm_pcm_register_irq(cyhal_pdm_pcm_t *obj, cyhal_pdm_pcm_irq_handler_t handler, void *handler_arg);

/** Configure PDM/PCM interrupt enablement.
 *
 * @param[in] obj      The PDM/PCM object
 * @param[in] event    The PDM/PCM IRQ type
 * @param[in] enable   True to turn on interrupts, False to turn off
 */
void cyhal_pdm_pcm_irq_enable(cyhal_pdm_pcm_t *obj, cyhal_pdm_pcm_irq_event_t event, bool enable);

/** \} group_hal_pdmpcm_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_hal_pdmpcm */
