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



#include "gattc_cache_manager.h"

#include "nrf_ble_gap.h"
#include "ble_conn_state.h"
#include "peer_manager_types.h"
#include "peer_database.h"
#include "id_manager.h"
#include "sdk_common.h"

#define MAX_SIMUL_SEC_PROCEDURES 2

typedef struct
{
    gccm_evt_handler_t  evt_handler;
} gccm_t;

static gccm_t m_gccm;

#define MODULE_INITIALIZED (m_gccm.evt_handler != NULL)
#include "sdk_macros.h"

static void internal_state_reset(gccm_t * gccm)
{
    memset(gccm, 0, sizeof(gccm_t));
}


/**@brief Event handler for events from the peer_database module.
 *
 * @param[in]  p_event The event that has happend with peer id and flags.
 */
static void pdb_evt_handler(pdb_evt_t const * p_event)
{
    gccm_evt_t gccm_evt;
    gccm_evt.evt_id  = GCCM_EVT_REMOTE_DB_STORED;
    gccm_evt.peer_id = p_event->peer_id;
    m_gccm.evt_handler(&gccm_evt);
}


ret_code_t gccm_init(gccm_evt_handler_t evt_handler)
{
    ret_code_t err_code;
    if (evt_handler == NULL)
    {
        err_code = NRF_ERROR_NULL;
    }
    else
    {
        err_code = pdb_register(pdb_evt_handler);
        if (err_code == NRF_SUCCESS)
        {
            internal_state_reset(&m_gccm);
            m_gccm.evt_handler = evt_handler;
        }
    }
    return err_code;
}


ret_code_t gccm_remote_db_store(pm_peer_id_t        peer_id,
                                ble_gatt_db_srv_t * p_remote_db,
                                uint32_t            n_services)
{
    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_NULL(p_remote_db);

    // Initialize the peer_data
    pm_peer_data_const_t peer_data;
    memset(&peer_data, 0, sizeof(peer_data));
    peer_data.data_id = PM_PEER_DATA_ID_GATT_REMOTE;
    peer_data.p_remote_gatt_db = p_remote_db;
    peer_data.length_words = PM_REMOTE_DB_N_WORDS(n_services);

    return pdb_raw_store(peer_id, &peer_data, NULL);
}


ret_code_t gccm_remote_db_retrieve(pm_peer_id_t        peer_id,
                                   ble_gatt_db_srv_t * p_remote_db,
                                   uint32_t          * p_n_services)
{
    VERIFY_MODULE_INITIALIZED();
    VERIFY_PARAM_NOT_NULL(p_remote_db);
    VERIFY_PARAM_NOT_NULL(p_n_services);

    // Initialize the peer_data
    pm_peer_data_t peer_data;
    memset(&peer_data, 0, sizeof(peer_data));
    peer_data.data_id = PM_PEER_DATA_ID_GATT_REMOTE;
    peer_data.p_remote_gatt_db = p_remote_db;
    peer_data.length_words = PM_REMOTE_DB_N_WORDS(*p_n_services);

    ret_code_t err_code = pdb_raw_read(peer_id, PM_PEER_DATA_ID_GATT_REMOTE, &peer_data);

    *p_n_services = PM_REMOTE_DB_N_SERVICES(peer_data.length_words);

    return err_code;
}
