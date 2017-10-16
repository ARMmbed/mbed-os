/*******************************************************************************
* \file cy_ble_stack_gap_peripheral.h
* \version 2.0
*
* \brief
*  This file contains declarations of public BLE APIs of Generic Access Profile.
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
#ifndef CY_BLE_STACK_GAP_PERIPHERAL_H_
#define CY_BLE_STACK_GAP_PERIPHERAL_H_


/***************************************
* Common stack includes
***************************************/
#include "cy_ble_stack_gap.h"

/***************************************
** GAP Constants
***************************************/
/** Broadcaster and discoverable mode  */
#define CY_BLE_GAPP_NONE_DISC_BROADCAST_MODE     0x00u /**< Non discoverable, broadcast mode */
#define CY_BLE_GAPP_LTD_DISC_MODE                0x01u /**< Limited discoverable mode */
#define CY_BLE_GAPP_GEN_DISC_MODE                0x02u /**< General discoverable mode */

/** Limited Discoverable mode timeout max. */
#define CY_BLE_GAPP_LTD_DISC_MODE_TIME_MAX       180u

/** Advertising filter policy*/
#define CY_BLE_GAPP_SCAN_ANY_CONN_ANY            0x00u /**< scan and connect any device */
#define CY_BLE_GAPP_SCAN_WHITELIST_CONN_ANY      0x01u /**< scan only whitelist and connect any device */
#define CY_BLE_GAPP_SCAN_ANY_CONN_WHITELIST      0x02u /**< scan any, connect only from whitelist */
#define CY_BLE_GAPP_SCAN_CONN_WHITELIST_ONLY     0x03u /**< scan and connect only from whitelist */

/**
 \addtogroup group_ble_common_api_gap_definitions
 @{
*/

/***************************************
** Enumerated Types
***************************************/

/** Advertisement type  */
typedef enum
{
    /** Connectable undirected advertising */
    CY_BLE_GAPP_CONNECTABLE_UNDIRECTED_ADV = 0x00u,

    /** Connectable high duty cycle directed advertising */
    CY_BLE_GAPP_CONNECTABLE_HIGH_DC_DIRECTED_ADV,

    /** Scannable undirected advertising */
    CY_BLE_GAPP_SCANNABLE_UNDIRECTED_ADV,

    /** Non connectable undirected advertising */
    CY_BLE_GAPP_NON_CONNECTABLE_UNDIRECTED_ADV,

    /** Connectable low duty cycle directed advertising */
    CY_BLE_GAPP_CONNECTABLE_LOW_DC_DIRECTED_ADV

}cy_en_ble_gapp_adv_t;


/***************************************
** Exported structures and unions
***************************************/

/** Advertising parameters   */
typedef struct
{
    /** Minimum advertising interval for undirected
       and low duty cycle directed advertising
         - Time Range: 20 ms to 10.24 sec
     */
    uint16_t                  advIntvMin;

    /** Maximum advertising interval for undirected
       and low duty cycle directed advertising.
        - Time Range: 20 ms to 10.24 sec
     */
    uint16_t                  advIntvMax;

    /** Type of advertisement
        - Connectable undirected advertising (0x00)
        - Connectable high duty cycle directed advertising (0x01)
        - Scannable undirected advertising (0x02)
        - Non connectable undirected advertising (0x03)
        - Connectable low duty cycle directed advertising (0x04)
     */
    cy_en_ble_gapp_adv_t        advType;

    /** Own BD Address Type
        - CY_BLE_GAP_ADDR_TYPE_PUBLIC
        - CY_BLE_GAP_ADDR_TYPE_RANDOM
        - CY_BLE_GAP_ADDR_TYPE_PUBLIC_RPA
        - CY_BLE_GAP_ADDR_TYPE_RANDOM_RPA
     */
    uint8_t                   ownAddrType;

    /** Address type of the Bluetooth device address being used for
       directed advertising, not applicable otherwise
        - CY_BLE_GAP_ADDR_TYPE_PUBLIC (Public device address)
        - CY_BLE_GAP_ADDR_TYPE_RANDOM (Random device address)
     */
    uint8_t                   directAddrType;

    /** This parameter specifies Bluetooth device address of the
        device to be connected while using directed advertising.
        In case of none direct advertising, parameter will be 0
     */
    uint8_t                   directAddr[CY_BLE_GAP_BD_ADDR_SIZE];

    /** Advertising channels that shall be used when transmitting advertising packets.
       Channel map selection:
        - Enable channel 37 = bitmask. xxxxxxx1b
        - Enable channel 38 = bitmask. xxxxxx1xb
        - Enable channel 39 = bitmask. xxxxx1xxb
     */
    uint8_t                   advChannelMap;

    /** Advertising Filter Policy
        - CY_BLE_GAPP_SCAN_ANY_CONN_ANY (Allow Scan Request from Any, Allow Connect Request
           from Any (Default))
        - CY_BLE_GAPP_SCAN_WHITELIST_CONN_ANY (Allow Scan Request from White List Only,
           Allow Connect Request)
        - CY_BLE_GAPP_SCAN_ANY_CONN_WHITELIST (Allow Scan Request from Any, Allow Connect
           Request from White List Only)
        - CY_BLE_GAPP_SCAN_CONN_WHITELIST_ONLY (Allow Scan Request from White List Only,
           Allow Connect Request from White List Only)
     */
    uint8_t                   advFilterPolicy;

}cy_stc_ble_gapp_disc_param_t;

/** Advertising data   */
typedef struct
{
    /**GAP Advertisement Parameters which includes Flags, Service UUIDs and short name*/
    uint8_t      advData[CY_BLE_GAP_MAX_ADV_DATA_LEN];

    /**length of the advertising data. This should be made zero if there is no data */
    uint8_t      advDataLen;

} cy_stc_ble_gapp_disc_data_t;

/** Scan response data   */
typedef struct
{
    /** Static user data transmitted in scan response. This should be made NULL if
    there is no data. Maximum length of the data is equal to 31 bytes */
    uint8_t     scanRspData[CY_BLE_GAP_MAX_SCAN_RSP_DATA_LEN];

    /** Length of the scan response data. This should be made zero if there is no data */
    uint8_t     scanRspDataLen;

} cy_stc_ble_gapp_scan_rsp_data_t;

/** Advertising information   */
typedef struct
{
    /** Broadcaster and discoverable mode
        - CY_BLE_GAPP_NONE_DISC_BROADCAST_MODE
           (Applicable for Broadcaster or non-discoverable mode)
        - CY_BLE_GAPP_LTD_DISC_MODE (Limited discovery mode)
        - CY_BLE_GAPP_GEN_DISC_MODE (General discovery mode)
     */
    uint8_t                           discMode;

    /** Advertisement parameters are not allowed to be modified if ADV is already active
        This needs to be NULL, if ADV is active.
    */
    cy_stc_ble_gapp_disc_param_t         * advParam;
    /** Advertisement data */
    cy_stc_ble_gapp_disc_data_t          * advData;
    /** Scan Response data */
    cy_stc_ble_gapp_scan_rsp_data_t      * scanRspData;

    /** Advertisement timeout is in seconds. If timeout is set to 0,
        then there will not be any timeout. The parameter 'advTo' can
        be used for all GAP timeouts related to peripheral operation.
        For General discoverable mode, this timer will be ignored.
        The application is expected to exit from discoverable mode
        explicitly by calling the Cy_BLE_GAPP_ExitDiscoveryMode() function.
        For Limited discoverable mode, 'advTo' should not exceed 180 sec.
     */
    uint16_t                          advTo;

} cy_stc_ble_gapp_disc_mode_info_t;
/** @} */

/***************************************
** Exported APIs
***************************************/
/**
 \addtogroup group_ble_common_api_gap_peripheral_functions
 @{
*/

/******************************************************************************
* Function Name: Cy_BLE_GAPP_EnterDiscoveryMode
***************************************************************************//**
*
*  This function sets the device into discoverable mode. In the discoverable
*  mode, based on the parameters passed to this function, the BLE Device starts
*  advertisements and can respond to scan requests. This is a non-blocking
*  function. It is to be used by the device in the GAP Peripheral mode of operation
*  to set parameters essential for starting advertisement procedure.
*
*  Note that this API is not allowed if advertisement is already active.
*
*  On start of advertisement, the GAP Peripheral receives
*  CY_BLE_EVT_GAPP_ADVERTISEMENT_START_STOP event. The following events can
*  occur on invoking this function.
*
*  * CY_BLE_EVT_GAP_DEVICE_CONNECTED - If the device connects to a GAP Central and
*    Link Layer Privacy is disabled in component customizer.
*  * CY_BLE_EVT_GAP_ENHANCE_CONN_COMPLETE - If the device connects to a GAP Central and
*    Link Layer Privacy is enabled in component customizer.
*
*  * CY_BLE_EVT_TIMEOUT - If no device in the GAP Central mode connects to this
*     device within the specified timeout limit. This event can occur if
*     'param ->discMode' is equal to CY_BLE_GAPP_LTD_DISC_MODE or
*     CY_BLE_GAPP_GEN_DISC_MODE. 'param-> advTo' specifies the timeout duration.
*     Set the 'param-> advTo' to 0 when 'param -> discMode' is set to
*     CY_BLE_GAPP_GEN_DISC_MODE so that the timeout event does not occur and the
*     advertisement continues until the Cy_BLE_GAPP_ExitDiscoveryMode() function
*     is invoked. If 'param -> discMode' is set to CY_BLE_GAPP_GEN_DISC_MODE, then
*     'param-> advTo' is ignored.
*
*  \param param: Structure of type cy_stc_ble_gapp_disc_mode_info_t, which contains the
*            advertisement parameters
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                             | Description
*   ------------                             | -----------
*   CY_BLE_SUCCESS                           | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER           | On specifying NULL for 'param' or if any of the elements of this structure are invalid.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED    | Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES      | Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPP_EnterDiscoveryMode
(
    cy_stc_ble_gapp_disc_mode_info_t  *  param
);

/******************************************************************************
* Function Name: Cy_BLE_GAPP_UpdateAdvScanData
***************************************************************************//**
*
*  This function sets Advertisement or/and Scan Response data.
*  This API can be used during active ADV operation as well. It is expected that the
*  application layer ensures the validity of adv data content as per the
*  adv parameters being used to while starting the Advertisement.
*
*  On completion of this operation, GAP Peripheral receives
*  CY_BLE_EVT_GAPP_UPDATE_ADV_SCAN_DATA_COMPLETE event.
*
*  \param param: Structure of type cy_stc_ble_gapp_disc_mode_info_t,
        Only cy_stc_ble_gapp_disc_mode_info_t.advData and 
        cy_stc_ble_gapp_disc_mode_info_t.scanRspData fields will be used.
        Other fields in the structure will be ignored and will not be used.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                             | Description
*   ------------                             | -----------
*   CY_BLE_SUCCESS                           | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER           | On specifying NULL for 'param' or if any of the elements of this structure are invalid.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED    | Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES      | Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPP_UpdateAdvScanData
(
    cy_stc_ble_gapp_disc_mode_info_t  *  param
);


/******************************************************************************
* Function Name: Cy_BLE_GAPP_ExitDiscoveryMode
***************************************************************************//**
*
*  This function is used to exit from discoverable mode. This is a non-blocking
*  function. After the execution of this function, the device stops advertising.
*
*  On stopping advertising, GAP Peripheral receives
*  CY_BLE_EVT_GAPP_ADVERTISEMENT_START_STOP event. It is expected that the
*  application layer keeps track of the function call performed before occurrence
*  of this event, as this event can occur on making a call to the
*  Cy_BLE_GAPP_EnterDiscoveryMode () function as well.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                             | Description
*   ------------                             | -----------
*   CY_BLE_SUCCESS                           | On successful operation.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED    | Memory allocation failed.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES      | Stack resources are unavailable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPP_ExitDiscoveryMode
(
    void
);


/******************************************************************************
* Function Name: Cy_BLE_GAPP_AuthReqReply
***************************************************************************//**
*
*  This function is used to pass security information for authentication in reply
*  to an authentication request from the master device. It should be invoked on
*  receiving the CY_BLE_EVT_GAP_AUTH_REQ event. Events shown in the following table
*  may be received by the application based on the authentication result.
*
*   Event Name                   | Description
*   ------------                 | -----------
*   CY_BLE_EVT_GAP_AUTH_COMPLETE | Pointer to structure of type 'cy_stc_ble_gap_auth_info_t' is returned as parameter to both the peer devices on successful authentication.
*   CY_BLE_EVT_GAP_AUTH_FAILED   | Received by both GAP Central and Peripheral devices (peers) on authentication failure. Data is of type 'cy_en_ble_gap_auth_failed_reason_t'.
*
*
*  Following events may occur if API result is CY_BLE_SUCCESS:
*
*   Event Name                             | Description
*   ------------                           | -----------
*   CY_BLE_EVT_GAP_SMP_NEGOTIATED_AUTH_INFO| SMP has completed pairing properties (feature exchange) negotiation.
*   CY_BLE_EVT_GAP_KEYINFO_EXCHNGE_CMPLT   | SMP keys exchange with peer device is completed.
*   CY_BLE_EVT_GAP_ENCRYPT_CHANGE          | When there is a change in encryption after pairing procedure.
*
*  Based on IO capabilities, security modes following events may occur
*  during pairing procedure.
*  Please refer Core spec 4.2 volume 3 part - H section 2.3.5.1
*  to know more about Selecting Key Generation Method.
*
*   | Event Name                                |
*   | ------------------------                  |
*   | CY_BLE_EVT_GAP_PASSKEY_DISPLAY_REQUEST    |
*   | CY_BLE_EVT_GAP_KEYPRESS_NOTIFICATION      |
*   | CY_BLE_EVT_GAP_NUMERIC_COMPARISON_REQUEST |
*
*  \param param: Pointer to a variable containing security information of the
*                device of type cy_stc_ble_gap_auth_info_t. Param->security can take 
*                the value from enum cy_en_ble_gap_sec_level_t.
*
*  NOTE: If the bonding flag in param is set to CY_BLE_GAP_BONDING_NONE then, 
*        SMP keys will not be distributed even if application has generated 
*        and set the keys explicitly.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                        | Description
*   ------------                        | -----------
*   CY_BLE_SUCCESS                      | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER      | On specifying NULL for 'param' or if any of the elements of this structure are invalid.
*   CY_BLE_ERROR_NO_DEVICE_ENTITY       | Device identified using 'bdHandle' does not exist.
*   CY_BLE_ERROR_INVALID_OPERATION      | Operation is not permitted.
*   CY_BLE_ERROR_INSUFFICIENT_RESOURCES | On bonded device is full and application tries to initiate pairing with bonding enable.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPP_AuthReqReply
(
    cy_stc_ble_gap_auth_info_t  * param
);

/** @} */

#endif /* CY_BLE_STACK_GAP_PERIPHERAL_H_ */

/*EOF*/
