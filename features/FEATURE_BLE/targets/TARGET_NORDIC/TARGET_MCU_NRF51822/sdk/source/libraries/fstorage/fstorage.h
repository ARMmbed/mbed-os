/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef FS_H__
#define FS_H__

 /** @file
 *
 * @defgroup fstorage FStorage
 * @{
 * @ingroup app_common
 * @brief Module which provides low level functionality to store data to flash.
 *
 */


#include <stdint.h>
#include "section_vars.h"
#include "fstorage_config.h"
#include "sdk_errors.h"


typedef uint16_t fs_length_t;


typedef enum
{
    FS_OP_NONE          = 0,
    FS_OP_STORE         = 1,
    FS_OP_ERASE         = 2
} fs_oper_t;


/**@brief Callback for flash operations.
 *
 * @param[in]   op_code         Flash access operation code.
 * @param[in]   result          Result of the operation.
 * @param[in]   data            Pointer to resulting data (or NULL if not in use).
 * @param[in]   length_words    Length of data in words.
 */
typedef void (*fs_cb_t)(uint8_t             op_code,
                        uint32_t            result,
                        uint32_t    const * p_data,
                        fs_length_t         length_words);


/**@brief Flash storage config variable.
 *
 * @details     The fstorage module will update the start_addr and end_address according to 
 *              ordering rules and the number of pages requested by the fstorage module user.
 */
typedef struct
{
    const fs_cb_t   cb;               /**< Callback to run when flash operation has completed. */
    const uint8_t   num_pages;        /**< The number of pages to reserve for flash storage. */
    const uint8_t   page_order;       /**< The order used to allocate pages. */
    uint32_t      * p_start_addr;     /**< Pointer to the start address of the allocated flash storage. Set by running @ref fs_init. */
    uint32_t *      p_end_addr;       /**< Pointer to the end address of the allcoated flash storage. Set by running @ref fs_init. */
} fs_config_t;


/**@brief Macro for registering of flash storage configuration variable.
 *
 * @details This macro is expected to be invoked in the code unit that that require
 *          flash storage. Invoking this places the registered configuration variable
 *          in a section named "fs_data" that the fstorage module uses during initialization
 *          and regular operation.
 */
#define FS_SECTION_VARS_ADD(type_def) NRF_SECTION_VARS_ADD(fs_data, type_def)


/**@brief Function to initialize FStorage.
 *
 * @details     This function allocates flash data pages according to the
 *              number requested in the config variable. The data used to initialize.
 *              the fstorage is section placed variables in the data section "fs_data".
 */
ret_code_t fs_init(void);


/**@brief Function to store data in flash.
 *
 * @warning The data to be written to flash has to be kept in memory until the operation has
 *          terminated, i.e., a callback is received.
 *
 * @param[in]   p_config        Const pointer to configiguration of module user that requests a store operation.
 * @param[in]   p_addr          Write address of store operation.
 * @param[in]   p_data          Pointer to the data to store.
 * @param[in]   length_words    Length of the data to store.
 *
 * @retval NRF_SUCCESS                  Success. Command queued.
 * @retval NRF_ERROR_INVALID_STATE      Error. The module is not initialized.
 * @retval NRF_ERROR_INVALID_ADDR       Error. Data is unaligned or invalid configuration.
 * @retval Any error returned by the SoftDevice flash API.
 */
ret_code_t fs_store(fs_config_t const *       p_config,
                    uint32_t    const *       p_addr,
                    uint32_t    const * const p_data,
                    fs_length_t               length_words);


/** Function to erase a page in flash.
 *
 * @note        The erase address must be aligned on a page boundary. The length in words must be 
 *              equivalent to the page size.
 *
 * @param[in]   p_config        Pointer to the configuration of the user that requests the operation.
 * @param[in]   p_addr          Address of page to erase (the same as first word in the page).
 * @param[in]   length_words    Length (in 4 byte words) of the area to erase.
 *
 * @retval NRF_SUCCESS                  Success. Command queued.
 * @retval NRF_ERROR_INVALID_STATE      Error. The module is not initialized.
 * @retval NRF_ERROR_INVALID_ADDR       Error. Data is unaligned or invalid configuration.
 * @retval Any error returned by the SoftDevice flash API.
 */
ret_code_t fs_erase(fs_config_t const *       p_config,
                    uint32_t          * const p_addr,
                    fs_length_t               length_words);


/**@brief Function to call to handle events from the SoftDevice
 *
 * @param   sys_evt     System event from the SoftDevice
 */
void fs_sys_event_handler(uint32_t sys_evt);

/** @} */

#endif // FS_H__
