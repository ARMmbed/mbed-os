/***************************************************************************//**
* \file cy_ble_lls.c
* \version 2.0
*
* \brief
*  This file contains the source code for the Link Loss Service of the BLE
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_LLS))
static cy_ble_callback_t Cy_BLE_LLS_ApplCallback = NULL;

#ifdef CY_BLE_LLS_CLIENT
/* LLS Center Service characteristics GATT DB handles structure */
cy_stc_ble_llsc_t cy_ble_llsc[CY_BLE_CONFIG_GATTC_COUNT];

/* Internal storage for last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_llscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_LLS_CLIENT) */

/* The pointer on global BLE LLS Config structure */
cy_stc_ble_lls_config_t *cy_ble_llsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_LLS_SERVER
static void Cy_BLE_LLSS_ConnectEventHandler(void);
static cy_en_ble_gatt_err_code_t Cy_BLE_LLSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* (CY_BLE_LLS_SERVER) */

#ifdef CY_BLE_LLS_CLIENT
static void Cy_BLE_LLSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_LLSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_LLSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_LLSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_LLSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* (CY_BLE_LLS_CLIENT) */
static cy_en_ble_gatt_err_code_t Cy_BLE_LLS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_LLS_Init
***************************************************************************//**
*
*  This function initializes the Link Loss Service.
*
*  \param config: Configuration structure for the Link Loss Service.
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
cy_en_ble_api_result_t Cy_BLE_LLS_Init(cy_stc_ble_lls_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_llsConfigPtr = config;

        /* Registers Event Handler for the LLS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_LLS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_llsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_LLS_ApplCallback = cy_ble_llsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_LLS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_LLS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_llsc[idx].alertLevelChar.valueHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_LLS].uuid = CY_BLE_UUID_LINK_LOSS_SERVICE;
                }
                cy_ble_llscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* (CY_BLE_LLS_CLIENT) */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_LLS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc: An application layer event callback function to receive
*                    events from the BLE Component. The definition of
*                    cy_ble_callback_t for Link Loss Service is: \n
*                    typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                       void *eventParam)
*                    * eventCode indicates the event that triggered this
*                      callback (e.g. CY_BLE_EVT_LLSS_NOTIFICATION_ENABLED).
*                    * eventParam contains the parameters corresponding to the
*                      current event. (e.g. pointer to cy_stc_ble_lls_char_value_t
*                      structure that contains details of the characteristic
*                      for which notification enabled event was triggered).
*
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
cy_en_ble_api_result_t Cy_BLE_LLS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_LLS_ApplCallback = callbackFunc;
    if(cy_ble_llsConfigPtr != NULL)
    {
        cy_ble_llsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_LLS_SERVER

/******************************************************************************
* Function Name: Cy_BLE_LLSS_ConnectEventHandler
***************************************************************************//**
*
*  Handles the Connection Indication event for Link Loss Service.
*
******************************************************************************/
static void Cy_BLE_LLSS_ConnectEventHandler(void)
{
    uint8_t tmpAlertLevel = CY_BLE_NO_ALERT;

    /* Input parameters validation passed, so save Alert Level */
    cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
    {
        .handleValuePair.attrHandle = cy_ble_llsConfigPtr->llss->alertLevelCharHandle,
        .handleValuePair.value.len  = CY_BLE_LLS_ALERT_LEVEL_SIZE,
        .offset                     = 0u,
        .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
    };

    dbAttrValInfo.handleValuePair.value.val = &tmpAlertLevel;
    (void)Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);
}


/******************************************************************************
* Function Name: Cy_BLE_LLSS_WriteEventHandler
***************************************************************************//**
*
*  Handles the Write Request Event for the Link Loss service.
*
*  \param cy_stc_ble_gatts_write_cmd_req_param_t * eventParam:  the pointer to the data structure
*                                               specified by the event.
*
* \return
*  cy_en_ble_gatt_err_code_t: Function result returns one of the following status values:
*   * CY_BLE_GATT_ERR_NONE - The Alert Level Characteristic was written successfully
*   * CY_BLE_GATT_ERR_UNLIKELY_ERROR - An Internal Stack error occurred
*   * CY_BLE_GATT_ERR_INVALID_ATTRIBUTE_LEN - The Data length for the Alert Level
*                                             Characteristic handle is invalid
*   * CY_BLE_GATT_ERR_INVALID_HANDLE - Alert Level Characteristic handle is invalid
*   * CY_BLE_GATT_ERR_OUT_OF_RANGE - Value is out of allowed range
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_LLSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;

    if(Cy_BLE_LLS_ApplCallback != NULL)
    {
        if(cy_ble_llsConfigPtr->llss->alertLevelCharHandle == eventParam->handleValPair.attrHandle)
        {
            /* Check if attribute handle is handle of Alert Level characteristic of
             * Link Loss service. */
            if((CY_BLE_HIGH_ALERT >= eventParam->handleValPair.value.val[0u]))
            {
                if(eventParam->handleValPair.value.len == CY_BLE_LLS_ALERT_LEVEL_SIZE)
                {
                    /* Input parameters validation passed, so save Alert Level */
                    cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                    {
                        .handleValuePair = eventParam->handleValPair,
                        .connHandle      = eventParam->connHandle,
                        .offset          = 0u,
                        .flags           = CY_BLE_GATT_DB_PEER_INITIATED,
                    };
                    gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);

                    /* Send callback to user if no error occurred while writing Alert Level */
                    if(gattErr == CY_BLE_GATT_ERR_NONE)
                    {
                        cy_stc_ble_lls_char_value_t wrReqParam =
                        {
                            .connHandle = eventParam->connHandle,
                            .charIndex  = CY_BLE_LLS_ALERT_LEVEL,
                            .value      = &eventParam->handleValPair.value
                        };
                        Cy_BLE_LLS_ApplCallback((uint32_t)CY_BLE_EVT_LLSS_WRITE_CHAR_REQ, (void*)&wrReqParam);
                    }
                }
                else
                {
                    gattErr = CY_BLE_GATT_ERR_INVALID_ATTRIBUTE_LEN;
                }
            }
            else
            {
                gattErr = CY_BLE_GATT_ERR_OUT_OF_RANGE;
            }

            /* Indicate that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }

    return(gattErr);
}


/******************************************************************************
* Function Name: Cy_BLE_LLSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets an Alert Level characteristic value of the service, which is identified
*  by charIndex.
*
*  \param charIndex: The index of an Alert Level characteristic.
*  \param attrSize: The size of the Alert Level characteristic value attribute.
*  \param attrValue: The pointer to the location where an Alert Level characteristic
*               value data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic value was read successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_LLSS_GetCharacteristicValue(cy_en_ble_lls_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((attrValue != NULL) && (charIndex == CY_BLE_LLS_ALERT_LEVEL) && (attrSize == CY_BLE_LLS_ALERT_LEVEL_SIZE))
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_llsConfigPtr->llss->alertLevelCharHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };
        /* Get Alert Level characteristic value from GATT database */
        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            /* Indicate success */
            apiResult = CY_BLE_SUCCESS;
        }
    }

    /* Return status */
    return(apiResult);
}

#endif /* (CY_BLE_LLS_SERVER) */


#ifdef CY_BLE_LLS_CLIENT

/******************************************************************************
* Function Name: Cy_BLE_LLSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_LLSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_LLS))
    {
        if(discCharInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_ALERT_LEVEL)
        {
            /* Save Alert Level Characteristic handle */
            Cy_BLE_CheckStoreCharHandle(cy_ble_llsc[discIdx].alertLevelChar);
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_LLSC_GetCharRange
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
static void Cy_BLE_LLSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_LLS)
    {
        /* LLS does not have any discriptions, return CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE */
        charRangeInfo->range.startHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        charRangeInfo->range.endHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_LLSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles the Read Response Event.
*
*  \param cy_stc_ble_gattc_read_rsp_param_t *eventParam: The pointer to the data structure.
*
******************************************************************************/
static void Cy_BLE_LLSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_LLS_ApplCallback != NULL) &&
       (cy_ble_llscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        /* Fill Link Loss Service read response parameter structure */
        cy_stc_ble_lls_char_value_t rdRspParam =
        {
            .connHandle = eventParam->connHandle,
            .charIndex  = CY_BLE_LLS_ALERT_LEVEL,
            .value      = &eventParam->value
        };
        cy_ble_llscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        Cy_BLE_LLS_ApplCallback((uint32_t)CY_BLE_EVT_LLSC_READ_CHAR_RESPONSE, &rdRspParam);
    }
}


/******************************************************************************
* Function Name: Cy_BLE_LLSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles the Write Response Event for the Link Loss Service.
*
*  \param cy_stc_ble_conn_handle_t *eventParam: The pointer to the connection handle.
*
******************************************************************************/
static void Cy_BLE_LLSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_LLS_ApplCallback != NULL) &&
       (cy_ble_llsc[discIdx].alertLevelChar.valueHandle == cy_ble_llscReqHandle[discIdx]) &&
       (cy_ble_llscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        cy_stc_ble_lls_char_value_t wrRspParam =
        {
            .connHandle = *eventParam,
            .charIndex  = CY_BLE_LLS_ALERT_LEVEL,
            .value      = NULL
        };
        cy_ble_llscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        Cy_BLE_LLS_ApplCallback((uint32_t)CY_BLE_EVT_LLSC_WRITE_CHAR_RESPONSE, (void*)&wrRspParam);
    }
}


/******************************************************************************
* Function Name: Cy_BLE_LLSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles the Error Response Event for the Link Loss Service.
*
*  \param  *eventParam: Pointer to the cy_stc_ble_gatt_err_param_t structure.
*
******************************************************************************/
static void Cy_BLE_LLSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if(cy_ble_llscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        cy_ble_llscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_LLSC_SetCharacteristicValue
***************************************************************************//**
*
*  Sets the Alert Level characteristic value of the Link Loss Service, which is
*  identified by charIndex. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_LLSS_WRITE_CHAR_REQ event is generated.
*  On successful request execution on the Server side the Write Response is
*  sent to the Client.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the Alert Level service characteristic.
*  \param attrSize: The size of the Alert Level characteristic value attribute.
*  \param attrValue: The pointer to the Alert Level characteristic value data that
*               should be sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request was sent successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the LLS service-specific callback is registered
*      (with Cy_BLE_LLS_RegisterAttrCallback):
*  * CY_BLE_EVT_LLSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_lls_char_value_t.
*  .
*   Otherwise (if the LLS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_LLSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_lls_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (attrValue != NULL) && (charIndex == CY_BLE_LLS_ALERT_LEVEL) &&
       (attrSize == CY_BLE_LLS_ALERT_LEVEL_SIZE) && (*attrValue <= CY_BLE_HIGH_ALERT) &&
       (cy_ble_llsc[discIdx].alertLevelChar.valueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        /* Fill all fields of write command request structure ... */
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_llsc[discIdx].alertLevelChar.valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* Send request to write Alert Level characteristic value */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);
        cy_ble_llscReqHandle[discIdx] = cy_ble_llsc[discIdx].alertLevelChar.valueHandle;
    }

    /* Return status */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_LLSC_GetCharacteristicValue
***************************************************************************//**
*
*  Sends a request to get characteristic value of the Link Loss Service, which
*  is identified by charIndex.
*
*  This function call can result in generation of the following events based on
*  the response from the server device:
*  * CY_BLE_EVT_LLSC_READ_CHAR_RESPONSE
*  * CY_BLE_EVT_GATTC_ERROR_RSP
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the Link Loss Service characteristic.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request was sent successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the LLS service-specific callback is registered
*      (with Cy_BLE_LLS_RegisterAttrCallback):
*  * CY_BLE_EVT_LLSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_lls_char_value_t.
*  .
*   Otherwise (if the LLS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_LLSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_lls_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex == CY_BLE_LLS_ALERT_LEVEL) && (discIdx < CY_BLE_CONFIG_GATTC_COUNT))
    {
        /* Send request to write Alert Level characteristic value */
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_llscReqHandle[discIdx],
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);
        cy_ble_llscReqHandle[discIdx] = cy_ble_llsc[discIdx].alertLevelChar.valueHandle;
    }
    else
    {
        /* apiResult equals CY_BLE_ERROR_INVALID_PARAMETER */
    }

    /* Return status */
    return(apiResult);
}

#endif /* (CY_BLE_LLS_CLIENT) */


/******************************************************************************
* Function Name: Cy_BLE_LLS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Link Loss Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_LLS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_LLS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_LLSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_LLSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_LLS_CLIENT */

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
        #ifdef CY_BLE_LLS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_LLSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;


            case CY_BLE_EVT_GATT_CONNECT_IND:
                Cy_BLE_LLSS_ConnectEventHandler();
                break;
        #endif /* CY_BLE_LLS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_LLS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_LLSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_LLSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_LLSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_LLS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_LLS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
