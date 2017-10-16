/***************************************************************************//**
* \file cy_ble_wss.c
* \version 2.0
*
* \brief
*  Contains the source code for the Weight Scale Service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_WSS))

#ifdef CY_BLE_WSS_SERVER
/* The internal storage for the last request handle to check response for server */
static cy_ble_gatt_db_attr_handle_t cy_ble_wsssReqHandle;
#endif /* CY_BLE_WSS_SERVER */

#ifdef CY_BLE_WSS_CLIENT
/* WSS Center Service characteristics GATT DB handles structure */
cy_stc_ble_wssc_t cy_ble_wssc[CY_BLE_CONFIG_GATTC_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_wsscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_WSS_CLIENT) */

static cy_ble_callback_t Cy_BLE_WSS_ApplCallback = NULL;

/* The pointer on global BLE WSS Config structure */
cy_stc_ble_wss_config_t *cy_ble_wssConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_WSS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_WSSS_WriteEventHandler(const cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
static void Cy_BLE_WSSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
#endif /* CY_BLE_WSS_SERVER */

#ifdef CY_BLE_WSS_CLIENT
static void Cy_BLE_WSSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_WSSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_WSSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_WSSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam);
static void Cy_BLE_WSSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_WSSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_WSSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* CY_BLE_WSS_CLIENT */
static cy_en_ble_gatt_err_code_t Cy_BLE_WSS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_WSS_Init
***************************************************************************//**
*
*  This function initializes Weight Scale Service.
*
*  \param config: Configuration structure for the Weight Scale Service.
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
cy_en_ble_api_result_t Cy_BLE_WSS_Init(cy_stc_ble_wss_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_wssConfigPtr = config;

        /* Registers Event Handler for the WSS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_WSS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_wssConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_WSS_ApplCallback = cy_ble_wssConfigPtr->callbackFunc;
        }

        #ifdef CY_BLE_WSS_SERVER
        cy_ble_wsssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        #endif /* CY_BLE_WSS_SERVER */

        #ifdef CY_BLE_WSS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_WSS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_wssc[idx], 0, sizeof(cy_stc_ble_wssc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_WSS].uuid = CY_BLE_UUID_WEIGHT_SCALE_SERVICE;
                }
                cy_ble_wsscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
        #endif /* CY_BLE_WSS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WSS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc:  An application layer event callback function to receive
*                 events from the BLE Component. The definition of
*                 cy_ble_callback_t is: \n
*                 typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                    void *eventParam)
*                 * eventCode - Indicates the event that triggered this
*                   callback (e.g. CY_BLE_EVT_WSSS_INDICATION_ENABLED).
*                 * eventParam - Contains the parameters corresponding to the
*                   current event. (e.g. pointer to cy_stc_ble_wss_char_value_t
*                   structure that contains details of the characteristic
*                   for which an indication enabled event was triggered).
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
cy_en_ble_api_result_t Cy_BLE_WSS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_WSS_ApplCallback = callbackFunc;
    if(cy_ble_wssConfigPtr != NULL)
    {
        cy_ble_wssConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_WSS_SERVER

/******************************************************************************
* Function Name: Cy_BLE_WSSS_WriteEventHandler
***************************************************************************//**
*
*  Handles Write Request Event for Weight Scale Service.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*   * CY_BLE_GATT_ERR_NONE - Write is successful.
*   * CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED - The request is not supported.
*   * CY_BLE_GATT_ERR_INVALID_HANDLE - 'handleValuePair.attrHandle' is not valid.
*   * CY_BLE_GATT_ERR_WRITE_NOT_PERMITTED - The write operation is not permitted on
*                                          this attribute.
*   * CY_BLE_GATT_ERR_INVALID_OFFSET - The offset value is invalid.
*   * CY_BLE_GATT_ERR_UNLIKELY_ERROR - Some other error occurred.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_WSSS_WriteEventHandler(const cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    uint32_t event = (uint32_t)CY_BLE_EVT_WSSS_INDICATION_DISABLED;

    if(Cy_BLE_WSS_ApplCallback != NULL)
    {
        /* Client Characteristic Configuration descriptor write request */
        if(eventParam->handleValPair.attrHandle ==
           cy_ble_wssConfigPtr->wsss->charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].descrHandle[CY_BLE_WSS_CCCD])
        {
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair = eventParam->handleValPair,
                .connHandle      = eventParam->connHandle,
                .offset          = 0u,
                .flags           = CY_BLE_GATT_DB_PEER_INITIATED
            };

            if(CY_BLE_IS_INDICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
            {
                event = (uint32_t)CY_BLE_EVT_WSSS_INDICATION_ENABLED;
            }
            gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);

            if(gattErr == CY_BLE_GATT_ERR_NONE)
            {
                cy_stc_ble_wss_char_value_t wrReqParam =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = CY_BLE_WSS_WEIGHT_MEASUREMENT,
                    .value      = NULL
                };
                Cy_BLE_WSS_ApplCallback(event, &wrReqParam);
            }

        #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
            (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
            /* Set flag to store bonding data to flash */
            if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
            {
                cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
            }
        #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */

            /* Clear callback flag indicating that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }

    return(gattErr);
}


/******************************************************************************
* Function Name: Cy_BLE_WSSS_ConfirmationEventHandler
***************************************************************************//**
*
*  Handles a Value Confirmation request event from the BLE stack.
*
*  \param eventParam: The pointer to a structure of type cy_stc_ble_conn_handle_t.
*
******************************************************************************/
static void Cy_BLE_WSSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    if((Cy_BLE_WSS_ApplCallback != NULL) && (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_wsssReqHandle))
    {
        /* Only Weight Measurement Characteristic has the Indication property. Check if
         * the requested handle is the handle of the Descriptor Value Change handle.
         */
        if(cy_ble_wsssReqHandle == cy_ble_wssConfigPtr->wsss->charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].charHandle)
        {
            /* Fill in event data and inform Application about successfully
             * confirmed indication.
             */
            cy_stc_ble_wss_char_value_t locCharValue =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_WSS_WEIGHT_MEASUREMENT,
                .value      = NULL
            };
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            cy_ble_wsssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_WSS_ApplCallback((uint32_t)CY_BLE_EVT_WSSS_INDICATION_CONFIRMED, &locCharValue);
        }
    }
}


#if (CY_BLE_GAP_ROLE_PERIPHERAL)

/******************************************************************************
* Function Name: Cy_BLE_WSS_SetAdUserId
***************************************************************************//**
*
*  Sets the User ID List to the advertisement packet. To be able to set the
*  User ID List with this function, the advertisement packet should be
*  configured in the component GUI to include Weight Scale Service UUID in the
*  Service Data field. The Service Data should have enough room to fit the
*  User ID List that is planned to be advertised. To reserve the room for the
*  User ID List, the Service Data for WSS should be filled with Unknown User
*  ID - 0xFF. The amount of 0xFF's should be equal to User List Size that is
*  planned to be advertised.
*  This function must be called when Cy_BLE_GetBleSsState() returns
*  CY_BLE_BLESS_STATE_EVENT_CLOSE state.
*
*  \param listSize: The size of the User List.
*  \param userIdList: The array contains a User List.
*  \param advertisingParamIndex: The index of the peripheral and broadcast
*                                configuration in customizer. For example:
*  * CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX     0x00
*  * CY_BLE_BROADCASTER_CONFIGURATION_0_INDEX    0x01
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - On NULL pointer, Data length in input
*                parameter exceeds maximum advertisement packet length.
*  * CY_BLE_ERROR_INVALID_OPERATION - The advertisement packet doesn't contain
*                the User List or it is to small or ADV Event is not closed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WSS_SetAdUserId(uint8_t listSize,
                                              const uint8_t userIdList[],
                                              uint8_t advertisingParamIndex)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    uint32_t i;
    uint32_t fFlag = 0u;
    uint8_t adLength = 0u;
    uint8_t byteCounter = 0u;

    if((advertisingParamIndex >= CY_BLE_GAPP_CONF_COUNT) || (userIdList == NULL) ||
       (listSize > (CY_BLE_GAP_MAX_ADV_DATA_LEN - CY_BLE_AD_SERVICE_DATA_OVERHEAD)))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        while((byteCounter < cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advData->advDataLen) &&
              (fFlag == 0u))
        {
            adLength = cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advData->advData[byteCounter];

            if(adLength != 0u)
            {
                uint16_t servUuid;

                /* Increment byte counter so that it points to AD type field */
                byteCounter++;

                servUuid = Cy_BLE_Get16ByPtr(&cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advData->
                                              advData[byteCounter + 1u]);

                /* Check if "Service Data" AD type is found and service UUID is WSS UUID */
                if((cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advData->advData[byteCounter] ==
                    CY_BLE_AD_TYPE_SERVICE_DATA) && (servUuid == CY_BLE_UUID_WEIGHT_SCALE_SERVICE))
                {
                    /* WSS Service Data was found. Set flag and exit the loop. */
                    fFlag = 1u;
                }
                else
                {
                    byteCounter += adLength;
                }
            }
            else
            {
                /* End of advertisement data structure was encountered so exit loop. */
                break;
            }
        }
        if(fFlag != 0u)
        {
            /* Check if there is enough space to fit user index list */
            if((adLength - CY_BLE_AD_SERVICE_DATA_OVERHEAD) >= listSize)
            {
                /* Increment byte counter so that it points to data value */
                byteCounter += CY_BLE_AD_SERVICE_DATA_OVERHEAD;

                for(i = 0u; i < ((uint32_t)adLength - CY_BLE_AD_SERVICE_DATA_OVERHEAD); i++)
                {
                    if(i <= listSize)
                    {
                        /* Copy user index element to advertisement data byte */
                        cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advData->advData[byteCounter + i] =
                            userIdList[i];
                    }
                    else
                    {
                        /* Fill remaining bytes with "Unknown User" ID */
                        cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advData->advData[byteCounter + i] =
                            CY_BLE_WSS_UNKNOWN_USER;
                    }
                }

                /* We are done. Indicate success. */
                apiResult = CY_BLE_SUCCESS;

                if((Cy_BLE_GetAdvertisementState() == CY_BLE_ADV_STATE_ADVERTISING) &&
                   (cy_ble_advIndex == advertisingParamIndex))
                {
                    /* Update the advertisement packet if the device is in the advertising mode. */
                    apiResult = Cy_BLE_GAPP_UpdateAdvScanData(&cy_ble_configPtr->discoveryModeInfo[cy_ble_advIndex]);
                }
            }
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WSS_GetAdUserIdListSize
***************************************************************************//**
*
*  Returns the size (in bytes) of User ID List in the advertisement packet.
*
*  \param advertisingParamIndex: The index of the peripheral and broadcast
*                                configuration in customizer. For example:
*  * CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX     0x00
*  * CY_BLE_BROADCASTER_CONFIGURATION_0_INDEX    0x01
*
* \return
*  Size of User ID List.
*
******************************************************************************/
uint8_t Cy_BLE_WSS_GetAdUserIdListSize(uint8_t advertisingParamIndex)
{
    uint8_t uiCount = 0u;
    uint8_t adLength = 0u;
    uint32_t fFlag = 0u;
    uint8_t byteCounter = 0u;

    if(advertisingParamIndex >= CY_BLE_GAPP_CONF_COUNT)
    {
        uiCount = 0u;
    }
    else
    {
        while((byteCounter < cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advData->advDataLen))
        {
            adLength = cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advData->advData[byteCounter];

            if(adLength != 0u)
            {
                uint16_t servUuid;

                /* Increment byte counter so that it points to AD type field */
                byteCounter++;

                servUuid = Cy_BLE_Get16ByPtr(&cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advData->
                                              advData[byteCounter + 1u]);

                /* Check if "Service Data" AD type is found and service UUID is WSS UUID */
                if((cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advData->advData[byteCounter] ==
                    CY_BLE_AD_TYPE_SERVICE_DATA) && (servUuid == CY_BLE_UUID_WEIGHT_SCALE_SERVICE))
                {
                    /* WSS Service Data was found. Set flag and exit the loop. */
                    fFlag = 1u;
                }
                else
                {
                    byteCounter += adLength;
                }
            }
            else
            {
                /* End of advertisement data structure was encountered so exit loop. */
                break;
            }
        }
        if(fFlag != 0u)
        {
            uiCount = adLength - CY_BLE_AD_SERVICE_DATA_OVERHEAD;
        }
    }

    return(uiCount);
}

#endif /* (CY_BLE_GAP_ROLE_PERIPHERAL) */


/******************************************************************************
* Function Name: Cy_BLE_WSSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets a value for one of two characteristic values of the Weight Scale
*  Service. The characteristic is identified by charIndex.
*
*  \param charIndex: The index of a Weight Scale Service characteristic.
*  \param attrSize:  The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*                    stored to the GATT database.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic value was written successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WSSS_SetCharacteristicValue(cy_en_ble_wss_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
    {
        .handleValuePair.value.len = attrSize,
        .handleValuePair.value.val = attrValue,
        .offset                    = 0u,
        .flags                     = CY_BLE_GATT_DB_LOCALLY_INITIATED
    };

    if((attrValue != NULL) && (charIndex < CY_BLE_WSS_CHAR_COUNT))
    {
        /* Fill structure */
        if(charIndex == CY_BLE_WSS_WEIGHT_SCALE_FEATURE)
        {
            dbAttrValInfo.handleValuePair.attrHandle =
                cy_ble_wssConfigPtr->wsss->charInfo[CY_BLE_WSS_WEIGHT_SCALE_FEATURE].charHandle;
        }
        else
        {
            dbAttrValInfo.handleValuePair.attrHandle =
                cy_ble_wssConfigPtr->wsss->charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].charHandle;
        }

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
* Function Name: Cy_BLE_WSSS_GetCharacteristicValue
***************************************************************************//**
*
*  Reads a characteristic value of the Weight Scale Service, which is identified
*  by charIndex from the GATT database.
*
*  \param charIndex:  The index of the Weight Scale Service characteristic.
*  \param attrSize:   The size of the Weight Scale Service characteristic value attribute.
*  \param attrValue: The pointer to the location where characteristic value data
*                     should be stored.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic value was read successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WSSS_GetCharacteristicValue(cy_en_ble_wss_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
    {
        .handleValuePair.value.len = attrSize,
        .handleValuePair.value.val = attrValue,
        .flags                     = CY_BLE_GATT_DB_LOCALLY_INITIATED
    };

    if((attrValue != NULL) && (charIndex < CY_BLE_WSS_CHAR_COUNT))
    {
        if(charIndex == CY_BLE_WSS_WEIGHT_SCALE_FEATURE)
        {
            dbAttrValInfo.handleValuePair.attrHandle =
                cy_ble_wssConfigPtr->wsss->charInfo[CY_BLE_WSS_WEIGHT_SCALE_FEATURE].charHandle;
        }
        else
        {
            dbAttrValInfo.handleValuePair.attrHandle =
                cy_ble_wssConfigPtr->wsss->charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].charHandle;
        }

        /* Get characteristic value from GATT database */
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
* Function Name: Cy_BLE_WSSS_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sets the characteristic descriptor of the specified characteristic.
*
*  \param connHandle:   The connection handle.
*  \param charIndex:    The index of the service characteristic.
*  \param descrIndex:   The index of the descriptor.
*  \param attrSize:     The size of the characteristic descriptor attribute.
*  \param attrValue:    The pointer to the descriptor value data to be stored in the GATT
*                       database.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WSSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_wss_char_index_t charIndex,
                                                               cy_en_ble_wss_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex == CY_BLE_WSS_WEIGHT_MEASUREMENT) && (descrIndex == CY_BLE_WSS_CCCD) && (attrValue != NULL))
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_wssConfigPtr->wsss->charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].
                                           descrHandle[CY_BLE_WSS_CCCD],
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .connHandle                 = connHandle,
            .offset                     = 0u,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };
        /* Read characteristic value from database */
        if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_SUCCESS;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WSSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Reads a a characteristic descriptor of a specified characteristic of the
*  Weight Scale Service from the GATT database.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the characteristic.
*  \param descrIndex: The index of the descriptor.
*  \param attrSize:   The size of the descriptor value.
*  \param attrValue: The pointer to the location where characteristic descriptor value
*                     data should be stored.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WSSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_wss_char_index_t charIndex,
                                                               cy_en_ble_wss_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex == CY_BLE_WSS_WEIGHT_MEASUREMENT) && (descrIndex == CY_BLE_WSS_CCCD) && (attrValue != NULL))
    {
        if(cy_ble_wssConfigPtr->wsss->charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].descrHandle[CY_BLE_WSS_CCCD] !=
           CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle =
                    cy_ble_wssConfigPtr->wsss->charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].descrHandle[CY_BLE_WSS_CCCD],
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .connHandle                 = connHandle,
                .offset                     = 0u,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };
            /* Get characteristic value from GATT database */
            if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
            {
                /* Indicate success */
                apiResult = CY_BLE_SUCCESS;
            }
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WSSS_SendIndication
***************************************************************************//**
*
*  Sends an indication with a characteristic value of the Weight Scale Service,
*  which is a value specified by charIndex, to the client's device.
*
*  On enabling indication successfully it sends out a 'Handle Value Indication' which
*  results in CY_BLE_EVT_WSSC_INDICATION or CY_BLE_EVT_GATTC_HANDLE_VALUE_IND (if
*  service-specific callback function is not registered) event at the GATT Client's end.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*                     sent to the client's device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request was handled successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_IND_DISABLED - Indication is not enabled by the client.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the WSS service-specific callback is registered
*      (with Cy_BLE_WSS_RegisterAttrCallback):
*  * CY_BLE_EVT_WSSS_INDICATION_CONFIRMED - If the indication is
*                                successfully delivered to the peer device.
*  .
*   Otherwise (if the WSS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF - If the indication is
*                                successfully delivered to the peer device.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WSSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_wss_char_index_t charIndex,
                                                  uint8_t attrSize,
                                                  uint8_t *attrValue)
{
    /* Store new data in database */
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if(charIndex == CY_BLE_WSS_WEIGHT_MEASUREMENT)
    {
        /* Send indication if it is enabled and connected */
        if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
        {
            apiResult = CY_BLE_ERROR_INVALID_STATE;
        }
        else if(!CY_BLE_IS_INDICATION_ENABLED(connHandle.attId,
                                              cy_ble_wssConfigPtr->wsss->charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].descrHandle[
                                                  CY_BLE_WSS_CCCD]))
        {
            apiResult = CY_BLE_ERROR_IND_DISABLED;
        }
        else
        {
            /* Fill all fields of write request structure ... */
            cy_stc_ble_gatts_handle_value_ind_t indReqParam =
            {
                .handleValPair.attrHandle = cy_ble_wssConfigPtr->wsss->charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].charHandle,
                .handleValPair.value.val  = attrValue,
                .handleValPair.value.len  = attrSize,
                .connHandle               = connHandle
            };
            /* Send indication to client using previously filled structure */
            apiResult = Cy_BLE_GATTS_Indication(&indReqParam);

            /* Save handle to support service-specific value confirmation response from client */
            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_wsssReqHandle = indReqParam.handleValPair.attrHandle;
            }
        }
    }

    return(apiResult);
}

#endif /* CY_BLE_WSS_SERVER */


#ifdef CY_BLE_WSS_CLIENT

/******************************************************************************
* Function Name: Cy_BLE_WSSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_WSSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* WSS characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_wsscCharUuid[CY_BLE_WSS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_WEIGHT_SCALE_FEATURE,
        CY_BLE_UUID_CHAR_WEIGHT_MEASUREMENT
    };
    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_WSS))
    {
        /* Update last characteristic endHandle to declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        /* Search through all available characteristics */
        for(i = (uint32_t)CY_BLE_WSS_WEIGHT_SCALE_FEATURE; (i < (uint32_t)CY_BLE_WSS_CHAR_COUNT); i++)
        {
            if(cy_ble_wsscCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_wssc[discIdx].charInfo[i].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_wssc[discIdx].charInfo[i].valueHandle = discCharInfo->valueHandle;
                    cy_ble_wssc[discIdx].charInfo[i].properties = discCharInfo->properties;
                    lastEndHandle[discIdx] = &cy_ble_wssc[discIdx].charInfo[i].endHandle;
                    break;
                }
                else
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo);
                }
            }
        }

        /* Init characteristic endHandle to Service endHandle. Characteristic endHandle
         * will be updated to the declaration handler of the following characteristic,
         * in the following characteristic discovery procedure.
         */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_WSSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_FIND_INFO_RSP event.
*  Based on the descriptor UUID, an appropriate data structure is populated using
*  the data received as part of the callback.
*
*  \param discDescrInfo: The pointer to a descriptor information structure.
*
******************************************************************************/
static void Cy_BLE_WSSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t notSupportedDescr = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t descIdx;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_WSS)
    {
        switch(discDescrInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_CLIENT_CONFIG:
                descIdx = (uint32_t)CY_BLE_WSS_CCCD;
                break;

            default:
                /* Not supported descriptor */
                notSupportedDescr = 1u;
                break;
        }

        if(notSupportedDescr == 0u)
        {
            if(cy_ble_wssc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] ==
               CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_wssc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] =
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
* Function Name: Cy_BLE_WSSC_GetCharRange
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
static void Cy_BLE_WSSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_WSS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_WSS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if((cy_ble_wssc[discIdx].charInfo[charIdx].endHandle - cy_ble_wssc[discIdx].charInfo[charIdx].valueHandle) != 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_wssc[discIdx].charInfo[charIdx].valueHandle + 1u;
                charRangeInfo->range.endHandle = cy_ble_wssc[discIdx].charInfo[charIdx].endHandle;
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
* Function Name: Cy_BLE_WSSC_IndicationEventHandler
***************************************************************************//**
*
*  Handles an indication event for Weight Scale Service.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_WSSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_WSS_ApplCallback != NULL))
    {
        if(cy_ble_wssc[discIdx].charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].valueHandle ==
           eventParam->handleValPair.attrHandle)
        {
            cy_stc_ble_wss_char_value_t indicationValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_WSS_WEIGHT_MEASUREMENT,
                .value      = &eventParam->handleValPair.value
            };
            Cy_BLE_WSS_ApplCallback((uint32_t)CY_BLE_EVT_WSSC_INDICATION, &indicationValue);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_WSSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles a Read Response event for the Weight Scale Service.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_WSSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint8_t locReqHandle = 1u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_WSS_ApplCallback != NULL) &&
       (cy_ble_wsscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if((cy_ble_wsscReqHandle[discIdx] == cy_ble_wssc[discIdx].charInfo[CY_BLE_WSS_WEIGHT_SCALE_FEATURE].valueHandle) ||
           (cy_ble_wsscReqHandle[discIdx] == cy_ble_wssc[discIdx].charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].valueHandle))
        {
            cy_stc_ble_wss_char_value_t locCharValue;

            if(cy_ble_wsscReqHandle[discIdx] == cy_ble_wssc[discIdx].charInfo[CY_BLE_WSS_WEIGHT_SCALE_FEATURE].valueHandle)
            {
                locCharValue.charIndex = CY_BLE_WSS_WEIGHT_SCALE_FEATURE;
            }
            else
            {
                locCharValue.charIndex = CY_BLE_WSS_WEIGHT_MEASUREMENT;
            }

            locCharValue.connHandle = eventParam->connHandle;
            locCharValue.value = &eventParam->value;
            cy_ble_wsscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_WSS_ApplCallback((uint32_t)CY_BLE_EVT_WSSC_READ_CHAR_RESPONSE, &locCharValue);
        }
        else if(cy_ble_wsscReqHandle[discIdx] ==
                cy_ble_wssc[discIdx].charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].descrHandle[CY_BLE_WSS_CCCD])
        {
            cy_stc_ble_wss_descr_value_t locDescrValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_WSS_WEIGHT_MEASUREMENT,
                .descrIndex = CY_BLE_WSS_CCCD,
                .value      = &eventParam->value
            };
            cy_ble_wsscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_WSS_ApplCallback((uint32_t)CY_BLE_EVT_WSSC_READ_DESCR_RESPONSE, &locDescrValue);
        }
        else
        {
            locReqHandle = 0u;
        }

        if(locReqHandle != 0u)
        {
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_WSSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Write Response event for the Weight Scale Service.
*
*  \param eventParam: The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_WSSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    /* Check if service handler was registered and request handle is valid */
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_WSS_ApplCallback != NULL) &&
       (cy_ble_wsscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_wsscReqHandle[discIdx] ==
           cy_ble_wssc[discIdx].charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].descrHandle[CY_BLE_WSS_CCCD])
        {
            cy_stc_ble_wss_descr_value_t locDescrValue =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_WSS_WEIGHT_MEASUREMENT,
                .descrIndex = CY_BLE_WSS_CCCD,
                .value      = NULL
            };
            cy_ble_wsscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_WSS_ApplCallback((uint32_t)CY_BLE_EVT_WSSC_WRITE_DESCR_RESPONSE, &locDescrValue);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_WSSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles an Error Response event for the Weight Scale Service.
*
*  \param eventParam - The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_WSSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
        if(eventParam->errInfo.attrHandle == cy_ble_wsscReqHandle[discIdx])
        {
            cy_ble_wsscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_WSSC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read a characteristic value, which is a value
*  identified by charIndex, from the server.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic. Starts with zero.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The read request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                              the particular characteristic.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established.
*  * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                    characteristic.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the WSS service-specific callback is registered
*      (with Cy_BLE_WSS_RegisterAttrCallback):
*  * CY_BLE_EVT_WSSC_READ_CHAR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_wss_char_value_t.
*  .
*   Otherwise (if the WSS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP - If the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with an event parameters
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WSSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_wss_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_WSS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_wssc[discIdx].charInfo[charIndex].valueHandle)
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_wssc[discIdx].charInfo[charIndex].valueHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_wsscReqHandle[discIdx] = cy_ble_wssc[discIdx].charInfo[charIndex].valueHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WSSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  This function is used to write the characteristic descriptor to the server,
*  which is identified by charIndex and descrIndex.
*
*  Internally, Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server side the following events can be
*  generated:
*  * CY_BLE_EVT_WSSS_INDICATION_ENABLED
*  * CY_BLE_EVT_WSSS_INDICATION_DISABLED
*
*  \param connHandle:   The connection handle.
*  \param charIndex:    The index of the service characteristic. Starts with zero.
*  \param descrIndex:   The index of the service characteristic descriptor.
*  \param attrSize:     The size of the characteristic value attribute.
*  \param attrValue:   The pointer to the characteristic descriptor value data that
*                       should be sent to the server device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                    the specified attribute.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the WSS service-specific callback is registered
*      (with Cy_BLE_WSS_RegisterAttrCallback):
*  * CY_BLE_EVT_WSSC_WRITE_DESCR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_wss_descr_value_t.
*  .
*   Otherwise (if the WSS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - If the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WSSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_wss_char_index_t charIndex,
                                                               cy_en_ble_wss_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_WSS_CHAR_COUNT) || (descrIndex >= CY_BLE_WSS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_wssc[discIdx].charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].descrHandle[CY_BLE_WSS_CCCD] !=
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_wssc[discIdx].charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].
                                         descrHandle[CY_BLE_WSS_CCCD],
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* ... and send request to server's device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_wsscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WSSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Sends a request to get the characteristic descriptor of the specified
*  characteristic of the service.
*
*  \param connHandle:   The connection handle.
*  \param charIndex:    The index of the service characteristic. Starts with zero.
*  \param descrIndex:   The index of the service characteristic descriptor.
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
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the WSS service-specific callback is registered
*      (with Cy_BLE_WSS_RegisterAttrCallback):
*  * CY_BLE_EVT_WSSC_READ_DESCR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with an event parameter structure
*                                of type cy_stc_ble_wss_descr_value_t.
*  .
*  Otherwise (if the WSS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP - If the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with an event parameters
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WSSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_wss_char_index_t charIndex,
                                                               cy_en_ble_wss_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_WSS_CHAR_COUNT) || (descrIndex >= CY_BLE_WSS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_wssc[discIdx].charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].descrHandle[CY_BLE_WSS_CCCD] !=
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_wssc[discIdx].charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].descrHandle[CY_BLE_WSS_CCCD],
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_wsscReqHandle[discIdx] = cy_ble_wssc[discIdx].charInfo[CY_BLE_WSS_WEIGHT_MEASUREMENT].
                                             descrHandle[CY_BLE_WSS_CCCD];
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }

    return(apiResult);
}

#endif /* CY_BLE_WSS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_WSS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Weight Scale Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_WSS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_WSS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_WSSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_WSSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_WSSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_WSS_CLIENT */

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
        #ifdef CY_BLE_WSS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_WSSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF:
                Cy_BLE_WSSS_ConfirmationEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_WSS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_WSS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_WSSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_IND:
                Cy_BLE_WSSC_IndicationEventHandler((cy_stc_ble_gattc_handle_value_ind_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_WSSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_WSSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_WSS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_WSS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
