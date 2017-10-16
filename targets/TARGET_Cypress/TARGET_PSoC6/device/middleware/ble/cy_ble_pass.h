/***************************************************************************//**
* \file cy_ble_pass.h
* \version 2.0
*
* \brief
*  This file contains the function prototypes and constants used in
*  the Phone Alert Status Profile of the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_PASS_H
#define CY_BLE_PASS_H


#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_PASS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \addtogroup group_ble_service_api_PASS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** Service Characteristics indexes */
typedef enum
{
    CY_BLE_PASS_AS,          /**< Alert Status characteristic index */
    CY_BLE_PASS_RS,          /**< Ringer Setting characteristic index */
    CY_BLE_PASS_CP,          /**< Ringer Control Point characteristic index */
    CY_BLE_PASS_CHAR_COUNT   /**< Total count of PASS characteristics */
}cy_en_ble_pass_char_index_t;

/** Service Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_PASS_CCCD,           /**< Client Characteristic Configuration descriptor index */
    CY_BLE_PASS_DESCR_COUNT     /**< Total count of PASS descriptors */
}cy_en_ble_pass_descr_index_t;

/** @} */

/* Alert Status values */
#define CY_BLE_PASS_AS_RINGER     (0x01) /* Ringer State */
#define CY_BLE_PASS_AS_VIBRATE    (0x02) /* Vibrate State */
#define CY_BLE_PASS_AS_DISPLAY    (0x04) /* Display Alert Status */

/**
 * \addtogroup group_ble_service_api_PASS_definitions
 * @{
 */

/** Ringer Setting values */
typedef enum
{
    CY_BLE_PASS_RS_SILENT,   /**< Ringer Silent */
    CY_BLE_PASS_RS_NORMAL    /**< Ringer Normal */
}cy_en_ble_pass_rs_t;

/** Ringer Control Point values */
typedef enum
{
    CY_BLE_PASS_CP_SILENT = 1,   /**< Silent Mode */
    CY_BLE_PASS_CP_MUTE,         /**< Mute Once */
    CY_BLE_PASS_CP_CANCEL        /**< Cancel Silent Mode */
}cy_en_ble_pass_cp_t;

/** Structure with Phone Alert Status Service characteristics and descriptors attribute handles */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t charHandle;                            /**< Handle of characteristic value */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_PASS_DESCR_COUNT];  /**< Handle of descriptor */
}cy_stc_ble_passs_char_t;

/** Structure with Phone Alert Status Service attribute handles */
typedef struct
{
    /** Phone Alert Status Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Phone Alert Status Service characteristics info array */
    cy_stc_ble_passs_char_t      charInfo[CY_BLE_PASS_CHAR_COUNT];
} cy_stc_ble_passs_t;

/** Phone Alert Status Client Server's Characteristic structure type */
typedef struct
{
    /** Properties for value field */
    uint8_t                      properties;

    /** Handle of Server database attribute value entry */
    cy_ble_gatt_db_attr_handle_t valueHandle;

    /** Phone Alert Status Client characteristics descriptors handles */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_PASS_DESCR_COUNT];

    /** Characteristic End Handle */
    cy_ble_gatt_db_attr_handle_t endHandle;
}cy_stc_ble_passc_char_t;

/** Structure with discovered attributes information of Phone Alert Status Service */
typedef struct
{
    cy_stc_ble_passc_char_t charInfo[CY_BLE_PASS_CHAR_COUNT];    /**< Characteristics handle and properties array */
}cy_stc_ble_passc_t;

/** Phone Alert Status Service Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;                      /**< Peer device handle */
    cy_en_ble_pass_char_index_t charIndex;                       /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t     *value;                          /**< Characteristic value */
} cy_stc_ble_pass_char_value_t;

/** Phone Alert Status Service Characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t     connHandle;                     /**< Peer device handle */
    cy_en_ble_pass_char_index_t  charIndex;                      /**< Index of service characteristic */
    cy_en_ble_pass_descr_index_t descrIndex;                     /**< Index of service characteristic descriptor */
    cy_stc_ble_gatt_value_t      *value;                         /**< Descriptor value */
} cy_stc_ble_pass_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** Phone Alert Status Profile GATT DB handles structure */
    const cy_stc_ble_passs_t *passs;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t        callbackFunc;
} cy_stc_ble_pass_config_t;

/** @} */


/***************************************
* Function Prototypes
***************************************/

/**
 * \addtogroup group_ble_service_api_PASS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_PASS_Init(cy_stc_ble_pass_config_t *config);
cy_en_ble_api_result_t Cy_BLE_PASS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_PASS_SERVER
/**
 * \addtogroup group_ble_service_api_PASS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_PASSS_SetCharacteristicValue(cy_en_ble_pass_char_index_t charIndex,
                                                           uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_PASSS_GetCharacteristicValue(cy_en_ble_pass_char_index_t charIndex,
                                                           uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_PASSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_pass_char_index_t charIndex,
                                                                cy_en_ble_pass_descr_index_t descrIndex, uint8_t attrSize,
                                                                uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_PASSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     cy_en_ble_pass_char_index_t charIndex, uint8_t attrSize,
                                                     uint8_t *attrValue);
/** @} */
#endif /* CY_BLE_PASS_SERVER */

#ifdef CY_BLE_PASS_CLIENT
/**
 * \addtogroup group_ble_service_api_PASS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_PASSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_pass_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_PASSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_pass_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_PASSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_pass_char_index_t charIndex,
                                                                cy_en_ble_pass_descr_index_t descrIndex, uint8_t attrSize,
                                                                uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_PASSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_pass_char_index_t charIndex,
                                                                cy_en_ble_pass_descr_index_t descrIndex);

/** @} */
#endif /* CY_BLE_PASS_CLIENT */


/***************************************
* Private Function Prototypes
***************************************/

/** \cond IGNORE
 * Internal macros
 */
#ifdef CY_BLE_PASS_SERVER
#define CY_BLE_PASS_IS_GMC_SUPPORTED \
    (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_passConfig->passs->charInfo[CY_BLE_PASS_GLMC].charHandle)
#endif /* CY_BLE_PASS_SERVER */
/** \endcond */


/***************************************
*  External data references
***************************************/

#ifdef CY_BLE_PASS_SERVER
extern uint8_t cy_ble_passFlag;
#endif /* CY_BLE_PASSC_SERVER */

#ifdef CY_BLE_PASS_CLIENT
/* Phone Alert Status Service Server's GATT DB handles structure */
extern cy_stc_ble_passc_t cy_ble_passc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_PASS_CLIENT */

extern cy_stc_ble_pass_config_t cy_ble_passConfig;
extern cy_stc_ble_pass_config_t *cy_ble_passConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_PASS) */
#endif /* CY_BLE_PASS_H */

/* [] END OF FILE */
