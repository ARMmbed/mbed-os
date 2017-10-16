/***************************************************************************//**
* \file cy_ble_gap.c
* \version 2.0
*
* \brief
*  This file contains the source code for the GAP API of the BLE Component.
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

#include "cy_ble_hal_pvt.h"
#include <stdlib.h>

/***************************************
* Function Prototypes
***************************************/

#if (CY_BLE_GATT_ROLE_SERVER)
    #if (CY_BLE_GAP_ROLE_PERIPHERAL)
static void Cy_BLE_ChangeAdLocalName(const char8 name[], uint8_t dest);
    #endif /* (CY_BLE_GAP_ROLE_PERIPHERAL) */
#endif     /* (CY_BLE_GATT_ROLE_SERVER) */


/***************************************
* Global Variables
***************************************/

cy_en_ble_state_t cy_ble_state;

#if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)
cy_en_ble_adv_state_t cy_ble_advState;
uint8_t cy_ble_advIndex;
#endif  /* (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER) */

#if (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER)
cy_en_ble_scan_state_t cy_ble_scanState;
uint8_t cy_ble_scanIndex;
#endif /* (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER) */

#if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL))

/* This global variable is initialized after connection with peer device,
 * after CY_BLE_EVT_GATT_CONNECT_IND event, and could be used by application code
 * to send data to peer device.
 */
cy_stc_ble_conn_handle_t cy_ble_connHandle[CY_BLE_CONN_COUNT];
cy_en_ble_conn_state_t cy_ble_connState[CY_BLE_CONN_COUNT];

#if (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES)
uint8_t cy_ble_peerBonding[CY_BLE_CONN_COUNT];
uint8_t cy_ble_pendingFlashWrite;

#if (CY_BLE_GATT_DB_CCCD_COUNT != 0u)
static uint8_t cy_ble_pendingFlashClearCccdHandle;
#endif /* CY_BLE_GATT_DB_CCCD_COUNT != 0u */

#endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */
#endif /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */

#if (CY_BLE_GAP_ROLE_CENTRAL)

cy_stc_ble_gapc_conn_info_t cy_ble_connectionParameters;

/* Connecting timeout is set to 30 seconds in Cy_BLE_Init function.
 * Not zero value starts timer in Cy_BLE_GAPC_ConnectDevice().
 */
cy_stc_ble_timer_info_t cy_ble_connectingTimeout;

#endif /* CY_BLE_GAP_ROLE_CENTRAL */

cy_stc_ble_gap_bd_addr_t * cy_ble_sflashDeviceAddress = CY_BLE_SFLASH_DEVICE_ADDRESS_PTR;


/******************************************************************************
* Function Name: Cy_BLE_ServiceInit
***************************************************************************//**
*
*  Initializes all included services.
*
* \return
*  None
*
******************************************************************************/
void Cy_BLE_ServiceInit(void)
{
    #if (CY_BLE_GATT_ROLE_CLIENT)
    uint32_t idx;
    for(idx = 0u; idx < CY_BLE_CONFIG_GATTC_COUNT; idx++)
    {
        /* Check service range before clearing to support partial discovery */
        if(cy_ble_serverInfo[idx][CY_BLE_SRVI_GAP].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            (void)memset(&cy_ble_discovery[idx].gapc, 0, sizeof(cy_stc_ble_gapc_t));

            /* Initialize UUID  */
            cy_ble_serverInfo[idx][CY_BLE_SRVI_GAP].uuid = CY_BLE_UUID_GAP_SERVICE;
        }

        /* Check service range before clearing to support partial discovery */
        if(cy_ble_serverInfo[idx][CY_BLE_SRVI_GATT].range.startHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            (void)memset(&cy_ble_discovery[idx].gattc, 0, sizeof(cy_stc_ble_gattc_t));

            /* Initialize UUID  */
            cy_ble_serverInfo[idx][CY_BLE_SRVI_GATT].uuid = CY_BLE_UUID_GATT_SERVICE;
        }

        #ifdef CY_BLE_IPSS_CLIENT
        cy_ble_serverInfo[idx][CY_BLE_SRVI_IPSS].uuid = CY_BLE_UUID_INTERNET_PROTOCOL_SUPPORT_SERVICE;
        #endif /* CY_BLE_IPSS_CLIENT */
    }
    #endif     /* CY_BLE_GATT_ROLE_CLIENT */

    #ifdef CY_BLE_ANCS
    (void)Cy_BLE_ANCS_Init(&cy_ble_ancsConfig);
    #endif /* CY_BLE_ANCS */

    #ifdef CY_BLE_ANS
    (void)Cy_BLE_ANS_Init(&cy_ble_ansConfig);
    #endif /* CY_BLE_ANS */

    #ifdef CY_BLE_AIOS
    (void)Cy_BLE_AIOS_Init(&cy_ble_aiosConfig);
    #endif /* CY_BLE_AIOS */

    #ifdef CY_BLE_BAS
    (void)Cy_BLE_BAS_Init(&cy_ble_basConfig);
    #endif /* CY_BLE_BAS */

    #ifdef CY_BLE_BCS
    (void)Cy_BLE_BCS_Init(&cy_ble_bcsConfig);
    #endif /* CY_BLE_BCS */

    #ifdef CY_BLE_BLS
    (void)Cy_BLE_BLS_Init(&cy_ble_blsConfig);
    #endif /* CY_BLE_BLS */

    #ifdef CY_BLE_BMS
    (void)Cy_BLE_BMS_Init(&cy_ble_bmsConfig);
    #endif /* CY_BLE_BMS */

    #ifdef CY_BLE_BTS
    (void)Cy_BLE_BTS_Init(&cy_ble_btsConfig);
    #endif /* CY_BLE_BTS */

    #ifdef CY_BLE_CGMS
    (void)Cy_BLE_CGMS_Init(&cy_ble_cgmsConfig);
    #endif /* CY_BLE_CGMS */

    #ifdef CY_BLE_CPS
    (void)Cy_BLE_CPS_Init(&cy_ble_cpsConfig);
    #endif /* CY_BLE_CPS */

    #ifdef CY_BLE_CSCS
    (void)Cy_BLE_CSCS_Init(&cy_ble_cscsConfig);
    #endif /* CY_BLE_CSCS */

    #ifdef CY_BLE_CTS
    (void)Cy_BLE_CTS_Init(&cy_ble_ctsConfig);
    #endif /* CY_BLE_CTS */

    #ifdef CY_BLE_CUSTOM
    (void)Cy_BLE_CUSTOM_Init(&cy_ble_customConfig);
    #endif /* CY_BLE_CUSTOM */

    #ifdef CY_BLE_DIS
    (void)Cy_BLE_DIS_Init(&cy_ble_disConfig);
    #endif /* CY_BLE_DIS */

    #ifdef CY_BLE_ESS
    (void)Cy_BLE_ESS_Init(&cy_ble_essConfig);
    #endif /* CY_BLE_ESS */

    #ifdef CY_BLE_GLS
    (void)Cy_BLE_GLS_Init(&cy_ble_glsConfig);
    #endif /* CY_BLE_GLS */

    #ifdef CY_BLE_HIDS
    (void)Cy_BLE_HIDS_Init(&cy_ble_hidsConfig);
    #endif /* CY_BLE_HIDS */

    #ifdef CY_BLE_HPS
    (void)Cy_BLE_HPS_Init(&cy_ble_hpsConfig);
    #endif /* CY_BLE_HPS */

    #ifdef CY_BLE_HRS
    (void)Cy_BLE_HRS_Init(&cy_ble_hrsConfig);
    #endif /* CY_BLE_HRS */

    #ifdef CY_BLE_HTS
    (void)Cy_BLE_HTS_Init(&cy_ble_htsConfig);
    #endif /* CY_BLE_RSCS */

    #ifdef CY_BLE_IAS
    (void)Cy_BLE_IAS_Init(&cy_ble_iasConfig);
    #endif /* CY_BLE_IAS */

    #ifdef CY_BLE_IPS
    (void)Cy_BLE_IPS_Init(&cy_ble_ipsConfig);
    #endif /* CY_BLE_IPS */

    #ifdef CY_BLE_LLS
    (void)Cy_BLE_LLS_Init(&cy_ble_llsConfig);
    #endif /* CY_BLE_LLS */

    #ifdef CY_BLE_LNS
    (void)Cy_BLE_LNS_Init(&cy_ble_lnsConfig);
    #endif /* CY_BLE_LNS */

    #ifdef CY_BLE_NDCS
    (void)Cy_BLE_NDCS_Init(&cy_ble_ndcsConfig);
    #endif /* CY_BLE_NDCS */

    #ifdef CY_BLE_PASS
    (void)Cy_BLE_PASS_Init(&cy_ble_passConfig);
    #endif /* CY_BLE_PASS */

    #ifdef CY_BLE_RSCS
    (void)Cy_BLE_RSCS_Init(&cy_ble_rscsConfig);
    #endif /* CY_BLE_RSCS */

    #ifdef CY_BLE_PLXS
    (void)Cy_BLE_PLXS_Init(&cy_ble_plxsConfig);
    #endif /* CY_BLE_PLXS */

    #ifdef CY_BLE_RTUS
    (void)Cy_BLE_RTUS_Init(&cy_ble_rtusConfig);
    #endif /* CY_BLE_RTUS */

    #ifdef CY_BLE_SCPS
    (void)Cy_BLE_SCPS_Init(&cy_ble_scpsConfig);
    #endif /* CY_BLE_SCPS */

    #ifdef CY_BLE_TPS
    (void)Cy_BLE_TPS_Init(&cy_ble_tpsConfig);
    #endif /* CY_BLE_TPS */

    #ifdef CY_BLE_UDS
    (void)Cy_BLE_UDS_Init(&cy_ble_udsConfig);
    #endif /* CY_BLE_UDS */

    #ifdef CY_BLE_WPTS
    (void)Cy_BLE_WPTS_Init(&cy_ble_wptsConfig);
    #endif /* CY_BLE_WPTS */

    #ifdef CY_BLE_WSS
    (void)Cy_BLE_WSS_Init(&cy_ble_wssConfig);
    #endif /* CY_BLE_WSS */
}


/******************************************************************************
* Function Name: Cy_BLE_StoreAppData
***************************************************************************//**
*
*  This function instructs the Stack to backup application specific data into
*  flash. This API must be called by application to backup application specific
*  data.
*
*  \param param: parameter of type 'cy_stc_ble_app_flash_param_t
*   * param->srcBuff: Source buffer
*   * param->destAddr: Destination address
*   * param->buffLen: Length of srcData
*   * param->writeMode: blocked or non-blocked write mode
*
*  This function works in two write modes: blocked or non-blocked :
*    CY_BLE_STORE_DATA_MODE_BLOCKING (0x00) - the function writes all data from
*       srcBuff and returns CY_BLE_SUCCESS when finish. After writing one rows, 
*       the function calls Cy_BLE_ProcessEvents that allows stack to process 
*       pending events.
*    CY_BLE_STORE_DATA_MODE_NON_BLOCKING (0x01) - the function writes one row 
*       per call and returns CY_BLE_INFO_FLASH_WRITE_IN_PROGRESS when write in 
*       progresing or CY_BLE_SUCCESS, when finish.  Afret writing one rows, the
*       function calls Cy_BLE_ProcessEvents that allows stack to process pending
*       events.
*       Application should keep calling this function till it returns 
*       CY_BLE_SUCCESS. 
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                              | Description
*   ------------                             | -----------
*   CY_BLE_SUCCESS                           | On successful operation
*   CY_BLE_INFO_FLASH_WRITE_IN_PROGRESS      | Write operation in progress
*   CY_BLE_ERROR_INVALID_PARAMETER           | Invalid input parameter
*   CY_BLE_ERROR_FLASH_WRITE                 | Error in flash Write
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_StoreAppData(const cy_stc_ble_app_flash_param_t *param)
{
    static cy_en_ble_api_result_t storeState = CY_BLE_SUCCESS;
    static uint32_t tLength = 0u;

    uint8_t *tSrcPtr   = (uint8_t *) param->srcBuff;
    uint32_t tOffset   = (uint32) param->destAddr - CY_FLASH_BASE;  /* Get offset */
    uint32_t blockSize;      
    bool exitFlag = false;
    
    switch (storeState)
    {           
        case CY_BLE_SUCCESS: 
        case CY_BLE_ERROR_INVALID_PARAMETER:
        case CY_BLE_ERROR_FLASH_WRITE:
            tLength = param->buffLen;
            break;
            
        case CY_BLE_INFO_FLASH_WRITE_IN_PROGRESS:
            if (tLength == 0u)
            {
                storeState = CY_BLE_SUCCESS;
                exitFlag = true;
            }
            else
            {
                /* Restore offset and srcPtr according to length */
                tSrcPtr += param->buffLen - tLength;
                tOffset += param->buffLen - tLength;
            }
            break;
            
       default:
            break;
    }
    
    while((tLength !=0u) && (exitFlag == false))
    {
        /* Split srcBuff into parts of flash row size (512 bytes) */
        blockSize = (tOffset >> 9) << 9;                            /* Get current row size regarding to address */
        blockSize = CY_FLASH_SIZEOF_ROW - (tOffset - blockSize);    /* Calculate final length of the block which
                                                                       we can write to row */
        blockSize = (blockSize > tLength )? tLength : blockSize;

        /* Write 512 byte block */
        storeState = Cy_BLE_HAL_NvramWrite(tSrcPtr, (uint8_t *)(tOffset + CY_FLASH_BASE), blockSize);

        tSrcPtr += blockSize;
        tOffset += blockSize;
        tLength -= blockSize;
        
        /* For CY_BLE_STORE_DATA_MODE_NON_BLOCKING mode write one row per call */
        if(param->writeMode == CY_BLE_STORE_DATA_MODE_NON_BLOCKING)
        {
            if (storeState == CY_BLE_SUCCESS)
            {
                storeState = CY_BLE_INFO_FLASH_WRITE_IN_PROGRESS;
            } 
            exitFlag = true;
        }
        
        /* Cy_BLE_ProcessEvents() allows BLE stack to process pending events */
        Cy_BLE_ProcessEvents();
    }
    
    return(storeState);
}


#if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))


/******************************************************************************
* Function Name: Cy_BLE_StoreBondingData
***************************************************************************//**
*
*  This function writes the new bonding data from RAM to the dedicated Flash
*  location as defined by the component. It performs data comparing between RAM
*  and Flash before writing to Flash. If there is no change between RAM and Flash
*  data, then no write is performed. It writes only one flash row in one call.
*  Application should keep calling this function till it returns CY_BLE_SUCCESS.
*  This function is available only when Bonding requirement is selected in
*  Security settings.
*
*
* \return
*  Return value is of type cy_en_ble_api_result_t.
*
*   Error Codes                           | Description
*   ----------------------------------    | ------------------------------------
*   CY_BLE_SUCCESS                        | On successful operation
    CY_BLE_INFO_FLASH_WRITE_IN_PROGRESS   | Writing in progress
*   CY_BLE_ERROR_FLASH_WRITE_NOT_PERMITED | Flash Write is not permitted
*   CY_BLE_ERROR_FLASH_WRITE              | Error in flash Write
*
*  \globalvars
*   The cy_ble_pendingFlashWrite variable is used to detect status
*   of pending write to flash operation for stack data and CCCD.
*   This function automatically clears pending bits after write operation
*   complete.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_StoreBondingData(void)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;

    /* Store Stack data */
    if((cy_ble_pendingFlashWrite & CY_BLE_PENDING_STACK_FLASH_WRITE_BIT) != 0u)
    {
        cy_stc_ble_stack_flash_param_t stackFlashParam =
        {
            .bitField = CY_BLE_PERSISTENT_BOND_LIST_BITMASK |
                        CY_BLE_PERSISTENT_RPA_LIST_BITMASK |
                        CY_BLE_PERSISTENT_WHITELIST_BITMASK
        };

        apiResult = Cy_BLE_StoreStackData(&stackFlashParam);
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_pendingFlashWrite &= (uint8_t) ~CY_BLE_PENDING_STACK_FLASH_WRITE_BIT;
            
            /* Change apiResult if there are more pending data to store */
            if((cy_ble_pendingFlashWrite & CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT) != 0u)
            {
                apiResult = CY_BLE_INFO_FLASH_WRITE_IN_PROGRESS;
            }
        }
    }
#if (CY_BLE_GATT_DB_CCCD_COUNT != 0u)
    
    /* Store CCCD values */
    if((apiResult == CY_BLE_SUCCESS) &&
       ((cy_ble_pendingFlashWrite & CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT) != 0u))
    {
        uint32_t i;
        cy_stc_ble_app_flash_param_t appFlashParam =
        {
            .buffLen = CY_BLE_GATT_DB_CCCD_COUNT + CY_BLE_CCCD_CRC_BYTE,
        };

        /* Update CCCD values from RAM for active connections supporting bonding */
        for(i = 0u; i < CY_BLE_CONN_COUNT; i++)
        {
            if((Cy_BLE_GetConnectionState(cy_ble_connHandle[i]) >= CY_BLE_CONN_STATE_CONNECTED) &&
               (cy_ble_peerBonding[cy_ble_connHandle[i].attId] == CY_BLE_GAP_BONDING))
            {
                uint8_t cccdBlockCrc;
                appFlashParam.srcBuff = (uint8_t*)cy_ble_attValuesCccdMultiple[cy_ble_connHandle[i].attId];
                appFlashParam.destAddr = (uint8_t*)cy_ble_configPtr->flashStorage->attValuesCCCDFlashMemory
                                         [cy_ble_connHandle[i].bdHandle];

                /* Calculate CRC for CCCD block */
                cccdBlockCrc = Cy_BLE_HAL_CalcCRC8(cy_ble_attValuesCccdMultiple[cy_ble_connHandle[i].attId], 
                                                   CY_BLE_GATT_DB_CCCD_COUNT);
                /* Store new CRC value */                    
                cy_ble_attValuesCccdMultiple[cy_ble_connHandle[i].attId][CY_BLE_GATT_DB_CCCD_COUNT] = cccdBlockCrc;
                
                apiResult = Cy_BLE_StoreAppData(&appFlashParam);
                if(apiResult != CY_BLE_SUCCESS)
                {
                    break;
                }
            }
        }
        if(apiResult == CY_BLE_SUCCESS)
        {
            cy_ble_pendingFlashWrite &= (uint8_t) ~CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT;
        }
    }

    /* Clear requested CCCD values */
    if((apiResult == CY_BLE_SUCCESS) &&
       ((cy_ble_pendingFlashWrite & CY_BLE_PENDING_CCCD_FLASH_CLEAR_MASK) != 0u))
    {
        if((cy_ble_pendingFlashWrite & CY_BLE_PENDING_CCCD_FLASH_CLEAR_ALL_BIT) != 0u)
        {
            /* Remove CCCD values for all bonded devices */
            uint8_t defaultValuesCCCD[CY_BLE_MAX_BONDED_DEVICES + CY_BLE_CONN_COUNT][CY_BLE_GATT_DB_CCCD_COUNT + CY_BLE_CCCD_CRC_BYTE];
            cy_stc_ble_app_flash_param_t appFlashParam =
            {
                .srcBuff  = (uint8_t*)defaultValuesCCCD,
                .destAddr = (uint8_t*)cy_ble_configPtr->flashStorage->attValuesCCCDFlashMemory,
                .buffLen  = sizeof(defaultValuesCCCD),
            };

            (void)memset(defaultValuesCCCD, 0, sizeof(defaultValuesCCCD));

            apiResult = Cy_BLE_StoreAppData(&appFlashParam);
            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_pendingFlashWrite &= (uint8_t) ~CY_BLE_PENDING_CCCD_FLASH_CLEAR_ALL_BIT;
            }
        }
        else /* Remove CCCD values for particular device */
        {
            uint8_t defaultValuesCCCD[CY_BLE_GATT_DB_CCCD_COUNT + CY_BLE_CCCD_CRC_BYTE] = { 0u };
            cy_stc_ble_app_flash_param_t appFlashParam =
            {
                .srcBuff  = (uint8_t*)defaultValuesCCCD,
                .destAddr = (uint8_t*)cy_ble_configPtr->flashStorage->
                             attValuesCCCDFlashMemory[cy_ble_pendingFlashClearCccdHandle],
                .buffLen  = CY_BLE_GATT_DB_CCCD_COUNT + CY_BLE_CCCD_CRC_BYTE,
            };

            apiResult = Cy_BLE_StoreAppData(&appFlashParam);
            if(apiResult == CY_BLE_SUCCESS)
            {
                cy_ble_pendingFlashWrite &= (uint8_t) ~CY_BLE_PENDING_CCCD_FLASH_CLEAR_BIT;
            }
        }
    }
#endif /* CY_BLE_GATT_DB_CCCD_COUNT != 0u */
    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GAP_RemoveBondedDevice
***************************************************************************//**
*
*  This function marks the device untrusted. It removes the bonding information
*  of the device including CCCD values. This function removes device from the
*  white list also when auto-populate white list with bonded devices option is
*  enabled.
*
*  This function is available only when Bonding requirement is selected in
*  Security settings.
*
*  \param bdAddr: Pointer to peer device address, of type cy_stc_ble_gap_bd_addr_t.
*                 If device address is set to 0, then all devices shall be
*                 removed from trusted list and white list.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*   Error Codes                       | Description
*   --------------------------------- | ------------------------------------
*   CY_BLE_SUCCESS                    | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER    | On specifying NULL as input parameter for 'bdAddr'.
*   CY_BLE_ERROR_INVALID_OPERATION    | Whitelist is already in use or there is pending write to flash operation.
*   CY_BLE_ERROR_NO_DEVICE_ENTITY     | Device does not exist in the bond list.
*
*  \globalvars
*   The bdHandle is set in cy_ble_pendingFlashWrite variable to indicate that
*   data should be stored to flash by Cy_BLE_StoreBondingData afterwards.
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAP_RemoveBondedDevice(cy_stc_ble_gap_bd_addr_t* bdAddr)
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    cy_stc_ble_gap_peer_addr_info_t peerAddrInfoParam;

    if(cy_ble_pendingFlashWrite == 0u)
    {
    #if (CY_BLE_GATT_DB_CCCD_COUNT != 0u)
        /* Request to clear CCCD values which will be done by Cy_BLE_StoreBondingData() */
        cy_stc_ble_gap_bd_addr_t invalidBdAddr = { { 0u, 0u, 0u, 0u, 0u, 0u }, 0u };

        if(memcmp(((uint8_t*)&(bdAddr->bdAddr)), ((uint8_t*)&(invalidBdAddr.bdAddr)), CY_BLE_GAP_BD_ADDR_SIZE) == 0u)
        {
            /* Request to remove all bonded devices by Cy_BLE_StoreBondingData() */
            cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_CLEAR_ALL_BIT;
        }
        else
        {
            /* Get the BD handle from Address */
            peerAddrInfoParam.bdAddr = *bdAddr;
            apiResult = Cy_BLE_GAP_GetPeerBdHandle(&peerAddrInfoParam);
            if(apiResult == CY_BLE_SUCCESS)
            {
                /* Store BD handle to clear CCCD values by Cy_BLE_StoreBondingData() */
                cy_ble_pendingFlashWrite |= CY_BLE_PENDING_CCCD_FLASH_CLEAR_BIT;
                cy_ble_pendingFlashClearCccdHandle = peerAddrInfoParam.bdHandle;
            }
        }
    #endif /* CY_BLE_GATT_DB_CCCD_COUNT != 0u */
        if(apiResult == CY_BLE_SUCCESS)
        {
            apiResult = Cy_BLE_GAP_RemoveDeviceFromBondList(bdAddr);
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }

    return(apiResult);
}


#endif /* (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */


#if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)

/******************************************************************************
 *  Function Name: Cy_BLE_GAPP_StartAdvertisement
 ***************************************************************************//**
 *
 *  This function is used to start the advertisement using the advertisement data
 *  set in the component customizer's GUI indicated by the advertisingParamIndex.
 *  After invoking this function, the device will be available for connection by
 *  the devices configured for GAP central role. It is only included if the
 *  device is configured for GAP Peripheral or GAP Peripheral + Central role.
 *
 *  On start of advertisement, GAP Peripheral receives the
 *  CY_BLE_EVT_GAPP_ADVERTISEMENT_START_STOP event. The following events are
 *  possible on invoking this function:
 *
 *  * CY_BLE_EVT_GAP_DEVICE_CONNECTED - If the device connects to a GAP Central and
 *     Link Layer Privacy is disabled in component customizer.
 *  * CY_BLE_EVT_GAP_ENHANCE_CONN_COMPLETE - If the device connects to a GAP Central and
 *     Link Layer Privacy is enabled in component customizer.
 *  * CY_BLE_EVT_TIMEOUT: If no device in GAP Central mode connects to this
 *                       device within the specified timeout limit. Stack
 *                       automatically initiate stop advertising when Slow
 *                       advertising was initiated, or starts Slow advertising
 *                       after Fast advertising timeout occur.
 *  * CY_BLE_EVT_GAPP_ADVERTISEMENT_START_STOP: If device started or stopped
 *                       advertising. Use Cy_BLE_GetAdvertisementState()to
 *                       determine the state. Sequential advertising could be
 *                       started when CY_BLE_ADV_STATE_STOPPED state is returned.
 *
 *  \param advertisingIntervalType: Fast or slow advertising interval with timings
 *                                 entered in Advertising settings section of the
 *                                 customizer.
 *  * CY_BLE_ADVERTISING_FAST   0x00u
 *  * CY_BLE_ADVERTISING_SLOW   0x01u
 *  * CY_BLE_ADVERTISING_CUSTOM 0x02u
 *
 *  \param advertisingParamIndex: The index of the peripheral and broadcast
 *                                configuration in customizer. For example:
 *  * CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX     0x00
 *  * CY_BLE_BROADCASTER_CONFIGURATION_0_INDEX    0x01
 *
 * \return
 *  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
 *  failed. The following are possible error codes.
 *
 *   Error Codes                       |  Description
 *   --------------------------------- |  --------------------------------
 *   CY_BLE_SUCCESS                    |  On successful operation.
 *   CY_BLE_ERROR_INVALID_PARAMETER    |  On passing an invalid parameter.
 *   CY_BLE_ERROR_INVALID_STATE        |  On calling this function not in Stopped state.
 *   CY_BLE_ERROR_INVALID_OPERATION    |  The operation is not permitted due to connection
 *                                     |  limit exceeded. 
 *
 *******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPP_StartAdvertisement(uint8_t advertisingIntervalType,
                                                      uint8_t advertisingParamIndex)
{
    cy_en_ble_api_result_t apiResult;

    if((advertisingIntervalType > CY_BLE_ADVERTISING_CUSTOM) || (advertisingParamIndex >= CY_BLE_GAPP_CONF_COUNT))
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
#if (CY_BLE_GAP_ROLE_PERIPHERAL) 
    else if((Cy_BLE_GetNumOfActiveConn() == CY_BLE_CONN_COUNT) && 
            ((cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advParam->advType <
                CY_BLE_GAPP_SCANNABLE_UNDIRECTED_ADV) ||
             (cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advParam->advType >
                CY_BLE_GAPP_NON_CONNECTABLE_UNDIRECTED_ADV)))
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
#endif /*CY_BLE_GAP_ROLE_PERIPHERAL */
    else if(Cy_BLE_GetAdvertisementState() == CY_BLE_ADV_STATE_STOPPED)
    {
        cy_ble_advIndex = advertisingParamIndex;
        if(advertisingIntervalType == CY_BLE_ADVERTISING_FAST)
        {
            cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advTo =
                cy_ble_configPtr->gappAdvParams[advertisingParamIndex].fastAdvTimeOut;
            cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advParam->advIntvMin =
                cy_ble_configPtr->gappAdvParams[advertisingParamIndex].fastAdvIntervalMin;
            cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advParam->advIntvMax =
                cy_ble_configPtr->gappAdvParams[advertisingParamIndex].fastAdvIntervalMax;
        }
        else if(advertisingIntervalType == CY_BLE_ADVERTISING_SLOW)
        {
            cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advTo =
                cy_ble_configPtr->gappAdvParams[advertisingParamIndex].slowAdvTimeOut;
            cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advParam->advIntvMin =
                cy_ble_configPtr->gappAdvParams[advertisingParamIndex].slowAdvIntervalMin;
            cy_ble_configPtr->discoveryModeInfo[advertisingParamIndex].advParam->advIntvMax =
                cy_ble_configPtr->gappAdvParams[advertisingParamIndex].slowAdvIntervalMax;
        }
        else /* Do not update advertising intervals */
        {
        }
        cy_ble_advertisingIntervalType = advertisingIntervalType;
        apiResult = Cy_BLE_GAPP_EnterDiscoveryMode(&cy_ble_configPtr->discoveryModeInfo[cy_ble_advIndex]);

        if(apiResult == CY_BLE_SUCCESS)
        {
            Cy_BLE_SetAdvertisementState(CY_BLE_ADV_STATE_ADV_INITIATED);
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GAPP_StopAdvertisement
***************************************************************************//**
*
*   This function can be used to exit from discovery mode. After the execution
*   of this function, there will no longer be any advertisements. On stopping
*   advertising, GAP Peripheral receives CY_BLE_EVT_GAPP_ADVERTISEMENT_START_STOP
*   event. It is expected that the application layer tracks the function call
*   performed before occurrence of this event as this event can occur on making
*   a call to Cy_BleGappStartAdvertisement(), Cy_BLE_GAPP_EnterDiscoveryMode(),
*   or Cy_BLE_GAPP_StartAdvertisement() functions as well.
*
*   The following event occurs on invoking this function:
*   * CY_BLE_EVT_GAPP_ADVERTISEMENT_START_STOP
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*   Error Codes                           | Description
*   ------------                          | -----------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*   CY_BLE_ERROR_INVALID_STATE            | On calling this function not in Advertising state.
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPP_StopAdvertisement(void)
{
    cy_en_ble_api_result_t apiResult;

    if(Cy_BLE_GetAdvertisementState() != CY_BLE_ADV_STATE_STOPPED)
    {
        apiResult = Cy_BLE_GAPP_ExitDiscoveryMode();
        if(apiResult == CY_BLE_SUCCESS)
        {
            Cy_BLE_SetAdvertisementState(CY_BLE_ADV_STATE_STOP_INITIATED);
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_ChangeAdDeviceAddress
***************************************************************************//**
*
*  This function is used to set the Bluetooth device address into the
*  advertisement or scan response data structures.
*
*  \param bdAddr: Bluetooth Device address. The variable is of type cy_stc_ble_gap_bd_addr_t.
*  \param dest: 0 - selects advertisement structure, not zero value selects scan
*               response structure.
*
* \return
*  None
*
*******************************************************************************/
void Cy_BLE_ChangeAdDeviceAddress(const cy_stc_ble_gap_bd_addr_t* bdAddr,
                                  uint8_t dest)
{
    uint32_t i;
    uint32_t fFlag;
    uint32_t adLength;
    uint32_t byteCounter;
    uint32_t maxLength;
    uint32_t advIndex = 0u;
    uint8_t *destBuffer;

    do
    {
        fFlag = 0u;
        byteCounter = 0u;

        if(dest == 0u)      /* Destination - advertising structure */
        {
            destBuffer = cy_ble_configPtr->discoveryModeInfo[advIndex].advData->advData;
            maxLength = cy_ble_configPtr->discoveryModeInfo[advIndex].advData->advDataLen;
        }
        else                /* Destination - scan response structure */
        {
            destBuffer = cy_ble_configPtr->discoveryModeInfo[advIndex].scanRspData->scanRspData;
            maxLength = cy_ble_configPtr->discoveryModeInfo[advIndex].scanRspData->scanRspDataLen;
        }

        while((byteCounter < maxLength) && (fFlag == 0u))
        {
            adLength = destBuffer[byteCounter];

            if(adLength != 0u)
            {
                /* Increment byte counter so it can point to AD type */
                byteCounter++;

                if(destBuffer[byteCounter] == CY_BLE_ADV_DEVICE_ADDR)
                {
                    /* Start of the device address type was found. Set flag and exit the loop. */
                    fFlag = 1u;
                }
                else
                {
                    byteCounter += adLength;
                }
            }
            else
            {
                /* The end of advertisement data structure was encountered though exit the loop. */
                break;
            }
        }

        if(fFlag != 0u)
        {
            /* Increment byte counter so it can point to Device address */
            byteCounter++;

            /* Update Device Address type */
            destBuffer[byteCounter] = bdAddr->type;

            for(i = 0u; i < CY_BLE_GAP_BD_ADDR_SIZE; i++)
            {
                destBuffer[byteCounter + i + 1u] = bdAddr->bdAddr[i];
            }
        }
        advIndex++;
    }
    while(advIndex < CY_BLE_GAPP_CONF_COUNT);
}

#endif /* CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER */


#if (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER)

/******************************************************************************
* Function Name: Cy_BLE_GAPC_StartScan
***************************************************************************//**
*
*   This function is used for discovering GAP peripheral devices that are
*   available for connection. It performs the scanning routine using the
*   parameters entered in the component's customizer indicated by
*   scanParamIndex parameter.
*
*   As soon as the discovery operation starts, CY_BLE_EVT_GAPC_SCAN_START_STOP
*   event is generated. The CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT event is
*   generated when a GAP peripheral device is located. There are three discovery
*   procedures can be selected in the customizer's GUI:
*
*   * Observation procedure: A device performing the observer role receives only
*                            advertisement data from devices irrespective of
*                            their discoverable mode settings. Advertisement
*                            data received is provided by the event,
*                            CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT. This procedure
*                            requires the scanType sub parameter to be passive
*                            scanning.
*
*   * Discovery procedure: A device performing the discovery procedure receives
*                          the advertisement data and scan response data from
*                          devices in both limited discoverable mode and the
*                          general discoverable mode. Received data is provided
*                          by the event, CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT.
*                          This procedure requires the scanType sub-parameter
*                          to be active scanning.
*
*   Every Advertisement / Scan response packet is received in a new event,
*   CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT.
*   If 'scanTo' sub-parameter is a non-zero value, then upon commencement of
*   discovery procedure and elapsed time = 'scanTo', CY_BLE_EVT_TIMEOUT event
*   is generated with the event parameter indicating CY_BLE_GAP_SCAN_TO.
*   Possible generated events are:
*   * CY_BLE_EVT_GAPC_SCAN_START_STOP: If a device started or stopped scanning.
*                                     Use Cy_BLE_GetScanState() to determine the
*                                     state. Sequential scanning could be
*                                     started when CY_BLE_ADV_STATE_STOPPED
*                                     state is returned.
*   * CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT
*   * CY_BLE_EVT_TIMEOUT (CY_BLE_GAP_SCAN_TO)
*
*   \param scanningIntervalType:  Fast or slow scanning interval with
*       timings entered in Scan settings section of the customizer.
*       * CY_BLE_SCANNING_FAST   0x00u
*       * CY_BLE_SCANNING_SLOW   0x01u
*       * CY_BLE_SCANNING_CUSTOM 0x02u
*
*  \param scanParamIndex:  The index of the central and scan configuration
*                          in customizer.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*   Error Codes                        | Description
*   ---------------------------------- | -----------------------------------
*   CY_BLE_SUCCESS                     | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER     | On passing an invalid parameter.
*   CY_BLE_ERROR_INVALID_STATE         | On calling this function not in Stopped state.
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPC_StartScan(uint8_t scanningIntervalType,
                                             uint8_t scanParamIndex)
{
    cy_en_ble_api_result_t apiResult;

    if(scanningIntervalType > CY_BLE_SCANNING_CUSTOM)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(Cy_BLE_GetScanState() == CY_BLE_SCAN_STATE_STOPPED)
    {
        cy_ble_scanIndex = scanParamIndex;
        if(scanningIntervalType == CY_BLE_SCANNING_FAST)
        {
            cy_ble_configPtr->discoveryInfo[scanParamIndex].scanTo =
                cy_ble_configPtr->gapcScanParams[scanParamIndex].fastScanTimeOut;
            cy_ble_configPtr->discoveryInfo[scanParamIndex].scanIntv =
                cy_ble_configPtr->gapcScanParams[scanParamIndex].fastScanInterval;
            cy_ble_configPtr->discoveryInfo[scanParamIndex].scanWindow =
                cy_ble_configPtr->gapcScanParams[scanParamIndex].fastScanWindow;
        }
        else if(scanningIntervalType == CY_BLE_SCANNING_SLOW)
        {
            cy_ble_configPtr->discoveryInfo[scanParamIndex].scanTo =
                cy_ble_configPtr->gapcScanParams[scanParamIndex].slowScanTimeOut;
            cy_ble_configPtr->discoveryInfo[scanParamIndex].scanIntv =
                cy_ble_configPtr->gapcScanParams[scanParamIndex].slowScanInterval;
            cy_ble_configPtr->discoveryInfo[scanParamIndex].scanWindow =
                cy_ble_configPtr->gapcScanParams[scanParamIndex].slowScanWindow;
        }
        else /* Do not update scanning intervals */
        {
        }

        cy_ble_scanningIntervalType = scanningIntervalType;
        apiResult = Cy_BLE_GAPC_StartDiscovery(&cy_ble_configPtr->discoveryInfo[scanParamIndex]);

        if(apiResult == CY_BLE_SUCCESS)
        {
            Cy_BLE_SetScanState(CY_BLE_SCAN_STATE_SCAN_INITIATED);
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GAPC_StopScan
***************************************************************************//**
*
*   This function used to stop the discovery of devices. On stopping discovery
*   operation, CY_BLE_EVT_GAPC_SCAN_START_STOP event is generated. Application
*   layer needs to keep track of the function call made before receiving this
*   event to associate this event with either the start or stop discovery
*   function.
*
*   Possible events generated are:
*    * CY_BLE_EVT_GAPC_SCAN_START_STOP
*
*  \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*   Error Codes                           | Description
*   ------------------------------------- | -----------------------------------
*   CY_BLE_SUCCESS                        | On successful operation.
*   CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED | Memory allocation failed.
*   CY_BLE_ERROR_INVALID_STATE            | On calling this function not in Scanning state.
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPC_StopScan(void)
{
    cy_en_ble_api_result_t apiResult;

    if(Cy_BLE_GetScanState() == CY_BLE_SCAN_STATE_SCANNING)
    {
        apiResult = Cy_BLE_GAPC_StopDiscovery();
        if(apiResult == CY_BLE_SUCCESS)
        {
            Cy_BLE_SetScanState(CY_BLE_SCAN_STATE_STOP_INITIATED);
        }
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }

    return(apiResult);
}


#endif /* CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER */


#if (CY_BLE_GAP_ROLE_CENTRAL)

/******************************************************************************
* Function Name: Cy_BLE_GAPC_ConnectDevice
***************************************************************************//**
*
*  This function is used to send a connection request to the remote device with
*  the connection parameters set in the component customizer. This function needs
*  to be called only once after the target device is discovered by
*  Cy_BLE_GAPC_StartScan() and further scanning has stopped. Scanning is
*  successfully stopped on invoking Cy_BLE_GAPC_StopScan() and then receiving the
*  event CY_BLE_EVT_GAPC_SCAN_START_STOP with sub-parameter 'success' = 0x01u.
*
*  On successful connection, the following events are generated at the GAP
*  Central device (as well as the GAP Peripheral device), in the following order.
*  * CY_BLE_EVT_GATT_CONNECT_IND
*  * CY_BLE_EVT_GAP_DEVICE_CONNECTED - If the device connects to a GAP Central and
*    Link Layer Privacy is disabled in component customizer.
*  * CY_BLE_EVT_GAP_ENHANCE_CONN_COMPLETE - If the device connects to a GAP Central and
*    Link Layer Privacy is enabled in component customizer.
*  * CY_BLE_EVT_GAP_DEVICE_CONNECTED
*
*  A procedure is considered to have timed out if a connection response packet is
*  not received within time set by cy_ble_connectingTimeout global variable
*  (30 seconds by default). CY_BLE_EVT_TIMEOUT event with CY_BLE_GENERIC_APP_TO parameter
*  will indicate about connection procedure timeout. Connection will automatically
*  be canceled and state will be changed to CY_BLE_STATE_ON.
*
*  \param address: The device address of the remote device to connect to.
*  \param centralParamIndex: The index of the central configuration in customizer.
*    For example:
*  * CY_BLE_CENTRAL_CONFIGURATION_0_INDEX    0x00
*  * CY_BLE_CENTRAL_CONFIGURATION_1_INDEX    0x01
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*   Error Codes                       | Description
*   ------------                      | -----------
*   CY_BLE_SUCCESS                    | On successful operation.
*   CY_BLE_ERROR_STACK_INTERNAL       | On error occurred in the BLE stack.
*   CY_BLE_ERROR_INVALID_PARAMETER    | On passing an invalid parameter.
*   CY_BLE_ERROR_INVALID_STATE        | On calling this function not in Disconnected state.
*   CY_BLE_ERROR_INVALID_OPERATION    | The operation is not permitted due to connection
*                                     | limit exceeded. 
*
*   Note: Please refer the description of Cy_BLE_GAPC_InitConnection() for recommended
*   Connection Interval values.
*
*
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPC_ConnectDevice(const cy_stc_ble_gap_bd_addr_t * address,
                                                 uint8_t centralParamIndex)
{   
#if (CY_BLE_GAP_ROLE_PERIPHERAL)
    cy_en_ble_adv_state_t advState = Cy_BLE_GetAdvertisementState();
#endif /* CY_BLE_GAP_ROLE_PERIPHERAL */

    cy_en_ble_api_result_t apiResult;
    uint32_t i;

    if(Cy_BLE_GetState() != CY_BLE_STATE_ON)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else if(address == NULL)
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }
    else if(Cy_BLE_GetNumOfActiveConn() == CY_BLE_CONN_COUNT)
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
#if (CY_BLE_GAP_ROLE_PERIPHERAL)
    else if(((CY_BLE_CONN_COUNT - Cy_BLE_GetNumOfActiveConn()) == 1u) && (advState == CY_BLE_ADV_STATE_ADVERTISING) && 
            ((cy_ble_configPtr->discoveryModeInfo[cy_ble_advIndex].advParam->advType < 
                CY_BLE_GAPP_SCANNABLE_UNDIRECTED_ADV) ||
             (cy_ble_configPtr->discoveryModeInfo[cy_ble_advIndex].advParam->advType > 
                CY_BLE_GAPP_NON_CONNECTABLE_UNDIRECTED_ADV)))
    {
        apiResult = CY_BLE_ERROR_INVALID_OPERATION;
    }
#endif /* CY_BLE_GAP_ROLE_PERIPHERAL */
    else
    {
        /* Init default connection parameters.
         * Application may change it before initiating a connection */
        cy_ble_connectionParameters.scanIntv = cy_ble_configPtr->discoveryInfo[centralParamIndex].scanIntv;
        cy_ble_connectionParameters.scanWindow = cy_ble_configPtr->discoveryInfo[centralParamIndex].scanWindow;
        cy_ble_connectionParameters.ownAddrType = cy_ble_configPtr->discoveryInfo[centralParamIndex].ownAddrType;
        cy_ble_connectionParameters.initiatorFilterPolicy =
            cy_ble_configPtr->discoveryInfo[centralParamIndex].scanFilterPolicy;
        cy_ble_connectionParameters.connIntvMin =
            cy_ble_configPtr->gapcScanParams[centralParamIndex].gapcConnectionIntervalMin;
        cy_ble_connectionParameters.connIntvMax =
            cy_ble_configPtr->gapcScanParams[centralParamIndex].gapcConnectionIntervalMax;
        cy_ble_connectionParameters.connLatency =
            cy_ble_configPtr->gapcScanParams[centralParamIndex].gapcConnectionSlaveLatency;
        cy_ble_connectionParameters.supervisionTO =
            cy_ble_configPtr->gapcScanParams[centralParamIndex].gapcConnectionTimeOut;
        cy_ble_connectionParameters.minCeLength = 0x0000u;
        cy_ble_connectionParameters.maxCeLength = 0xFFFFu;
        cy_ble_connectingTimeout.timeout = CY_BLE_GAPC_CONNECTING_TIMEOUT;

        for(i = 0u; i < CY_BLE_GAP_BD_ADDR_SIZE; i++)
        {
            cy_ble_connectionParameters.peerBdAddr[i] = address->bdAddr[i];
        }
        cy_ble_connectionParameters.peerAddrType = address->type;

        apiResult = Cy_BLE_StartTimer(&cy_ble_connectingTimeout);

        if(apiResult == CY_BLE_SUCCESS)
        {
            apiResult = Cy_BLE_GAPC_InitConnection(&cy_ble_connectionParameters);

            if(apiResult == CY_BLE_SUCCESS)
            {
                Cy_BLE_SetState(CY_BLE_STATE_CONNECTING);
            }
        }
        else
        {
            (void)Cy_BLE_StopTimer(&cy_ble_connectingTimeout);
        }
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GAPC_CancelDeviceConnection
***************************************************************************//**
*
*  This function cancels a previously initiated connection with the remote
*  device. It is a blocking function. No event is generated on calling this
*  function. If the devices are already connected then this function should not
*  be used. If you intend to disconnect from an existing connection, the function
*  Cy_BLE_GAP_Disconnect() should be used.
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*   Error Codes                         | Description
*   ----------------------------------- | --------------------------------------
*   CY_BLE_SUCCESS                      | On successful operation.
*   CY_BLE_ERROR_STACK_INTERNAL         | An error occurred in the BLE stack.
*   CY_BLE_ERROR_INVALID_STATE          | On calling this function not in Connecting state.
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GAPC_CancelDeviceConnection(void)
{
    cy_en_ble_api_result_t apiResult;

    if(Cy_BLE_GetState() != CY_BLE_STATE_CONNECTING)
    {
        apiResult = CY_BLE_ERROR_INVALID_STATE;
    }
    else
    {
        apiResult = Cy_BLE_GAPC_CancelConnection();

        if(cy_ble_connectingTimeout.timeout != 0u)
        {
            (void)Cy_BLE_StopTimer(&cy_ble_connectingTimeout);
        }

        if(apiResult == CY_BLE_SUCCESS)
        {
            Cy_BLE_SetState(CY_BLE_STATE_ON);
        }
    }

    return(apiResult);
}

#endif /* CY_BLE_GAP_ROLE_CENTRAL */


#if (CY_BLE_GATT_ROLE_SERVER)

#if (CY_BLE_GAP_ROLE_PERIPHERAL)

/******************************************************************************
* Function Name: Cy_BLE_ChangeAdLocalName
***************************************************************************//**
*  This function is used to set the local device name in the advertisement or scan
*  response data structure.
*
*  \param name: The local device name string to be set in advertisement data
*            structure.
*  \param dest: 0 - advertisement structure, not zero value selects scan response
*              structure.
*
* \return
*  None
*
*******************************************************************************/
static void Cy_BLE_ChangeAdLocalName(const char8 name[],
                                     uint8_t dest)
{
    uint32_t i;
    uint8_t fFlag;
    uint8_t adLength;
    uint8_t byteCounter;
    uint8_t *destBuffer;
    uint8_t maxLength;
    uint8_t advIndex = 0u;

    do
    {
        fFlag = 0u;
        adLength = 0u;
        byteCounter = 0u;

        if(dest == 0u)      /* Destination - advertising structure */
        {
            destBuffer = cy_ble_configPtr->discoveryModeInfo[advIndex].advData->advData;
            maxLength = cy_ble_configPtr->discoveryModeInfo[advIndex].advData->advDataLen;
        }
        else                /* Destination - scan response structure */
        {
            destBuffer = cy_ble_configPtr->discoveryModeInfo[advIndex].scanRspData->scanRspData;
            maxLength = cy_ble_configPtr->discoveryModeInfo[advIndex].scanRspData->scanRspDataLen;
        }

        while((byteCounter < maxLength) && (fFlag == 0u))
        {
            adLength = destBuffer[byteCounter];

            if(adLength != 0u)
            {
                /* Increment byte counter so it can point to AD type */
                byteCounter++;

                if((CY_BLE_SHORT_LOCAL_NAME == destBuffer[byteCounter]) ||
                   (CY_BLE_COMPLETE_LOCAL_NAME == destBuffer[byteCounter]))
                {
                    /* Start of the Local Name AD type was found. Set flag and exit the loop. */
                    fFlag = 1u;
                }
                else
                {
                    byteCounter += adLength;
                }
            }
            else
            {
                /* The end of advertisement data structure was encountered though exit the loop. */
                break;
            }
        }

        if(fFlag != 0u)
        {
            /* Reuse "adLength" to hold location of the last character of local name in
             * AD structure. */
            adLength += byteCounter;

            /* Increment byte counter to point to start of the local Name string */
            byteCounter++;

            for(i = byteCounter; ((i < (adLength)) && (name[i - byteCounter] != CY_BLE_NULL_CHARCTER)); i++)
            {
                destBuffer[i] = (uint8_t)name[i - byteCounter];
            }

            /* This loop handles the case when new local name is shorter than old one.
             * In this case all remaining characters should be null characters. */
            while(adLength > i)
            {
                /* Terminate string */
                destBuffer[i] = (uint8_t)CY_BLE_NULL_CHARCTER;
                i++;
            }
        }
        advIndex++;
    }
    while(advIndex < CY_BLE_GAPP_CONF_COUNT);
}

#endif /* (CY_BLE_GAP_ROLE_PERIPHERAL) */


/******************************************************************************
 * Function Name: Cy_BLE_SetLocalName
 ***************************************************************************//**
 *  This function is used to set the local device name - a Characteristic of the
 *  GAP Service. If the characteristic length entered in the component customizer
 *  is shorter than the string specified by the "name" parameter, the local device
 *  name will be cut to the length specified in the customizer.
 *
 *  \param name: The local device name string. The name string to be written as
 *              the local device name. It represents a UTF-8 encoded User
 *              Friendly Descriptive Name for the device. The length of the local
 *              device string is entered into the component customizer and it can
 *              be set to a value from 0 to 248 bytes. If the name contained in
 *              the parameter is shorter than the length from the customizer, the
 *              end of the name is indicated by a NULL octet (0x00).
 *
 * \return
 *  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
 *  failed. The following are possible error codes.
 *
 *   Error Codes                         | Description
 *   ------------                        | -----------
 *   CY_BLE_SUCCESS                      | The function completed successfully.
 *   CY_BLE_ERROR_INVALID_PARAMETER      | On specifying NULL as input parameter.
 *
 *******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_SetLocalName(const char8 name[])
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    uint32_t i;
    char8 *ptr;
    uint8_t charLen;

    if(NULL != name)
    {
        /* Get the pointer to the Device Name characteristic  */
        ptr = (char8*)CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_PTR(cy_ble_configPtr->gaps->deviceNameCharHandle);

        /* First need to get the maximum length of the characteristic data in the GATT
         *  database to make sure there is enough place for the data. The length
         *  can't be longer than 248, so only the LSB of 16 bit of length is to
         *  be used. */
        charLen = (uint8_t)CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN(cy_ble_configPtr->gaps->deviceNameCharHandle);

        /* Copy name into characteristic */
        for(i = 0u; ((i < charLen) && (name[i] != CY_BLE_NULL_CHARCTER)); i++)
        {
            ptr[i] = name[i];
        }

        if(i < charLen)
        {
            /* Terminate string */
            ptr[i] = CY_BLE_NULL_CHARCTER;
        }

        /* Update device name in advertising and scan response structure */
        #if (CY_BLE_GAP_ROLE_PERIPHERAL)
        Cy_BLE_ChangeAdLocalName(name, 0u);
        Cy_BLE_ChangeAdLocalName(name, 1u);
        #endif /* (CY_BLE_GAP_ROLE_PERIPHERAL) */
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }

    return(apiResult);
}


/******************************************************************************
* Function Name: Cy_BLE_GetLocalName
***************************************************************************//**
*
*  This function is used to read the local device name - a Characteristic of
*  the GAP Service.
*
* \param name: The local device name string. Used to read the local name to the
*             given string array. It represents a UTF-8 encoded User Friendly
*             Descriptive Name for the device. The length of the local device
*             string is entered into the component customizer and it can be set
*             to a value from 0 to 248 bytes. If the name contained in the
*             parameter is shorter than the length from the customizer, the end
*             of the name is indicated by a NULL octet (0x00).
*
* \return
*  cy_en_ble_api_result_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes.
*
*   Error Codes                         | Description
*   ------------                        | -----------
*   CY_BLE_SUCCESS                      | The function completed successfully.
*   CY_BLE_ERROR_INVALID_PARAMETER      | On specifying NULL as input parameter.
*
*******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GetLocalName(char8 name[])
{
    cy_en_ble_api_result_t apiResult = CY_BLE_SUCCESS;
    uint8_t charLen;
    uint32_t i;
    char8 *ptr;

    if(NULL != name)
    {
        /* Get the pointer to the Device Name characteristic  */
        ptr = (char8*)CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_PTR(cy_ble_configPtr->gaps->deviceNameCharHandle);

        /* First need to get the length of the characteristic data in the GATT
         *  database to make sure there is enough place for the data. The length
         *  can't be longer than 248, so only the LSB of 16 bit of length is to
         *  be used. */
        charLen = (uint8_t)CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_LEN(cy_ble_configPtr->gaps->deviceNameCharHandle);

        /* Copy name from characteristic */
        for(i = 0u; ((i < charLen) && (CY_BLE_NULL_CHARCTER != ptr[i])); i++)
        {
            name[i] = ptr[i];
        }

        /* Terminate string */
        name[i] = CY_BLE_NULL_CHARCTER;
    }
    else
    {
        apiResult = CY_BLE_ERROR_INVALID_PARAMETER;
    }

    return(apiResult);
}

#endif /* (CY_BLE_GATT_ROLE_SERVER) */

#endif /* defined(CY_BLE_MODE_PROFILE) */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/* [] END OF FILE */
