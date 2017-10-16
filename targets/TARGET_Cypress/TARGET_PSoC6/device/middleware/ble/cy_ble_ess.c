/***************************************************************************//**
* \file cy_ble_ess.c
* \version 2.0
*
* \brief
*  Contains the source code for the Environmental Sensing Service.
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

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_ESS))

#ifdef CY_BLE_ESS_SERVER
/* The internal storage for the last request handle to check response for server */
static cy_ble_gatt_db_attr_handle_t cy_ble_esssReqHandle;
#endif /* CY_BLE_ESS_SERVER */

#ifdef CY_BLE_ESS_CLIENT
/* The internal storage for the last request handle to check response */
static cy_ble_gatt_db_attr_handle_t cy_ble_esscReqHandle[CY_BLE_CONFIG_GATTC_COUNT];

/* Read Long Descriptors variables */
static uint8_t * cy_ble_esscRdLongBuffPtr[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
static uint32_t cy_ble_esscRdLongBuffLen[CY_BLE_CONFIG_GATTC_COUNT] = { 0u };
static uint8_t cy_ble_esscCurrLen[CY_BLE_CONFIG_GATTC_COUNT] = { 0u };

uint8_t activeCharIndex[CY_BLE_CONFIG_GATTC_COUNT] = { 0u };
uint8_t activeCharInstance[CY_BLE_CONFIG_GATTC_COUNT] = { 0u };
uint8_t prevCharInstIndex[CY_BLE_CONFIG_GATTC_COUNT];

#endif /* (CY_BLE_ESS_CLIENT) */

static cy_ble_callback_t Cy_BLE_ESS_ApplCallback = NULL;

/* The pointer on global BLE ESS Config structure */
cy_stc_ble_ess_config_t *cy_ble_essConfigPtr = NULL;

/***************************************
* Private Function Prototypes
***************************************/
#ifdef CY_BLE_ESS_SERVER
static cy_en_ble_gatt_err_code_t Cy_BLE_ESSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
static void Cy_BLE_ESSS_PrepareWriteRequestEventHandler(const cy_stc_ble_gatts_prep_write_req_param_t *eventParam);
static void Cy_BLE_ESSS_ExecuteWriteRequestEventHandler(cy_stc_ble_gatts_exec_write_req_t *eventParam);
static void Cy_BLE_ESSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
#endif /* CY_BLE_ESS_SERVER */

#ifdef CY_BLE_ESS_CLIENT
static void Cy_BLE_ESSC_DiscoverCharacteristicsEventHandler(const cy_stc_ble_disc_char_info_t *discCharInfo);
static void Cy_BLE_ESSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
static void Cy_BLE_ESSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
static void Cy_BLE_ESSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam);
static void Cy_BLE_ESSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam);
static void Cy_BLE_ESSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_ESSC_ReadLongRespEventHandler(const cy_stc_ble_gattc_read_rsp_param_t *eventParam);
static void Cy_BLE_ESSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam);
static void Cy_BLE_ESSC_ExecuteWriteResponseEventHandler(const cy_stc_ble_gattc_exec_write_rsp_t *eventParam);
static void Cy_BLE_ESSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
#endif /* CY_BLE_ESS_CLIENT */
static cy_en_ble_gatt_err_code_t Cy_BLE_ESS_EventHandler(uint32_t eventCode, void *eventParam);


/******************************************************************************
* Function Name: Cy_BLE_ESS_Init
***************************************************************************//**
*
*  This function initializes the Environmental Sensing Service.
*
*  \param config: Configuration structure for the Environmental Sensing Service.
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
cy_en_ble_api_result_t Cy_BLE_ESS_Init(cy_stc_ble_ess_config_t *config)
{
    cy_en_ble_api_result_t apiResult;

    if(config == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Registers a pointer to config structure*/
        cy_ble_essConfigPtr = config;

        /* Registers Event Handler for the ESS Service */
        apiResult = Cy_BLE_RegisterServiceEventHandler(&Cy_BLE_ESS_EventHandler);

        /* Registers a callback function via config structure */
        if(cy_ble_essConfigPtr->callbackFunc != NULL)
        {
            Cy_BLE_ESS_ApplCallback = cy_ble_essConfigPtr->callbackFunc;
        }

    #ifdef CY_BLE_ESS_SERVER
        cy_ble_esssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
    #endif /* CY_BLE_ESS_SERVER */

    #ifdef CY_BLE_ESS_CLIENT
        {
            uint32_t idx;
            for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
            {
                uint32_t i;
                uint32_t j;

                if(cy_ble_serverInfo[idx][CY_BLE_SRVI_ESS].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    for(i = (uint32_t)CY_BLE_ESS_DESCRIPTOR_VALUE_CHANGED; (i < ((uint32_t)CY_BLE_ESS_CHAR_COUNT)); i++)
                    {
                        for(j = 0u; j < cy_ble_essConfigPtr->esscCharInstances[i]; j++)
                        {
                            (void)memset(&cy_ble_essConfigPtr->essc[idx].charInfoAddr[i].charInfoPtr[j], 0,
                                         sizeof(cy_stc_ble_essc_char_t));
                        }
                    }

                    /* initialize uuid  */
                    cy_ble_serverInfo[idx][CY_BLE_SRVI_ESS].uuid = CY_BLE_UUID_ENVIRONMENTAL_SENSING_SERVICE;
                }
                cy_ble_esscRdLongBuffPtr[idx] = NULL;
                cy_ble_esscRdLongBuffLen[idx] = 0u;
                cy_ble_esscCurrLen[idx] = 0u;
                activeCharIndex[idx] = 0u;
                activeCharInstance[idx] = 0u;
                prevCharInstIndex[idx] = 0u;
                cy_ble_esscReqHandle[idx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            }
        }
    #endif /* CY_BLE_ESS_CLIENT */
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ESS_RegisterAttrCallback
***************************************************************************//**
*
*  Registers a callback function for service-specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
*
*  \param callbackFunc:  An application layer event callback function to receive
*                 events from the BLE Component. The definition of
*                 cy_ble_callback_t for ESS Service is:\n
*                 typedef void (* cy_ble_callback_t) (uint32_t eventCode,
*                                                    void *eventParam)
*                 * eventCode:  Indicates the event that triggered this callback
*                   (e.g. CY_BLE_EVT_ESSS_NOTIFICATION_ENABLED).
*                 * eventParam: Contains the parameters corresponding to the
*                   current event. (e.g. Pointer to cy_stc_ble_ess_char_value_t
*                   structure that contains details of the characteristic
*                   for which the notification enabled event was triggered).
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
cy_en_ble_api_result_t Cy_BLE_ESS_RegisterAttrCallback(cy_ble_callback_t callbackFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    
    Cy_BLE_ESS_ApplCallback = callbackFunc;
    if(cy_ble_essConfigPtr != NULL)
    {
        cy_ble_essConfigPtr->callbackFunc = callbackFunc;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    
    return(apiResult);
}


#ifdef CY_BLE_ESS_SERVER

/******************************************************************************
* Function Name: Cy_BLE_ESS_Get24ByPtr
***************************************************************************//**
*
*  Returns a three-bytes value by using a pointer to the LSB. The value is
*  returned as uint32_t.
*
*  \param ptr: The pointer to the LSB of three-byte data (little-endian).
*
* \return
*  uint32_t value: Three-byte data.
*
******************************************************************************/
uint32_t Cy_BLE_ESS_Get24ByPtr(const uint8_t ptr[])
{
    return(((uint32_t)ptr[0u]) | ((uint32_t)(((uint32_t)ptr[1u]) << 8u)) | ((uint32_t)((uint32_t)ptr[2u]) << 16u));
}


/******************************************************************************
* Function Name: Cy_BLE_ESS_Get32ByPtr
***************************************************************************//**
*
*  Returns a four-byte value by using a pointer to the LSB.
*
*  \param ptr: The pointer to the LSB of four-byte data (little-endian).
*
* \return
*  uint32_t value: Four-byte data.
*
******************************************************************************/
uint32_t Cy_BLE_ESS_Get32ByPtr(const uint8_t ptr[])
{
    return(((uint32_t)ptr[0u]) | ((uint32_t)(((uint32_t)ptr[1u]) << 8u)) | ((uint32_t)((uint32_t)ptr[2u]) << 16u) |
           ((uint32_t)((uint32_t)ptr[3u]) << 24u));
}


/******************************************************************************
* Function Name: Cy_BLE_ESSS_CheckIfInRange
***************************************************************************//**
*
*  Checks if the "value" is in the range of "min" and "max". As the ESS
*  characteristics operate with signed or unsigned value types and the value in
*  the GATT database are stored in unsigned variables. Need to clearly identify
*  the signedness of each "value", "min" and "max". After that the values can
*  be properly compared. The signedness of each parameter is encoded in "state"
*  and has the following meaning:
*     State 0: All three are positive (all three are of unsigned type)
*     State 1: Min - positive, Max - positive, Value - negative
*     State 2: Min - positive, Max - negative, Value - positive
*     State 3: Min - positive, Max - negative, Value - negative
*     State 4: Min - negative, Max - positive, Value - positive
*     State 5: Min - negative, Max - positive, Value - negative
*     State 6: Min - negative, Max - negative, Value - positive
*     State 7: All three are negative
*
*  min - Minimum inclusive limit.
*  max - Minimum inclusive limit.
*  value - The value to be checked if it is in the range.
*  state - The state as shown above.
*
* \return
*  A return value is of type cy_en_ble_gatt_err_code_t.
*   * CY_BLE_GATT_ERR_NONE - "value" is in the range.
*   * CY_BLE_GATT_ERR_OUT_OF_RANGE - "value" is not in the range.
*
******************************************************************************/
cy_en_ble_gatt_err_code_t Cy_BLE_ESSS_CheckIfInRange(uint32_t min,
                                                     uint32_t max,
                                                     uint32_t value,
                                                     uint8_t state)
{
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;

    switch(state)
    {
        /* All values are positive */
        case CY_BLE_ESS_STATE_0:
            if((min > value) || (max < value))
            {
                gattErr = CY_BLE_GATT_ERR_OUT_OF_RANGE;
            }
            break;

        /* "Not in range" cases. Min and Max are negative and value is positive or
         * vice versa.
         */
        case CY_BLE_ESS_STATE_1:
        case CY_BLE_ESS_STATE_6:
            gattErr = CY_BLE_GATT_ERR_OUT_OF_RANGE;
            break;

        /* Min is negative, Max and value are positive */
        case CY_BLE_ESS_STATE_4:
            if(max < value)
            {
                gattErr = CY_BLE_GATT_ERR_OUT_OF_RANGE;
            }
            break;

        /* Max is positive, Min and value are negative */
        case CY_BLE_ESS_STATE_5:
            if(min > value)
            {
                gattErr = CY_BLE_GATT_ERR_OUT_OF_RANGE;
            }
            break;

        /* All values are negative */
        case CY_BLE_ESS_STATE_7:
            if((min > value) || (max < value))
            {
                gattErr = CY_BLE_GATT_ERR_OUT_OF_RANGE;
            }
            break;

        /* Invalid cases. Handled by customizer. */
        case CY_BLE_ESS_STATE_2:
        case CY_BLE_ESS_STATE_3:
        default:
            gattErr = CY_BLE_GATT_ERR_OUT_OF_RANGE;
            break;
    }

    return(gattErr);
}


/******************************************************************************
* Function Name: Cy_BLE_ESS_HandleRangeCheck
***************************************************************************//**
*
*  Performs an extraction of the characteristic value ranges then compares it to
*  the value received from the client and returns a result of the comparison.
*
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param charInstance: The instance number of the characteristic specified by
*                "charIndex".
*  \param length: The length of a buffer to store the main and may ranges. Can be 2,4,6
*          or 8 bytes.
*  \param type: Identifies the type of the value pointed by "pValue" (0 - the value
*        is of unsigned type, 1 - the value is of signed type).
*  \param pValue: The pointer to the value to be validated if it is in the range.
*
*
* \return
*  A return value is of type cy_en_ble_gatt_err_code_t.
*   * CY_BLE_GATT_ERR_NONE - The value stored in "pValue" is in ranges specified
*                           by Valid Range Descriptor of the characteristic
*                           addressed by "charIndex" and "charInstance".
*   * CY_BLE_GATT_ERR_UNLIKELY_ERROR - Failed to read Valid Range Descriptor
*                                     value.
*   * CY_BLE_GATT_ERR_OUT_OF_RANGE - The value stored in "pValue" is not in the
*                                   valid ranges.
*
******************************************************************************/
cy_en_ble_gatt_err_code_t Cy_BLE_ESS_HandleRangeCheck(cy_stc_ble_conn_handle_t connHandle,
                                                      cy_en_ble_ess_char_index_t charIndex,
                                                      uint8_t charInstance,
                                                      uint16_t length,
                                                      uint8_t type,
                                                      const uint8_t pValue[])
{
    uint8_t locState = 0u;
    uint8_t skipSignCheck = 0u;
    uint8_t tmpBuff[8u];
    uint32_t maxLimit = 0u;
    uint32_t minLimit = 0u;
    uint32_t operand = 0u;
    uint32_t u32Sign = 0u;
    cy_en_ble_gatt_err_code_t apiResult = CY_BLE_GATT_ERR_UNLIKELY_ERROR;

    if(Cy_BLE_ESSS_GetCharacteristicDescriptor(connHandle, charIndex, charInstance, CY_BLE_ESS_VRD, length, tmpBuff) ==
       CY_BLE_SUCCESS)
    {
        switch(length)
        {
            case CY_BLE_ESS_2BYTES_LENGTH:
                operand = (uint32_t)(pValue[0u]);
                minLimit = (uint32_t)(tmpBuff[0u]);
                maxLimit = (uint32_t)(tmpBuff[1u]);
                u32Sign = CY_BLE_ESS_U8_SIGN_BIT;
                break;

            case CY_BLE_ESS_4BYTES_LENGTH:
                operand = (uint32_t)Cy_BLE_ESS_Get16ByPtr(&pValue[0u]);
                minLimit = (uint32_t)Cy_BLE_ESS_Get16ByPtr(&tmpBuff[0u]);
                maxLimit = (uint32_t)Cy_BLE_ESS_Get16ByPtr(&tmpBuff[2u]);
                u32Sign = CY_BLE_ESS_U16_SIGN_BIT;
                break;

            case CY_BLE_ESS_6BYTES_LENGTH:
                operand = (uint32_t)Cy_BLE_ESS_Get24ByPtr(&pValue[0u]);
                minLimit = (uint32_t)Cy_BLE_ESS_Get24ByPtr(&tmpBuff[0u]);
                maxLimit = (uint32_t)Cy_BLE_ESS_Get24ByPtr(&tmpBuff[3u]);
                u32Sign = CY_BLE_ESS_U24_SIGN_BIT;
                break;

            case CY_BLE_ESS_8BYTES_LENGTH:
                operand = (uint32_t)Cy_BLE_ESS_Get32ByPtr(&pValue[0u]);
                minLimit = (uint32_t)Cy_BLE_ESS_Get32ByPtr(&tmpBuff[0u]);
                maxLimit = (uint32_t)Cy_BLE_ESS_Get32ByPtr(&tmpBuff[4u]);
                break;

            default:
                skipSignCheck = 1u;
                break;
        }

        if(skipSignCheck == 0u)
        {
            if(type != CY_BLE_ESS_UNSIGNED_TYPE)
            {
                locState = CY_BLE_ESS_IS_NEGATIVE(minLimit, u32Sign) << 1u;
                locState = (locState | CY_BLE_ESS_IS_NEGATIVE(maxLimit, u32Sign)) << 1u;
                locState |= CY_BLE_ESS_IS_NEGATIVE(operand, u32Sign);
            }
            else
            {
                locState = 0u;
            }
        }

        /* Compare the operand to the limits with respect to the state
         * and return the results.
         */
        apiResult = Cy_BLE_ESSS_CheckIfInRange(minLimit, maxLimit, operand, locState);
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ESSS_WriteEventHandler
***************************************************************************//**
*
*  Handles a Write Request event for Environmental Sensing Service.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
* \return
*  A return value is of type cy_en_ble_gatt_err_code_t.
*   * CY_BLE_GATT_ERR_NONE - Write is successful.
*   * CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED - Request is not supported.
*   * CY_BLE_GATT_ERR_INVALID_HANDLE - 'handleValuePair.attrHandle' is not valid.
*   * CY_BLE_GATT_ERR_WRITE_NOT_PERMITTED - Write operation is not permitted on
*                                          this attribute.
*   * CY_BLE_GATT_ERR_INVALID_OFFSET - the offset value is invalid.
*   * CY_BLE_GATT_ERR_UNLIKELY_ERROR - Some other error occurred.
*   * CY_BLE_GATT_ERR_CONDITION_NOT_SUPPORTED - The condition in ES Trigger Settings
*                                              Descriptor is not supported.
*   * CY_BLE_GATT_ERR_WRITE_REQ_REJECTED - A write request was rejected.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_ESSS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam)
{
    uint32_t event = (uint32_t)CY_BLE_EVT_ESSS_DESCR_WRITE;
    uint32_t foundItem = 0u;
    uint8_t i;
    uint8_t j;
    cy_stc_ble_esss_char_t *essCharInfoPtr;
    cy_stc_ble_ess_descr_value_t wrDescrReqParam = { .connHandle = eventParam->connHandle };
    cy_ble_gatt_db_attr_handle_t tmpHandle;
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;

    tmpHandle = eventParam->handleValPair.attrHandle;

    if(Cy_BLE_ESS_ApplCallback != NULL)
    {
        wrDescrReqParam.descrIndex = CY_BLE_ESS_DESCR_COUNT;

        /* Go through all possible Service characteristics.
         * Exit the handler in following conditions:
         * 1) No more characteristic left;
         * 2) Characteristic or descriptor was successfully written;
         * 3) Error occurred while writing characteristic or descriptor.
         */
        for(i = 0u; ((i < (uint8_t)CY_BLE_ESS_CHAR_COUNT) && (foundItem == 0u) && (gattErr == CY_BLE_GATT_ERR_NONE)); i++)
        {
            /* Check if characteristic is enabled. If the pointer to the characteristic
             * is not "NULL", there is at least one instance of the characteristic in
             * the ES Service.
             */
            if(cy_ble_essConfigPtr->esss->charInfoAddr[i].charInfoPtr != NULL)
            {
                for(j = 0u; j < cy_ble_essConfigPtr->esssCharInstances[i]; j++)
                {
                    essCharInfoPtr = &cy_ble_essConfigPtr->esss->charInfoAddr[i].charInfoPtr[j];

                    /* Client Characteristic Configuration Descriptor Write Request handling */
                    if(tmpHandle == essCharInfoPtr->descrHandle[CY_BLE_ESS_CCCD])
                    {
                        /* Verify that optional notification property is enabled for characteristic */
                        if(CY_BLE_IS_NOTIFICATION_SUPPORTED(essCharInfoPtr->charHandle))
                        {
                            if(CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                            {
                                event = (uint32_t)CY_BLE_EVT_ESSS_NOTIFICATION_ENABLED;
                            }
                            else
                            {
                                event = (uint32_t)CY_BLE_EVT_ESSS_NOTIFICATION_DISABLED;
                            }

                            /* Value is NULL for descriptors */
                            wrDescrReqParam.value = NULL;
                            wrDescrReqParam.descrIndex = CY_BLE_ESS_CCCD;
                            foundItem = CY_BLE_ESS_DESCRIPTOR_ITEM;

                        #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
                            (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
                            /* Set flag to store bonding data to flash */
                            if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
                            {
                                cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
                            }
                        #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */
                        }
                        else if(CY_BLE_IS_INDICATION_SUPPORTED(essCharInfoPtr->charHandle))
                        {
                            if(CY_BLE_IS_INDICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                            {
                                event = (uint32_t)CY_BLE_EVT_ESSS_INDICATION_ENABLED;
                            }
                            else
                            {
                                event = (uint32_t)CY_BLE_EVT_ESSS_INDICATION_DISABLED;
                            }

                            /* Value is NULL for descriptors */
                            wrDescrReqParam.value = NULL;
                            wrDescrReqParam.descrIndex = CY_BLE_ESS_CCCD;
                            foundItem = CY_BLE_ESS_DESCRIPTOR_ITEM;

                        #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && \
                            (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
                            /* Set flag to store bonding data to flash */
                            if(cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING)
                            {
                                cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
                            }
                        #endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */
                        }
                        else
                        {
                            gattErr = (cy_en_ble_gatt_err_code_t)CY_BLE_GATT_ERR_WRITE_REQ_REJECTED;
                        }
                    }
                    else if(tmpHandle == essCharInfoPtr->descrHandle[CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR1])
                    {
                        wrDescrReqParam.descrIndex = CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR1;
                        foundItem = CY_BLE_ESS_DESCRIPTOR_ITEM;
                    }
                    else if(tmpHandle == essCharInfoPtr->descrHandle[CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR2])
                    {
                        wrDescrReqParam.descrIndex = CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR2;
                        foundItem = CY_BLE_ESS_DESCRIPTOR_ITEM;
                    }
                    else if(tmpHandle == essCharInfoPtr->descrHandle[CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR3])
                    {
                        wrDescrReqParam.descrIndex = CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR3;
                        foundItem = CY_BLE_ESS_DESCRIPTOR_ITEM;
                    }
                    else if(tmpHandle == essCharInfoPtr->descrHandle[CY_BLE_ESS_ES_CONFIG_DESCR])
                    {
                        wrDescrReqParam.descrIndex = CY_BLE_ESS_ES_CONFIG_DESCR;
                        foundItem = CY_BLE_ESS_DESCRIPTOR_ITEM;
                    }
                    else if(tmpHandle == essCharInfoPtr->descrHandle[CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR])
                    {
                        wrDescrReqParam.descrIndex = CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR;
                        foundItem = CY_BLE_ESS_DESCRIPTOR_ITEM;
                    }
                    else
                    {
                        /* No handle match was found */
                    }

                    /* Verify if requested handle was found and successfully handled */
                    if((gattErr == CY_BLE_GATT_ERR_NONE) && (0u != foundItem))
                    {
                        switch(wrDescrReqParam.descrIndex)
                        {
                            case CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR1:
                            case CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR2:
                            case CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR3:
                                if(eventParam->handleValPair.value.val[0u] > CY_BLE_ESS_TRIG_WHILE_EQUAL_NOT_TO)
                                {
                                    /* Trigger condition is not supported */
                                    gattErr = (cy_en_ble_gatt_err_code_t)CY_BLE_GATT_ERR_CONDITION_NOT_SUPPORTED;
                                }
                                /* If Valid Range Descriptor for the characteristic is present, then
                                 * check the characteristic value range. The ranges should be properly
                                 * set in the server or otherwise the server will constantly return an
                                 * "Out of range" error.
                                 */
                                else if((CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE !=
                                         essCharInfoPtr->descrHandle[CY_BLE_ESS_VRD]) &&
                                        (eventParam->handleValPair.value.len > 1u) &&
                                        (eventParam->handleValPair.value.val[0u] >= CY_BLE_ESS_TRIG_WHILE_LESS_THAN))
                                {
                                    /* The following "switch" groups characteristics are based on their value
                                     * type.
                                     */
                                    switch((cy_en_ble_ess_char_index_t)i)
                                    {
                                        /* uint8_t */
                                        case CY_BLE_ESS_BAROMETRIC_PRESSURE_TREND:
                                        case CY_BLE_ESS_GUST_FACTOR:
                                        case CY_BLE_ESS_UV_INDEX:
                                            gattErr = Cy_BLE_ESS_HandleRangeCheck(eventParam->connHandle,
                                                                                  (cy_en_ble_ess_char_index_t)i, j,
                                                                                  CY_BLE_ESS_2BYTES_LENGTH,
                                                                                  CY_BLE_ESS_UNSIGNED_TYPE,
                                                                                  &eventParam->handleValPair.value.val[1u]);
                                            break;

                                        /* uint16_t */
                                        case CY_BLE_ESS_APPARENT_WIND_DIR:
                                        case CY_BLE_ESS_APPARENT_WIND_SPEED:
                                        case CY_BLE_ESS_HUMIDITY:
                                        case CY_BLE_ESS_IRRADIANCE:
                                        case CY_BLE_ESS_MAGNETIC_DECLINATION:
                                        case CY_BLE_ESS_RAINFALL:
                                        case CY_BLE_ESS_TRUE_WIND_DIR:
                                        case CY_BLE_ESS_TRUE_WIND_SPEED:
                                            gattErr = Cy_BLE_ESS_HandleRangeCheck(eventParam->connHandle,
                                                                                  (cy_en_ble_ess_char_index_t)i, j,
                                                                                  CY_BLE_ESS_4BYTES_LENGTH,
                                                                                  CY_BLE_ESS_UNSIGNED_TYPE,
                                                                                  &eventParam->handleValPair.value.val[1u]);
                                            break;

                                        /* uint24 */
                                        case CY_BLE_ESS_POLLEN_CONCENTRATION:
                                            gattErr = Cy_BLE_ESS_HandleRangeCheck(eventParam->connHandle,
                                                                                  (cy_en_ble_ess_char_index_t)i, j,
                                                                                  CY_BLE_ESS_6BYTES_LENGTH,
                                                                                  CY_BLE_ESS_UNSIGNED_TYPE,
                                                                                  &eventParam->handleValPair.value.val[1u]);
                                            break;

                                        /* uint32_t */
                                        case CY_BLE_ESS_PRESSURE:
                                            gattErr = Cy_BLE_ESS_HandleRangeCheck(eventParam->connHandle,
                                                                                  (cy_en_ble_ess_char_index_t)i, j,
                                                                                  CY_BLE_ESS_8BYTES_LENGTH,
                                                                                  CY_BLE_ESS_UNSIGNED_TYPE,
                                                                                  &eventParam->handleValPair.value.val[1u]);
                                            break;

                                        /* int8_t */
                                        case CY_BLE_ESS_DEW_POINT:
                                        case CY_BLE_ESS_HEAT_INDEX:
                                        case CY_BLE_ESS_WIND_CHILL:
                                            gattErr = Cy_BLE_ESS_HandleRangeCheck(eventParam->connHandle,
                                                                                  (cy_en_ble_ess_char_index_t)i, j,
                                                                                  CY_BLE_ESS_2BYTES_LENGTH,
                                                                                  CY_BLE_ESS_SIGNED_TYPE,
                                                                                  &eventParam->handleValPair.value.val[1u]);
                                            break;

                                        /* int16_t */
                                        case CY_BLE_ESS_TEMPERATURE:
                                            gattErr = Cy_BLE_ESS_HandleRangeCheck(eventParam->connHandle,
                                                                                  (cy_en_ble_ess_char_index_t)i, j,
                                                                                  CY_BLE_ESS_4BYTES_LENGTH,
                                                                                  CY_BLE_ESS_SIGNED_TYPE,
                                                                                  &eventParam->handleValPair.value.val[1u]);
                                            break;

                                        /* int24 */
                                        case CY_BLE_ESS_ELEVATION:
                                            gattErr = Cy_BLE_ESS_HandleRangeCheck(eventParam->connHandle,
                                                                                  (cy_en_ble_ess_char_index_t)i, j,
                                                                                  CY_BLE_ESS_6BYTES_LENGTH,
                                                                                  CY_BLE_ESS_SIGNED_TYPE,
                                                                                  &eventParam->handleValPair.value.val[1u]);
                                            break;

                                        case CY_BLE_ESS_MAGNETIC_FLUX_DENSITY_2D:
                                            /* No validation required */
                                            break;

                                        case CY_BLE_ESS_MAGNETIC_FLUX_DENSITY_3D:
                                            /* No validation required */
                                            break;

                                        default:
                                            /* Invalid characteristic */
                                            gattErr = CY_BLE_GATT_ERR_UNLIKELY_ERROR;
                                            break;
                                    }
                                }
                                else /* No error */
                                {
                                    if(CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN(
                                           essCharInfoPtr->descrHandle[wrDescrReqParam.descrIndex]) >=
                                       eventParam->handleValPair.value.len)
                                    {
                                        /* Set new length of the descriptor value */
                                        CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(
                                            essCharInfoPtr->descrHandle[wrDescrReqParam.descrIndex],
                                            eventParam->handleValPair.value.len);
                                    }
                                    else
                                    {
                                        gattErr = CY_BLE_GATT_ERR_INVALID_ATTRIBUTE_LEN;
                                    }
                                }
                                break;

                            case CY_BLE_ESS_ES_CONFIG_DESCR:
                                if(eventParam->handleValPair.value.val[0u] > CY_BLE_ESS_CONF_BOOLEAN_OR)
                                {
                                    /* Trigger Logic value is not supported */
                                    gattErr = (cy_en_ble_gatt_err_code_t)CY_BLE_GATT_ERR_WRITE_REQ_REJECTED;
                                }
                                break;

                            case CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR:
                                /* The ESS spec. states are the following: "The Server may also choose to reject
                                 * a write request to the Characteristic User Description if it determines that
                                 * the contents of the new value are unsuitable, such as a string containing
                                 * characters in a language that the implementation does not support."
                                 * This validation is omitted in the current version of the service.
                                 */

                                if(CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN(
                                       essCharInfoPtr->descrHandle[CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR]) >=
                                   eventParam->handleValPair.value.len)
                                {
                                    /* Set new length of the descriptor value */
                                    CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(
                                        essCharInfoPtr->descrHandle[CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR],
                                        eventParam->handleValPair.value.len);
                                }
                                else
                                {
                                    gattErr = CY_BLE_GATT_ERR_INVALID_ATTRIBUTE_LEN;
                                }
                                break;

                            default:
                                break;
                        }

                        /* Check is there no error conditions happen yet */
                        if(gattErr == CY_BLE_GATT_ERR_NONE)
                        {
                            gattErr = Cy_BLE_GATT_DbCheckPermission(eventParam->handleValPair.attrHandle,
                                                                    &eventParam->connHandle, CY_BLE_GATT_DB_WRITE |
                                                                    CY_BLE_GATT_DB_PEER_INITIATED);

                            if(gattErr == CY_BLE_GATT_ERR_NONE)
                            {
                                /* Fill data and pass it to user */
                                wrDescrReqParam.charIndex = (cy_en_ble_ess_char_index_t)i;
                                wrDescrReqParam.charInstance = j;
                                wrDescrReqParam.gattErrorCode = CY_BLE_GATT_ERR_NONE;

                                if(wrDescrReqParam.descrIndex != CY_BLE_ESS_CCCD)
                                {
                                    /* Check if descriptor index is not CCCD index as "event" and
                                     * "wrDescrReqParam.value" parameters for CCCD were handled
                                     * above.
                                     */
                                    wrDescrReqParam.value = &eventParam->handleValPair.value;
                                }
                                Cy_BLE_ESS_ApplCallback(event, &wrDescrReqParam);

                                if(wrDescrReqParam.gattErrorCode == CY_BLE_GATT_ERR_NONE)
                                {
                                    /* Write value to GATT database */
                                    cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
                                    {
                                        .handleValuePair = eventParam->handleValPair,
                                        .connHandle      = eventParam->connHandle,
                                        .offset          = 0u,
                                        .flags           = CY_BLE_GATT_DB_PEER_INITIATED
                                    };
                                    gattErr = Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo);
                                }
                            }
                        }

                        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                        break;
                    }
                }
            }
        }
    }

    if(CY_BLE_GATT_ERR_NONE != gattErr)
    {
        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }

    return(gattErr);
}


/******************************************************************************
* Function Name: Cy_BLE_ESSS_PrepareWriteRequestEventHandler
***************************************************************************//**
*
*  Handles the Write Request Event for the Environmental Sensing Service.
*
*  \param eventParam: The pointer to the data that received with a prepare write
*               request event for the  Environmental Sensing Service.
*
******************************************************************************/
static void Cy_BLE_ESSS_PrepareWriteRequestEventHandler(const cy_stc_ble_gatts_prep_write_req_param_t *eventParam)
{
    uint32_t i;
    uint32_t j;
    uint8_t exitLoop = 0u;
    cy_stc_ble_esss_char_t *essCharInfoPtr;

    if(Cy_BLE_ESS_ApplCallback != NULL)
    {
        for(i = 0u; ((i < ((uint8_t)CY_BLE_ESS_CHAR_COUNT)) && (exitLoop == 0u)); i++)
        {
            /* Check if characteristic is enabled. If the pointer to the characteristic
             * is not "NULL", there is at least one instance of the characteristic in
             * the ES Service.
             */
            if(cy_ble_essConfigPtr->esss->charInfoAddr[i].charInfoPtr != NULL)
            {
                for(j = 0u; ((j < cy_ble_essConfigPtr->esssCharInstances[i]) && (exitLoop == 0u)); j++)
                {
                    essCharInfoPtr = &cy_ble_essConfigPtr->esss->charInfoAddr[i].charInfoPtr[j];

                    /* Need to check following conditions: 1) if requested handle is the handle of
                     * Characteristic User Description Descriptor; 2) and if there is no active
                     * requests; 3) or there is an active request but the requested handle is
                     * different handle. the thord condition means that previous request resulted
                     * with an error so need to hanle that.
                     */
                    if(eventParam->baseAddr[eventParam->currentPrepWriteReqCount - 1u].handleValuePair.attrHandle ==
                       essCharInfoPtr->descrHandle[CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR])
                    {
                        if(cy_ble_esssReqHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                        {
                            /* Send Prepare Write Response which identifies acknowledgment for
                             * long characteristic value write.
                             */
                            cy_ble_esssReqHandle =
                                eventParam->baseAddr[eventParam->currentPrepWriteReqCount - 1u].handleValuePair.attrHandle;
                        }
                        /* Indicate that request was handled */
                        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;

                        /* Set the flag to exit the loop */
                        exitLoop = 1u;
                    }
                }
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ESSS_ExecuteWriteRequestEventHandler
***************************************************************************//**
*
*  Handles the Execute Write Request Event for the Environmental Sensing Service.
*
*  \param eventParam: The pointer to the data that came with a write request for the
*              Environmental Sensing Service.
*
******************************************************************************/
static void Cy_BLE_ESSS_ExecuteWriteRequestEventHandler(cy_stc_ble_gatts_exec_write_req_t *eventParam)
{
    uint8_t i;
    uint8_t j;
    uint8_t locCount;
    uint8_t exitLoop = 0u;
    uint16_t locLength = 0u;
    cy_stc_ble_gatt_value_t locDescrValue;
    cy_stc_ble_ess_descr_value_t wrDescrReqParam = { .connHandle = eventParam->connHandle };

    if(Cy_BLE_ESS_ApplCallback != NULL)
    {
        for(i = 0u; ((i < ((uint8_t)CY_BLE_ESS_CHAR_COUNT)) && (exitLoop == 0u)); i++)
        {
            /* Check if characteristic is enabled. If the pointer to the characteristic
             * is not "NULL", there is at least one instance of the characteristic in
             * the ES Service.
             */
            if(cy_ble_essConfigPtr->esss->charInfoAddr[i].charInfoPtr != NULL)
            {
                for(j = 0u; ((j < cy_ble_essConfigPtr->esssCharInstances[i]) && (exitLoop == 0u)); j++)
                {
                    if((eventParam->baseAddr[0u].handleValuePair.attrHandle ==
                        cy_ble_essConfigPtr->esss->charInfoAddr[i].charInfoPtr[j].
                         descrHandle[CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR]) &&
                       (cy_ble_esssReqHandle == eventParam->baseAddr[0u].handleValuePair.attrHandle))
                    {
                        /* Check the execWriteFlag before execute or cancel write long operation */
                        if(eventParam->execWriteFlag == CY_BLE_GATT_EXECUTE_WRITE_EXEC_FLAG)
                        {
                            for(locCount = 0u; locCount < eventParam->prepWriteReqCount; locCount++)
                            {
                                locLength += eventParam->baseAddr[locCount].handleValuePair.value.len;
                            }

                            if(CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN(cy_ble_esssReqHandle) >= locLength)
                            {
                                locDescrValue = eventParam->baseAddr[0u].handleValuePair.value;
                                /* Fill data and pass it to user */
                                wrDescrReqParam.charIndex = (cy_en_ble_ess_char_index_t)i;
                                wrDescrReqParam.charInstance = j;
                                wrDescrReqParam.gattErrorCode = CY_BLE_GATT_ERR_NONE;
                                wrDescrReqParam.value = &locDescrValue;
                                wrDescrReqParam.value->len = locLength;
                                wrDescrReqParam.descrIndex = CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR;

                                Cy_BLE_ESS_ApplCallback((uint32_t)CY_BLE_EVT_ESSS_DESCR_WRITE, &wrDescrReqParam);

                                if(wrDescrReqParam.gattErrorCode == CY_BLE_GATT_ERR_NONE)
                                {
                                    CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(cy_ble_esssReqHandle, locLength);
                                }
                            }
                            else
                            {
                                wrDescrReqParam.gattErrorCode = CY_BLE_GATT_ERR_INVALID_ATTRIBUTE_LEN;
                            }

                            /* Pass user error code to Stack */
                            eventParam->gattErrorCode = (uint8_t)wrDescrReqParam.gattErrorCode;
                        }

                        /* Set the flag to exit the loop */
                        exitLoop = 1u;

                        /* Indicate that request was handled */
                        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;

                        /* Clear requested handle */
                        cy_ble_esssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                    }
                }
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ESSS_ConfirmationEventHandler
***************************************************************************//**
*
*  Handles a Value Confirmation request event from the BLE stack.
*
*  eventParam - The pointer to a structure of type cy_stc_ble_conn_handle_t.
*
******************************************************************************/
static void Cy_BLE_ESSS_ConfirmationEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    if((Cy_BLE_ESS_ApplCallback != NULL) && (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_esssReqHandle))
    {
        /* Only Descriptor Value Change Characteristic has Indication property.
         * Check if the requested handle is the handle of Descriptor Value Change
         * handle.
         */
        if(cy_ble_esssReqHandle ==
           cy_ble_essConfigPtr->esss->charInfoAddr[CY_BLE_ESS_DESCRIPTOR_VALUE_CHANGED].charInfoPtr[0u].charHandle)
        {
            /* Fill in event data and inform Application about
             * successfully confirmed indication.
             */
            cy_stc_ble_ess_char_value_t locCharValue =
            {
                .connHandle = *eventParam,
                .charIndex  = CY_BLE_ESS_DESCRIPTOR_VALUE_CHANGED,
                .value      = NULL
            };
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            cy_ble_esssReqHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
            Cy_BLE_ESS_ApplCallback((uint32_t)CY_BLE_EVT_ESSS_INDICATION_CONFIRMATION, &locCharValue);
        }
    }
}


#if (CY_BLE_GAP_ROLE_PERIPHERAL)

/******************************************************************************
* Function Name: Cy_BLE_ESSS_SetChangeIndex
***************************************************************************//**
*
*  Performs write operation of two-byte pseudo-random change index to the
*  advertisement packet. The "Service Data" field should be selected in the
*  component customizer GUI and contain a two-byte initial change index value
*  and in opposite case the function will always return
*  "CY_BLE_ERROR_INVALID_OPERATION".
*
*  This function must be called when Cy_BLE_GetBleSsState() returns
*  CY_BLE_BLESS_STATE_EVENT_CLOSE state.
*
*  \param essIndex: A two-byte pseudo-random change index to be written to the
*            advertisement data.
*
*  \param advertisingParamIndex: The index of the peripheral and broadcast
*                                 configuration in customizer. For example:
*   *CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX     0x00
*   *CY_BLE_BROADCASTER_CONFIGURATION_0_INDEX    0x01
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - On NULL pointer, Data length in input
*        parameter exceeds 31 bytes.
*  * CY_BLE_ERROR_INVALID_OPERATION - The change index is not present in the
*       advertisement data or its length is not equal to two bytes or ADV
*       Event is not closed, BLESS is active or ADV is not enabled.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ESSS_SetChangeIndex(uint16_t essIndex,
                                                  uint8_t advertisingParamIndex)
{
    uint32_t fFlag = 0u;
    uint8_t adLength = 0u;
    uint32_t byteCounter = 0u;
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_OPERATION;

    if(advertisingParamIndex >= CY_BLE_GAPP_CONF_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        while((byteCounter < cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advData->advDataLen) &&
              (fFlag == 0u))
        {
            adLength = cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advData->advData[byteCounter];

            if(adLength != 0u)
            {
                uint16_t servUuid;

                /* Increment byte counter so that it points to AD type field */
                byteCounter++;

                servUuid = Cy_BLE_Get16ByPtr(&cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advData->
                                              advData[byteCounter + 1u]);

                if((cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advData->advData[byteCounter] ==
                    CY_BLE_AD_TYPE_SERVICE_DATA) && (servUuid == CY_BLE_UUID_ENVIRONMENTAL_SENSING_SERVICE))
                {
                    /* Start of a "Service Data" AD type was found. Set flag and exit the loop. */
                    fFlag = 1u;
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
            /* Check if length is proper */
            if(adLength == CY_BLE_ESS_SERVICE_DATA_LENGTH)
            {
                /* Increment byte counter so that it points to data value */
                byteCounter += CY_BLE_AD_SERVICE_DATA_OVERHEAD;

                Cy_BLE_Set16ByPtr(&cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].
                                   advData->advData[byteCounter], essIndex);

                /* We are done. Indicate success. */
                apiResult = CY_BLE_SUCCESS;

                if((Cy_BLE_GetAdvertisementState() == CY_BLE_ADV_STATE_ADVERTISING) &&
                   (cy_ble_advIndex == advertisingParamIndex))
                {
                    /* Update the advertisement packet if the device is in the advertising mode. */
                    apiResult = Cy_BLE_GAPP_UpdateAdvScanData(&cy_ble_configPtr->discoveryModeInfo[cy_ble_advIndex]);
                }
            }
        }
    }

    return(apiResult);
}

#endif /* (CY_BLE_GAP_ROLE_PERIPHERAL) */


/******************************************************************************
* Function Name: Cy_BLE_ESSS_SetCharacteristicValue
***************************************************************************//**
*
*  Sets the characteristic value of the service in the local database.
*
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param charInstance: The instance number of the characteristic specified by
*                "charIndex".
*  \param attrSize: The size (in Bytes) of the characteristic value attribute.
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
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ESSS_SetCharacteristicValue(cy_en_ble_ess_char_index_t charIndex,
                                                          uint8_t charInstance,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    /* Validate range of ESS characteristics */
    if((charIndex < CY_BLE_ESS_CHAR_COUNT) && (charInstance < cy_ble_essConfigPtr->esssCharInstances[charIndex]))
    {
        /* Check if requested characteristic is present in Service.
         * There are three conditions which should be checked: 1) The pointer to
         * "cy_stc_ble_esss_char_t" is not NULL, 2) The handle of the characteristic is
         * a valid handle, 3) The requested instance is a valid characteristic
         * instance.
         */
        if((cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].charInfoPtr != NULL) &&
           (cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].charInfoPtr[charInstance].charHandle !=
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
        {
            /* Store data in database */
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].
                                               charInfoPtr[charInstance].charHandle,
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
        else
        {
            apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ESSS_GetCharacteristicValue
***************************************************************************//**
*
*  Gets the characteristic value of the service, which is a value identified by
*  charIndex.
*
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param charInstance: The instance number of the characteristic specified by
*                "charIndex".
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
cy_en_ble_api_result_t Cy_BLE_ESSS_GetCharacteristicValue(cy_en_ble_ess_char_index_t charIndex,
                                                          uint8_t charInstance,
                                                          uint8_t attrSize,
                                                          uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex < CY_BLE_ESS_CHAR_COUNT) && (charInstance < cy_ble_essConfigPtr->esssCharInstances[charIndex]))
    {
        /* Check if requested characteristic is present in Service. There are three
         * conditions to be checked: 1) The pointer to "cy_stc_ble_esss_char_t" record in
         * the cy_ble_esss struct is not NULL, 2) The handle of the characteristic is a
         * valid handle, 3) The requested instance is a valid characteristic instance.
         */
        if((cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].charInfoPtr != NULL) &&
           (cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].charInfoPtr[charInstance].charHandle !=
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
        {
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].
                                               charInfoPtr[charInstance].charHandle,
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED,
            };
            /* Read characteristic value from database */
            if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_SUCCESS;

                /* Set new length of the descriptor value */
                CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].
                                                      charInfoPtr[charInstance].charHandle, attrSize);
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
* Function Name: Cy_BLE_ESSS_SetCharacteristicDescriptor
***************************************************************************//**
*
*  Sets the characteristic descriptor of the specified characteristic.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*  \param charInstance: The instance number of the characteristic specified by
*                "charIndex".
*  \param descrIndex: The index of the service characteristic descriptor of type
*              cy_en_ble_ess_descr_index_t.
*  \param attrSize: The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the descriptor value data to
*             be stored in the GATT database.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is
*                                               absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ESSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ess_char_index_t charIndex,
                                                               uint8_t charInstance,
                                                               cy_en_ble_ess_descr_index_t descrIndex,
                                                               uint16_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex < CY_BLE_ESS_CHAR_COUNT) && (charInstance < cy_ble_essConfigPtr->esssCharInstances[charIndex]) &&
       (descrIndex < CY_BLE_ESS_DESCR_COUNT))
    {
        /* Check if requested descriptor is present in Service.
         * There are three conditions to be checked: 1) The pointer to
         * "cy_stc_ble_esss_char_t" is not NULL, 2) The handle of the characteristic is
         * a valid handle, 3) The handle of the descriptor is a valid handle.
         */
        if((cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].charInfoPtr != NULL) &&
           (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE !=
            cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].charInfoPtr[charInstance].charHandle) &&
           (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].
             charInfoPtr[charInstance].descrHandle[descrIndex]))
        {
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].
                                               charInfoPtr[charInstance].descrHandle[descrIndex],
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .connHandle                 = connHandle,
                .offset                     = 0u,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };
            /* Read characteristic value from database */
            if(Cy_BLE_GATTS_WriteAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_SUCCESS;

                /* Set new length of the descriptor value */
                CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].
                                                      charInfoPtr[charInstance].descrHandle[descrIndex], attrSize);
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
* Function Name: Cy_BLE_ESSS_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Gets the characteristic descriptor of the specified characteristic.
*
*  \param connHandle: The connection handle
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param charInstance: The instance number of the characteristic specified by
*                "charIndex".
*  \param descrIndex: The index of the service characteristic descriptor of type
*              cy_en_ble_ess_descr_index_t.
*  \param attrSize: The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the location where characteristic descriptor
*              value data should be stored.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request handled successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is
*                                               absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ESSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ess_char_index_t charIndex,
                                                               uint8_t charInstance,
                                                               cy_en_ble_ess_descr_index_t descrIndex,
                                                               uint16_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex < CY_BLE_ESS_CHAR_COUNT) && (charInstance < cy_ble_essConfigPtr->esssCharInstances[charIndex]) &&
       (descrIndex < CY_BLE_ESS_DESCR_COUNT))
    {
        /* Check if requested descriptor is present in Service.
         * There are three conditions to be checked: 1) The pointer to
         * "cy_stc_ble_esss_char_t" is not NULL, 2) The handle of the characteristic is
         * a valid handle, 3) The handle of the descriptor is a valid handle.
         */
        if((cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].charInfoPtr != NULL) &&
           (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE !=
            cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].charInfoPtr[charInstance].charHandle) &&
           (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE !=
            cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].charInfoPtr[charInstance].descrHandle[descrIndex]))
        {
            cy_stc_ble_gatts_db_attr_val_info_t dbAttrValInfo =
            {
                .handleValuePair.attrHandle = cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].
                                               charInfoPtr[charInstance].descrHandle[descrIndex],
                .handleValuePair.value.len  = attrSize,
                .handleValuePair.value.val  = attrValue,
                .connHandle                 = connHandle,
                .flags                      = CY_BLE_GATT_DB_LOCALLY_INITIATED
            };
            if(Cy_BLE_GATTS_ReadAttributeValueCCCD(&dbAttrValInfo) == CY_BLE_GATT_ERR_NONE)
            {
                apiResult = CY_BLE_SUCCESS;
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
* Function Name: Cy_BLE_ESSS_SendNotification
***************************************************************************//**
*
*  Sends a notification with a characteristic value of the Environmental Sensing
*  Service, which is a value specified by charIndex, to the client's device.
*
*  On enabling notification successfully for a service characteristic it sends out a
*  'Handle Value Notification' which results in CY_BLE_EVT_ESSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param charInstance: The instance number of the characteristic specified by
*                "charIndex".
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the client's device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_NTF_DISABLED - A notification is not enabled by the client.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is
*                                               absent.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ESSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_ess_char_index_t charIndex,
                                                    uint8_t charInstance,
                                                    uint8_t attrSize,
                                                    uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex < CY_BLE_ESS_CHAR_COUNT) && (charInstance < cy_ble_essConfigPtr->esssCharInstances[charIndex]))
    {
        if((cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].charInfoPtr != NULL) &&
           (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].
             charInfoPtr[charInstance].descrHandle[CY_BLE_ESS_CCCD]))
        {
            /* Send notification if it is enabled and connected */
            if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
            {
                apiResult = CY_BLE_ERROR_INVALID_STATE;
            }

            else if(!CY_BLE_IS_NOTIFICATION_ENABLED(connHandle.attId, cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].
                                                     charInfoPtr[charInstance].descrHandle[CY_BLE_ESS_CCCD]))
            {
                apiResult = CY_BLE_ERROR_NTF_DISABLED;
            }
            else
            {
                /* Fill all fields of write request structure ... */
                cy_stc_ble_gatts_handle_value_ntf_t ntfReqParam =
                {
                    .handleValPair.attrHandle = cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].
                                                 charInfoPtr[charInstance].charHandle,
                    .handleValPair.value.val  = attrValue,
                    .handleValPair.value.len  = attrSize,
                    .connHandle               = connHandle,
                };
                /* Send notification to client using previously filled structure */
                apiResult = Cy_BLE_GATTS_Notification(&ntfReqParam);
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
* Function Name: Cy_BLE_ESSS_SendIndication
***************************************************************************//**
*
*  Sends an indication with a characteristic value of the Environmental Sensing
*  Service, which is a value specified by charIndex, to the client's device.
*
*  On enabling indication successfully it sends out a 'Handle Value Indication' which
*  results in CY_BLE_EVT_ESSC_INDICATION or CY_BLE_EVT_GATTC_HANDLE_VALUE_IND (if
*  service-specific callback function is not registered) event at the GATT Client's end.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*  \param charInstance: The instance number of the characteristic specified by
*                "charIndex".
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the client's device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*   * CY_BLE_SUCCESS - The request handled successfully
*   * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CY_BLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*   * CY_BLE_ERROR_IND_DISABLED - Indication is not enabled by the client.
*   * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is
*                                               absent.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the ESS service-specific callback is registered
*      (with Cy_BLE_ESS_RegisterAttrCallback):
*  * CY_BLE_EVT_ESSS_INDICATION_CONFIRMED - in case if the indication is
*                                successfully delivered to the peer device.
*  .
*   Otherwise (if the ESS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF - in case if the indication is
*                                successfully delivered to the peer device.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ESSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_ess_char_index_t charIndex,
                                                  uint8_t charInstance,
                                                  uint8_t attrSize,
                                                  uint8_t *attrValue)
{
    /* Store new data in database */
    cy_en_ble_api_result_t apiResult = CY_BLE_ERROR_INVALID_PARAMETER;

    if((charIndex == CY_BLE_ESS_DESCRIPTOR_VALUE_CHANGED) &&
       (charInstance < cy_ble_essConfigPtr->esssCharInstances[charIndex]))
    {
        /* Send indication if it is enabled and connected */
        if(Cy_BLE_GetConnectionState(connHandle) < CY_BLE_CONN_STATE_CONNECTED)
        {
            apiResult = CY_BLE_ERROR_INVALID_STATE;
        }
        else if(!CY_BLE_IS_INDICATION_ENABLED(connHandle.attId,
                                              cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].charInfoPtr[charInstance].
                                               descrHandle[CY_BLE_ESS_CCCD]))
        {
            apiResult = CY_BLE_ERROR_IND_DISABLED;
        }
        else
        {
            if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE !=
               cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].charInfoPtr[charInstance].charHandle)
            {
                /* Fill all fields of write request structure ... */
                cy_stc_ble_gatts_handle_value_ind_t indReqParam =
                {
                    .handleValPair.attrHandle = cy_ble_essConfigPtr->esss->charInfoAddr[charIndex].
                                                 charInfoPtr[charInstance].charHandle,
                    .handleValPair.value.val  = attrValue,
                    .handleValPair.value.len  = attrSize,
                    .connHandle               = connHandle
                };
                /* Send indication to client using previously filled structure */
                apiResult = Cy_BLE_GATTS_Indication(&indReqParam);
                /* Save handle to support service-specific value confirmation response from client */
                if(apiResult == CY_BLE_SUCCESS)
                {
                    cy_ble_esssReqHandle = indReqParam.handleValPair.attrHandle;
                }
            }
            else
            {
                apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
            }
        }
    }

    return(apiResult);
}


#endif /* CY_BLE_ESS_SERVER */


#ifdef CY_BLE_ESS_CLIENT

/******************************************************************************
* Function Name: Cy_BLE_ESSC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param discCharInfo: The pointer to a characteristic information structure.
*
******************************************************************************/
static void Cy_BLE_ESSC_DiscoverCharacteristicsEventHandler(const cy_stc_ble_disc_char_info_t *discCharInfo)
{
    /* ESS characteristics UUIDs */
    static const cy_ble_uuid16_t cy_ble_esscCharUuid[CY_BLE_ESS_CHAR_COUNT] =
    {
        CY_BLE_UUID_CHAR_DESCR_VALUE_CHANGED,
        CY_BLE_UUID_CHAR_APPARENT_WIND_DIRECTION,
        CY_BLE_UUID_CHAR_APPARENT_WIND_SPEED,
        CY_BLE_UUID_CHAR_DEW_POINT,
        CY_BLE_UUID_CHAR_ELEVATION,
        CY_BLE_UUID_CHAR_GUST_FACTOR,
        CY_BLE_UUID_CHAR_HEAT_INDEX,
        CY_BLE_UUID_CHAR_HUMIDITY,
        CY_BLE_UUID_CHAR_IRRADIANCE,
        CY_BLE_UUID_CHAR_POLLEN_CONCENTRATION,
        CY_BLE_UUID_CHAR_RAINFALL,
        CY_BLE_UUID_CHAR_PRESSURE,
        CY_BLE_UUID_CHAR_THEMPERATURE,
        CY_BLE_UUID_CHAR_TRUE_WIND_DIRECTION,
        CY_BLE_UUID_CHAR_TRUE_WIND_SPEED,
        CY_BLE_UUID_CHAR_UV_INDEX,
        CY_BLE_UUID_CHAR_WIND_CHILL,
        CY_BLE_UUID_CHAR_BAR_PRESSURE_TREND,
        CY_BLE_UUID_CHAR_MAGNETIC_DECLINATION,
        CY_BLE_UUID_CHAR_MAGNETIC_FLUX_DENSITY_2D,
        CY_BLE_UUID_CHAR_MAGNETIC_FLUX_DENSITY_3D
    };

    static cy_ble_gatt_db_attr_handle_t *lastEndHandle[CY_BLE_CONFIG_GATTC_COUNT] = { NULL };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);
    uint32_t i;
    uint32_t j;
    uint32_t exitLoop = 0u;

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_ESS))
    {
        /* Update last characteristic endHandle to declaration handle of this characteristic */
        if(lastEndHandle[discIdx] != NULL)
        {
            *lastEndHandle[discIdx] = discCharInfo->charDeclHandle - 1u;
            lastEndHandle[discIdx] = NULL;
        }

        /* Search through all available characteristics */
        for(i = (uint32_t)CY_BLE_ESS_DESCRIPTOR_VALUE_CHANGED; ((i < (uint32_t)CY_BLE_ESS_CHAR_COUNT) &&
                                                                (exitLoop == 0u)); i++)
        {
            for(j = 0u; j < cy_ble_essConfigPtr->esscCharInstances[i]; j++)
            {
                if(cy_ble_esscCharUuid[i] == discCharInfo->uuid.uuid16)
                {
                    if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[i].charInfoPtr != NULL)
                    {
                        if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[i].charInfoPtr[j].valueHandle ==
                           CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                        {
                            cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[i].charInfoPtr[j].valueHandle =
                                discCharInfo->valueHandle;
                            cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[i].charInfoPtr[j].properties =
                                discCharInfo->properties;
                            lastEndHandle[discIdx] =
                                &cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[i].charInfoPtr[j].endHandle;
                            exitLoop = 1u;
                            break;
                        }
                    }
                }
            }
        }

        /* Initialize characteristic endHandle to Service endHandle.
         * Characteristic endHandle will be updated to the declaration
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
* Function Name: Cy_BLE_ESSC_DiscoverCharDescriptorsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_FIND_INFO_RSP event.
*  Based on the descriptor UUID, an appropriate data structure is populated using
*  the data received as part of the callback.
*
*  \param discDescrInfo: The pointer to a descriptor information structure.
*
******************************************************************************/
static void Cy_BLE_ESSC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo)
{
    uint32_t notSupportedDescr = 0u;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discDescrInfo->connHandle);
    uint32_t charIdx = cy_ble_discovery[discIdx].charCount;
    uint32_t charInst = activeCharInstance[discIdx];
    uint32_t descIdx;


    if((cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_ESS) &&
       (cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIdx].charInfoPtr != NULL))
    {
        switch(discDescrInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_CLIENT_CONFIG:
                descIdx = (uint32_t)CY_BLE_ESS_CCCD;
                break;

            case CY_BLE_UUID_CHAR_VALID_RANGE:
                descIdx = (uint32_t)CY_BLE_ESS_VRD;
                break;

            case CY_BLE_UUID_CHAR_USER_DESCRIPTION:
                descIdx = (uint32_t)CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR;
                break;

            case CY_BLE_UUID_CHAR_ES_CONFIGURATION:
                descIdx = (uint32_t)CY_BLE_ESS_ES_CONFIG_DESCR;
                break;

            case CY_BLE_UUID_CHAR_EXTENDED_PROPERTIES:
                descIdx = (uint32_t)CY_BLE_ESS_CHAR_EXTENDED_PROPERTIES;
                break;

            case CY_BLE_UUID_CHAR_ES_MEASUREMENT:
                descIdx = (uint32_t)CY_BLE_ESS_ES_MEASUREMENT_DESCR;
                break;

            case CY_BLE_UUID_CHAR_ES_TRIGGER_SETTING:
                if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIdx].charInfoPtr[charInst].
                    descrHandle[CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR1] == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    descIdx = (uint32_t)CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR1;
                }
                else if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIdx].charInfoPtr[charInst].
                         descrHandle[CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR2] == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    descIdx = (uint32_t)CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR2;
                }
                else
                {
                    descIdx = (uint32_t)CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR3;
                }
                break;

            default:
                /* Not supported descriptor */
                notSupportedDescr = 1u;
                break;
        }

        if(notSupportedDescr == 0u)
        {
            if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIdx].charInfoPtr[charInst].descrHandle[descIdx] ==
               CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
            {
                cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIdx].charInfoPtr[charInst].descrHandle[descIdx] =
                    discDescrInfo->descrHandle;
            }
            else    /* Duplication of descriptor */
            {
                Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_DESCR_DUPLICATION, &discDescrInfo);
            }
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ESSC_GetCharRange
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
static void Cy_BLE_ESSC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(charRangeInfo->connHandle);
    uint32_t exitFlag = 0u;

    if(cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_ESS)
    {
        if(charRangeInfo->srviIncIdx == CY_BLE_DISCOVERY_INIT)
        {
            /* CY_BLE_DISCOVERY_INIT */
            activeCharInstance[discIdx] = 0u;
        }
        else
        {
            /* CY_BLE_DISCOVERY_CONTINUE */
            activeCharInstance[discIdx]++;
            if(activeCharInstance[discIdx] == cy_ble_essConfigPtr->esscCharInstances[cy_ble_discovery[discIdx].charCount])
            {
                cy_ble_discovery[discIdx].charCount++;
                activeCharInstance[discIdx] = 0u;
            }
        }

        while((cy_ble_discovery[discIdx].charCount < (uint32_t)CY_BLE_ESS_CHAR_COUNT) && (exitFlag == 0u))
        {
            uint32_t charIdx = cy_ble_discovery[discIdx].charCount;

            while((activeCharInstance[discIdx] < cy_ble_essConfigPtr->esscCharInstances[charIdx]) && (exitFlag == 0u))
            {
                uint32_t charInst = activeCharInstance[discIdx];
                if((cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIdx].charInfoPtr[charInst].valueHandle !=
                    CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE) &&
                   ((cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIdx].charInfoPtr[charInst].endHandle -
                     cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIdx].charInfoPtr[charInst].valueHandle) != 0u))
                {
                    charRangeInfo->range.startHandle =
                        cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIdx].charInfoPtr[charInst].valueHandle + 1u;
                    charRangeInfo->range.endHandle =
                        cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIdx].charInfoPtr[charInst].endHandle;
                    exitFlag = 1u;
                }
                else
                {
                    activeCharInstance[discIdx]++;
                }
            }
            if(exitFlag == 0u)
            {
                cy_ble_discovery[discIdx].charCount++;
                activeCharInstance[discIdx] = 0u;
            }
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ESSC_NotificationEventHandler
***************************************************************************//**
*
*  Handles a notification event for the Environmental Sensing Service.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_ESSC_NotificationEventHandler(cy_stc_ble_gattc_handle_value_ntf_param_t *eventParam)
{
    uint32_t j;
    uint32_t exitLoop = 0u;
    cy_en_ble_ess_char_index_t locCharIndex;

    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_ESS_ApplCallback != NULL))
    {
        for(locCharIndex = (cy_en_ble_ess_char_index_t)0u; (locCharIndex < CY_BLE_ESS_CHAR_COUNT) &&
            (exitLoop == 0u); locCharIndex++)
        {
            if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr != NULL)
            {
                for(j = 0u; j < cy_ble_essConfigPtr->esscCharInstances[locCharIndex]; j++)
                {
                    if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr[j].valueHandle ==
                       eventParam->handleValPair.attrHandle)
                    {
                        cy_stc_ble_ess_char_value_t notifValue =
                        {
                            .connHandle = eventParam->connHandle,
                            .charIndex  = locCharIndex,
                            .value      = &eventParam->handleValPair.value
                        };
                        Cy_BLE_ESS_ApplCallback((uint32_t)CY_BLE_EVT_ESSC_NOTIFICATION, &notifValue);
                        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                        exitLoop = 1u;
                        break;
                    }
                }
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ESSC_IndicationEventHandler
***************************************************************************//**
*
*  Handles an indication event for Environmental Sensing Service.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_ESSC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_ESS_ApplCallback != NULL))
    {
        if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[CY_BLE_ESS_DESCRIPTOR_VALUE_CHANGED].
            charInfoPtr[0u].valueHandle == eventParam->handleValPair.attrHandle)
        {
            cy_stc_ble_ess_char_value_t indicationValue =
            {
                .connHandle = eventParam->connHandle,
                .charIndex  = CY_BLE_ESS_DESCRIPTOR_VALUE_CHANGED,
                .value      = &eventParam->handleValPair.value,
            };
            Cy_BLE_ESS_ApplCallback((uint32_t)CY_BLE_EVT_ESSC_INDICATION, &indicationValue);
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ESSC_ReadLongRespEventHandler
***************************************************************************//**
*
*  Handles a Read Long Response event for the Environmental Sensing Service.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_ESSC_ReadLongRespEventHandler(const cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t j;
    uint32_t i;
    uint32_t isReqEnded = 0u;
    cy_en_ble_ess_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_ESS_ApplCallback != NULL))
    {
        /* Go trough all characteristics */
        for(locCharIndex = CY_BLE_ESS_DESCRIPTOR_VALUE_CHANGED; locCharIndex < CY_BLE_ESS_CHAR_COUNT; locCharIndex++)
        {
            /* Check if specific characteristic exist */
            if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr != NULL)
            {
                /* Go trough all characteristic instances */
                for(j = 0u; j < cy_ble_essConfigPtr->esscCharInstances[locCharIndex]; j++)
                {
                    /* Read Long Request is only supported for Characteristic User
                     * Description descriptor. Check if requested handle equals
                     * the descriptor.
                     */
                    if((cy_ble_esscReqHandle[discIdx] == cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].
                         charInfoPtr[j].descrHandle[CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR]) &&
                       (cy_ble_esscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
                    {
                        cy_stc_ble_gatt_xchg_mtu_param_t mtuParam = { .connHandle = eventParam->connHandle };

                        /* Update user buffer with received data */
                        for(i = 0u; (i < eventParam->value.len) && (cy_ble_esscCurrLen[discIdx] <
                                                                    cy_ble_esscRdLongBuffLen[discIdx]); i++)
                        {
                            cy_ble_esscRdLongBuffPtr[discIdx][cy_ble_esscCurrLen[discIdx]] = eventParam->value.val[i];
                            cy_ble_esscCurrLen[discIdx]++;
                        }
                        (void)Cy_BLE_GATT_GetMtuSize(&mtuParam);

                        /* If the received data length is less than the MTU size, the Read Long
                         * request is completed or the provided user's buffer is full.
                         */
                        if(((mtuParam.mtu - 1u) > eventParam->value.len))
                        {
                            cy_ble_esscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                        }
                        else if(cy_ble_esscCurrLen[discIdx] == cy_ble_esscRdLongBuffLen[discIdx])
                        {
                            cy_stc_ble_gattc_stop_cmd_param_t stopCmdParam = { .connHandle = eventParam->connHandle };
                            (void)Cy_BLE_GATTC_StopCmd(&stopCmdParam);
                        }
                        else
                        {
                            isReqEnded = 1u;
                        }

                        /* If the buffer is full, then stop any remaining read long requests */
                        if(isReqEnded == 0u)
                        {
                            cy_stc_ble_gatt_value_t rdValue =
                            {
                                .val = cy_ble_esscRdLongBuffPtr[discIdx],
                                .len = cy_ble_esscCurrLen[discIdx]
                            };
                            cy_stc_ble_ess_descr_value_t rdDescrValue =
                            {
                                .connHandle   = eventParam->connHandle,
                                .charIndex    = locCharIndex,
                                .charInstance = j,
                                .descrIndex   = CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR
                            };
                            rdDescrValue.value = &rdValue;
                            Cy_BLE_ESS_ApplCallback((uint32_t)CY_BLE_EVT_ESSC_READ_DESCR_RESPONSE, &rdDescrValue);
                        }

                        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                        break;
                    }
                }
            }
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ESSC_ReadResponseEventHandler
***************************************************************************//**
*
*  Handles a Read Response event for the Environmental Sensing Service.
*
*  \param eventParam: The pointer to the data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_ESSC_ReadResponseEventHandler(cy_stc_ble_gattc_read_rsp_param_t *eventParam)
{
    uint32_t j;
    uint32_t locReqHandle = 0u;
    cy_en_ble_ess_char_index_t locCharIndex;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_ESS_ApplCallback != NULL) &&
       (cy_ble_esscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locCharIndex = CY_BLE_ESS_DESCRIPTOR_VALUE_CHANGED; (locCharIndex < CY_BLE_ESS_CHAR_COUNT) &&
            (locReqHandle == 0u); locCharIndex++)
        {
            if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr != NULL)
            {
                for(j = 0u; ((j < cy_ble_essConfigPtr->esscCharInstances[locCharIndex]) && (locReqHandle == 0u)); j++)
                {
                    if(cy_ble_esscReqHandle[discIdx] ==
                       cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr[j].valueHandle)
                    {
                        cy_stc_ble_ess_char_value_t locCharValue =
                        {
                            .connHandle   = eventParam->connHandle,
                            .charIndex    = locCharIndex,
                            .charInstance = j,
                            .value        = &eventParam->value
                        };
                        cy_ble_esscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                        Cy_BLE_ESS_ApplCallback((uint32_t)CY_BLE_EVT_ESSC_READ_CHAR_RESPONSE, &locCharValue);
                        locReqHandle = 1u;
                    }
                    else
                    {
                        cy_en_ble_ess_descr_index_t locDescIndex;

                        for(locDescIndex = CY_BLE_ESS_CCCD; (locDescIndex < CY_BLE_ESS_DESCR_COUNT) &&
                            (locReqHandle == 0u); locDescIndex++)
                        {
                            if(cy_ble_esscReqHandle[discIdx] ==
                               cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr[j].
                                descrHandle[locDescIndex])
                            {
                                cy_stc_ble_ess_descr_value_t locDescrValue =
                                {
                                    .connHandle   = eventParam->connHandle,
                                    .charIndex    = locCharIndex,
                                    .descrIndex   = locDescIndex,
                                    .charInstance = j,
                                    .value        = &eventParam->value
                                };
                                cy_ble_esscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                                Cy_BLE_ESS_ApplCallback((uint32_t)CY_BLE_EVT_ESSC_READ_DESCR_RESPONSE, &locDescrValue);
                                locReqHandle = 1u;
                            }
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
* Function Name: Cy_BLE_ESSC_WriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Write Response event for the Environmental Sensing Service.
*
*  \param eventParam: The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_ESSC_WriteResponseEventHandler(const cy_stc_ble_conn_handle_t *eventParam)
{
    uint8_t j;
    uint32_t locReqHandle = 0u;
    cy_en_ble_ess_char_index_t locCharIndex;
    cy_stc_ble_ess_descr_value_t locDescrValue = { .connHandle = *eventParam };

    /* Check if service handler was registered and request handle is
     * valid.
     */
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(*eventParam);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_ESS_ApplCallback != NULL) &&
       (cy_ble_esscReqHandle[discIdx] != CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locCharIndex = CY_BLE_ESS_DESCRIPTOR_VALUE_CHANGED; (locCharIndex < CY_BLE_ESS_CHAR_COUNT) &&
            (locReqHandle == 0u); locCharIndex++)
        {
            /* If this condition is false, this means that the current characteristic
             * is not included in the GUI, in other words support for the characteristic is
             * not enabled.
             */
            if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr != NULL)
            {
                for(j = 0u; ((j < cy_ble_essConfigPtr->esscCharInstances[locCharIndex]) && (locReqHandle == 0u)); j++)
                {
                    /* Check if requested descriptor handle is in
                     * characteristic range.
                     */
                    if((cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr[j].valueHandle <
                        cy_ble_esscReqHandle[discIdx]) &&
                       (cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr[j].endHandle >=
                        cy_ble_esscReqHandle[discIdx]))
                    {
                        if(cy_ble_esscReqHandle[discIdx] ==
                           cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr[j].
                            descrHandle[CY_BLE_ESS_CCCD])
                        {
                            locDescrValue.descrIndex = CY_BLE_ESS_CCCD;
                        }
                        else
                            if(cy_ble_esscReqHandle[discIdx] ==
                               cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr[j].
                                descrHandle[CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR1])
                            {
                                locDescrValue.descrIndex = CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR1;
                            }
                            else
                                if(cy_ble_esscReqHandle[discIdx] ==
                                   cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr[j].
                                    descrHandle[CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR2])
                                {
                                    locDescrValue.descrIndex = CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR2;
                                }
                                else if(cy_ble_esscReqHandle[discIdx] ==
                                        cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr[j].
                                         descrHandle[CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR3])
                                {
                                    locDescrValue.descrIndex = CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR3;
                                }
                                else if(cy_ble_esscReqHandle[discIdx] ==
                                        cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr[j].
                                         descrHandle[CY_BLE_ESS_ES_CONFIG_DESCR])
                                {
                                    locDescrValue.descrIndex = CY_BLE_ESS_ES_CONFIG_DESCR;
                                }
                                else if(cy_ble_esscReqHandle[discIdx] ==
                                        cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr[j].
                                         descrHandle[CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR])
                                {
                                    locDescrValue.descrIndex = CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR;
                                }
                                else
                                {
                                    /* Should never enter here */
                                }

                        locDescrValue.charIndex = locCharIndex;
                        locDescrValue.charInstance = j;
                        locDescrValue.value = NULL;
                        cy_ble_esscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                        Cy_BLE_ESS_ApplCallback((uint32_t)CY_BLE_EVT_ESSC_WRITE_DESCR_RESPONSE, &locDescrValue);
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
* Function Name: Cy_BLE_ESSC_ExecuteWriteResponseEventHandler
***************************************************************************//**
*
*  Handles a Execute Write Response event for the Environmental Sensing Service.
*
*  \param eventParam: The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_ESSC_ExecuteWriteResponseEventHandler(const cy_stc_ble_gattc_exec_write_rsp_t *eventParam)
{
    uint8_t j;
    uint32_t locReqHandle = 0u;
    cy_en_ble_ess_char_index_t locCharIndex;
    cy_stc_ble_ess_descr_value_t locDescrValue = { .connHandle = eventParam->connHandle };

    /* Check if service handler was registered and request handle is
     * valid.
     */
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (Cy_BLE_ESS_ApplCallback != NULL) && (cy_ble_esscReqHandle[discIdx] !=
                                                                                      CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
    {
        for(locCharIndex = CY_BLE_ESS_DESCRIPTOR_VALUE_CHANGED; (locCharIndex < CY_BLE_ESS_CHAR_COUNT) &&
            (locReqHandle == 0u); locCharIndex++)
        {
            /* If this condition is false, this means that the current characteristic
             * is not included in the GUI, in other words the
             * characteristic support is not enabled.
             */
            if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr != NULL)
            {
                for(j = 0u; ((j < cy_ble_essConfigPtr->esscCharInstances[locCharIndex]) && (locReqHandle == 0u)); j++)
                {
                    if(cy_ble_esscReqHandle[discIdx] ==
                       cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[locCharIndex].charInfoPtr[j].
                        descrHandle[CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR])
                    {
                        locDescrValue.connHandle = eventParam->connHandle;
                        locDescrValue.charIndex = locCharIndex;
                        locDescrValue.charInstance = j;
                        locDescrValue.value = NULL;
                        locDescrValue.descrIndex = CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR;
                        cy_ble_esscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                        Cy_BLE_ESS_ApplCallback((uint32_t)CY_BLE_EVT_ESSC_WRITE_DESCR_RESPONSE, &locDescrValue);
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
* Function Name: Cy_BLE_ESSC_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles an Error Response event for the Environmental Sensing Service.
*
*  \param eventParam: The pointer to a data structure specified by an event.
*
******************************************************************************/
static void Cy_BLE_ESSC_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    if(eventParam != NULL)
    {
        uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
        if(cy_ble_esscReqHandle[discIdx] == eventParam->errInfo.attrHandle)
        {
            cy_ble_esscReqHandle[discIdx] = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ESSC_SetCharacteristicValue
***************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_ESSS_WRITE_CHAR events is generated.
*  On successful request execution on the Server side the Write Response is
*  sent to the Client.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param charInstance: The instance number of the characteristic specified by
*                "charIndex".
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*             sent to the server device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                               the particular characteristic.
*  * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                     characteristic.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is absent.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the ESS service-specific callback is registered
*      (with Cy_BLE_ESS_RegisterAttrCallback):
*  * CY_BLE_EVT_ESSC_WRITE_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_ess_char_value_t.
*  .
*   Otherwise (if the ESS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ESSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_ess_char_index_t charIndex,
                                                          uint8_t charInstance,
                                                          uint8_t attrSize,
                                                          uint8_t * attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_ESS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(charInstance >= cy_ble_essConfigPtr->esscCharInstances[charIndex])
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE !=
            cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIndex].charInfoPtr[charInstance].valueHandle)
    {
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIndex].
                                         charInfoPtr[charInstance].valueHandle,
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        apiResult = Cy_BLE_GATTC_WriteCharacteristicValue(&writeReqParam);
        /* Save handle to support service-specific write response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_esscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ESSC_GetCharacteristicValue
***************************************************************************//**
*
*  This function is used to read a characteristic value, which is a value
*  identified by charIndex, from the server.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param charInstance: The instance number of the characteristic specified by
*                "charIndex".
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The read request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The peer device doesn't have
*                                              the particular characteristic.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_STATE - Connection with the server is not established.
*  * CY_BLE_ERROR_INVALID_OPERATION - Operation is invalid for this
*                                     characteristic.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic is
*                                              absent.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the ESS service-specific callback is registered
*      (with Cy_BLE_ESS_RegisterAttrCallback):
*  * CY_BLE_EVT_ESSC_READ_CHAR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index , value, etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_ess_char_value_t.
*  .
*   Otherwise (if the ESS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_ESSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_ess_char_index_t charIndex,
                                                          uint8_t charInstance)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_ESS_CHAR_COUNT) || (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(charInstance >= cy_ble_essConfigPtr->esscCharInstances[charIndex])
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE !=
            cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIndex].charInfoPtr[charInstance].valueHandle)
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIndex].charInfoPtr[charInstance].valueHandle,
            .connHandle = connHandle,
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_esscReqHandle[discIdx] =
                cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIndex].charInfoPtr[charInstance].valueHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ESSC_SetCharacteristicDescriptor
***************************************************************************//**
*
*  This function is used to write the characteristic (which is identified by
*  charIndex) value attribute in the server. As a result a Write Request is
*  sent to the GATT Server and on successful execution of the request on the
*  Server side the CY_BLE_EVT_ESSS_DESCR_WRITE events is generated.
*  On successful request execution on the Server side the Write Response is
*  sent to the Client.
*
*  Internally, Write Request is sent to the GATT Server and on successful
*  execution of the request on the Server side the following events can be
*  generated:
*  * CY_BLE_EVT_ESSS_NOTIFICATION_ENABLED
*  * CY_BLE_EVT_ESSS_NOTIFICATION_DISABLED
*  * CY_BLE_EVT_ESSS_INDICATION_ENABLED
*  * CY_BLE_EVT_ESSS_INDICATION_DISABLED
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param charInstance: The instance number of the characteristic specified by
*                "charIndex".
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic descriptor value data that
*             should be sent to the server device.
*
* \return
*  A return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                    the specified attribute.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional Characteristic Descriptor
*                                              is absent.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the ESS service-specific callback is registered
*      (with Cy_BLE_ESS_RegisterAttrCallback):
*  * CY_BLE_EVT_ESSC_WRITE_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_ess_descr_value_t.
*  .
*   Otherwise (if the ESS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ESSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ess_char_index_t charIndex,
                                                               uint8_t charInstance,
                                                               cy_en_ble_ess_descr_index_t descrIndex,
                                                               uint8_t attrSize,
                                                               uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_ESS_CHAR_COUNT) || (descrIndex >= CY_BLE_ESS_DESCR_COUNT) ||
            (charInstance >= cy_ble_essConfigPtr->esscCharInstances[charIndex]) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if((descrIndex == CY_BLE_ESS_VRD) || (descrIndex == CY_BLE_ESS_ES_MEASUREMENT_DESCR))
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
    else if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIndex].charInfoPtr[charInstance].descrHandle[descrIndex] ==
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gattc_write_req_t writeReqParam =
        {
            .handleValPair.attrHandle = cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIndex].
                                         charInfoPtr[charInstance].descrHandle[descrIndex],
            .handleValPair.value.val  = attrValue,
            .handleValPair.value.len  = attrSize,
            .connHandle               = connHandle
        };
        /* ... and send request to server's device. */
        apiResult = Cy_BLE_GATTC_WriteCharacteristicDescriptors(&writeReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_esscReqHandle[discIdx] = writeReqParam.handleValPair.attrHandle;
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ESSC_GetCharacteristicDescriptor
***************************************************************************//**
*
*  Sends a request to get the characteristic descriptor of the specified
*  characteristic of the service.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param charInstance: The instance number of the characteristic specified by
*                "charIndex".
*  \param descrIndex: The index of the service characteristic descriptor.
*
*  \return
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional Characteristic Descriptor
*                                              is absent.
*  \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the ESS service-specific callback is registered
*      (with Cy_BLE_ESS_RegisterAttrCallback):
*  * CY_BLE_EVT_ESSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_ess_descr_value_t.
*  .
*  Otherwise (if the ESS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_ESSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ess_char_index_t charIndex,
                                                               uint8_t charInstance,
                                                               cy_en_ble_ess_descr_index_t descrIndex)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_ESS_CHAR_COUNT) || (descrIndex >= CY_BLE_ESS_DESCR_COUNT) ||
            (charInstance >= cy_ble_essConfigPtr->esscCharInstances[charIndex]) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIndex].charInfoPtr[charInstance].descrHandle[descrIndex] ==
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_read_req_t readReqParam =
        {
            .attrHandle = cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIndex].charInfoPtr[charInstance].
                           descrHandle[descrIndex],
            .connHandle = connHandle,
        };
        apiResult = Cy_BLE_GATTC_ReadCharacteristicDescriptors(&readReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_esscReqHandle[discIdx] = cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIndex].
                                             charInfoPtr[charInstance].descrHandle[descrIndex];
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ESSC_SetLongCharacteristicDescriptor
***************************************************************************//**
*
*  This function is used to write a long characteristic descriptor to the server,
*  which is identified by charIndex and descrIndex.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param charInstance: The instance number of the characteristic specified by
*                "charIndex".
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic descriptor value data that
*             should be sent to the server device.
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                    the specified attribute.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - An optional characteristic Descriptor
*                                              is absent.
*
* \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*   If the ESS service-specific callback is registered
*      (with Cy_BLE_ESS_RegisterAttrCallback):
*  * CY_BLE_EVT_ESSC_WRITE_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index etc.) are
*                                provided with event parameter structure
*                                of type cy_stc_ble_ess_descr_value_t.
*  .
*   Otherwise (if the ESS service-specific callback is not registered):
*  * CY_BLE_EVT_GATTC_WRITE_RSP - in case if the requested attribute is
*                                successfully written on the peer device.
*  * CY_BLE_EVT_GATTC_ERROR_RSP - in case if there is some trouble with the
*                                requested attribute on the peer device,
*                                the details are provided with event parameters
*                                structure (cy_stc_ble_gatt_err_param_t).
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_ESSC_SetLongCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                   cy_en_ble_ess_char_index_t charIndex,
                                                                   uint8_t charInstance,
                                                                   cy_en_ble_ess_descr_index_t descrIndex,
                                                                   uint16_t attrSize,
                                                                   uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_ESS_CHAR_COUNT) || (descrIndex >= CY_BLE_ESS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIndex].charInfoPtr[charInstance].descrHandle[descrIndex] ==
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else if(descrIndex == CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR)
    {
        /* Fill all fields of write request structure ... */
        cy_stc_ble_gattc_prep_write_req_t writeReqParam =
        {
            .handleValOffsetPair.handleValuePair.attrHandle = cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIndex].
                                                               charInfoPtr[charInstance].descrHandle[descrIndex],
            .handleValOffsetPair.handleValuePair.value.val  = attrValue,
            .handleValOffsetPair.handleValuePair.value.len  = attrSize,
            .handleValOffsetPair.offset                     = 0u,
            .connHandle                                     = connHandle
        };
        /* ... and send request to server's device. */
        apiResult = Cy_BLE_GATTC_WriteLongCharacteristicValues(&writeReqParam);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_esscReqHandle[discIdx] = writeReqParam.handleValOffsetPair.handleValuePair.attrHandle;
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ESSC_GetLongCharacteristicDescriptor
***************************************************************************//**
*
*  Sends a request to read long characteristic descriptor of the specified
*  characteristic of the service.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic. Starts with zero.
*  \param charInstance: The instance number of the characteristic specified by
*                "charIndex".
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the buffer where the read long characteristic
*             descriptor value should be stored.
*
* \return
*  * CY_BLE_SUCCESS - The request was sent successfully.
*  * CY_BLE_ERROR_INVALID_PARAMETER - Validation of the input parameters failed.
*  * CY_BLE_ERROR_INVALID_STATE - The state is not valid.
*  * CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed.
*  * CY_BLE_ERROR_INVALID_OPERATION - This operation is not permitted on
*                                     the specified attribute.
*  * CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE - The optional Characteristic Descriptor
*                                              is absent.
*
*  \events
*  In case of successful execution (return value = CY_BLE_SUCCESS)
*  the next events can appear: \n
*  If the ESS service-specific callback is registered
*      (with Cy_BLE_ESS_RegisterAttrCallback):
*  * CY_BLE_EVT_ESSC_READ_DESCR_RESPONSE - in case if the requested attribute is
*                                successfully written on the peer device,
*                                the details (char index, descr index, value, etc.)
*                                are provided with event parameter structure
*                                of type cy_stc_ble_ess_descr_value_t.
*  .
*  Otherwise (if the ESS service-specific callback is not registered):
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
cy_en_ble_api_result_t Cy_BLE_ESSC_GetLongCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                   cy_en_ble_ess_char_index_t charIndex,
                                                                   uint8_t charInstance,
                                                                   cy_en_ble_ess_descr_index_t descrIndex,
                                                                   uint16_t attrSize,
                                                                   uint8_t *attrValue)
{
    cy_en_ble_api_result_t apiResult;
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(Cy_BLE_GetConnectionState(connHandle) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if((charIndex >= CY_BLE_ESS_CHAR_COUNT) || (descrIndex >= CY_BLE_ESS_DESCR_COUNT) || 
            (discIdx >= CY_BLE_CONFIG_GATTC_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIndex].charInfoPtr[charInstance].descrHandle[descrIndex] ==
            CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
    {
        apiResult = CY_BLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE;
    }
    else
    {
        cy_stc_ble_gattc_read_blob_req_t readLongdata =
        {
            .handleOffset.offset     = 0u,
            .handleOffset.attrHandle = cy_ble_essConfigPtr->essc[discIdx].charInfoAddr[charIndex].charInfoPtr[charInstance].
                                        descrHandle[descrIndex],
            .connHandle              = connHandle,
        };
        cy_ble_esscRdLongBuffLen[discIdx] = (uint32_t)attrSize;
        cy_ble_esscRdLongBuffPtr[discIdx] = attrValue;

        apiResult = Cy_BLE_GATTC_ReadLongCharacteristicDescriptors(&readLongdata);

        /* Save handle to support service-specific read response from device */
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_esscReqHandle[discIdx] = readLongdata.handleOffset.attrHandle;
        }
    }

    return(apiResult);
}

#endif /* CY_BLE_ESS_CLIENT */

/******************************************************************************
* Function Name: Cy_BLE_ESS_EventHandler
***************************************************************************//**
*
*  Handles the events from the BLE stack for the  Environmental Sensing Service.
*
*  \param eventCode:  the event code
*  \param eventParam:  the event parameters
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
static cy_en_ble_gatt_err_code_t Cy_BLE_ESS_EventHandler(uint32_t eventCode,
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
        #ifdef CY_BLE_ESS_CLIENT
            /* Discovery Events */
            case CY_BLE_EVT_GATTC_DISC_CHAR:
                Cy_BLE_ESSC_DiscoverCharacteristicsEventHandler((cy_stc_ble_disc_char_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR:
                Cy_BLE_ESSC_DiscoverCharDescriptorsEventHandler((cy_stc_ble_disc_descr_info_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE:
                Cy_BLE_ESSC_GetCharRange((cy_stc_ble_disc_range_info_t*)eventParam);
                break;
        #endif /* CY_BLE_ESS_CLIENT */

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
        #ifdef CY_BLE_ESS_SERVER
            case CY_BLE_EVT_GATTS_WRITE_REQ:
                gattErr = Cy_BLE_ESSS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF:
                Cy_BLE_ESSS_ConfirmationEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_PREP_WRITE_REQ:
                Cy_BLE_ESSS_PrepareWriteRequestEventHandler((cy_stc_ble_gatts_prep_write_req_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTS_EXEC_WRITE_REQ:
                Cy_BLE_ESSS_ExecuteWriteRequestEventHandler((cy_stc_ble_gatts_exec_write_req_t*)eventParam);
                break;
        #endif /* CY_BLE_ESS_SERVER */

            /**************************************************************************
             * Handling GATT Client Events
             ***************************************************************************/
        #ifdef CY_BLE_ESS_CLIENT
            case CY_BLE_EVT_GATTC_ERROR_RSP:
            {
                uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(((cy_stc_ble_gatt_err_param_t*)eventParam)->connHandle);
                if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag == 0u) &&
                   (((cy_stc_ble_gatt_err_param_t*)eventParam)->errInfo.errorCode !=
                    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
                {
                    Cy_BLE_ESSC_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)eventParam);
                }
            }
            break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
                Cy_BLE_ESSC_NotificationEventHandler((cy_stc_ble_gattc_handle_value_ntf_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_HANDLE_VALUE_IND:
                Cy_BLE_ESSC_IndicationEventHandler((cy_stc_ble_gattc_handle_value_ind_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_RSP:
                Cy_BLE_ESSC_ReadResponseEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_WRITE_RSP:
                Cy_BLE_ESSC_WriteResponseEventHandler((cy_stc_ble_conn_handle_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_READ_BLOB_RSP:
                Cy_BLE_ESSC_ReadLongRespEventHandler((cy_stc_ble_gattc_read_rsp_param_t*)eventParam);
                break;

            case CY_BLE_EVT_GATTC_EXEC_WRITE_RSP:
                Cy_BLE_ESSC_ExecuteWriteResponseEventHandler((cy_stc_ble_gattc_exec_write_rsp_t*)eventParam);
                break;
        #endif /* CY_BLE_ESS_CLIENT */

            default:
                break;
        }
    }

    return(gattErr);
}

#endif /* defined(CY_BLE_ESS) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
