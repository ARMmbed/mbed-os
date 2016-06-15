/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */


#include "peer_data_storage.h"

#include <stdint.h>
#include <string.h>
#include "sdk_errors.h"
#include "peer_manager_types.h"
#include "peer_manager_internal.h"
#include "peer_id.h"
#include "peer_data.h"
#include "fds.h"
#include "sdk_common.h"


// The number of user that can register with the module.
#define MAX_REGISTRANTS    6

// Macro for verifying that param is not zero.
#define VERIFY_PARAM_NOT_ZERO(param)        \
do                                          \
{                                           \
    if (param == 0)                         \
    {                                       \
        return NRF_ERROR_NULL;              \
    }                                       \
} while(0)


// Macro for verifying that the peer id is within a valid range.
#define VERIFY_PEER_ID_IN_RANGE(id)         \
do                                          \
{                                           \
    if (id >= PM_PEER_ID_N_AVAILABLE_IDS)   \
    {                                       \
        return NRF_ERROR_INVALID_PARAM;     \
    }                                       \
} while (0)


// Macro for verifying that the peer data id is withing a valid range.
#define VERIFY_PEER_DATA_ID_IN_RANGE(id)    \
do                                          \
{                                           \
    if (!PM_PEER_DATA_ID_IS_VALID(id))      \
    {                                       \
        return NRF_ERROR_INVALID_PARAM;     \
    }                                       \
} while (0)


// Macro for initializing the peer ID tracking system if it is not already initialized.
#define PEER_IDS_INITIALIZE()               \
do                                          \
{                                           \
    if (!m_pds.peer_ids_initialized)        \
    {                                       \
        peer_ids_init();                    \
    }                                       \
} while (0)


typedef struct
{
    bool                peer_ids_initialized;
    pds_evt_handler_t   evt_handlers[MAX_REGISTRANTS];
    uint8_t             n_registrants;
    bool                clearing;
    bool                clear_queued;
} pds_t;


static pds_t m_pds = { .n_registrants = 0 };


#define MODULE_INITIALIZED (m_pds.n_registrants > 0) /**< Expression which is true when the module is initialized. */
#include "sdk_macros.h"

static void internal_state_reset(pds_t * p_pds)
{
    memset(p_pds, 0, sizeof(pds_t));
}


// Function for dispatching outbound events to all registered event handlers.
static void pds_evt_send(pds_evt_t * p_event)
{
    for (uint32_t i = 0; i < m_pds.n_registrants; i++)
    {
        m_pds.evt_handlers[i](p_event);
    }
}


// Function to convert peer IDs to file IDs.
static uint16_t peer_id_to_file_id(pm_peer_id_t peer_id)
{
    return (uint16_t)(peer_id + PEER_ID_TO_FILE_ID);
}


// Function to convert peer data id to type id.
static pm_peer_id_t file_id_to_peer_id(uint16_t file_id)
{
    return (pm_peer_id_t)(file_id + FILE_ID_TO_PEER_ID);
}


// Function to convert peer data IDs to record keys.
static uint16_t peer_data_id_to_record_key(pm_peer_data_id_t peer_data_id)
{
    return (uint16_t)peer_data_id + (uint16_t)PEER_DATA_ID_TO_RECORD_KEY;
}


// Function to convert record keys to peer data IDs.
static pm_peer_data_id_t record_key_to_peer_data_id(uint16_t record_key)
{
    return (pm_peer_data_id_t)(record_key + RECORD_KEY_TO_PEER_DATA_ID);
}


// Function for clearing all peer data of one peer.
// These operations will be sent to FDS one at a time.
static void peer_data_clear()
{
    ret_code_t        retval;
    uint16_t          file_id;
    fds_record_desc_t desc;
    fds_find_token_t  token   = {0};
    pm_peer_id_t      peer_id = peer_id_get_next_deleted(PM_PEER_ID_INVALID);

    while (    (peer_id != PM_PEER_ID_INVALID)
            && (fds_record_find_in_file(peer_id_to_file_id(peer_id), &desc, &token)
            == FDS_ERR_NOT_FOUND))
    {
        peer_id_free(peer_id);
        peer_id = peer_id_get_next_deleted(peer_id);
    }

    if (!m_pds.clearing && (peer_id != PM_PEER_ID_INVALID))
    {
        file_id = peer_id_to_file_id(peer_id);
        retval  = fds_file_delete(file_id);

        if (retval == FDS_SUCCESS)
        {
            m_pds.clearing = true;
        }
        else if (retval == FDS_ERR_NO_SPACE_IN_QUEUES)
        {
            m_pds.clear_queued = true;
        }
        else
        {
            pds_evt_t pds_evt;

            pds_evt.evt_id      = PDS_EVT_ERROR_UNEXPECTED;
            pds_evt.peer_id     = peer_id;
            pds_evt.data_id     = PM_PEER_DATA_ID_INVALID;
            pds_evt.store_token = PM_STORE_TOKEN_INVALID;
            pds_evt.result      = retval;

            pds_evt_send(&pds_evt);
        }
    }
}


static ret_code_t find_fds_item(pm_peer_id_t              peer_id,
                                pm_peer_data_id_t         data_id,
                                fds_record_desc_t * const p_desc)
{
    fds_find_token_t find_tok = {0};

    VERIFY_PEER_ID_IN_RANGE(peer_id);
    VERIFY_PEER_DATA_ID_IN_RANGE(data_id);
    // pp_record verified by caller.

    uint16_t file_id    = peer_id_to_file_id(peer_id);
    uint16_t record_key = peer_data_id_to_record_key(data_id);

    return fds_record_find(file_id, record_key, p_desc, &find_tok);
}


static void peer_ids_init()
{
    fds_record_desc_t  record_desc;
    fds_flash_record_t record;
    fds_find_token_t   find_tok = {0};

    uint16_t const record_key = peer_data_id_to_record_key(PM_PEER_DATA_ID_BONDING);

    if (!m_pds.peer_ids_initialized)
    {
        while(fds_record_find_by_key(record_key, &record_desc, &find_tok) == FDS_SUCCESS)
        {
            pm_peer_id_t peer_id;

            // It is safe to ignore the return value since we just obtained
            // this descriptor and also 'record' is different from NULL.
            (void)fds_record_open(&record_desc, &record);
            peer_id = file_id_to_peer_id(record.p_header->ic.file_id);
            (void)fds_record_close(&record_desc);

            (void)peer_id_allocate(peer_id);
        }

        m_pds.peer_ids_initialized = true;
    }
}


static void fds_evt_handler(fds_evt_t const * const p_fds_evt)
{
    pds_evt_t pds_evt;
    bool      send_event = true;

    pds_evt.result = (p_fds_evt->result == FDS_SUCCESS);

    switch(p_fds_evt->id)
    {
        case FDS_EVT_WRITE:
            pds_evt.evt_id = (p_fds_evt->result == FDS_SUCCESS) ? PDS_EVT_STORED :
                                                                  PDS_EVT_ERROR_STORE;

            pds_evt.peer_id     = file_id_to_peer_id(p_fds_evt->write.file_id);
            pds_evt.data_id     = record_key_to_peer_data_id(p_fds_evt->write.record_key);
            pds_evt.result      = p_fds_evt->result;
            pds_evt.store_token = p_fds_evt->write.record_id;
            break;

        case FDS_EVT_UPDATE:
            pds_evt.evt_id = (p_fds_evt->result == FDS_SUCCESS) ? PDS_EVT_UPDATED :
                                                                  PDS_EVT_ERROR_UPDATE;

            pds_evt.peer_id     = file_id_to_peer_id(p_fds_evt->write.file_id);
            pds_evt.data_id     = record_key_to_peer_data_id(p_fds_evt->write.record_key);
            pds_evt.result      = p_fds_evt->result;
            pds_evt.store_token = p_fds_evt->write.record_id;
            break;

        case FDS_EVT_DEL_RECORD:
            pds_evt.evt_id = (p_fds_evt->result == FDS_SUCCESS) ? PDS_EVT_CLEARED :
                                                                  PDS_EVT_ERROR_CLEAR;

            pds_evt.peer_id     = file_id_to_peer_id(p_fds_evt->del.file_id);
            pds_evt.data_id     = record_key_to_peer_data_id(p_fds_evt->del.record_key);
            pds_evt.store_token = p_fds_evt->del.record_id;
            break;

        case FDS_EVT_DEL_FILE:
            {
                if ((p_fds_evt->del.record_key == FDS_RECORD_KEY_DIRTY) &&
                    (p_fds_evt->del.file_id    != FDS_FILE_ID_INVALID))
                {
                    pds_evt.peer_id = file_id_to_peer_id(p_fds_evt->del.file_id);
                    pds_evt.data_id = record_key_to_peer_data_id(p_fds_evt->del.record_key);

                    pds_evt.data_id = PM_PEER_DATA_ID_INVALID;
                    if (p_fds_evt->result == FDS_SUCCESS)
                    {
                        pds_evt.evt_id = PDS_EVT_PEER_ID_CLEAR;
                        peer_id_free(pds_evt.peer_id);
                    }
                    else
                    {
                        pds_evt.evt_id = PDS_EVT_ERROR_PEER_ID_CLEAR;
                    }
                    m_pds.clearing = false;
                    m_pds.clear_queued = false;

                    peer_data_clear();
                }
            }
            break;

        case FDS_EVT_GC:
            pds_evt.evt_id = PDS_EVT_COMPRESSED;
            break;

        default:
            send_event = false;
            break;
    }

    if (send_event)
    {
        pds_evt_send(&pds_evt);
    }

    if (m_pds.clear_queued)
    {
        m_pds.clear_queued = false;
        peer_data_clear();
    }
}


ret_code_t pds_register(pds_evt_handler_t evt_handler)
{
    if (m_pds.n_registrants >= MAX_REGISTRANTS)
    {
        return NRF_ERROR_NO_MEM;
    }

    VERIFY_PARAM_NOT_NULL(evt_handler);

    if (!MODULE_INITIALIZED)
    {
        ret_code_t retval;
        internal_state_reset(&m_pds);
        peer_id_init();

        retval = fds_register(fds_evt_handler);
        if (retval != FDS_SUCCESS)
        {
            return NRF_ERROR_NO_MEM;
        }

        retval = fds_init();
        if (retval != FDS_SUCCESS)
        {
            return NRF_ERROR_INTERNAL;
        }
    }

    m_pds.evt_handlers[m_pds.n_registrants] = evt_handler;
    m_pds.n_registrants += 1;

    return NRF_SUCCESS;
}


ret_code_t pds_peer_data_read_ptr_get(pm_peer_id_t            peer_id,
                                      pm_peer_data_id_t       data_id,
                                      pm_peer_data_flash_t  * p_data,
                                      pm_store_token_t      * p_token)
{
    ret_code_t retval;

    fds_flash_record_t record;
    fds_record_desc_t  record_desc;

    VERIFY_MODULE_INITIALIZED();
    VERIFY_PEER_ID_IN_RANGE(peer_id);
    VERIFY_PEER_DATA_ID_IN_RANGE(data_id);
    VERIFY_PARAM_NOT_NULL(p_data);

    retval = find_fds_item(peer_id, data_id, &record_desc);
    if (retval != FDS_SUCCESS)
    {
        return NRF_ERROR_NOT_FOUND;
    }

    // Shouldn't fail, unless the record was deleted.
    (void)fds_record_open(&record_desc, &record);

    if (p_data != NULL)
    {
        p_data->data_id      = data_id;
        p_data->length_words = record.p_header->tl.length_words;
        p_data->p_all_data   = record.p_data;
    }

    if (p_token != NULL)
    {
        *p_token = (uint32_t)record.p_header->record_id;
    }

    // Shouldn't fail, unless the record was already closed.
    (void)fds_record_close(&record_desc);

    return NRF_SUCCESS;
}


ret_code_t pds_peer_data_lock(pm_store_token_t store_token)
{
    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_ZERO(store_token);

    // TODO: Not implemented in fds yet.

    return NRF_SUCCESS;
}


ret_code_t pds_peer_data_verify(pm_store_token_t store_token)
{
    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_ZERO(store_token);

    // TODO: Not implemented in fds yet.

    return NRF_SUCCESS;
}


ret_code_t pds_peer_data_read(pm_peer_id_t        peer_id,
                              pm_peer_data_id_t   data_id,
                              pm_peer_data_t    * p_data,
                              uint16_t          * p_len_words)
{
    ret_code_t           retval;
    pm_peer_data_flash_t peer_data_flash;

    VERIFY_PEER_ID_IN_RANGE(peer_id);
    VERIFY_PEER_DATA_ID_IN_RANGE(data_id);
    VERIFY_PARAM_NOT_NULL(p_len_words);
    VERIFY_PARAM_NOT_NULL(p_data);

    retval = pds_peer_data_read_ptr_get(peer_id, data_id, &peer_data_flash, NULL);
    if (retval != NRF_SUCCESS)
    {
        return retval;
    }

    if ((*p_len_words) == 0)
    {
        (*p_len_words) = peer_data_flash.length_words;
        return NRF_SUCCESS;
    }
    else if ((*p_len_words) < peer_data_flash.length_words)
    {
        return NRF_ERROR_NO_MEM;
    }

    VERIFY_PARAM_NOT_NULL(p_data->p_all_data);

    retval = peer_data_deserialize(&peer_data_flash, p_data);

    return retval;
}


ret_code_t pds_peer_data_write_prepare(pm_peer_data_const_t const * p_peer_data,
                                       pm_prepare_token_t         * p_prepare_token)
{
    ret_code_t retval;

    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_NULL(p_peer_data);
    VERIFY_PEER_DATA_ID_IN_RANGE(p_peer_data->data_id);
    //VERIFY_PARAM_NOT_NULL(p_prepare_token);  redundant, see fds_reserve().

    retval = fds_reserve((fds_reserve_token_t*)p_prepare_token, p_peer_data->length_words);

    switch (retval)
    {
        case FDS_SUCCESS:
            return NRF_SUCCESS;

        case FDS_ERR_NULL_ARG:
            return NRF_ERROR_NULL;

        case FDS_ERR_RECORD_TOO_LARGE:
            return NRF_ERROR_INVALID_LENGTH;

        case FDS_ERR_NO_SPACE_IN_FLASH:
            return NRF_ERROR_NO_MEM;

        default:
            return NRF_ERROR_INTERNAL;
    }
}


ret_code_t pds_peer_data_write_prepare_cancel(pm_prepare_token_t prepare_token)
{
    ret_code_t retval;

    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_ZERO(prepare_token);

    retval = fds_reserve_cancel((fds_reserve_token_t*)&prepare_token);

    switch (retval)
    {
        case FDS_SUCCESS:
            return NRF_SUCCESS;

        case FDS_ERR_NULL_ARG:
            return NRF_ERROR_NULL;

        default:
            return NRF_ERROR_INTERNAL;
    }
}


ret_code_t pds_peer_data_write_prepared(pm_peer_id_t                 peer_id,
                                        pm_peer_data_const_t const * p_peer_data,
                                        pm_prepare_token_t           prepare_token,
                                        pm_store_token_t           * p_store_token)
{
    ret_code_t         retval;
    fds_record_t       record;
    fds_record_desc_t  record_desc;
    fds_record_chunk_t chunks[2];
    uint16_t           n_chunks;

    VERIFY_MODULE_INITIALIZED();
    VERIFY_PEER_ID_IN_RANGE(peer_id);
    VERIFY_PARAM_NOT_NULL(p_peer_data);
    VERIFY_PEER_DATA_ID_IN_RANGE(p_peer_data->data_id);
    VERIFY_PARAM_NOT_ZERO(prepare_token);

    // Create chunks.
    peer_data_parts_get(p_peer_data, chunks, &n_chunks);

    // Prepare the record to be written.
    record.file_id         = peer_id_to_file_id(peer_id);
    record.key             = peer_data_id_to_record_key(p_peer_data->data_id);
    record.data.p_chunks   = chunks;
    record.data.num_chunks = n_chunks;

    retval = fds_record_write_reserved(&record_desc,
                                       &record,
                                       (fds_reserve_token_t*)&prepare_token);

    if ((retval == FDS_SUCCESS) && (p_store_token != NULL))
    {
        // If fds_record_write_reserved() returned sucessfully, it is safe
        // to ignore the return value from fds_record_id_from_desc() since
        // the descriptor is valid, and also p_store_token is different from NULL.
        (void)fds_record_id_from_desc(&record_desc, (uint32_t*)p_store_token);
    }

    switch (retval)
    {
        case FDS_SUCCESS:
            return NRF_SUCCESS;

        case FDS_ERR_BUSY:
        case FDS_ERR_NO_SPACE_IN_QUEUES:
            return NRF_ERROR_BUSY;

        default:
            return NRF_ERROR_INTERNAL;
    }
}


ret_code_t pds_peer_data_write(pm_peer_id_t                 peer_id,
                               pm_peer_data_const_t const * p_peer_data,
                               pm_store_token_t           * p_store_token)
{
    ret_code_t         retval;
    fds_record_t       record;
    fds_record_desc_t  record_desc;
    fds_record_chunk_t chunks[2];
    uint16_t           n_chunks;

    VERIFY_MODULE_INITIALIZED();
    VERIFY_PEER_ID_IN_RANGE(peer_id);
    VERIFY_PARAM_NOT_NULL(p_peer_data);
    VERIFY_PEER_DATA_ID_IN_RANGE(p_peer_data->data_id);

    // Create chunks.
    peer_data_parts_get(p_peer_data, chunks, &n_chunks);

    // Prepare the record to be written.
    record.file_id         = peer_id_to_file_id(peer_id);
    record.key             = peer_data_id_to_record_key(p_peer_data->data_id);
    record.data.p_chunks   = chunks;
    record.data.num_chunks = n_chunks;

    retval = fds_record_write(&record_desc, &record);

    if ((retval == FDS_SUCCESS) && (p_store_token != NULL))
    {
        // If fds_record_write() returned sucessfully, it is safe
        // to ignore the return value from fds_record_id_from_desc() since
        // the descriptor is valid, and also p_store_token is different from NULL.
        (void)fds_record_id_from_desc(&record_desc, (uint32_t*)p_store_token);
    }

    switch (retval)
    {
        case FDS_SUCCESS:
            return NRF_SUCCESS;

        case FDS_ERR_BUSY:
        case FDS_ERR_NO_SPACE_IN_QUEUES:
            return NRF_ERROR_BUSY;

        case FDS_ERR_NO_SPACE_IN_FLASH:
            return NRF_ERROR_NO_MEM;

        default:
            return NRF_ERROR_INTERNAL;
    }
}


ret_code_t pds_peer_data_update(pm_peer_id_t                 peer_id,
                                pm_peer_data_const_t const * p_peer_data,
                                pm_store_token_t             old_token,
                                pm_store_token_t           * p_store_token)
{
    ret_code_t         retval;
    fds_record_t       record;
    fds_record_desc_t  record_desc;
    fds_record_chunk_t chunks[2];
    uint16_t           n_chunks;

    VERIFY_MODULE_INITIALIZED();
    VERIFY_PEER_ID_IN_RANGE(peer_id);
    VERIFY_PARAM_NOT_NULL(p_peer_data);
    VERIFY_PEER_DATA_ID_IN_RANGE(p_peer_data->data_id);

    // Create chunks.
    peer_data_parts_get(p_peer_data, chunks, &n_chunks);

    // Prepare the record to be written.
    record.file_id         = peer_id_to_file_id(peer_id);
    record.key             = peer_data_id_to_record_key(p_peer_data->data_id);
    record.data.p_chunks   = chunks;
    record.data.num_chunks = n_chunks;

    // Obtain the descriptor of the record to be updated.
    // It is safe to ignore the return value if record_desc is different from NULL.
    (void)fds_descriptor_from_rec_id(&record_desc, (uint32_t)old_token);

    retval = fds_record_update(&record_desc, &record);

    if ((retval == FDS_SUCCESS) && (p_store_token != NULL))
    {
        // If fds_record_update() returned sucessfully, it is safe
        // to ignore the return value from fds_record_id_from_desc() since
        // the descriptor is valid, and also p_store_token is different from NULL.
        (void)fds_record_id_from_desc(&record_desc, (uint32_t*)p_store_token);
    }

    switch (retval)
    {
        case FDS_SUCCESS:
            return NRF_SUCCESS;

        case FDS_ERR_BUSY:
        case FDS_ERR_NO_SPACE_IN_QUEUES:
            return NRF_ERROR_BUSY;

        case FDS_ERR_NO_SPACE_IN_FLASH:
            return NRF_ERROR_NO_MEM;

        default:
            return NRF_ERROR_INTERNAL;
    }
}

ret_code_t pds_peer_data_clear(pm_peer_id_t peer_id, pm_peer_data_id_t data_id)
{
    ret_code_t        retval;
    uint16_t          file_id;
    uint16_t          record_key;
    fds_record_desc_t record_desc;
    fds_find_token_t  find_tok = {0};

    VERIFY_MODULE_INITIALIZED();
    VERIFY_PEER_ID_IN_RANGE(peer_id);
    VERIFY_PEER_DATA_ID_IN_RANGE(data_id);

    file_id    = peer_id_to_file_id(peer_id);
    record_key = peer_data_id_to_record_key(data_id);

    retval = fds_record_find(file_id, record_key, &record_desc, &find_tok);
    if(retval != FDS_SUCCESS)
    {
        return NRF_ERROR_NOT_FOUND;
    }

    retval = fds_record_delete(&record_desc);

    switch (retval)
    {
        case FDS_SUCCESS:
            return NRF_SUCCESS;

        case FDS_ERR_NO_SPACE_IN_QUEUES:
            return NRF_ERROR_BUSY;

        default:
            return NRF_ERROR_INTERNAL;
    }
}


pm_peer_id_t pds_peer_id_allocate(void)
{
    if (!MODULE_INITIALIZED)
    {
        return PM_PEER_ID_INVALID;
    }
    PEER_IDS_INITIALIZE();
    return peer_id_allocate(PM_PEER_ID_INVALID);
}


ret_code_t pds_peer_id_free(pm_peer_id_t peer_id)
{
    VERIFY_MODULE_INITIALIZED();
    VERIFY_PEER_ID_IN_RANGE(peer_id);
    PEER_IDS_INITIALIZE();

    (void)peer_id_delete(peer_id);
    peer_data_clear();

    return NRF_SUCCESS;
}


bool pds_peer_id_is_allocated(pm_peer_id_t peer_id)
{
    if (!MODULE_INITIALIZED)
    {
        return false;
    }
    PEER_IDS_INITIALIZE();

    return peer_id_is_allocated(peer_id);
}


pm_peer_id_t pds_next_peer_id_get(pm_peer_id_t prev_peer_id)
{
    if (!MODULE_INITIALIZED)
    {
        return PM_PEER_ID_INVALID;
    }
    PEER_IDS_INITIALIZE();

    return peer_id_get_next_used(prev_peer_id);
}


uint32_t pds_n_peers(void)
{
    if (!MODULE_INITIALIZED)
    {
        return 0;
    }
    PEER_IDS_INITIALIZE();
    return peer_id_n_ids();
}

//lint -restore
