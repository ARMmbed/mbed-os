/***************************************************************************//**
* \file CY_BLE_ips.c
* \version 2.0
*
* \brief
*  Contains the source code for Indoor Positioning Service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_IPS))

#ifdef CY_BLE_IPS_CLIENT
/* Server's Indoor Positioning Service characteristics GATT DB handles structure */
cy_stc_ble_ipsc_t cy_ble_ipsc[CY_BLE_CONFIG_GATTC_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_ipscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];

/* Read Long Descriptors variables */
static uint8_t *cy_ble_ipscRdLongBuffPtr[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
static uint16_t cy_ble_ipscRdLongBuffLen[CY_BLE_CONFIG_GATTC_COUNT] = { 0u };
static uint16_t cy_ble_ipscCurrLen[CY_BLE_CONFIG_GATTC_COUNT] = { 0u };
#endif /* (CY_BLE_IPS_CLIENT) */

static cy_ble_callback_t Cy_BLE_IPS_ApplCallback = NULL;

/* The pointer on global BLE IPS Config structure */
cy_stc_ble_ips_config_t *cy_ble_ipsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_IPS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_IPSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
static void Cy_BLE_IPSS_PrepareWriteRequestEventHandler(const cy_stc_ble_gatts_prep_write_req_param_t *eventParam);
static void Cy_BLE_IPSS_ExecuteWriteRequestEventHandler(cy_stc_ble_gatts_exec_write_req_t *eventParam);
static void Cy_BLE_IPSS_WriteCmdEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
static cy_en_ble_api_result_t Cy_BLE_IPSS_SetAdvertisementData(void);
#endif /* CY_BLE_IPS_SERVER */

#ifdef CY_BLE_IPS_CLIENT
static void Cy_BLE_IPSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_IPSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_IPSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_IPSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_IPSC_ReadMultipleResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_IPSC_ReadLongRespEventHandler(const cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_IPSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_IPSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
static void Cy_BLE_IPSC_ExecuteWriteResponseEventHandler(const cy_stc_ble_gattc_exec_write_rsp_t *eventParam);
#endif /* CY_BLE_IPS_CLIENT */
static cy_en_ble_gatt_err_code_t Cy_BLE_IPS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_IPS_Init
***************************************************************************//**
*
*  This function initializes the Indoor Positioning Service.
*  Initializes the profile internals.
*
*  \param config: Configuration structure for the Indoor Positioning Service.
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
cy_en_ble_api_result_t Cy_BLE_IPS_Init(cy_stc_ble_ips_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_ipsConfigPtr = config;

        /* Registers Event Handler for the IPS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_IPS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_ipsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_IPS_ApplCallback = cy_ble_ipsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_IPS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                cy_ble_ipscRdLongBuffPtr[idx] = NULL;
                cy_ble_ipscRdLongBuffLen[idx] = 0u;
                cy_ble_ipscCurrLen[idx] = 0u;

                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_IPS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_ipsc[idx], 0, sizeof(cy_stc_ble_ipsc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_IPS].uuid = CY_BLE_UUID_IPS_SERVICE;
                }
                cy_ble_ipscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_IPS_CLIENT */
    }

    return(apiResult);
}

/******************************************************************************
* Function Name: Cy_BLE_IPS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc:  An application layer event callback function to receive
*                 events from the BLE Component. The definition of
*                 cy_ble_callback_t for IPS Service is:\n
*                 typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                    void *eventParam)
*                 * eventCode:  Indicates the event that triggered this callback
*                   (e.g. CY_BLE_EVT_IPS_NOTIFICATION_ENABLED).
*                 * eventParam: Contains the parameters corresponding to the
*                   current event. (e.g. Pointer to cy_stc_ble_ips_char_value_t
*                   structure that contains details of the characteristic
*                   for which the notification enabled event was triggered).
*
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
cy_en_ble_api_result_t Cy_BLE_IPS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_IPS_ApplCallback = callbackFunc;
    if(cy_ble_ipsConfigPtr != NULL)
    {
        cy_ble_ipsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_IPS_SERVER

/******************************************************************************
* Function Name: Cy_BLE_IPSS_WriteEventHandler
***************************************************************************//**
*
*  Handles the Write Request Event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
*  \return
*  cy_en_ble_gatt_err_code_t - A function result state if it succeeded
*  (CY_BLE_GATT_ERR_NONE) or failed with error codes:
*   * CY_BLE_GATTS_ERR_PROCEDURE_ALREADY_IN_PROGRESS
*   * CY_BLE_GATTS_ERR_CCCD_IMPROPERLY_CONFIGURED
* 
*  NOTE: when advertisement is active this API uses Cy_BLE_GAPP_UpdateAdvScanData() to 
*        update the advertisement packet. In the case of successful execution (return value = CY_BLE_SUCCESS)
*        the event CY_BLE_EVT_GAPP_UPDATE_ADV_SCAN_DATA_COMPLETE appears.
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_IPSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;

    if(Cy_BLE_IPS_ApplCallback != NULL)
    {
        cy_stc_ble_ips_char_value_t locCharValue =
        {
            .connHandle    = eventParam->connHandle,
            .gattErrorCode = CY_BLE_GATT_ERR_NONE
        };

        for(locCharValue.charIndex = CY_BLE_IPS_INDOOR_POSITINING_CONFIG; locCharValue.charIndex < CY_BLE_IPS_CHAR_COUNT;
            locCharValue.charIndex++)
        {
            if((eventParam->handleValPair.attrHandle ==
                cy_ble_ipsConfigPtr->ipss->charInfo[locCharValue.charIndex].charHandle) ||
               (eventParam->handleValPair.attrHandle ==
                cy_ble_ipsConfigPtr->ipss->charInfo[locCharValue.charIndex].descrHandle[CY_BLE_IPS_SCCD]) ||
               (eventParam->handleValPair.attrHandle ==
                cy_ble_ipsConfigPtr->ipss->charInfo[locCharValue.charIndex].descrHandle[CY_BLE_IPS_CEPD]))
            {
                gattErr = Cy_BLE_GATT_DbCheckPermission(eventParam->handleValPair.attrHandle, &eventParam->connHandle,
                                                        CY_BLE_GATT_DB_WRITE | CY_BLE_GATT_DB_PEER_INITIATED);

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
                        Cy_BLE_IPS_ApplCallback((uint32_t)CY_BLE_EVT_IPSS_WRITE_CHAR, &locCharValue);

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

                            if((gattErr == CY_BLE_GATT_ERR_NONE) &&
                               (eventParam->handleValPair.attrHandle == cy_ble_ipsConfigPtr->
                                 ipss->charInfo[locCharValue.charIndex].charHandle))
                            {
                                CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN
                                    (cy_ble_ipsConfigPtr->ipss->charInfo[locCharValue.charIndex].charHandle,
                                    locCharValue.value->len);

                                (void)Cy_BLE_IPSS_SetAdvertisementData();
                            }
                        }
                    }
                }
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;
            }
        }
    }


    return(gattErr);
}

/******************************************************************************
* Function Name: Cy_BLE_IPSS_WriteCmdEventHandler
***************************************************************************//**
*
*  Handles the Write Without Response Request Event.
*
*  \param eventParam:  The pointer to the data structure specified by the event.
*
*  NOTE: when advertisement is active this API uses Cy_BLE_GAPP_UpdateAdvScanData() to 
*        update the advertisement packet. In the case of successful execution (return value = CY_BLE_SUCCESS)
*        the event CY_BLE_EVT_GAPP_UPDATE_ADV_SCAN_DATA_COMPLETE appears.
*
******************************************************************************/
static void Cy_BLE_IPSS_WriteCmdEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_stc_ble_ips_char_value_t locCharValue =
    {
        .connHandle = eventParam->connHandle,
        .value      = &eventParam->handleValPair.value
    };
    cy_en_ble_gatt_err_code_t gattErr;

    for(locCharValue.charIndex = CY_BLE_IPS_INDOOR_POSITINING_CONFIG; locCharValue.charIndex < CY_BLE_IPS_CHAR_COUNT;
        locCharValue.charIndex++)
    {
        if((eventParam->handleValPair.attrHandle ==
            cy_ble_ipsConfigPtr->ipss->charInfo[locCharValue.charIndex].charHandle) &&
           (Cy_BLE_IPS_ApplCallback != NULL))
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
                Cy_BLE_IPS_ApplCallback((uint32_t)CY_BLE_EVT_IPSS_WRITE_CHAR, &locCharValue);
                (void)Cy_BLE_IPSS_SetAdvertisementData();
            }
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}

/******************************************************************************
* Function Name: Cy_BLE_IPSS_PrepareWriteRequestEventHandler
***************************************************************************//**
*
*  Handles the Prepare Write Request Event.
*
*  \param eventParam: The pointer to the data that comes with a prepare
*                      write request.
*
******************************************************************************/
static void Cy_BLE_IPSS_PrepareWriteRequestEventHandler(const cy_stc_ble_gatts_prep_write_req_param_t *eventParam)
{
    cy_en_ble_ips_char_index_t locCharIndex;

    for(locCharIndex = CY_BLE_IPS_INDOOR_POSITINING_CONFIG; locCharIndex < CY_BLE_IPS_CHAR_COUNT; locCharIndex++)
    {
        if(eventParam->baseAddr[eventParam->currentPrepWriteReqCount - 1u].handleValuePair.attrHandle ==
           cy_ble_ipsConfigPtr->ipss->charInfo[locCharIndex].charHandle)
        {
            /* Indicate that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            break;
        }
    }
}

/******************************************************************************
* Function Name: Cy_BLE_IPSS_ExecuteWriteRequestEventHandler
***************************************************************************//**
*
*  Handles the Execute Write Request Event.
*
*  \param eventParam: The pointer to the data that came with a write request.
*
******************************************************************************/
static void Cy_BLE_IPSS_ExecuteWriteRequestEventHandler(cy_stc_ble_gatts_exec_write_req_t *eventParam)
{
    uint32_t locCount;
    cy_stc_ble_ips_char_value_t locCharValue[CY_BLE_IPS_CHAR_COUNT];
    cy_stc_ble_gatt_value_t locGattValue[CY_BLE_IPS_CHAR_COUNT];
    cy_en_ble_ips_char_index_t locCharIndex;

    for(locCharIndex = CY_BLE_IPS_INDOOR_POSITINING_CONFIG; locCharIndex < CY_BLE_IPS_CHAR_COUNT; locCharIndex++)
    {
        locGattValue[locCharIndex].len = 0u;
        locGattValue[locCharIndex].val = NULL;
    }

    for(locCount = 0u; locCount < eventParam->prepWriteReqCount; locCount++)
    {
        for(locCharIndex = CY_BLE_IPS_INDOOR_POSITINING_CONFIG; locCharIndex < CY_BLE_IPS_CHAR_COUNT; locCharIndex++)
        {
            if(eventParam->baseAddr[locCount].handleValuePair.attrHandle ==
               cy_ble_ipsConfigPtr->ipss->charInfo[locCharIndex].charHandle)
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

    for(locCharIndex = CY_BLE_IPS_INDOOR_POSITINING_CONFIG; locCharIndex < CY_BLE_IPS_CHAR_COUNT; locCharIndex++)
    {
        if((Cy_BLE_IPS_ApplCallback != NULL) && (locGattValue[locCharIndex].len != 0u) &&
           (locGattValue[locCharIndex].len <=
            CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN(cy_ble_ipsConfigPtr->ipss->charInfo[locCharIndex].charHandle)))
        {
            /* Check the execWriteFlag before execute or cancel write long operation */
            if(eventParam->execWriteFlag == CY_BLE_GATT_EXECUTE_WRITE_EXEC_FLAG)
            {
                locCharValue[locCharIndex].gattErrorCode = CY_BLE_GATT_ERR_NONE;
                locCharValue[locCharIndex].connHandle = eventParam->connHandle;
                locCharValue[locCharIndex].charIndex = locCharIndex;
                locCharValue[locCharIndex].value = &locGattValue[locCharIndex];

                Cy_BLE_IPS_ApplCallback((uint32_t)CY_BLE_EVT_IPSS_WRITE_CHAR, &locCharValue[locCharIndex]);

                CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(cy_ble_ipsConfigPtr->ipss->charInfo[locCharIndex].charHandle,
                                                     locGattValue[locCharIndex].len);
            }

            /* Indicate that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}

/******************************************************************************
* Function Name: Cy_BLE_IPSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets the characteristic value of the service in the local database.
*
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param attrSize: The size (in bytes) of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*             stored in the GATT database.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is 
*                                               absent.
*
*  NOTE: when advertisement is active this API uses Cy_BLE_GAPP_UpdateAdvScanData() to 
*        update the advertisement packet. In the case of successful execution (return value = CY_BLE_SUCCESS)
*        the event CY_BLE_EVT_GAPP_UPDATE_ADV_SCAN_DATA_COMPLETE appears.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_IPSS_SetCharacteristicValue(cy_en_ble_ips_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    if(charIndex >= CY_BLE_IPS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE == cy_ble_ipsConfigPtr->ipss->charInfo[charIndex].charHandle)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Store characteristic value into GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_ipsConfigPtr->ipss->charInfo[charIndex].charHandle,
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
            apiResult = Cy_BLE_IPSS_SetAdvertisementData();
        }
    }

    return(apiResult);
}

/******************************************************************************
* Function Name: Cy_BLE_IPSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets the characteristic value of the service, which is a value identified by
*  charIndex.
*
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the location where characteristic value data
*             should be stored.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is
*                                               absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_IPSS_GetCharacteristicValue(cy_en_ble_ips_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check parameters */
    if(charIndex >= CY_BLE_IPS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE == cy_ble_ipsConfigPtr->ipss->charInfo[charIndex].charHandle)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get characteristic value from GATT database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_ipsConfigPtr->ipss->charInfo[charIndex].charHandle,
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
* Function Name: Cy_BLE_IPSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets a characteristic descriptor of a specified characteristic of the Indoor
*  Positioning Service from the local GATT database.
*
*  \param charIndex: The index of the characteristic.
*  \param descrIndex: The index of the characteristic descriptor.
*  \param attrSize: The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the location where characteristic descriptor
*               value data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional descriptor is absent
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_IPSS_GetCharacteristicDescriptor(cy_en_ble_ips_char_index_t charIndex,
                                                               cy_en_ble_ips_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    /* Check parameters */
    if((charIndex >= CY_BLE_IPS_CHAR_COUNT) || (descrIndex >= CY_BLE_IPS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE == cy_ble_ipsConfigPtr->ipss->charInfo[charIndex].
             descrHandle[descrIndex])
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get data from database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_ipsConfigPtr->ipss->charInfo[charIndex].descrHandle[descrIndex],
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
* Function Name: Cy_BLE_IPSS_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Set a characteristic descriptor of a specified characteristic of the Indoor
*  Positioning Service from the local GATT database.
*
*  \param charIndex: The index of the characteristic.
*  \param descrIndex: The index of the characteristic descriptor.
*  \param attrSize: The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the descriptor value data to
*             be stored in the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional descriptor is absent
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_IPSS_SetCharacteristicDescriptor(cy_en_ble_ips_char_index_t charIndex,
                                                               cy_en_ble_ips_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Check parameters */
    if((charIndex >= CY_BLE_IPS_CHAR_COUNT) || (descrIndex >= CY_BLE_IPS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE == cy_ble_ipsConfigPtr->ipss->charInfo[charIndex].
             descrHandle[descrIndex])
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Get data from database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_ipsConfigPtr->ipss->charInfo[charIndex].descrHandle[descrIndex],
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
            apiResult = Cy_BLE_IPSS_SetAdvertisementData();
        }
    }

    return(apiResult);
}

#if (CY_BLE_GAP_ROLE_PERIPHERAL)

/******************************************************************************
* Function Name: Cy_BLE_IPSS_SetAdvertisementData
***************************************************************************//**
*
*  Sets the characteristics values in advertisement packet depending on a value of
*  Indoor Positioning Configuration characteristic.
*
*  This function must be called when Cy_BLE_GetBleSsState() returns
*  CY_BLE_BLESS_STATE_EVENT_CLOSE state.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The advertisement packet collected successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Read operation is not permitted on this
*                attribute or on NULL pointer, Data length in input
*                parameter exceeds 31 bytes.
*  * CY_BLE_ERROR_INVALID_OPERATION - The advertisement packet doesn't contain
*                the User List or it is to small or ADV Event is not closed,
*                BLESS is active or ADV is not enabled.
*
*
******************************************************************************/
static cy_en_ble_api_result_t Cy_BLE_IPSS_SetAdvertisementData(void)
{
    uint32_t fFlag;
    uint8_t adLength;
    uint8_t dataLengthAddress = 0u;
    uint8_t byteCounter;
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    uint8_t ipssConfigFlags;
    uint8_t advIndex = 0u;

    do
    {
        byteCounter = 0u;
        fFlag = 0u;
        while((byteCounter < cy_ble_configPtr->discoveryModeInfo[advIndex].advData->advDataLen) && (fFlag == 0u))
        {
            adLength = cy_ble_configPtr->discoveryModeInfo[advIndex].advData->advData[byteCounter];

            if(adLength != 0u)
            {
                /* Increment byte counter so that it points to AD type field */
                byteCounter++;

                if(cy_ble_configPtr->discoveryModeInfo[advIndex].advData->advData[byteCounter] == CY_BLE_IPS_AD_TYPE)
                {
                    /* Start if a "Indoor Positioning" AD type was found. Set flag and exit the loop. */
                    fFlag = 1u;
                    dataLengthAddress = byteCounter - 1u;
                }
                else
                {
                    byteCounter += adLength;
                }
            }
            else
            {
                /* End of advertisement data structure was encountered so exit loop. */
                break;
            }
        }

        if(fFlag != 0u)
        {
            /* Get Configuration value from GATT database */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .offset = 0u,
                .flags  = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };

            dbAttrValInfo.handleValuePair.attrHandle = cy_ble_ipsConfigPtr->ipss->
                                                        charInfo[CY_BLE_IPS_INDOOR_POSITINING_CONFIG].charHandle;
            dbAttrValInfo.handleValuePair.value.len = 1u;
            dbAttrValInfo.handleValuePair.value.val = &ipssConfigFlags;

            /* Increment byte counter to point to Indoor Positioning data */
            byteCounter++;

            if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
            else if((ipssConfigFlags & CY_BLE_IPS_CHARACTERISTICS_IN_ADVERTISING) != 0u)
            {
                cy_ble_configPtr->discoveryModeInfo[advIndex].advData->advData[byteCounter] = ipssConfigFlags;
                byteCounter++;

                if((ipssConfigFlags & CY_BLE_IPS_COORDINATES_IN_ADVERTISING) != 0u)
                {
                    if((ipssConfigFlags & CY_BLE_IPS_TYPE_OF_COORDINATE_IN_ADVERTISING) == CY_BLE_IPS_WGS84_COORDINATE)
                    {
                        /* CY_BLE_IPS_WGS84_COORDINATE
                         * Latitude
                         */
                        dbAttrValInfo.handleValuePair.attrHandle = cy_ble_ipsConfigPtr->ipss->
                                                                    charInfo[CY_BLE_IPS_LATITUDE].charHandle;
                        dbAttrValInfo.handleValuePair.value.len = 4u;
                        dbAttrValInfo.handleValuePair.value.val = &cy_ble_configPtr->discoveryModeInfo[advIndex].
                                                                   advData->advData[byteCounter];

                        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
                        {
                            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                        }
                        byteCounter += (uint8_t)dbAttrValInfo.handleValuePair.value.len;

                        /* Longitude */
                        dbAttrValInfo.handleValuePair.attrHandle = cy_ble_ipsConfigPtr->ipss->
                                                                    charInfo[CY_BLE_IPS_LONGITUDE].charHandle;
                        dbAttrValInfo.handleValuePair.value.len = 4u;
                        dbAttrValInfo.handleValuePair.value.val = &cy_ble_configPtr->discoveryModeInfo[advIndex].
                                                                   advData->advData[byteCounter];

                        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
                        {
                            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                        }
                        byteCounter += (uint8_t)dbAttrValInfo.handleValuePair.value.len;
                    }
                    else
                    {   /*  CY_BLE_IPS_LOCAL_COORDINATE
                         * North coordinate
                         */
                        dbAttrValInfo.handleValuePair.attrHandle = cy_ble_ipsConfigPtr->ipss->
                                                                    charInfo[CY_BLE_IPS_LOCAL_NORTH_COORDINATE].charHandle;
                        dbAttrValInfo.handleValuePair.value.len = 2u;
                        dbAttrValInfo.handleValuePair.value.val = &cy_ble_configPtr->discoveryModeInfo[advIndex].
                                                                   advData->advData[byteCounter];

                        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
                        {
                            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                        }
                        byteCounter += (uint8_t)dbAttrValInfo.handleValuePair.value.len;

                        /* East coordinate */
                        dbAttrValInfo.handleValuePair.attrHandle = cy_ble_ipsConfigPtr->ipss->
                                                                    charInfo[CY_BLE_IPS_LOCAL_EAST_COORDINATE].charHandle;
                        dbAttrValInfo.handleValuePair.value.len = 2u;
                        dbAttrValInfo.handleValuePair.value.val = &cy_ble_configPtr->discoveryModeInfo[advIndex].
                                                                   advData->advData[byteCounter];

                        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
                        {
                            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                        }
                        byteCounter += (uint8_t)dbAttrValInfo.handleValuePair.value.len;
                    }
                }

                if((ipssConfigFlags & CY_BLE_IPS_TX_POWER_IN_ADVERTISING) != 0u)
                {
                    cy_ble_configPtr->discoveryModeInfo[advIndex].advData->advData[byteCounter] = 0xAAu;
                    byteCounter++;
                }

                if((ipssConfigFlags & CY_BLE_IPS_FLOOR_NUMBER_IN_ADVERTISING) != 0u)
                {
                    dbAttrValInfo.handleValuePair.attrHandle = cy_ble_ipsConfigPtr->ipss->
                                                                charInfo[CY_BLE_IPS_FLOOR_NUMBER].charHandle;
                    dbAttrValInfo.handleValuePair.value.len = 1u;
                    dbAttrValInfo.handleValuePair.value.val = &cy_ble_configPtr->discoveryModeInfo[advIndex].
                                                               advData->advData[byteCounter];

                    if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
                    {
                        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                    }
                    byteCounter += (uint8_t)dbAttrValInfo.handleValuePair.value.len;
                }

                if((ipssConfigFlags & CY_BLE_IPS_ALTITUDE_IN_ADVERTISING) != 0u)
                {
                    dbAttrValInfo.handleValuePair.attrHandle = cy_ble_ipsConfigPtr->ipss->charInfo[CY_BLE_IPS_ALTITUDE].
                                                                charHandle;
                    dbAttrValInfo.handleValuePair.value.len = 2u;
                    dbAttrValInfo.handleValuePair.value.val = &cy_ble_configPtr->discoveryModeInfo[advIndex].
                                                               advData->advData[byteCounter];

                    if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
                    {
                        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                    }
                    byteCounter += (uint8_t)dbAttrValInfo.handleValuePair.value.len;
                }

                if((ipssConfigFlags & CY_BLE_IPS_UNCERTAINTY_NUMBER_IN_ADVERTISING) != 0u)
                {
                    dbAttrValInfo.handleValuePair.attrHandle = cy_ble_ipsConfigPtr->ipss->
                                                                charInfo[CY_BLE_IPS_UNCERTAINTY].charHandle;
                    dbAttrValInfo.handleValuePair.value.len = 2u;
                    dbAttrValInfo.handleValuePair.value.val = &cy_ble_configPtr->discoveryModeInfo[advIndex].
                                                               advData->advData[byteCounter];

                    if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
                    {
                        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                    }
                    byteCounter += (uint8_t)dbAttrValInfo.handleValuePair.value.len;
                }
            }
            else /* No CY_BLE_IPS_CHARACTERISTICS_IN_ADVERTISING  */
            {
            }

            cy_ble_configPtr->discoveryModeInfo[advIndex].advData->advData[dataLengthAddress] =
                byteCounter - dataLengthAddress - 1u;
            cy_ble_configPtr->discoveryModeInfo[advIndex].advData->advDataLen = byteCounter;
        }

        advIndex++;
    }
    while(advIndex < CY_BLE_GAPP_CONF_COUNT);

    if(Cy_BLE_GetAdvertisementState() == CY_BLE_ADV_STATE_ADVERTISING)
    {
        apiResult = Cy_BLE_GAPP_UpdateAdvScanData(&cy_ble_configPtr->discoveryModeInfo[cy_ble_advIndex]);
    }

    return(apiResult);
}

#endif /* (CY_BLE_GAP_ROLE_PERIPHERAL) */

#endif /* CY_BLE_IPS_SERVER */

#ifdef CY_BLE_IPS_CLIENT

/******************************************************************************
* Function Name: Cy_BLE_IPSC_SetCharacteristicValueWithoutResponse
***************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server without response.
*
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue:  The pointer to the characteristic value data that should be
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
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_IPSC_SetCharacteristicValueWithoutResponse(cy_stc_ble_conn_handle_t connHandle,
                                                                         cy_en_ble_ips_char_index_t charIndex,
                                                                         uint8_t attrSize,
                                                                         uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);
    cy_stc_ble_gatt_xchg_mtu_param_t mtuParam = { .connHandle = connHandle };

    (void)Cy_BLE_GATT_GetMtuSize(&mtuParam);

    /* Check parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_IPS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_WRITE & cy_ble_ipsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else if((mtuParam.mtu - CY_BLE_GATT_WRITE_HEADER_LEN) < attrSize)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        cy_stc_ble_gattc_write_cmd_req_t writeCmdReqParam =
        {
            .handleValPair.attrHandle = cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        apiResult = Cy_BLE_GATTC_WriteWithoutResponse(&writeCmdReqParam);

        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_ipscReqHandle[discIdx] = cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}

/******************************************************************************
* Function Name: Cy_BLE_IPSC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_IPSS_WRITE_CHAR events is generated.
*  On successful request execution on the Server side the Write Response is
*  sent to the Client.
*
*  The Write Response just confirms the operation success.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize:   The size of the characteristic value attribute.
*  \param attrValue:  The pointer to the characteristic value data that should be
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
*   If the IPS service-specific callback is registered
*      (with Cy_BLE_IPS_RegisterAttrCallback):
*  * CY_BLE_EVT_IPSC_WRITE_CHAR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_ips_char_value_t.
*  .
*  Otherwise (if the IPS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - If the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_EXEC_WRITE_RSP - If the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with an event parameter
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_IPSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_ips_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);
    cy_stc_ble_gatt_xchg_mtu_param_t mtuParam = { .connHandle = connHandle };

    (void)Cy_BLE_GATT_GetMtuSize(&mtuParam);

    /* Check parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_IPS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_WRITE & cy_ble_ipsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else if((mtuParam.mtu - CY_BLE_GATT_WRITE_HEADER_LEN) < attrSize)
    {
        cy_stc_ble_gattc_prep_write_req_t prepWriteReqParam =
        {
            .handleValOffsetPair.handleValuePair.attrHandle = cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValOffsetPair.offset                     = 0u,
            .handleValOffsetPair.handleValuePair.value.val  = attrValue,
            .handleValOffsetPair.handleValuePair.value.len  = attrSize,
            .connHandle                                     = connHandle
        };
        apiResult = Cy_BLE_GATTC_WriteLongCharacteristicValues(&prepWriteReqParam);


        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_ipscReqHandle[discIdx] = cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }
    else
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };

        apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_ipscReqHandle[discIdx] = cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_IPSC_ReliableWriteCharacteristicValue
***************************************************************************//**
*
*  This function is used to perform a reliable write command for the
*  Indoor Positioning Service (identified by charIndex) value attribute to the server.
*
*  The Write response just confirms the operation success.
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
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the IPS service-specific callback is registered
*      (with Cy_BLE_IPS_RegisterAttrCallback):
*  * CY_BLE_EVT_IPSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_ips_char_value_t.
*  .
*   Otherwise (if the IPS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_EXEC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_IPSC_ReliableWriteCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                                    cy_en_ble_ips_char_index_t charIndex,
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
    else if((charIndex >= CY_BLE_IPS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_EXTENDED_PROPERTIES & cy_ble_ipsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_reliable_write_req_t prepWriteReqParam = { .connHandle = connHandle };
        cy_stc_ble_gatt_handle_value_offset_param_t handleValOffsetPairParam =
        {
            .handleValuePair.attrHandle = cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValuePair.value.val  = attrValue,
            .handleValuePair.value.len  = attrSize,
            .offset                     = 0u
        };
        prepWriteReqParam.handleValOffsetPair = &handleValOffsetPairParam;
        prepWriteReqParam.numOfRequests = 1u;

        apiResult = Cy_BLE_GATTC_ReliableWrites(&prepWriteReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_ipscReqHandle[discIdx] = cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}

/******************************************************************************
* Function Name: Cy_BLE_IPSC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read the characteristic Value from a server,
*  as identified by its charIndex
*
*  The Read Response returns the characteristic Value in the Attribute Value
*  parameter.
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
*   If the IPS service-specific callback is registered
*      (with Cy_BLE_IPS_RegisterAttrCallback):
*  * CY_BLE_EVT_IPSC_READ_CHAR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_ips_char_value_t.
*  .
*   Otherwise (if the IPS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_IPSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_ips_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_IPS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_READ & cy_ble_ipsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle,
            .connHandle = connHandle,
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);
    }

    if(apiResult == CY_BLE_SUCCESS)
    {
        cy_ble_ipscReqHandle[discIdx] = cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_IPSC_GetLongCharacteristicValue
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
*   If the IPS service-specific callback is registered
*      (with Cy_BLE_IPS_RegisterAttrCallback):
*  * CY_BLE_EVT_IPSC_READ_CHAR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_ips_char_value_t.
*  .
*   Otherwise (if the IPS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_IPSC_GetLongCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                              cy_en_ble_ips_char_index_t charIndex,
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
    else if((attrSize == 0u) || (attrValue == NULL) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((CY_BLE_CHAR_PROP_READ & cy_ble_ipsc[discIdx].charInfo[charIndex].properties) == 0u)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        cy_stc_ble_gattc_read_blob_req_t readBlobReqParam =
        {
            .handleOffset.attrHandle = cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle,
            .handleOffset.offset     = 0u,
            .connHandle              = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadLongCharacteristicValues(&readBlobReqParam);
    }

    if(apiResult == CY_BLE_SUCCESS)
    {
        cy_ble_ipscReqHandle[discIdx] = cy_ble_ipsc[discIdx].charInfo[charIndex].valueHandle;
        cy_ble_ipscRdLongBuffLen[discIdx] = attrSize;
        cy_ble_ipscRdLongBuffPtr[discIdx] = attrValue;
        cy_ble_ipscCurrLen[discIdx] = 0u;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_IPSC_GetMultipleCharacteristicValues
***************************************************************************//**
*
*  This function reads multiple Characteristic Values from a GATT Server when
*  the GATT Client knows the Characteristic value handles. This is a
*  non-blocking function.
*
*  Internally, Read Multiple Request is sent to the peer device in response to
*  which Read Multiple Response is received. This results in
*  CY_BLE_EVT_GATTC_READ_MULTI_RSP event, which is propagated to the application
*  layer.
*
*  An Error Response event is sent by the server (CY_BLE_EVT_GATTC_ERROR_RSP) in
*  response to the Read Multiple Request if insufficient authentication,
*  insufficient authorization, insufficient encryption key size is used by the
*  client, or if a read operation is not permitted on any of the Characteristic
*  values. The Error Code parameter is set as specified in the Attribute
*  Protocol.
*
*  Refer to Bluetooth 4.1 core specification, Volume 3, Part G, section 4.8.4
*  for more details on the sequence of operations.
*
*  \param connHandle: Connection handle to identify the peer GATT entity, of type
*               cy_stc_ble_conn_handle_t.
*  \param charIndexesList: Pointer to a list of Characteristic value handles
*  \param numberOfCharIndexes: Number of requested Characteristic handles
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                              | Description
*  ------------                              | -----------
*   CY_BLE_SUCCESS                           | On successful operation
*   CY_BLE_ERROR_INVALID_PARAMETER           | 'connHandle' value does not represent any existing entry in the Stack
*   CY_BLE_ERROR_INVALID_OPERATION           | This operation is not permitted
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED    | Memory allocation failed
*   CY_BLE_ERROR_INVALID_STATE               | Connection with the Client is not established.
*   CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE | The peer device doesn't have the particular characteristic.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the IPS service-specific callback is registered
*      (with Cy_BLE_IPS_RegisterAttrCallback):
*  * CY_BLE_EVT_IPSC_READ_MULTIPLE_CHAR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_ips_char_value_t.
*  .
*   Otherwise (if the IPS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_MULTI_RSP - If the requested attribute is
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
cy_en_ble_api_result_t Cy_BLE_IPSC_GetMultipleCharacteristicValues(cy_stc_ble_conn_handle_t connHandle,
                                                                   const cy_en_ble_ips_char_index_t  *charIndexesList,
                                                                   uint8_t numberOfCharIndexes)

{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    cy_stc_ble_gattc_read_mult_req_t readMultiReqParam = { .connHandle = connHandle };
    uint16_t valueHandles[CY_BLE_IPS_CHAR_COUNT];
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);
    uint32_t indexInList;

    /* Check parameters */
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
        for(indexInList = 0u; ((indexInList < numberOfCharIndexes) && (apiResult == CY_BLE_SUCCESS)); indexInList++)
        {
            if(cy_ble_ipsc[discIdx].charInfo[charIndexesList[indexInList]].valueHandle ==
               CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
            }
            else if((CY_BLE_CHAR_PROP_READ & cy_ble_ipsc[discIdx].charInfo[charIndexesList[indexInList]].properties) == 0u)
            {
                apiResult = CY_BLE_ERROR_INVALID_OPERATION;
            }
            else
            {
                valueHandles[indexInList] = cy_ble_ipsc[discIdx].charInfo[charIndexesList[indexInList]].valueHandle;
            }
        }

        if(apiResult == CY_BLE_SUCCESS)
        {
            readMultiReqParam.handleListType.listCount = numberOfCharIndexes;
            readMultiReqParam.handleListType.handleList = valueHandles;
            apiResult = Cy_BLE_GATTC_ReadMultipleCharacteristicValues(&readMultiReqParam);
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_IPSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  This function is used to write the characteristic Value to the server,
*  as identified by its charIndex.
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
*   If the IPS service-specific callback is registered
*      (with Cy_BLE_IPS_RegisterAttrCallback):
*  * CY_BLE_EVT_IPSC_WRITE_DESCR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with an event parameter structure
*                                of type cy_stc_ble_ips_descr_value_t.
*  .
*   Otherwise (if the IPS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - If the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - If there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with an event parameter
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_IPSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ips_char_index_t charIndex,
                                                               cy_en_ble_ips_descr_index_t descrIndex,
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
    else if((charIndex >= CY_BLE_IPS_CHAR_COUNT) || (descrIndex >= CY_BLE_IPS_DESCR_COUNT) || 
            (attrSize != CY_BLE_CCCD_LEN) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_ipsc[discIdx].charInfo[charIndex].descrHandle[descrIndex] == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_ipsc[discIdx].charInfo[charIndex].descrHandle[descrIndex],
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = CY_BLE_CCCD_LEN,
            .connHandle               = connHandle
        };
        /* ... and send request to server device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            /* Save handle to support service specific read response from device */
            cy_ble_ipscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_IPSC_GetCharacteristicDescriptor
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
*  In the case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the IPS service-specific callback is registered
*      (with Cy_BLE_IPS_RegisterAttrCallback):
*  * CY_BLE_EVT_IPSC_READ_DESCR_RESPONSE - If the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with an event parameter structure
*                                of type cy_stc_ble_ips_descr_value_t.
*  .
*  Otherwise (if the IPS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_IPSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ips_char_index_t charIndex,
                                                               cy_en_ble_ips_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    /* Check parameters */
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_IPS_CHAR_COUNT) || (descrIndex >= CY_BLE_IPS_DESCR_COUNT) ||
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_ipsc[discIdx].charInfo[charIndex].descrHandle[descrIndex] == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_ipsc[discIdx].charInfo[charIndex].descrHandle[descrIndex],
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_ipscReqHandle[discIdx] = cy_ble_ipsc[discIdx].charInfo[charIndex].descrHandle[descrIndex];
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_IPSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_IPSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* User Data Service characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_ipscCharUuid[CY_BLE_IPS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_INDOOR_POSITINING_CONFIG,  /**< Set of characteristic values included in the Indoor Positioning
                                                     *   Service AD type */
        CY_BLE_UUID_CHAR_LATITUDE,                  /**< WGS84 North coordinate of the device.*/
        CY_BLE_UUID_CHAR_LONGITUDE,                 /**< WGS84 East coordinate of the device.*/
        CY_BLE_UUID_CHAR_LOCAL_NORTH_COORDINATE,    /**< North coordinate of the device using local coordinate system. */
        CY_BLE_UUID_CHAR_LOCAL_EAST_COORDINATE,     /**< East coordinate of the device using local coordinate system. */
        CY_BLE_UUID_CHAR_FLOOR_NUMBER,              /**< Describes in which floor the device is installed. */
        CY_BLE_UUID_CHAR_ALTITUDE,                  /**< Altitude of the device. */
        CY_BLE_UUID_CHAR_UNCERTAINTY,               /**< Uncertainty of the location information the device exposes. */
        CY_BLE_UUID_CHAR_LOCATION_NAME              /**< Name of the location the device is installed. */
    };
    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_IPS))
    {
        /* Update last characteristic endHandle to declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        for(i = 0u; i < (uint32_t)CY_BLE_IPS_CHAR_COUNT; i++)
        {
            if(cy_ble_ipscCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_ipsc[discIdx].charInfo[i].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_ipsc[discIdx].charInfo[i].valueHandle = discCharInfo->valueHandle;
                    cy_ble_ipsc[discIdx].charInfo[i].properties = discCharInfo->properties;
                    lastEndHandle[discIdx] = &cy_ble_ipsc[discIdx].charInfo[i].endHandle;
                }
                else
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, &discCharInfo);
                }
            }
        }

        /* Init characteristic endHandle to Service endHandle.
         * Characteristic endHandle will be updated to the declaration
         * Handler of the following characteristic,
         * in the following characteristic discovery procedure. */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = cy_ble_serverInfo[discIdx][CY_BLE_SRVI_IPS].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_IPSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to the characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_IPSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t notSupportedDescr = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t descIdx;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_IPS)
    {
        switch(discDescrInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_SERVER_CONFIG:
                descIdx = (uint32_t)CY_BLE_IPS_SCCD;
                break;

            case CY_BLE_UUID_CHAR_EXTENDED_PROPERTIES:
                descIdx = (uint32_t)CY_BLE_IPS_CEPD;
                break;

            default:
                /* Not supported descriptor */
                notSupportedDescr = 1u;
                break;
        }

        if(notSupportedDescr == 0u)
        {
            if(cy_ble_ipsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] ==
               CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_ipsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] =
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
* Function Name: Cy_BLE_IPSC_GetCharRange
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
static void Cy_BLE_IPSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_IPS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_IPS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if((cy_ble_ipsc[discIdx].charInfo[charIdx].endHandle - cy_ble_ipsc[discIdx].charInfo[charIdx].valueHandle) != 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_ipsc[discIdx].charInfo[charIdx].valueHandle + 1u;
                charRangeInfo->range.endHandle = cy_ble_ipsc[discIdx].charInfo[charIdx].endHandle;
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
* Function Name: Cy_BLE_IPSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles the Read Response Event.
*
*  \param eventParam - The pointer to the data structure.
*
******************************************************************************/
static void Cy_BLE_IPSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    cy_en_ble_ips_char_index_t locCharIndex;
    uint32_t locReqHandle = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_IPS_ApplCallback != NULL) &&
       (cy_ble_ipscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locCharIndex = CY_BLE_IPS_INDOOR_POSITINING_CONFIG; (locCharIndex < CY_BLE_IPS_CHAR_COUNT) &&
            (locReqHandle == 0u); locCharIndex++)
        {
            if(cy_ble_ipscReqHandle[discIdx] == cy_ble_ipsc[discIdx].charInfo[locCharIndex].valueHandle)
            {
                cy_stc_ble_ips_char_value_t locCharValue =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = locCharIndex,
                    .value      = &eventParam->value
                };
                cy_ble_ipscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_IPS_ApplCallback((uint32_t)CY_BLE_EVT_IPSC_READ_CHAR_RESPONSE, &locCharValue);
                locReqHandle = 1u;
            }
            else
            {
                cy_en_ble_ips_descr_index_t locDescIndex;

                for(locDescIndex = CY_BLE_IPS_SCCD; (locDescIndex < CY_BLE_IPS_DESCR_COUNT) &&
                    (locReqHandle == 0u); locDescIndex++)
                {
                    if(cy_ble_ipscReqHandle[discIdx] == cy_ble_ipsc[discIdx].charInfo[locCharIndex].
                        descrHandle[locDescIndex])
                    {
                        cy_stc_ble_ips_descr_value_t locDescrValue =
                        {
                            .connHandle = eventParam->connHandle,
                            .charIndex  = locCharIndex,
                            .descrIndex = locDescIndex,
                            .value      = &eventParam->value
                        };

                        cy_ble_ipscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                        Cy_BLE_IPS_ApplCallback((uint32_t)CY_BLE_EVT_IPSC_READ_DESCR_RESPONSE, &locDescrValue);
                        locReqHandle = 1u;
                    }
                }
            }
        }
        if(locReqHandle != 0u)
        {
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}

/******************************************************************************
* Function Name: Cy_BLE_IPSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles the Read Response Event.
*
*  \param eventParam - The pointer to the data structure.
*
******************************************************************************/
static void Cy_BLE_IPSC_ReadMultipleResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    if(Cy_BLE_IPS_ApplCallback != NULL)
    {
        Cy_BLE_IPS_ApplCallback((uint32_t)CY_BLE_EVT_IPSC_READ_MULTIPLE_CHAR_RESPONSE, eventParam);
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}

/******************************************************************************
* Function Name: Cy_BLE_IPSC_ReadLongRespEventHandler
***************************************************************************//**
*
*  Handles a Read Long Response event.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_IPSC_ReadLongRespEventHandler(const cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    cy_stc_ble_gattc_stop_cmd_param_t stopCmdParam = { .connHandle = eventParam->connHandle };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_IPS_ApplCallback != NULL) &&
       (cy_ble_ipscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        cy_stc_ble_ips_char_value_t locCharValue = { .connHandle = eventParam->connHandle };
        cy_stc_ble_gatt_value_t locGattValue;
        /* Go through all long characteristics */
        for(locCharValue.charIndex = CY_BLE_IPS_INDOOR_POSITINING_CONFIG; locCharValue.charIndex < CY_BLE_IPS_CHAR_COUNT;
            locCharValue.charIndex++)
        {
            if(cy_ble_ipsc[discIdx].charInfo[locCharValue.charIndex].valueHandle == cy_ble_ipscReqHandle[discIdx])
            {
                uint32_t i;

                /* Update user buffer with received data */
                for(i = 0u; i < eventParam->value.len; i++)
                {
                    if(cy_ble_ipscCurrLen[discIdx] < cy_ble_ipscRdLongBuffLen[discIdx])
                    {
                        cy_ble_ipscRdLongBuffPtr[discIdx][cy_ble_ipscCurrLen[discIdx]] = eventParam->value.val[i];
                        cy_ble_ipscCurrLen[discIdx]++;
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
                    cy_stc_ble_gatt_xchg_mtu_param_t mtuParam = { .connHandle = eventParam->connHandle };
                    (void)Cy_BLE_GATT_GetMtuSize(&mtuParam);

                    /* If received data length is less than MTU size, Read Long
                     * request is completed or provided user's buffer is full.
                     */
                    locGattValue.val = cy_ble_ipscRdLongBuffPtr[discIdx];
                    locGattValue.len = cy_ble_ipscCurrLen[discIdx];
                    locCharValue.value = &locGattValue;

                    if(((mtuParam.mtu - 1u) > eventParam->value.len))
                    {
                        Cy_BLE_IPS_ApplCallback((uint32_t)CY_BLE_EVT_IPSC_READ_CHAR_RESPONSE, &locCharValue);
                        cy_ble_ipscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                    }
                    else
                    {
                        Cy_BLE_IPS_ApplCallback((uint32_t)CY_BLE_EVT_IPSC_READ_BLOB_RSP, &locCharValue);
                    }

                    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                }

                break;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_IPSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles the Write Response Event.
*
*  \param eventParam: The pointer to a data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_IPSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t locReqHandle = 0u;
    cy_en_ble_ips_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_IPS_ApplCallback != NULL) &&
       (cy_ble_ipscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locCharIndex = CY_BLE_IPS_INDOOR_POSITINING_CONFIG; (locCharIndex < CY_BLE_IPS_CHAR_COUNT) &&
            (locReqHandle == 0u); locCharIndex++)
        {
            if(cy_ble_ipscReqHandle[discIdx] == cy_ble_ipsc[discIdx].charInfo[locCharIndex].valueHandle)
            {
                cy_stc_ble_ips_char_value_t locCharValue =
                {
                    .connHandle = *eventParam,
                    .charIndex  = locCharIndex,
                    .value      = NULL
                };
                cy_ble_ipscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_IPS_ApplCallback((uint32_t)CY_BLE_EVT_IPSC_WRITE_CHAR_RESPONSE, &locCharValue);
                locReqHandle = 1u;
            }
            else
            {
                cy_en_ble_ips_descr_index_t locDescIndex;
                for(locDescIndex = CY_BLE_IPS_SCCD; (locDescIndex < CY_BLE_IPS_DESCR_COUNT) &&
                    (locReqHandle == 0u); locDescIndex++)
                {
                    if(cy_ble_ipscReqHandle[discIdx] == cy_ble_ipsc[discIdx].charInfo[locCharIndex].
                        descrHandle[locDescIndex])
                    {
                        cy_stc_ble_ips_descr_value_t locDescrValue =
                        {
                            .connHandle = *eventParam,
                            .charIndex  = locCharIndex,
                            .descrIndex = locDescIndex,
                            .value      = NULL
                        };

                        cy_ble_ipscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                        Cy_BLE_IPS_ApplCallback((uint32_t)CY_BLE_EVT_IPSC_WRITE_DESCR_RESPONSE, &locDescrValue);
                        locReqHandle = 1u;
                    }
                }
            }
        }
        if(locReqHandle != 0u)
        {
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}

/******************************************************************************
* Function Name: Cy_BLE_IPSC_ExecuteWriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Execute Write Response event for the User Data Service.
*
*  \param eventParam: The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_IPSC_ExecuteWriteResponseEventHandler(const cy_stc_ble_gattc_exec_write_rsp_t *eventParam)
{
    cy_stc_ble_ips_char_value_t locCharVal = { .connHandle = eventParam->connHandle };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
    uint32_t i;

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_IPS_ApplCallback != NULL) &&
       (cy_ble_ipscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(i = 0u; i < ((uint8_t)CY_BLE_IPS_CHAR_COUNT); i++)
        {
            if(cy_ble_ipsc[discIdx].charInfo[i].valueHandle == cy_ble_ipscReqHandle[discIdx])
            {
                locCharVal.charIndex = (cy_en_ble_ips_char_index_t)i;
                locCharVal.value = NULL;
                cy_ble_ipscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_IPS_ApplCallback((uint32_t)CY_BLE_EVT_IPSC_WRITE_CHAR_RESPONSE, &locCharVal);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            }
        }
    }
}

/******************************************************************************
* Function Name: Cy_BLE_IPSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles the Error Response Event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_IPSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
        if(cy_ble_ipscReqHandle[discIdx] == eventParam->errInfo.attrHandle)
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
                if(Cy_BLE_IPS_ApplCallback != NULL)
                {
                    cy_stc_ble_ips_char_value_t locGattError =
                    {
                        .connHandle    = eventParam->connHandle,
                        .value         = NULL,
                        .gattErrorCode = eventParam->errInfo.errorCode
                    };

                    for(locGattError.charIndex = CY_BLE_IPS_INDOOR_POSITINING_CONFIG;
                        locGattError.charIndex < CY_BLE_IPS_CHAR_COUNT; locGattError.charIndex++)
                    {
                        if(cy_ble_ipsc[discIdx].charInfo[locGattError.charIndex].valueHandle ==
                           eventParam->errInfo.attrHandle)
                        {
                            Cy_BLE_IPS_ApplCallback((uint32_t)CY_BLE_EVT_IPSC_ERROR_RESPONSE, &locGattError);
                            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                            break;
                        }
                    }
                }

                cy_ble_ipscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    }
}

#endif /* CY_BLE_IPS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_IPS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for Indoor Positioning Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_IPS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_IPS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_IPSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_IPSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_IPSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_IPS_CLIENT */

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
        #ifdef CY_BLE_IPS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_IPSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_WRITE_CMD_REQ:
                Cy_BLE_IPSS_WriteCmdEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_EXEC_WRITE_REQ:
                Cy_BLE_IPSS_ExecuteWriteRequestEventHandler((cy_stc_ble_gatts_exec_write_req_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_PREP_WRITE_REQ:
                Cy_BLE_IPSS_PrepareWriteRequestEventHandler((cy_stc_ble_gatts_prep_write_req_param_t*)eventParam);
                break;
        #endif /* CY_BLE_IPS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_IPS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_IPSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_IPSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_IPSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_MULTI_RSP:
                Cy_BLE_IPSC_ReadMultipleResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_BLOB_RSP:
                Cy_BLE_IPSC_ReadLongRespEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_EXEC_WRITE_RSP:
                Cy_BLE_IPSC_ExecuteWriteResponseEventHandler((cy_stc_ble_gattc_exec_write_rsp_t*)eventParam);
                break;
        #endif /* CY_BLE_IPS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_IPS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
