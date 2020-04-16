/***************************************************************************//**
* \file cyhal_sdio.h
*
* \brief
* Provides a high level interface for interacting with the Cypress SDIO interface.
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
* \addtogroup group_hal_sdio SDIO (Secure Digital Input Output)
* \ingroup group_hal
* \{
* High level interface for interacting with the Secure Digital Input Output (SDIO) interface.
*
* The Secure Digital Input Output (SDIO) protocol is an extension of the SD
* interface for general I/O functions.
*
* This driver allows commands to be sent over the SDIO bus; the supported commands
* can be found in cyhal_sdio_command_t. Bulk data transfer is also supported
* via cyhal_sdio_bulk_transfer().
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define CYHAL_SDIO_RET_NO_ERRORS           (0x00)    /**< No error*/
#define CYHAL_SDIO_RET_NO_SP_ERRORS        (0x01)    /**< Non-specific error code*/
#define CYHAL_SDIO_RET_CMD_CRC_ERROR       (0x02)    /**< There was a CRC error on the Command/Response*/
#define CYHAL_SDIO_RET_CMD_IDX_ERROR       (0x04)    /**< The index for the command didn't match*/
#define CYHAL_SDIO_RET_CMD_EB_ERROR        (0x08)    /**< There was an end bit error on the command*/
#define CYHAL_SDIO_RET_DAT_CRC_ERROR       (0x10)    /**< There was a data CRC Error*/
#define CYHAL_SDIO_RET_CMD_TIMEOUT         (0x20)    /**< The command didn't finish before the timeout period was over*/
#define CYHAL_SDIO_RET_DAT_TIMEOUT         (0x40)    /**< The data didn't finish before the timeout period was over*/
#define CYHAL_SDIO_RET_RESP_FLAG_ERROR     (0x80)    /**< There was an error in the resposne flag for command 53*/

#define CYHAL_SDIO_CLOCK_ERROR             (0x100)    /**< Failed to initial clock for SDIO */
#define CYHAL_SDIO_BAD_ARGUMENT            (0x200)    /**< Bad argument passed for SDIO */
#define CYHAL_SDIO_SEMA_NOT_INITED         (0x400)    /**< Semaphore is not initiated */
#define CYHAL_SDIO_FUNC_NOT_SUPPORTED      (0x800)    /**< Function is not supported */
#define CYHAL_SDIO_CANCELED               (0x1000)    /**< Operation canceled */

/* HAL return value defines */

/** Incorrect parameter value define */
#define CYHAL_SDIO_RSLT_ERR_BAD_PARAM          CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, \
                                                              CYHAL_RSLT_MODULE_SDIO, \
                                                              CYHAL_SDIO_BAD_ARGUMENT)

/** Clock initialization error define */
#define CYHAL_SDIO_RSLT_ERR_CLOCK             CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, \
                                                             CYHAL_RSLT_MODULE_SDIO, \
                                                             CYHAL_SDIO_CLOCK_ERROR)

/** Semaphore not initiated error define */
#define CYHAL_SDIO_RSLT_ERR_SEMA_NOT_INITED   CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, \
                                                             CYHAL_RSLT_MODULE_SDIO, \
                                                             CYHAL_SDIO_SEMA_NOT_INITED)

/** Error define based on SDIO lower function return value */
#define CYHAL_SDIO_RSLT_ERR_FUNC_RET(retVal)  CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, \
                                                             CYHAL_RSLT_MODULE_SDIO, (retVal))

/** Define to indicate canceled operation */
#define CYHAL_SDIO_RSLT_CANCELED             CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, \
                                                            CYHAL_RSLT_MODULE_SDIO, \
                                                            CYHAL_SDIO_CANCELED)

/** Commands that can be issued */
typedef enum
{
    CYHAL_SDIO_CMD_GO_IDLE_STATE      =  0, //!> Go to idle state
    CYHAL_SDIO_CMD_SEND_RELATIVE_ADDR =  3, //!> Send a relative address
    CYHAL_SDIO_CMD_IO_SEND_OP_COND    =  5, //!> Send an OP IO
    CYHAL_SDIO_CMD_SELECT_CARD        =  7, //!> Send a card select
    CYHAL_SDIO_CMD_GO_INACTIVE_STATE  = 15, //!> Go to inactive state
    CYHAL_SDIO_CMD_IO_RW_DIRECT       = 52, //!> Perform a direct read/write
    CYHAL_SDIO_CMD_IO_RW_EXTENDED     = 53, //!> Perform an extended read/write
} cyhal_sdio_command_t;

/** Types of transfer that can be performed */
typedef enum
{
    CYHAL_READ, //!> Read from the card
    CYHAL_WRITE //!> Write to the card
} cyhal_transfer_t;

/** Types of events that could be asserted by SDIO */
typedef enum {
    /* Interrupt-based thread events */
    CYHAL_SDIO_CMD_COMPLETE   = 0x00001,  //!> Command Complete
    CYHAL_SDIO_XFER_COMPLETE  = 0x00002,  //!> Host read/write transfer is complete
    CYHAL_SDIO_BGAP_EVENT     = 0x00004,  //!> This bit is set when both read/write transaction is stopped
    CYHAL_SDIO_DMA_INTERRUPT  = 0x00008,  //!> Host controller detects an SDMA Buffer Boundary during transfer
    CYHAL_SDIO_BUF_WR_READY   = 0x00010,  //!> This bit is set if the Buffer Write Enable changes from 0 to 1
    CYHAL_SDIO_BUF_RD_READY   = 0x00020,  //!> This bit is set if the Buffer Read Enable changes from 0 to 1
    CYHAL_SDIO_CARD_INSERTION = 0x00040,  //!> This bit is set if the Card Inserted in the Present State
    CYHAL_SDIO_CARD_REMOVAL   = 0x00080,  //!> This bit is set if the Card Inserted in the Present State
    CYHAL_SDIO_CARD_INTERRUPT = 0x00100,  //!> The synchronized value of the DAT[1] interrupt input for SD mode
    CYHAL_SDIO_INT_A          = 0x00200,  //!> Reserved: set to 0
    CYHAL_SDIO_INT_B          = 0x00400,  //!> Reserved: set to 0
    CYHAL_SDIO_INT_C          = 0x00800,  //!> Reserved: set to 0
    CYHAL_SDIO_RE_TUNE_EVENT  = 0x01000,  //!> Reserved: set to 0
    CYHAL_SDIO_FX_EVENT       = 0x02000,  //!> This status is set when R[14] of response register is set to 1
    CYHAL_SDIO_CQE_EVENT      = 0x04000,  //!> This status is set if Command Queuing/Crypto event has occurred
    CYHAL_SDIO_ERR_INTERRUPT  = 0x08000,  //!> If any of the bits in the Error Interrupt Status register are set

    /* Non-interrupt-based thread events */
    CYHAL_SDIO_GOING_DOWN     = 0x10000, //!> The interface is going away (eg: powering down for some period of time)
    CYHAL_SDIO_COMING_UP      = 0x20000, //!> The interface is back up (eg: came back from a low power state)

    CYHAL_SDIO_ALL_INTERRUPTS = 0x0E1FF,  //!> Is used to enable/disable all interrupts events
} cyhal_sdio_event_t;

/** @brief SDIO controller initial configuration */
typedef struct
{
    uint32_t frequencyhal_hz; //!< Clock frequency, in hertz
    uint16_t block_size;      //!< Block size
} cyhal_sdio_cfg_t;

/** Callback for SDIO events */
typedef void (*cyhal_sdio_event_callback_t)(void *callback_arg, cyhal_sdio_event_t event);

/** Initialize the SDIO peripheral
 *
 * @param[out] obj               The SDIO object
 * @param[out] clk               The pin connected to the clk signal
 * @param[in]  cmd               The pin connected to the command signal
 * @param[in]  data0             The pin connected to the data0 signal
 * @param[in]  data1             The pin connected to the data1 signal
 * @param[in]  data2             The pin connected to the data2 signal
 * @param[in]  data3             The pin connected to the data3 signal
 * @return The status of the init request
 */
cy_rslt_t cyhal_sdio_init(cyhal_sdio_t *obj, cyhal_gpio_t cmd, cyhal_gpio_t clk, cyhal_gpio_t data0, cyhal_gpio_t data1, cyhal_gpio_t data2, cyhal_gpio_t data3);

/** Release the SDIO peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param[in,out] obj The SDIO object
 */
void cyhal_sdio_free(cyhal_sdio_t *obj);

/** Configure the SDIO block.
 *
 * @param[in,out] obj    The SDIO object
 * @param[in]     config The SDIO configuration to apply
 * @return The status of the configure request
 */
cy_rslt_t cyhal_sdio_configure(cyhal_sdio_t *obj, const cyhal_sdio_cfg_t *config);

/** Sends a command to the SDIO block.
 *
 * @param[in,out] obj       The SDIO object
 * @param[in]     direction The direction of transfer (read/write)
 * @param[in]     command   The SDIO command to send
 * @param[in]     argument  The argument to the command
 * @param[out]    response  The response from the SDIO device
 * @return The status of the configure request
 */
cy_rslt_t cyhal_sdio_send_cmd(const cyhal_sdio_t *obj, cyhal_transfer_t direction, cyhal_sdio_command_t command, uint32_t argument, uint32_t* response);

/** Performs a bulk data transfer (CMD=53) to the SDIO block.
 *
 * @param[in,out] obj       The SDIO object
 * @param[in]     direction The direction of transfer (read/write)
 * @param[in]     argument  The argument to the command
 * @param[in]     data      The data to send to the SDIO device. A bulk transfer is done in block
 *                          size (default: 64 bytes) chunks for better performance. Therefore,
 *                          the size of the data buffer passed into this function must be at least
 *                          `length` bytes and a multiple of the block size. For example, when
 *                          requesting to read 100 bytes of data with a block size 64 bytes, the
 *                          data buffer needs to be at least 128 bytes. The first 100 bytes of data
 *                          in the buffer will be the requested data.
 * @param[in]     length    The number of bytes to send
 * @param[out]    response  The response from the SDIO device
 * @return The status of the configure request
 */
cy_rslt_t cyhal_sdio_bulk_transfer(cyhal_sdio_t *obj, cyhal_transfer_t direction, uint32_t argument, const uint32_t* data, uint16_t length, uint32_t* response);

/** Performs a bulk asynchronus data transfer (CMD=53) to the SDIO block.
 *
 * @param[in,out] obj       The SDIO object
 * @param[in]     direction The direction of transfer (read/write)
 * @param[in]     argument  The argument to the command
 * @param[in]     data      The data to send to the SDIO device
 * @param[in]     length    The number of bytes to send
 * @return The status of the configure request
 */
cy_rslt_t cyhal_sdio_transfer_async(cyhal_sdio_t *obj, cyhal_transfer_t direction, uint32_t argument, const uint32_t* data, uint16_t length);

/** Checks if the specified SDIO is in use
 *
 * @param[in]  obj  The SDIO peripheral to check
 * @return Indication of whether the SDIO is still transmitting
 */
bool cyhal_sdio_is_busy(const cyhal_sdio_t *obj);

/** Abort an SDIO transfer
 *
 * @param[in] obj The SDIO peripheral to stop
 * @return The status of the abort_async request
 */
cy_rslt_t cyhal_sdio_abort_async(const cyhal_sdio_t *obj);

/** The SDIO event callback registration
 *
 * @param[in] obj          The SDIO object
 * @param[in] callback     The callback function which will be invoked when the event triggers
 * @param[in] callback_arg Generic argument that will be provided to the callback when executed
 */
void cyhal_sdio_register_callback(cyhal_sdio_t *obj, cyhal_sdio_event_callback_t callback, void *callback_arg);

/** Configure which SDIO events trigger callbacks.
 *
 * @param[in] obj           The SDIO object
 * @param[in] event         The SDIO event type
 * @param[in] intrPriority  The priority for NVIC interrupt events
 * @param[in] enable        Set to true to enable events, or false to disable them
 */
void cyhal_sdio_enable_event(cyhal_sdio_t *obj, cyhal_sdio_event_t event, uint8_t intrPriority, bool enable);

/*******************************************************************************
* Backward compatibility macro. The following code is DEPRECATED and must
* not be used in new projects
*******************************************************************************/
/** \cond INTERNAL */
#define cyhal_sdio_register_irq        cyhal_sdio_register_callback
#define cyhal_sdio_irq_enable(obj, event, enable)          cyhal_sdio_enable_event(obj, event, CYHAL_ISR_PRIORITY_DEFAULT, enable)
typedef cyhal_sdio_event_t             cyhal_sdio_irq_event_t;
typedef cyhal_sdio_event_callback_t    cyhal_sdio_irq_handler_t;
/** \endcond */

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_SDIO_IMPL_HEADER
#include CYHAL_SDIO_IMPL_HEADER
#endif /* CYHAL_SDIO_IMPL_HEADER */

/** \} group_hal_sdio */
