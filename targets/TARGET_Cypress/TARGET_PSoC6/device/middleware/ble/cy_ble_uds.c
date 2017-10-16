/***************************************************************************//**
* \file cy_ble_uds.c
* \version 2.0
*
* \brief
*  This file contains the source code for
*  the User Data Service of the BLE Component.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_UDS))

static cy_ble_callback_t Cy_BLE_UDS_ApplCallback;

#ifdef CY_BLE_UDS_SERVER
uint8_t cy_ble_udsFlag;

/* Internal storage for last request handle to check response for server */
static cy_ble_gatt_db_attr_handle_t cy_ble_udssReqHandle;
#endif /* CY_BLE_UDS_SERVER */

#ifdef CY_BLE_UDS_CLIENT
/* Server's User Data Service characteristics GATT DB handles structure */
cy_stc_ble_udsc_t cy_ble_udsc[CY_BLE_CONFIG_GATTC_COUNT];

/* Internal storage for last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_udscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];

/* Read Long Descriptors variables */
static uint8_t * cy_ble_udscRdLongBuffPtr[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
static uint16_t cy_ble_udscRdLongBuffLen[CY_BLE_CONFIG_GATTC_COUNT] = { 0u };
static uint8_t cy_ble_udscCurrLen[CY_BLE_CONFIG_GATTC_COUNT] = { 0u };
#endif /* CY_BLE_UDS_CLIENT */

/* The pointer on global BLE UDS Config structure */
cy_stc_ble_uds_config_t *cy_ble_udsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/

#ifdef CY_BLE_UDS_SERVER
static void Cy_BLE_UDSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_UDSS_ReadRequestEventHandler(cy_stc_ble_gatts_char_val_read_req_t *eventParam);
static cy_en_ble_gatt_err_code_t Cy_BLE_UDSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
static void Cy_BLE_UDSS_PrepareWriteRequestEventHandler(cy_stc_ble_gatts_prep_write_req_param_t *eventParam);
static void Cy_BLE_UDSS_ExecuteWriteRequestEventHandler(cy_stc_ble_gatts_exec_write_req_t *eventParam);
#endif /* CY_BLE_UDS_SERVER */

#ifdef CY_BLE_UDS_CLIENT
static void Cy_BLE_UDSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_UDSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_UDSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_UDSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_UDSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam);
static void Cy_BLE_UDSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_UDSC_ReadLongRespEventHandler(const cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_UDSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_UDSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
static void Cy_BLE_UDSC_ExecuteWriteResponseEventHandler(const cy_stc_ble_gattc_exec_write_rsp_t *eventParam);
#endif /* CY_BLE_UDS_CLIENT */
static cy_en_ble_gatt_err_code_t Cy_BLE_UDS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_UDS_Init
***************************************************************************//**
*
*  Initializes the profile internals.
*
*  \param config: Configuration structure for Uds.
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
cy_en_ble_api_result_t Cy_BLE_UDS_Init(cy_stc_ble_uds_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_udsConfigPtr = config;

        /* Registers Event Handler for the UDS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_UDS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_udsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_UDS_ApplCallback = cy_ble_udsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_UDS_SERVER
        cy_ble_udsFlag = 0u;
        cy_ble_udssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    #endif /* CY_BLE_UDS_SERVER */

    #ifdef CY_BLE_UDS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_UDS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_udsc[idx], 0, sizeof(cy_stc_ble_udsc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_UDS].uuid = CY_BLE_UUID_USER_DATA_SERVICE;
                }
                cy_ble_udscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_UDS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_UDS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service-specific write requests from a peer device will not be handled with
*  an unregistered callback function.
*
*  \param callbackFunc: An application layer event callback function to receive
*                        events from the BLE Component. The definition of
*                        cy_ble_callback_t for UDS is: \n
*    typedef void (* cy_ble_callback_t) (uint32_t eventCode, void *eventParam),
*    where:
*      * eventCode indicates the event that triggered this callback.
*      * eventParam contains the parameters corresponding to the current event.
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
cy_en_ble_api_result_t Cy_BLE_UDS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_UDS_ApplCallback = callbackFunc;
    if(cy_ble_udsConfigPtr != NULL)
    {
        cy_ble_udsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_UDS_SERVER


/******************************************************************************
* Function Name: Cy_BLE_UDSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets the value of the characteristic, as identified by charIndex.
*
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*                     stored to the GATT database.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is absent
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_UDSS_SetCharacteristicValue(cy_en_ble_uds_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check parameters */
    if(charIndex >= CY_BLE_UDS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_udsConfigPtr->udss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Store characteristic value into GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_udsConfigPtr->udss->charInfo[charIndex].charHandle,
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
* Function Name: Cy_BLE_UDSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets the value of the characteristic, as identified by charIndex.
*
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue: The pointer to the location where characteristic
*                     value data should be stored.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic value was read successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - A characteristic is absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_UDSS_GetCharacteristicValue(cy_en_ble_uds_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check parameters */
    if(charIndex >= CY_BLE_UDS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_udsConfigPtr->udss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get characteristic value from GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_udsConfigPtr->udss->charInfo[charIndex].charHandle,
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
* Function Name: Cy_BLE_UDSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets a characteristic descriptor of the specified characteristic.
*
*  \param connHandle: The connection handle
*  \param charIndex:  The index of the characteristic.
*  \param descrIndex: The index of the descriptor.
*  \param attrSize:   The size of the descriptor value attribute.
*  \param attrValue: The pointer to the location where characteristic descriptor value
*                     data should be stored.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - Characteristic Descriptor value was read successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - A characteristic is absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_UDSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_uds_char_index_t charIndex,
                                                               cy_en_ble_uds_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check parameters */
    if((charIndex >= CY_BLE_UDS_CHAR_COUNT) || (descrIndex >= CY_BLE_UDS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_udsConfigPtr->udss->charInfo[charIndex].descrHandle[CY_BLE_UDS_CCCD] ==
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get data from database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_udsConfigPtr->udss->charInfo[charIndex].descrHandle[descrIndex],
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
        else
        {
            apiResult = CY_BLE_SUCCESS;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_UDSS_SendNotification
***************************************************************************//**
*
*  Sends a notification of the specified characteristic value, as identified by
*  the charIndex.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_UDSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The connection handle which consist of the device ID and ATT
*                     connection ID.
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data
*                     that should be sent to the client device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                     characteristic
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is absent.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the client.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_UDSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_uds_char_index_t charIndex,
                                                    uint8_t attrSize,
                                                    uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send Notification if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(charIndex >= CY_BLE_UDS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_udsConfigPtr->udss->charInfo[charIndex].descrHandle[CY_BLE_UDS_CCCD] ==
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if(!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId,
                                            cy_ble_udsConfigPtr->udss->charInfo[charIndex].descrHandle[CY_BLE_UDS_CCCD]))
    {
        apiResult = CY_BLE_ERROR_NTF_DISABLED;
    }
    else
    {
        cy_stc_ble_gatts_handle_value_ntf_t ntfParam =
        {
            /* Fill all fields of write request structure ... */
            .handleValPair.attrHandle = cy_ble_udsConfigPtr->udss->charInfo[charIndex].charHandle,
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
* Function Name: Cy_BLE_UDSS_SendIndication
***************************************************************************//**
*
*  Sends an indication of the specified characteristic value, as identified by
*  the charIndex.
*
*  On enabling indication successfully it sends out a 'Handle Value Indication' which
*  results in CY_BLE_EVT_UDSC_INDICATION or CY_BLE_EVT_GATTC_HANDLE_VALUE_IND (if
*  service-specific callback function is not registered) event at the GATT Client's end.
*
*  \param connHandle: The connection handle which consist of the device ID and ATT
*                     connection ID.
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be sent
*                     to the client device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is absent.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the client.
*   * CY_BLE_ERROR_IND_DISABLED - Indication is disabled for this characteristic.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the UDS service-specific callback is registered
*      (with Cy_BLE_UDS_RegisterAttrCallback):
*  * CY_BLE_EVT_UDSS_INDICATION_CONFIRMED - If the indication is
*                                successfully delivered to the peer device.
*  .
*   Otherwise (if the UDS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF - If the indication is
*                                successfully delivered to the peer device.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_UDSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_uds_char_index_t charIndex,
                                                  uint8_t attrSize,
                                                  uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send Indication if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(charIndex >= CY_BLE_UDS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_udsConfigPtr->udss->charInfo[charIndex].descrHandle[CY_BLE_UDS_CCCD] ==
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if(!CY_BLE_IS_INDICATION_ENABLED(connHandle.attId,
                                          cy_ble_udsConfigPtr->udss->charInfo[charIndex].descrHandle[CY_BLE_UDS_CCCD]))
    {
        apiResult = CY_BLE_ERROR_IND_DISABLED;
    }
    else
    {
        cy_stc_ble_gatts_handle_value_ind_t indParam =
        {
            /* Fill all fields of write request structure ... */
            .handleValPair.attrHandle = cy_ble_udsConfigPtr->udss->charInfo[charIndex].charHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };

        /* Send notification to client using previously filled structure */
        apiResult = Cy_BLE_GATTS_Indication(&indParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            /* Save handle to support service-specific value confirmation response from client */
            cy_ble_udssReqHandle = indParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_UDSS_ConfirmationEventHandler
***************************************************************************//**
*
*  Handles the Value Confirmation request event from the BLE stack.
*
*  \param eventParam: The pointer to a structure of type cy_stc_ble_conn_handle_t.
*
******************************************************************************/
static void Cy_BLE_UDSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    if((Cy_BLE_UDS_ApplCallback != NULL)
       && (cy_ble_udsConfigPtr->udss->charInfo[CY_BLE_UDS_UCP].charHandle == cy_ble_udssReqHandle))
    {
        cy_stc_ble_uds_char_value_t locCharIndex =
        {
            .connHandle = *eventParam,
            .charIndex  = CY_BLE_UDS_UCP,
            .value      = NULL
        };

        cy_ble_udssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        cy_ble_udsFlag &= (uint8_t) ~CY_BLE_UDS_FLAG_PROCESS;

        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        Cy_BLE_UDS_ApplCallback((uint32_t)CY_BLE_EVT_UDSS_INDICATION_CONFIRMED, &locCharIndex);
    }
}


/******************************************************************************
* Function Name: Cy_BLE_UDSS_ReadRequestEventHandler
***************************************************************************//**
*
*  Handles the Read Request Event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_UDSS_ReadRequestEventHandler(cy_stc_ble_gatts_char_val_read_req_t *eventParam)
{
    if(Cy_BLE_UDS_ApplCallback != NULL)
    {
        cy_stc_ble_uds_char_value_t locCharValue =
        {
            .connHandle    = eventParam->connHandle,
            .gattErrorCode = CY_BLE_GATT_ERR_NONE,
            .value         = NULL
        };

        for(locCharValue.charIndex = CY_BLE_UDS_FNM;
            locCharValue.charIndex < CY_BLE_UDS_CHAR_COUNT;
            locCharValue.charIndex++)
        {
            if(locCharValue.charIndex == CY_BLE_UDS_UCP)
            {
                /* Omit UCP characteristic */
                locCharValue.charIndex = CY_BLE_UDS_LNG;
            }

            if(eventParam->attrHandle == cy_ble_udsConfigPtr->udss->charInfo[locCharValue.charIndex].charHandle)
            {
                Cy_BLE_UDS_ApplCallback((uint32_t)CY_BLE_EVT_UDSS_READ_CHAR, &locCharValue);

                if(locCharValue.gattErrorCode == CY_BLE_GATT_ERR_USER_DATA_ACCESS_NOT_PERMITTED)
                {
                    eventParam->gattErrorCode = CY_BLE_GATT_ERR_USER_DATA_ACCESS_NOT_PERMITTED;
                }

                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_UDSS_WriteEventHandler
***************************************************************************//**
*
*  Handles the Write Request Event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*  cy_en_ble_gatt_err_code_t - A finction result state if it succeeded
*  (CY_BLE_GATT_ERR_NONE) or failed with error codes:
*   * CY_BLE_GATTS_ERR_PROCEDURE_ALREADY_IN_PROGRESS
*   * CY_BLE_GATTS_ERR_CCCD_IMPROPERLY_CONFIGURED
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_UDSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;

    if(Cy_BLE_UDS_ApplCallback != NULL)
    {
        cy_stc_ble_uds_char_value_t locCharValue;
        locCharValue.connHandle = eventParam->connHandle;
        locCharValue.gattErrorCode = CY_BLE_GATT_ERR_NONE;

        if((eventParam->handleValPair.attrHandle ==
            cy_ble_udsConfigPtr->udss->charInfo[CY_BLE_UDS_DCI].descrHandle[CY_BLE_UDS_CCCD]) ||
           (eventParam->handleValPair.attrHandle ==
            cy_ble_udsConfigPtr->udss->charInfo[CY_BLE_UDS_UCP].descrHandle[CY_BLE_UDS_CCCD]))
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
                uint32_t eventCode;

                if(eventParam->handleValPair.attrHandle == cy_ble_udsConfigPtr->udss->charInfo[CY_BLE_UDS_DCI].
                    descrHandle[CY_BLE_UDS_CCCD])
                {
                    locCharValue.charIndex = CY_BLE_UDS_DCI;

                    if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                    {
                        eventCode = (uint32_t)CY_BLE_EVT_UDSS_NOTIFICATION_ENABLED;
                    }
                    else
                    {
                        eventCode = (uint32_t)CY_BLE_EVT_UDSS_NOTIFICATION_DISABLED;
                    }
                }
                else
                {
                    locCharValue.charIndex = CY_BLE_UDS_UCP;

                    if(CY_BLE_IS_INDICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                    {
                        eventCode = (uint32_t)CY_BLE_EVT_UDSS_INDICATION_ENABLED;
                    }
                    else
                    {
                        eventCode = (uint32_t)CY_BLE_EVT_UDSS_INDICATION_DISABLED;
                    }
                }

                Cy_BLE_UDS_ApplCallback(eventCode, &locCharValue);
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
        }
        else
        {
            for(locCharValue.charIndex = CY_BLE_UDS_FNM;
                locCharValue.charIndex < CY_BLE_UDS_CHAR_COUNT;
                locCharValue.charIndex++)
            {
                if(eventParam->handleValPair.attrHandle ==
                   cy_ble_udsConfigPtr->udss->charInfo[locCharValue.charIndex].charHandle)
                {
                    gattErr = Cy_BLE_GATT_DbCheckPermission(eventParam->handleValPair.attrHandle,
                                                            &eventParam->connHandle, CY_BLE_GATT_DB_WRITE |
                                                            CY_BLE_GATT_DB_PEER_INITIATED);

                    if((gattErr == CY_BLE_GATT_ERR_NONE) && (locCharValue.charIndex == CY_BLE_UDS_UCP))
                    {
                        if(CY_BLE_UDS_IS_PROCEDURE_IN_PROGRESS)
                        {
                            gattErr = CY_BLE_GATT_ERR_PROCEDURE_ALREADY_IN_PROGRESS;
                        }
                        else if(!CY_BLE_IS_INDICATION_ENABLED(eventParam->connHandle.attId,
                                                              cy_ble_udsConfigPtr->udss->charInfo[CY_BLE_UDS_UCP].
                                                               descrHandle[CY_BLE_UDS_CCCD]))
                        {
                            gattErr = CY_BLE_GATT_ERR_CCCD_IMPROPERLY_CONFIGURED;
                        }
                        else
                        {
                        }
                    }

                    if(gattErr == CY_BLE_GATT_ERR_NONE)
                    {
                        locCharValue.value = &eventParam->handleValPair.value;

                        if(!CY_BLE_GATT_DB_ATTR_CHECK_PRPTY(eventParam->handleValPair.attrHandle,
                                                            CY_BLE_GATT_DB_CH_PROP_WRITE))
                        {
                            gattErr = CY_BLE_GATT_ERR_WRITE_NOT_PERMITTED;
                        }
                        else if(eventParam->handleValPair.value.len >
                                CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN(eventParam->handleValPair.attrHandle))
                        {
                            gattErr = CY_BLE_GATT_ERR_INVALID_ATTRIBUTE_LEN;
                        }
                        else
                        {
                            Cy_BLE_UDS_ApplCallback((uint32_t)CY_BLE_EVT_UDSS_WRITE_CHAR, &locCharValue);

                            if(locCharValue.gattErrorCode == CY_BLE_GATT_ERR_USER_DATA_ACCESS_NOT_PERMITTED)
                            {
                                gattErr = CY_BLE_GATT_ERR_USER_DATA_ACCESS_NOT_PERMITTED;
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

                                gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);

                                if(gattErr == CY_BLE_GATT_ERR_NONE)
                                {
                                    CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(cy_ble_udsConfigPtr->udss->
                                                                          charInfo[locCharValue.charIndex].charHandle,
                                                                         locCharValue.value->len);

                                    if(locCharValue.charIndex == CY_BLE_UDS_UCP)
                                    {
                                        cy_ble_udsFlag |= CY_BLE_UDS_FLAG_PROCESS;
                                    }
                                }
                            }
                        }
                    }
                    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                    break;
                }
            }
        }
    }

    return(gattErr);
}

/******************************************************************************
* Function Name: Cy_BLE_UDSS_PrepareWriteEventHandler
***************************************************************************//**
*
*  Handles the Prepare Write Request Event.
*
*  \param eventParam: The pointer to the data that comes with a prepare
*                      write request.
*
*
******************************************************************************/
static void Cy_BLE_UDSS_PrepareWriteRequestEventHandler(cy_stc_ble_gatts_prep_write_req_param_t *eventParam)
{
    cy_en_ble_uds_char_index_t locCharIndex;
    cy_stc_ble_gatt_xchg_mtu_param_t mtuParam = { .connHandle = eventParam->connHandle };

    (void)Cy_BLE_GATT_GetMtuSize(&mtuParam);

    for(locCharIndex = CY_BLE_UDS_FNM; locCharIndex <= CY_BLE_UDS_EML; locCharIndex++)
    {
        if(eventParam->baseAddr[eventParam->currentPrepWriteReqCount - 1u].handleValuePair.attrHandle ==
           cy_ble_udsConfigPtr->udss->charInfo[locCharIndex].charHandle)
        {
            if(CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN(cy_ble_udsConfigPtr->udss->charInfo[locCharIndex].charHandle) <=
               (mtuParam.mtu - CY_BLE_GATT_WRITE_HEADER_LEN))
            {
                eventParam->gattErrorCode = (uint8_t)CY_BLE_GATT_ERR_ATTRIBUTE_NOT_LONG;
            }

            /* Indicate that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            break;
        }
    }
}

/******************************************************************************
* Function Name: Cy_BLE_UDSS_ExecuteWriteRequestEventHandler
***************************************************************************//**
*
*  Handles the Execute Write Request Event.
*
*  \param eventParam: The pointer to the data that came with a write request.
*
******************************************************************************/
static void Cy_BLE_UDSS_ExecuteWriteRequestEventHandler(cy_stc_ble_gatts_exec_write_req_t *eventParam)
{
    uint32_t locCount;
    cy_stc_ble_uds_char_value_t locCharValue[CY_BLE_UDS_LONG_CHAR_COUNT];
    cy_stc_ble_gatt_value_t locGattValue[CY_BLE_UDS_LONG_CHAR_COUNT];
    cy_en_ble_uds_char_index_t locCharIndex;

    for(locCharIndex = CY_BLE_UDS_FNM; locCharIndex <= CY_BLE_UDS_EML; locCharIndex++)
    {
        locGattValue[locCharIndex].len = 0u;
        locGattValue[locCharIndex].val = NULL;
    }

    for(locCount = 0u; locCount < eventParam->prepWriteReqCount; locCount++)
    {
        for(locCharIndex = CY_BLE_UDS_FNM; locCharIndex <= CY_BLE_UDS_EML; locCharIndex++)
        {
            if(eventParam->baseAddr[locCount].handleValuePair.attrHandle ==
               cy_ble_udsConfigPtr->udss->charInfo[locCharIndex].charHandle)
            {
                locGattValue[locCharIndex].len = eventParam->baseAddr[locCount].offset +
                                                 eventParam->baseAddr[locCount].handleValuePair.value.len;

                if(locGattValue[locCharIndex].val == NULL)
                {
                    locGattValue[locCharIndex].val = eventParam->baseAddr[locCount].handleValuePair.value.val;
                }
                else if(eventParam->baseAddr[locCount].offset == 0u)
                {
                    /* Case when client wants to rewrite value from beginning */
                    locGattValue[locCharIndex].val = eventParam->baseAddr[locCount].handleValuePair.value.val;
                }
                else
                {
                    /* Do nothing */
                }
            }
        }
    }

    for(locCharIndex = CY_BLE_UDS_FNM; locCharIndex <= CY_BLE_UDS_EML; locCharIndex++)
    {
        if((Cy_BLE_UDS_ApplCallback != NULL) && (locGattValue[locCharIndex].len != 0u) &&
           (locGattValue[locCharIndex].len <=
            CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN(cy_ble_udsConfigPtr->udss->charInfo[locCharIndex].charHandle)))
        {
            /* Check the execWriteFlag before execute or cancel write long operation */
            if(eventParam->execWriteFlag == CY_BLE_GATT_EXECUTE_WRITE_EXEC_FLAG)
            {
                locCharValue[locCharIndex].gattErrorCode = CY_BLE_GATT_ERR_NONE;
                locCharValue[locCharIndex].connHandle = eventParam->connHandle;
                locCharValue[locCharIndex].charIndex = locCharIndex;
                locCharValue[locCharIndex].value = &locGattValue[locCharIndex];

                Cy_BLE_UDS_ApplCallback((uint32_t)CY_BLE_EVT_UDSS_WRITE_CHAR, &locCharValue[locCharIndex]);

                if(locCharValue[locCharIndex].gattErrorCode == CY_BLE_GATT_ERR_USER_DATA_ACCESS_NOT_PERMITTED)
                {
                    eventParam->gattErrorCode = (uint8_t)CY_BLE_GATT_ERR_USER_DATA_ACCESS_NOT_PERMITTED;
                }
                else
                {
                    CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(cy_ble_udsConfigPtr->udss->charInfo[locCharIndex].charHandle,
                                                         locGattValue[locCharIndex].len);
                }
            }

            /* Indicate that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


#endif /* CY_BLE_UDS_SERVER */

#ifdef CY_BLE_UDS_CLIENT


/******************************************************************************
* Function Name: Cy_BLE_UDSC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_UDSS_WRITE_CHAR events is generated.
*  On successful request execution on the Server side the Write Response is
*  sent to the Client.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*                     sent to the server device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                              the particular characteristic.
*  * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                    characteristic.
*
* \events
*  In the case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the UDS service-specific callback is registered
*      (with Cy_BLE_UDS_RegisterAttrCallback):
*  * CY_BLE_EVT_UDSC_WRITE_CHAR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_uds_char_value_t.
*  .
*   Otherwise (if the UDS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - If the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with an event parameter
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_UDSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_uds_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    cy_stc_ble_gatt_xchg_mtu_param_t mtuParam = { .connHandle = connHandle };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    (void)Cy_BLE_GATT_GetMtuSize(&mtuParam);

    /* Check parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_UDS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_udsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_WRITE & cy_ble_udsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else if((mtuParam.mtu - 3u) < attrSize)
    {
        cy_stc_ble_gattc_prep_write_req_t prepWriteReqParam =
        {
            .handleValOffsetPair.handleValuePair.attrHandle = cy_ble_udsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValOffsetPair.offset                     = 0u,
            .handleValOffsetPair.handleValuePair.value.val  = attrValue,
            .handleValOffsetPair.handleValuePair.value.len  = attrSize,
            .connHandle                                     = connHandle
        };
        apiResult = Cy_BLE_GATTC_WriteLongCharacteristicValues(&prepWriteReqParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_udscReqHandle[discIdx] = cy_ble_udsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }
    else
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_udsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };

        apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_udscReqHandle[discIdx] = cy_ble_udsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_UDSC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read the characteristic Value from a server,
*  as identified by its charIndex. As a result a Read Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_UDSS_READ_CHAR events is generated.
*  On successful request execution on the Server side the Read Response is
*  sent to the Client.
*
*  The Read Response only contains the characteristic Value that is less than or
*  equal to (MTU - 1) octets in length. If the characteristic Value is greater
*  than (MTU - 1) octets in length, the Read Long Characteristic Value procedure
*  may be used if the rest of the characteristic Value is required.
*
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
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
*   If the UDS service-specific callback is registered
*      (with Cy_BLE_UDS_RegisterAttrCallback):
*  * CY_BLE_EVT_UDSC_READ_CHAR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_uds_char_value_t.
*  .
*   Otherwise (if the UDS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP - If the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with an event parameter
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with an event parameter
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_UDSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_uds_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_UDS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_udsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_READ & cy_ble_udsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_udsc[discIdx].charInfo[charIndex].valueHandle,
            .connHandle = connHandle
        };

        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);
    }

    if(apiResult == CY_BLE_SUCCESS)
    {
        cy_ble_udscReqHandle[discIdx] = cy_ble_udsc[discIdx].charInfo[charIndex].valueHandle;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_UDSC_GetLongCharacteristicValue
***************************************************************************//**
*
*  Sends a request to read a long characteristic.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue:  The pointer to the buffer where the read long characteristic
*                     descriptor value should be stored.
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
*                                    characteristic
*
* \events
*  In the case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the UDS service-specific callback is registered
*      (with Cy_BLE_UDS_RegisterAttrCallback):
*  * CY_BLE_EVT_UDSC_READ_CHAR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_uds_char_value_t.
*  .
*   Otherwise (if the UDS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_BLOB_RSP - If the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with an event parameter
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with an event parameter
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_UDSC_GetLongCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                              cy_en_ble_uds_char_index_t charIndex,
                                                              uint16_t attrSize,
                                                              uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(((charIndex > CY_BLE_UDS_EML) && (charIndex != CY_BLE_UDS_LNG)) || (attrSize == 0u) || 
            (attrValue == NULL) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_udsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_READ & cy_ble_udsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_read_blob_req_t readBlobReqParam =
        {
            .handleOffset.attrHandle = cy_ble_udsc[discIdx].charInfo[charIndex].valueHandle,
            .handleOffset.offset     = 0u,
            .connHandle              = connHandle
        };

        apiResult = Cy_BLE_GATTC_ReadLongCharacteristicValues(&readBlobReqParam);
    }

    if(apiResult == CY_BLE_SUCCESS)
    {
        cy_ble_udscReqHandle[discIdx] = cy_ble_udsc[discIdx].charInfo[charIndex].valueHandle;
        cy_ble_udscRdLongBuffLen[discIdx] = attrSize;
        cy_ble_udscRdLongBuffPtr[discIdx] = attrValue;
        cy_ble_udscCurrLen[discIdx] = 0u;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_UDSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  This function is used to write the characteristic Value to the server,
*  as identified by its charIndex.
*
*  Internally, Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server side the following events can be
*  generated:
*  * CY_BLE_EVT_UDSS_INDICATION_ENABLED
*  * CY_BLE_EVT_UDSS_INDICATION_DISABLED
*  * CY_BLE_EVT_UDSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_UDSS_NOTIFICATION_DISABLED
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param attrSize:   The size of the characteristic descriptor value attribute.
*  \param attrValue: The pointer to the characteristic descriptor value data that
*                      should be sent to the server device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                              the particular characteristic.
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                    the specified attribute
*
* \events
*  In the case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the UDS service-specific callback is registered
*      (with Cy_BLE_UDS_RegisterAttrCallback):
*  * CY_BLE_EVT_UDSC_WRITE_DESCR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_uds_descr_value_t.
*  .
*   Otherwise (if the UDS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - If the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with an event parameter
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_UDSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_uds_char_index_t charIndex,
                                                               cy_en_ble_uds_descr_index_t descrIndex,
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
    else if((charIndex >= CY_BLE_UDS_CHAR_COUNT) || (descrIndex >= CY_BLE_UDS_DESCR_COUNT) ||
            (attrSize != CY_BLE_CCCD_LEN) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_udsc[discIdx].charInfo[charIndex].descrHandle[descrIndex] == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            /* Fill all fields of write request structure ... */
            .handleValPair.attrHandle = cy_ble_udsc[discIdx].charInfo[charIndex].descrHandle[descrIndex],
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = CY_BLE_CCCD_LEN,
            .connHandle               = connHandle
        };

        /* ... and send request to server device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            /* Save handle to support service-specific read response from device */
            cy_ble_udscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_UDSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets the characteristic descriptor of the specified characteristic.
*
*  \param connHandle: The connection handle
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param descrIndex: The index of the service characteristic descriptor.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                              the particular descriptor.
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                    the specified attribute.
*
* \events
*  In the case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the UDS service-specific callback is registered
*      (with Cy_BLE_UDS_RegisterAttrCallback):
*  * CY_BLE_EVT_UDSC_READ_DESCR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with an event parameter structure
*                                of type cy_stc_ble_uds_descr_value_t.
*  .
*  Otherwise (if the UDS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP - If the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with an event parameter
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with an event parameter
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_UDSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_uds_char_index_t charIndex,
                                                               cy_en_ble_uds_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_UDS_CHAR_COUNT) || (descrIndex >= CY_BLE_UDS_DESCR_COUNT) ||
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_udsc[discIdx].charInfo[charIndex].descrHandle[descrIndex] == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_udsc[discIdx].charInfo[charIndex].descrHandle[descrIndex],
            .connHandle = connHandle
        };

        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_udscReqHandle[discIdx] = cy_ble_udsc[discIdx].charInfo[charIndex].descrHandle[descrIndex];
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_UDSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_UDSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* User Data Service characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_udscCharUuid[CY_BLE_UDS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_FIRST_NAME,
        CY_BLE_UUID_CHAR_LAST_NAME,
        CY_BLE_UUID_CHAR_EMAIL_ADDRESS,
        CY_BLE_UUID_CHAR_AGE,
        CY_BLE_UUID_CHAR_DATE_OF_BIRTH,
        CY_BLE_UUID_CHAR_GENDER,
        CY_BLE_UUID_CHAR_WEIGHT,
        CY_BLE_UUID_CHAR_HEIGHT,
        CY_BLE_UUID_CHAR_VO2_MAX,
        CY_BLE_UUID_CHAR_HEART_RATE_MAX,
        CY_BLE_UUID_CHAR_RESTING_HEART_RATE,
        CY_BLE_UUID_CHAR_MRH,
        CY_BLE_UUID_CHAR_AEROBIC_THRESHOLD,
        CY_BLE_UUID_CHAR_ANAEROBIC_THRESHOLD,
        CY_BLE_UUID_CHAR_STP,
        CY_BLE_UUID_CHAR_DATE_OF_THRESHOLD_ASSESSMENT,
        CY_BLE_UUID_CHAR_WAIST_CIRCUMFERENCE,
        CY_BLE_UUID_CHAR_HIP_CIRCUNFERENCE,
        CY_BLE_UUID_CHAR_FBL,
        CY_BLE_UUID_CHAR_FBU,
        CY_BLE_UUID_CHAR_AEL,
        CY_BLE_UUID_CHAR_AEU,
        CY_BLE_UUID_CHAR_ANL,
        CY_BLE_UUID_CHAR_ANU,
        CY_BLE_UUID_CHAR_FIVE_ZONE_HEART_RATE_LIMITS,
        CY_BLE_UUID_CHAR_THREE_ZONE_HEART_RATE_LIMITS,
        CY_BLE_UUID_CHAR_TWO_ZONE_HEART_RATE_LIMIT,
        CY_BLE_UUID_CHAR_DATABASE_CHANGE_INCREMENT,
        CY_BLE_UUID_CHAR_USER_INDEX,
        CY_BLE_UUID_CHAR_USER_CONTROL_POINT,
        CY_BLE_UUID_CHAR_LANGUAGE
    };
    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_UDS))
    {
        /* Update last characteristic endHandle to declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        for(i = 0u; i < (uint32_t)CY_BLE_UDS_CHAR_COUNT; i++)
        {
            if(cy_ble_udscCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_udsc[discIdx].charInfo[i].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_udsc[discIdx].charInfo[i].valueHandle = discCharInfo->valueHandle;
                    cy_ble_udsc[discIdx].charInfo[i].properties = discCharInfo->properties;
                    lastEndHandle[discIdx] = &cy_ble_udsc[discIdx].charInfo[i].endHandle;
                }
                else
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo->uuid);
                }
            }
        }

        /* Init characteristic endHandle to Service endHandle.
         * Characteristic endHandle will be updated to the declaration
         * Handler of the following characteristic,
         * in the following characteristic discovery procedure. */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = cy_ble_serverInfo[discIdx][CY_BLE_SRVI_UDS].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_UDSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to the characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_UDSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t notSupportedDescr = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t descIdx;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_UDS)
    {
        switch(discDescrInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_CLIENT_CONFIG:
                descIdx = (uint32_t)CY_BLE_UDS_CCCD;
                break;

            default:
                /* Not supported descriptor */
                notSupportedDescr = 1u;
                break;
        }

        if(notSupportedDescr == 0u)
        {
            if(cy_ble_udsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] ==
               CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_udsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] =
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
* Function Name: Cy_BLE_UDSC_GetCharRange
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
static void Cy_BLE_UDSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_UDS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_UDS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if((cy_ble_udsc[discIdx].charInfo[charIdx].endHandle - cy_ble_udsc[discIdx].charInfo[charIdx].valueHandle) != 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_udsc[discIdx].charInfo[charIdx].valueHandle + 1u;
                charRangeInfo->range.endHandle = cy_ble_udsc[discIdx].charInfo[charIdx].endHandle;
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
* Function Name: Cy_BLE_UDSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles the Notification Event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_UDSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_UDS_ApplCallback != NULL) &&
       (cy_ble_udsc[discIdx].charInfo[CY_BLE_UDS_DCI].valueHandle == eventParam->handleValPair.attrHandle))
    {
        cy_stc_ble_uds_char_value_t locCharValue =
        {
            .connHandle = eventParam->connHandle,
            .charIndex  = CY_BLE_UDS_DCI,
            .value      = &eventParam->handleValPair.value
        };

        Cy_BLE_UDS_ApplCallback((uint32_t)CY_BLE_EVT_UDSC_NOTIFICATION, &locCharValue);
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_UDSC_IndicationEventHandler
***************************************************************************//**
*
*  Handles the Indication Event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_UDSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_UDS_ApplCallback != NULL))
    {
        if(cy_ble_udsc[discIdx].charInfo[CY_BLE_UDS_UCP].valueHandle == eventParam->handleValPair.attrHandle)
        {
            cy_stc_ble_uds_char_value_t locCharValue =
            {
                .charIndex  = CY_BLE_UDS_UCP,
                .connHandle = eventParam->connHandle,
                .value      = &eventParam->handleValPair.value
            };
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_UDS_ApplCallback((uint32_t)CY_BLE_EVT_UDSC_INDICATION, &locCharValue);
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_UDSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles the Read Response Event.
*
*  \param eventParam - The pointer to the data structure.
*
******************************************************************************/
static void Cy_BLE_UDSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_udscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(Cy_BLE_UDS_ApplCallback != NULL)
        {
            cy_en_ble_uds_char_index_t i;

            for(i = CY_BLE_UDS_FNM; i < CY_BLE_UDS_CHAR_COUNT; i++)
            {
                if(cy_ble_udsc[discIdx].charInfo[i].valueHandle == cy_ble_udscReqHandle[discIdx])
                {
                    cy_stc_ble_uds_char_value_t locCharValue =
                    {
                        .connHandle = eventParam->connHandle,
                        .charIndex  = i,
                        .value      = &eventParam->value
                    };

                    Cy_BLE_UDS_ApplCallback((uint32_t)CY_BLE_EVT_UDSC_READ_CHAR_RESPONSE, &locCharValue);
                    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                    break;
                }
            }

            if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) != 0u)
            {
                for(i = CY_BLE_UDS_FNM; i < CY_BLE_UDS_CHAR_COUNT; i++)
                {
                    if(cy_ble_udsc[discIdx].charInfo[i].descrHandle[CY_BLE_UDS_CCCD] == cy_ble_udscReqHandle[discIdx])
                    {
                        cy_stc_ble_uds_descr_value_t locDescrValue =
                        {
                            .connHandle = eventParam->connHandle,
                            .charIndex  = i,
                            .descrIndex = CY_BLE_UDS_CCCD,
                            .value      = &eventParam->value
                        };

                        Cy_BLE_UDS_ApplCallback((uint32_t)CY_BLE_EVT_UDSC_READ_DESCR_RESPONSE, &locDescrValue);
                        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                        break;
                    }
                }
            }
        }

        cy_ble_udscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_UDSC_ReadLongRespEventHandler
***************************************************************************//**
*
*  Handles a Read Long Response event.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
*
******************************************************************************/
static void Cy_BLE_UDSC_ReadLongRespEventHandler(const cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    cy_stc_ble_gattc_stop_cmd_param_t stopCmdParam = { .connHandle = eventParam->connHandle };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_UDS_ApplCallback != NULL) &&
       (cy_ble_udscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        cy_stc_ble_uds_char_value_t locCharValue = { .connHandle = eventParam->connHandle };
        cy_stc_ble_gatt_value_t locGattValue;

        /* Go trough all long characteristics */
        for(locCharValue.charIndex = CY_BLE_UDS_FNM; locCharValue.charIndex < CY_BLE_UDS_CHAR_COUNT;
            locCharValue.charIndex++)
        {
            if(locCharValue.charIndex == CY_BLE_UDS_AGE)
            {
                locCharValue.charIndex = CY_BLE_UDS_LNG;
            }

            if(cy_ble_udsc[discIdx].charInfo[locCharValue.charIndex].valueHandle == cy_ble_udscReqHandle[discIdx])
            {
                uint32_t i;

                /* Update user buffer with received data */
                for(i = 0u; i < eventParam->value.len; i++)
                {
                    if(cy_ble_udscCurrLen[discIdx] < cy_ble_udscRdLongBuffLen[discIdx])
                    {
                        cy_ble_udscRdLongBuffPtr[discIdx][cy_ble_udscCurrLen[discIdx]] = eventParam->value.val[i];
                        cy_ble_udscCurrLen[discIdx]++;
                    }
                    else
                    {
                        (void)Cy_BLE_GATTC_StopCmd(&stopCmdParam);
                        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                        break;
                    }
                }

                if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) != 0u)
                {
                    cy_stc_ble_gatt_xchg_mtu_param_t mtuParam =
                    {
                        .connHandle = eventParam->connHandle
                    };

                    (void)Cy_BLE_GATT_GetMtuSize(&mtuParam);

                    /* If received data length is less than MTU size, Read Long
                     * request is completed or provided user's buffer is full.
                     */
                    if(((mtuParam.mtu - 1u) > eventParam->value.len))
                    {
                        locGattValue.val = cy_ble_udscRdLongBuffPtr[discIdx];
                        locGattValue.len = cy_ble_udscCurrLen[discIdx];
                        locCharValue.value = &locGattValue;
                        Cy_BLE_UDS_ApplCallback((uint32_t)CY_BLE_EVT_UDSC_READ_CHAR_RESPONSE, &locCharValue);
                        cy_ble_udscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                    }

                    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                }

                break;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_UDSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles the Write Response Event.
*
*  \param eventParam: The pointer to a data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_UDSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_UDS_ApplCallback != NULL) &&
       (cy_ble_udscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_udsc[discIdx].charInfo[CY_BLE_UDS_UCP].valueHandle == cy_ble_udscReqHandle[discIdx])
        {
            cy_stc_ble_uds_char_value_t locCharIndex =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_UDS_UCP,
                .value      = NULL
            };

            cy_ble_udscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_UDS_ApplCallback((uint32_t)CY_BLE_EVT_UDSC_WRITE_CHAR_RESPONSE, &locCharIndex);
        }
        else
        {
            cy_en_ble_uds_char_index_t i;

            for(i = CY_BLE_UDS_FNM; i < CY_BLE_UDS_CHAR_COUNT; i++)
            {
                if(cy_ble_udsc[discIdx].charInfo[i].descrHandle[CY_BLE_UDS_CCCD] == cy_ble_udscReqHandle[discIdx])
                {
                    cy_stc_ble_uds_descr_value_t locDescIndex =
                    {
                        .connHandle = *eventParam,
                        .charIndex  = i,
                        .descrIndex = CY_BLE_UDS_CCCD,
                        .value      = NULL
                    };

                    cy_ble_udscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                    Cy_BLE_UDS_ApplCallback((uint32_t)CY_BLE_EVT_UDSC_WRITE_DESCR_RESPONSE, &locDescIndex);
                    break;
                }
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_UDSC_ExecuteWriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Execute Write Response event for the User Data Service.
*
*  \param eventParam: The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_UDSC_ExecuteWriteResponseEventHandler(const cy_stc_ble_gattc_exec_write_rsp_t *eventParam)
{
    uint32_t i;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_UDS_ApplCallback != NULL) &&
       (cy_ble_udscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(i = 0u; i < ((uint8_t)CY_BLE_UDS_CHAR_COUNT); i++)
        {
            if(cy_ble_udsc[discIdx].charInfo[i].valueHandle == cy_ble_udscReqHandle[discIdx])
            {
                cy_stc_ble_uds_char_value_t locCharVal =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = (cy_en_ble_uds_char_index_t)i,
                    .value      = NULL
                };

                cy_ble_udscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_UDS_ApplCallback((uint32_t)CY_BLE_EVT_UDSC_WRITE_CHAR_RESPONSE, &locCharVal);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_UDSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles the Error Response Event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
*
******************************************************************************/
static void Cy_BLE_UDSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
        if(cy_ble_udscReqHandle[discIdx] == eventParam->errInfo.attrHandle)
        {
            if((eventParam->errInfo.opCode == CY_BLE_GATT_READ_BLOB_REQ) &&
               (eventParam->errInfo.errorCode == CY_BLE_GATT_ERR_ATTRIBUTE_NOT_LONG))
            {
                cy_stc_ble_gattc_read_req_t readReqParam =
                {
                    .attrHandle = eventParam->errInfo.attrHandle,
                    .connHandle = eventParam->connHandle
                };

                (void)Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            }
            else
            {
                if(Cy_BLE_UDS_ApplCallback != NULL)
                {
                    cy_stc_ble_uds_char_value_t locGattError =
                    {
                        .gattErrorCode = eventParam->errInfo.errorCode,
                        .connHandle    = eventParam->connHandle,
                        .value         = NULL
                    };

                    for(locGattError.charIndex = CY_BLE_UDS_FNM; locGattError.charIndex < CY_BLE_UDS_CHAR_COUNT;
                        locGattError.charIndex++)
                    {
                        if(cy_ble_udsc[discIdx].charInfo[locGattError.charIndex].valueHandle ==
                           eventParam->errInfo.attrHandle)
                        {
                            Cy_BLE_UDS_ApplCallback((uint32_t)CY_BLE_EVT_UDSC_ERROR_RESPONSE, &locGattError);
                            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                            break;
                        }
                    }
                }

                cy_ble_udscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    }
}


#endif /* CY_BLE_UDS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_UDS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the User Data Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_UDS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_UDS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_UDSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_UDSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_UDSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_UDS_CLIENT */

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
        #ifdef CY_BLE_UDS_SERVER
            case CY_BLE_EVT_GATT_DISCONNECT_IND:
                (void)Cy_BLE_UDS_Init(cy_ble_udsConfigPtr);
                break;

            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_UDSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF:
                Cy_BLE_UDSS_ConfirmationEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_PREP_WRITE_REQ:
                Cy_BLE_UDSS_PrepareWriteRequestEventHandler((cy_stc_ble_gatts_prep_write_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_EXEC_WRITE_REQ:
                Cy_BLE_UDSS_ExecuteWriteRequestEventHandler((cy_stc_ble_gatts_exec_write_req_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ:
                Cy_BLE_UDSS_ReadRequestEventHandler((cy_stc_ble_gatts_char_val_read_req_t*)eventParam);
                break;
        #endif /* CY_BLE_UDS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_UDS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_UDSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_UDSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_IND:
                Cy_BLE_UDSC_IndicationEventHandler((cy_stc_ble_gattc_handle_value_ind_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_UDSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_UDSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_BLOB_RSP:
                Cy_BLE_UDSC_ReadLongRespEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_EXEC_WRITE_RSP:
                Cy_BLE_UDSC_ExecuteWriteResponseEventHandler((cy_stc_ble_gattc_exec_write_rsp_t*)eventParam);
                break;
        #endif /* CY_BLE_UDS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_UDS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
