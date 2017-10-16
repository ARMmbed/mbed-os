/***************************************************************************//**
* \file cy_ble_hts.c
* \version 2.0
*
* \brief
*  Contains the source code for Health Thermometer Service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_HTS))

static cy_ble_callback_t Cy_BLE_HTS_ApplCallback = NULL;

#ifdef CY_BLE_HTS_SERVER
/* The internal storage for the last request handle to check response for server */
static cy_ble_gatt_db_attr_handle_t cy_ble_htssReqHandle;
#endif /* CY_BLE_HTS_SERVER */

#ifdef CY_BLE_HTS_CLIENT
/* HTS Center Service characteristics GATT DB handles structure */
cy_stc_ble_htsc_t cy_ble_htsc[CY_BLE_CONFIG_GATTC_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_htscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_HTS_CLIENT) */

/* The pointer on global BLE HTS Config structure */
cy_stc_ble_hts_config_t *cy_ble_htsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_HTS_SERVER
static void Cy_BLE_HTSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static cy_en_ble_gatt_err_code_t Cy_BLE_HTSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* CY_BLE_HTS_SERVER */

#ifdef CY_BLE_HTS_CLIENT
static void Cy_BLE_HTSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_HTSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_HTSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_HTSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_HTSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam);
static void Cy_BLE_HTSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_HTSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_HTSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* (CY_BLE_HTS_CLIENT) */
static cy_en_ble_gatt_err_code_t Cy_BLE_HTS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_HTS_Init
***************************************************************************//**
*
*  This function initializes HTS Service.
*
*  \param config: Configuration structure for HTS Service.
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
cy_en_ble_api_result_t Cy_BLE_HTS_Init(cy_stc_ble_hts_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_htsConfigPtr = config;

        /* Registers Event Handler for the HTS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_HTS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_htsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_HTS_ApplCallback = cy_ble_htsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_HTS_SERVER
        cy_ble_htssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    #endif /* CY_BLE_HTS_SERVER */

    #ifdef CY_BLE_HTS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_HTS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_htsc[idx], 0, sizeof(cy_stc_ble_htsc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_HTS].uuid = CY_BLE_UUID_HEALTH_THERMOMETER_SERVICE;
                }
                cy_ble_htscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
        #endif /* CY_BLE_HTS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HTS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc: An application layer event callback function to receive
*                     events from the BLE Component. The definition of
*                     cy_ble_callback_t for HTS Service is:\n
*                     typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                        void *eventParam)
*                     * eventCode indicates the event that triggered this
*                       callback (e.g. CY_BLE_EVT_HTSS_NOTIFICATION_ENABLED).
*                     * eventParam contains the parameters corresponding to the
*                       current event. (e.g. pointer to cy_stc_ble_hts_char_value_t
*                       structure that contains details of the characteristic
*                       for which notification enabled event was triggered).
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
cy_en_ble_api_result_t Cy_BLE_HTS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_HTS_ApplCallback = callbackFunc;
    if(cy_ble_htsConfigPtr != NULL)
    {
        cy_ble_htsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_HTS_SERVER


/******************************************************************************
* Function Name: Cy_BLE_HTSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets the characteristic value of the service in the local database.
*
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size (in Bytes) of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               stored in the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HTSS_SetCharacteristicValue(cy_en_ble_hts_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if(charIndex >= CY_BLE_HTS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Store data in database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_htsConfigPtr->htss->charInfo[charIndex].charHandle,
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
* Function Name: Cy_BLE_HTSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets the characteristic value of the service, which is a value identified by
*  charIndex.
*
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the location where characteristic value data
*               should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HTSS_GetCharacteristicValue(cy_en_ble_hts_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if(charIndex >= CY_BLE_HTS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Read characteristic value from database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_htsConfigPtr->htss->charInfo[charIndex].charHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .offset                     = 0u,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED,
        };
        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HTSS_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sets the characteristic descriptor of the specified characteristic.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param attrSize:   The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the descriptor value data that should
*              be stored in the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HTSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hts_char_index_t charIndex,
                                                               cy_en_ble_hts_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if((charIndex >= CY_BLE_HTS_CHAR_COUNT) || (descrIndex >= CY_BLE_HTS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        if(cy_ble_htsConfigPtr->htss->charInfo[charIndex].descrHandle[descrIndex] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_htsConfigPtr->htss->charInfo[charIndex].descrHandle[descrIndex],
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .connHandle                 = connHandle,
                .offset                     = 0u,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED,
            };
            if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
        }
        else
        {
            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HTSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets the characteristic descriptor of the specified characteristic.
*
*  \param connHandle: The connection handle
*  \param charIndex: The index of the service characteristic.
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param attrSize: The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the location where characteristic descriptor
*               value data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HTSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hts_char_index_t charIndex,
                                                               cy_en_ble_hts_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if((charIndex >= CY_BLE_HTS_CHAR_COUNT) || (descrIndex >= CY_BLE_HTS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        if(cy_ble_htsConfigPtr->htss->charInfo[charIndex].descrHandle[descrIndex] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_htsConfigPtr->htss->charInfo[charIndex].descrHandle[descrIndex],
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
        else
        {
            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HTSS_WriteEventHandler
***************************************************************************//**
*
*  Handles Write Request Event for HTS service.
*
*  \param void *eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_HTSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_hts_char_index_t locCharIndex;
    cy_stc_ble_hts_char_value_t locCharValue = { .connHandle = eventParam->connHandle };
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    uint8_t locReqHandle = 0u;


    if(Cy_BLE_HTS_ApplCallback != NULL)
    {
        for(locCharIndex = CY_BLE_HTS_TEMP_MEASURE; (locCharIndex < CY_BLE_HTS_CHAR_COUNT) && (locReqHandle == 0u);
            locCharIndex++)
        {
            if((eventParam->handleValPair.attrHandle ==
                cy_ble_htsConfigPtr->htss->charInfo[locCharIndex].descrHandle[CY_BLE_HTS_CCCD]) ||
               (eventParam->handleValPair.attrHandle ==
                cy_ble_htsConfigPtr->htss->charInfo[locCharIndex].charHandle))
            {
                locCharValue.charIndex = locCharIndex;
                locCharValue.value = &eventParam->handleValPair.value;
                /* Characteristic value write request */
                if(eventParam->handleValPair.attrHandle == cy_ble_htsConfigPtr->htss->charInfo[locCharIndex].charHandle)
                {
                    /* Validate Measure Interval value */
                    if(locCharIndex == CY_BLE_HTS_MEASURE_INTERVAL)
                    {
                        uint8_t locAttrValue[CY_BLE_HTS_VRD_LEN];
                        uint16_t requestValue;

                        requestValue = Cy_BLE_Get16ByPtr(eventParam->handleValPair.value.val);

                        if(requestValue != 0u) /* 0 is valid interval value for no periodic measurement */
                        {
                            /* Check Valid range for Measure Interval characteristic value */
                            if(Cy_BLE_HTSS_GetCharacteristicDescriptor(eventParam->connHandle, locCharIndex,
                                                                       CY_BLE_HTS_VRD, CY_BLE_HTS_VRD_LEN, locAttrValue) ==
                               CY_BLE_SUCCESS)
                            {
                                uint16_t lowerValue;
                                uint16_t upperValue;

                                lowerValue = Cy_BLE_Get16ByPtr(locAttrValue);
                                upperValue = Cy_BLE_Get16ByPtr(locAttrValue + sizeof(lowerValue));
                                requestValue = Cy_BLE_Get16ByPtr(eventParam->handleValPair.value.val);
                                if((requestValue != 0u) && ((requestValue < lowerValue) || (requestValue > upperValue)))
                                {
                                    gattErr = CY_BLE_GATT_ERR_HTS_OUT_OF_RANGE;
                                }
                            }
                        }
                    }
                    if(gattErr == CY_BLE_GATT_ERR_NONE)
                    {   /* Store value to database */
                        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                        {
                            .handleValuePair = eventParam->handleValPair,
                            .connHandle      = eventParam->connHandle,
                            .offset          = 0u,
                            .flags           = CY_BLE_GATT_DB_PEER_INITIATED,
                        };
                        gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);
                        if(gattErr == CY_BLE_GATT_ERR_NONE)
                        {
                            Cy_BLE_HTS_ApplCallback((uint32_t)CY_BLE_EVT_HTSS_WRITE_CHAR, &locCharValue);
                        }
                    }
                }
                else /* Client Characteristic Configuration descriptor write request */
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
                        /* Check characteristic properties for Notification */
                        if(CY_BLE_IS_NOTIFICATION_SUPPORTED(cy_ble_htsConfigPtr->htss->charInfo[locCharIndex].charHandle))
                        {
                            if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                            {
                                Cy_BLE_HTS_ApplCallback((uint32_t)CY_BLE_EVT_HTSS_NOTIFICATION_ENABLED, &locCharValue);
                            }
                            else
                            {
                                Cy_BLE_HTS_ApplCallback((uint32_t)CY_BLE_EVT_HTSS_NOTIFICATION_DISABLED, &locCharValue);
                            }
                        }
                        /* Check characteristic properties for Indication */
                        if(CY_BLE_IS_INDICATION_SUPPORTED(cy_ble_htsConfigPtr->htss->charInfo[locCharIndex].charHandle))
                        {
                            if(CY_BLE_IS_INDICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                            {
                                Cy_BLE_HTS_ApplCallback((uint32_t)CY_BLE_EVT_HTSS_INDICATION_ENABLED, &locCharValue);
                            }
                            else
                            {
                                Cy_BLE_HTS_ApplCallback((uint32_t)CY_BLE_EVT_HTSS_INDICATION_DISABLED, &locCharValue);
                            }
                        }
                    #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
                        (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
                        /* Set flag to store bonding data to flash */
                        if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
                        {
                            cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
                        }
                    #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */
                    }
                }
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                locReqHandle = 1u;
            }
        }
    }

    return(gattErr);
}


/******************************************************************************
* Function Name: Cy_BLE_HTSS_SendNotification
***************************************************************************//**
*
*  Sends notification with a characteristic value of the Health Thermometer
*  Service, which is a value specified by charIndex, to the Client device.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_HTSC_NOTIFICATION event
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
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the client.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HTSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_hts_char_index_t charIndex,
                                                    uint8_t attrSize,
                                                    uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send Notification if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_htsConfigPtr->htss->charInfo[charIndex].
                                             descrHandle[CY_BLE_HTS_CCCD]))
    {
        apiResult = CY_BLE_ERROR_NTF_DISABLED;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
        {
            .handleValPair.attrHandle = cy_ble_htsConfigPtr->htss->charInfo[charIndex].charHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* Send notification to client using previously filled structure */
        apiResult = Cy_BLE_GATTS_Notification(&ntfReqParam);
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HTSS_SendIndication
***************************************************************************//**
*
*  Sends indication with a characteristic value of the Health Thermometer
*  Service, which is a value specified by charIndex, to the Client device.
*
*  On enabling indication successfully it sends out a 'Handle Value Indication' which
*  results in CY_BLE_EVT_HTSC_INDICATION or CY_BLE_EVT_GATTC_HANDLE_VALUE_IND (if
*  service-specific callback function is not registered) event at the GATT Client's end.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the Client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*   * CY_BLE_ERROR_IND_DISABLED - Indication is not enabled by the client
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the HTS service-specific callback is registered
*      (with Cy_BLE_HTS_RegisterAttrCallback):
*  * CY_BLE_EVT_HTSS_INDICATION_CONFIRMED - in case if the indication is
*                                successfully delivered to the peer device.
*  .
*   Otherwise (if the HTS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF - in case if the indication is
*                                successfully delivered to the peer device.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HTSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_hts_char_index_t charIndex,
                                                  uint8_t attrSize,
                                                  uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send Notification if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(!CY_BLE_IS_INDICATION_ENABLED(connHandle.attId, cy_ble_htsConfigPtr->htss->charInfo[charIndex].
                                           descrHandle[CY_BLE_HTS_CCCD]))
    {
        apiResult = CY_BLE_ERROR_IND_DISABLED;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
        {
            .handleValPair.attrHandle = cy_ble_htsConfigPtr->htss->charInfo[charIndex].charHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* Send indication to client using previously filled structure */
        apiResult = Cy_BLE_GATTS_Indication(&ntfReqParam);
        /* Save handle to support service-specific value confirmation response from client */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_htssReqHandle = ntfReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HTSS_ConfirmationEventHandler
***************************************************************************//**
*
*  Handles a Value Confirmation request event from the BLE stack.
*
*  *eventParam - The pointer to a structure of type cy_stc_ble_conn_handle_t.
*
******************************************************************************/
static void Cy_BLE_HTSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    cy_stc_ble_hts_char_value_t locCharValue = { .connHandle = *eventParam };
    cy_en_ble_hts_char_index_t locCharIndex;
    uint32_t locReqHandle = 0u;

    if((Cy_BLE_HTS_ApplCallback != NULL) && (cy_ble_htssReqHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locCharIndex = CY_BLE_HTS_TEMP_MEASURE; (locCharIndex < CY_BLE_HTS_CHAR_COUNT) && (locReqHandle == 0u);
            locCharIndex++)
        {
            if(cy_ble_htssReqHandle == cy_ble_htsConfigPtr->htss->charInfo[locCharIndex].charHandle)
            {
                locCharValue.charIndex = locCharIndex;
                locCharValue.value = NULL;
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                cy_ble_htssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_HTS_ApplCallback((uint32_t)CY_BLE_EVT_HTSS_INDICATION_CONFIRMED, &locCharValue);
                locReqHandle = 1u;
            }
        }
    }
}


#endif /* CY_BLE_HTS_SERVER */

#ifdef CY_BLE_HTS_CLIENT


/******************************************************************************
* Function Name: Cy_BLE_HTSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_HTSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* HTS characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_htscCharUuid[CY_BLE_HTS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_TEMPERATURE_MSMNT,
        CY_BLE_UUID_CHAR_TEMPERATURE_TYPE,
        CY_BLE_UUID_CHAR_INTERMEDIATE_TEMP,
        CY_BLE_UUID_CHAR_MSMNT_INTERVAL
    };
    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_HTS))
    {
        /* Update last characteristic endHandle to declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        for(i = (uint32_t)CY_BLE_HTS_TEMP_MEASURE; i < (uint32_t)CY_BLE_HTS_CHAR_COUNT; i++)
        {
            if(cy_ble_htscCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_htsc[discIdx].charInfo[i].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    Cy_BLE_CheckStoreCharHandle(cy_ble_htsc[discIdx].charInfo[i]);
                    lastEndHandle[discIdx] = &cy_ble_htsc[discIdx].charInfo[i].endHandle;
                    break;
                }
                else
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo);
                }
            }
        }

        /* Init characteristic endHandle to the Service endHandle. Characteristic endHandle will be updated to declaration
         * handler of the following characteristic, in the following characteristic discovery procedure. */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HTSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_FIND_INFO_RSP event.
*  Based on the descriptor UUID, an appropriate data structure is populated using
*  the data received as part of the callback.
*
*  \param discoveryCharIndex: The characteristic index which is discovered.
*  \param discDescrInfo: The pointer to a descriptor information structure.
*
******************************************************************************/
static void Cy_BLE_HTSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t notSupportedDescr = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t descIdx;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_HTS)
    {
        switch(discDescrInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_CLIENT_CONFIG:
                descIdx = (uint32_t)CY_BLE_HTS_CCCD;
                break;

            case CY_BLE_UUID_CHAR_VALID_RANGE:
                descIdx = (uint32_t)CY_BLE_HTS_VRD;
                break;

            default:
                /* Not supported descriptor */
                notSupportedDescr = 1u;
                break;
        }

        if(notSupportedDescr == 0u)
        {
            if(cy_ble_htsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] ==
               CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_htsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] =
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
* Function Name: Cy_BLE_HTSC_GetCharRange
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
static void Cy_BLE_HTSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_HTS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_HTS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if((cy_ble_htsc[discIdx].charInfo[charIdx].endHandle - cy_ble_htsc[discIdx].charInfo[charIdx].valueHandle) != 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_htsc[discIdx].charInfo[charIdx].valueHandle + 1u;
                charRangeInfo->range.endHandle = cy_ble_htsc[discIdx].charInfo[charIdx].endHandle;
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
* Function Name: Cy_BLE_HTSC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_HTSS_WRITE_CHAR events is generated.
*  On successful request execution on the Server side the Write Response is
*  sent to the Client.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic
*  * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                     characteristic
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the HTS service-specific callback is registered
*      (with Cy_BLE_HTS_RegisterAttrCallback):
*  * CY_BLE_EVT_HTSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_hts_char_value_t.
*  .
*   Otherwise (if the HTS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HTSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_hts_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_HTS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_htsc[discIdx].charInfo[charIndex].valueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_htsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);
        /* Save handle to support service-specific write response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_htscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HTSC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read a characteristic value, which is a value
*  identified by charIndex, from the server.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The read request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established
*  * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                     characteristic
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the HTS service-specific callback is registered
*      (with Cy_BLE_HTS_RegisterAttrCallback):
*  * CY_BLE_EVT_HTSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_hts_char_value_t.
*  .
*   Otherwise (if the HTS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_HTSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_hts_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_HTS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_htsc[discIdx].charInfo[charIndex].valueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_htsc[discIdx].charInfo[charIndex].valueHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);
        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_htscReqHandle[discIdx] = cy_ble_htsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HTSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  This function is used to write the characteristic descriptor to the server,
*  which is identified by charIndex and descrIndex.
*
*  Internally, Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server side the following events can be
*  generated:
*  * CY_BLE_EVT_HTSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_HTSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_HTSS_INDICATION_ENABLED
*  * CY_BLE_EVT_HTSS_INDICATION_DISABLED
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic descriptor value data that
*               should be sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the HTS service-specific callback is registered
*      (with Cy_BLE_HTS_RegisterAttrCallback):
*  * CY_BLE_EVT_HTSC_WRITE_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_hts_descr_value_t.
*  .
*   Otherwise (if the HTS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HTSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hts_char_index_t charIndex,
                                                               cy_en_ble_hts_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_HTS_CHAR_COUNT) || (descrIndex >= CY_BLE_HTS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(descrIndex != CY_BLE_HTS_CCCD)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_htsc[discIdx].charInfo[charIndex].descrHandle[CY_BLE_HTS_CCCD],
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* ... and send request to server device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_htscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HTSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets the characteristic descriptor of the specified characteristic of the
*  service.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*  \param descrIndex: The index of the service characteristic descriptor.
*
* \return
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the HTS service-specific callback is registered
*      (with Cy_BLE_HTS_RegisterAttrCallback):
*  * CY_BLE_EVT_HTSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_hts_descr_value_t.
*  .
*  Otherwise (if the HTS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_HTSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hts_char_index_t charIndex,
                                                               cy_en_ble_hts_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_HTS_CHAR_COUNT) || (descrIndex >= CY_BLE_HTS_DESCR_COUNT) ||
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_htsc[discIdx].charInfo[charIndex].descrHandle[descrIndex],
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_htscReqHandle[discIdx] = cy_ble_htsc[discIdx].charInfo[charIndex].descrHandle[descrIndex];
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HTSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles Notification Event.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_HTSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    cy_en_ble_hts_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_HTS_ApplCallback != NULL))
    {
        for(locCharIndex = CY_BLE_HTS_TEMP_MEASURE; locCharIndex < CY_BLE_HTS_CHAR_COUNT; locCharIndex++)
        {
            if(cy_ble_htsc[discIdx].charInfo[locCharIndex].valueHandle == eventParam->handleValPair.attrHandle)
            {
                cy_stc_ble_hts_char_value_t notifValue =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = locCharIndex,
                    .value      = &eventParam->handleValPair.value
                };

                Cy_BLE_HTS_ApplCallback((uint32_t)CY_BLE_EVT_HTSC_NOTIFICATION, &notifValue);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HTSC_IndicationEventHandler
***************************************************************************//**
*
*  Handles Indication Event.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_HTSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam)
{
    cy_en_ble_hts_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_HTS_ApplCallback != NULL))
    {
        for(locCharIndex = CY_BLE_HTS_TEMP_MEASURE; locCharIndex < CY_BLE_HTS_CHAR_COUNT; locCharIndex++)
        {
            if(cy_ble_htsc[discIdx].charInfo[locCharIndex].valueHandle == eventParam->handleValPair.attrHandle)
            {
                cy_stc_ble_hts_char_value_t indicationValue =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = locCharIndex,
                    .value      = &eventParam->handleValPair.value
                };

                Cy_BLE_HTS_ApplCallback((uint32_t)CY_BLE_EVT_HTSC_INDICATION, &indicationValue);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HTSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles Read Response Event.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_HTSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    cy_en_ble_hts_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
    uint32_t locReqHandle = 0u;

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_HTS_ApplCallback != NULL) &&
       (cy_ble_htscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locCharIndex = CY_BLE_HTS_TEMP_MEASURE; (locCharIndex < CY_BLE_HTS_CHAR_COUNT) && (locReqHandle == 0u);
            locCharIndex++)
        {
            if(cy_ble_htscReqHandle[discIdx] == cy_ble_htsc[discIdx].charInfo[locCharIndex].valueHandle)
            {
                cy_stc_ble_hts_char_value_t locCharValue =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = locCharIndex,
                    .value      = &eventParam->value
                };
                cy_ble_htscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_HTS_ApplCallback((uint32_t)CY_BLE_EVT_HTSC_READ_CHAR_RESPONSE, &locCharValue);
                locReqHandle = 1u;
            }
            else
            {
                cy_en_ble_hts_descr_index_t locDescIndex;

                for(locDescIndex = CY_BLE_HTS_CCCD; (locDescIndex < CY_BLE_HTS_DESCR_COUNT) &&
                    (locReqHandle == 0u); locDescIndex++)
                {
                    if(cy_ble_htscReqHandle[discIdx] == cy_ble_htsc[discIdx].charInfo[locCharIndex].
                        descrHandle[locDescIndex])
                    {
                        cy_stc_ble_hts_descr_value_t locDescrValue =
                        {
                            .connHandle = eventParam->connHandle,
                            .charIndex  = locCharIndex,
                            .descrIndex = locDescIndex,
                            .value      = &eventParam->value
                        };

                        cy_ble_htscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                        Cy_BLE_HTS_ApplCallback((uint32_t)CY_BLE_EVT_HTSC_READ_DESCR_RESPONSE, &locDescrValue);
                        locReqHandle = 1u;
                    }
                }
            }
        }
        if(locReqHandle != 0u)
        {
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HTSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles Write Response Event.
*
*  \param eventParam: The pointer to a data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_HTSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    cy_en_ble_hts_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);
    uint32_t locReqHandle = 0u;

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_HTS_ApplCallback != NULL) &&
       (cy_ble_htscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locCharIndex = CY_BLE_HTS_TEMP_MEASURE; (locCharIndex < CY_BLE_HTS_CHAR_COUNT) && (locReqHandle == 0u);
            locCharIndex++)
        {
            if(cy_ble_htscReqHandle[discIdx] == cy_ble_htsc[discIdx].charInfo[locCharIndex].valueHandle)
            {
                cy_stc_ble_hts_char_value_t locCharValue =
                {
                    .connHandle = *eventParam,
                    .charIndex  = locCharIndex,
                    .value      = NULL
                };
                cy_ble_htscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_HTS_ApplCallback((uint32_t)CY_BLE_EVT_HTSC_WRITE_CHAR_RESPONSE, &locCharValue);
                locReqHandle = 1u;
            }
            else if(cy_ble_htscReqHandle[discIdx] == cy_ble_htsc[discIdx].charInfo[locCharIndex].
                     descrHandle[CY_BLE_HTS_CCCD])
            {
                cy_stc_ble_hts_descr_value_t locDescrValue =
                {
                    .connHandle = *eventParam,
                    .charIndex  = locCharIndex,
                    .descrIndex = CY_BLE_HTS_CCCD,
                    .value      = NULL
                };
                cy_ble_htscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_HTS_ApplCallback((uint32_t)CY_BLE_EVT_HTSC_WRITE_DESCR_RESPONSE, &locDescrValue);
                locReqHandle = 1u;
            }
            else /* No other destination for write operation is possible */
            {
            }
        }
        if(locReqHandle != 0u)
        {
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HTSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles Error Response Event.
*
*  \param eventParam: The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_HTSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
        if(eventParam->errInfo.attrHandle == cy_ble_htscReqHandle[discIdx])
        {
            cy_ble_htscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}

#endif /* (CY_BLE_HTS_CLIENT) */

/******************************************************************************
* Function Name: Cy_BLE_HTS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Health Thermometer Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_HTS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_HTS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_HTSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_HTSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_HTSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_HTS_CLIENT */

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
        #ifdef CY_BLE_HTS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_HTSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF:
                Cy_BLE_HTSS_ConfirmationEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_HTS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_HTS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_HTSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_HTSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_IND:
                Cy_BLE_HTSC_IndicationEventHandler((cy_stc_ble_gattc_handle_value_ind_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_HTSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_HTSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_HTS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_HTS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */

