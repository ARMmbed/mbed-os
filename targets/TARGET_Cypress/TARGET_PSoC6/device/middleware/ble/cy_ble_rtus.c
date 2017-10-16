/***************************************************************************//**
* \file cy_ble_rtus.c
* \version 2.0
*
* \brief
*  Contains the source code for Reference Time Update Service.
*
********************************************************************************
*
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_RTUS))
static cy_ble_callback_t Cy_BLE_RTUS_ApplCallback = NULL;

#if defined(CY_BLE_RTUS_CLIENT)
/* RTUS Center Service characteristics GATT DB handles structure */
cy_stc_ble_rtusc_t cy_ble_rtusc[CY_BLE_CONFIG_GATTC_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_rtuscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_RTUS_CLIENT) */

/* The pointer on global BLE RTUS Config structure */
cy_stc_ble_rtus_config_t *cy_ble_rtusConfigPtr = NULL;


/***************************************
* Private Function Prototypes
***************************************/
#if defined(CY_BLE_RTUS_SERVER)
static void Cy_BLE_RTUSS_WriteCmdEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* CY_BLE_RTUS_SERVER */

#if defined(CY_BLE_RTUS_CLIENT)
static void Cy_BLE_RTUSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_RTUSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_RTUSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
#endif /* (CY_BLE_RTUS_CLIENT) */
static cy_en_ble_gatt_err_code_t Cy_BLE_RTUS_EventHandler(uint32_t eventCode, void *eventParam);


/*******************************************************************************
* Function Name: Cy_BLE_RTUS_Init
****************************************************************************//**
*
*  This function initializes the Reference Time Update Service.
*
*  \param config: Configuration structure for Reference Time Update Service.
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
cy_en_ble_api_result_t Cy_BLE_RTUS_Init(cy_stc_ble_rtus_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_rtusConfigPtr = config;

        /* Registers Event Handler for the RTUS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_RTUS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_rtusConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_RTUS_ApplCallback = cy_ble_rtusConfigPtr->callbackFunc;
        }
    #ifdef CY_BLE_RTUS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_RTUS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_rtusc[idx], 0, sizeof(cy_stc_ble_rtusc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_RTUS].uuid = CY_BLE_UUID_REF_TIME_UPDATE_SERVICE;
                }
                cy_ble_rtuscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_RTUS_CLIENT */
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_RTUS_RegisterAttrCallback
****************************************************************************//**
*
*  Registers a callback function for Reference Time Update Service specific
*  attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc:  An application layer event callback function to receive
*                     events from the BLE Component. The definition of
*                     cy_ble_callback_t for RTUS is: \n
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
cy_en_ble_api_result_t Cy_BLE_RTUS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_RTUS_ApplCallback = callbackFunc;
    if(cy_ble_rtusConfigPtr != NULL)
    {
        cy_ble_rtusConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#if defined(CY_BLE_RTUS_SERVER)

/*******************************************************************************
* Function Name: Cy_BLE_RTUSS_WriteEventHandler
****************************************************************************//**
*
*  Handles the Write Without Response Request Event for the Reference Time Update
*  Service.
*
*  \param cy_stc_ble_gatts_write_cmd_req_param_t * eventParam: The pointer to a data structure
*                                                  specified by the event.
*
*
*******************************************************************************/
static void Cy_BLE_RTUSS_WriteCmdEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    if(Cy_BLE_RTUS_ApplCallback != NULL)
    {
        /* Check if attribute handle is handle of Time Update Control Point
         * Characteristic of Reference Time Update Service. */
        if((cy_ble_rtusConfigPtr->rtuss->timeUpdateCpHandle == eventParam->handleValPair.attrHandle) &&
           (CY_BLE_RTUS_TIME_UPDATE_CP_SIZE == eventParam->handleValPair.value.len) &&
           (CY_BLE_RTUS_CANCEL_REF_UPDATE >= eventParam->handleValPair.value.val[0u]))
        {
            /* Input parameters validation passed, so save Time Update Control Point value */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair = eventParam->handleValPair,
                .connHandle      = eventParam->connHandle,
                .offset          = 0u,
                .flags           = CY_BLE_GATT_DB_PEER_INITIATED
            };
            if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
            {
                cy_stc_ble_rtus_char_value_t wrCmdParam =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = CY_BLE_RTUS_TIME_UPDATE_CONTROL_POINT,
                    .value      = &eventParam->handleValPair.value
                };
                /* Send callback to user if no error occurred while writing Time Update Control
                 * Point.
                 */
                Cy_BLE_RTUS_ApplCallback((uint32_t)CY_BLE_EVT_RTUSS_WRITE_CHAR_CMD, (void*)&wrCmdParam);
                cy_ble_eventHandlerFlag &= (uint8_t)(~CY_BLE_CALLBACK);
            }
        }
        /* As this handler handles Write Without Response request the error response
         * can't be sent for the Client. The erroneous value will be with
         * CY_BLE_EVT_GATTS_WRITE_CMD_REQ event. User will decide how to handle it. */
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_RTUSS_SetCharacteristicValue
****************************************************************************//**
*
*  Sets characteristic value of the Reference Time Update Service, which is
*  identified by charIndex in the local database.
*
*  \param charIndex:  The index of a service characteristic of
*                     type cy_en_ble_rtus_char_index_t.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value
*                     data that should be stored to the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - the request is handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - validation of the input parameters failed
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_RTUSS_SetCharacteristicValue(cy_en_ble_rtus_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_stc_ble_rtus_time_update_state_t tUState;
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
    {
        .handleValuePair.value.len = attrSize,
        .handleValuePair.value.val = attrValue,
        .offset                    = 0u,
        .flags                     = CY_BLE_GATT_DB_LOCALLY_INITIATED
    };

    if((CY_BLE_RTUS_CHAR_COUNT > charIndex))
    {
        if(charIndex == CY_BLE_RTUS_TIME_UPDATE_CONTROL_POINT)
        {
            dbAttrValInfo.handleValuePair.attrHandle = cy_ble_rtusConfigPtr->rtuss->timeUpdateCpHandle;

            if(*attrValue <= CY_BLE_RTUS_CANCEL_REF_UPDATE)
            {
                apiResult = CY_BLE_SUCCESS;
            }
        }
        else
        {
            dbAttrValInfo.handleValuePair.attrHandle = cy_ble_rtusConfigPtr->rtuss->timeUpdateStateHandle;

            tUState = *(cy_stc_ble_rtus_time_update_state_t*)attrValue;

            if((tUState.result <= CY_BLE_RTUS_RESULT_NO_UPDATE_ATTEMPT) &&
               (tUState.currentState <= CY_BLE_RTUS_CURR_STATE_UPDATE_PENDING))
            {
                apiResult = CY_BLE_SUCCESS;
            }
        }
        /* Store data in database */
        if(apiResult == CY_BLE_SUCCESS)
        {
            if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
        }
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_RTUSS_GetCharacteristicValue
****************************************************************************//**
*
*  Gets a characteristic value of the Reference Time Update Service, which
*  is identified by charIndex.
*
*  \param charIndex:  The index of a service characteristic of
*                     type cy_en_ble_rtus_char_index_t.
*
*  \param attrSize:   The size of the characteristic value attribute.
*
*  \param attrValue: The pointer to the location where
*                     characteristic value data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - the request is handled successfully;
*   * CY_BLE_ERROR_INVALID_PARAMETER - validation of the input parameter failed.
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_RTUSS_GetCharacteristicValue(cy_en_ble_rtus_char_index_t charIndex,
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

    if(CY_BLE_RTUS_CHAR_COUNT > charIndex)
    {
        /* Read characteristic value from database */
        if(charIndex == CY_BLE_RTUS_TIME_UPDATE_CONTROL_POINT)
        {
            dbAttrValInfo.handleValuePair.attrHandle = cy_ble_rtusConfigPtr->rtuss->timeUpdateCpHandle;
        }
        else
        {
            dbAttrValInfo.handleValuePair.attrHandle = cy_ble_rtusConfigPtr->rtuss->timeUpdateStateHandle;
        }

        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_SUCCESS;
        }
    }

    return(apiResult);
}

#endif /* (CY_BLE_RTUS_SERVER) */


#if defined(CY_BLE_RTUS_CLIENT)

/*******************************************************************************
* Function Name: Cy_BLE_RTUSC_DiscoverCharacteristicsEventHandler
****************************************************************************//**
*
*  This function is called on receiving a "CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP"
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
*
*******************************************************************************/
static void Cy_BLE_RTUSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* RTUS characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_rtusCharUuid[CY_BLE_RTUS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_TIME_UPDATE_CONTROL_POINT,
        CY_BLE_UUID_CHAR_TIME_UPDATE_STATE
    };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_RTUS))
    {
        for(i = 0u; i < (uint32_t)CY_BLE_RTUS_CHAR_COUNT; i++)
        {
            if(cy_ble_rtusCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_rtusc[discIdx].charInfo[i].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_rtusc[discIdx].charInfo[i].valueHandle = discCharInfo->valueHandle;
                    cy_ble_rtusc[discIdx].charInfo[i].properties = discCharInfo->properties;
                }
                else
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo);
                }
            }
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}



/******************************************************************************
* Function Name: Cy_BLE_RTUSC_GetCharRange
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
static void Cy_BLE_RTUSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_RTUS)
    {
        /* RTUS does not have any discriptions, return CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE */
        charRangeInfo->range.startHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        charRangeInfo->range.endHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_RTUSC_ReadResponseEventHandler
****************************************************************************//**
*
*  Handles Read Response Event for Reference Time Update Service.
*
*  \param eventParam: The pointer to the data that came with a read response for RTUS.
*
*
******************************************************************************/
static void Cy_BLE_RTUSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_RTUS_ApplCallback != NULL) &&
       (cy_ble_rtuscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_rtusc[discIdx].charInfo[CY_BLE_RTUS_TIME_UPDATE_STATE].valueHandle == cy_ble_rtuscReqHandle[discIdx])
        {
            /* Fill Reference Time Update Service read response parameter structure with
             * characteristic info.
             */
            cy_stc_ble_rtus_char_value_t rdRspParam =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_RTUS_TIME_UPDATE_STATE,
                .value      = &eventParam->value
            };
            Cy_BLE_RTUS_ApplCallback((uint32_t)CY_BLE_EVT_RTUSC_READ_CHAR_RESPONSE, (void*)&rdRspParam);

            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            cy_ble_rtuscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_RTUSC_SetCharacteristicValue
****************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_RTUSS_WRITE_CHAR_CMD event is generated.
*
*  \param connHandle:    The connection handle.
*  \param charIndex:     The index of a service characteristic.
*  \param attrSize:      Size of the characteristic value attribute.
*  \param attrValue:    Pointer to the characteristic value data
*                        that should be sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request was sent successfully.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the Client is not established.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic.
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_RTUSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_rtus_char_index_t charIndex,
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
            (charIndex == CY_BLE_RTUS_TIME_UPDATE_CONTROL_POINT) &&
            (cy_ble_rtusc[discIdx].charInfo[CY_BLE_RTUS_TIME_UPDATE_CONTROL_POINT].valueHandle !=
             CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        /* Fill all fields of write command request structure ... */
        cy_stc_ble_gattc_write_cmd_req_t wrReqParam =
        {
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .handleValPair.attrHandle = cy_ble_rtusc[discIdx].charInfo[CY_BLE_RTUS_TIME_UPDATE_CONTROL_POINT].valueHandle,
            .connHandle               = connHandle
        };
        /* Send request to write characteristic value */
        apiResult = Cy_BLE_GATTC_WriteWithoutResponse(&wrReqParam);
    }
    else
    {
        /* apiResult equals to CY_BLE_ERROR_INVALID_PARAMETER */
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_RTUSC_GetCharacteristicValue
****************************************************************************//**
*
*  Sends a request to a peer device to set characteristic value of the Reference
*  Time Update Service, which is identified by charIndex.
*
*  \param connHandle:    The connection handle.
*  \param charIndex:     The index of a service characteristic.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - the request was sent successfully;
*   * CY_BLE_ERROR_INVALID_STATE - connection with the Client is not established.
*   * CY_BLE_ERROR_INVALID_PARAMETER - validation of the input parameters failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the RTUS service-specific callback is registered
*      (with Cy_BLE_RTUS_RegisterAttrCallback):
*  * CY_BLE_EVT_RTUSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_rtus_char_value_t.
*  .
*   Otherwise (if the RTUS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_RTUSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_rtus_char_index_t charIndex)
{
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
        if(charIndex == CY_BLE_RTUS_TIME_UPDATE_STATE)
        {
            cy_stc_ble_gattc_read_req_t readReqParam =
            {
                .attrHandle = cy_ble_rtusc[discIdx].charInfo[CY_BLE_RTUS_TIME_UPDATE_STATE].valueHandle,
                .connHandle = connHandle
            };
            /* Send request to read characteristic value */
            apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);

            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_rtuscReqHandle[discIdx] = cy_ble_rtusc[discIdx].charInfo[CY_BLE_RTUS_TIME_UPDATE_STATE].valueHandle;
            }
        }
    }

    return(apiResult);
}


#endif /* CY_BLE_RTUS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_RTUS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the for Reference Time Update Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_RTUS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_RTUS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_RTUSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_RTUSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_RTUS_CLIENT */

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
        #ifdef CY_BLE_RTUS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_CMD_REQ:
                Cy_BLE_RTUSS_WriteCmdEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;
        #endif /* CY_BLE_RTUS_SERVER */


            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_RTUS_CLIENT
            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_RTUSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;
        #endif /* CY_BLE_RTUS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_RTUS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
