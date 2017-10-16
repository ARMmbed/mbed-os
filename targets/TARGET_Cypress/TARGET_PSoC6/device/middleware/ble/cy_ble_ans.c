/***************************************************************************//**
* \file cy_ble_ans.c
* \version 2.0
*
* \brief
*  Contains the source code for the Alert Notification Service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_ANS))

static cy_ble_callback_t Cy_BLE_ANS_ApplCallback = NULL;

#ifdef CY_BLE_ANS_CLIENT
/* ANS Center Service characteristics GATT DB handles structure */
cy_stc_ble_ansc_t cy_ble_ansc[CY_BLE_CONFIG_GATTC_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_anscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_ANS_CLIENT) */

/* The pointer to the global BLE ANS config structure */
cy_stc_ble_ans_config_t *cy_ble_ansConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_ANS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_ANSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* CY_BLE_ANS_SERVER */

#ifdef CY_BLE_ANS_CLIENT
static void Cy_BLE_ANSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_ANSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_ANSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_ANSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_ANSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_ANSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_ANSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* (CY_BLE_ANS_CLIENT) */
static cy_en_ble_gatt_err_code_t Cy_BLE_ANS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_ANS_Init
***************************************************************************//**
*
*  This function initializes the Alert Notification Service.
*
*  \param config: The configuration structure for the Alert Notification Service.
*
*  \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*   Error Codes                             | Description
*   ------------                            | -----------
*   CY_BLE_SUCCESS                          | The function completed successfully.
*   CY_BLE_ERROR_INVALID_PARAMETER          | On specifying NULL as input parameter
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED   | Buffer overflow in the registration callback.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANS_Init(cy_stc_ble_ans_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to the config structure */
        cy_ble_ansConfigPtr = config;

        /* Registers Event Handler for the ANS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_ANS_EventHandler);

        /* Registers a callback function via the config structure */
        if(cy_ble_ansConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_ANS_ApplCallback = cy_ble_ansConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_ANS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_ANS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_ansc[idx], 0, sizeof(cy_stc_ble_ansc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_ANS].uuid = CY_BLE_UUID_ALERT_NOTIFICATION_SERVICE;
                }
                cy_ble_anscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_ANS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ANS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for Alert Notification Service specific
*  attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc: An application layer event callback function to receive
*                    service-specific events from the BLE Component. The
*                    definition of cy_ble_callback_t for Alert Notification
*                    Service is,\n
*                    typedef void (* cy_ble_callback_t)
*                        (uint32_t eventCode, void *eventParam)
*                    * eventCode indicates the event that triggered this
*                       callback (e.g. CY_BLE_EVT_ANSS_NOTIFICATION_ENABLED)
*                    * eventParam contains the parameters corresponding to
*                      the current event (e.g. Pointer to cy_stc_ble_ans_char_value_t
*                      structure that contains details of the  characteristic
*                      for which notification enabled event was triggered).
*
* \sideeffect
*  The *eventParams in the callback function should not be used by the
*  application once the callback function execution is finished. Otherwise
*  this data may become corrupted.
*
*  \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes:
*
*   Error Codes                             | Description
*   ------------                            | -----------
*   CY_BLE_SUCCESS                          | The function completed successfully.
*   CY_BLE_ERROR_INVALID_OPERATION          | This operation is not permitted
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_ANS_ApplCallback = callbackFunc;
    if(cy_ble_ansConfigPtr != NULL)
    {
        cy_ble_ansConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_ANS_SERVER

/******************************************************************************
* Function Name: Cy_BLE_ANSS_WriteEventHandler
***************************************************************************//**
*
*  Handles Write Request Event for Alert Notification Service.
*
*  \param eventParam: The pointer to the data that came with a write request for
*                Alert Notification Service.
*
* \return
*  Return a value of type cy_en_ble_gatt_err_code_t:
*   * CY_BLE_GATT_ERR_NONE - The function terminated successfully.
*   * CY_BLE_GATT_ERR_INVALID_HANDLE - The handle of Client Configuration
*     Characteristic Descriptor or Characteristic of Alert Notification Service
*     is not valid.
*   * CY_BLE_GATT_ERR_UNLIKELY_ERROR - An Internal Stack error occurred.
*   * CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED - The notification property of a
*     specific characteristic of Alert Notification Service is disabled.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_ANSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_stc_ble_ans_char_value_t wrReqParam;
    cy_ble_gatt_db_attr_handle_t tmpHandle;
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    uint32_t fFlag = 0u;
    uint32_t event = (uint32_t)CY_BLE_EVT_ANSS_WRITE_CHAR;

    tmpHandle = eventParam->handleValPair.attrHandle;

    /* Client Characteristic Configuration Descriptor Write Request */
    if((tmpHandle == cy_ble_ansConfigPtr->anss->charInfo[CY_BLE_ANS_NEW_ALERT].descrHandle[CY_BLE_ANS_CCCD]) ||
       (tmpHandle == cy_ble_ansConfigPtr->anss->charInfo[CY_BLE_ANS_UNREAD_ALERT_STATUS].descrHandle[CY_BLE_ANS_CCCD]))
    {
        /* Verify that optional notification property is enabled for Characteristic */
        if(CY_BLE_IS_NOTIFICATION_SUPPORTED(cy_ble_ansConfigPtr->anss->charInfo[CY_BLE_ANS_NEW_ALERT].charHandle) ||
           CY_BLE_IS_NOTIFICATION_SUPPORTED(cy_ble_ansConfigPtr->anss->charInfo[CY_BLE_ANS_UNREAD_ALERT_STATUS].charHandle))
        {
            if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
            {
                event = (uint32_t)CY_BLE_EVT_ANSS_NOTIFICATION_ENABLED;
            }
            else
            {
                event = (uint32_t)CY_BLE_EVT_ANSS_NOTIFICATION_DISABLED;
            }

            if(tmpHandle == cy_ble_ansConfigPtr->anss->charInfo[CY_BLE_ANS_NEW_ALERT].descrHandle[CY_BLE_ANS_CCCD])
            {
                wrReqParam.charIndex = CY_BLE_ANS_NEW_ALERT;
            }
            else
            {
                wrReqParam.charIndex = CY_BLE_ANS_UNREAD_ALERT_STATUS;
            }

            /* Value is NULL for descriptors */
            wrReqParam.value = NULL;
        #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
            /* Set fFlag to store bonding data to flash */
            if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
            {
                cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
            }
        #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */
        }
        else
        {
            gattErr = CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED;
        }
    }
    else if(tmpHandle == cy_ble_ansConfigPtr->anss->charInfo[CY_BLE_ANS_ALERT_NTF_CONTROL_POINT].charHandle)
    {
        /* Validate Command ID and Category ID ranges */
        if((eventParam->handleValPair.value.val[0u] <= CY_BLE_ANS_IMM_UNREAD_ALERT_STATUS_NTF) &&
           ((eventParam->handleValPair.value.val[1u] <= CY_BLE_ANS_CAT_ID_INSTANT_MESSAGE) ||
            (eventParam->handleValPair.value.val[1u] == CY_BLE_ANS_CAT_ID_ALL)))
        {
            wrReqParam.charIndex = CY_BLE_ANS_ALERT_NTF_CONTROL_POINT;
            wrReqParam.value = &eventParam->handleValPair.value;
        }
        else /* Command ID or Category ID received is invalid */
        {
            /* Erroneous request won't be notified to user but Error response will be sent. */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;

            gattErr = CY_BLE_GATT_ERR_ANS_COMMAND_NOT_SUPPORTED;
        }
    }
    else
    {
        /* Requested handle does not belong to Alert Notification Service
         *  Characteristic or Descriptor. */
        fFlag = 1u;
    }

    if((gattErr == CY_BLE_GATT_ERR_NONE) && (fFlag == 0u))
    {
        /* Write value to GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair = eventParam->handleValPair,
            .connHandle      = eventParam->connHandle,
            .offset          = 0u,
            .flags           = CY_BLE_GATT_DB_PEER_INITIATED
        };

        gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);

        if(gattErr == CY_BLE_GATT_ERR_NONE)
        {
            wrReqParam.connHandle = eventParam->connHandle;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;

            if(Cy_BLE_ANS_ApplCallback != NULL)
            {
                Cy_BLE_ANS_ApplCallback(event, &wrReqParam);
            }
        }
    }

    if(CY_BLE_GATT_ERR_NONE != gattErr)
    {
        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }

    return(gattErr);
}


/******************************************************************************
* Function Name: Cy_BLE_ANSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets a characteristic value of Alert Notification Service, which is a value
*  identified by charIndex, to the local database.
*
*  \param charIndex: The index of the service characteristic of type
*              cy_en_ble_ans_char_index_t. The valid values are,
*              * CY_BLE_ANS_SUPPORTED_NEW_ALERT_CAT
*              * CY_BLE_ANS_SUPPORTED_UNREAD_ALERT_CAT
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to characteristic value data that should be stored
*               in the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request is handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANSS_SetCharacteristicValue(cy_en_ble_ans_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if(charIndex < CY_BLE_ANS_CHAR_COUNT)
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_ansConfigPtr->anss->charInfo[charIndex].charHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .offset                     = 0u,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };

        /* Store data in database */
        if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_SUCCESS;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ANSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets a characteristic value of Alert Notification Service. The value is
*  identified by charIndex.
*
*  \param charIndex: The index of the service characteristic of type cy_en_ble_ans_char_index_t.
*              The valid values are,
*              * CY_BLE_ANS_NEW_ALERT
*              * CY_BLE_ANS_UNREAD_ALERT_STATUS
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the location where characteristic value data
*               should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request is handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANSS_GetCharacteristicValue(cy_en_ble_ans_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if(charIndex < CY_BLE_ANS_CHAR_COUNT)
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_ansConfigPtr->anss->charInfo[charIndex].charHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };

        /* Read characteristic value from database */
        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_SUCCESS;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ANSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets a characteristic descriptor of the specified characteristic of Alert
*  Notification Service.
*
*  \param connHandle: The connection handle
*  \param charIndex:  The index of the service characteristic of type cy_en_ble_ans_char_index_t.
*              The valid values are,
*              * CY_BLE_ANS_NEW_ALERT
*              * CY_BLE_ANS_UNREAD_ALERT_STATUS
*  \param descrIndex: The index of the service characteristic descriptor of type
*              cy_en_ble_ans_descr_index_t. The valid value is,
*              * CY_BLE_ANS_CCCD
*  \param attrSize:   The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the location where characteristic descriptor value
*              data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request is handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ans_char_index_t charIndex,
                                                               cy_en_ble_ans_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if(((charIndex == CY_BLE_ANS_NEW_ALERT) || (charIndex == CY_BLE_ANS_UNREAD_ALERT_STATUS)) &&
       (descrIndex == CY_BLE_ANS_CCCD))
    {
        if(cy_ble_ansConfigPtr->anss->charInfo[charIndex].descrHandle[descrIndex] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_ansConfigPtr->anss->charInfo[charIndex].descrHandle[descrIndex],
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .connHandle                 = connHandle,
                .offset                     = 0u,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };

            if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_SUCCESS;
            }
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ANSS_SendNotification
***************************************************************************//**
*
*  Sends a notification with the characteristic value, as specified by its charIndex,
*  to the Client device.
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_ANSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic of type cy_en_ble_ans_char_index_t.
*              The valid values are,
*              * CY_BLE_ANS_UNREAD_ALERT_STATUS
*              * CY_BLE_ANS_NEW_ALERT
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be sent
*               to the Client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The function completed successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of input parameter is failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this.
*                                      characteristic.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the client.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_ans_char_index_t charIndex,
                                                    uint8_t attrSize,
                                                    uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex == CY_BLE_ANS_NEW_ALERT) || (charIndex == CY_BLE_ANS_UNREAD_ALERT_STATUS))
    {
        /* Send Notification if it is enabled and connected */
        if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
        {
            apiResult = CY_BLE_ERROR_INVALID_STATE;
        }
        else if(!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_ansConfigPtr->anss->charInfo[charIndex].
                                                 descrHandle[CY_BLE_ANS_CCCD]))
        {
            apiResult = CY_BLE_ERROR_NTF_DISABLED;
        }
        else
        {
            cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
            {
                /* Fill all fields of write request structure ... */
                .handleValPair.attrHandle = cy_ble_ansConfigPtr->anss->charInfo[charIndex].charHandle,
                .handleValPair.value.val  = attrValue,
                .handleValPair.value.len  = attrSize,
                .connHandle               = connHandle
            };
            /* Send notification to client using previously filled structure */
            apiResult = Cy_BLE_GATTS_Notification(&ntfReqParam);
        }
    }

    return(apiResult);
}

#endif /* CY_BLE_ANS_SERVER */


#ifdef CY_BLE_ANS_CLIENT

/******************************************************************************
* Function Name: Cy_BLE_ANSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_ANSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* ANS characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_anscCharUuid[CY_BLE_ANS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_SPRTD_NEW_ALRT_CTGRY,
        CY_BLE_UUID_CHAR_NEW_ALERT,
        CY_BLE_UUID_CHAR_SPRT_UNRD_ALRT_CTGRY,
        CY_BLE_UUID_CHAR_UNREAD_ALRT_STATUS,
        CY_BLE_UUID_CHAR_ALERT_NTF_CONTROL_POINT
    };

    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_ANS))
    {
        /* Update last characteristic endHandle to declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        for(i = (uint32_t)CY_BLE_ANS_SUPPORTED_NEW_ALERT_CAT; i < (uint32_t)CY_BLE_ANS_CHAR_COUNT; i++)
        {
            if(cy_ble_anscCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_ansc[discIdx].characteristics[i].charInfo.valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_ansc[discIdx].characteristics[i].charInfo.valueHandle = discCharInfo->valueHandle;
                    cy_ble_ansc[discIdx].characteristics[i].charInfo.properties = discCharInfo->properties;
                    lastEndHandle[discIdx] = &cy_ble_ansc[discIdx].characteristics[i].endHandle;
                }
                else
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo);
                }
            }
        }

        /* Initialize the characteristic endHandle to the Service endHandle. The characteristic endHandle will
         * be updated to the declaration handle of the following characteristic, in the following characteristic
         * discovery procedure. */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ANS_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_FIND_INFO_RSP event.
*  This event is generated when a server successfully sends the data for
*  CY_BLE_EVT_GATTC_FIND_INFO_REQ. Based on the service UUID, an appropriate data
*  structure is populated to the service with a service callback.
*
*  \param cy_stc_ble_disc_descr_info_t * discCharInfo: The pointer to a characteristic
*                                           information structure.
*
******************************************************************************/
static void Cy_BLE_ANSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t * discDescrInfo)
{
    uint32_t notSupportedDescr = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t descIdx;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_ANS)
    {
        switch(discDescrInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_CLIENT_CONFIG:
                descIdx = (uint32_t)CY_BLE_ANS_CCCD;
                break;

            default:
                /* Not supported descriptor */
                notSupportedDescr = 1u;
                break;
        }

        if(notSupportedDescr == 0u)
        {
            if(cy_ble_ansc[discIdx].characteristics[cy_ble_discovery[discIdx].charCount].descriptors[descIdx] ==
               CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_ansc[discIdx].characteristics[cy_ble_discovery[discIdx].charCount].descriptors[descIdx] =
                    discDescrInfo->descrHandle;
            }
            else
            {
                /* Duplication of the descriptor */
                Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_DESCR_DUPLICATION, &discDescrInfo);
            }
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ANSC_GetCharRange
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
static void Cy_BLE_ANSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_ANS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_ANS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if((cy_ble_ansc[discIdx].characteristics[charIdx].endHandle -
                cy_ble_ansc[discIdx].characteristics[charIdx].charInfo.valueHandle) != 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_ansc[discIdx].characteristics[charIdx].charInfo.valueHandle + 1u;
                charRangeInfo->range.endHandle = cy_ble_ansc[discIdx].characteristics[charIdx].endHandle;
                exitFlag = 1u;
            }
            else
            {
                cy_ble_discovery[discIdx].charCount++;
            }
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ANSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles Notification Event for Alert Notification Service.
*
*  \param cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam: The pointer to a
*                                                     cy_stc_ble_gattc_handle_value_ntf_param_t
*                                                     data structure specified by
*                                                     the event.
*
******************************************************************************/
static void Cy_BLE_ANSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_ANS_ApplCallback != NULL))
    {
        if((cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_NEW_ALERT].charInfo.valueHandle ==
            eventParam->handleValPair.attrHandle) ||
           (cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_UNREAD_ALERT_STATUS].charInfo.valueHandle ==
            eventParam->handleValPair.attrHandle))
        {
            cy_stc_ble_ans_char_value_t ntfParam =
            {
                .charIndex  = CY_BLE_ANS_NEW_ALERT,
                .connHandle = eventParam->connHandle,
                .value      = &eventParam->handleValPair.value
            };

            Cy_BLE_ANS_ApplCallback((uint32_t)CY_BLE_EVT_ANSC_NOTIFICATION, (void*)&ntfParam);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ANSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles Read Response Event for Alert Notification Service.
*
*  \param cy_stc_ble_gattc_read_rsp_param_t *eventParam: The pointer to the data that came
*                                            with a read response for ANS.
*
******************************************************************************/
static void Cy_BLE_ANSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t fFlag = 1u;
    uint32_t attrVal = 0u;
    cy_en_ble_ans_char_index_t idx;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_ANS_ApplCallback != NULL) &&
       (cy_ble_anscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_SUPPORTED_NEW_ALERT_CAT].charInfo.valueHandle ==
           cy_ble_anscReqHandle[discIdx])
        {
            idx = CY_BLE_ANS_SUPPORTED_NEW_ALERT_CAT;
        }
        else if(cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_SUPPORTED_UNREAD_ALERT_CAT].charInfo.valueHandle ==
                cy_ble_anscReqHandle[discIdx])
        {
            idx = CY_BLE_ANS_SUPPORTED_UNREAD_ALERT_CAT;
        }
        else if(cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_NEW_ALERT].descriptors[CY_BLE_ANS_CCCD] ==
                cy_ble_anscReqHandle[discIdx])
        {
            /* Attribute is characteristic descriptor */
            attrVal = 1u;
            idx = CY_BLE_ANS_NEW_ALERT;
        }
        else if(cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_UNREAD_ALERT_STATUS].descriptors[CY_BLE_ANS_CCCD] ==
                cy_ble_anscReqHandle[discIdx])
        {
            /* Attribute is characteristic descriptor */
            attrVal = 1u;
            idx = CY_BLE_ANS_UNREAD_ALERT_STATUS;
        }
        else
        {
            /* No ANS characteristic was requested for read */
            fFlag = 0u;
        }

        if(fFlag != 0u)
        {
            /* Read response for characteristic */
            if(attrVal == 0u)
            {
                cy_stc_ble_ans_char_value_t rdRspParam =
                {
                    /* Fill Alert Notification Service read response parameter structure with characteristic info. */
                    .connHandle = eventParam->connHandle,
                    .charIndex  = idx,
                    .value      = &eventParam->value
                };
                rdRspParam.charIndex = idx;
                Cy_BLE_ANS_ApplCallback((uint32_t)CY_BLE_EVT_ANSC_READ_CHAR_RESPONSE, (void*)&rdRspParam);
            }
            else /* Read response for characteristic descriptor */
            {
                cy_stc_ble_ans_descr_value_t rdRspParam =
                {
                    /* Fill Alert Notification Service read response parameter structure with characteristic descriptor
                     * info. */
                    .connHandle = eventParam->connHandle,
                    .charIndex  = idx,
                    .descrIndex = CY_BLE_ANS_CCCD,
                    .value      = &eventParam->value
                };
                rdRspParam.charIndex = idx;
                Cy_BLE_ANS_ApplCallback((uint32_t)CY_BLE_EVT_ANSC_READ_DESCR_RESPONSE, (void*)&rdRspParam);
            }

            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            cy_ble_anscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ANSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles Write Response Event for Alert Notification Service.
*
*  \param cy_stc_ble_conn_handle_t *eventParam: The pointer to the cy_stc_ble_conn_handle_t data
*                                   structure.
*
******************************************************************************/
static void Cy_BLE_ANSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t fFlag = 1u;
    uint32_t attrVal = 0u;
    cy_en_ble_ans_char_index_t idx;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_ANS_ApplCallback != NULL) &&
       (cy_ble_anscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        cy_stc_ble_ans_char_value_t wrRspParam = { .connHandle = *eventParam };

        if(cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_ALERT_NTF_CONTROL_POINT].charInfo.valueHandle ==
           cy_ble_anscReqHandle[discIdx])
        {
            idx = CY_BLE_ANS_ALERT_NTF_CONTROL_POINT;
        }
        else if(cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_NEW_ALERT].descriptors[CY_BLE_ANS_CCCD] ==
                cy_ble_anscReqHandle[discIdx])
        {
            /* Attribute is Characteristic Descriptor */
            attrVal = 1u;
            idx = CY_BLE_ANS_NEW_ALERT;
        }
        else if(cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_UNREAD_ALERT_STATUS].descriptors[CY_BLE_ANS_CCCD] ==
                cy_ble_anscReqHandle[discIdx])
        {
            /* Attribute is Characteristic Descriptor */
            attrVal = 1u;
            idx = CY_BLE_ANS_UNREAD_ALERT_STATUS;
        }
        else
        {
            /* No ANS characteristic was requested for write */
            fFlag = 0u;
        }

        if(fFlag != 0u)
        {
            /* Write response for characteristic */
            if(attrVal == 0u)
            {
                wrRspParam.charIndex = idx;
                wrRspParam.value = NULL;

                Cy_BLE_ANS_ApplCallback((uint32_t)CY_BLE_EVT_ANSC_WRITE_CHAR_RESPONSE, (void*)&wrRspParam);
            }
            else /* Write response for characteristic descriptor */
            {
                /* Fill Alert Notification Service write response parameter structure with
                 * characteristic descriptor info. */
                cy_stc_ble_ans_descr_value_t wrDescRspParam =
                {
                    .connHandle = *eventParam,
                    .charIndex  = idx,
                    .descrIndex = CY_BLE_ANS_CCCD,
                    .value      = NULL
                };

                Cy_BLE_ANS_ApplCallback((uint32_t)CY_BLE_EVT_ANSC_WRITE_DESCR_RESPONSE, (void*)&wrDescRspParam);
            }

            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            cy_ble_anscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ANSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles Error Response Event for Alert Notification Service.
*
*  \param cy_stc_ble_gatt_err_param_t *eventParam: The pointer to
*                                           the cy_stc_ble_gatt_err_param_t
*                                           structure.
*
* \return
*  None
*
******************************************************************************/
static void Cy_BLE_ANSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        if(cy_ble_anscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] == eventParam->errInfo.attrHandle)
        {
            cy_ble_anscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ANSC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_ANSS_WRITE_CHAR events is generated.
*  On successful request execution on the Server side the Write Response is
*  sent to the Client.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   Size of the Characteristic value attribute.
*  \param attrValue: Pointer to the characteristic value data that should be
*              sent to the server device.
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request was sent successfully.
*   * CY_BLE_ERROR_INVALID_STATE - The component in in invalid state for current
*                                  operation.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the ANS service-specific callback is registered
*      (with Cy_BLE_ANS_RegisterAttrCallback):
*  * CY_BLE_EVT_ANSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_ans_char_value_t.
*  .
*   Otherwise (if the ANS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_ans_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) &&
            (attrValue != NULL) && (charIndex == CY_BLE_ANS_ALERT_NTF_CONTROL_POINT) &&
            (cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_ALERT_NTF_CONTROL_POINT].charInfo.valueHandle !=
             CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        cy_stc_ble_gattc_write_cmd_req_t writeReqParam =
        {
            /* Fill all fields of write command request structure ... */
            .connHandle               = connHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .handleValPair.attrHandle = cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_ALERT_NTF_CONTROL_POINT].
                                         charInfo.valueHandle
        };

        /* Send request to write characteristic value */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);

        /* Save handle to support service-specific write response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_anscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }
    else
    {
        /* Validation of the input parameters failed */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ANSC_GetCharacteristicValue
***************************************************************************//**
*
*  Sends a request to the peer device to get a characteristic value, as
*  identified by its charIndex.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request was sent successfully;
*   * CY_BLE_ERROR_INVALID_STATE - The component in in invalid state for current
*                                  operation.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the ANS service-specific callback is registered
*      (with Cy_BLE_ANS_RegisterAttrCallback):
*  * CY_BLE_EVT_ANSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_ans_char_value_t.
*  .
*   Otherwise (if the ANS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP - in case if the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with event parameters
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_ans_char_index_t charIndex)
{
    cy_stc_ble_gattc_read_req_t readReqParam;
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(discIdx >= CY_BLE_CONFIG_GATTC_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Select characteristic */
        switch(charIndex)
        {
            case CY_BLE_ANS_SUPPORTED_NEW_ALERT_CAT:
                readReqParam.attrHandle = cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_SUPPORTED_NEW_ALERT_CAT].
                                           charInfo.valueHandle;
                break;

            case CY_BLE_ANS_SUPPORTED_UNREAD_ALERT_CAT:
                readReqParam.attrHandle = cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_SUPPORTED_UNREAD_ALERT_CAT].
                                           charInfo.valueHandle;
                break;

            default:
                /* Characteristic wasn't found */
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                break;
        }

        if(apiResult == CY_BLE_SUCCESS)
        {
            readReqParam.connHandle = connHandle;

            /* Send request to read characteristic value */
            apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);

            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_anscReqHandle[discIdx] = readReqParam.attrHandle;
            }
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ANSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sends a request to the peer device to set the characteristic descriptor of the
*  specified characteristic of Alert Notification Service.
*
*  Internally, Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server side the following events can be
*  generated:
*  * CY_BLE_EVT_ANSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_ANSS_NOTIFICATION_DISABLED
*
*  \param connHandle: The BLE peer device connection handle.
*  \param charIndex: The index of the ANS characteristic.
*  \param descrIndex: The index of the ANS characteristic descriptor.
*  \param attrSize: The size of the characteristic descriptor attribute.
*  \param attrValue: Pointer to the characteristic descriptor value data that should be
*              sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The  request was sent successfully.
*   * CY_BLE_ERROR_INVALID_STATE - The component in in invalid state for current
*                                  operation.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the ANS service-specific callback is registered
*      (with Cy_BLE_ANS_RegisterAttrCallback):
*  * CY_BLE_EVT_ANSC_WRITE_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_ans_descr_value_t.
*  .
*   Otherwise (if the ANS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ans_char_index_t charIndex,
                                                               cy_en_ble_ans_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (attrValue != NULL) && 
            (descrIndex == CY_BLE_ANS_CCCD) && (attrSize == CY_BLE_CCCD_LEN))
    {
        if((charIndex == CY_BLE_ANS_NEW_ALERT) || (charIndex == CY_BLE_ANS_UNREAD_ALERT_STATUS))
        {
            /* Fill all fields of write request structure ... */
            cy_stc_ble_gattc_write_req_t writeReqParam =
            {
                .handleValPair.value.val = attrValue,
                .handleValPair.value.len = attrSize,
                .connHandle              = connHandle
            };
            if(charIndex == CY_BLE_ANS_NEW_ALERT)
            {
                writeReqParam.handleValPair.attrHandle = cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_NEW_ALERT].
                                                          descriptors[CY_BLE_ANS_CCCD];
            }
            else
            {
                writeReqParam.handleValPair.attrHandle =
                    cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_UNREAD_ALERT_STATUS].descriptors[CY_BLE_ANS_CCCD];
            }
            /* ... and send request to server device. */
            apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_anscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
            }
        }
    }
    else
    {
        /* Characteristic has not been discovered or has invalid fields */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ANSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Sends a request to the peer device to get the characteristic descriptor of the
*  specified characteristic of Alert Notification Service.
*
*  \param connHandle: BLE peer device connection handle.
*  \param charIndex: The index of the Service Characteristic.
*  \param descrIndex: The index of the Service Characteristic Descriptor.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - A request was sent successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*   * CY_BLE_ERROR_INVALID_STATE - The component is in invalid state for current
*                                 operation.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Cannot process a request to send PDU due
*                                     to invalid operation performed by the
*                                     application.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the ANS service-specific callback is registered
*      (with Cy_BLE_ANS_RegisterAttrCallback):
*  * CY_BLE_EVT_ANSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_ans_descr_value_t.
*  .
*  Otherwise (if the ANS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP - in case if the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with event parameters
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ans_char_index_t charIndex,
                                                               uint8_t descrIndex)
{
    cy_stc_ble_gattc_read_req_t readReqParam;
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) &&
            ((charIndex == CY_BLE_ANS_NEW_ALERT) || (charIndex == CY_BLE_ANS_UNREAD_ALERT_STATUS)) &&
            (descrIndex == (uint8_t)CY_BLE_ANS_CCCD))
    {
        if(charIndex == CY_BLE_ANS_NEW_ALERT)
        {
            readReqParam.attrHandle = cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_NEW_ALERT].
                                       descriptors[CY_BLE_ANS_CCCD];
        }
        else
        {
            readReqParam.attrHandle = cy_ble_ansc[discIdx].characteristics[CY_BLE_ANS_UNREAD_ALERT_STATUS].
                                       descriptors[CY_BLE_ANS_CCCD];
        }

        readReqParam.connHandle = connHandle;
        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_anscReqHandle[discIdx] = readReqParam.attrHandle;
        }
    }
    else
    {
        /* Characteristic has not been discovered or had invalid fields */
    }

    return(apiResult);
}

#endif /* CY_BLE_ANS_CLIENT */
/******************************************************************************
* Function Name: Cy_BLE_ANS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Alert Notification Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_ANS_EventHandler(uint32_t eventCode,
                                                         void *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;

    if(eventCode > (uint32_t)CY_BLE_EVT_MAX)
    {
        /**************************************************************************
         * Handling Service Specific Events
         ***************************************************************************/
        switch((cy_en_ble_evt_t)eventCode)
        {
        #ifdef CY_BLE_ANS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_ANSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_ANSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_ANSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_ANCS_CLIENT */

            default:
                break;
        }
    }
    else
    {
        switch((cy_en_ble_event_t)eventCode)
        {
            /**************************************************************************
             * Handling GATT Server Events
             ***************************************************************************/
        #ifdef CY_BLE_ANS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_ANSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;
        #endif /* CY_BLE_ANS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_ANS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_ANSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_ANSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;


            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_ANSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_ANSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_HRS_CLIENT */
            default:
                break;
        }
    }

    return(gattErr);
}
#endif /* CY_BLE_ANS */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
