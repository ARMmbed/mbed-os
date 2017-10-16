/***************************************************************************//**
* \file cy_ble_scps.c
* \version 2.0
*
* \brief
*  Contains the source code for the Scan Parameter service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_SCPS))

static cy_ble_callback_t Cy_BLE_SCPS_ApplCallback = NULL;

#ifdef CY_BLE_SCPS_CLIENT
/* SCPS Center Service characteristics GATT DB handles structure */
cy_stc_ble_scpsc_t cy_ble_scpsc[CY_BLE_CONFIG_GATTC_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_scpscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];

#endif /* CY_BLE_SCPS_SERVER */

/* The pointer on global BLE SCPS Config structure */
cy_stc_ble_scps_config_t *cy_ble_scpsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_SCPS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_SCPSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* CY_BLE_SCPS_SERVER */

#ifdef CY_BLE_SCPS_CLIENT
static void Cy_BLE_SCPSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_SCPSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_SCPSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_SCPSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_SCPSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_SCPSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_SCPSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* (CY_BLE_SCPS_CLIENT) */
static cy_en_ble_gatt_err_code_t Cy_BLE_SCPS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_SCPS_Init
***************************************************************************//**
*
*  This function initializes the SCPS Service.
*
*  \param config: Configuration structure for the SCPS Service.
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
cy_en_ble_api_result_t Cy_BLE_SCPS_Init(cy_stc_ble_scps_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_scpsConfigPtr = config;

        /* Registers Event Handler for the SCPS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_SCPS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_scpsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_SCPS_ApplCallback = cy_ble_scpsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_SCPS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_SCPS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_scpsc[idx], 0, sizeof(cy_stc_ble_scpsc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_SCPS].uuid = CY_BLE_UUID_SCAN_PARAM_SERVICE;
                }
                cy_ble_scpscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_SCPS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_SCPS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc:  An application layer event callback function to receive
*                     events from the BLE Component. The definition of
*                     cy_ble_callback_t for ScPS is: \n
*                     typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                       void *eventParam)
*                     * eventCode indicates the event that triggered this
*                       callback.
*                     * eventParam contains the parameters corresponding to the
*                       current event.
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
cy_en_ble_api_result_t Cy_BLE_SCPS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_SCPS_ApplCallback = callbackFunc;
    if(cy_ble_scpsConfigPtr != NULL)
    {
        cy_ble_scpsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_SCPS_SERVER


/******************************************************************************
* Function Name: Cy_BLE_SCPSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets a characteristic value of the Scan Parameters service, which is
*  identified by charIndex.
*
*  \param charIndex: The index of the service characteristic.
*              * CY_BLE_SCPS_SCAN_INT_WIN - The Scan Interval Window characteristic index
*              * CY_BLE_SCPS_SCAN_REFRESH - The Scan Refresh characteristic index
*
*  \param attrSize: The size of the characteristic value attribute.
*
*  \param attrValue: The pointer to the characteristic value data that should be
*               stored to the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is absent
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SCPSS_SetCharacteristicValue(cy_en_ble_scps_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    cy_ble_gatt_db_attr_handle_t charValueHandle;
    uint16_t scanInterval;
    uint16_t locScanWindow;

    if(charIndex >= CY_BLE_SCPS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        if(charIndex == CY_BLE_SCPS_SCAN_INT_WIN)
        {
            scanInterval = Cy_BLE_Get16ByPtr(attrValue);
            locScanWindow = Cy_BLE_Get16ByPtr(attrValue + sizeof(scanInterval));
            if((scanInterval < CY_BLE_SCAN_INTERVAL_WINDOW_MIN) || (scanInterval > CY_BLE_SCAN_INTERVAL_WINDOW_MAX) ||
               (locScanWindow < CY_BLE_SCAN_INTERVAL_WINDOW_MIN) || (locScanWindow > scanInterval) ||
               (attrSize > CY_BLE_INTERVAL_WINDOW_CHAR_LEN))
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
            else
            {
                charValueHandle = cy_ble_scpsConfigPtr->scpss->intervalWindowCharHandle;
            }
        }
        else    /* Scan Refresh characteristic */
        {
            if(attrSize > CY_BLE_REFRESH_CHAR_LEN)
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
            else
            {
                charValueHandle = cy_ble_scpsConfigPtr->scpss->refreshCharHandle;
            }
        }
    }
    if(apiResult == CY_BLE_SUCCESS)
    {
        if(charValueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
        else
        {
            /* Store data in database */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = charValueHandle,
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .offset                     = 0u,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };

            if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
        }
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_SCPSS_GetCharacteristicValue
****************************************************************************//**
*
*  Gets a characteristic value of the Scan Parameters service, which is identified
*  by charIndex.
*
*  \param charIndex: The index of the service characteristic.
*             * CY_BLE_SCPS_SCAN_INT_WIN - The Scan Interval Window characteristic index
*             * CY_BLE_SCPS_SCAN_REFRESH - The Scan Refresh characteristic index
*
*  \param attrSize: The size of the characteristic value attribute.
*
*  \param attrValue: The pointer to the location where characteristic value data
*               should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SCPSS_GetCharacteristicValue(cy_en_ble_scps_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    cy_ble_gatt_db_attr_handle_t charValueHandle;

    if(charIndex >= CY_BLE_SCPS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        if(charIndex == CY_BLE_SCPS_SCAN_INT_WIN)
        {
            if(attrSize > CY_BLE_INTERVAL_WINDOW_CHAR_LEN)
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
            else
            {
                charValueHandle = cy_ble_scpsConfigPtr->scpss->intervalWindowCharHandle;
            }
        }
        else    /* Scan Refresh characteristic */
        {
            if(attrSize > CY_BLE_REFRESH_CHAR_LEN)
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
            else
            {
                charValueHandle = cy_ble_scpsConfigPtr->scpss->refreshCharHandle;
            }
        }
    }
    if(apiResult == CY_BLE_SUCCESS)
    {
        if(charValueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
        else
        {
            /* Read characteristic value from database */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = charValueHandle,
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };

            if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
        }
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_SCPSS_GetCharacteristicDescriptor
****************************************************************************//**
*
*  Gets a characteristic descriptor of the specified characteristic of the
*  Scan Parameters service.
*
*  \param connHandle: The connection handle
*  \param charIndex: The index of the characteristic.
*             * CY_BLE_SCPS_SCAN_REFRESH - The Scan Refresh characteristic index
*
*  \param descrIndex: The index of the descriptor.
*             * CY_BLE_SCPS_SCAN_REFRESH_CCCD - The Client Characteristic
*                Configuration descriptor index of the Scan Refresh characteristic
*
*  \param attrSize: The size of the characteristic value attribute.
*
*  \param attrValue: The pointer to the location where the characteristic descriptor
*               value data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional descriptor is absent
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SCPSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_scps_char_index_t charIndex,
                                                                cy_en_ble_scps_descr_index_t descrIndex,
                                                                uint8_t attrSize,
                                                                uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if((charIndex != CY_BLE_SCPS_SCAN_REFRESH) || (descrIndex >= CY_BLE_SCPS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        if(cy_ble_scpsConfigPtr->scpss->refreshCccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
        else
        {
            /* Get data from database */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_scpsConfigPtr->scpss->refreshCccdHandle,
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .connHandle                 = connHandle,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };

            if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
        }
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_SCPSS_WriteEventHandler
****************************************************************************//**
*
*  Handles the Write Request Event for the service.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*   * CY_BLE_GATT_ERR_NONE - Write request handled successfully.
*   * CY_BLE_GATT_ERR_UNLIKELY_ERROR - Internal error while writing attribute value
*
*******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_SCPSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    cy_stc_ble_scps_char_value_t locChar = { .connHandle = eventParam->connHandle };

    if(Cy_BLE_SCPS_ApplCallback != NULL)
    {
        if((eventParam->handleValPair.attrHandle == cy_ble_scpsConfigPtr->scpss->refreshCccdHandle) ||
           (eventParam->handleValPair.attrHandle == cy_ble_scpsConfigPtr->scpss->intervalWindowCharHandle))
        {
            /* Store value to database */
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
                /* Client Characteristic Configuration descriptor write request */
                if(eventParam->handleValPair.attrHandle == cy_ble_scpsConfigPtr->scpss->refreshCccdHandle)
                {
                    uint32_t eventCode;
                    locChar.charIndex = CY_BLE_SCPS_SCAN_REFRESH;
                    locChar.value = NULL;

                    if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                    {
                        eventCode = (uint32_t)CY_BLE_EVT_SCPSS_NOTIFICATION_ENABLED;
                    }
                    else
                    {
                        eventCode = (uint32_t)CY_BLE_EVT_SCPSS_NOTIFICATION_DISABLED;
                    }
                    Cy_BLE_SCPS_ApplCallback(eventCode, &locChar);

                #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
                    (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))

                    /* Set flag to store bonding data to flash */
                    if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
                    {
                        cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
                    }
                #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */
                }
                else /* Scan Interval Window characteristic write without response request */
                {
                    locChar.charIndex = CY_BLE_SCPS_SCAN_INT_WIN;
                    locChar.value = &eventParam->handleValPair.value;
                    Cy_BLE_SCPS_ApplCallback((uint32_t)CY_BLE_EVT_SCPSS_SCAN_INT_WIN_WRITE_CHAR, &locChar);
                }
            }
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }

    return(gattErr);
}


/*******************************************************************************
* Function Name: Cy_BLE_SCPSS_SendNotification
****************************************************************************//**
*
*  This function notifies the client that the server requires the Scan Interval
*  Window Characteristic to be written with the latest values upon notification.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_SCPSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The connection handle
*
*  \param charIndex: The index of the characteristic.
*              * CY_BLE_SCPS_SCAN_REFRESH - The Scan Refresh characteristic index
*
*  \param attrSize: The size of the characteristic value attribute.
*
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the Client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the client.
*
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SCPSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     cy_en_ble_scps_char_index_t charIndex,
                                                     uint8_t attrSize,
                                                     uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    if((charIndex != CY_BLE_SCPS_SCAN_REFRESH) || (attrSize != CY_BLE_REFRESH_CHAR_LEN))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Send Notification if it is enabled and connected */
        if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
        {
            apiResult = CY_BLE_ERROR_INVALID_STATE;
        }
        else if((cy_ble_scpsConfigPtr->scpss->refreshCccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE) ||
                (!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_scpsConfigPtr->scpss->refreshCccdHandle)))
        {
            apiResult = CY_BLE_ERROR_NTF_DISABLED;
        }
        else
        {
            /* Fill all fields of write request structure ... */
            cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
            {
                .handleValPair.attrHandle = cy_ble_scpsConfigPtr->scpss->refreshCharHandle,
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

#endif /* CY_BLE_SCPS_SERVER */

#ifdef CY_BLE_SCPS_CLIENT


/*******************************************************************************
* Function Name: Cy_BLE_SCPSC_DiscoverCharacteristicsEventHandler
****************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP event.
*  Based on the service UUID, an appropriate data structure is populated using the
*  data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
*
*******************************************************************************/
static void Cy_BLE_SCPSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_SCPS))
    {
        switch(discCharInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_SCAN_REFRESH:
                Cy_BLE_CheckStoreCharHandle(cy_ble_scpsc[discIdx].refreshChar);
                break;

            case CY_BLE_UUID_CHAR_SCAN_WINDOW:
                Cy_BLE_CheckStoreCharHandle(cy_ble_scpsc[discIdx].intervalWindowChar);
                break;

            default:
                break;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_SCPSC_DiscoverCharDescriptorsEventHandler
****************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_FIND_INFO_RSP event.
*  This event is generated when the server successfully sends the data for
*  CY_BLE_EVT_GATTC_FIND_INFO_REQ. Based on the service UUID, an appropriate data
*  structure is populated to the service with a service callback.
*
*  \param discDescrInfo: The pointer to a descriptor information structure.
*
*
*******************************************************************************/
static void Cy_BLE_SCPSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_SCPS)
    {
        if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_CLIENT_CONFIG)
        {
            Cy_BLE_CheckStoreCharDescrHandle(cy_ble_scpsc[discIdx].refreshCccdHandle);
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_SCPSC_GetCharRange
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
static void Cy_BLE_SCPSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_SCPS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        if(cy_ble_discovery[discIdx].charCount == 0u)
        {
            /* One descriptor is available per characteristic */
            charRangeInfo->range.startHandle = cy_ble_scpsc[discIdx].refreshChar.valueHandle + 1u;
            charRangeInfo->range.endHandle = cy_ble_serverInfo[discIdx][CY_BLE_SRVI_SCPS].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_SCPSC_SetCharacteristicValue
****************************************************************************//**
*
*  Sets a characteristic value of the Scan Parameters Service, which is
*  identified by charIndex.
*
*  This function call can result in generation of the following events based on
*  the response from the server device:
*  * CY_BLE_EVT_GATTC_WRITE_RSP
*  * CY_BLE_EVT_GATTC_ERROR_RSP
*
*  The CY_BLE_EVT_SCPSS_SCAN_INT_WIN_WRITE_CHAR event is received by the peer
*  device on invoking this function.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue:  The pointer to the characteristic value data that should be
*                     sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                              the particular characteristic.
*  * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                    characteristic.
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SCPSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_scps_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if((charIndex != CY_BLE_SCPS_SCAN_INT_WIN) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        if(cy_ble_scpsc[discIdx].intervalWindowChar.valueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            cy_stc_ble_gattc_write_cmd_req_t writeCmdParam =
            {
                .handleValPair.attrHandle = cy_ble_scpsc[discIdx].intervalWindowChar.valueHandle,
                .handleValPair.value.val  = attrValue,
                .handleValPair.value.len  = attrSize,
                .connHandle               = connHandle
            };

            apiResult = Cy_BLE_GATTC_WriteWithoutResponse(&writeCmdParam);
        }
        else
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_SCPSC_SetCharacteristicDescriptor
****************************************************************************//**
*
*  Sets characteristic descriptor of specified characteristic of the Scan
*  Parameters Service.
*
*  Internally, Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server side the following events can be
*  generated:
*  * CY_BLE_EVT_SCPSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_SCPSS_NOTIFICATION_DISABLED
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param descrIndex:  The index of the service characteristic descriptor.
*  \param attrSize:   The size of the descriptor value attribute.
*  \param attrValue: The pointer to the characteristic descriptor value data that
*              should be sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the SCPS service-specific callback is registered
*      (with Cy_BLE_SCPS_RegisterAttrCallback):
*  * CY_BLE_EVT_SCPSC_WRITE_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_scps_descr_value_t.
*  .
*   Otherwise (if the SCPS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SCPSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_scps_char_index_t charIndex,
                                                                cy_en_ble_scps_descr_index_t descrIndex,
                                                                uint8_t attrSize,
                                                                uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex != CY_BLE_SCPS_SCAN_REFRESH) || (descrIndex >= CY_BLE_SCPS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        if(cy_ble_scpsc[discIdx].refreshChar.valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
        else
        {
            cy_stc_ble_gattc_write_req_t writeReqParam =
            {
                /* Fill all fields of write request structure ... */
                .handleValPair.attrHandle = cy_ble_scpsc[discIdx].refreshCccdHandle,
                .handleValPair.value.val  = attrValue,
                .handleValPair.value.len  = attrSize,
                .connHandle               = connHandle
            };

            /* ... and send request to server device. */
            apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

            /* Save handle to support service-specific read response from device */
            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_scpscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
            }
        }
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_SCPSC_GetCharacteristicDescriptor
****************************************************************************//**
*
*  Gets characteristic descriptor of specified characteristic of the Scan
*  Parameters Service.
*
*  This function call can result in generation of the following events based on
*  the response from the server device:
*  * CY_BLE_EVT_SCPSC_READ_DESCR_RESPONSE
*  * CY_BLE_EVT_GATTC_ERROR_RSP
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of a Service Characteristic.
*  \param descrIndex: The index of a Service Characteristic Descriptor.
*
* \return
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                              the particular descriptor
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                    the specified attribute
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the SCPS service-specific callback is registered
*      (with Cy_BLE_SCPS_RegisterAttrCallback):
*  * CY_BLE_EVT_SCPSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_scps_descr_value_t.
*  .
*  Otherwise (if the SCPS service-specific callback is not registered):
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
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SCPSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_scps_char_index_t charIndex,
                                                                cy_en_ble_scps_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex != CY_BLE_SCPS_SCAN_REFRESH) || (descrIndex >= CY_BLE_SCPS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        if(cy_ble_scpsc[discIdx].refreshChar.valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
        else
        {
            cy_stc_ble_gattc_read_req_t readReqParam =
            {
                .attrHandle = cy_ble_scpsc[discIdx].refreshCccdHandle,
                .connHandle = connHandle
            };

            apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);

            /* Save handle to support service-specific read response from device */
            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_scpscReqHandle[discIdx] = cy_ble_scpsc[discIdx].refreshCccdHandle;
            }
        }
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_SCPSC_NotificationEventHandler
****************************************************************************//**
*
*  Handles the Notification Event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
*******************************************************************************/
static void Cy_BLE_SCPSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_SCPS_ApplCallback != NULL))
    {
        if(cy_ble_scpsc[discIdx].refreshChar.valueHandle == eventParam->handleValPair.attrHandle)
        {
            cy_stc_ble_scps_char_value_t locCharValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_SCPS_SCAN_REFRESH,
                .value      = &eventParam->handleValPair.value
            };

            Cy_BLE_SCPS_ApplCallback((uint32_t)CY_BLE_EVT_SCPSC_NOTIFICATION, &locCharValue);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_SCPSC_ReadResponseEventHandler
****************************************************************************//**
*
*  Handles the Read Response Event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
*******************************************************************************/
static void Cy_BLE_SCPSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_SCPS_ApplCallback != NULL) &&
       (cy_ble_scpscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_scpsc[discIdx].refreshCccdHandle == cy_ble_scpscReqHandle[discIdx])
        {
            cy_stc_ble_scps_descr_value_t locDescrValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_SCPS_SCAN_REFRESH,
                .descrIndex = CY_BLE_SCPS_SCAN_REFRESH_CCCD,
                .value      = &eventParam->value
            };

            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            cy_ble_scpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_SCPS_ApplCallback((uint32_t)CY_BLE_EVT_SCPSC_READ_DESCR_RESPONSE, &locDescrValue);
        }
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_SCPSC_WriteResponseEventHandler
****************************************************************************//**
*
*  Handles the Write Response Event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
*******************************************************************************/
static void Cy_BLE_SCPSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_SCPS_ApplCallback != NULL) &&
       (cy_ble_scpscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_scpsc[discIdx].refreshCccdHandle == cy_ble_scpscReqHandle[discIdx])
        {
            cy_stc_ble_scps_descr_value_t locDescrValue =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_SCPS_SCAN_REFRESH,
                .descrIndex = CY_BLE_SCPS_SCAN_REFRESH_CCCD,
                .value      = NULL
            };

            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            cy_ble_scpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_SCPS_ApplCallback((uint32_t)CY_BLE_EVT_SCPSC_WRITE_DESCR_RESPONSE, &locDescrValue);
        }
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_SCPSC_ErrorResponseEventHandler
****************************************************************************//**
*
*  Handles the Error Response Event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
*******************************************************************************/
static void Cy_BLE_SCPSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        if(cy_ble_scpscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] == eventParam->errInfo.attrHandle)
        {
            cy_ble_scpscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}

#endif /* (CY_BLE_SCPS_CLIENT) */


/******************************************************************************
* Function Name: Cy_BLE_SCPS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Scan Parameter service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_SCPS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_SCPS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_SCPSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_SCPSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_SCPSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_SCPS_CLIENT */

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
        #ifdef CY_BLE_SCPS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_SCPSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_WRITE_CMD_REQ:
                (void)Cy_BLE_SCPSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;
        #endif /* CY_BLE_SCPS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_SCPS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_SCPSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_SCPSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_SCPSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_SCPSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_SCPS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_SCPS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
