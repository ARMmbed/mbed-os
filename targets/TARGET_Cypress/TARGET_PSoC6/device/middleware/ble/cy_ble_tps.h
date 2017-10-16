/***************************************************************************//**
* \file cy_ble_tps.h
* \version 2.0
*
* \brief
*  This file contains the function prototypes and constants used in the Tx
*  Power Service of the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_TPS_H
#define CY_BLE_TPS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_TPS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Conditional Compilation Parameters
***************************************/

/* Maximum supported Tx Power Services */
#define CY_BLE_TPS_SERVICE_COUNT    (1u)

/**
 * \addtogroup group_ble_service_api_TPS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** TPS Characteristic indexes */
typedef enum
{
    CY_BLE_TPS_TX_POWER_LEVEL,                      /**< Tx Power Level characteristic index */
    CY_BLE_TPS_CHAR_COUNT                           /**< Total count of characteristics */
}cy_en_ble_tps_char_index_t;

/** TPS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_TPS_CCCD,                    /**< Tx Power Level Client Characteristic configuration descriptor index */
    CY_BLE_TPS_DESCR_COUNT              /**< Total count of Tx Power Service characteristic descriptors */
}cy_en_ble_tps_char_descriptors_t;

/** Tx Power Service Characteristic Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;          /**< Connection handle */
    cy_en_ble_tps_char_index_t charIndex;           /**< Characteristic index of Tx Power Service */
    cy_stc_ble_gatt_value_t    *value;              /**< Pointer to value of Tx Power Service characteristic */
} cy_stc_ble_tps_char_value_t;

/** Tx Power Service Characteristic Descriptor Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t         connHandle;     /**< Connection handle */
    cy_en_ble_tps_char_index_t       charIndex;      /**< Characteristic index of Tx Power Service */
    cy_en_ble_tps_char_descriptors_t descrIndex;     /**< Characteristic index Descriptor of Tx Power Service */
    cy_stc_ble_gatt_value_t          *value;         /**< Pointer to value of Tx Power Service characteristic */
} cy_stc_ble_tps_descr_value_t;

/** Structure with Tx Power Service attribute handles */
typedef struct
{
    /** Tx Power Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Tx Power Level Characteristic handle */
    cy_ble_gatt_db_attr_handle_t txPowerLevelCharHandle;

    /** Tx Power Level Client Characteristic Configuration Descriptor handle */
    cy_ble_gatt_db_attr_handle_t txPowerLevelCccdHandle;
} cy_stc_ble_tpss_t;

/** Structure with discovered attributes information of Tx Power Service */
typedef struct
{
    /** Tx Power Level Characteristic handle */
    cy_stc_ble_srvr_char_info_t  txPowerLevelChar;

    /** Tx Power Level Client Characteristic Configuration Descriptor handle */
    cy_ble_gatt_db_attr_handle_t txPowerLevelCccdHandle;
} cy_stc_ble_tpsc_t;

/** Service configuration structure */
typedef struct
{
    /** Tx Power Service  GATT DB handles structure */
    const cy_stc_ble_tpss_t *tpss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_tps_config_t;

/** @} */


/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_TPS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_TPS_Init(cy_stc_ble_tps_config_t *config);
cy_en_ble_api_result_t Cy_BLE_TPS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_TPS_SERVER
/**
 * \addtogroup group_ble_service_api_TPS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_TPSS_SetCharacteristicValue(cy_en_ble_tps_char_index_t charIndex, uint8_t attrSize,
                                                          int8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_TPSS_GetCharacteristicValue(cy_en_ble_tps_char_index_t charIndex, uint8_t attrSize,
                                                          int8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_TPSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_tps_char_index_t charIndex,
                                                               cy_en_ble_tps_char_descriptors_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_TPSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_tps_char_index_t charIndex, uint8_t attrSize,
                                                    int8_t *attrValue);

/** @} */
#endif /* (CY_BLE_TPS_SERVER) */

#ifdef CY_BLE_TPS_CLIENT
/**
 * \addtogroup group_ble_service_api_TPS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_TPSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_tps_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_TPSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_tps_char_index_t charIndex,
                                                               cy_en_ble_tps_char_descriptors_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_TPSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_tps_char_index_t charIndex,
                                                               cy_en_ble_tps_char_descriptors_t descrIndex);

/** @} */
#endif /* (CY_BLE_TPS_CLIENT) */


/***************************************
* API Constants
***************************************/
#define CY_BLE_TPS_TX_POWER_LEVEL_SIZE    (1u)

/***************************************
* Internal macros
***************************************/
#ifdef CY_BLE_TPS_CLIENT
#define Cy_BLE_TPSC_GetCharacteristicValueHandle(connHandle, charIndex) \
    (((charIndex) >= CY_BLE_TPS_CHAR_COUNT) ?                           \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                            \
     cy_ble_tpsc[Cy_BLE_GetDiscoveryIdx(connHandle)].txPowerLevelChar.valueHandle)

#define Cy_BLE_TPSC_GetCharacteristicDescriptorHandle(connHandle, charIndex, descrIndex)    \
    ((((charIndex) >= CY_BLE_TPS_CHAR_COUNT) || ((descrIndex) >= CY_BLE_TPS_DESCR_COUNT)) ? \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                                                \
     cy_ble_tpsc[Cy_BLE_GetDiscoveryIdx(connHandle)].txPowerLevelCccdHandle)
#endif /* (CY_BLE_TPS_CLIENT) */

/***************************************
* Variables with external linkage
***************************************/
#ifdef CY_BLE_TPS_CLIENT
extern cy_stc_ble_tpsc_t cy_ble_tpsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_TPS_CLIENT) */

extern cy_stc_ble_tps_config_t cy_ble_tpsConfig;
extern cy_stc_ble_tps_config_t *cy_ble_tpsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_TPS) */
#endif /* CY_BLE_TPS_H */

/* [] END OF FILE */
