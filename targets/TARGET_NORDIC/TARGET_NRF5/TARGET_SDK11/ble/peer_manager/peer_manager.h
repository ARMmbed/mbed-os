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



/**
 * @file peer_manager.h
 *
 * @defgroup peer_manager Peer Manager
 * @ingroup ble_sdk_lib
 * @{
 * @brief Module for managing BLE bonding, which includes controlling encryption and pairing
 *        procedures as well as persistently storing different pieces of data that must be stored
 *        when bonded.
 *
 * @details The API consists of functions for configuring the pairing and encryption behavior of the
 *          device and functions for manipulating the stored data.
 */


#ifndef PEER_MANAGER_H__
#define PEER_MANAGER_H__

#include <stdint.h>
#include <stdbool.h>
#include "sdk_common.h"
#include "nrf_ble.h"
#include "nrf_ble_gap.h"
#include "peer_manager_types.h"
#include "peer_database.h"



/**@brief Security status of a connection.
 */
typedef struct
{
    uint8_t connected      : 1; /**< @brief The connection is active (not disconnected). */
    uint8_t encrypted      : 1; /**< @brief Communication on this link is encrypted. */
    uint8_t mitm_protected : 1; /**< @brief The encrypted communication is also protected against man-in-the-middle attacks. */
    uint8_t bonded         : 1; /**< @brief The peer is bonded with us. */
} pm_conn_sec_status_t;


/**@brief Types of events that can come from the @ref peer_manager module.
 */
typedef enum
{
    PM_EVT_BONDED_PEER_CONNECTED,           /**< @brief A connected peer has been identified as one with which we have a bond. When performing bonding with a peer for the first time, this event will not be sent until a new connection is established with the peer. When we are central, this event is always sent when the Peer Manager receives the @ref BLE_GAP_EVT_CONNECTED event. When we are peripheral, this event might in rare cases arrive later. */
    PM_EVT_CONN_SEC_START,                  /**< @brief A security procedure has started on a link, initiated either locally or remotely. The security procedure is using the last parameters provided via @ref pm_sec_params_set. This event is always followed by either a @ref PM_EVT_CONN_SEC_SUCCEEDED or a @ref PM_EVT_CONN_SEC_FAILED event. This is an informational event; no action is needed for the procedure to proceed. */
    PM_EVT_CONN_SEC_SUCCEEDED,              /**< @brief A link has been encrypted, either as a result of a call to @ref pm_conn_secure or a result of an action by the peer. The event structure contains more information about the circumstances. This event might contain a peer ID with the value @ref PM_PEER_ID_INVALID, which means that the peer (central) used an address that could not be identified, but it used an encryption key (LTK) that is present in the database. */
    PM_EVT_CONN_SEC_FAILED,                 /**< @brief A pairing or encryption procedure has failed. In some cases, this means that security is not possible on this link (temporarily or permanently). How to handle this error depends on the application. */
    PM_EVT_CONN_SEC_CONFIG_REQ,             /**< @brief The peer (central) has requested pairing, but a bond already exists with that peer. Reply by calling @ref pm_conn_sec_config_reply before the event handler returns. If no reply is sent, a default is used. */
    PM_EVT_STORAGE_FULL,                    /**< @brief There is no more room for peer data in flash storage. To solve this problem, delete data that is not needed anymore and run a garbage collection procedure in FDS. */
    PM_EVT_ERROR_UNEXPECTED,                /**< @brief An unrecoverable error happened inside Peer Manager. An operation failed with the provided error. */
    PM_EVT_PEER_DATA_UPDATE_SUCCEEDED,      /**< @brief A piece of peer data was stored, updated, or cleared in flash storage. This event is sent for all successful changes to peer data, also those initiated internally in Peer Manager. To identify an operation, compare the store token in the event with the store token received during the initiating function call. Events from internally initiated changes might have invalid store tokens. */
    PM_EVT_PEER_DATA_UPDATE_FAILED,         /**< @brief A piece of peer data could not be stored, updated, or cleared in flash storage. This event is sent instead of @ref PM_EVT_PEER_DATA_UPDATE_SUCCEEDED for the failed operation. */
    PM_EVT_PEER_DELETE_SUCCEEDED,           /**< @brief A peer was cleared from flash storage, for example because a call to @ref pm_peer_delete succeeded. This event can also be sent as part of a call to @ref pm_peers_delete or internal cleanup. */
    PM_EVT_PEER_DELETE_FAILED,              /**< @brief A peer could not be cleared from flash storage. This event is sent instead of @ref PM_EVT_PEER_DELETE_SUCCEEDED for the failed operation. */
    PM_EVT_PEERS_DELETE_SUCCEEDED,          /**< @brief A call to @ref pm_peers_delete has completed successfully. Flash storage now contains no peer data. */
    PM_EVT_PEERS_DELETE_FAILED,             /**< @brief A call to @ref pm_peers_delete has failed, which means that at least one of the peers could not be deleted. Other peers might have been deleted, or might still be queued to be deleted. No more @ref PM_EVT_PEERS_DELETE_SUCCEEDED or @ref PM_EVT_PEERS_DELETE_FAILED events are sent until the next time @ref pm_peers_delete is called. */
    PM_EVT_LOCAL_DB_CACHE_APPLIED,          /**< @brief Local database values for a peer (taken from flash storage) have been provided to the SoftDevice. */
    PM_EVT_LOCAL_DB_CACHE_APPLY_FAILED,     /**< @brief Local database values for a peer (taken from flash storage) were rejected by the SoftDevice, which means that either the database has changed or the user has manually set the local database to an invalid value (using @ref pm_peer_data_store). */
    PM_EVT_SERVICE_CHANGED_IND_SENT,        /**< @brief A service changed indication has been sent to a peer, as a result of a call to @ref pm_local_database_has_changed. This event will be followed by a @ref PM_EVT_SERVICE_CHANGED_IND_CONFIRMED event if the peer acknowledges the indication. */
    PM_EVT_SERVICE_CHANGED_IND_CONFIRMED,   /**< @brief A service changed indication that was sent has been confirmed by a peer. The peer can now be considered aware that the local database has changed. */
} pm_evt_id_t;


/**@brief Parameters specific to the @ref PM_EVT_CONN_SEC_SUCCEEDED event.
 */
typedef struct
{
    pm_conn_sec_procedure_t procedure; /**< @brief The procedure that led to securing the link. */
} pm_conn_secured_evt_t;


/**@brief Parameters specific to the @ref PM_EVT_CONN_SEC_FAILED event.
 */
typedef struct
{
    pm_conn_sec_procedure_t  procedure; /**< @brief The procedure that failed. */
    pm_sec_error_code_t error;          /**< @brief An error code that describes the failure. */
    uint8_t             error_src;      /**< @brief The party that raised the error, see @ref BLE_GAP_SEC_STATUS_SOURCES. */
} pm_conn_secure_failed_evt_t;


/**@brief Actions that can be performed to peer data in persistent storage.
 */
typedef enum
{
    PM_PEER_DATA_OP_UPDATE, /**< @brief Writing or overwriting the data. */
    PM_PEER_DATA_OP_DELETE, /**< @brief Removing the data. */
} pm_peer_data_op_t;


/**@brief Parameters specific to the @ref PM_EVT_PEER_DATA_UPDATE_SUCCEEDED event.
 */
typedef struct
{
    pm_peer_data_id_t data_id;           /**< @brief The type of the data that was changed. */
    pm_peer_data_op_t action;            /**< @brief What happened to the data. */
    uint8_t           flash_changed : 1; /**< @brief If this is false, no operation was done in flash, because the value was already what it should be. Please note that in certain scenarios, this flag will be true even if the new value is the same as the old. */
    pm_store_token_t  token;             /**< @brief Token that identifies the operation. For @ref PM_PEER_DATA_OP_DELETE actions, this token can be disregarded. For @ref PM_PEER_DATA_OP_UPDATE actions, compare this token with the token that is received from a call to a @ref PM_PEER_DATA_FUNCTIONS function. */
} pm_peer_data_update_succeeded_evt_t;


/**@brief Parameters specific to the @ref PM_EVT_PEER_DATA_UPDATE_FAILED event.
 */
typedef struct
{
    pm_peer_data_id_t data_id; /**< @brief The type of the data that was supposed to be changed. */
    pm_peer_data_op_t action;  /**< @brief The action that failed. */
    pm_store_token_t  token;   /**< @brief Token that identifies the operation. For @ref PM_PEER_DATA_OP_DELETE actions, this token can be disregarded. For @ref PM_PEER_DATA_OP_UPDATE actions, compare this token with the token that is received from a call to a @ref PM_PEER_DATA_FUNCTIONS function. */
    ret_code_t        error;   /**< @brief An error code that describes the failure. */
} pm_peer_data_update_failed_t;


/**@brief Standard parameters for failure events.
 */
typedef struct
{
    ret_code_t error; /**< @brief The error that occurred. */
} pm_failure_evt_t;


/**@brief An event from the @ref peer_manager module.
 *
 * @details The structure contains both standard parameters and parameters that are specific to some events.
 */
typedef struct
{
    pm_evt_id_t  evt_id;      /**< @brief The type of the event. */
    uint16_t     conn_handle; /**< @brief The connection that this event pertains to, or @ref BLE_CONN_HANDLE_INVALID. */
    pm_peer_id_t peer_id;     /**< @brief The bonded peer that this event pertains to, or @ref PM_PEER_ID_INVALID. */
    union
    {
        pm_conn_secured_evt_t               conn_sec_succeeded;         /**< @brief Parameters specific to the @ref PM_EVT_CONN_SEC_SUCCEEDED event. */
        pm_conn_secure_failed_evt_t         conn_sec_failed;            /**< @brief Parameters specific to the @ref PM_EVT_CONN_SEC_FAILED event. */
        pm_peer_data_update_succeeded_evt_t peer_data_update_succeeded; /**< @brief Parameters specific to the @ref PM_EVT_PEER_DATA_UPDATE_SUCCEEDED event. */
        pm_peer_data_update_failed_t        peer_data_update_failed;    /**< @brief Parameters specific to the @ref PM_EVT_PEER_DATA_UPDATE_FAILED event. */
        pm_failure_evt_t                    peer_delete_failed;         /**< @brief Parameters specific to the @ref PM_EVT_PEER_DELETE_FAILED event. */
        pm_failure_evt_t                    peers_delete_failed_evt;    /**< @brief Parameters specific to the @ref PM_EVT_PEERS_DELETE_FAILED event. */
        pm_failure_evt_t                    error_unexpected;           /**< @brief Parameters specific to the @ref PM_EVT_PEER_DELETE_FAILED event. */
    } params;
} pm_evt_t;


/**@brief Event handler for events from the @ref peer_manager module.
 *
 * @sa pm_register
 *
 * @param[in]  p_event  The event that has occurred.
 */
typedef void (*pm_evt_handler_t)(pm_evt_t const * p_event);


/**@brief Function for initializing the Peer Manager.
 *
 * @details You must initialize the Peer Manager before you can call any other Peer Manager
 *          functions.
 *
 * @retval NRF_SUCCESS              If initialization was successful.
 * @retval NRF_ERROR_INTERNAL       If another error occurred.
 */
ret_code_t pm_init(void);


/**@brief Function for registering an event handler with the Peer Manager.
 *
 * @param[in] event_handler  Callback for events from the @ref peer_manager module. @p event_handler
 *                           is called for every event that the Peer Manager sends after this
 *                           function is called.
 *
 * @retval NRF_SUCCESS              If initialization was successful.
 * @retval NRF_ERROR_NULL           If @p event_handler was NULL.
 * @retval NRF_ERROR_INVALID_STATE  If the Peer Manager is not initialized.
 */
ret_code_t pm_register(pm_evt_handler_t event_handler);


/**@brief Function for providing pairing and bonding parameters to use for pairing procedures.
 *
 * @details Until this function is called, all bonding procedures that are initiated by the
 *          peer are rejected.
 *
 *          This function can be called multiple times with different parameters, even with NULL as
 *          @p p_sec_params, in which case the Peer Manager starts rejecting all procedures again.
 *
 * @param[in]  p_sec_params  Security parameters to be used for subsequent security procedures.
 *
 * @retval NRF_SUCCESS              If the parameters were set successfully.
 * @retval NRF_ERROR_INVALID_PARAM  If the combination of parameters is invalid.
 * @retval NRF_ERROR_INVALID_STATE  If the Peer Manager is not initialized.
 * @retval NRF_ERROR_INTERNAL       If another error occurred.
 */
ret_code_t pm_sec_params_set(ble_gap_sec_params_t * p_sec_params);


/**@brief Function for passing BLE events to the Peer Manager.
 *
 * @details For the module to work as expected, this function must be called with each BLE event
 *          from the SoftDevice. It must be called after @ref ble_conn_state_on_ble_evt, but before
 *          the application processes the event.
 *
 *          Calling this function before @ref pm_init is safe, but without effect.
 *
 * @param[in]  p_ble_evt  BLE stack event that is dispatched to the function.
 */
void pm_on_ble_evt(ble_evt_t * p_ble_evt);


/**@brief Function for establishing encryption on a connection, and optionally establishing a bond.
 *
 * @details This function attempts to secure the link that is specified by @p conn_handle. It uses
 *          the parameters that were previously provided in a call to @ref pm_sec_params_set.
 *
 *          If the connection is a master connection, calling this function starts a security
 *          procedure on the link. If we have keys from a previous bonding procedure with this peer
 *          and the keys meet the security requirements in the currently active sec_params, the
 *          function attempts to establish encryption with the existing keys. If no key exists, the
 *          function attempts to pair and bond according to the currently active sec_params.
 *
 *          If the function completes successfully, a @ref PM_EVT_CONN_SEC_START event is sent.
 *          The procedure might be queued, in which case the @ref PM_EVT_CONN_SEC_START event is
 *          delayed until the procedure is initiated in the SoftDevice.
 *
 *          If the connection is a slave connection, the function sends a security request to
 *          the peer (master). It is up to the peer then to initiate pairing or encryption.
 *          If the peer ignores the request, a @ref BLE_GAP_EVT_TIMEOUT event occurs
 *          with the source @ref BLE_GAP_TIMEOUT_SRC_SECURITY_REQUEST. Otherwise, the peer initiates
 *          security, in which case things happen as if the peer had initiated security itself.
 *          See @ref PM_EVT_CONN_SEC_START for information about peer-initiated security.
 *
 * @param[in]  conn_handle      Connection handle of the link as provided by the SoftDevice.
 * @param[in]  force_repairing  Whether to force a pairing procedure even if there is an existing
 *                              encryption key. This argument is relevant only for
 *                              the central role. Recommended value: false.
 *
 * @retval NRF_SUCCESS                    If the operation completed successfully.
 * @retval NRF_ERROR_TIMEOUT              If there was an SMP time-out, so that no more SMP
 *                                        operations can be performed on this link.
 * @retval BLE_ERROR_INVALID_CONN_HANDLE  If the connection handle is invalid.
 * @retval NRF_ERROR_NOT_FOUND            If the security parameters have not been set.
 * @retval NRF_ERROR_NO_MEM               If there is no more space in flash.
 * @retval NRF_ERROR_INVALID_STATE        If the Peer Manager is not initialized, or the peer is
 *                                        disconnected or in the process of disconnecting.
 * @retval NRF_ERROR_INTERNAL             If another error occurred.
 */
ret_code_t pm_conn_secure(uint16_t conn_handle, bool force_repairing);


/**@brief Function for providing security configuration for a link.
 *
 * @details This function is optional, and must be called in reply to a @ref
 *          PM_EVT_CONN_SEC_CONFIG_REQ event, before the Peer Manager event handler returns. If it
 *          is not called in time, a default configuration is used. See @ref pm_conn_sec_config_t
 *          for the value of the default.
 *
 * @param[in]  conn_handle        The connection to set the configuration for.
 * @param[in]  p_conn_sec_config  The configuration.
 */
void pm_conn_sec_config_reply(uint16_t conn_handle, pm_conn_sec_config_t * p_conn_sec_config);


/**@brief Function for manually informing that the local database has changed.
 *
 * @details This function sends a service changed indication to all bonded and/or connected peers
 *          that subscribe to this indication. If a bonded peer is not connected, the indication is
 *          sent when it reconnects. Every time an indication is sent, a @ref
 *          PM_EVT_SERVICE_CHANGED_IND_SENT event occurs, followed by a @ref
 *          PM_EVT_SERVICE_CHANGED_IND_CONFIRMED when the peer sends its confirmation. Peers that
 *          are not subscribed to the service changed indication when this function is called do not
 *          receive an indication, and no events are sent to the user. Likewise, if the service
 *          changed characteristic is not present in the local database, this no indications are
 *          sent peers, and no events are sent to the user.
 */
void pm_local_database_has_changed(void);


/**@brief Function for getting the security status of a connection.
 *
 * @param[in]  conn_handle    Connection handle of the link as provided by the SoftDevice.
 * @param[out] p_conn_sec_status  Security status of the link.
 *
 * @retval NRF_SUCCESS                    If pairing was initiated successfully.
 * @retval BLE_ERROR_INVALID_CONN_HANDLE  If the connection handle is invalid.
 * @retval NRF_ERROR_NULL                 If @p p_conn_sec_status was NULL.
 * @retval NRF_ERROR_INVALID_STATE        If the Peer Manager is not initialized.
 */
ret_code_t pm_conn_sec_status_get(uint16_t conn_handle, pm_conn_sec_status_t * p_conn_sec_status);


/**@brief Experimental function for specifying the public key to use for LESC operations.
 *
 * @details This function can be called multiple times. The specified public key will be used for
 *          all subsequent LESC (LE Secure Connections) operations until the next time this function
 *          is called.
 *
 * @note The key must continue to reside in application memory as it is not copied by Peer Manager.
 *
 * @param[in]  p_public_key  The public key to use for all subsequent LESC operations.
 *
 * @retval NRF_SUCCESS                    Pairing initiated successfully.
 * @retval NRF_ERROR_INVALID_STATE        Peer Manager is not initialized.
 */
ret_code_t pm_lesc_public_key_set(ble_gap_lesc_p256_pk_t * p_public_key);


/**
 * @brief Function for constructing a whitelist for use when advertising.
 *
 * @details This function constructs a whitelist that contains the addresses and IRKs of the
 *          provided peer IDs. If @p p_peer_ids is NULL, the first (lowest) 8 peer IDs are chosen.
 *          If @ref ble_gap_whitelist_t.pp_addrs in @p p_whitelist is NULL, the whitelist contains
 *          only IRKs, and vice versa.
 *
 * @note When using a whitelist, always use the whitelist that was created or set by the most recent
 *       call to this function or to @ref pm_whitelist_custom.
 * @note Do not call this function while advertising or scanning with another whitelist.
 *
 * @param[in]     p_peer_ids   The IDs of the peers to be added to the whitelist, or NULL.
 * @param[in]     n_peer_ids   The number of peer IDs in @p p_peer_ids.
 * @param[in,out] p_whitelist  The constructed whitelist. Note that @p p_whitelist->pp_addrs
 *                             must be NULL or point to an array with size @ref
 *                             BLE_GAP_WHITELIST_ADDR_MAX_COUNT and @p p_whitelist->pp_irks
 *                             must be NULL or point to an array with size @ref
 *                             BLE_GAP_WHITELIST_IRK_MAX_COUNT.
 *
 * @retval NRF_SUCCESS              If the whitelist was created successfully.
 * @retval NRF_ERROR_NULL           If @p p_whitelist was NULL.
 * @retval NRF_ERROR_INVALID_STATE  If the Peer Manager is not initialized.
 */
ret_code_t pm_whitelist_create(pm_peer_id_t        * p_peer_ids,
                           uint8_t               n_peer_ids,
                           ble_gap_whitelist_t * p_whitelist);


/**
 * @brief Function for informing the Peer Manager of what whitelist should be used.
 *
 * @details This function should be used if the application wants to use a whitelist that is
 *          created in the application. When using Peer Manager, this function must be called to
 *          inform that the custom whitelist should be used instead of the one in Peer Manager.
 *
 * @note When using a whitelist, always use the whitelist that was created or set by the most recent
 *       call to this function or to @ref pm_whitelist_create.
 * @note Do not call this function while advertising or scanning with another whitelist.
 * @note Do not add any IRKs to the whitelist that are not present in the Peer Manager's persistent
 *       storage.
 *
 * @param[in] p_whitelist  The whitelist.
 *
 * @retval NRF_SUCCESS              If the operation completed successfully.
 * @retval NRF_ERROR_NULL           If @p p_whitelist was NULL.
 * @retval NRF_ERROR_NOT_FOUND      If one or more of the whitelist's IRKs was not found in the Peer
 *                                  Manager's persistent storage.
 * @retval NRF_ERROR_INVALID_STATE  If the Peer Manager is not initialized.
 */
ret_code_t pm_whitelist_custom(ble_gap_whitelist_t * p_whitelist);


/**
 * @brief Function for getting the connection handle of the connection with a bonded peer.
 *
 * @param[in]  peer_id        The peer ID of the bonded peer.
 * @param[out] p_conn_handle  Connection handle, or @ref BLE_ERROR_INVALID_CONN_HANDLE if the peer
 *                            is not connected.
 *
 * @retval NRF_SUCCESS              If the connection handle was determined successfully.
 * @retval NRF_ERROR_NULL           If @p p_conn_handle was NULL.
 * @retval NRF_ERROR_INVALID_STATE  If the Peer Manager is not initialized.
 */
ret_code_t pm_conn_handle_get(pm_peer_id_t peer_id, uint16_t * p_conn_handle);


/**@brief Function for getting the peer ID of a connected peer.
 *
 * @param[in]  conn_handle  Connection handle.
 * @param[out] p_peer_id    Peer ID, or @ref PM_PEER_ID_INVALID if the peer is not bonded or
 *                          @p conn_handle does not refer to a connection.
 *
 * @retval NRF_SUCCESS              If the peer ID was retrieved successfully.
 * @retval NRF_ERROR_NULL           If @p p_peer_id was NULL.
 * @retval NRF_ERROR_INVALID_STATE  If the Peer Manager is not initialized.
 */
ret_code_t pm_peer_id_get(uint16_t conn_handle, pm_peer_id_t * p_peer_id);


/**@brief Function for getting the next peer ID in the sequence of all used peer IDs.
 *
 * @details This function can be used to loop through all used peer IDs. The order in which
 *          peer IDs are returned should be considered unpredictable. @ref PM_PEER_ID_INVALID
 *          is considered to be before the first and after the last used peer ID.
 *
 * @details To loop through all peer IDs exactly once, use the following constuct:
 *          @code{c}
 *              pm_peer_id_t current_peer_id = pm_next_peer_id_get(PM_PEER_ID_INVALID);
 *              while (current_peer_id != PM_PEER_ID_INVALID)
 *              {
 *                  // Do something with current_peer_id.
 *                  current_peer_id = pm_next_peer_id_get(current_peer_id)
 *              }
 *          @endcode
 *
 * @param[in]  prev_peer_id  The previous peer ID.
 *
 * @return  The next peer ID. If @p prev_peer_id was @ref PM_PEER_ID_INVALID, the
 *          next peer ID is the first used peer ID. If @p prev_peer_id was the last
 *          used peer ID, the function returns @ref PM_PEER_ID_INVALID.
 */
pm_peer_id_t pm_next_peer_id_get(pm_peer_id_t prev_peer_id);


/**@brief Function for querying the number of valid peer IDs that are available.
 *
 * @details This function returns the number of peers for which there is data in persistent storage.
 *
 * @return  The number of valid peer IDs.
 */
uint32_t pm_peer_count(void);




/**@anchor PM_PEER_DATA_FUNCTIONS
 * @name Functions (Peer Data)
 * Functions for manipulating peer data.
 * @{
 */

/**
 * @{
 */

/**@brief Function for retrieving stored data of a peer.
 *
 * @note The length of the provided buffer must be a multiple of 4.
 *
 * @param[in]    peer_id   Peer ID to get data for.
 * @param[in]    data_id   Which type of data to read.
 * @param[out]   p_data    Where to put the retrieved data.
 * @param[inout] p_len     In: The length in bytes of @p p_data.
 *                         Out: The length in bytes of the read data, if the read was successful.
 *
 * @retval NRF_SUCCESS              If the data was read successfully.
 * @retval NRF_ERROR_INVALID_PARAM  If the the data type or the peer ID was invalid or unallocated,
 *                                  or if the length in @p p_length was not a multiple of 4.
 * @retval NRF_ERROR_NULL           If a pointer parameter was NULL.
 * @retval NRF_ERROR_NOT_FOUND      If no stored data was found for this peer ID/data ID combination.
 * @retval NRF_ERROR_DATA_SIZE      If the provided buffer was not large enough.
 * @retval NRF_ERROR_INVALID_STATE  If the Peer Manager is not initialized.
 */
ret_code_t pm_peer_data_load(pm_peer_id_t      peer_id,
                             pm_peer_data_id_t data_id,
                             void            * p_data,
                             uint16_t        * p_len);

/**@brief Function for reading a peer's bonding data (@ref PM_PEER_DATA_ID_BONDING).
 * @details See @ref pm_peer_data_load for parameters and return values. */
ret_code_t pm_peer_data_bonding_load(pm_peer_id_t             peer_id,
                                     pm_peer_data_bonding_t * p_data);

/**@brief Function for reading a peer's remote DB values. (@ref PM_PEER_DATA_ID_GATT_REMOTE).
 * @details See @ref pm_peer_data_load for parameters and return values. */
ret_code_t pm_peer_data_remote_db_load(pm_peer_id_t        peer_id,
                                       ble_gatt_db_srv_t * p_data,
                                       uint16_t          * p_len);

/**@brief Function for reading a peer's application data. (@ref PM_PEER_DATA_ID_APPLICATION).
 * @details See @ref pm_peer_data_load for parameters and return values. */
ret_code_t pm_peer_data_app_data_load(pm_peer_id_t peer_id,
                                      uint8_t    * p_data,
                                      uint16_t   * p_len);
/** @}*/


/**
 * @{
 */

/**@brief Function for setting or updating stored data of a peer.
 *
 * @note Writing the data to persistent storage happens asynchronously. Therefore, the buffer
 *       that contains the data must be kept alive until the operation has completed.
 *
 * @note The data written using this function may later be overwritten as a result of internal
 *       operations in the Peer Manager. A Peer Manager event is sent each time data is updated,
 *       regardless of whether the operation originated internally or from action by the user.
 *
 * @param[in]  peer_id  Peer ID to set data for.
 * @param[in]  data_id  Which type of data to set.
 * @param[in]  p_data   New value to set.
 * @param[in]  len      The length in bytes of @p p_data.
 * @param[out] p_token  A token that identifies this particular store operation. The token can be
 *                      used to identify events that pertain to this operation. This parameter can
 *                      be NULL.
 *
 * @retval NRF_SUCCESS              If the data is scheduled to be written to persistent storage.
 * @retval NRF_ERROR_NULL           If @p p_data is NULL.
 * @retval NRF_ERROR_NOT_FOUND      If no peer was found for the peer ID.
 * @retval NRF_ERROR_BUSY           If the underlying flash handler is busy with other flash
 *                                  operations. Try again after receiving a Peer Manager event.
 * @retval NRF_ERROR_INVALID_STATE  If the Peer Manager is not initialized.
 */
ret_code_t pm_peer_data_store(pm_peer_id_t       peer_id,
                              pm_peer_data_id_t  data_id,
                              void       const * p_data,
                              uint16_t           len,
                              pm_store_token_t * p_token);

/**@brief Function for setting or updating a peer's bonding data (@ref PM_PEER_DATA_ID_BONDING).
 * @details See @ref pm_peer_data_store for parameters and return values. */
ret_code_t pm_peer_data_bonding_store(pm_peer_id_t                   peer_id,
                                      pm_peer_data_bonding_t const * p_data,
                                      pm_store_token_t             * p_token);

/**@brief Function for setting or updating a peer's remote DB values. (@ref PM_PEER_DATA_ID_GATT_REMOTE).
 * @details See @ref pm_peer_data_store for parameters and return values. */
ret_code_t pm_peer_data_remote_db_store(pm_peer_id_t              peer_id,
                                        ble_gatt_db_srv_t const * p_data,
                                        uint16_t                  len,
                                        pm_store_token_t        * p_token);

/**@brief Function for setting or updating a peer's application data. (@ref PM_PEER_DATA_ID_APPLICATION).
 * @details See @ref pm_peer_data_store for parameters and return values. */
ret_code_t pm_peer_data_app_data_store(pm_peer_id_t       peer_id,
                                       uint8_t    const * p_data,
                                       uint16_t           len,
                                       pm_store_token_t * p_token);
/** @}*/


/**
 * @{
 */

/**@brief Function for deleting a peer's stored pieces of data.
 *
 * @details This function deletes specific data that is stored for a peer. Note that bonding data
 *          cannot be cleared separately.
 *
 *          To delete all data for a peer (including bonding data), use @ref pm_peer_delete.
 *
 * @note Clearing data in persistent storage happens asynchronously.
 *
 * @param[in]  peer_id  Peer ID to clear data for.
 * @param[in]  data_id  Which data to clear.
 *
 * @retval NRF_SUCCESS              If the clear procedure was initiated successfully.
 * @retval NRF_ERROR_INVALID_PARAM  If @p data_id was PM_PEER_DATA_ID_BONDING or invalid, or
 *                                  @p peer_id was invalid.
 * @retval NRF_ERROR_NOT_FOUND      If there was no data to clear for this peer ID/data ID combination.
 * @retval NRF_ERROR_BUSY           If the underlying flash handler is busy with other flash
 *                                  operations. Try again after receiving a Peer Manager event.
 * @retval NRF_ERROR_INVALID_STATE  If the Peer Manager is not initialized.
 * @retval NRF_ERROR_INTERNAL       If another error occurred.
 */
ret_code_t pm_peer_data_delete(pm_peer_id_t peer_id, pm_peer_data_id_t data_id);


/**@brief Function for manually adding a peer to the persistent storage.
 *
 * @details This function allocates a new peer ID and stores bonding data for the new peer. The
 *          bonding data is necessary to prevent ambiguity/inconsistency in peer data.
 *
 * @param[in]  p_bonding_data  The bonding data of the new peer (must contain a public/static
 *                             address or a non-zero IRK).
 * @param[out] p_new_peer_id   Peer ID for the new peer, or an existing peer if a match was found.
 * @param[out] p_token         A token that identifies this particular store operation (storing the
 *                             bonding data). The token can be used to identify events that pertain
 *                             to this operation. This parameter can be NULL.
 *
 * @retval NRF_SUCCESS              If the store operation for bonding data was initiated successfully.
 * @retval NRF_ERROR_NULL           If @p p_bonding_data or @p p_new_peer_id is NULL.
 * @retval NRF_ERROR_NO_MEM         If there is no more space in persistent storage, so that the new
 *                                  peer cannot be allocated.
 * @retval NRF_ERROR_BUSY           If the underlying flash handler is busy with other flash
 *                                  operations. Try again after receiving a Peer Manager event.
 * @retval NRF_ERROR_INVALID_PARAM  If the bonding data is invalid.
 * @retval NRF_ERROR_INVALID_STATE  If the Peer Manager is not initialized.
 */
ret_code_t pm_peer_new(pm_peer_id_t           * p_new_peer_id,
                       pm_peer_data_bonding_t * p_bonding_data,
                       pm_store_token_t       * p_token);


/**@brief Function for freeing persistent storage for a peer.
 *
 * @details This function deletes every piece of data that is associated with the specified peer and
 *          frees the peer ID to be used for another peer. The deletion happens asynchronously, and
 *          the peer ID is not freed until the data is deleted. When the operation finishes, a @ref
 *          PM_EVT_PEER_DELETE_SUCCEEDED or @ref PM_EVT_PEER_DELETE_FAILED event is sent.
 *
 * @warning Use this function only when not connected to or connectable for the peer that is being
 *          deleted. If the peer is or becomes connected or data is manually written in flash during
 *          this procedure (until the success or failure event happens), the behavior is undefined.
 *
 * @param[in]  peer_id  Peer ID to be freed and have all associated data deleted.
 *
 * @retval NRF_SUCCESS              If the operation was initiated successfully.
 * @retval NRF_ERROR_INVALID_PARAM  If the peer ID was not valid.
 * @retval NRF_ERROR_INVALID_STATE  If the Peer Manager is not initialized.
 */
ret_code_t pm_peer_delete(pm_peer_id_t peer_id);


/**@brief Function for deleting all data stored for all peers.
 *
 * @details This function sends either a @ref PM_EVT_PEERS_DELETE_SUCCEEDED or a @ref
 *          PM_EVT_PEERS_DELETE_FAILED event. In addition, a @ref PM_EVT_PEER_DELETE_SUCCEEDED or
 *          @ref PM_EVT_PEER_DELETE_FAILED event is sent for each deleted peer.
 *
 * @note No event is sent when there is no peer data in flash.
 *
 * @warning Use this function only when not connected or connectable. If a peer is or becomes
 *          connected or a @ref PM_PEER_DATA_FUNCTIONS function is used during this procedure (until
 *          the success or failure event happens), the behavior is undefined.
 *
 * @retval NRF_SUCCESS              If the deletion process was initiated successfully.
 * @retval NRF_ERROR_INVALID_STATE  If the Peer Manager is not initialized.
 * @retval NRF_ERROR_INTERNAL       If another error occurred.
 */
ret_code_t pm_peers_delete(void);
/** @}*/


/**
 * @{
 */


/**@brief Function for finding the highest and lowest ranked peers.
 *
 * @details The rank is saved in persistent storage under the data ID @ref PM_PEER_DATA_ID_PEER_RANK.
 *
 * @details The interpretation of rank is up to the user, because the rank is only updated by
 *          calling @ref pm_peer_rank_highest or by manipulating the value using a @ref
 *          PM_PEER_DATA_FUNCTIONS function.
 *
 * @note Any argument that is NULL is ignored.
 *
 * @param[out] p_highest_ranked_peer  The peer ID with the highest rank of all peers, for example,
 *                                    the most recently used peer.
 * @param[out] p_highest_rank         The highest rank.
 * @param[out] p_lowest_ranked_peer   The peer ID with the lowest rank of all peers, for example,
 *                                    the least recently used peer.
 * @param[out] p_lowest_rank          The lowest rank.
 *
 * @retval NRF_SUCCESS              If the operation completed successfully.
 * @retval NRF_ERROR_NOT_FOUND      If no peers were found.
 * @retval NRF_ERROR_INVALID_STATE  If the Peer Manager is not initialized.
 * @retval NRF_ERROR_INTERNAL       If another error occurred.
 */
ret_code_t pm_peer_ranks_get(pm_peer_id_t * p_highest_ranked_peer,
                             uint32_t     * p_highest_rank,
                             pm_peer_id_t * p_lowest_ranked_peer,
                             uint32_t     * p_lowest_rank);


/**@brief Function for updating the rank of a peer to be highest among all stored peers.
 *
 * @details If this function returns @ref NRF_SUCCESS, either a @ref PM_EVT_PEER_DATA_UPDATE_SUCCEEDED or a
 *          @ref PM_EVT_PEER_DATA_UPDATE_FAILED event is sent with a @ref
 *          PM_STORE_TOKEN_INVALID store token when the operation is complete. Until the operation
 *          is complete, this function returns @ref NRF_ERROR_BUSY.
 *
 *          When the operation is complete, the peer is the highest ranked peer as reported by
 *          @ref pm_peer_ranks_get.
 *
 * @note The @ref PM_EVT_PEER_DATA_UPDATE_SUCCEEDED event can arrive before the function returns if the peer
 *       is already ranked highest. In this case, the @ref pm_peer_data_update_succeeded_evt_t::flash_changed flag
 *       in the event will be false.
 *
 * @param[in]  peer_id  The peer to rank highest.
 *
 * @retval NRF_SUCCESS              If the peer's rank is, or will be updated to be highest.
 * @retval NRF_ERROR_BUSY           If the underlying flash handler is busy with other flash
 *                                  operations, or if a previous call to this function has not
 *                                  completed. Try again after receiving a Peer Manager event.
 * @retval NRF_ERROR_INTERNAL       If another error occurred.
 * @retval NRF_ERROR_INVALID_STATE  If the Peer Manager is not initialized.
 */
ret_code_t pm_peer_rank_highest(pm_peer_id_t peer_id);

/** @}*/

/** @} */

/** @} */

#endif // PEER_MANAGER_H__
