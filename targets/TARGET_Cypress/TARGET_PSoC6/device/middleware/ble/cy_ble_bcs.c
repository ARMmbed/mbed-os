/***************************************************************************//**
* \file cy_ble_bcs.c
* \version 2.0
*
* \brief
*  Contains the source code for the Body Composition Service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_BCS))

#ifdef CY_BLE_BCS_SERVER
/* The internal storage for the last request handle to check response for server */
static cy_ble_gatt_db_attr_handle_t cy_ble_bcssReqHandle;
#endif /* CY_BLE_BCS_SERVER */

#ifdef CY_BLE_BCS_CLIENT
/* BCS Center Service characteristics GATT DB handles structure */
cy_stc_ble_bcsc_t cy_ble_bcsc[CY_BLE_CONFIG_GATTC_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_bcscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_BCS_CLIENT) */

static cy_ble_callback_t Cy_BLE_BCS_ApplCallback = NULL;

/* The pointer to the global BLE BCS config structure */
cy_stc_ble_bcs_config_t *cy_ble_bcsConfigPtr;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_BCS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_BCSS_WriteEventHandler(const cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
static void Cy_BLE_BCSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
#endif /* CY_BLE_BCS_SERVER */

#ifdef CY_BLE_BCS_CLIENT
static void Cy_BLE_BCSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_BCSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_BCSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_BCSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam);
static void Cy_BLE_BCSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_BCSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_BCSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* CY_BLE_BCS_CLIENT */
static cy_en_ble_gatt_err_code_t Cy_BLE_BCS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_BCS_Init
***************************************************************************//**
*
*  This function initializes the Body Composition Service.
*
*  \param config: The configuration structure for the Body Composition Service.
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
cy_en_ble_api_result_t Cy_BLE_BCS_Init(cy_stc_ble_bcs_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure*/
        cy_ble_bcsConfigPtr = config;

        /* Registers Event Handler for the BCS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_BCS_EventHandler);

        /* Registers a callback function via config structure*/
        if(cy_ble_bcsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_BCS_ApplCallback = cy_ble_bcsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_BCS_SERVER
        cy_ble_bcssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    #endif /* CY_BLE_BCS_SERVER */

    #ifdef CY_BLE_BCS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_BCS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_bcsc[idx], 0, sizeof(cy_stc_ble_bcsc_t));

                    /* Initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_BCS].uuid = CY_BLE_UUID_BODY_COMPOSITION_SERVICE;
                }
                cy_ble_bcscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_BCS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BCS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific Write requests from a peer device will not be handled with
*  an unregistered callback function.
*
*  \param callbackFunc: An application layer event callback function to receive
*                 events from the BLE Component. The definition of
*                 cy_ble_callback_t is: \n
*                 typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                    void *eventParam)
*                 * eventCode: Indicates the event that triggered this
*                   callback (e.g. CY_BLE_EVT_BCSS_INDICATION_ENABLED).
*                 * eventParam: Contains the parameters corresponding to the
*                   current event. (e.g. the pointer to cy_stc_ble_bcs_char_value_t
*                   structure that contains details of the characteristic
*                   the CY_BLE_EVT_BCSS_INDICATION_ENABLED event triggered for).
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
cy_en_ble_api_result_t Cy_BLE_BCS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_BCS_ApplCallback = callbackFunc;
    if(cy_ble_bcsConfigPtr != NULL)
    {
        cy_ble_bcsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_BCS_SERVER

/******************************************************************************
* Function Name: Cy_BLE_BCSS_WriteEventHandler
***************************************************************************//**
*
*  Handles a Write Request event for the Body Composition Service.
*
*  \param void *eventParam - The pointer to the data structure specified by the event.
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*   * CY_BLE_GATT_ERR_NONE - The Write is successful.
*   * CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED - The request is not supported.
*   * CY_BLE_GATT_ERR_INVALID_HANDLE - 'handleValuePair.attrHandle' is not valid.
*   * CY_BLE_GATT_ERR_WRITE_NOT_PERMITTED - The Write operation is not permitted on
*                                          this attribute.
*   * CY_BLE_GATT_ERR_INVALID_OFFSET - The offset value is invalid.
*   * CY_BLE_GATT_ERR_UNLIKELY_ERROR - Some other error occurred.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_BCSS_WriteEventHandler(const cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    uint32_t event = (uint32_t)CY_BLE_EVT_BCSS_INDICATION_DISABLED;

    if(Cy_BLE_BCS_ApplCallback != NULL)
    {
        /* Client Characteristic Configuration descriptor Write request */
        if(eventParam->handleValPair.attrHandle ==
           cy_ble_bcsConfigPtr->bcss->charInfo[CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT].descrHandle[CY_BLE_BCS_CCCD])
        {
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair = eventParam->handleValPair,
                .connHandle      = eventParam->connHandle,
                .offset          = 0u,
                .flags           = CY_BLE_GATT_DB_PEER_INITIATED
            };
            if(CY_BLE_IS_INDICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
            {
                event = (uint32_t)CY_BLE_EVT_BCSS_INDICATION_ENABLED;
            }

            gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);
            if(gattErr == CY_BLE_GATT_ERR_NONE)
            {
                cy_stc_ble_bcs_char_value_t wrReqParam =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT,
                    .value      = NULL
                };
                Cy_BLE_BCS_ApplCallback(event, &wrReqParam);
            }

        #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
            (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
            /* Set a flag to store bonding data to flash */
            if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
            {
                cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
            }
        #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */

            /* Clear a callback flag indicating that the request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }

    return(gattErr);
}


/******************************************************************************
* Function Name: Cy_BLE_BCSS_ConfirmationEventHandler
***************************************************************************//**
*
*  Handles a Value Confirmation request event from the BLE stack.
*
*  \param eventParam: The pointer to a structure of type cy_stc_ble_conn_handle_t.
*
******************************************************************************/
static void Cy_BLE_BCSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    if((Cy_BLE_BCS_ApplCallback != NULL) && (cy_ble_bcssReqHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        /* Only Weight Measurement characteristic has the Indication property. Check if
         * the requested handle is the handle of the Descriptor Value Change handle.
         */
        if(cy_ble_bcssReqHandle == cy_ble_bcsConfigPtr->bcss->charInfo[CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT].charHandle)
        {
            /* Fill in the event data and inform Application about successfully
             * confirmed indication.
             */
            cy_stc_ble_bcs_char_value_t locCharValue =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT,
                .value      = NULL
            };
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            cy_ble_bcssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_BCS_ApplCallback((uint32_t)CY_BLE_EVT_BCSS_INDICATION_CONFIRMED, &locCharValue);
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BCSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets a value for one of the three characteristic values of the Body Composition
*  Service. The characteristic is identified by charIndex.
*
*  \param charIndex: The index of a Body Composition Service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*             stored to the GATT database.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic value was written successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BCSS_SetCharacteristicValue(cy_en_ble_bcs_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo;

    if((attrValue != NULL) && (charIndex < CY_BLE_BCS_CHAR_COUNT))
    {
        /* Fill the structure */
        if(charIndex == CY_BLE_BCS_BODY_COMPOSITION_FEATURE)
        {
            dbAttrValInfo.handleValuePair.attrHandle = cy_ble_bcsConfigPtr->
                                                        bcss->charInfo[CY_BLE_BCS_BODY_COMPOSITION_FEATURE].charHandle;
        }
        else
        {
            dbAttrValInfo.handleValuePair.attrHandle = cy_ble_bcsConfigPtr->
                                                        bcss->charInfo[CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT].charHandle;
        }
        dbAttrValInfo.handleValuePair.value.len = attrSize;
        dbAttrValInfo.handleValuePair.value.val = attrValue;
        dbAttrValInfo.offset = 0u;
        dbAttrValInfo.flags = CY_BLE_GATT_DB_LOCALLY_INITIATED;
        if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            /* Indicate success */
            apiResult = CY_BLE_SUCCESS;
        }
    }

    /* Return a status */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BCSS_GetCharacteristicValue
***************************************************************************//**
*
*  Reads a characteristic value of the Body Composition Service identified
*  by charIndex from the GATT database.
*
*  \param charIndex: The index of the Body Composition Service characteristic.
*  \param attrSize: The size of the Body Composition Service characteristic value attribute.
*  \param attrValue: The pointer to the location where characteristic value data
*              should be stored.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic value was read successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BCSS_GetCharacteristicValue(cy_en_ble_bcs_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_ble_gatt_db_attr_handle_t tmpCharHandle;
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((attrValue != NULL) && (charIndex < CY_BLE_BCS_CHAR_COUNT))
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.value.len = attrSize,
            .handleValuePair.value.val = attrValue,
            .flags                     = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };

        if(charIndex == CY_BLE_BCS_BODY_COMPOSITION_FEATURE)
        {
            tmpCharHandle = cy_ble_bcsConfigPtr->bcss->charInfo[CY_BLE_BCS_BODY_COMPOSITION_FEATURE].charHandle;
        }
        else
        {
            tmpCharHandle = cy_ble_bcsConfigPtr->bcss->charInfo[CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT].charHandle;
        }
        dbAttrValInfo.handleValuePair.attrHandle = tmpCharHandle;

        /* Get a characteristic value from the GATT database */
        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            /* Indicate success */
            apiResult = CY_BLE_SUCCESS;
        }
    }

    /* Return a status */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BCSS_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sets the characteristic descriptor of the specified characteristic.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param attrSize: The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the descriptor value data to be stored in the GATT
*              database.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BCSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bcs_char_index_t charIndex,
                                                               cy_en_ble_bcs_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex == CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT) && (descrIndex == CY_BLE_BCS_CCCD) && (attrValue != NULL))
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_bcsConfigPtr->bcss->charInfo[CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT].
                                           descrHandle[CY_BLE_BCS_CCCD],
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .connHandle                 = connHandle,
            .offset                     = 0u,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
        };
        /* Read the characteristic value from the database */
        if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_SUCCESS;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BCSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Reads a a characteristic descriptor of the specified characteristic of the
*  Body Composition Service from the GATT database.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the characteristic.
*  \param descrIndex: The index of the descriptor.
*  \param attrSize: The size of the descriptor value.
*  \param attrValue: The pointer to the location where characteristic descriptor value
*              data should be stored.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully.
* CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The optional descriptor is absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BCSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bcs_char_index_t charIndex,
                                                               cy_en_ble_bcs_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex == CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT) && (descrIndex == CY_BLE_BCS_CCCD) && (attrValue != NULL))
    {
        if(cy_ble_bcsConfigPtr->bcss->charInfo[CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT].descrHandle[CY_BLE_BCS_CCCD] !=
           CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_bcsConfigPtr->bcss->
                                               charInfo[CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT].
                                               descrHandle[CY_BLE_BCS_CCCD],
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .connHandle                 = connHandle,
                .offset                     = 0u,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };

            /* Get a characteristic value from the GATT database */
            if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
            {
                /* Indicate success */
                apiResult = CY_BLE_SUCCESS;
            }
            else
            {
                apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
            }
        }
        else
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BCSS_SendIndication
***************************************************************************//**
*
*  Sends indication with a characteristic value of the Body Composition Service,
*  which is a value specified by charIndex to the Client device.
*
*  On enabling indication successfully it sends out a Handle Value Indication which
*  results in CY_BLE_EVT_BCSC_INDICATION or CY_BLE_EVT_GATTC_HANDLE_VALUE_IND (if
*  service-specific callback function is not registered) event at the GATT Client's end.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*              sent to the Client's device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the Client is not established.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_IND_DISABLED - Indication is not enabled by the Client.
*
* \events
*  If execution is successful (return value = CY_BLE_SUCCESS),
*  these events can appear: \n
*   If a BCS service-specific callback is registered
*      (with Cy_BLE_BCS_RegisterAttrCallback):
*  * CY_BLE_EVT_BCSS_INDICATION_CONFIRMED - If the indication is
*                                successfully delivered to the peer device.
*  .
*   Otherwise (if a BCS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF - If the indication is
*                                successfully delivered to the peer device.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BCSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_bcs_char_index_t charIndex,
                                                  uint8_t attrSize,
                                                  uint8_t *attrValue)
{
    /* Store new data in database */
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if(charIndex == CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT)
    {
        /* Send indication if it is enabled and connected */
        if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
        {
            apiResult = CY_BLE_ERROR_INVALID_STATE;
        }
        else if(!CY_BLE_IS_INDICATION_ENABLED(connHandle.attId,
                                              cy_ble_bcsConfigPtr->bcss->charInfo[CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT].
                                               descrHandle[CY_BLE_BCS_CCCD]))
        {
            apiResult = CY_BLE_ERROR_IND_DISABLED;
        }
        else
        {
            /* Fill all the fields of the Write request structure ... */
            cy_stc_ble_gatts_handle_value_ind_t indReqParam =
            {
                .handleValPair.attrHandle = cy_ble_bcsConfigPtr->bcss->
                                             charInfo[CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT].charHandle,
                .handleValPair.value.val  = attrValue,
                .handleValPair.value.len  = attrSize,
                .connHandle               = connHandle
            };
            /* Send indication to the Client using a previously filled structure */
            apiResult = Cy_BLE_GATTS_Indication(&indReqParam);

            /* Save a handle to support service-specific value confirmation response from the Client */
            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_bcssReqHandle = indReqParam.handleValPair.attrHandle;
            }
        }
    }

    return(apiResult);
}

#endif /* CY_BLE_BCS_SERVER */


#ifdef CY_BLE_BCS_CLIENT

/******************************************************************************
* Function Name: Cy_BLE_BCSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_BCSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* BCS characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_bcscCharUuid[CY_BLE_BCS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_BODY_COMPOSITION_FEATURE,
        CY_BLE_UUID_CHAR_BODY_COMPOSITION_MEASUREMENT
    };

    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_BCS))
    {
        /* Update the last characteristic endHandle to the declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        /* Search through all available characteristics */
        for(i = (uint32_t)CY_BLE_BCS_BODY_COMPOSITION_FEATURE; i < (uint32_t)CY_BLE_BCS_CHAR_COUNT; i++)
        {
            if(cy_ble_bcscCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_bcsc[discIdx].charInfo[i].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_bcsc[discIdx].charInfo[i].valueHandle = discCharInfo->valueHandle;
                    cy_ble_bcsc[discIdx].charInfo[i].properties = discCharInfo->properties;
                    lastEndHandle[discIdx] = &cy_ble_bcsc[discIdx].charInfo[i].endHandle;
                    break;
                }
                else
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo);
                }
            }
        }

        /* Initialize the characteristic endHandle to the service endHandle. The characteristic endHandle
         * will be updated to the declaration handle of the following characteristic
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


/******************************************************************************s
* Function Name: Cy_BLE_BCSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_FIND_INFO_RSP event.
*  Based on the descriptor UUID, the appropriate data structure is populated using
*  the data received as part of the callback.
*
*  \param discDescrInfo: The pointer to a descriptor information structure.
*
******************************************************************************/
static void Cy_BLE_BCSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_BCS)
    {
        if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_CLIENT_CONFIG)
        {
            if(cy_ble_bcsc[discIdx].bodyCompositionMeasurementCccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_bcsc[discIdx].bodyCompositionMeasurementCccdHandle = discDescrInfo->descrHandle;
            }
            else    /* Duplication of the descriptor */
            {
                Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_DESCR_DUPLICATION, &discDescrInfo);
            }
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}

/******************************************************************************
* Function Name: Cy_BLE_BCSC_GetCharRange
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
static void Cy_BLE_BCSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_BCS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        if(cy_ble_discovery[discIdx].charCount == 0u)
        {
            charRangeInfo->range.startHandle = cy_ble_bcsc[discIdx].charInfo[CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT].
                                                valueHandle + 1u;
            charRangeInfo->range.endHandle = cy_ble_bcsc[discIdx].charInfo[CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT].
                                              endHandle;
        }
        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}

/******************************************************************************
* Function Name: Cy_BLE_BCSC_IndicationEventHandler
***************************************************************************//**
*
*  Handles an Indication event for the Body Composition Service.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_BCSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_BCS_ApplCallback != NULL))
    {
        if(cy_ble_bcsc[discIdx].charInfo[CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT].valueHandle ==
           eventParam->handleValPair.attrHandle)
        {
            cy_stc_ble_bcs_char_value_t indicationValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT,
                .value      = &eventParam->handleValPair.value
            };
            Cy_BLE_BCS_ApplCallback((uint32_t)CY_BLE_EVT_BCSC_INDICATION, &indicationValue);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BCSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles a Read Response event for the Body Composition Service.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_BCSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t locReqHandle = 1u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_BCS_ApplCallback != NULL) &&
       (cy_ble_bcscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if((cy_ble_bcscReqHandle[discIdx] ==
            cy_ble_bcsc[discIdx].charInfo[CY_BLE_BCS_BODY_COMPOSITION_FEATURE].valueHandle) ||
           (cy_ble_bcscReqHandle[discIdx] ==
            cy_ble_bcsc[discIdx].charInfo[CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT].valueHandle))
        {
            cy_stc_ble_bcs_char_value_t locCharValue = { .connHandle = eventParam->connHandle };
            if(cy_ble_bcscReqHandle[discIdx] == cy_ble_bcsc[discIdx].charInfo[CY_BLE_BCS_BODY_COMPOSITION_FEATURE].
                valueHandle)
            {
                locCharValue.charIndex = CY_BLE_BCS_BODY_COMPOSITION_FEATURE;
            }
            else
            {
                locCharValue.charIndex = CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT;
            }

            locCharValue.value = &eventParam->value;
            cy_ble_bcscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_BCS_ApplCallback((uint32_t)CY_BLE_EVT_BCSC_READ_CHAR_RESPONSE, &locCharValue);
        }
        else if(cy_ble_bcscReqHandle[discIdx] == cy_ble_bcsc[discIdx].bodyCompositionMeasurementCccdHandle)
        {
            cy_stc_ble_bcs_descr_value_t locDescrValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT,
                .descrIndex = CY_BLE_BCS_CCCD,
                .value      = &eventParam->value
            };
            cy_ble_bcscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_BCS_ApplCallback((uint32_t)CY_BLE_EVT_BCSC_READ_DESCR_RESPONSE, &locDescrValue);
        }
        else
        {
            locReqHandle = 0u;
        }

        if(locReqHandle != 0u)
        {
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BCSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Write Response event for the Body Composition Service.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_BCSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    /* Check if the service handler was registered and the request handle is valid */
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_BCS_ApplCallback != NULL) &&
       (cy_ble_bcscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_bcscReqHandle[discIdx] == cy_ble_bcsc[discIdx].bodyCompositionMeasurementCccdHandle)
        {
            cy_stc_ble_bcs_descr_value_t locDescrValue =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT,
                .descrIndex = CY_BLE_BCS_CCCD,
                .value      = NULL
            };
            cy_ble_bcscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_BCS_ApplCallback((uint32_t)CY_BLE_EVT_BCSC_WRITE_DESCR_RESPONSE, &locDescrValue);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BCSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles an Error Response event for the Body Composition Service.
*
*  \param eventParam - The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_BCSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        if(cy_ble_bcscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] == eventParam->errInfo.attrHandle)
        {
            cy_ble_bcscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BCSC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read a characteristic value, which is a value
*  identified by charIndex, from the Server.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic. Starts with a zero.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The read request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                              the particular characteristic.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established.
*  * CY_BLE_ERROR_INVALID_OPERATION - The operation is invalid for this
*                                    characteristic.
*
* \events
*  If execution is successful (return value = CY_BLE_SUCCESS),
*  these events can appear: \n
*   If a BCS service-specific callback is registered
*      (with Cy_BLE_BCS_RegisterAttrCallback):
*  * CY_BLE_EVT_BCSC_READ_CHAR_RESPONSE - If the requested attribute is
*                                successfully read on the peer device,         !
*                                the details (char index , value, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_bcs_char_value_t.
*  .
*   Otherwise (if a BCS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP - If the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with the event parameters
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with the event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BCSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_bcs_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_BCS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_bcsc[discIdx].charInfo[charIndex].valueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_bcsc[discIdx].charInfo[charIndex].valueHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);

        /* Save the handle to support a service-specific Read response from the device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_bcscReqHandle[discIdx] = cy_ble_bcsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BCSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  This function is used to write the characteristic descriptor to the Server
*  identified by charIndex and descrIndex.
*
*  Internally, a Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server's side the following events can be
*  generated:
*  * CY_BLE_EVT_BCSS_INDICATION_ENABLED
*  * CY_BLE_EVT_BCSS_INDICATION_DISABLED
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic descriptor value data that
*               should be sent to the server device.
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
*  If execution is successful (return value = CY_BLE_SUCCESS),
*  these events can appear: \n
*   If a BCS service-specific callback is registered
*      (with Cy_BLE_BCS_RegisterAttrCallback):
*  * CY_BLE_EVT_BCSC_WRITE_DESCR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_bcs_descr_value_t.
*  .
*   Otherwise (if a BCS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - If the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with the event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BCSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bcs_char_index_t charIndex,
                                                               cy_en_ble_bcs_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_BCS_CHAR_COUNT) || (descrIndex >= CY_BLE_BCS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_bcsc[discIdx].bodyCompositionMeasurementCccdHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        /* Fill all the fields of the Write request structure ... */
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_bcsc[discIdx].bodyCompositionMeasurementCccdHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* ... and send a request to the Server device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

        /* Save the handle to support a service-specific Read response from the device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_bcscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BCSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Sends a request to get the characteristic descriptor of the specified
*  characteristic of the service.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic. Starts with a zero.
*  \param descrIndex: The index of the service characteristic descriptor.
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
*  If execution is successful (return value = CY_BLE_SUCCESS),
*  these events can appear: \n
*  If a BCS service-specific callback is registered
*      (with Cy_BLE_BCS_RegisterAttrCallback):
*  * CY_BLE_EVT_BCSC_READ_DESCR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with an event parameter structure
*                                of type cy_stc_ble_bcs_descr_value_t.
*  .
*  Otherwise (if a BCS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP - If the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with the event parameters
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with the event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BCSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bcs_char_index_t charIndex,
                                                               cy_en_ble_bcs_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_BCS_CHAR_COUNT) || (descrIndex >= CY_BLE_BCS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_bcsc[discIdx].bodyCompositionMeasurementCccdHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_bcsc[discIdx].bodyCompositionMeasurementCccdHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);

        /* Save the handle to support a service-specific Read response from the device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_bcscReqHandle[discIdx] = cy_ble_bcsc[discIdx].bodyCompositionMeasurementCccdHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }

    return(apiResult);
}

#endif /* CY_BLE_BCS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_BCS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Body Composition Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_BCS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_BCS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_BCSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_BCSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_BCSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_BCS_CLIENT */

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
        #ifdef CY_BLE_BCS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_BCSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF:
                Cy_BLE_BCSS_ConfirmationEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_BCS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_BCS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_BCSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_IND:
                Cy_BLE_BCSC_IndicationEventHandler((cy_stc_ble_gattc_handle_value_ind_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_BCSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_BCSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_BCS_CLIENT */
            default:
                break;
        }
    }

    return(gattErr);
}
#endif /* defined(CY_BLE_BCS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
