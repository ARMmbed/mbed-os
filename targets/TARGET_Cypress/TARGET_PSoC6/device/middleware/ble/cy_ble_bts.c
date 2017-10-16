/***************************************************************************//**
* \file cy_ble_bts.c
* \version 2.0
*
* \brief
*  Contains the source code for the Bootloader service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_ble_event_handler.h"
#include "cy_ble_hal_pvt.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_BTS))
static cy_ble_callback_t Cy_BLE_BTS_ApplCallback;

/* The pointer on global BLE BTS Config structure */
cy_stc_ble_bts_config_t *cy_ble_btsConfigPtr;

/***************************************
* Private Function Prototypes
***************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_BTS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
static void Cy_BLE_BTSS_WriteCmdEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
static void Cy_BLE_BTSS_ExecuteWriteRequestEventHandler(cy_stc_ble_gatts_exec_write_req_t *eventParam);
static void Cy_BLE_BTSS_PrepareWriteRequestEventHandler(cy_stc_ble_gatts_prep_write_req_param_t *eventParam);
static cy_en_ble_gatt_err_code_t Cy_BLE_BTS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_BTS_Init
***************************************************************************//**
*
*  This function initializes the BTS service.
*
*  \param config: Configuration structure for the Bootloader service.
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
cy_en_ble_api_result_t Cy_BLE_BTS_Init(cy_stc_ble_bts_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_btsConfigPtr = config;

        /* Registers a callback function via config structure */
        if(cy_ble_btsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_BTS_ApplCallback = cy_ble_btsConfigPtr->callbackFunc;
        }

        /* Registers Event Handler for the Bootloader Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_BTS_EventHandler);
    }

    return(apiResult);
}

/******************************************************************************
* Function Name: Cy_BLE_BTS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service-specific write requests from a peer device will not be handled with
*  an unregistered callback function.
*
*  \param callbackFunc: An application layer event callback function to receive
*                        events from the BLE Component. The definition of
*                        cy_ble_callback_t for BTS is: \n
*    typedef void (* cy_ble_callback_t) (uint32_t eventCode, void *eventParam),
*    where:
*      * eventCode indicates the event that triggered this callback.
*      * eventParam contains the parameters corresponding to the current event.
*
*  \sideeffect
*  The *eventParams in the callback function should not be used by the
*  application once the callback function execution is finished. Otherwise this
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
cy_en_ble_api_result_t Cy_BLE_BTS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_BTS_ApplCallback = callbackFunc;
    if(cy_ble_btsConfigPtr != NULL)
    {
        cy_ble_btsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_BTS_WriteEventHandler
***************************************************************************//**
*
*  Handles a Write Request event for the Bootloader Service.
*
*  \param cy_stc_ble_gatts_write_cmd_req_param_t * eventParam: The pointer to data that came
*                                              with a Write Request for BTS.
*
* \return
*  Return a value of type cy_en_ble_gatt_err_code_t:
*   * CY_BLE_GATT_ERR_NONE - A function finished successfully.
*   * CY_BLE_GATT_ERR_INVALID_HANDLE - The Handle of the Bootloader
*                                     Characteristic's Client Configuration
*                                     Characteristic Descriptor is not valid.
*   * CY_BLE_GATT_ERR_WRITE_NOT_PERMITTED - Write operation is not permitted on
*                                          this attribute.
*   * CY_BLE_GATT_ERR_UNLIKELY_ERROR - An Internal Stack error occurred.
*   * CY_BLE_GATT_ERR_INVALID_OFFSET - Offset value is invalid.
*   * CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED - The notification property of the
*                                            Bootloader Characteristic's Client
*                                            Configuration Characteristic
*                                            Descriptor is disabled.
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_BTS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
    {
        .handleValuePair = eventParam->handleValPair,
        .connHandle      = eventParam->connHandle,
        .offset          = 0u,
        .flags           = CY_BLE_GATT_DB_PEER_INITIATED
    };

    if(Cy_BLE_BTS_ApplCallback != NULL)
    {
        /* Client Characteristic Configuration descriptor Write Request */
        if(eventParam->handleValPair.attrHandle == cy_ble_btsConfigPtr->btss->btServiceInfo[CY_BLE_BTS_BT_SERVICE].
            btServiceCharDescriptors[CY_BLE_BTS_BT_SERVICE_CCCD])
        {
            /* Verify that notification property is enabled for Bootloader Characteristic */
            if(CY_BLE_IS_NOTIFICATION_SUPPORTED(cy_ble_btsConfigPtr->btss->btServiceInfo[CY_BLE_BTS_BT_SERVICE].
                                                 btServiceCharHandle))
            {
                gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);
                if(gattErr == CY_BLE_GATT_ERR_NONE)
                {
                    if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                    {
                        Cy_BLE_BTS_ApplCallback((uint32_t)CY_BLE_EVT_BTSS_NOTIFICATION_ENABLED, NULL);
                    }
                    else
                    {
                        Cy_BLE_BTS_ApplCallback((uint32_t)CY_BLE_EVT_BTSS_NOTIFICATION_DISABLED, NULL);
                    }
                }
            }
            else
            {
                gattErr = CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED;
            }

            /* Indicate that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else if(eventParam->handleValPair.attrHandle == cy_ble_btsConfigPtr->btss->btServiceInfo[CY_BLE_BTS_BT_SERVICE].
                 btServiceCharHandle)
        {
            cy_stc_ble_bts_char_value_t locCharValue =
            {
                .charIndex  = CY_BLE_BTS_BT_SERVICE,
                .connHandle = eventParam->connHandle,
                .value      = &eventParam->handleValPair.value
            };

            /* Write value to GATT database */
            gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);

            /* Send callback to user */
            Cy_BLE_BTS_ApplCallback((uint32_t)CY_BLE_EVT_BTSS_WRITE_REQ, &locCharValue);

            /* Indicate that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else
        {
        }
    }

    return(gattErr);
}

/******************************************************************************
* Function Name: Cy_BLE_BTSS_WriteCmdEventHandler
***************************************************************************//**
*
*  Handles the Write Without Response Request Event for the Bootloader Service.
*  Service.
*
*  \param cy_stc_ble_gatts_write_cmd_req_param_t * eventParam: The pointer to a data structure
*                                                    specified by the event.
*
* \return
*  Return a value of type cy_en_ble_gatt_err_code_t:
*   * CY_BLE_GATT_ERR_NONE - A function finished successfully.
*   * CY_BLE_GATT_ERR_INVALID_HANDLE - The Handle of the Bootloader
*                                     Characteristic's Client Configuration
*                                     Characteristic Descriptor is not valid.
*   * CY_BLE_GATT_ERR_WRITE_NOT_PERMITTED - Write operation is not permitted on
*                                          this attribute.
*   * CY_BLE_GATT_ERR_UNLIKELY_ERROR - An Internal Stack error occurred.
*   * CY_BLE_GATT_ERR_INVALID_OFFSET - Offset value is invalid.
*   * CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED - The notification property of the
*                                            Bootloader Characteristic's Client
*                                            Configuration Characteristic
*                                            Descriptor is disabled.
******************************************************************************/
static void Cy_BLE_BTSS_WriteCmdEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
    {
        .handleValuePair = eventParam->handleValPair,
        .connHandle      = eventParam->connHandle,
        .offset          = 0u,
        .flags           = CY_BLE_GATT_DB_PEER_INITIATED
    };

    if(Cy_BLE_BTS_ApplCallback != NULL)
    {
        /* Client Characteristic Configuration descriptor Write Request */
        if(eventParam->handleValPair.attrHandle == cy_ble_btsConfigPtr->btss->btServiceInfo[CY_BLE_BTS_BT_SERVICE].
            btServiceCharDescriptors[CY_BLE_BTS_BT_SERVICE_CCCD])
        {
            /* Verify that notification property is enabled for Bootloader Characteristic */
            if(CY_BLE_IS_NOTIFICATION_SUPPORTED(cy_ble_btsConfigPtr->btss->btServiceInfo[CY_BLE_BTS_BT_SERVICE].
                                                 btServiceCharHandle))
            {
                if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
                {
                    if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                    {
                        Cy_BLE_BTS_ApplCallback((uint32_t)CY_BLE_EVT_BTSS_NOTIFICATION_ENABLED, NULL);
                    }
                    else
                    {
                        Cy_BLE_BTS_ApplCallback((uint32_t)CY_BLE_EVT_BTSS_NOTIFICATION_DISABLED, NULL);
                    }
                }
            }

            /* Indicate that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }

        else if(eventParam->handleValPair.attrHandle == cy_ble_btsConfigPtr->btss->btServiceInfo[CY_BLE_BTS_BT_SERVICE].
                 btServiceCharHandle)
        {
            cy_stc_ble_bts_char_value_t locCharValue =
            {
                .charIndex  = CY_BLE_BTS_BT_SERVICE,
                .connHandle = eventParam->connHandle,
                .value      = &eventParam->handleValPair.value
            };

            /* Write value to GATT database */
            (void)Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);

            /* Send callback to user */
            Cy_BLE_BTS_ApplCallback((uint32_t)CY_BLE_EVT_BTSS_WRITE_CMD_REQ, &locCharValue);

            /* Indicate that request was handled */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else
        {
        }
    }
}

/******************************************************************************
* Function Name: Cy_BLE_BTSS_PrepareWriteRequestEventHandler
***************************************************************************//**
*
*  Handles a Prepare Write Request event for the Bootloader Service.
*
*  \param cy_stc_ble_gatts_prep_write_req_param_t *eventParam: The pointer to the data
*                                                  received with a Prepare Write
*                                                  Request event for the
*                                                  Bootloader service.
*
* \return
*  None.
*
******************************************************************************/
static void Cy_BLE_BTSS_PrepareWriteRequestEventHandler(cy_stc_ble_gatts_prep_write_req_param_t *eventParam)
{
    if((Cy_BLE_BTS_ApplCallback != NULL) &&
       (eventParam->baseAddr[0u].handleValuePair.attrHandle == cy_ble_btsConfigPtr->btss->
         btServiceInfo[CY_BLE_BTS_BT_SERVICE].btServiceCharHandle))
    {
        /* Send callback to user */
        Cy_BLE_BTS_ApplCallback((uint32_t)CY_BLE_EVT_BTSS_PREP_WRITE_REQ, eventParam);

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_BTSS_ExecuteWriteRequestEventHandler
***************************************************************************//**
*
*  Handles an Execute Write Request event for the Bootloader Service.
*
*  \param cy_stc_ble_gatts_exec_write_req_t *eventParam: The pointer to the data that came
*                                            with a Write Request for the
*                                            Bootloader service.
*
* \return
*  None.
*
******************************************************************************/
static void Cy_BLE_BTSS_ExecuteWriteRequestEventHandler(cy_stc_ble_gatts_exec_write_req_t *eventParam)
{
    if((Cy_BLE_BTS_ApplCallback != NULL) && (eventParam->baseAddr[0u].handleValuePair.attrHandle ==
                                             cy_ble_btsConfigPtr->btss->btServiceInfo[CY_BLE_BTS_BT_SERVICE].
                                              btServiceCharHandle))
    {
        /* Send callback to user */
        Cy_BLE_BTS_ApplCallback((uint32_t)CY_BLE_EVT_BTSS_EXEC_WRITE_REQ, eventParam);

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}

/******************************************************************************
* Function Name: Cy_BLE_BTSS_SendNotification
***************************************************************************//**
*
*  This function updates the value of the Bootloader service characteristic in the
*  GATT database. If the client has configured a notification on the Battery
*  Level characteristic, the function additionally sends this value using a
*  GATT Notification message.
**
*  \param connHandle: The BLE peer device connection handle
*
*  \param charIndex: The index of a service characteristic of type
*              cy_en_ble_bts_char_index_t.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the Client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the client.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_BTSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_bts_char_index_t charIndex,
                                                    uint32_t attrSize,
                                                    const uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint8_t *btCmdCharacteristicPtr;

    if(charIndex >= CY_BLE_BTS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Store new data in database
         * Get pointer to Bootloader Characteristic value to store response there
         */
        btCmdCharacteristicPtr = CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_PTR(cy_ble_btsConfigPtr->btss->btServiceInfo[charIndex].
                                                                       btServiceCharHandle);

        /* Prepare and send notification with the bootloader packet emulator response if notifications are enabled */
        (void)memcpy((void*)btCmdCharacteristicPtr, (const void*)attrValue, attrSize);


        /* Send Notification if it is enabled and connected */
        if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
        {
            apiResult = CY_BLE_ERROR_INVALID_STATE;
        }
        else if(CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_btsConfigPtr->btss->btServiceInfo[charIndex].
                                                btServiceCharDescriptors[CY_BLE_BTS_BT_SERVICE_CCCD]))
        {
            cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
            {
                /* Fill all fields of write request structure ... */
                .handleValPair.attrHandle = cy_ble_btsConfigPtr->btss->btServiceInfo[charIndex].btServiceCharHandle,
                .handleValPair.value.val  = btCmdCharacteristicPtr,
                .handleValPair.value.len  = attrSize,
                .connHandle               = connHandle
            };

            /* Send notification to client using previously filled structure */
            apiResult = Cy_BLE_GATTS_Notification(&ntfReqParam);
        }
        else
        {
            apiResult = CY_BLE_ERROR_NTF_DISABLED;
        }
    }

    return(apiResult);
}

/******************************************************************************
* Function Name: Cy_BLE_BTS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Bootloader service.
*
*  \param eventCode:   the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_BTS_EventHandler(uint32_t eventCode,
                                                         void *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;

    switch((cy_en_ble_event_t)eventCode)
    {
        case CY_BLE_EVT_GATTS_EXEC_WRITE_REQ:
            Cy_BLE_BTSS_ExecuteWriteRequestEventHandler((cy_stc_ble_gatts_exec_write_req_t*)eventParam);
            break;

        case CY_BLE_EVT_GATTS_PREP_WRITE_REQ:
            if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) != 0u)
            {
                Cy_BLE_BTSS_PrepareWriteRequestEventHandler((cy_stc_ble_gatts_prep_write_req_param_t*)eventParam);
            }
            break;

        case CY_BLE_EVT_GATTS_WRITE_CMD_REQ:
            (void)Cy_BLE_GATTS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
            if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) != 0u)
            {
                Cy_BLE_BTSS_WriteCmdEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
            }
            break;

        case CY_BLE_EVT_GATTS_WRITE_REQ:
            if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) != 0u)
            {
                gattErr = Cy_BLE_BTS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
            }
            break;

        default:
            break;
    }

    return(gattErr);
}
#endif /* defined(CY_BLE_BTS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
