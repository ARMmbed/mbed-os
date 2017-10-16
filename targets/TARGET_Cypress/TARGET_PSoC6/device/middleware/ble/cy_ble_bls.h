/***************************************************************************//**
* \file cy_ble_bls.h
* \version 2.0
*
* \brief
*  This file contains the function prototypes and constants used in
*  the Blood Pressure Profile of the BLE Component.
*
********************************************************************************
* \copyright
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
 * \addtogroup group_ble_service_api_BLS_definitions
 * @{
 */


#ifndef CY_BLE_BLS_H
#define CY_BLE_BLS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_BLS))

/* The C binding of definitions if building with the C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Data Types
***************************************/

/** Service Characteristics indexes */
typedef enum
{
    CY_BLE_BLS_BPM,              /**< Blood Pressure Measurement characteristic index */
    CY_BLE_BLS_ICP,              /**< Intermediate Cuff Pressure Context characteristic index */
    CY_BLE_BLS_BPF,              /**< Blood Pressure Feature characteristic index */
    CY_BLE_BLS_CHAR_COUNT        /**< Total count of BLS characteristics */
}cy_en_ble_bls_char_index_t;

/** Service Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_BLS_CCCD,             /**< Client Characteristic Configuration descriptor index */
    CY_BLE_BLS_DESCR_COUNT       /**< Total count of BLS descriptors */
}cy_en_ble_bls_descr_index_t;

/** Characteristic with descriptors */
typedef struct
{
    /** Blood Pressure Service characteristic's handle */
    cy_ble_gatt_db_attr_handle_t charHandle;

    /** Blood Pressure Service characteristic descriptor's handle */
    cy_ble_gatt_db_attr_handle_t cccdHandle;
}cy_stc_ble_blss_char_t;

/** Structure with Blood Pressure Service attribute handles */
typedef struct
{
    /** Blood Pressure Service handle*/
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Array of Blood Pressure Service characteristics + descriptors handles */
    cy_stc_ble_blss_char_t       charInfo[CY_BLE_BLS_CHAR_COUNT];
} cy_stc_ble_blss_t;

/** Blood Pressure Client Server's characteristic structure type */
typedef struct
{
    /** Properties for the value field */
    uint8_t                      properties;

    /** Handle of Server database attribute value entry */
    cy_ble_gatt_db_attr_handle_t valueHandle;

    /** Blood Pressure Client characteristic config descriptor's handle */
    cy_ble_gatt_db_attr_handle_t cccdHandle;

    /** Characteristic's end handle */
    cy_ble_gatt_db_attr_handle_t endHandle;
}cy_stc_ble_blsc_char_t;

/** Structure with discovered attributes information of Blood Pressure Service */
typedef struct
{
    /** Structure with characteristic handles + properties of Blood Pressure Service */
    cy_stc_ble_blsc_char_t charInfo[CY_BLE_BLS_CHAR_COUNT];
}cy_stc_ble_blsc_t;

/** Blood Pressure Service characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;                      /**< Peer device handle */
    cy_en_ble_bls_char_index_t charIndex;                       /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t    *value;                          /**< Characteristic value */
} cy_stc_ble_bls_char_value_t;

/** Blood Pressure Service characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;                     /**< Peer device handle */
    cy_en_ble_bls_char_index_t  charIndex;                      /**< Index of service characteristic */
    cy_en_ble_bls_descr_index_t descrIndex;                     /**< Index of service characteristic descriptor */
    cy_stc_ble_gatt_value_t     *value;                         /**< Descriptor value */
} cy_stc_ble_bls_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** Blood Pressure Service GATT DB handles structure */
    const cy_stc_ble_blss_t *blss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_bls_config_t;

/** @} */


/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_BLS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_BLS_Init(cy_stc_ble_bls_config_t *config);
cy_en_ble_api_result_t Cy_BLE_BLS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_BLS_SERVER
/**
 * \addtogroup group_ble_service_api_BLS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_BLSS_SetCharacteristicValue(cy_en_ble_bls_char_index_t charIndex,
                                                          uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BLSS_GetCharacteristicValue(cy_en_ble_bls_char_index_t charIndex,
                                                          uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BLSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bls_char_index_t charIndex,
                                                               cy_en_ble_bls_descr_index_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BLSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_bls_char_index_t charIndex,
                                                    uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BLSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_bls_char_index_t charIndex,
                                                  uint8_t attrSize, uint8_t *attrValue);
/** @} */
#endif /* CY_BLE_BLS_SERVER */

#ifdef CY_BLE_BLS_CLIENT
/**
 * \addtogroup group_ble_service_api_BLS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_BLSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_bls_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_BLSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bls_char_index_t charIndex,
                                                               cy_en_ble_bls_descr_index_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BLSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_bls_char_index_t charIndex,
                                                               cy_en_ble_bls_descr_index_t descrIndex);
/** @} */
#endif /* CY_BLE_BLS_CLIENT */


/***************************************
* Internal macros
***************************************/

#ifdef CY_BLE_BLS_SERVER
#define CY_BLE_BLS_IS_ICP_SUPPORTED \
    (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_blsConfigPtr->blss->charInfo[CY_BLE_BLS_ICP].charHandle)
#endif /* CY_BLE_BLS_SERVER */

/* ##Macro Functions */

#ifdef CY_BLE_BLS_CLIENT
#define Cy_BLE_BLSC_GetCharacteristicValueHandle(connHandle, charIndex) \
    (((charIndex) >= CY_BLE_BLS_CHAR_COUNT) ?                           \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                            \
     cy_ble_blsc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfo[charIndex].valueHandle)

#define Cy_BLE_BLSC_GetCharacteristicDescriptorHandle(connHandle, charIndex, descrIndex)    \
    ((((charIndex) >= CY_BLE_BLS_CHAR_COUNT) || ((descrIndex) >= CY_BLE_BLS_DESCR_COUNT)) ? \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                                                \
     cy_ble_blsc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfo[charIndex].cccdHandle)
#endif  /* CY_BLE_BLS_CLIENT */

/***************************************
* External data references
***************************************/
#ifdef CY_BLE_BLS_CLIENT
/* Blood Pressure Service Server's GATT DB handles structure */
extern cy_stc_ble_blsc_t cy_ble_blsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_BLS_CLIENT */
extern cy_stc_ble_bls_config_t cy_ble_blsConfig;
extern cy_stc_ble_bls_config_t *cy_ble_blsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_BLS) */
#endif /* CY_BLE_BLS_H */



/* [] END OF FILE */
