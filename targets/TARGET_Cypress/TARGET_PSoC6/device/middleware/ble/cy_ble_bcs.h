/***************************************************************************//**
* \file cy_ble_bcs.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for the Body Composition Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
 * \addtogroup group_ble_service_api_BCS_definitions
 * @{
 */


#ifndef CY_BLE_BCS_H
#define CY_BLE_BCS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_BCS))

/* The C binding of definitions if building with the C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Conditional Compilation Parameters
***************************************/


/***************************************
* Data Types
***************************************/

/** BCS Characteristic indexes */
typedef enum
{
    CY_BLE_BCS_BODY_COMPOSITION_FEATURE,         /**< Body Composition Feature Characteristic index */
    CY_BLE_BCS_BODY_COMPOSITION_MEASUREMENT,     /**< Body Composition Measurement Characteristic index */
    CY_BLE_BCS_CHAR_COUNT                        /**< Total count of BCS Characteristics */
}cy_en_ble_bcs_char_index_t;

/** BCS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_BCS_CCCD,                             /**< Client Characteristic Configuration Descriptor index */
    CY_BLE_BCS_DESCR_COUNT                       /**< Total count of Descriptors */
}cy_en_ble_bcs_descr_index_t;

/** BCS Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;       /**< Peer device handle */
    cy_en_ble_bcs_char_index_t charIndex;        /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t    *value;           /**< Characteristic value */
} cy_stc_ble_bcs_char_value_t;

/** BCS Characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;      /**< Peer device handle */
    cy_en_ble_bcs_char_index_t  charIndex;       /**< Index of service characteristic */
    cy_en_ble_bcs_descr_index_t descrIndex;      /**< Index of descriptor */
    cy_stc_ble_gatt_value_t     *value;          /**< Characteristic value */
} cy_stc_ble_bcs_descr_value_t;

/** Structure with Body Composition Service attribute handles */
typedef struct
{
    /** Handle of Characteristic Value */
    cy_ble_gatt_db_attr_handle_t charHandle;
    /** Array of Descriptor handles */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_BCS_DESCR_COUNT];
} cy_stc_ble_bcss_char_t;

/** BCS Characteristic with descriptors handles */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t serviceHandle;                   /**< Body Composition Service handle */
    cy_stc_ble_bcss_char_t       charInfo[CY_BLE_BCS_CHAR_COUNT]; /**< Array of characteristics and descriptors handles */
} cy_stc_ble_bcss_t;

/** BCS Client Characteristic structure type */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t valueHandle;                /**< Handle of characteristic value */
    uint8_t                      properties;                 /**< Properties for value field */
    cy_ble_gatt_db_attr_handle_t endHandle;                  /**< End handle of a characteristic */
} cy_stc_ble_bcsc_char_t;

/** BCS Characteristic with descriptors */
typedef struct
{
    /** Body Composition Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Body Composition Service characteristics info structure */
    cy_stc_ble_bcsc_char_t       charInfo[CY_BLE_BCS_CHAR_COUNT];

    /** Body Composition Measurement Client Characteristic Configuration handle */
    cy_ble_gatt_db_attr_handle_t bodyCompositionMeasurementCccdHandle;
} cy_stc_ble_bcsc_t;

/** Service configuration structure */
typedef struct
{
    /** Body Composition Service GATT DB handles structure */
    const cy_stc_ble_bcss_t *bcss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_bcs_config_t;

/***************************************
* API Constants
***************************************/

/** @} */


/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_BCS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_BCS_Init(cy_stc_ble_bcs_config_t *config);
cy_en_ble_api_result_t Cy_BLE_BCS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_BCS_SERVER
/**
 * \addtogroup group_ble_service_api_BCS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_BCSS_SetCharacteristicValue(cy_en_ble_bcs_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BCSS_GetCharacteristicValue(cy_en_ble_bcs_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BCSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bcs_char_index_t charIndex,
                                                               cy_en_ble_bcs_descr_index_t descrIndex, uint8_t attrSize,
                                                               uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BCSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bcs_char_index_t charIndex,
                                                               cy_en_ble_bcs_descr_index_t descrIndex, uint8_t attrSize,
                                                               uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BCSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_bcs_char_index_t charIndex, uint8_t attrSize,
                                                  uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_BCS_SERVER */

#ifdef CY_BLE_BCS_CLIENT
/**
 * \addtogroup group_ble_service_api_BCS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_BCSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_bcs_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_BCSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bcs_char_index_t charIndex,
                                                               cy_en_ble_bcs_descr_index_t
                                                               descrIndex, uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BCSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bcs_char_index_t charIndex,
                                                               cy_en_ble_bcs_descr_index_t
                                                               descrIndex);

/** @} */
#endif /* CY_BLE_BCS_CLIENT */


/***************************************
* External data references
***************************************/
#ifdef CY_BLE_BCS_CLIENT
extern cy_stc_ble_bcsc_t cy_ble_bcsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_BCS_CLIENT */

extern cy_stc_ble_bcs_config_t cy_ble_bcsConfig;
extern cy_stc_ble_bcs_config_t *cy_ble_bcsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_BCS) */
#endif /* CY_BLE_ANCS_H */

/* [] END OF FILE */
