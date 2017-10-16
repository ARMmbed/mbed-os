/***************************************************************************//**
* \file cy_ble_ancs.c
* \version 2.0
*
* \brief
*  This file contains the source code for
*  the Apple Notification Center Service of the BLE Component.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_ANCS))

static cy_ble_callback_t Cy_BLE_ANCS_ApplCallback;

#if (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GATT_ROLE_CLIENT)
/* Apple Notification Center Service UUID */
const cy_stc_ble_uuid128_t cy_ble_ancscServUuid =
{
    { 0xD0u, 0x00u, 0x2Du, 0x12u, 0x1Eu, 0x4Bu, 0x0Fu, 0xA4u, 0x99u, 0x4Eu, 0xCEu, 0xB5u, 0x31u, 0xF4u, 0x05u, 0x79u }
};

/* Apple Notification Center Service characteristics UUIDs */
const cy_stc_ble_uuid128_t cy_ble_ancscCharUuid[CY_BLE_ANCS_CHAR_COUNT] =
{
    /* Notification Source characteristic UUID */
    { { 0xBDu, 0x1Du, 0xA2u, 0x99u, 0xE6u, 0x25u, 0x58u, 0x8Cu, 0xD9u, 0x42u, 0x01u, 0x63u, 0x0Du, 0x12u, 0xBFu, 0x9Fu } },
    /* Control Point characteristic UUID */
    { { 0xD9u, 0xD9u, 0xAAu, 0xFDu, 0xBDu, 0x9Bu, 0x21u, 0x98u, 0xA8u, 0x49u, 0xE1u, 0x45u, 0xF3u, 0xD8u, 0xD1u, 0x69u } },
    /* Data Source characteristic UUID */
    { { 0xFBu, 0x7Bu, 0x7Cu, 0xCEu, 0x6Au, 0xB3u, 0x44u, 0xBEu, 0xB5u, 0x4Bu, 0xD6u, 0x24u, 0xE9u, 0xC6u, 0xEAu, 0x22u } }
};
#endif /* CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GATT_ROLE_CLIENT */

#ifdef CY_BLE_ANCS_CLIENT

/* Server's Apple Notification Center Service characteristics GATT DB handles structure */
cy_stc_ble_ancsc_t cy_ble_ancsc[CY_BLE_CONFIG_GATTC_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_ancscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];

#endif /* CY_BLE_ANCS_CLIENT */

/* The pointer to the global BLE ANCS config structure */
cy_stc_ble_ancs_config_t *cy_ble_ancsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_ANCS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_ANCSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* CY_BLE_ANCS_SERVER */

#ifdef CY_BLE_ANCS_CLIENT
static void Cy_BLE_ANCSC_DiscoverServiceEventHandler(const cy_stc_ble_disc_srv_info_t *discServInfo);
static void Cy_BLE_ANCSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_ANCSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_ANCSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_ANCSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_ANCSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_ANCSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_ANCSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* CY_BLE_ANCS_CLIENT */
static cy_en_ble_gatt_err_code_t Cy_BLE_ANCS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_ANCS_Init
***************************************************************************//**
*
*  Initializes profile internals.
*
*  \param config: The configuration structure for the Apple Notification
*                 Center Service.
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
cy_en_ble_api_result_t Cy_BLE_ANCS_Init(cy_stc_ble_ancs_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to the config structure */
        cy_ble_ancsConfigPtr = config;

        /* Registers Event Handler for the ANCS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_ANCS_EventHandler);

        /* Registers a callback function via the config structure */
        if(cy_ble_ancsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_ANCS_ApplCallback = cy_ble_ancsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_ANCS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_ANCS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_ancsc[idx], 0, sizeof(cy_stc_ble_ancsc_t));

                    /* Initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_ANCS].uuid = 0x0000u;
                }
                cy_ble_ancscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_ANCS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ANCS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific Write requests from a peer device will not be handled with
*  an unregistered callback function.
*
*  \param callbackFunc:  An application layer event callback function to receive
*                        events from the BLE Component. The definition of
*                        cy_ble_callback_t for ANCS is:
*    typedef void (* cy_ble_callback_t) (uint32_t eventCode, void *eventParam),
*    where:
*      * eventCode indicates the event that triggered this callback.
*      * eventParam contains the parameters corresponding to the current event.
*
*
*  \sideeffect
*  The *eventParams in the callback function should not be used by the
*  application once the callback function execution is finished. Otherwise, this
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
cy_en_ble_api_result_t Cy_BLE_ANCS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_ANCS_ApplCallback = callbackFunc;
    if(cy_ble_ancsConfigPtr != NULL)
    {
        cy_ble_ancsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}

#ifdef CY_BLE_ANCS_SERVER

/******************************************************************************
* Function Name: Cy_BLE_ANCSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets the value of the characteristic identified by charIndex.
*
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*                   stored to the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANCSS_SetCharacteristicValue(cy_en_ble_ancs_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check the parameters */
    if(charIndex >= CY_BLE_ANCS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE == cy_ble_ancsConfigPtr->ancss->charInfo[charIndex].charHandle)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Store the characteristic value into the GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_ancsConfigPtr->ancss->charInfo[charIndex].charHandle,
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
* Function Name: Cy_BLE_ANCSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets the value of the characteristic, as identified by charIndex.
*
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue: The pointer to the location where characteristic
*              value data should be stored.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic value was read successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this characteristic.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - A characteristic is absent.
*
* \events
*  None.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANCSS_GetCharacteristicValue(cy_en_ble_ancs_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check the parameters */
    if(charIndex >= CY_BLE_ANCS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE == cy_ble_ancsConfigPtr->ancss->charInfo[charIndex].charHandle)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get characteristic value from GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_ancsConfigPtr->ancss->charInfo[charIndex].charHandle,
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
* Function Name: Cy_BLE_ANCSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets a characteristic descriptor of the specified characteristic.
*
*  \param connHandle: The connection handle
*  \param charIndex:  The index of the characteristic.
*  \param descrIndex: The index of the descriptor.
*  \param attrSize:   The size of the descriptor value attribute.
*  \param attrValue: The pointer to the location where characteristic
*                      descriptor value data should be stored.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The Characteristic Descriptor value was read successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - A characteristic is absent.
*
* \events
*  None.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANCSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_ancs_char_index_t charIndex,
                                                                cy_en_ble_ancs_descr_index_t descrIndex,
                                                                uint8_t attrSize,
                                                                uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check the parameters */
    if((charIndex >= CY_BLE_ANCS_CHAR_COUNT) || (descrIndex >= CY_BLE_ANCS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_ancsConfigPtr->ancss->charInfo[charIndex].descrHandle[CY_BLE_ANCS_CCCD] ==
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get data from the database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_ancsConfigPtr->ancss->charInfo[charIndex].descrHandle[descrIndex],
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
* Function Name: Cy_BLE_ANCSS_SendNotification
***************************************************************************//**
*
*  Sends a notification of the specified characteristic value, as identified by
*  the charIndex.
*  On enabling notification successfully for a service characteristic it sends out a
*  Handle Value notification which results in a CY_BLE_EVT_ANCSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The connection handle that consists of the device ID and ATT
*                     connection ID.
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue:  The pointer to the characteristic value data that should
*                     be sent to the Client device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is absent
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the Client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the Client
*
* \events
*  None.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANCSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     cy_en_ble_ancs_char_index_t charIndex,
                                                     uint8_t attrSize,
                                                     uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send notification if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(charIndex >= CY_BLE_ANCS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_ancsConfigPtr->ancss->charInfo[charIndex].descrHandle[CY_BLE_ANCS_CCCD] ==
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else if(!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_ancsConfigPtr->ancss->charInfo[charIndex].
                                             descrHandle[CY_BLE_ANCS_CCCD]))
    {
        apiResult = CY_BLE_ERROR_NTF_DISABLED;
    }
    else
    {
        cy_stc_ble_gatts_handle_value_ntf_t ntfParam =
        {
            /* Fill all the fields of the Write request structure ... */
            .handleValPair.attrHandle = cy_ble_ancsConfigPtr->ancss->charInfo[charIndex].charHandle,
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
* Function Name: Cy_BLE_ANCSS_WriteEventHandler
***************************************************************************//**
*
*  Handles the Write Request event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*  cy_en_ble_gatt_err_code_t - A function result state if it succeeded
*  (CY_BLE_GATT_ERR_NONE) or failed with error codes:
*   * CY_BLE_GATTS_ERR_PROCEDURE_ALREADY_IN_PROGRESS;
*   * CY_BLE_GATTS_ERR_CCCD_IMPROPERLY_CONFIGURED.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_ANCSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;

    if(Cy_BLE_ANCS_ApplCallback != NULL)
    {
        cy_stc_ble_ancs_char_value_t locCharValue;
        locCharValue.charIndex = CY_BLE_ANCS_CHAR_COUNT;

        if(eventParam->handleValPair.attrHandle ==
           cy_ble_ancsConfigPtr->ancss->charInfo[CY_BLE_ANCS_NS].descrHandle[CY_BLE_ANCS_CCCD])
        {
            locCharValue.charIndex = CY_BLE_ANCS_NS;
        }
        else if(eventParam->handleValPair.attrHandle ==
                cy_ble_ancsConfigPtr->ancss->charInfo[CY_BLE_ANCS_DS].descrHandle[CY_BLE_ANCS_CCCD])
        {
            locCharValue.charIndex = CY_BLE_ANCS_DS;
        }
        else
        {
            /* Leave locCharValue.charIndex = CY_BLE_ANCS_CHAR_COUNT */
        }

        if(locCharValue.charIndex != CY_BLE_ANCS_CHAR_COUNT)
        {
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair = eventParam->handleValPair,
                .connHandle      = eventParam->connHandle,
                .offset          = 0u,
                .flags           = CY_BLE_GATT_DB_PEER_INITIATED
            };
            gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);

            locCharValue.connHandle = eventParam->connHandle;
            locCharValue.value = NULL;
            if(gattErr == CY_BLE_GATT_ERR_NONE)
            {
                uint32_t eventCode;

                if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                {
                    eventCode = (uint32_t)CY_BLE_EVT_ANCSS_NOTIFICATION_ENABLED;
                }
                else
                {
                    eventCode = (uint32_t)CY_BLE_EVT_ANCSS_NOTIFICATION_DISABLED;
                }

                Cy_BLE_ANCS_ApplCallback(eventCode, &locCharValue);
            }

        #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
            (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
            /* Set a flag to store bonding data to flash */
            if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
            {
                cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
            }
        #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */

            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else
        {
            if(eventParam->handleValPair.attrHandle == cy_ble_ancsConfigPtr->ancss->charInfo[CY_BLE_ANCS_CP].charHandle)
            {
                cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                {
                    .handleValuePair = eventParam->handleValPair,
                    .connHandle      = eventParam->connHandle,
                    .offset          = 0u,
                    .flags           = CY_BLE_GATT_DB_PEER_INITIATED
                };
                gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);
                locCharValue.charIndex = CY_BLE_ANCS_CP;
                locCharValue.value = &eventParam->handleValPair.value;

                if(gattErr == CY_BLE_GATT_ERR_NONE)
                {
                    CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(cy_ble_ancsConfigPtr->ancss->charInfo[locCharValue.charIndex].
                                                          charHandle, locCharValue.value->len);

                    Cy_BLE_ANCS_ApplCallback((uint32_t)CY_BLE_EVT_ANCSS_WRITE_CHAR, &locCharValue);

                    if((locCharValue.gattErrorCode == CY_BLE_GATT_ERR_ANCS_UNKNOWN_COMMAND) ||
                       (locCharValue.gattErrorCode == CY_BLE_GATT_ERR_ANCS_INVALID_COMMAND) ||
                       (locCharValue.gattErrorCode == CY_BLE_GATT_ERR_ANCS_INVALID_PARAMETER) ||
                       (locCharValue.gattErrorCode == CY_BLE_GATT_ERR_ANCS_ACTION_FAILED))
                    {
                        gattErr = locCharValue.gattErrorCode;
                    }
                }

                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            }
        }
    }

    return(gattErr);
}

#endif /* CY_BLE_ANCS_SERVER */

#ifdef CY_BLE_ANCS_CLIENT

/******************************************************************************
* Function Name: Cy_BLE_ANCSC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write the characteristic (identified by
*  charIndex) value attribute in the Server. As a result, a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server's side, a CY_BLE_EVT_ANCSS_WRITE_CHAR events is generated.
*  On successful request execution on the Server's side, a Write Response is
*  sent to the Client.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue:  The pointer to the characteristic value data that should be
*                     sent to the Server device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                              the particular characteristic.
*  * CY_BLE_ERROR_INVALID_OPERATION - The operation is invalid for this
*                                    characteristic.
*
* \events
*  If execution is successful (return value = CY_BLE_SUCCESS),
*  these events can appear: \n
*   If an ANCS service-specific callback is registered
*      (with Cy_BLE_ANCS_RegisterAttrCallback):
*  * CY_BLE_EVT_ANCSC_WRITE_CHAR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_ancs_char_value_t.
*  .
*   Otherwise (if an ANCS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - If the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with an event parameter
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANCSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_ancs_char_index_t charIndex,
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
    else if((charIndex >= CY_BLE_ANCS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_ancsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_WRITE & cy_ble_ancsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_ancsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };

        apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_ancscReqHandle[discIdx] = cy_ble_ancsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ANCSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  This function is used to write the a characteristic value to the Server
*  identified by its charIndex.
*
*  Internally, a Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server's side these following events can be
*  generated:
*  * CY_BLE_EVT_ANCSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_ANCSS_NOTIFICATION_DISABLED
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param attrSize:   The size of the characteristic descriptor value attribute.
*  \param attrValue:  The pointer to the characteristic descriptor value data that
*                     should be sent to the server device.
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
*  If execution is successful (return value = CY_BLE_SUCCESS)
*  these events can appear: \n
*   If an ANCS service-specific callback is registered
*      (with Cy_BLE_ANCS_RegisterAttrCallback):
*  * CY_BLE_EVT_ANCSC_WRITE_DESCR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_ancs_descr_value_t.
*  .
*   Otherwise (if an ANCS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - If the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with an event parameter
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANCSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_ancs_char_index_t charIndex,
                                                                cy_en_ble_ancs_descr_index_t descrIndex,
                                                                uint8_t
                                                                attrSize,
                                                                uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check the parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_ANCS_CHAR_COUNT) || (descrIndex >= CY_BLE_ANCS_DESCR_COUNT) ||
            (attrSize != CY_BLE_CCCD_LEN)  || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_ancsc[discIdx].charInfo[charIndex].descrHandle[descrIndex] == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            /* Fill all fields of the Write Request structure ... */
            .handleValPair.attrHandle = cy_ble_ancsc[discIdx].charInfo[charIndex].descrHandle[descrIndex],
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = CY_BLE_CCCD_LEN,
            .connHandle               = connHandle
        };

        /* ... and send request to Server device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            /* Save the handle to support service specific read response from the device */
            cy_ble_ancscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ANCSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets the characteristic descriptor of the specified characteristic.
*
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
*  If execution is successful (return value = CY_BLE_SUCCESS)
*  these next events can appear: \n
*  If an ANCS service-specific callback is registered
*      (with Cy_BLE_ANCS_RegisterAttrCallback):
*  * CY_BLE_EVT_ANCSC_READ_DESCR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with an event parameter structure
*                                of type cy_stc_ble_ancs_descr_value_t.
*  .
*  Otherwise (if n ANCS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP - If the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with an event parameter
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with an event parameter
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ANCSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_ancs_char_index_t charIndex,
                                                                cy_en_ble_ancs_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check the parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_ANCS_CHAR_COUNT) || (descrIndex >= CY_BLE_ANCS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_ancsc[discIdx].charInfo[charIndex].descrHandle[descrIndex] == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_ancsc[discIdx].charInfo[charIndex].descrHandle[descrIndex],
            .connHandle = connHandle
        };

        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_ancscReqHandle[discIdx] = cy_ble_ancsc[discIdx].charInfo[charIndex].descrHandle[descrIndex];
        }
    }

    return(apiResult);
}

/******************************************************************************
* Function Name: Cy_BLE_ANCSC_DiscoverServiceEventHandler
***************************************************************************//**
*
*  This function is called on receiving a Read-By-Group Response event or
*  Read response with 128-bit service uuid.
*
*  \param discServInfo: The pointer to the service information structure.
*
* \return
*  None.
*
******************************************************************************/
static void Cy_BLE_ANCSC_DiscoverServiceEventHandler(const cy_stc_ble_disc_srv_info_t *discServInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discServInfo->connHandle);

    if(memcmp(&cy_ble_ancscServUuid, &discServInfo->srvcInfo->uuid.uuid128, CY_BLE_GATT_128_BIT_UUID_SIZE) == 0u)
    {
        if(cy_ble_serverInfo[discIdx][CY_BLE_SRVI_ANCS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            cy_ble_serverInfo[discIdx][CY_BLE_SRVI_ANCS].range = discServInfo->srvcInfo->range;
            cy_ble_discovery[discIdx].servCount++;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}

/******************************************************************************
* Function Name: Cy_BLE_ANCSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, the appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to the characteristic information structure.
*
* \return
*  None.
*
******************************************************************************/
static void Cy_BLE_ANCSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    cy_en_ble_ancs_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);

    if((discCharInfo->uuidFormat == CY_BLE_GATT_128_BIT_UUID_FORMAT) &&
       (discCharInfo->valueHandle >= cy_ble_serverInfo[discIdx][CY_BLE_SRVI_ANCS].range.startHandle) &&
       (discCharInfo->valueHandle <= cy_ble_serverInfo[discIdx][CY_BLE_SRVI_ANCS].range.endHandle))
    {
        /* Update the last characteristic endHandle to the declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        for(locCharIndex = CY_BLE_ANCS_NS; locCharIndex < CY_BLE_ANCS_CHAR_COUNT; locCharIndex++)
        {
            if(memcmp(&cy_ble_ancscCharUuid[locCharIndex], &discCharInfo->uuid.uuid128,
                      CY_BLE_GATT_128_BIT_UUID_SIZE) == 0u)
            {
                if(cy_ble_ancsc[discIdx].charInfo[locCharIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_ancsc[discIdx].charInfo[locCharIndex].valueHandle = discCharInfo->valueHandle;
                    cy_ble_ancsc[discIdx].charInfo[locCharIndex].properties = discCharInfo->properties;
                    lastEndHandle[discIdx] = &cy_ble_ancsc[discIdx].charInfo[locCharIndex].endHandle;
                }
                else
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo);
                }
            }
        }

        /* Initialize the characteristic endHandle to the Service endHandle. The characteristic endHandle will
         * be updated to the declaration handle of the following characteristic, in the following characteristic
         * discovery procedure. */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = cy_ble_serverInfo[discIdx][CY_BLE_SRVI_ANCS].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ANCSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, the appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to the characteristic information structure.
*
* \return
*  None.
*
******************************************************************************/
static void Cy_BLE_ANCSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t notSupportedDescr = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t descIdx;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_ANCS)
    {
        switch(discDescrInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_CLIENT_CONFIG:
                descIdx = (uint32_t)CY_BLE_ANCS_CCCD;
                break;

            default:
                /* Not supported descriptor */
                notSupportedDescr = 1u;
                break;
        }

        if(notSupportedDescr == 0u)
        {
            if(cy_ble_ancsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] ==
               CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_ancsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] =
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
* Function Name: Cy_BLE_ANCSC_GetCharRange
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
static void Cy_BLE_ANCSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_ANCS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_ANCS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if((cy_ble_ancsc[discIdx].charInfo[charIdx].endHandle -
                cy_ble_ancsc[discIdx].charInfo[charIdx].valueHandle) != 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_ancsc[discIdx].charInfo[charIdx].valueHandle + 1u;
                charRangeInfo->range.endHandle = cy_ble_ancsc[discIdx].charInfo[charIdx].endHandle;
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
* Function Name: Cy_BLE_ANCSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles a Notification event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*  None.
*
******************************************************************************/
static void Cy_BLE_ANCSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_ANCS_ApplCallback != NULL))
    {
        cy_stc_ble_ancs_char_value_t locCharValue;

        if(cy_ble_ancsc[discIdx].charInfo[CY_BLE_ANCS_NS].valueHandle == eventParam->handleValPair.attrHandle)
        {
            locCharValue.charIndex = CY_BLE_ANCS_NS;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else if(cy_ble_ancsc[discIdx].charInfo[CY_BLE_ANCS_DS].valueHandle == eventParam->handleValPair.attrHandle)
        {
            locCharValue.charIndex = CY_BLE_ANCS_DS;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else
        {
            /* Apple Notification Center Service doesn't support any other notification */
        }

        if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) == 0u)
        {
            locCharValue.connHandle = eventParam->connHandle;
            locCharValue.value = &eventParam->handleValPair.value;
            Cy_BLE_ANCS_ApplCallback((uint32_t)CY_BLE_EVT_ANCSC_NOTIFICATION, &locCharValue);
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ANCSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles a Read Response event.
*
*  \param eventParam: The pointer to the data structure.
*
* \return
*  None.
*
******************************************************************************/
static void Cy_BLE_ANCSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_ANCS_ApplCallback != NULL) &&
       (cy_ble_ancscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        cy_stc_ble_ancs_descr_value_t locDescrValue;

        if(cy_ble_ancsc[discIdx].charInfo[CY_BLE_ANCS_NS].descrHandle[CY_BLE_ANCS_CCCD] == cy_ble_ancscReqHandle[discIdx])
        {
            locDescrValue.charIndex = CY_BLE_ANCS_NS;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else if(cy_ble_ancsc[discIdx].charInfo[CY_BLE_ANCS_DS].descrHandle[CY_BLE_ANCS_CCCD] ==
                cy_ble_ancscReqHandle[discIdx])
        {
            locDescrValue.charIndex = CY_BLE_ANCS_DS;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else
        {
        }

        if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) == 0u)
        {
            locDescrValue.connHandle = eventParam->connHandle;
            locDescrValue.value = &eventParam->value;

            Cy_BLE_ANCS_ApplCallback((uint32_t)CY_BLE_EVT_ANCSC_READ_DESCR_RESPONSE, &locDescrValue);
        }

        cy_ble_ancscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ANCSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Write Response event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*  None.
*
******************************************************************************/
static void Cy_BLE_ANCSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_ANCS_ApplCallback != NULL) &&
       (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_ancscReqHandle[discIdx]))
    {
        if(cy_ble_ancsc[discIdx].charInfo[CY_BLE_ANCS_CP].valueHandle == cy_ble_ancscReqHandle[discIdx])
        {
            cy_stc_ble_ancs_char_value_t locCharIndex =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_ANCS_CP,
                .value      = NULL
            };
            cy_ble_ancscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_ANCS_ApplCallback((uint32_t)CY_BLE_EVT_ANCSC_WRITE_CHAR_RESPONSE, &locCharIndex);
        }
        else
        {
            cy_stc_ble_ancs_descr_value_t locDescIndex;

            if(cy_ble_ancsc[discIdx].charInfo[CY_BLE_ANCS_NS].descrHandle[CY_BLE_ANCS_CCCD] ==
               cy_ble_ancscReqHandle[discIdx])
            {
                locDescIndex.charIndex = CY_BLE_ANCS_NS;
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            }
            else if(cy_ble_ancsc[discIdx].charInfo[CY_BLE_ANCS_DS].descrHandle[CY_BLE_ANCS_CCCD] ==
                    cy_ble_ancscReqHandle[discIdx])
            {
                locDescIndex.charIndex = CY_BLE_ANCS_DS;
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            }
            else
            {
                /* Apple Notification Center Service doesn't support any other notification */
            }

            if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) == 0u)
            {
                locDescIndex.connHandle = *eventParam;
                locDescIndex.descrIndex = CY_BLE_ANCS_CCCD;
                locDescIndex.value = NULL;

                cy_ble_ancscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                Cy_BLE_ANCS_ApplCallback((uint32_t)CY_BLE_EVT_ANCSC_WRITE_DESCR_RESPONSE, &locDescIndex);
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ANCSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles an Error Response event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*  None.
*
******************************************************************************/
static void Cy_BLE_ANCSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
        if(eventParam->errInfo.attrHandle == cy_ble_ancscReqHandle[discIdx])
        {
            if((Cy_BLE_ANCS_ApplCallback != NULL) &&
               (cy_ble_ancscReqHandle[discIdx] == cy_ble_ancsc[discIdx].charInfo[CY_BLE_ANCS_CP].valueHandle))
            {
                cy_stc_ble_ancs_char_value_t locGattError =
                {
                    .connHandle    = eventParam->connHandle,
                    .charIndex     = CY_BLE_ANCS_CP,
                    .value         = NULL,
                    .gattErrorCode = eventParam->errInfo.errorCode
                };

                Cy_BLE_ANCS_ApplCallback((uint32_t)CY_BLE_EVT_ANCSC_ERROR_RESPONSE, &locGattError);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            }

            cy_ble_ancscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}
#endif /* CY_BLE_ANCS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_ANCS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Apple Notification Center Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_ANCS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_ANCS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_SERVICE:
                Cy_BLE_ANCSC_DiscoverServiceEventHandler((cy_stc_ble_disc_srv_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_ANCSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_ANCSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_ANCSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_ANCS_CLIENT */

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
        #ifdef CY_BLE_ANCS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_ANCSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;
        #endif /* CY_BLE_ANCS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_ANCS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_ANCSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_ANCSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_ANCSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_ANCSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_ANCS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* CY_BLE_ANCS */
#ifdef __cplusplus
}
#endif /* __cplusplus */


/* [] END OF FILE */
