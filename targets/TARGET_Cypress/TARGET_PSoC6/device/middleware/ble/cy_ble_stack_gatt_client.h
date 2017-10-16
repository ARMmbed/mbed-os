/*******************************************************************************
* \file cy_ble_stack_gatt_client.h
* \version 2.0
*
* \brief
*  This file contains declarations of public BLE APIs of Generic Attribute Profile - Client Role.
*  Also specifies the defines, constants, and data structures required for the APIs.
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

#ifndef CY_BLE_STACK_GATT_CLIENT_H_
#define CY_BLE_STACK_GATT_CLIENT_H_


/***************************************
* Common stack includes
***************************************/

#include "cy_ble_stack_gatt.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
** Exported data structures
***************************************/
/**
 \addtogroup group_ble_common_api_gatt_definitions
 @{
*/

/** Stop command parameter */
typedef struct
{
    /**Connection handle*/
    cy_stc_ble_conn_handle_t     connHandle; 
    
}cy_stc_ble_gattc_stop_cmd_param_t;

/** GATT Discover primary service request parameter */
typedef struct
{
    /** Handle Range */
    cy_stc_ble_gatt_attr_handle_range_t      range;     

    /**Connection handle*/
    cy_stc_ble_conn_handle_t                 connHandle; 
    
}cy_stc_ble_gattc_read_by_group_req_t;

/** GATT Discover primary service by UUID request parameter */
typedef struct
{
    /** Attribute Value to Find */
    cy_stc_ble_gatt_value_t                   value; 

    /** Handle Range - Start and End Handle */
    cy_stc_ble_gatt_attr_handle_range_t         range;  

    /** 16-bit UUID to Find */
    cy_ble_uuid16_t                           uuid; 
    
    /**Connection handle*/
    cy_stc_ble_conn_handle_t                 connHandle; 

}cy_stc_ble_gattc_find_by_type_value_req_t;

/** GATT read by type request parameter */
typedef struct
{
    /** Handle Range */
    cy_stc_ble_gatt_attr_handle_range_t      range;     

    /** GATT UUID type */
    cy_ble_uuid_t                            uuid;   

   /** Format indicating, 16-bit or 128-bit UUIDs
       * For 16-bit UUID format - CY_BLE_GATT_16_BIT_UUID_FORMAT (0x01)
       * For 128-bit UUID format - CY_BLE_GATT_128_BIT_UUID_FORMAT (0x02)
    */
    uint8_t                                 uuidFormat;   
    
    /**Connection handle*/
    cy_stc_ble_conn_handle_t                 connHandle; 
    
}cy_stc_ble_gattc_read_by_type_req_t;

/** GATT find info request parameter */
typedef cy_stc_ble_gattc_read_by_group_req_t cy_stc_ble_gattc_find_info_req_t;

/** Read request parameter */
typedef struct
{
    /** Handle on which Read Blob is requested */
    cy_ble_gatt_db_attr_handle_t        attrHandle;    

    /**Connection handle*/
    cy_stc_ble_conn_handle_t             connHandle; 
    
}cy_stc_ble_gattc_read_req_t;

/** Handle-offset pair parameter */
typedef struct
{
    /** Handle on which Read Blob is requested */
    cy_ble_gatt_db_attr_handle_t        attrHandle;    

    /** Value Offset from which the Read is Requested */
    uint16_t                             offset;    
    
}cy_stc_ble_gattc_handle_offset_pair_t;

/** Read blob request parameter */
typedef struct
{
    /** Handle-offset pair parameter */
    cy_stc_ble_gattc_handle_offset_pair_t    handleOffset;    
    
    /**Connection handle*/
    cy_stc_ble_conn_handle_t                 connHandle; 
    
}cy_stc_ble_gattc_read_blob_req_t;

/** GATT handle list type */
typedef struct
{
    /** Handle list where the UUID with value Indicated is found */
    uint16_t    * handleList;

    /** Number of Handles in the list */
    uint16_t    listCount;

    /** Actual Number of Handles Packed. This is a output parameter */
    uint16_t    actualCount;

}cy_stc_ble_gattc_handle_list_t;

/** Read multiple request parameter */
typedef struct
{
    /** GATT handle list type */
    cy_stc_ble_gattc_handle_list_t  handleListType;
    
    /**Connection handle*/
    cy_stc_ble_conn_handle_t        connHandle; 

}cy_stc_ble_gattc_read_mult_req_t;

/** Write request parameter */
typedef cy_stc_ble_gatt_write_param_t  cy_stc_ble_gattc_write_req_t;

/** Prepare Write request parameter */
typedef cy_stc_ble_gatt_prep_write_param_t  cy_stc_ble_gattc_prep_write_req_t;

/** Prepare Write request parameter for reliable write request */
typedef struct
{
    /** handle value offset pair */
    cy_stc_ble_gatt_handle_value_offset_param_t  * handleValOffsetPair; 

    /** Connection handle */
    cy_stc_ble_conn_handle_t                     connHandle;  
    
    /** Number of requests. That is, the count of array of structures 
             of type 'cy_stc_ble_gatt_handle_value_offset_param_t'. Each array element 
             represents a value and the attribute to which the value has 
             to be written */
    uint8_t                                   numOfRequests;
    
}cy_stc_ble_gattc_reliable_write_req_t;

/** Execute Write request parameter */
typedef struct
{
    /** Connection handle */
    cy_stc_ble_conn_handle_t     connHandle;             

    /** Indicates whether Queued Write is to be executed (0x01) or canceled (0x00) */
    uint8_t                   flag;
    
}cy_stc_ble_gattc_exec_write_req_t;

/** Write command request to be sent to Server */
typedef cy_stc_ble_gattc_write_req_t  cy_stc_ble_gattc_write_cmd_req_t;

/** Signed Write command request to be sent to Server */
typedef cy_stc_ble_gattc_write_req_t  cy_stc_ble_gattc_signed_write_cmd_req_t;

/** Signed Write command request to be sent to Server */
typedef cy_stc_ble_gattc_stop_cmd_param_t  cy_stc_ble_gattc_confirmation_req_t;

/* --------------------------Structure corresponding to events-------------------- */

/** Read response parameter type received from server*/
typedef struct
{
    /** Attribute Value  */
    cy_stc_ble_gatt_value_t      value;     

    /** Connection handle */
    cy_stc_ble_conn_handle_t     connHandle; 
    
}cy_stc_ble_gattc_read_rsp_param_t;

/** Handle value notification data received from server */
typedef cy_stc_ble_gattc_write_req_t  cy_stc_ble_gattc_handle_value_ntf_param_t;

/** GATT handle value indication parameter received from server type */
typedef cy_stc_ble_gattc_write_req_t  cy_stc_ble_gattc_handle_value_ind_param_t;

/** Data Element for Group Response */
typedef struct
{
    /** attribute handle value pair */
    uint8_t       * attrValue;

    /** Length of each Attribute Data Element including the Handle Range */
    uint16_t      length;

    /** Total Length of Attribute Data */
    uint16_t      attrLen;
}cy_stc_ble_gattc_grp_attr_data_list_t;

/** Read By Group Response received from Server*/
typedef struct
{
    /** Group attribute data list */
    cy_stc_ble_gattc_grp_attr_data_list_t   attrData;

    /** Connection handle */
    cy_stc_ble_conn_handle_t                connHandle; 
    
}cy_stc_ble_gattc_read_by_grp_rsp_param_t;

/** GATT read by type response received from server*/
typedef cy_stc_ble_gattc_read_by_grp_rsp_param_t cy_stc_ble_gattc_read_by_type_rsp_param_t;

/** GATT find by type value response received from server*/
typedef struct
{
    /** Handle Range list */
    cy_stc_ble_gatt_attr_handle_range_t        * range; 

     /** Size of list */
    uint8_t                                  count;  

    /** Connection handle */
    cy_stc_ble_conn_handle_t                 connHandle; 
     
}cy_stc_ble_gattc_find_by_type_rsp_param_t;

/** GATT list of Handle UUID pair parameter type */
typedef struct
{
    /** Handle - UUID Pair list
       This is a packed byte stream, hence it needs to be unpacked and decoded. */
    uint8_t      *list;     

    /** Number of elements in the list in bytes */
    uint16_t  byteCount;
    
}cy_stc_ble_gattc_handle_uuid_list_param_t;

/** GATT find info response received from Server*/
typedef struct
{    
    /** Handle Value list */    
    cy_stc_ble_gattc_handle_uuid_list_param_t       handleValueList;  

    /** Connection handle */
    cy_stc_ble_conn_handle_t                     connHandle;

    /** Format indicating, 16-bit (0x01) or 128-bit (0x02) UUIDs */    
    uint8_t                                   uuidFormat;         
    
}cy_stc_ble_gattc_find_info_rsp_param_t;

/** Execute Write result */
typedef struct
{
    /**Connection handle*/
    cy_stc_ble_conn_handle_t             connHandle; 
    
    /** Result of the execute write request */
    uint8_t                             result;       

}cy_stc_ble_gattc_exec_write_rsp_param_t;

/** Long procedure end indication event parameter */
typedef struct
{
    /**Connection handle*/
    cy_stc_ble_conn_handle_t             connHandle; 
    
    /** Result of the execute write request */
    uint8_t                             opcode;       

}cy_stc_ble_gattc_long_procedure_end_param_t;

/** Execute Write response parameter */
typedef cy_stc_ble_gattc_exec_write_req_t  cy_stc_ble_gattc_exec_write_rsp_t;
/** @} */
/***************************************
** Exported APIs
***************************************/
/**
 \addtogroup group_ble_common_api_gatt_client_functions
 @{
*/

/******************************************************************************
* Function Name: Cy_BLE_GATTC_StopCmd
***************************************************************************//**
* 
*  This function is used by the GATT Client to stop any of the following ongoing 
*  GATT procedures:
*   1.  Cy_BLE_GATTC_DiscoverPrimaryServices()
*   2.  Cy_BLE_GATTC_DiscoverPrimaryServiceByUuid()
*   3.  Cy_BLE_GATTC_FindIncludedServices()
*   4.  Cy_BLE_GATTC_DiscoverCharacteristics()
*   5.  Cy_BLE_GATTC_DiscoverCharacteristicByUuid()
*   6.  Cy_BLE_GATTC_DiscoverCharacteristicDescriptors()
*   7.  Cy_BLE_GATTC_ReadLongCharacteristicValues()
*   8.  Cy_BLE_GATTC_WriteLongCharacteristicValues()
*   9.  Cy_BLE_GATTC_ReliableWrites()
*   10. Cy_BLE_GATTC_ReadLongCharacteristicDescriptors()
*   11. Cy_BLE_GATTC_WriteLongCharacteristicDescriptors()
*   12. Cy_BLE_GATTC_ReadByTypeReq()
*   .
*  If none of the above procedures is ongoing, then this command will be ignored.
*  This function has no effect on ATT procedures other than those listed above.
*  
*  If the user intends to start a new GATT procedure including those listed above
*  and there is an ongoing GATT procedure (any one from the above list), the user
*  needs to call this function to stop the ongoing GATT procedure and then invoke
*  the desired GATT procedure.
*  This is not a blocking function. Successful operation is informed through event 
*  'CY_BLE_EVT_GATTC_STOP_CMD_COMPLETE' if procedure is not already ended.
* 
*  \param param: parameter is of type cy_stc_ble_gattc_stop_cmd_param_t.
*   param->connHandle: Connection handle to identify the peer GATT entity of type
*              cy_stc_ble_conn_handle_t.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                           | Description
*  ------------                          | -----------
*  CY_BLE_SUCCESS                        | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER        | If 'param' is NULL or 'connHandle' is invalid.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_StopCmd
(
    cy_stc_ble_gattc_stop_cmd_param_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_ExchangeMtuReq
***************************************************************************//**
* 
*  This function is used by the GATT Client to send Maximum Transmitted Unit
*  (GATT MTU) supported by the GATT Client. This is a non-blocking function.
* 
*  Default GATT MTU size as per Bluetooth 4.1 core specification is 23 bytes. If
*  the GATT Client supports a size greater than the default, it has to invoke
*  this function with the desired GATT MTU size. This function should be
*  initiated only once during a connection.
* 
*  Refer to  Bluetooth 4.1 core specification, Volume 3, Part G, section 4.3.1
*  for more details on GATT MTU exchange operation.
* 
*  This function call results in CY_BLE_EVT_GATTS_XCNHG_MTU_REQ event at the
*  GATT Server's end in response to which the GATT Server is expected to send
*  its GATT MTU size.
* 
*  The CY_BLE_EVT_GATTC_XCHNG_MTU_RSP event is generated at the
*  GATT Client's end on receiving GATT MTU response from the GATT Server.
* 
*  \param param: parameter is of type CY_BLE_GATT_XCHG_MTU_REQ_PARAM_T.
*   param->mtu: Size of GATT MTU. Max GATT MTU supported by BLE stack is 512 Bytes.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or 'connHandle' is invalid or, 'mtu' value is greater than that set on calling Cy_BLE_StackInit().
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_ExchangeMtuReq
(
    cy_stc_ble_gatt_xchg_mtu_param_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_DiscoverPrimaryServices
***************************************************************************//**
* 
* This function is used by the GATT Client to discover the primary services as 
* per the range provided on a GATT Server to which it is connected. 
* This is a non-blocking function.
*
* Internally, this function initiates multiple Read By Group Type Requests to
* the peer device in response to which it receives Read By Group Type Responses.
* Each Read By Group Type Response results in 
* CY_BLE_EVT_GATTC_READ_BY_GROUP_TYPE_RSP event, which is propagated to the 
* application layer for handling.
*
* Primary service discovery is complete when Error Response 
* (CY_BLE_EVT_GATTC_ERROR_RSP) is received and the Error Code is set to Attribute
* Not Found or when the End Group Handle in the Read by Group Type Response is
* 0xFFFF. Completion of this operation is notified to the upper layer(s) using
* CY_BLE_EVT_GATTC_ERROR_RSP with error code updated appropriately or with 
* CY_BLE_EVT_GATTC_LONG_PROCEDURE_END event
*
* It is permitted to end the above stated sequence of operations early if the
* desired primary service is found prior to discovering all the primary services
* on the GATT Server. This can be achieved by calling the Cy_BLE_GATTC_StopCmd() 
* function.
*
* Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.4.1 for
* more details on this sequence of operations.
*
*  \param param: parameter is of type cy_stc_ble_gattc_read_by_group_req_t.
*   param->connHandle: Connection handle to identify the peer GATT entity of type
*              cy_stc_ble_conn_handle_t.
*   param->range: Parameter is of type cy_stc_ble_gatt_attr_handle_range_t where,
*        1. 'range.startHandle' can be set to the start handle of the desired
*            primary service.
*        2. 'range.endHandle' can be set to the end handle of the desired 
*            primary service.
*   \note
*   'startHandle' & 'endHandle' should not be zero and 'startHandle' shall be less than or equal to the 'endHandle'.
*
* \return
* cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
* failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or if any of the input parameters are invalid.
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_DiscoverPrimaryServices
(
    cy_stc_ble_gattc_read_by_group_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_DiscoverPrimaryServiceByUuid
***************************************************************************//**
* 
*  This function is used by the GATT Client to discover a specific primary
*  service on a GATT Server, to which it is connected, when only the Service
*  UUID is known. 
*  This is a non-blocking function.
* 
*  Internally, this function initiates multiple Find By Type Value Requests with
*  the Attribute Type parameter set to the UUID for Primary Service and the
*  Attribute Value set to the 16-bit Bluetooth UUID or 128-bit UUID for the
*  specific primary service. Each Find By Type Value Response received from the
*  peer device is passed to the application as 
*  CY_BLE_EVT_GATTC_FIND_BY_TYPE_VALUE_RSP event.
* 
*  The sequence of operations is complete when the Error Response is received
*  and the Error Code is set to Attribute Not Found or when the End Group 
*  Handle in the Find By Type Value Response is 0xFFFF. Completion of this 
*  function is notified to the upper layer using the CY_BLE_EVT_GATTC_ERROR_RSP event
*  with the error code updated appropriately or with the CY_BLE_EVT_GATTC_LONG_PROCEDURE_END
*  event.
* 
*  It is permitted to end the function early by calling the Cy_BLE_GATTC_StopCmd()
*  function if a desired primary service is found prior to discovery of all the
*  primary services of the specified service UUID supported on the GATT Server.
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.4.2 
*  for more details on this sequence of operations.
*     
*  \param param: parameter is of type cy_stc_ble_gattc_find_by_type_value_req_t.
*   param->connHandle: Connection handle to identify the peer GATT entity of type 
*               cy_stc_ble_conn_handle_t.
*   param->range and param->uuid are ignored in this API.
*   param->value: Parameter is of type cy_stc_ble_gatt_value_t, where,
*          1. 'value.val' should point to uint8_t array containing the UUID to 
*              look for. UUID can be 16 or 128 bit.
*          2. 'value.len' should be set to 2 if the 16-bit UUID is to be found.
*              The length should be set to 16 if 128-bit UUID is to be found.
*          3. 'value.actualLen' is an unused parameter and should be ignored 
*              as it is unused.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or 'connHandle' is invalid.
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_DiscoverPrimaryServiceByUuid
(
    cy_stc_ble_gattc_find_by_type_value_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_DiscoverCharacteristics
***************************************************************************//**
* 
*  This function is used by the GATT Client to find all characteristic
*  declarations within a service definition on a GATT Server connected to it when
*  only the service handle range is known. 
*  This is a non-blocking function.
* 
*  Internally, multiple Read By Type Requests are sent to the GATT Server in
*  response to which Read By Type Responses are received. Each response results
*  in the event CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP, which is passed to the 
*  application layer for handling.
* 
*  The function may end early by calling the Cy_BLE_GATTC_StopCmd() 
*  function if a desired characteristic is found prior to discovering all the
*  characteristics of the specified service supported on the GATT Server.
*  Completion of this function is notified to the upper layer using
*  CY_BLE_EVT_GATTC_ERROR_RSP or with the CY_BLE_EVT_GATTC_LONG_PROCEDURE_END
*  event.
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.6.1 for
*  more details on the sequence of operations.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_read_by_type_req_t,
*                     Where, the following needs to be set:
*                     param->range.startHandle
*                     param->range.endHandle
*                                 param->connHandle  
*                                 Where, the following needs to be ignored:
*                     param->uuidFormat (CY_BLE_GATT_16_BIT_UUID_FORMAT or
*                                                     CY_BLE_GATT_128_BIT_UUID_FORMAT)
*                     param->uuid.uuid16 or readByTypeReqParam->uuid.uuid128 
*                     based on the uuidFormat
*  \note
*   'startHandle' & 'endHandle' should not be zero, and 'startHandle' shall be less than or equal to the 'endHandle'.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or if any of the input parameters are invalid.
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_DiscoverCharacteristics
(
    cy_stc_ble_gattc_read_by_type_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_DiscoverCharacteristicByUuid
***************************************************************************//**
* 
*  This function is used by the GATT Client to discover service characteristics
*  on a GATT Server when only the service handle ranges are known and the
*  characteristic UUID is known. 
*  This is a non-blocking function.
* 
*  Internally, multiple Read By Type Requests are sent to the peer device in
*  response to which Read By Type Responses are received. Each of these responses
*  results in the event CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP, which is passed to the
*  application layer for further processing.
* 
*  The function may end early by calling the Cy_BLE_GATTC_StopCmd() 
*  function if a desired characteristic is found prior to discovering all the
*  characteristics for the specified service supported on the GATT Server. 
*  Completion of this function is notified to the upper layer using 
*  CY_BLE_EVT_GATTC_ERROR_RSP or with CY_BLE_EVT_GATTC_LONG_PROCEDURE_END
*  event.
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.6.2
*  for more details on the sequence of operations.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_read_by_type_req_t,
*                     Where, the following must be set:
*                     param->range.startHandle
*                     param->range.endHandle
*                                 param->connHandle  
*                     param->uuidFormat (CY_BLE_GATT_16_BIT_UUID_FORMAT or
*                                                     CY_BLE_GATT_128_BIT_UUID_FORMAT)
*                     param->uuid.uuid16 or readByTypeReqParam->uuid.uuid128 
*                     based on the uuidFormat
*  \note
*   'startHandle' & 'endHandle' should not be zero, and 'startHandle' shall be less than or equal to the 'endHandle'.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or if any of the input parameters are invalid.
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_DiscoverCharacteristicByUuid
(
    cy_stc_ble_gattc_read_by_type_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_DiscoverCharacteristicDescriptors
***************************************************************************//**
* 
*  This function is used by the GATT Client to find all the characteristic
*  descriptors. This is a non-blocking function.
* 
*  Internally, multiple Find Information Requests are sent to the peer device in
*  response to which Find Information Responses are received by the GATT Client.
*  Each of these responses generate CY_BLE_EVT_GATTC_FIND_INFO_RSP event at the
*  GATT Client end which is propagated to the application layer for further
*  processing.
* 
*  The function may end early by calling the Cy_BLE_GATTC_StopCmd()
*  function if the desired Characteristic Descriptor is found prior to discovering 
*  all the characteristic descriptors of the specified characteristic. Completion
*  of this function is notified to the upper layer using CY_BLE_EVT_GATTC_ERROR_RSP or 
*  the CY_BLE_EVT_GATTC_LONG_PROCEDURE_END event.
*  
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.7.1 for
*  more details on the sequence of operations.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_find_info_req_t.
*                     Where, the following needs to be set:
*                     param->range.startHandle
*                     param->range.endHandle
*                                 param->connHandle  
*  \note
*   'startHandle' & 'endHandle' should not be zero, and 'startHandle' shall be less than or equal to the 'endHandle'.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or if any of the input parameters are invalid.
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_DiscoverCharacteristicDescriptors
(
    cy_stc_ble_gattc_find_info_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_FindIncludedServices
***************************************************************************//**
* 
*  This function is used by the GATT Client to find Included Service
*  declarations within a GATT Service to which it is connected. This is a 
*  non-blocking function.
* 
*  Internally, multiple Read By Type Requests are sent to the peer device in
*  response to which Read By Type Responses are received 
*  (CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP) and passed to the application layer.
* 
*  When Read By Type Response data does not contain the service UUID,
*  indicating the service UUID is a 128-bit UUID, the application layer can
*  choose to get the service UUID by performing the following steps:
*  1. Stop ongoing GATT operation by invoking Cy_BLE_GATTC_StopCmd()
*  2. Send Read Request by invoking the function 
*      Cy_BLE_GATTC_ReadCharacteristicValue() with the read request handle set to
*      the attribute handle of the included service. Handle associated events. 
*  3. Re-initiate Cy_BLE_GATTC_FindIncludedServices function, setting the start
*      handle to the attribute handle that is placed next to the one used in
*      the above step.
* 
*  The function may end early if a desired included service is
*  found prior to discovering all the included services of the specified
*  service supported on the server by calling the Cy_BLE_GATTC_StopCmd() 
*  function. If the Cy_BLE_GATTC_StopCmd() function is not invoked, completion of
*  this function is notified to the upper layer using CY_BLE_EVT_GATTC_ERROR_RSP 
*  or the CY_BLE_EVT_GATTC_LONG_PROCEDURE_END event.
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.5.1 
*  for more details on the sequence of operations.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_read_by_type_req_t,
*                     Where, the following needs to be set:
*                     param->range.startHandle
*                     param->range.endHandle
*                                 param->connHandle  
*                                 Where, the following needs to be ignored:
*                     param->uuidFormat (CY_BLE_GATT_16_BIT_UUID_FORMAT or
*                                                     CY_BLE_GATT_128_BIT_UUID_FORMAT)
*                     param->uuid.uuid16 or readByTypeReqParam->uuid.uuid128 
*                     based on the uuidFormat
*  \note
*   'startHandle' & 'endHandle' should not be zero, and 'startHandle' shall be less than or equal to the 'endHandle'.
*
*  \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or if any of the input parameters are invalid.
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
*     
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_FindIncludedServices
(
    cy_stc_ble_gattc_read_by_type_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_ReadByTypeReq
***************************************************************************//**
* 
*  This function allows the user to send Read by type request to peer server
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.5.1 
*  for more details on the sequence of operations.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_read_by_type_req_t,
*                     Where, the following needs to be set:
*                     param->range.startHandle
*                     param->range.endHandle
*                                 param->connHandle  
*                     param->uuidFormat (CY_BLE_GATT_16_BIT_UUID_FORMAT or
*                                                     CY_BLE_GATT_128_BIT_UUID_FORMAT)
*                     param->uuid.uuid16 or readByTypeReqParam->uuid.uuid128 
*                     based on the uuidFormat
* 
*  \note
*   'startHandle' & 'endHandle' should not be zero, and 'startHandle' shall be less than or equal to the 'endHandle'.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or if any of the input parameters are invalid.
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
*     
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_ReadByTypeReq
(
    cy_stc_ble_gattc_read_by_type_req_t      * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_ReadCharacteristicValue
***************************************************************************//**
* 
*  This function reads a Characteristic Value from a GATT Server when the GATT
*  Client knows the Characteristic Value Handle. This is a non-blocking function.
* 
*  Internally, Read Request is sent to the peer device in response to which Read
*  Response is received. This response results in CY_BLE_EVT_GATTC_READ_RSP event
*  which is propagated to the application for handling the event data. An Error
*  Response (CY_BLE_EVT_GATTC_ERROR_RSP event at the GATT Client's end) is sent
*  by the GATT Server in response to the Read Request on insufficient 
*  authentication or insufficient authorization or insufficient encryption key
*  size is caused by the GATT Client, or if a read operation is not permitted on
*  the Characteristic Value. The Error Code parameter is set as specified in
*  the Attribute Protocol.
*  Execution of this API generates CY_BLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ event 
*  at GATT server's end if the corresponding characteristic's attribute permission is set 
*  to CY_BLE_GATT_DB_ATTR_CHAR_VAL_RD_EVENT.
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.8.1
*  for more details on the sequence of operations.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_read_req_t.
*                     Where, the following needs to be set:
*                     param->attrHandle: Attribute handle to be read
*                                 param->connHandle  
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or 'connHandle' is invalid, or if 'attrHandle' is zero. 
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_ReadCharacteristicValue
(
    cy_stc_ble_gattc_read_req_t  * param
); 


/******************************************************************************
* Function Name: Cy_BLE_GATTC_ReadUsingCharacteristicUuid
***************************************************************************//**
* 
*  This function reads a Characteristic Value from the GATT Server when the GATT
*  Client knows only the characteristic UUID and does not know the handle of the
*  characteristic. This is a non-blocking function.
* 
*  Internally, Read By Type Request is sent to the peer device in response to
*  which Read By Type Response is received by the GATT Client. This results in
*  CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP event, which is propagated to the application
*  layer for further handling.
*  Execution of this API generates the CY_BLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ event 
*  at the GATT server's end if the corresponding characteristic's attribute permission has is to
*  CY_BLE_GATT_DB_ATTR_CHAR_VAL_RD_EVENT.
* 
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.8.2 for
*  more details on the sequence of operations.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_read_by_type_req_t,
*                     Where, the following needs to be set:
*                     param->range.startHandle
*                     param->range.endHandle
*                                 param->connHandle  
*                     param->uuidFormat (CY_BLE_GATT_16_BIT_UUID_FORMAT or
*                                                     CY_BLE_GATT_128_BIT_UUID_FORMAT)
*                     param->uuid.uuid16 or readByTypeReqParam->uuid.uuid128 
*                     based on the uuidFormat
*  \note
*   'startHandle' & 'endHandle' should not be zero, and 'startHandle' shall be less than or equal to the 'endHandle'.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or if any of the input parameters are invalid.
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_ReadUsingCharacteristicUuid
(
    cy_stc_ble_gattc_read_by_type_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_ReadLongCharacteristicValues
***************************************************************************//**
* 
*  This function reads a Characteristic Value from the GATT Server when the GATT
*  Client knows the Characteristic Value Handle and the length of the
*  Characteristic Value is longer than can be sent in a single Read Response
*  Attribute Protocol message. This is a non-blocking function.
* 
*  Internally multiple Read Blob Requests are sent to the peer device in response
*  to which Read Blob Responses are received. For each Read Blob Request, a Read
*  Blob Response event is received (CY_BLE_EVT_GATTC_READ_BLOB_RSP) with a
*  portion of the Characteristic Value contained in the Part Attribute Value
*  parameter. These events are propagated to the application layer
*  for further processing. Each read blob response will return up to (GATT MTU-1) bytes of
*  data. If the size of characteristic value field is an integral multiple of (GATT MTU-1)
*  then the operation terminates with an error response event, where the error code is
*  CY_BLE_GATT_ERR_INVALID_OFFSET. If the size of the characteristic value field is
*  not an integral multiple of (GATT MTU-1), the last read blob response will return
*  data bytes that are less than (GATT MTU-1). The application must monitor these two
*  conditions before proceeding with the initiation of any other GATT operation.
* 
*  An Error Response event (CY_BLE_EVT_GATTC_ERROR_RSP) is sent by the GATT Server
*  in response to the Read Blob Request if insufficient authentication, 
*  insufficient authorization, insufficient encryption key size is used by the
*  client, or if a read operation is not permitted on the Characteristic Value.
*  The Error Code parameter is set as specified in the Attribute Protocol.
* 
*  If the Characteristic Value is not longer than (GATT MTU - 1), an Error Response
*  with the Error Code set to Attribute Not Long is received by the GATT Client
*  on the first Read Blob Request.
*
*  Execution of this API generates the CY_BLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ event 
*  at the GATT server's end if the corresponding characteristic's attribute permission is set to
*  CY_BLE_GATT_DB_ATTR_CHAR_VAL_RD_EVENT.
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.8.3
*  for more details on the sequence of operations.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_read_blob_req_t.
*                     Where, the following needs to be set:
*                     param->attrHandle
*                     param->offset
*                                 param->connHandle  
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or 'connHandle' is invalid, or if 'attrHandle' is zero. 
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_ReadLongCharacteristicValues
(
    cy_stc_ble_gattc_read_blob_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_ReadMultipleCharacteristicValues
***************************************************************************//**
* 
*  This function reads multiple Characteristic Values from a GATT Server when
*  the GATT Client knows the Characteristic Value Handles. This is a 
*  non-blocking function.
* 
*  Internally, Read Multiple Request is sent to the peer device in response to
*  which Read Multiple Response is received. This results in the 
*  CY_BLE_EVT_GATTC_READ_MULTI_RSP event, which is propagated to the application
*  layer.
* 
*  An Error Response event is sent by the server (CY_BLE_EVT_GATTC_ERROR_RSP) in
*  response to the Read Multiple Request if insufficient authentication, 
*  insufficient authorization, or insufficient encryption key size is used by the
*  client, or if a read operation is not permitted on any of the Characteristic
*  Values. The Error Code parameter is set as specified in the Attribute
*  Protocol.
*
*  Execution of this API generates CY_BLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ event 
*  at the GATT server's end if corresponding characteristic's attribute permission is set to
*  CY_BLE_GATT_DB_ATTR_CHAR_VAL_RD_EVENT.
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.8.4
*  for more details on the sequence of operations.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_read_mult_req_t.
*                     Where, the following needs to be set:
*                     param->handleList
*                     param->listCount
*                                 param->connHandle  
*
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or if any of the input parameters are invalid.
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_ReadMultipleCharacteristicValues
(
    cy_stc_ble_gattc_read_mult_req_t            * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_ReadCharacteristicDescriptors
***************************************************************************//**
* 
*  This function reads a characteristic descriptor from a GATT Server when the
*  GATT Client knows the Attribute handle from the characteristic descriptor
*  declaration. This is a non-blocking function.
* 
*  Internally, Read Request is sent to the peer device in response to which
*  Read Response is received. This response results in CY_BLE_EVT_GATTC_READ_RSP
*  event, which is propagated to the application for handling the event data.
* 
*  An Error Response (the CY_BLE_EVT_GATTC_ERROR_RSP event at the GATT Client's end) 
*  is sent by the GATT Server in response to the Read Request on insufficient
*  authentication or insufficient authorization or insufficient encryption
*  key size is caused by the GATT Client, or if a read operation is not
*  permitted on the Characteristic Value. The Error Code parameter is set as
*  specified in the Attribute Protocol.
*
* 
*  Execution of this API generates CY_BLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ event 
*  at GATT server's end if corresponding attribute's permission has set to
*  CY_BLE_GATT_DB_ATTR_CHAR_VAL_RD_EVENT.
*
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.12.1
*  for more details on the sequence of operations.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_read_req_t.
*                     Where, the following needs to be set:
*                     param->attrHandle: Attribute handle to be read
*                                 param->connHandle  
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or 'connHandle' is invalid, or if 'attrHandle' is zero.
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_ReadCharacteristicDescriptors
(
    cy_stc_ble_gattc_read_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_ReadLongCharacteristicDescriptors
***************************************************************************//**
* 
*  This function reads a characteristic descriptor from a GATT Server when the
*  GATT Client knows the Attribute handle from the characteristic descriptor
*  declaration and the length of the characteristic descriptor declaration is
*  longer than what can be sent in a single Read Response Attribute Protocol
*  message. This is a non-blocking function.
* 
*  Internally multiple Read Blob Requests are sent to the peer device in response
*  to which Read Blob Responses are received. For each Read Blob Request, a Read
*  Blob Response event is received (CY_BLE_EVT_GATTC_READ_BLOB_RSP) with a portion
*  of the Characteristic Value contained in the Part Attribute Value parameter.
*  These events are propagated to the application layer for further processing.
*  Each read blob response will return up to (GATT MTU-1) bytes of data. If the size of 
*  characteristic descriptor field is an integral multiple of (GATT MTU-1) then the 
*  operation terminates with an error response event, where the error code is
*  CY_BLE_GATT_ERR_INVALID_OFFSET. If the size of the characteristic descriptor
*  field is not an integral multiple of (GATT MTU-1), the last read blob response will
*  return data bytes that are less than (GATT MTU-1). The application must monitor 
*  these two conditions before proceeding with the initiation of any other GATT
*  operation.
* 
*  An Error Response event (CY_BLE_EVT_GATTC_ERROR_RSP) is sent by the GATT Server
*  in response to the Read Blob Request if insufficient authentication,
*  insufficient authorization, or insufficient encryption key size is used by the
*  client, or if a read operation is not permitted on the Characteristic Value.
*  The Error Code parameter is set as specified in the Attribute Protocol. If
*  the Characteristic Value is not longer than (GATT MTU - 1) an Error Response with
*  the Error Code set to Attribute Not Long is received by the GATT Client on
*  the first Read Blob Request.
*  
*  Execution of this API generates CY_BLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ event 
*  at the GATT server's end if the corresponding attribute's permission has set to
*  CY_BLE_GATT_DB_ATTR_CHAR_VAL_RD_EVENT.
* 
*  Refer to Bluetooth 4.1 core specification,
*  Volume 3, Part G, section 4.12.2 for more details on the sequence of operations.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_read_blob_req_t.
*                     Where, the following needs to be set:
*                     param->attrHandle
*                     param->offset
*                                 param->connHandle  
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or 'connHandle' is invalid, or if 'attrHandle' is zero. 
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_ReadLongCharacteristicDescriptors
(
    cy_stc_ble_gattc_read_blob_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_WriteCharacteristicValue
***************************************************************************//**
* 
*  This function writes a Characteristic Value to a GATT Server when the GATT
*  Client knows the Characteristic Value Handle. This is a non-blocking function.
* 
*  Internally, Write Request is sent to the GATT Server in response to which
*  Write Response is received. This results in the event
*  CY_BLE_EVT_GATTC_WRITE_RSP, which indicates that the write operation succeeded.
* 
*  An Error Response event (CY_BLE_EVT_GATTC_ERROR_RSP) is sent by the server in
*  response to the Write Request if insufficient authentication, insufficient
*  authorization, or insufficient encryption key size is used by the client, or if
*  a write operation is not permitted on the Characteristic Value. The Error 
*  Code parameter is set as specified in the Attribute Protocol.
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section
*  4.9.3 for more details on the sequence of operations.
*
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_write_req_t.
*                     Where, the following needs to be set:
*                     param->handleValPair
*                                 param->connHandle  
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or 'connHandle' is invalid, or if 'attrHandle' is zero or if 'param->handleValPair.value.len' value is greater than (Effective GATT MTU-3).
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_WriteCharacteristicValue
(
    cy_stc_ble_gattc_write_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_WriteCharacteristicDescriptors
***************************************************************************//**
* 
*  This function writes a characteristic descriptor value to a GATT Server
*  when the GATT Client knows the characteristic descriptor handle. This is a
*  non-blocking function.
* 
*  Internally, Write Request is sent to the GATT Server in response to which
*  Write Response is received. This results in the event 
*  CY_BLE_EVT_GATTC_WRITE_RSP, which indicates that the write operation succeeded.
* 
*  An Error Response event (CY_BLE_EVT_GATTC_ERROR_RSP) is sent by the server in
*  response to the Write Request if insufficient authentication, insufficient 
*  authorization, or insufficient encryption key size is used by the client, or if
*  a write operation is not permitted on the Characteristic Value. The Error 
*  Code parameter is set as specified in the Attribute Protocol.
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.12.3
*  for more details on the sequence of operations.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_write_req_t.
*                     Where, the following needs to be set:
*                     param->handleValPair
*                                 param->connHandle  
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or 'connHandle' is invalid, or if 'attrHandle' is zero or if 'param->handleValPair.value.len' value is greater than (Effective GATT MTU-3).
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_WriteCharacteristicDescriptors
(
    cy_stc_ble_gattc_write_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_WriteLongCharacteristicValues
***************************************************************************//**
* 
*  This function writes a Characteristic Value to a GATT Server when the GATT
*  Client knows the Characteristic Value Handle but the length of the
*  Characteristic Value is longer than GATT MTU size and cannot be sent in a single
*  Write Request Attribute Protocol message. This is a non-blocking function.
* 
*  Internally, multiple Prepare Write Requests are sent to the GATT Server using
*  the CY_BLE_EVT_GATTS_PREP_WRITE_REQ event, in response to which Prepare Write Responses 
*  are received. No events are generated by the BLE Stack during these operations.
* 
*  Prepare Write Requests are repeated until the complete Characteristic Value
*  has been transferred to the GATT Server, after which an Execute Write Request
*  is sent to the GATT Server to write the initially transferred value at the
*  GATT Server's end. This generates CY_BLE_EVT_GATTS_EXEC_WRITE_REQ at the GATT
*  Server's end.
* 
*  Once the GATT Server responds, CY_BLE_EVT_GATTC_EXEC_WRITE_RSP event
*  is generated at the GATT Client's end. The value associated with this event
*  has to be checked by the application layer to confirm that the long write
*  operation succeeded.
* 
*  An Error Response event CY_BLE_EVT_GATTC_ERROR_RSP is
*  received by the GATT Client in response to the Prepare Write Request if
*  insufficient authentication, insufficient authorization, or insufficient
*  encryption key size is used by the client, or if a write operation is not
*  permitted on the Characteristic Value. The Error Code parameter is set as
*  specified in the Attribute Protocol.
*  
*  Execution of this API generates CY_BLE_EVT_MEMORY_REQUEST event for request
*  CY_BLE_PREPARED_WRITE_REQUEST at GATT server's end. GATT server application has to
*  allocate memory of type cy_stc_ble_prepare_write_request_memory_t and set configMemory 
*  in this event parameter to allocated memory.
*     
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.9.4
*  for more details on the sequence of operations.
*  
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_prep_write_req_t,
*                      where 'param->value.val' points to the actual
*                      data to be written. 'param' and all associated
*                      variables need to be retained in memory by the calling
*                      application until the GATT Write Long Characteristic Value
*                      operation is completed successfully.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or 'connHandle' is invalid, or if 'attrHandle' is zero.
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_WriteLongCharacteristicValues
(
    cy_stc_ble_gattc_prep_write_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_ReliableWrites
***************************************************************************//**
* 
*  This function writes a Characteristic Value to a GATT Server when the GATT
*  Client knows the Characteristic Value Handle, and assurance is required
*  that the correct Characteristic Value is going to be written by transferring
*  the Characteristic Value to be written in both directions before the write
*  is performed. This is a non-blocking function.
* 
*  Internally, multiple Prepare Write Requests are sent to the GATT Server using 
*  CY_BLE_EVT_GATTS_PREP_WRITE_REQ event, in response to which Prepare Write Responses 
*  are received. No events are generated by the BLE Stack during these operations.
* 
*  Prepare Write Requests are repeated until the complete Characteristic Value
*  has been transferred to the GATT Server, after which an Execute Write Request
*  is sent to the GATT Server to write the initially transferred value at the
*  GATT Server's end. This generates CY_BLE_EVT_GATTS_EXEC_WRITE_REQ at the GATT
*  Server's end.
* 
*  Once the GATT Server responds, a CY_BLE_EVT_GATTC_EXEC_WRITE_RSP event is
*  generated at the GATT Client's end. The value associated with this event has 
*  to be checked by the application layer to confirm that the long write 
*  operation succeeded. An Error Response event CY_BLE_EVT_GATTC_ERROR_RSP is 
*  received by the GATT Client in response to the Prepare Write Request if 
*  insufficient authentication, insufficient authorization, insufficient 
*  encryption key size is used by the client, or if a write operation is not 
*  permitted on the Characteristic Value. The Error Code parameter is set as 
*  specified in the Attribute Protocol.
*  
*  Execution of this API generates the CY_BLE_EVT_MEMORY_REQUEST event for request
*  CY_BLE_PREPARED_WRITE_REQUEST at the GATT server's end. The GATT server application must
*  allocate memory of type cy_stc_ble_prepare_write_request_memory_t and set configMemory 
*  in this event parameter to allocated memory.
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.9.5
*  for more details on the sequence of operations.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_reliable_write_req_t.
*                      Since more than one writes are performed  'param' and 
*                      all associated variables need to be retained in memory by
*                      the calling application until the GATT Reliable Write 
*                      operation is completed successfully.
*  param->handleValOffsetPair: array of type 'cy_stc_ble_gatt_handle_value_offset_param_t', which 
*                                           contains the values to be written, has to be specified.
*  param->numOfRequests: Number of requests. That is, the count of array of structures 
*                  of type cy_stc_ble_gatt_handle_value_offset_param_t. Each array element 
*                  represents a value and the attribute to which the value has 
*                  to be written.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' or 'handleValOffsetPair' is NULL or 'connHandle' is invalid, or if 'numOfRequests' is zero.
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_ReliableWrites
(
    cy_stc_ble_gattc_reliable_write_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_WriteLongCharacteristicDescriptors
***************************************************************************//**
* 
*  This function writes a characteristic descriptor value to a GATT Server
*  when the GATT Client knows the characteristic descriptor handle but the
*  length of the characteristic descriptor value is longer than what can be
*  sent in a single Write Request Attribute Protocol message. This is a
*  non-blocking function.
* 
*  Internally, multiple Prepare Write Requests are sent
*  to the GATT Server using CY_BLE_EVT_GATTS_PREP_WRITE_REQ event, in response to 
*  which Prepare Write Responses are received. No events are generated by the 
*  BLE Stack during these operations.
* 
*  Prepare Write Requests are repeated until the complete Characteristic
*  Descriptor Value has been transferred to the GATT Server, after which an
*  Execute Write Request is sent to the GATT Server to write the initially
*  transferred value at the GATT Server's end. This generates
*  CY_BLE_EVT_GATTS_EXEC_WRITE_REQ at the GATT Server's end.
* 
*  Once the GATT Server responds, the CY_BLE_EVT_GATTC_EXEC_WRITE_RSP event is
*  generated at the GATT Client's end. The value associated with this event must
*  be checked by the application layer to confirm that the long write operation
*  succeeded.
* 
*  An Error Response event CY_BLE_EVT_GATTC_ERROR_RSP is received by the GATT
*  Client in response to the Prepare Write Request if insufficient
*  authentication, insufficient authorization, or insufficient encryption key
*  size is used by the client, or if a write operation is not permitted on the
*  Characteristic Value. The Error Code parameter is set as specified in the
*  Attribute Protocol.
*
*  Execution of this API generates CY_BLE_EVT_MEMORY_REQUEST event for request
*  CY_BLE_PREPARED_WRITE_REQUEST at GATT server's end. GATT server application must
*  allocate memory of type cy_stc_ble_prepare_write_request_memory_t and set configMemory 
*  in this event parameter to allocated memory.
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3,
*  Part G, section 4.12.4 for more details on the sequence of operations.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_prep_write_req_t,
*                      where 'param->value.val' points to the actual
*                      data to be written. 'param' and all associated
*                      variables must be retained in memory by the calling
*                      application until the GATT Write Long Characteristic Value
*                      operation is completed successfully.
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or 'connHandle' is invalid, or if 'attrHandle' is zero. 
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_WriteLongCharacteristicDescriptors
(
    cy_stc_ble_gattc_prep_write_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_SendExecuteWriteReq
***************************************************************************//**
* 
*  This function allows the user to send execute write request to remote server.
*  This function should be called if client has previously initiated long/reliable 
*  write operation and remote has send error response. Based on error response 
*  application may choose to execute all pending requests or cancel the request.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_exec_write_req_t.
*   param->connHandle: Connection handle to identify the peer GATT entity of type 
*               cy_stc_ble_conn_handle_t.
*   param->flag: Indicates whether Queued Write is to be executed (0x01) or canceled (0x00)
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or 'connHandle' is invalid.
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
*     
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_SendExecuteWriteReq
(
    cy_stc_ble_gattc_exec_write_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_WriteWithoutResponse
***************************************************************************//**
* 
*  This function writes a Characteristic Value to a GATT Server when the GATT
*  Client knows the Characteristic Value Handle and the client does not need an
*  acknowledgment that the write was successfully performed. This is a blocking
*  function. No event is generated on calling this function.
* 
*  Internally, Write Command is sent to the GATT Server and nothing is
*  received in response from the GATT Server.
* 
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.9.1
*  for more details on the sequence of operations.
*
*  On the server side CY_BLE_EVT_GATTS_WRITE_CMD_REQ event is sent to application
*  to indicate GATT Write Without Response request is received from client.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_write_req_t.
*                     Where, the following needs to be set:
*                     param->handleValPair
*                                 param->connHandle  
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or 'connHandle' is invalid, or if 'attrHandle' is zero or if 'param->handleValPair.value.len' value is greater than (Effective GATT MTU-3).
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_WriteWithoutResponse
(
    cy_stc_ble_gattc_write_cmd_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_SignedWriteWithoutRsp
***************************************************************************//**
* 
*  This function writes a Characteristic Value to a server when the client knows
*  the Characteristic Value Handle and the ATT Bearer is not encrypted. This 
*  procedure shall be used only if the Characteristic Properties
*  authenticated bit is enabled and the client and server device share a bond
*  as defined in Bluetooth Spec4.1 [Vol. 3] Part C, Generic Access Profile.
* 
*  This function only writes the first (GATT_MTU - 15) octets of an 
*  Attribute Value. This function cannot be used to write a long Attribute.
* 
*  Internally, Signed Write Command is used.
*  Refer to Bluetooth Spec 4.1 Security Manager [Vol. 3] Part H, Section 2.4.5.
* 
*  If the authenticated Characteristic Value that is written is the wrong size, 
*  has an invalid value as defined by the profile, or the signed value does not
*  authenticate the client, then the write shall not succeed and no error shall
*  be generated by the server.
*  
*  On the server side CY_BLE_EVT_GATTS_DATA_SIGNED_CMD_REQ event is sent to the application
*  to indicate the GATT Signed write cmd is received from the client.
* 
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_write_req_t.
*                     Where, the following needs to be set:
*                     param->handleValPair
*                                 param->connHandle  
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or if 'attrHandle' is zero.
*  CY_BLE_ERROR_NO_DEVICE_ENTITY        | 'connHandle.bdHandle' value does not represent any existing entry in the Stack.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
*  CY_BLE_ERROR_INSUFFICIENT_RESOURCES  | Stack resources are unavailable.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_SignedWriteWithoutRsp
(
    cy_stc_ble_gattc_signed_write_cmd_req_t  * param
);


/******************************************************************************
* Function Name: Cy_BLE_GATTC_Confirmation
***************************************************************************//**
* 
*  This function sends confirmation to the GATT Server on receiving Handle Value
*  Indication event CY_BLE_EVT_GATTC_HANDLE_VALUE_IND at the GATT Client's end.
*  This is a non-blocking function.
* 
*  This function call results in CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF event at the
*  GATT Server's end.
* 
*  Refer to the Bluetooth 4.1 core specification, Volume 3, Part G, section 4.11.1
*  for more details on the sequence of operations.
*     
*  \param param: Pointer to a variable of type cy_stc_ble_gattc_confirmation_req_t.
*                     Where, the following needs to be set:
*                                 param->connHandle  
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Error codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If 'param' is NULL or 'connHandle' is invalid.
*  CY_BLE_ERROR_INVALID_OPERATION       | This operation is not permitted.
*  CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED| Memory allocation failed.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_Confirmation
(
    cy_stc_ble_gattc_confirmation_req_t  * param
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */

#endif /* CY_BLE_STACK_GATT_CLIENT_H_ */

/* EOF */
