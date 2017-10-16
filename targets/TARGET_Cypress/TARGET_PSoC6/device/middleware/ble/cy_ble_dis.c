/***************************************************************************//**
* \file cy_ble_dis.c
* \version 2.0
*
* \brief
*  Contains the source code for the Device Information Service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_DIS))

#ifdef CY_BLE_DIS_CLIENT
/* DIS Center Service characteristics GATT DB handles structure */
cy_stc_ble_disc_t cy_ble_disc[CY_BLE_CONFIG_GATTC_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_discReqHandle[CY_BLE_CONFIG_GATTC_COUNT];
static cy_ble_callback_t Cy_BLE_DIS_ApplCallback = NULL;
#endif /* (CY_BLE_DIS_CLIENT) */


/* The pointer on global BLE DIS Config structure */
cy_stc_ble_dis_config_t *cy_ble_disConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_DIS_CLIENT
static void Cy_BLE_DISC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_DISC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_DISC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* (CY_BLE_DIS_CLIENT) */
static cy_en_ble_gatt_err_code_t Cy_BLE_DIS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_DIS_Init
***************************************************************************//**
*
*  This function initializes the DIS service.
*
*  \param config: Configuration structure for the DIS service.
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
cy_en_ble_api_result_t Cy_BLE_DIS_Init(cy_stc_ble_dis_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_disConfigPtr = config;

        /* Registers Event Handler for the DIS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_DIS_EventHandler);


    #ifdef CY_BLE_DIS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_DIS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_disc[idx], 0, sizeof(cy_stc_ble_disc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_DIS].uuid = CY_BLE_UUID_DEVICE_INFO_SERVICE;
                }
                cy_ble_discReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
            /* Registers a callback function via config structure*/
            if(cy_ble_disConfigPtr->callbackFunc != NULL)
            {
                Cy_BLE_DIS_ApplCallback = cy_ble_disConfigPtr->callbackFunc;
            }
        }
    #endif /* CY_BLE_DIS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_DIS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Callback doesn't have events in server role.
*
*  \param callbackFunc: An application layer event callback function to receive
*                    events from the BLE Component. The definition of
*                    cy_ble_callback_t for Device Information Service is:\n
*                    typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                       void *eventParam)
*                    * eventCode indicates the event that triggered this
*                      callback.
*                    * eventParam contains the parameters corresponding to the
*                      current event.
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
cy_en_ble_api_result_t Cy_BLE_DIS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{ 
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
#ifdef CY_BLE_DIS_CLIENT
    Cy_BLE_DIS_ApplCallback = callbackFunc;
    if(cy_ble_disConfigPtr != NULL)
    {
        cy_ble_disConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
#else
    if(callbackFunc != NULL) /* Callback doesn't have events in server role */
    {
    }
#endif /* CY_BLE_DIS_CLIENT */  
    return(apiResult);  
}

#ifdef CY_BLE_DIS_SERVER


/******************************************************************************
* Function Name: Cy_BLE_DISS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets a characteristic value of the service, which is identified by charIndex,
*  to the local database.
*
*
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param *attrValue: The pointer to the characteristic value data that should be
*                   stored to the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_DISS_SetCharacteristicValue(cy_en_ble_dis_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if(charIndex >= CY_BLE_DIS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Store data in database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_disConfigPtr->diss->charHandle[charIndex],
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
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
* Function Name: Cy_BLE_DISS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets a characteristic value of the service, which is identified by charIndex,
*  from the GATT database.
*
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param *attrValue: The pointer to the location where characteristic value data
*               should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_DISS_GetCharacteristicValue(cy_en_ble_dis_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if(charIndex >= CY_BLE_DIS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Read characteristic value from database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_disConfigPtr->diss->charHandle[charIndex],
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

#endif /* CY_BLE_DIS_SERVER */

#ifdef CY_BLE_DIS_CLIENT


/******************************************************************************
* Function Name: Cy_BLE_DISC_HandleDiscoverCharacteristics
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP event.
*  Based on the service UUID, an appropriate data structure is populated using the
*  data received as part of the callback.
*
*  \param *discCharInfo:  the pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_DISC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_DIS))
    {
        switch(discCharInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_MANUFACTURER_NAME:
                Cy_BLE_DISC_CheckCharHandle(cy_ble_disc[discIdx].charInfo[CY_BLE_DIS_MANUFACTURER_NAME]);
                break;

            case CY_BLE_UUID_CHAR_MODEL_NUMBER:
                Cy_BLE_DISC_CheckCharHandle(cy_ble_disc[discIdx].charInfo[CY_BLE_DIS_MODEL_NUMBER]);
                break;

            case CY_BLE_UUID_CHAR_SERIAL_NUMBER:
                Cy_BLE_DISC_CheckCharHandle(cy_ble_disc[discIdx].charInfo[CY_BLE_DIS_SERIAL_NUMBER]);
                break;

            case CY_BLE_UUID_CHAR_HARDWARE_REV:
                Cy_BLE_DISC_CheckCharHandle(cy_ble_disc[discIdx].charInfo[CY_BLE_DIS_HARDWARE_REV]);
                break;

            case CY_BLE_UUID_CHAR_FIRMWARE_REV:
                Cy_BLE_DISC_CheckCharHandle(cy_ble_disc[discIdx].charInfo[CY_BLE_DIS_FIRMWARE_REV]);
                break;

            case CY_BLE_UUID_CHAR_SOFTWARE_REV:
                Cy_BLE_DISC_CheckCharHandle(cy_ble_disc[discIdx].charInfo[CY_BLE_DIS_SOFTWARE_REV]);
                break;

            case CY_BLE_UUID_CHAR_SYSTEM_ID:
                Cy_BLE_DISC_CheckCharHandle(cy_ble_disc[discIdx].charInfo[CY_BLE_DIS_SYSTEM_ID]);
                break;

            case CY_BLE_UUID_CHAR_REG_CERT_DATA:
                Cy_BLE_DISC_CheckCharHandle(cy_ble_disc[discIdx].charInfo[CY_BLE_DIS_REG_CERT_DATA]);
                break;

            case CY_BLE_UUID_CHAR_PNP_ID:
                Cy_BLE_DISC_CheckCharHandle(cy_ble_disc[discIdx].charInfo[CY_BLE_DIS_PNP_ID]);
                break;

            default:
                break;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_DISC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read the characteristic Value from a server
*  which is identified by charIndex.
*
*  The Read Response returns the characteristic value in the Attribute Value
*  parameter. The Read Response only contains the characteristic value that is
*  less than or equal to (MTU - 1) octets in length. If the characteristic value
*  is greater than (MTU - 1) octets in length, a Read Long Characteristic Value
*  procedure may be used if the rest of the characteristic value is required.
*
*  This function call can result in generation of the following events based on
*  the response from the server device.
*
*  * CY_BLE_EVT_DISC_READ_CHAR_RESPONSE
*  * CY_BLE_EVT_GATTC_ERROR_RSP
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The read request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                     characteristic
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the DIS service-specific callback is registered
*      (with Cy_BLE_DIS_RegisterAttrCallback):
*  * CY_BLE_EVT_DISC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_dis_char_value_t.
*  .
*   Otherwise (if the DIS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_DISC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_dis_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if((charIndex >= CY_BLE_DIS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_disc[discIdx].charInfo[charIndex].valueHandle,
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_discReqHandle[discIdx] = cy_ble_disc[discIdx].charInfo[charIndex].valueHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_DISC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles the Read Response Event.
*
*  \param *eventParam: the pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_DISC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    cy_en_ble_dis_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_DIS_ApplCallback != NULL) &&
       (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_discReqHandle[discIdx]))
    {
        for(locCharIndex = CY_BLE_DIS_MANUFACTURER_NAME; locCharIndex < CY_BLE_DIS_CHAR_COUNT; locCharIndex++)
        {
            if(cy_ble_discReqHandle[discIdx] == cy_ble_disc[discIdx].charInfo[locCharIndex].valueHandle)
            {
                cy_stc_ble_dis_char_value_t locCharValue =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = locCharIndex,
                    .value      = &eventParam->value
                };

                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                cy_ble_discReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_DIS_ApplCallback((uint32_t)CY_BLE_EVT_DISC_READ_CHAR_RESPONSE, &locCharValue);
                break;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_DISC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles the Error Response Event.
*
*  \param *eventParam: the pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_DISC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        if(cy_ble_discReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] == eventParam->errInfo.attrHandle)
        {
            cy_ble_discReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}

#endif /* (CY_BLE_DIS_CLIENT) */

/******************************************************************************
* Function Name: Cy_BLE_DIS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Device Information Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_DIS_EventHandler(uint32_t eventCode,
                                                         void *eventParam)
{
    if(eventCode > (uint32_t)CY_BLE_EVT_MAX)
    {
        /**************************************************************************
         * Handling Service Specific Events
         ***************************************************************************/
        switch((cy_en_ble_evt_t)eventCode)
        {
        #ifdef CY_BLE_DIS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                (void)Cy_BLE_DISC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;
        #endif /* CY_BLE_DIS_CLIENT */

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
        #ifdef CY_BLE_DIS_CLIENT
            case CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP:
                /* implementation in cy_ble_event_handler.c */
                break;

            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode != CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) != 0u)
                    {
                        Cy_BLE_DISC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                    }
                }
            }
            break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) != 0u)
                {
                    Cy_BLE_DISC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                }
                break;
        #endif /* CY_BLE_DIS_CLIENT */
            default:
                break;
        }
    }

    return(CY_BLE_GATT_ERR_NONE);
}

#endif /* defined(CY_BLE_DIS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */

