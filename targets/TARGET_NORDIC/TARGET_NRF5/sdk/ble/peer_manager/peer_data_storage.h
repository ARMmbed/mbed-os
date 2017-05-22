/* 
 * Copyright (c) 2015 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
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



#ifndef PEER_DATA_STORAGE_H__
#define PEER_DATA_STORAGE_H__


#include <stdint.h>
#include "sdk_errors.h"
#include "nrf_ble_gap.h"
#include "peer_manager_types.h"
#include "peer_manager_internal.h"


/**
 * @cond NO_DOXYGEN
 * @defgroup peer_data_storage Peer Data Storage
 * @ingroup peer_manager
 * @{
 * @brief An internal module of @ref peer_manager. This module provides a Peer Manager-specific API
 *        to the persistent storage.
 */

#define PDS_PREPARE_TOKEN_INVALID   0  /**< Invalid value for prepare token. */

enum
{
    PEER_ID_TO_FILE_ID         = 0xC000,
    FILE_ID_TO_PEER_ID         = -PEER_ID_TO_FILE_ID,
    PEER_DATA_ID_TO_RECORD_KEY = 0xC000,
    RECORD_KEY_TO_PEER_DATA_ID = -PEER_DATA_ID_TO_RECORD_KEY,
};


/**@brief The types of events that can come from the peer_data_storage module.
 */
typedef enum
{
    PDS_EVT_STORED,                 /**< The specified data has been successfully stored. */
    PDS_EVT_UPDATED,                /**< The specified data has been successfully updated. */
    PDS_EVT_CLEARED,                /**< The specified data has been successfully cleared. */
    PDS_EVT_ERROR_STORE,            /**< The specified data could not be stored. */
    PDS_EVT_ERROR_UPDATE,           /**< The specified data could not be updated. */
    PDS_EVT_ERROR_CLEAR,            /**< The specified data could not be cleared. */
    PDS_EVT_PEER_ID_CLEAR,          /**< The peer id has been successfully cleared. */
    PDS_EVT_ERROR_PEER_ID_CLEAR,    /**< The peer id has been successfully cleared. */
    PDS_EVT_COMPRESSED,             /**< A compress procedure has finished successfully. */
    PDS_EVT_ERROR_UNEXPECTED,       /**< An unexpected, possibly fatal error occurred. The unexpected error is included in the event structure. */
} pds_evt_id_t;


/**@brief Events that can come from the peer_data_storage module.
 */
typedef struct
{
    pds_evt_id_t      evt_id;      /**< The type of event. */
    pm_peer_id_t      peer_id;     /**< The peer the event pertains to. */
    pm_peer_data_id_t data_id;     /**< The data the event pertains to. */
    pm_store_token_t  store_token; /**< A unique identifier for the operation. Can be compare to the token received when starting the operation. */
    ret_code_t        result;      /**< The result of the operation, or the unexpected error. */
} pds_evt_t;


/**@brief Event handler for events from the peer_data_storage module.
 *
 * @param[in]  event    The event that has happened.
 * @param[in]  peer_id  The id of the peer the event pertains to.
 * @param[in]  flags    The data the event pertains to.
 */
typedef void (*pds_evt_handler_t)(pds_evt_t const * p_event);


/**@brief Function for registering for events from the peer database.
 *
 * @note This function will initialize the module if it is not already initialized.
 *
 * @param[in]  evt_handler  Event handler to register.
 *
 * @retval NRF_SUCCESS              Registration successful.
 * @retval NRF_ERROR_NO_MEM         No more event handlers can be registered.
 * @retval NRF_ERROR_NULL           evt_handler was NULL.
 * @retval NRF_ERROR_INTERNAL       An unexpected error happened.
 * @retval NRF_ERROR_INTERNAL       Unexpected error.
 */
ret_code_t pds_register(pds_evt_handler_t evt_handler);


/**@brief Function for retrieving a direct pointer to peer data in persistent storage.
 *
 * @param[in]  peer_id      The id of the peer whose data to read.
 * @param[in]  data_id      Which data to get.
 * @param[out] p_data       The peer data pointer.
 * @param[out] p_token      Token that can be used to lock data in flash and check data validity.
 *
 * @retval NRF_SUCCESS              The pointer was successfully retrieved.
 * @retval NRF_ERROR_INVALID_PARAM  Invalid data_id.
 * @retval NRF_ERROR_NOT_FOUND      The requested data was not found in persistent storage.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t pds_peer_data_read_ptr_get(pm_peer_id_t            peer_id,
                                      pm_peer_data_id_t       data_id,
                                      pm_peer_data_flash_t  * p_data,
                                      pm_store_token_t      * p_token);


#if 0
// @TODO: Finish documentation
/**@brief Function to lock the flash data (to defer compression from invalidating data)
 *
 * @param[in]   store_token     The token representing the item to lock
 *
 * @retval NRF_SUCCESS  Successfully locked
 */
ret_code_t pds_peer_data_lock(pm_store_token_t store_token);


/**@brief Function to verify flash data integrity
 *
 * @param[in]   store_token     The token representing the item to lock
 *
 * @retval NRF_SUCCESS              The data integrity is valid.
 * @retval NRF_ERROR_NULL           The token is invalid.
 * @retval NRF_ERROR_INVALID_DATA   The data integrity is not valid.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t  pds_peer_data_verify(pm_store_token_t store_token);
#endif


/**@brief Function for retrieving peer data from persistent storage by making a copy.
 *
 * @param[in]       peer_id     The id of the peer whose data to read.
 * @param[in]       data_id     Which piece of data to read.
 * @param[out]      p_data      Pointer to the peer data.
 * @param[in,out]   p_len_words Length available to copy to (in words).
 *                              If set to NULL, then no copy will be made and the
 *                              length will be reflected in p_len_words after the call returns.
 *
 * @retval NRF_SUCCESS              The read was successful.
 * @retval NRF_ERROR_INVALID_PARAM  Invalid data_id or peer_id.
 * @retval NRF_ERROR_NULL           Either \c p_data or \c p_len_words were \c NULL, or \c p_data
 *                                  contained a NULL pointer.
 * @retval NRF_ERROR_NOT_FOUND      The requested data was not found in persistent storage.
 * @retval NRF_ERROR_NO_MEM         The length of stored data too large to copy out.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t pds_peer_data_read(pm_peer_id_t          peer_id,
                              pm_peer_data_id_t     data_id,
                              pm_peer_data_t      * p_data,
                              uint16_t            * p_len_words);


/**@brief Function for preparing persistent storage for a write.
 *
 * @details If this call succeeds, space is reserved in persistent storage, so the write will fit.
 *
 * @note If space has already been prepared for this peer_id/data_id pair, no new space will be
 *       reserved, unless the previous reservation had too small size.
 *
 * @param[in]  p_peer_data      Data to prepare for. The data needs not be ready, but length and type
 *                              values must.
 * @param[out] p_prepare_token  A token identifying the prepared memory area.
 *
 * @retval NRF_SUCCESS               The call was successful.
 * @retval NRF_ERROR_NULL            Either \c p_peer_data or \c p_prepare code were \c NULL.
 * @retval NRF_ERROR_INVALID_PARAM   Invalid data ID.
 * @retval NRF_ERROR_INVALID_LENGTH  Data length exceeds the maximum length allowed.
 * @retval NRF_ERROR_NO_MEM          No space available in persistent storage.
 * @retval NRF_ERROR_INVALID_STATE   Module is not initialized.
 * @retval NRF_ERROR_INTERNAL        Internal error.
 */
ret_code_t pds_peer_data_write_prepare(pm_peer_data_const_t const * p_peer_data,
                                       pm_prepare_token_t         * p_prepare_token);


/**@brief Function for undoing a previous call to @ref pds_peer_data_write_prepare.
 *
 * @param[in]  prepare_token  A token identifying the prepared memory area to cancel.
 *
 * @retval NRF_SUCCESS               The call was successful.
 * @retval NRF_ERROR_NULL            Invalid value for \c prepare_token.
 * @retval NRF_ERROR_INVALID_STATE   Module is not initialized.
 * @retval NRF_ERROR_INTERNAL        Internal error.
 */
ret_code_t pds_peer_data_write_prepare_cancel(pm_prepare_token_t prepare_token);


/**@brief Function for writing prepared (reserved) peer data to persistent storage.
 *
 * @details Writing happens asynchronously. Expect a @ref PDS_EVT_STORED or @ref PDS_EVT_ERROR_STORE
 *          event.
 *
 * @param[in]  peer_id        The id of the peer the data pertains to.
 * @param[in]  p_peer_data    The peer data.
 * @param[in]  prepare_token  A token identifying the prepared memory area to write into. If
 *                            the prepare token is invalid, e.g. PDS_PREPARE_TOKEN_INVALID, the
 *                            prepare/write sequence will happen atomically.
 * @param[out] p_store_token  A token identifying this particular store operation. The token can be
 *                            used to identify events pertaining to this operation.
 *
 * @retval NRF_SUCCESS               The write was initiated successfully.
 * @retval NRF_ERROR_INVALID_PARAM   Invalid peer ID or data ID.
 * @retval NRF_ERROR_NULL            \c p_peer_data was \c NULL or contained a \c NULL pointer or
 *                                   \c prepare_token was zero.
 * @retval NRF_ERROR_NO_MEM          No space available in persistent storage. This can only happen
 *                                   if \c p_prepare_token is \c NULL.
 * @retval NRF_ERROR_BUSY            FDS or underlying modules are busy and can't take any
 *                                   more requests.
 * @retval NRF_ERROR_INVALID_STATE   Module is not initialized.
 * @retval NRF_ERROR_INTERNAL        Internal error.
 */
ret_code_t pds_peer_data_write_prepared(pm_peer_id_t                    peer_id,
                                        pm_peer_data_const_t    const * p_peer_data,
                                        pm_prepare_token_t              prepare_token,
                                        pm_store_token_t              * p_store_token);


/**@brief Function for writing peer data to persistent storage.
 *
 * @details Writing happens asynchronously. Expect a @ref PDS_EVT_STORED or @ref PDS_EVT_ERROR_STORE
 *          event.
 *
 * @param[in]  peer_id        The id of the peer the data pertains to.
 * @param[in]  p_peer_data    The peer data.
 * @param[out] p_store_token  A token identifying this particular store operation. The token can be
 *                            used to identify events pertaining to this operation.
 *
 * @retval NRF_SUCCESS               The write was initiated successfully.
 * @retval NRF_ERROR_INVALID_PARAM   Invalid data ID or peer ID.
 * @retval NRF_ERROR_NULL            \c p_peer_data was \c NULL or data contained a \c NULL pointer.
 * @retval NRF_ERROR_NO_MEM          No space available in persistent storage.
 * @retval NRF_ERROR_BUSY            FDS or underlying modules are busy and can't take any
 *                                   more requests.
 * @retval NRF_ERROR_INVALID_STATE   Module is not initialized.
 * @retval NRF_ERROR_INTERNAL        Internal error.
 */
ret_code_t pds_peer_data_write(pm_peer_id_t                 peer_id,
                               pm_peer_data_const_t const * p_peer_data,
                               pm_store_token_t           * p_store_token);


/**@brief Function for updating currently stored peer data to a new version
 *
 * @details Updating happens asynchronously.
 *          Expect a @ref PDS_EVT_STORED or @ref PDS_EVT_ERROR_STORE for the store token
 *          and a @ref PDS_EVT_ERROR_CLEAR or @ref PDS_EVT_ERROR_CLEAR for the old token
 *
 * @param[in]   peer_id             The peer which the data is associated to.
 * @param[in]   p_peer_data         New data.
 * @param[in]   old_token           Store token for the old data.
 * @param[out]  p_store_token       Store token for the new data.
 *
 * @retval NRF_SUCESS               The update was initiated successfully
 * @retval NRF_ERROR_NULL           \c p_peer_data was \c NULL or data contained a \c NULL pointer.
 * @retval NRF_ERROR_NO_MEM         No space available in persistent storage.
 * @retval NRF_ERROR_BUSY           FDS or underlying modules are busy and can't take any
 *                                  more requests at this moment.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 * @retval NRF_ERROR_INTERNAL       Internal error.
 */
ret_code_t pds_peer_data_update(pm_peer_id_t                 peer_id,
                                pm_peer_data_const_t const * p_peer_data,
                                pm_store_token_t             old_token,
                                pm_store_token_t           * p_store_token);


/**@brief Function for clearing peer data from persistent storage.
 *
 * @details Clearing happens asynchronously. Expect a @ref PDS_EVT_CLEARED or @ref PDS_EVT_ERROR_CLEAR
 *          event.
 *
 * @param[in]  peer_id  The id of the peer the data pertains to.
 * @param[in]  data_id  Which data to clear.
 *
 * @retval NRF_SUCCESS              The clear was initiated successfully.
 * @retval NRF_ERROR_INVALID_PARAM  Data ID or peer ID was invalid.
 * @retval NRF_ERROR_NOT_FOUND      Nothing to clear for this peer ID.
 * @retval NRF_ERROR_BUSY           FDS or underlying modules are busy and can't take any
 *                                  more requests at this moment.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 * @retval NRF_ERROR_INTERNAL       Internal error.
 */
ret_code_t pds_peer_data_clear(pm_peer_id_t peer_id, pm_peer_data_id_t data_id);


/**@brief Function for claiming an unused peer ID.
 *
 * @return  The first unused peer ID.
 * @retval  PM_PEER_ID_INVALID  If no peer ID is available or module is not initialized.
 */
pm_peer_id_t pds_peer_id_allocate(void);


/**@brief Function for freeing a peer ID and clearing all data associated with it in persistent
 *        storage.
 *
 * @param[in]  peer_id  Peer ID to free.
 *
 * @retval NRF_SUCCESS             The clear was initiated successfully.
 * @retval NRF_ERROR_INVALID_STATE Module not initialized.
 * @retval NRF_ERROR_INVALID_PARAM Invalid peer ID.
 */
ret_code_t pds_peer_id_free(pm_peer_id_t peer_id);


/**@brief Function for finding out whether a peer ID is in use.
 *
 * @param[in]  peer_id  The peer ID to inquire about.
 *
 * @retval  true   peer_id is in use.
 * @retval  false  peer_id is free, or the module is not initialized.
 */
bool pds_peer_id_is_allocated(pm_peer_id_t peer_id);


/**@brief Function for getting the next peer ID in the sequence of all used peer IDs. Can be
 *        used to loop through all used peer IDs.
 *
 * @note @ref PM_PEER_ID_INVALID is considered to be before the first and after the last ordinary
 *       peer ID.
 *
 * @param[in]  prev_peer_id  The previous peer ID.
 *
 * @return  The next peer ID.
 * @return  The first ordinary peer ID  if prev_peer_id was @ref PM_PEER_ID_INVALID.
 * @retval  PM_PEER_ID_INVALID          if prev_peer_id was the last ordinary peer ID or the module
 *                                      is not initialized.
 */
pm_peer_id_t pds_next_peer_id_get(pm_peer_id_t prev_peer_id);


/**@brief Function for querying the number of valid peer IDs available. I.E the number of peers
 *        in persistent storage.
 *
 * @return  The number of valid peer IDs, or 0 if module is not initialized.
 */
uint32_t pds_n_peers(void);


/** @}
 * @endcond
 */

#endif /* PEER_DATA_STORAGE_H__ */
