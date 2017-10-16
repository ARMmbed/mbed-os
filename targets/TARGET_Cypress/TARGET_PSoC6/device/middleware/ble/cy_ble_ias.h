/***************************************************************************//**
* \file cy_ble_ias.h
* \version 2.0
*
* \brief
*  This file contains the function prototypes and constants used in
*  the Immediate Alert Service of the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_IAS_H
#define CY_BLE_IAS_H


#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_IAS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \addtogroup group_ble_service_api_IAS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** Immediate Alert Service Characteristic indexes */
typedef enum
{
    CY_BLE_IAS_ALERT_LEVEL,                          /**< Alert Level Characteristic index */
    CY_BLE_IAS_CHAR_COUNT                            /**< Total count of characteristics */
}cy_en_ble_ias_char_index_t;

/** Structure with Immediate Alert Service attribute handles */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t serviceHandle;           /**< Immediate Alert Service handle*/
    cy_ble_gatt_db_attr_handle_t alertLevelCharHandle;    /**< Handle of Alert Level Characteristic */
} cy_stc_ble_iass_t;

/** Immediate Alert Service Characteristic Value parameters structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;       /**< Connection handle */
    cy_en_ble_ias_char_index_t charIndex;        /**< Characteristic index of Immediate Alert Service */
    cy_stc_ble_gatt_value_t    *value;           /**< Pointer to value of Immediate Alert Service characteristic */
} cy_stc_ble_ias_char_value_t;

/** Structure with discovered attributes information of Immediate Alert Service */
typedef struct
{
    /** Handle of Alert Level Characteristic of Immediate Alert Service */
    cy_stc_ble_srvr_char_info_t alertLevelChar;
} cy_stc_ble_iasc_t;

/** Service configuration structure */
typedef struct
{
    /** Immediate Alert Service GATT DB handles structure */
    const cy_stc_ble_iass_t *iass;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_ias_config_t;
/** @} */


/***************************************
* Function Prototypes
***************************************/
/**
 * \addtogroup group_ble_service_api_IAS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_IAS_Init(cy_stc_ble_ias_config_t *config);
/** @} */

#ifdef CY_BLE_IAS_SERVER
/**
 * \addtogroup group_ble_service_api_IAS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_IAS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);

cy_en_ble_api_result_t Cy_BLE_IASS_GetCharacteristicValue(cy_en_ble_ias_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

/** @} */
#endif /* (CY_BLE_IAS_SERVER) */

#ifdef CY_BLE_IAS_CLIENT
/**
 * \addtogroup group_ble_service_api_IAS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_IASC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_ias_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

/** @} */
#endif /* (CY_BLE_IAS_CLIENT) */


/***************************************
* API Constants
***************************************/

#define CY_BLE_IAS_ALERT_LEVEL_SIZE    (1u)


/***************************************
* Variables with external linkage
***************************************/
#ifdef CY_BLE_IAS_CLIENT
extern cy_stc_ble_iasc_t cy_ble_iasc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_IAS_CLIENT) */

extern cy_stc_ble_ias_config_t cy_ble_iasConfig;
extern cy_stc_ble_ias_config_t *cy_ble_iasConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_IAS) */
#endif /* CY_BLE_IAS_H */


/* [] END OF FILE */
