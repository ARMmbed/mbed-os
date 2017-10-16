/***************************************************************************//**
* \file cy_ble_hrs.c
* \version 2.0
*
* \brief
*  This file contains the source code for the Heart Rate Service of
*  the BLE Component.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_HRS))

static cy_ble_callback_t Cy_BLE_HRS_ApplCallback;

#ifdef CY_BLE_HRS_CLIENT
/* Server's Heart Rate Service characteristics GATT DB handles structure */
cy_stc_ble_hrsc_t cy_ble_hrsc[CY_BLE_CONFIG_GATTC_COUNT];

/* Internal storage for last request handle to check the response */
static cy_ble_gatt_db_attr_handle_t cy_ble_hrscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_HRS_CLIENT */

/* The pointer on global BLE HRS Config structure */
cy_stc_ble_hrs_config_t *cy_ble_hrsConfigPtr = NULL;


/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_HRS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_HRSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* CY_BLE_HRS_SERVER */

#ifdef CY_BLE_HRS_CLIENT
static void Cy_BLE_HRSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_HRSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_HRSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_HRSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_HRSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_HRSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_HRSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* CY_BLE_HRS_CLIENT */
static cy_en_ble_gatt_err_code_t Cy_BLE_HRS_EventHandler(uint32_t eventCode, void *eventParam);

/******************************************************************************
* Function Name: Cy_BLE_HRS_Init
***************************************************************************//**
*
*  Initializes the profile internals.
*
*  \param config: Configuration structure for the Heart Rate Service.
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
cy_en_ble_api_result_t Cy_BLE_HRS_Init(cy_stc_ble_hrs_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure*/
        cy_ble_hrsConfigPtr = config;

        /* Registers Event Handler for the HRS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_HRS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_hrsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_HRS_ApplCallback = cy_ble_hrsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_HRS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_HRS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_hrsc[idx], 0, sizeof(cy_stc_ble_hrsc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_HRS].uuid = CY_BLE_UUID_HEART_RATE_SERVICE;
                }
                cy_ble_hrscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_GATT_ROLE_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HRS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc: An application layer event callback function to receive
*                    events from the BLE Component. The definition of
*                    cy_ble_callback_t for HRS Service is: \n
*                    typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                       void *eventParam)
*                    * eventCode indicates the event that triggered this
*                      callback (e.g. CY_BLE_EVT_HRSS_NOTIFICATION_ENABLED).
*                    * eventParam contains the parameters corresponding to the
*                      current event. (e.g. pointer to cy_stc_ble_hrs_char_value_t
*                      structure that contains details of the characteristic
*                      for which notification enabled event was triggered).
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
cy_en_ble_api_result_t Cy_BLE_HRS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_HRS_ApplCallback = callbackFunc;
    if(cy_ble_hrsConfigPtr != NULL)
    {
        cy_ble_hrsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_HRS_SERVER


/******************************************************************************
* Function Name: Cy_BLE_HRSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets local characteristic value of the specified Heart Rate Service
*  characteristic.
*
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*                   stored in the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HRSS_SetCharacteristicValue(cy_en_ble_hrs_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if(charIndex >= CY_BLE_HRS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if((charIndex == CY_BLE_HRS_BSL) && (!CY_BLE_HRS_IS_BSL_SUPPORTED))
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Store characteristic value into GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .handleValuePair.attrHandle = cy_ble_hrsConfigPtr->hrss->charHandle[charIndex],
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
* Function Name: Cy_BLE_HRSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets the local characteristic value of specified Heart Rate Service
*  characteristic.
*
*  \param charIndex:  The index of a service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue: The pointer to the location where characteristic value data
*              should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HRSS_GetCharacteristicValue(cy_en_ble_hrs_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if(charIndex >= CY_BLE_HRS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if((charIndex == CY_BLE_HRS_BSL) && (!CY_BLE_HRS_IS_BSL_SUPPORTED))
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get characteristic value from GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_hrsConfigPtr->hrss->charHandle[charIndex],
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .flags                      = CY_BLE_GATT_DB_READ | CY_BLE_GATT_DB_LOCALLY_INITIATED
        };

        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HRSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets the local characteristic descriptor of the specified Heart Rate
*  Service characteristic.
*
*  \param connHandle: The connection handle
*  \param charIndex:  The index of the characteristic.
*  \param descrIndex: The index of the descriptor.
*  \param attrSize:   The size of the descriptor value attribute. The Heart Rate
*              Measurement characteristic client configuration descriptor has
*              2 bytes length.
*  \param attrValue: The pointer to the location where characteristic descriptor
*              value data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional descriptor is absent
*
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HRSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hrs_char_index_t charIndex,
                                                               cy_en_ble_hrs_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    if((charIndex >= CY_BLE_HRS_CHAR_COUNT) || (descrIndex >= CY_BLE_HRS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if((charIndex != CY_BLE_HRS_HRM) || (descrIndex != CY_BLE_HRS_HRM_CCCD))
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get data from database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_hrsConfigPtr->hrss->hrmCccdHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .connHandle                 = connHandle,
            .flags                      = CY_BLE_GATT_DB_READ | CY_BLE_GATT_DB_LOCALLY_INITIATED
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
* Function Name: Cy_BLE_HRSS_SendNotification
***************************************************************************//**
*
*  Sends notification of a specified Heart Rate Service characteristic value
*  to the Client device. No response is expected.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_HRSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The connection handle which consist of the device ID and ATT
*              connection ID.
*  \param charIndex:  The index of a service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*              sent to the client device.
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
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HRSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_hrs_char_index_t charIndex,
                                                    uint8_t attrSize,
                                                    uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send Notification if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(charIndex >= CY_BLE_HRS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if((cy_ble_hrsConfigPtr->hrss->hrmCccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            || (!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_hrsConfigPtr->hrss->hrmCccdHandle)))
    {
        apiResult = CY_BLE_ERROR_NTF_DISABLED;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
        {
            .handleValPair.attrHandle = cy_ble_hrsConfigPtr->hrss->charHandle[charIndex],
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
* Function Name: Cy_BLE_HRSS_WriteEventHandler
***************************************************************************//**
*
*  Handles the Heart Rate Measurement Client Configuration Characteristic
*  Descriptor Write Event or Control Point Characteristic Write Event.
*
*  \param void *eventParam: The pointer to the data structure specified by the event.
*
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_HRSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    uint32_t eventCode = 0u;
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    cy_stc_ble_hrs_char_value_t locCharIndex;

    locCharIndex.connHandle = eventParam->connHandle;
    locCharIndex.value = NULL;

    if(eventParam->handleValPair.attrHandle == cy_ble_hrsConfigPtr->hrss->hrmCccdHandle)
    {
        locCharIndex.charIndex = CY_BLE_HRS_HRM;
        /* Heart Rate Measurement characteristic descriptor write request */
        if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
        {
            eventCode = (uint32_t)CY_BLE_EVT_HRSS_NOTIFICATION_ENABLED;
        }
        else
        {
            eventCode = (uint32_t)CY_BLE_EVT_HRSS_NOTIFICATION_DISABLED;
        }

    #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
        /* Set flag to store bonding data to flash */
        if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
        {
            cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
        }
    #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */

        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
    else if(eventParam->handleValPair.attrHandle == cy_ble_hrsConfigPtr->hrss->charHandle[CY_BLE_HRS_CPT])
    {
        locCharIndex.charIndex = CY_BLE_HRS_CPT;
        /* Heart Rate Control Point characteristic write request */
        if(CY_BLE_HRS_RESET_ENERGY_EXPENDED == eventParam->handleValPair.value.val[0u])
        {
            eventCode = (uint32_t)CY_BLE_EVT_HRSS_ENERGY_EXPENDED_RESET;
        }
        else
        {
            gattErr = CY_BLE_GATT_ERR_HEART_RATE_CONTROL_POINT_NOT_SUPPORTED;
        }

        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
    else
    {
        /* Heart Rate Service doesn't support any other write requests */
    }

    if(0u != eventCode)
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
            if(Cy_BLE_HRS_ApplCallback != NULL)
            {
                Cy_BLE_HRS_ApplCallback(eventCode, &locCharIndex);
            }
            else
            {
                Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTS_WRITE_REQ, eventParam);
            }
        }
    }

    return(gattErr);
}

#endif /* CY_BLE_HRS_SERVER */

#ifdef CY_BLE_HRS_CLIENT


/******************************************************************************
* Function Name: Cy_BLE_HRSC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write the characteristic value attribute
*  (identified by charIndex) to the server. The Write Response just confirms
*  the operation success.
*
*  This function call can result in generation of the following events based on
*  the response from the server device:
*  * CY_BLE_EVT_HRSC_WRITE_CHAR_RESPONSE
*  * CY_BLE_EVT_GATTC_ERROR_RSP
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a service characteristic.
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
*   If the HRS service-specific callback is registered
*      (with Cy_BLE_HRS_RegisterAttrCallback):
*  * CY_BLE_EVT_HRSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_hrs_char_value_t.
*  .
*   Otherwise (if the HRS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HRSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_hrs_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_HRS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT) ||
            ((charIndex == CY_BLE_HRS_CPT) && (attrSize > CY_BLE_HRS_CPT_CHAR_LEN)))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_hrsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_WRITE & cy_ble_hrsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_hrsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_hrscReqHandle[discIdx] = cy_ble_hrsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HRSC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read the characteristic Value from a server
*  which is identified by charIndex.
*
*  The Read Response returns the characteristic Value in the Attribute Value
*  parameter.
*
*  The Read Response only contains the characteristic Value that is less than or
*  equal to (MTU - 1) octets in length. If the characteristic Value is greater
*  than (MTU - 1) octets in length, the Read Long Characteristic Value procedure
*  may be used if the rest of the characteristic Value is required.
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
*   If the HRS service-specific callback is registered
*      (with Cy_BLE_HRS_RegisterAttrCallback):
*  * CY_BLE_EVT_HRSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_hrs_char_value_t.
*  .
*   Otherwise (if the HRS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_HRSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_hrs_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_HRS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_hrsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_READ & cy_ble_hrsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_hrsc[discIdx].charInfo[charIndex].valueHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_hrscReqHandle[discIdx] = cy_ble_hrsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HRSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  This function is used to write the characteristic Value to the server, which
*  is identified by charIndex.
*
*  This function call can result in generation of the following events based on
*  the response from the server device:
*  * CY_BLE_EVT_HRSC_WRITE_DESCR_RESPONSE
*  * CY_BLE_EVT_GATTC_ERROR_RSP
*
*  One of the following events is received by the peer device, on invoking
*  this function:
*  * CY_BLE_EVT_HRSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_HRSS_NOTIFICATION_DISABLED
*  * CY_BLE_EVT_HRSS_ENERGY_EXPENDED_RESET
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param attrSize: The size of the characteristic descriptor value attribute.
*  \param attrValue: The pointer to the characteristic descriptor value data that
*               should be sent to the server device.
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
*   If the HRS service-specific callback is registered
*      (with Cy_BLE_HRS_RegisterAttrCallback):
*  * CY_BLE_EVT_HRSC_WRITE_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_hrs_descr_value_t.
*  .
*   Otherwise (if the HRS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HRSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hrs_char_index_t charIndex,
                                                               cy_en_ble_hrs_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_HRS_CHAR_COUNT) || (descrIndex >= CY_BLE_HRS_DESCR_COUNT) ||
            (attrSize != CY_BLE_CCCD_LEN) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(charIndex != CY_BLE_HRS_HRM)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else if(cy_ble_hrsc[discIdx].hrmCccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_hrsc[discIdx].hrmCccdHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = CY_BLE_CCCD_LEN,
            .connHandle               = connHandle
        };

        /* ... and send request to server device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_hrscReqHandle[discIdx] = cy_ble_hrsc[discIdx].hrmCccdHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HRSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets a characteristic descriptor of a specified characteristic of the service.
*
*  This function call can result in generation of the following events based on
*  the response from the server device:
*  * CY_BLE_EVT_HRSC_READ_DESCR_RESPONSE
*  * CY_BLE_EVT_GATTC_ERROR_RSP
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
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
*  If the HRS service-specific callback is registered
*      (with Cy_BLE_HRS_RegisterAttrCallback):
*  * CY_BLE_EVT_HRSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_hrs_descr_value_t.
*  .
*  Otherwise (if the HRS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_HRSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hrs_char_index_t charIndex,
                                                               cy_en_ble_hrs_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_HRS_CHAR_COUNT) || (descrIndex >= CY_BLE_HRS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(charIndex != CY_BLE_HRS_HRM)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else if(cy_ble_hrsc[discIdx].hrmCccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_hrsc[discIdx].hrmCccdHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_hrscReqHandle[discIdx] = cy_ble_hrsc[discIdx].hrmCccdHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HRSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
*
******************************************************************************/
static void Cy_BLE_HRSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* Heart Rate Service characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_hrscCharUuid[CY_BLE_HRS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_HR_MSRMT,
        CY_BLE_UUID_CHAR_BODY_SENSOR_LOC,
        CY_BLE_UUID_CHAR_HR_CNTRL_POINT
    };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_HRS))
    {
        for(i = 0u; i < (uint32_t)CY_BLE_HRS_CHAR_COUNT; i++)
        {
            if(cy_ble_hrscCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_hrsc[discIdx].charInfo[i].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_hrsc[discIdx].charInfo[i].valueHandle = discCharInfo->valueHandle;
                    cy_ble_hrsc[discIdx].charInfo[i].properties = discCharInfo->properties;
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
* Function Name: Cy_BLE_HRSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_HRSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_HRS)
    {
        if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_CLIENT_CONFIG)
        {
            if(cy_ble_hrsc[discIdx].hrmCccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_hrsc[discIdx].hrmCccdHandle = discDescrInfo->descrHandle;
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
* Function Name: Cy_BLE_HRSC_GetCharRange
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
static void Cy_BLE_HRSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_HRS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        if(cy_ble_discovery[discIdx].charCount == 0u)
        {
            /* Read characteristic range */
            charRangeInfo->range.startHandle = cy_ble_hrsc[discIdx].charInfo[CY_BLE_HRS_HRM].valueHandle + 1u;
            charRangeInfo->range.endHandle = cy_ble_serverInfo[discIdx][CY_BLE_SRVI_HRS].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HRSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles the Notification Event.
*
*  \param eventParam: The pointer to a data structure specified by the event.
*
*
******************************************************************************/
static void Cy_BLE_HRSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) &&
       (cy_ble_hrsc[discIdx].charInfo[CY_BLE_HRS_HRM].valueHandle == eventParam->handleValPair.attrHandle))
    {
        if(Cy_BLE_HRS_ApplCallback != NULL)
        {
            cy_stc_ble_hrs_char_value_t locCharValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_HRS_HRM,
                .value      = &eventParam->handleValPair.value
            };

            Cy_BLE_HRS_ApplCallback((uint32_t)CY_BLE_EVT_HRSC_NOTIFICATION, &locCharValue);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HRSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles the Read Response Event.
*
*  \param cy_stc_ble_gattc_read_rsp_param_t *eventParam: The pointer to the data structure.
*
*
******************************************************************************/
static void Cy_BLE_HRSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_HRS_ApplCallback != NULL) &&
       (cy_ble_hrscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_hrsc[discIdx].charInfo[CY_BLE_HRS_BSL].valueHandle == cy_ble_hrscReqHandle[discIdx])
        {
            cy_stc_ble_hrs_char_value_t locCharValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_HRS_BSL,
                .value      = &eventParam->value
            };
            cy_ble_hrscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_HRS_ApplCallback((uint32_t)CY_BLE_EVT_HRSC_READ_CHAR_RESPONSE, &locCharValue);
        }
        else if(cy_ble_hrsc[discIdx].hrmCccdHandle == cy_ble_hrscReqHandle[discIdx])
        {
            cy_stc_ble_hrs_descr_value_t locDescrValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_HRS_HRM,
                .descrIndex = CY_BLE_HRS_HRM_CCCD,
                .value      = &eventParam->value
            };
            cy_ble_hrscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_HRS_ApplCallback((uint32_t)CY_BLE_EVT_HRSC_READ_DESCR_RESPONSE, &locDescrValue);
        }
        else
        {
            /* No any more read response handles */
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HRSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles the Write Response Event.
*
*  \param eventParam: The pointer to a data structure specified by the event.
*
*
******************************************************************************/
static void Cy_BLE_HRSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_HRS_ApplCallback != NULL) &&
       (cy_ble_hrscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_hrsc[discIdx].charInfo[CY_BLE_HRS_CPT].valueHandle == cy_ble_hrscReqHandle[discIdx])
        {
            cy_stc_ble_hrs_char_value_t locCharIndex =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_HRS_CPT,
                .value      = NULL
            };
            cy_ble_hrscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_HRS_ApplCallback((uint32_t)CY_BLE_EVT_HRSC_WRITE_CHAR_RESPONSE, &locCharIndex);
        }
        if(cy_ble_hrsc[discIdx].hrmCccdHandle == cy_ble_hrscReqHandle[discIdx])
        {
            cy_stc_ble_hrs_descr_value_t locDescIndex =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_HRS_HRM,
                .descrIndex = CY_BLE_HRS_HRM_CCCD,
                .value      = NULL
            };
            cy_ble_hrscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_HRS_ApplCallback((uint32_t)CY_BLE_EVT_HRSC_WRITE_DESCR_RESPONSE, &locDescIndex);
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HRSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles the Error Response Event.
*
*  \param eventParam: Pointer to the data structure specified by the event.
*
*
******************************************************************************/
static void Cy_BLE_HRSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        if(cy_ble_hrscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] == eventParam->errInfo.attrHandle)
        {
            cy_ble_hrscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}

#endif /* CY_BLE_HRS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_HRS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Heart Rate Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_HRS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_HRS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_HRSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_HRSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_HRSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_HRS_CLIENT */

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
        #ifdef CY_BLE_HRS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_HRSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;
        #endif /* CY_BLE_HRS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_HRS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);

                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_HRSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_HRSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;


            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_HRSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_HRSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_HRS_CLIENT */
            default:
                break;
        }
    }

    return(gattErr);
}
#endif /* defined(CY_BLE_HRS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
