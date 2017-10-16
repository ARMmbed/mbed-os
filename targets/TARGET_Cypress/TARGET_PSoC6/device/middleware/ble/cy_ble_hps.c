/***************************************************************************//**
* \file cy_ble_hps.c
* \version 2.0
*
* \brief
*  Contains the source code for HTTP Proxy Service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_HPS))

#ifdef CY_BLE_HPS_SERVER
/* The internal storage for the last request handle to check response for server */
static cy_ble_gatt_db_attr_handle_t cy_ble_hpssReqHandle;
#endif /* CY_BLE_HPS_SERVER */

#ifdef CY_BLE_HPS_CLIENT
/* HPS Center Service characteristics GATT DB handles structure */
cy_stc_ble_hpsc_t cy_ble_hpsc[CY_BLE_CONFIG_GATTC_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_hpscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];

/* Variables for Read Long Characteristic Values handling */
static uint8_t * cy_ble_hpscRdLongBuffPtr;
static uint16_t cy_ble_hpscRdLongBuffLen;
static uint16_t cy_ble_hpscCurrLen;
#endif /* CY_BLE_HPS_CLIENT */

static cy_ble_callback_t Cy_BLE_HPS_ApplCallback = NULL;

/* The pointer on global BLE HPS Config structure */
cy_stc_ble_hps_config_t *cy_ble_hpsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_HPS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_HPSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
static void Cy_BLE_HPSS_PrepareWriteRequestEventHandler(const cy_stc_ble_gatts_prep_write_req_param_t *eventParam);
static void Cy_BLE_HPSS_ExecuteWriteRequestEventHandler(cy_stc_ble_gatts_exec_write_req_t *eventParam);
#endif /* CY_BLE_HPS_SERVER */

#ifdef CY_BLE_HPS_CLIENT
static void Cy_BLE_HPSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_HPSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_HPSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_HPSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_HPSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_HPSC_ReadLongRespEventHandler(const cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_HPSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_HPSC_ExecuteWriteResponseEventHandler(const cy_stc_ble_gattc_exec_write_rsp_t *eventParam);
static void Cy_BLE_HPSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* CY_BLE_HPS_CLIENT */
static cy_en_ble_gatt_err_code_t Cy_BLE_HPS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_HPS_Init
***************************************************************************//**
*
*  This function initializes HTTP Proxy Service.
*
*  \param config: Configuration structure for the HTTP Proxy Service.
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
cy_en_ble_api_result_t Cy_BLE_HPS_Init(cy_stc_ble_hps_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_hpsConfigPtr = config;

        /* Registers Event Handler for the HPS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_HPS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_hpsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_HPS_ApplCallback = cy_ble_hpsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_HPS_SERVER
        cy_ble_hpssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    #endif /* CY_BLE_HPS_SERVER */

    #ifdef CY_BLE_HPS_CLIENT
        {
            uint32_t idx;
            cy_ble_hpscRdLongBuffPtr = NULL;
            cy_ble_hpscRdLongBuffLen = 0u;
            cy_ble_hpscCurrLen = 0u;

            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_HPS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_hpsc[idx], 0, sizeof(cy_stc_ble_hpsc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_HPS].uuid = CY_BLE_UUID_HTTP_PROXY_SERVICE;
                }
                cy_ble_hpscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_HPS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HPS_RegisterAttrCallback
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
*                                                    void *eventParam) \n
*                 * eventCode - Indicates the event that triggered this
*                   callback (e.g. CY_BLE_EVT_HPSS_NOTIFICATION_ENABLED).
*                 * eventParam - Contains the parameters corresponding to the
*                   current event. (e.g. pointer to cy_stc_ble_hps_char_value_t
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
cy_en_ble_api_result_t Cy_BLE_HPS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_HPS_ApplCallback = callbackFunc;
    if(cy_ble_hpsConfigPtr != NULL)
    {
        cy_ble_hpsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_HPS_SERVER

/******************************************************************************
* Function Name: Cy_BLE_HPSS_WriteEventHandler
***************************************************************************//**
*
*  Handles Write Request Event for HTTP Proxy Service.
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
static cy_en_ble_gatt_err_code_t Cy_BLE_HPSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    uint32_t event = (uint32_t)CY_BLE_EVT_HPSS_NOTIFICATION_DISABLED;
    cy_stc_ble_hps_char_value_t wrCharReqParam =
    {
        .connHandle = eventParam->connHandle
    };

    if(Cy_BLE_HPS_ApplCallback != NULL)
    {
        /* Client Characteristic Configuration descriptor write request */
        if(eventParam->handleValPair.attrHandle ==
           cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_STATUS_CODE].descrHandle[CY_BLE_HPS_CCCD])
        {
            wrCharReqParam.charIndex = CY_BLE_HPS_HTTP_STATUS_CODE;
            wrCharReqParam.value = NULL;

            if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
            {
                event = (uint32_t)CY_BLE_EVT_HPSS_NOTIFICATION_ENABLED;
            }
        }
        else
        {
            event = (uint32_t)CY_BLE_EVT_HPSS_WRITE_CHAR;
            wrCharReqParam.value = &eventParam->handleValPair.value;

            if(cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_URI].charHandle == eventParam->handleValPair.attrHandle)
            {
                wrCharReqParam.charIndex = CY_BLE_HPS_URI;
            }
            else if(cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_HEADERS].charHandle ==
                    eventParam->handleValPair.attrHandle)
            {
                wrCharReqParam.charIndex = CY_BLE_HPS_HTTP_HEADERS;
            }
            else if(cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_ENTITY_BODY].charHandle ==
                    eventParam->handleValPair.attrHandle)
            {
                wrCharReqParam.charIndex = CY_BLE_HPS_HTTP_ENTITY_BODY;
            }
            else if(cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_CP].charHandle ==
                    eventParam->handleValPair.attrHandle)
            {
                if(CY_BLE_IS_NOTIFICATION_ENABLED(eventParam->connHandle.attId,
                                                  cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_STATUS_CODE].
                                                   descrHandle[CY_BLE_HPS_CCCD]))
                {
                    wrCharReqParam.charIndex = CY_BLE_HPS_HTTP_CP;
                }
                else
                {
                    gattErr = CY_BLE_GATT_ERR_CCCD_IMPROPERLY_CONFIGURED;

                    /* Clear callback flag indicating that request was handled */
                    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                }
            }
            else
            {
                /* Set charIndex  to CY_BLE_HPS_CHAR_COUNT as the requested handle doesn't
                 * match to any handles of HPS characteristics.
                 */
                wrCharReqParam.charIndex = CY_BLE_HPS_CHAR_COUNT;
            }
        }

        if(gattErr == CY_BLE_GATT_ERR_NONE)
        {
            if(wrCharReqParam.charIndex < CY_BLE_HPS_CHAR_COUNT)
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
                    if((event == (uint32_t)CY_BLE_EVT_HPSS_WRITE_CHAR) &&
                       (wrCharReqParam.value->len <=
                        (CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN(eventParam->handleValPair.attrHandle))))
                    {
                        CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(eventParam->handleValPair.attrHandle,
                                                             wrCharReqParam.value->len);
                    }

                    /* Make sure that GATT error is set to "No error" */
                    wrCharReqParam.gattErrorCode = CY_BLE_GATT_ERR_NONE;
                    Cy_BLE_HPS_ApplCallback(event, &wrCharReqParam);
                    gattErr = wrCharReqParam.gattErrorCode;
                }

                if((event == (uint32_t)CY_BLE_EVT_HPSS_NOTIFICATION_ENABLED) ||
                   (event == (uint32_t)CY_BLE_EVT_HPSS_NOTIFICATION_DISABLED))
                {
                #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
                    (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
                    /* Set flag to store bonding data to flash */
                    if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
                    {
                        cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
                    }
                #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */
                }

                /* Clear callback flag indicating that request was handled */
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            }
        }
    }

    return(gattErr);
}


/******************************************************************************
* Function Name: Cy_BLE_HPSS_PrepareWriteRequestEventHandler
***************************************************************************//**
*
*  Handles the Write Request HTTP Proxy Service.
*
*  \param eventParam: The pointer to the data that received with a prepare write
*                     request event for the HTTP Proxy Service.
*
******************************************************************************/
static void Cy_BLE_HPSS_PrepareWriteRequestEventHandler(const cy_stc_ble_gatts_prep_write_req_param_t *eventParam)
{
    if(Cy_BLE_HPS_ApplCallback != NULL)
    {
        if((eventParam->baseAddr[eventParam->currentPrepWriteReqCount - 1u].handleValuePair.attrHandle ==
            cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_URI].charHandle) ||
           (eventParam->baseAddr[eventParam->currentPrepWriteReqCount - 1u].handleValuePair.attrHandle ==
            cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_HEADERS].charHandle) ||
           (eventParam->baseAddr[eventParam->currentPrepWriteReqCount - 1u].handleValuePair.attrHandle ==
            cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_ENTITY_BODY].charHandle))
        {
            if(cy_ble_hpssReqHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                /* Send Prepare Write Response which identifies acknowledgment for
                 * long characteristic value write.
                 */

                cy_ble_hpssReqHandle =
                    eventParam->baseAddr[eventParam->currentPrepWriteReqCount - 1u].handleValuePair.attrHandle;
            }
            /* Indicate that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HPSS_ExecuteWriteRequestEventHandler
***************************************************************************//**
*
*  Handles the Execute Write Request Event for the HTTP Proxy Service.
*
*  \param eventParam: The pointer to the data that came with a write request
*                     for the HTTP Proxy Service.
*
******************************************************************************/
static void Cy_BLE_HPSS_ExecuteWriteRequestEventHandler(cy_stc_ble_gatts_exec_write_req_t *eventParam)
{
    uint32_t locCount;
    uint16_t locLength = 0u;
    cy_stc_ble_gatt_value_t locCharValue;
    cy_stc_ble_hps_char_value_t wrCharReqParam = { .connHandle = eventParam->connHandle };

    if((Cy_BLE_HPS_ApplCallback != NULL) && (cy_ble_hpssReqHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if((eventParam->baseAddr[0u].handleValuePair.attrHandle ==
            cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_URI].charHandle) ||
           (eventParam->baseAddr[0u].handleValuePair.attrHandle ==
            cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_HEADERS].charHandle) ||
           (eventParam->baseAddr[0u].handleValuePair.attrHandle ==
            cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_ENTITY_BODY].charHandle))
        {
            /* Check the execWriteFlag before execute or cancel write long operation */
            if(eventParam->execWriteFlag == CY_BLE_GATT_EXECUTE_WRITE_EXEC_FLAG)
            {
                /* Calculate total length */
                for(locCount = 0u; locCount < eventParam->prepWriteReqCount; locCount++)
                {
                    locLength += eventParam->baseAddr[locCount].handleValuePair.value.len;
                }

                if(CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN(cy_ble_hpssReqHandle) >= locLength)
                {
                    /* Fill data and pass it to user */
                    if(eventParam->baseAddr[0u].handleValuePair.attrHandle == cy_ble_hpsConfigPtr->hpss->
                        charInfo[CY_BLE_HPS_URI].charHandle)
                    {
                        wrCharReqParam.charIndex = CY_BLE_HPS_URI;
                    }
                    else if(eventParam->baseAddr[0u].handleValuePair.attrHandle ==
                            cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_HEADERS].charHandle)
                    {
                        wrCharReqParam.charIndex = CY_BLE_HPS_HTTP_HEADERS;
                    }
                    else
                    {
                        wrCharReqParam.charIndex = CY_BLE_HPS_HTTP_ENTITY_BODY;
                    }


                    wrCharReqParam.gattErrorCode = CY_BLE_GATT_ERR_NONE;
                    locCharValue = eventParam->baseAddr[0u].handleValuePair.value;
                    wrCharReqParam.value = &locCharValue;
                    wrCharReqParam.value->len = locLength;

                    Cy_BLE_HPS_ApplCallback((uint32_t)CY_BLE_EVT_HPSS_WRITE_CHAR, &wrCharReqParam);

                    if(wrCharReqParam.gattErrorCode == CY_BLE_GATT_ERR_NONE)
                    {
                        CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(cy_ble_hpssReqHandle, locLength);
                    }
                }
                else
                {
                    wrCharReqParam.gattErrorCode = CY_BLE_GATT_ERR_INVALID_ATTRIBUTE_LEN;
                }

                /* Pass user error code to Stack */
                eventParam->gattErrorCode = (uint8_t)wrCharReqParam.gattErrorCode;
            }

            /* Indicate that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;

            /* Clear requested handle */
            cy_ble_hpssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HPSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets a value for one of characteristic values of the HTTP Proxy
*  Service. The characteristic is identified by charIndex.
*
*  \param charIndex: The index of a HTTP Proxy Service characteristic.
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
cy_en_ble_api_result_t Cy_BLE_HPSS_SetCharacteristicValue(cy_en_ble_hps_char_index_t charIndex,
                                                          uint16_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((attrValue != NULL) && (charIndex < CY_BLE_HPS_CHAR_COUNT))
    {
        /* Fill structure */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_hpsConfigPtr->hpss->charInfo[charIndex].charHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .offset                     = 0u,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };

        if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            if(attrSize <= CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN(dbAttrValInfo.handleValuePair.attrHandle))
            {
                CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(dbAttrValInfo.handleValuePair.attrHandle, attrSize);
            }

            /* Indicate success */
            apiResult = CY_BLE_SUCCESS;
        }
    }

    /* Return status */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HPSS_GetCharacteristicValue
***************************************************************************//**
*
*  Reads a characteristic value of the HTTP Proxy Service, which is identified
*  by charIndex from the GATT database.
*
*  \param charIndex: The index of the HTTP Proxy Service characteristic.
*  \param attrSize:  The size of the HTTP Proxy Service characteristic value attribute.
*  \param attrValue: The pointer to the location where characteristic value data
*                    should be stored.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic value was read successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HPSS_GetCharacteristicValue(cy_en_ble_hps_char_index_t charIndex,
                                                          uint16_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((attrValue != NULL) && (charIndex < CY_BLE_HPS_CHAR_COUNT))
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_hpsConfigPtr->hpss->charInfo[charIndex].charHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
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

    /* Return status */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HPSS_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sets the characteristic descriptor value of the specified characteristic.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param descrIndex: The index of the descriptor.
*  \param attrSize:   The size of the characteristic descriptor attribute.
*  \param attrValue:  The pointer to the descriptor value data to be stored in the GATT
*                     database.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HPSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hps_char_index_t charIndex,
                                                               cy_en_ble_hps_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex == CY_BLE_HPS_HTTP_STATUS_CODE) && (descrIndex == CY_BLE_HPS_CCCD) && (attrValue != NULL))
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_STATUS_CODE].
                                           descrHandle[CY_BLE_HPS_CCCD],
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .connHandle                 = connHandle,
            .offset                     = 0u,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };

        /* Sets characteristic value to database */
        if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_SUCCESS;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HPSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Reads a a characteristic descriptor of a specified characteristic of the
*  HTTP Proxy Service from the GATT database.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the characteristic.
*  \param descrIndex: The index of the descriptor.
*  \param attrSize:   The size of the descriptor value.
*  \param attrValue:  The pointer to the location where characteristic descriptor value
*                     data should be stored.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HPSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hps_char_index_t charIndex,
                                                               cy_en_ble_hps_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex == CY_BLE_HPS_HTTP_STATUS_CODE) && (descrIndex == CY_BLE_HPS_CCCD) && (attrValue != NULL))
    {
        if(cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_STATUS_CODE].descrHandle[CY_BLE_HPS_CCCD] !=
           CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_STATUS_CODE].
                                               descrHandle[CY_BLE_HPS_CCCD],
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
* Function Name: Cy_BLE_HPSS_SendNotification
***************************************************************************//**
*
*  Sends a notification with a characteristic value of the HTTP Proxy
*  Service, which is a value specified by charIndex, to the client's device.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_HPSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the client's device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - A notification is not enabled by the client.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is
*                                               absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HPSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_hps_char_index_t charIndex,
                                                    uint8_t attrSize,
                                                    uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if(charIndex < CY_BLE_HPS_CHAR_COUNT)
    {
        if(cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_STATUS_CODE].descrHandle[CY_BLE_HPS_CCCD] !=
           CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            /* Send notification if it is enabled and peer device is connected */
            if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
            {
                apiResult = CY_BLE_ERROR_INVALID_STATE;
            }

            else if(!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId,
                                                    cy_ble_hpsConfigPtr->hpss->
                                                     charInfo[CY_BLE_HPS_HTTP_STATUS_CODE].descrHandle[CY_BLE_HPS_CCCD]))
            {
                apiResult = CY_BLE_ERROR_NTF_DISABLED;
            }
            else
            {
                cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
                {
                    /* Fill all fields of write request structure ... */
                    .handleValPair.attrHandle = cy_ble_hpsConfigPtr->hpss->charInfo[CY_BLE_HPS_HTTP_STATUS_CODE].
                                                 charHandle,
                    .handleValPair.value.val  = attrValue,
                    .handleValPair.value.len  = attrSize,
                    .connHandle               = connHandle
                };

                /* Send notification to client using previously filled structure */
                apiResult = Cy_BLE_GATTS_Notification(&ntfReqParam);
            }
        }
        else
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
    }

    return(apiResult);
}

#endif /* CY_BLE_HPS_SERVER */


#ifdef CY_BLE_HPS_CLIENT

/******************************************************************************
* Function Name: Cy_BLE_HPSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_HPSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* HPS characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_hpscCharUuid[CY_BLE_HPS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_URI,
        CY_BLE_UUID_CHAR_HTTP_HEADERS,
        CY_BLE_UUID_CHAR_HTTP_ENTITY_BODY,
        CY_BLE_UUID_CHAR_HTTP_CP,
        CY_BLE_UUID_CHAR_HTTP_STATUS_CODE,
        CY_BLE_UUID_CHAR_HTTPS_SECURITY
    };
    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_HPS))
    {
        /* Update last characteristic endHandle to declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        /* Search through all available characteristics */
        for(i = (uint32_t)CY_BLE_HPS_URI; (i < (uint32_t)CY_BLE_HPS_CHAR_COUNT); i++)
        {
            if(cy_ble_hpscCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_hpsc[discIdx].charInfo[i].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_hpsc[discIdx].charInfo[i].valueHandle = discCharInfo->valueHandle;
                    cy_ble_hpsc[discIdx].charInfo[i].properties = discCharInfo->properties;
                    lastEndHandle[discIdx] = &cy_ble_hpsc[discIdx].charInfo[i].endHandle;
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
* Function Name: Cy_BLE_HPSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_FIND_INFO_RSP event.
*  Based on the descriptor UUID, an appropriate data structure is populated using
*  the data received as part of the callback.
*
*  \param discDescrInfo: The pointer to a descriptor information structure.
*
******************************************************************************/
static void Cy_BLE_HPSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t notSupportedDescr = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t descIdx;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_HPS)
    {
        switch(discDescrInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_CLIENT_CONFIG:
                descIdx = (uint32_t)CY_BLE_HPS_CCCD;
                break;

            default:
                /* Not supported descriptor */
                notSupportedDescr = 1u;
                break;
        }

        if(notSupportedDescr == 0u)
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if(cy_ble_hpsc[discIdx].charInfo[charIdx].descrHandle[descIdx] == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_hpsc[discIdx].charInfo[charIdx].descrHandle[descIdx] = discDescrInfo->descrHandle;
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
* Function Name: Cy_BLE_HPSC_GetCharRange
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
static void Cy_BLE_HPSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_HPS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_HPS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if((cy_ble_hpsc[discIdx].charInfo[charIdx].endHandle - cy_ble_hpsc[discIdx].charInfo[charIdx].valueHandle) != 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_hpsc[discIdx].charInfo[charIdx].valueHandle + 1u;
                charRangeInfo->range.endHandle = cy_ble_hpsc[discIdx].charInfo[charIdx].endHandle;
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
* Function Name: Cy_BLE_HPSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles a notification event for the HTTP Service.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_HPSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    if(Cy_BLE_HPS_ApplCallback != NULL)
    {
        uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
        if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) &&
           (cy_ble_hpsc[discIdx].charInfo[CY_BLE_HPS_HTTP_STATUS_CODE].valueHandle ==
            eventParam->handleValPair.attrHandle))
        {
            cy_stc_ble_hps_char_value_t notifValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_HPS_HTTP_STATUS_CODE,
                .value      = &eventParam->handleValPair.value
            };

            Cy_BLE_HPS_ApplCallback((uint32_t)CY_BLE_EVT_HPSC_NOTIFICATION, &notifValue);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HPSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles a Read Response event for the HTTP Proxy Service.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_HPSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    cy_en_ble_hps_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_HPS_ApplCallback != NULL) &&
       (cy_ble_hpscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locCharIndex = CY_BLE_HPS_URI; (locCharIndex < CY_BLE_HPS_CHAR_COUNT); locCharIndex++)
        {
            if(cy_ble_hpscReqHandle[discIdx] == cy_ble_hpsc[discIdx].charInfo[locCharIndex].valueHandle)
            {
                break;
            }
        }

        if(locCharIndex < CY_BLE_HPS_CHAR_COUNT)
        {
            cy_stc_ble_hps_char_value_t locCharValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = locCharIndex,
                .value      = &eventParam->value
            };

            cy_ble_hpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_HPS_ApplCallback((uint32_t)CY_BLE_EVT_HPSC_READ_CHAR_RESPONSE, &locCharValue);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else if(cy_ble_hpscReqHandle[discIdx] == cy_ble_hpsc[discIdx].charInfo[CY_BLE_HPS_HTTP_STATUS_CODE].
                 descrHandle[CY_BLE_HPS_CCCD])
        {
            cy_stc_ble_hps_descr_value_t locDescrValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_HPS_HTTP_STATUS_CODE,
                .descrIndex = CY_BLE_HPS_CCCD,
                .value      = &eventParam->value
            };

            cy_ble_hpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_HPS_ApplCallback((uint32_t)CY_BLE_EVT_HPSC_READ_DESCR_RESPONSE, &locDescrValue);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else
        {
            /* Do nothing */
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HPSC_ReadLongRespEventHandler
***************************************************************************//**
*
*  Handles a Read Long Response event for the HTTP Proxy Service.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_HPSC_ReadLongRespEventHandler(const cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    cy_stc_ble_gattc_stop_cmd_param_t stopCmdParam = { .connHandle = eventParam->connHandle };
    cy_stc_ble_gatt_xchg_mtu_param_t mtuParam = { .connHandle = eventParam->connHandle };
    cy_en_ble_hps_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
    uint32_t isReqEnded = 1u;

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_HPS_ApplCallback != NULL))
    {
        /* Check if requested handle equals to any of HPS characteristic handles */
        if(cy_ble_hpscReqHandle[discIdx] == cy_ble_hpsc[discIdx].charInfo[CY_BLE_HPS_URI].valueHandle)
        {
            locCharIndex = CY_BLE_HPS_URI;
        }
        else if(cy_ble_hpscReqHandle[discIdx] == cy_ble_hpsc[discIdx].charInfo[CY_BLE_HPS_HTTP_HEADERS].valueHandle)
        {
            locCharIndex = CY_BLE_HPS_HTTP_HEADERS;
        }
        else if(cy_ble_hpscReqHandle[discIdx] == cy_ble_hpsc[discIdx].charInfo[CY_BLE_HPS_HTTP_ENTITY_BODY].valueHandle)
        {
            locCharIndex = CY_BLE_HPS_HTTP_ENTITY_BODY;
        }
        else
        {
            locCharIndex = CY_BLE_HPS_CHAR_COUNT;
        }

        /* If match was found then proceed with the request handling */
        if((locCharIndex < CY_BLE_HPS_CHAR_COUNT) && (cy_ble_hpscReqHandle[discIdx] !=
                                                      CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
        {
            /* Update user buffer with received data */
            if(cy_ble_hpscCurrLen < cy_ble_hpscRdLongBuffLen)
            {
                (void)memcpy((void*)&cy_ble_hpscRdLongBuffPtr[cy_ble_hpscCurrLen], (void*)&eventParam->value.val[0],
                             (uint32_t)eventParam->value.len);

                cy_ble_hpscCurrLen += eventParam->value.len;
            }

            (void)Cy_BLE_GATT_GetMtuSize(&mtuParam);

            /* If the received data length is less than the MTU size, the Read Long
             * request is completed or the provided user's buffer is full.
             */
            if(((mtuParam.mtu - 1u) > eventParam->value.len))
            {
                cy_ble_hpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
            else if(cy_ble_hpscCurrLen == cy_ble_hpscRdLongBuffLen)
            {
                (void)Cy_BLE_GATTC_StopCmd(&stopCmdParam);
            }
            else
            {
                isReqEnded = 0u;
            }

            /* If the buffer is full, then stop processing any remaining read long
             * requests.
             */
            if(isReqEnded == 1u)
            {
                cy_stc_ble_hps_char_value_t rdCharValue = { .connHandle = eventParam->connHandle };

                cy_stc_ble_gatt_value_t rdValue =
                {
                    .val = cy_ble_hpscRdLongBuffPtr,
                    .len = cy_ble_hpscCurrLen
                };
                rdCharValue.charIndex = locCharIndex;
                rdCharValue.value = &rdValue;

                Cy_BLE_HPS_ApplCallback((uint32_t)CY_BLE_EVT_HPSC_READ_CHAR_RESPONSE, &rdCharValue);
                cy_ble_hpscCurrLen = 0u;
            }

            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HPSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Write Response event for the HTTP Proxy Service.
*
*  \param eventParam: The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_HPSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    cy_en_ble_hps_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_HPS_ApplCallback != NULL) &&
       (cy_ble_hpscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locCharIndex = CY_BLE_HPS_URI; (locCharIndex < CY_BLE_HPS_CHAR_COUNT); locCharIndex++)
        {
            if(cy_ble_hpscReqHandle[discIdx] == cy_ble_hpsc[discIdx].charInfo[locCharIndex].valueHandle)
            {
                break;
            }
        }

        if(locCharIndex < CY_BLE_HPS_CHAR_COUNT)
        {
            cy_stc_ble_hps_char_value_t locCharValue =
            {
                .connHandle = *eventParam,
                .charIndex  = locCharIndex,
                .value      = NULL
            };

            cy_ble_hpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_HPS_ApplCallback((uint32_t)CY_BLE_EVT_HPSC_WRITE_CHAR_RESPONSE, &locCharValue);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else if(cy_ble_hpscReqHandle[discIdx] ==
                cy_ble_hpsc[discIdx].charInfo[CY_BLE_HPS_HTTP_STATUS_CODE].descrHandle[CY_BLE_HPS_CCCD])
        {
            cy_stc_ble_hps_descr_value_t locDescrValue =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_HPS_HTTP_STATUS_CODE,
                .descrIndex = CY_BLE_HPS_CCCD,
                .value      = NULL
            };

            cy_ble_hpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_HPS_ApplCallback((uint32_t)CY_BLE_EVT_HPSC_WRITE_DESCR_RESPONSE, &locDescrValue);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else
        {
            /* The requested handle doesn't belong to HPS */
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HPSC_ExecuteWriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Execute Write Response event for the HTTP Proxy Service.
*
*  \param eventParam: The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_HPSC_ExecuteWriteResponseEventHandler(const cy_stc_ble_gattc_exec_write_rsp_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
    cy_en_ble_hps_char_index_t locCharIndex;

    /* Check if service handler was registered and request handle is valid. */
    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_HPS_ApplCallback != NULL))
    {
        /* Check if requested handle equals to any of HPS characteristic handles */
        if(cy_ble_hpscReqHandle[discIdx] == cy_ble_hpsc[discIdx].charInfo[CY_BLE_HPS_URI].valueHandle)
        {
            locCharIndex = CY_BLE_HPS_URI;
        }
        else if(cy_ble_hpscReqHandle[discIdx] == cy_ble_hpsc[discIdx].charInfo[CY_BLE_HPS_HTTP_HEADERS].valueHandle)
        {
            locCharIndex = CY_BLE_HPS_HTTP_HEADERS;
        }
        else if(cy_ble_hpscReqHandle[discIdx] == cy_ble_hpsc[discIdx].charInfo[CY_BLE_HPS_HTTP_ENTITY_BODY].valueHandle)
        {
            locCharIndex = CY_BLE_HPS_HTTP_ENTITY_BODY;
        }
        else
        {
            locCharIndex = CY_BLE_HPS_CHAR_COUNT;
        }

        if(locCharIndex < CY_BLE_HPS_CHAR_COUNT)
        {
            cy_stc_ble_hps_char_value_t locCharValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = locCharIndex,
                .value      = NULL
            };

            cy_ble_hpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_HPS_ApplCallback((uint32_t)CY_BLE_EVT_HPSC_WRITE_CHAR_RESPONSE, &locCharValue);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HPSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles an Error Response event for the HTTP Proxy Service.
*
*  \param eventParam - The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_HPSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        if(cy_ble_hpscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] == eventParam->errInfo.attrHandle)
        {
            cy_ble_hpscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HPSC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_HPSS_WRITE_CHAR events is generated.
*  On successful request execution on the Server side the Write Response is
*  sent to the Client.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*             sent to the server device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic.
*  * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                    characteristic.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the HPS service-specific callback is registered
*      (with Cy_BLE_HPS_RegisterAttrCallback):
*  * CY_BLE_EVT_HPSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_hps_char_value_t.
*
*   Otherwise (if the HPS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there was some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HPSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_hps_char_index_t charIndex,
                                                          uint16_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_HPS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_hpsc[discIdx].charInfo[charIndex].valueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_hpsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };

        apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);
        /* Save handle to support service-specific write response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_hpscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HPSC_GetCharacteristicValue
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
*   If the HPS service-specific callback is registered
*      (with Cy_BLE_HPS_RegisterAttrCallback): \n
*  * CY_BLE_EVT_HPSC_READ_CHAR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_hps_char_value_t.
*  .
*   Otherwise (if the HPS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_HPSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_hps_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_HPS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_hpsc[discIdx].charInfo[charIndex].valueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_hpsc[discIdx].charInfo[charIndex].valueHandle,
            .connHandle = connHandle
        };

        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_hpscReqHandle[discIdx] = cy_ble_hpsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HPSC_SetLongCharacteristicValue
***************************************************************************//**
*
*  Sends a request to set a long characteristic value of the service, which is
*  a value identified by charIndex, to the server's device.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*                    sent to the server device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic.
*  * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                     characteristic.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the HPS service-specific callback is registered
*      (with Cy_BLE_HPS_RegisterAttrCallback):
*  * CY_BLE_EVT_HPSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_hps_char_value_t.
*
*   Otherwise (if the HPS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_EXEC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HPSC_SetLongCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                              cy_en_ble_hps_char_index_t charIndex,
                                                              uint16_t attrSize,
                                                              uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_HPS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_hpsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_prep_write_req_t writeReqParam =
        {
            .handleValOffsetPair.handleValuePair.attrHandle = cy_ble_hpsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValOffsetPair.handleValuePair.value.val  = attrValue,
            .handleValOffsetPair.handleValuePair.value.len  = attrSize,
            .handleValOffsetPair.offset                     = 0u,
            .connHandle                                     = connHandle
        };

        /* ... and send request to server's device. */
        apiResult = Cy_BLE_GATTC_WriteLongCharacteristicValues(&writeReqParam);

        /* Save handle to support service-specific write response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_hpscReqHandle[discIdx] = writeReqParam.handleValOffsetPair.handleValuePair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HPSC_GetLongCharacteristicValue
***************************************************************************//**
*
*  This function is used to read a long characteristic value, which is a value
*  identified by charIndex, from the server.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic. Starts with zero.
*  \param attrSize:   The size of the buffer to store long characteristic value.
*  \param attrValue:  The pointer to the buffer where the read long characteristic
*                     value should be stored.
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
*   If the HPS service-specific callback is registered
*      (with Cy_BLE_HPS_RegisterAttrCallback):
*  * CY_BLE_EVT_HPSC_READ_CHAR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_hps_char_value_t.
*
*   Otherwise (if the HPS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_BLOB_RSP - If the requested attribute is
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
cy_en_ble_api_result_t Cy_BLE_HPSC_GetLongCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                              cy_en_ble_hps_char_index_t charIndex,
                                                              uint16_t attrSize,
                                                              uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_HPS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_hpsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_read_blob_req_t readLongData =
        {
            .handleOffset.attrHandle = cy_ble_hpsc[discIdx].charInfo[charIndex].valueHandle,
            .handleOffset.offset     = 0u,
            .connHandle              = connHandle
        };

        cy_ble_hpscRdLongBuffLen = attrSize;
        cy_ble_hpscRdLongBuffPtr = attrValue;

        apiResult = Cy_BLE_GATTC_ReadLongCharacteristicValues(&readLongData);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_hpscReqHandle[discIdx] = cy_ble_hpsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HPSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  This function is used to write the characteristic descriptor to the server,
*  which is identified by charIndex and descrIndex.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic. Starts with zero.
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue:  The pointer to the characteristic descriptor value data that
*                     should be sent to the server device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*
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
*   If the HPS service-specific callback is registered
*      (with Cy_BLE_HPS_RegisterAttrCallback):
*  * CY_BLE_EVT_HPSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_hps_char_value_t.
*   Otherwise (if the HPS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - If the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HPSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hps_char_index_t charIndex,
                                                               cy_en_ble_hps_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_HPS_CHAR_COUNT) || (descrIndex >= CY_BLE_HPS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_hpsc[discIdx].charInfo[charIndex].descrHandle[descrIndex] == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_hpsc[discIdx].charInfo[charIndex].descrHandle[descrIndex],
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* ... and send request to server's device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

        /* Save handle to support service-specific write response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_hpscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HPSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Sends a request to get the characteristic descriptor of the specified
*  characteristic of the service.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic. Starts with zero.
*  \param descrIndex: The index of the service characteristic descriptor.
*
* \return
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
*  If the HPS service-specific callback is registered
*      (with Cy_BLE_HPS_RegisterAttrCallback):
*  * CY_BLE_EVT_HPSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully read on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_hps_descr_value_t.
*  .
*  Otherwise (if the HPS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP -  If the requested attribute is
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
cy_en_ble_api_result_t Cy_BLE_HPSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hps_char_index_t charIndex,
                                                               cy_en_ble_hps_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_HPS_CHAR_COUNT) || (descrIndex >= CY_BLE_HPS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_hpsc[discIdx].charInfo[charIndex].descrHandle[descrIndex] == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_hpsc[discIdx].charInfo[charIndex].descrHandle[descrIndex],
            .connHandle = connHandle
        };

        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_hpscReqHandle[discIdx] = cy_ble_hpsc[discIdx].charInfo[charIndex].descrHandle[descrIndex];
        }
    }

    return(apiResult);
}

#endif /* CY_BLE_HPS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_HPS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the HTTP Proxy Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_HPS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_HPS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_HPSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_HPSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_HPSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_HPS_CLIENT */

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
        #ifdef CY_BLE_HPS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_HPSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_PREP_WRITE_REQ:
                Cy_BLE_HPSS_PrepareWriteRequestEventHandler((cy_stc_ble_gatts_prep_write_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_EXEC_WRITE_REQ:
                Cy_BLE_HPSS_ExecuteWriteRequestEventHandler((cy_stc_ble_gatts_exec_write_req_t*)eventParam);
                break;
        #endif /* CY_BLE_HPS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_HPS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);

                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_HPSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_HPSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_HPSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_BLOB_RSP:
                Cy_BLE_HPSC_ReadLongRespEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_EXEC_WRITE_RSP:
                Cy_BLE_HPSC_ExecuteWriteResponseEventHandler((cy_stc_ble_gattc_exec_write_rsp_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_HPSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_HPS_CLIENT */
            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_HPS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
