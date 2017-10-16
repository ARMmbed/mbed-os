/***************************************************************************//**
* \file cy_ble_tps.c
* \version 2.0
*
* \brief
*  This file contains the source code for the Tx Power Service of the BLE
*  Component.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_TPS))
static cy_ble_callback_t Cy_BLE_TPS_ApplCallback = NULL;

#ifdef CY_BLE_TPS_CLIENT
/* TPS Center Service characteristics GATT DB handles structure */
cy_stc_ble_tpsc_t cy_ble_tpsc[CY_BLE_CONFIG_GATTC_COUNT];

/* Internal storage for last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_tpscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_TPS_CLIENT */

/* The pointer on global BLE TPS Config structure */
cy_stc_ble_tps_config_t *cy_ble_tpsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_TPS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_TPSS_WriteEventHandler(const cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* (CY_BLE_TPS_SERVER) */

#ifdef CY_BLE_TPS_CLIENT
static void Cy_BLE_TPSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_TPSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_TPSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_TPSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_TPSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_TPSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_TPSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* (CY_BLE_TPS_CLIENT) */
static cy_en_ble_gatt_err_code_t Cy_BLE_TPS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_TPS_Init
***************************************************************************//**
*
*  This function initializes the Tx Power Service.
*
*  \param config: Configuration structure for TPS.
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
cy_en_ble_api_result_t Cy_BLE_TPS_Init(cy_stc_ble_tps_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_tpsConfigPtr = config;

        /* Registers Event Handler for the TPS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_TPS_EventHandler);


        /* Registers a callback function via config structure */
        if(cy_ble_tpsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_TPS_ApplCallback = cy_ble_tpsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_TPS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_TPS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset((void*)&cy_ble_tpsc[idx], 0, sizeof(cy_stc_ble_tpsc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_TPS].uuid = CY_BLE_UUID_TX_POWER_SERVICE;
                }
                cy_ble_tpscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_TPS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_TPS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc:  An application layer event callback function to receive
*                     events from the BLE Component. The definition of
*                     cy_ble_callback_t for TPS is: \n
*                     typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                       void *eventParam)
*                     * eventCode indicates the event that triggered this
*                       callback.
*                     * eventParam contains the parameters corresponding to the
*                       current event.
* \sideeffect
*  The *eventParams in the callback function should not be used by the
*  application once the callback function execution is finished. Otherwise this
*  data may become corrupted.
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
cy_en_ble_api_result_t Cy_BLE_TPS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_TPS_ApplCallback = callbackFunc;
    if(cy_ble_tpsConfigPtr != NULL)
    {
        cy_ble_tpsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_TPS_SERVER

/******************************************************************************
* Function Name: cy_ble_TpssWriteEventHandler
***************************************************************************//**
*
*  Handles the Write Request Event for Tx Power service.
*
*  \param cy_stc_ble_gatts_write_cmd_req_param_t * eventParam: The pointer to the data that came
*                                               with a write request for TPS.
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*   * CY_BLE_GATT_ERR_NONE - Write is successful.
*   * CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED - Request is not supported.
*   * CY_BLE_GATT_ERR_INVALID_HANDLE - 'handleValuePair.attrHandle' is not valid.
*   * CY_BLE_GATT_ERR_WRITE_NOT_PERMITTED - Write operation is not permitted on
*                                          this attribute.
*   * CY_BLE_GATT_ERR_INVALID_OFFSET - Offset value is invalid.
*   * CY_BLE_GATT_ERR_UNLIKELY_ERROR - Some other error occurred.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_TPSS_WriteEventHandler(const cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    uint32_t event = (uint32_t)CY_BLE_EVT_TPSS_NOTIFICATION_DISABLED;


    /* Check event code if it defines write request */
    if(NULL != Cy_BLE_TPS_ApplCallback)
    {
        /* Client Characteristic Configuration descriptor write request */
        if(eventParam->handleValPair.attrHandle == cy_ble_tpsConfigPtr->tpss->txPowerLevelCccdHandle)
        {
            /* Verify that optional notification property is enabled for Tx Power Level characteristic */
            if(CY_BLE_IS_NOTIFICATION_SUPPORTED(cy_ble_tpsConfigPtr->tpss->txPowerLevelCharHandle))
            {
                cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                {
                    .handleValuePair = eventParam->handleValPair,
                    .connHandle      = eventParam->connHandle,
                    .offset          = 0u,
                    .flags           = CY_BLE_GATT_DB_PEER_INITIATED
                };
                gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);

                if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                {
                    event = (uint32_t)CY_BLE_EVT_TPSS_NOTIFICATION_ENABLED;
                }

                if(gattErr == CY_BLE_GATT_ERR_NONE)
                {
                    cy_stc_ble_tps_char_value_t wrReqParam =
                    {
                        .connHandle = eventParam->connHandle,
                        .charIndex  = CY_BLE_TPS_TX_POWER_LEVEL,
                        .value      = NULL
                    };
                    Cy_BLE_TPS_ApplCallback(event, (void*)&wrReqParam);
                }
            }
            else
            {
                gattErr = CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED;
            }
            /* Indicate that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }

    return(gattErr);
}


/******************************************************************************
* Function Name: cy_ble_TpssSetCharacteristicValue
***************************************************************************//**
*
*  Sets characteristic value of the Tx Power Service, which is identified by
*  charIndex.
*
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               stored in the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic value was read successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of input parameters failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_TPSS_SetCharacteristicValue(cy_en_ble_tps_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          int8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if(charIndex < CY_BLE_TPS_CHAR_COUNT)
    {
        /* Set Tx Power Level characteristic value to GATT database. Need to handle return type difference of
         * Cy_BLE_GATTS_WriteAttributeValueCCCD() and cy_ble_tpssSetCharacteristicValue(). */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_tpsConfigPtr->tpss->txPowerLevelCharHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = (uint8_t*)attrValue,
            .offset                     = 0u,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };
        if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            /* Indicate success */
            apiResult = CY_BLE_SUCCESS;
        }
        
    }

    /* Return status */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_TPSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets characteristic value of the Tx Power Service, which is identified by
*  charIndex.
*
*  \param charIndex: The index of the Tx Power characteristic.
*  \param attrSize: The size of the Tx Power characteristic value attribute.
*  \param attrValue: The pointer to the location where Tx Power characteristic
*               value data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - Characteristic value was read successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_TPSS_GetCharacteristicValue(cy_en_ble_tps_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          int8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((attrValue != NULL) && (charIndex == CY_BLE_TPS_TX_POWER_LEVEL) && (attrSize == CY_BLE_TPS_TX_POWER_LEVEL_SIZE))
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_tpsConfigPtr->tpss->txPowerLevelCharHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = (uint8_t*)attrValue,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };
        /* Get Tx Power Level characteristic value from GATT database */
        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            /* Indicate success */
            apiResult = CY_BLE_SUCCESS;
        }
    }

    /* Return status */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_TPSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets characteristic descriptor of specified characteristic of the Tx Power
*  Service.
*
*  \param connHandle: The connection handle
*  \param charIndex: The index of the characteristic.
*  \param descrIndex: The index of the descriptor.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the location where characteristic descriptor value
*               data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - Characteristic Descriptor value was read successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of input parameters failed
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional descriptor is absent
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_TPSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_tps_char_index_t charIndex,
                                                               cy_en_ble_tps_char_descriptors_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex == CY_BLE_TPS_TX_POWER_LEVEL) && (descrIndex == CY_BLE_TPS_CCCD))
    {
        if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_tpsConfigPtr->tpss->txPowerLevelCccdHandle)
        {
            /* Set Client Characteristic Configuration Descriptor of Tx Power Level
             *  characteristic to GATT database.
             *  Need to handle return type difference of Cy_BLE_GATTS_WriteAttributeValueCCCD() and
             *  cy_ble_TpssGetCharacteristicDescriptor(). */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_tpsConfigPtr->tpss->txPowerLevelCccdHandle,
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = (uint8_t*)attrValue,
                .connHandle                 = connHandle,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };
            if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
            {
                /* Indicate success */
                apiResult = CY_BLE_SUCCESS;
            }
        }
        else
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_TPSS_SendNotification
***************************************************************************//**
*
*  Sends a notification with the characteristic value, as specified by charIndex,
*  to the Client device.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_TPSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the client's device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of input parameter failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this.
*                                      characteristic.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with client is not established.
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the client.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_TPSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_tps_char_index_t charIndex,
                                                    uint8_t attrSize,
                                                    int8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((attrValue != NULL) && (charIndex == CY_BLE_TPS_TX_POWER_LEVEL) && (attrSize == CY_BLE_TPS_TX_POWER_LEVEL_SIZE))
    {
        if(CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_tpsConfigPtr->tpss->txPowerLevelCccdHandle))
        {
            /* Set Tx Power Level characteristic value to GATT database.
             *  Need to handle return type difference of Cy_BLE_GATTS_WriteAttributeValueCCCD() and
             *  cy_ble_tpssSetCharacteristicValue(). */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_tpsConfigPtr->tpss->txPowerLevelCharHandle,
                .handleValuePair.value.val  = (uint8_t*)attrValue,
                .handleValuePair.value.len  = attrSize,
                .connHandle                 = connHandle,
                .offset                     = 0u,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };
            if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
            {
                if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
                {
                    apiResult = CY_BLE_ERROR_INVALID_STATE;
                }
                else
                {
                    /* Fill all fields of write request structure ... */
                    cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
                    {
                        .handleValPair.attrHandle = dbAttrValInfo.handleValuePair.attrHandle,
                        .handleValPair.value.val  = dbAttrValInfo.handleValuePair.value.val,
                        .handleValPair.value.len  = dbAttrValInfo.handleValuePair.value.len,
                        .connHandle               = connHandle
                    };
                    /* Send notification to client using previously filled structure */
                    apiResult = Cy_BLE_GATTS_Notification(&ntfReqParam);
                }
            }
        }
        else
        {
            apiResult = CY_BLE_ERROR_NTF_DISABLED;
        }
    }

    /* Return status */
    return(apiResult);
}

#endif /* (CY_BLE_GATT_ROLE_SERVER) */


#ifdef CY_BLE_TPS_CLIENT

/******************************************************************************
* Function Name: Cy_BLE_TPSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP event.
*  Based on the service UUID, an appropriate data structure is populated using the
*  data received as part of the callback.
*
*  \param cy_stc_ble_disc_char_info_t * discCharInfo: The pointer to the characteristic
*                                          information structure.
*
*
******************************************************************************/
static void Cy_BLE_TPSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_TPS))
    {
        if(discCharInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_TX_POWER_LEVEL)
        {
            /* Save Tx Power Level Characteristic handle */
            Cy_BLE_CheckStoreCharHandle(cy_ble_tpsc[discIdx].txPowerLevelChar);
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_TPSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_FIND_INFO_RSP event.
*  This event is generated when a server successfully sends the data for
*  CY_BLE_EVT_GATTC_FIND_INFO_REQ. Based on the service UUID, an appropriate data
*  structure is populated to the service with a service callback.
*
*  \param cy_stc_ble_disc_descr_info_t * discDescrInfo: The pointer to descriptor information
*                                            structure.
*
******************************************************************************/
static void Cy_BLE_TPSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_TPS)
    {
        if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_CLIENT_CONFIG)
        {
            Cy_BLE_CheckStoreCharDescrHandle(cy_ble_tpsc[discIdx].txPowerLevelCccdHandle);
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_TPSC_GetCharRange
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
static void Cy_BLE_TPSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_TPS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        if(cy_ble_discovery[discIdx].charCount == 0u)
        {
            /* One descriptor is available per characteristic */
            charRangeInfo->range.startHandle = cy_ble_tpsc[discIdx].txPowerLevelChar.valueHandle + 1u;
            charRangeInfo->range.endHandle = cy_ble_serverInfo[discIdx][CY_BLE_SRVI_TPS].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_TPSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles the Notification Event for the Tx Power Service.
*
*  \param cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam: The pointer to the data
*                                                     structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_TPSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_TPS_ApplCallback != NULL))
    {
        if(cy_ble_tpsc[discIdx].txPowerLevelChar.valueHandle == eventParam->handleValPair.attrHandle)
        {
            cy_stc_ble_tps_char_value_t ntfParam =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_TPS_TX_POWER_LEVEL,
                .value      = &eventParam->handleValPair.value
            };
            Cy_BLE_TPS_ApplCallback((uint32_t)CY_BLE_EVT_TPSC_NOTIFICATION, (void*)&ntfParam);
            cy_ble_eventHandlerFlag &= (uint8_t)(~CY_BLE_CALLBACK);
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_TPSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles the Read Response Event.
*
*  \param cy_stc_ble_gattc_read_rsp_param_t *eventParam: The pointer to the data that came
*                                             with a read response for the Tx
*                                             Power Service.
*
******************************************************************************/
static void Cy_BLE_TPSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_TPS_ApplCallback != NULL) &&
       (cy_ble_tpscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_tpsc[discIdx].txPowerLevelChar.valueHandle == cy_ble_tpscReqHandle[discIdx])
        {
            /* Fill Tx Power Service read response parameter structure for Tx Power Level
             * Characteristic. */
            cy_stc_ble_tps_char_value_t rdRspParam =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_TPS_TX_POWER_LEVEL,
                .value      = &eventParam->value
            };
            cy_ble_tpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_TPS_ApplCallback((uint32_t)CY_BLE_EVT_TPSC_READ_CHAR_RESPONSE, &rdRspParam);
        }
        else if(cy_ble_tpsc[discIdx].txPowerLevelCccdHandle == cy_ble_tpscReqHandle[discIdx])
        {
            /* Fill Tx Power Service read response parameter structure for Tx Power Level
             * Client Characteristic Configuration Descriptor. */
            cy_stc_ble_tps_descr_value_t rdRspParam =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_TPS_TX_POWER_LEVEL,
                .descrIndex = CY_BLE_TPS_CCCD,
                .value      = &eventParam->value
            };
            cy_ble_tpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_TPS_ApplCallback((uint32_t)CY_BLE_EVT_TPSC_READ_DESCR_RESPONSE, &rdRspParam);
        }
        else
        {
            /* No TPS characteristic was requested for read */
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_TPSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles the Write Response Event.
*
*  \param cy_stc_ble_conn_handle_t *eventParam: The pointer to a cy_stc_ble_conn_handle_t data structure.
*
*
******************************************************************************/
static void Cy_BLE_TPSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_TPS_ApplCallback != NULL) &&
       (cy_ble_tpsc[discIdx].txPowerLevelCccdHandle == cy_ble_tpscReqHandle[discIdx]) &&
       (cy_ble_tpscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        cy_stc_ble_tps_descr_value_t wrRspParam =
        {
            .connHandle = *eventParam,
            .charIndex  = CY_BLE_TPS_TX_POWER_LEVEL,
            .descrIndex = CY_BLE_TPS_CCCD,
            .value      = NULL
        };
        cy_ble_tpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        Cy_BLE_TPS_ApplCallback((uint32_t)CY_BLE_EVT_TPSC_WRITE_DESCR_RESPONSE, &wrRspParam);
    }
}


/******************************************************************************
* Function Name: Cy_BLE_TPSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles the Error Response Event for the Tx Power Service.
*
*  \param cy_stc_ble_gatt_err_param_t *eventParam: The pointer to the
*                                            cy_stc_ble_gatt_err_param_t structure.
*
* \return
*  None
*
******************************************************************************/
static void Cy_BLE_TPSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
        if(eventParam->errInfo.attrHandle != cy_ble_tpscReqHandle[discIdx])
        {
            cy_ble_tpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_TPSC_GetCharacteristicValue
***************************************************************************//**
*
*  Gets the characteristic value of the Tx Power Service, which is
*  identified by charIndex.
*
*  This function call can result in generation of the following events based on
*  the response from the server device:
*  * CY_BLE_EVT_TPSC_READ_CHAR_RESPONSE
*  * CY_BLE_EVT_GATTC_ERROR_RSP
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the characteristic.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - Request was sent successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                     characteristic
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the TPS service-specific callback is registered
*      (with Cy_BLE_TPS_RegisterAttrCallback):
*  * CY_BLE_EVT_TPSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_tps_char_value_t.
*  .
*   Otherwise (if the TPS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_TPSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_tps_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    if((charIndex == CY_BLE_TPS_TX_POWER_LEVEL) && (discIdx < CY_BLE_CONFIG_GATTC_COUNT))
    {
        /* Send request to write Tx Power Level characteristic value */
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_tpsc[discIdx].txPowerLevelChar.valueHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_tpscReqHandle[discIdx] = cy_ble_tpsc[discIdx].txPowerLevelChar.valueHandle;
        }
    }
    else
    {
        /* apiResult equals CY_BLE_ERROR_INVALID_PARAMETER */
    }

    /* Return status */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_TPSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sets a characteristic descriptor value of the Tx Power Service.
*
*  Internally, Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server side the following events can be
*  generated:
*  * CY_BLE_EVT_TPSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_TPSS_NOTIFICATION_DISABLED
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the Characteristic
*  \param descrIndex: The index of the TX Power Service characteristic descriptor.
*  \param attrSize:   The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the characteristic descriptor value data that
*              should be sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request was sent successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established.
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on the
*                                      specified attribute.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the TPS service-specific callback is registered
*      (with Cy_BLE_TPS_RegisterAttrCallback):
*  * CY_BLE_EVT_TPSC_WRITE_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_tps_descr_value_t.
*  .
*   Otherwise (if the TPS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_TPSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_tps_char_index_t charIndex,
                                                               cy_en_ble_tps_char_descriptors_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (attrValue != NULL) && 
       (charIndex == CY_BLE_TPS_TX_POWER_LEVEL) && (descrIndex == CY_BLE_TPS_CCCD) &&
       (attrSize == CY_BLE_CCCD_LEN))
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_tpsc[discIdx].txPowerLevelCccdHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* ... and send request to server device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_tpscReqHandle[discIdx] = cy_ble_tpsc[discIdx].txPowerLevelCccdHandle;
        }
    }
    else
    {
        /* apiResult equals CY_BLE_ERROR_INVALID_PARAMETER */
    }

    /* Return status */
    return(apiResult);
}


/******************************************************************************
* Function Name: TpscGetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets a characteristic descriptor of the Tx Power Service.
*
*  This function call can result in generation of the following events based on
*  the response from the server device:
*  * CY_BLE_EVT_TPSC_READ_DESCR_RESPONSE
*  * CY_BLE_EVT_GATTC_ERROR_RSP
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the characteristic.
*  \param descrIndex: The index of the characteristic descriptor.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - Request was sent successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*   * CY_BLE_ERROR_INVALID_STATE - The component in in invalid state for current
*                                 operation.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Cannot process request to send PDU due to
*                                     invalid operation performed by the
*                                     application.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the TPS service-specific callback is registered
*      (with Cy_BLE_TPS_RegisterAttrCallback):
*  * CY_BLE_EVT_TPSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_tps_descr_value_t.
*  .
*  Otherwise (if the TPS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_TPSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_tps_char_index_t charIndex,
                                                               cy_en_ble_tps_char_descriptors_t descrIndex)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (charIndex == CY_BLE_TPS_TX_POWER_LEVEL) && 
            (descrIndex == CY_BLE_TPS_CCCD))
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_tpsc[discIdx].txPowerLevelCccdHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_tpscReqHandle[discIdx] = cy_ble_tpsc[discIdx].txPowerLevelCccdHandle;
        }
    }
    else
    {
        /* apiResult equals CY_BLE_ERROR_INVALID_PARAMETER */
    }

    /* Return status */
    return(apiResult);
}

#endif /* CY_BLE_TPS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_TPS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Tx Power Service of the BLE.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_TPS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_TPS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_TPSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_TPSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_TPSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_TPS_CLIENT */

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
        #ifdef CY_BLE_TPS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_TPSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;
        #endif /* CY_BLE_TPS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_TPS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_TPSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_TPSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_TPSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_TPSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_TPS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_TPS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
