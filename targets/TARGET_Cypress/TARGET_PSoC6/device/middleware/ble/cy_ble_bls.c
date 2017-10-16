/***************************************************************************//**
* \file cy_ble_bls.c
* \version 2.0
*
* \brief
*  This file contains the source code for the Blood Pressure Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#include "cy_ble_event_handler.h"

/* The C binding of definitions if building with the C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_BLS))

static cy_ble_callback_t Cy_BLE_BLS_ApplCallback;

#ifdef CY_BLE_BLS_SERVER
/* The internal storage for the last request handle to check response for server */
static cy_ble_gatt_db_attr_handle_t cy_ble_blssReqHandle;
#endif /* CY_BLE_BLS_SERVER */

#ifdef CY_BLE_BLS_CLIENT
/* Server's Blood Pressure Service characteristics GATT DB handles structure */
cy_stc_ble_blsc_t cy_ble_blsc[CY_BLE_CONFIG_GATTC_COUNT];

/* Internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_blscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_BLS_CLIENT */

/* The pointer to the global BLE BLS config structure */
cy_stc_ble_bls_config_t *cy_ble_blsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_BLS_SERVER
static void Cy_BLE_BLSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static cy_en_ble_gatt_err_code_t Cy_BLE_BLSS_WriteEventHandler(const cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* CY_BLE_BLS_SERVER */

#ifdef CY_BLE_BLS_CLIENT
static void Cy_BLE_BLSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_BLSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_BLSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_BLSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_BLSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam);
static void Cy_BLE_BLSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_BLSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_BLSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* CY_BLE_BLS_CLIENT */
static cy_en_ble_gatt_err_code_t Cy_BLE_BLS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_BLS_Init
***************************************************************************//**
*
*  This function initializes the BLS Service.
*
*  \param config: The configuration structure for the Blood Pressure Service.
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
cy_en_ble_api_result_t Cy_BLE_BLS_Init(cy_stc_ble_bls_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to the config structure */
        cy_ble_blsConfigPtr = config;

        /* Registers Event Handler for the BLS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_BLS_EventHandler);

        /* Registers a callback function via the config structure */
        if(cy_ble_blsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_BLS_ApplCallback = cy_ble_blsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_BLS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_BLS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_blsc[idx], 0, sizeof(cy_stc_ble_blsc_t));

                    /* Initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_BLS].uuid = CY_BLE_UUID_BLOOD_PRESSURE_SERVICE;
                }
                cy_ble_blscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_GATT_ROLE_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BLS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific Write requests from the peer device will not be handled with
*  an unregistered callback function.
*
*  \param callbackFunc: An application layer event callback function to receive
*                    events from the BLE Component. The definition of
*                    cy_ble_callback_t for Blood Pressure Service is:\n
*                    typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                     void *eventParam)
*                    * eventCode indicates the event that triggered this
*                      callback (e.g. CY_BLE_EVT_BASS_NOTIFICATION_ENABLED)
*                    * eventParam contains the parameters corresponding to the
*                      current event (e.g. the pointer to the cy_stc_ble_bls_char_value_t
*                      structure that contains details of the characteristic
*                      the CY_BLE_EVT_BLSS_NOTIFICATION_ENABLED event triggered for).
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
cy_en_ble_api_result_t Cy_BLE_BLS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_BLS_ApplCallback = callbackFunc;
    if(cy_ble_blsConfigPtr != NULL)
    {
        cy_ble_blsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_BLS_SERVER


/******************************************************************************
* Function Name: Cy_BLE_BLSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets a value of the characteristic which is identified by charIndex.
*
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*              stored to the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BLSS_SetCharacteristicValue(cy_en_ble_bls_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if(charIndex >= CY_BLE_BLS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_blsConfigPtr->blss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Store the characteristic value into the GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_blsConfigPtr->blss->charInfo[charIndex].charHandle,
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
* Function Name: Cy_BLE_BLSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets a characteristic value of the Blood Pressure Service identified by
*  charIndex.
*
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               in the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BLSS_GetCharacteristicValue(cy_en_ble_bls_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    /* Check the parameters */
    if(charIndex >= CY_BLE_BLS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_blsConfigPtr->blss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get the characteristic value from the GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_blsConfigPtr->blss->charInfo[charIndex].charHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
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
* Function Name: Cy_BLE_BLSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets a characteristic descriptor of a specified characteristic of the Blood
*  Pressure Service from the local GATT database.
*
*  \param connHandle: The connection handle
*  \param charIndex: The index of the characteristic.
*  \param descrIndex: The index of the characteristic descriptor.
*  \param attrSize: The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the location where the characteristic descriptor
*               value data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional descriptor is absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BLSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bls_char_index_t charIndex,
                                                               cy_en_ble_bls_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    /* Check the parameters */
    if((charIndex >= CY_BLE_BLS_CHAR_COUNT) || (descrIndex >= CY_BLE_BLS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_blsConfigPtr->blss->charInfo[charIndex].cccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get data from the database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_blsConfigPtr->blss->charInfo[charIndex].cccdHandle,
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
* Function Name: Cy_BLE_BLSS_SendNotification
***************************************************************************//**
*
*  Sends notification of the specified characteristic to the Client device.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  Handle Value Notification which results in CY_BLE_EVT_BLSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The connection handle which consists of the device ID and ATT
*               connection ID.
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the Client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the Client is not established.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed .
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the Client.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BLSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_bls_char_index_t charIndex,
                                                    uint8_t attrSize,
                                                    uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send notification if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(charIndex >= CY_BLE_BLS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_blsConfigPtr->blss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if(!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_blsConfigPtr->blss->charInfo[charIndex].cccdHandle))
    {
        apiResult = CY_BLE_ERROR_NTF_DISABLED;
    }
    else
    {
        /* Fill all fields of Write request structure ... */
        cy_stc_ble_gatts_handle_value_ntf_t ntfParam =
        {
            .handleValPair.attrHandle = cy_ble_blsConfigPtr->blss->charInfo[charIndex].charHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* Send notification to the Client using a previously filled structure */
        apiResult = Cy_BLE_GATTS_Notification(&ntfParam);
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BLSS_SendIndication
***************************************************************************//**
*
*  Sends an indication of the specified characteristic to the Client device.
*
*  On enabling indication successfully it sends out a Handle Value Indication which
*  results in a CY_BLE_EVT_BLSC_INDICATION or CY_BLE_EVT_GATTC_HANDLE_VALUE_IND (if the
*  service-specific callback function is not registered) event at the GATT Client's end.
*
*  \param connHandle: The connection handle which consists of the device ID and ATT
*               connection ID.
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the Client is not established.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_IND_DISABLED - Indication is not enabled by the Client.
*
* \events
*  If execution is successful(return value = CY_BLE_SUCCESS),
*  these events can appear: \n
*   If a BLS service-specific callback is registered
*      (with Cy_BLE_BLS_RegisterAttrCallback):
*  * CY_BLE_EVT_BLSS_INDICATION_CONFIRMED - if the indication is
*                                successfully delivered to the peer device.
*  .
*   Otherwise (if a BLS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF - if the indication is
*                                successfully delivered to the peer device.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BLSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_bls_char_index_t charIndex,
                                                  uint8_t attrSize,
                                                  uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send indication if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(charIndex >= CY_BLE_BLS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(charIndex != CY_BLE_BLS_BPM)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else if(!CY_BLE_IS_INDICATION_ENABLED(connHandle.attId, cy_ble_blsConfigPtr->blss->charInfo[charIndex].cccdHandle))
    {
        apiResult = CY_BLE_ERROR_IND_DISABLED;
    }
    else
    {
        /* Fill all fields of the Write request structure ... */
        cy_stc_ble_gatts_handle_value_ind_t indParam =
        {
            .handleValPair.attrHandle = cy_ble_blsConfigPtr->blss->charInfo[charIndex].charHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* Send indication to the Client using a previously filled structure */
        apiResult = Cy_BLE_GATTS_Indication(&indParam);

        /* Save the handle to support a service-specific value confirmation response from the Client */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_blssReqHandle = indParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BLSS_ConfirmationEventHandler
***************************************************************************//**
*
*  Handles a Value Confirmation request event from the BLE stack.
*
*  \param eventParam - The pointer to a structure of type cy_stc_ble_conn_handle_t.
*
******************************************************************************/
static void Cy_BLE_BLSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    if((Cy_BLE_BLS_ApplCallback != NULL) &&
       (cy_ble_blsConfigPtr->blss->charInfo[CY_BLE_BLS_BPM].charHandle == cy_ble_blssReqHandle))
    {
        cy_stc_ble_bls_char_value_t locCharIndex =
        {
            .connHandle = *eventParam,
            .charIndex  = CY_BLE_BLS_BPM,
            .value      = NULL
        };
        cy_ble_blssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        Cy_BLE_BLS_ApplCallback((uint32_t)CY_BLE_EVT_BLSS_INDICATION_CONFIRMED, &locCharIndex);
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BLSS_WriteEventHandler
***************************************************************************//**
*
*  Handles a Write Request event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*   cy_en_ble_gatt_err_code_t - The function result state if it succeeded
*                            (CY_BLE_GATT_ERR_NONE) or GATT error codes
*                            returned by Cy_BLE_GATTS_WriteAttributeValueCCCD().
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_BLSS_WriteEventHandler(const cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;

    if(Cy_BLE_BLS_ApplCallback != NULL)
    {
        cy_stc_ble_bls_char_value_t locCharIndex;
        locCharIndex.connHandle = eventParam->connHandle;
        locCharIndex.value = NULL;

        for(locCharIndex.charIndex = CY_BLE_BLS_BPM; locCharIndex.charIndex < CY_BLE_BLS_BPF; locCharIndex.charIndex++)
        {
            if(eventParam->handleValPair.attrHandle == cy_ble_blsConfigPtr->blss->charInfo[locCharIndex.charIndex].
                cccdHandle)
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
                    uint32_t eventCode;

                    if(locCharIndex.charIndex == CY_BLE_BLS_ICP)
                    {
                        if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                        {
                            eventCode = (uint32_t)CY_BLE_EVT_BLSS_NOTIFICATION_ENABLED;
                        }
                        else
                        {
                            eventCode = (uint32_t)CY_BLE_EVT_BLSS_NOTIFICATION_DISABLED;
                        }
                    }
                    else
                    {
                        if(CY_BLE_IS_INDICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                        {
                            eventCode = (uint32_t)CY_BLE_EVT_BLSS_INDICATION_ENABLED;
                        }
                        else
                        {
                            eventCode = (uint32_t)CY_BLE_EVT_BLSS_INDICATION_DISABLED;
                        }
                    }
                #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
                    (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
                    /* Set a flag to store bonding data to flash */
                    if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
                    {
                        cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
                    }
                #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */

                    Cy_BLE_BLS_ApplCallback(eventCode, &locCharIndex);
                }

                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;
            }
        }
    }

    return(gattErr);
}

#endif /* CY_BLE_BLS_SERVER */

#ifdef CY_BLE_BLS_CLIENT


/******************************************************************************
* Function Name: Cy_BLE_BLSC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read the characteristic value from the Server
*  identified by charIndex.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The read request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the Server is not established.
*  * CY_BLE_ERROR_INVALID_OPERATION - The operation is invalid for this
*                                     characteristic.
*
* \events
*  If execution is successful(return value = CY_BLE_SUCCESS)
*  these events can appear: \n
*   If a BLS service specific callback is registered
*      (with Cy_BLE_BLS_RegisterAttrCallback):
*  * CY_BLE_EVT_BLSC_READ_CHAR_RESPONSE - if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_bls_char_value_t.
*  .
*   Otherwise (if a BLS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP - if the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with the event parameters
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_ERROR_RSP - if there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with the event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BLSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_bls_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_BLS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_blsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_READ & cy_ble_blsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_blsc[discIdx].charInfo[charIndex].valueHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_blscReqHandle[discIdx] = cy_ble_blsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BLSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sends a request to set the characteristic descriptor of the specified Blood Pressure
*  Service characteristic to the Server device.
*
*  Internally, a Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server's side the following events can be
*  generated:
*  * CY_BLE_EVT_BLSS_INDICATION_ENABLED
*  * CY_BLE_EVT_BLSS_INDICATION_DISABLED
*  * CY_BLE_EVT_BLSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_BLSS_NOTIFICATION_DISABLED
*
*  \param connHandle: The BLE peer device connection handle.
*  \param charIndex: The index of the service characteristic.
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param attrSize: The size of the characteristic descriptor value attribute.
*  \param attrValue: The pointer to the characteristic descriptor value data that should
*               be sent to the Server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic.
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute.
*
* \events
*  If execution is successful(return value = CY_BLE_SUCCESS),
*  the next events can appear: \n
*   If a BLS service-specific callback is registered
*      (with Cy_BLE_BLS_RegisterAttrCallback):
*  * CY_BLE_EVT_BLSC_WRITE_DESCR_RESPONSE - if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_bls_descr_value_t.
*  .
*   Otherwise (if the BLS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - if there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with the event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BLSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bls_char_index_t charIndex,
                                                               cy_en_ble_bls_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_BLS_CHAR_COUNT) || (descrIndex >= CY_BLE_BLS_DESCR_COUNT)
            || (attrSize != CY_BLE_CCCD_LEN) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_blsc[discIdx].charInfo[charIndex].cccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Fill all fields of a write request structure ... */
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_blsc[discIdx].charInfo[charIndex].cccdHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = CY_BLE_CCCD_LEN,
            .connHandle               = connHandle
        };
        /* ... and send a request to the Server device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

        /* Save the handle to support a service-specific Read response from the device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_blscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BLSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Sends a request to get the characteristic descriptor of the specified Blood Pressure
*  Service characteristic from the Server device. This function call can result
*  in the generation of the following events based on the response from the server
*  device:
*  * CY_BLE_EVT_BLSC_READ_DESCR_RESPONSE
*  * CY_BLE_EVT_GATTC_ERROR_RSP
*
*  \param connHandle: The BLE peer device connection handle.
*  \param charIndex: The index of a service characteristic.
*  \param descrIndex: The index of a service characteristic descriptor.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
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
*  If execution is successful (return value = CY_BLE_SUCCESS),
*  these events can appear: \n
*  If a BLS service-specific callback is registered
*      (with Cy_BLE_BLS_RegisterAttrCallback):
*  * CY_BLE_EVT_BLSC_READ_DESCR_RESPONSE - if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with an event parameter structure
*                                of type cy_stc_ble_bls_descr_value_t.
*  .
*  Otherwise (if a BLS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP - if the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with the event parameters
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_ERROR_RSP - if there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with the event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BLSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bls_char_index_t charIndex,
                                                               cy_en_ble_bls_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_BLS_CHAR_COUNT) || (descrIndex >= CY_BLE_BLS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_blsc[discIdx].charInfo[charIndex].cccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_blsc[discIdx].charInfo[charIndex].cccdHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_blscReqHandle[discIdx] = cy_ble_blsc[discIdx].charInfo[charIndex].cccdHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BLSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, the appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo:  The pointer to the characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_BLSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* Blood Pressure Service characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_blscCharUuid[CY_BLE_BLS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_BP_MSRMT,
        CY_BLE_UUID_CHAR_INTRMDT_CUFF_PRSR,
        CY_BLE_UUID_CHAR_BP_FEATURE
    };
    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_BLS))
    {
        /* Update the last characteristic endHandle to the declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        for(i = 0u; i < (uint32_t)CY_BLE_BLS_CHAR_COUNT; i++)
        {
            if(cy_ble_blscCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_blsc[discIdx].charInfo[i].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_blsc[discIdx].charInfo[i].valueHandle = discCharInfo->valueHandle;
                    cy_ble_blsc[discIdx].charInfo[i].properties = discCharInfo->properties;
                    lastEndHandle[discIdx] = &cy_ble_blsc[discIdx].charInfo[i].endHandle;
                }
                else
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo);
                }
            }
        }

        /* Initialize the characteristic endHandle to the Service endHandle.
         * The characteristic endHandle will be updated to the declaration
         * handle of the following characteristic,
         * in the following characteristic discovery procedure. */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = cy_ble_serverInfo[discIdx][CY_BLE_SRVI_BLS].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BLSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, the appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo:  The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_BLSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t * discDescrInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_BLS)
    {
        if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_CLIENT_CONFIG)
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if(cy_ble_blsc[discIdx].charInfo[charIdx].cccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_blsc[discIdx].charInfo[charIdx].cccdHandle = discDescrInfo->descrHandle;
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
* Function Name: Cy_BLE_BLSC_GetCharRange
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
static void Cy_BLE_BLSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_BLS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_BLS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if((cy_ble_blsc[discIdx].charInfo[charIdx].endHandle - cy_ble_blsc[discIdx].charInfo[charIdx].valueHandle) != 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_blsc[discIdx].charInfo[charIdx].valueHandle + 1u;
                charRangeInfo->range.endHandle = cy_ble_blsc[discIdx].charInfo[charIdx].endHandle;
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
* Function Name: Cy_BLE_BLSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles a Notification event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_BLSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_BLS_ApplCallback != NULL) &&
       (cy_ble_blsc[discIdx].charInfo[CY_BLE_BLS_ICP].valueHandle == eventParam->handleValPair.attrHandle))
    {
        cy_stc_ble_bls_char_value_t locCharValue =
        {
            .connHandle = eventParam->connHandle,
            .charIndex  = CY_BLE_BLS_ICP,
            .value      = &eventParam->handleValPair.value
        };
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        Cy_BLE_BLS_ApplCallback((uint32_t)CY_BLE_EVT_BLSC_NOTIFICATION, &locCharValue);
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BLSC_IndicationEventHandler
***************************************************************************//**
*
*  Handles an Indication event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_BLSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_BLS_ApplCallback != NULL) &&
       (cy_ble_blsc[discIdx].charInfo[CY_BLE_BLS_BPM].valueHandle == eventParam->handleValPair.attrHandle))
    {
        cy_stc_ble_bls_char_value_t locCharValue =
        {
            .charIndex  = CY_BLE_BLS_BPM,
            .connHandle = eventParam->connHandle,
            .value      = &eventParam->handleValPair.value
        };
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        Cy_BLE_BLS_ApplCallback((uint32_t)CY_BLE_EVT_BLSC_INDICATION, &locCharValue);
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BLSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles a Read Response event.
*
*  \param cy_stc_ble_gattc_read_rsp_param_t *eventParam: The pointer to the data structure.
*
******************************************************************************/
static void Cy_BLE_BLSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_BLS_ApplCallback != NULL) &&
       (cy_ble_blscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_blsc[discIdx].charInfo[CY_BLE_BLS_BPF].valueHandle == cy_ble_blscReqHandle[discIdx])
        {
            cy_stc_ble_bls_char_value_t locCharValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_BLS_BPF,
                .value      = &eventParam->value
            };
            cy_ble_blscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_BLS_ApplCallback((uint32_t)CY_BLE_EVT_BLSC_READ_CHAR_RESPONSE, &locCharValue);
        }
        else
        {
            cy_en_ble_bls_char_index_t i;

            for(i = CY_BLE_BLS_BPM; i < CY_BLE_BLS_BPF; i++)
            {
                if(cy_ble_blsc[discIdx].charInfo[i].cccdHandle == cy_ble_blscReqHandle[discIdx])
                {
                    cy_stc_ble_bls_descr_value_t locDescrValue =
                    {
                        .connHandle = eventParam->connHandle,
                        .charIndex  = i,
                        .descrIndex = CY_BLE_BLS_CCCD,
                        .value      = &eventParam->value
                    };
                    cy_ble_blscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                    Cy_BLE_BLS_ApplCallback((uint32_t)CY_BLE_EVT_BLSC_READ_DESCR_RESPONSE, &locDescrValue);
                    break;
                }
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BLSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Write Response event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_BLSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_BLS_ApplCallback != NULL) &&
       (cy_ble_blscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        cy_en_ble_bls_char_index_t i;
        for(i = CY_BLE_BLS_BPM; i < CY_BLE_BLS_BPF; i++)
        {
            if(cy_ble_blsc[discIdx].charInfo[i].cccdHandle == cy_ble_blscReqHandle[discIdx])
            {
                cy_stc_ble_bls_descr_value_t locDescIndex =
                {
                    .connHandle = *eventParam,
                    .charIndex  = i,
                    .descrIndex = CY_BLE_BLS_CCCD,
                    .value      = NULL
                };
                cy_ble_blscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                Cy_BLE_BLS_ApplCallback((uint32_t)CY_BLE_EVT_BLSC_WRITE_DESCR_RESPONSE, &locDescIndex);
                break;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BLSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles an Error Response event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_BLSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        if(cy_ble_blscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] == eventParam->errInfo.attrHandle)
        {
            cy_ble_blscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


#endif /* CY_BLE_BLS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_BLSS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Blood Pressure Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_BLS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_BLS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_BLSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_BLSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_BLSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_BLS_CLIENT */

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
        #ifdef CY_BLE_BLS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_BLSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF:
                Cy_BLE_BLSS_ConfirmationEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_BLS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_BLS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode != CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_BLSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_BLSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_IND:
                Cy_BLE_BLSC_IndicationEventHandler((cy_stc_ble_gattc_handle_value_ind_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_BLSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_BLSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_BLS_CLIENT */
            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_BLS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
