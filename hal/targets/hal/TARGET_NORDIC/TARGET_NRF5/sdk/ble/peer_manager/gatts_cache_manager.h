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


#ifndef GATTS_CACHE_MANAGER_H__
#define GATTS_CACHE_MANAGER_H__

#include <stdint.h>
#include "sdk_errors.h"
#include "ble.h"
#include "ble_gap.h"
#include "peer_manager_types.h"



/**
 * @cond NO_DOXYGEN
 * @defgroup gatts_cache_manager GATT Server Cache Manager
 * @ingroup peer_manager
 * @{
 * @brief An internal module of @ref peer_manager. A module for managing persistent storing of GATT
 *        attributes pertaining to the GATT server role of the local device.
 */


/**@brief Events that can come from the GATT Server Cache Manager module.
 */
typedef enum
{
    GSCM_EVT_LOCAL_DB_CACHE_STORED,  /**< The persistent cache for the local database has been updated with provided values, for one peer. */
    GSCM_EVT_LOCAL_DB_CACHE_UPDATED, /**< The persistent cache for the local database has been updated with values from the SoftDevice, for one peer. */
    GSCM_EVT_SC_STATE_STORED,        /**< The service changed pending flag in persistent storage has been updated, for one peer. */
} gscm_evt_id_t;


/**@brief Structure containing an event from the GSCM module.
 */
typedef struct
{
    gscm_evt_id_t evt_id;          /**< The type of event this is. */
    pm_peer_id_t  peer_id;         /**< The peer ID this event pertains to. */
    union
    {
        struct
        {
            uint16_t conn_handle;  /**< The connection this event pertains to. */
        } local_db_cache_updated;
        struct
        {
            bool state;            /**< The newly stored state of the Service Changed pending flag. */
        } sc_state_stored;
    } params;                      /**< Event parameters specific to certain event types. */
} gscm_evt_t;

/**@brief Event handler for events from the GATT Server Cache Manager module.
 *
 * @param[in]  event   The event that has happened.
 * @param[in]  peer  The id of the peer the event pertains to.
 * @param[in]  flags   The data the event pertains to.
 */
typedef void (*gscm_evt_handler_t)(gscm_evt_t const * p_event);


/**@brief Function for initializing the GATT Server Cache Manager module.
 *
 * @param[in]  evt_handler  Callback for events from the GATT Server Cache Manager module.
 *
 * @retval NRF_SUCCESS         Initialization was successful.
 * @retval NRF_ERROR_NULL      evt_handler was NULL.
 */
ret_code_t gscm_init(gscm_evt_handler_t evt_handler);


/**@brief Function for triggering local GATT database data to be stored persistently. Values are
 *        retrieved from the SoftDevice and written to persistent storage.
 *
 * @param[in]  conn_handle  Connection handle to perform update on.
 *
 * @retval NRF_SUCCESS                    Store operation started.
 * @retval BLE_ERROR_INVALID_CONN_HANDLE  conn_handle does not refer to an active connection with a
 *                                        bonded peer.
 * @retval NRF_ERROR_BUSY                 Unable to perform operation at this time. Reattempt later.
 * @retval NRF_ERROR_DATA_SIZE            Write buffer not large enough. Call will never work with
 *                                        this GATT database.
 * @retval NRF_ERROR_NO_MEM               No room in persistent_storage. Free up space; the
 *                                        operation will be automatically reattempted after the
 *                                        next compression procedure
 * @retval NRF_ERROR_INVALID_STATE        Module is not initialized.
 */
ret_code_t gscm_local_db_cache_update(uint16_t conn_handle);


/**@brief Function for applying stored local GATT database data to the SoftDevice. Values are
 *        retrieved from persistent storage and given to the SoftDevice.
 *
 * @param[in]  conn_handle  Connection handle to apply values to.
 *
 * @retval NRF_SUCCESS                    Store operation started.
 * @retval BLE_ERROR_INVALID_CONN_HANDLE  conn_handle does not refer to an active connection with a
 *                                        bonded peer.
 * @retval NRF_ERROR_INVALID_DATA         The stored data was rejected by the SoftDevice, which
 *                                        probably means that the local database has changed. The
 *                                        system part of the sys_attributes was attempted applied,
 *                                        so service changed indications can be sent to subscribers.
 * @retval NRF_ERROR_BUSY                 Unable to perform operation at this time. Reattempt later.
 * @retval NRF_ERROR_INVALID_STATE        Module is not initialized.
 * @return An unexpected return value from an internal function call.
 */
ret_code_t gscm_local_db_cache_apply(uint16_t conn_handle);


/**@brief Function for setting new values in the local database cache.
 *
 * @note If the peer is connected, the values will also be applied immediately to the connection.
 * @note The data in the pointer must be available until the GSCM_EVT_LOCAL_DB_STORED event is
 *       received.
 *
 * @param[in]  peer_id     Peer to set values for.
 * @param[in]  p_local_db  Database values to apply. If NULL, the values will instead be cleared.
 *
 * @retval NRF_SUCCESS              Operation started, and values were applied (if connected).
 * @retval NRF_ERROR_NOT_FOUND      The peer ID was invalid or unallocated.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 * @return An unexpected return value from an internal function call.
 */
ret_code_t gscm_local_db_cache_set(pm_peer_id_t peer_id, pm_peer_data_local_gatt_db_t * p_local_db);


/**@brief Function for retrieving values in the local database cache.
 *
 * @param[in]    peer_id     Peer to get values for.
 * @param[inout] p_local_db  Where to store the data. The length field needs to reflect the
 *                           available buffer space. On a successful read, the length field is
 *                           updated to match the length of the read data.
 *
 * @retval NRF_SUCCESS              Values retrieved successfully.
 * @retval NRF_ERROR_NOT_FOUND      The peer ID was invalid or unallocated.
 * @retval NRF_ERROR_NULL           p_local_db was NULL.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t gscm_local_db_cache_get(pm_peer_id_t peer_id, pm_peer_data_local_gatt_db_t * p_local_db);


/**@brief Function for storing the fact that the local database has changed, for all currently
 *        bonded peers.
 *
 * @note This will cause a later call to @ref gscm_service_changed_ind_needed to return true for
 *       a connection with a currently bonded peer.
 */
void gscm_local_database_has_changed(void);


/**@brief Function for checking if a service changed indication should be sent.
 *
 * @param[in]  conn_handle  The connection to check.
 *
 * @return true if a service changed indication should be sent, false if not.
 */
bool gscm_service_changed_ind_needed(uint16_t conn_handle);


/**@brief Function for sending a service changed indication to a connected peer.
 *
 * @param[in]  conn_handle  The connection to send the indication on.
 *
 * @retval NRF_SUCCESS                       Indication sent or not needed.
 * @retval BLE_ERROR_INVALID_CONN_HANDLE     conn_handle does not refer to an active connection.
 * @retval NRF_ERROR_BUSY                    Unable to send indication at this time. Reattempt later.
 * @retval BLE_ERROR_GATTS_SYS_ATTR_MISSING  Information missing. Apply local cache, then reattempt.
 * @retval NRF_ERROR_INVALID_PARAM           From @ref sd_ble_gatts_service_changed. Unexpected.
 * @retval NRF_ERROR_NOT_SUPPORTED           Service changed characteristic is not present.
 * @retval NRF_ERROR_INVALID_STATE           Service changed cannot be indicated to this peer
 *                                           because the peer has not subscribed to it.
 */
ret_code_t gscm_service_changed_ind_send(uint16_t conn_handle);


/**@brief Function for specifying that a peer has been made aware of the latest local database
 *        change.
 *
 * @note After calling this, a later call to @ref gscm_service_changed_ind_needed will to return
 *       false for this peer unless @ref gscm_local_database_has_changed is called again.
 *
 * @param[in]  peer_id  The connection to send the indication on.
 */
void gscm_db_change_notification_done(pm_peer_id_t peer_id);

/** @} 
 * @endcond
*/

#endif /* GATTS_CACHE_MANAGER_H__ */
