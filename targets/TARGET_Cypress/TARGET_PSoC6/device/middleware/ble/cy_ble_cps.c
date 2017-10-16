/***************************************************************************//**
* \file cy_ble_cps.c
* \version 2.0
*
* \brief
*  Contains the source code for Cycling Power Service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_CPS))

static cy_ble_callback_t Cy_BLE_CPS_ApplCallback = NULL;

#ifdef CY_BLE_CPS_SERVER
/* The internal storage for the last request handle to check response for server */
static cy_ble_gatt_db_attr_handle_t cy_ble_cpssReqHandle;

cy_stc_ble_cps_cp_adjustment_t cy_ble_cpssAdjustment;
static uint8_t cy_ble_cpssFlag;
static cy_stc_ble_gap_conn_param_updated_in_controller_t cy_ble_cpssConnParam;
#endif /* CY_BLE_CPS_SERVER */

#ifdef CY_BLE_CPS_CLIENT
/* CPS Center Service characteristics GATT DB handles structure */
cy_stc_ble_cpsc_t cy_ble_cpsc[CY_BLE_CONFIG_GATTC_COUNT];

/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_cpscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];

static uint8_t cy_ble_cpscFlag[CY_BLE_CONFIG_GATTC_COUNT];
static cy_stc_ble_timer_info_t cy_ble_cpscCpTimeout[CY_BLE_CONFIG_GATTC_COUNT];
static uint8_t cy_ble_cpscObserverFlag;
#endif /* (CY_BLE_CPS_CLIENT) */

#if CY_BLE_GAP_ROLE_BROADCASTER
cy_stc_ble_gapp_disc_param_t cy_ble_cpssBroadcastParam;
cy_stc_ble_gapp_disc_data_t cy_ble_cpssBroadcastData =    
{
    {   /* Length, FLAGS, BR/EDR NOT Supported */
        CY_BLE_GAP_ADV_FLAGS_PACKET_LENGTH, (uint8_t)CY_BLE_GAP_ADV_FLAGS, CY_BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED,

        /* Advertising Interval data type contains the advInterval value */
        CY_BLE_GAP_ADV_ADVERT_INTERVAL_PACKET_LENGTH, (uint8_t)CY_BLE_GAP_ADV_ADVERT_INTERVAL,
        CY_LO8(CY_BLE_GAP_ADV_ADVERT_INTERVAL_NONCON_MIN), CY_HI8(CY_BLE_GAP_ADV_ADVERT_INTERVAL_NONCON_MIN),

        /* The Service Data data type consists of a CPS service UUID with the Cycling Power Measurement
         * characteristic value */
        CY_BLE_CPSS_BROADCAST_DATA_LEN_MIN,                                 /* Packet length */
        (uint8_t)CY_BLE_GAP_ADV_SRVC_DATA_16UUID,                           /* Service Data */
        CY_LO8(CY_BLE_UUID_CPS_SERVICE), CY_HI8(CY_BLE_UUID_CPS_SERVICE),   /* CPS service UUID */
        0x00u, 0x00u,                                                       /* Flags */
        0x11u, 0x11u,                                                       /* Instantaneous Power */
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u
    },

    /* Advertising data length */
    CY_BLE_CPSS_BROADCAST_DATA_LEN_MIN + CY_BLE_CPSS_BROADCAST_HEADER_LEN,
};

cy_stc_ble_gapp_disc_mode_info_t cy_ble_cpssBroadcastModeInfo =
{
    CY_BLE_GAPP_NONE_DISC_BROADCAST_MODE,           /* discMode */
    &cy_ble_cpssBroadcastParam,
    &cy_ble_cpssBroadcastData,
    NULL,
    0u,                                             /* advTo */
};
#endif /* CY_BLE_GAP_ROLE_BROADCASTER */

/* The pointer on global BLE CPS Config structure */
cy_stc_ble_cps_config_t *cy_ble_cpsConfigPtr;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_CPS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_CPSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
static void Cy_BLE_CPSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_CPSS_ConnParamUpdateRspEventHandler(const cy_stc_ble_l2cap_conn_update_rsp_param_t *eventParam);
static void Cy_BLE_CPSS_ConnUpdateCompleteEventHandler(const cy_stc_ble_gap_conn_param_updated_in_controller_t
                                                       *eventParam);

#if CY_BLE_GAP_ROLE_BROADCASTER
static void Cy_BLE_CPSS_AdvertisementStartStopEventHandler(void);
#endif /* CY_BLE_GAP_ROLE_BROADCASTER */
#endif     /* CY_BLE_CPS_SERVER */

#ifdef CY_BLE_CPS_CLIENT
static void Cy_BLE_CPSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_CPSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_CPSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_CPSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_CPSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam);
static void Cy_BLE_CPSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_CPSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_CPSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
static void Cy_BLE_CPSC_ScanStartStopEventHandler(void);
static void Cy_BLE_CPSC_ScanProcessEventHandler(cy_stc_ble_gapc_adv_report_param_t *eventParam);
static void Cy_BLE_CPSC_TimeOutEventHandler(const cy_stc_ble_timeout_param_t *eventParam);
#endif /* (CY_BLE_CPS_CLIENT) */

static cy_en_ble_gatt_err_code_t Cy_BLE_CPS_EventHandler(uint32_t eventCode, void *eventParam);


#ifdef CY_BLE_CPS_SERVER
/******************************************************************************
* Function Name: Cy_BLE_CPSS_Init
***************************************************************************//**
*
*  This function initializes CPS Service in Server role.
*
******************************************************************************/
void Cy_BLE_CPSS_Init(void)
{
    cy_ble_cpssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    cy_ble_cpssFlag = 0u;
}

#endif /* CY_BLE_CPS_SERVER */

/******************************************************************************
* Function Name: Cy_BLE_CPS_Init
***************************************************************************//**
*
*  This function initializes CPS Service.
*
*  \param config: Configuration structure for CPS Service.
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
cy_en_ble_api_result_t Cy_BLE_CPS_Init(cy_stc_ble_cps_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure */
        cy_ble_cpsConfigPtr = config;

        /* Registers Event Handler for the ANCS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_CPS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_cpsConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_CPS_ApplCallback = cy_ble_cpsConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_CPS_SERVER
        Cy_BLE_CPSS_Init();
        cy_ble_cpssAdjustment.samplingRate = CY_BLE_CPS_SAMLING_RATE_DEFAULT;
    #endif /* CY_BLE_CPS_SERVER */

    #ifdef CY_BLE_CPS_CLIENT
        {
            uint32_t idx;
            cy_ble_cpscObserverFlag = 0u;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_CPS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    (void)memset(&cy_ble_cpsc[idx], 0, sizeof(cy_stc_ble_cpsc_t));

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_CPS].uuid = CY_BLE_UUID_CPS_SERVICE;
                }
                cy_ble_cpscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                cy_ble_cpscFlag[idx] = 0u;
                cy_ble_cpscCpTimeout[idx].timeout = CY_BLE_CPS_CP_PROCEDURE_TIMEOUT;
            }
        }
    #endif /* CY_BLE_CPS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CPS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc:   An application layer event callback function to receive
*                 events from the BLE Component. The definition of
*                 cy_ble_callback_t for CPS is: \n
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
cy_en_ble_api_result_t Cy_BLE_CPS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_CPS_ApplCallback = callbackFunc;
    if(cy_ble_cpsConfigPtr != NULL)
    {
        cy_ble_cpsConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_CPS_SERVER

/******************************************************************************
* Function Name: Cy_BLE_CPSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets a characteristic value of the service in the local database.
*
*  \param charIndex: The index of a service characteristic of type
*                       cy_en_ble_cps_char_index_t.
*  \param attrSize:  The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*                       stored to the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request is handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CPSS_SetCharacteristicValue(cy_en_ble_cps_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if(charIndex >= CY_BLE_CPS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Store data in database */
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_cpsConfigPtr->cpss->charInfo[charIndex].charHandle,
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
* Function Name: Cy_BLE_CPSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets a characteristic value of the service, which is a value identified by charIndex.
*
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_cps_char_index_t.
*  \param attrSize:        The size of the characteristic value attribute.
*  \param attrValue:      The pointer to the location where characteristic value data
*                   should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request is handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CPSS_GetCharacteristicValue(cy_en_ble_cps_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if(charIndex >= CY_BLE_CPS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            /* Read characteristic value from database */
        {
            .handleValuePair.attrHandle = cy_ble_cpsConfigPtr->cpss->charInfo[charIndex].charHandle,
            .handleValuePair.value.len  = attrSize,
            .handleValuePair.value.val  = attrValue,
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
* Function Name: Cy_BLE_CPSS_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sets a characteristic descriptor of a specified characteristic of the service.
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_cps_char_index_t.
*  \param descrIndex:      The index of a service characteristic descriptor of type
*                   cy_en_ble_cps_descr_index_t.
*  \param attrSize:        The size of the characteristic descriptor attribute.
*  \param attrValue:      The pointer to the descriptor value data that should
*                   be stored to the GATT database.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request is handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CPSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_cps_char_index_t charIndex,
                                                               cy_en_ble_cps_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if((charIndex >= CY_BLE_CPS_CHAR_COUNT) || (descrIndex >= CY_BLE_CPS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_cpsConfigPtr->cpss->charInfo[charIndex].descrHandle[descrIndex],
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
* Function Name: Cy_BLE_CPSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets a characteristic descriptor of a specified characteristic of the service.
*
*  \param connHandle:      The connection handle
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_cps_char_index_t.
*  \param descrIndex:      The index of a service characteristic descriptor of type
*                   cy_en_ble_cps_descr_index_t.
*  \param attrSize:        The size of the characteristic descriptor attribute.
*  \param attrValue:      The pointer to the location where characteristic descriptor
*                   value data should be stored.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CPSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_cps_char_index_t charIndex,
                                                               cy_en_ble_cps_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if((charIndex >= CY_BLE_CPS_CHAR_COUNT) || (descrIndex >= CY_BLE_CPS_DESCR_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
        {
            .handleValuePair.attrHandle = cy_ble_cpsConfigPtr->cpss->charInfo[charIndex].descrHandle[descrIndex],
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

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CPSS_WriteEventHandler
***************************************************************************//**
*
*  Handles Write Request Event for CPS service.
*
*  \param void *eventParam: The pointer to the data structure specified by the event.
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*   * CY_BLE_GATT_ERR_NONE - write is successful
*   * CY_BLE_GATT_ERR_PROCEDURE_ALREADY_IN_PROGRESS
*   * CY_BLE_GATT_ERR_CCCD_IMPROPERLY_CONFIGURED
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_CPSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    cy_en_ble_cps_char_index_t locCharIndex;
    cy_stc_ble_cps_char_value_t locCharValue = { .connHandle = eventParam->connHandle };
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;
    uint8_t locReqHandle = 0u;

    if(Cy_BLE_CPS_ApplCallback != NULL)
    {
        /* Error conditions for CP Characteristic value write request */
        if((eventParam->handleValPair.attrHandle == cy_ble_cpsConfigPtr->cpss->charInfo[CY_BLE_CPS_POWER_CP].charHandle) &&
           (!CY_BLE_IS_INDICATION_ENABLED(eventParam->connHandle.attId,
                                          cy_ble_cpsConfigPtr->cpss->charInfo[CY_BLE_CPS_POWER_CP].
                                           descrHandle[CY_BLE_CPS_CCCD])))
        {
            gattErr = CY_BLE_GATT_ERR_CCCD_IMPROPERLY_CONFIGURED;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else if((eventParam->handleValPair.attrHandle ==
                 cy_ble_cpsConfigPtr->cpss->charInfo[CY_BLE_CPS_POWER_CP].charHandle) &&
                ((cy_ble_cpssFlag & CY_BLE_CPSS_FLAG_CP_PROCEDURE_IN_PROGRESS) != 0u))
        {
            gattErr = CY_BLE_GATT_ERR_PROCEDURE_ALREADY_IN_PROGRESS;
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else
        {
            for(locCharIndex = CY_BLE_CPS_POWER_MEASURE; (locCharIndex < CY_BLE_CPS_CHAR_COUNT) && (locReqHandle == 0u);
                locCharIndex++)
            {
                if((eventParam->handleValPair.attrHandle == cy_ble_cpsConfigPtr->cpss->charInfo[locCharIndex].
                     descrHandle[CY_BLE_CPS_CCCD]) ||
                   (eventParam->handleValPair.attrHandle == cy_ble_cpsConfigPtr->cpss->charInfo[locCharIndex].
                     descrHandle[CY_BLE_CPS_SCCD]) ||
                   (eventParam->handleValPair.attrHandle == cy_ble_cpsConfigPtr->cpss->charInfo[locCharIndex].charHandle))
                {
                    /* Store value to database */
                    cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                    {
                        .handleValuePair = eventParam->handleValPair,
                        .connHandle      = eventParam->connHandle,
                        .offset          = 0u,
                        .flags           = CY_BLE_GATT_DB_PEER_INITIATED
                    };

                    /* Clear event handled flag to send Write response */
                    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                    locReqHandle = 1u;

                    locCharValue.charIndex = locCharIndex;
                    locCharValue.value = &eventParam->handleValPair.value;


                    gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);

                    if(gattErr == CY_BLE_GATT_ERR_NONE)
                    {
                        /* Characteristic value write request */
                        if(eventParam->handleValPair.attrHandle ==
                           cy_ble_cpsConfigPtr->cpss->charInfo[locCharIndex].charHandle)
                        {
                            Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSS_WRITE_CHAR, &locCharValue);

                            /* In the context of the Cycling Power Control Point characteristic, a procedure is started
                             * when a write to the Cycling Power Control Point characteristic is successfully completed
                             */
                            if(locCharIndex == CY_BLE_CPS_POWER_CP)
                            {
                                cy_ble_cpssFlag |= CY_BLE_CPSS_FLAG_CP_PROCEDURE_IN_PROGRESS;
                            }
                        }

                        /* Client Characteristic Configuration descriptor write request */
                        else if(eventParam->handleValPair.attrHandle ==
                                cy_ble_cpsConfigPtr->cpss->charInfo[locCharIndex].descrHandle[CY_BLE_CPS_CCCD])
                        {
                            /* Check characteristic properties for Notification */
                            if(CY_BLE_IS_NOTIFICATION_SUPPORTED(cy_ble_cpsConfigPtr->cpss->charInfo[locCharIndex].
                                                                 charHandle))
                            {
                                if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                                {
                                    uint16_t requiredConnIntv;
                                    uint8_t prefConnParamCharValue[CY_BLE_PPCPC_LEN];
                                    requiredConnIntv = CY_BLE_CPS_SAMLING_RATE_TO_CONN_INTV /
                                                       cy_ble_cpssAdjustment.samplingRate;

                                    /* Cycling Power Vector characteristic has special behavior
                                     * described in CPS specification, section 3.5.1 */
                                    if((locCharIndex == CY_BLE_CPS_POWER_VECTOR) &&
                                       (cy_ble_cpssConnParam.connIntv > requiredConnIntv))
                                    {
                                        /* Read characteristic value from database */
                                        cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfoCCCD =
                                        {
                                            .handleValuePair.attrHandle = cy_ble_configPtr->gaps->prefConnParamCharHandle,
                                            .handleValuePair.value.len  = sizeof(prefConnParamCharValue),
                                            .handleValuePair.value.val  = prefConnParamCharValue,
                                            .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
                                        };

                                        if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfoCCCD) ==
                                           CY_BLE_GATT_ERR_NONE)
                                        {
                                            /* If the current connection parameters do not allow sending of
                                             * notification, request new connection parameters by using the GAP
                                             * Connection Parameter Update procedure.
                                             */
                                            cy_stc_ble_gap_conn_update_param_info_t connUpdateParam =
                                            {
                                                /* Send Connection Parameter Update Request to Client */
                                                .connIntvMin   = requiredConnIntv,
                                                .connIntvMax   = requiredConnIntv,
                                                .connLatency   = Cy_BLE_Get16ByPtr(prefConnParamCharValue +
                                                                                   CY_BLE_PPCPC_SLAVE_LATENCY_OFFSET),
                                                .supervisionTO = Cy_BLE_Get16ByPtr(prefConnParamCharValue +
                                                                                   CY_BLE_PPCPC_SUP_TIMEOUT_OFFSET),
                                                .bdHandle      = eventParam->connHandle.bdHandle
                                            };

                                            if(Cy_BLE_L2CAP_LeConnectionParamUpdateRequest(&connUpdateParam)
                                               == CY_BLE_SUCCESS)
                                            {
                                                cy_ble_cpssFlag |= CY_BLE_CPSS_FLAG_PV_PROCEDURE_IN_PROGRESS;

                                                /* Set event handled flag to not send Write response. Response will be
                                                 * sent when Central accept the request for connection parameter update
                                                 * in Cy_BLE_CPSS_ConnParamUpdateRspEventHandler function.
                                                 */
                                                cy_ble_eventHandlerFlag |= CY_BLE_CALLBACK;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSS_NOTIFICATION_ENABLED,
                                                                &locCharValue);
                                    }
                                }
                                else
                                {
                                    Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSS_NOTIFICATION_DISABLED,
                                                            &locCharValue);
                                }
                            }

                            /* Check characteristic properties for Indication */
                            if(CY_BLE_IS_INDICATION_SUPPORTED(cy_ble_cpsConfigPtr->cpss->charInfo[locCharIndex].charHandle))
                            {
                                if(CY_BLE_IS_INDICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                                {
                                    Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSS_INDICATION_ENABLED, &locCharValue);
                                }
                                else
                                {
                                    Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSS_INDICATION_DISABLED, &locCharValue);
                                }
                            }
                        #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
                            (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
                            /* Set flag to store bonding data to flash */
                            if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
                            {
                                cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
                            }
                        #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */
                        }
                        /* Server Characteristic Configuration descriptor write request */
                        else
                        {
                            /* Check characteristic properties for Broadcast */
                            if(CY_BLE_IS_BROADCAST_SUPPORTED(cy_ble_cpsConfigPtr->cpss->charInfo[locCharIndex].
                                                              charHandle))
                            {
                                if(CY_BLE_IS_BROADCAST_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                                {
                                    Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSS_BROADCAST_ENABLED, &locCharValue);
                                }
                                else
                                {
                                    Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSS_BROADCAST_DISABLED, &locCharValue);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return(gattErr);
}


/******************************************************************************
* Function Name: Cy_BLE_CPSS_SendNotification
***************************************************************************//**
*
*  Sends notification with a characteristic value of the CPS, which is a value
*  specified by charIndex, to the Client device.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_CPSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle:      The connection handle
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_cps_char_index_t.
*  \param attrSize:        The size of the characteristic value attribute.
*  \param attrValue:      The pointer to the characteristic value data that should be
*                   sent to the Client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the Client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - Notification is not enabled by the Client.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CPSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_cps_char_index_t charIndex,
                                                    uint8_t attrSize,
                                                    uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send Notification if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(charIndex >= CY_BLE_CPS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_cpsConfigPtr->cpss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((!CY_BLE_IS_NOTIFICATION_SUPPORTED(cy_ble_cpsConfigPtr->cpss->charInfo[charIndex].charHandle)) ||
            (!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_cpsConfigPtr->cpss->charInfo[charIndex].
                                              descrHandle[CY_BLE_CPS_CCCD])))
    {
        apiResult = CY_BLE_ERROR_NTF_DISABLED;
    }
    else
    {
        cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
        {
            /* Fill all fields of write request structure ... */
            .handleValPair.attrHandle = cy_ble_cpsConfigPtr->cpss->charInfo[charIndex].charHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* Send notification to the Client using previously filled structure */
        apiResult = Cy_BLE_GATTS_Notification(&ntfReqParam);
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CPSS_SendIndication
***************************************************************************//**
*
*  Sends indication with a characteristic value of the CPS, which is a value
*  specified by charIndex, to the Client device.
*
*  On enabling indication successfully it sends out a 'Handle Value Indication' which
*  results in CY_BLE_EVT_CPSC_INDICATION or CY_BLE_EVT_GATTC_HANDLE_VALUE_IND (if
*  service-specific callback function is not registered) event at the GATT Client's end.
*
*  \param connHandle:      The connection handle
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_cps_char_index_t.
*  \param attrSize:        The size of the characteristic value attribute.
*  \param attrValue:      The pointer to the characteristic value data that should be
*                   sent to the Client device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - Optional characteristic is absent
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the Client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*   * CY_BLE_ERROR_IND_DISABLED - Indication is not enabled by the Client
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the CPS service-specific callback is registered
*      (with Cy_BLE_CPS_RegisterAttrCallback):
*  * CY_BLE_EVT_CPSS_INDICATION_CONFIRMED - in case if the indication is
*                                successfully delivered to the peer device.
*  .
*   Otherwise (if the CPS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF - in case if the indication is
*                                successfully delivered to the peer device.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CPSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_cps_char_index_t charIndex,
                                                  uint8_t attrSize,
                                                  uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;

    /* Send Notification if it is enabled and connected */
    if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(charIndex >= CY_BLE_CPS_CHAR_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_cpsConfigPtr->cpss->charInfo[charIndex].charHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if((!CY_BLE_IS_INDICATION_SUPPORTED(cy_ble_cpsConfigPtr->cpss->charInfo[charIndex].charHandle)) ||
            (!CY_BLE_IS_INDICATION_ENABLED(connHandle.attId, cy_ble_cpsConfigPtr->cpss->charInfo[charIndex].
                                            descrHandle[CY_BLE_CPS_CCCD])))
    {
        apiResult = CY_BLE_ERROR_IND_DISABLED;
    }
    else
    {
        cy_stc_ble_gatts_handle_value_ind_t indReqParam =
        {
            /* Fill all fields of write request structure ... */
            .handleValPair.attrHandle = cy_ble_cpsConfigPtr->cpss->charInfo[charIndex].charHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };

        /* Send indication to Client using previously filled structure */
        apiResult = Cy_BLE_GATTS_Indication(&indReqParam);
        /* Save handle to support service-specific value confirmation response from Client */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_cpssReqHandle = indReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CPSS_ConfirmationEventHandler
***************************************************************************//**
*
*  Handles a Value Confirmation request event from the BLE stack.
*
*  *eventParam - Pointer to a structure of type 'cy_stc_ble_conn_handle_t'.
*
******************************************************************************/
static void Cy_BLE_CPSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    cy_en_ble_cps_char_index_t locCharIndex;
    cy_stc_ble_cps_char_value_t locCharValue = { .connHandle = *eventParam };
    uint8_t locReqHandle = 0u;

    if((Cy_BLE_CPS_ApplCallback != NULL) && (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_cpssReqHandle))
    {
        for(locCharIndex = CY_BLE_CPS_POWER_MEASURE; (locCharIndex < CY_BLE_CPS_CHAR_COUNT) && (locReqHandle == 0u);
            locCharIndex++)
        {
            if(cy_ble_cpssReqHandle == cy_ble_cpsConfigPtr->cpss->charInfo[locCharIndex].charHandle)
            {
                locCharValue.charIndex = locCharIndex;
                locCharValue.value = NULL;
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                cy_ble_cpssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;

                /* The Cycling Power Control Point procedure is complete when the Server indicated with the Op Code
                 * set to Response Code.
                 */
                if(locCharIndex == CY_BLE_CPS_POWER_CP)
                {
                    cy_ble_cpssFlag &= (uint8_t) ~CY_BLE_CPSS_FLAG_CP_PROCEDURE_IN_PROGRESS;
                }
                Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSS_INDICATION_CONFIRMED, &locCharValue);
                locReqHandle = 1u;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CPSS_ConnParamUpdateRspEventHandler
***************************************************************************//**
*
*  Handles the L2CAP connection parameter response event from
*  the BLE stack.
*
*  \param uint16_t response:
*      * Accepted = 0x0000
*      * Rejected = 0x0001
*
******************************************************************************/
static void Cy_BLE_CPSS_ConnParamUpdateRspEventHandler(const cy_stc_ble_l2cap_conn_update_rsp_param_t *eventParam)
{
    if((Cy_BLE_CPS_ApplCallback != NULL) && ((cy_ble_cpssFlag & CY_BLE_CPSS_FLAG_PV_PROCEDURE_IN_PROGRESS) != 0u))
    {
        cy_stc_ble_conn_handle_t locConnHandle =
        {
            .bdHandle = eventParam->bdHandle
        };

        cy_ble_cpssFlag &= (uint8_t) ~CY_BLE_CPSS_FLAG_PV_PROCEDURE_IN_PROGRESS;
        if(eventParam->result != 0u)
        {
            /* If the Client does not change the connection parameters Server shall return an ATT Error Response
             * to the Write Request with an Error Code set to the Application Error Code 0x80
             * (Inappropriate Connection Parameters). */
            cy_stc_ble_gatt_err_param_t err_param =
            {
                .errInfo.opCode     = (cy_en_ble_gatt_pdu_t)CY_BLE_GATT_WRITE_REQ,
                .errInfo.attrHandle = cy_ble_cpsConfigPtr->cpss->charInfo[CY_BLE_CPS_POWER_VECTOR].
                                       descrHandle[CY_BLE_CPS_CCCD],
                .errInfo.errorCode  = CY_BLE_GATT_ERR_CPS_INAPPROPRIATE_CONNECTION_PARAMETERS,
                /* get connHandle form cy_ble_connHandle, to have actual value of connHandle.attId */
                .connHandle         = cy_ble_connHandle[Cy_BLE_GetConnHandleIdx(locConnHandle)]
            };
            /* Send Error Response */
            (void)Cy_BLE_GATTS_ErrorRsp(&err_param);
        }
        else
        {
            cy_stc_ble_cps_char_value_t locCharValue =
            {
                .connHandle = cy_ble_connHandle[Cy_BLE_GetConnHandleIdx(locConnHandle)],
                .charIndex  = CY_BLE_CPS_POWER_VECTOR
            };

            cy_stc_ble_gatt_value_t locValue;
            uint8_t cccdVal[2u];
            Cy_BLE_Set16ByPtr(cccdVal, CY_BLE_CCCD_NOTIFICATION);

            /* Otherwise, the Server shall respond with a Write Response and start sending notifications
             * of the Cycling Power Vector characteristic. */
            (void)Cy_BLE_GATTS_WriteRsp(locConnHandle);

            /* get connHandle form cy_ble_connHandle, to have actual value of connHandle.attId */
            locValue.len = CY_BLE_CCCD_LEN;
            locValue.val = cccdVal;
            locCharValue.value = &locValue;

            Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSS_NOTIFICATION_ENABLED, &locCharValue);
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CPSS_ConnUpdateCompleteEventHandler
***************************************************************************//**
*
*  Handles the CY_BLE_EVT_GAP_CONNECTION_UPDATE_COMPLETE event from
*  the BLE stack.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_CPSS_ConnUpdateCompleteEventHandler(const cy_stc_ble_gap_conn_param_updated_in_controller_t *eventParam)
{
    /* Store connection parameters */
    cy_ble_cpssConnParam = *eventParam;
}

#if CY_BLE_GAP_ROLE_BROADCASTER

/******************************************************************************
* Function Name: Cy_BLE_CPSS_StartBroadcast
***************************************************************************//**
*
*  This function is used to start broadcasting of the Cycling Power
*  Measurement characteristic or update broadcasting data when it was started
*  before.
*
*  It is available only in Broadcaster role.
*
*  \param advInterval:     Advertising interval in 625 us units. The valid range is
*                   from CY_BLE_GAP_ADV_ADVERT_INTERVAL_NONCON_MIN
*                   to CY_BLE_GAP_ADV_ADVERT_INTERVAL_MAX. This parameter is ignored
*                   when when broadcasting is already started.
*  \param attrSize:        The size of the characteristic value attribute.
*                   This size is limited by maximum advertising packet length and
*                   advertising header size.
*  \param attrValue:      The pointer to the Cycling Power Measurement characteristic
*                   that include the mandatory fields (e.g. the Flags field and the
*                   Instantaneous Power field) and depending on the Flags field,
*                   some optional fields in a non connectable undirected advertising
*                   event.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*   Error Codes                        | Description
*   ------------                       | -----------
*   CY_BLE_SUCCESS                     | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER     | On passing an invalid parameter.
*   CY_BLE_ERROR_INVALID_OPERATION     | Operation is not permitted.
*
******************************************************************************/

cy_en_ble_api_result_t Cy_BLE_CPSS_StartBroadcast(uint16_t advInterval,
                                                  uint8_t attrSize,
                                                  const uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    if((advInterval < CY_BLE_GAP_ADV_ADVERT_INTERVAL_NONCON_MIN) || (advInterval > CY_BLE_GAP_ADV_ADVERT_INTERVAL_MAX) ||
       (attrSize > (CY_BLE_GAP_MAX_ADV_DATA_LEN - CY_BLE_CPSS_BROADCAST_HEADER_LEN)))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Copy Cycling Power Measurement characteristic value to advertising data packet */
        (void)memcpy(&cy_ble_cpssBroadcastData.advData[CY_BLE_CPSS_BROADCAST_DATA_OFFSET], attrValue, (uint32_t)attrSize);
        cy_ble_cpssBroadcastData.advData[CY_BLE_CPSS_BROADCAST_DATA_LEN_OFFSET] = attrSize;
        cy_ble_cpssBroadcastData.advDataLen = attrSize + CY_BLE_CPSS_BROADCAST_HEADER_LEN;

        if((cy_ble_cpssFlag & CY_BLE_CPSS_FLAG_BROADCAST_IN_PROGRESS) == 0u)
        {
            /* Put advertising interval to advertising data packet */
            cy_ble_cpssBroadcastData.advData[CY_BLE_CPSS_BROADCAST_ADVERT_INTERVAL_OFFSET] = CY_LO8(advInterval);
            cy_ble_cpssBroadcastData.advData[CY_BLE_CPSS_BROADCAST_ADVERT_INTERVAL_OFFSET + 1u] = CY_HI8(advInterval);

            /* Configure advertising timeout, interval and type */
            cy_ble_cpssBroadcastParam.advIntvMin = advInterval;
            cy_ble_cpssBroadcastParam.advIntvMax = advInterval;
            cy_ble_cpssBroadcastModeInfo.advTo = 0u;
            cy_ble_cpssBroadcastParam.advType = CY_BLE_GAPP_NON_CONNECTABLE_UNDIRECTED_ADV;
            cy_ble_cpssBroadcastParam.advChannelMap = cy_ble_discoveryParam[cy_ble_advIndex].advChannelMap;

            /* Start broadcasting */
            apiResult = Cy_BLE_GAPP_EnterDiscoveryMode(&cy_ble_cpssBroadcastModeInfo);
            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_cpssFlag |= CY_BLE_CPSS_FLAG_START_BROADCAST;
            }
        }
        else
        {
            /* Update the advertisement packet if the device is in the advertising mode. */
            apiResult = Cy_BLE_GAPP_UpdateAdvScanData(&cy_ble_cpssBroadcastModeInfo);
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CPSS_StopBroadcast
***************************************************************************//**
*
*  This function is used to stop broadcasting of the Cycling Power
*  Measurement characteristic.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*   Error Codes                           | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*   CY_BLE_ERROR_INVALID_STATE            | On calling this function not in CPS broadcasting mode.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CPSS_StopBroadcast(void)
{
    cy_en_ble_api_result_t apiResult;

    if((cy_ble_cpssFlag & CY_BLE_CPSS_FLAG_BROADCAST_IN_PROGRESS) != 0u)
    {
        apiResult = Cy_BLE_GAPP_ExitDiscoveryMode();
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_cpssFlag |= CY_BLE_CPSS_FLAG_STOP_BROADCAST;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CPSS_AdvertisementStartStopEventHandler
***************************************************************************//**
*
*  This function handles CY_BLE_EVT_GAPP_ADVERTISEMENT_START_STOP event for CPS.
*
******************************************************************************/
static void Cy_BLE_CPSS_AdvertisementStartStopEventHandler(void)
{
    if((cy_ble_cpssFlag & CY_BLE_CPSS_FLAG_START_BROADCAST) != 0u)
    {
        cy_ble_cpssFlag |= CY_BLE_CPSS_FLAG_BROADCAST_IN_PROGRESS;
        cy_ble_cpssFlag &= (uint8_t) ~CY_BLE_CPSS_FLAG_START_BROADCAST;
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
    if((cy_ble_cpssFlag & CY_BLE_CPSS_FLAG_STOP_BROADCAST) != 0u)
    {
        cy_ble_cpssFlag &= (uint8_t) ~(CY_BLE_CPSS_FLAG_BROADCAST_IN_PROGRESS | CY_BLE_CPSS_FLAG_STOP_BROADCAST);
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}

#endif /* CY_BLE_GAP_ROLE_BROADCASTER */

#endif /* CY_BLE_CPS_SERVER */

#ifdef CY_BLE_CPS_CLIENT


/******************************************************************************
* Function Name: Cy_BLE_CPSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_CPSC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* CPS characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_cpscCharUuid[CY_BLE_CPS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_CPS_MSRMT,         /* Cycling Power Measurement characteristic UUID */
        CY_BLE_UUID_CHAR_CPS_FEATURE,       /* Cycling Power Feature characteristic UUID */
        CY_BLE_UUID_CHAR_SENSOR_LOCATION,   /* Cycling Power Sensor Location characteristic UUID */
        CY_BLE_UUID_CHAR_CPS_VECTOR,        /* Cycling Power Vector characteristic UUID */
        CY_BLE_UUID_CHAR_CPS_CP             /* Cycling Power Control Point characteristic UUID */
    };
    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_CPS))
    {
        /* Update last characteristic endHandle to declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        for(i = (uint32_t)CY_BLE_CPS_POWER_MEASURE; i < (uint32_t)CY_BLE_CPS_CHAR_COUNT; i++)
        {
            if(cy_ble_cpscCharUuid[i] == discCharInfo->uuid.uuid16)
            {
                Cy_BLE_CheckStoreCharHandle(cy_ble_cpsc[discIdx].charInfo[i]);
                lastEndHandle[discIdx] = &cy_ble_cpsc[discIdx].charInfo[i].endHandle;
                break;
            }
        }

        /* Init characteristic endHandle to the Service endHandle.
         * Characteristic endHandle will be updated to declaration
         * handler of the following characteristic,
         * in the following characteristic discovery procedure. */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range.endHandle;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CPSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a "CY_BLE_EVT_GATTC_FIND_INFO_RSP" event.
*  Based on the descriptor UUID, an appropriate data structure is populated using
*  the data received as part of the callback.
*
*  \param discoveryCharIndex:  The characteristic index which is discovered.
*  \param discDescrInfo:      The pointer to a descriptor information structure.
*
******************************************************************************/
static void Cy_BLE_CPSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t notSupportedDescr = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t descIdx;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_CPS)
    {
        switch(discDescrInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_CLIENT_CONFIG:
                descIdx = (uint32_t)CY_BLE_CPS_CCCD;
                break;

            case CY_BLE_UUID_CHAR_SERVER_CONFIG:
                descIdx = (uint32_t)CY_BLE_CPS_SCCD;
                break;

            default:
                /* Not supported descriptor */
                notSupportedDescr = 1u;
                break;
        }

        if(notSupportedDescr == 0u)
        {
            if(cy_ble_cpsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] ==
               CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_cpsc[discIdx].charInfo[cy_ble_discovery[discIdx].charCount].descrHandle[descIdx] =
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
* Function Name: Cy_BLE_CPSC_GetCharRange
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
static void Cy_BLE_CPSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_CPS)
    {
        if(charRangeInfo->srviIncIdx != CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].charCount++;
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_CPS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
            if((cy_ble_cpsc[discIdx].charInfo[charIdx].endHandle -
                cy_ble_cpsc[discIdx].charInfo[charIdx].valueHandle) != 0u)
            {
                /* Read characteristic range */
                charRangeInfo->range.startHandle = cy_ble_cpsc[discIdx].charInfo[charIdx].valueHandle + 1u;
                charRangeInfo->range.endHandle = cy_ble_cpsc[discIdx].charInfo[charIdx].endHandle;
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
* Function Name: Cy_BLE_CPSC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_CPSS_WRITE_CHAR events is generated.
*  On successful request execution on the Server side the Write Response is
*  sent to the Client.
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_cps_char_index_t.
*  \param attrSize:        The size of the characteristic value attribute.
*  \param attrValue:      The pointer to the characteristic value data that
*                   should be send to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic
*  * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                     characteristic
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the CPS service-specific callback is registered
*      (with Cy_BLE_CPS_RegisterAttrCallback):
*  * CY_BLE_EVT_CPSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_cps_char_value_t.
*  .
*   Otherwise (if the CPS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CPSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_cps_char_index_t charIndex,
                                                          uint8_t attrSize,
                                                          uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_CPS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_cpsc[discIdx].charInfo[charIndex].valueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_cpsc[discIdx].charInfo[charIndex].valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };

        apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);

        /* Save handle to support service-specific write response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_cpscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
            /* Start CP procedure. It will ends when the Collector sends a confirmation to acknowledge the CP
             * indication sent by the CP Sensor. A procedure is considered to have timed out if a CP indication
             * is not received within the ATT transaction timeout, defined as 30 seconds.
             * CY_BLE_EVT_CPSC_TIMEOUT event with cy_stc_ble_cps_char_value_t parameter will indicate about CP
             * procedure timeout.
             */
            if(charIndex == CY_BLE_CPS_POWER_CP)
            {
                apiResult = Cy_BLE_StartTimer(&cy_ble_cpscCpTimeout[discIdx]);
                if(apiResult == CY_BLE_SUCCESS)
                {
                    cy_ble_cpscFlag[discIdx] |= CY_BLE_CPSC_FLAG_CP_PROCEDURE_IN_PROGRESS;
                }
            }
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CPSC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read a characteristic value, which is a value identified by
*  charIndex, from the server.
*
*  The Read Response returns the characteristic Value in the Attribute Value
*  parameter.
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_cps_char_index_t.
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
*   If the CPS service-specific callback is registered
*      (with Cy_BLE_CPS_RegisterAttrCallback):
*  * CY_BLE_EVT_CPSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_cps_char_value_t.
*  .
*   Otherwise (if the CPS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_CPSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_cps_char_index_t charIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_CPS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_cpsc[discIdx].charInfo[charIndex].valueHandle != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_cpsc[discIdx].charInfo[charIndex].valueHandle,
            .connHandle = connHandle,
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_cpscReqHandle[discIdx] = cy_ble_cpsc[discIdx].charInfo[charIndex].valueHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CPSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  This function is used to write the characteristic descriptor to the server
*  which is identified by charIndex.
*
*  Internally, Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server side the following events can be
*  generated:
*  * CY_BLE_EVT_CPSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_CPSS_NOTIFICATION_DISABLED
*  * CY_BLE_EVT_CPSS_INDICATION_ENABLED
*  * CY_BLE_EVT_CPSS_INDICATION_DISABLED
*  * CY_BLE_EVT_CPSS_BROADCAST_ENABLED
*  * CY_BLE_EVT_CPSS_BROADCAST_DISABLED
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type  cy_en_ble_cps_char_index_t.
*  \param descrIndex:      The index of a service characteristic descriptor of type cy_en_ble_cps_descr_index_t.
*  \param attrSize:        The size of the characteristic value attribute.
*  \param attrValue:      The pointer to the characteristic descriptor value data that
*                   should be sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the CPS service-specific callback is registered
*      (with Cy_BLE_CPS_RegisterAttrCallback):
*  * CY_BLE_EVT_CPSC_WRITE_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_cps_descr_value_t.
*  .
*   Otherwise (if the CPS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CPSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_cps_char_index_t charIndex,
                                                               cy_en_ble_cps_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_CPS_CHAR_COUNT) || (descrIndex >= CY_BLE_CPS_DESCR_COUNT) ||
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            /* Fill all fields of write request structure ... */
            .handleValPair.attrHandle = cy_ble_cpsc[discIdx].charInfo[charIndex].descrHandle[descrIndex],
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };

        /* ... and send request to server device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_cpscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CPSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Sends a request to get the characteristic descriptor of the specified
*  characteristic of the service.
*
*  \param connHandle:      The connection handle.
*  \param charIndex:       The index of a service characteristic of type
*                   cy_en_ble_cps_char_index_t.
*  \param descrIndex:      The index of a service characteristic descriptor of type
*                   cy_en_ble_cps_descr_index_t.
*
* \return
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the CPS service-specific callback is registered
*      (with Cy_BLE_CPS_RegisterAttrCallback):
*  * CY_BLE_EVT_CPSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_cps_descr_value_t.
*  .
*  Otherwise (if the CPS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_CPSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_cps_char_index_t charIndex,
                                                               cy_en_ble_cps_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_CPS_CHAR_COUNT) || (descrIndex >= CY_BLE_CPS_DESCR_COUNT) ||
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_cpsc[discIdx].charInfo[charIndex].descrHandle[descrIndex],
            .connHandle = connHandle
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_cpscReqHandle[discIdx] = cy_ble_cpsc[discIdx].charInfo[charIndex].descrHandle[descrIndex];
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CPSC_TimeOutEventHandler
***************************************************************************//**
*
*  Handles Timer Event.
*
*  \param eventParam:  The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_CPSC_TimeOutEventHandler(const cy_stc_ble_timeout_param_t *eventParam)
{
    if(((eventParam)->reasonCode == CY_BLE_GENERIC_APP_TO) && (Cy_BLE_CPS_ApplCallback != NULL))
    {
        uint32_t idx;
        for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
        {
            if(((cy_ble_cpscFlag[idx] & CY_BLE_CPSC_FLAG_CP_PROCEDURE_IN_PROGRESS) != 0u) &&
               (eventParam->timerHandle == cy_ble_cpscCpTimeout[idx].timerHandle))
            {
                cy_stc_ble_cps_char_value_t timeoutValue =
                {
                    .connHandle = cy_ble_connHandle[cy_ble_discovery[idx].connIndex],
                    .charIndex  = CY_BLE_CPS_POWER_CP
                };

                cy_ble_cpscFlag[idx] &= (uint8_t) ~CY_BLE_CPSC_FLAG_CP_PROCEDURE_IN_PROGRESS;
                cy_ble_cpscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;

                Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSC_TIMEOUT, &timeoutValue);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CPSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles Notification Event.
*
*  \param eventParam:  The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_CPSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    cy_en_ble_cps_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_CPS_ApplCallback != NULL))
    {
        for(locCharIndex = CY_BLE_CPS_POWER_MEASURE; locCharIndex < CY_BLE_CPS_CHAR_COUNT; locCharIndex++)
        {
            if(cy_ble_cpsc[discIdx].charInfo[locCharIndex].valueHandle == eventParam->handleValPair.attrHandle)
            {
                cy_stc_ble_cps_char_value_t notifValue =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = locCharIndex,
                    .value      = &eventParam->handleValPair.value
                };

                Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSC_NOTIFICATION, &notifValue);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CPSC_IndicationEventHandler
***************************************************************************//**
*
*  Handles Indication Event.
*
*  \param eventParam:  The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_CPSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam)
{
    cy_en_ble_cps_char_index_t locCharIndex;
    cy_stc_ble_cps_char_value_t indicationValue = { .connHandle = eventParam->connHandle };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_CPS_ApplCallback != NULL))
    {
        for(locCharIndex = CY_BLE_CPS_POWER_MEASURE; locCharIndex < CY_BLE_CPS_CHAR_COUNT; locCharIndex++)
        {
            if(cy_ble_cpsc[discIdx].charInfo[locCharIndex].valueHandle == eventParam->handleValPair.attrHandle)
            {
                /* Stop the timer. CP procedure is finished when received indication sent by the CP Sensor. */
                if((cy_ble_cpscReqHandle[discIdx] == cy_ble_cpsc[discIdx].charInfo[CY_BLE_CPS_POWER_CP].valueHandle) &&
                   ((cy_ble_cpscFlag[discIdx] & CY_BLE_CPSC_FLAG_CP_PROCEDURE_IN_PROGRESS) != 0u))
                {
                    (void)Cy_BLE_StopTimer(&cy_ble_cpscCpTimeout[discIdx]);
                    cy_ble_cpscFlag[discIdx] &= (uint8_t) ~CY_BLE_CPSC_FLAG_CP_PROCEDURE_IN_PROGRESS;
                }

                indicationValue.charIndex = locCharIndex;
                indicationValue.value = &eventParam->handleValPair.value;

                Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSC_INDICATION, &indicationValue);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CPSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles Read Response Event.
*
*  \param eventParam:  The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_CPSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t locReqHandle = 0u;
    cy_en_ble_cps_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_CPS_ApplCallback != NULL) &&
       (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_cpscReqHandle[discIdx]))
    {
        for(locCharIndex = CY_BLE_CPS_POWER_MEASURE; (locCharIndex < CY_BLE_CPS_CHAR_COUNT) && (locReqHandle == 0u);
            locCharIndex++)
        {
            if(cy_ble_cpscReqHandle[discIdx] == cy_ble_cpsc[discIdx].charInfo[locCharIndex].valueHandle)
            {
                cy_stc_ble_cps_char_value_t locCharValue =
                {
                    .connHandle = eventParam->connHandle,
                    .charIndex  = locCharIndex,
                    .value      = &eventParam->value
                };
                cy_ble_cpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSC_READ_CHAR_RESPONSE, &locCharValue);
                locReqHandle = 1u;
            }
            else
            {
                cy_en_ble_cps_descr_index_t locDescIndex;

                for(locDescIndex = CY_BLE_CPS_CCCD; (locDescIndex < CY_BLE_CPS_DESCR_COUNT) &&
                    (locReqHandle == 0u); locDescIndex++)
                {
                    if(cy_ble_cpscReqHandle[discIdx] == cy_ble_cpsc[discIdx].charInfo[locCharIndex].
                        descrHandle[locDescIndex])
                    {
                        cy_stc_ble_cps_descr_value_t locDescrValue =
                        {
                            .connHandle = eventParam->connHandle,
                            .charIndex  = locCharIndex,
                            .descrIndex = locDescIndex,
                            .value      = &eventParam->value
                        };

                        cy_ble_cpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                        Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSC_READ_DESCR_RESPONSE, &locDescrValue);
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
* Function Name: Cy_BLE_CPSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles Write Response Event.
*
*  \param eventParam:  The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_CPSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint8_t locReqHandle = 0u;
    cy_en_ble_cps_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_CPS_ApplCallback != NULL) &&
       (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_cpscReqHandle[discIdx]))
    {
        for(locCharIndex = CY_BLE_CPS_POWER_MEASURE; (locCharIndex < CY_BLE_CPS_CHAR_COUNT) && (locReqHandle == 0u);
            locCharIndex++)
        {
            if(cy_ble_cpscReqHandle[discIdx] == cy_ble_cpsc[discIdx].charInfo[locCharIndex].valueHandle)
            {
                cy_stc_ble_cps_char_value_t locCharValue =
                {
                    .connHandle = *eventParam,
                    .charIndex  = locCharIndex,
                    .value      = NULL
                };

                cy_ble_cpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSC_WRITE_CHAR_RESPONSE, &locCharValue);
                locReqHandle = 1u;
            }
            else
            {
                cy_en_ble_cps_descr_index_t locDescIndex;

                for(locDescIndex = CY_BLE_CPS_CCCD; (locDescIndex < CY_BLE_CPS_DESCR_COUNT) &&
                    (locReqHandle == 0u); locDescIndex++)
                {
                    if(cy_ble_cpscReqHandle[discIdx] == cy_ble_cpsc[discIdx].charInfo[locCharIndex].
                        descrHandle[locDescIndex])
                    {
                        cy_stc_ble_cps_descr_value_t locDescrValue =
                        {
                            .connHandle = *eventParam,
                            .charIndex  = locCharIndex,
                            .descrIndex = locDescIndex,
                            .value      = NULL
                        };

                        cy_ble_cpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                        Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSC_WRITE_DESCR_RESPONSE, &locDescrValue);
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
* Function Name: Cy_BLE_CPSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles Error Response Event.
*
*  \param eventParam:  The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_CPSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
        if(eventParam->errInfo.attrHandle == cy_ble_cpscReqHandle[discIdx])
        {
            cy_ble_cpscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}

/******************************************************************************
* Function Name: Cy_BLE_CPSC_StartObserve
***************************************************************************//**
*
*   This function is used for observing GAP peripheral devices.
*   A device performing the observer role receives only advertisement data from
*   devices irrespective of their discoverable mode settings. Advertisement
*   data received is provided by the event, CY_BLE_EVT_CPSC_SCAN_PROGRESS_RESULT.
*   This procedure sets the scanType sub parameter to passive scanning.
*
*   If 'scanTo' sub-parameter is set to zero value, then passive scanning
*   procedure will continue until you call Cy_BLE_CPSC_StopObserve().
*   Possible generated events are:
*   * CY_BLE_EVT_CPSC_SCAN_PROGRESS_RESULT
*
*  \param scanParamIndex:  The index of the scan configuration.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*   Error Codes                           | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter for
*                                         |   'scanInfo' or if any element within 'scanInfo' has an invalid value.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CPSC_StartObserve(uint8_t scanParamIndex)
{
    cy_en_ble_api_result_t apiResult;
    cy_stc_ble_gapc_disc_info_t locDiscoveryInfo =
    {
        .discProcedure    = CY_BLE_GAPC_OBSER_PROCEDURE,
        .scanType         = CY_BLE_GAPC_PASSIVE_SCANNING,
        .scanIntv         = cy_ble_configPtr->gapcScanParams[scanParamIndex].fastScanInterval,
        .scanWindow       = cy_ble_configPtr->gapcScanParams[scanParamIndex].fastScanWindow,
        .ownAddrType      = CY_BLE_GAP_ADDR_TYPE_PUBLIC,
        .scanFilterPolicy = CY_BLE_GAPC_ADV_ACCEPT_ALL_PKT,
        .scanTo           = 0u,           /* Disable Timeout */
        .filterDuplicates = CY_BLE_GAPC_FILTER_DUP_DISABLE
    };

    apiResult = Cy_BLE_GAPC_StartDiscovery(&locDiscoveryInfo);

    if(apiResult == CY_BLE_SUCCESS)
    {
        cy_ble_cpscObserverFlag |= CY_BLE_CPSC_FLAG_START_OBSERVE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CPSC_StopObserve
***************************************************************************//**
*
*   This function used to stop the discovery of devices.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*   Error Codes                           | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*   CY_BLE_ERROR_INVALID_STATE            | On calling this function not in CPS observing state.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_CPSC_StopObserve(void)
{
    cy_en_ble_api_result_t apiResult;

    if((cy_ble_cpscObserverFlag & CY_BLE_CPSC_FLAG_OBSERVE_IN_PROGRESS) != 0u)
    {
        apiResult = Cy_BLE_GAPC_StopDiscovery();
        cy_ble_cpscObserverFlag |= CY_BLE_CPSC_FLAG_STOP_OBSERVE;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_CPSC_ScanStartStopEventHandler
***************************************************************************//**
*
*  This function handles CY_BLE_EVT_GAPC_SCAN_START_STOP event for CPS.
*
******************************************************************************/
static void Cy_BLE_CPSC_ScanStartStopEventHandler(void)
{
    if((cy_ble_cpscObserverFlag & CY_BLE_CPSC_FLAG_START_OBSERVE) != 0u)
    {
        cy_ble_cpscObserverFlag |= CY_BLE_CPSC_FLAG_OBSERVE_IN_PROGRESS;
        cy_ble_cpscObserverFlag &= (uint8_t) ~CY_BLE_CPSC_FLAG_START_OBSERVE;
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
    if((cy_ble_cpscObserverFlag & CY_BLE_CPSC_FLAG_STOP_OBSERVE) != 0u)
    {
        cy_ble_cpscObserverFlag &= (uint8_t) ~(CY_BLE_CPSC_FLAG_OBSERVE_IN_PROGRESS | CY_BLE_CPSC_FLAG_STOP_OBSERVE);
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_CPSC_ScanProcessEventHandler
***************************************************************************//**
*
*  This function handles CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT event for CPS.
*
*  \param eventParam:  the pointer to a data structure specified by the event.
*
******************************************************************************/
static void Cy_BLE_CPSC_ScanProcessEventHandler(cy_stc_ble_gapc_adv_report_param_t *eventParam)
{
    if((cy_ble_cpscObserverFlag & CY_BLE_CPSC_FLAG_OBSERVE_IN_PROGRESS) != 0u)
    {
        if(eventParam->eventType == CY_BLE_GAPC_NON_CONN_UNDIRECTED_ADV)
        {
            if(Cy_BLE_CPS_ApplCallback != NULL)
            {
                cy_stc_ble_cps_char_value_t broadcastValue;
                cy_stc_ble_gatt_value_t locCharValue;
                uint8_t advIndex = 0u;
                do
                {
                    /* Show Cycling Power Measurement characteristic value from Service Data packet. */
                    if((eventParam->data[advIndex] > CY_BLE_CPSC_BROADCAST_DATA_OFFSET) &&
                       (eventParam->data[advIndex + 1u] == (uint8_t)CY_BLE_GAP_ADV_SRVC_DATA_16UUID))
                    {
                        broadcastValue.peerAddrType = eventParam->peerAddrType;
                        broadcastValue.peerBdAddr = eventParam->peerBdAddr;
                        broadcastValue.charIndex = CY_BLE_CPS_POWER_MEASURE;
                        broadcastValue.value = &locCharValue;
                        locCharValue.val = &eventParam->data[advIndex + CY_BLE_CPSC_BROADCAST_DATA_OFFSET];
                        locCharValue.len = (uint16_t)eventParam->data[advIndex] - (CY_BLE_CPSC_BROADCAST_DATA_OFFSET - 1u);
                        Cy_BLE_CPS_ApplCallback((uint32_t)CY_BLE_EVT_CPSC_SCAN_PROGRESS_RESULT, &broadcastValue);
                        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                    }
                    advIndex += eventParam->data[advIndex] + 1u;
                }
                while(advIndex < eventParam->dataLen);
            }
        }
        else    /* Filter for all connectable advertising packets */
        {
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}

#endif /* (CY_BLE_CPS_CLIENT) */


/******************************************************************************
* Function Name: Cy_BLE_CPS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the Cycling Power Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_CPS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_CPS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_CPSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_CPSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_CPSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_CPS_CLIENT */

            default:
                break;
        }
    }
    else
    {
        switch((cy_en_ble_event_t)eventCode)
        {
            case CY_BLE_EVT_GATT_DISCONNECT_IND:
        #ifdef CY_BLE_CPS_SERVER
            #if CY_BLE_GAP_ROLE_BROADCASTER
                (void)Cy_BLE_CPSS_DisconnectEventHandler();
            #endif /* CY_BLE_GAP_ROLE_BROADCASTER */
        #endif     /* CY_BLE_CPS_SERVER */

        #ifdef CY_BLE_CPS_CLIENT
                (void)Cy_BLE_CPSC_DisconnectEventHandler();
        #endif /* CY_BLE_ _CLIENT */
                break;

                /**************************************************************************
                 * Handling GATT Server Events
                 ***************************************************************************/
        #ifdef CY_BLE_CPS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_CPSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF:
                Cy_BLE_CPSS_ConfirmationEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;

            case CY_BLE_EVT_GATT_CONNECT_IND:
                Cy_BLE_CPSS_ConnectEventHandler();
                break;

            case CY_BLE_EVT_L2CAP_CONN_PARAM_UPDATE_RSP:
                Cy_BLE_CPSS_ConnParamUpdateRspEventHandler((cy_stc_ble_l2cap_conn_update_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GAP_CONNECTION_UPDATE_COMPLETE:
                Cy_BLE_CPSS_ConnUpdateCompleteEventHandler((cy_stc_ble_gap_conn_param_updated_in_controller_t*)eventParam);
                break;

            case CY_BLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
                #if (CY_BLE_GAP_ROLE_BROADCASTER)
                Cy_BLE_CPSS_AdvertisementStartStopEventHandler();
                #endif /* CY_BLE_GAP_ROLE_BROADCASTER */
                break;
        #endif         /* CY_BLE_CPS_SERVER */

                /**************************************************************************
                 * Handling GATT Client Events
                 ***************************************************************************/
        #ifdef CY_BLE_CPS_CLIENT
            case CY_BLE_EVT_GAPC_SCAN_START_STOP:
                Cy_BLE_CPSC_ScanStartStopEventHandler();
                break;

            case CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT:
                Cy_BLE_CPSC_ScanProcessEventHandler((cy_stc_ble_gapc_adv_report_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_CPSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_CPSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_IND:
                Cy_BLE_CPSC_IndicationEventHandler((cy_stc_ble_gattc_handle_value_ind_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_CPSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_CPSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;

            case CY_BLE_EVT_TIMEOUT:
                Cy_BLE_CPSC_TimeOutEventHandler((cy_stc_ble_timeout_param_t*)eventParam);
                break;
        #endif /* CY_BLE_CPS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_CPS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */

