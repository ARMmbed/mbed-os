/***************************************************************************//**
* \file cy_ble_lls.h
* \version 2.0
*
* \brief
*  This file contains the function prototypes and constants used in the Link
*  Loss Service of the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_LLS_H
#define CY_BLE_LLS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_LLS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \addtogroup group_ble_service_api_LLS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** Link Loss Service Characteristic indexes */
typedef enum
{
    CY_BLE_LLS_ALERT_LEVEL,                          /**< Alert Level Characteristic index */
    CY_BLE_LLS_CHAR_COUNT                            /**< Total count of characteristics */
}cy_en_ble_lls_char_index_t;

/** Link Loss Service Characteristic Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;          /**< Connection handle */
    cy_en_ble_lls_char_index_t charIndex;           /**< Characteristic index of Link Loss Service */
    cy_stc_ble_gatt_value_t    *value;              /**< Pointer to value of Link Loss Service characteristic */
} cy_stc_ble_lls_char_value_t;

/** Structure with Link Loss Service attribute handles */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t serviceHandle;        /**< Link Loss Service handle*/
    cy_ble_gatt_db_attr_handle_t alertLevelCharHandle; /**< Handle of Alert Level Characteristic */
} cy_stc_ble_llss_t;

/** Structure with discovered attributes information of Link Loss Service */
typedef struct
{
    cy_stc_ble_srvr_char_info_t alertLevelChar;         /**< Handle of Alert Level Characteristic of Link Loss Service */
} cy_stc_ble_llsc_t;

/** Service configuration structure */
typedef struct
{
    /** Link Loss Service GATT DB handles structure */
    const cy_stc_ble_llss_t *llss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_lls_config_t;

/** @} */


/***************************************
* Function Prototypes
***************************************/

/**
 * \addtogroup group_ble_service_api_LLS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_LLS_Init(cy_stc_ble_lls_config_t *config);
cy_en_ble_api_result_t Cy_BLE_LLS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_LLS_SERVER
/**
 * \addtogroup group_ble_service_api_LLS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_LLSS_GetCharacteristicValue(cy_en_ble_lls_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

/** @} */
#endif /* (CY_BLE_LLS_SERVER) */

#ifdef CY_BLE_LLS_CLIENT
/**
 * \addtogroup group_ble_service_api_LLS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_LLSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_lls_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_LLSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_lls_char_index_t charIndex);

/** @} */
#endif /* (CY_BLE_LLS_CLIENT) */


/***************************************
* API Constants
***************************************/

#define CY_BLE_LLS_ALERT_LEVEL_SIZE    (1u)


/***************************************
* Variables with external linkage
***************************************/
#ifdef CY_BLE_LLS_CLIENT
extern cy_stc_ble_llsc_t cy_ble_llsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_LLS_CLIENT) */

extern cy_stc_ble_lls_config_t cy_ble_llsConfig;
extern cy_stc_ble_lls_config_t *cy_ble_llsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_LLS) */
#endif /* CY_BLE_LLS_H */


/* [] END OF FILE */
