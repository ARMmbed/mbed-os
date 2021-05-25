/***************************************************************************//**
* \file cyhal_dma.h
*
* \brief
* Provides a high level interface for interacting with the Cypress DMA.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
*
********************************************************************************
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
*******************************************************************************/

/**
 * \addtogroup group_hal_dma DMA (Direct Memory Access)
 * \ingroup group_hal
 * \{
 * High level interface for interacting with the direct memory access (DMA).
 * The DMA driver allows for initializing and configuring a DMA channel in
 * order to trigger data transfers to and from memory and peripherals. The
 * transfers occur independently of the CPU and are triggered in software.
 * Multiple channels can be active at the same time each with their own
 * user-selectable priority and transfer characteristics.
 *
 * \section section_dma_features Features
 * * CPU independent memory access
 * * Access to memory and peripherals
 * * Multiple independent channels
 * * Configurable transer sizes and bursts
 * * Configurable priorities
 * * Event completion notification
 *
 * \section section_dma_quickstart Quick Start
 *
 * See \ref subsection_dma_snippet_1 for a code snippet that sets up a DMA
 * transfer to move memory from one location to another.
 *
 * \section section_dma_snippets Code snippets
 * \note Error handling code has been intentionally left out of snippets to highlight API usage.
 *
 * \subsection subsection_dma_snippet_1 Snippet 1: Simple DMA initialization and transfer
 * The following snippet initializes a DMA channel and uses it to transfer a a single block of memory.
 * The DMA channel is reserved by calling \ref cyhal_dma_init. It then needs to be configured with
 * \ref cyhal_dma_configure and then the transfer is started with \ref cyhal_dma_start_transfer.<br>
 * If the DMA channel is not needed anymore, it can be released by calling \ref cyhal_dma_free
 *
 * \snippet hal_dma.c snippet_cyhal_dma_simple_init
 *
 *
 * \subsection subsection_dma_snippet_2 Snippet 2: Configuring the DMA channel based on memory requirements
 * \ref cyhal_dma_configure can be used after DMA initialization to handle a variety of memory layouts.
 *
 * \snippet hal_dma.c snippet_cyhal_dma_configure
 *
 *
 * \subsection subsection_dma_snippet_3 Snippet 3: Interrupts and retriggering DMA transfers
 * DMA events like transfer complete or error events can be used to trigger a callback function. <br>
 * This snippet uses \ref cyhal_dma_configure to break the full transfer into multiple bursts. This
 * allows higher priority items access to the memory bus if necessary while the DMA operation is still
 * in progress. It then uses \ref cyhal_dma_enable_event() to enable the transfer complete event to
 * trigger the callback function registered by \ref cyhal_dma_register_callback().
 *
 * \snippet hal_dma.c snippet_cyhal_dma_events
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** \addtogroup group_hal_results_dma DMA HAL Results
 *  DMA specific return codes
 *  \ingroup group_hal_results
 *  \{ *//**
 */

/** Invalid transfer width parameter error */
#define CYHAL_DMA_RSLT_ERR_INVALID_TRANSFER_WIDTH       \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_DMA, 0))
/** Invalid parameter error */
#define CYHAL_DMA_RSLT_ERR_INVALID_PARAMETER            \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_DMA, 1))
/** Invalid priority parameter error */
#define CYHAL_DMA_RSLT_ERR_INVALID_PRIORITY             \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_DMA, 2))
/** Invalid src or dst addr alignment error */
#define CYHAL_DMA_RSLT_ERR_INVALID_ALIGNMENT            \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_DMA, 3))
/** Invalid burst_size paramenter error */
#define CYHAL_DMA_RSLT_ERR_INVALID_BURST_SIZE           \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_DMA, 4))
/** Channel busy error */
#define CYHAL_DMA_RSLT_ERR_CHANNEL_BUSY                 \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_DMA, 5))
/** Transfer has already been started warning */
#define CYHAL_DMA_RSLT_WARN_TRANSFER_ALREADY_STARTED    \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_WARNING, CYHAL_RSLT_MODULE_DMA, 6))
/** Unsupported hardware error */
#define CYHAL_DMA_RSLT_FATAL_UNSUPPORTED_HARDWARE       \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_FATAL, CYHAL_RSLT_MODULE_DMA, 7))

/**
 * \}
 */

/** Direction for DMA transfers. */
typedef enum
{
    CYHAL_DMA_DIRECTION_MEM2MEM,       //!< Memory to memory
    CYHAL_DMA_DIRECTION_MEM2PERIPH,    //!< Memory to peripheral
    CYHAL_DMA_DIRECTION_PERIPH2MEM,    //!< Peripheral to memory
    CYHAL_DMA_DIRECTION_PERIPH2PERIPH, //!< Peripheral to peripheral
} cyhal_dma_direction_t;

/** Flags enum of DMA events. Multiple events can be enabled. */
typedef enum
{
    CYHAL_DMA_NO_INTR             = 0,      //!< No interrupt
    CYHAL_DMA_TRANSFER_COMPLETE   = 1 << 0, //!< Indicates that a burst or full transfer has completed
    CYHAL_DMA_SRC_BUS_ERROR       = 1 << 1, //!< Indicates that there is a source bus error
    CYHAL_DMA_DST_BUS_ERROR       = 1 << 2, //!< Indicates that there is a destination bus error
    CYHAL_DMA_SRC_MISAL           = 1 << 3, //!< Indicates that the source address is not aligned
    CYHAL_DMA_DST_MISAL           = 1 << 4, //!< Indicates that the destination address is not aligned
    CYHAL_DMA_CURR_PTR_NULL       = 1 << 5, //!< Indicates that the current descriptor pointer is null
    CYHAL_DMA_ACTIVE_CH_DISABLED  = 1 << 6, //!< Indicates that the active channel is disabled
    CYHAL_DMA_DESCR_BUS_ERROR     = 1 << 7, //!< Indicates that there has been a descriptor bus error
} cyhal_dma_event_t;

/** Specifies the transfer type to trigger when an input signal is received.
 * */
typedef enum
{
    CYHAL_DMA_INPUT_TRIGGER_SINGLE_ELEMENT, //!< Transfer a single element when an input signal is received
    CYHAL_DMA_INPUT_TRIGGER_SINGLE_BURST,   //!< Transfer a single burst when an input signal is received
    CYHAL_DMA_INPUT_TRIGGER_ALL_ELEMENTS,   //!< Transfer all elements when an input signal is received
} cyhal_dma_input_t;

/** Specifies the transfer completion event that triggers a signal output.  */
typedef enum
{
    CYHAL_DMA_OUTPUT_TRIGGER_SINGLE_ELEMENT, //!< Trigger an output when a single element is transferred
    CYHAL_DMA_OUTPUT_TRIGGER_SINGLE_BURST,   //!< Trigger an output when a single burst is transferred
    CYHAL_DMA_OUTPUT_TRIGGER_ALL_ELEMENTS,   //!< Trigger an output when all elements are transferred
} cyhal_dma_output_t;

/** If burst_size is used, selects whether a single trigger of the channel
 * transfers a single burst of burst_size or a full transfer of size length
 * (that is, every burst is triggered). This will also set the initial transfer
 * type that will trigger an output signal on completion.*/
typedef enum
{
    CYHAL_DMA_TRANSFER_BURST, //!< A single burst is triggered and a transfer completion event will occur after the burst
    CYHAL_DMA_TRANSFER_FULL,  //!< All bursts are triggered and a single transfer completion event will occur at the end of all of them
} cyhal_dma_transfer_action_t;

/** \brief Configuration of a DMA channel. When configuring address,
 * increments, and transfer width keep in mind your hardware may have more
 * stringent address and data alignment requirements. */
typedef struct
{
    uint32_t src_addr;                  //!< Source address
    int16_t  src_increment;             //!< Source address auto increment amount in multiples of transfer_width
    uint32_t dst_addr;                  //!< Destination address
    int16_t  dst_increment;             //!< Destination address auto increment amount in multiples of transfer_width
    uint8_t  transfer_width;            //!< Transfer width in bits. Valid values are: 8, 16, or 32
    uint32_t length;                    //!< Number of elements to be transferred in total
    uint32_t burst_size;                //!< Number of elements to be transferred per trigger. If set to 0 every element is transferred, otherwise burst_size must evenly divide length.
    cyhal_dma_transfer_action_t action; //!< Sets the behavior of the channel when triggered (using start_transfer). Ignored if burst_size is not configured.
} cyhal_dma_cfg_t;

/** Event handler for DMA interrupts */
typedef void (*cyhal_dma_event_callback_t)(void *callback_arg, cyhal_dma_event_t event);

/** DMA input connection information to setup while initializing the driver. */
typedef struct
{
    cyhal_source_t      source; //!< Source of signal to DMA; obtained from another driver's cyhal_<PERIPH>_enable_output
    cyhal_dma_input_t   input;  //!< DMA input signal to be driven
} cyhal_dma_src_t;

/** DMA output connection information to setup while initializing the driver. */
typedef struct
{
    cyhal_dma_output_t  output; //!< Output signal of DMA
    cyhal_dest_t        dest;   //!< Destination of DMA signal
} cyhal_dma_dest_t;

/** Initialize the DMA peripheral.
 *
 * If a source signal is provided for \p src, this will connect the provided signal to the DMA
 * just as would be done by calling \ref cyhal_dma_connect_digital. Similarly, if a destination
 * target is provided for \p dest this will enable the specified output just as would be done
 * by calling \ref cyhal_dma_enable_output.
 * @param[out] obj  Pointer to a DMA object. The caller must allocate the memory
 *  for this object but the init function will initialize its contents.
 * @param[in]  src          An optional source signal to connect to the DMA
 * @param[in]  dest         An optional destination singal to drive from the DMA
 * @param[out] dest_source  An optional pointer to user-allocated source signal object which
 * will be initialized by enable_output. If \p dest is non-null, this must also be non-null.
 * \p dest_source should be passed to (dis)connect_digital functions to (dis)connect the
 * associated endpoints.
 * @param[in]  priority     The priority of this DMA operation relative to others. The number of
 * priority levels which are supported is hardware dependent. All implementations define a
 * #CYHAL_DMA_PRIORITY_DEFAULT constant which is always valid. If supported, implementations will
 * also define #CYHAL_DMA_PRIORITY_HIGH, #CYHAL_DMA_PRIORITY_MEDIUM, and #CYHAL_DMA_PRIORITY_LOW.
 * The behavior of any other value is implementation defined. See the implementation-specific DMA
 * documentation for more details.
 * @param[in]  direction    The direction memory is copied
 * @return The status of the init request
 */
cy_rslt_t cyhal_dma_init_adv(cyhal_dma_t *obj, cyhal_dma_src_t *src, cyhal_dma_dest_t *dest, cyhal_source_t *dest_source, uint8_t priority, cyhal_dma_direction_t direction);

/** Initialize the DMA peripheral.
 *
 * @param[out] obj  Pointer to a DMA object. The caller must allocate the memory
 *  for this object but the init function will initialize its contents.
 * @param[in]  priority     The priority of this DMA operation relative to others. The number of priority levels which are supported is hardware dependent. All implementations define a #CYHAL_DMA_PRIORITY_DEFAULT constant which is always valid. If supported, implementations will also define #CYHAL_DMA_PRIORITY_HIGH, #CYHAL_DMA_PRIORITY_MEDIUM, and #CYHAL_DMA_PRIORITY_LOW. The behavior of any other value is implementation defined. See the implementation-specific DMA documentation for more details.
 * @param[in]  direction    The direction memory is copied
 * @return The status of the init request
 */
#define cyhal_dma_init(obj, priority, direction)    (cyhal_dma_init_adv(obj, NULL, NULL, NULL, priority, direction))

/** Free the DMA object. Freeing a DMA object while a transfer is in
    progress (see @ref cyhal_dma_is_busy) is invalid.
 *
 * @param[in,out] obj The DMA object
 */
void cyhal_dma_free(cyhal_dma_t *obj);

/** Setup a DMA descriptor for specified resource
 *
 * @param[in] obj    The DMA object
 * @param[in] cfg    Configuration parameters for the transfer
 * @return The status of the configure request
 */
cy_rslt_t cyhal_dma_configure(cyhal_dma_t *obj, const cyhal_dma_cfg_t *cfg);

/** Initiates DMA channel transfer for specified DMA object
 *
 * @param[in] obj    The DMA object
 * @return The status of the start_transfer request
 */
cy_rslt_t cyhal_dma_start_transfer(cyhal_dma_t *obj);

/** Checks whether a transfer is pending or running on the DMA channel
 *
 * @param[in] obj    The DMA object
 * @return True if DMA channel is busy
 */
bool cyhal_dma_is_busy(cyhal_dma_t *obj);

/** Register a DMA callback handler.
 *
 * This function will be called when one of the events enabled by \ref cyhal_dma_enable_event occurs.
 *
 * @param[in] obj          The DMA object
 * @param[in] callback     The callback handler which will be invoked when an event triggers
 * @param[in] callback_arg Generic argument that will be provided to the callback when called
 */
void cyhal_dma_register_callback(cyhal_dma_t *obj, cyhal_dma_event_callback_t callback, void *callback_arg);

/** Configure DMA event enablement.
 *
 * When an enabled event occurs, the function specified by \ref cyhal_dma_register_callback will be called.
 *
 * @param[in] obj            The DMA object
 * @param[in] event          The DMA event type
 * @param[in] intr_priority  The priority for NVIC interrupt events. The priority from the most recent call will take precedence, i.e all events will have the same priority.
 * @param[in] enable         True to turn on interrupts, False to turn off
 */
void cyhal_dma_enable_event(cyhal_dma_t *obj, cyhal_dma_event_t event, uint8_t intr_priority, bool enable);

/** Connects a source signal and enables the specified input to the DMA
 * channel
 *
 * @param[in] obj         The DMA object
 * @param[in] source      Source signal obtained from another driver's cyhal_<PERIPH>_enable_output
 * @param[in] input       Which input to enable
 * @return The status of the connection
 * */
cy_rslt_t cyhal_dma_connect_digital(cyhal_dma_t *obj, cyhal_source_t source, cyhal_dma_input_t input);

/** Enables the specified output signal from a DMA channel that is triggered when a transfer is completed
 *
 * @param[in]  obj         The DMA object
 * @param[in]  output      Which event triggers the output
 * @param[out] source      Pointer to user-allocated source signal object which
 * will be initialized by enable_output. \p source should be passed to
 * (dis)connect_digital functions to (dis)connect the associated endpoints.
 * @return The status of the output enable
 * */
cy_rslt_t cyhal_dma_enable_output(cyhal_dma_t *obj, cyhal_dma_output_t output, cyhal_source_t *source);

/** Disconnects a source signal and disables the specified input to the DMA channel
 *
 * @param[in] obj         The DMA object
 * @param[in] source      Source signal from cyhal_<PERIPH>_enable_output to disable
 * @param[in] input       Which input to disable
 * @return The status of the disconnect
 * */
cy_rslt_t cyhal_dma_disconnect_digital(cyhal_dma_t *obj, cyhal_source_t source, cyhal_dma_input_t input);

/** Disables the specified output signal from a DMA channel
 *
 * @param[in]  obj         The DMA object
 * @param[in]  output      Which output to disable
 * @return The status of the disablement
 * */
cy_rslt_t cyhal_dma_disable_output(cyhal_dma_t *obj, cyhal_dma_output_t output);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_DMA_IMPL_HEADER
#include CYHAL_DMA_IMPL_HEADER
#endif /* CYHAL_DMA_IMPL_HEADER */

/** \} group_hal_dma */
