/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
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

/** @file
 *
 * @defgroup ble_sdk_lib_bond_manager Bonds and Persistent Data Manager
 * @{
 * @ingroup ble_sdk_lib
 * @brief This module handles multiple bonds and persistent attributes.
 *
 * @details When using <i>Bluetooth</i> low energy, a central device and a peripheral device can
 *          exchange identification information which they are capable of storing in memory (for
 *          example, in non-volatile memory). This information can be used for identity verification
 *          between the devices when they reconnect in the future. This relationship is known as a
 *          'bond'.
 *
 *          <b>Bonding Information:</b>
 *
 *          The S110 SoftDevice handles the BLE on-air transactions necessary to establish a new
 *          bond or to use a previous bond when it reconnects to a bonded central. It is however up
 *          to the application layer to memorize the <i>identification information</i> between
 *          connections and to provide them back to the S110 stack when it detects a re-connection
 *          from a previously bonded central. This identification information is referred to as
 *          Bonding Information in code and the SDK.
 *
 *          <b>System Attributes:</b>
 *
 *          If the application is a GATT server, it stores a set of persistent attributes related
 *          to a connection when bonding with a central. On reconnection with the known bonded
 *          central, the application restores the related persistent attributes in the last known
 *          state back to the S110 SoftDevice. These persistent attributes mainly include the Client
 *          Characteristic Configuration Descriptor (or CCCD, see the <i><b>Bluetooth</b>
 *          Core Specification</i> for more information) states and could include other attributes
 *          in the future. Persistent attributes are referred to as System Attributes in code and
 *          in the SDK documentation.
 *
 *          An application can use the Bonds and Persistent Data Manager module (referred to as the
 *          bond manager)  included in the nRF51 SDK to handle most of the operations related to
 *          the Bonding Information and System Attributes. The bond manager notifies the
 *          application when it's connected to a new bonded central or to a previously bonded central.
 *          The application can use the Bond Manager API to store or load (or restore) the
 *          Bonding Information and System Attributes. The bond manager identifies all the centrals
 *          the application is bonded to and restores their respective Bonding Information and
 *          System Attributes to the S110 stack.
 *
 *          In addition, you can use the bond manager to set up your application to advertise:
 *
 *          - To a given bonded central using directed advertisement.
 *          - To a list of bonded centrals - i.e. using whitelist.
 *
 *          The bond manager automatically writes the Bonding Information to the flash memory when
 *          the bonding procedure to a new central is finished. Upon disconnection, the application
 *          should use the Bond Manager API @ref ble_bondmngr_bonded_centrals_store to write the
 *          latest Bonding Information and System Attributes to flash.
 *
 *          The bond manager provides the API @ref ble_bondmngr_sys_attr_store to allow the
 *          application to write the System Attributes to flash while in a connection. Your
 *          application should call this API when it considers that all CCCDs and other persistent
 *          attributes are in a stable state. This API call will safely fail if System Attributes
 *          data for the current connected central is already present in the flash. The API does so
 *          because a flash write in such a situation would require an erase of flash, which takes a
 *          long time (21 milliseconds) to complete. This may disrupt the radio.
 *
 *          Applications using the Bond Manager must have a configuration file named
 *          ble_bondmngr_cfg.h (see below for details).
 *
 *          Refer to @ref ble_bond_mgr_msc to see the flow of events when connecting to a central
 *          using the Bond Manager
 *
 *
 * @section ble_sdk_lib_bond_manager_cfg Configuration File
 * Applications using the Bond Manager must have a configuration file named ble_bondmngr_cfg.h.
 * Here is an example of this file:
 *
 * @code
 * #ifndef BLE_BONDMNGR_CFG_H__
 * #define BLE_BONDMNGR_CFG_H__
 *
 * #define BLE_BONDMNGR_CCCD_COUNT           1
 * #define BLE_BONDMNGR_MAX_BONDED_CENTRALS  4
 *
 * #endif // BLE_BONDMNGR_CFG_H__
 * @endcode
 *
 * BLE_BONDMNGR_CCCD_COUNT is the number of CCCDs used in the application, and
 * BLE_BONDMNGR_MAX_BONDED_CENTRALS is the maximum number of bonded centrals to be supported by the
 * application.
 */

#ifndef BLE_BONDMNGR_H__
#define BLE_BONDMNGR_H__

#include <stdint.h>
#include "nordic_global.h"
#include "ble.h"
#include "ble_srv_common.h"


/** @defgroup ble_bond_mgr_msc Message Sequence Charts
 * @{
 * @brief Bond Manager interaction with S110 Stack
 * @{
 * @defgroup UNBONDED_CENTRAL Connecting to an unbonded central
 * @image html bond_manager_unbonded_master.jpg Connecting to an unbonded central
 * @defgroup BONDED_CENTRAL Connecting to a bonded central
 * @image html bond_manager_bonded_master.jpg Connecting to a bonded central
 * @}
 * @}
 */

/** @defgroup DEFINES Defines
 *  @brief Macros defined by this module.
 * @{ */

#define INVALID_CENTRAL_HANDLE       (-1)                        /**< Invalid handle, used to indicate that the central is not a known bonded central. */

/** @} */

/** @defgroup ENUMS Enumerations
 *  @brief Enumerations defined by this module.
 * @{ */

/**@brief Bond Manager Module event type. */
typedef enum
{
    BLE_BONDMNGR_EVT_NEW_BOND,                                  /**< New bond has been created. */
    BLE_BONDMNGR_EVT_CONN_TO_BONDED_CENTRAL,                    /**< Connected to a previously bonded central. */
    BLE_BONDMNGR_EVT_ENCRYPTED,                                 /**< Current link is encrypted. */
    BLE_BONDMNGR_EVT_AUTH_STATUS_UPDATED,                       /**< Authentication status updated for current central. */
    BLE_BONDMNGR_EVT_BOND_FLASH_FULL                            /**< Flash block for storing Bonding Information is full. */
} ble_bondmngr_evt_type_t;

/** @} */


/** @defgroup DATA_STRUCTURES Data Structures
 *  @brief Data Structures defined by this module.
 * @{ */

/**@brief Bond Manager Module event. */
typedef struct
{
    ble_bondmngr_evt_type_t evt_type;                           /**< Type of event. */
    int8_t                  central_handle;                     /**< Handle to the current central. This is an index to the central list maintained internally by the bond manager. */
    uint16_t                central_id;                         /**< Identifier to the central. This will be the same as Encryption diversifier of the central (see @ref ble_gap_evt_sec_info_request_t). This value is constant for the duration of the bond. */
} ble_bondmngr_evt_t;

/** @} */

/** @defgroup TYPEDEFS Typedefs
 *  @brief Typedefs defined by this module.
 * @{ */

/**@brief Bond Manager Module event handler type. */
typedef void (*ble_bondmngr_evt_handler_t) (ble_bondmngr_evt_t * p_evt);

/** @} */

/** @addtogroup DATA_STRUCTURES
 * @{ */

/**@brief Bond Manager Module init structure. This contains all options and data needed for
 *        initialization of the Bond Manager module. */
typedef struct
{
    uint8_t                     flash_page_num_bond;            /**< Flash page number to use for storing Bonding Information. */
    uint8_t                     flash_page_num_sys_attr;        /**< Flash page number to use for storing System Attributes. */
    bool                        bonds_delete;                   /**< TRUE if bonding and System Attributes for all centrals is to be deleted from flash during initialization, FALSE otherwise. */
    ble_bondmngr_evt_handler_t  evt_handler;                    /**< Event handler to be called for handling events in bond manager. */
    ble_srv_error_handler_t     error_handler;                  /**< Function to be called in case of an error. */
} ble_bondmngr_init_t;

/** @} */


/** @defgroup FUNCTIONS Functions
 *  @brief Functions/APIs implemented and exposed by this module.
 * @{ */

/**@brief Function for initializing the Bond Manager.
 *
 * @param[in]  p_init  This contains information needed to initialize this module.
 *
 * @return     NRF_SUCCESS on successful initialization, otherwise an error code.
 */
uint32_t ble_bondmngr_init(ble_bondmngr_init_t * p_init);

/**@brief Function for handling all events from the BLE stack that relate to this module.
 *
 * @param[in]  p_ble_evt  The event received from the BLE stack.
 *
 * @return     NRF_SUCCESS if all operations went successfully,
 *             NRF_ERROR_NO_MEM if the maximum number of bonded centrals has been reached.
 *             Other error codes in other situations.
 */
void ble_bondmngr_on_ble_evt(ble_evt_t * p_ble_evt);

/**@brief Function for storing the bonded centrals data including bonding info and System Attributes into
 *          flash memory.
 *
 * @details If the data to be written is different from the existing data, this function erases the
 *          flash pages before writing to flash.
 *
 * @warning This function could prevent the radio from running. Therefore it MUST be called ONLY
 *          when the application knows that the <i>Bluetooth</i> radio is not active. An example of
 *          such a state is when the application has received the Disconnected event and has not yet
 *          started advertising. <b>If it is called in any other state, or if it is not called at
 *          all, the behavior is undefined.</b>
 *
 * @return  NRF_SUCCESS on success, an error_code otherwise.
 */
uint32_t ble_bondmngr_bonded_centrals_store(void);

/**@brief Function for deleting the bonded central database from flash.
 *
 * @details After calling this function you should call ble_bondmngr_init() to re-initialize the
 *          RAM database.
 *
 * @return  NRF_SUCCESS on success, an error_code otherwise.
 */
uint32_t ble_bondmngr_bonded_centrals_delete(void);

/**@brief Function for getting the whitelist containing all currently bonded centrals.
 *
 * @details     This function populates the whitelist with either the IRKs or the public adresses
 *              of all bonded centrals.
 *
 * @param[out]  p_whitelist  Whitelist structure with all bonded centrals.
 *
 * @return      NRF_SUCCESS on success, an error_code otherwise.
 */
uint32_t ble_bondmngr_whitelist_get(ble_gap_whitelist_t * p_whitelist);

/**@brief Function for getting the central's address corresponding to a given central_handle.
 *
 * @note        This function returns NRF_ERROR_INVALID_PARAM if the given central has a private
 *              address.
 *
 * @param[in]   central_handle  Central's handle.
 * @param[out]  p_central_addr  Pointer to the central's address which can be used for
 *                             directed advertising.
 */
uint32_t ble_bondmngr_central_addr_get(int8_t central_handle, ble_gap_addr_t * p_central_addr);

/**@brief Function for storing the System Attributes of a newly connected central.
 *
 * @details This function fetches the System Attributes of the current central from the stack, adds
 *          it to the database in memory, and also stores it in the flash (without erasing any
 *          flash page).
 *          This function is intended to facilitate the storage of System Attributes when connected
 *          to new central (whose System Attributes are NOT yet stored in flash) even in connected
 *          state without affecting radio link. This function can, for example, be called after the
 *          CCCD is written by a central. The function will succeed if the central is a new central.
 *          See @ref ble_sdk_app_hids_keyboard or @ref ble_sdk_app_hids_mouse for sample usage.
 *
 * @return  NRF_SUCCESS on success, otherwise an error code.
 *          NRF_ERROR_INVALID_STATE is returned if the System Attributes of the current central is
 *          already present in the flash because it is a previously known central.
 */
uint32_t ble_bondmngr_sys_attr_store(void);

/**@brief Function for fetching the identifiers of known centrals.
 *
 * @details         This function fetches the identifiers of the centrals that are currently in the
 *                  database, or in other words, known to the bond manager.
 *
 * @param[out]      p_central_ids   Pointer to the array of central identifiers. It is recommended
 *                                  that the length of this array be equal to
 *                                  MAX_NUMBER_OF_BONDED_CENTRALS * 2 bytes. If value of this pointer
 *                                  is NULL, only the number of centrals in the database will be
 *                                  filled in p_length. This can be used to find out the
 *                                  required size of the array pointed to by p_central_ids in
 *                                  a subsequent call.
 * @param[in, out]  p_length        Pointer to the length of p_central_ids array provided as
 *                                  input. On return, this function will write the number of central
 *                                  identifiers found to p_length
 *
 * @return          NRF_SUCCESS on success.
 *                  NRF_ERROR_NULL if the input parameter p_length is NULL.
 *                  NRF_ERROR_INVALID_STATE is returned if the bond manager was not initialized.
 *                  NRF_ERROR_DATA_SIZE is returned if the length of the input parameter
 *                  p_central_ids provided is not enough to fit in all the central identifiers in the
 *                  database.
 */
uint32_t ble_bondmngr_central_ids_get(uint16_t * p_central_ids, uint16_t * p_length);

/**@brief Function for deleting a single central from the database.
 * @details     This function deletes the Bonding Information and System Attributes of a single
 *              central from the flash.
 *              The application can use the @ref ble_bondmngr_central_ids_get function to fetch the
 *              identifiers of centrals present in the database and then call this function.
 *
 * @warning     This function could prevent the radio from running. Therefore it MUST be called ONLY
 *              when the application knows that the <i>Bluetooth</i> radio is not active. An example
 *              of such a state could be when the application is not in a connected state AND is
 *              also not advertising. <b>If it is called in any other state, the behavior is
 *              undefined.</b>
 *
 * @param[in]   central_id   Identifier of the central to be deleted.
 *
 * @return      NRF_SUCCESS on success.
 *              NRF_ERROR_INVALID_STATE is returned if the bond manager was not initialized.
 *              NRF_ERROR_NOT_FOUND if the central with the given identifier is not found in the
 *              database.
 */
uint32_t ble_bondmngr_bonded_central_delete(uint16_t central_id);

/**@brief Function to verify encryption status link with bonded central is encrypted or not.
 * @details     This function provides status of encrption of the link with a bonded central.
 *              Its is recommended that the application can use the
 *              @ref ble_bondmngr_central_ids_get function to verify if the central is in the
 *              database and then call this function.
 *
 * @warning     Currently the central id paramater is unused and is added only for future extension.
 *              As, today only one link is permitted for the peripheral device, status of current
 *              link is provided. In future, with more possibilities in the topology, central_id
 *              will be needed to identify the central.
 *
 * @param[out]  status    Status of encryption, true implies link encrypted.
 *
 * @return      NRF_SUCCESS on success.
 *              NRF_ERROR_INVALID_STATE is returned if the bond manager was not initialized.
 */
uint32_t ble_bondmngr_is_link_encrypted(bool * status);

/** @} */

#endif // BLE_BONDMNGR_H__

/** @} */
