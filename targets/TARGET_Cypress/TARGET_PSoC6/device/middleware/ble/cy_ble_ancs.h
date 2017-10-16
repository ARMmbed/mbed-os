/***************************************************************************//**
* \file cy_ble_ancs.h
* \version 2.0
*
* \brief
*  This file contains the function prototypes and constants used in
*  the Apple Notification Center (ANC) Service of the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CY_BLE_ANCS_H
#define CY_BLE_ANCS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_ANCS))

/* The C binding of definitions if building with the C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CY_BLE_ANCS_FLAG_PROCESS    (0x01u)

/***************************************
* Data Types
***************************************/

/**
 * \addtogroup group_ble_service_api_ANCS_definitions
 * @{
 */

/** ANC Service Characteristics indexes */
typedef enum
{
    CY_BLE_ANCS_NS,                 /**< Notification Source characteristic index */
    CY_BLE_ANCS_CP,                 /**< Control Point characteristic index */
    CY_BLE_ANCS_DS,                 /**< Data Source characteristic index */
    CY_BLE_ANCS_CHAR_COUNT          /**< Total count of ANCS characteristics */
}cy_en_ble_ancs_char_index_t;

/** ANC Service Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_ANCS_CCCD,               /**< Client Characteristic Configuration descriptor index */
    CY_BLE_ANCS_DESCR_COUNT         /**< Total count of ANCS descriptors */
}cy_en_ble_ancs_descr_index_t;

/** ANC Service Characteristic structure type */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t charHandle;                           /**< Handle of characteristic value */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_ANCS_DESCR_COUNT]; /**< Handle of descriptor */
}cy_stc_ble_ancss_char_t;

/** Structure with ANC Service attribute handles */
typedef struct
{
    /** ANC Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** ANC Service characteristics info array */
    cy_stc_ble_ancss_char_t      charInfo[CY_BLE_ANCS_CHAR_COUNT];
} cy_stc_ble_ancss_t;

/** ANCS client characteristic structure type */
typedef struct
{
    /** Properties for value field */
    uint8_t                      properties;

    /** Handle of server database attribute value entry */
    cy_ble_gatt_db_attr_handle_t valueHandle;

    /** ANCS client char. descriptor handle */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_ANCS_DESCR_COUNT];

    /** Characteristic End Handle */
    cy_ble_gatt_db_attr_handle_t endHandle;
}cy_stc_ble_ancsc_char_t;

/** Structure with discovered attributes information of ANC Service */
typedef struct
{
    cy_stc_ble_ancsc_char_t charInfo[CY_BLE_ANCS_CHAR_COUNT];   /**< Characteristics handle + properties array */
}cy_stc_ble_ancsc_t;

/** ANCS Characteristic Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;                     /**< Peer device handle */
    cy_en_ble_ancs_char_index_t charIndex;                      /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t     *value;                         /**< Characteristic value */
    cy_en_ble_gatt_err_code_t   gattErrorCode;                  /**< GATT error code for access control */
} cy_stc_ble_ancs_char_value_t;

/** ANCS Characteristic Descriptor Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t     connHandle;                    /**< Peer device handle */
    cy_en_ble_ancs_char_index_t  charIndex;                     /**< Index of service characteristic */
    cy_en_ble_ancs_descr_index_t descrIndex;                    /**< Index of service characteristic descriptor */
    cy_stc_ble_gatt_value_t      *value;                        /**< Descriptor value */
} cy_stc_ble_ancs_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** Service GATT DB handles structure */
    const cy_stc_ble_ancss_t *ancss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t        callbackFunc;
} cy_stc_ble_ancs_config_t;

/** @} */


/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_ANCS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_ANCS_Init(cy_stc_ble_ancs_config_t *config);
cy_en_ble_api_result_t Cy_BLE_ANCS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_ANCS_SERVER
/**
 * \addtogroup group_ble_service_api_ANCS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_ANCSS_SetCharacteristicValue(cy_en_ble_ancs_char_index_t charIndex,
                                                           uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ANCSS_GetCharacteristicValue(cy_en_ble_ancs_char_index_t charIndex,
                                                           uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ANCSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_ancs_char_index_t charIndex,
                                                                cy_en_ble_ancs_descr_index_t descrIndex, uint8_t attrSize,
                                                                uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ANCSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     cy_en_ble_ancs_char_index_t charIndex, uint8_t attrSize,
                                                     uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_ANCS_SERVER */

#ifdef CY_BLE_ANCS_CLIENT
/**
 * \addtogroup group_ble_service_api_ANCS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_ANCSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_ancs_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ANCSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_ancs_char_index_t charIndex,
                                                                cy_en_ble_ancs_descr_index_t descrIndex, uint8_t attrSize,
                                                                uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ANCSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_ancs_char_index_t charIndex,
                                                                cy_en_ble_ancs_descr_index_t descrIndex);

/** @} */
#endif /* CY_BLE_ANCS_CLIENT */

/***************************************
*  External data references
***************************************/

#if (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GATT_ROLE_CLIENT)
extern const cy_stc_ble_uuid128_t cy_ble_ancscServUuid;
extern const cy_stc_ble_uuid128_t cy_ble_ancscCharUuid[CY_BLE_ANCS_CHAR_COUNT];
#endif /* CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GATT_ROLE_CLIENT */

#ifdef CY_BLE_ANCS_CLIENT
/* ANC Service Server's GATT DB handles structure */
extern cy_stc_ble_ancsc_t cy_ble_ancsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_ANCS_CLIENT */

extern cy_stc_ble_ancs_config_t cy_ble_ancsConfig;
extern cy_stc_ble_ancs_config_t *cy_ble_ancsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_ANCS) */
#endif /* CY_BLE_ANCS_H */

/* [] END OF FILE */
