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


#include "peer_manager.h"
#include <string.h>
#include "app_util.h"
#include "security_manager.h"
#include "gatt_cache_manager.h"
#include "peer_database.h"
#include "id_manager.h"
#include "ble_conn_state.h"
#include "sdk_common.h"
#include "peer_manager_internal.h"

#define MODULE_INITIALIZED m_pm.initialized
#include "sdk_macros.h"


#define MAX_REGISTRANTS 3  /**< The number of event handlers that can be registered with the module. */


/**@brief Internal state of the module.
 */
typedef struct
{
    uint8_t                       initialized           : 1;     /**< Whether or not @ref pm_init has been called successfully. */
    uint8_t                       peer_rank_initialized : 1;     /**< Whether or not @ref rank_init has been called successfully. */
    uint8_t                       deleting_all          : 1;     /**< True from when @ref pm_peers_delete is called until all peers have been deleted. */
    pm_store_token_t              peer_rank_token;               /**< The store token of an ongoing peer rank update via a call to @ref pm_peer_rank_highest. If @ref PM_STORE_TOKEN_INVALID, there is no ongoing update. */
    uint32_t                      current_highest_peer_rank;     /**< The current highest peer rank. Used by @ref pm_peer_rank_highest. */
    pm_peer_id_t                  highest_ranked_peer;           /**< The peer with the highest peer rank. Used by @ref pm_peer_rank_highest. */
    pm_evt_handler_t              evt_handlers[MAX_REGISTRANTS]; /**< The subscribers to Peer Manager events, as registered through @ref pm_register. */
    uint8_t                       n_registrants;                 /**< The number of event handlers registered through @ref pm_register. */
    ble_conn_state_user_flag_id_t pairing_flag_id;               /**< The flag ID for which connections are paired. */
    ble_conn_state_user_flag_id_t bonding_flag_id;               /**< The flag ID for which connections are bonded. */
} pm_t;


/**@brief Instantiation of Peer Manager internal state.
 */
static pm_t m_pm =
{
    .initialized           = 0,
    .peer_rank_initialized = 0,
};

/**@brief Function for sending a Peer Manager event to all subscribers.
 *
 * @param[in]  p_pm_evt  The event to send.
 */
static void evt_send(pm_evt_t * p_pm_evt)
{
    for (int i = 0; i < m_pm.n_registrants; i++)
    {
        m_pm.evt_handlers[i](p_pm_evt);
    }
}


/**@brief Event handler for events from the Peer Database module.
 *
 * @param[in]  p_pdb_evt  The incoming Peer Database event.
 */
static void pdb_evt_handler(pdb_evt_t const * p_pdb_evt)
{
    bool send_evt = true;
    pm_evt_t pm_evt;

    memset(&pm_evt, 0, sizeof(pm_evt_t));
    pm_evt.peer_id = p_pdb_evt->peer_id;
    pm_evt.conn_handle = im_conn_handle_get(pm_evt.peer_id);

    switch (p_pdb_evt->evt_id)
    {
        case PDB_EVT_WRITE_BUF_STORED:
            pm_evt.evt_id = PM_EVT_PEER_DATA_UPDATE_SUCCEEDED;
            pm_evt.params.peer_data_update_succeeded.data_id       = p_pdb_evt->data_id;
            pm_evt.params.peer_data_update_succeeded.action        = PM_PEER_DATA_OP_UPDATE;
            pm_evt.params.peer_data_update_succeeded.token         = PM_STORE_TOKEN_INVALID;
            pm_evt.params.peer_data_update_succeeded.flash_changed = true;
            break;

        case PDB_EVT_RAW_STORED:
            pm_evt.evt_id = PM_EVT_PEER_DATA_UPDATE_SUCCEEDED;
            pm_evt.params.peer_data_update_succeeded.data_id = p_pdb_evt->data_id;
            pm_evt.params.peer_data_update_succeeded.action  = PM_PEER_DATA_OP_UPDATE;
            pm_evt.params.peer_data_update_succeeded.token
                                            = p_pdb_evt->params.raw_stored_evt.store_token;
            pm_evt.params.peer_data_update_succeeded.flash_changed = true;

            if(    (m_pm.peer_rank_token != PM_STORE_TOKEN_INVALID)
                && (m_pm.peer_rank_token == p_pdb_evt->params.raw_stored_evt.store_token))
            {
                m_pm.peer_rank_token     = PM_STORE_TOKEN_INVALID;
                m_pm.highest_ranked_peer = pm_evt.peer_id;

                pm_evt.params.peer_data_update_succeeded.token = PM_STORE_TOKEN_INVALID;
            }
            break;

        case PDB_EVT_RAW_STORE_FAILED:
            pm_evt.evt_id = PM_EVT_PEER_DATA_UPDATE_FAILED;
            pm_evt.params.peer_data_update_failed.data_id = p_pdb_evt->data_id;
            pm_evt.params.peer_data_update_failed.action  = PM_PEER_DATA_OP_UPDATE;
            pm_evt.params.peer_data_update_failed.token
                                            = p_pdb_evt->params.error_raw_store_evt.store_token;
            pm_evt.params.peer_data_update_failed.error
                                            = p_pdb_evt->params.error_raw_store_evt.err_code;

            if(    (m_pm.peer_rank_token != PM_STORE_TOKEN_INVALID)
                && (m_pm.peer_rank_token == p_pdb_evt->params.raw_stored_evt.store_token))
            {
                m_pm.peer_rank_token            = PM_STORE_TOKEN_INVALID;
                m_pm.current_highest_peer_rank -= 1;

                pm_evt.params.peer_data_update_succeeded.token = PM_STORE_TOKEN_INVALID;
            }
            break;

        case PDB_EVT_CLEARED:
            pm_evt.evt_id = PM_EVT_PEER_DATA_UPDATE_SUCCEEDED;
            pm_evt.params.peer_data_update_succeeded.data_id = p_pdb_evt->data_id;
            pm_evt.params.peer_data_update_succeeded.action  = PM_PEER_DATA_OP_DELETE;
            pm_evt.params.peer_data_update_succeeded.token   = PM_STORE_TOKEN_INVALID;
            break;

        case PDB_EVT_CLEAR_FAILED:
            pm_evt.evt_id = PM_EVT_PEER_DATA_UPDATE_FAILED;
            pm_evt.params.peer_data_update_failed.data_id = p_pdb_evt->data_id;
            pm_evt.params.peer_data_update_failed.action  = PM_PEER_DATA_OP_DELETE;
            pm_evt.params.peer_data_update_failed.error
                                                    = p_pdb_evt->params.clear_failed_evt.err_code;
            break;

        case PDB_EVT_PEER_FREED:
            pm_evt.evt_id = PM_EVT_PEER_DELETE_SUCCEEDED;
            if (m_pm.deleting_all && (pdb_next_peer_id_get(PM_PEER_ID_INVALID) == PM_PEER_ID_INVALID))
            {
                // pm_peers_delete() has been called and this is the last peer to be deleted.

                m_pm.deleting_all = false;

                pm_evt_t pm_delete_all_evt;
                memset(&pm_delete_all_evt, 0, sizeof(pm_evt_t));
                pm_delete_all_evt.evt_id      = PM_EVT_PEERS_DELETE_SUCCEEDED;
                pm_delete_all_evt.peer_id     = PM_PEER_ID_INVALID;
                pm_delete_all_evt.conn_handle = BLE_CONN_HANDLE_INVALID;

                evt_send(&pm_delete_all_evt);
            }
            break;

        case PDB_EVT_PEER_FREE_FAILED:
            pm_evt.evt_id = PM_EVT_PEER_DELETE_FAILED;
            pm_evt.params.peer_delete_failed.error
                                                = p_pdb_evt->params.peer_free_failed_evt.err_code;
            if (m_pm.deleting_all)
            {
                // pm_peers_delete() has been called and has thus failed.

                m_pm.deleting_all = false;

                pm_evt_t pm_delete_all_evt;
                memset(&pm_delete_all_evt, 0, sizeof(pm_evt_t));
                pm_delete_all_evt.evt_id      = PM_EVT_PEERS_DELETE_FAILED;
                pm_delete_all_evt.peer_id     = PM_PEER_ID_INVALID;
                pm_delete_all_evt.conn_handle = BLE_CONN_HANDLE_INVALID;
                pm_delete_all_evt.params.peers_delete_failed_evt.error
                                              = p_pdb_evt->params.peer_free_failed_evt.err_code;

                evt_send(&pm_delete_all_evt);
            }
            break;

        case PDB_EVT_COMPRESSED:
            send_evt = false;
            // Do nothing
            break;

        case PDB_EVT_ERROR_NO_MEM:
            pm_evt.evt_id = PM_EVT_STORAGE_FULL;
            break;

        case PDB_EVT_ERROR_UNEXPECTED:
            pm_evt.evt_id = PM_EVT_ERROR_UNEXPECTED;
            break;

        default:
            send_evt = false;
            break;
    }

    if (send_evt)
    {
        evt_send(&pm_evt);
    }
}


/**@brief Event handler for events from the Security Manager module.
 *
 * @param[in]  p_sm_evt  The incoming Security Manager event.
 */
static void sm_evt_handler(sm_evt_t const * p_sm_evt)
{
    bool find_peer_id = true;
    bool send_evt     = true;
    pm_evt_t pm_evt;
    memset(&pm_evt, 0, sizeof(pm_evt_t));
    pm_evt.conn_handle = p_sm_evt->conn_handle;

    switch (p_sm_evt->evt_id)
    {
        case SM_EVT_SLAVE_SECURITY_REQ:
            find_peer_id = false;
            send_evt     = false;
            break;

        case SM_EVT_SEC_PROCEDURE_START:
        {
            pm_evt.evt_id = PM_EVT_CONN_SEC_START;
            bool pairing = p_sm_evt->params.sec_procedure_start.procedure
                                != PM_LINK_SECURED_PROCEDURE_ENCRYPTION;
            bool bonding = p_sm_evt->params.sec_procedure_start.procedure
                                == PM_LINK_SECURED_PROCEDURE_BONDING;
            ble_conn_state_user_flag_set(p_sm_evt->conn_handle, m_pm.pairing_flag_id, pairing);
            ble_conn_state_user_flag_set(p_sm_evt->conn_handle, m_pm.bonding_flag_id, bonding);
            break;
        }

        case SM_EVT_PAIRING_SUCCESS:
            pm_evt.evt_id = PM_EVT_CONN_SEC_SUCCEEDED;
            pm_evt.params.conn_sec_succeeded.procedure =
                        p_sm_evt->params.pairing_success.bonded
                        ? PM_LINK_SECURED_PROCEDURE_BONDING
                        : PM_LINK_SECURED_PROCEDURE_PAIRING;
            ble_conn_state_user_flag_set(p_sm_evt->conn_handle, m_pm.pairing_flag_id, true);
            ble_conn_state_user_flag_set(p_sm_evt->conn_handle,
                                         m_pm.bonding_flag_id,
                                         p_sm_evt->params.pairing_success.bonded
            );
            break;

        case SM_EVT_PAIRING_FAIL:
            pm_evt.evt_id = PM_EVT_CONN_SEC_FAILED;
            pm_evt.params.conn_sec_failed.procedure =
                        ble_conn_state_user_flag_get(p_sm_evt->conn_handle, m_pm.bonding_flag_id)
                        ? PM_LINK_SECURED_PROCEDURE_BONDING
                        : PM_LINK_SECURED_PROCEDURE_PAIRING;
            pm_evt.params.conn_sec_failed.error_src
                = p_sm_evt->params.pairing_failed.error_src;
            pm_evt.params.conn_sec_failed.error
                = p_sm_evt->params.pairing_failed.error;
            break;

        case SM_EVT_LINK_ENCRYPTION_UPDATE:
            if (!ble_conn_state_user_flag_get(p_sm_evt->conn_handle, m_pm.pairing_flag_id))
            {
                pm_evt.evt_id = PM_EVT_CONN_SEC_SUCCEEDED;
                pm_evt.params.conn_sec_succeeded.procedure = PM_LINK_SECURED_PROCEDURE_ENCRYPTION;
            }
            else
            {
                find_peer_id = false;
                send_evt     = false;
            }
            break;

        case SM_EVT_LINK_ENCRYPTION_FAILED:
            pm_evt.evt_id = PM_EVT_CONN_SEC_FAILED;
            pm_evt.params.conn_sec_failed.procedure
                            = PM_LINK_SECURED_PROCEDURE_ENCRYPTION;
            pm_evt.params.conn_sec_failed.error_src
                            = p_sm_evt->params.link_encryption_failed.error_src;
            pm_evt.params.conn_sec_failed.error
                            = p_sm_evt->params.link_encryption_failed.error;
            break;

        case SM_EVT_BONDING_INFO_STORED:
            pm_evt.evt_id  = PM_EVT_PEER_DATA_UPDATE_SUCCEEDED;
            pm_evt.peer_id = p_sm_evt->params.bonding_info_stored.peer_id;
            pm_evt.params.peer_data_update_succeeded.data_id = PM_PEER_DATA_ID_BONDING;
            pm_evt.params.peer_data_update_succeeded.action  = PM_PEER_DATA_OP_UPDATE;
            find_peer_id = false;
            break;

        case SM_EVT_ERROR_BONDING_INFO:
            pm_evt.evt_id = PM_EVT_PEER_DATA_UPDATE_FAILED;
            pm_evt.peer_id = p_sm_evt->params.error_bonding_info.peer_id;
            pm_evt.params.peer_data_update_failed.data_id = PM_PEER_DATA_ID_BONDING;
            pm_evt.params.peer_data_update_failed.action  = PM_PEER_DATA_OP_UPDATE;
            pm_evt.params.peer_data_update_failed.error
                = p_sm_evt->params.error_bonding_info.error;
            find_peer_id = false;
            break;

        case SM_EVT_ERROR_UNEXPECTED:
            pm_evt.evt_id = PM_EVT_ERROR_UNEXPECTED;
            pm_evt.params.error_unexpected.error = p_sm_evt->params.error_unexpected.error;
            break;

        case SM_EVT_ERROR_NO_MEM:
            pm_evt.evt_id = PM_EVT_STORAGE_FULL;
            break;

        case SM_EVT_ERROR_SMP_TIMEOUT:
            pm_evt.evt_id = PM_EVT_CONN_SEC_FAILED;
            pm_evt.params.conn_sec_failed.procedure
                        = ble_conn_state_user_flag_get(p_sm_evt->conn_handle, m_pm.bonding_flag_id)
                        ? PM_LINK_SECURED_PROCEDURE_BONDING
                        : PM_LINK_SECURED_PROCEDURE_PAIRING;
            pm_evt.params.conn_sec_failed.error_src  = BLE_GAP_SEC_STATUS_SOURCE_LOCAL;
            pm_evt.params.conn_sec_failed.error      = PM_CONN_SEC_ERROR_SMP_TIMEOUT;
            break;

        case SM_EVT_CONN_SEC_CONFIG_REQ:
            pm_evt.evt_id = PM_EVT_CONN_SEC_CONFIG_REQ;
            break;

        default:
            send_evt = false;
            break;
    }

    if (find_peer_id)
    {
        pm_evt.peer_id = im_peer_id_get_by_conn_handle(p_sm_evt->conn_handle);
    }

    if (send_evt)
    {
        evt_send(&pm_evt);
    }
}


/**@brief Event handler for events from the GATT Cache Manager module.
 *
 * @param[in]  p_gcm_evt  The incoming GATT Cache Manager event.
 */
static void gcm_evt_handler(gcm_evt_t const * p_gcm_evt)
{

    bool send_evt = true;
    pm_evt_t pm_evt;

    memset(&pm_evt, 0, sizeof(pm_evt_t));
    pm_evt.peer_id = p_gcm_evt->peer_id;
    pm_evt.conn_handle = im_conn_handle_get(pm_evt.peer_id);

    switch (p_gcm_evt->evt_id)
    {
        case GCM_EVT_LOCAL_DB_CACHE_STORED:
            pm_evt.evt_id = PM_EVT_PEER_DATA_UPDATE_SUCCEEDED;
            pm_evt.params.peer_data_update_succeeded.action = PM_PEER_DATA_OP_UPDATE;
            pm_evt.params.peer_data_update_succeeded.data_id =  PM_PEER_DATA_ID_GATT_LOCAL;
            break;

        case GCM_EVT_LOCAL_DB_CACHE_UPDATED:
            pm_evt.evt_id = PM_EVT_PEER_DATA_UPDATE_SUCCEEDED;
            pm_evt.params.peer_data_update_succeeded.action = PM_PEER_DATA_OP_UPDATE;
            pm_evt.params.peer_data_update_succeeded.data_id =  PM_PEER_DATA_ID_GATT_LOCAL;
            break;

        case GCM_EVT_LOCAL_DB_CACHE_APPLIED:
            pm_evt.evt_id = PM_EVT_LOCAL_DB_CACHE_APPLIED;
            break;

        case GCM_EVT_ERROR_LOCAL_DB_CACHE_APPLY:
            pm_evt.evt_id = PM_EVT_LOCAL_DB_CACHE_APPLY_FAILED;
            break;

        case GCM_EVT_REMOTE_DB_CACHE_UPDATED:
            pm_evt.evt_id = PM_EVT_PEER_DATA_UPDATE_SUCCEEDED;
            pm_evt.params.peer_data_update_succeeded.action = PM_PEER_DATA_OP_UPDATE;
            pm_evt.params.peer_data_update_succeeded.data_id =  PM_PEER_DATA_ID_GATT_REMOTE;
            break;

        case GCM_EVT_SERVICE_CHANGED_IND_SENT:
            pm_evt.evt_id = PM_EVT_SERVICE_CHANGED_IND_SENT;
            break;

        case GCM_EVT_SERVICE_CHANGED_IND_CONFIRMED:
            pm_evt.evt_id = PM_EVT_SERVICE_CHANGED_IND_CONFIRMED;
            break;

        case GCM_EVT_ERROR_DATA_SIZE:
            send_evt = false;
            break;

        case GCM_EVT_ERROR_STORAGE_FULL:
            pm_evt.evt_id = PM_EVT_STORAGE_FULL;
            break;

        case GCM_EVT_ERROR_UNEXPECTED:
            pm_evt.evt_id = PM_EVT_ERROR_UNEXPECTED;
            pm_evt.params.error_unexpected.error = p_gcm_evt->params.error_unexpected.error;
            pm_evt.conn_handle = p_gcm_evt->params.error_unexpected.conn_handle;
            break;
    }

    if (send_evt)
    {
        evt_send(&pm_evt);
    }
}


/**@brief Event handler for events from the ID Manager module.
 *
 * @param[in]  p_im_evt  The incoming ID Manager event.
 */
static void im_evt_handler(im_evt_t const * p_im_evt)
{
    pm_evt_t pm_evt;
    ret_code_t err_code;

    switch (p_im_evt->evt_id)
    {
        case IM_EVT_DUPLICATE_ID:
            // Attempt to delete the duplicate data to free space and avoid finding old data when
            // scanning in the future
            err_code = pm_peer_delete(p_im_evt->params.duplicate_id.peer_id_2);
            UNUSED_VARIABLE(err_code);
            break;

        case IM_EVT_BONDED_PEER_CONNECTED:
            ble_conn_state_user_flag_set(p_im_evt->conn_handle, m_pm.bonding_flag_id, true);
            memset(&pm_evt, 0, sizeof(pm_evt_t));
            pm_evt.conn_handle = p_im_evt->conn_handle;
            pm_evt.peer_id = im_peer_id_get_by_conn_handle(p_im_evt->conn_handle);
            pm_evt.evt_id = PM_EVT_BONDED_PEER_CONNECTED;
            evt_send(&pm_evt);
            break;
    }
}


void pm_on_ble_evt(ble_evt_t * p_ble_evt)
{
    VERIFY_MODULE_INITIALIZED_VOID();

    im_ble_evt_handler(p_ble_evt);
    sm_ble_evt_handler(p_ble_evt);
    gcm_ble_evt_handler(p_ble_evt);
}


/**@brief Function for resetting the internal state of this module.
 */
static void internal_state_reset()
{
    memset(&m_pm, 0, sizeof(m_pm));
    m_pm.highest_ranked_peer = PM_PEER_ID_INVALID;
    m_pm.peer_rank_token     = PM_STORE_TOKEN_INVALID;
    m_pm.pairing_flag_id     = BLE_CONN_STATE_USER_FLAG_INVALID;
    m_pm.bonding_flag_id     = BLE_CONN_STATE_USER_FLAG_INVALID;
}


ret_code_t pm_init(void)
{
    ret_code_t err_code;

    err_code = pdb_register(pdb_evt_handler);
    if (err_code != NRF_SUCCESS)
    {
        if (err_code != NRF_ERROR_INVALID_STATE)
        {
            err_code = NRF_ERROR_INTERNAL;
        }
        return err_code;
    }

    err_code = sm_register(sm_evt_handler);
    if (err_code != NRF_SUCCESS)
    {
        return NRF_ERROR_INTERNAL;
    }

    err_code = gcm_init(gcm_evt_handler);
    if (err_code != NRF_SUCCESS)
    {
        return NRF_ERROR_INTERNAL;
    }

    err_code = im_register(im_evt_handler);
    if (err_code != NRF_SUCCESS)
    {
        return NRF_ERROR_INTERNAL;
    }

    internal_state_reset();

    m_pm.pairing_flag_id = ble_conn_state_user_flag_acquire();
    if (m_pm.pairing_flag_id == BLE_CONN_STATE_USER_FLAG_INVALID)
    {
        return NRF_ERROR_INTERNAL;
    }

    m_pm.bonding_flag_id = ble_conn_state_user_flag_acquire();
    if (m_pm.bonding_flag_id == BLE_CONN_STATE_USER_FLAG_INVALID)
    {
        return NRF_ERROR_INTERNAL;
    }

    m_pm.peer_rank_initialized = false;
    m_pm.initialized           = true;

    return NRF_SUCCESS;
}


ret_code_t pm_register(pm_evt_handler_t event_handler)
{
    VERIFY_MODULE_INITIALIZED();

    if (m_pm.n_registrants >= MAX_REGISTRANTS)
    {
        return NRF_ERROR_NO_MEM;
    }

    m_pm.evt_handlers[m_pm.n_registrants] = event_handler;
    m_pm.n_registrants += 1;

    return NRF_SUCCESS;
}


ret_code_t pm_sec_params_set(ble_gap_sec_params_t * p_sec_params)
{
    VERIFY_MODULE_INITIALIZED();

    ret_code_t err_code;

    err_code = sm_sec_params_set(p_sec_params);

    if (err_code == NRF_ERROR_INVALID_STATE)
    {
        err_code = NRF_ERROR_INTERNAL;
    }

    return err_code;
}


ret_code_t pm_conn_secure(uint16_t conn_handle, bool force_repairing)
{
    VERIFY_MODULE_INITIALIZED();

    ret_code_t err_code;

    err_code = sm_link_secure(conn_handle, force_repairing);

    return err_code;
}


void pm_conn_sec_config_reply(uint16_t conn_handle, pm_conn_sec_config_t * p_conn_sec_config)
{
    sm_conn_sec_config_reply(conn_handle, p_conn_sec_config);
}


ret_code_t pm_sec_params_reply(uint16_t conn_handle, ble_gap_sec_params_t * p_sec_params)
{
    VERIFY_MODULE_INITIALIZED();
    return NRF_SUCCESS;
}


void pm_local_database_has_changed(void)
{
    VERIFY_MODULE_INITIALIZED_VOID();
    gcm_local_database_has_changed();
}


ret_code_t pm_whitelist_create(pm_peer_id_t * p_peer_ids,
                               uint8_t n_peer_ids,
                               ble_gap_whitelist_t * p_whitelist)
{
    VERIFY_MODULE_INITIALIZED();
    return im_whitelist_create(p_peer_ids, n_peer_ids, p_whitelist);
}


ret_code_t pm_whitelist_custom(ble_gap_whitelist_t * p_whitelist)
{
    VERIFY_MODULE_INITIALIZED();
    return im_whitelist_custom(p_whitelist);
}


ret_code_t pm_conn_sec_status_get(uint16_t conn_handle, pm_conn_sec_status_t * p_conn_sec_status)
{
    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_NULL(p_conn_sec_status);

    ble_conn_state_status_t status = ble_conn_state_status(conn_handle);

    if (status == BLE_CONN_STATUS_INVALID)
    {
        return BLE_ERROR_INVALID_CONN_HANDLE;
    }

    p_conn_sec_status->connected      = (status == BLE_CONN_STATUS_CONNECTED);
    p_conn_sec_status->bonded         = ble_conn_state_user_flag_get(conn_handle, m_pm.bonding_flag_id);
    p_conn_sec_status->encrypted      = ble_conn_state_encrypted(conn_handle);
    p_conn_sec_status->mitm_protected = ble_conn_state_mitm_protected(conn_handle);
    return NRF_SUCCESS;
}


ret_code_t pm_lesc_public_key_set(ble_gap_lesc_p256_pk_t * p_public_key)
{
    VERIFY_MODULE_INITIALIZED();
    return sm_lesc_public_key_set(p_public_key);
}


ret_code_t pm_conn_handle_get(pm_peer_id_t peer_id, uint16_t * p_conn_handle)
{
    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_NULL(p_conn_handle);
    *p_conn_handle = im_conn_handle_get(peer_id);
    return NRF_SUCCESS;
}


ret_code_t pm_peer_id_get(uint16_t conn_handle, pm_peer_id_t * p_peer_id)
{
    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_NULL(p_peer_id);
    *p_peer_id = im_peer_id_get_by_conn_handle(conn_handle);
    return NRF_SUCCESS;
}


uint32_t pm_peer_count(void)
{
    if (!MODULE_INITIALIZED)
    {
        return 0;
    }
    return pdb_n_peers();
}


pm_peer_id_t pm_next_peer_id_get(pm_peer_id_t prev_peer_id)
{
    if (!MODULE_INITIALIZED)
    {
        return PM_PEER_ID_INVALID;
    }
    return pdb_next_peer_id_get(prev_peer_id);
}


ret_code_t pm_peer_data_load(pm_peer_id_t       peer_id,
                             pm_peer_data_id_t  data_id,
                             void             * p_data,
                             uint16_t         * p_length)
{
    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_NULL(p_data);
    VERIFY_PARAM_NOT_NULL(p_length);
    if (ALIGN_NUM(4, *p_length) != *p_length)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    pm_peer_data_t peer_data;
    memset(&peer_data, 0, sizeof(peer_data));
    peer_data.length_words = BYTES_TO_WORDS(*p_length);
    peer_data.data_id      = data_id;
    peer_data.p_all_data   = p_data;

    ret_code_t err_code = pdb_raw_read(peer_id, data_id, &peer_data);

    *p_length = peer_data.length_words * BYTES_PER_WORD;

    return err_code;
}


ret_code_t pm_peer_data_bonding_load(pm_peer_id_t             peer_id,
                                     pm_peer_data_bonding_t * p_data)
{
    uint16_t length = sizeof(pm_peer_data_bonding_t);
    return pm_peer_data_load(peer_id,
                             PM_PEER_DATA_ID_BONDING,
                             p_data,
                             &length);
}


ret_code_t pm_peer_data_remote_db_load(pm_peer_id_t        peer_id,
                                       ble_gatt_db_srv_t * p_data,
                                       uint16_t          * p_length)
{
    return pm_peer_data_load(peer_id,
                             PM_PEER_DATA_ID_GATT_REMOTE,
                             p_data,
                             p_length);
}


ret_code_t pm_peer_data_app_data_load(pm_peer_id_t       peer_id,
                                      uint8_t          * p_data,
                                      uint16_t         * p_length)
{
    return pm_peer_data_load(peer_id,
                             PM_PEER_DATA_ID_APPLICATION,
                             p_data,
                             p_length);
}


ret_code_t pm_peer_data_store(pm_peer_id_t       peer_id,
                              pm_peer_data_id_t  data_id,
                              void       const * p_data,
                              uint16_t           length,
                              pm_store_token_t * p_token)
{
    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_NULL(p_data);
    if (ALIGN_NUM(4, length) != length)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    pm_peer_data_const_t peer_data;
    memset(&peer_data, 0, sizeof(peer_data));
    peer_data.length_words = BYTES_TO_WORDS(length);
    peer_data.data_id      = data_id;
    peer_data.p_all_data   = p_data;

    return pdb_raw_store(peer_id, &peer_data, p_token);
}


ret_code_t pm_peer_data_bonding_store(pm_peer_id_t                   peer_id,
                                      pm_peer_data_bonding_t const * p_data,
                                      pm_store_token_t             * p_token)
{
    return pm_peer_data_store(peer_id,
                              PM_PEER_DATA_ID_BONDING,
                              p_data,
                              ALIGN_NUM(4, sizeof(pm_peer_data_bonding_t)),
                              p_token);
}


ret_code_t pm_peer_data_remote_db_store(pm_peer_id_t              peer_id,
                                        ble_gatt_db_srv_t const * p_data,
                                        uint16_t                  length,
                                        pm_store_token_t        * p_token)
{
    return pm_peer_data_store(peer_id,
                              PM_PEER_DATA_ID_GATT_REMOTE,
                              p_data,
                              length,
                              p_token);
}


ret_code_t pm_peer_data_app_data_store(pm_peer_id_t       peer_id,
                                       uint8_t    const * p_data,
                                       uint16_t           length,
                                       pm_store_token_t * p_token)
{
    return pm_peer_data_store(peer_id,
                              PM_PEER_DATA_ID_APPLICATION,
                              p_data,
                              length,
                              p_token);
}


ret_code_t pm_peer_data_delete(pm_peer_id_t peer_id, pm_peer_data_id_t data_id)
{
    VERIFY_MODULE_INITIALIZED();

    if (data_id == PM_PEER_DATA_ID_BONDING)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    return pdb_clear(peer_id, data_id);
}


ret_code_t pm_peer_new(pm_peer_id_t           * p_new_peer_id,
                       pm_peer_data_bonding_t * p_bonding_data,
                       pm_store_token_t       * p_token)
{
    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_NULL(p_bonding_data);
    VERIFY_PARAM_NOT_NULL(p_new_peer_id);
    ret_code_t err_code;

    *p_new_peer_id = pdb_peer_allocate();
    if (*p_new_peer_id == PM_PEER_ID_INVALID)
    {
        return NRF_ERROR_NO_MEM;
    }

    pm_peer_data_const_t peer_data;
    memset(&peer_data, 0, sizeof(pm_peer_data_const_t));
    peer_data.length_words        = BYTES_TO_WORDS(sizeof(pm_peer_data_bonding_t));
    peer_data.data_id             = PM_PEER_DATA_ID_BONDING;
    peer_data.p_bonding_data      = p_bonding_data;

    err_code = pm_peer_data_bonding_store(*p_new_peer_id, p_bonding_data, p_token);
    if (err_code != NRF_SUCCESS)
    {
        ret_code_t err_code_free = im_peer_free(*p_new_peer_id);
        if (err_code_free != NRF_SUCCESS)
        {
            err_code = NRF_ERROR_INTERNAL;
        }
    }
    return err_code;
}


ret_code_t pm_peer_delete(pm_peer_id_t peer_id)
{
    VERIFY_MODULE_INITIALIZED();

    return im_peer_free(peer_id);
}


ret_code_t pm_peers_delete(void)
{
    VERIFY_MODULE_INITIALIZED();

    m_pm.deleting_all = true;

    pm_peer_id_t current_peer_id = pdb_next_peer_id_get(PM_PEER_ID_INVALID);
    while (current_peer_id != PM_PEER_ID_INVALID)
    {
        ret_code_t err_code = pm_peer_delete(current_peer_id);
        if (err_code != NRF_SUCCESS)
        {
            return NRF_ERROR_INTERNAL;
        }

        current_peer_id = pdb_next_peer_id_get(current_peer_id);
    }

    return NRF_SUCCESS;
}


ret_code_t pm_peer_ranks_get(pm_peer_id_t * p_highest_ranked_peer,
                             uint32_t     * p_highest_rank,
                             pm_peer_id_t * p_lowest_ranked_peer,
                             uint32_t     * p_lowest_rank)
{
    VERIFY_MODULE_INITIALIZED();

    pm_peer_id_t         peer_id      = pdb_next_peer_id_get(PM_PEER_ID_INVALID);
    uint32_t             peer_rank    = 0;
    //lint -save -e65 -e64
    pm_peer_data_t       peer_data    = {.length_words = BYTES_TO_WORDS(sizeof(peer_rank)),
                                         .p_peer_rank  = &peer_rank};
    //lint -restore
    ret_code_t           err_code     = pdb_raw_read(peer_id, PM_PEER_DATA_ID_PEER_RANK, &peer_data);
    uint32_t             highest_rank = 0;
    uint32_t             lowest_rank  = 0xFFFFFFFF;
    pm_peer_id_t         highest_ranked_peer = PM_PEER_ID_INVALID;
    pm_peer_id_t         lowest_ranked_peer  = PM_PEER_ID_INVALID;

    if (err_code == NRF_ERROR_INVALID_PARAM)
    {
        // No peer IDs exist.
        return NRF_ERROR_NOT_FOUND;
    }

    while ((err_code == NRF_SUCCESS) || (err_code == NRF_ERROR_NOT_FOUND))
    {
        if (err_code == NRF_ERROR_NOT_FOUND)
        {
            peer_rank = 0;
        }
        if (peer_rank >= highest_rank)
        {
            highest_rank      = peer_rank;
            highest_ranked_peer = peer_id;
        }
        if (peer_rank < lowest_rank)
        {
            lowest_rank      = peer_rank;
            lowest_ranked_peer = peer_id;
        }
        peer_id = pdb_next_peer_id_get(peer_id);
        err_code = pdb_raw_read(peer_id, PM_PEER_DATA_ID_PEER_RANK, &peer_data);
    }
    if (peer_id == PM_PEER_ID_INVALID)
    {
        err_code = NRF_SUCCESS;
        if (p_highest_ranked_peer != NULL)
        {
            *p_highest_ranked_peer = highest_ranked_peer;
        }
        if (p_highest_rank != NULL)
        {
            *p_highest_rank = highest_rank;
        }
        if (p_lowest_ranked_peer != NULL)
        {
            *p_lowest_ranked_peer = lowest_ranked_peer;
        }
        if (p_lowest_rank != NULL)
        {
            *p_lowest_rank = lowest_rank;
        }
    }
    else
    {
        err_code = NRF_ERROR_INTERNAL;
    }
    return err_code;
}


/**@brief Function for initializing peer rank statistics.
 */
static void rank_init(void)
{
    ret_code_t err_code = pm_peer_ranks_get(&m_pm.highest_ranked_peer,
                                            &m_pm.current_highest_peer_rank,
                                            NULL,
                                            NULL);
    if ((err_code == NRF_SUCCESS) || (err_code == NRF_ERROR_NOT_FOUND))
    {
        m_pm.peer_rank_initialized = true;
    }
}


ret_code_t pm_peer_rank_highest(pm_peer_id_t peer_id)
{
    VERIFY_MODULE_INITIALIZED();

    ret_code_t err_code;
    //lint -save -e65 -e64
    pm_peer_data_const_t peer_data   = {.length_words = BYTES_TO_WORDS(sizeof(m_pm.current_highest_peer_rank)),
                                        .data_id      = PM_PEER_DATA_ID_PEER_RANK,
                                        .p_peer_rank  = &m_pm.current_highest_peer_rank};
    //lint -restore


    if (!m_pm.peer_rank_initialized)
    {
        rank_init();
    }

    if (!m_pm.peer_rank_initialized || (m_pm.peer_rank_token != PM_STORE_TOKEN_INVALID))
    {
        err_code = NRF_ERROR_BUSY;
    }
    else
    {
        if ((peer_id == m_pm.highest_ranked_peer) && (m_pm.current_highest_peer_rank > 0))
        {
            pm_evt_t pm_evt;

            // The reported peer is already regarded as highest (provided it has an index at all)
            err_code = NRF_SUCCESS;

            memset(&pm_evt, 0, sizeof(pm_evt));
            pm_evt.evt_id      = PM_EVT_PEER_DATA_UPDATE_SUCCEEDED;
            pm_evt.conn_handle = im_conn_handle_get(peer_id);
            pm_evt.peer_id     = peer_id;
            pm_evt.params.peer_data_update_succeeded.data_id       = PM_PEER_DATA_ID_PEER_RANK;
            pm_evt.params.peer_data_update_succeeded.action        = PM_PEER_DATA_OP_UPDATE;
            pm_evt.params.peer_data_update_succeeded.token         = PM_STORE_TOKEN_INVALID;
            pm_evt.params.peer_data_update_succeeded.flash_changed = false;

            evt_send(&pm_evt);
        }
        else
        {
            m_pm.current_highest_peer_rank += 1;
            err_code = pdb_raw_store(peer_id, &peer_data, &m_pm.peer_rank_token);
            if (err_code != NRF_SUCCESS)
            {
                m_pm.peer_rank_token    = PM_STORE_TOKEN_INVALID;
                m_pm.current_highest_peer_rank -= 1;
                if ((err_code != NRF_ERROR_BUSY) && (err_code != NRF_ERROR_NO_MEM))
                {
                    err_code = NRF_ERROR_INTERNAL;
                }
            }
        }
    }
    return err_code;
}
