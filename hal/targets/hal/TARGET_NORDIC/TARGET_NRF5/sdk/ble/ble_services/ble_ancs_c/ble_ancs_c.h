/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 */

/** @file
 *
 * @defgroup ble_sdk_srv_ancs_c Apple Notification Service client
 * @{
 * @ingroup ble_sdk_srv
 *
 * @brief Apple Notification Center Service Client Module.
 *
 * @details Disclaimer: This client implementation of the Apple Notification Center Service can
 *          be changed at any time by Nordic Semiconductor ASA. Server implementations such as the
 *          ones found in iOS can be changed at any time by Apple and may cause this client
 *          implementation to stop working.
 *
 * This module implements the Apple Notification Center Service (ANCS) client.
 * This client can be used as a Notification Consumer (NC) that receives data 
 * notifications from a Notification Provider (NP). The NP is typically an iOS 
 * device acting as a server. For terminology and up-to-date specs, see 
 * http://developer.apple.com.
 *
 * The term "notification" is used in two different meanings:
 * - An <i>iOS notification</i> is the data received from the Notification Provider.
 * - A <i>GATTC notification</i> is a way to transfer data with <i>Bluetooth</i> Smart.
 * In this module, we receive iOS notifications using GATTC notifications. 
 * We use the full term (iOS notification or GATTC notification) where required to avoid confusion.
 *
 * Upon initializing the module, you must add the different iOS notification attributes you
 * would like to receive for iOS notifications. @ref ble_ancs_c_attr_add.
 *
 * Once a connection is established with a central device, the module does a service discovery to
 * discover the ANVS server handles. If this succeeds (@ref BLE_ANCS_C_EVT_DISCOVERY_COMPLETE)
 * The handles for the CTS server are part of the @ref ble_ancs_c_evt_t structure and must be
 * assigned to a ANCS_C instance using the @ref ble_ancs_c_handles_assign function. For more
 * information about service discovery, see the ble_discovery module documentation
 * @ref lib_ble_db_discovery.
 *
 * The application can now subscribe to iOS notifications using 
 * @ref ble_ancs_c_notif_source_notif_enable. They arrive in the @ref BLE_ANCS_C_EVT_NOTIF event.
 * @ref ble_ancs_c_request_attrs can be used to request attributes for the notifications. They
 * arrive in the @ref BLE_ANCS_C_EVT_NOTIF_ATTRIBUTE event.
 *
 * @msc
 * hscale = "1.5";
 * Application, ANCS_C;
 * |||;
 * Application=>ANCS_C   [label = "ble_ancs_c_attr_add(attribute)"];
 * Application=>ANCS_C   [label = "ble_ancs_c_init(ancs_instance, event_handler)"];
 * ...;
 * Application<<=ANCS_C  [label = "BLE_ANCS_C_EVT_DISCOVERY_COMPLETE"];
 * Application=>ANCS_C   [label = "ble_ancs_c_handles_assign(ancs_instance, conn_handle, service_handles)"];
 * Application=>ANCS_C   [label = "ble_ancs_c_notif_source_notif_enable(ancs_instance)"];
 * Application=>ANCS_C   [label = "ble_ancs_c_data_source_notif_enable(ancs_instance)"];
 * |||;
 * ...;
 * |||;
 * Application<<=ANCS_C  [label = "BLE_ANCS_C_EVT_NOTIF"];
 * |||;
 * ...;
 * |||;
 * Application=>ANCS_C   [label = "ble_ancs_c_request_attrs(attr_id, buffer)"];
 * Application<<=ANCS_C  [label = "BLE_ANCS_C_EVT_NOTIF_ATTRIBUTE"];
 * |||;
 * @endmsc
 *
 * @note The application must propagate BLE stack events to this module
 *       by calling ble_ancs_c_on_ble_evt() from the @ref softdevice_handler callback.
 */
#ifndef BLE_ANCS_C_H__
#define BLE_ANCS_C_H__


#include "ble_types.h"
#include "ble_srv_common.h"
#include "device_manager.h"
#include "ble_db_discovery.h"


#define BLE_ANCS_ATTR_DATA_MAX              32  /**< Maximum data length of an iOS notification attribute. */
#define BLE_ANCS_NB_OF_CATEGORY_ID          12  /**< Number of iOS notification categories: Other, Incoming Call, Missed Call, Voice Mail, Social, Schedule, Email, News, Health And Fitness, Business And Finance, Location, Entertainment. */
#define BLE_ANCS_NB_OF_ATTRS                8   /**< Number of iOS notification attributes: AppIdentifier, Title, Subtitle, Message, MessageSize, Date, PositiveActionLabel, NegativeActionLabel. */
#define BLE_ANCS_NB_OF_EVT_ID               3   /**< Number of iOS notification events: Added, Modified, Removed.*/

/** @brief Length of the iOS notification data.
 *
 * @details 8 bytes:
 * Event ID |Event flags |Category ID |Category count|Notification UID
 * ---------|------------|------------|--------------|----------------
 * 1 byte   | 1 byte     | 1 byte     | 1 byte       | 4 bytes        
 */
#define BLE_ANCS_NOTIFICATION_DATA_LENGTH   8

#define ANCS_UUID_SERVICE                   0xF431  /**< 16-bit service UUID for the Apple Notification Center Service. */
#define ANCS_UUID_CHAR_CONTROL_POINT        0xD8F3  /**< 16-bit control point UUID. */
#define ANCS_UUID_CHAR_DATA_SOURCE          0xC6E9  /**< 16-bit data source UUID. */
#define ANCS_UUID_CHAR_NOTIFICATION_SOURCE  0x120D  /**< 16-bit notification source UUID. */

#define BLE_ANCS_EVENT_FLAG_SILENT          0       /**< 0b.......1 Silent: First (LSB) bit is set. All flags can be active at the same time.*/
#define BLE_ANCS_EVENT_FLAG_IMPORTANT       1       /**< 0b......1. Important: Second (LSB) bit is set. All flags can be active at the same time.*/
#define BLE_ANCS_EVENT_FLAG_PREEXISTING     2       /**< 0b.....1.. Pre-existing: Third (LSB) bit is set. All flags can be active at the same time.*/
#define BLE_ANCS_EVENT_FLAG_POSITIVE_ACTION 3       /**< 0b....1... Positive action: Fourth (LSB) bit is set. All flags can be active at the same time.*/
#define BLE_ANCS_EVENT_FLAG_NEGATIVE_ACTION 4       /**< 0b...1.... Negative action: Fifth (LSB) bit is set. All flags can be active at the same time. */


/**@brief Event types that are passed from client to application on an event. */
typedef enum
{
    BLE_ANCS_C_EVT_DISCOVERY_COMPLETE,         /**< A successful connection has been established and the service was found on the connected peer. */
    BLE_ANCS_C_EVT_DISCOVERY_FAILED,           /**< It was not possible to discover the service or characteristics of the connected peer. */
    BLE_ANCS_C_EVT_NOTIF,                      /**< An iOS notification was received on the notification source control point. */
    BLE_ANCS_C_EVT_INVALID_NOTIF,              /**< An iOS notification was received on the notification source control point, but the format is invalid. */
    BLE_ANCS_C_EVT_NOTIF_ATTRIBUTE             /**< A received iOS notification attribute has been parsed. */
} ble_ancs_c_evt_type_t;

/**@brief Category IDs for iOS notifications. */
typedef enum
{
    BLE_ANCS_CATEGORY_ID_OTHER,                /**< The iOS notification belongs to the "other" category.  */
    BLE_ANCS_CATEGORY_ID_INCOMING_CALL,        /**< The iOS notification belongs to the "Incoming Call" category. */
    BLE_ANCS_CATEGORY_ID_MISSED_CALL,          /**< The iOS notification belongs to the "Missed Call" category. */
    BLE_ANCS_CATEGORY_ID_VOICE_MAIL,           /**< The iOS notification belongs to the "Voice Mail" category. */
    BLE_ANCS_CATEGORY_ID_SOCIAL,               /**< The iOS notification belongs to the "Social" category. */
    BLE_ANCS_CATEGORY_ID_SCHEDULE,             /**< The iOS notification belongs to the "Schedule" category. */
    BLE_ANCS_CATEGORY_ID_EMAIL,                /**< The iOS notification belongs to the "E-mail" category. */
    BLE_ANCS_CATEGORY_ID_NEWS,                 /**< The iOS notification belongs to the "News" category. */
    BLE_ANCS_CATEGORY_ID_HEALTH_AND_FITNESS,   /**< The iOS notification belongs to the "Health and Fitness" category. */
    BLE_ANCS_CATEGORY_ID_BUSINESS_AND_FINANCE, /**< The iOS notification belongs to the "Buisness and Finance" category. */
    BLE_ANCS_CATEGORY_ID_LOCATION,             /**< The iOS notification belongs to the "Location" category. */
    BLE_ANCS_CATEGORY_ID_ENTERTAINMENT         /**< The iOS notification belongs to the "Entertainment" category. */
} ble_ancs_c_category_id_values_t;

/**@brief Event IDs for iOS notifications. */
typedef enum
{
    BLE_ANCS_EVENT_ID_NOTIFICATION_ADDED,     /**< The iOS notification was added. */
    BLE_ANCS_EVENT_ID_NOTIFICATION_MODIFIED,  /**< The iOS notification was modified. */
    BLE_ANCS_EVENT_ID_NOTIFICATION_REMOVED    /**< The iOS notification was removed. */
} ble_ancs_c_evt_id_values_t;

/**@brief Control point command IDs that the Notification Consumer can send to the Notification Provider. */
typedef enum
{
    BLE_ANCS_COMMAND_ID_GET_NOTIF_ATTRIBUTES,      /**< Requests attributes to be sent from the NP to the NC for a given notification. */
    BLE_ANCS_COMMAND_ID_GET_APP_ATTRIBUTES,        /**< Requests attributes to be sent from the NP to the NC for a given iOS App. */
    BLE_ANCS_COMMAND_ID_GET_PERFORM_NOTIF_ACTION,  /**< Requests an action to be performed on a given notification, for example dismiss an alarm. */
} ble_ancs_c_command_id_values_t;

/**@brief IDs for iOS notification attributes. */
typedef enum
{
    BLE_ANCS_NOTIF_ATTR_ID_APP_IDENTIFIER,         /**< Identifies that the attribute data is of an "App Identifier" type. */
    BLE_ANCS_NOTIF_ATTR_ID_TITLE,                  /**< Identifies that the attribute data is a "Title". */
    BLE_ANCS_NOTIF_ATTR_ID_SUBTITLE,               /**< Identifies that the attribute data is a "Subtitle". */
    BLE_ANCS_NOTIF_ATTR_ID_MESSAGE,                /**< Identifies that the attribute data is a "Message". */
    BLE_ANCS_NOTIF_ATTR_ID_MESSAGE_SIZE,           /**< Identifies that the attribute data is a "Message Size". */
    BLE_ANCS_NOTIF_ATTR_ID_DATE,                   /**< Identifies that the attribute data is a "Date". */
    BLE_ANCS_NOTIF_ATTR_ID_POSITIVE_ACTION_LABEL,  /**< The notification has a "Positive action" that can be executed associated with it. */
    BLE_ANCS_NOTIF_ATTR_ID_NEGATIVE_ACTION_LABEL,  /**< The notification has a "Negative action" that can be executed associated with it. */
} ble_ancs_c_notif_attr_id_values_t;


/**@brief Flags for iOS notifications. */
typedef struct
{
    uint8_t silent          : 1;  /**< If this flag is set, the notification has a low priority. */
    uint8_t important       : 1;  /**< If this flag is set, the notification has a high priority. */
    uint8_t pre_existing    : 1;  /**< If this flag is set, the notification is pre-existing. */
    uint8_t positive_action : 1;  /**< If this flag is set, the notification has a positive action that can be taken. */
    uint8_t negative_action : 1;  /**< If this flag is set, the notification has a negative action that can be taken. */
} ble_ancs_c_notif_flags_t;


/**@brief Parsing states for received iOS notification attributes.
 */
typedef enum
{
    COMMAND_ID_AND_NOTIF_UID,  /**< Parsing the command ID and the notification UID. */
    ATTR_ID,                   /**< Parsing attribute ID. */
    ATTR_LEN1,                 /**< Parsing the LSB of the attribute length. */
    ATTR_LEN2,                 /**< Parsing the MSB of the attribute length. */
    ATTR_DATA,                 /**< Parsing the attribute data. */
    DONE                       /**< Parsing is done. */
} ble_ancs_c_parse_state_t;


/**@brief iOS notification structure. */
typedef struct
{
    ble_ancs_c_evt_id_values_t      evt_id;          /**< Whether the notification was added, removed, or modified. */
    ble_ancs_c_notif_flags_t        evt_flags;       /**< Bitmask to signal if a special condition applies to the notification, for example, "Silent" or "Important". */
    ble_ancs_c_category_id_values_t category_id;     /**< Classification of the notification type, for example, email or location. */
    uint8_t                         category_count;  /**< Current number of active notifications for this category ID. */
    uint32_t                        notif_uid;       /**< Notification UID. */
} ble_ancs_c_evt_notif_t;


/**@brief iOS notification attribute structure for incomming attributes. */
typedef struct
{
    uint32_t                          notif_uid;    /**< UID of the notification that the attribute belongs to.*/
    uint16_t                          attr_len;     /**< Length of the received attribute data. */
    ble_ancs_c_notif_attr_id_values_t attr_id;      /**< Classification of the attribute type, for example, title or date. */
    uint8_t                         * p_attr_data;  /**< Pointer to where the memory is allocated for storing incoming attributes. */
} ble_ancs_c_evt_notif_attr_t;


/**@brief iOS notification attribute content wanted by our application. */
typedef struct
{
    bool                              get;          /**< Boolean to determine if this attribute will be requested from the Notification Provider. */
    ble_ancs_c_notif_attr_id_values_t attr_id;      /**< Attribute ID: AppIdentifier(0), Title(1), Subtitle(2), Message(3), MessageSize(4), Date(5), PositiveActionLabel(6), NegativeActionLabel(7). */
    uint16_t                          attr_len;     /**< Length of the attribute. If more data is received from the Notification Provider, all data beyond this length is discarded. */
    uint8_t                         * p_attr_data;  /**< Pointer to where the memory is allocated for storing incoming attributes. */
} ble_ancs_c_attr_list_t;


/**@brief Structure used for holding the Apple Notification Center Service found during the
          discovery process.
 */
typedef struct
{
    ble_gattc_service_t service;            /**< The GATT Service holding the discovered Apple Notification Center Service. (0xF431). */
    ble_gattc_char_t    control_point_char; /**< ANCS Control Point Characteristic. Allows interaction with the peer (0xD8F3). */
    ble_gattc_char_t    notif_source_char;  /**< ANCS Notification Source Characteristic. Keeps track of arrival, modification, and removal of notifications (0x120D). */
    ble_gattc_desc_t    notif_source_cccd;  /**< ANCS Notification Source Characteristic Descriptor. Enables or Disables GATT notifications */
    ble_gattc_char_t    data_source_char;   /**< ANCS Data Source Characteristic, where attribute data for the notifications is received from peer (0xC6E9). */
    ble_gattc_desc_t    data_source_cccd;   /**< ANCS Data Source Characteristic Descriptor. Enables or Disables GATT notifications */
} ble_ancs_c_service_t;


/**@brief ANCS client module event structure.
 *
 * @details The structure contains the event that should be handled by the main application.
 */
typedef struct
{
    ble_ancs_c_evt_type_t       evt_type;         /**< Type of event.*/
    uint16_t                    conn_handle;      /**< Connection handle on which the ANCS service was discovered on the peer device. This will be filled if the evt_type is @ref BLE_ANCS_C_EVT_DISCOVERY_COMPLETE.*/
    ble_ancs_c_evt_notif_t      notif;            /**< iOS notification. Will be filled if evt_type is @ref BLE_ANCS_C_EVT_NOTIF. */
    ble_ancs_c_evt_notif_attr_t attr;             /**< Currently received attribute for a given notification. Will be filled if the evt_type is @ref BLE_ANCS_C_EVT_NOTIF_ATTRIBUTE. */
    ble_ancs_c_service_t        service;          /**< Info on the discovered Alert Notification Service discovered. This will be filled if the evt_type is @ref BLE_ANCS_C_EVT_DISCOVERY_COMPLETE.*/
    uint32_t                    error_code;       /**< Additional status or error code if the event was caused by a stack error or GATT status, for example, during service discovery. */
} ble_ancs_c_evt_t;


/**@brief iOS notification event handler type. */
typedef void (*ble_ancs_c_evt_handler_t) (ble_ancs_c_evt_t * p_evt);


/**@brief iOS notification structure, which contains various status information for the client. */
typedef struct
{
    ble_ancs_c_evt_handler_t evt_handler;                             /**< Event handler to be called for handling events in the Apple Notification client application. */
    ble_srv_error_handler_t  error_handler;                           /**< Function to be called in case of an error. */
    uint16_t                 conn_handle;                             /**< Handle of the current connection. Set with @ref ble_ancs_c_handles_assign when connected. */
    ble_ancs_c_service_t     service;                                 /**< Struct to store the different handles and UUIDs related to the service. */
    ble_ancs_c_attr_list_t   ancs_attr_list[BLE_ANCS_NB_OF_ATTRS];    /**< For all attributes; contains whether they should be requested upon attribute request and the length and buffer of where to store attribute data. */
    uint32_t                 number_of_requested_attr;                /**< The number of attributes that will be requested upon a iOS notification attribute request is made. */
    uint32_t                 expected_number_of_attrs;                /**< The number of attributes expected upon receiving attributes. Keeps track of when to stop reading incoming attributes. */
    ble_ancs_c_parse_state_t parse_state;                             /**< ANCS notification attribute parsing state.  */
    uint8_t                * p_data_dest;                             /**< Attribute that the parsed data will be copied into. */
    uint16_t                 current_attr_index;                      /**< Variable to keep track of how much (for a given attribute) we are done parsing. */
    ble_ancs_c_evt_t         evt;                                     /**< The event is filled with several iteration of the parse_get_notif_attrs_response function when requesting iOS notification attributes. So we must allocate memory for it here.*/
} ble_ancs_c_t;


/**@brief Apple Notification client init structure, which contains all options and data needed for
 *        initialization of the client.*/
typedef struct
{
    ble_ancs_c_evt_handler_t evt_handler;    /**< Event handler to be called for handling events in the Battery Service. */
    ble_srv_error_handler_t  error_handler;  /**< Function to be called in case of an error. */
} ble_ancs_c_init_t;


/**@brief Apple Notification Center Service UUIDs. */
extern const ble_uuid128_t ble_ancs_base_uuid128;     /**< Service UUID. */
extern const ble_uuid128_t ble_ancs_cp_base_uuid128;  /**< Control point UUID. */
extern const ble_uuid128_t ble_ancs_ns_base_uuid128;  /**< Notification source UUID. */
extern const ble_uuid128_t ble_ancs_ds_base_uuid128;  /**< Data source UUID. */


/**@brief Function for handling the application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack that are of interest to the ANCS client.
 *
 * @param[in] p_ancs     ANCS client structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
void ble_ancs_c_on_ble_evt(ble_ancs_c_t * p_ancs, const ble_evt_t * p_ble_evt);


/**@brief     Function for handling events from the database discovery module.
 *
 * @details   This function will handle an event from the database discovery module, and determine
 *            if it relates to the discovery of ANCS at the peer. If so, it will
 *            call the application's event handler indicating that ANCS has been
 *            discovered at the peer. It also populates the event with the service related
 *            information before providing it to the application.
 *
 * @param[in] p_ancs Pointer to the ANCS client structure.
 * @param[in] p_evt  Pointer to the event received from the database discovery module.
 */
 void ble_ancs_c_on_db_disc_evt(ble_ancs_c_t * p_ancs, ble_db_discovery_evt_t * p_evt);


/**@brief Function for initializing the ANCS client.
 *
 * @param[out] p_ancs       ANCS client structure. This structure must be
 *                          supplied by the application. It is initialized by this function
 *                          and will later be used to identify this particular client instance.
 * @param[in]  p_ancs_init  Information needed to initialize the client.
 *
 * @retval NRF_SUCCESS  If the client was initialized successfully. Otherwise, an error code is returned.
 */
uint32_t ble_ancs_c_init(ble_ancs_c_t * p_ancs, const ble_ancs_c_init_t * p_ancs_init);


/**@brief Function for writing to the CCCD to enable notifications from the Apple Notification Service.
 *
 * @param[in] p_ancs  iOS notification structure. This structure must be supplied by
 *                    the application. It identifies the particular client instance to use.
 *
 * @retval NRF_SUCCESS If writing to the CCCD was successful. Otherwise, an error code is returned.
 */
uint32_t ble_ancs_c_notif_source_notif_enable(const ble_ancs_c_t * p_ancs);


/**@brief Function for writing to the CCCD to enable data source notifications from the ANCS.
 *
 * @param[in] p_ancs iOS notification structure. This structure must be supplied by
 *                   the application. It identifies the particular client instance to use.
 *
 * @retval NRF_SUCCESS If writing to the CCCD was successful. Otherwise, an error code is returned.
 */
uint32_t ble_ancs_c_data_source_notif_enable(const ble_ancs_c_t * p_ancs);


/**@brief Function for writing to the CCCD to disable notifications from the ANCS.
 *
 * @param[in] p_ancs  iOS notification structure. This structure must be supplied by
 *                    the application. It identifies the particular client instance to use.
 *
 * @retval NRF_SUCCESS If writing to the CCCD was successful. Otherwise, an error code is returned.
 */
uint32_t ble_ancs_c_notif_source_notif_disable(const ble_ancs_c_t * p_ancs);


/**@brief Function for writing to the CCCD to disable data source notifications from the ANCS.
 *
 * @param[in] p_ancs  iOS notification structure. This structure must be supplied by
 *                    the application. It identifies the particular client instance to use.
 *
 * @retval NRF_SUCCESS If writing to the CCCD was successful. Otherwise, an error code is returned.
 */
uint32_t ble_ancs_c_data_source_notif_disable(const ble_ancs_c_t * p_ancs);


/**@brief Function for registering attributes that will be requested if ble_ancs_c_request_attrs
 *        is called.
 *
 * @param[in] p_ancs ANCS client instance on which the attribute will be registered.
 * @param[in] id     ID of the attribute that will be added.
 * @param[in] p_data Pointer to a buffer where the data of the attribute can be stored.
 * @param[in] len    Length of the buffer where the data of the attribute can be stored.
  
 * @retval NRF_SUCCESS If all operations were successful. Otherwise, an error code is returned.
 */
uint32_t ble_ancs_c_attr_add(ble_ancs_c_t                          * p_ancs,
                             const ble_ancs_c_notif_attr_id_values_t id,
                             uint8_t                               * p_data,
                             const uint16_t                          len);


/**@brief Function for requesting attributes for a notification.
 *
 * @param[in] p_ancs   iOS notification structure. This structure must be supplied by
 *                     the application. It identifies the particular client instance to use.
 * @param[in] p_notif  Pointer to the notification whose attributes will be requested from
 *                     the Notification Provider.
 *
 * @retval NRF_SUCCESS If all operations were successful. Otherwise, an error code is returned.
 */
uint32_t ble_ancs_c_request_attrs(ble_ancs_c_t                 * p_ancs,
                                  const ble_ancs_c_evt_notif_t * p_notif);


/**@brief Function for assigning handle to a this instance of ancs_c.
 *
 * @details Call this function when a link has been established with a peer to
 *          associate this link to this instance of the module. This makes it 
 *          possible to handle several link and associate each link to a particular
 *          instance of this module. The connection handle and attribute handles will be
 *          provided from the discovery event @ref BLE_ANCS_C_EVT_DISCOVERY_COMPLETE.
 *
 * @param[in] p_ancs      Pointer to the ANCS client structure instance to associate with these
 *                        handles.
 * @param[in] conn_handle Connection handle to associated with the given ANCS Instance.
 * @param[in] p_service   Attribute handles on the ANCS server that you want this ANCS client to
 *                        interact with.
 *
 * @retval    NRF_SUCCESS    If the operation was successful.
 * @retval    NRF_ERROR_NULL If a p_ancs was a NULL pointer.
 */
uint32_t ble_ancs_c_handles_assign(ble_ancs_c_t * p_ancs, const uint16_t conn_handle, const ble_ancs_c_service_t * p_service);

#endif // BLE_ANCS_C_H__

/** @} */

