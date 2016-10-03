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


#include "peer_database.h"

#include <string.h>
#include "peer_manager_types.h"
#include "peer_data_storage.h"
#include "pm_buffer.h"


#define MAX_REGISTRANTS    6                         /**< The number of user that can register with the module. */

#define MODULE_INITIALIZED (m_pdb.n_registrants > 0) /**< Expression which is true when the module is initialized. */

#define N_WRITE_BUFFERS        8                     /**< The number of write buffers available. */
#define N_WRITE_BUFFER_RECORDS (N_WRITE_BUFFERS)     /**< The number of write buffer records. */

/**@brief Macro for verifying that the module is initialized. It will cause the function to return
 *        @ref NRF_ERROR_INVALID_STATE if not.
 */
#define VERIFY_MODULE_INITIALIZED()     \
do                                      \
{                                       \
    if (!MODULE_INITIALIZED)            \
    {                                   \
        return NRF_ERROR_INVALID_STATE; \
    }                                   \
} while(0)


/**@brief Macro for verifying that the module is initialized. It will cause the function to return
 *        if not.
 */
#define VERIFY_MODULE_INITIALIZED_VOID()\
do                                      \
{                                       \
    if (!MODULE_INITIALIZED)            \
    {                                   \
        return;                         \
    }                                   \
} while(0)


/**@brief Macro for verifying that the module is initialized. It will cause the function to return
 *        if not.
 *
 * @param[in] param  The variable to check if is NULL.
 */
#define VERIFY_PARAM_NOT_NULL(param)    \
do                                      \
{                                       \
    if (param == NULL)                  \
    {                                   \
        return NRF_ERROR_NULL;          \
    }                                   \
} while(0)


typedef struct
{
    pm_peer_id_t        peer_id;
    pm_peer_data_id_t   data_id;
    uint8_t             buffer_block_id;
    uint8_t             store_busy       : 1;
    uint8_t             store_flash_full : 1;
    uint8_t             store_requested  : 1;
    uint32_t            n_bufs;
    pm_prepare_token_t  prepare_token;
    pm_store_token_t    store_token;
} pdb_buffer_record_t;

typedef struct
{
    pdb_evt_handler_t   evt_handlers[MAX_REGISTRANTS];
    uint8_t             n_registrants;
    pm_buffer_t         write_buffer;
    pdb_buffer_record_t write_buffer_records[N_WRITE_BUFFER_RECORDS];
    uint32_t            n_writes;
} pdb_t;

static pdb_t m_pdb = {.n_registrants = 0};


/**@brief Function for invalidating a record of a write buffer allocation.
 *
 * @param[in]  p_record  The record to invalidate.
 */
static void write_buffer_record_invalidate(pdb_buffer_record_t * p_record)
{
    p_record->peer_id          = PM_PEER_ID_INVALID;
    p_record->data_id          = PM_PEER_DATA_ID_INVALID;
    p_record->buffer_block_id  = BUFFER_INVALID_ID;
    p_record->store_busy       = false;
    p_record->store_flash_full = false;
    p_record->store_requested  = false;
    p_record->n_bufs           = 0;
    p_record->prepare_token    = PDS_PREPARE_TOKEN_INVALID;
    p_record->store_token      = PDS_STORE_TOKEN_INVALID;
}


/**@brief Function for finding a record of a write buffer allocation.
 *
 * @param[in]  peer_id  The peer ID in the record.
 * @param[in]  data_id  The data ID in the record.
 *
 * @return  A pointer to the matching record, or NULL if none was found.
 */
static pdb_buffer_record_t * write_buffer_record_find(pm_peer_id_t      peer_id,
                                                      pm_peer_data_id_t data_id)
{
    for (int i = 0; i < N_WRITE_BUFFER_RECORDS; i++)
    {
        if   ((m_pdb.write_buffer_records[i].peer_id == peer_id)
           && (m_pdb.write_buffer_records[i].data_id == data_id))
        {
            return &m_pdb.write_buffer_records[i];
        }
    }
    return NULL;
}


/**@brief Function for finding an available record for write buffer allocation.
 *
 * @return  A pointer to the available record, or NULL if none was found.
 */
static pdb_buffer_record_t * write_buffer_record_find_unused(void)
{
    return write_buffer_record_find(PM_PEER_ID_INVALID, PM_PEER_DATA_ID_INVALID);
}


/**@brief Function for gracefully deactivating a write buffer record.
 *
 * @details This function will first release any buffers, then invalidate the record.
 *
 * @param[inout] p_write_buffer_record  The record to release.
 *
 * @return  A pointer to the matching record, or NULL if none was found.
 */
static void write_buffer_record_release(pdb_buffer_record_t * p_write_buffer_record)
{
    for (uint32_t i = 0; i < p_write_buffer_record->n_bufs; i++)
    {
        pm_buffer_release(&m_pdb.write_buffer, p_write_buffer_record->buffer_block_id + i);
    }

    write_buffer_record_invalidate(p_write_buffer_record);
}


static void write_buffer_record_get(pdb_buffer_record_t ** pp_write_buffer_record, pm_peer_id_t peer_id, pm_peer_data_id_t data_id)
{
    if (pp_write_buffer_record == NULL)
    {
        return;
    }
    *pp_write_buffer_record = write_buffer_record_find_unused();
    if (*pp_write_buffer_record == NULL)
    {
        // This also means the buffer is full.
        return;
    }
    (*pp_write_buffer_record)->peer_id = peer_id;
    (*pp_write_buffer_record)->data_id = data_id;
}


/**@brief Function for dispatching outbound events to all registered event handlers.
 *
 * @param[in]  p_event  The event to dispatch.
 */
static void pdb_evt_send(pdb_evt_t * p_event)
{
    for (int i = 0; i < m_pdb.n_registrants; i++)
    {
        m_pdb.evt_handlers[i](p_event);
    }
}


/**@brief Function for resetting the internal state of the Peer Database module.
 *
 * @param[out] p_event  The event to dispatch.
 */
static void internal_state_reset(pdb_t * pdb)
{
    memset(pdb, 0, sizeof(pdb_t));
    for (int i = 0; i < N_WRITE_BUFFER_RECORDS; i++)
    {
        write_buffer_record_invalidate(&pdb->write_buffer_records[i]);
    }
}


/**@brief Function for handling events from the Peer Data Storage module.
 *
 * @param[in]  p_event  The event to handle.
 */
static void pds_evt_handler(pds_evt_t const * p_event)
{
    ret_code_t            err_code;
    pdb_buffer_record_t * p_write_buffer_record;
    bool                  retry_flash_full = false;
    pdb_evt_t             event =
    {
        .peer_id = p_event->peer_id,
        .data_id = p_event->data_id,
    };

    p_write_buffer_record = write_buffer_record_find(p_event->peer_id, p_event->data_id);

    switch (p_event->evt_id)
    {
        case PDS_EVT_STORED:
            if (   (p_write_buffer_record != NULL)
                //&& (p_write_buffer_record->store_token == p_event->store_token)
                && (p_write_buffer_record->store_requested))
            {
                write_buffer_record_release(p_write_buffer_record);
                event.evt_id = PDB_EVT_WRITE_BUF_STORED;
                pdb_evt_send(&event);
            }
            else
            {
                event.evt_id = PDB_EVT_RAW_STORED;
                pdb_evt_send(&event);
            }
            break;
        case PDS_EVT_ERROR_STORE:
            if (   (p_write_buffer_record != NULL)
                && (p_write_buffer_record->store_token == p_event->store_token)
                && (p_write_buffer_record->store_requested))
            {
                // Retry if internal buffer.
                m_pdb.n_writes++;
                p_write_buffer_record->store_requested = false;
                p_write_buffer_record->store_busy      = true;
            }
            else
            {
                event.evt_id = PDB_EVT_RAW_STORE_FAILED;
                pdb_evt_send(&event);
            }
            break;
        case PDS_EVT_CLEARED:
            event.evt_id = PDB_EVT_CLEARED;
            pdb_evt_send(&event);
            break;
        case PDS_EVT_ERROR_CLEAR:
            event.evt_id = PDB_EVT_CLEAR_FAILED;
            pdb_evt_send(&event);
            break;
        case PDS_EVT_COMPRESSED:
            retry_flash_full = true;
            event.evt_id = PDB_EVT_COMPRESSED;
            pdb_evt_send(&event);
            break;
        default:
            break;
    }

    if (m_pdb.n_writes > 0)
    {
        for (int i = 0; i < N_WRITE_BUFFER_RECORDS; i++)
        {
            if  ((m_pdb.write_buffer_records[i].store_busy)
              || (m_pdb.write_buffer_records[i].store_flash_full && retry_flash_full))
            {
                err_code = pdb_write_buf_store(m_pdb.write_buffer_records[i].peer_id,
                                               m_pdb.write_buffer_records[i].data_id);
                if (err_code != NRF_SUCCESS)
                {
                    event.peer_id = m_pdb.write_buffer_records[i].peer_id;
                    event.data_id = m_pdb.write_buffer_records[i].data_id;
                    if (err_code == NRF_ERROR_NO_MEM)
                    {
                        event.evt_id = PDB_EVT_ERROR_NO_MEM;
                    }
                    else
                    {
                        event.evt_id = PDB_EVT_ERROR_UNEXPECTED;
                    }

                    pdb_evt_send(&event);
                    break;
                }
            }
        }
    }
}


ret_code_t pdb_register(pdb_evt_handler_t evt_handler)
{
    if (m_pdb.n_registrants >= MAX_REGISTRANTS)
    {
        return NRF_ERROR_NO_MEM;
    }

    VERIFY_PARAM_NOT_NULL(evt_handler);

    if (!MODULE_INITIALIZED)
    {
        ret_code_t err_code;

        internal_state_reset(&m_pdb);
        err_code = pds_register(pds_evt_handler);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
        PM_BUFFER_INIT(&m_pdb.write_buffer, N_WRITE_BUFFERS, PDB_WRITE_BUF_SIZE, err_code);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    m_pdb.evt_handlers[m_pdb.n_registrants] = evt_handler;
    m_pdb.n_registrants += 1;

    return NRF_SUCCESS;
}


pm_peer_id_t pdb_peer_allocate(void)
{
    if (!MODULE_INITIALIZED)
    {
        return PM_PEER_ID_INVALID;
    }

    return pds_peer_id_allocate();
}


ret_code_t pdb_peer_free(pm_peer_id_t peer_id)
{
    VERIFY_MODULE_INITIALIZED();

    return pds_peer_id_free(peer_id);
}


ret_code_t pdb_read_buf_get(pm_peer_id_t           peer_id,
                            pm_peer_data_id_t      data_id,
                            pm_peer_data_flash_t * p_peer_data,
                            pm_store_token_t     * p_token)
{
    VERIFY_MODULE_INITIALIZED();

    return pds_peer_data_read_ptr_get(peer_id, data_id, p_peer_data, p_token);
}


static void peer_data_point_to_buffer(pm_peer_data_t * p_peer_data, pm_peer_data_id_t data_id, uint8_t * p_buffer_memory, uint16_t n_bufs)
{
    uint16_t n_bytes = n_bufs * PDB_WRITE_BUF_SIZE;
    p_peer_data->data_type    = data_id;

    switch(p_peer_data->data_type)
    {
        case PM_PEER_DATA_ID_BONDING:
            p_peer_data->data.p_bonding_data = (pm_peer_data_bonding_t *)p_buffer_memory;
            p_peer_data->length_words = PM_BONDING_DATA_N_WORDS();
            break;
        case PM_PEER_DATA_ID_SERVICE_CHANGED_PENDING:
            p_peer_data->data.p_service_changed_pending = (bool *)p_buffer_memory;
            p_peer_data->length_words = PM_SC_STATE_N_WORDS();
            break;
        case PM_PEER_DATA_ID_GATT_LOCAL:
            p_peer_data->data.p_local_gatt_db = (pm_peer_data_local_gatt_db_t *)p_buffer_memory;
            p_peer_data->length_words = PM_LOCAL_DB_N_WORDS(n_bytes);
            break;
        case PM_PEER_DATA_ID_GATT_REMOTE:
            p_peer_data->data.p_remote_gatt_db = (pm_peer_data_remote_gatt_db_t *)p_buffer_memory;
            p_peer_data->length_words = PM_REMOTE_DB_N_WORDS(n_bytes / sizeof(ble_gatt_db_srv_t));
            break;
        case PM_PEER_DATA_ID_APPLICATION:
            p_peer_data->data.p_application_data = p_buffer_memory;
            p_peer_data->length_words = PM_N_WORDS(n_bytes);
            break;
        default:
            p_peer_data->length_words = 0;
            break;
    }
}


static void peer_data_const_point_to_buffer(pm_peer_data_const_t * p_peer_data, pm_peer_data_id_t data_id,  uint8_t * p_buffer_memory, uint32_t n_bufs)
{
    peer_data_point_to_buffer((pm_peer_data_t*)p_peer_data, data_id, p_buffer_memory, n_bufs);
}


ret_code_t pdb_write_buf_get(pm_peer_id_t       peer_id,
                             pm_peer_data_id_t  data_id,
                             uint32_t           n_bufs,
                             pm_peer_data_t   * p_peer_data)
{
    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_NULL(p_peer_data);
    if (   !PM_PEER_DATA_ID_IS_VALID(data_id)
        || (n_bufs == 0)
        || (n_bufs > N_WRITE_BUFFERS)
        || !pds_peer_id_is_allocated(peer_id))
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    pdb_buffer_record_t * write_buffer_record;
    uint8_t             * p_buffer_memory;

    write_buffer_record = write_buffer_record_find(peer_id, data_id);

    if ((write_buffer_record != NULL) && (write_buffer_record->n_bufs < n_bufs))
    {
        // @TODO: Copy?
        // Existing buffer is too small.
        for (uint8_t i = 0; i < write_buffer_record->n_bufs; i++)
        {
            pm_buffer_release(&m_pdb.write_buffer, write_buffer_record->buffer_block_id + i);
        }
        write_buffer_record_invalidate(write_buffer_record);
        write_buffer_record = NULL;
    }
    else if ((write_buffer_record != NULL) && write_buffer_record->n_bufs > n_bufs)
    {
        // Release excess blocks.
        for (uint8_t i = n_bufs; i < write_buffer_record->n_bufs; i++)
        {
            pm_buffer_release(&m_pdb.write_buffer, write_buffer_record->buffer_block_id + i);
        }
    }

    if (write_buffer_record == NULL)
    {
        write_buffer_record_get(&write_buffer_record, peer_id, data_id);
        if (write_buffer_record == NULL)
        {
            return NRF_ERROR_BUSY;
        }
    }

    if (write_buffer_record->buffer_block_id == BUFFER_INVALID_ID)
    {
        write_buffer_record->buffer_block_id = pm_buffer_block_acquire(&m_pdb.write_buffer, n_bufs);

        if (write_buffer_record->buffer_block_id == BUFFER_INVALID_ID)
        {
            write_buffer_record_invalidate(write_buffer_record);
            return NRF_ERROR_BUSY;
        }
    }

    write_buffer_record->n_bufs = n_bufs;

    p_buffer_memory = pm_buffer_ptr_get(&m_pdb.write_buffer, write_buffer_record->buffer_block_id);

    if (p_buffer_memory == NULL)
    {
        return NRF_ERROR_INTERNAL;
    }

    peer_data_point_to_buffer(p_peer_data, data_id, p_buffer_memory, n_bufs);
    switch(data_id)
    {
        case PM_PEER_DATA_ID_BONDING:
            /* No action needed. */
            break;
        case PM_PEER_DATA_ID_SERVICE_CHANGED_PENDING:
            /* No action needed. */
            break;
        case PM_PEER_DATA_ID_GATT_LOCAL:
        {
            uint32_t size_offset = sizeof(pm_peer_data_local_gatt_db_t);
            p_peer_data->data.p_local_gatt_db->p_data = &p_buffer_memory[size_offset];
            p_peer_data->data.p_local_gatt_db->len    = (PDB_WRITE_BUF_SIZE*n_bufs)-size_offset;
        }
            break;
        case PM_PEER_DATA_ID_GATT_REMOTE:
        {
            uint32_t size_offset = sizeof(pm_peer_data_remote_gatt_db_t);
            p_peer_data->data.p_remote_gatt_db->p_data = (ble_gatt_db_srv_t*)&(p_buffer_memory[size_offset]);
            p_peer_data->data.p_remote_gatt_db->service_count
                            = ((PDB_WRITE_BUF_SIZE*n_bufs)-size_offset)/sizeof(ble_gatt_db_srv_t);
        }
            break;
        case PM_PEER_DATA_ID_APPLICATION:
        {
            p_peer_data->data.p_application_data = p_buffer_memory;
        }
            break;
        default:
            // Invalid data_id. This should have been picked up earlier.
            return NRF_ERROR_INTERNAL;
    }

    return NRF_SUCCESS;
}


ret_code_t pdb_write_buf_release(pm_peer_id_t peer_id, pm_peer_data_id_t data_id)
{
    VERIFY_MODULE_INITIALIZED();

    ret_code_t            err_code = NRF_SUCCESS;
    pdb_buffer_record_t * p_write_buffer_record;
    p_write_buffer_record = write_buffer_record_find(peer_id, data_id);

    if (p_write_buffer_record == NULL)
    {
        return NRF_ERROR_NOT_FOUND;
    }

    if (p_write_buffer_record->prepare_token != PDS_PREPARE_TOKEN_INVALID)
    {
        err_code = pds_peer_data_write_prepare_cancel(p_write_buffer_record->prepare_token);
        if (err_code != NRF_SUCCESS)
        {
            err_code = NRF_ERROR_INTERNAL;
        }
    }

    write_buffer_record_release(p_write_buffer_record);

    return err_code;
}


ret_code_t pdb_write_buf_store_prepare(pm_peer_id_t peer_id, pm_peer_data_id_t data_id)
{
    VERIFY_MODULE_INITIALIZED();

    ret_code_t            err_code = NRF_SUCCESS;
    pdb_buffer_record_t * p_write_buffer_record;
    p_write_buffer_record = write_buffer_record_find(peer_id, data_id);

    if (p_write_buffer_record == NULL)
    {
        return NRF_ERROR_NOT_FOUND;
    }

    if (p_write_buffer_record->prepare_token == PDS_PREPARE_TOKEN_INVALID)
    {
        uint8_t * p_buffer_memory = pm_buffer_ptr_get(&m_pdb.write_buffer, p_write_buffer_record->buffer_block_id);
        pm_peer_data_const_t peer_data = {.data_type = data_id};

        if (p_buffer_memory == NULL)
        {
            return NRF_ERROR_INTERNAL;
        }

        peer_data_const_point_to_buffer(&peer_data, data_id, p_buffer_memory, p_write_buffer_record->n_bufs);

        err_code = pds_peer_data_write_prepare(&peer_data, &p_write_buffer_record->prepare_token);
        if (err_code == NRF_ERROR_INVALID_LENGTH)
        {
            return NRF_ERROR_INTERNAL;
        }
    }

    return err_code;
}


static ret_code_t write_or_update(pm_peer_id_t           peer_id,
                                  pm_peer_data_id_t      data_id,
                                  pm_peer_data_const_t * p_peer_data,
                                  pm_store_token_t     * p_store_token,
                                  pm_prepare_token_t     prepare_token)
{
    pm_peer_data_flash_t old_peer_data;
    pm_store_token_t     old_store_token;
    ret_code_t err_code = pds_peer_data_read_ptr_get(peer_id, data_id, &old_peer_data, &old_store_token);

    if (err_code == NRF_SUCCESS)
    {
        pds_peer_data_write_prepare_cancel(prepare_token);
        err_code = pds_peer_data_update(peer_id, p_peer_data, old_store_token, p_store_token);
    }
    else if (err_code == NRF_ERROR_NOT_FOUND)
    {
        if (prepare_token == PDS_PREPARE_TOKEN_INVALID)
        {
            err_code = pds_peer_data_write(peer_id, p_peer_data, p_store_token);
        }
        else
        {
            err_code = pds_peer_data_write_prepared(peer_id, p_peer_data, prepare_token, p_store_token);
        }
    }
    return err_code;
}


ret_code_t pdb_write_buf_store(pm_peer_id_t      peer_id,
                               pm_peer_data_id_t data_id)
{
    VERIFY_MODULE_INITIALIZED();

    ret_code_t            err_code = NRF_SUCCESS;
    pdb_buffer_record_t * p_write_buffer_record;
    uint8_t             * p_buffer_memory;
    pm_peer_data_const_t  peer_data = {.data_type = data_id};


    p_write_buffer_record = write_buffer_record_find(peer_id, data_id);

    if (p_write_buffer_record == NULL)
    {
        return NRF_ERROR_NOT_FOUND;
    }

    if (p_write_buffer_record->store_requested)
    {
        return NRF_SUCCESS;
    }

    p_buffer_memory = pm_buffer_ptr_get(&m_pdb.write_buffer, p_write_buffer_record->buffer_block_id);

    if (p_buffer_memory == NULL)
    {
        return NRF_ERROR_INTERNAL;
    }

    peer_data_const_point_to_buffer(&peer_data, data_id, p_buffer_memory, p_write_buffer_record->n_bufs);

    switch (data_id)
    {
        case PM_PEER_DATA_ID_BONDING:
            peer_data.length_words = PM_BONDING_DATA_N_WORDS();
            break;
        case PM_PEER_DATA_ID_SERVICE_CHANGED_PENDING:
            peer_data.length_words = PM_SC_STATE_N_WORDS();
            break;
        case PM_PEER_DATA_ID_GATT_LOCAL:
            peer_data.length_words = PM_LOCAL_DB_N_WORDS(peer_data.data.p_local_gatt_db->len);
            break;
        case PM_PEER_DATA_ID_GATT_REMOTE:
            peer_data.length_words = PM_REMOTE_DB_N_WORDS(peer_data.data.p_remote_gatt_db->service_count);
            break;
        case PM_PEER_DATA_ID_APPLICATION:
            peer_data.length_words = PM_N_WORDS(p_write_buffer_record->n_bufs * PDB_WRITE_BUF_SIZE);
            break;
        default:
            return NRF_ERROR_INVALID_PARAM;
    }

    err_code = write_or_update(peer_id, data_id, &peer_data, &p_write_buffer_record->store_token, p_write_buffer_record->prepare_token);

    if (p_write_buffer_record->store_busy && p_write_buffer_record->store_flash_full)
    {
        m_pdb.n_writes--;
    }

    if (err_code == NRF_SUCCESS)
    {
        p_write_buffer_record->store_requested  = true;
        p_write_buffer_record->store_busy       = false;
        p_write_buffer_record->store_flash_full = false;
    }
    else
    {
        if (err_code == NRF_ERROR_BUSY)
        {
            m_pdb.n_writes++;
            p_write_buffer_record->store_busy       = true;
            p_write_buffer_record->store_flash_full = false;
            err_code = NRF_SUCCESS;
        }
        else if (err_code == NRF_ERROR_NO_MEM)
        {
            m_pdb.n_writes++;
            p_write_buffer_record->store_busy       = false;
            p_write_buffer_record->store_flash_full = true;
        }
        else if ((err_code != NRF_ERROR_NO_MEM) && (err_code != NRF_ERROR_INVALID_PARAM))
        {
            err_code = NRF_ERROR_INTERNAL;
        }
    }

    return err_code;
}


ret_code_t pdb_clear(pm_peer_id_t peer_id, pm_peer_data_id_t data_id)
{
    VERIFY_MODULE_INITIALIZED();

    return pds_peer_data_clear(peer_id, data_id);
}


uint32_t pdb_n_peers(void)
{
    if (!MODULE_INITIALIZED)
    {
        return 0;
    }

    return pds_n_peers();
}


pm_peer_id_t pdb_next_peer_id_get(pm_peer_id_t prev_peer_id)
{
    if (!MODULE_INITIALIZED)
    {
        return PM_PEER_ID_INVALID;
    }

    return pds_next_peer_id_get(prev_peer_id);
}


ret_code_t pdb_raw_read(pm_peer_id_t      peer_id,
                        pm_peer_data_id_t data_id,
                        pm_peer_data_t  * p_peer_data)
{
    VERIFY_MODULE_INITIALIZED();
    return pds_peer_data_read(peer_id, data_id, p_peer_data, &p_peer_data->length_words);
}


ret_code_t pdb_raw_store(pm_peer_id_t           peer_id,
                         pm_peer_data_const_t * p_peer_data,
                         pm_store_token_t     * p_store_token)
{
    VERIFY_MODULE_INITIALIZED();
    
    return write_or_update(peer_id, p_peer_data->data_type, p_peer_data, p_store_token, PDS_PREPARE_TOKEN_INVALID);
}

