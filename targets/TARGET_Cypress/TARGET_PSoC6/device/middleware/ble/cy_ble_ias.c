/***************************************************************************//**
* \file cy_ble_ias.c
* \version 2.0
*
* \brief
*  This file contains the source code for the Immediate Alert Service of the BLE
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_IAS))

#ifdef CY_BLE_IAS_SERVER
static cy_ble_callback_t Cy_BLE_IAS_ApplCallback = NULL;
#endif /* (CY_BLE_IAS_SERVER) */
    
#ifdef CY_BLE_IAS_CLIENT
/* IAS Center Service characteristics GATT DB handles structure */
cy_stc_ble_iasc_t cy_ble_iasc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_IAS_CLIENT) */

/* The pointer on global BLE IAS Config structure */
cy_stc_ble_ias_config_t *cy_ble_iasConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_IAS_SERVER
static void Cy_BLE_IASS_DisconnectEventHandler(void);
static void Cy_BLE_IASS_WriteCmdEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* (CY_BLE_IAS_SERVER) */

#ifdef CY_BLE_IAS_CLIENT
static void Cy_BLE_IASC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_IASC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
#endif /* (CY_BLE_IAS_CLIENT) */
static cy_en_ble_gatt_err_code_t Cy_BLE_IAS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_IAS_Init
***************************************************************************//**
*
*  This function initializes the Immediate Alert Service.
*
*  \param config: Configuration structure for the Immediate Alert Service.
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
cy_en_ble_api_result_t Cy_BLE_IAS_Init(cy_stc_ble_ias_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_iasConfigPtr = config;

        /* Registers Event Handler for the IAS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_IAS_EventHandler);
        
    #ifdef CY_BLE_IAS_SERVER
        /* Registers a callback function via config structure */
        if(cy_ble_iasConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_IAS_ApplCallback = cy_ble_iasConfigPtr->callbackFunc;
        }
    #endif /* CY_BLE_IAS_SERVER */
    
    #ifdef CY_BLE_IAS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_IAS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_iasc[idx], 0, sizeof(cy_stc_ble_iasc_t));
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_IAS].uuid = CY_BLE_UUID_IMMEDIATE_ALERT_SERVICE;
                }
            }
        }
    #endif /* (CY_BLE_IAS_CLIENT) */
    }

    return(apiResult);
}


#ifdef CY_BLE_IAS_SERVER

/******************************************************************************
* Function Name: Cy_BLE_IAS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers callback function for service-specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc: An application layer event callback function to receive
*                     events from the BLE Component. The definition of
*                     cy_ble_callback_t for IAS Service is: \n
*                     typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                        void *eventParam)
*                     * eventCode indicates the event that triggered this
*                       callback (e.g. CY_BLE_EVT_IASS_NOTIFICATION_ENABLED).
*                     * eventParam contains the parameters corresponding to the
*                       current event. (e.g. pointer to cy_stc_ble_ias_char_value_t
*                       structure that contains details of the characteristic
*                       for which notification enabled event was triggered).
*
* \sideeffect
*  The *eventParams in the callback function should not be used by the
*  application once the callback function execution is finished. Otherwise
*  this data may become corrupted.\n
*  <b>Note</b>:
*  IAS only has events for the GATT server. There are no events for the GATT
*  client since the client sends data without waiting for response. Therefore
*  there is no need to register a callback through Cy_BLE_IAS_RegisterAttrCallback
*  for an IAS GATT client.
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
cy_en_ble_api_result_t Cy_BLE_IAS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_IAS_ApplCallback = callbackFunc;
    if(cy_ble_iasConfigPtr != NULL)
    {
        cy_ble_iasConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_IASS_DisconnectEventHandler
***************************************************************************//**
*
*  Handles the Disconnection Indication Event for the Immediate Alert service.
*
******************************************************************************/
static void Cy_BLE_IASS_DisconnectEventHandler(void)
{
    uint8_t tmpAlertLevel = CY_BLE_NO_ALERT;

    /* Set alert level to "No Alert" per IAS spec */
    cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
    {
        .handleValuePair.attrHandle = cy_ble_iasConfigPtr->iass->alertLevelCharHandle,
        .handleValuePair.value.len  = CY_BLE_IAS_ALERT_LEVEL_SIZE,
        .offset                     = 0u,
        .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
    };

    dbAttrValInfo.handleValuePair.value.val = &tmpAlertLevel;

    (void)Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);
}


/******************************************************************************
* Function Name: Cy_BLE_IASS_WriteCmdEventHandler
***************************************************************************//**
*
*  Handles the Write Without Response Request Event for the Immediate Alert
*  Service.
*
*  \param cy_stc_ble_gatts_write_cmd_req_param_t * eventParam: The pointer to a data structure
*                                                    specified by the event.
*
******************************************************************************/
static void Cy_BLE_IASS_WriteCmdEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    if(Cy_BLE_IAS_ApplCallback != NULL)
    {
        /* Check if attribute handle is handle of Alert Level characteristic of
         * Immediate Alert service. */
        if((cy_ble_iasConfigPtr->iass->alertLevelCharHandle == eventParam->handleValPair.attrHandle) &&
           (eventParam->handleValPair.value.len == CY_BLE_IAS_ALERT_LEVEL_SIZE) &&
           (eventParam->handleValPair.value.val[0u] <= CY_BLE_HIGH_ALERT))
        {
            /* Input parameters validation passed, so save Alert Level */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair = eventParam->handleValPair,
                .connHandle      = eventParam->connHandle,
                .offset          = 0u,
                .flags           = CY_BLE_GATT_DB_PEER_INITIATED
            };
            if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
            {
                cy_stc_ble_ias_char_value_t wrCmdParam =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = CY_BLE_IAS_ALERT_LEVEL,
                    .value      = &eventParam->handleValPair.value
                };
                /* Send callback to user if no error occurred while writing Alert Level */
                Cy_BLE_IAS_ApplCallback((uint32_t)CY_BLE_EVT_IASS_WRITE_CHAR_CMD, (void*)&wrCmdParam);
                cy_ble_eventHandlerFlag &= (uint8_t)(~CY_BLE_CALLBACK);
            }
        }
        /* As this handler handles Write Without Response request the error response
         * can't be sent for the Client. The erroneous value will be with
         * CY_BLE_EVT_GATTS_WRITE_CMD_REQ event. User will decide how to handle it. */
    }
}


/******************************************************************************
* Function Name: Cy_BLE_IASS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets the Alert Level characteristic value of the service, which is identified
*  by charIndex.
*
*  \param charIndex: The index of the Alert Level characteristic.
*  \param attrSize: The size of the Alert Level characteristic value attribute.
*  \param attrValue: The pointer to the location where the Alert Level characteristic
*               value data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic value was read successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_IASS_GetCharacteristicValue(cy_en_ble_ias_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((attrValue != NULL) && (charIndex == CY_BLE_IAS_ALERT_LEVEL) && (attrSize == CY_BLE_IAS_ALERT_LEVEL_SIZE))
    {
        /* Get Alert Level characteristic value from GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_iasConfigPtr->iass->alertLevelCharHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };
        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            /* Indicate success */
            apiResult = CY_BLE_SUCCESS;
        }
    }

    /* Return status */
    return(apiResult);
}

#endif /* (CY_BLE_IAS_SERVER) */


#ifdef CY_BLE_IAS_CLIENT

/******************************************************************************
* Function Name: Cy_BLE_IASC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP event.
*  Based on the service UUID, an appropriate data structure is populated using
*  the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_IASC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_IAS))
    {
        if(discCharInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_ALERT_LEVEL)
        {
            /* Save Alert Level Characteristic handle */
            Cy_BLE_CheckStoreCharHandle(cy_ble_iasc[discIdx].alertLevelChar);
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_IASC_GetCharRange
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
static void Cy_BLE_IASC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_IAS)
    {
        /* IAS does not have any discriptions, return CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE to skip */
        charRangeInfo->range.startHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        charRangeInfo->range.endHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_IASC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_IASS_WRITE_CHAR_CMD event is generated.
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
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                     characteristic
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_IASC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_ias_char_index_t charIndex,
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
            (charIndex == CY_BLE_IAS_ALERT_LEVEL) && (attrSize == CY_BLE_IAS_ALERT_LEVEL_SIZE) &&
            (*attrValue <= CY_BLE_HIGH_ALERT) &&
            (cy_ble_iasc[discIdx].alertLevelChar.valueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        /* Fill all fields of write command request structure ... */
        cy_stc_ble_gattc_write_cmd_req_t wrCmdReq =
        {
            .handleValPair.attrHandle = cy_ble_iasc[discIdx].alertLevelChar.valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* Send request to write Alert Level characteristic value */
        apiResult = Cy_BLE_GATTC_WriteWithoutResponse(&wrCmdReq);
    }
    else
    {
        /* apiResult equals CY_BLE_ERROR_INVALID_PARAMETER */
    }

    /* Return status */
    return(apiResult);
}

#endif /* (CY_BLE_IAS_CLIENT) */

/******************************************************************************
* Function Name: Cy_BLE_IAS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the the Immediate Alert Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_IAS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_IAS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_IASC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_IASC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_IAS_CLIENT */

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
        #ifdef CY_BLE_IAS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_CMD_REQ:
                Cy_BLE_IASS_WriteCmdEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATT_DISCONNECT_IND:
                Cy_BLE_IASS_DisconnectEventHandler();
                break;
        #endif /* CY_BLE_IAS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_IAS_CLIENT
        #endif /* CY_BLE_IAS_CLIENT */
            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_IAS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
