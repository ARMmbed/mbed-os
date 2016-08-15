/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
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
 * @file device_manager.h
 *
 * @defgroup device_manager Device Manager
 * @ingroup ble_sdk_lib
 * @{
 * @brief Device Manager Application Interface Abstraction.
 *
 * @details The Device Manager module manages Active and Bonded Peers. Management of peer includes
 *          book keeping of contextual information like the Security Keys, GATT
 *          configuration and any application specific information.
 *
 *          Active Peers are devices which are connected, and may or may not be bonded.
 *          Bonded Peers are devices which are bonded, and may or may not be Active (Connected).
 *          Active Bonded Peer refers to a device which is connected and bonded.
 *
 *          Paired Devices refers to peer devices that are connected and have necessary context
 *          establishment/exchange for the current connection session. On disconnect,
 *          all contextual information is flushed. For example, SMP Information Exchanged during
 *          pairing and GATT Configuration is not retained on disconnection.
 *
 *          Note that this module allows management of contextual information but 
 *          does not provide an interface for connection management. Therefore, entering connectible
 *          mode, connection establishment, or disconnection of a link with peer is not in scope
 *          of this module.
 *
 *          For bonded peers, the contextual information is required to be retained on disconnection
 *          and power cycling. Persistent storage of contextual information is handled by the
 *          module. This module categorizes the contextual information into 3 categories:
 *             - <b>Bonding Information</b>
 *               Bond information is the information exchanged between local and peer device to
 *               establish a bond. It also includes peer identification information,
 *               like the peer address or the IRK or both. From here on this category of information
 *               is referred to as Device Context.
 *             - <b>Service/Protocol Information</b>
 *               Service/Protocol information is the information retained for the peer to save on one-time
 *               procedures like the GATT Service Discovery procedures and Service Configurations.
 *               It allows devices to resume data exchange on subsequent reconnection without having
 *               to perform initial set-up procedures each time. From here on this category is
 *               referred to as Service Context.
 *             - <b>Application Information</b>
 *               Application information is the context that the application would like to associate with
 *               each of the bonded device. For example, if the application chooses to rank its peers
 *               in order to manage them better, the rank information could be treated as
 *               Application Information. This storage space is provided to save the application from
 *               maintaining a mapping table with each Device Instance and Application Information.
 *               However, if the application have no use for this, it is possible to not
 *               use or employ this at compile time. From here on this category of information is
 *               referred to as Application Context.
 */


#ifndef DEVICE_MANAGER_H__
#define DEVICE_MANAGER_H__

#include <stdint.h>
#include <stdbool.h>
#include "sdk_common.h"
#include "nrf_ble.h"
#include "ble_gap.h"
#include "device_manager_cnfg.h"

/**
 * @defgroup dm_service_cntext_types Service/Protocol Types
 *
 * @brief Describes the possible types of Service/Protocol Contexts for a bonded/peer device.
 *
 * @details Possible Service/Protocol context per peer device. The Device Manager provides the
 *          functionality of persistently storing the Service/Protocol context and can automatically
 *          load them when needed.
 *          For example system attributes for a GATT Server. Based on the nature of the application, 
 *          not all service types may be needed. The application can specify
 *          only the service/protocol context it wants to use at the time of registration.
 * @{
 */
#define DM_PROTOCOL_CNTXT_NONE         0x00  /**< No Service Context, this implies the application does not want to associate any service/protocol context with the peer device */
#define DM_PROTOCOL_CNTXT_GATT_SRVR_ID 0x01  /**< GATT Server Service Context, this implies the application does associate GATT Server with the peer device and this information will be loaded when needed for a bonded device */
#define DM_PROTOCOL_CNTXT_GATT_CLI_ID  0x02  /**< GATT Client Service Context, this implies the application does associate GATT Client with the peer device and this information will be loaded when needed for a bonded device */
#define DM_PROTOCOL_CNTXT_ALL                                                                     \
        (DM_PROTOCOL_CNTXT_GATT_SRVR_ID | DM_PROTOCOL_CNTXT_GATT_CLI_ID) /**< All Service/Protocol Context, this implies that the application wants to associate all Service/Protocol Information with the bonded device. This is configurable based on system requirements. If the application has only one type of service, this define could be altered to reflect the same.  */
/** @} */


/**
 * @defgroup dm_events Device Manager Events
 *
 * @brief This section describes the device manager events that are notified to the application.
 *
 * @details The Device Manager notifies the application of various asynchronous events using the
 *          asynchronous event notification callback. All events has been categorized into:
 *          a. General.
 *          b. Link Status.
 *          c. Context Management.
 *
 *          In the callback, these events are notified along with handle that uniquely identifies:
 *          application instance, active instance (if applicable), device instance
 *          bonding instance, (if applicable) and service instance.
 *          Not all events are pertaining to an active connection, for example a context deletion event could occur even if the peer
 *          is not connected. Also, general category of events may not be pertaining to any specific peer.
 *          See also \ref dm_event_cb_t and \ref dm_register.
 * @{
 */
/**
 * @defgroup general_events General Events
 *
 * @brief General or miscellaneous events.
 *
 * @details This category of events are general events not pertaining to a peer or context.
 *
 * @{
 */
#define DM_EVT_RFU   0x00 /**< Reserved for future use, is never notified. */
#define DM_EVT_ERROR 0x01 /**< Device Manager Event Error. */
/** @} */

/**
 * @defgroup link_status_events Link Status Events
 *
 * @brief Link Status Events.
 *
 * @details This category of events notify the application of the link status. Event result associated
 *          with the event is provided along with the event in the callback to provide more details of
 *          whether a procedure succeeded or failed and assist the application in decision making of
 *          how to proceed. For example if a DM_DEVICE_CONNECT_IND is indicated with NRF_SUCCESS
 *          result, the application may want to proceed with discovering and association with
 *          service of the peer. However, if indicated with a failure result, the application may
 *          want to take an alternate action such as reattempting to connect or go into a
 *          sleep mode.
 *
 * @{
 */
#define DM_EVT_CONNECTION              0x11 /**< Indicates that link with the peer is established. */
#define DM_EVT_DISCONNECTION           0x12 /**< Indicates that link with peer is torn down. */
#define DM_EVT_SECURITY_SETUP          0x13 /**< Security procedure for link started indication */
#define DM_EVT_SECURITY_SETUP_COMPLETE 0x14 /**< Security procedure for link completion indication. */
#define DM_EVT_LINK_SECURED            0x15 /**< Indicates that link with the peer is secured. For bonded devices, subsequent reconnections with bonded peer will result only in this event when the link is secured and setup procedures will not occur unless the bonding information is either lost or deleted on either or both sides.  */
#define DM_EVT_SECURITY_SETUP_REFRESH  0x16 /**< Indicates that the security on the link was re-established. */
/** @} */

/**
 * @defgroup context_mgmt_events Context Management Events
 *
 * @brief Context Management Events.
 *
 * @details These events notify the application of the status of context loading and storing.
 *
 * @{
 */
#define DM_EVT_DEVICE_CONTEXT_LOADED   0x21 /**< Indicates that device context for a peer is loaded. */
#define DM_EVT_DEVICE_CONTEXT_STORED   0x22 /**< Indicates that device context is stored persistently. */
#define DM_EVT_DEVICE_CONTEXT_DELETED  0x23 /**< Indicates that device context is deleted. */
#define DM_EVT_SERVICE_CONTEXT_LOADED  0x31 /**< Indicates that service context for a peer is loaded. */
#define DM_EVT_SERVICE_CONTEXT_STORED  0x32 /**< Indicates that service context is stored persistently. */
#define DM_EVT_SERVICE_CONTEXT_DELETED 0x33 /**< Indicates that service context is deleted. */
#define DM_EVT_APPL_CONTEXT_LOADED     0x41 /**< Indicates that application context for a peer is loaded. */
#define DM_EVT_APPL_CONTEXT_STORED     0x42 /**< Indicates that application context is stored persistently. */
#define DM_EVT_APPL_CONTEXT_DELETED    0x43 /**< Indicates that application context is deleted. */
/** @} */
/** @} */

#define DM_INVALID_ID 0xFF /**< Invalid instance idenitifer. */

/**
 * @defgroup dm_data_structure Device Manager Data Types
 *
 * @brief This section describes all the data types exposed by the module to the application.
 * @{
 */

/**
 * @brief Application Instance.
 *
 * @details Application instance uniquely identifies an application. The identifier is allocated by
 *          the device manager when application registers with the module. The application is
 *          expected to identify itself with this instance identifier when initiating subsequent
 *          requests. Application should use the utility API \ref dm_application_instance_set in
 *          order to set its application instance in dm_handle_t needed for all subsequent APIs.
 *          See also \ref dm_register.
 */
typedef uint8_t dm_application_instance_t;

/**
 * @brief Connection Instance.
 *
 * @details Identifies connection instance for an active device. This instance is allocated by the 
 *          device manager when a connection is established and is notified with DM_EVT_CONNECTION
 *          with the event result NRF_SUCCESS.
 */
typedef uint8_t dm_connection_instance_t;

/**
 * @brief Device Instance.
 *
 * @details Uniquely identifies a bonded peer device. The peer device may or may not be connected.
 *          In case of the central: The bonded device instance to identify the peer is allocated when bonding procedure is initiated by the central using dm_security_setup_req.
 *          In case of the peripheral: When the bonding procedure is successful, the DM_EVT_SECURITY_SETUP_COMPLETE event with success event result, is received.
 *          In case the module cannot add more bonded devices, no instance is allocated, this is indicated by an appropriate error code for the API/event as the case may be. Application can choose to disconnect the link.
 */
typedef uint8_t dm_device_instance_t;

/**
 * @brief Service Instance.
 *
 * @details Uniquely identifies a peer device. The peer device may or may not be connected. This
 *          instance is allocated by the device manager when a device is bonded and is notified
 *          when security procedures have been initiated.
 *          Security Procedures initiation is notified with DM_SECURITY_SETUP_IND with
 *          success event result. In case the event result indicates that the module cannot add more
 *          bonded devices, no instance is allocated. Application can chose to disconnect the link.
 */
typedef uint8_t dm_service_instance_t;

/**
 * @brief Service/Protocol Type Identifier.
 *
 * @details Uniquely identifies a service or a protocol type. Service/Protocol Type identification
 *          is needed as each service/protocol can have its own contextual data.
 *          This allows the peer to access more than one service at a time. \ref dm_service_cntext_types describes the
 *          list of services/protocols supported.
 */
typedef uint8_t service_type_t;

/**@brief Device Manager Master identification and encryption information. */
typedef struct dm_enc_key
{
    ble_gap_enc_info_t  enc_info;  /**< GAP encryption information. */
    ble_gap_master_id_t master_id; /**< Master identification. */
} dm_enc_key_t;

/** @brief Device Manager identity and address information. */
typedef struct dm_id_key
{
  ble_gap_irk_t  id_info;      /**< Identity information. */
  ble_gap_addr_t id_addr_info; /**< Identity address information. */
} dm_id_key_t;

/** @brief Device Manager signing information. */
typedef struct dm_sign_key
{
    ble_gap_sign_info_t sign_key; /**< GAP signing information. */
} dm_sign_key_t;

/** @brief Security keys. */
typedef struct dm_sec_keyset
{
    union 
    {
        dm_enc_key_t       * p_enc_key;  /**< Pointer to Device Manager encryption information structure. */
    } enc_key;
    dm_id_key_t   * p_id_key;            /**< Identity key, or NULL. */
    dm_sign_key_t * p_sign_key;          /**< Signing key, or NULL. */
} dm_sec_keys_t;

/** @brief Device Manager security key set. */
typedef struct
{
  dm_sec_keys_t keys_periph;  /**< Keys distributed by the device in the Peripheral role. */
  dm_sec_keys_t keys_central; /**< Keys distributed by the device in the Central role. */
} dm_sec_keyset_t;

/**
 * @brief Device Handle used for unique identification of each peer.
 *
 * @details This data type is used to uniquely identify each peer device. A peer device could be
 *          active and/or bonded. Therefore an instance for active and bonded is provided.
 *          However, the application is expected to treat this is an opaque structure and use this for
 *          all API interactions once stored on appropriate events.
 *          See \ref dm_events.
 */
typedef struct device_handle
{
    dm_application_instance_t    appl_id;       /**< Identifies the application instances for the device that is being managed. */
    dm_connection_instance_t     connection_id; /**< Identifies the active connection instance. */
    dm_device_instance_t         device_id;     /**< Identifies peer instance in the data base. */
    dm_service_instance_t        service_id;    /**< Service instance identifier. */
} dm_handle_t;

/**
 * @brief Definition of Data Context.
 *
 * @details Defines contextual data format, it consists of context data length and pointer to data.
 */
typedef struct
{
    uint32_t  flags;  /**< Additional flags identifying data. */
    uint32_t  len;    /**< Length of data. */
    uint8_t * p_data; /**< Pointer to contextual data, a copy is made of the data. */
} dm_context_t;


/**
 * @brief Device Context.
 *
 * @details Defines "device context" type for a device managed by device manager.
 */
typedef dm_context_t dm_device_context_t;

/**
 * @brief Service Context.
 *
 * @details Service context data for a service identified by the 'service_type' field.
 */
typedef struct
{
    service_type_t service_type; /**< Identifies the service/protocol to which the context data is related. */
    dm_context_t   context_data; /**< Contains length and pointer to context data */
} dm_service_context_t;

/**
 * @brief Application context.
 *
 * @details The application context can be used by the application to map any application level
 *          information that is to be mapped with a particular peer.
 *          For bonded peers, this information will be stored by the bond manager persistently.
 *          Note that the device manager treats this information as an
 *          opaque block of bytes.
 *          Necessary APIs to get and set this context for a peer have been provided.
 */
typedef dm_context_t dm_application_context_t;

/**
 * @brief Event parameters.
 *
 * @details Defines event parameters for each of the events notified by the module.
 */
typedef union
{
    ble_gap_evt_t            * p_gap_param;       /**< All events that are triggered in device manager as a result of GAP events, like connection, disconnection and security procedures are accompanied with GAP parameters. */
    dm_application_context_t * p_app_context;     /**< All events that are associated with application context procedures of store, load, and deletion have this as event parameter. */
    dm_service_context_t     * p_service_context; /**< All events that are associated with service context procedures of store, load and deletion have this as event parameter. */
    dm_device_context_t      * p_device_context;  /**< All events that are associated with device context procedures of store, load and deletion have this as event parameter. */
} dm_event_param_t;

/**
 * @brief Asynchronous events details notified to the application by the module.
 *
 * @details Defines event type along with event parameters notified to the application by the
 *          module.
 */
typedef struct
{
    uint8_t          event_id;       /**< Identifies the event. See \ref dm_events for details on event types and their significance. */
    dm_event_param_t event_param;    /**< Event parameters. Can be NULL if the event does not have any parameters. */
    uint16_t         event_paramlen; /**< Length of the event parameters, is zero if the event does not have any parameters. */
} dm_event_t;

/**
 * @brief Event notification callback registered by application with the module.
 *
 * @details Event notification callback registered by application with the module when registering
 *          the module using \ref dm_register API.
 *
 * @param[in] p_handle   Identifies the peer for which the event is being notified.
 * @param[in] p_event    Identifies the event, any associated parameters and parameter length.
 *                       See \ref dm_events for details on event types and their significance.
 * @param[in,out] event_result   Provide additional information on the event. 
 *                      In addition to SDK error codes there is also a return value
 *                      indicating if maximum number of connections has been reached when connecting or bonding.
 *
 * @retval NRF_SUCCESS on success, or a failure to indicate if it could handle the event
 *         successfully. There is no action taken in case application returns a failure.
 */
typedef ret_code_t (*dm_event_cb_t)(dm_handle_t const * p_handle,
                                    dm_event_t const  * p_event,
                                    ret_code_t        event_result);

/**
 * @brief Initialization Parameters.
 *
 * @details Indicates the application parameters. Currently this only encompasses clearing
 *          all persistent data.
 */
typedef struct
{
    bool clear_persistent_data; /**< Set to true in case the module should clear all persistent data. */
} dm_init_param_t;

/**
 * @brief Application Registration Parameters.
 *
 * @details Parameters needed by the module when registering with it.
 */
typedef struct
{
    dm_event_cb_t        evt_handler;  /**< Event Handler to be registered. It will receive asynchronous notification from the module, see \ref dm_events for asynchronous events. */
    uint8_t              service_type; /**< Bit mask identifying services that the application intends to support for all peers. */
    ble_gap_sec_params_t sec_param;    /**< Security parameters to be used for the application. */
} dm_application_param_t;

/**
 * @brief Defines possible security status/states.
 *
 * @details Defines possible security status/states of a link when requested by application using
 *          the \ref dm_security_status_req.
 */
typedef enum
{
    NOT_ENCRYPTED,          /**< The link is not secured. */
    ENCRYPTION_IN_PROGRESS, /**< Link security is being established.*/
    ENCRYPTED               /**< The link is secure.*/
} dm_security_status_t;
/** @} */

/**
 * @defgroup dm_api Device Module APIs
 *
 * @brief This section describes APIs exposed by the module.
 *
 * @details This section describes APIs exposed by the module. The APIs have been categorized to provide
 *          better and specific look up for developers. Categories are:
 *          - Set up APIs.
 *          - Context Management APIs.
 *          - Utility APIs.
 *
 *          MSCs describe usage of these APIs.  
 *          See @ref dm_msc.
 * @{
 */
/**
 * @defgroup dm_setup_api Device Module Set-up APIs
 *
 * @brief Initialization & registration APIs that are pre-requisite for all other module procedures.
 * @details This section describes the Module Initialization and Registration APIs needed to be set up by
 *          the application before device manager can start managing devices and device contexts
 *          for the application.
 *
 * @{
 */

/**
 * @brief Module Initialization Routine.
 *
 * @details Function for initializing the module. Must called before any other APIs of the module are used.
 *
 * @param[in] p_init_param Initialization parameters.
 *
 * @retval NRF_SUCCESS On success, else an error code indicating reason for failure.
 *
 * @note It is mandatory that pstorage is initialized before initializing this module.
 */
ret_code_t dm_init(dm_init_param_t const * p_init_param);

/**
 * @brief Function for registering the application.
 *
 * @details This routine is used by the application to register for asynchronous events with the
 *          device manager. During registration the application also indicates the services that it
 *          intends to support on this instance. It is possible to register multiple times with the
 *          device manager. At least one instance shall be registered with the device manager after
 *          the module has been initialized.
 *          Maximum number of application instances device manager can support is determined
 *          by DM_MAX_APPLICATIONS.
 *
 *          All applications must be registered before initiating or accepting connections from the peer.     
 *
 * @param[in]  p_appl_param    Application parameters.
 * @param[out] p_appl_instance Application Instance Identifier in case registration is successful.
 *
 * @retval NRF_SUCCESS             On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE If the API is called without module initialization.
 * @retval NRF_ERROR_NO_MEM        If module cannot support more applications.
 *
 * @note Currently only one application instance is supported by the module.
 */
ret_code_t dm_register(dm_application_instance_t    * p_appl_instance,
                       dm_application_param_t const * p_appl_param);

/**
 * @brief Function for handling BLE events.
 *
 * @details BLE Event Handler for the module. This routine should be called from BLE stack event
 *          dispatcher for the module to work as expected.
 *
 * @param[in] p_ble_evt BLE stack event being dispatched to the function.
 *
 */
void dm_ble_evt_handler(ble_evt_t * p_ble_evt);

/** @} */


/**
 * @defgroup dm_security_api APIs to set up or read status of security on a link.
 *
 * @brief This section describes APIs to set up Security. These APIs require that the peer is
 *        connected before the procedures can be requested.
 *
 * @details This group allows application to request security procedures
 *          or get the status of the security on a link.
 * @{
 */
/**
 * @brief Function for requesting setting up security on a link.
 *
 * @details This API initiates security procedures with a peer device.
 *          @note For the GAP Central role, in case peer is not bonded, request to bond/pair is
 *          initiated. If it is bonded, the link is re-encrypted using the existing bond information.
 *          For the GAP peripheral role, a Slave security request is sent.
 * @details If a pairing procedure is initiated successfully, application is notified of
 *          @ref DM_EVT_SECURITY_SETUP_COMPLETE. A result indicating success or failure is notified along with the event.
 *          In case the link is re-encrypted using existing bond information, @ref DM_EVT_LINK_SECURED is
 *          notified to the application.
 *
 * @param[in] p_handle Identifies the link on which security is desired.
 *
 * @retval NRF_SUCCESS             On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE If the API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If p_handle is NULL.
 * @retval NRF_ERROR_INVALID_ADDR  If the peer is not identified by the handle provided by the application
 *                                 or if the peer is not connected when this procedure is requested.
 */
ret_code_t dm_security_setup_req(dm_handle_t * p_handle);

/**
 * @brief Function for reading the status of the security on a link.
 *
 * @details This API allows application to query status of security on a link.
 *
 * @param[in]  p_handle  Identifies the link on which security is desired.
 * @param[out] p_status  Pointer where security status is provided to the application.
 *                       See \ref dm_security_status_t for possible statuses that can be expected.
 *
 * @retval NRF_SUCCESS             Or appropriate error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE If the API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If p_handle or p_status is NULL.
 * @retval NRF_ERROR_INVALID_ADDR  If peer is not identified by the handle provided by the application
 *                                 or if peer is not connected when this procedure is requested.
 */
ret_code_t dm_security_status_req(dm_handle_t const * p_handle, dm_security_status_t * p_status);

/**
 * @brief Function for creating the whitelist.
 *
 * @details This API allows application to create whitelist based on bonded peer devices in module
 *          data base.
 *
 * @param[in]  p_handle       Identifies the application requesting whitelist creation.
 * @param[in,out] p_whitelist Pointer where created whitelist is provided to the application.
 *
 * @note 'addr_count' and 'irk_count' fields of the structure should be populated with the maximum
 *       number of devices that the application wishes to request in the whitelist. 
 *       If the number of bonded devices is less than requested, the fields are updated with that number of devices.
 *       If the number of devices are more than requested, the module will populate the list
 *       with devices in the order the bond was established with the peer devices. Also, if this routine is
 *       called when a connection exists with one or more peer devices,
 *       those connected devices are not added to the whitelist.
 *
 * @retval NRF_SUCCESS             On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE If the API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If p_handle or p_whitelist is NULL.
 */
ret_code_t dm_whitelist_create(dm_application_instance_t const * p_handle,
                               ble_gap_whitelist_t             * p_whitelist);

/** @} */


/**
 * @defgroup dm_cntxt_mgmt_api Context Management APIs
 *
 * @brief Utility APIs offered by the device manager to get information about the peer if and
 *        when needed.
 *
 * @details This group of API allow the application to access information that is not required to be
 *          maintained by the application but may be needed. Hence it is possible to get the
 *          information from the module instead of mapping all the information with a device
 *          context.
 * @{
 */

ret_code_t dm_device_add(dm_handle_t               * p_handle,
                         dm_device_context_t const * p_context);

/**
 * @brief Function for deleting a peer device context and all related information from the database.
 *
 * @details Delete peer device context and all related information from database. If
 *          this API returns NRF_SUCCESS, DM_EVT_DEVICE_CONTEXT_DELETED event is notified to the
 *          application. Event result notified along with the event indicates success or failure
 *          of this procedure.
 *
 * @param[in] p_handle Identifies the peer device to be deleted.
 *
 * @retval NRF_SUCCESS             on success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE In the API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If p_handle is NULL.
 * @retval NRF_ERROR_INVALID_ADDR  If peer is not identified the handle provided by the application.
 *
 * @note Deleting device context results in deleting service and application context for the
 *       bonded device. The respective events DM_EVT_SERVICE_CONTEXT_DELETED and
 *       DM_EVT_APPL_CONTEXT_DELETED are not notified to the application.
 */
ret_code_t dm_device_delete(dm_handle_t const * p_handle);

/**
 * @brief Function for deleting all peer device context and all related information from the database.
 *
 * @details Delete peer device context and all related information from database. If
 *          this API returns NRF_SUCCESS, DM_EVT_DEVICE_CONTEXT_DELETED event is notified to the
 *          application for each device that is deleted from the data base. Event result
 *          notified along with the event indicates success or failure of this procedure.
 *
 * @param[in] p_handle Identifies application instance that is requesting
 *                     the deletion of all bonded devices.
 *
 * @retval NRF_SUCCESS             On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE If the API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If p_handle is NULL.
 * @retval NRF_ERROR_INVALID_ADDR  If peer is not identified the handle provided by the application.
 *
 * @note Deleting device context results in deleting both service and application context for the
 *       bonded device. The respective events DM_EVT_SERVICE_CONTEXT_DELETED and
 *       DM_EVT_APPL_CONTEXT_DELETED are not notified to the application.
 */
ret_code_t dm_device_delete_all(dm_application_instance_t const * p_handle);

/**
 * @brief Function for setting Service Context for a peer device identified by 'p_handle' parameter.
 *
 * @details This API allows application to Set Service Context for a peer device identified by the
 *          'p_handle' parameter. This API is useful when the Service Context cannot be requested
 *          from the SoftDevice, but needs to be assembled by the application or an another module.
 *          (or when service context is exchanged in an out of band way.)
 *          This API could also be used to trigger a storing of service context into persistent
 *          memory. If this is desired, a NULL pointer could be passed to the p_context.
 *
 * @param[in] p_handle  Identifies peer device for which the procedure is requested.
 * @param[in] p_context Service context being set. The context information includes length of
 *                      data and pointer to the contextual data being set. The memory pointed to by
 *                      the pointer to data is assumed to be resident when API is being called and
 *                      can be freed or reused once the set procedure is complete. Set procedure
 *                      completion is indicated by the event \ref DM_EVT_SERVICE_CONTEXT_STORED.
 *                      The Event result is notified along with the event and indicates success or failure of
 *                      this procedure.
 *
 * @retval NRF_SUCCESS             On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE If the API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If p_handle is NULL.
 * @retval NRF_ERROR_INVALID_ADDR  If the peer is not identified by the handle provided by the application.
 */
ret_code_t dm_service_context_set(dm_handle_t const          * p_handle,
                                  dm_service_context_t const * p_context);

/**
 * @brief Function for getting Service Context for a peer device identified by 'p_handle' parameter.
 *
 * @details Get Service Context for a peer device identified by the 'p_handle' parameter. If
 *          this API returns NRF_SUCCESS, DM_EVT_SERVICE_CONTEXT_LOADED event is notified to the
 *          application. The event result is notified along with the event indicates success or failure
 *          of this procedure.
 *
 * @param[in] p_handle  Identifies peer device for which procedure is requested.
 * @param[in] p_context Application context being requested. The context information includes length
 *                      of the data and a pointer to the data. Note that requesting a 'get'
 *                      of application does not need to provide memory, the pointer to data will be
 *                      pointing to service data and hence no data movement is involved.
 *
 * @retval NRF_SUCCESS             On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE In case API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If p_handle is NULL.
 * @retval NRF_ERROR_INVALID_ADDR  If the peer is not identified by the handle provided by the application.
 */
ret_code_t dm_service_context_get(dm_handle_t const    * p_handle,
                                  dm_service_context_t * p_context);

/**
 * @brief Function for deleting a Service Context for a peer device identified by the 'p_handle' parameter.
 *
 * @details This API allows application to delete a Service Context identified for a peer device
 *          identified by the 'p_handle' parameter. If this API returns NRF_SUCCESS,
 *          DM_EVT_SERVICE_CONTEXT_DELETED event is notified to the application. 
 *          Event result is notified along with the event and indicates success or failure of this
 *          procedure.
 *
 * @param[in] p_handle Identifies peer device for which procedure is requested.
 *
 * @retval NRF_SUCCESS             On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE If the API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If p_handle is NULL.
 * @retval NRF_ERROR_INVALID_ADDR  If the peer is not identified by the handle provided by the application.
 */
ret_code_t dm_service_context_delete(dm_handle_t const * p_handle);

/**
 * @brief Function for setting Application Context for a peer device identified by the 'p_handle' parameter.
 *
 * @details This application allows the setting of the application context for the peer device identified by
 *          the 'p_handle'. Application context is stored persistently by the module and can be
 *          requested by the application at any time using the \ref dm_application_context_get
 *          API. Note that this procedure is permitted only for bonded devices. If the
 *          device is not bonded, application context cannot be set. However, it is not mandatory
 *          that the bonded device is connected when requesting this procedure.
 *
 * @param[in] p_handle  Identifies peer device for which procedure is requested.
 *
 * @param[in] p_context Application context being set. The context information includes length of the
 *                      data and pointer to the contextual data being set. The memory pointed to by
 *                      the data pointer is assumed to be resident when API is being called and
 *                      can be freed or reused once the set procedure is complete. Set procedure
 *                      completion is notified by the event \ref DM_EVT_APPL_CONTEXT_STORED.
 *                      The event result is notified along with the event and indicates success or
 *                      failure of this procedure.
 *
 * @retval NRF_SUCCESS             On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE If the API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If p_handle and/or p_context is NULL.
 * @retval NRF_ERROR_INVALID_ADDR  If peer is not identified the handle provided by the application.
 *
 * @note The API returns FEATURE_NOT_ENABLED in case DEVICE_MANAGER_APP_CONTEXT_SIZE is set to zero.
 */
ret_code_t dm_application_context_set(dm_handle_t const              * p_handle,
                                      dm_application_context_t const * p_context);

/**
 * @brief Function for getting Application Context for a peer device identified by the 'p_handle' parameter.
 *
 * @details Get Application Context for a peer device identified by the 'p_handle' parameter. If
 *          this API returns NRF_SUCCESS, DM_EVT_APPL_CONTEXT_LOADED event is notified to the
 *          application. Event result notified along with the event indicates success or failure
 *          of this procedure.
 *
 * @param[in] p_handle  Identifies peer device for which procedure is requested.
 * @param[in] p_context Application context being requested. The context information includes
 *                      length of data and pointer to the contextual data is provided.
 *
 * @retval NRF_SUCCESS             On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE If the API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If p_handle and/or p_context is NULL.
 * @retval NRF_ERROR_INVALID_ADDR  If the peer is not identified by the handle provided by the application.
 * @retval DM_NO_APP_CONTEXT       If no application context was set that can be fetched.
 *
 * @note The API returns FEATURE_NOT_ENABLED in case DEVICE_MANAGER_APP_CONTEXT_SIZE is set to
 *       zero.
 */
ret_code_t dm_application_context_get(dm_handle_t const        * p_handle,
                                      dm_application_context_t * p_context);

/**
 * @brief Function for deleting Application Context for a peer device identified by the 'p_handle' parameter.
 *
 * @details Delete Application Context for a peer device identified by the 'p_handle' parameter. If
 *          this API returns NRF_SUCCESS, DM_EVT_APPL_CONTEXT_DELETED event is notified to the
 *          application. The event result notified along with the event and indicates success or failure
 *          of this procedure.
 *
 * @param[in] p_handle Identifies peer device for which procedure is requested.
 *
 * @retval NRF_SUCCESS             On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE If the API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If the p_handle is NULL.
 * @retval NRF_ERROR_INVALID_ADDR  If peer is not identified the handle provided by the application.
 * @retval DM_NO_APP_CONTEXT       If no application context was set that can be deleted.
 *
 * @note The API returns FEATURE_NOT_ENABLED if the DEVICE_MANAGER_APP_CONTEXT_SIZE is set to zero.
 */
ret_code_t dm_application_context_delete(dm_handle_t const * p_handle);

/** @} */


/**
 * @defgroup utility_api Utility APIs
 * @{
 * @brief This section describes the utility APIs offered by the module.
 *
 * @details APIs defined in this section are utility or assisting/helper APIs.
 */
/**
 * @brief Function for Setting/Copying Application instance to Device Manager handle.
 *
 * @param[in]  p_appl_instance Application instance to be set.
 * @param[out] p_handle        Device Manager handle for which the instance is to be copied.
 *
 * @retval NRF_SUCCESS             On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE If the API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If p_handle and/or p_addr is NULL.
 */
ret_code_t dm_application_instance_set(dm_application_instance_t const * p_appl_instance,
                                       dm_handle_t                     * p_handle);

/**
 * @brief Function for getting a peer's device address.
 *
 * @param[in]  p_handle Identifies the peer device whose address is requested. Can not be NULL.
 * @param[out] p_addr   Pointer where address is to be copied. Can not be NULL.
 *
 * @retval NRF_SUCCESS             On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE If the API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If p_handle and/or p_addr is NULL.
 * @retval NRF_ERROR_NOT_FOUND     If the peer could not be identified.
 */
ret_code_t dm_peer_addr_get(dm_handle_t const * p_handle,
                            ble_gap_addr_t    * p_addr);

/**
 * @brief Function for setting/updating a peer's device address.
 *
 * @param[in]  p_handle Identifies the peer device whose address is requested to be set/updated.
 * @param[out] p_addr   Address to be set/updated.
 *
 * @retval NRF_SUCCESS             On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE If the API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If p_handle and/or p_addr is NULL.
 * @retval NRF_ERROR_INVALID_ADDR  If the peer is not identified by the handle provided by the application.
 * @retval NRF_ERROR_INVALID_PARAM If this procedure is requested while connected to the peer or if the address
 *                                 type was set to BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE.
 *
 * @note Setting or updating a peer's device address is permitted 
 *       only for a peer that is bonded and disconnected.
 * @note Updated address is reflected only after DM_EVT_DEVICE_CONTEXT_STORED is notified to the
 *       application for this bonded device instance. In order to avoid abnormal behaviour, it is
 *       recommended to not invite/initiate connections on the updated address unless this event
 *       has been notified.
 */
ret_code_t dm_peer_addr_set(dm_handle_t const    * p_handle,
                            ble_gap_addr_t const * p_addr);

/**
 * @brief Function for initializing Device Manager handle.
 *
 * @param[in] p_handle Device Manager handle to be initialized.
 *
 * @retval NRF_SUCCESS    On success.
 * @retval NRF_ERROR_NULL If p_handle is NULL.
 *
 * @note This routine is permitted before initialization of the module.
 */
ret_code_t dm_handle_initialize(dm_handle_t * p_handle);

/**
 * @brief Function for getting distributed keys for a device.
 *
 * @param[in]  p_handle   Device Manager handle identifying the peer.
 * @param[out] p_key_dist Pointer to distributed keys.
 *
 * @retval NRF_SUCCESS             On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_INVALID_STATE If the API is called without module initialization and/or
 *                                 application registration.
 * @retval NRF_ERROR_NULL          If the p_handle and/or p_key_dist pointer is NULL.
 * @retval NRF_ERROR_INVALID_ADDR  If the peer is not identified by the handle provided by the application.
 */
ret_code_t dm_distributed_keys_get(dm_handle_t const * p_handle,
                                   dm_sec_keyset_t   * p_key_dist);

/**
 * @brief Function for getting the corresponding dm_handle_t based on the connection handle.
 *
 * @param[in]     conn_handle Connection handle as provided by the SoftDevice.
 * @param[in,out] p_handle    Pointer to the p_handle containg the application instance for the 
 *                            registered application. If the application instance is valid then 
 *                            the p_handle will be filled with requested data.
 *
 * @retval NRF_SUCCESS          On success, else an error code indicating reason for failure.
 * @retval NRF_ERROR_NULL       If the p_handle pointer is NULL.
 * @retval NRF_ERROR_NOT_FOUND  If no p_handle is found for the provided connection handle.
 */
ret_code_t dm_handle_get(uint16_t conn_handle, dm_handle_t * p_handle);

/** @} */
/** @} */
/** @} */
#endif // DEVICE_MANAGER_H__

