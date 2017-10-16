/***************************************************************************//**
* \file cy_ble_hids.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for HID service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_HIDS_H
#define CY_BLE_HIDS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_HIDS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Conditional Compilation Parameters
***************************************/



/* Maximum supported count of HID services and reports */
#define CY_BLE_HIDSS_SERVICE_COUNT    (CY_BLE_CONFIG_HIDSS_SERVICE_COUNT)
#define CY_BLE_HIDSS_REPORT_COUNT     (CY_BLE_CONFIG_HIDSS_REPORT_COUNT)

#ifdef CY_BLE_HIDS_CLIENT

/* Maximum supported count of HID services and reports */
#define CY_BLE_HIDSC_SERVICE_COUNT    (CY_BLE_CONFIG_HIDSC_SERVICE_COUNT)
#define CY_BLE_HIDSC_REPORT_COUNT     (CY_BLE_CONFIG_HIDSC_REPORT_COUNT)
#define CY_BLE_HIDSC_HOST_ROLE        (CY_BLE_CONFIG_HIDSC_HOST_ROLE)

#endif /* CY_BLE_HIDS_CLIENT */

/* Maximum 3 boot reports exist */
#define CY_BLE_HIDS_BOOT_REPORT_COUNT    (0x03u)

/* Define maximum report count over both roles */
#ifdef CY_BLE_HIDS_SERVER
    #ifdef CY_BLE_HIDS_CLIENT /* Both roles */
        #if (CY_BLE_HIDSC_REPORT_COUNT > CY_BLE_HIDSS_REPORT_COUNT)
            #define CY_BLE_HIDS_REPORT_COUNT    CY_BLE_HIDSC_REPORT_COUNT
        #else
            #define CY_BLE_HIDS_REPORT_COUNT    CY_BLE_HIDSS_REPORT_COUNT
        #endif /* CY_BLE_HIDSC_REPORT_COUNT > CY_BLE_HIDSS_REPORT_COUNT */
    #else /* Only server role */
        #define CY_BLE_HIDS_REPORT_COUNT        CY_BLE_HIDSS_REPORT_COUNT
    #endif /* CY_BLE_HIDS_CLIENT */
#else   /* Only client role*/
    #define CY_BLE_HIDS_REPORT_COUNT            CY_BLE_HIDSC_REPORT_COUNT
#endif /* CY_BLE_HIDS_SERVER */

/**
 * \addtogroup group_ble_service_api_HIDS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** HIDS characteristic indexes */
typedef enum
{
    CY_BLE_HIDS_PROTOCOL_MODE      = 0u,                   /**< Protocol Mode Characteristic index */
    CY_BLE_HIDS_INFORMATION        = 1u,                   /**< HID Information Characteristic index */
    CY_BLE_HIDS_CONTROL_POINT      = 2u,                   /**< HID Control Point Characteristic index */
    CY_BLE_HIDS_REPORT_MAP         = 3u,                   /**< Report Map Characteristic index */
    CY_BLE_HIDS_BOOT_KYBRD_IN_REP  = 4u,                   /**< Boot Keyboard Input Report Characteristic index */
    CY_BLE_HIDS_BOOT_KYBRD_OUT_REP = 5u,                   /**< Boot Keyboard Output Report Characteristic index */
    CY_BLE_HIDS_BOOT_MOUSE_IN_REP  = 6u,                   /**< Boot Mouse Input Report Characteristic index */
    CY_BLE_HIDS_REPORT             = 7u,                   /**< Report Characteristic index */
    CY_BLE_HIDS_REPORT_END =                               /**< Index of last Report Char */
             CY_BLE_HIDS_REPORT + CY_BLE_HIDS_REPORT_COUNT - 1u,
     CY_BLE_HIDS_CHAR_COUNT = CY_BLE_HIDS_REPORT_END + 1u   /**< Total count of characteristics */        
                                
}cy_en_ble_hids_char_index_t;

/** HID Service Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_HIDS_REPORT_CCCD,                         /**< Client Characteristic Configuration descriptor index */
    CY_BLE_HIDS_REPORT_RRD,                          /**< Report Reference descriptor index */
    CY_BLE_HIDS_REPORT_MAP_ERRD,                     /**< Report Map External Report Reference descriptor index */
    CY_BLE_HIDS_DESCR_COUNT                          /**< Total count of descriptors */
}cy_en_ble_hids_descr_t;

/** HID server Report Reference descriptor value - Report ID and Report Type */
typedef struct
{
    /** Non-zero value if there are more than one instance of the same Report Type */
    uint8_t reportId;

    /** Type of Report characteristic  */
    uint8_t reportType;
} cy_stc_ble_hidss_report_ref_t;

/** HID Information characteristic value */
typedef struct
{
    /** Version number of HIDSe USB HID Specification implemented by HID Device */
    uint16_t bcdHID;

    /** Identifies for which country hardware is localized for */
    uint8_t  bCountryCode;

    /** Bit 0: RemoteWake - Indicates whether HID Device is capable of
     *     sending wake-signal to HID Host.
     * Bit 1: NormallyConnectable - Indicates whether HID Device will be
     *     advertising when bonded but not connected.
     */
    uint8_t flags;
} cy_stc_ble_hidss_information_t;

/** HID Server Report characteristic */
typedef struct
{
    /** Handle of Report characteristic value */
    cy_ble_gatt_db_attr_handle_t reportHandle;

    /** Handle of Client Characteristic Configuration descriptor */
    cy_ble_gatt_db_attr_handle_t cccdHandle;

    /** Handle of Report Reference descriptor */
    cy_ble_gatt_db_attr_handle_t rrdHandle;
} cy_stc_ble_hidss_report_t;

/** Structure with HID Service attribute handles */
typedef struct
{
    /** Handle of HID service */
    cy_ble_gatt_db_attr_handle_t    serviceHandle;

    /** Handle of Protocol Mode Characteristic */
    cy_ble_gatt_db_attr_handle_t    protocolModeHandle;

    /** Number of report Characteristics */
    uint8_t                         reportCount;

    /** Info about report Characteristics */
    const cy_stc_ble_hidss_report_t *reportArray;

    /** Info about Boot Report Characteristics */
    cy_stc_ble_hidss_report_t       bootReportArray[CY_BLE_HIDS_BOOT_REPORT_COUNT];

    /** Handle of Report Map Characteristic */
    cy_ble_gatt_db_attr_handle_t    reportMapHandle;

    /** Handle of Report Map External Report Reference descr. */
    cy_ble_gatt_db_attr_handle_t    reportMapErrdHandle;

    /** Handle of HID Information Characteristic */
    cy_ble_gatt_db_attr_handle_t    informationHandle;

    /** Handle of HID Control Point Characteristic */
    cy_ble_gatt_db_attr_handle_t    controlPointHandle;
} cy_stc_ble_hidss_t;


#ifdef CY_BLE_HIDS_CLIENT

/** HID Client Report characteristic */
typedef struct
{
    /** Handle of Client Characteristic Configuration Descriptor */
    cy_ble_gatt_db_attr_handle_t cccdHandle;

    /** Handle of Report Reference Descriptor */
    cy_ble_gatt_db_attr_handle_t rrdHandle;

    /** Handle of Report Characteristic value */
    cy_ble_gatt_db_attr_handle_t valueHandle;

    /** End handle of Characteristic */
    cy_ble_gatt_db_attr_handle_t endHandle;

    /** Properties for value field */
    uint8_t                      properties;
} cy_stc_ble_hidsc_report_t;

/** HID client Report map characteristic */
typedef struct
{
    /** Handle of Report Map External Report Reference descriptor */
    cy_ble_gatt_db_attr_handle_t errdHandle;

    /** Handle of Report characteristic value */
    cy_ble_gatt_db_attr_handle_t valueHandle;

    /** End handle of characteristic */
    cy_ble_gatt_db_attr_handle_t endHandle;

    /** Properties for value field */
    uint8_t                      properties;
} cy_stc_ble_hidsc_report_map_t;

/** Structure with discovered attributes information of HID Service */
typedef struct
{
    /** Peer device handle */
    cy_stc_ble_conn_handle_t      connHandle;

    /** Protocol Mode Characteristic handle and properties */
    cy_stc_ble_srvr_char_info_t   protocolMode;

    /** Boot Report Characteristic info */
    cy_stc_ble_hidsc_report_t     bootReport[CY_BLE_HIDS_BOOT_REPORT_COUNT];

    /** Report Map Characteristic handle and descriptors */
    cy_stc_ble_hidsc_report_map_t reportMap;

    /** Information Characteristic handle and properties */
    cy_stc_ble_srvr_char_info_t   information;

    /** Control Point Characteristic handle and properties */
    cy_stc_ble_srvr_char_info_t   controlPoint;

    /** Report Characteristic info */
    cy_stc_ble_hidsc_report_t     report[CY_BLE_HIDSC_REPORT_COUNT];

    /** Number of report Characteristics */
    uint8_t                       reportCount;

    /** Included declaration handle */
    cy_ble_gatt_db_attr_handle_t  includeHandle;
} cy_stc_ble_hidsc_t;

/** Characteristic Value Write Sub-Procedure supported by HID Service */
typedef enum
{
    CY_BLE_HIDSC_WRITE_WITHOUT_RESPONSE,              /**< Write Without Response */
    CY_BLE_HIDSC_WRITE_CHAR_VALUE                     /**< Write Characteristic Value */
}cy_en_ble_hidsc_char_write_t;

/** Characteristic Value Read Sub-Procedure supported by HID Service */
typedef enum
{
    CY_BLE_HIDSC_READ_CHAR_VALUE,                    /**< Read Characteristic Value */
    CY_BLE_HIDSC_READ_LONG_CHAR_VALUE                /**< Read Long Characteristic Values */
}cy_en_ble_hidsc_char_read_t;

#endif /* (CY_BLE_HIDS_CLIENT) */

/** HID Service Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;          /**< Peer device handle */
    uint8_t                     serviceIndex;        /**< Index of HID Service */
    cy_en_ble_hids_char_index_t charIndex;           /**< Index of HID Service Characteristic */
    cy_stc_ble_gatt_value_t     *value;              /**< Pointer to Characteristic value */
} cy_stc_ble_hids_char_value_t;

/** HID Service Characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;         /**< Peer device handle */
    uint8_t                     serviceIndex;       /**< Index of HID Service */
    cy_en_ble_hids_char_index_t charIndex;          /**< Index of HID Service Characteristic */
    cy_en_ble_hids_descr_t      descrIndex;         /**< Service Characteristic Descriptor index */
    cy_stc_ble_gatt_value_t     *value;             /**< Pointer to value of Service Characteristic Descriptor value */
} cy_stc_ble_hids_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** HID Service GATT DB handles structure */
    const cy_stc_ble_hidss_t *hidss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t        callbackFunc;
} cy_stc_ble_hids_config_t;

/** @} */


/***************************************
* API Constants
***************************************/

/* Boot Report indexes */
#define CY_BLE_HIDS_BOOT_KYBRD_IN_REP_INDX                 (0x00u)
#define CY_BLE_HIDS_BOOT_KYBRD_OUT_REP_INDX                (0x01u)
#define CY_BLE_HIDS_BOOT_MOUSE_IN_REP_INDX                 (0x02u)

/* Report Type values */
#define CY_BLE_HIDS_PROTOCOL_MODE_BOOT                     (0x00u)
#define CY_BLE_HIDS_PROTOCOL_MODE_REPORT                   (0x01u)
#define CY_BLE_HIDS_PROTOCOL_DEFAULT                       CY_BLE_HIDS_PROTOCOL_MODE_REPORT

/* Report Type values */
#define CY_BLE_HIDS_REPORT_TYPE_INPUT                      (0x01u)
#define CY_BLE_HIDS_REPORT_TYPE_OUTPUT                     (0x02u)
#define CY_BLE_HIDS_REPORT_TYPE_FEATURE                    (0x03u)

/* Control-point attributes */
#define CY_BLE_HIDS_CP_SUSPEND                             (0x00u)
#define CY_BLE_HIDS_CP_EXIT_SUSPEND                        (0x01u)

/* Information characteristic value flags */
#define CY_BLE_HIDS_INFO_FLAG_REMOTE_WAKE_MASK             (0x01u)
#define CY_BLE_HIDS_INFO_FLAG_NORMALLY_CONNECTABLE_MASK    (0x02u)

#define CY_BLE_HIDS_PM_CHAR_LEN                            (0x01u)
#define CY_BLE_HIDS_CP_CHAR_LEN                            (0x01u)
#define CY_BLE_HIDS_RRD_LEN                                (0x02u)

#define CY_BLE_HIDSC_HOST_ROLE_BOOT                        (0x00u)
#define CY_BLE_HIDSC_HOST_ROLE_REPORT                      (0x01u)
#define CY_BLE_HIDSC_HOST_ROLE_BOOT_AND_REPORT             (0x02u)


/***************************************
* Function Prototypes
***************************************/
/**
 * \addtogroup group_ble_service_api_HIDS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_HIDS_Init(cy_stc_ble_hids_config_t *config);
cy_en_ble_api_result_t Cy_BLE_HIDS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_HIDS_SERVER
/**
 * \addtogroup group_ble_service_api_HIDS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_HIDSS_SetCharacteristicValue(uint8_t serviceIndex, cy_en_ble_hids_char_index_t charIndex,
                                                           uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HIDSS_GetCharacteristicValue(uint8_t serviceIndex, cy_en_ble_hids_char_index_t charIndex,
                                                           uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HIDSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle, uint8_t serviceIndex,
                                                                cy_en_ble_hids_char_index_t charIndex,
                                                                cy_en_ble_hids_descr_t descrIndex, uint8_t attrSize,
                                                                uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HIDSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     uint8_t serviceIndex, cy_en_ble_hids_char_index_t charIndex,
                                                     uint8_t attrSize, uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_HIDS_SERVER */

#ifdef CY_BLE_HIDS_CLIENT
/**
 * \addtogroup group_ble_service_api_HIDS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_HIDSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_hidsc_char_write_t subProcedure,
                                                           uint8_t serviceIndex, cy_en_ble_hids_char_index_t charIndex,
                                                           uint8_t attrSize, uint8_t * attrValue);

cy_en_ble_api_result_t Cy_BLE_HIDSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_hidsc_char_read_t subProcedure,
                                                           uint8_t serviceIndex, cy_en_ble_hids_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_HIDSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle, uint8_t serviceIndex,
                                                                cy_en_ble_hids_char_index_t charIndex,
                                                                cy_en_ble_hids_descr_t descrIndex, uint8_t attrSize,
                                                                uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HIDSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle, uint8_t serviceIndex,
                                                                cy_en_ble_hids_char_index_t charIndex,
                                                                cy_en_ble_hids_descr_t descrIndex);

/** @} */
#endif /* CY_BLE_HIDS_CLIENT */

/***************************************
* Private Function Prototypes
***************************************/

/** \cond IGNORE */

#ifdef CY_BLE_HIDS_CLIENT
cy_ble_gatt_db_attr_handle_t Cy_BLE_HIDSC_GetCharacteristicValueHandle(cy_stc_ble_conn_handle_t connHandle,
                                                                       uint8_t serviceIndex, cy_en_ble_hids_char_index_t
                                                                       charIndex);

cy_ble_gatt_db_attr_handle_t Cy_BLE_HIDSC_GetCharacteristicDescriptorHandle(cy_stc_ble_conn_handle_t connHandle,
                                                                            uint8_t serviceIndex, cy_en_ble_hids_char_index_t
                                                                            charIndex, cy_en_ble_hids_descr_t descrIndex);
#endif /* CY_BLE_HIDS_CLIENT */
/** \endcond */


/***************************************
* Macro Functions
***************************************/


#ifdef CY_BLE_HIDS_SERVER

/******************************************************************************
* Function Name: Cy_BLE_HIDS_GetProtocolMode
***************************************************************************//**
*
*  This function returns the current Protocol Mode of the service, as defined by
*  the service index.
*
*  \param serviceIndex: The sequence number of the HID Service
*
* \return
*  uint8_t protocolMode:
*   * CY_BLE_HIDS_PROTOCOL_MODE_BOOT      0   Boot protocol mode
*   * CY_BLE_HIDS_PROTOCOL_MODE_REPORT    1   Report Protocol mode
*
******************************************************************************/
#define Cy_BLE_HIDSS_GetProtocolMode(serviceIndex) \
    ((uint8_t)*CY_BLE_GATT_DB_ATTR_GET_ATTR_GEN_PTR(cy_ble_hidsConfigPtr->hidss[(serviceIndex)].protocolModeHandle))

#endif /* (CY_BLE_HIDS_SERVER) */


/***************************************
* External data references
***************************************/
#ifdef CY_BLE_HIDS_CLIENT
extern cy_stc_ble_hidsc_t cy_ble_hidsc[CY_BLE_CONFIG_GATTC_COUNT][CY_BLE_HIDSC_SERVICE_COUNT];
extern uint8_t cy_ble_hidscServiceCount[CY_BLE_CONFIG_GATTC_COUNT];
extern uint8_t cy_ble_hidscDisServiceIndex[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_HIDS_CLIENT */

extern cy_stc_ble_hids_config_t cy_ble_hidsConfig;
extern cy_stc_ble_hids_config_t *cy_ble_hidsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_HIDS) */
#endif /* CY_BLE_HIDS_H */


/* [] END OF FILE */
