/***************************************************************************//**
* \file cy_ble_ndcs.c
* \version 2.0
*
* \brief
*  Contains the source code for Next DST Change Service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_NDCS))

#ifdef CY_BLE_NDCS_CLIENT
static cy_ble_callback_t Cy_BLE_NDCS_ApplCallback = NULL;

/* NDCS Center Service characteristics GATT DB handles structure */
cy_stc_ble_ndcsc_t cy_ble_ndcsc[CY_BLE_CONFIG_GATTC_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_ndcscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_NDCS_CLIENT) */

/* The pointer on global BLE NDCS Config structure */
cy_stc_ble_ndcs_config_t *cy_ble_ndcsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#if defined(CY_BLE_NDCS_CLIENT)
static void Cy_BLE_NDCSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_NDCSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_NDCSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
#endif /* (CY_BLE_NDCS_CLIENT) */
static cy_en_ble_gatt_err_code_t Cy_BLE_NDCS_EventHandler(uint32_t eventCode, void *eventParam);


/*******************************************************************************
* Function Name: Cy_BLE_NDCS_Init
****************************************************************************//**
*
*  This function initializes the Next DST Change Service.
*
*  \param config: Configuration structure for the Next DST Change Service.
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
cy_en_ble_api_result_t Cy_BLE_NDCS_Init(cy_stc_ble_ndcs_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_ndcsConfigPtr = config;

        /* Registers Event Handler for the ANCS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_NDCS_EventHandler);

        #ifdef CY_BLE_NDCS_CLIENT
        /* Registers a callback function via config structure */
        if(cy_ble_ndcsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_NDCS_ApplCallback = cy_ble_ndcsConfigPtr->callbackFunc;
        }
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_NDCS].range.startHandle ==
                   CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_ndcsc[idx], 0, sizeof(cy_stc_ble_ndcsc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_NDCS].uuid = CY_BLE_UUID_NEXT_DST_CHANGE_SERVICE;
                }
                cy_ble_ndcscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
        #endif /* CY_BLE_NDCS_CLIENT */
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_NDCS_RegisterAttrCallback
****************************************************************************//**
*
*  Registers a callback function for Next DST Change Service specific attribute
*  operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc:  An application layer event callback function to receive
*                 events from the BLE Component. The definition of
*                 cy_ble_callback_t for NDCS is: \n
*                 typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                   void *eventParam)
*                 * eventCode indicates the event that triggered this
*                   callback.
*                 * eventParam contains the parameters corresponding to the
*                   current event.
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
cy_en_ble_api_result_t Cy_BLE_NDCS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

#ifdef CY_BLE_NDCS_CLIENT
    Cy_BLE_NDCS_ApplCallback = callbackFunc;
    if(cy_ble_ndcsConfigPtr != NULL)
    {
        cy_ble_ndcsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
#endif /* CY_BLE_NDCS_CLIENT */

    return(apiResult);
}

#if defined(CY_BLE_NDCS_SERVER)

/*******************************************************************************
* Function Name: Cy_BLE_NDCSS_SetCharacteristicValue
****************************************************************************//**
*
*  Sets characteristic value of the Next DST Change Service, which is identified
*  by charIndex in the local database.
*
*  \param charIndex: The index of a service characteristic of type cy_en_ble_ndcs_char_index_t.
*  \param attrSize:  The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be stored to the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - the request is handled successfully;
*   * CY_BLE_ERROR_INVALID_PARAMETER - validation of the input parameters failed.
*
*/
cy_en_ble_api_result_t Cy_BLE_NDCSS_SetCharacteristicValue(cy_en_ble_ndcs_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if(charIndex == CY_BLE_NDCS_TIME_WITH_DST)
    {
        /* Store data in database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_ndcsConfigPtr->ndcss->timeWithDst,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .offset                     = 0u,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED,
        };
        if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_SUCCESS;
        }
    }

    return(apiResult);
}


/*******************************************************************************
* Function Name: Cy_BLE_NDCSS_GetCharacteristicValue
****************************************************************************//**
*
*  Gets a characteristic value of the Next DST Change Service, which is
*  identified by charIndex.
*
*  \param charIndex:  The index of a service characteristic of
*                     type cy_en_ble_ndcs_char_index_t.
*  \param attrSize:   The size of the characteristic value attribute.
*
*  \param attrValue: The pointer to the location where
*                     characteristic value data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - the request is handled successfully;
*   * CY_BLE_ERROR_INVALID_PARAMETER - validation of the input parameter failed.
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_NDCSS_GetCharacteristicValue(cy_en_ble_ndcs_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if(charIndex == CY_BLE_NDCS_TIME_WITH_DST)
    {
        /* Read characteristic value from database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_ndcsConfigPtr->ndcss->timeWithDst,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED,
        };
        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
        {
            apiResult = CY_BLE_SUCCESS;
        }
    }

    return(apiResult);
}

#endif /* CY_BLE_NDCS_SERVER */


#if defined(CY_BLE_NDCS_CLIENT)

/*******************************************************************************
* Function Name: Cy_BLE_NDCSC_DiscoverCharacteristicsEventHandler
****************************************************************************//**
*
*  This function is called on receiving the CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
*
*******************************************************************************/
static void Cy_BLE_NDCSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_NDCS))
    {
        if(discCharInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_TIME_WITH_DST)
        {
            /* Using characteristic UUID store handle of requested characteristic */
            if(cy_ble_ndcsc[discIdx].charInfo[CY_BLE_NDCS_TIME_WITH_DST].valueHandle ==
               CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_ndcsc[discIdx].charInfo[CY_BLE_NDCS_TIME_WITH_DST].valueHandle = discCharInfo->valueHandle;
                cy_ble_ndcsc[discIdx].charInfo[CY_BLE_NDCS_TIME_WITH_DST].properties = discCharInfo->properties;
            }
            else
            {
                Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo);
            }
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}



/******************************************************************************
* Function Name: Cy_BLE_NDCSC_GetCharRange
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
static void Cy_BLE_NDCSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_NDCS)
    {
        /* NDCS does not have any discriptions, return CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE */
        charRangeInfo->range.startHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        charRangeInfo->range.endHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_NDCSC_ReadResponseEventHandler
****************************************************************************//**
*
*  Handles Read Response Event for Next DST Change Service.
*
*  \param eventParam: The pointer to the data that came with a read response for NDCS.
*
*******************************************************************************/
static void Cy_BLE_NDCSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_NDCS_ApplCallback != NULL) &&
       (cy_ble_ndcscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_ndcsc[discIdx].charInfo[CY_BLE_NDCS_TIME_WITH_DST].valueHandle == cy_ble_ndcscReqHandle[discIdx])
        {
            /* Fill Reference Time Update Service read response parameter structure with
             * characteristic info.
             */
            cy_stc_ble_ndcs_char_value_t rdRspParam =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_NDCS_TIME_WITH_DST,
                .value      = &eventParam->value
            };

            Cy_BLE_NDCS_ApplCallback((uint32_t)CY_BLE_EVT_NDCSC_READ_CHAR_RESPONSE, (void*)&rdRspParam);

            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            cy_ble_ndcscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


/*******************************************************************************
* Function Name: Cy_BLE_NDCSC_GetCharacteristicValue
****************************************************************************//**
*
*  Sends a request to peer device to set characteristic value of the Next
*  DST Change Service, which is identified by charIndex.
*
*  \param connHandle:    The connection handle.
*  \param charIndex:     The index of a service characteristic.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - the request was sent successfully.
*   * CY_BLE_ERROR_INVALID_STATE - connection with the client is not established.
*   * CY_BLE_ERROR_INVALID_PARAMETER - validation of the input parameters failed.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the NDCS service-specific callback is registered
*      (with Cy_BLE_NDCS_RegisterAttrCallback):
*  * CY_BLE_EVT_NDCSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_ndcs_char_value_t.
*  .
*   Otherwise (if the NDCS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_NDCSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_ndcs_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex == CY_BLE_NDCS_TIME_WITH_DST) && (discIdx < CY_BLE_CONFIG_GATTC_COUNT))
    {
        /* Send request to read characteristic value */
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_ndcsc[discIdx].charInfo[CY_BLE_NDCS_TIME_WITH_DST].valueHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_ndcscReqHandle[discIdx] = cy_ble_ndcsc[discIdx].charInfo[CY_BLE_NDCS_TIME_WITH_DST].valueHandle;
        }
    }
    else
    {
        /* apiResult equals to CY_BLE_ERROR_INVALID_PARAMETER */
    }

    return(apiResult);
}

#endif /* CY_BLE_NDCS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_NDCS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Next DST Change Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_NDCS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_NDCS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_NDCSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_NDCSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_NDCS_CLIENT */

            default:
                break;
        }
    }
    else
    {
        switch((cy_en_ble_event_t)eventCode)
        {
            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_NDCS_CLIENT
            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_NDCSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;
        #endif /* CY_BLE_NDCS_CLIENT */
            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_NDCS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
