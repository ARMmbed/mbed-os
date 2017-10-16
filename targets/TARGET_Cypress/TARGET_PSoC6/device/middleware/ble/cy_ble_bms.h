/***************************************************************************//**
* \file cy_ble_bms.h
* \version 2.0
*
* \brief
*  This file contains the function prototypes and constants used in the
*  Bond Management Service of the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_BMS_H
#define CY_BLE_BMS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_BMS))

/* The C binding of definitions if building with the C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \addtogroup group_ble_service_api_BMS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** Service Characteristics indexes */
typedef enum
{
    CY_BLE_BMS_BMCP,            /**< Bond Management Control Point characteristic index */
    CY_BLE_BMS_BMFT,            /**< Bond Management Feature characteristic index */
    CY_BLE_BMS_CHAR_COUNT       /**< Total count of BMS characteristics */
}cy_en_ble_bms_char_index_t;

/** Service Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_BMS_CEPD,            /**< Characteristic Extended Properties descriptor index */
    CY_BLE_BMS_DESCR_COUNT      /**< Total count of BMS descriptors */
}cy_en_ble_bms_descr_index_t;

/** Characteristic with descriptors type */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t charHandle;                             /**< Handle of Characteristic value */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_BMS_DESCR_COUNT];    /**< Handles of Descriptors */
} cy_stc_ble_bmss_char_t;

/** Structure with Service attribute handles */
typedef struct
{
    /** Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Service characteristics info array */
    cy_stc_ble_bmss_char_t       charInfo[CY_BLE_BMS_CHAR_COUNT];
} cy_stc_ble_bmss_t;

/** Client Characteristic structure type */
typedef struct
{
    /** Properties for the value field */
    uint8_t                      properties;

    /** Handle of Server database attribute value entry */
    cy_ble_gatt_db_attr_handle_t valueHandle;

    /** Characteristics descriptors handles */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_BMS_DESCR_COUNT];

    /** Characteristic End Handle */
    cy_ble_gatt_db_attr_handle_t endHandle;
}cy_stc_ble_bmsc_char_t;

/** Service structure type */
typedef struct
{
    /** Characteristics handle + properties array */
    cy_stc_ble_bmsc_char_t charInfo[CY_BLE_BMS_CHAR_COUNT];
}cy_stc_ble_bmsc_t;

/** Service Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;         /**< Peer device handle */
    cy_en_ble_bms_char_index_t charIndex;          /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t    *value;             /**< Characteristic value */
    cy_en_ble_gatt_err_code_t  gattErrorCode;      /**< GATT error code for checking the authorization code */
} cy_stc_ble_bms_char_value_t;

/** Service Characteristic Descriptor Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;        /**< Peer device handle */
    cy_en_ble_bms_char_index_t  charIndex;         /**< Index of service characteristic */
    cy_en_ble_bms_descr_index_t descrIndex;        /**< Index of service characteristic descriptor */
    cy_stc_ble_gatt_value_t     *value;            /**< Descriptor value */
} cy_stc_ble_bms_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** Bond Management Service GATT DB handles structure */
    const cy_stc_ble_bmss_t *bmss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_bms_config_t;

/** @} */


/***************************************
* API Constants
***************************************/

/** Bond Management Control Point characteristic supports Op Codes */
#define CY_BLE_BMS_BMCP_OPC_RD    (0x03u) /* Delete bond of requesting device (LE transport only). */
#define CY_BLE_BMS_BMCP_OPC_AB    (0x06u) /* Delete all bonds on server (LE transport only). */
#define CY_BLE_BMS_BMCP_OPC_BA    (0x09u) /* Delete all but the active bond on server (LE transport only). */

/** Bond Management Control Point characteristic supports Op Codes */
#define CY_BLE_BMS_BMFT_RD        (0x00000010u) /** Delete bond  of current connection (LE transport only) supported. */
#define CY_BLE_BMS_BMFT_RC        (0x00000020u) /** Authorization Code required for feature above. */
#define CY_BLE_BMS_BMFT_AB        (0x00000400u) /** Remove all bonds on server (LE transport only) supported. */
#define CY_BLE_BMS_BMFT_AC        (0x00000800u) /** Authorization Code required for feature above. */
#define CY_BLE_BMS_BMFT_BA        (0x00010000u) /** Remove all but the active bond on server (LE transport only) supported. */
#define CY_BLE_BMS_BMFT_BC        (0x00020000u) /** Authorization Code required for feature above. */

#define CY_BLE_BMS_FLAG_LW        (0x01u)       /** Long Write Procedure */


/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_BMS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_BMS_Init(cy_stc_ble_bms_config_t *config);
cy_en_ble_api_result_t Cy_BLE_BMS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_BMS_SERVER
/**
 * \addtogroup group_ble_service_api_BMS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_BMSS_SetCharacteristicValue(cy_en_ble_bms_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BMSS_GetCharacteristicValue(cy_en_ble_bms_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BMSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bms_char_index_t charIndex,
                                                               cy_en_ble_bms_descr_index_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BMSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bms_char_index_t charIndex,
                                                               cy_en_ble_bms_descr_index_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);
/** @} */
#endif /* CY_BLE_BMS_SERVER */

#ifdef CY_BLE_BMS_CLIENT
/**
 * \addtogroup group_ble_service_api_BMS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_BMSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_bms_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_BMSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_bms_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BMSC_ReliableWriteCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                                    cy_en_ble_bms_char_index_t charIndex,
                                                                    uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BMSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bms_char_index_t charIndex,
                                                               cy_en_ble_bms_descr_index_t
                                                               descrIndex);

/** @} */
#endif /* CY_BLE_BMS_CLIENT */


/***************************************
*  External data references
***************************************/
#ifdef CY_BLE_BMS_CLIENT
/* CGM Service server's GATT DB handles structure */
extern cy_stc_ble_bmsc_t cy_ble_bmsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_BMS_CLIENT */

extern cy_stc_ble_bms_config_t cy_ble_bmsConfig;
extern cy_stc_ble_bms_config_t *cy_ble_bmsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_BMS) */
#endif /* CY_BLE_BMS_H */

/* [] END OF FILE */
