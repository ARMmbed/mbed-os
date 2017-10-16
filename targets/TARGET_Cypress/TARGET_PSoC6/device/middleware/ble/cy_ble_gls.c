/***************************************************************************//**
* \file cy_ble_gls.c
* \version 2.0
*
* \brief
*  This file contains the source code for the Glucose Service of the BLE Component.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_GLS))

static cy_ble_callback_t Cy_BLE_GLS_ApplCallback;

#ifdef CY_BLE_GLS_SERVER
uint8_t cy_ble_glsFlag;
/* The internal storage for the last request handle to check response for server */
static cy_ble_gatt_db_attr_handle_t cy_ble_glssReqHandle;
#endif /* CY_BLE_GLS_SERVER */

#ifdef CY_BLE_GLS_CLIENT
/* Server's Glucose Service characteristics GATT DB handles structure */
cy_stc_ble_glsc_t cy_ble_glsc[CY_BLE_CONFIG_GATTC_COUNT];

/* Internal storage for last request handle to check the response */
static cy_ble_gatt_db_attr_handle_t cy_ble_glscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_GLS_CLIENT */

/* The pointer on global BLE GLS Config structure */
cy_stc_ble_gls_config_t *cy_ble_glsConfigPtr = NULL;


/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_GLS_SERVER
static void Cy_BLE_GLSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static cy_en_ble_gatt_err_code_t Cy_BLE_GLSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* CY_BLE_GLS_SERVER */

#ifdef CY_BLE_GLS_CLIENT
static void Cy_BLE_GLSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_GLSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_GLSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_GLSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_GLSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam);
static void Cy_BLE_GLSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_GLSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_GLSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* CY_BLE_GLS_CLIENT */
static cy_en_ble_gatt_err_code_t Cy_BLE_GLS_EventHandler(uint32_t eventCode, void *eventParam);

/******************************************************************************
* Function Name: Cy_BLE_GLS_Init
***************************************************************************//**
*
*  Initializes the profile internals.
*
*  \param config: Configuration structure for the Glucose Service Service.
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
cy_en_ble_api_result_t Cy_BLE_GLS_Init(cy_stc_ble_gls_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_glsConfigPtr = config;

        /* Registers Event Handler for the GLS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_GLS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_glsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_GLS_ApplCallback = cy_ble_glsConfigPtr->callbackFunc;
        }

        #ifdef CY_BLE_GLS_SERVER
        cy_ble_glsFlag = 0u;
        cy_ble_glssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        #endif /* CY_BLE_GLS_SERVER */
        #ifdef CY_BLE_GLS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_GLS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_glsc[idx], 0, sizeof(cy_stc_ble_glsc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_GLS].uuid = CY_BLE_UUID_GLUCOSE_SERVICE;
                }
                cy_ble_glscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
        #endif /* CY_BLE_GATT_ROLE_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GLS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc:  An application layer event callback function to receive
*                    events from the BLE Component. The definition of
*                    cy_ble_callback_t for Glucose Service is:\n
*                    typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                       void *eventParam)
*                    * eventCode indicates the event that triggered this
*                      callback.
*                    * eventParam contains the parameters corresponding to the
*                      current event.
*
* \sideeffect The *eventParams in the callback function should not be used
*                by the application once the callback function execution is
*                finished. Otherwise this data may become corrupted.
*
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
cy_en_ble_api_result_t Cy_BLE_GLS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_GLS_ApplCallback = callbackFunc;
    if(cy_ble_glsConfigPtr != NULL)
    {
        cy_ble_glsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_GLS_SERVER


/******************************************************************************
* Function Name: Cy_BLE_GLSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets a characteristic value of the service, which is identified by charIndex.
*
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param *attrValue: The pointer to the characteristic value data that should be
*               stored in the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*
* \events
*  None
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GLSS_SetCharacteristicValue(cy_en_ble_gls_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check the parameters */
    if(charIndex >= CY_BLE_GLS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_glsConfigPtr->glss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Store characteristic value into GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_glsConfigPtr->glss->charInfo[charIndex].charHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .offset                     = 0u,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };
        if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
        }
        else
        {
            apiResult = CY_BLE_SUCCESS;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GLSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets a characteristic value of the service, which is identified by charIndex.
*
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param *attrValue: Pointer to the location where Characteristic value data should
*               be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GLSS_GetCharacteristicValue(cy_en_ble_gls_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check the parameters */
    if(charIndex >= CY_BLE_GLS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_glsConfigPtr->glss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get characteristic value from GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_glsConfigPtr->glss->charInfo[charIndex].charHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };
        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
        }
        else
        {
            apiResult = CY_BLE_SUCCESS;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GLSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets the characteristic descriptor of the specified characteristic.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the characteristic.
*  \param descrIndex: The index of the descriptor.
*  \param attrSize:   The size of the descriptor value attribute.
*  \param *attrValue: Pointer to the location where the descriptor value
*              data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional descriptor is absent
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GLSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_gls_char_index_t charIndex,
                                                               cy_en_ble_gls_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check the parameters */
    if((charIndex >= CY_BLE_GLS_CHAR_COUNT) || (descrIndex >= CY_BLE_GLS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_glsConfigPtr->glss->charInfo[charIndex].cccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get data from database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_glsConfigPtr->glss->charInfo[charIndex].cccdHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .connHandle                 = connHandle,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };
        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
        }
        else
        {
            apiResult = CY_BLE_SUCCESS;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GLSS_SendNotification
***************************************************************************//**
*
*  Sends a notification of the specified characteristic to the client device,
*  as defined by the charIndex value.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_GLSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The connection handle which consist of the device ID and ATT
*               connection ID.
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param *attrValue: Pointer to the Characteristic value data that should be sent
*               to Client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the client
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GLSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_gls_char_index_t charIndex,
                                                    uint8_t attrSize,
                                                    uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send Notification if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(charIndex >= CY_BLE_GLS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_glsConfigPtr->glss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if(!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_glsConfigPtr->glss->charInfo[charIndex].cccdHandle))
    {
        apiResult = CY_BLE_ERROR_NTF_DISABLED;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
        {
            .handleValPair.attrHandle = cy_ble_glsConfigPtr->glss->charInfo[charIndex].charHandle,
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
* Function Name: Cy_BLE_GLSS_SendIndication
***************************************************************************//**
*
*  Sends an indication of the specified characteristic to the client device, as
*  defined by the charIndex value.
*
*  On enabling indication successfully it sends out a 'Handle Value Indication' which
*  results in CY_BLE_EVT_GLSC_INDICATION or CY_BLE_EVT_GATTC_HANDLE_VALUE_IND (if
*  service-specific callback function is not registered) event at the GATT Client's end.
*
*  \param connHandle: The connection handle which consist of the device ID and ATT
*               connection ID.
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param *attrValue: Pointer to the Characteristic value data that should be sent
*               to Client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*   * CY_BLE_ERROR_IND_DISABLED - Indication is not enabled by the client
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the GLS service-specific callback is registered
*      (with Cy_BLE_GLS_RegisterAttrCallback):
*  * CY_BLE_EVT_GLSS_INDICATION_CONFIRMED - in case if the indication is
*                                successfully delivered to the peer device.
*  .
*   Otherwise (if the GLS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF - in case if the indication is
*                                successfully delivered to the peer device.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GLSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_gls_char_index_t charIndex,
                                                  uint8_t attrSize,
                                                  uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send Indication if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(charIndex >= CY_BLE_GLS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_glsConfigPtr->glss->charInfo[charIndex].cccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if(!CY_BLE_IS_INDICATION_ENABLED(connHandle.attId, cy_ble_glsConfigPtr->glss->charInfo[charIndex].cccdHandle))
    {
        apiResult = CY_BLE_ERROR_IND_DISABLED;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gatts_handle_value_ind_t indParam =
        {
            .handleValPair.attrHandle = cy_ble_glsConfigPtr->glss->charInfo[charIndex].charHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* Send notification to client using previously filled structure */
        apiResult = Cy_BLE_GATTS_Indication(&indParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            /* Save handle to support service-specific value confirmation response from client */
            cy_ble_glssReqHandle = indParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GLSS_ConfirmationEventHandler
***************************************************************************//**
*
*  Handles the Value Confirmation request event from the BLE stack.
*
*  \param *eventParam: Pointer to a structure of type cy_stc_ble_conn_handle_t
*
******************************************************************************/
static void Cy_BLE_GLSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    if((Cy_BLE_GLS_ApplCallback != NULL) &&
       (cy_ble_glsConfigPtr->glss->charInfo[CY_BLE_GLS_RACP].charHandle == cy_ble_glssReqHandle))
    {
        cy_stc_ble_gls_char_value_t locCharIndex =
        {
            .connHandle = *eventParam,
            .charIndex  = CY_BLE_GLS_RACP,
            .value      = NULL
        };

        cy_ble_glssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        cy_ble_glsFlag &= (uint8_t) ~CY_BLE_GLS_FLAG_PROCESS;

        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        Cy_BLE_GLS_ApplCallback((uint32_t)CY_BLE_EVT_GLSS_INDICATION_CONFIRMED, &locCharIndex);
    }
}


/******************************************************************************
* Function Name: Cy_BLE_GLSS_WriteEventHandler
***************************************************************************//**
*
*  Handles the Write Request Event.
*
*  \param *eventParam: The pointer to the data structure specified by the event.
*
* \return
*  cy_en_ble_gatt_err_code_t - A function result state if it succeeded
*  (CY_BLE_GATT_ERR_NONE) or failed with error codes:
*   * CY_BLE_GATTS_ERR_PROCEDURE_ALREADY_IN_PROGRESS
*   * CY_BLE_GATTS_ERR_CCCD_IMPROPERLY_CONFIGURED
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_GLSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    cy_stc_ble_gls_char_value_t locCharValue = { .connHandle = eventParam->connHandle };

    if(Cy_BLE_GLS_ApplCallback != NULL)
    {
        if(eventParam->handleValPair.attrHandle == cy_ble_glsConfigPtr->glss->charInfo[CY_BLE_GLS_RACP].charHandle)
        {
            if(CY_BLE_GLS_IS_PROCEDURE_IN_PROGRESS
               && (CY_BLE_GLS_RACP_OPCODE_ABORT != eventParam->handleValPair.value.val[0u]))
            {
                gattErr = CY_BLE_GATTS_ERR_PROCEDURE_ALREADY_IN_PROGRESS;
            }
            else if(!CY_BLE_IS_INDICATION_ENABLED(eventParam->connHandle.attId,
                                                  cy_ble_glsConfigPtr->glss->charInfo[CY_BLE_GLS_RACP].cccdHandle))
            {
                gattErr = CY_BLE_GATTS_ERR_CCCD_IMPROPERLY_CONFIGURED;
            }
            else
            {
                cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                {
                    .handleValuePair = eventParam->handleValPair,
                    .connHandle      = eventParam->connHandle,
                    .offset          = 0u,
                    .flags           = CY_BLE_GATT_DB_PEER_INITIATED
                };

                locCharValue.charIndex = CY_BLE_GLS_RACP;
                locCharValue.value = &eventParam->handleValPair.value;

                gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);
                if(gattErr == CY_BLE_GATT_ERR_NONE)
                {
                    Cy_BLE_GLS_ApplCallback((uint32_t)CY_BLE_EVT_GLSS_WRITE_CHAR, &locCharValue);
                    cy_ble_glsFlag |= CY_BLE_GLS_FLAG_PROCESS;
                }
            }

            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else
        {
            for(locCharValue.charIndex = CY_BLE_GLS_GLMT;
                locCharValue.charIndex < CY_BLE_GLS_CHAR_COUNT; locCharValue.charIndex++)
            {
                if(eventParam->handleValPair.attrHandle ==
                   cy_ble_glsConfigPtr->glss->charInfo[locCharValue.charIndex].cccdHandle)
                {
                    cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                    {
                        .handleValuePair = eventParam->handleValPair,
                        .connHandle      = eventParam->connHandle,
                        .offset          = 0u,
                        .flags           = CY_BLE_GATT_DB_PEER_INITIATED
                    };
                    gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);

                    locCharValue.value = NULL;
                    if(gattErr == CY_BLE_GATT_ERR_NONE)
                    {
                        if(locCharValue.charIndex < CY_BLE_GLS_GLFT)
                        {
                            if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                            {
                                Cy_BLE_GLS_ApplCallback((uint32_t)CY_BLE_EVT_GLSS_NOTIFICATION_ENABLED, &locCharValue);
                            }
                            else
                            {
                                Cy_BLE_GLS_ApplCallback((uint32_t)CY_BLE_EVT_GLSS_NOTIFICATION_DISABLED, &locCharValue);
                            }
                        }
                        else
                        {
                            if(CY_BLE_IS_INDICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                            {
                                Cy_BLE_GLS_ApplCallback((uint32_t)CY_BLE_EVT_GLSS_INDICATION_ENABLED, &locCharValue);
                            }
                            else
                            {
                                Cy_BLE_GLS_ApplCallback((uint32_t)CY_BLE_EVT_GLSS_INDICATION_DISABLED, &locCharValue);
                            }
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

                    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                    break;
                }
            }
        }
    }

    return(gattErr);
}

#endif /* CY_BLE_GLS_SERVER */

#ifdef CY_BLE_GLS_CLIENT


/******************************************************************************
* Function Name: Cy_BLE_GLSC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_GLSS_WRITE_CHAR events is generated.
*  On successful request execution on the Server side the Write Response is
*  sent to the Client.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param *attrValue: The pointer to the characteristic value data that should be
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
*   If the GLS service-specific callback is registered
*      (with Cy_BLE_GLS_RegisterAttrCallback):
*  * CY_BLE_EVT_GLSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_gls_char_value_t.
*  .
*   Otherwise (if the GLS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GLSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_gls_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check the parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_GLS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_glsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_WRITE & cy_ble_glsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_glsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle,
        };
        apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_glscReqHandle[discIdx] = cy_ble_glsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GLSC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read the characteristic Value from a server
*  which is identified by charIndex.
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
*   If the GLS service-specific callback is registered
*      (with Cy_BLE_GLS_RegisterAttrCallback):
*  * CY_BLE_EVT_GLSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_gls_char_value_t.
*  .
*   Otherwise (if the GLS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_GLSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_gls_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check the parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_GLS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_glsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_READ & cy_ble_glsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_glsc[discIdx].charInfo[charIndex].valueHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_glscReqHandle[discIdx] = cy_ble_glsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GLSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sets the Characteristic Descriptor of the specified Characteristic.
*
*  Internally, Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server side the following events can be
*  generated:
*  * CY_BLE_EVT_GLSS_INDICATION_ENABLED
*  * CY_BLE_EVT_GLSS_INDICATION_DISABLED
*  * CY_BLE_EVT_GLSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_GLSS_NOTIFICATION_DISABLED
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a service characteristic.
*  \param descrIndex: The index of a service characteristic descriptor.
*  \param attrSize: The size of the characteristic descriptor value attribute.
*  \param *attrValue: Pointer to the characteristic descriptor value data that should
*               be sent to the server device.
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
*   If the GLS service-specific callback is registered
*      (with Cy_BLE_GLS_RegisterAttrCallback):
*  * CY_BLE_EVT_GLSC_WRITE_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_gls_descr_value_t.
*  .
*   Otherwise (if the GLS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GLSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_gls_char_index_t charIndex,
                                                               cy_en_ble_gls_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check the parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_GLS_CHAR_COUNT) || (descrIndex >= CY_BLE_GLS_DESCR_COUNT) ||
            (attrSize != CY_BLE_CCCD_LEN) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_glsc[discIdx].charInfo[charIndex].cccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_glsc[discIdx].charInfo[charIndex].cccdHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = CY_BLE_CCCD_LEN,
            .connHandle               = connHandle
        };
        /* ... and send request to server device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            /* Save handle to support service-specific read response from device */
            cy_ble_glscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GLSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets the characteristic descriptor of the specified characteristic.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a service characteristic.
*  \param descrIndex: The index of the service characteristic descriptor.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular descriptor
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the GLS service-specific callback is registered
*      (with Cy_BLE_GLS_RegisterAttrCallback):
*  * CY_BLE_EVT_GLSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_gls_descr_value_t.
*  .
*  Otherwise (if the GLS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_GLSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_gls_char_index_t charIndex,
                                                               cy_en_ble_gls_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check the parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_GLS_CHAR_COUNT) || (descrIndex >= CY_BLE_GLS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_glsc[discIdx].charInfo[charIndex].cccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_glsc[discIdx].charInfo[charIndex].cccdHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_glscReqHandle[discIdx] = cy_ble_glsc[discIdx].charInfo[charIndex].cccdHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GLSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_GLSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* Glucose Service characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_glscCharUuid[CY_BLE_GLS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_GLUCOSE_MSRMNT,
        CY_BLE_UUID_CHAR_GL_MSRMT_CTX,
        CY_BLE_UUID_CHAR_GL_FEATURE,
        CY_BLE_UUID_CHAR_RACP
    };
    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_GLS))
    {
        /* Update last characteristic endHandle to declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        for(i = 0u; i < (uint32_t)CY_BLE_GLS_CHAR_COUNT; i++)
        {
            if(cy_ble_glscCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_glsc[discIdx].charInfo[i].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_glsc[discIdx].charInfo[i].valueHandle = discCharInfo->valueHandle;
                    cy_ble_glsc[discIdx].charInfo[i].properties = discCharInfo->properties;
                    lastEndHandle[discIdx] = &cy_ble_glsc[discIdx].charInfo[i].endHandle;
                }
                else
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo);
                }
            }
        }

        /* Init characteristic endHandle to Service endHandle.
         * Characteristic endHandle will be updated to the declaration
         * Handler of the following characteristic,
         * in the following characteristic discovery procedure. */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = cy_ble_serverInfo[discIdx][CY_BLE_SRVI_GLS].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_GLSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param *discDescrInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_GLSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_GLS)
    {
        if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_CLIENT_CONFIG)
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if(cy_ble_glsc[discIdx].charInfo[charIdx].cccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_glsc[discIdx].charInfo[charIdx].cccdHandle = discDescrInfo->descrHandle;
            }
            else    /* Duplication of descriptor */
            {
                Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_DESCR_DUPLICATION, &discDescrInfo);
            }
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_GLSC_GetCharRange
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
static void Cy_BLE_GLSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_GLS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_GLS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if((cy_ble_glsc[discIdx].charInfo[charIdx].endHandle - cy_ble_glsc[discIdx].charInfo[charIdx].valueHandle) != 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_glsc[discIdx].charInfo[charIdx].valueHandle + 1u;
                charRangeInfo->range.endHandle = cy_ble_glsc[discIdx].charInfo[charIdx].endHandle;
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
* Function Name: Cy_BLE_GLSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles the Notification Event.
*
*  \param *eventParam: The pointer to a data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_GLSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_GLS_ApplCallback != NULL))
    {
        cy_stc_ble_gls_char_value_t locCharValue = { .connHandle = eventParam->connHandle };

        if(cy_ble_glsc[discIdx].charInfo[CY_BLE_GLS_GLMT].valueHandle == eventParam->handleValPair.attrHandle)
        {
            locCharValue.charIndex = CY_BLE_GLS_GLMT;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else if(cy_ble_glsc[discIdx].charInfo[CY_BLE_GLS_GLMC].valueHandle == eventParam->handleValPair.attrHandle)
        {
            locCharValue.charIndex = CY_BLE_GLS_GLMC;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else
        {
            /* Glucose Service doesn't support any other notifications */
        }

        if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) == 0u)
        {
            locCharValue.value = &eventParam->handleValPair.value;
            Cy_BLE_GLS_ApplCallback((uint32_t)CY_BLE_EVT_GLSC_NOTIFICATION, &locCharValue);
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_GLSC_IndicationEventHandler
***************************************************************************//**
*
*  Handles the Indication Event.
*
*  \param *eventParam:  the pointer to a data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_GLSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_GLS_ApplCallback != NULL))
    {
        if(cy_ble_glsc[discIdx].charInfo[CY_BLE_GLS_RACP].valueHandle == eventParam->handleValPair.attrHandle)
        {
            cy_stc_ble_gls_char_value_t locCharValue =
            {
                .charIndex  = CY_BLE_GLS_RACP,
                .connHandle = eventParam->connHandle,
                .value      = &eventParam->handleValPair.value
            };

            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_GLS_ApplCallback((uint32_t)CY_BLE_EVT_GLSC_INDICATION, &locCharValue);
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_GLSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles the Read Response Event.
*
*  \param *eventParam: The pointer to the data structure.
*
******************************************************************************/
static void Cy_BLE_GLSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_GLS_ApplCallback != NULL) &&
       (cy_ble_glscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_glsc[discIdx].charInfo[CY_BLE_GLS_GLFT].valueHandle == cy_ble_glscReqHandle[discIdx])
        {
            cy_stc_ble_gls_char_value_t locCharValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_GLS_GLFT,
                .value      = &eventParam->value
            };
            cy_ble_glscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_GLS_ApplCallback((uint32_t)CY_BLE_EVT_GLSC_READ_CHAR_RESPONSE, &locCharValue);
        }
        else
        {
            cy_en_ble_gls_char_index_t i;

            for(i = CY_BLE_GLS_GLMT; i < CY_BLE_GLS_CHAR_COUNT; i++)
            {
                if(cy_ble_glsc[discIdx].charInfo[i].cccdHandle == cy_ble_glscReqHandle[discIdx])
                {
                    cy_stc_ble_gls_descr_value_t locDescrValue =
                    {
                        .connHandle = eventParam->connHandle,
                        .charIndex  = i,
                        .descrIndex = CY_BLE_GLS_CCCD,
                        .value      = &eventParam->value
                    };
                    cy_ble_glscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                    Cy_BLE_GLS_ApplCallback((uint32_t)CY_BLE_EVT_GLSC_READ_DESCR_RESPONSE, &locDescrValue);
                    break;
                }
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_GLSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles the Write Response Event.
*
*  \param *eventParam: The pointer to a data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_GLSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_GLS_ApplCallback != NULL) &&
       (cy_ble_glscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_glsc[discIdx].charInfo[CY_BLE_GLS_RACP].valueHandle == cy_ble_glscReqHandle[discIdx])
        {
            cy_stc_ble_gls_char_value_t locCharIndex =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_GLS_RACP,
                .value      = NULL
            };
            cy_ble_glscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_GLS_ApplCallback((uint32_t)CY_BLE_EVT_GLSC_WRITE_CHAR_RESPONSE, &locCharIndex);
        }
        else
        {
            cy_en_ble_gls_char_index_t i;

            for(i = CY_BLE_GLS_GLMT; i < CY_BLE_GLS_CHAR_COUNT; i++)
            {
                if(cy_ble_glsc[discIdx].charInfo[i].cccdHandle == cy_ble_glscReqHandle[discIdx])
                {
                    cy_stc_ble_gls_descr_value_t locDescIndex =
                    {
                        .connHandle = *eventParam,
                        .charIndex  = i,
                        .descrIndex = CY_BLE_GLS_CCCD,
                        .value      = NULL
                    };
                    cy_ble_glscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                    Cy_BLE_GLS_ApplCallback((uint32_t)CY_BLE_EVT_GLSC_WRITE_DESCR_RESPONSE, &locDescIndex);
                    break;
                }
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_GLSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles the Error Response Event.
*
*  \param *eventParam: Pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_GLSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
        if(eventParam->errInfo.attrHandle == cy_ble_glscReqHandle[discIdx])
        {
            cy_ble_glscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


#endif /* CY_BLE_GLS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_GLS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Glucose Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_GLS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_GLS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_GLSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_GLSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_GLSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_GLS_CLIENT */

            default:
                break;
        }
    }
    else
    {
        switch((cy_en_ble_event_t)eventCode)
        {
            case CY_BLE_EVT_GATT_DISCONNECT_IND:
                (void)Cy_BLE_GLS_Init(cy_ble_glsConfigPtr);
                break;

                /**************************************************************************
                 * Handling GATT Server Events
                 ***************************************************************************/
        #ifdef CY_BLE_GLS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_GLSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF:
                Cy_BLE_GLSS_ConfirmationEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_GLS_SERVER */

                /**************************************************************************
                 * Handling GATT Client Events
                 ***************************************************************************/
        #ifdef CY_BLE_GLS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_GLSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_GLSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_IND:
                Cy_BLE_GLSC_IndicationEventHandler((cy_stc_ble_gattc_handle_value_ind_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_GLSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_GLSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_GLS_CLIENT */
            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_GLS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
