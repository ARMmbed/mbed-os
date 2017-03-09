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


#ifndef PEER_DATABASE_H__
#define PEER_DATABASE_H__

#include <stdint.h>
#include "peer_manager_types.h"
#include "sdk_errors.h"

/**
 * @defgroup peer_database Peer Database
 * @ingroup peer_manager
 * @{
 * @brief An internal module of @ref peer_manager. A module for simple management of reading and
 *        writing of peer data into persistent storage.
 *
 */

#define PDB_WRITE_BUF_SIZE (sizeof(pm_peer_data_bonding_t))

/**@brief Events that can come from the peer_database module.
 */
typedef enum
{
    PDB_EVT_WRITE_BUF_STORED,   /**< A pdb_write_buf_store operation has completed successfully. */
    PDB_EVT_RAW_STORED,         /**< A pdb_raw_store operation has completed successfully. */
    PDB_EVT_RAW_STORE_FAILED,   /**< A pdb_raw_store operation has failed. */
    PDB_EVT_CLEARED,            /**< A pdb_clear operation has completed successfully. */
    PDB_EVT_CLEAR_FAILED,       /**< A pdb_clear operation has failed. */
    PDB_EVT_COMPRESSED,         /**< A compress procedure has completed. */
    PDB_EVT_ERROR_NO_MEM,       /**< An operation is blocked because the flash is full. It will be reattempted automatically after the next compress procedure. */
    PDB_EVT_ERROR_UNEXPECTED,   /**< An unexpected error occurred. This is a fatal error. */
} pdb_evt_id_t;

/**@brief Events that can come from the peer_database module.
 */
typedef struct
{
    pdb_evt_id_t      evt_id;  /**< The event that has happened. */
    pm_peer_id_t      peer_id; /**< The id of the peer the event pertains to. */
    pm_peer_data_id_t data_id; /**< The data the event pertains to. */
    union
    {
        struct
        {
            pm_store_token_t store_token;  /**< A token identifying the store operation this event pertains to. */
        } raw_stored_evt;
        struct
        {
            pm_store_token_t store_token;  /**< A token identifying the store operation this event pertains to. */
        } error_raw_store_evt;
    } params;
} pdb_evt_t;

/**@brief Event handler for events from the peer_data_storage module.
 *
 * @param[in]  p_event   The event that has happened.
 */
typedef void (*pdb_evt_handler_t)(pdb_evt_t const * p_event);


/**@brief Function for registering for events from the peer database.
 *
 * @note This function will initialize the module if it is not already initialized.
 *
 * @param[in]  evt_handler  Event handler to register.
 *
 * @retval NRF_SUCCESS              Registration successful.
 * @retval NRF_ERROR_NO_MEM         No more event handlers can be registered.
 * @retval NRF_ERROR_NULL           evt_handler was NULL.
 * @retval NRF_ERROR_INVALID_PARAM  Unexpected return code from @ref pm_buffer_init.
 * @retval NRF_ERROR_INVALID_STATE  FDS has not been initalized.
 */
ret_code_t pdb_register(pdb_evt_handler_t evt_handler);


/**@brief Function for allocating persistent bond storage for a peer.
 *
 * @return  The ID of the newly allocated storage.
 * @retval  PM_PEER_ID_INVALID  If no peer ID is available.
 */
pm_peer_id_t pdb_peer_allocate(void);


/**@brief Function for freeing a peer's persistent bond storage.
 *
 * @note This function will call @ref pdb_write_buf_release on the data for this peer.
 *
 * @param[in] peer_id  ID to be freed.
 *
 * @retval NRF_SUCCESS              Peer ID was released and clear operation was initiated successfully.
 * @retval NRF_ERROR_BUSY           Another peer_id clear was already requested or could not be started.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t pdb_peer_free(pm_peer_id_t peer_id);


/**@brief Function for retrieving pointers to read-only peer data.
 *
 * @note  Reading this pointer is not safe in the strictest sense. If a safe read is required:
 *          - Disable interrupts
 *          - Call this function. If the return code is @ref NRF_SUCCESS, the following read is safe.
 *          - Read memory.
 *          - Enable interrupts.
 * @note  This buffer does not need to be released. It is a pointer directly to flash.
 *
 * @param[in]  peer_id      ID of peer to retrieve data for.
 * @param[in]  data_id      Which piece of data to get.
 * @param[out] p_peer_data  Pointer to immutable peer data.
 * @param[out] p_token      Token that can be used to lock data in flash and check data validity.
 *
 * @retval NRF_SUCCESS              Data retrieved successfully.
 * @retval NRF_ERROR_INVALID_PARAM  Data ID or Peer ID was invalid or unallocated.
 * @retval NRF_ERROR_NULL           p_peer_data was NULL.
 * @retval NRF_ERROR_NOT_FOUND      This data was not found for this peer ID.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t pdb_read_buf_get(pm_peer_id_t           peer_id,
                            pm_peer_data_id_t      data_id,
                            pm_peer_data_flash_t * p_peer_data,
                            pm_store_token_t     * p_token);


/**@brief Function for retrieving pointers to a write buffer for peer data.
 *
 * @details This function will provide pointers to a buffer of the data. The data buffer will not be
 *          written to persistent storage until @ref pdb_write_buf_store is called. The buffer is
 *          released by calling either @ref pdb_write_buf_release, @ref pdb_write_buf_store, or
 *          @ref pdb_peer_free.
 *
 *          When the data_id refers to a variable length data type, the available size is written
 *          to the data, both the top-level, and any internal length fields.
 *
 * @note Calling this function on a peer_id/data_id pair that already has a buffer created will
 *       give the same buffer, not create a new one. If n_bufs was increased since last time, the
 *       buffer might be relocated to be able to provide additional room. In this case, the data
 *       will be copied. If n_bufs was increased since last time, this function might return @ref
 *       NRF_ERROR_BUSY. In that case, the buffer is automatically released.
 *
 * @param[in]  peer_id      ID of peer to get a write buffer for.
 * @param[in]  data_id      Which piece of data to get.
 * @param[in]  n_bufs       The number of contiguous buffers needed.
 * @param[out] p_peer_data  Pointers to mutable peer data.
 *
 * @retval NRF_SUCCESS              Data retrieved successfully.
 * @retval NRF_ERROR_INVALID_PARAM  Data ID or Peer ID was invalid or unallocated, or n_bufs was 0
 *                                  or more than the total available buffers.
 * @retval NRF_ERROR_NULL           p_peer_data was NULL.
 * @retval NRF_ERROR_BUSY           Not enough buffer(s) available.
 * @retval NRF_ERROR_INTERNAL       Unexpected internal error.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t pdb_write_buf_get(pm_peer_id_t      peer_id,
                             pm_peer_data_id_t data_id,
                             uint32_t          n_bufs,
                             pm_peer_data_t  * p_peer_data);


/**@brief Function for freeing a write buffer allocated with @ref pdb_write_buf_get.
 *
 * @note This function will not write peer data to persistent memory. Data in released buffer will
 *       be lost.
 *
 * @note This function will undo any previous call to @ref pdb_write_buf_store_prepare for this
 *       piece of data.
 *
 * @param[in]  peer_id  ID of peer to release buffer for.
 * @param[in]  data_id  Which piece of data to release buffer for.
 *
 * @retval NRF_SUCCESS              Successfully released buffer.
 * @retval NRF_ERROR_NOT_FOUND      No buffer was allocated for this peer ID/data ID pair.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 * @retval NRF_ERROR_INTERNAL       Unexpected internal error.
 */
ret_code_t pdb_write_buf_release(pm_peer_id_t peer_id, pm_peer_data_id_t data_id);


/**@brief Function for reserving space in persistent storage for data in a buffer.
 *
 * @note This function only works for data which has a write buffer allocated. If the write buffer
 *       is released, this prepare is undone.
 *
 * @note If space has already been reserved for this data, nothing is done.
 *
 * @param[in]  peer_id  The peer whose data to reserve space for.
 * @param[in]  data_id  The type of data to reserve space for.
 *
 * @retval NRF_SUCCESS              Successfully reserved space in persistent storage.
 * @retval NRF_ERROR_NO_MEM         Not enough room in persistent storage.
 * @retval NRF_ERROR_BUSY           Could not process request at this time. Reattempt later.
 * @retval NRF_ERROR_NOT_FOUND      No buffer has been allocated for this peer ID/data ID pair.
 * @retval NRF_ERROR_INVALID_PARAM  Data ID or Peer ID was invalid or unallocated.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t pdb_write_buf_store_prepare(pm_peer_id_t peer_id, pm_peer_data_id_t data_id);


/**@brief Function for writing data into persistent storage. Writing happens asynchronously.
 *
 * @note This will unlock the data after it has been written.
 *
 * @param[in]  peer_id      ID of peer to store data for.
 * @param[in]  data_id      Which piece of data to store.
 *
 * @retval NRF_SUCCESS              Data storing was successfully started.
 * @retval NRF_ERROR_NO_MEM         No space available in persistent storage. Please clear some
 *                                  space, the operation will be reattempted after the next compress
 *                                  procedure. This error will not happen if
 *                                  @ref pdb_write_buf_store_prepare is called beforehand.
 * @retval NRF_ERROR_INVALID_PARAM  Data ID was invalid.
 * @retval NRF_ERROR_NOT_FOUND      No buffer has been allocated for this peer ID/data ID pair.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 * @retval NRF_ERROR_INTERNAL       Unexpected internal error.
 */
ret_code_t pdb_write_buf_store(pm_peer_id_t      peer_id,
                               pm_peer_data_id_t data_id);


/**@brief Function for clearing data from persistent storage.
 *
 * @param[in]  peer_id  ID of peer to clear data for.
 * @param[in]  data_id  Which piece of data to clear.
 *
 * @retval NRF_SUCCESS              Data clear was successfully started.
 * @retval NRF_ERROR_INVALID_PARAM  Data ID was invalid.
 * @retval NRF_ERROR_NOT_FOUND      Nothing to clear for this data for this peer ID.
 * @retval NRF_ERROR_BUSY           Could not process request at this time. Reattempt later.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t pdb_clear(pm_peer_id_t peer_id, pm_peer_data_id_t data_id);


/**@brief Function for querying the number of valid peer IDs available. I.E the number of peers
 *        in persistent storage.
 *
 * @return  The number of valid peer IDs.
 */
uint32_t pdb_n_peers(void);


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
 * @retval  PM_PEER_ID_INVALID          if prev_peer_id was the last ordinary peer ID.
 */
pm_peer_id_t pdb_next_peer_id_get(pm_peer_id_t prev_peer_id);


/**@brief Function for updating currently stored peer data to a new version
 *
 * @details Updating happens asynchronously. 
 *          Expect a @ref PDS_EVT_STORED or @ref PDS_EVT_ERROR_STORE for the store token
 *          and a @ref PDS_EVT_ERROR_CLEAR or @ref PDS_EVT_ERROR_CLEAR for the old token 
 *      
 * @param[in]   peer_data           New data
 * @param[in]   old_token           Store token for the old data
 * @param[out]  p_store_token       Store token for the new data
 *
 * @retval NRF_SUCESS               The update was initiated successfully
 * @retval NRF_ERROR_NOT_FOUND      The old store token was invalid.
 * @retval NRF_ERROR_NULL           Data contained a NULL pointer.
 * @retval NRF_ERROR_NO_MEM         No space available in persistent storage.
 * @retval NRF_ERROR_BUSY           FDS or underlying modules are busy and can't take any 
 *                                  more requests
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t pdb_peer_data_update(pm_peer_data_const_t        peer_data,
                                pm_store_token_t            old_token,
                                pm_store_token_t          * p_store_token);


/**@brief Function for reading data directly from persistent storage to external memory.
 *
 * @param[in]    peer_id      ID of peer to read data for.
 * @param[in]    data_id      Which piece of data to read.
 * @param[inout] p_peer_data  Where to store the data. If the data to be read has variable length,
 *                            the appropriate length field needs to reflect the available buffer
 *                            space. On a successful read, the length field is updated to match the
 *                            length of the read data.
 *
 * @retval NRF_SUCCESS              Data successfully read.
 * @retval NRF_ERROR_INVALID_PARAM  Data ID or Peer ID was invalid or unallocated.
 * @retval NRF_ERROR_NULL           p_peer_data contained a NULL pointer.
 * @retval NRF_ERROR_NOT_FOUND      This data was not found for this peer ID.
 * @retval NRF_ERROR_DATA_SIZE      The provided buffer was not large enough.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t pdb_raw_read(pm_peer_id_t      peer_id,
                        pm_peer_data_id_t data_id,
                        pm_peer_data_t  * p_peer_data);


/**@brief Function for writing data directly to persistent storage from external memory.
 *
 * @param[in]  peer_id        ID of peer to write data for.
 * @param[in]  p_peer_data    Data to store.
 * @param[out] p_store_token  A token identifying this particular store operation. The token can be
 *                            used to identify events pertaining to this operation.
 *
 * @retval NRF_SUCCESS               Data successfully written.
 * @retval NRF_ERROR_INVALID_PARAM   Data ID or Peer ID was invalid or unallocated.
 * @retval NRF_ERROR_NULL            p_peer_data contained a NULL pointer.
 * @retval NRF_ERROR_NO_MEM          No space available in persistent storage.
 * @retval NRF_ERROR_INVALID_LENGTH  Data length above the maximum allowed.
 * @retval NRF_ERROR_INVALID_STATE   Module is not initialized.
 */
ret_code_t pdb_raw_store(pm_peer_id_t           peer_id,
                         pm_peer_data_const_t * p_peer_data,
                         pm_store_token_t     * p_store_token);


/** @} */

#endif /* PEER_DATABASE_H__ */


