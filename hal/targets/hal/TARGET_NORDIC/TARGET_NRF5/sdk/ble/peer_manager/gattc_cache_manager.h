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



#ifndef GATTC_CACHE_MANAGER_H__
#define GATTC_CACHE_MANAGER_H__

#include <stdint.h>
#include "sdk_errors.h"
#include "nrf_ble.h"
#include "nrf_ble_gap.h"
#include "peer_manager_types.h"


/**
 * @cond NO_DOXYGEN
 * @defgroup gattc_cache_manager GATT Client Cache Manager
 * @ingroup peer_manager
 * @{
 * @brief An internal module of @ref peer_manager. A module for managing persistent storing of GATT
 *        attributes pertaining to the GATT client role of the local device.
 */


/**@brief Events that can come from the GATT Cache Manager module.
 */
typedef enum
{
    GCCM_EVT_REMOTE_DB_UPDATED,  /**< Values for the specified data has been updated in persistent storage. */
    GCCM_EVT_REMOTE_DB_STORED,   /**< New values for the specified data has been written in persistent storage. */
} gccm_evt_id_t;


typedef struct
{
    gccm_evt_id_t  evt_id;
    pm_peer_id_t   peer_id;
} gccm_evt_t;

/**@brief Event handler for events from the GATT Client Cache Manager module.
 *
 * @param[in]  event   The event that has happened.
 * @param[in]  peer    The id of the peer the event pertains to.
 * @param[in]  flags   The data the event pertains to.
 */
typedef void (*gccm_evt_handler_t)(gccm_evt_t const * p_event);


/**@brief Function for initializing the GATT Client Cache Manager module.
 *
 * @param[in]  evt_handler  Callback for events from the GATT Client Cache Manager module.
 *
 * @retval NRF_SUCCESS     Initialization was successful.
 * @retval NRF_ERROR_NULL  evt_handler was NULL.
 */
ret_code_t gccm_init(gccm_evt_handler_t evt_handler);


/**@brief Function for storing a discovered remote database persistently.
 *
 * @param[in]  peer_id      Peer to store the database for.
 * @param[in]  p_remote_db  Database values to store as an array. Can be NULL if n_services is 0.
 * @param[in]  n_services   Number of services in p_remote_db array. If 0, values are cleared.
 *
 * @retval NRF_SUCCESS              Store procedure successfully started.
 * @retval NRF_ERROR_NOT_FOUND      The peer id is invalid or unallocated.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t gccm_remote_db_store(pm_peer_id_t        peer_id,
                                ble_gatt_db_srv_t * p_remote_db,
                                uint32_t            n_services);


/**@brief Function for retrieving a persistently stored remote database.
 *
 * @param[in]    peer_id       Peer to retrieve data for.
 * @param[out]   p_remote_db  If p_n_services was large enough: Copied database values.
 * @param[inout] p_n_services In: Size of provided p_remote_db array. Out: Size of data in flash.
 *
 * @note p_n_services is always updated with the size of the data to be retrieved. The data is only
 *       copied if p_remote_db is large enough (p_n_services is large enough initially).
 *
 * @retval NRF_SUCCESS              Data retrieved successfully.
 * @retval NRF_ERROR_NOT_FOUND      The peer ID is invalid or unallocated.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t gccm_remote_db_retrieve(pm_peer_id_t        peer_id,
                                   ble_gatt_db_srv_t * p_remote_db,
                                   uint32_t          * p_n_services);


/** @}
 * @endcond
 */

#endif /* GATTC_CACHE_MANAGER_H__ */
