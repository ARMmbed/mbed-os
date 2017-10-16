/*******************************************************************************
* \file cy_ble_stack_gap_central.h
* \version 2.0
*
* \brief
*  This file contains declarations of public BLE APIs of Generic Access Profile - Central Role.
*  Also specified the defines, constants, and data structures required for the APIs.
*
*
* Related Document:
*  BLE Standard Spec - CoreV4.2, CSS, CSAs, ESR05, ESR06
*
********************************************************************************
* \copyright
* Copyright 2014-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#ifndef CY_BLE_STACK_GAP_CENTRAL_H_
#define CY_BLE_STACK_GAP_CENTRAL_H_

/***************************************
* Common stack includes
***************************************/

#include "cy_ble_stack_gap.h"


/***************************************
** GAP Constants
***************************************/

/** Observation and discovery procedure */
#define CY_BLE_GAPC_OBSER_PROCEDURE              0x00u /**< Observation procedure */
#define CY_BLE_GAPC_LTD_DISC_PROCEDURE           0x01u /**< Limited discovery procedure */
#define CY_BLE_GAPC_GEN_DISC_PROCEDURE           0x02u /**< General discovery procedure */

/** Type of discovery procedure use in cy_stc_ble_gapc_disc_info_t*/
#define CY_BLE_GAPC_PASSIVE_SCANNING             0x00u /**< passive scan */
#define CY_BLE_GAPC_ACTIVE_SCANNING              0x01u /**< active scan */
#define CY_BLE_GAPC_FILTER_DUP_DISABLE           0x00u /**< Filter duplicate disable */
#define CY_BLE_GAPC_FILTER_DUP_ENABLE            0x01u /**< Filter duplicate enable */

/** Scan filter policy */

/** Accept all advertisement packets except directed advertising packets not
  * addressed to this device
  */
#define CY_BLE_GAPC_ADV_ACCEPT_ALL_PKT                       0x00u

/** Accept only advertisement packets from devices where the advertiser's address
  * is in the whitelist.  Directed advertising packets that are not addressed for
  * this device shall be ignored.
  */
#define CY_BLE_GAPC_ADV_ACCEPT_WHITELIST_PKT                 0x01u

/** Accept all undirected advertisement packets, and directed advertising packets
  * where the initiator address is a resolvable private address, and directed advertising
  * packets addressed to this device.
  */
#define CY_BLE_GAPC_ADV_ACCEPT_DIRECTED_RPA_PKT              0x02u

/** Accept all advertisement packets from devices where the advertiser's address
  * is in the whitelist, and directed advertising packets where the initiator address
  * is a resolvable private address, and directed advertising packets addressed
  * to this device.
  */
#define CY_BLE_GAPC_ADV_ACCEPT_WHITELIST_DIRECTED_RPA_PKT    0x03u

/** Initiator filter policy*/
#define CY_BLE_GAPC_CONN_ALL                     0x00u /**< Filter policy- connect all */
#define CY_BLE_GAPC_CONN_WHITELIST               0x01u /**< Filter policy- connect only from whitelist */

/**
 \addtogroup group_ble_common_api_gap_definitions
 @{
*/

/***************************************
** Enumerated Types
***************************************/

/** Advertisement event type  */
typedef enum
{
    /** Connectable undirected advertising */
    CY_BLE_GAPC_CONN_UNDIRECTED_ADV = 0x00u,

    /** Connectable directed advertising */
    CY_BLE_GAPC_CONN_DIRECTED_ADV,

    /** Scannable undirected advertising */
    CY_BLE_GAPC_SCAN_UNDIRECTED_ADV,

    /** Non connectable undirected advertising */
    CY_BLE_GAPC_NON_CONN_UNDIRECTED_ADV,

    /** Scan Response*/
    CY_BLE_GAPC_SCAN_RSP

}cy_en_ble_gapc_adv_event_t;

/***************************************
** Exported structures and unions
***************************************/

/** Discovery (scan) parameter  */
typedef struct
{
    /** Observation and discovery procedure.
         - CY_BLE_GAPC_OBSER_PROCEDURE (Observation procedure)
         - CY_BLE_GAPC_LTD_DISC_PROCEDURE (Limited discovery procedure)
         - CY_BLE_GAPC_GEN_DISC_PROCEDURE (General discovery procedure)
     */
    uint8_t      discProcedure;

    /** Type of scan to perform
         - CY_BLE_GAPC_PASSIVE_SCANNING (Passive Scanning)
         - CY_BLE_GAPC_ACTIVE_SCANNING (Active scanning)
     */
    uint8_t      scanType;

    /** The time interval from when last LE scan is started until
       next subsequent LE scan.
        - Time Range: 2.5 ms to 10.24 sec.
     */
    uint16_t     scanIntv;

    /** The time duration of scanning to be performed
        - Time Range: 2.5 ms to 10.24 sec
     */
    uint16_t     scanWindow;

    /** Own BD Address Type
        - CY_BLE_GAP_ADDR_TYPE_PUBLIC
        - CY_BLE_GAP_ADDR_TYPE_RANDOM
        - CY_BLE_GAP_ADDR_TYPE_PUBLIC_RPA
        - CY_BLE_GAP_ADDR_TYPE_RANDOM_RPA
     */
    uint8_t      ownAddrType;

    /** Filter policies to be applied during scanning procedure
        - CY_BLE_GAPC_ADV_ACCEPT_ALL_PKT
        - CY_BLE_GAPC_ADV_ACCEPT_WHITELIST_PKT
        - CY_BLE_GAPC_ADV_ACCEPT_DIRECTED_RPA_PKT
        - CY_BLE_GAPC_ADV_ACCEPT_WHITELIST_DIRECTED_RPA_PKT
     */
    uint8_t      scanFilterPolicy;

    /** Scan timeout. Timeout is in seconds and none are zero.
       If timeout is set as 0, then there will not be any timeout.
       scanTo can be used for all GAP timeouts related to Central operation.
     */
    uint16_t     scanTo;

    /** Filter Duplicate Advertisement. The Filter Duplicates parameter controls whether
       the Link Layer shall filter duplicate advertising reports to the Host, or whether the
       Link Layer should generate advertising reports for each packet received.
        - CY_BLE_GAPC_FILTER_DUP_DISABLE (Duplicate filtering disabled)
        - CY_BLE_GAPC_FILTER_DUP_ENABLE (Duplicate filtering enabled)

       By default, duplicate filtering is enabled
     */
    uint8_t      filterDuplicates;

} cy_stc_ble_gapc_disc_info_t;

/** Connection parameter */
typedef struct
{
    /** The time interval from when last LE scan is started until next
       subsequent LE scan.
        - Time Range: 2.5 ms to 10.24 sec.
     */
    uint16_t      scanIntv;

    /** The time duration of scanning to be performed
        - Time Range: 2.5 ms to 10.24 sec
     */
    uint16_t      scanWindow;

    /** Filter policies to be applied during connection procedure
        - CY_BLE_GAPC_CONN_ALL (Whitelist is not used to determine with
        which advertiser to connect. Peer address is used)
        - CY_BLE_GAPC_CONN_WHITELIST (Whitelist is used to determine with
        which advertiser to connect. Peer address shall be ignored)
     */
    uint8_t       initiatorFilterPolicy;

    /** Peer's bd address with whom connection to be established */
    uint8_t       peerBdAddr[CY_BLE_GAP_BD_ADDR_SIZE];

    /** Peer's bd address type
        - CY_BLE_GAP_ADDR_TYPE_PUBLIC
        - CY_BLE_GAP_ADDR_TYPE_RANDOM
        - CY_BLE_GAP_ADDR_TYPE_PUBLIC_RPA
        - CY_BLE_GAP_ADDR_TYPE_RANDOM_RPA
     */
    uint8_t       peerAddrType;

    /** Own bd address type
        - CY_BLE_GAP_ADDR_TYPE_PUBLIC
        - CY_BLE_GAP_ADDR_TYPE_RANDOM
        - CY_BLE_GAP_ADDR_TYPE_PUBLIC_RPA
        - CY_BLE_GAP_ADDR_TYPE_RANDOM_RPA
    */
    uint8_t       ownAddrType;

    /** Minimum value for the connection event interval. This shall be less than
       or equal to conn_Interval_Max. Minimum connection interval will be
         connIntvMin * 1.25 ms
        * Time Range: 7.5 ms to 4 sec
     */
    uint16_t      connIntvMin;

    /** Maximum value for the connection event interval. This shall be greater
       than or equal to conn_Interval_Min. Maximum connection interval will be
         connIntvMax * 1.25 ms
        * Time Range: 7.5 ms to 4 sec
     */
    uint16_t      connIntvMax;

    /** Slave latency for the connection in number of connection events.
        * Range: 0x0000 to 0x01F3
     */
    uint16_t      connLatency;

    /** Supervision timeout for the LE Link. Supervision timeout will be
       supervisionTO * 10 ms
        * Time Range: 100 msec to 32 secs
     */
    uint16_t      supervisionTO;

    /** Minimum length of connection needed for this LE connection.
        * Range: 0x0000 - 0xFFFF
     */
    uint16_t      minCeLength;

    /** Maximum length of connection needed for this LE connection.
        * Range: 0x0000 - 0xFFFF
     */
    uint16_t      maxCeLength;

}cy_stc_ble_gapc_conn_info_t;

/** Resolve peer device parameter  */
typedef struct
{
    /** Peer Bluetooth device address */
    uint8_t      * bdAddr;

    /** Peer IRK */
    uint8_t       peerIrk[CY_BLE_GAP_SMP_IRK_SIZE];

}cy_stc_ble_gapc_resolve_peer_info_t;

/** Set peer BD Address parameter  */
typedef struct
{
    /** Peer Bluetooth device address */
    cy_stc_ble_gap_bd_addr_t  remoteAddr;

    /** Peer bdHandle */
    uint8_t                bdHandle;

}cy_stc_ble_gapc_peer_bd_addr_info_t;


/* --------------------------Structure corresponding to events-------------------- */

/** Advertisement report parameter */
typedef struct
{
    /** Advertisement event type
        - Connectable undirected advertising = 0x00
        - Connectable directed advertising = 0x01
        - Scannable undirected advertising = 0x02
        - Non connectable undirected advertising = 0x03
        - Scan Response = 0x04
     */
    cy_en_ble_gapc_adv_event_t      eventType;

    /** bd address type of the device advertising.
        - CY_BLE_GAP_ADDR_TYPE_PUBLIC
        - CY_BLE_GAP_ADDR_TYPE_RANDOM
        - CY_BLE_GAP_ADDR_TYPE_PUBLIC_RPA
        - CY_BLE_GAP_ADDR_TYPE_RANDOM_RPA
    */
    uint8_t                       peerAddrType;

    /** Public Device Address or Random Device Address for
       each device that responded to scanning. */
    uint8_t*                      peerBdAddr;

    /** length of the data for each device that responded to scanning */
    uint8_t                       dataLen;

    /** Pointer to advertising or scan response data */
    uint8_t*                      data;

    /** Rssi of the responding device.
                 * Range: -85 <= N <= 0
                 * Units: dBm */
    int8_t                        rssi;

} cy_stc_ble_gapc_adv_report_param_t;

/** Direct Advertising Report received by GAP Central */
typedef struct
{
    /** Buffer containing Random Device Address of Scanner (local device)
     * This is the address to which the directed advertisements are
     * being directed.
     */
    uint8_t                       * localBdAddr;

    /** Buffer containing Device Address of advertiser sending the directed advertisement */
    uint8_t                       * peerBdAddr;

    /** Device Address type of advertiser sending the directed advertisement */
    cy_en_ble_gap_adv_addr_type_t   peerBdAddrType;


    /** Rssi of the responding device.
     * Range: -127 <= N <= +20
     * Units: dBm
     * N = 127 -> RSSI not available
     */
    int8_t                        rssi;

} cy_stc_ble_gapc_direct_adv_report_param_t;
/** @} */



/***************************************
** Exported APIs
***************************************/
/**
 \addtogroup group_ble_common_api_gap_central_functions
 @{
*/

/******************************************************************************
* Function Name: Cy_BLE_GAPC_StartDiscovery
***************************************************************************//**
*
*  This function starts the discovery of devices that are advertising. This is a
*  non-blocking function. As soon as the discovery operation starts, the
*  CY_BLE_EVT_GAPC_SCAN_START_STOP event is generated.
*
*  Every Advertisement / Scan response packet received results in a new event,
*  CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT. If 'scanInfo->scanTo' is a non-zero value,
*  upon commencement of discovery procedure and elapsed time = 'scanInfo->scanTo',
*  CY_BLE_EVT_TIMEOUT event is generated with the event parameter indicating
*  CY_BLE_GAP_SCAN_TO.
*
*  If 'param->scanTo' is equal to zero, the scanning operation is performed
*  until the Cy_BLE_GAPC_StopDiscovery() function is invoked.
*  
*  If 'param->scanFilterPolicy' is set to 0x02 or 0x03, CY_BLE_EVT_GAPC_DIRECT_ADV_REPORT event
*  is received to indicate that directed advertisements have been received where 
*  the advertiser is using a resolvable private address.
*
*  There are three discovery procedures that can be specified as a parameter to
*  this function.
*
*  __Observation procedure__

*   A device performing the observer role receives only advertisement data from
*   devices irrespective of their discoverable mode settings. Advertisement data
*   received is provided by the event,
*
*   CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT
*
*   'param->scanType' should be set as passive scanning (0x00).
*
*  __Limited Discovery procedure__

*   A device performing the limited discovery procedure receives advertisement
*   data and scan response data from devices in the limited discoverable mode only.
*   Received data is provided by the event,
*
*   CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT
*
*   'param->scanType' should be set as active scanning (0x01).
*
*  __General Discovery procedure__

*   A device performing the general discovery procedure receives the advertisement
*   data and scan response data from devices in both limited discoverable mode and
*   general discoverable mode. Received data is provided by the event,
*
*   CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT
*
*   'param->scanType' should be set as active scanning (0x01).
*
*   \param param:  Pointer to a variable of type cy_stc_ble_gapc_disc_info_t
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter for 'param' or if any element within 'param' has an invalid value.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPC_StartDiscovery
(
    cy_stc_ble_gapc_disc_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAPC_StopDiscovery
***************************************************************************//**
*
*  This function stops the discovery of devices. This is a non-blocking function.
*  On stopping discovery operation, the CY_BLE_EVT_GAPC_SCAN_START_STOP event is
*  generated. Application layer must keep track of the function call made
*  before receiving this event to associate this event with either the start or
*  stop discovery function.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPC_StopDiscovery
(
    void
);


/******************************************************************************
* Function Name: Cy_BLE_GAPC_InitConnection
***************************************************************************//**
*  This function instructs the BLE Stack to initiate a connection request to the
*  remote device with required connection parameters. The connection request from
*  the application is acknowledged by the BLE Controller as
*  'CY_BLE_EVT_GAP_ENHANCE_CONN_COMPLETE' or 'CY_BLE_EVT_GAP_DEVICE_CONNECTED'
*  depending on whether Link Layer Privacy is enabled or not in the component customizer. That
*  means, a request is correct, permitted, and  all parameters that are part of the
*  request are correct. If the parameter validation or request is not permitted,
*  then the BLE controller throws the 'CY_BLE_EVT_GAP_DEVICE_CONNECTED' event with error code in the status,
*  instead of CY_BLE_EVT_GAP_DEVICE_CONNECTED/CY_BLE_EVT_GAP_ENHANCE_CONN_COMPLETE.
*  For a positive condition, the controller can issue a connect request to a peer. If it fails to establish
*  connection, 'CY_BLE_EVT_GAP_DEVICE_DISCONNECTED' is passed to application.
*  Once connection is established, CY_BLE_EVT_GAP_CONN_ESTB will be sent to the application
*  if the application has set the CY_BLE_CONN_ESTB_EVENT_MASK event mask using Cy_BLE_SetCustomEventMask().
*  The stack supports creating only one master connection at a time. 
*
*  This API should not be called until the previous call to the API has resulted in a connection
*  completion as indicated by the event CY_BLE_EVT_GAP_DEVICE_CONNECTED/
*  CY_BLE_EVT_GAP_DEVICE_CONNECTEDCY_BLE_EVT_GAP_ENHANCE_CONN_COMPLETE  Or
*  Cy_BLE_GAPC_CancelConnection() is not yet completed, which is informed through 
*  event 'CY_BLE_EVT_GAP_CREATE_CONN_CANCEL_COMPLETE'
*
*  The CY_BLE_EVT_GATT_CONNECT_IND event is generated at the GAP Peripheral end after connection is 
*  completed with a peer Central device. For a GAP Central device, this event is generated as an 
*  acknowledgment of receiving this event successfully by the BLE Controller. 
*
*  This is a non-blocking function. This function must be called after
*  successfully stopping scanning. Scanning is successfully stopped on invoking
*  the Cy_BLE_GAPC_StopDiscovery() function and receiving the event
*  CY_BLE_EVT_GAPC_SCAN_START_STOP with the event data of '0x01',
*  indicating success.
*
*  For details related to connection modes and procedures, refer to Bluetooth
*  4.1 Core Specification, Volume 3, Part C, Section 9.3.
*
*  \param param: Structure of type 'cy_stc_ble_gapc_conn_info_t' that contains the
*              connection parameters.\n
*             <b>Note</b> Any parameter of structure type cy_stc_ble_gapc_conn_info_t,
*             if not required by a specific Bluetooth Low Energy profile, may
*             be ignored.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter for 'param' or if any element within 'param' has an invalid value.
*   CY_BLE_ERROR_INVALID_OPERATION        | Operation is not permitted.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | Stack resources are unavailable.
*
*  Note: In multi connection use case, the recommended minimum connection interval per connection should be
*        greater than N * Max Time taken by individual connections to complete a BLE Connection Event (CE).
*        Min_CI = N * Average Time Per CE.
*        Average time for each CE is the amount of time taken to complete one BLE Tx and Rx transaction. This
*        time varies depending on Link Layer Data Length Extension (DLE) and BLE data rate (1 Mbps or 2 Mbps) 
*        configuration. The application can use the below timing lookup table for CE value:
*            1. If DLE is enabled and data rate is 1 Mbps, Average time = 6 ms
*            2. If DLE is enabled and data rate is 2 Mbps, Average time = 3.5 ms
*            3. If DLE is disabled and data rate is 1 Mbps, Average time = 2 ms
*            4. If DLE is disabled and data rate is 2 Mbps, Average time = 1.6 ms
*        For example, if application supports 4 BLE connections with DLE and 1 Mbps data rate, then the recommended
*        minimum connection interval for each of the connections is 4 * 6 = 24 ms.
*        Note that connection intervals less than this value will still work, but under certain circumstances, the
*        real time control procedures (connection update, channel map update, etc.) with shorter update instances might
*        result in link disconnection.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPC_InitConnection
(
    cy_stc_ble_gapc_conn_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAPC_CancelConnection
***************************************************************************//**
*
* Description:
*  This function cancels a previously initiated connection with the peer device.
*  Operation complete of this API is informed through event 'CY_BLE_EVT_GAP_CREATE_CONN_CANCEL_COMPLETE'
*
*  If the devices are already connected, then this function should not be used.
*  To disconnect from an existing connection, use the function
*  Cy_BLE_GAP_Disconnect().
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                             | Description
*   ------------                             | -----------
*   CY_BLE_SUCCESS                           | On successful operation.
*   CY_BLE_ERROR_INVALID_OPERATION           | Operation is not permitted.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED    | Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES      | Stack resources are unavailable.
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPC_CancelConnection
(
    void
);


/******************************************************************************
* Function Name: Cy_BLE_GAPC_ResolveDevice
***************************************************************************//**
*
*  This function enables the application to start a resolution procedure for a
*  device that is connected using a resolvable private address. This is a non-blocking function.
*  The application should use this function when in GAP Central mode.
*
*  Refer to Bluetooth 4.1 Core specification, Volume 3, Part C, section 10.8.2.3
*  Resolvable Private Address Resolution Procedure to understand the usage of
*  Private addresses.
* Request complete is informed through 'CY_BLE_EVT_GAP_RESOLVE_DEVICE_COMPLETE'
*
*  \param param: Parameter is of type cy_stc_ble_gapc_resolve_peer_info_t
*   param->bdAddr: Peer Bluetooth device address of length 6 bytes, not
*           NULL terminated.
*   param->peerIrk: 128-bit IRK to be used for resolving the peer's private
*        resolvable address.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter for 'param' or 'bdAddr'.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | Stack resources are unavailable.
*
* Note1: param should point to valid memory location until completion of API
*        is indicated via CY_BLE_EVT_GAP_RESOLVE_DEVICE_COMPLETE event.
* Note2: If using whitelist filtering, update the whitelist
*        with the new address using Cy_BLE_AddDeviceToWhiteList API.
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPC_ResolveDevice
(
    cy_stc_ble_gapc_resolve_peer_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAPC_SetRemoteAddr
***************************************************************************//**
*
*  This function allows the application to set the new address of remote device identified by bdHandle.
*  This API should be used when:
*  1. A peer device is previously bonded with a public address and changes its bd address to a resolvable private address.
*      The application should resolve the device by calling the 'Cy_BLE_GAPC_ResolveDevice()' API and setting the
*      new address if successfully resolved.
*  2. A device is previously bonded with a random, the application should call this API to set the new address(public/random).
*
*  \param param: Parameter of type 'cy_stc_ble_gapc_peer_bd_addr_info_t'
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                         | Description
*   ------------                         | -----------
*   CY_BLE_SUCCESS                       | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER       | On invalid bdHandle.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPC_SetRemoteAddr
(
    cy_stc_ble_gapc_peer_bd_addr_info_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GAPC_ConnectionParamUpdateRequest
***************************************************************************//**
*
*  This function sends the connection parameter update command to a local controller.
*  This function can be used only from a device connected in the GAP Central role.
*  \note: The connection parameter update procedure, defined as part of Bluetooth spec 4.1, is not supported.
*          This function will allow the GAP Central application to update the connection parameter for a local controller
*          and the local controller will follow the procedure as defined in Bluetooth Core specification 4.0.
* Request complete is informed through 'CY_BLE_EVT_GAP_CONNECTION_UPDATE_COMPLETE'
*
*  \param param: Parameter is of type 'cy_stc_ble_gap_conn_update_param_info_t'
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                          | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL for input parameter.
*   CY_BLE_ERROR_NO_DEVICE_ENTITY         | If connection does not exist for corresponding 'bdHandle'.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES   | Stack resources are unavailable.
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPC_ConnectionParamUpdateRequest
(
    cy_stc_ble_gap_conn_update_param_info_t  * param
);

/** @} */

#endif /* CY_BLE_STACK_GAP_CENTRAL_H_ */

/*EOF*/
