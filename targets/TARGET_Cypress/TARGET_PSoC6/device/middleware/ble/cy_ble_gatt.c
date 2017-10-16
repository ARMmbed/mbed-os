/***************************************************************************//**
* \file cy_ble_gatt.c
* \version 2.0
*
* \brief
*  This file contains the source code for the GATT API of the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#include "cy_ble_event_handler.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if (CY_BLE_MODE_PROFILE)

/***************************************
* Global variables
***************************************/
#if (CY_BLE_GATT_ROLE_SERVER)

#if (CY_BLE_GATT_DB_CCCD_COUNT != 0u)
uint8_t cy_ble_attValuesCccdMultiple[CY_BLE_CONN_COUNT][CY_BLE_GATT_DB_CCCD_COUNT + CY_BLE_CCCD_CRC_BYTE];
#endif  /* (CY_BLE_GATT_DB_CCCD_COUNT != 0u) */


/******************************************************************************
* Function Name: Cy_BLE_GATTS_ReInitGattDb
***************************************************************************//**
*
*  Reinitializes the GATT database.
*
*  \return
*  cy_en_ble_api_result_t: A function result states if it succeeded or failed with
*  error codes:
*
*  Error Codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | GATT database was reinitialized successfully.
*  CY_BLE_ERROR_INVALID_STATE           | If the function is called in any state except CY_BLE_STATE_ON.
*  CY_BLE_ERROR_INVALID_PARAMETER       | If the Database has zero entries or is a NULL pointer.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTS_ReInitGattDb(void)
{
    cy_en_ble_api_result_t apiResult;

    if(Cy_BLE_GetState() == CY_BLE_STATE_ON)
    {
        cy_stc_ble_gatts_db_info_t dbInfoParam =
        {
            .gattDbPtr          = cy_ble_configPtr->gattDB,
            .gattDbTotalEntries = cy_ble_configPtr->params->gattDbIndexCount,
            .gattDbMaxValue     = CY_BLE_CONFIG_GATT_DB_MAX_VALUE_LEN
        };
        apiResult = Cy_BLE_GATTS_DbRegister(&dbInfoParam);
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GATTS_WriteEventHandler
***************************************************************************//**
*
*  Handles the Write Request Event for GATT service.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
*  \return
*  cy_en_ble_gatt_err_code_t: A function returns one of the following status
*  values.
*
*  Error Codes                          | Description
*  --------------------                 | -----------
*  CY_BLE_GATT_ERR_NONE                 | Write is successful.
*
******************************************************************************/
cy_en_ble_gatt_err_code_t Cy_BLE_GATTS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;

    /* Client Characteristic Configuration descriptor write request */
    if(eventParam->handleValPair.attrHandle == cy_ble_configPtr->gatts->cccdHandle)
    {
        /* Store value to database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair = eventParam->handleValPair,
            .connHandle      = eventParam->connHandle,
            .flags           = CY_BLE_GATT_DB_PEER_INITIATED,
            .offset          = 0u
        };

        gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);
        if(gattErr == CY_BLE_GATT_ERR_NONE)
        {
            if(CY_BLE_IS_INDICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
            {
                Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTS_INDICATION_ENABLED, eventParam);
            }
            else
            {
                Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTS_INDICATION_DISABLED, eventParam);
            }
        }
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }

    return(gattErr);
}


/******************************************************************************
* Function Name: Cy_BLE_GATTS_WriteAttributeValueCCCD
***************************************************************************//**
*
*  This function extends Cy_BLE_GATTS_WriteAttributeValue API to support 
*  storing of separate CCCD values for each (separate) connections.
*
*  This function is used to write to the value field of the specified attribute
*  in the GATT database of a GATT Server. If attribute is CCCD, the value is 
*  stored in specific CCCD storages.
*  This is a blocking function. No event is generated on calling this function.
*
*  If a peer device connected to the GATT Server initiates a write operation,
*  this function is executed on the GATT Server. During such a call, the function
*  checks for the attribute permissions (flags) before executing the write
*  operation by calling.
*
*  \param param: Parameter is of type 'cy_stc_ble_gatts_db_attr_val_info_t'.
*
* \return
*  Return value is GATT Error code specified in 'cy_en_ble_gatt_err_code_t
*
******************************************************************************/
cy_en_ble_gatt_err_code_t Cy_BLE_GATTS_WriteAttributeValueCCCD(cy_stc_ble_gatts_db_attr_val_info_t *param)
{
    cy_en_ble_gatt_err_code_t errCode;

    errCode = Cy_BLE_GATTS_WriteAttributeValue(param);

#if (CY_BLE_GATT_DB_CCCD_COUNT != 0u)
    /* Support separate CCCD value for each connection */
    if((errCode == CY_BLE_GATT_ERR_NONE) &&
       (CY_BLE_GATT_DB_ATTR_CHECK_PRPTY(param->handleValuePair.attrHandle, CY_BLE_GATT_DB_CCCD_ATTR)))
    {
        (void)memcpy((void*)&cy_ble_attValuesCccdMultiple[param->connHandle.attId]
                     [CY_BLE_GATT_DB_ATTR_GET_CCCD_IDX(param->handleValuePair.attrHandle)],
                     (void*)param->handleValuePair.value.val, (uint32_t)param->handleValuePair.value.len);
    }
#endif /* (CY_BLE_GATT_DB_CCCD_COUNT != 0u) */

    return(errCode);
}


/******************************************************************************
* Function Name: Cy_BLE_GATTS_ReadAttributeValueCCCD
***************************************************************************//**
*
*  This function extends Cy_BLE_GATTS_ReadAttributeValue API to support 
*  re-storing of separate CCCD values for each(separate) connections.
*
*  This function is used to read the value field of the specified attribute from
*  the GATT database in a GATT Server. If attribute is CCCD, the value is 
*  restored from specific CCCD storages.
*  This is a blocking function. No event is generated on calling this function.
*
*  \param param: Parameter is of type 'cy_stc_ble_gatts_db_attr_val_info_t'.
*  param-> offset: not used, to be ignored
*
* \return
*  cy_en_ble_gatt_err_code_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*  Error Codes                            | Description
*  ------------                           | -----------
*   CY_BLE_GATT_ERR_NONE                  | On successful operation
*   CY_BLE_GATT_ERR_INVALID_HANDLE        | 'handleValuePair.attrHandle' is not valid
*   CY_BLE_GATT_ERR_READ_NOT_PERMITTED    | Read operation is not permitted on this attribute
*   CY_BLE_GATT_ERR_UNLIKELY_ERROR        | Invalid arguments passed
*
******************************************************************************/
cy_en_ble_gatt_err_code_t Cy_BLE_GATTS_ReadAttributeValueCCCD(cy_stc_ble_gatts_db_attr_val_info_t *param)
{
    cy_en_ble_gatt_err_code_t errCode;

    if((param->handleValuePair.attrHandle == 0u) || (param->handleValuePair.attrHandle > CY_BLE_GATT_DB_INDEX_COUNT) ||
       (param->connHandle.attId >= CY_BLE_CONN_COUNT))
    {
        errCode = CY_BLE_GATT_ERR_INVALID_HANDLE;
    }
    else
    {
    #if (CY_BLE_GATT_DB_CCCD_COUNT != 0u)
        if(CY_BLE_GATT_DB_ATTR_CHECK_PRPTY(param->handleValuePair.attrHandle, CY_BLE_GATT_DB_CCCD_ATTR))
        {
            CY_BLE_GATT_DB_ATTR_SET_GEN_VALUE(param->handleValuePair.attrHandle,
                                              (void*)&cy_ble_attValuesCccdMultiple[param->connHandle.attId]
                                              [CY_BLE_GATT_DB_ATTR_GET_CCCD_IDX(param->handleValuePair.attrHandle)],
                                              CY_BLE_CCCD_LEN);
        }
    #endif /* (CY_BLE_GATT_DB_CCCD_COUNT != 0u) */

        errCode = Cy_BLE_GATTS_ReadAttributeValue(param);
    }

    return(errCode);
}

#if (CY_BLE_GATT_DB_CCCD_COUNT != 0u)


/******************************************************************************
* Function Name: Cy_BLE_GATTS_ReadAttributeValueCCCDReqHandler
***************************************************************************//**
*
*  This function is used to handle CY_BLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ
*  request and updated CCCD value in GATT database in a GATT Server for
*  multi-connection support.
*
*  \param param: Parameter is of type 'cy_stc_ble_gatts_char_val_read_req_t'.
*
* \return
*  cy_en_ble_gatt_err_code_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*  Error Codes                          | Description
*  ------------                         | -----------
*   CY_BLE_GATT_ERR_NONE                | On successful operation
*   CY_BLE_GATT_ERR_INVALID_HANDLE      | 'handleValuePair.attrHandle' is not valid
*
******************************************************************************/
cy_en_ble_gatt_err_code_t Cy_BLE_GATTS_ReadAttributeValueCCCDReqHandler(const cy_stc_ble_gatts_char_val_read_req_t *param)
{
    cy_en_ble_gatt_err_code_t errCode = CY_BLE_GATT_ERR_NONE;

    if((param->attrHandle == 0u) || (param->attrHandle > CY_BLE_GATT_DB_INDEX_COUNT) ||
       (param->connHandle.attId >= CY_BLE_CONN_COUNT))
    {
        errCode = CY_BLE_GATT_ERR_INVALID_HANDLE;
    }
    else
    {
        if(CY_BLE_GATT_DB_ATTR_CHECK_PRPTY(param->attrHandle, CY_BLE_GATT_DB_CCCD_ATTR))
        {
            CY_BLE_GATT_DB_ATTR_SET_GEN_VALUE(param->attrHandle,
                                              (void*)&cy_ble_attValuesCccdMultiple[param->connHandle.attId]
                                              [CY_BLE_GATT_DB_ATTR_GET_CCCD_IDX(param->attrHandle)],
                                              CY_BLE_CCCD_LEN);
        }
    }

    return(errCode);
}

#endif /* (CY_BLE_GATT_DB_CCCD_COUNT != 0u) */

#endif /* (CY_BLE_GATT_ROLE_SERVER) */

#if (CY_BLE_GATT_ROLE_CLIENT)


/******************************************************************************
* Function Name: Cy_BLE_GATTC_StartDiscovery
***************************************************************************//**
*
*  Starts the automatic server discovery process. 
*   
*   Discovery procedure is based on the user configuration. It discovers only 
*    services, characteristics, descriptors which were declared in the 
*    GATT database. Discovery procedure has the following flow:
*   - discovering primary services by 
*        BLE Stack function Cy_BLE_GATTC_DiscoverPrimaryServices();
*   - discovering included services by 
*        BLE Stack function Cy_BLE_GATTC_FindIncludedServices();
*   - discovering characteristics for available services by 
*        BLE Stack function Cy_BLE_GATTC_DiscoverCharacteristicDescriptors();
*   - discovering characteristic descriptors by 
*        BLE Stack function CyBle_GattcDiscoverAllCharacteristicDescriptors();
*
*   During the discovery procedure the discovery-specific stack events are 
*   handled by the component and thus aren't passed to the application callback:
*   CY_BLE_EVT_GATTC_READ_BY_GROUP_TYPE_RSP, CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP,
*   CY_BLE_EVT_GATTC_FIND_INFO_RSP, CY_BLE_EVT_GATTC_ERROR_RSP.              
*   
*   After discovering procedure all information about available services is 
*   stored in cy_stc_ble_disc_srvc_info_t structures, and discovered 
*   attributes handles are stored in service-specific client structures, 
*   such as cy_stc_ble_basc_t for Battery Service or cy_stc_ble_hrsc_t for
*   Heart Rate Service.
*                
*   The following events may be generated after calling this function:
*   CY_BLE_EVT_GATTC_DISCOVERY_COMPLETE - event is generated when the remote 
*                                         device was successfully discovered. 
*   CY_BLE_EVT_GATTC_ERROR_RSP          - is generated if the device discovery
*                                         has failed.
*   CY_BLE_EVT_GATTC_SRVC_DUPLICATION   - is generated if duplicate service 
*                                         record was found during the server 
*                                         device discovery.
*   CY_BLE_EVT_GATTC_CHAR_DUPLICATION   - is generated if duplicate service's 
*                                         characteristic descriptor record was
*                                         found during the server device 
*                                         discovery.
*   CY_BLE_EVT_GATTC_DESCR_DUPLICATION  - is generated if duplicate service's 
*                                         characteristic descriptor record was
*                                         found during the server device 
*                                         discovery. 
*
* \return
*    cy_en_ble_api_result_t : Return value indicates if the function
*                             succeeded or failed. 
*                             The following are possible error codes:
*
*   <table>
*   <tr>
*      <th>Error Codes</th>
*      <th>Description</th>
*    </tr>
*    <tr>
*      <td>CY_BLE_SUCCESS</td>
*      <td>On successful operation</td>
*    </tr>
*    <tr>
*      <td>CY_BLE_ERROR_INVALID_PARAMETER</td>
*      <td>'connHandle' value does not represent any existing entry.</td>
*    </tr>
*    <tr>
*      <td>CY_BLE_ERROR_INVALID_OPERATION</td>
*      <td>The operation is not permitted</td>
*    </tr>
*   <tr>
*      <td>CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED</td>
*      <td>Memory allocation failed</td>
*    </tr>
*   <tr>
*      <td>CY_BLE_ERROR_INVALID_STATE</td>
*      <td>If the function is called in any state except connected or discovered</td>
*    </tr>
*   </table>
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_StartDiscovery(cy_stc_ble_conn_handle_t connHandle)
{
    uint32_t discIdx;
    cy_en_ble_api_result_t apiResult;
    cy_en_ble_conn_state_t connState = Cy_BLE_GetConnectionState(connHandle);

    if((connState != CY_BLE_CONN_STATE_CONNECTED) && (connState != CY_BLE_CONN_STATE_CLIENT_DISCOVERED))
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else
    {
        /* Clean old discovery information */
        uint32_t i;

        /* Register connHandle in cy_ble_discovery structure */
        apiResult = Cy_BLE_GATTC_AddConnHandle(connHandle);
        discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

        for(i = 0u; i < (uint8_t)CY_BLE_SRVI_COUNT; i++)
        {
            (void)memset(&cy_ble_serverInfo[discIdx][i].range, 0, sizeof(cy_ble_serverInfo[0][0].range));
        }
    }

    if(apiResult == CY_BLE_SUCCESS)
    {
        cy_stc_ble_gattc_read_by_group_req_t reqParam =
        {
            .connHandle = connHandle
        };

        cy_ble_discovery[discIdx].gattcDiscoveryRange.startHandle = CY_BLE_GATT_ATTR_HANDLE_START_RANGE;
        cy_ble_discovery[discIdx].gattcDiscoveryRange.endHandle = CY_BLE_GATT_ATTR_HANDLE_END_RANGE;
        reqParam.range = cy_ble_discovery[discIdx].gattcDiscoveryRange;

        Cy_BLE_ServiceInit();


        apiResult = Cy_BLE_GATTC_DiscoverPrimaryServices(&reqParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            Cy_BLE_SetConnectionState(connHandle, CY_BLE_CONN_STATE_CLIENT_SRVC_DISCOVERING);
            cy_ble_discovery[discIdx].autoDiscoveryFlag = 1u;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GATTC_StartPartialDiscovery
***************************************************************************//**
*
*  Starts the automatic server discovery process as per the range provided
*  on a GATT Server to which it is connected. This function could be used for
*  partial server discovery after indication received to the Service Changed
*  Characteristic Value. Two events may be generated
*  after calling this function - CY_BLE_EVT_GATTC_DISCOVERY_COMPLETE or
*  CY_BLE_EVT_GATTC_ERROR_RSP. The CY_BLE_EVT_GATTC_DISCOVERY_COMPLETE event is
*  generated when the remote device was successfully discovered. The
*  CY_BLE_EVT_GATTC_ERROR_RSP is generated if the device discovery is failed.
*
*  \param connHandle: The handle which consists of the device ID and ATT connection ID.
*  \param startHandle: Start of affected attribute handle range.
*  \param endHandle: End of affected attribute handle range.
*
*  \return
*    cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*                             failed. The following are possible error codes.
*
*   <table>
*   <tr>
*      <th>Error Codes</th>
*      <th>Description</th>
*    </tr>
*    <tr>
*      <td>CY_BLE_SUCCESS</td>
*      <td>On successful operation</td>
*    </tr>
*    <tr>
*      <td>CY_BLE_ERROR_INVALID_PARAMETER</td>
*      <td>'connHandle' value does not represent any existing entry.</td>
*    </tr>
*    <tr>
*      <td>CY_BLE_ERROR_INVALID_OPERATION</td>
*      <td>The operation is not permitted</td>
*    </tr>
*   <tr>
*      <td>CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED</td>
*      <td>Memory allocation failed</td>
*    </tr>
*   <tr>
*      <td>CY_BLE_ERROR_INVALID_STATE</td>
*      <td>If the function is called in any state except connected or discovered</td>
*    </tr>
*   </table>
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_StartPartialDiscovery(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_ble_gatt_db_attr_handle_t startHandle,
                                                          cy_ble_gatt_db_attr_handle_t endHandle)
{
    uint32_t discIdx;
    cy_en_ble_api_result_t apiResult;
    cy_en_ble_conn_state_t connState = Cy_BLE_GetConnectionState(connHandle);

    if((connState != CY_BLE_CONN_STATE_CONNECTED) && (connState != CY_BLE_CONN_STATE_CLIENT_DISCOVERED))
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else
    {
        uint32_t i;

        /* Register connHandle in cy_ble_discovery structure */
        apiResult = Cy_BLE_GATTC_AddConnHandle(connHandle);
        discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

        /* Clean old discovery information of affected attribute range */
        for(i = 0u; i < (uint8_t)CY_BLE_SRVI_COUNT; i++)
        {
            if((cy_ble_serverInfo[discIdx][i].range.startHandle >= startHandle) &&
               (cy_ble_serverInfo[discIdx][i].range.startHandle <= endHandle))
            {
                (void)memset(&cy_ble_serverInfo[discIdx][i].range, 0, sizeof(cy_ble_serverInfo[0u][0u].range));
            }
        }
    }

    if(apiResult == CY_BLE_SUCCESS)
    {
        cy_stc_ble_gattc_read_by_group_req_t readByGroupReqParam =
        {
            .connHandle = connHandle
        };

        cy_ble_discovery[discIdx].gattcDiscoveryRange.startHandle = startHandle;
        cy_ble_discovery[discIdx].gattcDiscoveryRange.endHandle = endHandle;

        Cy_BLE_ServiceInit();

        readByGroupReqParam.range = cy_ble_discovery[discIdx].gattcDiscoveryRange;

        apiResult = Cy_BLE_GATTC_DiscoverPrimaryServices(&readByGroupReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            Cy_BLE_SetConnectionState(connHandle, CY_BLE_CONN_STATE_CLIENT_SRVC_DISCOVERING);
            cy_ble_discovery[discIdx].autoDiscoveryFlag = 1u;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GATTC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a "CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP"
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
* \return
*  None
*
******************************************************************************/
void Cy_BLE_GATTC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].uuid == CY_BLE_UUID_GATT_SERVICE))
    {
        if(discCharInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_SERVICE_CHANGED)
        {
            Cy_BLE_CheckStoreCharHandle(cy_ble_discovery[discIdx].gattc.serviceChanged);
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_GATTC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a "CY_BLE_EVT_GATTC_FIND_INFO_RSP" event.
*  Based on the descriptor UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discDescrInfo: The pointer to a descriptor information structure.
*
* \return
*  None
*
******************************************************************************/
void Cy_BLE_GATTC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_GATT)
    {
        if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_CLIENT_CONFIG)
        {
            Cy_BLE_CheckStoreCharDescrHandle(cy_ble_discovery[discIdx].gattc.cccdHandle);
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}

/******************************************************************************
* Function Name: Cy_BLE_GATTC_GetCharRange
***************************************************************************//**
*
* Returns a possible range of the current characteristic descriptor via
* input parameter charRangeInfo->range
*
* \param *charRangeInfo: The pointer to a descriptor range information structure.
* \return
*  None.
*
******************************************************************************/
void Cy_BLE_GATTC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_GATT)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        if(cy_ble_discovery[discIdx].charCount == 0u)
        {
            charRangeInfo->range.startHandle = cy_ble_discovery[discIdx].gattc.serviceChanged.
                                                valueHandle + 1u;
            charRangeInfo->range.endHandle = cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].
                                              range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_GATTC_IndicationEventHandler
***************************************************************************//**
*
*  Handles the Indication Event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*  None.
*
******************************************************************************/
void Cy_BLE_GATTC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) &&
       (cy_ble_discovery[discIdx].gattc.serviceChanged.valueHandle == eventParam->handleValPair.attrHandle))
    {
        Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_INDICATION, eventParam);
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_GATTC_AddConnHandle
***************************************************************************//**
*
*  Register connHandle in cy_ble_discovery structure.
*
* \param connHandle: The handle which consists of the device ID and ATT connection ID.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - All client instances are used. 
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_AddConnHandle(cy_stc_ble_conn_handle_t connHandle)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    uint32_t fFlag = (uint32_t)CY_BLE_INVALID_CONN_HANDLE_VALUE;
    uint32_t i;

    /* Register connHandle in cy_ble_discovery structure */
    for(i = 0u; (i < CY_BLE_CONFIG_GATTC_COUNT) && (fFlag == CY_BLE_INVALID_CONN_HANDLE_VALUE); i++)
    {
        /* fFlag shows that we have already had connHandle index in cy_ble_discovery */
        if(cy_ble_discovery[i].connIndex != CY_BLE_INVALID_CONN_HANDLE_VALUE)
        {
            if(cy_ble_connHandle[cy_ble_discovery[i].connIndex].bdHandle == connHandle.bdHandle)
            {
                fFlag = i;
            }
        }
    }
    if(fFlag == CY_BLE_INVALID_CONN_HANDLE_VALUE)
    {
        for(i = 0u; (i < CY_BLE_CONFIG_GATTC_COUNT) && (fFlag == CY_BLE_INVALID_CONN_HANDLE_VALUE); i++)
        {
            if(cy_ble_discovery[i].connIndex == CY_BLE_INVALID_CONN_HANDLE_VALUE)
            {
                uint32_t j;
                for(j = 0u; (j < CY_BLE_CONN_COUNT) && (fFlag == CY_BLE_INVALID_CONN_HANDLE_VALUE); j++)
                {
                    if(cy_ble_connHandle[j].bdHandle == connHandle.bdHandle)
                    {
                        cy_ble_discovery[i].connIndex = j;
                        fFlag = i;
                    }
                }
            }
        }
    }
    if(fFlag == CY_BLE_INVALID_CONN_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED;
    }

    return(apiResult);
}

/******************************************************************************
* Function Name: Cy_BLE_GATTC_RemoveConnHandle
***************************************************************************//**
*
*  Unregister connHandle from cy_ble_discovery structure.
*  This function removes all connHandle indexes from cy_ble_discovery structure
*  if connHandle.bdHandle parameter is set to CY_BLE_INVALID_CONN_HANDLE_VALUE.
*
* \param connHandle: The handle which consists of the device ID and ATT connection ID.
*
* \return: A function result states if it succeeded or failed with
*  error codes:
*
*  Error Codes                          | Description
*  ------------                         | -----------
*  CY_BLE_SUCCESS                       | On successful operation
*  CY_BLE_ERROR_NO_DEVICE_ENTITY        | If discovery information is absent
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTC_RemoveConnHandle(cy_stc_ble_conn_handle_t connHandle)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    uint32_t fFlag = CY_BLE_INVALID_CONN_HANDLE_VALUE;
    uint32_t i;

    /* Remove connHandle from cy_ble_discovery structure */
    if(connHandle.bdHandle != CY_BLE_INVALID_CONN_HANDLE_VALUE)
    {
        for(i = 0u; (i < CY_BLE_CONFIG_GATTC_COUNT) && (fFlag == CY_BLE_INVALID_CONN_HANDLE_VALUE); i++)
        {
            /* fFlag shows that we found connHandle in cy_ble_discovery */
            if(cy_ble_connHandle[cy_ble_discovery[i].connIndex].bdHandle == connHandle.bdHandle)
            {
                (void)memset(&cy_ble_discovery[i], 0, sizeof(cy_stc_ble_discovery_t));
                cy_ble_discovery[i].connIndex = CY_BLE_INVALID_CONN_HANDLE_VALUE;
                fFlag = i;
            }
        }
        if(fFlag == CY_BLE_INVALID_CONN_HANDLE_VALUE)
        {
            apiResult = CY_BLE_ERROR_NO_DEVICE_ENTITY;
        }
    }
    else
    {
        /* Remove all connHandle indexes from cy_ble_discovery structure */
        (void)memset(&cy_ble_discovery, 0, sizeof(cy_ble_discovery));

        /* Update cy_ble_discovery[].connIndex with init values (CY_BLE_INVALID_CONN_HANDLE_VALUE) */
        for(i = 0u; i < CY_BLE_CONFIG_GATTC_COUNT; i++)
        {
            cy_ble_discovery[i].connIndex = CY_BLE_INVALID_CONN_HANDLE_VALUE;
        }
    }

    /* Clean old connection handle information */
    for(i = 0u; i < CY_BLE_CONN_COUNT; i++)
    {
        if(cy_ble_connHandle[i].bdHandle == connHandle.bdHandle)
        {
            cy_ble_connHandle[i].bdHandle = CY_BLE_INVALID_CONN_HANDLE_VALUE;
            cy_ble_connHandle[i].attId = CY_BLE_INVALID_CONN_HANDLE_VALUE;
        }
    }

    return(apiResult);
}

/******************************************************************************
* Function Name: Cy_BLE_GetDiscoveryIdx
***************************************************************************//**
*
*  This function returns the index of discovery structure according to connHandle.
*
* \return
*  uint8_t: index of discovery structure
*  CY_BLE_CONFIG_GATTC_COUNT: index is not found for connHandle
*
******************************************************************************/
uint32_t Cy_BLE_GetDiscoveryIdx(cy_stc_ble_conn_handle_t connHandle)
{
    uint32_t idx;
    uint32_t retValue = CY_BLE_CONFIG_GATTC_COUNT;

    for(idx = 0u; ((idx < CY_BLE_CONFIG_GATTC_COUNT) && (retValue == CY_BLE_CONFIG_GATTC_COUNT)); idx++)
    {
        if(cy_ble_connHandle[cy_ble_discovery[idx].connIndex].bdHandle == connHandle.bdHandle)
        {
            retValue = idx;
        }
    }

    return(retValue);
}

#endif /* (CY_BLE_GATT_ROLE_CLIENT) */
#endif /* defined(CY_BLE_MODE_PROFILE) */
#ifdef __cplusplus
}
#endif /* __cplusplus */


/* [] END OF FILE */
