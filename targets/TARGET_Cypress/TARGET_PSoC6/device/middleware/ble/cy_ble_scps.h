/***************************************************************************//**
* \file cy_ble_scps.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for the Scan Parameter service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_SCPS_H
#define CY_BLE_SCPS_H


#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_SCPS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \addtogroup group_ble_service_api_SCPS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** ScPS Characteristic indexes */
typedef enum
{
    CY_BLE_SCPS_SCAN_INT_WIN,                        /**< Scan Interval Window characteristic index */
    CY_BLE_SCPS_SCAN_REFRESH,                        /**< Scan Refresh characteristic index */
    CY_BLE_SCPS_CHAR_COUNT                           /**< Total count of characteristics */
}cy_en_ble_scps_char_index_t;

/** ScPS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_SCPS_SCAN_REFRESH_CCCD,                   /**< Client Characteristic Configuration descriptor index */
    CY_BLE_SCPS_DESCR_COUNT                          /**< Total count of descriptors */
}cy_en_ble_scps_descr_index_t;

/** Structure with Scan Parameters Service attribute handles */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t serviceHandle;            /**< Scan Parameter Service handle*/
    cy_ble_gatt_db_attr_handle_t intervalWindowCharHandle; /**< Handle of Scan Interval Window Characteristic */
    cy_ble_gatt_db_attr_handle_t refreshCharHandle;        /**< Handle of Scan Refresh Characteristic */
    cy_ble_gatt_db_attr_handle_t refreshCccdHandle;        /**< Handle of Client Characteristic Configuration Descriptor */
} cy_stc_ble_scpss_t;

/** Structure with discovered attributes information of Scan Parameters Service */
typedef struct
{
    cy_stc_ble_conn_handle_t     connHandle;             /**< Peer device handle */
    cy_stc_ble_srvr_char_info_t  intervalWindowChar;     /**< Handle + properties of Scan Interval Window Characteristic */
    cy_stc_ble_srvr_char_info_t  refreshChar;            /**< Handle + properties of Scan Refresh Characteristic */
    cy_ble_gatt_db_attr_handle_t refreshCccdHandle;      /**< Handle of Client Characteristic Configuration Descriptor */
} cy_stc_ble_scpsc_t;

/** Scan Parameters Service Characteristic Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;               /**< Peer device handle */
    cy_en_ble_scps_char_index_t charIndex;                /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t     *value;                   /**< Characteristic value */
} cy_stc_ble_scps_char_value_t;

/** Scan Parameters Service Characteristic Descriptor Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t     connHandle;               /**< Peer device handle */
    cy_en_ble_scps_char_index_t  charIndex;                /**< Index of service characteristic */
    cy_en_ble_scps_descr_index_t descrIndex;               /**< Index of service characteristic descriptor */
    cy_stc_ble_gatt_value_t      *value;                   /**< Descriptor value */
} cy_stc_ble_scps_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** Scan Parameter service GATT DB handles structure */
    const cy_stc_ble_scpss_t *scpss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t        callbackFunc;
} cy_stc_ble_scps_config_t;

/** @} */

/***************************************
* API Constants
***************************************/

#define CY_BLE_SCAN_REFRESH_ENABLED        (0x00u)
#define CY_BLE_SCAN_REFRESH_RESERVED       (0xFFu)

#define CY_BLE_REFRESH_CHAR_LEN            (0x01u)
#define CY_BLE_INTERVAL_WINDOW_CHAR_LEN    (0x04u)

#define CY_BLE_SCAN_INTERVAL_WINDOW_MIN    (0x0004u)
#define CY_BLE_SCAN_INTERVAL_WINDOW_MAX    (0x4000u)


/***************************************
* Macro Functions
***************************************/

#ifdef CY_BLE_SCPS_CLIENT

#define Cy_BLE_SCPSC_GetCharacteristicValueHandle(connHandle, charIndex)        \
    (((charIndex) >= CY_BLE_SCPS_CHAR_COUNT) ?                                  \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                                    \
     ((charIndex) == CY_BLE_SCPS_SCAN_INT_WIN) ?                                \
     cy_ble_scpsc[Cy_BLE_GetDiscoveryIdx(connHandle)].refreshChar.valueHandle : \
     cy_ble_scpsc[Cy_BLE_GetDiscoveryIdx(connHandle)].intervalWindowChar.valueHandle)

#define Cy_BLE_SCPSC_GetCharacteristicDescriptorHandle(connHandle, charIndex, descrIndex)  \
    ((((charIndex) != CY_BLE_SCPS_SCAN_REFRESH) ||                                         \
      ((descrIndex) >= CY_BLE_SCPS_DESCR_COUNT)) ? CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE : \
     cy_ble_scpsc[Cy_BLE_GetDiscoveryIdx(connHandle)].refreshCccdHandle)

#endif /* (CY_BLE_SCPS_CLIENT) */


/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_SCPS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_SCPS_Init(cy_stc_ble_scps_config_t *config);
cy_en_ble_api_result_t Cy_BLE_SCPS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_SCPS_SERVER
/**
 * \addtogroup group_ble_service_api_SCPS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_SCPSS_SetCharacteristicValue(cy_en_ble_scps_char_index_t charIndex,
                                                           uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_SCPSS_GetCharacteristicValue(cy_en_ble_scps_char_index_t charIndex,
                                                           uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_SCPSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_scps_char_index_t charIndex,
                                                                cy_en_ble_scps_descr_index_t descrIndex,
                                                                uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_SCPSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     cy_en_ble_scps_char_index_t charIndex, uint8_t attrSize,
                                                     uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_SCPS_SERVER */

#ifdef CY_BLE_SCPS_CLIENT
/**
 * \addtogroup group_ble_service_api_SCPS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_SCPSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_scps_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t * attrValue);

cy_en_ble_api_result_t Cy_BLE_SCPSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_scps_char_index_t charIndex,
                                                                cy_en_ble_scps_descr_index_t descrIndex,
                                                                uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_SCPSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_scps_char_index_t charIndex,
                                                                cy_en_ble_scps_descr_index_t descrIndex);

/** @} */
#endif /* (CY_BLE_SCPS_CLIENT) */


/***************************************
* External data references
***************************************/
#ifdef CY_BLE_SCPS_CLIENT
extern cy_stc_ble_scpsc_t cy_ble_scpsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_SCPS_SERVER */

extern cy_stc_ble_scps_config_t cy_ble_scpsConfig;
extern cy_stc_ble_scps_config_t *cy_ble_scpsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_SCPS) */
#endif /* CY_BLE_SCPH_H */

/* [] END OF FILE */
