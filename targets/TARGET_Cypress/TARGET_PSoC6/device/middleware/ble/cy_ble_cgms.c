/***************************************************************************//**
* \file cy_ble_cgms.c
* \version 2.0
*
* \brief
*  This file contains the source code for the Continuous Glucose Monitoring
*  Service of the BLE Component.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_CGMS))

static cy_ble_callback_t Cy_BLE_CGMS_ApplCallback;

#ifdef CY_BLE_CGMS_SERVER
/* Internal CGMS flags */
uint8_t cy_ble_cgmsFlag;

/* Internal storage for last request handle to check response for server */
static cy_ble_gatt_db_attr_handle_t cy_ble_cgmssReqHandle;
#endif /* CY_BLE_CGMS_SERVER */

#ifdef CY_BLE_CGMS_CLIENT
/* CGMS Center Service characteristics GATT DB handles structure */
cy_stc_ble_cgmsc_t cy_ble_cgmsc[CY_BLE_CONFIG_GATTC_COUNT];

/* Internal storage for last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_cgmscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_CGMS_CLIENT */

/* The pointer on global BLE CGMS Config structure */
cy_stc_ble_cgms_config_t *cy_ble_cgmsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_CGMS_SERVER
static void Cy_BLE_CGMSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static cy_en_ble_gatt_err_code_t Cy_BLE_CGMSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* CY_BLE_CGMS_SERVER */

#ifdef CY_BLE_CGMS_CLIENT
static void Cy_BLE_CGMSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_CGMSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_CGMSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_CGMSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_CGMSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam);
static void Cy_BLE_CGMSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_CGMSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_CGMSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* CY_BLE_CGMS_CLIENT */
static cy_en_ble_gatt_err_code_t Cy_BLE_CGMS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_CGMS_Init
***************************************************************************//**
*
*  Initializes the profile internals.
*
*  \param config: Configuration structure for the Continuous Glucose Monitoring
*  Service
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
cy_en_ble_api_result_t Cy_BLE_CGMS_Init(cy_stc_ble_cgms_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure*/
        cy_ble_cgmsConfigPtr = config;

        /* Registers Event Handler for the BCS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_CGMS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_cgmsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_CGMS_ApplCallback = cy_ble_cgmsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_CGMS_SERVER
        cy_ble_cgmsFlag = 0u;
        cy_ble_cgmssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    #endif /* CY_BLE_CGMS_SERVER */
    #ifdef CY_BLE_CGMS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_CGMS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_cgmsc[idx], 0, sizeof(cy_stc_ble_cgmsc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_CGMS].uuid = CY_BLE_UUID_CGM_SERVICE;
                }
                cy_ble_cgmscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_GATT_ROLE_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CGMS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc:  An application layer event callback function to receive
*                    events from the BLE Component. The definition of
*                    cy_ble_callback_t for CGM Service is,
*                    typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                       void *eventParam)
*                    * eventCode indicates the event that triggered this
*                      callback.
*                    * eventParam contains the parameters corresponding to the
*                      current event.
*
*
*  \sideeffect The *eventParams in the callback function should not be used
*                by the application once the callback function execution is
*                finished. Otherwise this data may become corrupted.
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
cy_en_ble_api_result_t Cy_BLE_CGMS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_CGMS_ApplCallback = callbackFunc;
    if(cy_ble_cgmsConfigPtr != NULL)
    {
        cy_ble_cgmsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_CGMS_SERVER

/******************************************************************************
* Function Name: Cy_BLE_CGMSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets a characteristic value of the service identified by charIndex.
*
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               stored in the GATT database.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CGMSS_SetCharacteristicValue(cy_en_ble_cgms_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check parameters */
    if(charIndex >= CY_BLE_CGMS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_cgmsConfigPtr->cgmss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Store characteristic value into GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_cgmsConfigPtr->cgmss->charInfo[charIndex].charHandle,
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
* Function Name: Cy_BLE_CGMSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets a characteristic value of the service identified by charIndex.
*
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the location where Characteristic value data should
*                    be stored.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CGMSS_GetCharacteristicValue(cy_en_ble_cgms_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check the parameters */
    if(charIndex >= CY_BLE_CGMS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_cgmsConfigPtr->cgmss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get characteristic value from GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_cgmsConfigPtr->cgmss->charInfo[charIndex].charHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .offset                     = 0u,
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
* Function Name: Cy_BLE_CGMSS_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sets a characteristic descriptor of a specified characteristic of the service.
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_cgms_char_index_t.
*  \param descrIndex:      The index of a service characteristic descriptor of type
*                   cy_en_ble_cgms_descr_index_t.
*  \param attrSize:        The size of the characteristic descriptor attribute.
*  \param attrValue:      The pointer to the descriptor value data that should
*                   be stored to the GATT database.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request is handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CGMSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_cgms_char_index_t charIndex,
                                                                cy_en_ble_cgms_descr_index_t descrIndex,
                                                                uint8_t attrSize,
                                                                uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if((charIndex >= CY_BLE_CGMS_CHAR_COUNT) || (descrIndex >= CY_BLE_CGMS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_cgmsConfigPtr->cgmss->charInfo[charIndex].descrHandle[descrIndex],
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .connHandle                 = connHandle,
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
* Function Name: Cy_BLE_CGMSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets a characteristic descriptor of a specified characteristic of the service.
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_cgms_char_index_t.
*  \param descrIndex:      The index of a service characteristic descriptor of type
*                   cy_en_ble_cgms_descr_index_t.
*  \param attrSize:        The size of the characteristic descriptor attribute.
*  \param attrValue:      The pointer to the location where characteristic descriptor
*                   value data should be stored.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CGMSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_cgms_char_index_t charIndex,
                                                                cy_en_ble_cgms_descr_index_t descrIndex,
                                                                uint8_t attrSize,
                                                                uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if((charIndex >= CY_BLE_CGMS_CHAR_COUNT) || (descrIndex >= CY_BLE_CGMS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_cgmsConfigPtr->cgmss->charInfo[charIndex].descrHandle[descrIndex],
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .connHandle                 = connHandle,
            .offset                     = 0u,
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
* Function Name: Cy_BLE_CGMSS_SendNotification
***************************************************************************//**
*
*  Sends a notification of the specified characteristic to the client device,
*  as defined by the charIndex value.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_CGMSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The connection handle which consists of the device ID and ATT
*               connection ID.
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the Characteristic value data that should be sent
*               to the client device.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is absent.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the client.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CGMSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     cy_en_ble_cgms_char_index_t charIndex,
                                                     uint8_t attrSize,
                                                     uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send notification if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(charIndex >= CY_BLE_CGMS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_cgmsConfigPtr->cgmss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if(!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_cgmsConfigPtr->cgmss->charInfo[charIndex].
                                             descrHandle[CY_BLE_CGMS_CCCD]))
    {
        apiResult = CY_BLE_ERROR_NTF_DISABLED;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gatts_handle_value_ntf_t ntfParam =
        {
            .handleValPair.attrHandle = cy_ble_cgmsConfigPtr->cgmss->charInfo[charIndex].charHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* Send notification to client using previously filled structure */
        apiResult = Cy_BLE_GATTS_Notification(&ntfParam);
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CGMSS_SendIndication
***************************************************************************//**
*
*  Sends an indication of the specified characteristic to the client device, as
*  defined by the charIndex value.
*
*  On enabling indication successfully it sends out a 'Handle Value Indication' which
*  results in CY_BLE_EVT_CGMSC_INDICATION or CY_BLE_EVT_GATTC_HANDLE_VALUE_IND (if
*  service-specific callback function is not registered) event at the GATT Client's end.
*
*  \param connHandle: The connection handle which consists of the device ID and ATT
*               connection ID.
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the Characteristic value data that should be sent
*               to Client device.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is absent.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_IND_DISABLED - Indication is not enabled by the client.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the CGMS service-specific callback is registered
*      (with Cy_BLE_CGMS_RegisterAttrCallback):
*  * CY_BLE_EVT_CGMSS_INDICATION_CONFIRMED - in case if the indication is
*                                successfully delivered to the peer device.
*  .
*   Otherwise (if the CGMS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF - in case if the indication is
*                                successfully delivered to the peer device.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CGMSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                   cy_en_ble_cgms_char_index_t charIndex,
                                                   uint8_t attrSize,
                                                   uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send indication if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(charIndex >= CY_BLE_CGMS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_cgmsConfigPtr->cgmss->charInfo[charIndex].descrHandle[CY_BLE_CGMS_CCCD] ==
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if(!CY_BLE_IS_INDICATION_ENABLED(connHandle.attId, cy_ble_cgmsConfigPtr->cgmss->charInfo[charIndex].
                                           descrHandle[CY_BLE_CGMS_CCCD]))
    {
        apiResult = CY_BLE_ERROR_IND_DISABLED;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gatts_handle_value_ind_t indParam =
        {
            .handleValPair.attrHandle = cy_ble_cgmsConfigPtr->cgmss->charInfo[charIndex].charHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* Send notification to client using previously filled structure */
        apiResult = Cy_BLE_GATTS_Indication(&indParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            /* Save handle to support service-specific value confirmation response from client */
            cy_ble_cgmssReqHandle = indParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CGMSS_ConfirmationEventHandler
***************************************************************************//**
*
*  Handles a Value Confirmation request event from the BLE stack.
*
*  \param eventParam: The pointer to a structure of type cy_stc_ble_conn_handle_t
*
******************************************************************************/
static void Cy_BLE_CGMSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    if((Cy_BLE_CGMS_ApplCallback != NULL)
       && (cy_ble_cgmsConfigPtr->cgmss->charInfo[CY_BLE_CGMS_RACP].charHandle == cy_ble_cgmssReqHandle))
    {
        cy_stc_ble_cgms_char_value_t locCharIndex;

        for(locCharIndex.charIndex = CY_BLE_CGMS_RACP;
            locCharIndex.charIndex < CY_BLE_CGMS_CHAR_COUNT; locCharIndex.charIndex++)
        {
            if(cy_ble_cgmsConfigPtr->cgmss->charInfo[locCharIndex.charIndex].charHandle == cy_ble_cgmssReqHandle)
            {
                cy_ble_cgmssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                locCharIndex.connHandle = *eventParam;
                locCharIndex.value = NULL;

                if(locCharIndex.charIndex == CY_BLE_CGMS_RACP)
                {
                    cy_ble_cgmsFlag &= (uint8_t) ~CY_BLE_CGMS_FLAG_PROCESS;
                }

                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                Cy_BLE_CGMS_ApplCallback((uint32_t)CY_BLE_EVT_CGMSS_INDICATION_CONFIRMED, &locCharIndex);
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CGMSS_WriteEventHandler
***************************************************************************//**
*
*  Handles a Write Request event. Calls the registered CGMS application
*  callback function.
*
* Note: Writing the attribute into GATT DB (if needed) is on the user's responsibility,
*  after the characteristic content check in the registered CGMS application
*  callback function.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*  cy_en_ble_gatt_err_code_t - A function result state if it succeeded
*  (CY_BLE_GATT_ERR_NONE) or failed with error codes:
*   * CY_BLE_GATTS_ERR_PROCEDURE_ALREADY_IN_PROGRESS
*   * CY_BLE_GATTS_ERR_CCCD_IMPROPERLY_CONFIGURED
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_CGMSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_stc_ble_cgms_char_value_t locCharValue =
    {
        .gattErrorCode = CY_BLE_GATT_ERR_NONE,
        .connHandle    = eventParam->connHandle
    };

    if(Cy_BLE_CGMS_ApplCallback != NULL)
    {
        for(locCharValue.charIndex = CY_BLE_CGMS_CGMT;
            locCharValue.charIndex < CY_BLE_CGMS_CHAR_COUNT; locCharValue.charIndex++)
        {
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair = eventParam->handleValPair,
                .connHandle      = eventParam->connHandle,
                .offset          = 0u,
                .flags           = CY_BLE_GATT_DB_PEER_INITIATED
            };
            if(eventParam->handleValPair.attrHandle ==
               cy_ble_cgmsConfigPtr->cgmss->charInfo[locCharValue.charIndex].charHandle)
            {
                locCharValue.value = &eventParam->handleValPair.value;

                if(locCharValue.charIndex == CY_BLE_CGMS_RACP)
                {
                    if(CY_BLE_CGMS_IS_PROCEDURE_IN_PROGRESS)
                    {
                        if(eventParam->handleValPair.value.val[0u] == CY_BLE_CGMS_RACP_OPCODE_ABORT)
                        {
                            cy_ble_cgmsFlag &= (uint8_t) ~CY_BLE_CGMS_FLAG_PROCESS;
                        }
                        else
                        {
                            locCharValue.gattErrorCode = CY_BLE_GATT_ERR_PROCEDURE_ALREADY_IN_PROGRESS;
                        }
                    }
                    else if(!CY_BLE_IS_INDICATION_ENABLED(eventParam->connHandle.attId,
                                                          cy_ble_cgmsConfigPtr->cgmss->
                                                           charInfo[CY_BLE_CGMS_RACP].descrHandle[CY_BLE_CGMS_CCCD]))
                    {
                        locCharValue.gattErrorCode = CY_BLE_GATT_ERR_CCCD_IMPROPERLY_CONFIGURED;
                    }
                    else
                    {
                        cy_ble_cgmsFlag |= CY_BLE_CGMS_FLAG_PROCESS;
                    }
                }
                else
                {
                    /* CY_BLE_CGMS_SOCP or CY_BLE_CGMS_SSTM */
                }

                if(locCharValue.gattErrorCode == CY_BLE_GATT_ERR_NONE)
                {
                    Cy_BLE_CGMS_ApplCallback((uint32_t)CY_BLE_EVT_CGMSS_WRITE_CHAR, &locCharValue);
                }

                if(locCharValue.gattErrorCode == CY_BLE_GATT_ERR_NONE)
                {
                    locCharValue.gattErrorCode = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);
                }

                /* Clear callback flag indicating that request was handled */
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                locCharValue.charIndex = CY_BLE_CGMS_CHAR_COUNT; /* instead of break */
            }
            else if(eventParam->handleValPair.attrHandle ==
                    cy_ble_cgmsConfigPtr->cgmss->charInfo[locCharValue.charIndex].descrHandle[CY_BLE_CGMS_CCCD])
            {
                locCharValue.value = NULL;
                locCharValue.gattErrorCode = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);
                if(locCharValue.gattErrorCode == CY_BLE_GATT_ERR_NONE)
                {
                    uint32_t eventCode;

                    if(locCharValue.charIndex == CY_BLE_CGMS_CGMT)
                    {
                        if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                        {
                            eventCode = (uint32_t)CY_BLE_EVT_CGMSS_NOTIFICATION_ENABLED;
                        }
                        else
                        {
                            eventCode = (uint32_t)CY_BLE_EVT_CGMSS_NOTIFICATION_DISABLED;
                        }
                    }
                    else
                    {
                        if(CY_BLE_IS_INDICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                        {
                            eventCode = (uint32_t)CY_BLE_EVT_CGMSS_INDICATION_ENABLED;
                        }
                        else
                        {
                            eventCode = (uint32_t)CY_BLE_EVT_CGMSS_INDICATION_DISABLED;
                        }
                    }

                    Cy_BLE_CGMS_ApplCallback(eventCode, &locCharValue);

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
                locCharValue.charIndex = CY_BLE_CGMS_CHAR_COUNT; /* instead of break */
            }
            else
            {
                /* Nothing else */
            }
        }
    }

    return(locCharValue.gattErrorCode);
}

#endif /* CY_BLE_CGMS_SERVER */

#ifdef CY_BLE_CGMS_CLIENT


/******************************************************************************
* Function Name: Cy_BLE_CGMSC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_CGMSS_WRITE_CHAR events is generated.
*  On successful request execution on the Server side the Write Response is
*  sent to the Client.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the server device.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
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
*   If the CGMS service-specific callback is registered
*      (with Cy_BLE_CGMS_RegisterAttrCallback):
*  * CY_BLE_EVT_CGMSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_cgms_char_value_t.
*  .
*   Otherwise (if the CGMS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CGMSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_cgms_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_CGMS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_cgmsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_WRITE & cy_ble_cgmsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_cgmsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_cgmscReqHandle[discIdx] = cy_ble_cgmsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CGMSC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read the characteristic Value from a server
*  identified by charIndex.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The read request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established.
*  * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                     characteristic.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the CGMS service-specific callback is registered
*      (with Cy_BLE_CGMS_RegisterAttrCallback):
*  * CY_BLE_EVT_CGMSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_cgms_char_value_t.
*  .
*   Otherwise (if the CGMS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_CGMSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_cgms_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_CGMS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_cgmsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_READ & cy_ble_cgmsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_cgmsc[discIdx].charInfo[charIndex].valueHandle,
            .connHandle = connHandle
        };

        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_cgmscReqHandle[discIdx] = cy_ble_cgmsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CGMSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sets the Characteristic Descriptor of the specified characteristic.
*
*  Internally, Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server side the following events can be
*  generated:
*  * CY_BLE_EVT_CGMSS_INDICATION_ENABLED
*  * CY_BLE_EVT_CGMSS_INDICATION_DISABLED
*  * CY_BLE_EVT_CGMSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_CGMSS_NOTIFICATION_DISABLED
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a service characteristic.
*  \param descrIndex: The index of a service characteristic descriptor.
*  \param attrSize: The size of the characteristic descriptor value attribute.
*  \param attrValue: The pointer to the characteristic descriptor value data that should
*               be sent to the server device.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic.
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the CGMS service-specific callback is registered
*      (with Cy_BLE_CGMS_RegisterAttrCallback):
*  * CY_BLE_EVT_CGMSC_WRITE_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_cgms_descr_value_t.
*  .
*   Otherwise (if the CGMS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CGMSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_cgms_char_index_t charIndex,
                                                                cy_en_ble_cgms_descr_index_t descrIndex,
                                                                uint8_t attrSize,
                                                                uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_CGMS_CHAR_COUNT) || (descrIndex >= CY_BLE_CGMS_DESCR_COUNT)
            || (attrSize != CY_BLE_CCCD_LEN) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_cgmsc[discIdx].charInfo[charIndex].descrHandle[CY_BLE_CGMS_CCCD] ==
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_cgmsc[discIdx].charInfo[charIndex].descrHandle[CY_BLE_CGMS_CCCD],
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = CY_BLE_CCCD_LEN,
            .connHandle               = connHandle
        };
        /* ... and send request to server device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            /* Save handle to support service-specific read response from device */
            cy_ble_cgmscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CGMSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets the characteristic descriptor of the specified characteristic.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a service characteristic.
*  \param descrIndex: The index of the service characteristic descriptor.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular descriptor.
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the CGMS service-specific callback is registered
*      (with Cy_BLE_CGMS_RegisterAttrCallback):
*  * CY_BLE_EVT_CGMSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_cgms_descr_value_t.
*  .
*  Otherwise (if the CGMS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_CGMSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_cgms_char_index_t charIndex,
                                                                cy_en_ble_cgms_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_CGMS_CHAR_COUNT) || (descrIndex >= CY_BLE_CGMS_DESCR_COUNT) ||
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_cgmsc[discIdx].charInfo[charIndex].descrHandle[CY_BLE_CGMS_CCCD] ==
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_cgmsc[discIdx].charInfo[charIndex].descrHandle[CY_BLE_CGMS_CCCD],
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_cgmscReqHandle[discIdx] = cy_ble_cgmsc[discIdx].charInfo[charIndex].descrHandle[CY_BLE_CGMS_CCCD];
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CGMSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_CGMSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* CGM Service characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_cgmscCharUuid[CY_BLE_CGMS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_CGM_MEASUREMENT,
        CY_BLE_UUID_CHAR_CGM_FEATURE,
        CY_BLE_UUID_CHAR_CGM_STATUS,
        CY_BLE_UUID_CHAR_CGM_SESSION_START_TIME,
        CY_BLE_UUID_CHAR_CGM_SESSION_RUN_TIME,
        CY_BLE_UUID_CHAR_RACP,
        CY_BLE_UUID_CHAR_CGM_SOCP
    };
    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_CGMS))
    {
        /* Update last characteristic endHandle to declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        for(i = 0u; i < (uint32_t)CY_BLE_CGMS_CHAR_COUNT; i++)
        {
            if(cy_ble_cgmscCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_cgmsc[discIdx].charInfo[i].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_cgmsc[discIdx].charInfo[i].valueHandle = discCharInfo->valueHandle;
                    cy_ble_cgmsc[discIdx].charInfo[i].properties = discCharInfo->properties;
                    lastEndHandle[discIdx] = &cy_ble_cgmsc[discIdx].charInfo[i].endHandle;
                }
                else
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo);
                }
            }
        }

        /* Init characteristic endHandle to Service endHandle.
         * Characteristic endHandle will be updated to declaration
         * Handler of following characteristic,
         * in following characteristic discovery procedure. */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = cy_ble_serverInfo[discIdx][CY_BLE_SRVI_CGMS].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CGMSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_CGMSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t notSupportedDescr = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t descIdx;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_CGMS)
    {
        switch(discDescrInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_CLIENT_CONFIG:
                descIdx = (uint32_t)CY_BLE_CGMS_CCCD;
                break;

            default:
                /* Not supported descriptor */
                notSupportedDescr = 1u;
                break;
        }

        if(notSupportedDescr == 0u)
        {
            if(cy_ble_cgmsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] ==
               CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_cgmsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] =
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
* Function Name: Cy_BLE_CGMSC_GetCharRange
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
static void Cy_BLE_CGMSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_CGMS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_CGMS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if((cy_ble_cgmsc[discIdx].charInfo[charIdx].endHandle -
                cy_ble_cgmsc[discIdx].charInfo[charIdx].valueHandle) != 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_cgmsc[discIdx].charInfo[charIdx].valueHandle + 1u;
                charRangeInfo->range.endHandle = cy_ble_cgmsc[discIdx].charInfo[charIdx].endHandle;
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
* Function Name: Cy_BLE_CGMSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles a Notification event.
*
*  \param eventParam: The pointer to a data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_CGMSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_CGMS_ApplCallback != NULL))
    {
        if(cy_ble_cgmsc[discIdx].charInfo[CY_BLE_CGMS_CGMT].valueHandle == eventParam->handleValPair.attrHandle)
        {
            cy_stc_ble_cgms_char_value_t locCharValue =
            {
                .charIndex  = CY_BLE_CGMS_CGMT,
                .connHandle = eventParam->connHandle,
                .value      = &eventParam->handleValPair.value
            };
            Cy_BLE_CGMS_ApplCallback((uint32_t)CY_BLE_EVT_CGMSC_NOTIFICATION, &locCharValue);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CGMSC_IndicationEventHandler
***************************************************************************//**
*
*  Handles an Indication event.
*
*  \param eventParam:  The pointer to a data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_CGMSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_CGMS_ApplCallback != NULL))
    {
        cy_stc_ble_cgms_char_value_t locCharValue;
        locCharValue.charIndex = CY_BLE_CGMS_RACP;

        while((cy_ble_cgmsc[discIdx].charInfo[locCharValue.charIndex].valueHandle !=
               eventParam->handleValPair.attrHandle) && (locCharValue.charIndex < CY_BLE_CGMS_CHAR_COUNT))
        {
            locCharValue.charIndex++;
        }

        if(locCharValue.charIndex < CY_BLE_CGMS_CHAR_COUNT)
        {
            locCharValue.connHandle = eventParam->connHandle;
            locCharValue.value = &eventParam->handleValPair.value;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_CGMS_ApplCallback((uint32_t)CY_BLE_EVT_CGMSC_INDICATION, &locCharValue);
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CGMSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles a Read Response event.
*
*  \param eventParam: The pointer to the data structure.
*
******************************************************************************/
static void Cy_BLE_CGMSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_CGMS_ApplCallback != NULL) &&
       (cy_ble_cgmscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        cy_en_ble_cgms_char_index_t charIdx;
        uint8_t fFlag = 0u;

        for(charIdx = CY_BLE_CGMS_CGMT; (charIdx < CY_BLE_CGMS_CHAR_COUNT) && (fFlag == 0u); charIdx++)
        {
            if(cy_ble_cgmsc[discIdx].charInfo[charIdx].valueHandle == cy_ble_cgmscReqHandle[discIdx])
            {
                cy_stc_ble_cgms_char_value_t locCharValue =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = charIdx,
                    .value      = &eventParam->value
                };
                cy_ble_cgmscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                Cy_BLE_CGMS_ApplCallback((uint32_t)CY_BLE_EVT_CGMSC_READ_CHAR_RESPONSE, &locCharValue);
                fFlag = 1u; /* instead of break */
            }
            else if(cy_ble_cgmsc[discIdx].charInfo[charIdx].descrHandle[CY_BLE_CGMS_CCCD] ==
                    cy_ble_cgmscReqHandle[discIdx])
            {
                cy_stc_ble_cgms_descr_value_t locDescrValue =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = charIdx,
                    .descrIndex = CY_BLE_CGMS_CCCD,
                    .value      = &eventParam->value
                };
                cy_ble_cgmscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                Cy_BLE_CGMS_ApplCallback((uint32_t)CY_BLE_EVT_CGMSC_READ_DESCR_RESPONSE, &locDescrValue);
                fFlag = 1u; /* instead of break */
            }
            else
            {
                /* nothing else */
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CGMSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Write Response event.
*
*  \param eventParam: The pointer to a data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_CGMSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_CGMS_ApplCallback != NULL) && (cy_ble_cgmscReqHandle[discIdx] !=
                                                                                       CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        cy_en_ble_cgms_char_index_t charIdx;
        uint8_t fFlag = 0u;

        for(charIdx = CY_BLE_CGMS_CGMT; (charIdx < CY_BLE_CGMS_CHAR_COUNT) && (fFlag == 0u); charIdx++)
        {
            if(cy_ble_cgmsc[discIdx].charInfo[charIdx].valueHandle == cy_ble_cgmscReqHandle[discIdx])
            {
                cy_stc_ble_cgms_char_value_t locCharValue =
                {
                    .connHandle = *eventParam,
                    .charIndex  = charIdx,
                    .value      = NULL
                };
                cy_ble_cgmscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                Cy_BLE_CGMS_ApplCallback((uint32_t)CY_BLE_EVT_CGMSC_WRITE_CHAR_RESPONSE, &locCharValue);
                fFlag = 1u; /* instead of break */
            }
            else if(cy_ble_cgmsc[discIdx].charInfo[charIdx].descrHandle[CY_BLE_CGMS_CCCD] == cy_ble_cgmscReqHandle[discIdx])
            {
                cy_stc_ble_cgms_descr_value_t locDescrValue =
                {
                    .connHandle = *eventParam,
                    .charIndex  = charIdx,
                    .descrIndex = CY_BLE_CGMS_CCCD,
                    .value      = NULL
                };
                cy_ble_cgmscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                Cy_BLE_CGMS_ApplCallback((uint32_t)CY_BLE_EVT_CGMSC_WRITE_DESCR_RESPONSE, &locDescrValue);
                fFlag = 1u; /* instead of break */
            }
            else
            {
                /* nothing else */
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CGMSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles an Error Response event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_CGMSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        if(cy_ble_cgmscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] == eventParam->errInfo.attrHandle)
        {
            cy_ble_cgmscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}

#endif /* CY_BLE_CGMS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_CGMS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the the Continuous Glucose Monitoring
*  Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_CGMS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_CGMS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_CGMSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_CGMSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_CGMSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_CGMS_CLIENT */

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
        #ifdef CY_BLE_CGMS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_CGMSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF:
                Cy_BLE_CGMSS_ConfirmationEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;

            case CY_BLE_EVT_GATT_CONNECT_IND:
                (void)Cy_BLE_CGMS_Init(cy_ble_cgmsConfigPtr);
                break;
        #endif /* CY_BLE_CGMS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_CGMS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_CGMSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_CGMSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_IND:
                Cy_BLE_CGMSC_IndicationEventHandler((cy_stc_ble_gattc_handle_value_ind_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_CGMSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_CGMSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_CGMS_CLIENT */
            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_CGMS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
