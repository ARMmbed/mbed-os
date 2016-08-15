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



#include "id_manager.h"

#include <string.h>
#include "nrf_soc.h"
#include "nrf_ble_gap.h"
#include "ble_conn_state.h"
#include "peer_manager_types.h"
#include "peer_database.h"
#include "nordic_common.h"
#include "sdk_common.h"

#define IM_MAX_CONN_HANDLES         8
#define IM_NO_INVALID_CONN_HANDLES  0xFF
#define MAX_REGISTRANTS             3
#define WHITELIST_MAX_COUNT         MAX(BLE_GAP_WHITELIST_ADDR_MAX_COUNT, \
                                        BLE_GAP_WHITELIST_IRK_MAX_COUNT)
#define IM_ADDR_CLEARTEXT_LENGTH    3
#define IM_ADDR_CIPHERTEXT_LENGTH   3

typedef struct
{
    pm_peer_id_t   peer_id;
    uint16_t       conn_handle;
    ble_gap_addr_t peer_address;
} im_connection_t;

typedef struct
{
    im_evt_handler_t              evt_handlers[MAX_REGISTRANTS];
    uint8_t                       n_registrants;
    im_connection_t               connections[8];
    pm_peer_id_t                  irk_whitelist_peer_ids[BLE_GAP_WHITELIST_IRK_MAX_COUNT];
    ble_gap_irk_t                 whitelist_irks[BLE_GAP_WHITELIST_IRK_MAX_COUNT];
    ble_gap_addr_t                whitelist_addrs[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
    uint8_t                       n_irk_whitelist_peer_ids;
    ble_conn_state_user_flag_id_t conn_state_user_flag_id;
} im_t;

static im_t m_im = {.n_registrants = 0};

#define MODULE_INITIALIZED (m_im.n_registrants > 0)
#include "sdk_macros.h"

static void internal_state_reset()
{
    memset(&m_im, 0, sizeof(im_t));
    m_im.n_registrants = 0;
    m_im.n_irk_whitelist_peer_ids = 0;
    m_im.conn_state_user_flag_id = BLE_CONN_STATE_USER_FLAG_INVALID;
    for (uint32_t i = 0; i < IM_MAX_CONN_HANDLES; i++)
    {
        m_im.connections[i].conn_handle = BLE_CONN_HANDLE_INVALID;
    }
}


/**@brief Function for sending an event to all registered event handlers.
 *
 * @param[in] p_event The event to distribute.
 */
static void evt_send(im_evt_t * p_event)
{
    for (uint32_t i = 0; i < m_im.n_registrants; i++)
    {
        m_im.evt_handlers[i](p_event);
    }
}

/**@brief Function finding a free position in m_im.connections.
 *
 * @detail All connection handles in the m_im.connections array are checked against the connection
 *         state module. The index of the first one that is not a connection handle for a current
 *         connection is returned. This position in the array can safely be used for a new connection.
 *
 * @return Either the index of a free position in the array or IM_NO_INVALID_CONN_HANDLES if no free
           position exists.
 */
uint8_t get_free_connection()
{
    for (uint32_t i = 0; i < IM_MAX_CONN_HANDLES; i++)
    {
        // Query the connection state module to check if the connection handle does not belong to a
        // valid connection.
        if (!ble_conn_state_user_flag_get(m_im.connections[i].conn_handle, m_im.conn_state_user_flag_id))
        {
            return i;
        }
    }
    // If all connection handles belong to a valid connection, return IM_NO_INVALID_CONN_HANDLES.
    return IM_NO_INVALID_CONN_HANDLES;
}


/**@brief Function finding a particular connection handle m_im.connections.
 *
 * @param[in]  conn_handle  The handle to find.
 *
 * @return Either the index of the conn_handle in the array or IM_NO_INVALID_CONN_HANDLES if the
 *         handle was not found.
 */
uint8_t get_connection_by_conn_handle(uint16_t conn_handle)
{
    if (ble_conn_state_user_flag_get(conn_handle, m_im.conn_state_user_flag_id))
    {
        for (uint32_t i = 0; i < IM_MAX_CONN_HANDLES; i++)
        {
            if (m_im.connections[i].conn_handle == conn_handle)
            {
                return i;
            }
        }
    }
    // If all connection handles belong to a valid connection, return IM_NO_INVALID_CONN_HANDLES.
    return IM_NO_INVALID_CONN_HANDLES;
}


/**@brief Function for registering a new connection instance.
 *
 * @param[in]  conn_handle  The handle of the new connection.
 * @param[in]  p_ble_addr   The address used to connect.
 *
 * @return Either the index of the new connection in the array or IM_NO_INVALID_CONN_HANDLES if no
 *         free position exists.
 */
uint8_t new_connection(uint16_t conn_handle, ble_gap_addr_t * p_ble_addr)
{
    uint8_t conn_index = IM_NO_INVALID_CONN_HANDLES;

    if ((p_ble_addr != NULL) && (conn_handle != BLE_CONN_HANDLE_INVALID))
    {
        ble_conn_state_user_flag_set(conn_handle, m_im.conn_state_user_flag_id, true);

        conn_index = get_connection_by_conn_handle(conn_handle);
        if (conn_index == IM_NO_INVALID_CONN_HANDLES)
        {
            conn_index = get_free_connection();
        }

        if (conn_index != IM_NO_INVALID_CONN_HANDLES)
        {
            m_im.connections[conn_index].conn_handle  = conn_handle;
            m_im.connections[conn_index].peer_id      = PM_PEER_ID_INVALID;
            m_im.connections[conn_index].peer_address = *p_ble_addr;
        }
    }
    return conn_index;
}


/**@brief Function checking the validity of an IRK
 *
 * @detail An all-zero IRK is not valid. This function will check if a given IRK is valid.
 *
 * @param[in] irk The IRK for which the validity is going to be checked.
 *
 * @retval true  The IRK is valid.
 * @retval false The IRK is invalid.
 */
bool is_valid_irk(ble_gap_irk_t const * irk)
{
    for (uint32_t i = 0; i < BLE_GAP_SEC_KEY_LEN; i++)
    {
        if (irk->irk[i] != 0)
        {
            return true;
        }
    }
    return false;
}


/**@brief Function for comparing two addresses to determine if they are identical
 *
 * @note The address type need to be identical, as well as every bit in the address itself.
 *
 * @param[in] p_addr1 The first address to be compared.
 * @param[in] p_addr2 The second address to be compared.
 *
 * @retval true  The addresses are identical.
 * @retval false The addresses are not identical.
 */
bool addr_compare(ble_gap_addr_t const * p_addr1, ble_gap_addr_t const * p_addr2)
{
    if ((p_addr1 == NULL) || (p_addr2 == NULL))
    {
        return false;
    }

    // Check that the addr type is identical, return false if it is not
    if (p_addr1->addr_type != p_addr2->addr_type)
    {
        return false;
    }
    // Check if the addr bytes are is identical
    return (memcmp(p_addr1->addr, p_addr2->addr, BLE_GAP_ADDR_LEN) == 0);
}


void im_ble_evt_handler(ble_evt_t * ble_evt)
{
    ret_code_t err_code;
    switch (ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
        {
            pm_peer_id_t bonded_matching_peer_id = PM_PEER_ID_INVALID;

            if (ble_evt->evt.gap_evt.params.connected.irk_match == 1)
            {
                // The peer was matched using a whitelist.
                bonded_matching_peer_id
                        = m_im.irk_whitelist_peer_ids[ble_evt->evt.gap_evt.params.connected.irk_match_idx];
            }
            else if (   ble_evt->evt.gap_evt.params.connected.peer_addr.addr_type
                     != BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE)
            {
                /* Search the database for bonding data matching the one that triggered the event.
                 * Public and static addresses can be matched on address alone, while resolvable
                 * random addresses can be resolved agains known IRKs. Non-resolvable random addresses
                 * are never matching because they are not longterm form of identification.
                 */
                pm_peer_id_t compared_peer_id = pdb_next_peer_id_get(PM_PEER_ID_INVALID);
                while (   (compared_peer_id        != PM_PEER_ID_INVALID)
                       && (bonded_matching_peer_id == PM_PEER_ID_INVALID))
                {
                    pm_peer_data_flash_t compared_data;
                    switch (ble_evt->evt.gap_evt.params.connected.peer_addr.addr_type)
                    {
                        case BLE_GAP_ADDR_TYPE_PUBLIC:
                            /* fall-through */
                        case BLE_GAP_ADDR_TYPE_RANDOM_STATIC:
                            err_code = pdb_read_buf_get(compared_peer_id,
                                                        PM_PEER_DATA_ID_BONDING,
                                                        &compared_data,
                                                        NULL);
                            if ((err_code == NRF_SUCCESS) &&
                                addr_compare(&ble_evt->evt.gap_evt.params.connected.peer_addr,
                                             &compared_data.p_bonding_data->peer_id.id_addr_info)
                            )
                            {
                                bonded_matching_peer_id = compared_peer_id;
                            }
                            break;

                        case BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE:
                            err_code = pdb_read_buf_get(compared_peer_id,
                                                        PM_PEER_DATA_ID_BONDING,
                                                        &compared_data,
                                                        NULL);
                            if (err_code == NRF_SUCCESS &&
                                im_address_resolve(&ble_evt->evt.gap_evt.params.connected.peer_addr,
                                                   &compared_data.p_bonding_data->peer_id.id_info)
                            )
                            {
                                bonded_matching_peer_id = compared_peer_id;
                            }
                            break;

                        case BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE:
                            // Should not happen.
                            break;

                        default:
                            break;
                    }
                    compared_peer_id = pdb_next_peer_id_get(compared_peer_id);
                }
            }
            uint8_t new_index = new_connection(ble_evt->evt.gap_evt.conn_handle, &ble_evt->evt.gap_evt.params.connected.peer_addr);
            UNUSED_VARIABLE(new_index);

            if (bonded_matching_peer_id != PM_PEER_ID_INVALID)
            {
                im_new_peer_id(ble_evt->evt.gap_evt.conn_handle, bonded_matching_peer_id);

                // Send a bonded peer event
                im_evt_t im_evt;
                im_evt.conn_handle = ble_evt->evt.gap_evt.conn_handle;
                im_evt.evt_id = IM_EVT_BONDED_PEER_CONNECTED;
                evt_send(&im_evt);
            }
        }
    }
}


/**@brief Function to compare two sets of bonding data to check if they belong to the same device.
 * @note  Invalid irks will never match even though they are identical.
 *
 * @param[in]  p_bonding_data1 First bonding data for comparison
 * @param[in]  p_bonding_data2 Second bonding data for comparison
 *
 * @return     True if the input matches, false if it does not.
 */
bool is_duplicate_bonding_data(pm_peer_data_bonding_t const * p_bonding_data1,
                               pm_peer_data_bonding_t const * p_bonding_data2)
{
    bool valid_irk = is_valid_irk(&p_bonding_data1->peer_id.id_info);
    bool duplicate_irk = valid_irk &&
        (memcmp(p_bonding_data1->peer_id.id_info.irk,
        p_bonding_data2->peer_id.id_info.irk,
        BLE_GAP_SEC_KEY_LEN) == 0
    );
    bool duplicate_addr = addr_compare(&p_bonding_data1->peer_id.id_addr_info,
                                       &p_bonding_data2->peer_id.id_addr_info
    );
    return duplicate_irk || duplicate_addr;
}


/**@brief Event handler for events from the peer_database module.
 *
 * @param[in]  p_event The event that has happend with peer id and flags.
 */
static void pdb_evt_handler(pdb_evt_t const * p_event)
{
    ret_code_t err_code;
    if ((p_event != NULL) && (p_event->evt_id == PDB_EVT_WRITE_BUF_STORED))
    {
        // If new data about peer id has been stored it is compared to other peers peer ids in
        // search of duplicates.
        if (p_event->data_id == PM_PEER_DATA_ID_BONDING)
        {
            pm_peer_data_flash_t written_data;
            err_code = pdb_read_buf_get(p_event->peer_id, PM_PEER_DATA_ID_BONDING, &written_data, NULL);
            if (err_code == NRF_SUCCESS)
            {
                pm_peer_id_t compared_peer_id = pdb_next_peer_id_get(PM_PEER_ID_INVALID);
                while (compared_peer_id != PM_PEER_ID_INVALID)
                {
                    pm_peer_data_flash_t compared_data;
                    err_code = pdb_read_buf_get(compared_peer_id,
                                                PM_PEER_DATA_ID_BONDING,
                                                &compared_data,
                                                NULL);
                    if ( err_code == NRF_SUCCESS &&
                        p_event->peer_id != compared_peer_id &&
                        is_duplicate_bonding_data(written_data.p_bonding_data,
                                                  compared_data.p_bonding_data)
                    )
                    {
                        im_evt_t im_evt;
                        im_evt.conn_handle = im_conn_handle_get(p_event->peer_id);
                        im_evt.evt_id = IM_EVT_DUPLICATE_ID;
                        im_evt.params.duplicate_id.peer_id_1 = p_event->peer_id;
                        im_evt.params.duplicate_id.peer_id_2 = compared_peer_id;
                        evt_send(&im_evt);
                    }
                    compared_peer_id = pdb_next_peer_id_get(compared_peer_id);
                }
            }
        }
    }
}


ret_code_t im_register(im_evt_handler_t evt_handler)
{
    VERIFY_PARAM_NOT_NULL(evt_handler);
    ret_code_t err_code = NRF_SUCCESS;

    if (!MODULE_INITIALIZED)
    {
        internal_state_reset();
        m_im.conn_state_user_flag_id = ble_conn_state_user_flag_acquire();
        if (m_im.conn_state_user_flag_id == BLE_CONN_STATE_USER_FLAG_INVALID)
        {
            err_code = NRF_ERROR_NO_MEM;
        }
        else
        {
            err_code = pdb_register(pdb_evt_handler);
        }
    }
    if (err_code == NRF_SUCCESS)
    {
        if ((m_im.n_registrants < MAX_REGISTRANTS))
        {
            m_im.evt_handlers[m_im.n_registrants++] = evt_handler;
        }
        else
        {
            err_code = NRF_ERROR_NO_MEM;
        }
    }
    return err_code;
}


pm_peer_id_t im_peer_id_get_by_conn_handle(uint16_t conn_handle)
{
    uint8_t conn_index = get_connection_by_conn_handle(conn_handle);

    if (MODULE_INITIALIZED && (conn_index != IM_NO_INVALID_CONN_HANDLES))
    {
        return m_im.connections[conn_index].peer_id;
    }

    return PM_PEER_ID_INVALID;
}


ret_code_t im_ble_addr_get(uint16_t conn_handle, ble_gap_addr_t * p_ble_addr)
{
    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_NULL(p_ble_addr);

    uint8_t conn_index = get_connection_by_conn_handle(conn_handle);
    if (conn_index != IM_NO_INVALID_CONN_HANDLES)
    {
        *p_ble_addr = m_im.connections[conn_index].peer_address;
        return NRF_SUCCESS;
    }

    return NRF_ERROR_NOT_FOUND;
}


bool im_master_ids_compare(ble_gap_master_id_t const * p_master_id1,
                           ble_gap_master_id_t const * p_master_id2)
{
    if(!im_master_id_is_valid(p_master_id1))
    {
        return false;
    }
    if (p_master_id1->ediv != p_master_id2->ediv)
    {
        return false;
    }
    return (memcmp(p_master_id1->rand, p_master_id2->rand, BLE_GAP_SEC_RAND_LEN) == 0);
}


pm_peer_id_t im_peer_id_get_by_master_id(ble_gap_master_id_t * p_master_id)
{
    ret_code_t err_code;
    // For each stored peer, check if the master_id match p_master_id
    pm_peer_id_t compared_peer_id = pdb_next_peer_id_get(PM_PEER_ID_INVALID);
    while (compared_peer_id != PM_PEER_ID_INVALID)
    {
        pm_peer_data_flash_t        compared_data;
        ble_gap_master_id_t const * p_compared_master_id;

        err_code = pdb_read_buf_get(compared_peer_id, PM_PEER_DATA_ID_BONDING, &compared_data, NULL);
        if (err_code == NRF_SUCCESS)
        {
            p_compared_master_id = &compared_data.p_bonding_data->own_ltk.master_id;
            if (im_master_ids_compare(p_master_id, p_compared_master_id))
            {
                // If a matching master_id is found return the peer_id
                return compared_peer_id;
            }
            p_compared_master_id = &compared_data.p_bonding_data->peer_ltk.master_id;
            if (im_master_ids_compare(p_master_id, p_compared_master_id))
            {
                // If a matching master_id is found return the peer_id
                return compared_peer_id;
            }
        }
        compared_peer_id = pdb_next_peer_id_get(compared_peer_id);
    }
    // If no matching master_id is found return the PM_PEER_ID_INVALID
    return PM_PEER_ID_INVALID;
}


pm_peer_id_t im_peer_id_get_by_irk_match_idx(uint8_t irk_match_idx)
{
    // Verify that the requested idx is within the list
    if (irk_match_idx < m_im.n_irk_whitelist_peer_ids)
    {
        // Return the peer_id from the white list
        return m_im.irk_whitelist_peer_ids[irk_match_idx];
    }
    else
    {
        // Return PM_PEER_ID_INVALID to indicate that there was no peer with the requested idx
        return PM_PEER_ID_INVALID;
    }
}


uint16_t im_conn_handle_get(pm_peer_id_t peer_id)
{
    for (uint32_t i = 0; i < IM_MAX_CONN_HANDLES; i++)
    {
        if (peer_id == m_im.connections[i].peer_id)
        {
            return m_im.connections[i].conn_handle;
        }
    }
    return BLE_CONN_HANDLE_INVALID;
}


bool im_master_id_is_valid(ble_gap_master_id_t const * p_master_id)
{
    if (p_master_id->ediv != 0)
    {
        return true;
    }
    for (uint32_t i = 0; i < BLE_GAP_SEC_RAND_LEN; i++)
    {
        if (p_master_id->rand[i] != 0)
        {
            return true;
        }
    }
    return false;
}


/**@brief Function to set the peer ID associated with a connection handle.
 *
 * @param[in]  conn_handle  The connection handle.
 * @param[in]  peer_id      The peer ID to associate with @c conn_handle.
 */
static void peer_id_set(uint16_t conn_handle, pm_peer_id_t peer_id)
{
    uint8_t conn_index = get_connection_by_conn_handle(conn_handle);
    if (conn_index != IM_NO_INVALID_CONN_HANDLES)
    {
        m_im.connections[conn_index].peer_id = peer_id;
    }
}


void im_new_peer_id(uint16_t conn_handle, pm_peer_id_t peer_id)
{
    peer_id_set(conn_handle, peer_id);
}


ret_code_t im_peer_free(pm_peer_id_t peer_id)
{
    VERIFY_MODULE_INITIALIZED();

    uint16_t   conn_handle = im_conn_handle_get(peer_id);
    ret_code_t err_code    = pdb_peer_free(peer_id);
    if ((conn_handle != BLE_CONN_HANDLE_INVALID) && (err_code == NRF_SUCCESS))
    {
        peer_id_set(conn_handle, PM_PEER_ID_INVALID);
    }
    return err_code;
}


ret_code_t im_whitelist_create(pm_peer_id_t *        p_peer_ids,
                               uint8_t               n_peer_ids,
                               ble_gap_whitelist_t * p_whitelist)
{
    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_NULL(p_whitelist);
    ret_code_t err_code;
    p_whitelist->addr_count = 0;
    p_whitelist->irk_count = 0;
    m_im.n_irk_whitelist_peer_ids = 0;
    for (uint32_t peer_index = 0; peer_index < n_peer_ids; peer_index++)
    {
        uint16_t conn_handle = im_conn_handle_get(p_peer_ids[peer_index]);
        if (ble_conn_state_status(conn_handle) != BLE_CONN_STATUS_CONNECTED)
        {
            pm_peer_data_flash_t peer_data;
            err_code = pdb_read_buf_get(p_peer_ids[peer_index], PM_PEER_DATA_ID_BONDING, &peer_data, NULL);
            if (err_code == NRF_ERROR_INVALID_PARAM || err_code == NRF_ERROR_NOT_FOUND)
            {
                return NRF_ERROR_INVALID_PARAM;
            }
            if (p_whitelist->pp_addrs != NULL &&
                peer_data.p_bonding_data->peer_id.id_addr_info.addr_type
                        != BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE &&
                peer_data.p_bonding_data->peer_id.id_addr_info.addr_type
                        != BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE
            )
            {
                memcpy(m_im.whitelist_addrs[peer_index].addr,
                    peer_data.p_bonding_data->peer_id.id_addr_info.addr,
                    BLE_GAP_ADDR_LEN
                );
                m_im.whitelist_addrs[peer_index].addr_type =
                    peer_data.p_bonding_data->peer_id.id_addr_info.addr_type;
                p_whitelist->pp_addrs[peer_index] = &m_im.whitelist_addrs[peer_index];
                p_whitelist->addr_count++;
            }
            if (p_whitelist->pp_irks != NULL &&
                is_valid_irk(&(peer_data.p_bonding_data->peer_id.id_info))
            )
            {
                memcpy(m_im.whitelist_irks[peer_index].irk,
                    peer_data.p_bonding_data->peer_id.id_info.irk,
                    BLE_GAP_SEC_KEY_LEN
                );
                p_whitelist->pp_irks[peer_index] = &m_im.whitelist_irks[peer_index];
                p_whitelist->irk_count++;
                m_im.irk_whitelist_peer_ids[peer_index] = p_peer_ids[peer_index];
                m_im.n_irk_whitelist_peer_ids++;
            }
        }
    }
    return NRF_SUCCESS;
}


ret_code_t im_whitelist_custom(ble_gap_whitelist_t const * p_whitelist)
{
    ret_code_t err_code;

    pm_peer_id_t new_irk_whitelist_peer_ids[BLE_GAP_WHITELIST_IRK_MAX_COUNT];
    uint32_t n_new_irk_whitelist_peer_ids = 0;
    VERIFY_PARAM_NOT_NULL(p_whitelist);
    for (uint32_t i = 0; i < BLE_GAP_WHITELIST_IRK_MAX_COUNT; i++)
    {
        new_irk_whitelist_peer_ids[i] = PM_PEER_ID_INVALID;
    }
    pm_peer_id_t compared_peer_id = pdb_next_peer_id_get(PM_PEER_ID_INVALID);
    while (compared_peer_id != PM_PEER_ID_INVALID)
    {
        pm_peer_data_flash_t compared_data;
        err_code = pdb_read_buf_get(compared_peer_id, PM_PEER_DATA_ID_BONDING, &compared_data, NULL);
        if (err_code == NRF_SUCCESS)
        {
            for (uint32_t i = 0; i < p_whitelist->irk_count; i++)
            {
                bool valid_irk = is_valid_irk(&compared_data.p_bonding_data->peer_id.id_info);
                bool duplicate_irk = valid_irk &&
                    (memcmp(p_whitelist->pp_irks[i]->irk,
                    compared_data.p_bonding_data->peer_id.id_info.irk,
                    BLE_GAP_SEC_KEY_LEN) == 0
                );
                if (duplicate_irk)
                {
                    new_irk_whitelist_peer_ids[i] = compared_peer_id;
                    n_new_irk_whitelist_peer_ids++;
                }
            }
        }
        compared_peer_id = pdb_next_peer_id_get(compared_peer_id);
    }
    if (n_new_irk_whitelist_peer_ids != p_whitelist->irk_count)
    {
        return NRF_ERROR_NOT_FOUND;
    }
    else
    {
        for (uint32_t i = 0; i < n_new_irk_whitelist_peer_ids; i++)
        {
            m_im.irk_whitelist_peer_ids[i] = new_irk_whitelist_peer_ids[i];
        }
        m_im.n_irk_whitelist_peer_ids = n_new_irk_whitelist_peer_ids;
        return NRF_SUCCESS;
    }
}


/**@brief Function for calculating the ah() hash function described in Bluetooth core specification
 *        4.2 section 3.H.2.2.2.
 *
 * @detail  BLE uses a hash function to calculate the first half of a resolvable address
 *          from the second half of the address and an irk. This function will use the ECB
 *          periferal to hash these data acording to the Bluetooth core specification.
 *
 * @note The ECB expect little endian input and output.
 *       This function expect big endian and will reverse the data as necessary.
 *
 * @param[in]  p_k          The key used in the hash function.
 *                          For address resolution this is should be the irk.
 *                          The array must have a length of 16.
 * @param[in]  p_r          The rand used in the hash function. For generating a new address
 *                          this would be a random number. For resolving a resolvable address
 *                          this would be the last half of the address being resolved.
 *                          The array must have a length of 3.
 * @param[out] p_local_hash The result of the hash operation. For address resolution this
 *                          will match the first half of the address being resolved if and only
 *                          if the irk used in the hash function is the same one used to generate
 *                          the address.
 *                          The array must have a length of 16.
 */
void ah(uint8_t const * p_k, uint8_t const * p_r, uint8_t * p_local_hash)
{
    ret_code_t err_code;
    nrf_ecb_hal_data_t ecb_hal_data;
    for (uint32_t i = 0; i < SOC_ECB_KEY_LENGTH; i++)
    {
        ecb_hal_data.key[i] = p_k[SOC_ECB_KEY_LENGTH - 1 - i];
    }
    memset(ecb_hal_data.cleartext, 0, SOC_ECB_KEY_LENGTH - IM_ADDR_CLEARTEXT_LENGTH);

    for (uint32_t i = 0; i < IM_ADDR_CLEARTEXT_LENGTH; i++)
    {
        ecb_hal_data.cleartext[SOC_ECB_KEY_LENGTH - 1 - i] = p_r[i];
    }

    err_code = sd_ecb_block_encrypt(&ecb_hal_data); // Can only return NRF_SUCCESS.
    UNUSED_VARIABLE(err_code);

    for (uint32_t i = 0; i < IM_ADDR_CIPHERTEXT_LENGTH; i++)
    {
        p_local_hash[i] = ecb_hal_data.ciphertext[SOC_ECB_KEY_LENGTH - 1 - i];
    }
}


bool im_address_resolve(ble_gap_addr_t const * p_addr, ble_gap_irk_t const * p_irk)
{
    if (p_addr->addr_type != BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE)
    {
        return false;
    }
    uint8_t hash[IM_ADDR_CIPHERTEXT_LENGTH];
    uint8_t local_hash[IM_ADDR_CIPHERTEXT_LENGTH];
    uint8_t prand[IM_ADDR_CLEARTEXT_LENGTH];
    memcpy(hash, p_addr->addr, IM_ADDR_CIPHERTEXT_LENGTH);
    memcpy(prand, &p_addr->addr[IM_ADDR_CIPHERTEXT_LENGTH], IM_ADDR_CLEARTEXT_LENGTH);
    ah(p_irk->irk, prand, local_hash);

    return (memcmp(hash, local_hash, IM_ADDR_CIPHERTEXT_LENGTH) == 0);
}
