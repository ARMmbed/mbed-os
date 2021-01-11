/***************************************************************************//**
* \file cyhal_sdhc.h
*
* \brief
* Provides a high level interface for interacting with the Cypress SDHC.
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
* \addtogroup group_hal_sdhc SDHC (SD Host Controller)
* \ingroup group_hal
* \{
* High level interface to the Secure Digital Host Controller (SDHC).
*
* The SDHC driver allows data to be read from and written to an SD Card using the SDHC block.
* The data is transferred in blocks with a block size of 512 bytes.
*
* \section subsection_sdhc_features Features
* * Supports the 4-bit interface
* * Supports Ultra High Speed (UHS-I) mode
* * Supports Default Speed (DS), High Speed (HS), SDR12, SDR25 and SDR50 speed modes
*
* \section subsection_sdhc_quickstart Quick Start
* Initialize SDHC by using \ref cyhal_sdhc_init by selecting the pins according to the target device used.
* Specify the SDHC configuration using the configuration structure (const \ref cyhal_sdhc_config_t * config). <br>
* See \ref subsection_sdhc_snippet_1
*
* \section subsection_sdhc_code_snippets Code Snippets
*
* \subsection subsection_sdhc_snippet_1 Snippet 1: SDHC Initialization and configuration
* The following snippet is used to initialize the SDHC block. SDHC object - \ref cyhal_sdhc_t,
* SDHC card configuration structure (const \ref cyhal_sdhc_config_t * config). The pins connected to the SDHC block
* needs to be provided to the \ref cyhal_sdhc_init function.
* \snippet sdhc.c snippet_cyhal_sdhc_init
*
* \subsection subsection_sdhc_snippet_2 Snippet 2: SDHC Initialization and configuration with custom card detect pin
* The following snippet is used to initialize the SDHC block with a custom card detect pin. Cy_SD_Host_IsCardConnected()
* function needs to be over-ridden depending on the card detect pin selected.
* \snippet sdhc.c snippet_cyhal_sdhc_init_custom_card_detect
*
* \subsection subsection_sdhc_snippet_3 Snippet 3: Reading a block of data from an SD Card
* The following snippet reads a block of data from the SD Card.
* \snippet sdhc.c snippet_cyhal_sdhc_read

* \subsection subsection_sdhc_snippet_4 Snippet 4: Writing a block of data to an SD Card
* The following snippet writes a block of data to the SD Card.
* \snippet sdhc.c snippet_cyhal_sdhc_write

*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
*       Defines
*******************************************************************************/

/** \addtogroup group_hal_results_sdhc SDHC HAL Results
 *  SDHC specific return codes
 *  \ingroup group_hal_results
 *  \{ *//**
 */

/**< Pin related Error. >*/
#define CYHAL_SDHC_RSLT_ERR_PIN                         \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SDHC, 0))
/** Requested feature is not supported on this hardware. */
#define CYHAL_SDHC_RSLT_ERR_UNSUPPORTED                 \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SDHC, 1))

/**
 * \}
 */


/*******************************************************************************
*       Enumerations
*******************************************************************************/

/** Card types */
typedef enum
{
    CYHAL_SDHC_SD, //!< Secure Digital card
    CYHAL_SDHC_SDIO, //!< SD Input Output card
    CYHAL_SDHC_EMMC, //!< Embedded Multimedia card
    CYHAL_SDHC_COMBO, //!< Combo Card (SD + SDIO)
    CYHAL_SDHC_UNUSABLE, //!< Unusable card or unsupported type
    CYHAL_SDHC_NOT_EMMC, //!< Not an eMMC card
} cyhal_sdhc_card_type_t;

/** SDHC interrupt triggers */
typedef enum {
    CYHAL_SDHC_CMD_COMPLETE         = 0x00001, //!> Command Complete
    CYHAL_SDHC_XFER_COMPLETE        = 0x00002, //!> Host read/write transfer is complete
    CYHAL_SDHC_BGAP_EVENT           = 0x00004, //!> This bit is set when both read/write transaction is stopped at the block gap
    CYHAL_SDHC_DMA_INTERRUPT        = 0x00008, //!> Host controller detects an SDMA Buffer Boundary during transfer
    CYHAL_SDHC_BUF_WR_READY         = 0x00010, //!> This bit is set if the Buffer Write Enable changes from 0 to 1
    CYHAL_SDHC_BUF_RD_READY         = 0x00020, //!> This bit is set if the Buffer Read Enable changes from 0 to 1
    CYHAL_SDHC_CARD_INSERTION       = 0x00040, //!> This bit is set if the Card Inserted in the Present State register changes from 0 to 1.
    CYHAL_SDHC_CARD_REMOVAL         = 0x00080, //!> This bit is set if the Card Inserted in the Present State register changes from 1 to 0.
    CYHAL_SDHC_CARD_INTERRUPT       = 0x00100, //!> The synchronized value of the DAT[1] interrupt input for SD mode
    CYHAL_SDHC_INT_A                = 0x00200,
    CYHAL_SDHC_INT_B                = 0x00400,
    CYHAL_SDHC_INT_C                = 0x00800,
    CYHAL_SDHC_RE_TUNE_EVENT        = 0x01000, //!> This bit is set if the Re-Tuning Request changes from 0 to 1
    CYHAL_SDHC_FX_EVENT             = 0x02000, //!> This status is set when R[14] of response register is set to 1
    CYHAL_SDHC_CQE_EVENT            = 0x04000, //!> This status is set if Command Queuing/Crypto event has occurred
    CYHAL_SDHC_ASYNC_READ_COMPLETE  = 0x10000, //!> Asynchronous read operation is complete
    CYHAL_SDHC_ASYNC_WRITE_COMPLETE = 0x20000, //!> Asynchronous write operation is complete
    CYHAL_SDHC_ERR_INTERRUPT        = 0x08000, //!> If any of the bits in the Error Interrupt Status register are set
    CYHAL_SDHC_ALL_INTERRUPTS       = 0x3FFFF, //!> Is used to enable/disable all interrupts
} cyhal_sdhc_event_t;


/*******************************************************************************
*       Typedefs
*******************************************************************************/

/** Handler for SDHC interrupts */
typedef void (*cyhal_sdhc_event_callback_t)(void *callback_arg, cyhal_sdhc_event_t event);

/*******************************************************************************
*       Data Structures
*******************************************************************************/

/** @brief Defines configuration options for the SDHC block */
typedef struct
{
    bool                 enableLedControl; //!< Drive one IO to indicate when the card is being accessed
    bool                 lowVoltageSignaling; //!< Whether 1.8V signaling is supported
    bool                 isEmmc;   //!< true if eMMC card, otherwise false
    uint8_t              busWidth; //!< The desired bus width
} cyhal_sdhc_config_t;

/*******************************************************************************
*       Functions
*******************************************************************************/

/** Initialize the SDHC peripheral
 *
 * @param[out] obj               Pointer to an SDHC object.
 *  The caller must allocate the memory for this object but the init function will
 *  initialize its contents.
 * @param[in]  config            The card configuration object
 * @param[out] clk               The pin connected to the clk signal
 * @param[in]  cmd               The pin connected to the command signal
 * @param[in]  data0             The pin connected to the data0 signal
 * @param[in]  data1             The pin connected to the data1 signal
 * @param[in]  data2             The pin connected to the data2 signal
 * @param[in]  data3             The pin connected to the data3 signal
 * @param[in]  data4             The pin connected to the data4 signal
 * @param[in]  data5             The pin connected to the data5 signal
 * @param[in]  data6             The pin connected to the data6 signal
 * @param[in]  data7             The pin connected to the data7 signal
 * @param[in]  cardDetect        The pin connected to the cardDetect signal
 * @param[out] ioVoltSel         The pin connected to the ioVoltSel signal
 * @param[out] cardIfPwrEn       The pin connected to the cardIfPwrEn signal
 * @param[in]  cardMechWriteProt The pin connected to the cardMechWriteProt signal
 * @param[in]  ledCtrl           The pin connected to the ledCtrl signal
 * @param[in]  cardEmmcReset     The pin connected to the cardEmmcReset signal
 * @return The status of the init request
 *
 */
cy_rslt_t cyhal_sdhc_init(cyhal_sdhc_t *obj,
                          const cyhal_sdhc_config_t *config,
                          cyhal_gpio_t cmd,
                          cyhal_gpio_t clk,
                          cyhal_gpio_t data0,
                          cyhal_gpio_t data1,
                          cyhal_gpio_t data2,
                          cyhal_gpio_t data3,
                          cyhal_gpio_t data4,
                          cyhal_gpio_t data5,
                          cyhal_gpio_t data6,
                          cyhal_gpio_t data7,
                          cyhal_gpio_t cardDetect,
                          cyhal_gpio_t ioVoltSel,
                          cyhal_gpio_t cardIfPwrEn,
                          cyhal_gpio_t cardMechWriteProt,
                          cyhal_gpio_t ledCtrl,
                          cyhal_gpio_t cardEmmcReset);

/** Release the SDHC peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param[in,out] obj The SDHC object
 */
void cyhal_sdhc_free(cyhal_sdhc_t *obj);

/** Attempts to read data synchronously over the SDHC peripheral.
 *
 * This will read as many blocks as possible, up to `length` blocks, into the buffer
 * pointed to by `data`, then return. The value pointed to by `length` will be
 * updated to reflect the number of words that were actually read.
 *
 * See \ref subsection_sdhc_snippet_3
 *
 * @param[in]     obj     The SDHC object
 * @param[in]     address The address to read data from
 * @param[out]    data    Pointer to the byte-array where data read from the device should be stored
 * @param[in,out] length  Number of 512 byte blocks to read, updated with the number actually read
 * @return The status of the read request
 */
cy_rslt_t cyhal_sdhc_read(const cyhal_sdhc_t *obj, uint32_t address, uint8_t *data, size_t *length);

/** Attempts to write data synchronously over SDHC peripheral
 *
 * This will write as many blocks as possible, up to `length` blocks, then return.
 * The value pointed to by `length` will be updated to reflect the number of words
 * that were actually read.
 *
 * See \ref subsection_sdhc_snippet_4
 *
 * @param[in]     obj     The SDHC object
 * @param[in]     address The address to write data to
 * @param[in]     data    Pointer to the byte-array of data to write to the device
 * @param[in,out] length  Number of 512 byte blocks to write, updated with the number actually written
 * @return The status of the write request
 *
 */
cy_rslt_t cyhal_sdhc_write(const cyhal_sdhc_t *obj, uint32_t address, const uint8_t *data, size_t *length);

/** Erases a block of data over the SDHC peripheral
 *
 * @param[in] obj        The SDHC object
 * @param[in] start_addr Is the address of the first byte to erase
 * @param[in] length     Number of 512 byte blocks (starting at start_addr) to erase
 * @return The status of the erase request
 *
 */
cy_rslt_t cyhal_sdhc_erase(const cyhal_sdhc_t *obj, uint32_t start_addr, size_t length);

/** Start SDHC asynchronous read
 *
 * This will transfer `length` 512 byte blocks into the buffer pointed to by `data` in the background.
 * When the requested quantity of data has been read, the @ref CYHAL_SDHC_ASYNC_READ_COMPLETE event will
 * be raised. See @ref cyhal_sdhc_register_callback and @ref cyhal_sdhc_enable_event.
 *
 * @param[in]     obj     The SDHC object that holds the transfer information
 * @param[in]     address The address to read data from
 * @param[out]    data    The receive buffer
 * @param[in,out] length  Number of 512 byte blocks to read, updated with the number actually read
 * @return The status of the read_async request
 */
cy_rslt_t cyhal_sdhc_read_async(const cyhal_sdhc_t *obj, uint32_t address, uint8_t *data, size_t *length);

/** Start asynchronous SDHC write
 *
 * This will transfer `length` 512 byte blocks from the buffer pointed to by `data` in the background.
 * When the requested quantity of data has been written, the @ref CYHAL_SDHC_ASYNC_WRITE_COMPLETE event
 * will be raised. See @ref cyhal_sdhc_register_callback and @ref cyhal_sdhc_enable_event.
 * @param[in]     obj     The SDHC object that holds the transfer information
 * @param[in]     address The address to write data to
 * @param[in]     data    The transmit buffer
 * @param[in,out] length  The number of 512 byte blocks to write, updated with the number actually written
 * @return The status of the write_async request
 */
cy_rslt_t cyhal_sdhc_write_async(const cyhal_sdhc_t *obj, uint32_t address, const uint8_t *data, size_t *length);

/** Checks if the specified SDHC peripheral is in use
 *
 * @param[in]  obj  The SDHC peripheral to check
 * @return Indication of whether the SDHC is still transmitting
 */
bool cyhal_sdhc_is_busy(const cyhal_sdhc_t *obj);

/** Abort an SDHC transfer
 *
 * @param[in] obj The SDHC peripheral to stop
 * @return The status of the abort_async request
 */
cy_rslt_t cyhal_sdhc_abort_async(const cyhal_sdhc_t *obj);

/** Register an SDHC callback handler
 *
 * This function will be called when one of the events enabled by \ref cyhal_sdhc_enable_event occurs.
 *
 * @param[in] obj          The SDHC object
 * @param[in] callback     The callback handler which will be invoked when the event fires
 * @param[in] callback_arg Generic argument that will be provided to the callback when called
 */
void cyhal_sdhc_register_callback(cyhal_sdhc_t *obj, cyhal_sdhc_event_callback_t callback, void *callback_arg);

/** Configure SDHC event enablement.
 *
 * When an enabled event occurs, the function specified by \ref cyhal_sdhc_register_callback will be called.
 *
 * @param[in] obj           The SDHC object
 * @param[in] event         The SDHC event type
 * @param[in] intr_priority  The priority for NVIC interrupt events
 * @param[in] enable        True to turn on interrupts, False to turn off
 */
void cyhal_sdhc_enable_event(cyhal_sdhc_t *obj, cyhal_sdhc_event_t event, uint8_t intr_priority, bool enable);


#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_SDHC_IMPL_HEADER
#include CYHAL_SDHC_IMPL_HEADER
#endif /* CYHAL_SDHC_IMPL_HEADER */

/** \} group_hal_sdhc */
