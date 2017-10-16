/***************************************************************************//**
* \file cy_ble_wpts.c
* \version 2.0
*
* \brief
*  Contains the source code for Wireless Power Transfer Service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_WPTS))

#ifdef CY_BLE_WPTS_SERVER
static cy_ble_gatt_db_attr_handle_t cy_ble_wptssReqHandle;
#endif /* CY_BLE_WPTS_SERVER */

#ifdef CY_BLE_WPTS_CLIENT
/* WPTS Center Service characteristics GATT DB handles structure */
cy_stc_ble_wptsc_t cy_ble_wptsc[CY_BLE_CONFIG_GATTC_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_wptscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];

/* WPTS characteristics 128-bit UUIDs */
const cy_stc_ble_uuid128_t cy_ble_wptscCharUuid128[CY_BLE_WPTS_CHAR_COUNT] =
{
    /* PRU Control Characteristic*/
    { { 0x67u, 0x9Au, 0x0Cu, 0x20u, 0x00u, 0x08u, 0x96u, 0x9Eu, 0xE2u, 0x11u, 0x46u, 0xA1u, 0x70u, 0xE6u, 0x55u, 0x64u } },
    /* PTU Static Parameter Characteristic*/
    { { 0x68u, 0x9Au, 0x0Cu, 0x20u, 0x00u, 0x08u, 0x96u, 0x9Eu, 0xE2u, 0x11u, 0x46u, 0xA1u, 0x70u, 0xE6u, 0x55u, 0x64u } },
    /* PRU Alert Characteristic*/
    { { 0x69u, 0x9Au, 0x0Cu, 0x20u, 0x00u, 0x08u, 0x96u, 0x9Eu, 0xE2u, 0x11u, 0x46u, 0xA1u, 0x70u, 0xE6u, 0x55u, 0x64u } },
    /* PRU Static Parameter Characteristic*/
    { { 0x6Au, 0x9Au, 0x0Cu, 0x20u, 0x00u, 0x08u, 0x96u, 0x9Eu, 0xE2u, 0x11u, 0x46u, 0xA1u, 0x70u, 0xE6u, 0x55u, 0x64u } },
    /* PRU Dynamic Parameter Characteristic*/
    { { 0x6Bu, 0x9Au, 0x0Cu, 0x20u, 0x00u, 0x08u, 0x96u, 0x9Eu, 0xE2u, 0x11u, 0x46u, 0xA1u, 0x70u, 0xE6u, 0x55u, 0x64u } }
};

#endif /* (CY_BLE_WPTS_CLIENT) */

static cy_ble_callback_t Cy_BLE_WPTS_ApplCallback = NULL;

/* The pointer on global BLE WPTS Config structure */
cy_stc_ble_wpts_config_t *cy_ble_wptsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_WPTS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_WPTSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
static void Cy_BLE_WPTSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
#endif /* CY_BLE_WPTS_SERVER */

#ifdef CY_BLE_WPTS_CLIENT
static void Cy_BLE_WPTSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_WPTSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_WPTSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_WPTSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_WPTSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam);
static void Cy_BLE_WPTSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_WPTSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_WPTSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* CY_BLE_WPTS_CLIENT */
static cy_en_ble_gatt_err_code_t Cy_BLE_WPTS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_WPTS_Init
***************************************************************************//**
*
*  This function initializes thr Wireless Power Transfer Service.
*
*  \param config: Configuration structure for the Wireless Power Transfer Service.
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
cy_en_ble_api_result_t Cy_BLE_WPTS_Init(cy_stc_ble_wpts_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_wptsConfigPtr = config;

        /* Registers Event Handler for the WPTS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_WPTS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_wptsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_WPTS_ApplCallback = cy_ble_wptsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_WPTS_SERVER
        cy_ble_wptssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    #endif /* CY_BLE_WPTS_SERVER */

    #ifdef CY_BLE_WPTS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_WPTS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_wptsc[idx], 0, sizeof(cy_stc_ble_wptsc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_WPTS].uuid = CY_BLE_UUID_WIRELESS_POWER_TRANSFER_SERVICE;
                }
                cy_ble_wptscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_WPTS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WPTS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc: An application layer event callback function to receive
*                 events from the BLE Component. The definition of
*                 cy_ble_callback_t is: \n
*                 typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                    void *eventParam)
*                 * eventCode indicates the event that triggered this
*                   callback (e.g. CY_BLE_EVT_WPTSS_INDICATION_ENABLED).
*                 * eventParam contains the parameters corresponding to the
*                   current event. (e.g. pointer to cy_stc_ble_wpts_char_value_t
*                   structure that contains details of the characteristic
*                   for which notification enabled event was triggered).
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
cy_en_ble_api_result_t Cy_BLE_WPTS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_WPTS_ApplCallback = callbackFunc;
    if(cy_ble_wptsConfigPtr != NULL)
    {
        cy_ble_wptsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_WPTS_SERVER


/******************************************************************************
* Function Name: Cy_BLE_WPTSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets a characteristic value of the Wireless Power Transfer Service in the
*  local GATT database. The characteristic is identified by charIndex.
*
*  \param charIndex: The index of a service characteristic of type
*                     cy_en_ble_wpts_char_index_t.
*  \param attrSize:  The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*                     stored to the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic value was written successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WPTSS_SetCharacteristicValue(cy_en_ble_wpts_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((attrValue != NULL) && (charIndex < CY_BLE_WPTS_CHAR_COUNT))
    {
        /* Fill the structure */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_wptsConfigPtr->wptss->charInfo[charIndex].charHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .offset                     = 0u,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };
        if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            /* Indicate success */
            apiResult = CY_BLE_SUCCESS;
        }
    }

    /* Return status */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WPTSS_GetCharacteristicValue
***************************************************************************//**
*
*  Reads a characteristic value of the Wireless Power Transfer Service, which
*  is identified by charIndex from the GATT database.
*
*  \param charIndex:       The index of a service characteristic of type
*                          cy_en_ble_wpts_char_index_t.
*  \param attrSize:        The size of the characteristic value attribute.
*  \param attrValue:       The pointer to the location where characteristic value data
*                          should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic value was read successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WPTSS_GetCharacteristicValue(cy_en_ble_wpts_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((attrValue != NULL) && (charIndex < CY_BLE_WPTS_CHAR_COUNT))
    {
        /* Fill the structure */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_wptsConfigPtr->wptss->charInfo[charIndex].charHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
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
* Function Name: Cy_BLE_WPTSS_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sets the characteristic descriptor of the specified characteristic.
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_wpts_char_index_t.
*  \param descrIndex:      The index of a service characteristic descriptor of type
*                   cy_en_ble_wpts_descr_index_t.
*  \param attrSize:        The size of the characteristic descriptor attribute.
*  \param attrValue:      The pointer to the descriptor value data that should
*                   be stored to the GATT database.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WPTSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_wpts_char_index_t charIndex,
                                                                cy_en_ble_wpts_descr_index_t descrIndex,
                                                                uint8_t attrSize,
                                                                uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex < CY_BLE_WPTS_CHAR_COUNT) && (descrIndex < CY_BLE_WPTS_DESCR_COUNT) && (attrValue != NULL))
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_wptsConfigPtr->wptss->charInfo[charIndex].descrHandle[descrIndex],
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .connHandle                 = connHandle,
            .offset                     = 0u,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };

        /* Set characteristic value to database */
        if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            /* Indicate success */
            apiResult = CY_BLE_SUCCESS;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WPTSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Reads a characteristic descriptor of a specified characteristic of the
*  Wireless Power Transfer Service from the GATT database.
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type
*                          cy_en_ble_wpts_char_index_t.
*  \param descrIndex:      The index of a service characteristic descriptor of type
*                          cy_en_ble_wpts_descr_index_t.
*  \param attrSize:        The size of the characteristic descriptor attribute.
*  \param attrValue:       The pointer to the location where characteristic descriptor
*                          value data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WPTSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_wpts_char_index_t charIndex,
                                                                cy_en_ble_wpts_descr_index_t descrIndex,
                                                                uint8_t attrSize,
                                                                uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex < CY_BLE_WPTS_CHAR_COUNT) && (descrIndex < CY_BLE_WPTS_DESCR_COUNT) && (attrValue != NULL))
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_wptsConfigPtr->wptss->charInfo[charIndex].descrHandle[descrIndex],
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .connHandle                 = connHandle,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };

        /* Get descriptor value from GATT database */
        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            /* Indicate success */
            apiResult = CY_BLE_SUCCESS;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WPTSS_SendNotification
***************************************************************************//**
*
*  Sends notification with a characteristic value of the WPTS, which is a value
*  specified by charIndex, to the Client device.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_WPTSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle:      The connection handle
*  \param charIndex:       The index of a service characteristic of type
*                          cy_en_ble_wpts_char_index_t.
*  \param attrSize:        The size of the characteristic value attribute.
*  \param attrValue:       The pointer to the characteristic value data that should be
*                          sent to the Client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the Client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the Client.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WPTSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     cy_en_ble_wpts_char_index_t charIndex,
                                                     uint8_t attrSize,
                                                     uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send Notification if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(charIndex >= CY_BLE_WPTS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_wptsConfigPtr->wptss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((!CY_BLE_IS_NOTIFICATION_SUPPORTED(cy_ble_wptsConfigPtr->wptss->charInfo[charIndex].charHandle)) ||
            (!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_wptsConfigPtr->wptss->charInfo[charIndex].
                                              descrHandle[CY_BLE_WPTS_CCCD])))
    {
        apiResult = CY_BLE_ERROR_NTF_DISABLED;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
        {
            .handleValPair.attrHandle = cy_ble_wptsConfigPtr->wptss->charInfo[charIndex].charHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* Send notification to the Client using previously filled structure */
        apiResult = Cy_BLE_GATTS_Notification(&ntfReqParam);
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WPTSS_SendIndication
***************************************************************************//**
*
*  Sends an indication with a characteristic value of the Wireless Power Transfer
*  Service, which is a value specified by charIndex, to the Client device.
*
*  On enabling indication successfully it sends out a 'Handle Value Indication' which
*  results in CY_BLE_EVT_WPTSC_INDICATION or CY_BLE_EVT_GATTC_HANDLE_VALUE_IND (if
*  service-specific callback function is not registered) event at the GATT Client's end.
*
*  \param connHandle:      The connection handle
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_wpts_char_index_t.
*  \param attrSize:        The size of the characteristic value attribute.
*  \param attrValue:      The pointer to the characteristic value data that should be
*                   sent to the Client device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_IND_DISABLED - Indication is not enabled by the client.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the WPTS service-specific callback is registered
*      (with Cy_BLE_WPTS_RegisterAttrCallback):
*  * CY_BLE_EVT_WPTSS_INDICATION_CONFIRMED - in case if the indication is
*                                successfully delivered to the peer device.
*  .
*   Otherwise (if the WPTS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF - in case if the indication is
*                                successfully delivered to the peer device.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WPTSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                   cy_en_ble_wpts_char_index_t charIndex,
                                                   uint8_t attrSize,
                                                   uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send Notification if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(charIndex >= CY_BLE_WPTS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_wptsConfigPtr->wptss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((!CY_BLE_IS_INDICATION_SUPPORTED(cy_ble_wptsConfigPtr->wptss->charInfo[charIndex].charHandle)) ||
            (!CY_BLE_IS_INDICATION_ENABLED(connHandle.attId, cy_ble_wptsConfigPtr->wptss->charInfo[charIndex].
                                            descrHandle[CY_BLE_WPTS_CCCD])))
    {
        apiResult = CY_BLE_ERROR_IND_DISABLED;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gatts_handle_value_ind_t indReqParam =
        {
            .handleValPair.attrHandle = cy_ble_wptsConfigPtr->wptss->charInfo[charIndex].charHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* Send indication to Client using previously filled structure */
        apiResult = Cy_BLE_GATTS_Indication(&indReqParam);
        /* Save handle to support service-specific value confirmation response from Client */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_wptssReqHandle = indReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WPTSS_WriteEventHandler
***************************************************************************//**
*
*  Handles Write Request Event for Wireless Power Transfer Service.
*
*  \param void *eventParam: The pointer to the data structure specified by the event.
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*   * CY_BLE_GATT_ERR_NONE - Write is successful.
*   * CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED - Request is not supported.
*   * CY_BLE_GATT_ERR_INVALID_HANDLE - 'handleValuePair.attrHandle' is not valid.
*   * CY_BLE_GATT_ERR_WRITE_NOT_PERMITTED - Write operation is not permitted on
*                                          this attribute.
*   * CY_BLE_GATT_ERR_INVALID_OFFSET - Offset value is invalid.
*   * CY_BLE_GATT_ERR_UNLIKELY_ERROR - Some other error occurred.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_WPTSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_wpts_char_index_t locCharIndex;
    cy_stc_ble_wpts_char_value_t locCharValue = { .connHandle = eventParam->connHandle };
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    uint32_t locReqHandle = 0u;

    if(Cy_BLE_WPTS_ApplCallback != NULL)
    {
        for(locCharIndex = CY_BLE_WPTS_PRU_CONTROL; (locCharIndex < CY_BLE_WPTS_CHAR_COUNT) && (locReqHandle == 0u);
            locCharIndex++)
        {
            if((eventParam->handleValPair.attrHandle ==
                cy_ble_wptsConfigPtr->wptss->charInfo[locCharIndex].descrHandle[CY_BLE_WPTS_CCCD])
               || (eventParam->handleValPair.attrHandle ==
                   cy_ble_wptsConfigPtr->wptss->charInfo[locCharIndex].charHandle))
            {
                /* Store value to database */
                cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                {
                    .handleValuePair = eventParam->handleValPair,
                    .connHandle      = eventParam->connHandle,
                    .offset          = 0u,
                    .flags           = CY_BLE_GATT_DB_PEER_INITIATED
                };
                locCharValue.charIndex = locCharIndex;
                locCharValue.value = &eventParam->handleValPair.value;
                gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);
                if(gattErr == CY_BLE_GATT_ERR_NONE)
                {
                    /* Characteristic value write request */
                    if(eventParam->handleValPair.attrHandle == cy_ble_wptsConfigPtr->wptss->charInfo[locCharIndex].
                        charHandle)
                    {
                        Cy_BLE_WPTS_ApplCallback((uint32_t)CY_BLE_EVT_WPTSS_WRITE_CHAR, &locCharValue);
                    }
                    else /* Client Characteristic Configuration descriptor write request */
                    {
                        /* Check characteristic properties for Notification */
                        if(CY_BLE_IS_NOTIFICATION_SUPPORTED(cy_ble_wptsConfigPtr->wptss->charInfo[locCharIndex].charHandle))
                        {
                            uint32_t eventCode;
                            if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                            {
                                eventCode = (uint32_t)CY_BLE_EVT_WPTSS_NOTIFICATION_ENABLED;
                            }
                            else
                            {
                                eventCode = (uint32_t)CY_BLE_EVT_WPTSS_NOTIFICATION_DISABLED;
                            }
                            Cy_BLE_WPTS_ApplCallback(eventCode, &locCharValue);
                        }
                        /* Check characteristic properties for Indication */
                        if(CY_BLE_IS_INDICATION_SUPPORTED(cy_ble_wptsConfigPtr->wptss->charInfo[locCharIndex].charHandle))
                        {
                            uint32_t eventCode;
                            if(CY_BLE_IS_INDICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                            {
                                eventCode = (uint32_t)CY_BLE_EVT_WPTSS_INDICATION_ENABLED;
                            }
                            else
                            {
                                eventCode = (uint32_t)CY_BLE_EVT_WPTSS_INDICATION_DISABLED;
                            }
                            Cy_BLE_WPTS_ApplCallback(eventCode, &locCharValue);
                        }
                    #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
                        (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
                        /* Set flag to store bonding data to flash */
                        if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
                        {
                            cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
                        }
                    #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */
                    }
                }
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                locReqHandle = 1u;
            }
        }
    }

    return(gattErr);
}


/******************************************************************************
* Function Name: Cy_BLE_WPTSS_ConfirmationEventHandler
***************************************************************************//**
*
*  Handles a Value Confirmation request event from the BLE stack.
*
*  *eventParam - The pointer to a structure of type cy_stc_ble_conn_handle_t.
*
******************************************************************************/
static void Cy_BLE_WPTSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    cy_en_ble_wpts_char_index_t locCharIndex;

    uint32_t locReqHandle = 0u;

    if((Cy_BLE_WPTS_ApplCallback != NULL) && (cy_ble_wptssReqHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locCharIndex = CY_BLE_WPTS_PRU_CONTROL; (locCharIndex < CY_BLE_WPTS_CHAR_COUNT) && (locReqHandle == 0u);
            locCharIndex++)
        {
            if(cy_ble_wptssReqHandle == cy_ble_wptsConfigPtr->wptss->charInfo[locCharIndex].charHandle)
            {
                /* Fill in event data and inform Application about successfully confirmed indication. */
                cy_stc_ble_wpts_char_value_t locCharValue =
                {
                    .connHandle = *eventParam,
                    .charIndex  = locCharIndex,
                    .value      = NULL
                };
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                cy_ble_wptssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_WPTS_ApplCallback((uint32_t)CY_BLE_EVT_WPTSS_INDICATION_CONFIRMED, &locCharValue);
                locReqHandle = 1u;
            }
        }
    }
}

#endif /* CY_BLE_WPTS_SERVER */


#ifdef CY_BLE_WPTS_CLIENT


/******************************************************************************
* Function Name: Cy_BLE_WPTSC_Discovery
***************************************************************************//**
*
*  This function discovers the PRU's WPT service and characteristics using the
*  GATT Primary Service Handle, received through the WPT Service Data
*  within the PRU advertisement payload, together with the handle offsets defined
*  A4WP specification.
*
*  The PTU may perform service discovery using the Cy_BLE_GATTC_StartDiscovery().
*  This function may be used in response to Service Changed indication or to
*  discover services other than the WPT service supported by the PRU.
*
*  \param servHandle: GATT Primary Service Handle of the WPT service.
*  \param connHandle: BLE peer device connection handle.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - All client instances are used. 
*    
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WPTSC_Discovery(cy_ble_gatt_db_attr_handle_t servHandle,
                                              cy_stc_ble_conn_handle_t connHandle)
{
    uint32_t discIdx;
    cy_en_ble_api_result_t apiResult;
    
    apiResult = Cy_BLE_GATTC_AddConnHandle(connHandle);
    
    if(apiResult == CY_BLE_SUCCESS)
    {
        discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

        cy_ble_wptsc[discIdx].serviceHandle = servHandle;
        cy_ble_wptsc[discIdx].charInfo[CY_BLE_WPTS_PRU_CONTROL].valueHandle =
            servHandle + CY_BLE_WPTS_PRU_CONTROL_CHAR_VALUE_OFFSET;
        cy_ble_wptsc[discIdx].charInfo[CY_BLE_WPTS_PTU_STATIC_PAR].valueHandle =
            servHandle + CY_BLE_WPTS_PTU_STATIC_PAR_CHAR_VALUE_OFFSET;
        cy_ble_wptsc[discIdx].charInfo[CY_BLE_WPTS_PRU_ALERT].valueHandle =
            servHandle + CY_BLE_WPTS_PRU_ALERT_PAR_CHAR_VALUE_OFFSET;
        cy_ble_wptsc[discIdx].charInfo[CY_BLE_WPTS_PRU_ALERT].descrHandle[CY_BLE_WPTS_CCCD] =
            servHandle + CY_BLE_WPTS_PRU_ALERT_PAR_CHAR_CCCD_OFFSET;
        cy_ble_wptsc[discIdx].charInfo[CY_BLE_WPTS_PRU_STATIC_PAR].valueHandle =
            servHandle + CY_BLE_WPTS_PRU_STATIC_PAR_CHAR_VALUE_OFFSET;
        cy_ble_wptsc[discIdx].charInfo[CY_BLE_WPTS_PRU_DYNAMIC_PAR].valueHandle =
            servHandle + CY_BLE_WPTS_PRU_DYNAMIC_PAR_CHAR_VALUE_OFFSET;

        Cy_BLE_SetConnectionState(connHandle, CY_BLE_CONN_STATE_CLIENT_DISCOVERED);
    }
    return(apiResult);
}

/******************************************************************************
* Function Name: Cy_BLE_WPTSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_WPTSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);

    /* WPTS service has only characteristics with 128-bit UUIDs */
    if((discCharInfo->uuidFormat == CY_BLE_GATT_128_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_WPTS))
    {
        /* Update last characteristic endHandle to declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        /* Compare common part of characteristics 128-bit UUID (except first byte) */
        if(memcmp(&cy_ble_wptscCharUuid128[CY_BLE_WPTS_PRU_CONTROL].value[1u],
                  &discCharInfo->uuid.uuid128.value[1u], CY_BLE_GATT_128_BIT_UUID_SIZE - 1u) == 0u)
        {
            /* Check the range of the first byte to be between the first and last characteristic */
            if((discCharInfo->uuid.uuid128.value[0u] >= cy_ble_wptscCharUuid128[CY_BLE_WPTS_PRU_CONTROL].value[0u]) &&
               (discCharInfo->uuid.uuid128.value[0u] <= cy_ble_wptscCharUuid128[CY_BLE_WPTS_CHAR_COUNT - 1u].value[0u]))
            {
                uint32_t locCharIndex;
                locCharIndex = (uint32_t)discCharInfo->uuid.uuid128.value[0u] -
                               cy_ble_wptscCharUuid128[CY_BLE_WPTS_PRU_CONTROL].value[0u];
                /* Verify that characteristic handler is not assigned already */
                if(cy_ble_wptsc[discIdx].charInfo[locCharIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_wptsc[discIdx].charInfo[locCharIndex].valueHandle = discCharInfo->valueHandle;
                    cy_ble_wptsc[discIdx].charInfo[locCharIndex].properties = discCharInfo->properties;
                    lastEndHandle[discIdx] = &cy_ble_wptsc[discIdx].charInfo[locCharIndex].endHandle;
                }
                else
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo);
                }
            }
        }

        /* Initialize characteristic endHandle to Service endHandle. Characteristic endHandle
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
* Function Name: Cy_BLE_WPTSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_FIND_INFO_RSP event.
*  Based on the descriptor UUID, an appropriate data structure is populated using
*  the data received as part of the callback.
*
*  \param discDescrInfo: The pointer to a descriptor information structure.
*
******************************************************************************/
static void Cy_BLE_WPTSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t notSupportedDescr = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t descIdx;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_WPTS)
    {
        switch(discDescrInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_CLIENT_CONFIG:
                descIdx = (uint32_t)CY_BLE_WPTS_CCCD;
                break;

            default:
                /* Not supported descriptor */
                notSupportedDescr = 1u;
                break;
        }

        if(notSupportedDescr == 0u)
        {
            if(cy_ble_wptsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] ==
               CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_wptsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] =
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
* Function Name: Cy_BLE_WPTSC_GetCharRange
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
static void Cy_BLE_WPTSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_WPTS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_WPTS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if((cy_ble_wptsc[discIdx].charInfo[charIdx].endHandle -
                cy_ble_wptsc[discIdx].charInfo[charIdx].valueHandle) != 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_wptsc[discIdx].charInfo[charIdx].valueHandle + 1u;
                charRangeInfo->range.endHandle = cy_ble_wptsc[discIdx].charInfo[charIdx].endHandle;
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
* Function Name: Cy_BLE_WPTSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles notification event for Wireless Power Transfer Service.
*
*  \param eventParam:  The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_WPTSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    cy_en_ble_wpts_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_WPTS_ApplCallback != NULL))
    {
        for(locCharIndex = CY_BLE_WPTS_PRU_CONTROL; locCharIndex < CY_BLE_WPTS_CHAR_COUNT; locCharIndex++)
        {
            if(cy_ble_wptsc[discIdx].charInfo[locCharIndex].valueHandle == eventParam->handleValPair.attrHandle)
            {
                cy_stc_ble_wpts_char_value_t notifValue =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = locCharIndex,
                    .value      = &eventParam->handleValPair.value
                };

                Cy_BLE_WPTS_ApplCallback((uint32_t)CY_BLE_EVT_WPTSC_NOTIFICATION, &notifValue);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_WPTSC_IndicationEventHandler
***************************************************************************//**
*
*  Handles an indication event for Wireless Power Transfer Service.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_WPTSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam)
{
    cy_en_ble_wpts_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_WPTS_ApplCallback != NULL))
    {
        for(locCharIndex = CY_BLE_WPTS_PRU_CONTROL; locCharIndex < CY_BLE_WPTS_CHAR_COUNT; locCharIndex++)
        {
            if(cy_ble_wptsc[discIdx].charInfo[locCharIndex].valueHandle == eventParam->handleValPair.attrHandle)
            {
                cy_stc_ble_wpts_char_value_t indicationValue =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = locCharIndex,
                    .value      = &eventParam->handleValPair.value
                };
                Cy_BLE_WPTS_ApplCallback((uint32_t)CY_BLE_EVT_WPTSC_INDICATION, &indicationValue);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_WPTSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles a Read Response event for the Wireless Power Transfer Service.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_WPTSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t locReqHandle = 0u;
    cy_en_ble_wpts_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_WPTS_ApplCallback != NULL) &&
       (cy_ble_wptscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locCharIndex = CY_BLE_WPTS_PRU_CONTROL; (locCharIndex < CY_BLE_WPTS_CHAR_COUNT) && (locReqHandle == 0u);
            locCharIndex++)
        {
            if(cy_ble_wptscReqHandle[discIdx] == cy_ble_wptsc[discIdx].charInfo[locCharIndex].valueHandle)
            {
                cy_stc_ble_wpts_char_value_t locCharValue =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = locCharIndex,
                    .value      = &eventParam->value
                };
                cy_ble_wptscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_WPTS_ApplCallback((uint32_t)CY_BLE_EVT_WPTSC_READ_CHAR_RESPONSE, &locCharValue);
                locReqHandle = 1u;
            }
            else if(cy_ble_wptscReqHandle[discIdx] == cy_ble_wptsc[discIdx].charInfo[locCharIndex].
                     descrHandle[CY_BLE_WPTS_CCCD])
            {
                cy_stc_ble_wpts_descr_value_t locDescrValue =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = locCharIndex,
                    .descrIndex = CY_BLE_WPTS_CCCD,
                    .value      = &eventParam->value
                };
                cy_ble_wptscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_WPTS_ApplCallback((uint32_t)CY_BLE_EVT_WPTSC_READ_DESCR_RESPONSE, &locDescrValue);
                locReqHandle = 1u;
            }
            else /* No more descriptors available */
            {
            }
        }
        if(locReqHandle != 0u)
        {
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_WPTSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Write Response event for the Wireless Power Transfer Service.
*
*  \param eventParam: The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_WPTSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t locReqHandle = 0u;
    cy_en_ble_wpts_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    /* Check if service handler was registered and request handle is valid */
    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_WPTS_ApplCallback != NULL) &&
       (cy_ble_wptscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locCharIndex = CY_BLE_WPTS_PRU_CONTROL; (locCharIndex < CY_BLE_WPTS_CHAR_COUNT) && (locReqHandle == 0u);
            locCharIndex++)
        {
            if(cy_ble_wptscReqHandle[discIdx] == cy_ble_wptsc[discIdx].charInfo[locCharIndex].valueHandle)
            {
                cy_stc_ble_wpts_char_value_t locCharValue =
                {
                    .connHandle = *eventParam,
                    .charIndex  = locCharIndex,
                    .value      = NULL
                };
                cy_ble_wptscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_WPTS_ApplCallback((uint32_t)CY_BLE_EVT_WPTSC_WRITE_CHAR_RESPONSE, &locCharValue);
                locReqHandle = 1u;
            }
            else if(cy_ble_wptscReqHandle[discIdx] == cy_ble_wptsc[discIdx].charInfo[locCharIndex].
                     descrHandle[CY_BLE_WPTS_CCCD])
            {
                cy_stc_ble_wpts_descr_value_t locDescrValue =
                {
                    .connHandle = *eventParam,
                    .charIndex  = locCharIndex,
                    .descrIndex = CY_BLE_WPTS_CCCD,
                    .value      = NULL
                };
                cy_ble_wptscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_WPTS_ApplCallback((uint32_t)CY_BLE_EVT_WPTSC_WRITE_DESCR_RESPONSE, &locDescrValue);
                locReqHandle = 1u;
            }
            else /* No more descriptors available */
            {
            }
        }
        if(locReqHandle != 0u)
        {
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_WPTSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles an Error Response event for the Wireless Power Transfer Service.
*
*  \param eventParam: The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_WPTSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
        if(eventParam->errInfo.attrHandle == cy_ble_wptscReqHandle[discIdx])
        {
            cy_ble_wptscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_WPTSC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_WPTSS_WRITE_CHAR events is generated.
*  On successful request execution on the Server side the Write Response is
*  sent to the Client.
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_wpts_char_index_t.
*  \param attrSize:        The size of the characteristic value attribute.
*  \param attrValue:      The pointer to the characteristic value data that
*                   should be send to the server device.
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
*   If the WPTS service-specific callback is registered
*      (with Cy_BLE_WPTS_RegisterAttrCallback):
*  * CY_BLE_EVT_WPTSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_wpts_char_value_t.
*  .
*   Otherwise (if the WPTS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WPTSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_wpts_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_WPTS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_wptsc[discIdx].charInfo[charIndex].valueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_wptsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);
        /* Save handle to support service-specific write response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_wptscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WPTSC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read a characteristic value, which is a value
*  identified by charIndex, from the server.
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_wpts_char_index_t.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
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
*   If the WPTS service-specific callback is registered
*      (with Cy_BLE_WPTS_RegisterAttrCallback):
*  * CY_BLE_EVT_WPTSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_wpts_char_value_t.
*  .
*   Otherwise (if the WPTS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_WPTSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_wpts_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_WPTS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_wptsc[discIdx].charInfo[charIndex].valueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_wptsc[discIdx].charInfo[charIndex].valueHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_wptscReqHandle[discIdx] = cy_ble_wptsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WPTSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  This function is used to write the characteristic descriptor to the server,
*  which is identified by charIndex and descrIndex.
*
*  Internally, Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server side the following events can be
*  generated:
*  * CY_BLE_EVT_WPTSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_WPTSS_NOTIFICATION_DISABLED
*  * CY_BLE_EVT_WPTSS_INDICATION_ENABLED
*  * CY_BLE_EVT_WPTSS_INDICATION_DISABLED
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_wpts_char_index_t.
*  \param descrIndex:      The index of a service characteristic descriptor of type
*                   cy_en_ble_wpts_descr_index_t.
*  \param attrSize:        The size of the characteristic value attribute.
*  \param attrValue:      The pointer to the characteristic descriptor value data that
*                   should be sent to the server device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
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
*   If the WPTS service-specific callback is registered
*      (with Cy_BLE_WPTS_RegisterAttrCallback):
*  * CY_BLE_EVT_WPTSC_WRITE_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_wpts_descr_value_t.
*  .
*   Otherwise (if the WPTS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_WPTSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_wpts_char_index_t charIndex,
                                                                cy_en_ble_wpts_descr_index_t descrIndex,
                                                                uint8_t attrSize,
                                                                uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_WPTS_CHAR_COUNT) || (descrIndex >= CY_BLE_WPTS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_wptsc[discIdx].charInfo[charIndex].descrHandle[descrIndex],
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* ... and send request to server's device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_wptscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_WPTSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Sends a request to get the characteristic descriptor of the specified
*  characteristic of the service.
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_wpts_char_index_t.
*  \param descrIndex:      The index of a service characteristic descriptor of type
*                   cy_en_ble_wpts_descr_index_t.
*
* \return
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the WPTS service-specific callback is registered
*      (with Cy_BLE_WPTS_RegisterAttrCallback):
*  * CY_BLE_EVT_WPTSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_wpts_descr_value_t.
*  .
*  Otherwise (if the WPTS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_WPTSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_wpts_char_index_t charIndex,
                                                                cy_en_ble_wpts_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_WPTS_CHAR_COUNT) || (descrIndex >= CY_BLE_WPTS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_wptsc[discIdx].charInfo[charIndex].descrHandle[descrIndex],
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_wptscReqHandle[discIdx] = cy_ble_wptsc[discIdx].charInfo[charIndex].descrHandle[descrIndex];
        }
    }

    return(apiResult);
}

#endif /* CY_BLE_WPTS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_WPTS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Wireless Power Transfer Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_WPTS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_WPTS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_WPTSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_WPTSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_WPTSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_WPTS_CLIENT */

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
        #ifdef CY_BLE_WPTS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_WPTSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF:
                Cy_BLE_WPTSS_ConfirmationEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_WPTS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_WPTS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_WPTSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_WPTSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_IND:
                Cy_BLE_WPTSC_IndicationEventHandler((cy_stc_ble_gattc_handle_value_ind_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_WPTSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_WPTSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_WPTS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_WPTS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
