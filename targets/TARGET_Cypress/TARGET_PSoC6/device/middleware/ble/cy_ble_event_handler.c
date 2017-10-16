/***************************************************************************//**
* \file cy_ble_event_handler.c
* \version 2.0
*
* \brief
*  This file contains the source code for the Event Handler State Machine
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

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if (CY_BLE_MODE_PROFILE)

/***************************************
* Private Function Prototypes
***************************************/

static void Cy_BLE_TimeOutEventHandler(const cy_stc_ble_timeout_param_t *eventParam);
static void Cy_BLE_L2Cap_ConnParamUpdateRspEventHandler(const cy_stc_ble_l2cap_conn_update_rsp_param_t *eventParam);
static void Cy_BLE_GAP_ConnUpdateCompleteEventHandler(const cy_stc_ble_gap_conn_param_updated_in_controller_t *eventParam);


#if (CY_BLE_GATT_ROLE_SERVER)
static void Cy_BLE_SendWriteResponse(const cy_stc_ble_gatts_write_cmd_req_param_t *eventParam,
                                     cy_en_ble_gatt_err_code_t gattErr);
#endif /* CY_BLE_GATT_ROLE_SERVER */

#if (CY_BLE_GATT_ROLE_CLIENT)
static void Cy_BLE_GAPC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
#endif /* CY_BLE_GATT_ROLE_CLIENT */


/***************************************
* Global Variables
***************************************/
volatile uint8_t cy_ble_eventHandlerFlag;
volatile uint8_t cy_ble_busyStatus[CY_BLE_CONN_COUNT];
volatile uint32_t cy_ble_cmdStatus;

#if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)
uint8_t cy_ble_advertisingIntervalType;
#endif /* CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER) */

#if (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER)
uint8_t cy_ble_scanningIntervalType;
#endif /* CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER */

#if (CY_BLE_GATT_ROLE_CLIENT)
cy_stc_ble_discovery_t cy_ble_discovery[CY_BLE_CONFIG_GATTC_COUNT];
cy_stc_ble_disc_srvc_info_t cy_ble_serverInfo[CY_BLE_CONFIG_GATTC_COUNT][CY_BLE_SRVI_COUNT];
#endif /* CY_BLE_GATT_ROLE_CLIENT */

#if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL)
cy_stc_ble_event_handler_t cy_ble_eventHandler;


/******************************************************************************
* Function Name: Cy_BLE_RegisterServiceEvtHandler
***************************************************************************//**
*
*  Registration Service Event Handler
*
*  \param  eventHandlerFunc: The pointer to the callback procedure.
*
*  \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*   Error Codes                           | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | The function completed successfully.
*   CY_BLE_ERROR_INVALID_PARAMETER        | On specifying NULL as input parameter.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Buffer overflow in the registration callback.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_RegisterServiceEventHandler(cy_ble_event_handle_t eventHandlerFunc)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    uint8_t existFlag = 0u;

    if(eventHandlerFunc == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Check if eventHandlerFunc has been already registered */
        uint32_t idx;
        for(idx = 0u; (idx < cy_ble_eventHandler.count) && (existFlag == 0u); idx++)
        {
            if(cy_ble_eventHandler.serviceEventHandler[idx] == eventHandlerFunc)
            {
                existFlag = 1u;
            }
        }
    }
    /* Register eventHandlerFunc callback */
    if((apiResult == CY_BLE_SUCCESS) && (existFlag == 0u))
    {
        if(cy_ble_eventHandler.count < CY_BLE_MAX_SRVI)
        {
            cy_ble_eventHandler.serviceEventHandler[cy_ble_eventHandler.count] = eventHandlerFunc;
            cy_ble_eventHandler.count += 1u;
        }
        else
        {
            apiResult = CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED;
        }
    }

    return(apiResult);
}

/******************************************************************************
* Function Name: Cy_BLE_InvokeServiceEventHandler
***************************************************************************//**
*
*  Invoke registered service event handlers.
*
*  \param eventCode:   The event code.
*  \param eventParam:  The event parameters.
*
* \return
*  Return value is of type cy_en_ble_gatt_err_code_t.
*
******************************************************************************/
cy_en_ble_gatt_err_code_t Cy_BLE_InvokeServiceEventHandler(uint32_t eventCode,
                                                           void *eventParam)
{
    uint32_t idx;
    cy_en_ble_gatt_err_code_t gattErr = CY_BLE_GATT_ERR_NONE;

    for(idx = 0u; (idx < cy_ble_eventHandler.count) && ((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) != 0u); idx++)
    {
        gattErr = cy_ble_eventHandler.serviceEventHandler[idx](eventCode, eventParam);
    }

    return(gattErr);
}
#endif /* CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL */

#if (CY_BLE_GATT_ROLE_SERVER)


/******************************************************************************
* Function Name: Cy_BLE_SendWriteResponse
***************************************************************************//**
*
*  Sends the Write Response to Write Request when event was handled by service.
*
*  \param eventParam - The event parameter.
*  \param gattErr - The error code.
*
* \return
*  None.
*
******************************************************************************/
static void Cy_BLE_SendWriteResponse(const cy_stc_ble_gatts_write_cmd_req_param_t *eventParam,
                                     cy_en_ble_gatt_err_code_t gattErr)
{
    /* Send response when event was handled by the service */
    if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) == 0u)
    {
        if(gattErr != CY_BLE_GATT_ERR_NONE)
        {
            cy_stc_ble_gatt_err_param_t err_param =
            {
                .errInfo.opCode     = CY_BLE_GATT_WRITE_REQ,
                .errInfo.attrHandle = eventParam->handleValPair.attrHandle,
                .errInfo.errorCode  = gattErr,
                .connHandle         = eventParam->connHandle
            };
            /* Send an Error Response */
            (void)Cy_BLE_GATTS_ErrorRsp(&err_param);
        }
        else
        {
            (void)Cy_BLE_GATTS_WriteRsp(eventParam->connHandle);
        }
    }
}


#endif /* CY_BLE_GATT_ROLE_SERVER */



/******************************************************************************
* Function Name: Cy_BLE_TimeOutEventHandler
***************************************************************************//**
*
*  Handles a CY_BLE_EVT_TIMEOUT event from the BLE stack.
*
*  \param eventParam: The pointer to a structure of type cy_stc_ble_timeout_param_t.
*
* \return
*  None
*
******************************************************************************/
static void Cy_BLE_TimeOutEventHandler(const cy_stc_ble_timeout_param_t *eventParam)
{
    if(eventParam->reasonCode == CY_BLE_GATT_RSP_TO)
    {
    #if (CY_BLE_GATT_ROLE_CLIENT)
        {
            uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->CY_BLE_HANDLE.connHandle);
            if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag != 0u))
            {
                switch(Cy_BLE_GetConnectionState(eventParam->CY_BLE_HANDLE.connHandle))
                {
                    case CY_BLE_CONN_STATE_CLIENT_SRVC_DISCOVERING:
                        Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_SRVC_DISCOVERY_FAILED,
                                            (cy_stc_ble_conn_handle_t*)&eventParam->CY_BLE_HANDLE.connHandle);
                        break;

                    case CY_BLE_CONN_STATE_CLIENT_INCL_DISCOVERING:
                        Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_INCL_DISCOVERY_FAILED,
                                            (cy_stc_ble_conn_handle_t*)&eventParam->CY_BLE_HANDLE.connHandle);
                        break;

                    case CY_BLE_CONN_STATE_CLIENT_CHAR_DISCOVERING:
                        Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DISCOVERY_FAILED,
                                            (cy_stc_ble_conn_handle_t*)&eventParam->CY_BLE_HANDLE.connHandle);
                        break;

                    case CY_BLE_CONN_STATE_CLIENT_DESCR_DISCOVERING:
                        Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_DESCR_DISCOVERY_FAILED,
                                            (cy_stc_ble_conn_handle_t*)&eventParam->CY_BLE_HANDLE.connHandle);
                        break;

                    default:        /* Other states should not be set in Auto discovery mode */
                        break;
                }
                cy_ble_discovery[discIdx].autoDiscoveryFlag = 0u;
                Cy_BLE_SetConnectionState(eventParam->CY_BLE_HANDLE.connHandle, CY_BLE_CONN_STATE_CONNECTED);
            }
        }
    #endif /* CY_BLE_GATT_ROLE_CLIENT */
    }

    #if (CY_BLE_GAP_ROLE_CENTRAL)
    /* Connection procedure timeout */
    if((eventParam->reasonCode == CY_BLE_GENERIC_APP_TO) &&
       (cy_ble_connectingTimeout.timerHandle == eventParam->timerHandle))
    {
        if(Cy_BLE_GetState() == CY_BLE_STATE_CONNECTING)
        {
            (void)Cy_BLE_GAPC_CancelDeviceConnection();
        }
    }
    #endif /* CY_BLE_GAP_ROLE_CENTRAL */
}


#if (CY_BLE_GATT_ROLE_SERVER || CY_BLE_GATT_ROLE_CLIENT)

/******************************************************************************
* Function Name: Cy_BLE_IsDeviceAddressValid
***************************************************************************//**
*
*  This function verifies that the BLE public address has been programmed to SFLASH
*  during manufacture. It can be used to verify if a public device address is
*  programmed to flash memory.
*
*  \param deviceAddress: The pointer to the BD address of type cy_stc_ble_gap_bd_addr_t.
*
* \return
*  A non-zero value when a device address differs from the default SFLASH content.
*
******************************************************************************/
uint8_t Cy_BLE_IsDeviceAddressValid(const cy_stc_ble_gap_bd_addr_t *deviceAddress)
{
    uint8_t addressValid = 0u;
    uint32_t i;

    if(deviceAddress->type == CY_BLE_GAP_ADDR_TYPE_PUBLIC)
    {
        for(i = 0u; i < CY_BLE_GAP_BD_ADDR_SIZE; i++)
        {
            if(deviceAddress->bdAddr[i] != 0u)
            {
                addressValid = 1u;
                break;
            }
        }
    }

    return(addressValid);
}
#endif /* CY_BLE_GATT_ROLE_SERVER || CY_BLE_GATT_ROLE_CLIENT */


/******************************************************************************
* Function Name: Cy_BLE_L2Cap_ConnParamUpdateRspHandler
***************************************************************************//**
*
*  Handles an L2CAP connection parameter response event from
*  the BLE stack.
*
*  \param eventParam:
*      * Accepted = 0x0000
*      * Rejected = 0x0001
*
* \return
*  None
*
******************************************************************************/
static void Cy_BLE_L2Cap_ConnParamUpdateRspEventHandler(const cy_stc_ble_l2cap_conn_update_rsp_param_t *eventParam)
{
    /* Unreferenced variable warning workaround */
    if(eventParam->result != 0u)
    {
    }
}


/******************************************************************************
* Function Name: Cy_BLE_GAP_ConnUpdateCompleteEventHandler
***************************************************************************//**
*
*  Handles a CY_BLE_EVT_GAP_CONNECTION_UPDATE_COMPLETE event from
*  the BLE stack.
*
*  \param eventParam: The pointer to the data structure specified by the event.
*
* \return
*  None
*
******************************************************************************/
static void Cy_BLE_GAP_ConnUpdateCompleteEventHandler(const cy_stc_ble_gap_conn_param_updated_in_controller_t *eventParam)
{
    /* Unreferenced variable warning workaround */
    if(eventParam != 0u)
    {
    }
}


/******************************************************************************
* Function Name: Cy_BLE_EventHandler
***************************************************************************//**
*
*  Handles events from the BLE stack.
*
*  \param event:   The event code.
*  \param evParam: The event parameters.
*
* \return
*  None.
*
******************************************************************************/
void Cy_BLE_EventHandler(cy_en_ble_event_t event,
                         void *evParam)
{
    cy_ble_eventHandlerFlag |= CY_BLE_CALLBACK;

    /* Common Profile event handling */
    switch(event)
    {
        /**********************************************************
         * General events
         ************************************************************/
        case CY_BLE_EVT_STACK_ON:

        #if (CY_BLE_GATT_ROLE_SERVER || CY_BLE_GATT_ROLE_CLIENT)
            {
                /* Set a device address  */
                if(Cy_BLE_IsDeviceAddressValid(cy_ble_sflashDeviceAddress) != 0u)
                {
                    (void)Cy_BLE_GAP_SetBdAddress(cy_ble_sflashDeviceAddress);
                #if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)
                    Cy_BLE_ChangeAdDeviceAddress(cy_ble_sflashDeviceAddress, 0u);
                    Cy_BLE_ChangeAdDeviceAddress(cy_ble_sflashDeviceAddress, 1u);
                #endif /* CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER */
                }
                else
                {
                    if(cy_ble_configPtr->params->siliconDeviceAddressEnabled != 0u)
                    {
                        uint32_t bdAddrLoc;
                        bdAddrLoc = ((uint32_t)SFLASH->DIE_X & (uint32_t)CY_BLE_SFLASH_DIE_X_MASK) |
                                    ((uint32_t)(((uint32_t)SFLASH->DIE_Y) & ((uint32_t)CY_BLE_SFLASH_DIE_Y_MASK)) <<
                                     CY_BLE_SFLASH_DIE_X_BITS) |
                                    ((uint32_t)(((uint32_t)SFLASH->DIE_WAFER) & ((uint32_t)CY_BLE_SFLASH_DIE_WAFER_MASK)) <<
                                     CY_BLE_SFLASH_DIE_XY_BITS) |
                                    ((uint32_t)(((uint32_t)SFLASH->DIE_LOT[0]) & ((uint32_t)CY_BLE_SFLASH_DIE_LOT_MASK)) <<
                                     CY_BLE_SFLASH_DIE_XYWAFER_BITS);

                        cy_ble_configPtr->deviceAddress->bdAddr[0] = (uint8_t)bdAddrLoc;
                        cy_ble_configPtr->deviceAddress->bdAddr[1] = (uint8_t)(bdAddrLoc >> 8u);
                        cy_ble_configPtr->deviceAddress->bdAddr[2] = (uint8_t)(bdAddrLoc >> 16u);
                    }
                    (void)Cy_BLE_GAP_SetBdAddress(cy_ble_configPtr->deviceAddress);

                #if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)
                    Cy_BLE_ChangeAdDeviceAddress(cy_ble_configPtr->deviceAddress, 0u);
                    Cy_BLE_ChangeAdDeviceAddress(cy_ble_configPtr->deviceAddress, 1u);
                #endif /* CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER */
                }
                /* Set the device IO Capability  */
                (void)Cy_BLE_GAP_SetIoCap((cy_en_ble_gap_iocap_t*)&cy_ble_configPtr->params->securityIoCapability);
                {
                    /* Enable all 4.1 events and configured 4.2 events */
                    uint8_t leMask[CY_BLE_LE_MASK_LENGTH] = { CY_LO8(CY_BLE_LE_MASK),
                                                              CY_HI8(CY_BLE_LE_MASK),0u,  0u, 0u, 0u, 0u, 0u };
                    (void)Cy_BLE_SetLeEventMask(leMask);
                }
            }
        #endif /* CY_BLE_GATT_ROLE_SERVER || CY_BLE_GATT_ROLE_CLIENT */
            Cy_BLE_SetState(CY_BLE_STATE_ON);
            break;

        case CY_BLE_EVT_STACK_SHUTDOWN_COMPLETE:
            Cy_BLE_SetState(CY_BLE_STATE_STOPPED);

            #if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)
            Cy_BLE_SetAdvertisementState(CY_BLE_ADV_STATE_STOPPED);
            #endif  /* (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER) */

            #if (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER)
            Cy_BLE_SetScanState(CY_BLE_SCAN_STATE_STOPPED);
            #endif /* (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER) */
            
            /* Adding a delay of 10ms to ensure that the controller is completely 
             * shut-down before generating the event to the application.
             * Controller generates CY_BLE_EVT_STACK_SHUTDOWN_COMPLETE event immediately 
             * after accepting the shut-down command (before resetting the controller), 
             * once this behaviour is fixed, this delay will no longer be required 
             */
            #if(CY_BLE_STACK_MODE_HOST_IPC)
                Cy_SysLib_Delay(10u);
            #endif /* CY_BLE_STACK_MODE_HOST_IPC */   
            
            /* Unregister BLE callback for Deepsleep/Sleep */
            Cy_BLE_UnregisterHostPMCallbacks();
            break;

        case CY_BLE_EVT_LE_SET_EVENT_MASK_COMPLETE:
        #if (CY_BLE_MODE_PROFILE)
            {
                if((cy_ble_cmdStatus & CY_BLE_STATUS_SET_TX_PWR_LVL) == 0u)
                {
                    cy_stc_ble_tx_pwr_lvl_info_t bleSsPowerLevel = { .pwrConfigParam.bdHandle = 0x0u };

                    /* Set the Tx Power Level for advertising and connection channels */
                    bleSsPowerLevel.blePwrLevel = cy_ble_configPtr->params->txPowerLevelAdv;
                    bleSsPowerLevel.pwrConfigParam.bleSsChId = CY_BLE_LL_ADV_CH_TYPE;
                    (void)Cy_BLE_SetTxPowerLevel(&bleSsPowerLevel);

                    bleSsPowerLevel.blePwrLevel = cy_ble_configPtr->params->txPowerLevelConn;
                    bleSsPowerLevel.pwrConfigParam.bleSsChId = CY_BLE_LL_CONN_CH_TYPE;
                    bleSsPowerLevel.pwrConfigParam.bdHandle = 0xFFu; /* Set the tx power level value for all connection handles.*/
                    (void)Cy_BLE_SetTxPowerLevel(&bleSsPowerLevel);
                    
                    /* Set flag that executed Cy_BLE_SetTxPowerLevel during componnent start-up  */
                    cy_ble_cmdStatus |= CY_BLE_STATUS_SET_TX_PWR_LVL;
                }
            }
        #endif /* CY_BLE_MODE_PROFILE */
            break;
            
        case CY_BLE_EVT_TIMEOUT:
            Cy_BLE_TimeOutEventHandler((cy_stc_ble_timeout_param_t*)evParam);
            #if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL)
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            #endif /*(CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL)*/
            break;

        case CY_BLE_EVT_STACK_BUSY_STATUS:
            #if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL)
            cy_ble_busyStatus[Cy_BLE_GetConnHandleByBdHandle(((cy_stc_ble_l2cap_state_info_t*)evParam)->bdHandle).attId] =
                ((cy_stc_ble_l2cap_state_info_t*)evParam)->flowState;
            #endif /*(CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL)*/
            break;

        case CY_BLE_EVT_MEMORY_REQUEST:
        #if (CY_BLE_MODE_PROFILE)
            {
                cy_stc_ble_memory_request_t *memReq = (cy_stc_ble_memory_request_t*)evParam;
                if(memReq->request == CY_BLE_PREPARED_WRITE_REQUEST)
                {
            #if (CY_BLE_GATT_ENABLE_EXTERNAL_PREP_WRITE_BUFF == 0u)
                    /* Stack requests to provide memory to process a remote request */
                    if(memReq->allocFree == CY_BLE_ALLOC_MEMORY)
                    {
                        static cy_stc_ble_prepare_write_request_memory_t gPrepWriteReqMem;

                        /* Configure and return a statically allocated buffer at the end of the cy_ble_stackMemoryRam buffer */
                        gPrepWriteReqMem.prepareWriteQueueSize = CY_BLE_GATT_MAX_PREPARE_BUFF_COUNT;
                        gPrepWriteReqMem.totalAttrValueLength = CY_BLE_GATT_PREPARE_LENGTH_ALIGN;
                        gPrepWriteReqMem.queueBuffer = &cy_ble_stackMemoryRam[CY_BLE_STACK_RAM_SIZE -
                                                                              CY_BLE_GATT_PREPARE_WRITE_BUFF_LEN];
                        memReq->configMemory = &gPrepWriteReqMem;
                    }
                    /* Clear the CY_BLE_CALLBACK flag not to provide a handled event to the application */
                    cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
            #endif  /* CY_BLE_GATT_ENABLE_EXTERNAL_PREP_WRITE_BUFF == 0u */
                }
            }
        #endif /* CY_BLE_MODE_PROFILE */
            break;

        /**********************************************************
         * GAP events
         ************************************************************/
        case CY_BLE_EVT_GAP_AUTH_REQ:
            break;

        case CY_BLE_EVT_GAP_AUTH_COMPLETE:
        #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
            {
                cy_stc_ble_conn_handle_t connectedHandle;
                connectedHandle = Cy_BLE_GetConnHandleByBdHandle(((cy_stc_ble_gap_auth_info_t*)evParam)->bdHandle);
                cy_ble_peerBonding[connectedHandle.attId] = ((cy_stc_ble_gap_auth_info_t*)evParam)->bonding;
            }
        #endif  /* CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES */
            break;

        case CY_BLE_EVT_GAP_CONNECTION_UPDATE_COMPLETE:
            Cy_BLE_GAP_ConnUpdateCompleteEventHandler((const cy_stc_ble_gap_conn_param_updated_in_controller_t*)evParam);
            #if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL)
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            #endif /*(CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) */
            break;

    #if (CY_BLE_LL_PRIVACY_FEATURE != 0u)
        case CY_BLE_EVT_GAP_ENHANCE_CONN_COMPLETE:
        {
            cy_stc_ble_gap_conn_param_updated_in_controller_t connCParam;
            cy_stc_ble_gap_enhance_conn_complete_param_t *connEnhanceParam;

            connEnhanceParam = (cy_stc_ble_gap_enhance_conn_complete_param_t*)evParam;
            connCParam.status = connEnhanceParam->status;
            connCParam.connIntv = connEnhanceParam->connIntv;
            connCParam.connLatency = connEnhanceParam->connLatency;
            connCParam.supervisionTO = connEnhanceParam->supervisionTo;
            Cy_BLE_GAP_ConnUpdateCompleteEventHandler((const cy_stc_ble_gap_conn_param_updated_in_controller_t*)&connCParam);

            /* Advertising is automatically stopped if a Slave is connected, so update the adv state to
             *  CY_BLE_ADV_STATE_STOPPED  */
            #if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)
            if(connEnhanceParam->role == (uint8_t)CY_BLE_GAP_LL_ROLE_SLAVE)
            {
                Cy_BLE_SetAdvertisementState(CY_BLE_ADV_STATE_STOPPED);
            }
            #endif  /* (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER) */
        }
    #else
        case CY_BLE_EVT_GAP_DEVICE_CONNECTED:
            Cy_BLE_GAP_ConnUpdateCompleteEventHandler((const cy_stc_ble_gap_conn_param_updated_in_controller_t*)evParam);

            /* Advertising is automatically stopped if a Slave is connected, so update the adv state to CY_BLE_ADV_STATE_STOPPED  */
            #if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)
            if(((cy_stc_ble_gap_connected_param_t*)evParam)->role == CY_BLE_GAP_LL_ROLE_SLAVE)
            {
                Cy_BLE_SetAdvertisementState(CY_BLE_ADV_STATE_STOPPED);
            }
            #endif /* (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER) */
    #endif         /* CY_BLE_LL_PRIVACY_FEATURE != 0u */
            #if (CY_BLE_GAP_ROLE_CENTRAL)
            if(cy_ble_connectingTimeout.timeout != 0u)
            {
                (void)Cy_BLE_StopTimer(&cy_ble_connectingTimeout);
            }
            #endif /* CY_BLE_GAP_ROLE_CENTRAL */
            Cy_BLE_SetState(CY_BLE_STATE_ON);
            break;

        case CY_BLE_EVT_GAP_DEVICE_DISCONNECTED:
            Cy_BLE_SetState(CY_BLE_STATE_ON);
            break;

        #if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)
        case CY_BLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
            #if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL)
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            #endif /* (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) */

            if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) != 0u)
            {
                /* After Cy_BLE_GAPP_StartAdvertisement, the first event indicates that advertising has started */
                if(Cy_BLE_GetAdvertisementState() == CY_BLE_ADV_STATE_ADV_INITIATED)
                {
                    Cy_BLE_SetAdvertisementState(CY_BLE_ADV_STATE_ADVERTISING);
                }
                /* When the application initiated a stop advertisement */
                else if(Cy_BLE_GetAdvertisementState() == CY_BLE_ADV_STATE_STOP_INITIATED)
                {
                    Cy_BLE_SetAdvertisementState(CY_BLE_ADV_STATE_STOPPED);
                }
                else /* The following event indicates that advertising has been stopped */
                {
                    if(Cy_BLE_GetAdvertisementState() == CY_BLE_ADV_STATE_ADVERTISING)
                    {
                        Cy_BLE_SetAdvertisementState(CY_BLE_ADV_STATE_STOPPED);
                        if((cy_ble_configPtr->gappAdvParams[cy_ble_advIndex].slowAdvEnable != 0u) &&
                           (cy_ble_configPtr->gappAdvParams[cy_ble_advIndex].fastAdvTimeOut != 0u))
                        {
                            if(cy_ble_advertisingIntervalType == CY_BLE_ADVERTISING_FAST)
                            {
                                /* When fast advertising time out occur: Start slow advertising */
                                if(Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_SLOW, cy_ble_advIndex) ==
                                   CY_BLE_SUCCESS)
                                {
                                    Cy_BLE_SetAdvertisementState(CY_BLE_ADV_STATE_ADV_INITIATED);
                                }
                            }
                        }
                    }
                }
            }
            break;
        #endif /* CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER */

        #if (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER)
        case CY_BLE_EVT_GAPC_SCAN_START_STOP:
            #if (CY_BLE_GAP_ROLE_CENTRAL)
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            #endif /* (CY_BLE_GAP_ROLE_CENTRAL) */

            if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) != 0u)
            {
                /* After Cy_BLE_GAPC_StartScan, the first event indicates that scanning has been started */
                if(Cy_BLE_GetScanState() == CY_BLE_SCAN_STATE_SCAN_INITIATED)
                {
                    Cy_BLE_SetScanState(CY_BLE_SCAN_STATE_SCANNING);
                }
                /* When the application initiated stop scanning */
                else if(Cy_BLE_GetScanState() == CY_BLE_SCAN_STATE_STOP_INITIATED)
                {
                    Cy_BLE_SetScanState(CY_BLE_SCAN_STATE_STOPPED);
                }
                else /* The following event indicates that scanning has been stopped by stack */
                {
                    if(Cy_BLE_GetScanState() == CY_BLE_SCAN_STATE_SCANNING)
                    {
                        Cy_BLE_SetScanState(CY_BLE_SCAN_STATE_STOPPED);
                        if((cy_ble_configPtr->gapcScanParams[cy_ble_scanIndex].slowScanEnabled != 0u) &&
                           (cy_ble_configPtr->gapcScanParams[cy_ble_scanIndex].fastScanTimeOut != 0u))
                        {
                            if(cy_ble_scanningIntervalType == CY_BLE_SCANNING_FAST)
                            {
                                /* When a fast scanning timeout occurs: Start slow scanning */
                                if(Cy_BLE_GAPC_StartScan(CY_BLE_SCANNING_SLOW, cy_ble_scanIndex) == CY_BLE_SUCCESS)
                                {
                                    Cy_BLE_SetScanState(CY_BLE_SCAN_STATE_SCAN_INITIATED);
                                }
                            }
                        }
                    }
                }
            }
            break;
        #endif /* CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER */

        /**********************************************************
         * L2AP events
         ************************************************************/
        case CY_BLE_EVT_L2CAP_CONN_PARAM_UPDATE_RSP:
            Cy_BLE_L2Cap_ConnParamUpdateRspEventHandler((cy_stc_ble_l2cap_conn_update_rsp_param_t*)evParam);

            #if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL)
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            #endif /* (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) */
            break;

            /**********************************************************
             * GATT events
             ************************************************************/
        #if ((CY_BLE_GATT_ROLE_SERVER) || (CY_BLE_GATT_ROLE_CLIENT))
        case CY_BLE_EVT_GATT_CONNECT_IND:
        #if (CY_BLE_GATT_ROLE_CLIENT)
            /* If connected to the same device which is already discovered */
            if((Cy_BLE_GetConnectionState(*(cy_stc_ble_conn_handle_t*)evParam) ==
                CY_BLE_CONN_STATE_CLIENT_DISCONNECTED_DISCOVERED) &&
               (((cy_stc_ble_conn_handle_t*)evParam)->bdHandle ==
                cy_ble_connHandle[((cy_stc_ble_conn_handle_t*)evParam)->attId].bdHandle))
            {   /* Set a discovered state */
                Cy_BLE_SetConnectionState(*(cy_stc_ble_conn_handle_t*)evParam, CY_BLE_CONN_STATE_CLIENT_DISCOVERED);
            }
            else    /* Connected to a new device */
        #endif /* CY_BLE_GATT_ROLE_CLIENT */
            {
                /* Clear the discovery index for the Client role */
                #if (CY_BLE_GATT_ROLE_CLIENT)
                (void)Cy_BLE_GATTC_RemoveConnHandle(*(cy_stc_ble_conn_handle_t*)evParam);
                #endif /* CY_BLE_GATT_ROLE_CLIENT */
                Cy_BLE_SetConnectionState(*(cy_stc_ble_conn_handle_t*)evParam, CY_BLE_CONN_STATE_CONNECTED);
                cy_ble_connHandle[((cy_stc_ble_conn_handle_t*)evParam)->attId] = *(cy_stc_ble_conn_handle_t*)evParam;
            }

            cy_ble_busyStatus[((cy_stc_ble_conn_handle_t*)evParam)->attId] = CY_BLE_STACK_STATE_FREE;

        #if ((CY_BLE_GATT_ROLE_SERVER) && (CY_BLE_GATT_DB_CCCD_COUNT != 0u) && \
            (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
            
            /* Initialize the CCCD values in the RAM when bonding is enabled */
            (void)memcpy((uint8_t*)cy_ble_attValuesCccdMultiple[((cy_stc_ble_conn_handle_t*)evParam)->attId],
                         cy_ble_configPtr->flashStorage->attValuesCCCDFlashMemory[((cy_stc_ble_conn_handle_t*)evParam)->
                                                                                   bdHandle],
                         CY_BLE_GATT_DB_CCCD_COUNT + CY_BLE_CCCD_CRC_BYTE);

            /* Check CRC for CCCD data */
            {
                uint8_t cccdCrc = Cy_BLE_HAL_CalcCRC8(cy_ble_attValuesCccdMultiple[((cy_stc_ble_conn_handle_t*)evParam)->attId], 
                                                      CY_BLE_GATT_DB_CCCD_COUNT);

                if(cy_ble_attValuesCccdMultiple[((cy_stc_ble_conn_handle_t*)evParam)->attId][CY_BLE_GATT_DB_CCCD_COUNT] != cccdCrc)
                {
                    /* Inform that the CRC for CCCD is wrong */
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTS_EVT_CCCD_CORRUPT, NULL);
                    
                    /* Clean the CCCD buffer in the RAM */
                    (void)memset((uint8_t*)cy_ble_attValuesCccdMultiple[((cy_stc_ble_conn_handle_t*)evParam)->attId],
                                  0, CY_BLE_GATT_DB_CCCD_COUNT + CY_BLE_CCCD_CRC_BYTE);
                }            
            }
           
        #endif /* CY_BLE_GATT_ROLE_SERVER && (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */

            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;

        case CY_BLE_EVT_GATT_DISCONNECT_IND:
        #if (CY_BLE_GATT_ROLE_CLIENT)
            if((Cy_BLE_GetConnectionState(*(cy_stc_ble_conn_handle_t*)evParam) == CY_BLE_CONN_STATE_CLIENT_DISCOVERED) &&
               (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
            {
                Cy_BLE_SetConnectionState(*(cy_stc_ble_conn_handle_t*)evParam,
                                          CY_BLE_CONN_STATE_CLIENT_DISCONNECTED_DISCOVERED);
            }
            else
        #endif /* CY_BLE_GATT_ROLE_CLIENT */
            {
                Cy_BLE_SetConnectionState(*(cy_stc_ble_conn_handle_t*)evParam, CY_BLE_CONN_STATE_DISCONNECTED);
            #if (CY_BLE_GATT_ROLE_CLIENT)
                (void)Cy_BLE_GATTC_RemoveConnHandle(*(cy_stc_ble_conn_handle_t*)evParam);
            #endif /* CY_BLE_GATT_ROLE_CLIENT */
            }

            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;

        case CY_BLE_EVT_GATTS_XCNHG_MTU_REQ:
        {
            cy_stc_ble_gatt_xchg_mtu_param_t mtuParam;
            mtuParam.connHandle = ((cy_stc_ble_gatt_xchg_mtu_param_t*)evParam)->connHandle;

            if(CY_BLE_GATT_MTU > ((cy_stc_ble_gatt_xchg_mtu_param_t*)evParam)->mtu)
            {
                mtuParam.mtu = ((cy_stc_ble_gatt_xchg_mtu_param_t*)evParam)->mtu;
            }
            else
            {
                mtuParam.mtu = CY_BLE_GATT_MTU;
            }
            (void)Cy_BLE_GATTS_ExchangeMtuRsp(&mtuParam);
        }
        break;
    #endif /* CY_BLE_GATT_ROLE_SERVER || CY_BLE_GATT_ROLE_CLIENT */

    #if (CY_BLE_GATT_ROLE_CLIENT)
        case CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT:
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;
    #endif /* CY_BLE_GATT_ROLE_CLIENT */

    #if (CY_BLE_GATT_ROLE_SERVER)
        case CY_BLE_EVT_GATTS_WRITE_REQ:
        {
            cy_en_ble_gatt_err_code_t gattErr;
            gattErr = Cy_BLE_GATTS_WriteEventHandler((cy_stc_ble_gatts_write_cmd_req_param_t*)evParam);
            if(gattErr == CY_BLE_GATT_ERR_NONE)
            {
                gattErr = Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            }
            Cy_BLE_SendWriteResponse((cy_stc_ble_gatts_write_cmd_req_param_t*)evParam, gattErr);
        }
        break;

        case CY_BLE_EVT_GATTS_WRITE_CMD_REQ:
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;

        case CY_BLE_EVT_GATTS_PREP_WRITE_REQ:
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;

        case CY_BLE_EVT_GATTS_EXEC_WRITE_REQ:
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;

        case CY_BLE_EVT_GATTS_HANDLE_VALUE_CNF:
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;

        case CY_BLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ:
        #if (CY_BLE_GATT_DB_CCCD_COUNT != 0u)
            (void)Cy_BLE_GATTS_ReadAttributeValueCCCDReqHandler((cy_stc_ble_gatts_char_val_read_req_t*)evParam);
        #endif  /* (CY_BLE_GATT_DB_CCCD_COUNT != 0u) */
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;
    #endif /* CY_BLE_GATT_ROLE_SERVER */

    #if (CY_BLE_GATT_ROLE_CLIENT)
        case CY_BLE_EVT_GATTC_READ_BY_GROUP_TYPE_RSP:
            Cy_BLE_ReadByGroupEventHandler((cy_stc_ble_gattc_read_by_grp_rsp_param_t*)evParam);
            break;

        case CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP:
            Cy_BLE_ReadByTypeEventHandler((cy_stc_ble_gattc_read_by_type_rsp_param_t*)evParam);
            break;

        case CY_BLE_EVT_GATTC_READ_BLOB_RSP:
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;

        case CY_BLE_EVT_GATTC_LONG_PROCEDURE_END:
            Cy_BLE_LongProcedureEndEventHandler(((cy_stc_ble_gattc_long_procedure_end_param_t*)evParam)->connHandle);
            break;

        case CY_BLE_EVT_GATTC_FIND_INFO_RSP:
            Cy_BLE_FindInfoEventHandler((cy_stc_ble_gattc_find_info_rsp_param_t*)evParam);
            break;

        case CY_BLE_EVT_GATTC_ERROR_RSP:
            Cy_BLE_ErrorResponseEventHandler((cy_stc_ble_gatt_err_param_t*)evParam);
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;

        case CY_BLE_EVT_GATTC_HANDLE_VALUE_NTF:
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;

        case CY_BLE_EVT_GATTC_HANDLE_VALUE_IND:
            Cy_BLE_GATTC_IndicationEventHandler((cy_stc_ble_gattc_handle_value_ind_param_t*)evParam);
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);

            /* Respond with a Handle Value Confirmation when request handled */
            if((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) == 0u)
            {
                cy_stc_ble_gattc_confirmation_req_t confirmationParam;
                confirmationParam.connHandle = ((cy_stc_ble_gattc_handle_value_ind_param_t*)evParam)->connHandle;

                (void)Cy_BLE_GATTC_Confirmation(&confirmationParam);
            }
            break;

        case CY_BLE_EVT_GATTC_READ_RSP:
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;

        case CY_BLE_EVT_GATTC_READ_MULTI_RSP:
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;

        case CY_BLE_EVT_GATTC_WRITE_RSP:
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;

        case CY_BLE_EVT_GATTC_EXEC_WRITE_RSP:
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)event, (void*)evParam);
            break;
    #endif /* CY_BLE_GATT_ROLE_CLIENT */

    #if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
        case CY_BLE_EVT_PENDING_FLASH_WRITE:
            cy_ble_pendingFlashWrite |= CY_BLE_PENDING_STACK_FLASH_WRITE_BIT;
            break;
    #endif  /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */

        default:
            break;
    }

    if((cy_ble_eventHandlerFlag & (CY_BLE_CALLBACK | CY_BLE_ENABLE_ALL_EVENTS)) != 0u)
    {
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        Cy_BLE_ApplCallback((uint32_t)event, evParam);
    }
}

#if (CY_BLE_GATT_ROLE_CLIENT)

/******************************************************************************
* Function Name: Cy_BLE_ReadByGroupEventHandler
***************************************************************************//**
*
*  Handles a Read By Group Response event during an automatic server discovery
*  process.
*
*  \param eventParam: The event parameters for a Read By Group Response.
*
* \return
*  None
*
******************************************************************************/
void Cy_BLE_ReadByGroupEventHandler(cy_stc_ble_gattc_read_by_grp_rsp_param_t *eventParam)
{
    cy_stc_ble_disc_srv_info_t locDiscServInfo = { .connHandle = eventParam->connHandle };
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
    uint16_t locDataLength;
    uint16_t attrLength;
    uint32_t fFlag;
    uint32_t j;
    uint32_t i;

    if((Cy_BLE_GetConnectionState(eventParam->connHandle) == CY_BLE_CONN_STATE_CLIENT_SRVC_DISCOVERING) &&
       (discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag != 0u))
    {
        locDataLength = eventParam->attrData.length;

        if((locDataLength == CY_BLE_DISC_SRVC_INFO_LEN) || (locDataLength == CY_BLE_DISC_SRVC_INFO_128_LEN))
        {
            attrLength = eventParam->attrData.attrLen;

            for(i = 0u; i < attrLength; i += locDataLength)
            {
                cy_ble_eventHandlerFlag |= CY_BLE_CALLBACK;

                locDiscServInfo.srvcInfo = (cy_stc_ble_disc_srvc128_info_t*)(eventParam->attrData.attrValue + i);
                fFlag = 0u;

                /* Received a 16-bit service UUID */
                if((locDiscServInfo.srvcInfo->range.startHandle >=
                    cy_ble_discovery[discIdx].gattcDiscoveryRange.startHandle) &&
                   (locDiscServInfo.srvcInfo->range.startHandle <=
                    cy_ble_discovery[discIdx].gattcDiscoveryRange.endHandle))
                {
                    if(locDataLength == CY_BLE_DISC_SRVC_INFO_LEN)
                    {
                        for(j = 0u; (j < (uint32_t)CY_BLE_SRVI_COUNT) && (fFlag == 0u); j++)
                        {
                            locDiscServInfo.uuidFormat = CY_BLE_GATT_16_BIT_UUID_FORMAT;
                            if(cy_ble_serverInfo[discIdx][j].uuid == locDiscServInfo.srvcInfo->uuid.uuid16)
                            {
                                if(cy_ble_serverInfo[discIdx][j].range.startHandle ==
                                   CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                                {
                                    cy_ble_serverInfo[discIdx][j].range = locDiscServInfo.srvcInfo->range;
                                    fFlag = 1u;
                                }
                                else    /* Duplication of service */
                                {
                                    /* For multiple service support next service has the same UUID */
                                    if((j >= (CY_BLE_SRVI_COUNT - 1u)) ||
                                       (cy_ble_serverInfo[discIdx][j + 1u].uuid != locDiscServInfo.srvcInfo->uuid.uuid16))
                                    {
                                        Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_SRVC_DUPLICATION, &locDiscServInfo);
                                        fFlag = 1u;
                                    }
                                }
                            }
                        }
                    }
                    else  /* Received a 128-bit service UUID */
                    {
                        locDiscServInfo.uuidFormat = CY_BLE_GATT_128_BIT_UUID_FORMAT;
                        
                        /* Loop thru all registered services and invoke CY_BLE_EVT_GATTC_DISC_CHAR event */
                        (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)CY_BLE_EVT_GATTC_DISC_SERVICE,
                                                               (void*)&locDiscServInfo);
                    }
                }
                
                /* Generate event CY_BLE_EVT_GATTC_DISC_SKIPPED_SERVICE, if the incoming service was not processed */
                if((fFlag == 0u) && ((cy_ble_eventHandlerFlag & CY_BLE_CALLBACK) != 0u))
                {
                    /* Inform application that we discovered the service which is not defined in GATT database */
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_DISC_SKIPPED_SERVICE, &locDiscServInfo);    
                }
                
            }
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else
        {
            Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_SRVC_DISCOVERY_FAILED, &eventParam->connHandle);
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_NextInclDiscovery
***************************************************************************//**
*
*  Looks for the included services in the current service (pointed by
*   cy_ble_disCount). If the current service handle range is invalid (any of start
*   or end handle is invalid), then increments the cy_ble_disCount and check
*   the next service range and does so until a valid range is caught or the end of
*   the service set is reached.
*
*  \param connHandle:     The connection handle.
*  \param incrementIndex: A non-zero value indicates that the service index should be
*                         incremented.
*
* \return
*  None.
*
******************************************************************************/
void Cy_BLE_NextInclDiscovery(cy_stc_ble_conn_handle_t connHandle,
                              uint8_t incrementIndex)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(incrementIndex != CY_BLE_DISCOVERY_INIT)
    {
        cy_ble_discovery[discIdx].servCount++;
    }
    else
    {
        cy_ble_discovery[discIdx].servCount = 0u;
        cy_ble_discovery[discIdx].inclInfo.inclDefHandle = 0u;
    }

    /* Skip not existing services and services out of the discovery range */
    while((cy_ble_discovery[discIdx].servCount < (uint8_t)CY_BLE_SRVI_COUNT) &&
          ((cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range.startHandle <
            cy_ble_discovery[discIdx].gattcDiscoveryRange.startHandle) ||
           (cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range.startHandle >
            cy_ble_discovery[discIdx].gattcDiscoveryRange.endHandle)))
    {
        cy_ble_discovery[discIdx].servCount++;
    }

    if(cy_ble_discovery[discIdx].servCount < (uint8_t)CY_BLE_SRVI_COUNT)
    {
        cy_stc_ble_gattc_read_by_type_req_t requestParam =
        {
            .range      = cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range,
            .connHandle = connHandle
        };
        if(Cy_BLE_GATTC_FindIncludedServices(&requestParam) != CY_BLE_SUCCESS)
        {
            Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_INCL_DISCOVERY_FAILED, &connHandle);
            cy_ble_discovery[discIdx].autoDiscoveryFlag = 0u;
        }
    }
    else /* An included service discovery procedure is done, start a characteristic discovery procedure */
    {
        if((cy_ble_eventHandlerFlag & CY_BLE_ENABLE_ALL_EVENTS) != 0u)
        {
            Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_INCL_DISCOVERY_COMPLETE, &connHandle);
        }
        Cy_BLE_SetConnectionState(connHandle, CY_BLE_CONN_STATE_CLIENT_CHAR_DISCOVERING);
        Cy_BLE_NextCharDiscovery(connHandle, CY_BLE_DISCOVERY_INIT);
    }
}



/******************************************************************************
* Function Name: Cy_BLE_GAPC_DiscoverCharacteristicsEventHandler
***************************************************************************//**
*
*  This function is called on receiving a CY_BLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
*
*  \param connHandle:    The connection handle.
*  \param discCharInfo:  The pointer to the characteristic information structure.
*
* \return
*  None
*
******************************************************************************/
static void Cy_BLE_GAPC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(discCharInfo->connHandle);

    if((discCharInfo->uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT) &&
       (cy_ble_discovery[discIdx].servCount == (uint32_t)CY_BLE_SRVI_GAP))
    {
        switch(discCharInfo->uuid.uuid16)
        {
            case CY_BLE_UUID_CHAR_DEVICE_NAME:
                CY_BLE_GapcCheckCharHandle(cy_ble_discovery[discIdx].gapc.deviceNameCharHandle, discCharInfo);
                break;

            case CY_BLE_UUID_CHAR_APPEARANCE:
                CY_BLE_GapcCheckCharHandle(cy_ble_discovery[discIdx].gapc.appearanceCharHandle, discCharInfo);
                break;

            case CY_BLE_UUID_CHAR_PERIPH_PRIVCY_FLAG:
                CY_BLE_GapcCheckCharHandle(cy_ble_discovery[discIdx].gapc.periphPrivacyCharHandle, discCharInfo);
                break;

            case CY_BLE_UUID_CHAR_RECONNECTION_ADDR:
                CY_BLE_GapcCheckCharHandle(cy_ble_discovery[discIdx].gapc.reconnAddrCharHandle, discCharInfo);
                break;

            case CY_BLE_UUID_CHAR_PRFRRD_CNXN_PARAM:
                CY_BLE_GapcCheckCharHandle(cy_ble_discovery[discIdx].gapc.prefConnParamCharHandle, discCharInfo);
                break;

            case CY_BLE_UUID_CHAR_CENTRAL_ADDRESS_RESOLUTION:
                CY_BLE_GapcCheckCharHandle(cy_ble_discovery[discIdx].gapc.centralAddrResolutionCharHandle, discCharInfo);
                break;

            case CY_BLE_UUID_CHAR_RESOLVABLE_PRIVATE_ADDR_ONLY:
                CY_BLE_GapcCheckCharHandle(cy_ble_discovery[discIdx].gapc.resolvablePrivateAddressOnly, discCharInfo);
                break;

            default:
                break;
        }

        /* Indicate that request was handled */
        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}



/******************************************************************************
* Function Name: Cy_BLE_ReadByTypeEventHandler
***************************************************************************//**
*
*  Handles a Read By Type Response event during an automatic server discovery
*  process.
*
*  \param eventParam: The event parameters for a Read By Type Response.
*
* \return
*  None.
*
******************************************************************************/
void Cy_BLE_ReadByTypeEventHandler(cy_stc_ble_gattc_read_by_type_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
    uint32_t locDataLength;
    uint32_t attrLength;
    uint32_t i;
    uint32_t j;
    uint8_t *attrVal;

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag != 0u))
    {
        /* Count of service information pieces in this mtu */
        locDataLength = (uint32_t)eventParam->attrData.length;
        attrLength = (uint32_t)eventParam->attrData.attrLen;
        attrVal = eventParam->attrData.attrValue;

        if(Cy_BLE_GetConnectionState(eventParam->connHandle) == CY_BLE_CONN_STATE_CLIENT_CHAR_DISCOVERING)
        {
            cy_stc_ble_disc_char_info_t locDiscCharInfo;
            locDiscCharInfo.connHandle = eventParam->connHandle;

            for(i = 0u; i < attrLength; i += locDataLength)
            {
                cy_ble_eventHandlerFlag |= CY_BLE_CALLBACK;

                /* Get Handle for characteristic declaration */
                locDiscCharInfo.charDeclHandle = Cy_BLE_Get16ByPtr(attrVal);
                attrVal += sizeof(locDiscCharInfo.charDeclHandle);

                /* Get Properties for value */
                locDiscCharInfo.properties = *attrVal;
                attrVal += sizeof(locDiscCharInfo.properties);

                /* Get Handle to server database attribute value entry */
                locDiscCharInfo.valueHandle = Cy_BLE_Get16ByPtr(attrVal);
                attrVal += sizeof(locDiscCharInfo.valueHandle);

                /* Get Characteristic UUID (128/16 bit) */
                if(locDataLength == CY_BLE_DISC_CHAR_INFO_128_LEN)
                {
                    locDiscCharInfo.uuidFormat = CY_BLE_GATT_128_BIT_UUID_FORMAT;
                    (void)memcpy(&locDiscCharInfo.uuid.uuid128, attrVal, CY_BLE_GATT_128_BIT_UUID_SIZE);
                    attrVal += CY_BLE_GATT_128_BIT_UUID_SIZE;
                }
                else if(locDataLength == CY_BLE_DISC_CHAR_INFO_LEN)
                {
                    locDiscCharInfo.uuidFormat = CY_BLE_GATT_16_BIT_UUID_FORMAT;
                    locDiscCharInfo.uuid.uuid16 = Cy_BLE_Get16ByPtr(attrVal);
                    attrVal += CY_BLE_GATT_16_BIT_UUID_SIZE;
                }
                else
                {
                    /* Unsupported data length value */
                }

                if((locDataLength == CY_BLE_DISC_CHAR_INFO_128_LEN) || (locDataLength == CY_BLE_DISC_CHAR_INFO_LEN))
                {
                    /* Process common services: GAP and GATT */
                    Cy_BLE_GAPC_DiscoverCharacteristicsEventHandler(&locDiscCharInfo);
                    Cy_BLE_GATTC_DiscoverCharacteristicsEventHandler(&locDiscCharInfo);

                    /* Loop thru all registered services and invoke CY_BLE_EVT_GATTC_DISC_CHAR event */
                    (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)CY_BLE_EVT_GATTC_DISC_CHAR, (void*)&locDiscCharInfo);
                }
            }
            /* The sub-procedure is complete when an Error Response is received and the
             *  Error Code is set to the Attribute Not Found; or the Read By Type Response has an
             *  Attribute Handle equal to the Ending Handle of the request - in this case
             *  a CY_BLE_EVT_GATTC_LONG_PROCEDURE_END event is generated by the stack. */
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else if(Cy_BLE_GetConnectionState(eventParam->connHandle) == CY_BLE_CONN_STATE_CLIENT_INCL_DISCOVERING)
        {
            cy_stc_ble_disc_incl_info_t locDiscInclInfo;
            locDiscInclInfo.connHandle = eventParam->connHandle;

            for(i = 0u; i < attrLength; i += locDataLength)
            {
                cy_ble_eventHandlerFlag |= CY_BLE_CALLBACK;

                locDiscInclInfo.inclDefHandle = Cy_BLE_Get16ByPtr(attrVal);
                attrVal += sizeof(locDiscInclInfo.inclDefHandle);

                locDiscInclInfo.inclHandleRange.startHandle = Cy_BLE_Get16ByPtr(attrVal);
                attrVal += sizeof(locDiscInclInfo.inclHandleRange.startHandle);

                locDiscInclInfo.inclHandleRange.endHandle = Cy_BLE_Get16ByPtr(attrVal);
                attrVal += sizeof(locDiscInclInfo.inclHandleRange.endHandle);

                if(locDataLength == CY_BLE_DISC_INCL_INFO_128_LEN)
                {
                    cy_stc_ble_gattc_stop_cmd_param_t stopCmdParam = { .connHandle = eventParam->connHandle };
                    cy_stc_ble_gattc_read_req_t readReqParam =
                    {
                        .attrHandle = locDiscInclInfo.inclHandleRange.startHandle,
                        .connHandle = eventParam->connHandle
                    };
                    locDiscInclInfo.uuidFormat = CY_BLE_GATT_128_BIT_UUID_FORMAT;

                    /* Stop ongoing GATT operation */
                    (void)Cy_BLE_GATTC_StopCmd(&stopCmdParam);

                    /* Get the included service UUID when the included service uses a 128-bit
                     * UUID, a Read Request is used. The Attribute Handle for the Read Request is
                     * the Attribute Handle of the included service.
                     */

                    if(Cy_BLE_GATTC_ReadCharacteristicValue(&readReqParam) == CY_BLE_SUCCESS)
                    {
                        /* Save handle to support a read response from device */
                        cy_ble_discovery[discIdx].inclInfo = locDiscInclInfo;
                    }
                    else
                    {
                        Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_INCL_DISCOVERY_FAILED, &eventParam->connHandle);
                        cy_ble_discovery[discIdx].autoDiscoveryFlag = 0u;
                    }
                }
                else if(locDataLength == CY_BLE_DISC_INCL_INFO_LEN)
                {
                    locDiscInclInfo.uuidFormat = CY_BLE_GATT_16_BIT_UUID_FORMAT;
                    locDiscInclInfo.uuid.uuid16 = Cy_BLE_Get16ByPtr(attrVal);
                    attrVal += CY_BLE_GATT_16_BIT_UUID_SIZE;

                    /* Store the range of the included service in the list of services for discovery */
                    for(j = 0u; j < (uint32_t)CY_BLE_SRVI_COUNT; j++)
                    {
                        if(cy_ble_serverInfo[discIdx][j].uuid == locDiscInclInfo.uuid.uuid16)
                        {
                            if(cy_ble_serverInfo[discIdx][j].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                            {
                                cy_ble_serverInfo[discIdx][j].range = locDiscInclInfo.inclHandleRange;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    /* Unsupported data length value */
                }

                /* Loop thru all registered services and invoke CY_BLE_EVT_GATTC_DISC_INCL event */
                (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)CY_BLE_EVT_GATTC_DISC_INCL, (void*)&locDiscInclInfo);
            }
            cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
        }
        else
        {
            /* Unhandled state value */
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_NextCharDiscovery
***************************************************************************//**
*
*  Looks for a characteristic handle range for the current service (pointed by
*   cy_ble_disCount). If the current range is invalid (any of start or end
*   handles is invalid), then increments the cy_ble_disCount and check
*   the next service range and does so until a valid range is caught or the end
*   of the service set is reached.
*
*  \param connHandle:     The connection handle.
*  \param incrementIndex: A non-zero value indicates that the characteristic index
*                         should be incremented.
*
* \return
*  None.
*
******************************************************************************/
void Cy_BLE_NextCharDiscovery(cy_stc_ble_conn_handle_t connHandle,
                              uint8_t incrementIndex)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if(incrementIndex != CY_BLE_DISCOVERY_INIT)
    {
        cy_ble_discovery[discIdx].servCount++;
    }
    else
    {
        cy_ble_discovery[discIdx].servCount = 0u;
    }

    /* Skip not existing services and services out of the discovery range */
    while((cy_ble_discovery[discIdx].servCount < (uint32_t)CY_BLE_SRVI_COUNT) &&
          ((cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range.startHandle <
            cy_ble_discovery[discIdx].gattcDiscoveryRange.startHandle) ||
           (cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range.startHandle >
            cy_ble_discovery[discIdx].gattcDiscoveryRange.endHandle)))
    {
        cy_ble_discovery[discIdx].servCount++;
    }

    if(cy_ble_discovery[discIdx].servCount < (uint8_t)CY_BLE_SRVI_COUNT)
    {
        cy_stc_ble_gattc_read_by_type_req_t reqParam =
        {
            .connHandle = connHandle,
            .range      = cy_ble_serverInfo[discIdx][cy_ble_discovery[discIdx].servCount].range
        };
        if(Cy_BLE_GATTC_DiscoverCharacteristics(&reqParam) != CY_BLE_SUCCESS)
        {
            Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DISCOVERY_FAILED, &connHandle);
            cy_ble_discovery[discIdx].autoDiscoveryFlag = 0u;
        }
    }
    else /* A Characteristic discovery procedure is done, start a descriptor discovery procedure */
    {
        if((cy_ble_eventHandlerFlag & CY_BLE_ENABLE_ALL_EVENTS) != 0u)
        {
            Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DISCOVERY_COMPLETE, &connHandle);
        }
        Cy_BLE_SetConnectionState(connHandle, CY_BLE_CONN_STATE_CLIENT_DESCR_DISCOVERING);
        Cy_BLE_NextCharDscrDiscovery(connHandle, CY_BLE_DISCOVERY_INIT);
    }
}


/******************************************************************************
* Function Name: Cy_BLE_FindInfoEventHandler
***************************************************************************//**
*
*  Handles a Find Info Response event during an automatic server discovery
*  process.
*
*  \param eventParam:  The event parameters for a Find Info Response.
*
* \return
*  None.
*
******************************************************************************/
void Cy_BLE_FindInfoEventHandler(cy_stc_ble_gattc_find_info_rsp_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);
    uint32_t locDataLength;
    uint32_t attrLength;
    uint32_t i;
    uint8_t *attrVal;

    /* Discovery descriptor information */
    cy_stc_ble_disc_descr_info_t locDiscDescrInfo;

    locDiscDescrInfo.descrHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;

    if((Cy_BLE_GetConnectionState(eventParam->connHandle) == CY_BLE_CONN_STATE_CLIENT_DESCR_DISCOVERING) &&
       (discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag != 0u))
    {
        attrLength = eventParam->handleValueList.byteCount;    /* Number of elements on list in bytes */
        locDiscDescrInfo.uuidFormat = eventParam->uuidFormat;
        locDiscDescrInfo.connHandle = eventParam->connHandle;

        if(locDiscDescrInfo.uuidFormat == CY_BLE_GATT_16_BIT_UUID_FORMAT)
        {
            locDataLength = CY_BLE_DB_ATTR_HANDLE_LEN + CY_BLE_GATT_16_BIT_UUID_SIZE;
        }
        else
        {
            locDataLength = CY_BLE_DB_ATTR_HANDLE_LEN + CY_BLE_GATT_128_BIT_UUID_SIZE;
        }
        attrVal = eventParam->handleValueList.list;

        for(i = 0u; i < attrLength; i += locDataLength)
        {
            cy_ble_eventHandlerFlag |= CY_BLE_CALLBACK;

            locDiscDescrInfo.descrHandle = Cy_BLE_Get16ByPtr(attrVal);
            attrVal += CY_BLE_DB_ATTR_HANDLE_LEN;


            if(locDiscDescrInfo.uuidFormat == CY_BLE_GATT_128_BIT_UUID_FORMAT)
            {
                (void)memcpy(&locDiscDescrInfo.uuid.uuid128, attrVal, CY_BLE_GATT_128_BIT_UUID_SIZE);
                attrVal += CY_BLE_GATT_128_BIT_UUID_SIZE;
            }
            else
            {
                locDiscDescrInfo.uuid.uuid16 = Cy_BLE_Get16ByPtr(attrVal);
                attrVal += CY_BLE_GATT_16_BIT_UUID_SIZE;
            }

            /* Process common services: GATT */
            Cy_BLE_GATTC_DiscoverCharDescriptorsEventHandler(&locDiscDescrInfo);

            /* Loop thru all registered services and invoke CY_BLE_EVT_GATTC_DISC_CHAR event */
            (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)CY_BLE_EVT_GATTC_DISC_DESCR, (void*)&locDiscDescrInfo);
        }

        /* The sub-procedure is complete when an Error Response is received and the
         * Error Code is set to the Attribute Not Found or the Find Information Response has
         * an Attribute Handle that is equal to the Ending Handle of the request in this
         * case a CY_BLE_EVT_GATTC_LONG_PROCEDURE_END event is generated by the stack. */

        cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
    }
}


/******************************************************************************
* Function Name: Cy_BLE_NextCharDscrDiscovery
***************************************************************************//**
*
*  Looks for a handle range for the current descriptor (pointed by
*   cy_ble_discovery[discIdx].servCount). If the current range is invalid (any of start or end
*   handles is invalid), then increments the cy_ble_discovery[discIdx].servCount and check
*   the next descriptor range and does so until a valid range is caught or the
*   end of the descriptor set is reached.
*
*  \param connHandle:     The connection handle.
*  \param incrementIndex: A non-zero value indicates that the characteristic index should be
*                         incremented.
*
* \return
*  None.
*
******************************************************************************/
void Cy_BLE_NextCharDscrDiscovery(cy_stc_ble_conn_handle_t connHandle,
                                  uint8_t incrementIndex)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);
    cy_stc_ble_disc_range_info_t charRangeInfo = { .connHandle = connHandle, .srviIncIdx = incrementIndex };

    do
    {
        cy_ble_eventHandlerFlag |= CY_BLE_CALLBACK;
        charRangeInfo.range.startHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;
        charRangeInfo.range.endHandle = CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE;

        if(incrementIndex == CY_BLE_DISCOVERY_INIT)
        {
            cy_ble_discovery[discIdx].servCount = 0u;
            cy_ble_discovery[discIdx].charCount = 0u;
            incrementIndex = CY_BLE_DISCOVERY_CONTINUE;
        }

        /* Get a possible range of the common service: GATT */
        Cy_BLE_GATTC_GetCharRange(&charRangeInfo);

        /* Loop thru all registered services and get a possible range of the characteristic descriptor */
        (void)Cy_BLE_InvokeServiceEventHandler((uint32_t)CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE, (void*)&charRangeInfo);

        if((charRangeInfo.range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE) ||
           (charRangeInfo.range.endHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE))
        {
            cy_ble_discovery[discIdx].servCount++;
            cy_ble_discovery[discIdx].charCount = 0u;
            charRangeInfo.srviIncIdx = CY_BLE_DISCOVERY_INIT;
        }
        else
        {
            charRangeInfo.srviIncIdx = CY_BLE_DISCOVERY_CONTINUE;
        }

        /* Skip not existing characteristics and characteristics out of discovery range */
    }
    while(((charRangeInfo.range.startHandle <= cy_ble_discovery[discIdx].gattcDiscoveryRange.startHandle) ||
           (charRangeInfo.range.startHandle > cy_ble_discovery[discIdx].gattcDiscoveryRange.endHandle) ||
           (charRangeInfo.range.endHandle < cy_ble_discovery[discIdx].gattcDiscoveryRange.startHandle) ||
           (charRangeInfo.range.startHandle > charRangeInfo.range.endHandle)) &&
          (cy_ble_discovery[discIdx].servCount < (uint8_t)CY_BLE_SRVI_COUNT));

    if(cy_ble_discovery[discIdx].servCount < (uint32_t)CY_BLE_SRVI_COUNT)
    {
        cy_stc_ble_gattc_find_info_req_t reqParam =
        {
            .connHandle = connHandle,
            .range      = charRangeInfo.range
        };

        if(Cy_BLE_GATTC_DiscoverCharacteristicDescriptors(&reqParam) != CY_BLE_SUCCESS)
        {
            Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_DESCR_DISCOVERY_FAILED, &connHandle);
            cy_ble_discovery[discIdx].autoDiscoveryFlag = 0u;
        }
    }
    else /* Discovery done */
    {
        Cy_BLE_SetConnectionState(connHandle, CY_BLE_CONN_STATE_CLIENT_DISCOVERED);
        Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_DISCOVERY_COMPLETE, &connHandle);
        cy_ble_discovery[discIdx].autoDiscoveryFlag = 0u;
    }
}

/******************************************************************************
* Function Name: Cy_BLE_LongProcedureEndEventHandler
***************************************************************************//**
*
*  Handles a Long Procedure End event during an automatic server discovery
*  process.
*
*  \param connHandle:  The connection handle.
*
* \return
*  None
*
******************************************************************************/
void Cy_BLE_LongProcedureEndEventHandler(cy_stc_ble_conn_handle_t connHandle)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag != 0u))
    {
        switch(Cy_BLE_GetConnectionState(connHandle))
        {
            case CY_BLE_CONN_STATE_CLIENT_SRVC_DISCOVERING:
                if((cy_ble_eventHandlerFlag & CY_BLE_ENABLE_ALL_EVENTS) != 0u)
                {
                    Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_SRVC_DISCOVERY_COMPLETE, &connHandle);
                }
                Cy_BLE_SetConnectionState(connHandle, CY_BLE_CONN_STATE_CLIENT_INCL_DISCOVERING);
                Cy_BLE_NextInclDiscovery(connHandle, CY_BLE_DISCOVERY_INIT);
                /* Do not propagate this event to the application level during an automatic discovery procedure */
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;

            case CY_BLE_CONN_STATE_CLIENT_INCL_DISCOVERING:
                Cy_BLE_NextInclDiscovery(connHandle, CY_BLE_DISCOVERY_CONTINUE);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;

            case CY_BLE_CONN_STATE_CLIENT_CHAR_DISCOVERING:
                Cy_BLE_NextCharDiscovery(connHandle, CY_BLE_DISCOVERY_CONTINUE);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;

            case CY_BLE_CONN_STATE_CLIENT_DESCR_DISCOVERING:
                Cy_BLE_NextCharDscrDiscovery(connHandle, CY_BLE_DISCOVERY_CONTINUE);
                cy_ble_eventHandlerFlag &= (uint8_t) ~CY_BLE_CALLBACK;
                break;

            default:
                break;
        }
    }
}


/******************************************************************************
* Function Name: Cy_BLE_ErrorResponseEventHandler
***************************************************************************//**
*
*  Handles an Error Response event during an automatic server discovery
*  process.
*
*  \param eventParam:  The event parameters for an Error Response.
*
* \return
*  None
*
******************************************************************************/
void Cy_BLE_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam)
{
    uint32_t discIdx = Cy_BLE_GetDiscoveryIdx(eventParam->connHandle);

    if((discIdx < CY_BLE_CONFIG_GATTC_COUNT) && (cy_ble_discovery[discIdx].autoDiscoveryFlag != 0u) &&
       (eventParam->errInfo.errorCode == CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND))
    {
        Cy_BLE_LongProcedureEndEventHandler(eventParam->connHandle);
    }
}

#endif /* CY_BLE_GATT_ROLE_CLIENT */
#endif /* defined(CY_BLE_MODE_PROFILE) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
