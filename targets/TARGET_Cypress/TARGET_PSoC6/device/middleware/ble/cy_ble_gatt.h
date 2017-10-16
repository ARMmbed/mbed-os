/***************************************************************************//**
* \file cy_ble_gatt.h
* \version 2.0
*
* \brief
*  Contains the prototypes and constants used in the BLE GATT profile.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_GATT_H
#define CY_BLE_GATT_H

#include "BLE_config.h"
#include "cy_ble_gap.h"


#if (CY_BLE_MODE_PROFILE)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Conditional Compilation Parameters
***************************************/

#define CY_BLE_GATT_ROLE           (CY_BLE_CONFIG_GATT_ROLE)
#define CY_BLE_GATT_SERVER         (0x01u)
#define CY_BLE_GATT_CLIENT         (0x02u)
#define CY_BLE_GATT_BOTH           (CY_BLE_GATT_SERVER | CY_BLE_GATT_CLIENT)

#define CY_BLE_GATT_ROLE_SERVER    (0u != (CY_BLE_GATT_ROLE & CY_BLE_GATT_SERVER))
#define CY_BLE_GATT_ROLE_CLIENT    (0u != (CY_BLE_GATT_ROLE & CY_BLE_GATT_CLIENT))

#if (CY_BLE_GATT_ROLE_SERVER)

#endif /* CY_BLE_GATT_ROLE_SERVER */

#define CY_BLE_GATT_DB_CCCD_COUNT              (CY_BLE_CONFIG_GATT_DB_CCCD_COUNT)

#if (CY_BLE_GATT_DB_CCCD_COUNT == 0u)
    #define CY_BLE_GATT_DB_FLASH_CCCD_COUNT    (1u)
#else
    #define CY_BLE_GATT_DB_FLASH_CCCD_COUNT    (CY_BLE_GATT_DB_CCCD_COUNT)
#endif /* CY_BLE_GATT_DB_CCCD_COUNT == 0u */

#define CY_BLE_CCCD_CRC_BYTE                   (0x1u)        /**< CCCD CRC byte */  

/**
 * \addtogroup group_ble_common_api_gatt_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** Service data received with read by group type response during discovery process */
typedef struct
{
    cy_stc_ble_gatt_attr_handle_range_t range;           /**< Handle range of the request */
    uint16_t                            uuid;            /**< 16-bit UUID */
}cy_stc_ble_disc_srvc_info_t;

/** Service data received with read by group type response during discovery process including 128 bit UUID */
typedef struct
{
    cy_stc_ble_gatt_attr_handle_range_t range;           /**< Handle range of the request */
    cy_ble_uuid_t                       uuid;            /**< 128-bit UUID */
}cy_stc_ble_disc_srvc128_info_t;

/** Service data that includes data received with read by group type response and connHandle */
typedef struct
{
    cy_stc_ble_conn_handle_t       connHandle;           /**< The connection handle */
    cy_stc_ble_disc_srvc128_info_t *srvcInfo;            /**< Service data */
    uint8_t                        uuidFormat;           /**< UUID Format - 16-bit (0x01) or 128-bit (0x02) */
}cy_stc_ble_disc_srv_info_t;

/** Included service data received with read by type response during discovery process */
typedef struct
{
    cy_stc_ble_conn_handle_t            connHandle;      /**< The connection handle */
    cy_ble_gatt_db_attr_handle_t        inclDefHandle;   /**< Included definition handle */
    cy_stc_ble_gatt_attr_handle_range_t inclHandleRange; /**< Included declaration handle range */
    cy_ble_uuid_t                       uuid;            /**< Included UUID */
    uint8_t                             uuidFormat;      /**< UUID Format - 16-bit (0x01) or 128-bit (0x02) */
}cy_stc_ble_disc_incl_info_t;

/** Characteristic data received with read by type response during discovery process */
typedef struct
{
    cy_stc_ble_conn_handle_t     connHandle;        /**< The connection handle */
    cy_ble_gatt_db_attr_handle_t charDeclHandle;    /**< Handle for characteristic declaration */
    uint8_t                      properties;        /**< Properties for value field */
    cy_ble_gatt_db_attr_handle_t valueHandle;       /**< Handle to server database attribute value entry */
    cy_ble_uuid_t                uuid;              /**< Characteristic UUID */
    uint8_t                      uuidFormat;        /**< UUID Format - 16-bit (0x01) or 128-bit (0x02) */
}cy_stc_ble_disc_char_info_t;

/** Characteristic Attribute handle + properties structure */
typedef struct
{
    /** Properties for value field */
    uint8_t                      properties;

    /** Handle of server database attribute value entry */
    cy_ble_gatt_db_attr_handle_t valueHandle;
}cy_stc_ble_srvr_char_info_t;

/** Characteristic descriptor data received with find info response during discovery process */
typedef struct
{
    cy_stc_ble_conn_handle_t     connHandle;        /**< The connection handle */
    cy_ble_gatt_db_attr_handle_t descrHandle;       /**< Descriptor handle */
    cy_ble_uuid_t                uuid;              /**< Descriptor UUID */
    uint8_t                      uuidFormat;        /**< UUID Format - 16-bit (0x01) or 128-bit (0x02) */
}cy_stc_ble_disc_descr_info_t;

/** Structure with Generic Attribute Service (GATTS) attribute handles */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t serviceHandle;          /**< Service handle */
    cy_ble_gatt_db_attr_handle_t serviceChangedHandle;   /**< Handle of the Service Changed characteristic */
    cy_ble_gatt_db_attr_handle_t cccdHandle;             /**< Client Characteristic Configuration descriptor handle */
} cy_stc_ble_gatts_t;

/** Structure for return a possible range of the current characteristic descriptor. */
typedef struct
{
    cy_stc_ble_conn_handle_t            connHandle;      /**< The connection handle */
    cy_stc_ble_gatt_attr_handle_range_t range;           /**< Handle range of the request */
    uint32_t                            srviIncIdx;      /**< A non-zero value indicates that the characteristic index
                                                          *   should be incremented
                                                          */
}
cy_stc_ble_disc_range_info_t;

/** Structure with discovered attributes information of Generic Attribute
 *  Service (GATTS) */
typedef struct
{
    cy_stc_ble_srvr_char_info_t  serviceChanged;         /**< Handle of the Service Changed characteristic */
    cy_ble_gatt_db_attr_handle_t cccdHandle;             /**< Client Characteristic Configuration descriptor handle */
} cy_stc_ble_gattc_t;

/** Structure to store bonding data */
typedef struct
{
    /** Stack internal bonding data */
    uint8_t stackFlashptr[CY_BLE_STACK_FLASH_SIZE];

    /** CCCD values */
    uint8_t attValuesCCCDFlashMemory[CY_BLE_MAX_BONDED_DEVICES + CY_BLE_CONN_COUNT][CY_BLE_GATT_DB_FLASH_CCCD_COUNT + CY_BLE_CCCD_CRC_BYTE];

    /** Number of CCCD */
    uint8_t cccdCount; /* CY_BLE_GATT_DB_CCCD_COUNT */

    /** Number of bonded devices */
    uint8_t boundedDevCount; /* CY_BLE_MAX_BONDED_DEVICES */
}cy_stc_ble_flash_storage_t;

/** BLE Configuration parameters structure */
typedef struct
{
    /** BLESS Power enum reflecting power level values supported by BLESS radio */
    cy_en_ble_bless_pwr_lvl_t txPowerLevelAdv;

    /** BLESS Power enum reflecting power level values supported by BLESS radio */
    cy_en_ble_bless_pwr_lvl_t txPowerLevelConn;

    /** IO capability */
    cy_en_ble_gap_iocap_t     securityIoCapability;

    /** Defines paring method */
    uint8_t                   securityPairingMethod;

    /** Silicon Device Address Enabled  */
    uint8_t                   siliconDeviceAddressEnabled;

    /** Index count for GATT DB  */
    uint16_t                  gattDbIndexCount;
}cy_stc_ble_params_t;

#endif  /* (CY_BLE_MODE_PROFILE) */

/** BLE Configuration structure */
typedef struct
{
#if (CY_BLE_MODE_PROFILE)
    /** Pointer to advertising information structure*/
    cy_stc_ble_gapp_disc_mode_info_t *discoveryModeInfo;

    /** Pointer to advertising configuration structure*/
    cy_stc_ble_gapp_adv_params_t     *gappAdvParams;

    /** Pointer to Discovery information collected by Client structure*/
    cy_stc_ble_gapc_disc_info_t      *discoveryInfo;

    /** Pointer to scan configuration structure*/
    cy_stc_ble_gapc_scan_params_t    *gapcScanParams;

    /** Pointer to structure to store bonding data */
    const cy_stc_ble_flash_storage_t *flashStorage;

    /** Pointer to structure with Generic Attribute Service (GATTS) attribute handles */
    const cy_stc_ble_gatts_t         *gatts;

    /** Pointer to structure with Generic Access Profile Service (GAPS) attribute handles */
    const cy_stc_ble_gaps_t          *gaps;

    /** Pointer to cy_ble_attValuesCCCD */
    uint8_t                          *attValuesCCCD;

    /** Pointer to GATT Database structure used in the GAP server */
    const cy_stc_ble_gatts_db_t      *gattDB;

    /** Pointer to Authentication Parameters Information structure */
    cy_stc_ble_gap_auth_info_t       *authInfo;

    /** Pointer to BLE Configuration Parameters structure */
    const cy_stc_ble_params_t        *params;

    /** An application layer event callback function to
    *  receive service events from the BLE Component */
    cy_ble_callback_t        callbackFunc;

    /** Bluetooth Device Address */
    cy_stc_ble_gap_bd_addr_t *deviceAddress;
#endif  /* (CY_BLE_MODE_PROFILE) */

    /** BLESS interrupt configuration */
    const cy_stc_sysint_t *blessIsrConfig;

#if (CY_BLE_STACK_MODE_HOST_UART)
    /** UART interrupt configuration for Host */
    const cy_stc_sysint_t          *uartHostIsrConfig;

    /** UART driver configuration for Host */
    const cy_stc_scb_uart_config_t *uartHostConfig;

    /** UART driver hardware instance */
    CySCB_Type                     *uartHostHw;
#endif /* CY_BLE_STACK_MODE_HOST_UART */

#if (CY_BLE_CONFIG_STACK_MODE_CONTR_UART)
    /** UART interrupt configuration */
    const cy_stc_sysint_t          *uartIsrConfig;

    /** UART driver configuration */
    const cy_stc_scb_uart_config_t *uartConfig;

    /** UART driver hardware instance */
    CySCB_Type                     *uartHw;
#endif /* (CY_BLE_CONFIG_STACK_MODE_CONTR_UART) */
}cy_stc_ble_config_t;

/** @} group_ble_common_api_gatt_definitions */


/***************************************
* Function Prototypes
***************************************/
/** \cond IGNORE */
cy_en_ble_api_result_t Cy_BLE_Init(cy_stc_ble_config_t *config);
/** \endcond */


#if (CY_BLE_MODE_PROFILE)

#if (CY_BLE_GATT_ROLE_SERVER)
cy_en_ble_api_result_t Cy_BLE_GATTS_ReInitGattDb(void);
#endif /* CY_BLE_GATT_ROLE_SERVER */

/**
 * \addtogroup group_ble_common_api_functions
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_SetLocalName(const char8 name[]);
cy_en_ble_api_result_t Cy_BLE_GetLocalName(char8 name[]);
/** @} group_ble_common_api_functions */


#if (CY_BLE_GATT_ROLE_CLIENT)
/**
 * \addtogroup group_ble_common_api_gatt_client_functions
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_GATTC_StartDiscovery(cy_stc_ble_conn_handle_t connHandle);
cy_en_ble_api_result_t Cy_BLE_GATTC_StartPartialDiscovery(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_ble_gatt_db_attr_handle_t startHandle,
                                                          cy_ble_gatt_db_attr_handle_t endHandle);
/** @} group_ble_common_api_gatt_client_functions
 */
#endif /* CY_BLE_GATT_ROLE_CLIENT */



/***************************************
* Private Function Prototypes
***************************************/

#if (CY_BLE_GATT_ROLE_SERVER)

cy_en_ble_gatt_err_code_t Cy_BLE_GATTS_WriteEventHandler(cy_stc_ble_gatts_write_cmd_req_param_t *eventParam);
cy_en_ble_gatt_err_code_t Cy_BLE_GATTS_ReadAttributeValueCCCDReqHandler(const cy_stc_ble_gatts_char_val_read_req_t *param);

/**
 * \addtogroup group_ble_common_api_gatt_server_functions
 * @{
 */
cy_en_ble_gatt_err_code_t Cy_BLE_GATTS_WriteAttributeValueCCCD(cy_stc_ble_gatts_db_attr_val_info_t *param);
cy_en_ble_gatt_err_code_t Cy_BLE_GATTS_ReadAttributeValueCCCD(cy_stc_ble_gatts_db_attr_val_info_t *param);
/** @} group_ble_common_api_gatt_server_functions */

#endif /* CY_BLE_GATT_ROLE_SERVER */

#if (CY_BLE_GATT_ROLE_CLIENT)

void Cy_BLE_GATTC_DiscoverCharacteristicsEventHandler(cy_stc_ble_disc_char_info_t *discCharInfo);
void Cy_BLE_GATTC_DiscoverCharDescriptorsEventHandler(cy_stc_ble_disc_descr_info_t *discDescrInfo);
void Cy_BLE_GATTC_GetCharRange(cy_stc_ble_disc_range_info_t *charRangeInfo);
void Cy_BLE_GATTC_IndicationEventHandler(cy_stc_ble_gattc_handle_value_ind_param_t *eventParam);

cy_en_ble_api_result_t Cy_BLE_GATTC_AddConnHandle(cy_stc_ble_conn_handle_t connHandle);
cy_en_ble_api_result_t Cy_BLE_GATTC_RemoveConnHandle(cy_stc_ble_conn_handle_t connHandle);
#endif /* CY_BLE_GATT_ROLE_CLIENT */



/***************************************
* Variables with external linkage
***************************************/
/** \cond IGNORE */
#if (CY_BLE_GATT_ROLE_SERVER)
#if (CY_BLE_GATT_DB_CCCD_COUNT != 0u)
extern uint8_t cy_ble_attValuesCccdMultiple[CY_BLE_CONN_COUNT][CY_BLE_GATT_DB_CCCD_COUNT + CY_BLE_CCCD_CRC_BYTE];
#endif /* CY_BLE_GATT_DB_CCCD_COUNT != 0u */
#endif /* (CY_BLE_GATT_ROLE_SERVER) */

#endif /* CY_BLE_MODE_PROFILE */

/* Global structure to initialize Cy_BLE_Init */
extern cy_stc_ble_config_t cy_ble_config;
extern cy_stc_ble_config_t *cy_ble_configPtr;
/** \endcond */

#if (CY_BLE_MODE_PROFILE)


/***************************************
* API Constants
***************************************/

/**
 * \addtogroup group_ble_common_api_macros_gatt_uuid_services
 * @{
 */
 
/***************************************
* Universal Unique Identifier (UUID): BLE Services
***************************************/

/* Generic Access Profile */
#define CY_BLE_UUID_GAP_SERVICE                          (0x1800u)  /**< Generic Access Service UUID */
/* Generic Attribute Profile */
#define CY_BLE_UUID_GATT_SERVICE                         (0x1801u)  /**< Generic Attribute Service UUID */

/* UUID: BLE Other Services */
#define CY_BLE_UUID_IMMEDIATE_ALERT_SERVICE              (0x1802u)  /**< Immediate Alert Service UUID */
#define CY_BLE_UUID_LINK_LOSS_SERVICE                    (0x1803u)  /**< Link Loss Service UUID */
#define CY_BLE_UUID_TX_POWER_SERVICE                     (0x1804u)  /**< Tx Power Service UUID */
#define CY_BLE_UUID_CURRENT_TIME_SERVICE                 (0x1805u)  /**< Current Time Service UUID */
#define CY_BLE_UUID_REF_TIME_UPDATE_SERVICE              (0x1806u)  /**< Reference Time Update Service UUID */
#define CY_BLE_UUID_NEXT_DST_CHANGE_SERVICE              (0x1807u)  /**< Next DST Change Service UUID */
#define CY_BLE_UUID_GLUCOSE_SERVICE                      (0x1808u)  /**< Glucose Service UUID */
#define CY_BLE_UUID_HEALTH_THERMOMETER_SERVICE           (0x1809u)  /**< Health Thermometer Service UUID */
#define CY_BLE_UUID_DEVICE_INFO_SERVICE                  (0x180Au)  /**< Device Information Service UUID */
#define CY_BLE_UUID_NWA_SERVICE                          (0x180Bu)  /**< NWA Service UUID */
#define CY_BLE_UUID_WATCH_DOG_SERVICE                    (0x180Cu)  /**< WATCH DOG Service UUID */
#define CY_BLE_UUID_HEART_RATE_SERVICE                   (0x180Du)  /**< Heart Rate Service UUID */
#define CY_BLE_UUID_PHONE_ALERT_STATUS_SERVICE           (0x180Eu)  /**< Phone Alert Status Service UUID */
#define CY_BLE_UUID_BAS_SERVICE                          (0x180Fu)  /**< Battery Service Service UUID */
#define CY_BLE_UUID_BLOOD_PRESSURE_SERVICE               (0x1810u)  /**< Blood Pressure Service UUID */
#define CY_BLE_UUID_ALERT_NOTIFICATION_SERVICE           (0x1811u)  /**< Alert Notification Service Service UUID */
#define CY_BLE_UUID_HIDS_SERVICE                         (0x1812u)  /**< Human Interface Device Service UUID */
#define CY_BLE_UUID_SCAN_PARAM_SERVICE                   (0x1813u)  /**< Scan Parameters Service UUID */
#define CY_BLE_UUID_RUNNING_SPEED_AND_CADENCE_SERVICE    (0x1814u)  /**< Running Speed and Cadence Service UUID */
#define CY_BLE_UUID_AUTOMATION_INPUT_OUTPUT_SERVICE      (0x1815u)  /**< Automation IO Service UUID */
#define CY_BLE_UUID_CYCLING_SPEED_AND_CADENCE_SERVICE    (0x1816u)  /**< Cycling Speed and Cadence Service UUID */
#define CY_BLE_UUID_CPS_SERVICE                          (0x1818u)  /**< Cycling Power Service UUID */
#define CY_BLE_UUID_LOCATION_AND_NAVIGATION_SERVICE      (0x1819u)  /**< Location and Navigation Service UUID */
#define CY_BLE_UUID_ENVIRONMENTAL_SENSING_SERVICE        (0x181Au)  /**< Environmental Sensing Service UUID */
#define CY_BLE_UUID_BODY_COMPOSITION_SERVICE             (0x181Bu)  /**< Body Composition Service UUID */
#define CY_BLE_UUID_USER_DATA_SERVICE                    (0x181Cu)  /**< User Data Service UUID */
#define CY_BLE_UUID_WEIGHT_SCALE_SERVICE                 (0x181Du)  /**< Weight Scale Service UUID */
#define CY_BLE_UUID_BOND_MANAGEMENT_SERVICE              (0x181Eu)  /**< Bond Management Service UUID */
#define CY_BLE_UUID_CGM_SERVICE                          (0x181Fu)  /**< Continuous Glucose Monitoring Service UUID */
#define CY_BLE_UUID_INTERNET_PROTOCOL_SUPPORT_SERVICE    (0x1820u)  /**< Internet Protocol Support Service UUID */
#define CY_BLE_UUID_IPS_SERVICE                          (0x1821u)  /**< Indoor Positioning Service UUID */
#define CY_BLE_UUID_PULSE_OXIMETER_SERVICE               (0x1822u)  /**< Pulse Oximeter Service UUID */
#define CY_BLE_UUID_HTTP_PROXY_SERVICE                   (0x1823u)  /**< HTTP Proxy Service UUID */
#define CY_BLE_UUID_WIRELESS_POWER_TRANSFER_SERVICE      (0xFFFEu)  /**< WIRELESS POWER TRANSFER Service UUID */
/** @} group_ble_common_api_macros_gatt_uuid_char_services */


/***************************************
* UUID: GATT Attribute Types defined by GATT Profile
***************************************/
/**
 * \addtogroup group_ble_common_api_macros_gatt_uuid_char_gatt_type
 * @{
 */
 
/** Primary Service Declaration */
#define CY_BLE_UUID_PRIMARY_SERVICE      (0x2800u)
/** Secondary Service Declaration */
#define CY_BLE_UUID_SECONDARY_SERVICE    (0x2801u)
/** Include Declaration */
#define CY_BLE_UUID_INCLUDE              (0x2802u)
/** Characteristic Declaration */
#define CY_BLE_UUID_CHARACTERISTIC       (0x2803u)

/** @} group_ble_common_api_macros_gatt_uuid_char_gatt_type */


/***************************************
* UUID: GATT Characteristic Descriptors
***************************************/
/**
 * \addtogroup group_ble_common_api_macros_gatt_uuid_char_desc
 * @{
 */
 
/** Characteristic Extended Properties */
#define CY_BLE_UUID_CHAR_EXTENDED_PROPERTIES      (0x2900u)
/** Characteristic User Description Descriptor */
#define CY_BLE_UUID_CHAR_USER_DESCRIPTION         (0x2901u)
/** Client Characteristic Configuration Descriptor */
#define CY_BLE_UUID_CHAR_CLIENT_CONFIG            (0x2902u)
/** Server Characteristic Configuration Descriptor */
#define CY_BLE_UUID_CHAR_SERVER_CONFIG            (0x2903u)
/** Characteristic Format Descriptor */
#define CY_BLE_UUID_CHAR_FORMAT                   (0x2904u)
/** Characteristic Aggregate Format Descriptor */
#define CY_BLE_UUID_CHAR_AGGREGATE_FORMAT         (0x2905u)
/** Characteristic Valid Range */
#define CY_BLE_UUID_CHAR_VALID_RANGE              (0x2906u)
/** External Report Reference */
#define CY_BLE_UUID_CHAR_EXTERNAL_REPORT_REF      (0x2907u)
/** Report Reference */
#define CY_BLE_UUID_CHAR_REPORT_REFERENCE         (0x2908u)
/** Number of Digitals */
#define CY_BLE_UUID_CHAR_NUMBER_OF_DIGITALS       (0x2909u)
/** Value Trigger Setting */
#define CY_BLE_UUID_CHAR_VALUE_TRIGGER_SETTING    (0x290Au)
/** Environmental Sensing Configuration */
#define CY_BLE_UUID_CHAR_ES_CONFIGURATION         (0x290Bu)
/** Environmental Sensing Measurement */
#define CY_BLE_UUID_CHAR_ES_MEASUREMENT           (0x290Cu)
/** Environmental Sensing Trigger Setting */
#define CY_BLE_UUID_CHAR_ES_TRIGGER_SETTING       (0x290Du)
/** Time Trigger Setting */
#define CY_BLE_UUID_CHAR_TIME_TRIGGER_SETTING     (0x290Eu)

/** @} group_ble_common_api_macros_gatt_uuid_char_desc */

/***************************************
* UUID: Commonly used GATT Characteristic Types
***************************************/
/**
 * \addtogroup group_ble_common_api_macros_gatt_uuid_char_type
 * @{
 */
 
/** Device Name Characteristic */
#define CY_BLE_UUID_CHAR_DEVICE_NAME                     (0x2A00u)
/** Appearance Characteristic */
#define CY_BLE_UUID_CHAR_APPEARANCE                      (0x2A01u)
/** Peripheral Privacy Flag Characteristic */
#define CY_BLE_UUID_CHAR_PERIPH_PRIVCY_FLAG              (0x2A02u)
/** Reconnection Address Characteristic */
#define CY_BLE_UUID_CHAR_RECONNECTION_ADDR               (0x2A03u)
/** Peripheral Preferred Connection Parameters Characteristic */
#define CY_BLE_UUID_CHAR_PRFRRD_CNXN_PARAM               (0x2A04u)
/** Service Changed Characteristic */
#define CY_BLE_UUID_CHAR_SERVICE_CHANGED                 (0x2A05u)
/** Central Address Resolution Characteristic */
#define CY_BLE_UUID_CHAR_CENTRAL_ADDRESS_RESOLUTION      (0x2AA6u)
/** Resolvable Private Address Only characteristic */
#define CY_BLE_UUID_CHAR_RESOLVABLE_PRIVATE_ADDR_ONLY    (0x2AC9u)


/***************************************
* UUID: Other Characteristic Types
***************************************/

#define CY_BLE_UUID_CHAR_ALERT_LEVEL                     (0x2A06u) /**< Alert Level GATT Characteristic UUID*/
#define CY_BLE_UUID_CHAR_TX_POWER_LEVEL                  (0x2A07u) /**< Tx Power Level GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_DATE_TIME                       (0x2A08u) /**< Date Time GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_DAY_OF_WEEK                     (0x2A09u) /**< Day of Week GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_DAY_DATE_TIME                   (0x2A0Au) /**< Day Date Time GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_EXACT_TIME_100                  (0x2A0Bu) /**< Exact Time 100 GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_EXACT_TIME_256                  (0x2A0Cu) /**< Exact Time 256 GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_DST_OFFSET                      (0x2A0Du) /**< DST Offset GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_TIME_ZONE                       (0x2A0Eu) /**< Time Zone GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_LOCAL_TIME_INFO                 (0x2A0Fu) /**< Local Time Information GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_SEC_TIME_ZONE                   (0x2A10u) /**< Sec Time Zone GATT Characteristic UUID  */
#define CY_BLE_UUID_CHAR_TIME_WITH_DST                   (0x2A11u) /**< Time with DST GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_TIME_ACCURACY                   (0x2A12u) /**< Time Accuracy GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_TIME_SOURCE                     (0x2A13u) /**< Time Source GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_REF_TIME_INFO                   (0x2A14u) /**< Reference Time Information GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_TIME_BROADCAST                  (0x2A15u) /**< Time Broadcast GATT Characteristic UUID*/
#define CY_BLE_UUID_CHAR_TIME_UPDATE_CONTROL_POINT       (0x2A16u) /**< Time Update Control Point GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_TIME_UPDATE_STATE               (0x2A17u) /**< Time Update State GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_GLUCOSE_MSRMNT                  (0x2A18u) /**< Glucose Measurement GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_BATTERY_LEVEL                   (0x2A19u) /**< Battery Level GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_TEMPERATURE_MSMNT               (0x2A1Cu) /**< Temperature Measurement GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_TEMPERATURE_TYPE                (0x2A1Du) /**< Temperature Type GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_INTERMEDIATE_TEMP               (0x2A1Eu) /**< Intermediate Temperature GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_MSMNT_INTERVAL                  (0x2A21u) /**< Measurement Interval GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_HIDS_BOOT_KEYBOARD_IN_RPT       (0x2A22u) /**< Boot Keyboard Input Report GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_SYSTEM_ID                       (0x2A23u) /**< System ID GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_MODEL_NUMBER                    (0x2A24u) /**< Model Number String GATT Characteristic UUID */
#define CY_BLE_UUID_CHAR_SERIAL_NUMBER                   (0x2A25u) /**< Serial Number String Characteristic UUID */
#define CY_BLE_UUID_CHAR_FIRMWARE_REV                    (0x2A26u) /**< Firmware Revision String Characteristic UUID */
#define CY_BLE_UUID_CHAR_HARDWARE_REV                    (0x2A27u) /**< Hardware Revision String Characteristic UUID */
#define CY_BLE_UUID_CHAR_SOFTWARE_REV                    (0x2A28u) /**< Software Revision String Characteristic UUID */
#define CY_BLE_UUID_CHAR_MANUFACTURER_NAME               (0x2A29u) /**< Manufacturer Name String Characteristic UUID */
#define CY_BLE_UUID_CHAR_REG_CERT_DATA                   (0x2A2Au) /**< IEEE 11073-20601 Regulatory Certification Data List */
#define CY_BLE_UUID_CHAR_CURRENT_TIME                    (0x2A2Bu) /**< Current Time Characteristic UUID */
#define CY_BLE_UUID_CHAR_MAGNETIC_DECLINATION            (0x2A2Cu) /**< Magnetic Declination Characteristic UUID */
#define CY_BLE_UUID_CHAR_SCAN_REFRESH                    (0x2A31u) /**< Scan Refresh Characteristic UUID */
#define CY_BLE_UUID_CHAR_HIDS_BOOT_KEYBOARD_OUT_RPT      (0x2A32u) /**< Boot Keyboard Output Report Characteristic UUID */
#define CY_BLE_UUID_CHAR_HIDS_BOOT_MOUSE_IN_RPT          (0x2A33u) /**< Boot Mouse Input Report Characteristic UUID */
#define CY_BLE_UUID_CHAR_GL_MSRMT_CTX                    (0x2A34u) /**< Glucose Measurement Context UUID */
#define CY_BLE_UUID_CHAR_BP_MSRMT                        (0x2A35u) /**< Blood Pressure Measurement Characteristic UUID */
#define CY_BLE_UUID_CHAR_INTRMDT_CUFF_PRSR               (0x2A36u) /**< Intermediate Cuff Pressure Characteristic UUID */
#define CY_BLE_UUID_CHAR_HR_MSRMT                        (0x2A37u) /**< Heart Rate Measurement Characteristic UUID */
#define CY_BLE_UUID_CHAR_BODY_SENSOR_LOC                 (0x2A38u) /**< Body Sensor Location Characteristic UUID */
#define CY_BLE_UUID_CHAR_HR_CNTRL_POINT                  (0x2A39u) /**< Heart Rate Control Point Characteristic UUID */
#define CY_BLE_UUID_CHAR_NWA                             (0x2A3Eu) /**< NWA Characteristic UUID */
#define CY_BLE_UUID_CHAR_ALERT_STATUS                    (0x2A3Fu) /**< Alert Status Characteristic UUID */
#define CY_BLE_UUID_CHAR_RINGER_CONTROL_POINT            (0x2A40u) /**< Ringer Control Point Characteristic UUID */
#define CY_BLE_UUID_CHAR_RINGER_SETTING                  (0x2A41u) /**< Ringer Setting Characteristic UUID */
#define CY_BLE_UUID_CHAR_ALERT_ID_BIT_MASK               (0x2A42u) /**< Alert Category ID Bit Mask Characteristic UUID */
#define CY_BLE_UUID_CHAR_ALERT_ID                        (0x2A43u) /**< Alert Category ID Characteristic UUID */
#define CY_BLE_UUID_CHAR_ALERT_NTF_CONTROL_POINT         (0x2A44u) /**< Alert Notification Control Point Characteristic UUID */
#define CY_BLE_UUID_CHAR_UNREAD_ALRT_STATUS              (0x2A45u) /**< Unread Alert Status Characteristic UUID */
#define CY_BLE_UUID_CHAR_NEW_ALERT                       (0x2A46u) /**< New Alert Characteristic UUID */
#define CY_BLE_UUID_CHAR_SPRTD_NEW_ALRT_CTGRY            (0x2A47u) /**< Supported New Alert Category Characteristic UUID */
#define CY_BLE_UUID_CHAR_SPRT_UNRD_ALRT_CTGRY            (0x2A48u) /**< Supported Unread Alert Category Characteristic UUID */
#define CY_BLE_UUID_CHAR_BP_FEATURE                      (0x2A49u) /**< Blood Pressure Feature Characteristic UUID */
#define CY_BLE_UUID_CHAR_HIDS_INFORMATION                (0x2A4Au) /**< HID Information Characteristic UUID */
#define CY_BLE_UUID_CHAR_HIDS_REPORT_MAP                 (0x2A4Bu) /**< Report Map Characteristic UUID */
#define CY_BLE_UUID_CHAR_HIDS_CONTROL_POINT              (0x2A4Cu) /**< HID Control Point Characteristic UUID */
#define CY_BLE_UUID_CHAR_HIDS_REPORT                     (0x2A4Du) /**< Report Characteristic UUID */
#define CY_BLE_UUID_CHAR_HIDS_PROTOCOL_MODE              (0x2A4Eu) /**< Protocol Mode Characteristic UUID */
#define CY_BLE_UUID_CHAR_SCAN_WINDOW                     (0x2A4Fu) /**< Scan Interval Window Characteristic UUID */
#define CY_BLE_UUID_CHAR_PNP_ID                          (0x2A50u) /**< PnP ID Characteristic UUID */
#define CY_BLE_UUID_CHAR_GL_FEATURE                      (0x2A51u) /**< Glucose Feature Characteristic UUID */
#define CY_BLE_UUID_CHAR_RACP                            (0x2A52u) /**< Record Access Control Point Characteristic UUID */
#define CY_BLE_UUID_CHAR_RSC_MSRMT                       (0x2A53u) /**< RSC Measurement Characteristic UUID */
#define CY_BLE_UUID_CHAR_RSC_FEATURE                     (0x2A54u) /**< RSC Feature Characteristic UUID */
#define CY_BLE_UUID_CHAR_SC_CONTROL_POINT                (0x2A55u) /**< SC Control Point Characteristic UUID */
#define CY_BLE_UUID_CHAR_DIGITAL                         (0x2A56u) /**< Digital Characteristic UUID */
#define CY_BLE_UUID_CHAR_ANALOG                          (0x2A58u) /**< Analog Characteristic UUID */
#define CY_BLE_UUID_CHAR_AGGREGATE                       (0x2A5Au) /**< Aggregate Characteristic UUID */
#define CY_BLE_UUID_CHAR_CSC_MSRMT                       (0x2A5Bu) /**< CSC Measurement Characteristic UUID */
#define CY_BLE_UUID_CHAR_CSC_FEATURE                     (0x2A5Cu) /**< CSC Feature Characteristic UUID */
#define CY_BLE_UUID_CHAR_SENSOR_LOCATION                 (0x2A5Du) /**< Sensor Location Characteristic UUID */
#define CY_BLE_UUID_CHAR_CPS_MSRMT                       (0x2A63u) /**< Cycling Power Measurement Characteristic UUID */
#define CY_BLE_UUID_CHAR_CPS_VECTOR                      (0x2A64u) /**< Cycling Power Vector Characteristic UUID */
#define CY_BLE_UUID_CHAR_CPS_FEATURE                     (0x2A65u) /**< Cycling Power Feature Characteristic UUID */
#define CY_BLE_UUID_CHAR_CPS_CP                          (0x2A66u) /**< Cycling Power Control Point Characteristic UUID */
#define CY_BLE_UUID_CHAR_LOCATION_AND_SPEED              (0x2A67u) /**< Location and Speed Characteristic UUID */
#define CY_BLE_UUID_CHAR_NAVIGATION                      (0x2A68u) /**< Navigation Characteristic UUID */
#define CY_BLE_UUID_CHAR_POSITION_QUALITY                (0x2A69u) /**< Position Quality Characteristic UUID */
#define CY_BLE_UUID_CHAR_LN_FEATURE                      (0x2A6Au) /**< LN Feature Characteristic UUID */
#define CY_BLE_UUID_CHAR_LN_CONTROL_POINT                (0x2A6Bu) /**< LN Control Point Characteristic UUID */
#define CY_BLE_UUID_CHAR_ELEVATION                       (0x2A6Cu) /**< Elevation Characteristic UUID */
#define CY_BLE_UUID_CHAR_PRESSURE                        (0x2A6Du) /**< Pressure Characteristic UUID */
#define CY_BLE_UUID_CHAR_THEMPERATURE                    (0x2A6Eu) /**< Temperature Characteristic UUID */
#define CY_BLE_UUID_CHAR_HUMIDITY                        (0x2A6Fu) /**< Humidity Characteristic UUID */
#define CY_BLE_UUID_CHAR_TRUE_WIND_SPEED                 (0x2A70u) /**< True Wind Speed Characteristic UUID */
#define CY_BLE_UUID_CHAR_TRUE_WIND_DIRECTION             (0x2A71u) /**< True Wind Direction Characteristic UUID */
#define CY_BLE_UUID_CHAR_APPARENT_WIND_SPEED             (0x2A72u) /**< Apparent Wind Speed Characteristic UUID */
#define CY_BLE_UUID_CHAR_APPARENT_WIND_DIRECTION         (0x2A73u) /**< Apparent Wind Direction Characteristic UUID */
#define CY_BLE_UUID_CHAR_GUST_FACTOR                     (0x2A74u) /**< Gust Factor Characteristic UUID */
#define CY_BLE_UUID_CHAR_POLLEN_CONCENTRATION            (0x2A75u) /**< Pollen Concentration Characteristic UUID */
#define CY_BLE_UUID_CHAR_UV_INDEX                        (0x2A76u) /**< UV Index Characteristic UUID */
#define CY_BLE_UUID_CHAR_IRRADIANCE                      (0x2A77u) /**< Irradiance Characteristic UUID */
#define CY_BLE_UUID_CHAR_RAINFALL                        (0x2A78u) /**< Rainfall Characteristic UUID */
#define CY_BLE_UUID_CHAR_WIND_CHILL                      (0x2A79u) /**< Wind Chill Characteristic UUID */
#define CY_BLE_UUID_CHAR_HEAT_INDEX                      (0x2A7Au) /**< Aerobic Heart Rate Lower Limit Characteristic UUID */
#define CY_BLE_UUID_CHAR_DEW_POINT                       (0x2A7Bu) /**< Dew Point Characteristic UUID */
#define CY_BLE_UUID_CHAR_DESCR_VALUE_CHANGED             (0x2A7Du) /**< Descriptor Value Changed Characteristic UUID */
#define CY_BLE_UUID_CHAR_AEL                             (0x2A7Eu) /**< Aerobic Heart Rate Lower Limit Characteristic UUID */
#define CY_BLE_UUID_CHAR_AEROBIC_THRESHOLD               (0x2A7Fu) /**< Aerobic Threshold Feature Characteristic UUID */
#define CY_BLE_UUID_CHAR_AGE                             (0x2A80u) /**< Age Feature Characteristic UUID */
#define CY_BLE_UUID_CHAR_ANL                             (0x2A81u) /**< Anaerobic Heart Rate Lower Limit Char. UUID */
#define CY_BLE_UUID_CHAR_ANU                             (0x2A82u) /**< Anaerobic Heart Rate Upper Limit Char. UUID */
#define CY_BLE_UUID_CHAR_ANAEROBIC_THRESHOLD             (0x2A83u) /**< Anaerobic Threshold Characteristic UUID */
#define CY_BLE_UUID_CHAR_AEU                             (0x2A84u) /**< Aerobic Heart Rate Upper Limit Char. UUID */
#define CY_BLE_UUID_CHAR_DATE_OF_BIRTH                   (0x2A85u) /**< Date of Birth Feature Characteristic UUID */
#define CY_BLE_UUID_CHAR_DATE_OF_THRESHOLD_ASSESSMENT    (0x2A86u) /**< Date of Threshold Assessment Char. UUID */
#define CY_BLE_UUID_CHAR_EMAIL_ADDRESS                   (0x2A87u) /**< Email Address Characteristic UUID */
#define CY_BLE_UUID_CHAR_FBL                             (0x2A88u) /**< Fat Burn Heart Rate Lower Limit Char. UUID */
#define CY_BLE_UUID_CHAR_FBU                             (0x2A89u) /**< Fat Burn Heart Rate Upper Limit Char. UUID */
#define CY_BLE_UUID_CHAR_FIRST_NAME                      (0x2A8Au) /**< First Name Characteristic UUID */
#define CY_BLE_UUID_CHAR_FIVE_ZONE_HEART_RATE_LIMITS     (0x2A8Bu) /**< Five Zone Heart Rate Limits Char. UUID */
#define CY_BLE_UUID_CHAR_GENDER                          (0x2A8Cu) /**< Gender  Characteristic UUID */
#define CY_BLE_UUID_CHAR_HEART_RATE_MAX                  (0x2A8Du) /**< Heart Rate Max Feature Characteristic UUID */
#define CY_BLE_UUID_CHAR_HEIGHT                          (0x2A8Eu) /**< Height Characteristic UUID */
#define CY_BLE_UUID_CHAR_HIP_CIRCUNFERENCE               (0x2A8Fu) /**< Hip Circumference Characteristic UUID */
#define CY_BLE_UUID_CHAR_LAST_NAME                       (0x2A90u) /**< Last Name Characteristic UUID */
#define CY_BLE_UUID_CHAR_MRH                             (0x2A91u) /**< Maximum Recommended Heart Rate Characteristic UUID */
#define CY_BLE_UUID_CHAR_RESTING_HEART_RATE              (0x2A92u) /**< Resting Heart Rate Characteristic UUID */
#define CY_BLE_UUID_CHAR_STP                             (0x2A93u) /**< Sport Type for Aerobic and Anaerobic Thresholds */
#define CY_BLE_UUID_CHAR_THREE_ZONE_HEART_RATE_LIMITS    (0x2A94u) /**< Three Zone Heart Rate Limits Characteristic UUID */
#define CY_BLE_UUID_CHAR_TWO_ZONE_HEART_RATE_LIMIT       (0x2A95u) /**< Two Zone Heart Rate Limit Characteristic UUID */
#define CY_BLE_UUID_CHAR_VO2_MAX                         (0x2A96u) /**< VO2 Max Characteristic UUID */
#define CY_BLE_UUID_CHAR_WAIST_CIRCUMFERENCE             (0x2A97u) /**< Waist Circumference Characteristic UUID */
#define CY_BLE_UUID_CHAR_WEIGHT                          (0x2A98u) /**< Weight Characteristic UUID */
#define CY_BLE_UUID_CHAR_DATABASE_CHANGE_INCREMENT       (0x2A99u) /**< Database Change Increment Characteristic UUID */
#define CY_BLE_UUID_CHAR_USER_INDEX                      (0x2A9Au) /**< User Index Characteristic UUID */
#define CY_BLE_UUID_CHAR_BODY_COMPOSITION_FEATURE        (0x2A9Bu) /**< Body Composition Feature Characteristic UUID */
#define CY_BLE_UUID_CHAR_BODY_COMPOSITION_MEASUREMENT    (0x2A9Cu) /**< Body Composition Measurement Characteristic UUID */
#define CY_BLE_UUID_CHAR_WEIGHT_MEASUREMENT              (0x2A9Du) /**< Weight Measurement Characteristic UUID */
#define CY_BLE_UUID_CHAR_WEIGHT_SCALE_FEATURE            (0x2A9Eu) /**< Weight Scale Feature Characteristic UUID */
#define CY_BLE_UUID_CHAR_USER_CONTROL_POINT              (0x2A9Fu) /**< User Control Point Characteristic UUID */
#define CY_BLE_UUID_CHAR_MAGNETIC_FLUX_DENSITY_2D        (0x2AA0u) /**< Magnetic Flux Density - 2D Characteristic UUID */
#define CY_BLE_UUID_CHAR_MAGNETIC_FLUX_DENSITY_3D        (0x2AA1u) /**< Magnetic Flux Density - 3D Characteristic UUID */
#define CY_BLE_UUID_CHAR_LANGUAGE                        (0x2AA2u) /**< Language Characteristic UUID */
#define CY_BLE_UUID_CHAR_BAR_PRESSURE_TREND              (0x2AA3u) /**< Barometric Pressure Trend Characteristic UUID */
/* Bond Management Characteristics defines */
#define CY_BLE_UUID_BOND_MANAGEMENT_CONTROL_POINT        (0x2AA4u) /**< Bond Management Control Point Characteristic UUID */
#define CY_BLE_UUID_BOND_MANAGEMENT_FEATURE              (0x2AA5u) /**< Bond Management Feature Characteristic UUID */
/* CGM Characteristics defines */
#define CY_BLE_UUID_CHAR_CGM_MEASUREMENT                 (0x2AA7u) /**< CGM Measurement Characteristic UUID */
#define CY_BLE_UUID_CHAR_CGM_FEATURE                     (0x2AA8u) /**< CGM Feature Characteristic UUID */
#define CY_BLE_UUID_CHAR_CGM_STATUS                      (0x2AA9u) /**< CGM Status Characteristic UUID */
#define CY_BLE_UUID_CHAR_CGM_SESSION_START_TIME          (0x2AAAu) /**< CGM Session Start Time Characteristic UUID */
#define CY_BLE_UUID_CHAR_CGM_SESSION_RUN_TIME            (0x2AABu) /**< CGM Session Run Time Characteristic UUID */
#define CY_BLE_UUID_CHAR_CGM_SOCP                        (0x2AACu) /**< CGM Specific Ops Control Point Characteristic UUID */
/* Pulse Oximeter Characteristics defines */
#define CY_BLE_UUID_CHAR_PLX_SPOT_CHK_MSRMT              (0x2A5Eu) /**< PLX Spot-Check Measurement Characteristic UUID */
#define CY_BLE_UUID_CHAR_PLX_CONTINUOUS_MSRMT            (0x2A5Fu) /**< PLX Continuous Measurement Characteristic UUID */
#define CY_BLE_UUID_CHAR_PLX_FEATURES                    (0x2A60u) /**< PLX Features Characteristic UUID */
/* CPM Characteristics Measurements */
#define CY_BLE_UUID_CHAR_CPM_MSRMT                       (0x2A63u) /**< Cycling Power Measurement Characteristic UUID */
#define CY_BLE_UUID_CHAR_CPM_VECTOR                      (0x2A64u) /**< Cycling Power Vector Characteristic UUID */
#define CY_BLE_UUID_CHAR_CPM_FEATURE                     (0x2A65u) /**< Cycling Power Feature Characteristic UUID */
#define CY_BLE_UUID_CHAR_CPM_CNTRL_PNT                   (0x2A66u) /**< Cycling Power Control Point Characteristic UUID */
/* IPS Characteristics defines */
#define CY_BLE_UUID_CHAR_INDOOR_POSITINING_CONFIG        (0x2AADu) /**< Indoor Positioning Configuration Char. UUID */
#define CY_BLE_UUID_CHAR_LATITUDE                        (0x2AAEu) /**< Latitude Characteristic UUID */
#define CY_BLE_UUID_CHAR_LONGITUDE                       (0x2AAFu) /**< Longitude Characteristic UUID */
#define CY_BLE_UUID_CHAR_LOCAL_NORTH_COORDINATE          (0x2AB0u) /**< Local North Coordinate Characteristic UUID */
#define CY_BLE_UUID_CHAR_LOCAL_EAST_COORDINATE           (0x2AB1u) /**< Local East Coordinate Characteristic UUID */
#define CY_BLE_UUID_CHAR_FLOOR_NUMBER                    (0x2AB2u) /**< Floor Number Characteristic UUID */
#define CY_BLE_UUID_CHAR_ALTITUDE                        (0x2AB3u) /**< Altitude Characteristic UUID */
#define CY_BLE_UUID_CHAR_UNCERTAINTY                     (0x2AB4u) /**< Uncertainty Characteristic UUID */
#define CY_BLE_UUID_CHAR_LOCATION_NAME                   (0x2AB5u) /**< Location Name Characteristic UUID */
/* HPS Characteristics defines */
#define CY_BLE_UUID_CHAR_URI                             (0x2AB6u) /**< URI Characteristic UUID */
#define CY_BLE_UUID_CHAR_HTTP_HEADERS                    (0x2AB7u) /**< HTTP Headers Characteristic UUID */
#define CY_BLE_UUID_CHAR_HTTP_ENTITY_BODY                (0x2AB9u) /**< HTTP Entity Body Characteristic UUID */
#define CY_BLE_UUID_CHAR_HTTP_CP                         (0x2ABAu) /**< HTTP Control Point Characteristic UUID */
#define CY_BLE_UUID_CHAR_HTTP_STATUS_CODE                (0x2AB8u) /**< HTTP Status Code Characteristic UUID */
#define CY_BLE_UUID_CHAR_HTTPS_SECURITY                  (0x2ABBu) /**< HTTPS Security Characteristic UUID */

/** @} group_ble_common_api_macros_gatt_uuid_char_type */

/**
 * \addtogroup group_ble_common_api_macros
 * @{
 */
 
/* GATT Characteristic Properties bit field */
#define CY_BLE_CHAR_PROP_BROADCAST                       (0x01u)   /**< Broadcast */
#define CY_BLE_CHAR_PROP_READ                            (0x02u)   /**< Read */
#define CY_BLE_CHAR_PROP_WRITE_WITHOUT_RSP               (0x04u)   /**< Write Without Response */
#define CY_BLE_CHAR_PROP_WRITE                           (0x08u)   /**< Write */
#define CY_BLE_CHAR_PROP_NOTIFY                          (0x10u)   /**< Notify */
#define CY_BLE_CHAR_PROP_INDICATE                        (0x20u)   /**< Indicate */
#define CY_BLE_CHAR_PROP_SIGNED_WRITE                    (0x40u)   /**< Authenticated Signed Writes */
#define CY_BLE_CHAR_PROP_EXTENDED_PROPERTIES             (0x80u)   /**< Extended properties */

 
/* Client Characteristic Configuration Descriptor values */
#define CY_BLE_CCCD_DEFAULT                              (0x0000u) /**< Notifications and Indications disabled */
#define CY_BLE_CCCD_NOTIFICATION                         (0x0001u) /**< Notifications enabled */
#define CY_BLE_CCCD_INDICATION                           (0x0002u) /**< Indications enabled */
#define CY_BLE_CCCD_LEN                                  (0x02u)   /**< Length of CCCD desctiptor */

/* Server Characteristic Configuration Descriptor values */
#define CY_BLE_SCCD_DEFAULT                              (0x0000u) /**< Broadcasts disabled */
#define CY_BLE_SCCD_BROADCAST                            (0x0001u) /**< Broadcasts enabled */
#define CY_BLE_SCCD_LEN                                  (0x02u)   /**< Length of SCCD desctiptor */

/* Characteristic Presentation Format Descriptor values */
#define CY_BLE_CPFD_BOOLEAN                              (0x01u)   /**< Unsigned 1-bit; 0=false, 1=true */
#define CY_BLE_CPFD_UINT2                                (0x02u)   /**< Unsigned 2-bit integer */
#define CY_BLE_CPFD_UINT4                                (0x03u)   /**< Unsigned 4-bit integer */
#define CY_BLE_CPFD_UINT8                                (0x04u)   /**< Unsigned 8-bit integer */
#define CY_BLE_CPFD_UINT12                               (0x05u)   /**< Unsigned 12-bit integer */
#define CY_BLE_CPFD_UINT16                               (0x06u)   /**< Unsigned 16-bit integer */
#define CY_BLE_CPFD_UINT24                               (0x07u)   /**< Unsigned 24-bit integer */
#define CY_BLE_CPFD_UINT32                               (0x08u)   /**< Unsigned 32-bit integer */
#define CY_BLE_CPFD_UINT48                               (0x09u)   /**< Unsigned 48-bit integer */
#define CY_BLE_CPFD_UINT64                               (0x0au)   /**< Unsigned 64-bit integer */
#define CY_BLE_CPFD_UINT128                              (0x0bu)   /**< Unsigned 128-bit integer */
#define CY_BLE_CPFD_INT8                                 (0x0cu)   /**< Signed 8-bit integer */
#define CY_BLE_CPFD_INT12                                (0x0du)   /**< Signed 12-bit integer */
#define CY_BLE_CPFD_INT16                                (0x0eu)   /**< Signed 16-bit integer */
#define CY_BLE_CPFD_INT24                                (0x0fu)   /**< Signed 24-bit integer */
#define CY_BLE_CPFD_INT32                                (0x10u)   /**< Signed 32-bit integer */
#define CY_BLE_CPFD_INT48                                (0x11u)   /**< Signed 48-bit integer */
#define CY_BLE_CPFD_INT64                                (0x12u)   /**< Signed 64-bit integer */
#define CY_BLE_CPFD_INT128                               (0x13u)   /**< Signed 128-bit integer */
#define CY_BLE_CPFD_IEEE754_FP32                         (0x14u)   /**< IEEE-754 32-bit floating point */
#define CY_BLE_CPFD_IEEE754_FP64                         (0x15u)   /**< IEEE-754 64-bit floating point */
#define CY_BLE_CPFD_IEEE11073_SFLOAT16                   (0x16u)   /**< IEEE-11073 16-bit SFLOAT */
#define CY_BLE_CPFD_IEEE11073_FLOAT32                    (0x17u)   /**< IEEE-11073 32-bit FLOAT */
#define CY_BLE_CPFD_IEEE20601_FORMAT                     (0x18u)   /**< IEEE-20601 format */
#define CY_BLE_CPFD_UTF8_STRING                          (0x19u)   /**< UTF-8 string */
#define CY_BLE_CPFD_UTF16_STRING                         (0x1au)   /**< UTF-16 string */
#define CY_BLE_CPFD_OPAQUE_STRUCTURE                     (0x1bu)   /**< Opaque structure */
#define CY_BLE_CPFD_LEN                                  (0x01u)   /**< Length of CPFD desctiptor */

/* Characteristic Extended Properties Descriptor values */
#define CY_BLE_CEPD_DEFAULT                              (0x0000u) /**< Writable Auxiliaries & Reliable Write disabled */
#define CY_BLE_CEPD_RELIABLE_WRITE                       (0x0001u) /**< Reliable Write enabled */
#define CY_BLE_CEPD_WRITABLE_AUXILIARIES                 (0x0002u) /**< Writable Auxiliaries enabled */
#define CY_BLE_CEPD_LEN                                  (0x02u)   /**< Length of CEPD desctiptor */

#define CY_BLE_DB_ATTR_HANDLE_LEN                        (0x02u)   /**< Length of DB ATTR HANDLE */

/* PDU data length */
#define CY_BLE_PDU_DATA_LEN_ZERO                         (0x00u)   /**< Zero */
#define CY_BLE_PDU_DATA_LEN_ONE_BYTE                     (0x01u)   /**< PDU len: 1 byte  */
#define CY_BLE_PDU_DATA_LEN_TWO_BYTES                    (0x02u)   /**< PDU len: 2 bytes */
#define CY_BLE_PDU_DATA_LEN_THREE_BYTES                  (0x03u)   /**< PDU len: 3 bytes */
#define CY_BLE_PDU_DATA_LEN_FOUR_BYTES                   (0x04u)   /**< PDU len: 4 bytes */
#define CY_BLE_PDU_DATA_LEN_FIVE_BYTES                   (0x05u)   /**< PDU len: 5 bytes */
#define CY_BLE_PDU_DATA_LEN_SIX_BYTES                    (0x06u)   /**< PDU len: 6 bytes */
#define CY_BLE_PDU_DATA_LEN_SEVEN_BYTES                  (0x07u)   /**< PDU len: 7 bytes */
#define CY_BLE_PDU_DATA_LEN_EIGHT_BYTES                  (0x08u)   /**< PDU len: 8 bytes */
#define CY_BLE_PDU_DATA_LEN_NINE_BYTES                   (0x09u)   /**< PDU len: 9 bytes */

/* Peripheral Preferred Connection Parameters Characteristic values */
#define CY_BLE_PPCPC_MIN_CONN_INTV_OFFSET                (0x00u)    /**< Offset of Minimum Connection Interval Field */
#define CY_BLE_PPCPC_MAX_CONN_INTV_OFFSET                (0x02u)    /**< Offset of Maximum Connection Interval Field */
#define CY_BLE_PPCPC_SLAVE_LATENCY_OFFSET                (0x04u)    /**< Offset of Slave Latency Field */
#define CY_BLE_PPCPC_SUP_TIMEOUT_OFFSET                  (0x06u)    /**< Offset of Connection Supervision Timeout Field */
#define CY_BLE_PPCPC_LEN                                 (0x08u)    /**< Length of PPCPC desctiptor */
/** @} group_ble_common_api_macros */

/***************************************
* Bluetooth Appearance values
***************************************/
/**
 * \addtogroup group_ble_common_api_macros_appearance_values
 * @{
 */
 
#define CY_BLE_APPEARANCE_UNKNOWN                              (0u)    /**< Unknown */
#define CY_BLE_APPEARANCE_GENERIC_PHONE                        (64u)   /**< Generic Phone */
#define CY_BLE_APPEARANCE_GENERIC_COMPUTER                     (128u)  /**< Generic Computer */
#define CY_BLE_APPEARANCE_GENERIC_WATCH                        (192u)  /**< Generic Watch */
#define CY_BLE_APPEARANCE_WATCH_SPORTS_WATCH                   (193u)  /**< Watch: Sports Watch */
#define CY_BLE_APPEARANCE_GENERIC_CLOCK                        (256u)  /**< Generic Clock */
#define CY_BLE_APPEARANCE_GENERIC_DISPLAY                      (320u)  /**< Generic Display */
#define CY_BLE_APPEARANCE_GENERIC_REMOTE_CONTROL               (384u)  /**< Generic Remote Control */
#define CY_BLE_APPEARANCE_GENERIC_EYE_GLASSES                  (448u)  /**< Generic Eye-glasses */
#define CY_BLE_APPEARANCE_GENERIC_TAG                          (512u)  /**< Generic Tag */
#define CY_BLE_APPEARANCE_GENERIC_KEYRING                      (576u)  /**< Generic Keyring */
#define CY_BLE_APPEARANCE_GENERIC_MEDIA_PLAYER                 (640u)  /**< Generic Media Player */
#define CY_BLE_APPEARANCE_GENERIC_BARCODE_SCANNER              (704u)  /**< Generic Barcode Scanner */
#define CY_BLE_APPEARANCE_GENERIC_THERMOMETER                  (768u)  /**< Generic Thermometer */
#define CY_BLE_APPEARANCE_THERMOMETER_EAR                      (769u)  /**< Thermometer: Ear */
#define CY_BLE_APPEARANCE_GENERIC_HEART_RATE_SENSOR            (832u)  /**< Generic Heart rate Sensor */
#define CY_BLE_APPEARANCE_HEART_RATE_SENSOR_HEART_RATE_BELT    (833u)  /**< Heart Rate Sensor: Heart Rate Belt */
#define CY_BLE_APPEARANCE_GENERIC_BLOOD_PRESSURE               (896u)  /**< Generic Glucose */
#define CY_BLE_APPEARANCE_BLOOD_PRESSURE_ARM                   (897u)  /**< Glucose: Arm */
#define CY_BLE_APPEARANCE_BLOOD_PRESSURE_WRIST                 (898u)  /**< Glucose: Wrist */
#define CY_BLE_APPEARANCE_GENERIC_HID                          (960u)  /**< Human Interface Device (HID) */
#define CY_BLE_APPEARANCE_HIDS_KEYBOARD                        (961u)  /**< Keyboard (HID Subtype) */
#define CY_BLE_APPEARANCE_HIDS_MOUSE                           (962u)  /**< Mouse (HID Subtype) */
#define CY_BLE_APPEARANCE_HIDS_JOYSTICK                        (963u)  /**< Joystiq (HID Subtype) */
#define CY_BLE_APPEARANCE_HIDS_GAMEPAD                         (964u)  /**< Gamepad (HID Subtype) */
#define CY_BLE_APPEARANCE_HIDS_DIGITIZERSUBTYPE                (965u)  /**< Digitizer Tablet (HID Subtype) */
#define CY_BLE_APPEARANCE_HIDS_CARD_READER                     (966u)  /**< Card Reader (HID Subtype) */
#define CY_BLE_APPEARANCE_HIDS_DIGITAL_PEN                     (967u)  /**< Digital Pen (HID Subtype) */
#define CY_BLE_APPEARANCE_HIDS_BARCODE                         (968u)  /**< Barcode Scanner (HID Subtype) */
#define CY_BLE_APPEARANCE_GENERIC_GLUCOSE_METER                (1024u) /**< Generic Glucose Meter */
#define CY_BLE_APPEARANCE_GENERIC_RUNNING_WALKING_SENSOR       (1088u) /**< Generic Running Walking Sensor */
#define CY_BLE_APPEARANCE_RUNNING_WALKING_SENSOR_IN_SHOE       (1089u) /**< Running Walking Sensor: In-Shoe */
#define CY_BLE_APPEARANCE_RUNNING_WALKING_SENSOR_ON_SHOE       (1090u) /**< Running Walking Sensor: On-Shoe */
#define CY_BLE_APPEARANCE_RUNNING_WALKING_SENSOR_ON_HIP        (1091u) /**< Running Walking Sensor: On-Hip */
#define CY_BLE_APPEARANCE_GENERIC_CYCLING                      (1152u) /**< Generic Cycling */
#define CY_BLE_APPEARANCE_CYCLING_CYCLING_COMPUTER             (1153u) /**< Cycling: Cycling Computer */
#define CY_BLE_APPEARANCE_CYCLING_SPEED_SENSOR                 (1154u) /**< Cycling: Speed Sensor */
#define CY_BLE_APPEARANCE_CYCLING_CADENCE_SENSOR               (1155u) /**< Cycling: Cadence Sensor */
#define CY_BLE_APPEARANCE_CYCLING_POWER_SENSOR                 (1156u) /**< Cycling: Power Sensor */
#define CY_BLE_APPEARANCE_CYCLING_SPEED_CADENCE_SENSOR         (1157u) /**< Cycling: Speed and Cadence Sensor */
/** @} group_ble_common_api_macros_appearance_values */


/**
 * \addtogroup group_ble_common_api_macros
 * @{
 */
 
/***************************************
* SMP Key size constants
***************************************/

#define CY_BLE_SMP_USER_PASSKEY_SIZE    (6u)          /**< User Passkey size */
#define CY_BLE_SMP_IDADDR_SIZE          (7u)          /**< Identity Address data size */
#define CY_BLE_SMP_IRK_SIZE             (16u)         /**< Identity Resolving Key size */
#define CY_BLE_SMP_CSRK_SIZE            (16u)         /**< Connection Signature Resolving key size */
#define CY_BLE_SMP_LTK_SIZE             (16u)         /**< Long Term Key Size size */
#define CY_BLE_SMP_EDIV_SIZE            (2u)          /**< Encrypted Diversifier size */
#define CY_BLE_SMP_RAND_24B_SIZE        (3u)          /**< 24 Bit Random Number size */
#define CY_BLE_SMP_RAND_64B_SIZE        (8u)          /**< 64 Bit Random Number size */
#define CY_BLE_SMP_RAND_128B_SIZE       (16u)         /**< 128 Bit Random Number size */

#define CY_BLE_NULL_CHARCTER            ((char8)(0))  /**< Null charter */

/***************************************
* Common constants for LLS and IAS services
***************************************/
#define CY_BLE_NO_ALERT      (0u)                     /**< Alert Level: No Alert */
#define CY_BLE_MILD_ALERT    (1u)                     /**< Alert Level: Mild Alert */
#define CY_BLE_HIGH_ALERT    (2u)                     /**< Alert Level: High Alert */

/** @} group_ble_common_api_macros */

/***************************************
* Database macro
***************************************/
/**
 * \addtogroup group_ble_common_api_macros_gatt_db
 * @{
 */
 
/** Get permission field of attribute handle */
#define CY_BLE_GATT_DB_GET_ATTR_PERMISSION(handle) \
    (uint32_t)(cy_ble_configPtr->gattDB[(handle) - 1u].permission)

/** Compare permission field of attribute entry to
 *  check desired permission property
 */
#define CY_BLE_GATT_DB_ATTR_CHECK_PRPTY(handle, prpty) \
    ((0ul) != (CY_BLE_GATT_DB_GET_ATTR_PERMISSION(handle) & (prpty)))

/** Macro to set generic value of attribute */
#define CY_BLE_GATT_DB_ATTR_SET_GEN_VALUE(handle, src, length)                                                    \
    (void)memcpy                                                                                                  \
    (                                                                                                             \
        (void*)(cy_ble_configPtr->gattDB[(handle) - 1u].attValue.attFormatValue.attGenericValLen->attGenericVal), \
        (const void*)(src),                                                                                       \
        (uint16_t)(length)                                                                                        \
    )


/** Macro to get generic value of attribute. This macro also used to get
 *  UUID32 or UUID 128 of the attribute.
 */
#define CY_BLE_GATT_DB_ATTR_GET_GEN_VALUE(dst, handle, length)                                                          \
    (void)memcpy                                                                                                        \
    (                                                                                                                   \
        (void*)(dst),                                                                                                   \
        (const void*)(cy_ble_configPtr->gattDB[(handle) - 1u].attValue.attFormatValue.attGenericValLen->attGenericVal), \
        (uint16_t)(length)                                                                                              \
    )

/** Macro to get start handle of attribute
 *  Start handle is only meaningful for characteristic and
 *  service declaration */
#define CY_BLE_GATT_DB_GET_START_HANDLE(handle) \
    cy_ble_configPtr->gattDB[(handle) - 1u].attHandle

/** Macro to get end handle of attribute
 *  Start handle is only meaningful for characteristic and
 *  service declaration */
#define CY_BLE_GATT_DB_GET_END_HANDLE(handle) \
    cy_ble_configPtr->gattDB[(handle) - 1u].attEndHandle


/** Macro to get attribute type (UUID) for handle */
#define CY_BLE_GATT_DB_GET_UUID(handle) \
    cy_ble_configPtr->gattDB[(handle) - 1u].attType


/** Macro to get attribute value UUID for handle. This macro is only applicable
 *  to UUID16. To get UUID32 or UUID 128 use CY_BLE_GATT_DB_ATTR_GET_GEN_VALUE().
 */
#define CY_BLE_GATT_DB_GET_ATTR_VALUE_UUID(handle) \
    cy_ble_configPtr->gattDB[(handle) - 1u].attValue.attValueUuid

/** Macro to get actual length of characteristic generic value */
#define CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_LEN(handle) \
    cy_ble_configPtr->gattDB[(handle) - 1u].attValue.attFormatValue.attGenericValLen->actualLength

/** Macro to set actual length of characteristic generic value.
 *  Note that this macro doesn't validate the actual length, which can't be greater then maximum length.
 *  Use CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN macro to read and validate maximum length.
 */
#define CY_BLE_GATT_DB_ATTR_SET_ATTR_GEN_LEN(handle, value)                                                       \
    do {                                                                                                          \
        cy_ble_configPtr->gattDB[(handle) - 1u].attValue.attFormatValue.attGenericValLen->actualLength = (value); \
    } while(0)

/** Macro to get maximum length of characteristic generic value */
#define CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_MAX_LEN(handle) \
    cy_ble_configPtr->gattDB[(handle) - 1u].attValue.attFormatValue.maxAttrLength

/** Macro to get index of CCCD value */
#define CY_BLE_GATT_DB_ATTR_GET_CCCD_IDX(handle)    (CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_PTR(handle) - \
                                                     cy_ble_configPtr->attValuesCCCD)

/** Macro to get pointer to characteristic generic value */
#define CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_PTR(handle) \
    (uint8_t*)(cy_ble_configPtr->gattDB[(handle) - 1u].attValue.attFormatValue.attGenericValLen->attGenericVal)

/** Macro to get pointer to CCCD value */
#define CY_BLE_GATT_DB_ATTR_GET_CCCD_ATTR_GEN_PTR(attId, handle) \
    (uint8_t*)(&cy_ble_attValuesCccdMultiple[attId][CY_BLE_GATT_DB_ATTR_GET_CCCD_IDX(handle)])

/** Checks Notification properties in permission field of attribute entry */
#define CY_BLE_IS_NOTIFICATION_SUPPORTED(handle) \
    CY_BLE_GATT_DB_ATTR_CHECK_PRPTY(handle, CY_BLE_GATT_DB_CH_PROP_NOTIFY)

/** Checks Notification bit in attribute value referenced by pointer */
#define CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(valPtr) \
    ((0u) != (CY_BLE_CCCD_NOTIFICATION & Cy_BLE_Get16ByPtr(valPtr)))

/** Checks Notification bit in attribute value referenced by handle */
#define CY_BLE_IS_NOTIFICATION_ENABLED(attId, handle) \
    CY_BLE_IS_NOTIFICATION_ENABLED_IN_PTR(CY_BLE_GATT_DB_ATTR_GET_CCCD_ATTR_GEN_PTR(attId, handle))

/** Checks Indication properties in permission field of attribute entry */
#define CY_BLE_IS_INDICATION_SUPPORTED(handle) \
    CY_BLE_GATT_DB_ATTR_CHECK_PRPTY(handle, CY_BLE_GATT_DB_CH_PROP_INDICATE)

/** Checks Indication bit in attribute value referenced by pointer */
#define CY_BLE_IS_INDICATION_ENABLED_IN_PTR(valPtr) \
    ((0u) != (CY_BLE_CCCD_INDICATION & Cy_BLE_Get16ByPtr(valPtr)))

/** Checks Indication bit in attribute value referenced by handle */
#define CY_BLE_IS_INDICATION_ENABLED(attId, handle) \
    CY_BLE_IS_INDICATION_ENABLED_IN_PTR(CY_BLE_GATT_DB_ATTR_GET_CCCD_ATTR_GEN_PTR(attId, handle))

/** Checks Broadcast properties in permission field of attribute entry */
#define CY_BLE_IS_BROADCAST_SUPPORTED(handle) \
    CY_BLE_GATT_DB_ATTR_CHECK_PRPTY(handle, CY_BLE_GATT_DB_CH_PROP_BROADCAST)

/** Checks Broadcast bit in attribute value referenced by pointer */
#define CY_BLE_IS_BROADCAST_ENABLED_IN_PTR(valPtr) \
    ((0u) != (CY_BLE_SCCD_BROADCAST & Cy_BLE_Get16ByPtr(valPtr)))

/** Checks Broadcast bit in attribute value referenced by handle */
#define CY_BLE_IS_BROADCAST_ENABLED(handle) \
    CY_BLE_IS_BROADCAST_ENABLED_IN_PTR(CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_PTR(handle))

/** Checks Reliable Write bit in attribute value referenced by pointer */
#define CY_BLE_IS_RELIABLE_WRITE_ENABLED_IN_PTR(valPtr) \
    ((0u) != (CY_BLE_CEPD_RELIABLE_WRITE & Cy_BLE_Get16ByPtr(valPtr)))

/** Checks Reliable Write bit in attribute value referenced by handle */
#define CY_BLE_IS_RELIABLE_WRITE_ENABLED(handle) \
    CY_BLE_IS_RELIABLE_WRITE_ENABLED_IN_PTR(CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_PTR(handle))

/** @} group_ble_common_api_macros_gatt_db */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE */
#endif /* CY_BLE_GATT_H */


/* [] END OF FILE */
