/***************************************************************************//**
* \file cy_ble_rscs.c
* \version 2.0
*
* \brief
*  Contains the source code for Running Speed and Cadence Service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_RSCS))

static cy_ble_callback_t Cy_BLE_RSCS_ApplCallback = NULL;

#if defined(CY_BLE_RSCS_SERVER)
/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_rscssReqHandle;
#endif /* CY_BLE_RSCS_SERVER */

#if defined(CY_BLE_RSCS_CLIENT)
/* RSCS Center Service characteristics GATT DB handles structure */
cy_stc_ble_rscsc_t cy_ble_rscsc[CY_BLE_CONFIG_GATTC_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_rscscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_RSCS_CLIENT) */

/* The pointer on global BLE RSCS Config structure */
cy_stc_ble_rscs_config_t *cy_ble_rscsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#if defined(CY_BLE_RSCS_SERVER)
static cy_en_ble_gatt_err_code_t Cy_BLE_RSCSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
static void Cy_BLE_RSCSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
#endif /* CY_BLE_RSCS_SERVER */

#if defined(CY_BLE_RSCS_CLIENT)
static void Cy_BLE_RSCSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_RSCSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_RSCSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_RSCSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_RSCSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam);
static void Cy_BLE_RSCSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_RSCSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_RSCSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* (CY_BLE_RSCS_CLIENT) */
static cy_en_ble_gatt_err_code_t Cy_BLE_RSCS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_RSCS_Init
***************************************************************************//**
*
*  This function initializes the Running Speed and Cadence Service.
*
*  \param config: Configuration structure for the Running Speed and Cadence Service.
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
cy_en_ble_api_result_t Cy_BLE_RSCS_Init(cy_stc_ble_rscs_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_rscsConfigPtr = config;

        /* Registers Event Handler for the RSCS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_RSCS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_rscsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_RSCS_ApplCallback = cy_ble_rscsConfigPtr->callbackFunc;
        }

    #if defined(CY_BLE_RSCS_SERVER)
        cy_ble_rscssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    #endif /* CY_BLE_RSCS_SERVER */

    #ifdef CY_BLE_RSCS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_RSCS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_rscsc[idx], 0, sizeof(cy_stc_ble_rscsc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_RSCS].uuid = CY_BLE_UUID_RUNNING_SPEED_AND_CADENCE_SERVICE;
                }
                cy_ble_rscscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_RSCS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_RSCS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for Running Speed and Cadence Service specific
*  attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc: An application layer event callback function to receive
*                     events from the BLE Component. The definition of
*                     cy_ble_callback_t for RSCS is: \n
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
cy_en_ble_api_result_t Cy_BLE_RSCS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_RSCS_ApplCallback = callbackFunc;
    if(cy_ble_rscsConfigPtr != NULL)
    {
        cy_ble_rscsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#if defined(CY_BLE_RSCS_SERVER)

/******************************************************************************
* Function Name: Cy_BLE_RSCSS_WriteEventHandler
***************************************************************************//**
*
*  Handles the Write Request Event for the Running Speed and Cadence Service.
*
*  \param cy_stc_ble_gatts_write_cmd_req_param_t *eventParam: The pointer to the data that came
*                                             with a write request for the
*                                             Running Speed and Cadence Service.
*
* \return
*  Return a value of type cy_en_ble_gatt_err_code_t:
*   * CY_BLE_GATT_ERR_NONE - Function terminated successfully.
*   * CY_BLE_GATT_ERR_INVALID_HANDLE - The handle of a Client Configuration
*                                     Characteristic Descriptor is not valid.
*   * CY_BLE_GATT_ERR_UNLIKELY_ERROR - An Internal Stack error occurred.
*   * CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED - The notification or indication
*                                            property of a specific
*                                            Characteristic of Running Speed and
*                                            Cadence Service is disabled.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_RSCSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_stc_ble_rscs_char_value_t wrReqParam = { .connHandle = eventParam->connHandle };
    cy_ble_gatt_db_attr_handle_t tmpHandle;
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    uint32_t event = (uint32_t)CY_BLE_EVT_RSCSS_WRITE_CHAR;
    uint32_t handleWasFound = 1u;

    tmpHandle = eventParam->handleValPair.attrHandle;

    if(NULL != Cy_BLE_RSCS_ApplCallback)
    {
        /* Client Characteristic Configuration Descriptor Write Request */
        if(tmpHandle == cy_ble_rscsConfigPtr->rscss->charInfo[CY_BLE_RSCS_RSC_MEASUREMENT].descrHandle[CY_BLE_RSCS_CCCD])
        {
            /* Verify that optional notification property is enabled for the Characteristic */
            if(CY_BLE_IS_NOTIFICATION_SUPPORTED(cy_ble_rscsConfigPtr->rscss->charInfo[CY_BLE_RSCS_RSC_MEASUREMENT].
                                                 charHandle))
            {
                if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                {
                    event = (uint32_t)CY_BLE_EVT_RSCSS_NOTIFICATION_ENABLED;
                }
                else
                {
                    event = (uint32_t)CY_BLE_EVT_RSCSS_NOTIFICATION_DISABLED;
                }

                wrReqParam.charIndex = CY_BLE_RSCS_RSC_MEASUREMENT;

                /* Value is NULL for descriptors */
                wrReqParam.value = NULL;

            #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
                (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
                /* Set flag to store bonding data to flash */
                if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
                {
                    cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
                }
            #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */
            }
            else
            {
                gattErr = CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED;
            }
        }
        else if(tmpHandle == cy_ble_rscsConfigPtr->rscss->charInfo[CY_BLE_RSCS_SC_CONTROL_POINT].
                 descrHandle[CY_BLE_RSCS_CCCD])
        {
            /* Verify that optional indication property is enabled for the Characteristic */
            if(CY_BLE_IS_INDICATION_SUPPORTED(cy_ble_rscsConfigPtr->rscss->charInfo[CY_BLE_RSCS_SC_CONTROL_POINT].
                                               charHandle))
            {
                if(CY_BLE_IS_INDICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                {
                    event = (uint32_t)CY_BLE_EVT_RSCSS_INDICATION_ENABLED;
                }
                else
                {
                    event = (uint32_t)CY_BLE_EVT_RSCSS_INDICATION_DISABLED;
                }

                wrReqParam.charIndex = CY_BLE_RSCS_SC_CONTROL_POINT;

                /* Value is NULL for descriptors */
                wrReqParam.value = NULL;

            #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
                (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
                /* Set flag to store bonding data to flash */
                if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
                {
                    cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
                }
            #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */
            }
            else
            {
                gattErr = CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED;
            }
        }
        else if(tmpHandle == cy_ble_rscsConfigPtr->rscss->charInfo[CY_BLE_RSCS_SC_CONTROL_POINT].charHandle)
        {
            if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE == cy_ble_rscssReqHandle)
            {
                /* Verify if indication are enabled for SC Control Point Characteristic */
                if(CY_BLE_IS_INDICATION_SUPPORTED(cy_ble_rscsConfigPtr->rscss->charInfo[CY_BLE_RSCS_SC_CONTROL_POINT].
                                                   charHandle))
                {
                    if(CY_BLE_IS_INDICATION_ENABLED(eventParam->connHandle.attId,
                                                    cy_ble_rscsConfigPtr->rscss->charInfo[CY_BLE_RSCS_SC_CONTROL_POINT].
                                                     descrHandle[CY_BLE_RSCS_CCCD]))
                    {
                        event = (uint32_t)CY_BLE_EVT_RSCSS_WRITE_CHAR;
                        wrReqParam.charIndex = CY_BLE_RSCS_RSC_MEASUREMENT;
                        wrReqParam.value = &eventParam->handleValPair.value;
                    }
                    else
                    {
                        gattErr = CY_BLE_GATTS_ERR_CCCD_IMPROPERLY_CONFIGURED;
                    }
                }
                else
                {
                    gattErr = CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED;
                }
            }
            else
            {
                gattErr = CY_BLE_GATTS_ERR_PROCEDURE_ALREADY_IN_PROGRESS;
                cy_ble_rscssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
        else
        {
            /* No valid service handles were found */
            handleWasFound = 0u;
        }

        if((gattErr == CY_BLE_GATT_ERR_NONE) && (handleWasFound != 0u))
        {
            /* Write value to GATT database */
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
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                Cy_BLE_RSCS_ApplCallback(event, &wrReqParam);
            }
        }
    }

    if(CY_BLE_GATT_ERR_NONE != gattErr)
    {
        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }

    return(gattErr);
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCSS_ConfirmationEventHandler
****************************************************************************//**
*
*  Handles the Value Confirmation request event from the BLE stack.
*
*  \param cy_stc_ble_conn_handle_t *event_params: Pointer to a structure of type
*                                     cy_stc_ble_conn_handle_t.
*
*******************************************************************************/
static void Cy_BLE_RSCSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    if((Cy_BLE_RSCS_ApplCallback != NULL) && (cy_ble_rscssReqHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_rscssReqHandle == cy_ble_rscsConfigPtr->rscss->charInfo[CY_BLE_RSCS_SC_CONTROL_POINT].charHandle)
        {
            cy_stc_ble_rscs_char_value_t locCharValue =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_RSCS_SC_CONTROL_POINT,
                .value      = NULL
            };
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            cy_ble_rscssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_RSCS_ApplCallback((uint32_t)CY_BLE_EVT_RSCSS_INDICATION_CONFIRMATION, &locCharValue);
        }
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCSS_SetCharacteristicValue
****************************************************************************//**
*
*  Sets the characteristic value of the Running Speed and Cadence Service in the
*  local GATT database. The characteristic is identified by charIndex.
*
*  \param charIndex: The index of a service characteristic. Valid values are,
*              * CY_BLE_RSCS_RSC_FEATURE
*              * CY_BLE_RSCS_SENSOR_LOCATION.
*  \param attrSize:  The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               stored in the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_RSCSS_SetCharacteristicValue(cy_en_ble_rscs_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex == CY_BLE_RSCS_RSC_FEATURE) || (charIndex == CY_BLE_RSCS_SENSOR_LOCATION))
    {
        if(cy_ble_rscsConfigPtr->rscss->charInfo[charIndex].charHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            /* Store data in database */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_rscsConfigPtr->rscss->charInfo[charIndex].charHandle,
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .offset                     = 0u,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };
            if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_SUCCESS;
            }
        }
        else
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCSS_GetCharacteristicValue
****************************************************************************//**
*
*  Gets the characteristic value of the Running Speed and Cadence Service from
*  the GATT database. The characteristic is identified by charIndex.
*
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the location where characteristic value data
*              should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Peer device doesn't have a
*                                                particular characteristic
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_RSCSS_GetCharacteristicValue(cy_en_ble_rscs_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if(charIndex < CY_BLE_RSCS_CHAR_COUNT)
    {
        if(cy_ble_rscsConfigPtr->rscss->charInfo[charIndex].charHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            /* Read characteristic value from database */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_rscsConfigPtr->rscss->charInfo[charIndex].charHandle,
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };

            if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_SUCCESS;
            }
        }
        else
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCSS_GetCharacteristicDescriptor
****************************************************************************//**
*
*  Gets the characteristic descriptor of a specified characteristic of the Running
*  Speed and Cadence Service from the GATT database.
*
*  \param connHandle: The connection handle
*  \param charIndex: The index of a service characteristic. Valid values are,
*              * CY_BLE_RSCS_RSC_MEASUREMENT
*              * CY_BLE_RSCS_SC_CONTROL_POINT
*  \param descrIndex: The index of a service characteristic descriptor. Valid value is,
*              * CY_BLE_RSCS_CCCD
*  \param attrSize: The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the location where characteristic descriptor value
*               data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Peer device doesn't have a
*                                                particular descriptor
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_RSCSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_rscs_char_index_t charIndex,
                                                                cy_en_ble_rscs_descr_index_t descrIndex,
                                                                uint8_t attrSize,
                                                                uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if(((charIndex == CY_BLE_RSCS_RSC_MEASUREMENT) || (charIndex == CY_BLE_RSCS_SC_CONTROL_POINT)) &&
       (descrIndex == CY_BLE_RSCS_CCCD))
    {
        if(cy_ble_rscsConfigPtr->rscss->charInfo[charIndex].descrHandle[descrIndex] !=
           CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_rscsConfigPtr->rscss->charInfo[charIndex].descrHandle[descrIndex],
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .connHandle                 = connHandle,
                .offset                     = 0u,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };
            if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_SUCCESS;
            }
        }
        else
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCSS_SendNotification
****************************************************************************//**
*
*  Sends a notification with the characteristic value to the Client device. This
*  is specified by charIndex of the Running Speed and Cadence Service.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_RSCSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a service characteristic. Valid value is,
*              * CY_BLE_RSCS_RSC_MEASUREMENT
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be sent
*               to the client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of input parameter is failed
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this.
*                                      characteristic.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the client
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_RSCSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     cy_en_ble_rscs_char_index_t charIndex,
                                                     uint8_t attrSize,
                                                     uint8_t *attrValue)
{
    /* Store new data in database */
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if(charIndex == CY_BLE_RSCS_RSC_MEASUREMENT)
    {
        /* Send Notification if it is enabled and connected */
        if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
        {
            apiResult = CY_BLE_ERROR_INVALID_STATE;
        }
        else if(!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_rscsConfigPtr->rscss->
                                                 charInfo[charIndex].descrHandle[CY_BLE_RSCS_CCCD]))
        {
            apiResult = CY_BLE_ERROR_NTF_DISABLED;
        }
        else
        {
            /* Fill all fields of write request structure ... */
            cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
            {
                .handleValPair.attrHandle = cy_ble_rscsConfigPtr->rscss->charInfo[charIndex].charHandle,
                .handleValPair.value.val  = attrValue,
                .handleValPair.value.len  = attrSize,
                .connHandle               = connHandle,
            };
            /* Send notification to client using previously filled structure */
            apiResult = Cy_BLE_GATTS_Notification(&ntfReqParam);
        }
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCSS_SendIndication
****************************************************************************//**
*
*  Sends an indication with a characteristic value to the Client device. This is
*  specified by charIndex of the Running Speed and Cadence Service.
*
*  On enabling indication successfully it sends out a 'Handle Value Indication' which
*  results in CY_BLE_EVT_RSCSC_INDICATION or CY_BLE_EVT_GATTC_HANDLE_VALUE_IND (if
*  service-specific callback function is not registered) event at the GATT Client's end.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be sent
*               to the client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of input parameter is failed
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this.
*                                      characteristic.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_IND_DISABLED - Indication is not enabled by the client
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Peer device doesn't have a
*                                                particular characteristic
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the RSCS service-specific callback is registered
*      (with Cy_BLE_RSCS_RegisterAttrCallback):
*  * CY_BLE_EVT_RSCSS_INDICATION_CONFIRMED - in case if the indication is
*                                successfully delivered to the peer device.
*  .
*   Otherwise (if the RSCS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF - in case if the indication is
*                                successfully delivered to the peer device.
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_RSCSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                   cy_en_ble_rscs_char_index_t charIndex,
                                                   uint8_t attrSize,
                                                   uint8_t *attrValue)
{
    /* Store new data in database */
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if(charIndex == CY_BLE_RSCS_SC_CONTROL_POINT)
    {
        /* Send Indication if it is enabled and connected */
        if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
        {
            apiResult = CY_BLE_ERROR_INVALID_STATE;
        }
        else if(!CY_BLE_IS_INDICATION_ENABLED(connHandle.attId, cy_ble_rscsConfigPtr->rscss->
                                               charInfo[charIndex].descrHandle[CY_BLE_RSCS_CCCD]))
        {
            apiResult = CY_BLE_ERROR_IND_DISABLED;
        }
        else
        {
            if(cy_ble_rscsConfigPtr->rscss->charInfo[charIndex].charHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                /* Fill all fields of write request structure ... */
                cy_stc_ble_gatts_handle_value_ind_t indReqParam =
                {
                    .handleValPair.attrHandle = cy_ble_rscsConfigPtr->rscss->charInfo[charIndex].charHandle,
                    .handleValPair.value.val  = attrValue,
                    .handleValPair.value.len  = attrSize,
                    .connHandle               = connHandle
                };
                /* Send indication to client using previously filled structure */
                apiResult = Cy_BLE_GATTS_Indication(&indReqParam);
                /* Save handle to support service-specific value confirmation response from client */
                if(apiResult == CY_BLE_SUCCESS)
                {
                    cy_ble_rscssReqHandle = indReqParam.handleValPair.attrHandle;
                }
            }
            else
            {
                apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
            }
        }
    }

    return(apiResult);
}

#endif /* CY_BLE_RSCS_SERVER */


#if defined(CY_BLE_RSCS_CLIENT)

/*******************************************************************************
* Function Name: Cy_BLE_RSCSC_DiscoverCharacteristicsEventHandler
****************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
*******************************************************************************/
static void Cy_BLE_RSCSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* RSCS characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_rscscCharUuid[CY_BLE_RSCS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_RSC_MSRMT,
        CY_BLE_UUID_CHAR_RSC_FEATURE,
        CY_BLE_UUID_CHAR_SENSOR_LOCATION,
        CY_BLE_UUID_CHAR_SC_CONTROL_POINT
    };
    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_RSCS))
    {
        /* Update last characteristic endHandle to declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        for(i = (uint32_t)CY_BLE_RSCS_RSC_MEASUREMENT; i < (uint32_t)CY_BLE_RSCS_CHAR_COUNT; i++)
        {
            if(cy_ble_rscscCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_rscsc[discIdx].characteristics[i].charInfo.valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_rscsc[discIdx].characteristics[i].charInfo.valueHandle = discCharInfo->valueHandle;
                    cy_ble_rscsc[discIdx].characteristics[i].charInfo.properties = discCharInfo->properties;
                    lastEndHandle[discIdx] = &cy_ble_rscsc[discIdx].characteristics[i].endHandle;
                    break;
                }
                else
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo);
                }
            }
        }

        /* Initially characteristic endHandle equals to the Service endHandle.
         * Characteristic endHandle will be updated to declaration handler of the
         * following characteristic,in the following characteristic discovery
         * procedure.
         */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCS_DiscoverCharDescriptorsEventHandler
****************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_FIND_INFO_RSP event.
*  This event is generated when a server successfully sends the data for
*  CY_BLE_EVT_GATTC_FIND_INFO_REQ. Based on the service UUID, an appropriate data
*  structure is populated to the service with a service callback.
*
*  \param cy_stc_ble_disc_descr_info_t * discCharInfo: The pointer to a characteristic information
*                                          structure.
*
*******************************************************************************/
static void Cy_BLE_RSCSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t * discDescrInfo)
{
    uint32_t notSupportedDescr = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t descIdx;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_RSCS)
    {
        switch(discDescrInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_CLIENT_CONFIG:
                descIdx = (uint32_t)CY_BLE_RSCS_CCCD;
                break;

            default:
                /* Not supported descriptor */
                notSupportedDescr = 1u;
                break;
        }

        if(notSupportedDescr == 0u)
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if(cy_ble_rscsc[discIdx].characteristics[charIdx].descriptors[descIdx] ==
               CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_rscsc[discIdx].characteristics[charIdx].descriptors[descIdx] = discDescrInfo->descrHandle;
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
* Function Name: Cy_BLE_RSCSC_GetCharRange
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
static void Cy_BLE_RSCSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_RSCS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_RSCS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if((cy_ble_rscsc[discIdx].characteristics[charIdx].endHandle -
                cy_ble_rscsc[discIdx].characteristics[charIdx].charInfo.valueHandle) != 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_rscsc[discIdx].characteristics[charIdx].charInfo.valueHandle + 1u;
                charRangeInfo->range.endHandle = cy_ble_rscsc[discIdx].characteristics[charIdx].endHandle;
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


/*******************************************************************************
* Function Name: Cy_BLE_RSCSC_NotificationEventHandler
****************************************************************************//**
*
*  Handles the Notification Event for the Running Speed and Cadence Service.
*
*  \param cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam: The pointer to the
*                                                    cy_stc_ble_gattc_handle_value_ntf_param_t
*                                                    data structure specified by
*                                                    the event.
*
*******************************************************************************/
static void Cy_BLE_RSCSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_RSCS_ApplCallback != NULL))
    {
        if(cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_RSC_MEASUREMENT].charInfo.valueHandle ==
           eventParam->handleValPair.attrHandle)
        {
            cy_stc_ble_rscs_char_value_t ntfParam =
            {
                .charIndex  = CY_BLE_RSCS_RSC_MEASUREMENT,
                .connHandle = eventParam->connHandle,
                .value      = &eventParam->handleValPair.value
            };

            Cy_BLE_RSCS_ApplCallback((uint32_t)CY_BLE_EVT_RSCSC_NOTIFICATION, (void*)&ntfParam);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCSC_IndicationEventHandler
****************************************************************************//**
*
*  Handles the Indication Event for the Running Speed and Cadence Service.
*
*  \param cy_stc_ble_gattc_handle_value_ind_param_t *eventParam: The pointer to the
*                                                    cy_stc_ble_gattc_handle_value_ntf_param_t
*                                                    data structure specified by the event.
*
*******************************************************************************/
static void Cy_BLE_RSCSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_RSCS_ApplCallback != NULL))
    {
        if(cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_SC_CONTROL_POINT].charInfo.valueHandle ==
           eventParam->handleValPair.attrHandle)
        {
            cy_stc_ble_rscs_char_value_t ntfParam =
            {
                .charIndex  = CY_BLE_RSCS_SC_CONTROL_POINT,
                .connHandle = eventParam->connHandle,
                .value      = &eventParam->handleValPair.value,
            };
            Cy_BLE_RSCS_ApplCallback((uint32_t)CY_BLE_EVT_RSCSC_INDICATION, (void*)&ntfParam);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCSC_ReadResponseEventHandler
****************************************************************************//**
*
*  Handles the Read Response Event for the Running Speed and Cadence Service.
*
*  \param cy_stc_ble_gattc_read_rsp_param_t *eventParam: The pointer to the data that came
*                                            with a read response for RSCS.
*
*******************************************************************************/
static void Cy_BLE_RSCSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t fFlag = 1u;
    uint32_t attrVal = 0u;
    cy_en_ble_rscs_char_index_t idx;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_RSCS_ApplCallback != NULL) &&
       (cy_ble_rscscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_RSC_FEATURE].charInfo.valueHandle ==
           cy_ble_rscscReqHandle[discIdx])
        {
            idx = CY_BLE_RSCS_RSC_FEATURE;
        }
        else if(cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_SENSOR_LOCATION].charInfo.valueHandle ==
                cy_ble_rscscReqHandle[discIdx])
        {
            idx = CY_BLE_RSCS_SENSOR_LOCATION;
        }
        else if(cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_RSC_MEASUREMENT].descriptors[CY_BLE_RSCS_CCCD] ==
                cy_ble_rscscReqHandle[discIdx])
        {
            /* Attribute is Characteristic Descriptor */
            attrVal = 1u;
            idx = CY_BLE_RSCS_RSC_MEASUREMENT;
        }
        else if(cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_SC_CONTROL_POINT].descriptors[CY_BLE_RSCS_CCCD] ==
                cy_ble_rscscReqHandle[discIdx])
        {
            /* Attribute is Characteristic Descriptor */
            attrVal = 1u;
            idx = CY_BLE_RSCS_SC_CONTROL_POINT;
        }
        else
        {
            /* No RSCS Characteristics were requested for read */
            fFlag = 0u;
        }

        if(fFlag != 0u)
        {
            /* Read response for characteristic */
            if(attrVal == 0u)
            {
                /* Fill Running Speed and Cadence Service read response parameter structure with
                 * Characteristic info. */
                cy_stc_ble_rscs_char_value_t rdRspParam = { .connHandle = eventParam->connHandle };

                rdRspParam.charIndex = idx;
                rdRspParam.value = &eventParam->value;

                Cy_BLE_RSCS_ApplCallback((uint32_t)CY_BLE_EVT_RSCSC_READ_CHAR_RESPONSE, (void*)&rdRspParam);
            }
            else /* Read response for characteristic descriptor */
            {
                /* Fill Running Speed and Cadence Service read response parameter structure with
                 * Characteristic Descriptor info. */
                cy_stc_ble_rscs_descr_value_t rdRspParam =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = idx,
                    .descrIndex = CY_BLE_RSCS_CCCD,
                    .value      = &eventParam->value
                };
                Cy_BLE_RSCS_ApplCallback((uint32_t)CY_BLE_EVT_RSCSC_READ_DESCR_RESPONSE, (void*)&rdRspParam);
            }

            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            cy_ble_rscscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCSC_WriteResponseEventHandler
****************************************************************************//**
*
*  Handles the Write Response Event for the Running Speed and Cadence Service.
*
*  \param cy_stc_ble_conn_handle_t *eventParam: The pointer to the cy_stc_ble_conn_handle_t data
*                                   structure.
*
*******************************************************************************/
static void Cy_BLE_RSCSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t fFlag = 1u;
    uint32_t attrType = 0u;
    cy_en_ble_rscs_char_index_t idx;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_RSCS_ApplCallback != NULL) &&
       (cy_ble_rscscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_RSC_FEATURE].charInfo.valueHandle ==
           cy_ble_rscscReqHandle[discIdx])
        {
            idx = CY_BLE_RSCS_RSC_FEATURE;
        }
        else if(cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_SENSOR_LOCATION].charInfo.valueHandle ==
                cy_ble_rscscReqHandle[discIdx])
        {
            idx = CY_BLE_RSCS_SENSOR_LOCATION;
        }
        else if(cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_SC_CONTROL_POINT].charInfo.valueHandle ==
                cy_ble_rscscReqHandle[discIdx])
        {
            idx = CY_BLE_RSCS_SC_CONTROL_POINT;
        }
        else if(cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_RSC_MEASUREMENT].descriptors[CY_BLE_RSCS_CCCD] ==
                cy_ble_rscscReqHandle[discIdx])
        {
            /* Attribute is Characteristic Descriptor */
            attrType = 1u;
            idx = CY_BLE_RSCS_RSC_MEASUREMENT;
        }
        else if(cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_SC_CONTROL_POINT].descriptors[CY_BLE_RSCS_CCCD] ==
                cy_ble_rscscReqHandle[discIdx])
        {
            /* Attribute is Characteristic Descriptor */
            attrType = 1u;
            idx = CY_BLE_RSCS_SC_CONTROL_POINT;
        }
        else
        {
            /* No RSCS Characteristics were requested for write */
            fFlag = 0u;
        }

        if(fFlag != 0u)
        {
            /* This should be cleared before calling to */
            cy_ble_rscscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;

            /* Write response for characteristic */
            if(attrType == 0u)
            {
                /* Fill Running Speed and Cadence Service write response parameter structure with
                 * Characteristic info. */
                cy_stc_ble_rscs_char_value_t rdRspParam = { .connHandle = *eventParam };
                rdRspParam.charIndex = idx;
                rdRspParam.value = NULL;

                Cy_BLE_RSCS_ApplCallback((uint32_t)CY_BLE_EVT_RSCSC_WRITE_CHAR_RESPONSE, (void*)&rdRspParam);
            }
            else /* Write response for characteristic descriptor */
            {
                /* Fill Running Speed and Cadence Service write response parameter structure with
                 * Characteristic Descriptor info. */
                cy_stc_ble_rscs_descr_value_t rdRspParam = { .connHandle = *eventParam };
                rdRspParam.charIndex = idx;
                rdRspParam.descrIndex = CY_BLE_RSCS_CCCD;
                rdRspParam.value = NULL;

                Cy_BLE_RSCS_ApplCallback((uint32_t)CY_BLE_EVT_RSCSC_WRITE_DESCR_RESPONSE, (void*)&rdRspParam);
            }

            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCSC_ErrorResponseEventHandler
****************************************************************************//**
*
*  Handles the Error Response Event for the Running Speed and Cadence Service.
*
*  \param  *eventParam: The pointer to the cy_stc_ble_gatt_err_param_t structure.
*
*******************************************************************************/
static void Cy_BLE_RSCSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if(cy_ble_rscscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        cy_ble_rscscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCSC_SetCharacteristicValue
****************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_RSCSS_WRITE_CHAR event is generated.
*  On successful request execution on the Server side the Write Response is
*  sent to the Client.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of a service characteristic.
*  \param attrSize:   Size of the characteristic value attribute.
*  \param attrValue: Pointer to the characteristic value data that should be
*              sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request was sent successfully
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Peer device doesn't have a
*                                                particular characteristic
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the RSCS service-specific callback is registered
*      (with Cy_BLE_RSCS_RegisterAttrCallback):
*  * CY_BLE_EVT_RSCSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_rscs_char_value_t.
*  .
*   Otherwise (if the RSCS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_RSCSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_rscs_char_index_t charIndex,
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
            (charIndex == CY_BLE_RSCS_SC_CONTROL_POINT))
    {
        if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE !=
           cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_SC_CONTROL_POINT].charInfo.valueHandle)
        {
            /* Fill all fields of write command request structure ... */
            cy_stc_ble_gattc_write_req_t wrReqParam =
            {
                .handleValPair.value.val  = attrValue,
                .handleValPair.value.len  = attrSize,
                .handleValPair.attrHandle = cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_SC_CONTROL_POINT].
                                             charInfo.valueHandle,
                .connHandle               = connHandle
            };
            /* Send request to write characteristic value */
            apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&wrReqParam);

            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_rscscReqHandle[discIdx] = wrReqParam.handleValPair.attrHandle;
            }
        }
        else
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
    }
    else
    {
        /* apiResult equals to CY_BLE_ERROR_INVALID_PARAMETER */
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCSC_GetCharacteristicValue
****************************************************************************//**
*
*  Sends a request to the peer device to set the characteristic value of the
*  Running Speed and Cadence Service.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request was sent successfully
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Peer device doesn't have a
*                                                particular characteristic
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the RSCS service-specific callback is registered
*      (with Cy_BLE_RSCS_RegisterAttrCallback):
*  * CY_BLE_EVT_RSCSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_rscs_char_value_t.
*  .
*   Otherwise (if the RSCS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_RSCSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_rscs_char_index_t charIndex)
{
    cy_stc_ble_gattc_read_req_t readReqParam = { .connHandle = connHandle };
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
        /* Select characteristic */
        switch(charIndex)
        {
            case CY_BLE_RSCS_RSC_FEATURE:
                readReqParam.attrHandle = cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_RSC_FEATURE].
                                           charInfo.valueHandle;
                break;

            case CY_BLE_RSCS_SENSOR_LOCATION:
                readReqParam.attrHandle = cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_SENSOR_LOCATION].
                                           charInfo.valueHandle;
                break;

            default:
                /* Characteristic wasn't found */
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                break;
        }

        if(apiResult == CY_BLE_SUCCESS)
        {
            if(readReqParam.attrHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                /* Send request to read characteristicW value */
                apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);
                if(apiResult == CY_BLE_SUCCESS)
                {
                    cy_ble_rscscReqHandle[discIdx] = readReqParam.attrHandle;
                }
            }
            else
            {
                apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
            }
        }
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCSC_SetCharacteristicDescriptor
****************************************************************************//**
*
*  Sends a request to the peer device to get the characteristic descriptor of
*  the specified characteristic of the Running Speed and Cadence Service.
*
*  Internally, Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server side the following events can be
*  generated:
*  * CY_BLE_EVT_RSCSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_RSCSS_NOTIFICATION_DISABLED
*  * CY_BLE_EVT_RSCSS_INDICATION_ENABLED
*  * CY_BLE_EVT_RSCSS_INDICATION_DISABLED
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of a RSCS characteristic.
*  \param descrIndex: The index of a RSCS characteristic descriptor.
*  \param attrSize:   The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the characteristic descriptor value data that
*              should be sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - the  request was sent successfully
*   * CY_BLE_ERROR_INVALID_STATE - connection with the client is not established
*   * CY_BLE_ERROR_INVALID_PARAMETER - validation of the input parameters failed
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Peer device doesn't have a
*                                                particular descriptor
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the RSCS service-specific callback is registered
*      (with Cy_BLE_RSCS_RegisterAttrCallback):
*  * CY_BLE_EVT_RSCSC_WRITE_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_rscs_descr_value_t.
*  .
*   Otherwise (if the RSCS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_RSCSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_rscs_char_index_t charIndex,
                                                                cy_en_ble_rscs_descr_index_t descrIndex,
                                                                uint8_t attrSize,
                                                                uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((attrValue != NULL) && (descrIndex == CY_BLE_RSCS_CCCD) && (attrSize == CY_BLE_CCCD_LEN))
    {
        if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && ((charIndex == CY_BLE_RSCS_RSC_MEASUREMENT) ||
           (charIndex == CY_BLE_RSCS_SC_CONTROL_POINT)))
        {
            /* Fill all fields of write request structure ... */
            cy_stc_ble_gattc_write_req_t writeReqParam =
            {
                .connHandle              = connHandle,
                .handleValPair.value.val = attrValue,
                .handleValPair.value.len = attrSize
            };
            if(charIndex == CY_BLE_RSCS_RSC_MEASUREMENT)
            {
                writeReqParam.handleValPair.attrHandle =
                    cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_RSC_MEASUREMENT].descriptors[CY_BLE_RSCS_CCCD];
            }
            else
            {
                writeReqParam.handleValPair.attrHandle =
                    cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_SC_CONTROL_POINT].descriptors[CY_BLE_RSCS_CCCD];
            }

            if(writeReqParam.handleValPair.attrHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                /* ... and send request to server device. */
                apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

                if(apiResult == CY_BLE_SUCCESS)
                {
                    cy_ble_rscscReqHandle[discIdx] =
                        cy_ble_rscsc[discIdx].characteristics[charIndex].descriptors[CY_BLE_RSCS_CCCD];
                }
            }
            else
            {
                apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
            }
        }
    }
    else
    {
        /* alidation of input parameters is failed */
    }

    /* Return the status */
    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_RSCSC_GetCharacteristicDescriptor
****************************************************************************//**
*
*  Sends a request to the peer device to get characteristic descriptor of the
*  specified characteristic of the Running Speed and Cadence Service.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a Service Characteristic.
*  \param descrIndex: The index of a Service Characteristic Descriptor.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request was sent successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_INVALID_OPERATION - Cannot process a request to send PDU due to
*                                     invalid operation performed by the
*                                     application
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Peer device doesn't have a
*                                                particular descriptor
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the RSCS service-specific callback is registered
*      (with Cy_BLE_RSCS_RegisterAttrCallback):
*  * CY_BLE_EVT_RSCSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_rscs_descr_value_t.
*  .
*  Otherwise (if the RSCS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_RSCSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_rscs_char_index_t charIndex,
                                                                uint8_t descrIndex)
{
    cy_stc_ble_gattc_read_req_t readReqParam = { .connHandle = connHandle };
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && 
            ((charIndex == CY_BLE_RSCS_RSC_MEASUREMENT) || (charIndex == CY_BLE_RSCS_SC_CONTROL_POINT)) &&
            (descrIndex == (uint8_t)CY_BLE_RSCS_CCCD))
    {
        if(charIndex == CY_BLE_RSCS_RSC_MEASUREMENT)
        {
            readReqParam.attrHandle =
                cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_RSC_MEASUREMENT].descriptors[CY_BLE_RSCS_CCCD];
        }
        else
        {
            readReqParam.attrHandle =
                cy_ble_rscsc[discIdx].characteristics[CY_BLE_RSCS_SC_CONTROL_POINT].descriptors[CY_BLE_RSCS_CCCD];
        }

        if(readReqParam.attrHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);

            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_rscscReqHandle[discIdx] = readReqParam.attrHandle;
            }
        }
        else
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
    }
    else
    {
        /* Characteristic has not been discovered or had invalid fields */
    }

    return(apiResult);
}

#endif /* (CY_BLE_RSCS_CLIENT) */

/******************************************************************************
* Function Name: Cy_BLE_RSCS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Running Speed and Cadence Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_RSCS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_RSCS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_RSCSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_RSCSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_RSCSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_RSCS_CLIENT */

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
        #ifdef CY_BLE_RSCS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_RSCSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF:
                Cy_BLE_RSCSS_ConfirmationEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;

            case CY_BLE_EVT_GATT_CONNECT_IND:
                (void)Cy_BLE_RSCS_Init(cy_ble_rscsConfigPtr);
                break;
        #endif /* CY_BLE_RSCS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_RSCS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_RSCSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_RSCSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_IND:
                Cy_BLE_RSCSC_IndicationEventHandler((cy_stc_ble_gattc_handle_value_ind_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_RSCSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_RSCSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_RSCS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_RSCS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */

