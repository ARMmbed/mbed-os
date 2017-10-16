/***************************************************************************//**
* \file cy_ble_custom.c
* \version 2.0
*
* \brief
*  Contains the source code for the Custom Service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_CUSTOM))

/* The pointer on global BLE CUSTOM Config structure */
cy_stc_ble_custom_config_t *cy_ble_customConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_CUSTOM_CLIENT
static void Cy_BLE_CUSTOMC_DiscoverServiceEventHandler(const cy_stc_ble_disc_srv_info_t *discServInfo);
static void Cy_BLE_CUSTOMC_DiscoverCharacteristicsEventHandler(const cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_CUSTOMC_DiscoverCharDescriptorsEventHandler(const cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_CUSTOMC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
#endif /* (CY_BLE_CUSTOM_CLIENT) */
static cy_en_ble_gatt_err_code_t Cy_BLE_CUSTOM_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_CUSTOM_Init
***************************************************************************//**
*
*  This function initializes a Custom Service.
*
*  \param config: Configuration structure for the Custom Service.
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
cy_en_ble_api_result_t Cy_BLE_CUSTOM_Init(cy_stc_ble_custom_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_customConfigPtr = config;

        /* Registers Event Handler for the Custom Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_CUSTOM_EventHandler);

    #ifdef CY_BLE_CUSTOM_CLIENT
        {
            uint32_t discIdx;
            uint32_t locServIdx;
            uint32_t locCharIdx;
            uint32_t locDescIdx;
            for(discIdx = 0u; discIdx < CY_BLE_CONFIG_GATTC_COUNT; discIdx++)
            {
                for(locServIdx = 0u; locServIdx < CY_BLE_CUSTOMC_SERVICE_COUNT; locServIdx++)
                {
                    if(cy_ble_serverInfo[discIdx][CY_BLE_SRVI_CUSTOMS + locServIdx].range.startHandle ==
                       CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                    {
                        for(locCharIdx = 0u; locCharIdx < cy_ble_customConfigPtr->customc[locServIdx].charCount;
                            locCharIdx++)
                        {
                            cy_ble_customConfigPtr->customc[locServIdx].customServChar[locCharIdx].
                             customServCharHandle[discIdx] = 0u;

                            for(locDescIdx = 0u; locDescIdx < cy_ble_customConfigPtr->customc[locServIdx].
                                 customServChar[locCharIdx].descCount; locDescIdx++)
                            {
                                cy_ble_customConfigPtr->customc[locServIdx].customServChar[locCharIdx].
                                 customServCharDesc[locDescIdx].descHandle[discIdx] = 0u;
                            }

                            /* initialize uuid  */
                            cy_ble_serverInfo[discIdx][CY_BLE_SRVI_CUSTOMS + locServIdx].uuid = 0x0000u;
                        }
                    }
                }
            }
        }
    #endif /* (CY_BLE_CUSTOM_CLIENT) */
    }

    return(apiResult);
}


#ifdef CY_BLE_CUSTOM_CLIENT


/******************************************************************************
* Function Name: Cy_BLE_CUSTOMC_DiscoverServiceEventHandler
***************************************************************************//**
*
*  This function is called on receiving a Read By Group Response event or
*  Read response with 128-bit service uuid.
*
*  \param connHandle: The connection handle.
*  \param discServInfo: The pointer to a service information structure.
*
* \return
*  None
*
******************************************************************************/
static void Cy_BLE_CUSTOMC_DiscoverServiceEventHandler(const cy_stc_ble_disc_srv_info_t *discServInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discServInfo->connHandle);
    uint32_t fFlag = 0u;
    uint32_t j;

    /* Services with 128 bit UUID have discServInfo->uuid equal to 0 and address to
     * 128 uuid is stored in cy_ble_customCServ.uuid128
     */
    for(j = 0u; (j < CY_BLE_CUSTOMC_SERVICE_COUNT) && (fFlag == 0u); j++)
    {
        if(cy_ble_customConfigPtr->customc[j].uuidFormat == CY_BLE_GATT_128_BIT_UUID_FORMAT)
        {
            if(memcmp(cy_ble_customConfigPtr->customc[j].uuid, &discServInfo->srvcInfo->uuid.uuid128,
                      CY_BLE_GATT_128_BIT_UUID_SIZE) == 0u)
            {
                if(cy_ble_serverInfo[discIdx][j + (uint32_t)CY_BLE_SRVI_CUSTOMS].range.startHandle ==
                   CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cy_ble_serverInfo[discIdx][j + (uint32_t)CY_BLE_SRVI_CUSTOMS].range = discServInfo->srvcInfo->range;
                    cy_ble_discovery[discIdx].servCount++;
                    fFlag = 1u;
                }

                /* Indicate that request was handled */
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CUSTOMC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service index, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
* \return
*  None
*
******************************************************************************/
static void Cy_BLE_CUSTOMC_DiscoverCharacteristicsEventHandler(const cy_stc_ble_disc_char_info_t *discCharInfo)
{
    static cy_ble_gatt_db_attr_handle_t *customsLastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    static uint32_t discoveryLastServ[CY_BLE_CONFIG_GATTC_COUNT] = { 0u };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t servIdx = cy_ble_discovery[discIdx].servCount - (uint32_t)CY_BLE_SRVI_CUSTOMS;
    uint32_t locReqHandle = 0u;
    uint32_t locCharIndex;

    if((cy_ble_discovery[discIdx].servCount >= (uint32_t)CY_BLE_SRVI_CUSTOMS) &&
       (cy_ble_discovery[discIdx].servCount <= (uint32_t)CY_BLE_SRVI_CUSTOMS_END))
    {
        /* Update last characteristic endHandle to declaration handle of this characteristic */
        if(customsLastEndHandle[discIdx] != NULL)
        {
            if(discoveryLastServ[discIdx] == servIdx)
            {
                *customsLastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            }
            customsLastEndHandle[discIdx] = NULL;
        }

        for(locCharIndex = 0u; (locCharIndex < cy_ble_customConfigPtr->customc[servIdx].charCount) && (locReqHandle == 0u);
            locCharIndex++)
        {
            uint32_t fFlag = 0u;

            /* Support 128 bit uuid */
            if((discCharInfo->uuidFormat == CY_BLE_GATT_128_BIT_UUID_FORMAT) &&
               (cy_ble_customConfigPtr->customc[servIdx].customServChar[locCharIndex].uuidFormat ==
                CY_BLE_GATT_128_BIT_UUID_FORMAT))
            {
                if(memcmp(cy_ble_customConfigPtr->customc[servIdx].customServChar[locCharIndex].uuid,
                          &discCharInfo->uuid.uuid128, CY_BLE_GATT_128_BIT_UUID_SIZE) == 0u)
                {
                    fFlag = 1u;
                }
            }

            /* And support 16 bit uuid */
            if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
               (cy_ble_customConfigPtr->customc[servIdx].customServChar[locCharIndex].uuidFormat ==
                CY_BLE_GATT_16_BIT_UUID_FORMAT))
            {
                if(memcmp(cy_ble_customConfigPtr->customc[servIdx].customServChar[locCharIndex].uuid,
                          &discCharInfo->uuid.uuid16, CY_BLE_GATT_16_BIT_UUID_SIZE) == 0u)
                {
                    fFlag = 1u;
                }
            }

            if((fFlag == 1u) &&
               (cy_ble_customConfigPtr->customc[servIdx].customServChar[locCharIndex].customServCharHandle[discIdx] ==
                CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
            {
                cy_ble_customConfigPtr->customc[servIdx].customServChar[locCharIndex].customServCharHandle[discIdx] =
                    discCharInfo->valueHandle;
                cy_ble_customConfigPtr->customc[servIdx].customServChar[locCharIndex].properties[discIdx] =
                    discCharInfo->properties;

                /* Init pointer to characteristic endHandle */
                customsLastEndHandle[discIdx] = &cy_ble_customConfigPtr->customc[servIdx].customServChar[locCharIndex].
                                                 customServCharEndHandle[discIdx];

                /* Init service index of discovered characteristic */
                discoveryLastServ[discIdx] = servIdx;
                locReqHandle = 1u;
            }
        }

        /* Init characteristic endHandle to Service endHandle.
         * Characteristic endHandle will be updated to the declaration
         * Handler of the following characteristic,
         * in the following characteristic discovery procedure. */
        if(customsLastEndHandle[discIdx] != NULL)
        {
            *customsLastEndHandle[discIdx] = cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range.
                                              endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_GetCustomCharRange
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
static void Cy_BLE_CUSTOMC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if((cy_ble_discovery[discIdx].servCount >= (uint32_t)CY_BLE_SRVI_CUSTOMS) &&
       (cy_ble_discovery[discIdx].servCount <= (uint32_t)CY_BLE_SRVI_CUSTOMS_END))
    {
        uint32_t servIdx = cy_ble_discovery[discIdx].servCount - (uint32_t)CY_BLE_SRVI_CUSTOMS;

        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < cy_ble_customConfigPtr->customc[servIdx].charCount) &&
              (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if(cy_ble_customConfigPtr->customc[servIdx].customServChar[charIdx].descCount > 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_customConfigPtr->customc[servIdx].customServChar[charIdx].
                                                    customServCharHandle[discIdx] + 1u;
                charRangeInfo->range.endHandle = cy_ble_customConfigPtr->customc[servIdx].customServChar[charIdx].
                                                  customServCharEndHandle[discIdx];
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
* Function Name: Cy_BLE_CUSTOMC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_FIND_INFO_RSP event.
*  Based on the descriptor UUID, an appropriate data structure is populated using
*  the data received as part of the callback.
*
*  \param discDescrInfo: The pointer to a descriptor information structure.
*
* \return
*  None
*
******************************************************************************/
static void Cy_BLE_CUSTOMC_DiscoverCharDescriptorsEventHandler(const cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t locDescIndex;
    uint32_t locReqHandle = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);

    if((cy_ble_discovery[discIdx].servCount >= ((uint32_t)CY_BLE_SRVI_CUSTOMS)) &&
       (cy_ble_discovery[discIdx].servCount <= ((uint32_t)CY_BLE_SRVI_CUSTOMS_END)))
    {
        uint32_t servIdx = cy_ble_discovery[discIdx].servCount - (uint32_t)CY_BLE_SRVI_CUSTOMS;
        uint32_t charIdx = cy_ble_discovery[discIdx].charCount;

        for(locDescIndex = 0u; (locDescIndex < cy_ble_customConfigPtr->customc[servIdx].customServChar[charIdx].descCount) &&
            (locReqHandle == 0u); locDescIndex++)
        {
            uint32_t fFlag = 0u;

            if((discDescrInfo->uuidFormat == CY_BLE_GATT_128_BIT_UUID_FORMAT) &&
               (cy_ble_customConfigPtr->customc[servIdx].customServChar[charIdx].customServCharDesc[locDescIndex].
                 uuidFormat == CY_BLE_GATT_128_BIT_UUID_FORMAT))
            {
                if(memcmp(cy_ble_customConfigPtr->customc[servIdx].customServChar[charIdx].
                           customServCharDesc[locDescIndex].uuid, &discDescrInfo->uuid.uuid128,
                          CY_BLE_GATT_128_BIT_UUID_SIZE) == 0u)
                {
                    fFlag = 1u;
                }
            }

            if((discDescrInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
               (cy_ble_customConfigPtr->customc[servIdx].customServChar[charIdx].customServCharDesc[locDescIndex].
                 uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT))
            {
                if(memcmp(cy_ble_customConfigPtr->customc[servIdx].customServChar[charIdx].
                           customServCharDesc[locDescIndex].uuid, &discDescrInfo->uuid.uuid16,
                          CY_BLE_GATT_16_BIT_UUID_SIZE) == 0u)
                {
                    fFlag = 1u;
                }
            }

            if((fFlag == 1u) &&
               (cy_ble_customConfigPtr->customc[servIdx].customServChar[charIdx].customServCharDesc[locDescIndex].
                 descHandle[discIdx] == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
            {
                cy_ble_customConfigPtr->customc[servIdx].customServChar[charIdx].customServCharDesc[locDescIndex].
                 descHandle[discIdx] = discDescrInfo->descrHandle;
                locReqHandle = 1u;
            }
        }
    }
}

#endif /* (CY_BLE_CUSTOM_CLIENT) */

/******************************************************************************
* Function Name: Cy_BLE_CUSTOM_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Custom Service.
*
*  \param eventCode:  The event code
*  \param eventParam:  The event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_CUSTOM_EventHandler(uint32_t eventCode,
                                                            void *eventParam)
{
    if(eventCode > (uint32_t)CY_BLE_EVT_MAX)
    {
        /**************************************************************************
         * Handling Service Specific Events
         ***************************************************************************/
        switch((cy_en_ble_evt_t)eventCode)
        {
        #ifdef CY_BLE_CUSTOM_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_SERVICE:
                Cy_BLE_CUSTOMC_DiscoverServiceEventHandler((cy_stc_ble_disc_srv_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_CUSTOMC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_CUSTOMC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_CUSTOMC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_CUSTOM_CLIENT */

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

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_CUSTOM_CLIENT
            case CY_BLE_EVT_GATTC_READ_RSP:
            case CY_BLE_EVT_GATTC_READ_MULTI_RSP:
            {
                cy_stc_ble_gattc_read_rsp_param_t *eventParamCast = (cy_stc_ble_gattc_read_rsp_param_t*)eventParam;
                cy_stc_ble_conn_handle_t locConnHandle = eventParamCast->connHandle;
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(locConnHandle);

                /* Read response with 128-bit included service uuid */
                if((Cy_BLE_GetConnectionState(locConnHandle) == CY_BLE_CONN_STATE_CLIENT_INCL_DISCOVERING) &&
                   (discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].inclInfo.inclDefHandle != 0u))
                {
                    cy_stc_ble_disc_srvc128_info_t discServ128Info;
                    cy_stc_ble_disc_srv_info_t locDiscServInfo =
                    {
                        .srvcInfo   = &discServ128Info,
                        .connHandle = locConnHandle
                    };

                    /* Store service range */
                    discServ128Info.range = cy_ble_discovery[discIdx].inclInfo.inclHandleRange;
                    (void)memcpy((void*)discServ128Info.uuid.uuid128.value, (void*)eventParamCast->value.val,
                                 (uint32_t)eventParamCast->value.len);
                    Cy_BLE_CUSTOMC_DiscoverServiceEventHandler(&locDiscServInfo);

                    /* Re-initiate Cy_BLE_GATTC_FindIncludedServices function, setting the start
                     * handle to the attribute handle which is placed next to the one used in
                     * the above step.
                     */
                    discServ128Info.range.startHandle = cy_ble_discovery[discIdx].inclInfo.inclDefHandle + 1u;
                    discServ128Info.range.endHandle =
                        cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range.endHandle;
                    if(discServ128Info.range.startHandle <= discServ128Info.range.endHandle)
                    {
                        cy_stc_ble_gattc_read_by_type_req_t requestParam =
                        {
                            .range      = discServ128Info.range,
                            .connHandle = eventParamCast->connHandle
                        };
                        if(Cy_BLE_GATTC_FindIncludedServices(&requestParam) != CY_BLE_SUCCESS)
                        {
                            Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_INCL_DISCOVERY_FAILED,
                                                &eventParamCast->connHandle);
                            cy_ble_discovery[discIdx].autoDiscoveryFlag = 0u;
                        }
                    }
                    else     /* Continue discovery of the following service */
                    {
                        Cy_BLE_NextInclDiscovery(eventParamCast->connHandle, CY_BLE_DISCOVERY_CONTINUE);
                    }

                    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                    cy_ble_discovery[discIdx].inclInfo.inclDefHandle = 0u;
                }
            }
            break;
        #endif /* CY_BLE_CUSTOM_CLIENT */

            default:
                break;
        }
    }

    return(CY_BLE_GATT_ERR_NONE);
}
#endif /* defined(CY_BLE_CUSTOM) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
