/***************************************************************************//**
* \file cy_ble_bms.c
* \version 2.0
*
* \brief
*  This file contains the source code for the Bond Management Service
*  of the BLE Component.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_BMS))

static cy_ble_callback_t Cy_BLE_BMS_ApplCallback;

#ifdef CY_BLE_BMS_CLIENT
/* BMS Center Service characteristics GATT DB handles structure */
cy_stc_ble_bmsc_t cy_ble_bmsc[CY_BLE_CONFIG_GATTC_COUNT];

/* Internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_bmscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_BMS_CLIENT */

/* The pointer to the global BLE BMS config structure */
cy_stc_ble_bms_config_t *cy_ble_bmsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_BMS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_BMSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
static void Cy_BLE_BMSS_PrepareWriteRequestEventHandler(cy_stc_ble_gatts_prep_write_req_param_t *eventParam);
static void Cy_BLE_BMSS_ExecuteWriteRequestEventHandler(cy_stc_ble_gatts_exec_write_req_t *eventParam);
#endif /* CY_BLE_BMS_SERVER */

#ifdef CY_BLE_BMS_CLIENT
static void Cy_BLE_BMSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_BMSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_BMSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_BMSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_BMSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_BMSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
static void Cy_BLE_BMSC_ExecuteWriteResponseEventHandler(const cy_stc_ble_gattc_exec_write_rsp_t *eventParam);
#endif /* CY_BLE_BMS_CLIENT */
static cy_en_ble_gatt_err_code_t Cy_BLE_BMS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_BMS_Init
***************************************************************************//**
*
*  Initializes the profile internals.
*
*  \param config: The configuration structure for the Bond Management Service.
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
cy_en_ble_api_result_t Cy_BLE_BMS_Init(cy_stc_ble_bms_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to the config structure */
        cy_ble_bmsConfigPtr = config;

        /* Registers Event Handler for the BMS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_BMS_EventHandler);

        /* Registers a callback function via the config structure */
        if(cy_ble_bmsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_BMS_ApplCallback = cy_ble_bmsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_BMS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_BMS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_bmsc[idx], 0, sizeof(cy_stc_ble_bmsc_t));

                    /* Initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_BMS].uuid = CY_BLE_UUID_BOND_MANAGEMENT_SERVICE;
                }
                cy_ble_bmscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_GATT_ROLE_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BMS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific Write requests from the peer device will not be handled with
*  an unregistered callback function.
*
*  \param callbackFunc:  An application layer event callback function to receive
*                    events from the BLE Component. The definition of
*                    cy_ble_callback_t for BM Service is:\n
*                    typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                       void *eventParam)
*                    * eventCode indicates the event that triggered this
*                      callback.
*                    * eventParam contains the parameters corresponding to the
*                      current event.
*
* \sideeffect The *eventParams in the callback function should not be used
*                by the application once the callback function execution is
*                finished. Otherwise, this data may become corrupted.
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
cy_en_ble_api_result_t Cy_BLE_BMS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_BMS_ApplCallback = callbackFunc;
    if(cy_ble_bmsConfigPtr != NULL)
    {
        cy_ble_bmsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}

#ifdef CY_BLE_BMS_SERVER


/******************************************************************************
* Function Name: Cy_BLE_BMSS_SetCharacteristicValue
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
cy_en_ble_api_result_t Cy_BLE_BMSS_SetCharacteristicValue(cy_en_ble_bms_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check the parameters */
    if(charIndex >= CY_BLE_BMS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_bmsConfigPtr->bmss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Store a characteristic value into the GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_bmsConfigPtr->bmss->charInfo[charIndex].charHandle,
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
* Function Name: Cy_BLE_BMSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets a characteristic value of the service identified by charIndex.
*
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the location where Characteristic value data should
*               be stored.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BMSS_GetCharacteristicValue(cy_en_ble_bms_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check the parameters */
    if(charIndex >= CY_BLE_BMS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_bmsConfigPtr->bmss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get the characteristic value from the GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_bmsConfigPtr->bmss->charInfo[charIndex].charHandle,
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
* Function Name: Cy_BLE_BMSS_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sets a characteristic descriptor of the specified characteristic of the service.
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_bms_char_index_t.
*  \param descrIndex:      The index of a service characteristic descriptor of type
*                   cy_en_ble_bms_descr_index_t.
*  \param attrSize:        The size of the characteristic descriptor attribute.
*  \param attrValue:       The pointer to the descriptor value data that should
*                   be stored to the GATT database.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request is handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BMSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bms_char_index_t charIndex,
                                                               cy_en_ble_bms_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if((charIndex >= CY_BLE_BMS_CHAR_COUNT) || (descrIndex >= CY_BLE_BMS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_bmsConfigPtr->bmss->charInfo[charIndex].descrHandle[descrIndex],
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
* Function Name: Cy_BLE_BMSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets a characteristic descriptor of a specified characteristic of the service.
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_bms_char_index_t.
*  \param descrIndex:      The index of a service characteristic descriptor of type
*                   cy_en_ble_bms_descr_index_t.
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
cy_en_ble_api_result_t Cy_BLE_BMSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bms_char_index_t charIndex,
                                                               cy_en_ble_bms_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if((charIndex >= CY_BLE_BMS_CHAR_COUNT) || (descrIndex >= CY_BLE_BMS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_bmsConfigPtr->bmss->charInfo[charIndex].descrHandle[descrIndex],
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
            .offset                     = 0u,
            .connHandle                 = connHandle,
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
* Function Name: Cy_BLE_BMSS_WriteEventHandler
***************************************************************************//**
*
*  Handles a Write Request event. Calls the registered BMS application
*  callback function.
*
* Note: Writing an attribute into GATT DB (if needed) is the user's responsibility
*  after the Authorization Code check in the registered BMS application
*  callback function.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*  cy_en_ble_gatt_err_code_t - A function result state if it succeeded
*  (CY_BLE_GATT_ERR_NONE) or failed with error codes:
*   * CY_BLE_GATT_ERR_OP_CODE_NOT_SUPPORTED
*   * CY_BLE_GATT_ERR_INSUFFICIENT_AUTHORIZATION
*
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_BMSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_stc_ble_bms_char_value_t locCharValue =
    {
        .connHandle    = eventParam->connHandle,
        .gattErrorCode = CY_BLE_GATT_ERR_NONE
    };

    if((Cy_BLE_BMS_ApplCallback != NULL) &&
       (eventParam->handleValPair.attrHandle == cy_ble_bmsConfigPtr->bmss->charInfo[CY_BLE_BMS_BMCP].charHandle))
    {
        locCharValue.gattErrorCode =
            Cy_BLE_GATT_DbCheckPermission(eventParam->handleValPair.attrHandle, &eventParam->connHandle,
                                          CY_BLE_GATT_DB_WRITE | CY_BLE_GATT_DB_PEER_INITIATED);

        if(locCharValue.gattErrorCode == CY_BLE_GATT_ERR_NONE)
        {
            locCharValue.charIndex = CY_BLE_BMS_BMCP;
            locCharValue.value = &eventParam->handleValPair.value;

            Cy_BLE_BMS_ApplCallback((uint32_t)CY_BLE_EVT_BMSS_WRITE_CHAR, &locCharValue);

            if(locCharValue.gattErrorCode == CY_BLE_GATT_ERR_NONE)
            {
                cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                {
                    .handleValuePair = eventParam->handleValPair,
                    .connHandle      = eventParam->connHandle,
                    .offset          = 0u,
                    .flags           = CY_BLE_GATT_DB_PEER_INITIATED
                };
                locCharValue.gattErrorCode = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);
                if(locCharValue.gattErrorCode == CY_BLE_GATT_ERR_NONE)
                {
                    CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(cy_ble_bmsConfigPtr->bmss->charInfo[CY_BLE_BMS_BMCP].charHandle,
                                                         locCharValue.value->len);
                }
            }
        }

        /* Indicate that the request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }

    return(locCharValue.gattErrorCode);
}

/******************************************************************************
* Function Name: Cy_BLE_BMSS_PrepareWriteEventHandler
***************************************************************************//**
*
*  Handles a Prepare Write Request event.
*
*  \param cy_stc_ble_gatts_prep_write_req_param_t *eventParam: The pointer to the data that
*                                                  came with a Prepare Write
*                                                  request.
*
******************************************************************************/
static void Cy_BLE_BMSS_PrepareWriteRequestEventHandler(cy_stc_ble_gatts_prep_write_req_param_t *eventParam)
{
    if(eventParam->baseAddr[eventParam->currentPrepWriteReqCount - 1u].handleValuePair.attrHandle ==
       cy_ble_bmsConfigPtr->bmss->charInfo[CY_BLE_BMS_BMCP].charHandle)
    {
        cy_stc_ble_gatt_xchg_mtu_param_t mtuParam = { .connHandle = eventParam->connHandle };
        (void)Cy_BLE_GATT_GetMtuSize(&mtuParam);


        if(((cy_ble_bmsConfigPtr->bmss->charInfo[CY_BLE_BMS_BMCP].descrHandle[CY_BLE_BMS_CEPD] !=
             CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE) &&
            CY_BLE_IS_RELIABLE_WRITE_ENABLED(cy_ble_bmsConfigPtr->bmss->
                                              charInfo[CY_BLE_BMS_BMCP].descrHandle[CY_BLE_BMS_CEPD])) ||
           (CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN(cy_ble_bmsConfigPtr->bmss->charInfo[CY_BLE_BMS_BMCP].charHandle) >
            (mtuParam.mtu - CY_BLE_GATT_WRITE_HEADER_LEN)))
        {
            /* The first prepare write event */
        }
        else
        {
            eventParam->gattErrorCode = (uint8_t)CY_BLE_GATT_ERR_ATTRIBUTE_NOT_LONG;
        }


        /* Indicate that the request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}

/******************************************************************************
* Function Name: Cy_BLE_BMSS_ExecuteWriteRequestEventHandler
***************************************************************************//**
*
*  Handles an Execute Write Request event.
*
*  \param cy_stc_ble_gatts_exec_write_req_t *eventParam: The pointer to the data that came
*                                            with a Write request.
*
******************************************************************************/
static void Cy_BLE_BMSS_ExecuteWriteRequestEventHandler(cy_stc_ble_gatts_exec_write_req_t *eventParam)
{
    uint32_t locCount;
    cy_stc_ble_gatt_value_t locGattValue = { .len = 0u, .val = NULL };

    for(locCount = 0u; locCount < eventParam->prepWriteReqCount; locCount++)
    {
        if(eventParam->baseAddr[locCount].handleValuePair.attrHandle ==
           cy_ble_bmsConfigPtr->bmss->charInfo[CY_BLE_BMS_BMCP].charHandle)
        {
            locGattValue.len = eventParam->baseAddr[locCount].offset + eventParam->baseAddr[locCount].
                                handleValuePair.value.len;

            if(locGattValue.val == NULL)
            {
                locGattValue.val = eventParam->baseAddr[locCount].handleValuePair.value.val;
            }
            else if(eventParam->baseAddr[locCount].offset == 0u)
            {
                /* The case when the Client wants to rewrite the value from the start */
                locGattValue.val = eventParam->baseAddr[locCount].handleValuePair.value.val;
            }
            else
            {
                /* Do nothing */
            }
        }
    }

    if((Cy_BLE_BMS_ApplCallback != NULL) && (locGattValue.len != 0u) &&
       (locGattValue.len <= CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN(cy_ble_bmsConfigPtr->bmss->charInfo[CY_BLE_BMS_BMCP].
                                                                      charHandle)))
    {
        /* Check the execWriteFlag before executing or cancelling the Write Long operation */
        if(eventParam->execWriteFlag == CY_BLE_GATT_EXECUTE_WRITE_EXEC_FLAG)
        {
            cy_stc_ble_bms_char_value_t locCharValue = { .connHandle = eventParam->connHandle };
            locCharValue.gattErrorCode = CY_BLE_GATT_ERR_NONE;
            locCharValue.value = &locGattValue;

            Cy_BLE_BMS_ApplCallback((uint32_t)CY_BLE_EVT_BMSS_WRITE_CHAR, &locCharValue);

            if(locCharValue.gattErrorCode == CY_BLE_GATT_ERR_NONE)
            {
                CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(cy_ble_bmsConfigPtr->bmss->
                                                      charInfo[CY_BLE_BMS_BMCP].charHandle, locGattValue.len);
            }

            eventParam->gattErrorCode = (uint8_t)locCharValue.gattErrorCode;
        }

        /* Indicate that the request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}

#endif /* CY_BLE_BMS_SERVER */

#ifdef CY_BLE_BMS_CLIENT


/******************************************************************************
* Function Name: Cy_BLE_BMSC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read the characteristic value from a Server
*  identified by charIndex.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the Service characteristic.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The read request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the Server is not established.
*  * CY_BLE_ERROR_INVALID_OPERATION - The operation is invalid for this
*                                     characteristic.
*
* \events
*  If execution is successful(return value = CY_BLE_SUCCESS),
*  these events can appear: \n
*   If a BMS service-specific callback is registered
*      (with Cy_BLE_BMS_RegisterAttrCallback):
*  * CY_BLE_EVT_BMSC_READ_CHAR_RESPONSE - if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_bms_char_value_t.
*  .
*   Otherwise (if a BMS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_BMSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_bms_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check the parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex != CY_BLE_BMS_BMFT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_bmsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_READ & cy_ble_bmsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMFT].valueHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_bmscReqHandle[discIdx] = cy_ble_bmsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BMSC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write a characteristic (identified by
*  charIndex) value attribute to the Server. The function supports the Write Long
*  procedure - it depends on the attrSize parameter - if it is larger than the
*  current MTU size - 1, then the Write Long will be executed.
*  As a result, a Write request is sent to the GATT Server and on successful
*  execution of the request on the Server's side, a CY_BLE_EVT_BMSS_WRITE_CHAR
*  events is generated.
*  On successful request execution on the Server's side, a Write response is
*  sent to the Client.
*
*  The Write response only confirms the operation success.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the Server device.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the Server is not established.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic.
*  * CY_BLE_ERROR_INVALID_OPERATION - The operation is invalid for this
*                                     characteristic.
*
* \events
*  If execution is successful(return value = CY_BLE_SUCCESS),
*  these events can appear: \n
*   If a BMS service-specific callback is registered
*      (with Cy_BLE_BMS_RegisterAttrCallback):
*  * CY_BLE_EVT_BMSC_WRITE_CHAR_RESPONSE - if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_bms_char_value_t.
*  .
*   Otherwise (if a BMS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_EXEC_WRITE_RSP - if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with the event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BMSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_bms_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);
    cy_stc_ble_gatt_xchg_mtu_param_t mtuParam = { .connHandle = connHandle };

    (void)Cy_BLE_GATT_GetMtuSize(&mtuParam);

    /* Check the parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex != CY_BLE_BMS_BMCP) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_WRITE & cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else if((mtuParam.mtu - CY_BLE_GATT_WRITE_HEADER_LEN) < attrSize)
    {
        cy_stc_ble_gattc_prep_write_req_t prepWriteReqParam =
        {
            .handleValOffsetPair.handleValuePair.attrHandle = cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].valueHandle,
            .handleValOffsetPair.handleValuePair.value.val  = attrValue,
            .handleValOffsetPair.handleValuePair.value.len  = attrSize,
            .handleValOffsetPair.offset                     = 0u,
            .connHandle                                     = connHandle
        };
        apiResult = Cy_BLE_GATTC_WriteLongCharacteristicValues(&prepWriteReqParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_bmscReqHandle[discIdx] = cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].valueHandle;
        }
    }
    else
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_bmscReqHandle[discIdx] = cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BMSC_ReliableWriteCharacteristicValue
***************************************************************************//**
*
*  Performs a Reliable Write command for the
*  Bond Management Control Point characteristic (identified by
*  charIndex) value attribute to the Server.
*
*  The Write response only confirms the operation success.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the Server device.
*
* \return
*  The return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the Server is not established.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic.
*  * CY_BLE_ERROR_INVALID_OPERATION - The operation is invalid for this
*                                     characteristic.
* \events
*  If execution is successful (return value = CY_BLE_SUCCESS),
*  these events can appear: \n
*   If a BMS service specific callback is registered
*      (with Cy_BLE_BMS_RegisterAttrCallback):
*  * CY_BLE_EVT_BMSC_WRITE_CHAR_RESPONSE - if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_bms_char_value_t.
*  .
*   Otherwise (if a BMS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_EXEC_WRITE_RSP - if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - if there is trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with the event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BMSC_ReliableWriteCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                                    cy_en_ble_bms_char_index_t charIndex,
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
    else if((charIndex != CY_BLE_BMS_BMCP) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_EXTENDED_PROPERTIES & cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_reliable_write_req_t prepWriteReqParam = { .connHandle = connHandle };
        cy_stc_ble_gatt_handle_value_offset_param_t handleValOffsetPairParam =
        {
            .handleValuePair.attrHandle = cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].valueHandle,
            .handleValuePair.value.val  = attrValue,
            .handleValuePair.value.len  = attrSize,
            .offset                     = 0u
        };

        prepWriteReqParam.handleValOffsetPair = &handleValOffsetPairParam;
        prepWriteReqParam.numOfRequests = 1u;

        apiResult = Cy_BLE_GATTC_ReliableWrites(&prepWriteReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_bmscReqHandle[discIdx] = cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BMSC_GetCharacteristicDescriptor
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
*  If a BMS service-specific callback is registered
*      (with Cy_BLE_BMS_RegisterAttrCallback):
*  * CY_BLE_EVT_BMSC_READ_DESCR_RESPONSE - if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with an event parameter structure
*                                of type cy_stc_ble_bms_descr_value_t.
*  .
*  Otherwise (if a BMS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_BMSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bms_char_index_t charIndex,
                                                               cy_en_ble_bms_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check the parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_BMS_CHAR_COUNT) || (descrIndex >= CY_BLE_BMS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].descrHandle[CY_BLE_BMS_CEPD] ==
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].descrHandle[CY_BLE_BMS_CEPD],
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_bmscReqHandle[discIdx] = cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].descrHandle[CY_BLE_BMS_CEPD];
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BMSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to the characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_BMSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* BM Service characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_bmscCharUuid[CY_BLE_BMS_CHAR_COUNT] =
    {
        CY_BLE_UUID_BOND_MANAGEMENT_CONTROL_POINT,
        CY_BLE_UUID_BOND_MANAGEMENT_FEATURE
    };
    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_BMS))
    {
        /* Update the last characteristic endHandle to the declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        for(i = 0u; i < (uint8_t)CY_BLE_BMS_CHAR_COUNT; i++)
        {
            if(cy_ble_bmscCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_bmsc[discIdx].charInfo[i].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_bmsc[discIdx].charInfo[i].valueHandle = discCharInfo->valueHandle;
                    cy_ble_bmsc[discIdx].charInfo[i].properties = discCharInfo->properties;
                    lastEndHandle[discIdx] = &cy_ble_bmsc[discIdx].charInfo[i].endHandle;
                }
                else
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo);
                }
            }
        }

        /* Initialize the characteristic endHandle to Service endHandle.
         * the characteristic endHandle will be updated to the declaration
         * handle of the following characteristic,
         * in the following characteristic discovery procedure. */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = cy_ble_serverInfo[discIdx][CY_BLE_SRVI_BMS].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}

/******************************************************************************
* Function Name: Cy_BLE_BMSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, the appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_BMSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t notSupportedDescr = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t descIdx;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_BMS)
    {
        switch(discDescrInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_EXTENDED_PROPERTIES:
                descIdx = (uint32_t)CY_BLE_BMS_CEPD;
                break;

            default:
                /* Not supported descriptor */
                notSupportedDescr = 1u;
                break;
        }

        if(notSupportedDescr == 0u)
        {
            if(cy_ble_bmsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] ==
               CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_bmsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] =
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
* Function Name: Cy_BLE_BMSC_GetCharRange
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
static void Cy_BLE_BMSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_BMS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_BMS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if((cy_ble_bmsc[discIdx].charInfo[charIdx].endHandle - cy_ble_bmsc[discIdx].charInfo[charIdx].valueHandle) != 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_bmsc[discIdx].charInfo[charIdx].valueHandle + 1u;
                charRangeInfo->range.endHandle = cy_ble_bmsc[discIdx].charInfo[charIdx].endHandle;
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
* Function Name: Cy_BLE_BMSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles a Read Response event.
*
*  \param eventParam: The pointer to the data structure.
*
******************************************************************************/
static void Cy_BLE_BMSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_BMS_ApplCallback != NULL) &&
       (cy_ble_bmscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMFT].valueHandle == cy_ble_bmscReqHandle[discIdx])
        {
            cy_stc_ble_bms_char_value_t locCharValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_BMS_BMFT,
                .value      = &eventParam->value
            };
            cy_ble_bmscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_BMS_ApplCallback((uint32_t)CY_BLE_EVT_BMSC_READ_CHAR_RESPONSE, &locCharValue);
        }
        else if(cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].descrHandle[CY_BLE_BMS_CEPD] ==
                cy_ble_bmscReqHandle[discIdx])
        {
            cy_stc_ble_bms_descr_value_t locDescrValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_BMS_BMCP,
                .descrIndex = CY_BLE_BMS_CEPD,
                .value      = &eventParam->value,
            };
            cy_ble_bmscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_BMS_ApplCallback((uint32_t)CY_BLE_EVT_BMSC_READ_DESCR_RESPONSE, &locDescrValue);
        }
        else
        {
            /* Do nothing */
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BMSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Write Response event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_BMSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_BMS_ApplCallback != NULL) &&
       (cy_ble_bmscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].valueHandle == cy_ble_bmscReqHandle[discIdx])
        {
            cy_stc_ble_bms_char_value_t locCharValue =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_BMS_BMCP,
                .value      = NULL
            };
            cy_ble_bmscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_BMS_ApplCallback((uint32_t)CY_BLE_EVT_BMSC_WRITE_CHAR_RESPONSE, &locCharValue);
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BMSC_ExecuteWriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Write Response event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_BMSC_ExecuteWriteResponseEventHandler(const cy_stc_ble_gattc_exec_write_rsp_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_BMS_ApplCallback != NULL) &&
       (cy_ble_bmsc[discIdx].charInfo[CY_BLE_BMS_BMCP].valueHandle == cy_ble_bmscReqHandle[discIdx]))
    {
        cy_stc_ble_bms_char_value_t locCharValue =
        {
            .connHandle = eventParam->connHandle,
            .charIndex  = CY_BLE_BMS_BMCP,
            .value      = NULL
        };
        cy_ble_bmscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        Cy_BLE_BMS_ApplCallback((uint32_t)CY_BLE_EVT_BMSC_WRITE_CHAR_RESPONSE, &locCharValue);
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BMSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles an Error Response event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_BMSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        if(cy_ble_bmscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] == eventParam->errInfo.attrHandle)
        {
            cy_ble_bmscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}
#endif /* CY_BLE_BMS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_BMS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Bond Management Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_BMS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_BMS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_BMSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_BMSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_BMSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_BMS_CLIENT */

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
        #ifdef CY_BLE_BMS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_BMSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_PREP_WRITE_REQ:
                Cy_BLE_BMSS_PrepareWriteRequestEventHandler((cy_stc_ble_gatts_prep_write_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_EXEC_WRITE_REQ:
                Cy_BLE_BMSS_ExecuteWriteRequestEventHandler((cy_stc_ble_gatts_exec_write_req_t*)eventParam);
                break;
        #endif /* CY_BLE_BMS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_BMS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_BMSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_BMSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_BMSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_EXEC_WRITE_RSP:
                Cy_BLE_BMSC_ExecuteWriteResponseEventHandler((cy_stc_ble_gattc_exec_write_rsp_t*)eventParam);
                break;
        #endif /* CY_BLE_BMS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}
#endif /* defined(CY_BLE_BMS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
