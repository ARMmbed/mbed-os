/***************************************************************************//**
* \file cy_ble_cts.c
* \version 2.0
*
* \brief
*  This file contains the source code for the Current Time Service of the BLE
*  Component.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_CTS))

static cy_ble_callback_t Cy_BLE_CTS_ApplCallback = NULL;

#ifdef CY_BLE_CTS_CLIENT
/* CTS Center Service characteristics GATT DB handles structure */
cy_stc_ble_ctsc_t cy_ble_ctsc[CY_BLE_CONFIG_GATTC_COUNT];

/* Internal storage for last request handle to check the response */
static cy_ble_gatt_db_attr_handle_t cy_ble_ctscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_CTS_CLIENT) */

/* The pointer on global BLE CTS Config structure */
cy_stc_ble_cts_config_t *cy_ble_ctsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_CTS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_CTSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* (CY_BLE_CTS_SERVER) */

#ifdef CY_BLE_CTS_CLIENT
static void Cy_BLE_CTSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_CTSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_CTSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_CTSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_CTSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_CTSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_CTSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* (CY_BLE_CTS_CLIENT) */
static cy_en_ble_gatt_err_code_t Cy_BLE_CTS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_CTS_Init
***************************************************************************//**
*
*  This function initializes the Current Time Service.
*
*  \param config: Configuration structure for the Current Time Service.
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
cy_en_ble_api_result_t Cy_BLE_CTS_Init(cy_stc_ble_cts_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_ctsConfigPtr = config;

        /* Registers Event Handler for the CTS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_CTS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_ctsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_CTS_ApplCallback = cy_ble_ctsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_CTS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_CTS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset((void*)&cy_ble_ctsc[idx], 0, sizeof(cy_stc_ble_ctsc_t));
                }
                cy_ble_ctscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;

                /* initialize uuid  */
                cy_ble_serverInfo[idx][CY_BLE_SRVI_CTS].uuid = CY_BLE_UUID_CURRENT_TIME_SERVICE;
            }
        }
    #endif /* (CY_BLE_CTS_CLIENT) */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CTS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc: An application layer event callback function to receive
*                    events from the BLE Component. The definition of
*                    cy_ble_callback_t for Current Time Service is:\n
*                    typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                       void *eventParam)
*                    * eventCode indicates the event that triggered this
*                      callback (e.g. CY_BLE_EVT_CTSS_NOTIFICATION_ENABLED)
*                    * eventParam contains the parameters corresponding to the
*                      current event (e.g. Pointer to cy_stc_ble_cts_char_value_t
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
cy_en_ble_api_result_t Cy_BLE_CTS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_CTS_ApplCallback = callbackFunc;
    if(cy_ble_ctsConfigPtr != NULL)
    {
        cy_ble_ctsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_CTS_SERVER

/******************************************************************************
* Function Name: Cy_BLE_CTSS_WriteEventHandler
***************************************************************************//**
*
*  Handles the Write Request Event for the Current Time Service.
*
*  \param eventParam: The pointer to the data that came with a write request for the
*              Current Time Service.
*
* \return
*  Return a value of type cy_en_ble_gatt_err_code_t:
*   * CY_BLE_GATT_ERR_NONE - Function terminated successfully.
*   * CY_BLE_GATT_ERR_INVALID_HANDLE - The Handle of the Current Time Client
*                                      Configuration Characteristic Descriptor
*                                      is not valid.
*   * CY_BLE_GATT_ERR_UNLIKELY_ERROR - An Internal Stack error occurred.
*   * CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED - The notification property of the
*                                             Current Time Client Configuration
*                                             Characteristic Descriptor is
*                                             disabled.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_CTSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    uint32_t event = (uint32_t)CY_BLE_EVT_CTSS_NOTIFICATION_DISABLED;
    cy_stc_ble_cts_char_value_t wrReqParam =
    {
        .gattErrorCode = CY_BLE_GATT_ERR_NONE,
        .connHandle    = eventParam->connHandle,
        .value         = &eventParam->handleValPair.value
    };

    if(Cy_BLE_CTS_ApplCallback != NULL)
    {
        /* Client Characteristic Configuration descriptor write request */
        if(eventParam->handleValPair.attrHandle == cy_ble_ctsConfigPtr->ctss->currTimeCccdHandle)
        {
            /* Verify that optional notification property is enabled for Current Time
             * Characteristic.
             */
            if(CY_BLE_IS_NOTIFICATION_SUPPORTED(cy_ble_ctsConfigPtr->ctss->currTimeCharHandle))
            {
                cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                {
                    .handleValuePair = eventParam->handleValPair,
                    .connHandle      = eventParam->connHandle,
                    .offset          = 0u,
                    .flags           = CY_BLE_GATT_DB_PEER_INITIATED
                };
                wrReqParam.gattErrorCode = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);

                if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                {
                    event = (uint32_t)CY_BLE_EVT_CTSS_NOTIFICATION_ENABLED;
                }

                if(wrReqParam.gattErrorCode == CY_BLE_GATT_ERR_NONE)
                {
                    wrReqParam.charIndex = CY_BLE_CTS_CURRENT_TIME;
                    wrReqParam.value = NULL;
                    Cy_BLE_CTS_ApplCallback(event, &wrReqParam);
                }

            #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
                (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
                /* Set flag to store bonding data to flash */
                if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
                {
                    cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
                }
            #endif /* (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
                    * (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES)
                    */
            }
            else
            {
                wrReqParam.gattErrorCode = CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED;
            }

            /* Clear the callback flag indicating that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else if((eventParam->handleValPair.attrHandle == cy_ble_ctsConfigPtr->ctss->currTimeCharHandle) ||
                (eventParam->handleValPair.attrHandle == cy_ble_ctsConfigPtr->ctss->localTimeInfCharHandle))
        {
            if(eventParam->handleValPair.attrHandle == cy_ble_ctsConfigPtr->ctss->currTimeCharHandle)
            {
                wrReqParam.charIndex = CY_BLE_CTS_CURRENT_TIME;
            }
            else
            {
                wrReqParam.charIndex = CY_BLE_CTS_LOCAL_TIME_INFO;
            }
            /* Check if write property is supported */
            wrReqParam.gattErrorCode = Cy_BLE_GATT_DbCheckPermission(eventParam->handleValPair.attrHandle,
                                                                     &eventParam->connHandle, CY_BLE_GATT_DB_WRITE |
                                                                     CY_BLE_GATT_DB_PEER_INITIATED);

            if(wrReqParam.gattErrorCode == CY_BLE_GATT_ERR_NONE)
            {
                /* Send callback to user. User must validate the fields of the
                 * Current Time Characteristic and then perform database
                 * Characteristic Write procedure to write all or only selected
                 * fields to the database. In case if user want to ignore some
                 * fields of Current Time Characteristic user has to set the
                 * CY_BLE_GATT_ERR_CTS_DATA_FIELD_IGNORED error value in the
                 * "gattErrorCode" field of the structure passed from the with
                 * CY_BLE_EVT_CTSS_WRITE_CHAR event. In case if no fields in the
                 * Current Time Characteristic is ignored the "gattErrorCode"
                 * should be ignored.
                 */
                Cy_BLE_CTS_ApplCallback((uint32_t)CY_BLE_EVT_CTSS_WRITE_CHAR, (void*)&wrReqParam);

                if(wrReqParam.gattErrorCode != CY_BLE_GATT_ERR_CTS_DATA_FIELD_IGNORED)
                {
                    /* Ignore all gatt errors except permitted "Data Field Ignored" */
                    wrReqParam.gattErrorCode = CY_BLE_GATT_ERR_NONE;
                }
            }
            else
            {
                wrReqParam.gattErrorCode = CY_BLE_GATT_ERR_WRITE_NOT_PERMITTED;
            }

            /* Clear the callback flag indicating that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else
        {
            /* This empty else statement is required by MISRA */
        }
    }

    return(wrReqParam.gattErrorCode);
}


/******************************************************************************
* Function Name: Cy_BLE_CTSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets a value for one of three characteristic values of the Current Time
*  Service. The characteristic is identified by charIndex.
*
*  \param charIndex: The index of the Current Time Service characteristic.
*  \param attrSize:  The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*              stored to the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic value was written successfully
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CTSS_SetCharacteristicValue(cy_en_ble_cts_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    uint16_t locYear;
    cy_stc_ble_cts_current_time_t currTime;
    cy_stc_ble_cts_local_time_info_t localTime;
    cy_stc_ble_cts_reference_time_info_t refTime;
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if(attrValue != NULL)
    {
        switch(charIndex)
        {
            case CY_BLE_CTS_CURRENT_TIME:
                if(attrSize == CY_BLE_CTS_CURRENT_TIME_SIZE)
                {
                    (void)memcpy((void*)&currTime, (void*)attrValue, (uint32_t)attrSize);

                    /* Validate characteristic value
                     * First, validate "locYear" field
                     */
                    locYear = ((uint16_t)((uint16_t)currTime.yearHigh << CY_BLE_CTS_8_BIT_OFFSET)) |
                              ((uint16_t)currTime.yearLow);

                    if((locYear >= CY_BLE_CTS_YEAR_MIN) && (locYear <= CY_BLE_CTS_YEAR_MAX))
                    {
                        /* Validation passed */
                        apiResult = CY_BLE_SUCCESS;
                    }

                    /* Validate "Month" field */
                    if((CY_BLE_ERROR_INVALID_PARAMETER != apiResult) && (CY_BLE_CTS_MONTH_MAX < currTime.month))
                    {
                        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                    }

                    /* Next is "Day" field */
                    if((CY_BLE_ERROR_INVALID_PARAMETER != apiResult) && (CY_BLE_CTS_DAY_MAX < currTime.day))
                    {
                        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                    }

                    /* Validate "Hours" field */
                    if((CY_BLE_ERROR_INVALID_PARAMETER != apiResult) && (CY_BLE_CTS_HOURS_MAX < currTime.hours))
                    {
                        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                    }

                    /* Validate "Minutes" field */
                    if((CY_BLE_ERROR_INVALID_PARAMETER != apiResult) && (CY_BLE_CTS_MINUTES_MAX < currTime.minutes))
                    {
                        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                    }

                    /* Validate "Seconds" field */
                    if((CY_BLE_ERROR_INVALID_PARAMETER != apiResult) && (CY_BLE_CTS_SECONDS_MAX < currTime.seconds))
                    {
                        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                    }

                    /* Validate "dayOfWeek" field */
                    if((CY_BLE_ERROR_INVALID_PARAMETER != apiResult) && (CY_BLE_CTS_DAY_OF_WEEK_MAX <= currTime.dayOfWeek))
                    {
                        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                    }

                    if(apiResult != CY_BLE_ERROR_INVALID_PARAMETER)
                    {
                        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                        {
                            .handleValuePair.attrHandle = cy_ble_ctsConfigPtr->ctss->currTimeCharHandle,
                            .handleValuePair.value.len  = attrSize,
                            .offset                     = 0u,
                            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
                        };
                        dbAttrValInfo.handleValuePair.value.val = (uint8_t*)&currTime;

                        /* Verify "Adjust Reason". If it is set to incorrect value then it will
                         * be changed to "Unknown" Adjust Reason. */
                        CY_BLE_CTS_CHECK_ADJUST_REASON(currTime.adjustReason);

                        /* Set Current Time Characteristic value to GATT database.
                         * Need to handle return type difference of Cy_BLE_GATTS_WriteAttributeValueCCCD() and
                         * Cy_BLE_CTSS_SetCharacteristicValue(). */

                        if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
                        {
                            /* Indicate success */
                            apiResult = CY_BLE_SUCCESS;
                        }
                    }
                }
                break;

            case CY_BLE_CTS_LOCAL_TIME_INFO:
                if(attrSize == CY_BLE_CTS_LOCAL_TIME_INFO_SIZE)
                {
                    (void)memcpy((void*)&localTime, (void*)attrValue, (uint32_t)attrSize);

                    if(((localTime.timeZone <= CY_BLE_CTS_UTC_OFFSET_MAX) &&
                        (localTime.timeZone >= CY_BLE_CTS_UTC_OFFSET_MIN)) ||
                       (localTime.timeZone == CY_BLE_CTS_UTC_OFFSET_UNKNOWN))
                    {
                        /* Validation passed */
                        apiResult = CY_BLE_SUCCESS;
                    }

                    if(apiResult != CY_BLE_ERROR_INVALID_PARAMETER)
                    {
                        /* Validate DST offset */
                        switch(localTime.dst)
                        {
                            case CY_BLE_CTS_STANDARD_TIME:
                            case CY_BLE_CTS_DAYLIGHT_TIME_0_5:
                            case CY_BLE_CTS_DAYLIGHT_TIME_1_0:
                            case CY_BLE_CTS_DAYLIGHT_TIME_2_0:
                            case CY_BLE_CTS_DST_UNKNOWN:
                                /* DST offset is correct */
                                apiResult = CY_BLE_SUCCESS;
                                break;

                            default:
                                /* Validation failed */
                                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                                break;
                        }
                    }

                    if(apiResult != CY_BLE_ERROR_INVALID_PARAMETER)
                    {
                        if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_ctsConfigPtr->ctss->localTimeInfCharHandle)
                        {
                            /* Set Local Time Characteristic value to GATT database.
                             *  Need to handle return type difference of Cy_BLE_GATTS_WriteAttributeValueCCCD() and
                             *  Cy_BLE_CTSS_SetCharacteristicValue(). */
                            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                            {
                                .handleValuePair.attrHandle = cy_ble_ctsConfigPtr->ctss->localTimeInfCharHandle,
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
                        else
                        {
                            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
                        }
                    }
                }
                break;

            case CY_BLE_CTS_REFERENCE_TIME_INFO:
                if(attrSize == CY_BLE_CTS_REFERENCE_TIME_INFO_SIZE)
                {
                    (void)memcpy((void*)&refTime, (void*)attrValue, (uint32_t)attrSize);

                    /* Validate "Time Source" value */
                    if(CY_BLE_CTS_TIME_SRC_CELL_NET >= refTime.timeSource)
                    {
                        /* Time source is correct */
                        apiResult = CY_BLE_SUCCESS;
                    }

                    if(apiResult != CY_BLE_ERROR_INVALID_PARAMETER)
                    {
                        /* Validate "Hours since update" field */
                        if(refTime.hoursSinseUpdate <= CY_BLE_CTS_HOURS_MAX)
                        {
                            /* Value is correct */
                        }
                        else if(refTime.daysSinceUpdate == CY_BLE_CTS_MAX_DAYS_SINCE_UPDATE)
                        {
                            /* Per CTS spec "Hours since update" is set to 255 as "Days since update"
                             * is 255. */
                            refTime.hoursSinseUpdate = CY_BLE_CTS_MAX_HOURS_SINCE_UPDATE;
                        }
                        else
                        {
                            /* Invalid value encountered */
                            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                        }
                    }

                    if(apiResult != CY_BLE_ERROR_INVALID_PARAMETER)
                    {
                        if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_ctsConfigPtr->ctss->localTimeInfCharHandle)
                        {
                            /* Set Reference Time Characteristic value to GATT database.
                             *  Need to handle return type difference of Cy_BLE_GATTS_WriteAttributeValueCCCD() and
                             *  Cy_BLE_CTSS_SetCharacteristicValue(). */
                            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                            {
                                .handleValuePair.attrHandle = cy_ble_ctsConfigPtr->ctss->refTimeInfCharHandle,
                                .handleValuePair.value.len  = attrSize,
                                .offset                     = 0u,
                                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
                            };

                            dbAttrValInfo.handleValuePair.value.val = (uint8_t*)&refTime;

                            if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
                            {
                                /* Indicate success */
                                apiResult = CY_BLE_SUCCESS;
                            }
                        }
                        else
                        {
                            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
                        }
                    }
                }
                break;

            default:
                /* Characteristic wasn't found */
                break;
        }
    }

    /* Return status */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CTSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets a characteristic value of the Current Time Service, which is identified
*  by charIndex.
*
*  \param charIndex: The index of a Current Time Service characteristic.
*  \param attrSize: The size of the Current Time Service characteristic value attribute.
*  \param attrValue: The pointer to the location where characteristic value data
*             should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic value was read successfully
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CTSS_GetCharacteristicValue(cy_en_ble_cts_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
    {
        .handleValuePair.value.len = attrSize,
        .handleValuePair.value.val = attrValue,
        .flags                     = CY_BLE_GATT_DB_LOCALLY_INITIATED
    };

    if(attrValue != NULL)
    {
        switch(charIndex)
        {
            case CY_BLE_CTS_CURRENT_TIME:
                if(attrSize == CY_BLE_CTS_CURRENT_TIME_SIZE)
                {
                    dbAttrValInfo.handleValuePair.attrHandle = cy_ble_ctsConfigPtr->ctss->currTimeCharHandle;
                    apiResult = CY_BLE_SUCCESS;
                }
                break;

            case CY_BLE_CTS_LOCAL_TIME_INFO:
                if(attrSize == CY_BLE_CTS_LOCAL_TIME_INFO_SIZE)
                {
                    dbAttrValInfo.handleValuePair.attrHandle = cy_ble_ctsConfigPtr->ctss->localTimeInfCharHandle;
                    apiResult = CY_BLE_SUCCESS;
                }
                break;

            case CY_BLE_CTS_REFERENCE_TIME_INFO:
                if(attrSize == CY_BLE_CTS_REFERENCE_TIME_INFO_SIZE)
                {
                    dbAttrValInfo.handleValuePair.attrHandle = cy_ble_ctsConfigPtr->ctss->refTimeInfCharHandle;
                    apiResult = CY_BLE_SUCCESS;
                }
                break;

            default:
                /* Characteristic wasn't found */
                break;
        }

        if(apiResult != CY_BLE_ERROR_INVALID_PARAMETER)
        {
            if(dbAttrValInfo.handleValuePair.attrHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                /* Get characteristic value from GATT database */
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
    }

    /* Return status */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CTSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets a characteristic descriptor of a specified characteristic of the Current
*  Time Service.
*
*  \param connHandle: The connection handle
*  \param charIndex: The index of the characteristic.
*  \param descrIndex: The index of the descriptor.
*  \param attrSize: The size of the descriptor value.
*  \param attrValue: The pointer to the location where characteristic descriptor value
*             data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional descriptor is absent
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CTSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_cts_char_index_t charIndex,
                                                               cy_en_ble_cts_char_descriptors_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex == CY_BLE_CTS_CURRENT_TIME) && (descrIndex == CY_BLE_CTS_CURRENT_TIME_CCCD))
    {
        if(cy_ble_ctsConfigPtr->ctss->currTimeCccdHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            /* Get characteristic value from GATT database */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_ctsConfigPtr->ctss->currTimeCccdHandle,
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .connHandle                 = connHandle,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };
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
* Function Name: Cy_BLE_CTSS_SendNotification
***************************************************************************//**
*
*  Sends a notification to the client's device. A characteristic value also gets
*  written to the GATT database.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_CTSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a service characteristic to be send as a notification
*             to the Client device.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*             sent to the Client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The characteristic notification was sent successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this.
*                                      characteristic.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the client.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CTSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_cts_char_index_t charIndex,
                                                    uint8_t attrSize,
                                                    uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    if(CY_BLE_CTS_CURRENT_TIME != charIndex)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else
    {
        apiResult = Cy_BLE_CTSS_SetCharacteristicValue(charIndex, attrSize, attrValue);
        if(apiResult == CY_BLE_SUCCESS)
        {
            if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
            {
                apiResult = CY_BLE_ERROR_INVALID_STATE;
            }
            else if((cy_ble_ctsConfigPtr->ctss->currTimeCccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE) ||
                    (!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_ctsConfigPtr->ctss->currTimeCccdHandle)))
            {
                apiResult = CY_BLE_ERROR_NTF_DISABLED;
            }
            else
            {
                /* Fill all fields of write request structure ... */
                cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
                {
                    .handleValPair.attrHandle = cy_ble_ctsConfigPtr->ctss->currTimeCharHandle,
                    .handleValPair.value.val  = attrValue,
                    .handleValPair.value.len  = attrSize,
                    .connHandle               = connHandle
                };
                /* Send notification to client using previously filled structure */
                apiResult = Cy_BLE_GATTS_Notification(&ntfReqParam);
            }
        }
    }

    /* Return status */
    return(apiResult);
}

#endif /* (CY_BLE_CTS_SERVER) */


#ifdef CY_BLE_CTS_CLIENT

/******************************************************************************
* Function Name: Cy_BLE_CTSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_CTSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* CTS characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_ctsCharUuid[CY_BLE_CTS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_CURRENT_TIME,
        CY_BLE_UUID_CHAR_LOCAL_TIME_INFO,
        CY_BLE_UUID_CHAR_REF_TIME_INFO
    };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_CTS))
    {
        for(i = 0u; i < (uint32_t)CY_BLE_CTS_CHAR_COUNT; i++)
        {
            if(cy_ble_ctsCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                if(cy_ble_ctsc[discIdx].currTimeCharacteristics[i].valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_ctsc[discIdx].currTimeCharacteristics[i].valueHandle = discCharInfo->valueHandle;
                    cy_ble_ctsc[discIdx].currTimeCharacteristics[i].properties = discCharInfo->properties;
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
* Function Name: Cy_BLE_CTSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_FIND_INFO_RSP event.
*  This event is generated when a server successfully sends the data for
*  CY_BLE_EVT_GATTC_FIND_INFO_REQ" Based on the service UUID, an appropriate data
*  structure is populated to the service with a service callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_CTSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t * discDescrInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_CTS)
    {
        if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_CLIENT_CONFIG)
        {
            if(cy_ble_ctsc[discIdx].currTimeCccdHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_ctsc[discIdx].currTimeCccdHandle = discDescrInfo->descrHandle;
            }
            else    /* Duplication of descriptor */
            {
                Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_DESCR_DUPLICATION, &discDescrInfo->uuid);
            }
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CTSC_GetCharRange
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
static void Cy_BLE_CTSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_CTS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        if(cy_ble_discovery[discIdx].charCount == 0u)
        {
            /* Read characteristic range */
            charRangeInfo->range.startHandle = cy_ble_ctsc[discIdx].currTimeCharacteristics[CY_BLE_CTS_CURRENT_TIME].
                                                valueHandle + 1u;
            charRangeInfo->range.endHandle = cy_ble_serverInfo[discIdx][CY_BLE_SRVI_CTS].range.endHandle;
        }
        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CTSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles the Notification Event.
*
*  \param eventParam: The pointer to the cy_stc_ble_gattc_handle_value_ntf_param_t data
*              structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_CTSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_CTS_ApplCallback != NULL))
    {
        if(cy_ble_ctsc[discIdx].currTimeCharacteristics[CY_BLE_CTS_CURRENT_TIME].valueHandle ==
           eventParam->handleValPair.attrHandle)
        {
            cy_stc_ble_cts_char_value_t ntfParam =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_CTS_CURRENT_TIME,
                .value      = &eventParam->handleValPair.value
            };
            Cy_BLE_CTS_ApplCallback((uint32_t)CY_BLE_EVT_CTSC_NOTIFICATION, (void*)&ntfParam);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CTSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles the Read Response Event for the Current Time Service.
*
*  \param eventParam: The pointer to the data that came with a read response for CTS.
*
******************************************************************************/
static void Cy_BLE_CTSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t fFlag = 1u;
    uint32_t attrVal = 0u;
    cy_en_ble_cts_char_index_t idx = CY_BLE_CTS_CURRENT_TIME;

    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_CTS_ApplCallback != NULL) &&
       (cy_ble_ctscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_ctsc[discIdx].currTimeCharacteristics[CY_BLE_CTS_CURRENT_TIME].valueHandle ==
           cy_ble_ctscReqHandle[discIdx])
        {
            idx = CY_BLE_CTS_CURRENT_TIME;
        }
        else
            if(cy_ble_ctsc[discIdx].currTimeCharacteristics[CY_BLE_CTS_LOCAL_TIME_INFO].valueHandle ==
               cy_ble_ctscReqHandle[discIdx])
            {
                idx = CY_BLE_CTS_LOCAL_TIME_INFO;
            }
            else
                if(cy_ble_ctsc[discIdx].currTimeCharacteristics[CY_BLE_CTS_REFERENCE_TIME_INFO].valueHandle ==
                   cy_ble_ctscReqHandle[discIdx])
                {
                    idx = CY_BLE_CTS_REFERENCE_TIME_INFO;
                }
                else if(cy_ble_ctsc[discIdx].currTimeCccdHandle == cy_ble_ctscReqHandle[discIdx])
                {
                    /* Attribute is Characteristic Descriptor  */
                    attrVal = 1u;
                }
                else
                {
                    /* No CTS characteristics were requested for read */
                    fFlag = 0u;
                }

        if(fFlag != 0u)
        {
            /* Read response for characteristic */
            if(attrVal == 0u)
            {
                /* Fill Current Time Service read response parameter structure with
                 * characteristic info. */
                cy_stc_ble_cts_char_value_t rdRspParam = { .connHandle = eventParam->connHandle };
                rdRspParam.charIndex = idx;
                rdRspParam.value = &eventParam->value;

                Cy_BLE_CTS_ApplCallback((uint32_t)CY_BLE_EVT_CTSC_READ_CHAR_RESPONSE, (void*)&rdRspParam);
            }
            else /* Read response for characteristic descriptor */
            {
                /* Fill Current Time Service read response parameter structure with
                 *  characteristic descriptor info. */
                cy_stc_ble_cts_descr_value_t rdRspParam =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = CY_BLE_CTS_CURRENT_TIME,
                    .descrIndex = CY_BLE_CTS_CURRENT_TIME_CCCD,
                    .value      = &eventParam->value
                };
                Cy_BLE_CTS_ApplCallback((uint32_t)CY_BLE_EVT_CTSC_READ_DESCR_RESPONSE, (void*)&rdRspParam);
            }

            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            cy_ble_ctscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CTSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles the Write Response Event for the Current Time Service.
*
*  \param eventParam: The pointer to the cy_stc_ble_conn_handle_t data structure.
*
******************************************************************************/
static void Cy_BLE_CTSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_CTS_ApplCallback != NULL) &&
       (cy_ble_ctscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        if(cy_ble_ctsc[discIdx].currTimeCccdHandle == cy_ble_ctscReqHandle[discIdx])
        {
            cy_stc_ble_cts_descr_value_t wrRspParam =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_CTS_CURRENT_TIME,
                .descrIndex = CY_BLE_CTS_CURRENT_TIME_CCCD,
                .value      = NULL
            };
            cy_ble_ctscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_CTS_ApplCallback((uint32_t)CY_BLE_EVT_CTSC_WRITE_DESCR_RESPONSE, (void*)&wrRspParam);
        }
        else if((cy_ble_ctsc[discIdx].currTimeCharacteristics[CY_BLE_CTS_CURRENT_TIME].valueHandle ==
                 cy_ble_ctscReqHandle[discIdx]) ||
                (cy_ble_ctsc[discIdx].currTimeCharacteristics[CY_BLE_CTS_LOCAL_TIME_INFO].valueHandle ==
                 cy_ble_ctscReqHandle[discIdx]))
        {
            cy_stc_ble_cts_char_value_t wrRspParam;
            if(cy_ble_ctsc[discIdx].currTimeCharacteristics[CY_BLE_CTS_CURRENT_TIME].valueHandle ==
               cy_ble_ctscReqHandle[discIdx])
            {
                wrRspParam.charIndex = CY_BLE_CTS_CURRENT_TIME;
            }
            else
            {
                wrRspParam.charIndex = CY_BLE_CTS_LOCAL_TIME_INFO;
            }
            wrRspParam.connHandle = *eventParam;
            wrRspParam.value = NULL;

            cy_ble_ctscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            Cy_BLE_CTS_ApplCallback((uint32_t)CY_BLE_EVT_CTSC_WRITE_CHAR_RESPONSE, (void*)&wrRspParam);
        }
        else
        {
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CTSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles the Error Response Event for the Current Time Service.
*
*  \param eventParam: The pointer to the cy_stc_ble_gatt_err_param_t structure.
*
******************************************************************************/
static void Cy_BLE_CTSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
        if(eventParam->errInfo.attrHandle == cy_ble_ctscReqHandle[discIdx])
        {
            cy_ble_ctscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}

/******************************************************************************
* Function Name: Cy_BLE_CTSC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_CTSS_WRITE_CHAR events is generated.
*  On successful request execution on the Server side the Write Response is
*  sent to the Client.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic descriptor attribute.
*  \param attrValue: Pointer to the characteristic value data that should be
*                    sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request was sent successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                     characteristic.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Peer device doesn't have a
*                                               particular characteristic.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the CTS service-specific callback is registered
*      (with Cy_BLE_CTS_RegisterAttrCallback):
*  * CY_BLE_EVT_CTSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_cts_char_value_t.
*  .
*   Otherwise (if the CTS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_CTSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_cts_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    cy_stc_ble_gattc_write_req_t writeReqParam =
    {
        .handleValPair.value.val = attrValue,
        .handleValPair.value.len = attrSize,
        .connHandle              = connHandle
    };

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
            case CY_BLE_CTS_CURRENT_TIME:
                writeReqParam.handleValPair.attrHandle = cy_ble_ctsc[discIdx].
                                                          currTimeCharacteristics[CY_BLE_CTS_CURRENT_TIME].valueHandle;
                break;

            case CY_BLE_CTS_LOCAL_TIME_INFO:
                writeReqParam.handleValPair.attrHandle = cy_ble_ctsc[discIdx].
                                                          currTimeCharacteristics[CY_BLE_CTS_LOCAL_TIME_INFO].valueHandle;
                break;

            case CY_BLE_CTS_REFERENCE_TIME_INFO:
                /* Reference Time Information is read only */
                apiResult = CY_BLE_ERROR_INVALID_OPERATION;
                break;

            default:
                /* Characteristic wasn't found */
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
                break;
        }

        if(apiResult == CY_BLE_SUCCESS)
        {
            if(writeReqParam.handleValPair.attrHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                /* Send request to read characteristic value */
                apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);

                if(apiResult == CY_BLE_SUCCESS)
                {
                    cy_ble_ctscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
                }
            }
            else
            {
                apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
            }
        }
    }

    /* Return status */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CTSC_GetCharacteristicValue
***************************************************************************//**
*
*  Gets a characteristic value of the Current Time Service, which is identified
*  by charIndex.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of a service characteristic.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request was sent successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established.
*   * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                      characteristic.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Peer device doesn't have a
*                                                particular characteristic.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the CTS service-specific callback is registered
*      (with Cy_BLE_CTS_RegisterAttrCallback):
*  * CY_BLE_EVT_CTSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_cts_char_value_t.
*  .
*   Otherwise (if the CTS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_CTSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_cts_char_index_t charIndex)
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
            case CY_BLE_CTS_CURRENT_TIME:
                readReqParam.attrHandle = cy_ble_ctsc[discIdx].currTimeCharacteristics[CY_BLE_CTS_CURRENT_TIME].valueHandle;
                break;

            case CY_BLE_CTS_LOCAL_TIME_INFO:
                readReqParam.attrHandle = cy_ble_ctsc[discIdx].currTimeCharacteristics[CY_BLE_CTS_LOCAL_TIME_INFO].
                                           valueHandle;
                break;

            case CY_BLE_CTS_REFERENCE_TIME_INFO:
                readReqParam.attrHandle = cy_ble_ctsc[discIdx].currTimeCharacteristics[CY_BLE_CTS_REFERENCE_TIME_INFO].
                                           valueHandle;
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
                /* Send request to read characteristic value */
                apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);

                if(apiResult == CY_BLE_SUCCESS)
                {
                    cy_ble_ctscReqHandle[discIdx] = readReqParam.attrHandle;
                }
            }
            else
            {
                apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
            }
        }
    }

    /* Return status */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CTSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sets a characteristic descriptor of the Current Time Characteristic of the
*  Current Time Service.
*
*  Internally, Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server side the following events can be
*  generated:
*  * CY_BLE_EVT_CTSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_CTSS_NOTIFICATION_DISABLED
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the Current Time Service characteristic.
*  \param descrIndex: The index of the Current Time Service characteristic descriptor.
*  \param attrSize: The size of the characteristic descriptor attribute.
*  \param attrValue: Pointer to the characteristic descriptor value data that should be
*             sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The  request was sent successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established.
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                      specified attribute.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Peer device doesn't have a
*                                                particular descriptor.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the CTS service-specific callback is registered
*      (with Cy_BLE_CTS_RegisterAttrCallback):
*  * CY_BLE_EVT_CTSC_WRITE_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_cts_descr_value_t.
*  .
*   Otherwise (if the CTS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CTSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_cts_char_index_t charIndex,
                                                               cy_en_ble_cts_char_descriptors_t descrIndex,
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
            (charIndex == CY_BLE_CTS_CURRENT_TIME) && (descrIndex == CY_BLE_CTS_CURRENT_TIME_CCCD) && 
            (attrSize == CY_BLE_CCCD_LEN))
    {
        if(cy_ble_ctsc[discIdx].currTimeCccdHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            /* Fill all fields of write request structure ... */
            cy_stc_ble_gattc_write_req_t writeReqParam =
            {
                .handleValPair.value.val  = attrValue,
                .handleValPair.value.len  = attrSize,
                .handleValPair.attrHandle = cy_ble_ctsc[discIdx].currTimeCccdHandle,
                .connHandle               = connHandle
            };
            /* ... and send request to server device. */
            apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_ctscReqHandle[discIdx] = cy_ble_ctsc[discIdx].currTimeCccdHandle;
            }
        }
        else
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
    }
    else
    {
        /* Parameter validation failed */
    }

    /* Return the status */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CTSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets a characteristic descriptor of the Current Time Characteristic of the
*  Current Time Service.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param descrIndex: The index of a service characteristic descriptor.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request was sent successfully.
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*   * CY_BLE_ERROR_INVALID_STATE - State is not valid.
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                      specified attribute.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Peer device doesn't have a
*                                                particular descriptor.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the CTS service-specific callback is registered
*      (with Cy_BLE_CTS_RegisterAttrCallback):
*  * CY_BLE_EVT_CTSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_cts_descr_value_t.
*  .
*  Otherwise (if the CTS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_CTSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_cts_char_index_t charIndex,
                                                               uint8_t descrIndex)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (charIndex == CY_BLE_CTS_CURRENT_TIME) &&
            (descrIndex == ((uint8_t)CY_BLE_CTS_CURRENT_TIME_CCCD)))
    {
        if(cy_ble_ctsc[discIdx].currTimeCccdHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            cy_stc_ble_gattc_read_req_t readReqParam =
            {
                .attrHandle = cy_ble_ctsc[discIdx].currTimeCccdHandle,
                .connHandle = connHandle
            };
            apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);

            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_ctscReqHandle[discIdx] = cy_ble_ctsc[discIdx].currTimeCccdHandle;
            }
        }
        else
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
    }
    else
    {
        /* Validation of input parameters failed */
    }

    /* Return status */
    return(apiResult);
}

#endif /* (CY_BLE_CTS_CLIENT) */

/******************************************************************************
* Function Name: Cy_BLE_CTS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Current Time Service Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_CTS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_CTS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_CTSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_CTSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_CTSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_CTS_CLIENT */

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
        #ifdef CY_BLE_CTS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_CTSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;
        #endif /* CY_BLE_CTS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_CTS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_CTSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_CTSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_CTSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_CTSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_CTS_CLIENT */
            default:
                break;
        }
    }

    return(gattErr);
}
#endif /* defined(CY_BLE_CTS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
