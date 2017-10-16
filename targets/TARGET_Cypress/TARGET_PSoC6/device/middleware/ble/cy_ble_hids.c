/***************************************************************************//**
* \file cy_ble_hids.c
* \version 2.0
*
* \brief
*  Contains the source code for the HID service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_HIDS))

static cy_ble_callback_t Cy_BLE_HIDS_ApplCallback;

#ifdef CY_BLE_HIDS_SERVER
/* Local function prototypes */
static uint32_t Cy_BLE_HIDSS_CccdWriteEventHandler(const cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
#endif /* CY_BLE_HIDS_SERVER */

#ifdef CY_BLE_HIDS_CLIENT
/* HIDS Center Service characteristics GATT DB handles structure */
cy_stc_ble_hidsc_t cy_ble_hidsc[CY_BLE_CONFIG_GATTC_COUNT][CY_BLE_HIDSC_SERVICE_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_hidscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];

uint8_t cy_ble_hidscServiceCount[CY_BLE_CONFIG_GATTC_COUNT];

/* To discovery descriptors for multiple HID service instances */
uint8_t cy_ble_hidscDisServiceIndex[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_HIDS_CLIENT */

/* The pointer on global BLE HIDS Config structure */
cy_stc_ble_hids_config_t *cy_ble_hidsConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_HIDS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_HIDSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
static void Cy_BLE_HIDSS_OnDeviceConnected(void);
#endif /* CY_BLE_HIDS_SERVER */

#ifdef CY_BLE_HIDS_CLIENT
static void Cy_BLE_HIDSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_HIDSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_HIDSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_HIDSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_HIDSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_HIDSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_HIDSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
static void Cy_BLE_HIDSC_InclDiscoveryEventHandler(const cy_stc_ble_disc_incl_info_t *discInclInfo);
#endif /* CY_BLE_HIDS_CLIENT */
static cy_en_ble_gatt_err_code_t Cy_BLE_HIDS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_HIDS_Init
***************************************************************************//**
*
*  This function initializes the HID Service.
*
*  \param config: Configuration structure for the HID Service.
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
cy_en_ble_api_result_t Cy_BLE_HIDS_Init(cy_stc_ble_hids_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_hidsConfigPtr = config;

        /* Registers Event Handler for the HID Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_HIDS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_hidsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_HIDS_ApplCallback = cy_ble_hidsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_HIDS_CLIENT
        {
            uint32_t idx;
            uint32_t locServIndex;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                for(locServIndex = 0u; locServIndex < CY_BLE_HIDSC_SERVICE_COUNT; locServIndex++)
                {
                    if(cy_ble_serverInfo[idx][CY_BLE_SRVI_HIDS + locServIndex].range.startHandle ==
                       CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                    {
                        (void)memset(&cy_ble_hidsc[idx][locServIndex], 0, sizeof(cy_stc_ble_hidsc_t));

                        /* initialize uuid  */
                        cy_ble_serverInfo[idx][CY_BLE_SRVI_HIDS + locServIndex].uuid = CY_BLE_UUID_HIDS_SERVICE;
                    }
                }
                cy_ble_hidscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;

                cy_ble_hidscServiceCount[idx] = 0u;
            }
        }
    #endif /* CY_BLE_HIDS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HIDS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc: An application layer event callback function to receive
*                    events from the BLE Component. The definition of
*                    cy_ble_callback_t for HID Service is:\n
*                    typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                       void *eventParam)
*                    * eventCode indicates the event that triggered this
*                      callback (e.g. CY_BLE_EVT_HIDS_NOTIFICATION_ENABLED).
*                    * eventParam contains the parameters corresponding to the
*                      current event. (e.g. pointer to cy_stc_ble_hids_char_value_t
*                      structure that contains details of the characteristic for
*                      which notification enabled event was triggered).
*
*
* \sideeffect The *eventParams in the callback function should not be used by
*                the application once the callback function execution is finished.
*                Otherwise this data may become corrupted.
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
cy_en_ble_api_result_t Cy_BLE_HIDS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_HIDS_ApplCallback = callbackFunc;
    if(cy_ble_hidsConfigPtr != NULL)
    {
        cy_ble_hidsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_HIDS_SERVER


/******************************************************************************
* Function Name: Cy_BLE_HIDSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets local characteristic value of the specified HID Service characteristics.
*
*  \param serviceIndex: The index of the service instance. e.g. If two HID Services are
*                 supported in your design, then first service will be identified
*                 by serviceIndex of 0 and the second by serviceIndex of 1.
*  \param charIndex: The index of a service characteristic.
*              * CY_BLE_HIDS_PROTOCOL_MODE - Protocol Mode characteristic
*              * CY_BLE_HIDS_REPORT_MAP - Report Map characteristic
*              * CY_BLE_HIDS_INFORMATION - HID Information characteristic
*              * CY_BLE_HIDS_CONTROL_POINT - HID Control Point characteristic
*              * CY_BLE_HIDS_BOOT_KYBRD_IN_REP - Boot Keyboard Input Report Characteristic
*              * CY_BLE_HIDS_BOOT_KYBRD_OUT_REP - Boot Keyboard Output Report Characteristic
*              * CY_BLE_HIDS_BOOT_MOUSE_IN_REP - Boot Mouse Input Report Characteristic
*              * CY_BLE_HIDS_REPORT - Report Characteristic
*
*  \param attrSize: The size of the characteristic value attribute.
*
*  \param attrValue: The pointer to the characteristic value data that should be
*               stored in the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HIDSS_SetCharacteristicValue(uint8_t serviceIndex,
                                                           cy_en_ble_hids_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    cy_ble_gatt_db_attr_handle_t charValueHandle;

    if((serviceIndex >= CY_BLE_HIDSS_SERVICE_COUNT) || (charIndex >= CY_BLE_HIDS_CHAR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        switch(charIndex)
        {
            case CY_BLE_HIDS_PROTOCOL_MODE:
                charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].protocolModeHandle;
                break;

            case CY_BLE_HIDS_REPORT_MAP:
                charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].reportMapHandle;
                break;

            case CY_BLE_HIDS_INFORMATION:
                charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].informationHandle;
                break;

            case CY_BLE_HIDS_CONTROL_POINT:
                charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].controlPointHandle;
                break;

            case CY_BLE_HIDS_BOOT_KYBRD_IN_REP:
                charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].
                                   bootReportArray[CY_BLE_HIDS_BOOT_KYBRD_IN_REP_INDX].reportHandle;
                break;

            case CY_BLE_HIDS_BOOT_KYBRD_OUT_REP:
                charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].
                                   bootReportArray[CY_BLE_HIDS_BOOT_KYBRD_OUT_REP_INDX].reportHandle;
                break;

            case CY_BLE_HIDS_BOOT_MOUSE_IN_REP:
                charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].
                                   bootReportArray[CY_BLE_HIDS_BOOT_MOUSE_IN_REP_INDX].reportHandle;
                break;

            default:                    /* Report characteristic */
                /* Verify that requested report exists in particular service */
                if(((uint8_t)charIndex - (uint8_t)CY_BLE_HIDS_REPORT) <
                   cy_ble_hidsConfigPtr->hidss[serviceIndex].reportCount)
                {
                    charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].
                                       reportArray[charIndex - CY_BLE_HIDS_REPORT].reportHandle;
                }
                else
                {
                    charValueHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                }
                break;
        }
        if(charValueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
        else
        {
            /* Store data in database */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = charValueHandle,
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
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HIDSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets local characteristic value of the specified HID Service characteristics.
*
*  \param serviceIndex: The index of the service instance. e.g. If two HID Services are
*                 supported in your design, then first service will be identified
*                 by serviceIndex of 0 and the second by serviceIndex of 1.
*
*  \param charIndex: The index of the service characteristic.
*              * CY_BLE_HIDS_PROTOCOL_MODE - Protocol Mode characteristic
*              * CY_BLE_HIDS_REPORT_MAP - Report Map characteristic
*              * CY_BLE_HIDS_INFORMATION - HID Information characteristic
*              * CY_BLE_HIDS_CONTROL_POINT - HID Control Point characteristic
*              * CY_BLE_HIDS_BOOT_KYBRD_IN_REP - Boot Keyboard Input Report Characteristic
*              * CY_BLE_HIDS_BOOT_KYBRD_OUT_REP - Boot Keyboard Output Report Characteristic
*              * CY_BLE_HIDS_BOOT_MOUSE_IN_REP - Boot Mouse Input Report Characteristic
*              * CY_BLE_HIDS_REPORT - Report Characteristic
*
*  \param attrSize: The size of the characteristic value attribute.
*
*  \param attrValue: The pointer to the location where characteristic value data
*               should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HIDSS_GetCharacteristicValue(uint8_t serviceIndex,
                                                           cy_en_ble_hids_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    cy_ble_gatt_db_attr_handle_t charValueHandle;

    if((serviceIndex >= CY_BLE_HIDSS_SERVICE_COUNT) || (charIndex >= CY_BLE_HIDS_CHAR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        switch(charIndex)
        {
            case CY_BLE_HIDS_PROTOCOL_MODE:
                charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].protocolModeHandle;
                break;

            case CY_BLE_HIDS_REPORT_MAP:
                charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].reportMapHandle;
                break;

            case CY_BLE_HIDS_INFORMATION:
                charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].informationHandle;
                break;

            case CY_BLE_HIDS_CONTROL_POINT:
                charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].controlPointHandle;
                break;

            case CY_BLE_HIDS_BOOT_KYBRD_IN_REP:
                charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].
                                   bootReportArray[CY_BLE_HIDS_BOOT_KYBRD_IN_REP_INDX].reportHandle;
                break;

            case CY_BLE_HIDS_BOOT_KYBRD_OUT_REP:
                charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].
                                   bootReportArray[CY_BLE_HIDS_BOOT_KYBRD_OUT_REP_INDX].reportHandle;
                break;

            case CY_BLE_HIDS_BOOT_MOUSE_IN_REP:
                charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].
                                   bootReportArray[CY_BLE_HIDS_BOOT_MOUSE_IN_REP_INDX].reportHandle;
                break;

            default:                    /* Report characteristic */
                /* Verify that requested report exists in particular service */
                if(((uint8_t)charIndex - (uint8_t)CY_BLE_HIDS_REPORT) <
                   cy_ble_hidsConfigPtr->hidss[serviceIndex].reportCount)
                {
                    charValueHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].
                                       reportArray[charIndex - CY_BLE_HIDS_REPORT].reportHandle;
                }
                else
                {
                    charValueHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                }
                break;
        }

        if(charValueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
        else
        {
            /* Read characteristic value from database */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = charValueHandle,
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };

            if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HIDSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets local characteristic descriptor of the specified HID Service
*  characteristic.
*
*  \param connHandle: BLE peer device connection handle.
*  \param serviceIndex: The index of the service instance. e.g. If two HID Services are
*                 supported in your design, then first service will be identified
*                 by serviceIndex of 0 and the second by serviceIndex of 1.
*
*  \param charIndex: The index of the characteristic.
*              * CY_BLE_HIDS_REPORT_MAP          - Report Map Characteristic
*              * CY_BLE_HIDS_BOOT_KYBRD_IN_REP   - Boot Keyboard Input Report Characteristic
*              * CY_BLE_HIDS_BOOT_KYBRD_OUT_REP  - Boot Keyboard Output Report Characteristic
*              * CY_BLE_HIDS_BOOT_MOUSE_IN_REP   - Boot Mouse Input Report Characteristic
*              * CY_BLE_HIDS_REPORT              - Report Characteristic
*
*  \param descrIndex: The index of the descriptor.
*               * CY_BLE_HIDS_REPORT_CCCD - Client Characteristic Configuration descriptor
*               * CY_BLE_HIDS_REPORT_RRD - Report Reference descriptor
*               * CY_BLE_HIDS_REPORT_MAP_ERRD - Report Map External Report Reference descriptor
*
*  \param attrSize: The size of the descriptor value attribute.
*
*  \param attrValue: The pointer to the location where characteristic descriptor
*               value data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional descriptor is absent
*
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HIDSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                uint8_t serviceIndex,
                                                                cy_en_ble_hids_char_index_t charIndex,
                                                                cy_en_ble_hids_descr_t descrIndex,
                                                                uint8_t attrSize,
                                                                uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    cy_ble_gatt_db_attr_handle_t locDescrHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    const cy_stc_ble_hidss_report_t *locReport;

    if((serviceIndex >= CY_BLE_HIDSS_SERVICE_COUNT) || (descrIndex >= CY_BLE_HIDS_DESCR_COUNT) ||
       (charIndex >= CY_BLE_HIDS_CHAR_COUNT) || (charIndex < CY_BLE_HIDS_REPORT_MAP) ||
       (charIndex == CY_BLE_HIDS_BOOT_KYBRD_OUT_REP))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        if(charIndex == CY_BLE_HIDS_REPORT_MAP)
        {
            if(descrIndex == CY_BLE_HIDS_REPORT_MAP_ERRD)
            {
                locDescrHandle = cy_ble_hidsConfigPtr->hidss[serviceIndex].reportMapErrdHandle;
            }
            else
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
        }
        else /* Report characteristics */
        {
            /* Get report structure */
            if((charIndex >= CY_BLE_HIDS_BOOT_KYBRD_IN_REP) && (charIndex <= CY_BLE_HIDS_BOOT_MOUSE_IN_REP))
            {
                locReport = &cy_ble_hidsConfigPtr->hidss[serviceIndex].
                             bootReportArray[charIndex - CY_BLE_HIDS_BOOT_KYBRD_IN_REP];
            }
            else
            {
                locReport = &cy_ble_hidsConfigPtr->hidss[serviceIndex].reportArray[charIndex - CY_BLE_HIDS_REPORT];
            }
            /* Get descriptor handle from report structure */
            if(descrIndex == CY_BLE_HIDS_REPORT_CCCD)
            {
                locDescrHandle = locReport->cccdHandle;
            }
            else if(descrIndex == CY_BLE_HIDS_REPORT_RRD)
            {
                locDescrHandle = locReport->rrdHandle;
            }
            else /* External Report Reference descriptor doesn't exist for report characteristic */
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
        }
    }
    if(apiResult != CY_BLE_ERROR_INVALID_PARAMETER)
    {
        if(locDescrHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
        else
        {
            /* Read value from database */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = locDescrHandle,
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .connHandle                 = connHandle,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };

            if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) != CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HIDSS_CccdWriteEventHandler
***************************************************************************//**
*
*  Handles the Write Request Event of the Client Characteristic Configuration
*  descriptor.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*  uint32_t eventCode: The event code to be send to application.
*
******************************************************************************/
static uint32_t Cy_BLE_HIDSS_CccdWriteEventHandler(const cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    uint32_t eventCode;

    if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
    {
        eventCode = (uint32_t)CY_BLE_EVT_HIDSS_NOTIFICATION_ENABLED;
    }
    else
    {
        eventCode = (uint32_t)CY_BLE_EVT_HIDSS_NOTIFICATION_DISABLED;
    }
    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;

#if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
    /* Set flag to store bonding data to flash */
    if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
    {
        cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
    }
#endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */

    return(eventCode);
}


/******************************************************************************
* Function Name: Cy_BLE_HIDSS_WriteEventHandler
***************************************************************************//**
*
*  Handles the Write Request Event.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*   * CY_BLE_GATT_ERR_NONE - Write is successful.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_HIDSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    uint8_t locServIndex = 0u;
    uint32_t reportIndex;
    cy_en_ble_hids_char_index_t locCharIndex;
    cy_stc_ble_hids_char_value_t locCharValue;
    uint32_t eventCode = 0u;

    if(Cy_BLE_HIDS_ApplCallback != NULL)
    {
        locCharValue.connHandle = eventParam->connHandle;
        locCharValue.value = NULL;
        do
        {
            locCharValue.serviceIndex = locServIndex;
            /* Protocol Mode characteristic write request */
            if(eventParam->handleValPair.attrHandle == cy_ble_hidsConfigPtr->hidss[locServIndex].protocolModeHandle)
            {
                locCharValue.charIndex = CY_BLE_HIDS_PROTOCOL_MODE;
                if(eventParam->handleValPair.value.len == CY_BLE_HIDS_PM_CHAR_LEN)
                {
                    switch(eventParam->handleValPair.value.val[0u])
                    {
                        case CY_BLE_HIDS_PROTOCOL_MODE_BOOT:
                            eventCode = (uint32_t)CY_BLE_EVT_HIDSS_BOOT_MODE_ENTER;
                            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                            break;

                        case CY_BLE_HIDS_PROTOCOL_MODE_REPORT:
                            eventCode = (uint32_t)CY_BLE_EVT_HIDSS_REPORT_MODE_ENTER;
                            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                            break;

                        default:    /* Reserved for Future Use. */
                            break;
                    }
                }
            }
            /* Control Point characteristic write request */
            else if(eventParam->handleValPair.attrHandle == cy_ble_hidsConfigPtr->hidss[locServIndex].controlPointHandle)
            {
                locCharValue.charIndex = CY_BLE_HIDS_CONTROL_POINT;
                if(eventParam->handleValPair.value.len == CY_BLE_HIDS_CP_CHAR_LEN)
                {
                    switch(eventParam->handleValPair.value.val[0u])
                    {
                        case CY_BLE_HIDS_CP_SUSPEND:
                            eventCode = (uint32_t)CY_BLE_EVT_HIDSS_SUSPEND;
                            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                            break;

                        case CY_BLE_HIDS_CP_EXIT_SUSPEND:
                            eventCode = (uint32_t)CY_BLE_EVT_HIDSS_EXIT_SUSPEND;
                            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                            break;

                        default:    /* Reserved for Future Use. */
                            break;
                    }
                }
            }
            else if(eventParam->handleValPair.attrHandle == cy_ble_hidsConfigPtr->hidss[locServIndex].
                     bootReportArray[CY_BLE_HIDS_BOOT_KYBRD_IN_REP_INDX].reportHandle)
            {
                locCharValue.charIndex = CY_BLE_HIDS_BOOT_KYBRD_IN_REP;
                locCharValue.value = &eventParam->handleValPair.value;
                eventCode = (uint32_t)CY_BLE_EVT_HIDSS_REPORT_WRITE_CHAR;
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            }
            else if(eventParam->handleValPair.attrHandle == cy_ble_hidsConfigPtr->hidss[locServIndex].
                     bootReportArray[CY_BLE_HIDS_BOOT_KYBRD_IN_REP_INDX].cccdHandle)
            {
                locCharValue.charIndex = CY_BLE_HIDS_BOOT_KYBRD_IN_REP;
                eventCode = Cy_BLE_HIDSS_CccdWriteEventHandler(eventParam);
            }
            else if(eventParam->handleValPair.attrHandle == cy_ble_hidsConfigPtr->hidss[locServIndex].
                     bootReportArray[CY_BLE_HIDS_BOOT_KYBRD_OUT_REP_INDX].reportHandle)
            {
                locCharValue.charIndex = CY_BLE_HIDS_BOOT_KYBRD_OUT_REP;
                locCharValue.value = &eventParam->handleValPair.value;
                eventCode = (uint32_t)CY_BLE_EVT_HIDSS_REPORT_WRITE_CHAR;
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            }
            else if(eventParam->handleValPair.attrHandle == cy_ble_hidsConfigPtr->hidss[locServIndex].
                     bootReportArray[CY_BLE_HIDS_BOOT_MOUSE_IN_REP_INDX].reportHandle)
            {
                locCharValue.charIndex = CY_BLE_HIDS_BOOT_MOUSE_IN_REP;
                locCharValue.value = &eventParam->handleValPair.value;
                eventCode = (uint32_t)CY_BLE_EVT_HIDSS_REPORT_WRITE_CHAR;
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            }
            else if(eventParam->handleValPair.attrHandle == cy_ble_hidsConfigPtr->hidss[locServIndex].
                     bootReportArray[CY_BLE_HIDS_BOOT_MOUSE_IN_REP_INDX].cccdHandle)
            {
                locCharValue.charIndex = CY_BLE_HIDS_BOOT_MOUSE_IN_REP;
                eventCode = Cy_BLE_HIDSS_CccdWriteEventHandler(eventParam);
            }
            else
            {
                uint8_t locReqHandle = 0u;

                locCharIndex = CY_BLE_HIDS_REPORT;
                for(reportIndex = 0u; (reportIndex < cy_ble_hidsConfigPtr->hidss[locServIndex].reportCount) &&
                    (locReqHandle == 0u); reportIndex++)
                {
                    if(eventParam->handleValPair.attrHandle ==
                       cy_ble_hidsConfigPtr->hidss[locServIndex].reportArray[reportIndex].reportHandle)
                    {
                        locCharValue.charIndex = locCharIndex;
                        locCharValue.value = &eventParam->handleValPair.value;
                        eventCode = (uint32_t)CY_BLE_EVT_HIDSS_REPORT_WRITE_CHAR;
                        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                        locReqHandle = 1u;
                    }
                    if(eventParam->handleValPair.attrHandle ==
                       cy_ble_hidsConfigPtr->hidss[locServIndex].reportArray[reportIndex].cccdHandle)
                    {
                        locCharValue.charIndex = locCharIndex;
                        eventCode = Cy_BLE_HIDSS_CccdWriteEventHandler(eventParam);
                        locReqHandle = 1u;
                    }
                    locCharIndex++;
                }
            }
            locServIndex++;

            /* Store data to database if event is handled */
            if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) == 0u)
            {
                cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                {
                    .handleValuePair.attrHandle = eventParam->handleValPair.attrHandle,
                    .handleValuePair.value.len  = eventParam->handleValPair.value.len,
                    .handleValuePair.value.val  = eventParam->handleValPair.value.val,
                    .offset                     = 0u,
                    .connHandle                 = eventParam->connHandle,
                    .flags                      = CY_BLE_GATT_DB_PEER_INITIATED
                };

                gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);
                if(gattErr == CY_BLE_GATT_ERR_NONE)
                {
                    Cy_BLE_HIDS_ApplCallback(eventCode, &locCharValue);
                }
                break;
            }
        }
        while(locServIndex < CY_BLE_HIDSS_SERVICE_COUNT);
    }

    return(gattErr);
}


/******************************************************************************
* Function Name: Cy_BLE_HIDSS_SendNotification
***************************************************************************//**
*
*  Sends specified HID Service characteristic notification to the Client device.
*
*  CY_BLE_EVT_HIDSC_NOTIFICATION event is received by the peer device, on invoking
*   this function.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_HIDSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: BLE peer device connection handle.
*  \param serviceIndex: The index of the HID service instance. e.g. If two HID Services
*                 are supported in your design, then first service will be
*                 identified by serviceIndex of 0 and the second by serviceIndex
*                 of 1.
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: Pointer to the characteristic value data that should be sent to
*               the Client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the client.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HIDSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     uint8_t serviceIndex,
                                                     cy_en_ble_hids_char_index_t charIndex,
                                                     uint8_t attrSize,
                                                     uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    const cy_stc_ble_hidss_report_t *locReport;

    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((serviceIndex >= CY_BLE_HIDSS_SERVICE_COUNT) || (charIndex >= CY_BLE_HIDS_CHAR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        if(charIndex == CY_BLE_HIDS_BOOT_KYBRD_IN_REP)
        {
            locReport = &cy_ble_hidsConfigPtr->hidss[serviceIndex].bootReportArray[CY_BLE_HIDS_BOOT_KYBRD_IN_REP_INDX];
        }
        else if(charIndex == CY_BLE_HIDS_BOOT_MOUSE_IN_REP)
        {
            locReport = &cy_ble_hidsConfigPtr->hidss[serviceIndex].bootReportArray[CY_BLE_HIDS_BOOT_MOUSE_IN_REP_INDX];
        }
        else if((charIndex >= CY_BLE_HIDS_REPORT) && (charIndex <= CY_BLE_HIDS_REPORT_END))
        {
            /* Verify that requested report exists in particular service */
            if(((uint8_t)charIndex - (uint8_t)CY_BLE_HIDS_REPORT) < cy_ble_hidsConfigPtr->hidss[serviceIndex].reportCount)
            {
                locReport = &cy_ble_hidsConfigPtr->hidss[serviceIndex].reportArray[charIndex - CY_BLE_HIDS_REPORT];
            }
            else
            {
                apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
            }
        }
        else
        {
            apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
        }
        if(apiResult == CY_BLE_SUCCESS)
        {
            if(locReport->reportHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
            }
            else
            {
                /* Send Notification if it is enabled */
                if(CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, locReport->cccdHandle))
                {
                    /* Fill all fields of write request structure ... */
                    cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
                    {
                        .handleValPair.attrHandle = locReport->reportHandle,
                        .handleValPair.value.val  = attrValue,
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
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_HIDSS_OnDeviceConnected
***************************************************************************//**
*
*  Handles the connection establishment request.
*
* \return
*  None
*
******************************************************************************/
static void Cy_BLE_HIDSS_OnDeviceConnected(void)
{
    /* The Protocol Mode characteristic value shall be reset to the default value
     * following a connection establishment */
    uint8_t defaultProtocol = CY_BLE_HIDS_PROTOCOL_DEFAULT;
    uint8_t locServIndex = 0u;

    do
    {
        if(cy_ble_hidsConfigPtr->hidss[locServIndex].protocolModeHandle !=
           CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            /* Store default protocol value in database */
            CY_BLE_GATT_DB_ATTR_SET_GEN_VALUE(cy_ble_hidsConfigPtr->hidss[locServIndex].protocolModeHandle,
                                              &defaultProtocol, sizeof(defaultProtocol));
        }
        ++locServIndex;
    }
    while(locServIndex < CY_BLE_HIDSS_SERVICE_COUNT);
}


#endif /* CY_BLE_HIDS_SERVER */

#ifdef CY_BLE_HIDS_CLIENT


/******************************************************************************
* Function Name: Cy_BLE_HIDSC_InclDiscoveryEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP event
*  Based on the service UUID, an appropriate data structure is populated using
*  the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*  \param discoveryService: The index of the service instance.
*
*
******************************************************************************/
static void Cy_BLE_HIDSC_InclDiscoveryEventHandler(const cy_stc_ble_disc_incl_info_t *discInclInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discInclInfo->connHandle);
    uint32_t discoveryService = cy_ble_discovery[discIdx].servCount - CY_BLE_SRVI_HIDS;

    if((discInclInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount >= (uint32_t)CY_BLE_SRVI_HIDS) &&
       (cy_ble_discovery[discIdx].servCount <= (uint32_t)CY_BLE_SRVI_HIDS_END))
    {
        cy_ble_hidsc[discIdx][discoveryService].includeHandle = discInclInfo->inclDefHandle;

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HIDSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP event
*  Based on the service UUID, an appropriate data structure is populated using
*  the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_HIDSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    static uint32_t discoveryLastServ[CY_BLE_CONFIG_GATTC_COUNT] = { 0u };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t discoveryService = cy_ble_discovery[discIdx].servCount - CY_BLE_SRVI_HIDS;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount >= (uint32_t)CY_BLE_SRVI_HIDS) &&
       (cy_ble_discovery[discIdx].servCount <= (uint32_t)CY_BLE_SRVI_HIDS_END))
    {
        /* Update last characteristic endHandle to declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            if(discoveryLastServ[discIdx] == discoveryService)
            {
                *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            }
            lastEndHandle[discIdx] = NULL;
        }

        switch(discCharInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_HIDS_PROTOCOL_MODE:
                Cy_BLE_CheckStoreCharHandle(cy_ble_hidsc[discIdx][discoveryService].protocolMode);
                break;

            case CY_BLE_UUID_CHAR_HIDS_INFORMATION:
                Cy_BLE_CheckStoreCharHandle(cy_ble_hidsc[discIdx][discoveryService].information);
                break;

            case CY_BLE_UUID_CHAR_HIDS_REPORT_MAP:
                Cy_BLE_CheckStoreCharHandle(cy_ble_hidsc[discIdx][discoveryService].reportMap);
                lastEndHandle[discIdx] = &cy_ble_hidsc[discIdx][discoveryService].reportMap.endHandle;
                break;

            case CY_BLE_UUID_CHAR_HIDS_CONTROL_POINT:
                Cy_BLE_CheckStoreCharHandle(cy_ble_hidsc[discIdx][discoveryService].controlPoint);
                break;

            case CY_BLE_UUID_CHAR_HIDS_REPORT:
                Cy_BLE_CheckStoreCharHandle(cy_ble_hidsc[discIdx][discoveryService].
                                             report[cy_ble_hidsc[discIdx][discoveryService].reportCount]);
                lastEndHandle[discIdx] = &cy_ble_hidsc[discIdx][discoveryService].
                                          report[cy_ble_hidsc[discIdx][discoveryService].reportCount].endHandle;

                if(cy_ble_hidsc[discIdx][discoveryService].reportCount < CY_BLE_HIDSC_REPORT_COUNT)
                {
                    cy_ble_hidsc[discIdx][discoveryService].reportCount++;
                }
                break;

            case CY_BLE_UUID_CHAR_HIDS_BOOT_KEYBOARD_IN_RPT:
                Cy_BLE_CheckStoreCharHandle(cy_ble_hidsc[discIdx][discoveryService].
                                             bootReport[CY_BLE_HIDS_BOOT_KYBRD_IN_REP_INDX]);
                lastEndHandle[discIdx] = &cy_ble_hidsc[discIdx][discoveryService].
                                          bootReport[CY_BLE_HIDS_BOOT_KYBRD_IN_REP_INDX].endHandle;
                break;

            case CY_BLE_UUID_CHAR_HIDS_BOOT_KEYBOARD_OUT_RPT:
                Cy_BLE_CheckStoreCharHandle(cy_ble_hidsc[discIdx][discoveryService].
                                             bootReport[CY_BLE_HIDS_BOOT_KYBRD_OUT_REP_INDX]);
                lastEndHandle[discIdx] = &cy_ble_hidsc[discIdx][discoveryService].
                                          bootReport[CY_BLE_HIDS_BOOT_KYBRD_OUT_REP_INDX].endHandle;
                break;

            case CY_BLE_UUID_CHAR_HIDS_BOOT_MOUSE_IN_RPT:
                Cy_BLE_CheckStoreCharHandle(cy_ble_hidsc[discIdx][discoveryService].
                                             bootReport[CY_BLE_HIDS_BOOT_MOUSE_IN_REP_INDX]);
                lastEndHandle[discIdx] = &cy_ble_hidsc[discIdx][discoveryService].
                                          bootReport[CY_BLE_HIDS_BOOT_MOUSE_IN_REP_INDX].endHandle;
                break;

            default:
                break;
        }
        /* Init characteristic endHandle to Service endHandle.
         * Characteristic endHandle will be updated to the declaration
         * Handler of the following characteristic, in the following characteristic discovery procedure. */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range.endHandle;
            /* Init service index of discovered characteristic */
            discoveryLastServ[discIdx] = discoveryService;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HIDSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_FIND_INFO_RSP event.
*  This event is generated when the server successfully sends the data for
*  CY_BLE_EVT_GATTC_FIND_INFO_REQ. Based on the service UUID, an appropriate data
*  structure is populated to the service with a service callback.
*
*  \param discDescrInfo: The pointer to a descriptor information structure.
*  \param discoveryService: The index of the service instance.
*
******************************************************************************/
static void Cy_BLE_HIDSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t hidsServIdx = cy_ble_discovery[discIdx].servCount - (uint32_t)CY_BLE_SRVI_HIDS;

    if((cy_ble_discovery[discIdx].servCount >= (uint32_t)CY_BLE_SRVI_HIDS) &&
       (cy_ble_discovery[discIdx].servCount <= (uint32_t)CY_BLE_SRVI_HIDS_END))
    {
        switch((cy_en_ble_hids_char_index_t)cy_ble_discovery[discIdx].charCount)
        {
            case CY_BLE_HIDS_REPORT_MAP:
                /* Descriptors for reportMap characteristics */
                if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_EXTERNAL_REPORT_REF)
                {
                    Cy_BLE_CheckStoreCharDescrHandle(cy_ble_hidsc[discIdx][hidsServIdx].reportMap.errdHandle);
                }
                break;

            case CY_BLE_HIDS_BOOT_KYBRD_IN_REP:
            case CY_BLE_HIDS_BOOT_KYBRD_OUT_REP:
            case CY_BLE_HIDS_BOOT_MOUSE_IN_REP:
            {
                /* Descriptors for report characteristics */
                uint32_t bootRepIdx = cy_ble_discovery[discIdx].charCount - CY_BLE_HIDS_BOOT_KYBRD_IN_REP;
                if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_CLIENT_CONFIG)
                {
                    Cy_BLE_CheckStoreCharDescrHandle(cy_ble_hidsc[discIdx][hidsServIdx].bootReport[bootRepIdx].
                                                      cccdHandle);
                }
                else if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_REPORT_REFERENCE)
                {
                    Cy_BLE_CheckStoreCharDescrHandle(cy_ble_hidsc[discIdx][hidsServIdx].bootReport[bootRepIdx].
                                                      rrdHandle);
                }
                else        /* Report Characteristic doesn't support other descriptors */
                {
                }
                break;
            }

            default:
            {
                if((cy_ble_discovery[discIdx].charCount >= (uint32_t)CY_BLE_HIDS_REPORT) &&
                   (cy_ble_discovery[discIdx].charCount <= (uint32_t)CY_BLE_HIDS_REPORT_END))
                {
                    /* Descriptors for report characteristics */
                    uint32_t reportIdx = cy_ble_discovery[discIdx].charCount - (uint32_t)CY_BLE_HIDS_REPORT;

                    if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_CLIENT_CONFIG)
                    {
                        Cy_BLE_CheckStoreCharDescrHandle(cy_ble_hidsc[discIdx][hidsServIdx].report[reportIdx].
                                                          cccdHandle);
                    }
                    else if(discDescrInfo->uuid.uuid16 == CY_BLE_UUID_CHAR_REPORT_REFERENCE)
                    {
                        Cy_BLE_CheckStoreCharDescrHandle(cy_ble_hidsc[discIdx][hidsServIdx].report[reportIdx].
                                                          rrdHandle);
                    }
                    else        /* Report Characteristic doesn't support other descriptors */
                    {
                    }
                }
                break;
            }
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HIDSC_GetCharRange
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
static void Cy_BLE_HIDSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t hidsServIdx = cy_ble_discovery[discIdx].servCount - (uint32_t)CY_BLE_SRVI_HIDS;
    uint32_t exitFlag = 0u;

    if((cy_ble_discovery[discIdx].servCount >= (uint32_t)CY_BLE_SRVI_HIDS) &&
       (cy_ble_discovery[discIdx].servCount <= (uint32_t)CY_BLE_SRVI_HIDS_END))
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_HIDS_CHAR_COUNT) && (exitFlag == 0u))
        {
            switch((cy_en_ble_hids_char_index_t)cy_ble_discovery[discIdx].charCount)
            {
                case CY_BLE_HIDS_REPORT_MAP:
                    /* Read report map characteristic range */
                    if((cy_ble_hidsc[discIdx][hidsServIdx].reportMap.endHandle -
                        cy_ble_hidsc[discIdx][hidsServIdx].reportMap.valueHandle) != 0u)
                    {
                        charRangeInfo->range.startHandle = cy_ble_hidsc[discIdx][hidsServIdx].reportMap.valueHandle + 1u;
                        charRangeInfo->range.endHandle = cy_ble_hidsc[discIdx][hidsServIdx].reportMap.endHandle;
                        exitFlag = 1u;
                    }
                    break;

                case CY_BLE_HIDS_BOOT_KYBRD_IN_REP:
                case CY_BLE_HIDS_BOOT_KYBRD_OUT_REP:
                case CY_BLE_HIDS_BOOT_MOUSE_IN_REP:
                {
                    /* Read boot report characteristic range */
                    uint32_t bootRepIdx = cy_ble_discovery[discIdx].charCount - CY_BLE_HIDS_BOOT_KYBRD_IN_REP;

                    if((cy_ble_hidsc[discIdx][hidsServIdx].bootReport[bootRepIdx].endHandle -
                        cy_ble_hidsc[discIdx][hidsServIdx].bootReport[bootRepIdx].valueHandle) != 0u)
                    {
                        charRangeInfo->range.startHandle = cy_ble_hidsc[discIdx][hidsServIdx].bootReport[bootRepIdx].
                                                            valueHandle + 1u;
                        charRangeInfo->range.endHandle = cy_ble_hidsc[discIdx][hidsServIdx].bootReport[bootRepIdx].
                                                          endHandle;
                        exitFlag = 1u;
                    }
                    break;
                }

                default:
                {
                    if((cy_ble_discovery[discIdx].charCount >= (uint32_t)CY_BLE_HIDS_REPORT) &&
                       (cy_ble_discovery[discIdx].charCount <= (uint32_t)CY_BLE_HIDS_REPORT_END))
                    {
                        /* Read report characteristic range */
                        uint32_t reportIdx = cy_ble_discovery[discIdx].charCount - CY_BLE_HIDS_REPORT;
                        if((cy_ble_hidsc[discIdx][hidsServIdx].report[reportIdx].endHandle -
                            cy_ble_hidsc[discIdx][hidsServIdx].report[reportIdx].valueHandle) != 0u)
                        {
                            charRangeInfo->range.startHandle = cy_ble_hidsc[discIdx][hidsServIdx].report[reportIdx].
                                                                valueHandle + 1u;
                            charRangeInfo->range.endHandle = cy_ble_hidsc[discIdx][hidsServIdx].report[reportIdx].
                                                              endHandle;
                            exitFlag = 1u;
                        }
                    }
                    break;
                }
            }

            if(exitFlag == 0u)
            {
                cy_ble_discovery[discIdx].charCount++;
            }
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}
/******************************************************************************
* Function Name: Cy_BLE_HIDSC_SetCharacteristicValue
***************************************************************************//**
*
*  Sends a request to set characteristic value of the specified HID Service,
*  which is identified by serviceIndex and reportIndex, on the server device.
*  This function call can result in generation of the following events based on
*  the response from the server device:
*  * CY_BLE_EVT_HIDSC_WRITE_CHAR_RESPONSE
*  * CY_BLE_EVT_GATTC_ERROR_RSP
*
*  \param connHandle: The connection handle.
*  \param subProcedure: Characteristic value write sub-procedure.
*                 * CY_BLE_HIDSC_WRITE_WITHOUT_RESPONSE
*                 * CY_BLE_HIDSC_WRITE_CHAR_VALUE
*  \param serviceIndex: The index of the service instance. e.g. If two HID Services
*                 are supported in your design, then first service will be
*                 identified by serviceIndex of 0 and the second by
*                 serviceIndex of 1.
*  \param charIndex: The index of a service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic
*  * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                     characteristic
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the HIDS service-specific callback is registered
*      (with Cy_BLE_HIDS_RegisterAttrCallback):
*  * CY_BLE_EVT_HIDSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_hids_char_value_t.
*  .
*   Otherwise (if the HIDS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HIDSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_hidsc_char_write_t subProcedure,
                                                           uint8_t serviceIndex,
                                                           cy_en_ble_hids_char_index_t charIndex,
                                                           uint8_t attrSize,
                                                           uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    cy_ble_gatt_db_attr_handle_t charValueHandle;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);
    
    if((serviceIndex >= CY_BLE_HIDSC_SERVICE_COUNT) || (charIndex >= CY_BLE_HIDS_CHAR_COUNT) || 
       (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        charValueHandle = Cy_BLE_HIDSC_GetCharacteristicValueHandle(connHandle, serviceIndex, charIndex);

        if(charValueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            cy_stc_ble_gattc_write_cmd_req_t writeReqParam =
            {
                .handleValPair.attrHandle = charValueHandle,
                .handleValPair.value.val  = attrValue,
                .handleValPair.value.len  = attrSize,
                .connHandle               = connHandle
            };
            /* Use WriteWithoutResponse subprocedure for the following characteristics:
             * CY_BLE_HIDS_PROTOCOL_MODE
             * CY_BLE_HIDS_CONTROL_POINT
             */
            if(subProcedure == CY_BLE_HIDSC_WRITE_WITHOUT_RESPONSE)
            {
                apiResult = Cy_BLE_GATTC_WriteWithoutResponse(&writeReqParam);
            }
            else
            {
                apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);
                /* Save handle to support service-specific write response from device */
                if(apiResult == CY_BLE_SUCCESS)
                {
                    cy_ble_hidscReqHandle[discIdx] = charValueHandle;
                }
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
* Function Name: Cy_BLE_HIDSC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read the characteristic value from a server
*  which is identified by charIndex.
*
*  The Read Response returns the characteristic value in the Attribute Value
*  parameter.
*
*  The Read Response only contains the characteristic value that is less than or
*  equal to (MTU - 1) octets in length. If the characteristic value is greater
*  than (MTU - 1) octets in length, the Read Long Characteristic Value procedure
*  may be used if the rest of the characteristic Value is required.
*
*  \param connHandle: The connection handle.
*  \param subProcedure: The characteristic value read sub-procedure.
*                * CY_BLE_HIDSC_READ_CHAR_VALUE
*                * CY_BLE_HIDSC_READ_LONG_CHAR_VALUE.
*  \param serviceIndex: The index of the service instance.
*  \param charIndex: The index of the service characteristic.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The read request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established
*  * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                     characteristic
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the HIDS service-specific callback is registered
*      (with Cy_BLE_HIDS_RegisterAttrCallback):
*  * CY_BLE_EVT_HIDSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_hids_char_value_t.
*  .
*   Otherwise (if the HIDS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_READ_RSP - in case if the requested attribute is
*                                successfully read on the peer device,
*                                the details (handle, value, etc.) are
*                                provided with event parameters
*                                structure (cy_stc_ble_gattc_read_rsp_param_t).
*  * CY_BLE_EVT_GATTC_READ_BLOB_RSP - in case if the requested attribute is
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
cy_en_ble_api_result_t Cy_BLE_HIDSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_hidsc_char_read_t subProcedure,
                                                           uint8_t serviceIndex,
                                                           cy_en_ble_hids_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    cy_ble_gatt_db_attr_handle_t charValueHandle;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((serviceIndex >= CY_BLE_HIDSC_SERVICE_COUNT) || (charIndex >= CY_BLE_HIDS_CHAR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        charValueHandle = Cy_BLE_HIDSC_GetCharacteristicValueHandle(connHandle, serviceIndex, charIndex);

        if(charValueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            /* Use Read Long Characteristic procedure for Report map characteristic */
            if(subProcedure == CY_BLE_HIDSC_READ_LONG_CHAR_VALUE)
            {
                cy_stc_ble_gattc_read_blob_req_t charReadLongParam =
                {
                    .handleOffset.attrHandle = charValueHandle,
                    .handleOffset.offset     = 0u,
                    .connHandle              = connHandle
                };

                apiResult = Cy_BLE_GATTC_ReadLongCharacteristicValues(&charReadLongParam);
            }
            else
            {
                cy_stc_ble_gattc_read_req_t readReqParam =
                {
                    .attrHandle = charValueHandle,
                    .connHandle = connHandle
                };

                apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);
            }
            /* Save handle to support service-specific read response from device */
            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_hidscReqHandle[discIdx] = charValueHandle;
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
* Function Name: Cy_BLE_HIDSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  This function is used to write the characteristic descriptor to the server,
*  which is identified by charIndex.
*
*  Internally, Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server side the following events can be
*  generated:
*  * CY_BLE_EVT_HIDSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_HIDSS_NOTIFICATION_DISABLED
*
*  \param connHandle: The BLE peer device connection handle.
*  \param serviceIndex: The index of the service instance. e.g. If two HID Services
*                 are supported in your design, then first service will be
*                 identified by serviceIndex of 0 and the second by
*                 serviceIndex of 1.
*  \param charIndex: The index of the HID service characteristic.
*  \param descrIndex: The index of the HID service characteristic descriptor.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic descriptor value data that
*               should be sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the HIDS service-specific callback is registered
*      (with Cy_BLE_HIDS_RegisterAttrCallback):
*  * CY_BLE_EVT_HIDSC_WRITE_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_hids_descr_value_t.
*  .
*   Otherwise (if the HIDS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_HIDSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                uint8_t serviceIndex,
                                                                cy_en_ble_hids_char_index_t charIndex,
                                                                cy_en_ble_hids_descr_t descrIndex,
                                                                uint8_t attrSize,
                                                                uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    cy_ble_gatt_db_attr_handle_t locDescrHandle;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);
    
    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((serviceIndex >= CY_BLE_HIDSC_SERVICE_COUNT) || (charIndex >= CY_BLE_HIDS_CHAR_COUNT) ||
            (descrIndex >= CY_BLE_HIDS_DESCR_COUNT) || (charIndex < CY_BLE_HIDS_REPORT_MAP) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        locDescrHandle = Cy_BLE_HIDSC_GetCharacteristicDescriptorHandle(connHandle, serviceIndex, charIndex, descrIndex);

        if(locDescrHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            /* Fill all fields of write request structure ... */
            cy_stc_ble_gattc_write_req_t writeReqParam =
            {
                .handleValPair.attrHandle = locDescrHandle,
                .handleValPair.value.val  = attrValue,
                .handleValPair.value.len  = attrSize,
                .connHandle               = connHandle,
            };

            /* ... and send request to server device. */
            apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);
            /* Save handle to support service-specific read response from device */
            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_hidscReqHandle[discIdx] = locDescrHandle;
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
* Function Name: Cy_BLE_HIDSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets a characteristic descriptor of the specified characteristic of the HID
*  Service from the server device.
*
*  This function call can result in generation of the following events based on
*  the response from the server device.
*  * CY_BLE_EVT_HIDSC_READ_DESCR_RESPONSE
*  * CY_BLE_EVT_GATTC_ERROR_RSP
*
*  \param connHandle: The connection handle.
*  \param serviceIndex: The index of the service instance. e.g. If two HID Services
*                 are supported in your design, then first service will be
*                 identified by serviceIndex of 0 and the second by
*                 serviceIndex of 1.
*  \param charIndex: The index of the service characteristic.
*  \param descrIndex: The index of the HID Service characteristic descriptor.
*
* \return
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular descriptor
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the HIDS service-specific callback is registered
*      (with Cy_BLE_HIDS_RegisterAttrCallback):
*  * CY_BLE_EVT_HIDSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_hids_descr_value_t.
*  .
*  Otherwise (if the HIDS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_HIDSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                uint8_t serviceIndex,
                                                                cy_en_ble_hids_char_index_t charIndex,
                                                                cy_en_ble_hids_descr_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    cy_stc_ble_gattc_read_req_t readReqParam;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if((serviceIndex >= CY_BLE_HIDSC_SERVICE_COUNT) || (charIndex >= CY_BLE_HIDS_CHAR_COUNT) ||
       (descrIndex >= CY_BLE_HIDS_DESCR_COUNT) || (charIndex < CY_BLE_HIDS_REPORT_MAP) || 
       (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        readReqParam.attrHandle = Cy_BLE_HIDSC_GetCharacteristicDescriptorHandle(connHandle, serviceIndex, charIndex,
                                                                                 descrIndex);

        if(readReqParam.attrHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            readReqParam.connHandle = connHandle;
            apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);
            /* Save handle to support service-specific read response from device */
            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_hidscReqHandle[discIdx] = readReqParam.attrHandle;
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
* Function Name: Cy_BLE_HIDSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles the Notification Event.
*
*  \param eventParam:  pointer to the data structure specified by the event.
*
*
******************************************************************************/
static void Cy_BLE_HIDSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    uint8_t locServIndex;
    cy_en_ble_hids_char_index_t locCharIndex;

    if(Cy_BLE_HIDS_ApplCallback != NULL)
    {
        for(locServIndex = 0u; locServIndex < CY_BLE_HIDSC_SERVICE_COUNT; locServIndex++)
        {
            for(locCharIndex = CY_BLE_HIDS_PROTOCOL_MODE; locCharIndex < CY_BLE_HIDS_CHAR_COUNT; locCharIndex++)
            {
                if(Cy_BLE_HIDSC_GetCharacteristicValueHandle(eventParam->connHandle, locServIndex, locCharIndex) ==
                   eventParam->handleValPair.attrHandle)
                {
                    cy_stc_ble_hids_char_value_t notifValue =
                    {
                        .connHandle   = eventParam->connHandle,
                        .serviceIndex = locServIndex,
                        .charIndex    = locCharIndex,
                        .value        = &eventParam->handleValPair.value
                    };

                    Cy_BLE_HIDS_ApplCallback((uint32_t)CY_BLE_EVT_HIDSC_NOTIFICATION, &notifValue);
                    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                    break;
                }
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_HIDSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles the Read Response Event.
*
*  \param eventParam:  pointer to the data structure specified by the event.
*
*
******************************************************************************/
static void Cy_BLE_HIDSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint8_t locReqHandle = 0u;
    uint8_t locServIndex;
    cy_en_ble_hids_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_HIDS_ApplCallback != NULL) &&
       (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_hidscReqHandle[discIdx]))
    {
        for(locServIndex = 0u; (locServIndex < CY_BLE_HIDSC_SERVICE_COUNT) && (locReqHandle == 0u); locServIndex++)
        {
            for(locCharIndex = CY_BLE_HIDS_PROTOCOL_MODE; (locCharIndex < CY_BLE_HIDS_CHAR_COUNT) && (locReqHandle == 0u);
                locCharIndex++)
            {
                if(cy_ble_hidscReqHandle[discIdx] ==
                   Cy_BLE_HIDSC_GetCharacteristicValueHandle(eventParam->connHandle, locServIndex, locCharIndex))
                {
                    cy_stc_ble_hids_char_value_t locCharValue =
                    {
                        .connHandle   = eventParam->connHandle,
                        .serviceIndex = locServIndex,
                        .charIndex    = locCharIndex,
                        .value        = &eventParam->value
                    };

                    cy_ble_hidscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                    Cy_BLE_HIDS_ApplCallback((uint32_t)CY_BLE_EVT_HIDSC_READ_CHAR_RESPONSE, &locCharValue);
                    locReqHandle = 1u;
                }
                if(locCharIndex >= CY_BLE_HIDS_REPORT_MAP)
                {
                    cy_en_ble_hids_descr_t locDescIndex;

                    for(locDescIndex = CY_BLE_HIDS_REPORT_CCCD; (locDescIndex < CY_BLE_HIDS_DESCR_COUNT) &&
                        (locReqHandle == 0u); locDescIndex++)
                    {
                        if(cy_ble_hidscReqHandle[discIdx] ==
                           Cy_BLE_HIDSC_GetCharacteristicDescriptorHandle(eventParam->connHandle, locServIndex,
                                                                          locCharIndex, locDescIndex))
                        {
                            cy_stc_ble_hids_descr_value_t locDescrValue =
                            {
                                .connHandle   = eventParam->connHandle,
                                .serviceIndex = locServIndex,
                                .charIndex    = locCharIndex,
                                .descrIndex   = locDescIndex,
                                .value        = &eventParam->value,
                            };

                            cy_ble_hidscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                            Cy_BLE_HIDS_ApplCallback((uint32_t)CY_BLE_EVT_HIDSC_READ_DESCR_RESPONSE, &locDescrValue);
                            locReqHandle = 1u;
                        }
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
* Function Name: Cy_BLE_HIDSC_WriteResponseEventHandler
***************************************************************************//**
*  Handles the Write Response Event.
*
*  \param eventParam:  the pointer to the data structure specified by the event.
*
*
******************************************************************************/
static void Cy_BLE_HIDSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint8_t locServIndex;
    uint8_t locReqHandle = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);
    cy_en_ble_hids_char_index_t locCharIndex;

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_HIDS_ApplCallback != NULL) &&
       (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_hidscReqHandle[discIdx]))
    {
        for(locServIndex = 0u; (locServIndex < CY_BLE_HIDSC_SERVICE_COUNT) && (locReqHandle == 0u); locServIndex++)
        {
            for(locCharIndex = CY_BLE_HIDS_PROTOCOL_MODE; (locCharIndex < CY_BLE_HIDS_CHAR_COUNT) && (locReqHandle == 0u);
                locCharIndex++)
            {
                if(cy_ble_hidscReqHandle[discIdx] == Cy_BLE_HIDSC_GetCharacteristicValueHandle(*eventParam, locServIndex,
                                                                                               locCharIndex))
                {
                    cy_stc_ble_hids_char_value_t locCharValue =
                    {
                        .connHandle   = *eventParam,
                        .serviceIndex = locServIndex,
                        .charIndex    = locCharIndex,
                        .value        = NULL
                    };

                    cy_ble_hidscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                    Cy_BLE_HIDS_ApplCallback((uint32_t)CY_BLE_EVT_HIDSC_WRITE_CHAR_RESPONSE, &locCharValue);
                    locReqHandle = 1u;
                }
                if(locCharIndex >= CY_BLE_HIDS_REPORT_MAP)
                {
                    cy_en_ble_hids_descr_t locDescIndex;

                    for(locDescIndex = CY_BLE_HIDS_REPORT_CCCD; (locDescIndex < CY_BLE_HIDS_DESCR_COUNT) &&
                        (locReqHandle == 0u); locDescIndex++)
                    {
                        if(cy_ble_hidscReqHandle[discIdx] ==
                           Cy_BLE_HIDSC_GetCharacteristicDescriptorHandle(*eventParam, locServIndex, locCharIndex,
                                                                          locDescIndex))
                        {
                            cy_stc_ble_hids_descr_value_t locDescrValue =
                            {
                                .connHandle   = *eventParam,
                                .serviceIndex = locServIndex,
                                .charIndex    = locCharIndex,
                                .descrIndex   = locDescIndex,
                                .value        = NULL
                            };

                            cy_ble_hidscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                            Cy_BLE_HIDS_ApplCallback((uint32_t)CY_BLE_EVT_HIDSC_WRITE_DESCR_RESPONSE, &locDescrValue);
                            locReqHandle = 1u;
                        }
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
* Function Name: Cy_BLE_HIDSC_GetCharacteristicValueHandle
***************************************************************************//**
*
*  Returns the discovered peer device characteristic value handle.
*
*  \param serviceIndex: The index of the service instance.
*  \param charIndex: The index of a service characteristic.
*
* \return
*  Returns characteristic value handle of type cy_ble_gatt_db_attr_handle_t.
*  * CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE: when a peer device doesn't have an optional characteristic
*
******************************************************************************/
cy_ble_gatt_db_attr_handle_t Cy_BLE_HIDSC_GetCharacteristicValueHandle(cy_stc_ble_conn_handle_t connHandle,
                                                                       uint8_t serviceIndex,
                                                                       cy_en_ble_hids_char_index_t charIndex)
{
    cy_ble_gatt_db_attr_handle_t returnHandle;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if((serviceIndex >= CY_BLE_HIDSC_SERVICE_COUNT) || (charIndex >= CY_BLE_HIDS_CHAR_COUNT))
    {
        returnHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    }
    else
    {
        if(charIndex == CY_BLE_HIDS_PROTOCOL_MODE)
        {
            returnHandle = cy_ble_hidsc[discIdx][serviceIndex].protocolMode.valueHandle;
        }
        else if(charIndex == CY_BLE_HIDS_INFORMATION)
        {
            returnHandle = cy_ble_hidsc[discIdx][serviceIndex].information.valueHandle;
        }
        else if(charIndex == CY_BLE_HIDS_CONTROL_POINT)
        {
            returnHandle = cy_ble_hidsc[discIdx][serviceIndex].controlPoint.valueHandle;
        }
        else if(charIndex == CY_BLE_HIDS_REPORT_MAP)
        {
            returnHandle = cy_ble_hidsc[discIdx][serviceIndex].reportMap.valueHandle;
        }
        else if(charIndex <= CY_BLE_HIDS_BOOT_MOUSE_IN_REP)
        {
            returnHandle = cy_ble_hidsc[discIdx][serviceIndex].bootReport[charIndex - CY_BLE_HIDS_BOOT_KYBRD_IN_REP].
                            valueHandle;
        }
        else if(((uint8_t)charIndex - (uint8_t)CY_BLE_HIDS_REPORT) < cy_ble_hidsc[discIdx][(serviceIndex)].reportCount)
        {
            returnHandle = cy_ble_hidsc[discIdx][serviceIndex].report[charIndex - CY_BLE_HIDS_REPORT].valueHandle;
        }
        else
        {
            returnHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }

    return(returnHandle);
}


/******************************************************************************
* Function Name: Cy_BLE_HIDSC_GetCharacteristicDescriptorHandle
***************************************************************************//**
*
*  Returns the discovered peer device characteristic descriptor handle.
*
*  \param serviceIndex:    the index of the service instance
*  \param charIndex:       the index of a service characteristic.
*  \param descrIndex:      the index of the descriptor.
*
* \return
*  Returns characteristic value handle of type cy_ble_gatt_db_attr_handle_t.
*  * CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE: when a peer device doesn't have an optional descriptor
*
******************************************************************************/
cy_ble_gatt_db_attr_handle_t Cy_BLE_HIDSC_GetCharacteristicDescriptorHandle(cy_stc_ble_conn_handle_t connHandle,
                                                                            uint8_t serviceIndex,
                                                                            cy_en_ble_hids_char_index_t charIndex,
                                                                            cy_en_ble_hids_descr_t descrIndex)
{
    cy_ble_gatt_db_attr_handle_t returnHandle;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if((serviceIndex >= CY_BLE_HIDSC_SERVICE_COUNT) || (charIndex >= CY_BLE_HIDS_CHAR_COUNT) ||
       (charIndex < CY_BLE_HIDS_REPORT_MAP))
    {
        returnHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    }
    else
    {
        if(charIndex == CY_BLE_HIDS_REPORT_MAP)
        {
            if(descrIndex == CY_BLE_HIDS_REPORT_MAP_ERRD)
            {
                returnHandle = cy_ble_hidsc[discIdx][serviceIndex].reportMap.errdHandle;
            }
            else
            {
                returnHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
        else if(charIndex <= CY_BLE_HIDS_BOOT_MOUSE_IN_REP)
        {
            if(descrIndex == CY_BLE_HIDS_REPORT_CCCD)
            {
                returnHandle = cy_ble_hidsc[discIdx][serviceIndex].bootReport[charIndex - CY_BLE_HIDS_BOOT_KYBRD_IN_REP].
                                cccdHandle;
            }
            else if(descrIndex == CY_BLE_HIDS_REPORT_RRD)
            {
                returnHandle = cy_ble_hidsc[discIdx][serviceIndex].bootReport[charIndex - CY_BLE_HIDS_BOOT_KYBRD_IN_REP].
                                rrdHandle;
            }
            else
            {
                returnHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
        else if(((uint8_t)charIndex - (uint8_t)CY_BLE_HIDS_REPORT) < cy_ble_hidsc[discIdx][serviceIndex].reportCount)
        {
            if(descrIndex == CY_BLE_HIDS_REPORT_CCCD)
            {
                returnHandle = cy_ble_hidsc[discIdx][serviceIndex].report[charIndex - CY_BLE_HIDS_REPORT].cccdHandle;
            }
            else if(descrIndex == CY_BLE_HIDS_REPORT_RRD)
            {
                returnHandle = cy_ble_hidsc[discIdx][serviceIndex].report[charIndex - CY_BLE_HIDS_REPORT].rrdHandle;
            }
            else
            {
                returnHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
        else
        {
            returnHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }

    return(returnHandle);
}


/******************************************************************************
* Function Name: Cy_BLE_HIDSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles the Error Response Event.
*
*  \param eventParam:  the pointer to the data structure specified by the event.
*
* \return
*  None
*
******************************************************************************/
static void Cy_BLE_HIDSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        if(cy_ble_hidscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] == eventParam->errInfo.attrHandle)
        {
            cy_ble_hidscReqHandle[Cy_BLE_GetDiscoveryIdx(eventParam->connHandle)] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


#endif /* CY_BLE_HIDS_CLIENT */


/******************************************************************************
* Function Name: Cy_BLE_HIDS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the HID service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_HIDS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_HIDS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_HIDSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_INCL:
                Cy_BLE_HIDSC_InclDiscoveryEventHandler((cy_stc_ble_disc_incl_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_HIDSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_HIDSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_HIDS_CLIENT */

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
        #ifdef CY_BLE_HIDS_SERVER
            case CY_BLE_EVT_GATT_CONNECT_IND:
                Cy_BLE_HIDSS_OnDeviceConnected();
                break;

            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_HIDSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_WRITE_CMD_REQ:
                (void)Cy_BLE_HIDSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;
        #endif /* CY_BLE_HIDS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_HIDS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_HIDSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_HIDSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_HIDSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_HIDSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;
        #endif /* CY_BLE_HIDS_CLIENT */
            default:
                break;
        }
    }

    return(gattErr);
}
#endif /* defined(CY_BLE_HIDS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */


/* [] END OF FILE */
