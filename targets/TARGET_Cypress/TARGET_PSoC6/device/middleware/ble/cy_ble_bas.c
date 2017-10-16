/***************************************************************************//**
* \file cy_ble_bas.c
* \version 2.0
*
* \brief
*  Contains the source code for the Battery Service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_BAS))

static cy_ble_callback_t Cy_BLE_BAS_ApplCallback = NULL;

#ifdef CY_BLE_BAS_CLIENT
/* BAS Center Service characteristics GATT DB handles structure */
cy_stc_ble_basc_t cy_ble_basc[CY_BLE_CONFIG_GATTC_COUNT][CY_BLE_BASC_SERVICE_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_bascReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_BAS_CLIENT) */

/* The pointer to the global BLE BAS config structure */
cy_stc_ble_bas_config_t *cy_ble_basConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_BAS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_BASS_WriteEventHandler(
    const cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* CY_BLE_BAS_SERVER */

#ifdef CY_BLE_BAS_CLIENT
static void Cy_BLE_BASC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_BASC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_BASC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_BASC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_BASC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_BASC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_BASC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* (CY_BLE_BAS_CLIENT) */
static cy_en_ble_gatt_err_code_t Cy_BLE_BAS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_BAS_Init
***************************************************************************//**
*
*  This function initializes the BAS Service.
*
*  \param config: The configuration structure for the BAS Service.
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
cy_en_ble_api_result_t Cy_BLE_BAS_Init(cy_stc_ble_bas_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_basConfigPtr = config;

        /* Registers Event Handler for the BAS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_BAS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_basConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_BAS_ApplCallback = cy_ble_basConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_BAS_CLIENT
        {
            uint32_t idx;
            uint8_t locServIndex;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                for(locServIndex = 0u; locServIndex < (uint32_t)CY_BLE_BASC_SERVICE_COUNT; locServIndex++)
                {
                    if(cy_ble_serverInfo[idx][CY_BLE_SRVI_BAS + locServIndex].range.startHandle ==
                       CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                    {
                        (void)memset(&cy_ble_basc[idx][locServIndex], 0, sizeof(cy_stc_ble_basc_t));

                        /* Initialize uuid  */
                        cy_ble_serverInfo[idx][CY_BLE_SRVI_BAS + locServIndex].uuid = CY_BLE_UUID_BAS_SERVICE;
                    }
                }
                cy_ble_bascReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_BAS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BAS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific Write requests from the peer device will not be handled with
*  an unregistered callback function.
*
*  \param callbackFunc: An application layer event callback function to receive
*                    Battery Service events from the BLE Component. The
*                    definition of cy_ble_callback_t for Battery Service is,\n
*                    typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                         void *eventParam)
*                    * eventCode indicates the event that triggered this
*                       callback (e.g. CY_BLE_EVT_BASS_NOTIFICATION_ENABLED)
*                    * eventParam contains the parameters corresponding to the
*                       current event (e.g., the pointer to the cy_stc_ble_bas_char_value_t
*                       structure that contains details of the characteristic
*                       for which a notification enabled event was triggered)
*
* \sideeffect The *eventParams in the callback function should not be used
*                by the application when the callback function execution is
*                finished. Otherwise, this data may become corrupted.
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
cy_en_ble_api_result_t Cy_BLE_BAS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_BAS_ApplCallback = callbackFunc;
    if(cy_ble_basConfigPtr != NULL)
    {
        cy_ble_basConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_BAS_SERVER


/******************************************************************************
* Function Name: Cy_BLE_BASS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets a characteristic value of the service in the local database.
*
*  \param serviceIndex: The index of the service instance.
*  \param charIndex: The index of the service characteristic of type
*              cy_en_ble_bas_char_index_t.
*  \param attrSize: The size of the characteristic value attribute. A battery level
*             characteristic has a 1-byte length.
*  \param attrValue: The pointer to the characteristic value data that should be
*               stored to the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BASS_SetCharacteristicValue(uint8_t serviceIndex,
                                                          cy_en_ble_bas_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if((serviceIndex >= CY_BLE_BASS_SERVICE_COUNT) || (charIndex >= CY_BLE_BAS_CHAR_COUNT) ||
       (attrSize != CY_BLE_BAS_BATTERY_LEVEL_LEN) || (*attrValue > CY_BLE_BAS_MAX_BATTERY_LEVEL_VALUE))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Store data in the database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_basConfigPtr->bass[serviceIndex].batteryLevelHandle,
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

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BASS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets a characteristic value of the Battery Service, identified by charIndex.
*
*  \param serviceIndex: The index of the service instance. e.g. if two Battery Services
*                 are supported in your design, then the first service will be
*                 identified by serviceIndex of 0 and the second service - by serviceIndex
*                 of 1.
*  \param charIndex: The index of a service characteristic of type cy_en_ble_bas_char_index_t.
*  \param attrSize: The size of the characteristic value attribute. A battery level
*             characteristic has a 1-byte length.
*  \param attrValue: The pointer to the location where characteristic value data
*               should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BASS_GetCharacteristicValue(uint8_t serviceIndex,
                                                          cy_en_ble_bas_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if((serviceIndex >= CY_BLE_BASS_SERVICE_COUNT) || (charIndex >= CY_BLE_BAS_CHAR_COUNT) ||
       (attrSize != CY_BLE_BAS_BATTERY_LEVEL_LEN))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Read characteristic value from database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_basConfigPtr->bass[serviceIndex].batteryLevelHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };

        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BASS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets a characteristic descriptor of a specified characteristic of the
*   Battery Service from the local GATT database.
*
*  \param connHandle: The BLE peer device connection handle.
*  \param serviceIndex: The index of the service instance. e.g. If two Battery Services
*                 are supported in your design, then the first service will be
*                 identified by serviceIndex of 0 and the second service - by serviceIndex
*                 of 1.
*  \param charIndex: The index of a service characteristic of type
*              cy_en_ble_bas_char_index_t.
*  \param descrIndex: The index of a service characteristic descriptor of type
*               cy_en_ble_bas_descr_index_t.
*  \param attrSize: The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the location where the characteristic descriptor
*               value data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BASS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               uint8_t serviceIndex,
                                                               cy_en_ble_bas_char_index_t charIndex,
                                                               cy_en_ble_bas_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if((serviceIndex >= CY_BLE_BASS_SERVICE_COUNT) || (charIndex >= CY_BLE_BAS_CHAR_COUNT) ||
       (descrIndex >= CY_BLE_BAS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.value.len = attrSize,
            .handleValuePair.value.val = attrValue,
            .connHandle                = connHandle,
            .flags                     = CY_BLE_GATT_DB_LOCALLY_INITIATED,
        };

        /* Get data from the database */
        if(descrIndex == CY_BLE_BAS_BATTERY_LEVEL_CCCD)
        {
            dbAttrValInfo.handleValuePair.attrHandle = cy_ble_basConfigPtr->bass[serviceIndex].cccdHandle;
        }
        else
        {
            dbAttrValInfo.handleValuePair.attrHandle = cy_ble_basConfigPtr->bass[serviceIndex].cpfdHandle;
        }

        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BASS_WriteEventHandler
***************************************************************************//**
*
*  Handles a Write Request Event for the Battery Service.
*
*  \param void *eventParam: The pointer to the data structure specified by the event.
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*   * CY_BLE_GATT_ERR_NONE - A Write request is handled successfully.
*   * CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED - Notification isn't supported.
*   * CY_BLE_GATT_ERR_UNLIKELY_ERROR - An internal error while writing attribute
*                                      value.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_BASS_WriteEventHandler(const cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    uint8_t locServIndex = 0u;
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    cy_stc_ble_bas_char_value_t locCharIndex = { .connHandle = eventParam->connHandle };

    if(Cy_BLE_BAS_ApplCallback != NULL)
    {
        do
        {
            /* Client Characteristic Configuration descriptor Write request */
            if(eventParam->handleValPair.attrHandle == cy_ble_basConfigPtr->bass[locServIndex].cccdHandle)
            {
                /* Verify that the optional notification property is enabled for the Battery Level characteristic */
                if(CY_BLE_IS_NOTIFICATION_SUPPORTED(cy_ble_basConfigPtr->bass[locServIndex].batteryLevelHandle))
                {
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
                        locCharIndex.serviceIndex = locServIndex;
                        locCharIndex.charIndex = CY_BLE_BAS_BATTERY_LEVEL;
                        locCharIndex.value = NULL;

                        if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                        {
                            Cy_BLE_BAS_ApplCallback((uint32_t)CY_BLE_EVT_BASS_NOTIFICATION_ENABLED, &locCharIndex);
                        }
                        else
                        {
                            Cy_BLE_BAS_ApplCallback((uint32_t)CY_BLE_EVT_BASS_NOTIFICATION_DISABLED, &locCharIndex);
                        }
                    #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
                        (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))

                        /* Sets a flag to store bonding data to flash */
                        if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
                        {
                            cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
                        }
                    #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */
                    }
                }
                else
                {
                    gattErr = CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED;
                }
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;
            }
            locServIndex++;
        }
        while(locServIndex < CY_BLE_BASS_SERVICE_COUNT);
    }

    return(gattErr);
}


/******************************************************************************
* Function Name: Cy_BLE_BASS_SendNotification
***************************************************************************//**
*
*  This function updates the value of the Battery Level characteristic in the
*  GATT database. If the Client has configured a notification on the Battery
*  Level characteristic, the function additionally sends this value using a
*  GATT Notification message.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  Handle Value notification which results in CY_BLE_EVT_BASC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The BLE peer device connection handle
*  \param serviceIndex: The index of the service instance. e.g. If two Battery Services
*                 are supported in your design, then the first service will be
*                 identified by serviceIndex of 0 and the second service - by
*                 serviceIndex of 1.
*  \param charIndex: The index of a service characteristic of type
*              cy_en_ble_bas_char_index_t.
*  \param attrSize: The size of the characteristic value attribute. A battery level
*             characteristic has a 1-byte length.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the Client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the Client.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BASS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    uint8_t serviceIndex,
                                                    cy_en_ble_bas_char_index_t charIndex,
                                                    uint8_t attrSize,
                                                    uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Store new data in the database */
    apiResult = Cy_BLE_BASS_SetCharacteristicValue(serviceIndex, charIndex, attrSize, attrValue);

    if(apiResult == CY_BLE_SUCCESS)
    {
        /* Send notification if it is enabled and connected */
        if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
        {
            apiResult = CY_BLE_ERROR_INVALID_STATE;
        }
        else if((cy_ble_basConfigPtr->bass[serviceIndex].cccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE) ||
                (!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_basConfigPtr->bass[serviceIndex].cccdHandle)))
        {
            apiResult = CY_BLE_ERROR_NTF_DISABLED;
        }
        else
        {
            cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
            {
                /* Fill all fields of the Write request structure ... */
                .handleValPair.attrHandle = cy_ble_basConfigPtr->bass[serviceIndex].batteryLevelHandle,
                .handleValPair.value.val  = attrValue,
                .handleValPair.value.len  = attrSize,
                .connHandle               = connHandle
            };

            /* Send notification to the Client using a previously filled structure */
            apiResult = Cy_BLE_GATTS_Notification(&ntfReqParam);
        }
    }

    return(apiResult);
}

#endif /* CY_BLE_BAS_SERVER */

#ifdef CY_BLE_BAS_CLIENT


/******************************************************************************
* Function Name: Cy_BLE_BASC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, the appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_BASC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (discCharInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_BATTERY_LEVEL) &&
       (cy_ble_discovery[discIdx].servCount >= (uint32_t)CY_BLE_SRVI_BAS) &&
       (cy_ble_discovery[discIdx].servCount <= (uint32_t)CY_BLE_SRVI_BAS_END))
    {
        /* The index of the service instance */
        uint32_t basServIdx = cy_ble_discovery[discIdx].servCount - (uint32_t)CY_BLE_SRVI_BAS;

        /* Check and store characteristic handle */
        Cy_BLE_CheckStoreCharHandle(cy_ble_basc[discIdx][basServIdx].batteryLevel);

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BASC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_FIND_INFO_RSP event.
*  Based on the descriptor UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discDescrInfo: The pointer to a descriptor information structure.
*  \param discoveryService: The index of the service instance.
*
******************************************************************************/
static void Cy_BLE_BASC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t basServIdx = cy_ble_discovery[discIdx].servCount - (uint32_t)CY_BLE_SRVI_BAS;

    if((cy_ble_discovery[discIdx].servCount >= (uint32_t)CY_BLE_SRVI_BAS) &&
       (cy_ble_discovery[discIdx].servCount <= (uint32_t)CY_BLE_SRVI_BAS_END))
    {
        if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_CLIENT_CONFIG)
        {
            Cy_BLE_CheckStoreCharDescrHandle(cy_ble_basc[discIdx][basServIdx].cccdHandle);
        }
        else if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_FORMAT)
        {
            Cy_BLE_CheckStoreCharDescrHandle(cy_ble_basc[discIdx][basServIdx].cpfdHandle);
        }
        else if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_REPORT_REFERENCE)
        {
            Cy_BLE_CheckStoreCharDescrHandle(cy_ble_basc[discIdx][basServIdx].rrdHandle);
        }
        else    /* BAS doesn't support other descriptors */
        {
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BASC_GetCharRange
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
static void Cy_BLE_BASC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);

    if((cy_ble_discovery[discIdx].servCount >= (uint32_t)CY_BLE_SRVI_BAS) &&
       (cy_ble_discovery[discIdx].servCount <= (uint32_t)CY_BLE_SRVI_BAS_END))
    {
        /* The index of the service instance */
        uint32_t basServIdx = cy_ble_discovery[discIdx].servCount - (uint32_t)CY_BLE_SRVI_BAS;

        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        if((cy_ble_discovery[discIdx].charCount == 0u) &&
           (cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range.endHandle !=
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
        {
            charRangeInfo->range.startHandle = cy_ble_basc[discIdx][basServIdx].batteryLevel.valueHandle + 1u;
            charRangeInfo->range.endHandle = cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}

/******************************************************************************
* Function Name: Cy_BLE_BASC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read the characteristic value from the Server
*  identified by charIndex.
*
*  This function call can result in generation of the following events based on
*  a response from the Server device:
*  * CY_BLE_EVT_BASC_READ_CHAR_RESPONSE
*  * CY_BLE_EVT_GATTC_ERROR_RSP
*
*  \param connHandle: The BLE peer device connection handle.
*  \param serviceIndex: The index of the service instance. e.g. If two Battery Services are
*                 supported in your design, then the first service will be identified
*                 by serviceIndex of 0 and the second service - by serviceIndex of 1.
*  \param charIndex: The index of a service characteristic of type cy_en_ble_bas_char_index_t.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The Read request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                              the particular characteristic.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the Server is not established.
*  * CY_BLE_ERROR_INVALID_OPERATION - The operation is invalid for this
*                                    characteristic.
*
* \events
*  If execution is successful (return value = CY_BLE_SUCCESS)
*  these events can appear: \n
*   If the BAS service-specific callback is registered
*      (with Cy_BLE_BAS_RegisterAttrCallback):
*  * CY_BLE_EVT_BASC_READ_CHAR_RESPONSE - if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_bas_char_value_t.
*  .
*   Otherwise (if a BAS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP - if the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with the event parameters
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_ERROR_RSP - if there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with the event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BASC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          uint8_t serviceIndex,
                                                          cy_en_ble_bas_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((serviceIndex >= (uint32_t)CY_BLE_BASC_SERVICE_COUNT) || (charIndex > CY_BLE_BAS_BATTERY_LEVEL) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_basc[discIdx][serviceIndex].batteryLevel.valueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_basc[discIdx][serviceIndex].batteryLevel.valueHandle,
            .connHandle = connHandle
        };

        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);
        /* Save the handle to support service-specific Read response from the device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_bascReqHandle[discIdx] = cy_ble_basc[discIdx][serviceIndex].batteryLevel.valueHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BASC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sends a request to set a characteristic descriptor of the specified Battery Service
*  characteristic on the server device.
*
*  Internally, a Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server's side the following events can be
*  generated:
*  * CY_BLE_EVT_BASS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_BASS_NOTIFICATION_DISABLED
*
*  \param connHandle: The BLE peer device connection handle.
*  \param serviceIndex: The index of the service instance. e.g. If two Battery Services
*                       are supported in your design, then the first service will be
*                       identified by serviceIndex of 0 and the second service - by
*                       serviceIndex of 1.
*  \param charIndex: The index of a service characteristic of type
*                    cy_en_ble_bas_char_index_t.
*  \param descrIndex: The index of a service characteristic descriptor of type
*                     cy_en_ble_bas_descr_index_t.
*  \param attrSize: The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the characteristic descriptor value data that should
*                    be sent to the Server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute.
*
* \events
*  If execution is successful (return value = CY_BLE_SUCCESS),
*  these events can appear: \n
*   If a BAS service-specific callback is registered
*      (with Cy_BLE_BAS_RegisterAttrCallback):
*  * CY_BLE_EVT_BASC_WRITE_DESCR_RESPONSE - if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_bas_descr_value_t.
*  .
*   Otherwise (if a BAS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - if there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with the event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BASC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               uint8_t serviceIndex,
                                                               cy_en_ble_bas_char_index_t charIndex,
                                                               cy_en_ble_bas_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((serviceIndex >= (uint32_t)CY_BLE_BASC_SERVICE_COUNT) || (charIndex > CY_BLE_BAS_BATTERY_LEVEL) ||
            (descrIndex >= CY_BLE_BAS_DESCR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(descrIndex != CY_BLE_BAS_BATTERY_LEVEL_CCCD)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        /* Fill all the fields of the Write request structure ... */
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_basc[discIdx][serviceIndex].cccdHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };

        /* ... and send a request to the Server device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

        /* Save the handle to support service-specific Read response from the device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_bascReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BASC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Sends a request to get a characteristic descriptor of the specified Battery Service
*  characteristic from the Server device. This function call can result in
*  generation of the following events based on a response from the Server
*  device:
*  * CY_BLE_EVT_BASC_READ_DESCR_RESPONSE
*  * CY_BLE_EVT_GATTC_ERROR_RSP
*
*  \param connHandle: The BLE peer device connection handle.
*  \param serviceIndex: The index of the service instance. e.g. If two Battery Services are
*                 supported in your design, then the first service will be identified
*                 by serviceIndex of 0 and the second - by serviceIndex of 1.
*  \param charIndex: The index of a Battery Service characteristic of type
*              cy_en_ble_bas_char_index_t.
*  \param descrIndex: The index of a Battery Service characteristic descriptor of type
*               cy_en_ble_bas_descr_index_t.
*
* \return
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute.
*
* \events
*  If execution is successful (return value = CY_BLE_SUCCESS),
*  these events can appear: \n
*  If a BAS service-specific callback is registered
*      (with Cy_BLE_BAS_RegisterAttrCallback):
*  * CY_BLE_EVT_BASC_READ_DESCR_RESPONSE - if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with an event parameter structure
*                                of type cy_stc_ble_bas_descr_value_t.
*  .
*  Otherwise (if a BAS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP - if the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with the event parameters
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_ERROR_RSP - if there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with the event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BASC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               uint8_t serviceIndex,
                                                               cy_en_ble_bas_char_index_t charIndex,
                                                               cy_en_ble_bas_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    cy_stc_ble_gattc_read_req_t readReqParam = { .connHandle = connHandle };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((serviceIndex >= CY_BLE_BASC_SERVICE_COUNT) || (charIndex > CY_BLE_BAS_BATTERY_LEVEL) ||
            (descrIndex >= CY_BLE_BAS_DESCR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        if(descrIndex == CY_BLE_BAS_BATTERY_LEVEL_CCCD)
        {
            readReqParam.attrHandle = cy_ble_basc[discIdx][serviceIndex].cccdHandle;
        }
        else /* CY_BLE_BAS_BATTERY_LEVEL_CPFD */
        {
            readReqParam.attrHandle = cy_ble_basc[discIdx][serviceIndex].cpfdHandle;
        }

        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);

        /* Save the handle to support a service-specific Read response from the device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_bascReqHandle[discIdx] = readReqParam.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BASC_NotificationEventHandler
***************************************************************************//**
*
*  Handles a Notification event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_BASC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
    uint32_t i;

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_BAS_ApplCallback != NULL))
    {
        for(i = 0u; i < CY_BLE_BASC_SERVICE_COUNT; i++)
        {
            if(cy_ble_basc[discIdx][i].batteryLevel.valueHandle == eventParam->handleValPair.attrHandle)
            {
                cy_stc_ble_bas_char_value_t locCharValue =
                {
                    .serviceIndex = (uint8_t)i,
                    .connHandle   = eventParam->connHandle,
                    .charIndex    = CY_BLE_BAS_BATTERY_LEVEL,
                    .value        = &eventParam->handleValPair.value,
                };
                Cy_BLE_BAS_ApplCallback((uint32_t)CY_BLE_EVT_BASC_NOTIFICATION, &locCharValue);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BASC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles a Read Response event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_BASC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
    uint32_t locServIndex;
    uint32_t locReqHandle = 0u;

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_BAS_ApplCallback != NULL) &&
       (cy_ble_bascReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locServIndex = 0u; (locServIndex < CY_BLE_BASC_SERVICE_COUNT) && (locReqHandle == 0u); locServIndex++)
        {
            if(cy_ble_basc[discIdx][locServIndex].batteryLevel.valueHandle == cy_ble_bascReqHandle[discIdx])
            {
                cy_stc_ble_bas_char_value_t batteryLevelValue =
                {
                    .connHandle   = eventParam->connHandle,
                    .serviceIndex = locServIndex,
                    .charIndex    = CY_BLE_BAS_BATTERY_LEVEL,
                    .value        = &eventParam->value
                };

                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                cy_ble_bascReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_BAS_ApplCallback((uint32_t)CY_BLE_EVT_BASC_READ_CHAR_RESPONSE, &batteryLevelValue);
                locReqHandle = 1u;
            }
            else if((cy_ble_basc[discIdx][locServIndex].cccdHandle == cy_ble_bascReqHandle[discIdx]) ||
                    (cy_ble_basc[discIdx][locServIndex].cpfdHandle == cy_ble_bascReqHandle[discIdx]))
            {
                cy_stc_ble_bas_descr_value_t locDescrValue =
                {
                    .connHandle   = eventParam->connHandle,
                    .serviceIndex = locServIndex,
                    .charIndex    = CY_BLE_BAS_BATTERY_LEVEL,
                    .descrIndex   = ((cy_ble_basc[discIdx][locServIndex].cccdHandle == cy_ble_bascReqHandle[discIdx]) ?
                                     CY_BLE_BAS_BATTERY_LEVEL_CCCD : CY_BLE_BAS_BATTERY_LEVEL_CPFD),
                    .value        = &eventParam->value
                };

                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                cy_ble_bascReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_BAS_ApplCallback((uint32_t)CY_BLE_EVT_BASC_READ_DESCR_RESPONSE, &locDescrValue);
                locReqHandle = 1u;
            }
            else /* Unsupported event code */
            {
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BASC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Write Response event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_BASC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);
    uint32_t locServIndex;

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_BAS_ApplCallback != NULL) &&
       (cy_ble_bascReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locServIndex = 0u; locServIndex < (uint32_t)CY_BLE_BASC_SERVICE_COUNT; locServIndex++)
        {
            if(cy_ble_basc[discIdx][locServIndex].cccdHandle == cy_ble_bascReqHandle[discIdx])
            {
                cy_stc_ble_bas_descr_value_t locDescIndex =
                {
                    .connHandle   = *eventParam,
                    .serviceIndex = locServIndex,
                    .charIndex    = CY_BLE_BAS_BATTERY_LEVEL,
                    .descrIndex   = CY_BLE_BAS_BATTERY_LEVEL_CCCD,
                    .value        = NULL
                };

                cy_ble_bascReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                Cy_BLE_BAS_ApplCallback((uint32_t)CY_BLE_EVT_BASC_WRITE_DESCR_RESPONSE, &locDescIndex);
                break;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BASC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles an Error Response event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*  None
*
******************************************************************************/
static void Cy_BLE_BASC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        if(cy_ble_bascReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] == eventParam->errInfo.attrHandle)
        {
            cy_ble_bascReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}

#endif /* (CY_BLE_BAS_CLIENT) */


/******************************************************************************
* Function Name: Cy_BLE_BAS_EventHandler
***************************************************************************//**
*
*  Handles events from the BLE stack for the Battery Service.
*
*  \param eventCode:  The event code.
*  \param eventParam:  The event parameters.
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_BAS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_BAS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_BASC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_BASC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_BASC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_BAS_CLIENT */

            default:
                break;
        }
    }
    else
    {
        switch((cy_en_ble_event_t)eventCode)
        {
            /**************************************************************************
             * Handling GATT Server events
             ***************************************************************************/
        #ifdef CY_BLE_BAS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_BASS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;
        #endif /* CY_BLE_BAS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_BAS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_BASC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_BASC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_BASC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_BASC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_BAS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_BAS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */

