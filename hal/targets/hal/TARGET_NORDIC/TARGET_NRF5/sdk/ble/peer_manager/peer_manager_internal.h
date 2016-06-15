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


#ifndef PEER_MANAGER_INTERNAL_H__
#define PEER_MANAGER_INTERNAL_H__

#include <stdint.h>
#include "sdk_errors.h"
#include "ble.h"
#include "ble_gap.h"
#include "peer_manager_types.h"


/**
 * @cond NO_DOXYGEN
 * @file peer_manager_types.h
 *
 * @addtogroup peer_manager
 * @brief File containing definitions used solely inside the Peer Manager's modules.
 * @{
 */

ANON_UNIONS_ENABLE

/**@brief One piece of data associated with a peer, together with its type.
 *
 * @note This type is deprecated.
 */
typedef struct
{
    uint16_t              length_words; /**< @brief The length of the data in words. */
    pm_peer_data_id_t     data_id;      /**< @brief ID that specifies the type of data (defines which member of the union is used). */
    union
    {
        pm_peer_data_bonding_t       * p_bonding_data;            /**< @brief The exchanged bond information in addition to metadata of the bonding. */
        uint32_t                     * p_peer_rank;               /**< @brief A value locally assigned to this peer. Its interpretation is up to the user. The rank is not set automatically by the Peer Manager, but it is assigned by the user using either @ref pm_peer_rank_highest or a @ref PM_PEER_DATA_FUNCTIONS function. */
        bool                         * p_service_changed_pending; /**< @brief Whether a service changed indication should be sent to the peer. */
        pm_peer_data_local_gatt_db_t * p_local_gatt_db;           /**< @brief Persistent information pertaining to a peer GATT client. */
        ble_gatt_db_srv_t            * p_remote_gatt_db;          /**< @brief Persistent information pertaining to a peer GATT server. */
        uint8_t                      * p_application_data;        /**< @brief Arbitrary data to associate with the peer. This data can be freely used by the application. */
        void                         * p_all_data;                /**< @brief Generic access pointer to the data. It is used only to handle the data without regard to type. */
    }; /**< @brief The data. */
} pm_peer_data_t;


/**@brief Immutable version of @ref pm_peer_data_t.
 *
 * @note This type is deprecated.
 */
typedef struct
{
    uint16_t                    length_words; /**< @brief The length of the data in words. */
    pm_peer_data_id_t           data_id;      /**< @brief ID that specifies the type of data (defines which member of the union is used). */
    union
    {
        pm_peer_data_bonding_t       const * p_bonding_data;            /**< @brief Immutable @ref pm_peer_data_t::p_bonding_data. */
        uint32_t                     const * p_peer_rank;               /**< @brief Immutable @ref pm_peer_data_t::p_peer_rank. */
        bool                         const * p_service_changed_pending; /**< @brief Immutable @ref pm_peer_data_t::p_service_changed_pending. */
        pm_peer_data_local_gatt_db_t const * p_local_gatt_db;           /**< @brief Immutable @ref pm_peer_data_t::p_local_gatt_db. */
        ble_gatt_db_srv_t            const * p_remote_gatt_db;          /**< @brief Immutable @ref pm_peer_data_t::p_remote_gatt_db. */
        uint8_t                      const * p_application_data;        /**< @brief Immutable @ref pm_peer_data_t::p_application_data. */
        void                         const * p_all_data;                /**< @brief Immutable @ref pm_peer_data_t::p_all_data. */
    }; /**< @brief The data. */
} pm_peer_data_const_t;

ANON_UNIONS_DISABLE


/**@brief Version of @ref pm_peer_data_t that reflects the structure of peer data in flash.
 *
 * @note This type is deprecated.
 */
typedef pm_peer_data_const_t pm_peer_data_flash_t;


/**@brief Macro for calculating the flash size of bonding data.
 *
 * @return The number of words that the data takes in flash.
 */
#define PM_BONDING_DATA_N_WORDS() BYTES_TO_WORDS(sizeof(pm_peer_data_bonding_t))


/**@brief Macro for calculating the flash size of service changed pending state.
 *
 * @return The number of words that the data takes in flash.
 */
#define PM_SC_STATE_N_WORDS() BYTES_TO_WORDS(sizeof(bool))


/**@brief Macro for calculating the flash size of local GATT database data.
 *
 * @param[in]  local_db_len  The length, in bytes, of the database as reported by the SoftDevice.
 *
 * @return The number of words that the data takes in flash.
 */
#define PM_LOCAL_DB_N_WORDS(local_db_len) \
                                    BYTES_TO_WORDS((local_db_len) + PM_LOCAL_DB_LEN_OVERHEAD_BYTES)


/**@brief Macro for calculating the length of a local GATT database attribute array.
 *
 * @param[in]  n_words  The number of words that the data takes in flash.
 *
 * @return The length of the database attribute array.
 */
#define PM_LOCAL_DB_LEN(n_words) (((n_words) * BYTES_PER_WORD) - PM_LOCAL_DB_LEN_OVERHEAD_BYTES)


/**@brief Macro for calculating the flash size of remote GATT database data.
 *
 * @param[in]  service_count  The number of services in the service array.
 *
 * @return The number of words that the data takes in flash.
 */
#define PM_REMOTE_DB_N_WORDS(service_count) BYTES_TO_WORDS(sizeof(ble_gatt_db_srv_t) * (service_count))


/**@brief Macro for calculating the flash size of remote GATT database data.
 *
 * @param[in]  n_words  The length in number of words.
 *
 * @return The number of words that the data takes in flash.
 */
#define PM_REMOTE_DB_N_SERVICES(n_words) (((n_words) * BYTES_PER_WORD) / sizeof(ble_gatt_db_srv_t))


/**@brief Function for calculating the flash size of the usage index.
 *
 * @return The number of words that the data takes in flash.
 */
#define PM_USAGE_INDEX_N_WORDS() BYTES_TO_WORDS(sizeof(uint32_t))

/** @}
 * @endcond
 */

#endif /* PEER_MANAGER_INTERNAL_H__ */
