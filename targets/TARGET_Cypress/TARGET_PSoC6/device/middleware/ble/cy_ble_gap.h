/***************************************************************************//**
* \file cy_ble_gap.h
* \version 2.0
*
* \brief
*  Contains the prototypes and constants used in the BLE GAP profile.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_GAP_H
#define CY_BLE_GAP_H

#include "cy_ble.h"


/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/***************************************
* Type Definitions
***************************************/

#if (CY_BLE_MODE_PROFILE)

/***************************************
* Enumerated Types and Structures
***************************************/

/**
 * \addtogroup group_ble_common_api_gap_definitions
 * @{
 */

/** New Event handler state machine type */
typedef enum
{
    CY_BLE_STATE_STOPPED,                             /**< BLE is turned off */
    CY_BLE_STATE_INITIALIZING,                        /**< Initializing state */
    CY_BLE_STATE_ON,                                  /**< BLE is turned on */
#if (CY_BLE_GAP_ROLE_CENTRAL)
    CY_BLE_STATE_CONNECTING,                          /**< Connecting */
#endif /* CY_BLE_GAP_ROLE_CENTRAL */
} cy_en_ble_state_t;


/** Scanning state machine type */
typedef enum
{
    CY_BLE_SCAN_STATE_STOPPED,                        /**< Scanning is stopped */
    CY_BLE_SCAN_STATE_SCAN_INITIATED,                 /**< Scanning is initiated */
    CY_BLE_SCAN_STATE_SCANNING,                       /**< Scanning process */
    CY_BLE_SCAN_STATE_STOP_INITIATED                  /**< Stop scanning is initiated */
} cy_en_ble_scan_state_t;

/** Advertising state machine type */
typedef enum
{
    CY_BLE_ADV_STATE_STOPPED,                         /**< Advertising is stopped */
    CY_BLE_ADV_STATE_ADV_INITIATED,                   /**< Advertising is initiated */
    CY_BLE_ADV_STATE_ADVERTISING,                     /**< Advertising process */
    CY_BLE_ADV_STATE_STOP_INITIATED                   /**< Stop advertising is initiated */
} cy_en_ble_adv_state_t;

/** Connection state machine type */
typedef enum
{
    CY_BLE_CONN_STATE_DISCONNECTED,                   /**< Essentially idle state */
    CY_BLE_CONN_STATE_CLIENT_DISCONNECTED_DISCOVERED, /**< Server is disconnected but discovered */
    CY_BLE_CONN_STATE_CONNECTED,                      /**< Peer device is connected for this and following states */
    CY_BLE_CONN_STATE_CLIENT_SRVC_DISCOVERING,        /**< Server services are being discovered */
    CY_BLE_CONN_STATE_CLIENT_INCL_DISCOVERING,        /**< Server included services are being discovered */
    CY_BLE_CONN_STATE_CLIENT_CHAR_DISCOVERING,        /**< Server characteristics are being discovered */
    CY_BLE_CONN_STATE_CLIENT_DESCR_DISCOVERING,       /**< Server char. descriptors are being discovered */
    CY_BLE_CONN_STATE_CLIENT_DISCOVERED               /**< Server is discovered */
} cy_en_ble_conn_state_t;


/** Structure with Generic Access Profile Service (GAPS) attribute handles */
typedef struct
{
    /** Service handle*/
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Handle of the GAPS Device Name Characteristic */
    cy_ble_gatt_db_attr_handle_t deviceNameCharHandle;

    /** Handle of the GAPS Appearance Characteristic */
    cy_ble_gatt_db_attr_handle_t appearanceCharHandle;

    /** Handle of the GAPS Peripheral Preferred Connection Parameters Characteristic */
    cy_ble_gatt_db_attr_handle_t prefConnParamCharHandle;

    /** Handle of the GAPS Central Address Resolution characteristic */
    cy_ble_gatt_db_attr_handle_t centralAddrResolutionCharHandle;

    /** Handle of the GAPS Resolvable Private Address Only characteristic */
    cy_ble_gatt_db_attr_handle_t resolvablePrivateAddressOnly;
} cy_stc_ble_gaps_t;


/** GAP Service characteristics server's GATT DB handles structure type */
typedef struct
{
    /** Handle of the GAPS Device Name Characteristic */
    cy_ble_gatt_db_attr_handle_t deviceNameCharHandle;

    /** Handle of the GAPS Appearance Characteristic */
    cy_ble_gatt_db_attr_handle_t appearanceCharHandle;

    /** Handle of the GAPS Peripheral Privacy Flag Parameters Characteristic */
    cy_ble_gatt_db_attr_handle_t periphPrivacyCharHandle;

    /** Handle of the GAPS Reconnection Address Characteristic */
    cy_ble_gatt_db_attr_handle_t reconnAddrCharHandle;

    /** Handle of the GAPS Peripheral Preferred Connection Parameters Characteristic */
    cy_ble_gatt_db_attr_handle_t prefConnParamCharHandle;

    /** Handle of the GAPS Central Address Resolution characteristic */
    cy_ble_gatt_db_attr_handle_t centralAddrResolutionCharHandle;

    /** Handle of the GAPS Resolvable Private Address Only characteristic */
    cy_ble_gatt_db_attr_handle_t resolvablePrivateAddressOnly;
}cy_stc_ble_gapc_t;

/* Stack mode defines */


/** Advertisement SIG assigned numbers */
typedef enum
{
    /** Flags */
    CY_BLE_GAP_ADV_FLAGS = 0x01u,

    /** Incomplete List of 16-bit Service Class UUIDs */
    CY_BLE_GAP_ADV_INCOMPL_16UUID,

    /** Complete List of 16-bit Service Class UUIDs */
    CY_BLE_GAP_ADV_COMPL_16UUID,

    /** Incomplete List of 32-bit Service Class UUIDs */
    CY_BLE_GAP_ADV_INCOMPL_32_UUID,

    /** Complete List of 32-bit Service Class UUIDs */
    CY_BLE_GAP_ADV_COMPL_32_UUID,

    /** Incomplete List of 128-bit Service Class UUIDs */
    CY_BLE_GAP_ADV_INCOMPL_128_UUID,

    /** Complete List of 128-bit Service Class UUIDs */
    CY_BLE_GAP_ADV_COMPL_128_UUID,

    /** Shortened Local Name */
    CY_BLE_GAP_ADV_SHORT_NAME,

    /** Complete Local Name */
    CY_BLE_GAP_ADV_COMPL_NAME,

    /** Tx Power Level */
    CY_BLE_GAP_ADV_TX_PWR_LVL,

    /** Class of Device */
    CY_BLE_GAP_ADV_CLASS_OF_DEVICE = 0x0Du,

    /** Simple Pairing Hash C */
    CY_BLE_GAP_ADV_SMPL_PAIR_HASH_C,

    /** Simple Pairing Randomizer R */
    CY_BLE_GAP_ADV_SMPL_PAIR_RANDOM_R,

    /** Device ID*/
    CY_BLE_GAP_ADV_DEVICE_ID,

    /** Security Manager TK Value */
    CY_BLE_GAP_ADV_SCRT_MNGR_TK_VAL = 0x10u,

    /** Security Manager Out of Band Flags */
    CY_BLE_GAP_ADV_SCRT_MNGR_OOB_FLAGS,

    /** Slave Connection Interval Range */
    CY_BLE_GAP_ADV_SLAVE_CONN_INTRV_RANGE,

    /** List of 16-bit Service Solicitation UUIDs */
    CY_BLE_GAP_ADV_SOLICIT_16UUID = 0x14u,

    /** List of 128-bit Service Solicitation UUIDs */
    CY_BLE_GAP_ADV_SOLICIT_128UUID,

    /** Service Data - 16-bit UUID */
    CY_BLE_GAP_ADV_SRVC_DATA_16UUID,

    /** Public Target Address */
    CY_BLE_GAP_ADV_PUBLIC_TARGET_ADDR,

    /** Random Target Address */
    CY_BLE_GAP_ADV_RANDOM_TARGET_ADDR,

    /** Appearance */
    CY_BLE_GAP_ADV_APPEARANCE,

    /** Advertising Interval */
    CY_BLE_GAP_ADV_ADVERT_INTERVAL,

    /** LE Bluetooth Device Address */
    CY_BLE_GAP_ADV_LE_BT_DEVICE_ADDR,

    /** LE Role */
    CY_BLE_GAP_ADV_LE_ROLE,

    /** Simple Pairing Hash C-256 */
    CY_BLE_GAP_ADV_SMPL_PAIR_HASH_C256,

    /** Simple Pairing Randomizer R-256 */
    CY_BLE_GAP_ADV_SMPL_PAIR_RANDOM_R256,

    /** List of 32-bit Service Solicitation UUIDs */
    CY_BLE_GAP_ADV_SOLICIT_32UUID,

    /** Service Data - 32-bit UUID */
    CY_BLE_GAP_ADV_SRVC_DATA_32UUID,

    /** Service Data - 128-bit UUID */
    CY_BLE_GAP_ADV_SRVC_DATA_128UUID,

    /** 3D Information Data */
    CY_BLE_GAP_ADV_3D_INFO_DATA = 0x3D,

    /** Manufacturer Specific Data */
    CY_BLE_GAP_ADV_MANUFACTURER_SPECIFIC_DATA = 0xFF
}cy_en_ble_gap_adv_assign_numbers_t;

/** GAPP advertising configuration parameters structure */
typedef struct
{
    /** The minimum interval for fast advertising the data and establishing the LE Connection */
    uint16_t fastAdvIntervalMin;

    /** The maximum interval for fast advertising the data and establishing the LE Connection */
    uint16_t fastAdvIntervalMax;

    /** The timeout value of  fast advertising with fast advertising interval parameters */
    uint16_t fastAdvTimeOut;

    /** Slow advertising enable */
    uint8_t  slowAdvEnable;

    /** The minimum interval for slow advertising the data and establishing the LE Connection */
    uint16_t slowAdvIntervalMin;

    /** The maximum interval for slow  advertising the data and establishing the LE Connection */
    uint16_t slowAdvIntervalMax;

    /** The timeout value of slow advertising with slow advertising interval parameters */
    uint16_t slowAdvTimeOut;
}cy_stc_ble_gapp_adv_params_t;

/** GAPC advertising configuration parameters structure */
typedef struct
{
    /** The scan interval when operating in Fast connection */
    uint16_t fastScanInterval;

    /** Defines the scan window when operating in Fast connection */
    uint16_t fastScanWindow;

    /** The timeout value of scanning with fast scan parameters */
    uint16_t fastScanTimeOut;

    /** Slow scan enable/disable */
    uint8_t  slowScanEnabled;

    /** The scan interval when operating in Slow connection */
    uint16_t slowScanInterval;

    /** Defines the scan window when operating in Slow connection */
    uint16_t slowScanWindow;

    /** The timeout value of scanning with Slow scan parameters */
    uint16_t slowScanTimeOut;

    /** The minimum permissible connection time value to be used during a connection event */
    uint16_t gapcConnectionIntervalMin;

    /** The maximum permissible connection time value to be used during a connection event */
    uint16_t gapcConnectionIntervalMax;

    /** Defines the maximum time between two received Data Packet PDUs before
     *  the connection is considered lost */
    uint16_t gapcConnectionSlaveLatency;

    /** Defines the LE link supervision timeout interval */
    uint16_t gapcConnectionTimeOut;
}cy_stc_ble_gapc_scan_params_t;

/** Store BLE Application Data parameter into flash */
typedef struct
{
    /** Source buffer*/
    const uint8_t *srcBuff;

    /** Destination buffer */
    const uint8_t *destAddr;

    /** Source buffer length */
    uint32_t      buffLen;
    
    /** Write mode: blocking (default) / non-blocking */
    uint32_t      writeMode;
    
} cy_stc_ble_app_flash_param_t;


/** @} group_ble_common_api_gap_definitions */
 
/***************************************
* API Constants
***************************************/


#define CY_BLE_AD_TYPE_MORE16UUID                  (0x02u)
#define CY_BLE_AD_TYPE_CMPL16UUID                  (0x03u)
#define CY_BLE_AD_TYPE_MORE32UUID                  (0x04u)
#define CY_BLE_AD_TYPE_CMPL32UUID                  (0x05u)
#define CY_BLE_AD_TYPE_MORE128UUID                 (0x06u)
#define CY_BLE_AD_TYPE_CMPL128UUID                 (0x07u)

#define CY_BLE_AD_TYPE_SERVICE_DATA                (0x16u)
#define CY_BLE_AD_SERVICE_DATA_OVERHEAD            (0x03u)

#define CY_BLE_DISCOVERY_IDLE                      (0x00u)
#define CY_BLE_DISCOVERY_SERVICE                   (0x01u)
#define CY_BLE_DISCOVERY_CHAR                      (0x02u)
#define CY_BLE_DISCOVERY_DESCR                     (0x03u)
#define CY_BLE_DISCOVERY_DONE                      (0x04u)

/* Cy_BLE_NextCharDscrDiscovery parameters */
#define CY_BLE_DISCOVERY_INIT                      (0x00u)
#define CY_BLE_DISCOVERY_CONTINUE                  (0x01u)

#define CY_BLE_SFLASH_DIE_X_MASK                   (0x3Fu)
#define CY_BLE_SFLASH_DIE_X_BITS                   (6u)
#define CY_BLE_SFLASH_DIE_Y_MASK                   (0x3Fu)
#define CY_BLE_SFLASH_DIE_Y_BITS                   (6u)
#define CY_BLE_SFLASH_DIE_XY_BITS                  (CY_BLE_SFLASH_DIE_X_BITS + CY_BLE_SFLASH_DIE_Y_BITS)
#define CY_BLE_SFLASH_DIE_WAFER_MASK               (0x1Fu)
#define CY_BLE_SFLASH_DIE_WAFER_BITS               (5u)
#define CY_BLE_SFLASH_DIE_XYWAFER_BITS             (CY_BLE_SFLASH_DIE_XY_BITS + CY_BLE_SFLASH_DIE_WAFER_BITS)
#define CY_BLE_SFLASH_DIE_LOT_MASK                 (0x7Fu)
#define CY_BLE_SFLASH_DIE_LOT_BITS                 (7u)

/* Device address stored by user in ROW4 of the SFLASH */
#define CY_BLE_SFLASH_DEVICE_ADDRESS_PTR           ((cy_stc_ble_gap_bd_addr_t*)(SFLASH->BLE_DEVICE_ADDRESS))

#define CY_BLE_AD_STRUCTURE_MAX_LENGTH             (31u)

/* AD types for complete, shortened local name and device address */
#define CY_BLE_SHORT_LOCAL_NAME                    (0x08u)  /**< Shortened Local Name */
#define CY_BLE_COMPLETE_LOCAL_NAME                 (0x09u)  /**< Complete Local Name */
#define CY_BLE_ADV_DEVICE_ADDR                     (0x1Bu)  /**< LE Bluetooth Device Address */


#define CY_BLE_ADVERTISING_FAST                    (0x00u)
#define CY_BLE_ADVERTISING_SLOW                    (0x01u)
#define CY_BLE_ADVERTISING_CUSTOM                  (0x02u)

#define CY_BLE_SCANNING_FAST                       (0x00u)
#define CY_BLE_SCANNING_SLOW                       (0x01u)
#define CY_BLE_SCANNING_CUSTOM                     (0x02u)

#define CY_BLE_PENDING_STACK_FLASH_WRITE_BIT       (0x01u)
#define CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT        (0x02u)
#define CY_BLE_PENDING_CCCD_FLASH_CLEAR_BIT        (0x04u)
#define CY_BLE_PENDING_CCCD_FLASH_CLEAR_ALL_BIT    (0x08u)
#define CY_BLE_PENDING_CCCD_FLASH_CLEAR_MASK \
    (CY_BLE_PENDING_CCCD_FLASH_CLEAR_BIT | CY_BLE_PENDING_CCCD_FLASH_CLEAR_ALL_BIT)

    
/**
 * \addtogroup group_ble_common_api_macros
 * @{
 */
 
/* GAP Advertisement Flags */
#define CY_BLE_GAP_ADV_FLAG_LE_LIMITED_DISC_MODE        (0x01u)     /**< LE Limited Discoverable Mode. */
#define CY_BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE        (0x02u)     /**< LE General Discoverable Mode. */
#define CY_BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED        (0x04u)     /**< BR/EDR not supported. */
#define CY_BLE_GAP_ADV_FLAG_LE_BR_EDR_CONTROLLER        (0x08u)     /**< Simultaneous LE and BR/EDR, Controller. */
#define CY_BLE_GAP_ADV_FLAG_LE_BR_EDR_HOST              (0x10u)     /**< Simultaneous LE and BR/EDR, Host. */
#define CY_BLE_GAP_ADV_FLAGS_PACKET_LENGTH              (0x02u)     /**< Length of flags in an advertisement packet */

/* GAP Advertising interval min and max */
#define CY_BLE_GAP_ADV_ADVERT_INTERVAL_MIN              (0x0020u)   /**< Minimum Advertising interval in 625 us units,
                                                                     *   i.e. 20 ms. */
#define CY_BLE_GAP_ADV_ADVERT_INTERVAL_NONCON_MIN       (0x00A0u)   /**< Minimum Advertising interval in 625 us units
                                                                     *   for non connectable mode, i.e. 100 ms. */
#define CY_BLE_GAP_ADV_ADVERT_INTERVAL_MAX              (0x4000u)   /**< Maximum Advertising interval in 625 us units,
                                                                     *   i.e. 10.24 s. */
#define CY_BLE_GAP_ADV_ADVERT_INTERVAL_PACKET_LENGTH    (0x03u)     /**< Length of the Advertising Interval AD type in 
                                                                     *   an advertisement packet */
#define CY_BLE_GAPC_CONNECTING_TIMEOUT                  (30u)       /**< Seconds */
#define CY_BLE_INVALID_CONN_HANDLE_VALUE                (0xFFu)     /**< Invalid Connection Handle Value */

/* Write mode for Cy_BLE_StoreAppData */
#define CY_BLE_STORE_DATA_MODE_BLOCKING                 (0x0u)      /**< Blocking write */
#define CY_BLE_STORE_DATA_MODE_NON_BLOCKING             (0x1u)      /**< Non-blocking write */

/** @} group_ble_common_api_macros */

/***************************************
* Private Function Prototypes
***************************************/
void Cy_BLE_ServiceInit(void);
uint32_t Cy_BLE_GetDiscoveryIdx(cy_stc_ble_conn_handle_t connHandle);


/***************************************
* Function Prototypes
***************************************/
/**
 * \addtogroup group_ble_common_api_functions
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_StoreAppData(const cy_stc_ble_app_flash_param_t *param);

 
#if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))
cy_en_ble_api_result_t Cy_BLE_StoreBondingData(void);
cy_en_ble_api_result_t Cy_BLE_GAP_RemoveBondedDevice(cy_stc_ble_gap_bd_addr_t* bdAddr);
#endif /* (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */

/** @} group_ble_common_api_functions */


/**
 * \addtogroup group_ble_common_api_gap_peripheral_functions
 * @{
 */
 
#if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)
cy_en_ble_api_result_t Cy_BLE_GAPP_StartAdvertisement(uint8_t advertisingIntervalType, uint8_t advertisingParamIndex);
cy_en_ble_api_result_t Cy_BLE_GAPP_StopAdvertisement(void);

/** \cond IGNORE */
void Cy_BLE_ChangeAdDeviceAddress(const cy_stc_ble_gap_bd_addr_t* bdAddr, uint8_t dest);
/** \endcond */

#endif /* CY_BLE_GAP_ROLE_PERIPHERAL */

/** @} group_ble_common_api_gap_peripheral_functions */

/**
 * \addtogroup group_ble_common_api_gap_central_functions
 * @{
 */
 
#if (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER)
cy_en_ble_api_result_t Cy_BLE_GAPC_StartScan(uint8_t scanningIntervalType, uint8_t scanParamIndex);
cy_en_ble_api_result_t Cy_BLE_GAPC_StopScan(void);
#endif /* CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER */

#if (CY_BLE_GAP_ROLE_CENTRAL)
cy_en_ble_api_result_t Cy_BLE_GAPC_ConnectDevice(const cy_stc_ble_gap_bd_addr_t * address, uint8_t centralParamIndex);
cy_en_ble_api_result_t Cy_BLE_GAPC_CancelDeviceConnection(void);
#endif /* CY_BLE_GAP_ROLE_CENTRAL */

/** @} group_ble_common_api_gap_central_functions */


/***************************************
* External data references
***************************************/
extern volatile uint8_t cy_ble_eventHandlerFlag;
extern cy_stc_ble_gap_bd_addr_t *cy_ble_sflashDeviceAddress;
extern cy_stc_ble_gap_bd_addr_t cy_ble_deviceAddress;
extern cy_en_ble_state_t cy_ble_state;

#if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL))
extern cy_stc_ble_gap_auth_info_t cy_ble_authInfo[CY_BLE_CONFIG_AUTH_INFO_COUNT];
#endif /* ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL)) */

#if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)
extern cy_en_ble_adv_state_t cy_ble_advState;
extern uint8_t cy_ble_advIndex;
#endif  /* (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER) */

#if (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER)
extern cy_en_ble_scan_state_t cy_ble_scanState;
extern uint8_t cy_ble_scanIndex;
#endif /* (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER) */

#if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL))
extern cy_stc_ble_conn_handle_t cy_ble_connHandle[CY_BLE_CONN_COUNT];
extern cy_en_ble_conn_state_t cy_ble_connState[CY_BLE_CONN_COUNT];
#endif /* ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL)) */

#if (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER)
extern cy_stc_ble_gapc_disc_info_t cy_ble_discoveryInfo[CY_BLE_GAPC_CONF_COUNT];
#endif /* CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER */

#if (CY_BLE_GAP_ROLE_CENTRAL)
extern cy_stc_ble_gapc_conn_info_t cy_ble_connectionParameters;
extern cy_stc_ble_timer_info_t cy_ble_connectingTimeout;
#endif /* CY_BLE_GAP_ROLE_CENTRAL */

#if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)
extern cy_stc_ble_gapp_disc_mode_info_t cy_ble_discoveryModeInfo[CY_BLE_GAPP_CONF_COUNT];
extern cy_stc_ble_gapp_disc_param_t cy_ble_discoveryParam[CY_BLE_GAPP_CONF_COUNT];
extern cy_stc_ble_gapp_disc_data_t cy_ble_discoveryData[CY_BLE_GAPP_CONF_COUNT];
extern cy_stc_ble_gapp_scan_rsp_data_t cy_ble_scanRspData[CY_BLE_GAPP_CONF_COUNT];
#endif /* CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER */



#if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL) && (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES))

/** This is a two-bit variable that contains status of pending write to flash operation.
 * This variable is initialized to zero in Cy_BLE_Init().
 * CY_BLE_PENDING_CCCD_FLASH_WRITE_BIT flag is set after write to CCCD event when a
 * peer device supports bonding (cy_ble_peerBonding[eventParam->connHandle.attId] == CY_BLE_GAP_BONDING).
 * CY_BLE_PENDING_STACK_FLASH_WRITE_BIT flag is set after CY_BLE_EVT_PENDING_FLASH_WRITE event.
 * Cy_BLE_StoreBondingData() should be called to store pending bonding data.
 * This function automatically clears pending bits after write operation is complete.
 */
extern uint8_t cy_ble_pendingFlashWrite;

/** Bonding type setting of peer device, CY_BLE_GAP_BONDING_NONE or CY_BLE_GAP_BONDING.
 * It is initialized after pairing with peer device and used for cy_ble_pendingFlashWrite variable
 * setting.
 */
extern uint8_t cy_ble_peerBonding[CY_BLE_CONN_COUNT];


#endif  /* (CY_BLE_BONDING_REQUIREMENT == CY_BLE_BONDING_YES) */


/***************************************
* Exported Functions (MACROS)
***************************************/
#if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL))

/**
 * \addtogroup group_ble_common_api_functions
 * @{
*/
    
/******************************************************************************
* Function Name: Cy_BLE_GetConnHandleIdx
***************************************************************************//**
*  This function returns the index of connHandle in cy_ble_connHandle which correspond to
*  input parameter
*
*  \param connHandle: The connection handle.
*
*  \return
*   retValue - the index of connHandle
*
******************************************************************************/
__STATIC_INLINE uint8_t Cy_BLE_GetConnHandleIdx(cy_stc_ble_conn_handle_t connHandle)
{
    uint8_t idx;
    uint8_t retValue = CY_BLE_CONN_COUNT;

    for(idx = 0u; idx < CY_BLE_CONN_COUNT; idx++)
    {
        if(cy_ble_connHandle[idx].bdHandle == connHandle.bdHandle)
        {
            retValue = idx;
            break;
        }
    }
    if(retValue == CY_BLE_CONN_COUNT)
    {
        /* HALT in debug mode */
        CY_ASSERT(retValue != CY_BLE_CONN_COUNT);
        retValue = 0u;
    }

    return(retValue);
}
#endif  /* ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL)) */

/******************************************************************************
* Function Name: Cy_BLE_GetState
***************************************************************************//**
*  This function is used to determine the current state of the component
*  state machine.
*
*  The component is in the state CY_BLE_STATE_INITIALIZING after the Cy_BLE_Start()
*  is called and until the CY_BLE_EVT_STACK_ON event is not received. After
*  successful initialization the state is changed to CY_BLE_STATE_ON.
*
*  For GAP Central role when the Cy_BLE_GAPC_ConnectDevice() is called, the state
*  is changed to CY_BLE_STATE_CONNECTING. After successful connection indicated
*  by CY_BLE_EVT_GAP_DEVICE_CONNECTED or CY_BLE_EVT_GAP_ENHANCE_CONN_COMPLETE
*  event or the timeout indicated by CY_BLE_EVT_TIMEOUT event, the state is
*  changed to CY_BLE_STATE_ON.
*
* \return
*   * CY_BLE_STATE_STOPPED       - BLE is turned off
*   * CY_BLE_STATE_INITIALIZING, - Initializing state
*   * CY_BLE_STATE_ON            - BLE is turned on
*   * CY_BLE_STATE_CONNECTING    - Connecting
*
******************************************************************************/
#define Cy_BLE_GetState()    (cy_ble_state)


/******************************************************************************
* Function Name: Cy_BLE_SetState
***************************************************************************//**
*
*  Used to set the component state machine's state.
*
*  \param state: The desired state to which the event handler's
*   state machine should be set. The parameter state is a variable
*   of type cy_en_ble_state_t.
*
* \return
*  None
*
******************************************************************************/
#define Cy_BLE_SetState(state)    (cy_ble_state = (state))

#if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)


/******************************************************************************
* Function Name: Cy_BLE_GetAdvertisementState
***************************************************************************//**
*  This function returns the state of the link layer hardware advertisement
*  engine.
*
*  When Cy_BLE_GAPP_StartAdvertisement() is called, the state is set to
*  CY_BLE_ADV_STATE_ADV_INITIATED. It automatically changes to
*  CY_BLE_ADV_STATE_ADVERTISING when CY_BLE_EVT_GAPP_ADVERTISEMENT_START_STOP
*  event is received. When Cy_BLE_GAPP_StopAdvertisement() is called, the
*  state is set to CY_BLE_ADV_STATE_STOP_INITIATED. It automatically changes
*  to CY_BLE_ADV_STATE_STOPPED when the CY_BLE_EVT_GAPP_ADVERTISEMENT_START_STOP
*  event is received.
*
*  \return
*  cy_en_ble_adv_state_t - The current advertising state.
*
******************************************************************************/
__STATIC_INLINE cy_en_ble_adv_state_t Cy_BLE_GetAdvertisementState(void)
{
    return(cy_ble_advState);
}

/******************************************************************************
* Function Name: Cy_BLE_SetAdvertisementState
***************************************************************************//**
*
*  Used to set the state of the link layer hardware advertisement engine.
*
*  \param state: The desired state.
*
* \return
*  None
*
******************************************************************************/
__STATIC_INLINE void Cy_BLE_SetAdvertisementState(cy_en_ble_adv_state_t state)
{
    cy_ble_advState = state;
}

#endif /* (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER) */

#if (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER)

/******************************************************************************
* Function Name: Cy_BLE_GetScanState
***************************************************************************//**
*  This returns the state of the link layer hardware scan engine.
*
*  When Cy_BLE_GAPC_StartScan() is called the state is set to
*  CY_BLE_SCAN_STATE_SCAN_INITIATED. It automatically changes to
*  CY_BLE_ SCAN _STATE_SCANNING when the CY_BLE_EVT_GAPC_SCAN_START_STOP event is
*  received. When Cy_BLE_GAPC_StopScan() is called, the state is set to
*  CY_BLE_SCAN_STATE_STOP_INITIATED. It automatically changes to
*  CY_BLE_SCAN_STATE_STOPPED when the CY_BLE_EVT_GAPC_SCAN_START_STOP event is
*  received.
*
*  \return
*  cy_en_ble_scan_state_t - The current scan state.
*
******************************************************************************/
__STATIC_INLINE cy_en_ble_scan_state_t Cy_BLE_GetScanState(void)
{
    return(cy_ble_scanState);
}


/******************************************************************************
* Function Name: Cy_BLE_SetScanState
***************************************************************************//**
*
*  Used to set the state of the link layer hardware scan engine.
*
*  \param state: The desired state.
*
* \return
*  None
*
******************************************************************************/
__STATIC_INLINE void Cy_BLE_SetScanState(cy_en_ble_scan_state_t state)
{
    cy_ble_scanState = state;
}

#endif  /* (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER) */

 
#if ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL))

/******************************************************************************
* Function Name: Cy_BLE_GetConnectionState
***************************************************************************//**
*  This function returns the state of the BLE link for the specified connection
*  handle.
*
*  The state is set to CY_BLE_CONN_CONNECTED when CY_BLE_EVT_GATT_CONNECT_IND
*  event is received with the corresponding connection handle.
*  The state is set to CY_BLE_CONN_DISCONNECTED when the
*  CY_BLE_EVT_GATT_DISCONNECT_IND event is received.
*
*  For GATT Client role when Cy_BLE_GATTC_StartDiscovery() is called, the state
*  indicates the current flow of the discovery procedure by
*  CY_BLE_CONN_STATE_CLIENT_SRVC_DISCOVERING,
*  CY_BLE_CONN_STATE_CLIENT_INCL_DISCOVERING,
*  CY_BLE_CONN_STATE_CLIENT_CHAR_DISCOVERING,
*  CY_BLE_CONN_STATE_CLIENT_DESCR_DISCOVERING and changes to
*  CY_BLE_CONN_STATE_CLIENT_DISCOVERED when the procedure successfully completes.
*
*  The state changes from CY_BLE_CONN_STATE_CLIENT_DISCOVERED to
*  CY_BLE_CONN_STATE_CLIENT_DISCONNECTED_DISCOVERED when
*  the CY_BLE_EVT_GATT_DISCONNECT_IND event is received.
*  The state comes backs to CY_BLE_CONN_STATE_CLIENT_DISCOVERED when
*  CY_BLE_EVT_GATT_CONNECT_IND event is received and resolvable device
*  address is not changed.
*
*  \param connHandle: The connection handle.
*
*  \return
*  cy_en_ble_conn_state_t - The current client state.
*
******************************************************************************/
__STATIC_INLINE cy_en_ble_conn_state_t Cy_BLE_GetConnectionState(cy_stc_ble_conn_handle_t connHandle)
{
    return((connHandle.attId < CY_BLE_CONN_COUNT) ? cy_ble_connState[connHandle.attId] : CY_BLE_CONN_STATE_DISCONNECTED);
}


/******************************************************************************
* Function Name: Cy_BLE_SetConnectionState
***************************************************************************//**
*
*  Used to set BLE link state for the specified connection handle.
*
*  \param connHandle: Peer device handle
*  \param state:  The state that is desired to be set
*
******************************************************************************/
__STATIC_INLINE void Cy_BLE_SetConnectionState(cy_stc_ble_conn_handle_t connHandle,
                                               cy_en_ble_conn_state_t state)
{
    if(connHandle.attId < CY_BLE_CONN_COUNT)
    {
        cy_ble_connState[connHandle.attId] = state;
    }
    else
    {
        /* Halt in debug mode*/
        CY_ASSERT(connHandle.attId >= CY_BLE_CONN_COUNT);
    }
}

/******************************************************************************
* Function Name: Cy_BLE_GetNumOfActiveConn
***************************************************************************//**
*
*  Used to get active number of connections.
*
*  \return
*  connNum - number of active connections
*
******************************************************************************/
__STATIC_INLINE uint8_t Cy_BLE_GetNumOfActiveConn(void)
{
    uint32_t i;
    uint8_t connNum = 0u;

    for(i = 0u; i < CY_BLE_CONN_COUNT; i++)
    {
        if(cy_ble_connState[i] >= CY_BLE_CONN_STATE_CONNECTED)
        {
            connNum++;
        }
    }

    return(connNum);
}

/******************************************************************************
* Function Name: Cy_BLE_GetConnHandleByBdHandle
***************************************************************************//**
*
*  Used to get connection handle by bdHandle
*  This function halts in debug mode if bdHandle does not exist in the connected device
*  list
*
*  \param bdHandle: Peer device handle
*
*  \return
*   connHandle: Full connection handle.
*   connHandle.attId = CY_BLE_INVALID_CONN_HANDLE_VALUE: invalid bdHandle
*
******************************************************************************/
__STATIC_INLINE cy_stc_ble_conn_handle_t Cy_BLE_GetConnHandleByBdHandle(uint8_t bdHandle)
{
    uint32_t i;
    cy_stc_ble_conn_handle_t cHandle =
    {
        .attId    = CY_BLE_INVALID_CONN_HANDLE_VALUE,
        .bdHandle = CY_BLE_INVALID_CONN_HANDLE_VALUE
    };

    for(i = 0u; i < CY_BLE_CONN_COUNT; i++)
    {
        if((Cy_BLE_GetConnectionState(cy_ble_connHandle[i]) >= CY_BLE_CONN_STATE_CONNECTED) &&
                (cy_ble_connHandle[i].bdHandle == bdHandle))
        {
            cHandle = cy_ble_connHandle[i];
            break;
        }
    }
    if(cHandle.attId == CY_BLE_INVALID_CONN_HANDLE_VALUE)
    {
        /* HALT in debug mode */
        CY_ASSERT(cHandle.attId != CY_BLE_INVALID_CONN_HANDLE_VALUE);
    }

    return cHandle;
}

#endif /* ((CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL)) */
/** @} group_ble_common_api_functions */

#endif /* CY_BLE_MODE_PROFILE */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_GAP_H */

/* [] END OF FILE */
