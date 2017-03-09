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



#ifndef PEER_ID_MANAGER_H__
#define PEER_ID_MANAGER_H__

#include <stdint.h>
#include "sdk_errors.h"
#include "nrf_ble.h"
#include "nrf_ble_gap.h"
#include "peer_manager_types.h"


/**
 * @cond NO_DOXYGEN
 * @defgroup id_manager ID Manager
 * @ingroup peer_manager
 * @{
 * @brief An internal module of @ref peer_manager. A module for keeping track of peer identities
 *       (IRK and peer address).
 */


/**@brief Events that can come from the ID Manager module.
 */
typedef enum
{
    IM_EVT_DUPLICATE_ID,          /**< The ID Manager module has detected that two stored peers represent the same peer. */
    IM_EVT_BONDED_PEER_CONNECTED, /**< A connected peer has been identified as one of the bonded peers. This can happen immediately on connection, or at a later time. */
} im_evt_id_t;


typedef struct
{
    im_evt_id_t evt_id;
    uint16_t    conn_handle;
    union
    {
        struct
        {
            pm_peer_id_t peer_id_1;
            pm_peer_id_t peer_id_2;
        } duplicate_id;
    } params;
} im_evt_t;


/**@brief Event handler for events from the ID Manager module.
 *
 * @param[in]  p_event   The event that has happened.
 */
typedef void (*im_evt_handler_t)(im_evt_t const * p_event);

/**@brief Function for registering for events from the ID Manager module.
 *
 * @note This will also initialize the module if needed.
 *
 * @param[in]  evt_handler  Callback for events from the ID Manager module.
 *
 * @retval NRF_SUCCESS       Registration was successful.
 * @retval NRF_ERROR_NO_MEM  No more registrations possible.
 * @retval NRF_ERROR_NULL    evt_handler was NULL.
 */
ret_code_t im_register(im_evt_handler_t evt_handler);


/**@brief Function for dispatching SoftDevice events to the ID Manager module.
 *
 * @param[in]  p_ble_evt  The SoftDevice event.
 */
void im_ble_evt_handler(ble_evt_t * p_ble_evt);


/**@brief Function for getting the corresponding peer ID from a connection handle.
 *
 * @param[in]  conn_handle  The connection handle.
 *
 * @return The corresponding peer ID, or @ref PM_PEER_ID_INVALID if none could be resolved.
 */
pm_peer_id_t im_peer_id_get_by_conn_handle(uint16_t conn_handle);


/**@brief Function for getting the corresponding peer ID from a master ID (EDIV and rand).
 *
 * @param[in]  p_master_id  The master ID.
 *
 * @return The corresponding peer ID, or @ref PM_PEER_ID_INVALID if none could be resolved.
 */
pm_peer_id_t im_peer_id_get_by_master_id(ble_gap_master_id_t * p_master_id);


/**@brief Function for getting the corresponding peer ID from an IRK match index, see @ref
 *        ble_gap_evt_connected_t.
 *
 * @param[in]  irk_match_idx  The IRK match index.
 *
 * @return The corresponding peer ID, or @ref PM_PEER_ID_INVALID if none could be resolved.
 */
pm_peer_id_t im_peer_id_get_by_irk_match_idx(uint8_t irk_match_idx);


/**@brief Function for getting the corresponding connection handle from a peer ID.
 *
 * @param[in] peer_id  The peer ID.
 *
 * @return The corresponding connection handle, or @ref BLE_CONN_HANDLE_INVALID if none could be
 *         resolved.
 */
uint16_t im_conn_handle_get(pm_peer_id_t peer_id);


/**@brief Function for comparing two master ids
 * @note  Two invalid master IDs will not match.
 *
 * @param[in]  p_master_id1 First master id for comparison
 * @param[in]  p_master_id2 Second master id for comparison
 *
 * @return     True if the input matches, false if it does not.
 */
bool im_master_ids_compare(ble_gap_master_id_t const * p_master_id1,
                           ble_gap_master_id_t const * p_master_id2);


/**@brief Function for getting the BLE address used by the peer when connecting.
 *
 * @param[in]  conn_handle  The connection handle.
 * @param[out] p_ble_addr   The BLE address used by the peer when the connection specified by
 *                          conn_handle was established.
 *
 * @retval NRF_SUCCESS                   The address was found and copied.
 * @retval NRF_ERROR_INVALID_STATE       Module not initialized.
 * @retval BLE_ERROR_CONN_HANDLE_INVALID conn_handle does not refer to an active connection.
 * @retval NRF_ERROR_NULL                p_ble_addr was NULL.
 */
ret_code_t im_ble_addr_get(uint16_t conn_handle, ble_gap_addr_t * p_ble_addr);


/**@brief Function for checking whether a master ID is valid or invalid
 *
 * @param[in]  p_master_id  The master ID.
 *
 * @retval true   The master id is valid.
 * @retval true   The master id is invalid (i.e. all zeros).
 */
bool im_master_id_is_valid(ble_gap_master_id_t const * p_master_id);


/**@brief Function for reporting that a new peer ID has been allocated for a specified connection.
 *
 * @param[in]  conn_handle  The connection.
 * @param[in]  peer_id      The new peer ID.
 */
void im_new_peer_id(uint16_t conn_handle, pm_peer_id_t peer_id);


/**@brief Function for deleting all of a peer's data from flash and disassociating it from any
 *        connection handles it is associated with.
 *
 * @param[in]  peer_id  The peer to free.
 *
 * @return Any error code returned by @ref pdb_peer_free.
 */
ret_code_t im_peer_free(pm_peer_id_t peer_id);


/**@brief Function for informing this module of what whitelist will be used.
 *
 * @details This function is meant to be used when the app wants to use a custom whitelist.
 *          When using peer manager, this function must be used if a custom whitelist is used.
 *          Only whitelisted IRKs are of any importance for this function.
 *
 * @note When using a whitelist, always use the whitelist created/set by the most recent
 *       call to @ref im_whitelist_create or to this function, whichever happened most recently.
 * @note Do not call this function while scanning with another whitelist.
 * @note Do not add any irks to the whitelist that are not present in the bonding data of a peer in
 *       the peer database.
 * @note The whitelist is not altered in any way by calling this function. Only the internal state
 *       of the module is changed.
 *
 * @param[in] p_whitelist  The whitelist.
 *
 * @retval NRF_SUCCESS         Whitelist successfully set.
 * @retval NRF_ERROR_NULL      p_whitelist was NULL.
 * @retval NRF_ERROR_NOT_FOUND One or more of the whitelists irks was not found in the peer_database.
 */
ret_code_t im_whitelist_custom(ble_gap_whitelist_t const * p_whitelist);


/**
 * @brief Function for constructing a whitelist for use when advertising.
 *
 * @note When advertising with whitelist, always use the whitelist created/set by the most recent
 *       call to this function or to @ref im_whitelist_custom, whichever happened most recently.
 * @note Do not call this function while advertising with another whitelist.
 *
 * @param[in]     p_peer_ids   The ids of the peers to be added to the whitelist.
 * @param[in]     n_peer_ids   The number of peer ids in p_peer_ids.
 * @param[in,out] p_whitelist  The constructed whitelist. Note that p_adv_whitelist->pp_addrs
 *                             must be NULL or point to an array with size @ref
 *                             BLE_GAP_WHITELIST_ADDR_MAX_COUNT and p_adv_whitelist->pp_irks
 *                             must be NULL or point to an array with size @ref
 *                             BLE_GAP_WHITELIST_IRK_MAX_COUNT.
 *
 * @retval NRF_SUCCESS     Whitelist successfully created.
 * @retval NRF_ERROR_NULL  p_whitelist was NULL.
 */
ret_code_t im_whitelist_create(pm_peer_id_t        * p_peer_ids,
                               uint8_t               n_peer_ids,
                               ble_gap_whitelist_t * p_whitelist);

/**
 * @brief Function for resolving a resolvable address with an identity resolution key (IRK).
 *
 * @details This function will use the ECB peripheral to resolve a resolvable address.
 *          This can be used to resolve the identity of a device distributing a random
 *          resolvable address based on any IRKs you have received earlier. If an address is
 *          resolved by an IRK, the device disributing the address must also know the IRK.
 *
 * @param[in] p_addr  A random resolvable address.
 * @param[in] p_irk   An identity resolution key (IRK).
 *
 * @retval true   The irk used matched the one used to create the address.
 * @retval false  The irk used did not match the one used to create the address, or an argument was
 *                NULL.
 */
bool im_address_resolve(ble_gap_addr_t const * p_addr, ble_gap_irk_t const * p_irk);

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
 *
 * @note    ====IMPORTANT====
 *          This is a special modification to the original nRF5x SDK required by the mbed BLE API
 *          to be able to generate BLE private resolvable addresses. This function is used by
 *          the BLE API implementation for nRF5xSecurityManager::getAddressFromBondTable() in the
 *          ble-nrf52832 yotta module.
 *          =================
 */
void ah(uint8_t const * p_k, uint8_t const * p_r, uint8_t * p_local_hash);  

/** @}
 * @endcond
 */

#endif /* PEER_ID_MANAGER_H__ */
