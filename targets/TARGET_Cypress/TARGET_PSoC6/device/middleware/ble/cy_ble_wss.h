/***************************************************************************//**
* \file cy_ble_wss.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for Weight Scale Service.
*
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_WSS_H
#define CY_BLE_WSS_H


#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_WSS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Conditional Compilation Parameters
***************************************/


/**
 * \addtogroup group_ble_service_api_WSS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** WSS Characteristic indexes */
typedef enum
{
    CY_BLE_WSS_WEIGHT_SCALE_FEATURE,              /**< Weight Scale Feature Characteristic index */
    CY_BLE_WSS_WEIGHT_MEASUREMENT,                /**< Weight Measurement Characteristic index */
    CY_BLE_WSS_CHAR_COUNT                         /**< Total count of WSS Characteristics */
}cy_en_ble_wss_char_index_t;

/** WSS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_WSS_CCCD,                              /**< Client Characteristic Configuration Descriptor index */
    CY_BLE_WSS_DESCR_COUNT                        /**< Total count of Descriptors */
}cy_en_ble_wss_descr_index_t;

/** WSS Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;        /**< Peer device handle */
    cy_en_ble_wss_char_index_t charIndex;         /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t    *value;            /**< Characteristic value */
} cy_stc_ble_wss_char_value_t;

/** WSS Characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;       /**< Peer device handle */
    cy_en_ble_wss_char_index_t  charIndex;        /**< Index of service characteristic */
    cy_en_ble_wss_descr_index_t descrIndex;       /**< Index of descriptor */
    cy_stc_ble_gatt_value_t     *value;           /**< Characteristic value */
} cy_stc_ble_wss_descr_value_t;

/** Structure with Weight Scale Service attribute handles */
typedef struct
{
    /** Handle of characteristic value */
    cy_ble_gatt_db_attr_handle_t charHandle;

    /** Array of descriptor handles */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_WSS_DESCR_COUNT];
} cy_stc_ble_wsss_char_t;

/** WSS Characteristic with descriptors handles */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t serviceHandle;                   /**< Weight Scale Service handle */
    cy_stc_ble_wsss_char_t       charInfo[CY_BLE_WSS_CHAR_COUNT]; /**< Array of characteristics and descriptors handles */
} cy_stc_ble_wsss_t;

/** WSS Service Full characteristic information structure */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t valueHandle;                         /**< Handle of characteristic value */
    uint8_t                      properties;                          /**< Properties for value field */
    cy_ble_gatt_db_attr_handle_t endHandle;                           /**< End handle of characteristic */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_WSS_DESCR_COUNT]; /**< Array of descriptor handles */
} cy_stc_ble_wssc_char_t;

/** Structure with discovered attributes information of Weight Scale Service */
typedef struct
{
    /** Weight Scale Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Weight Scale Service characteristics info structure */
    cy_stc_ble_wssc_char_t       charInfo[CY_BLE_WSS_CHAR_COUNT];
} cy_stc_ble_wssc_t;

/** Service configuration structure */
typedef struct
{
    /** Weight Scale Service GATT DB handles structure */
    const cy_stc_ble_wsss_t *wsss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_wss_config_t;

/** @} */


/***************************************
* API Constants
***************************************/
#define CY_BLE_WSS_UNKNOWN_USER    (0xFFu)


/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_WSS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_WSS_Init(cy_stc_ble_wss_config_t *config);
cy_en_ble_api_result_t Cy_BLE_WSS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_WSS_SERVER
/**
 * \addtogroup group_ble_service_api_WSS_server
 * @{
 */
#if (CY_BLE_GAP_ROLE_PERIPHERAL)
uint8_t Cy_BLE_WSS_GetAdUserIdListSize(uint8_t advertisingParamIndex);

cy_en_ble_api_result_t Cy_BLE_WSS_SetAdUserId(uint8_t listSize, const uint8_t userIdList[],
                                              uint8_t advertisingParamIndex);
#endif /* (CY_BLE_GAP_ROLE_PERIPHERAL) */

cy_en_ble_api_result_t Cy_BLE_WSSS_SetCharacteristicValue(cy_en_ble_wss_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_WSSS_GetCharacteristicValue(cy_en_ble_wss_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_WSSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_wss_char_index_t charIndex,
                                                               cy_en_ble_wss_descr_index_t descrIndex, uint8_t attrSize,
                                                               uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_WSSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_wss_char_index_t charIndex,
                                                               cy_en_ble_wss_descr_index_t descrIndex, uint8_t attrSize,
                                                               uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_WSSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_wss_char_index_t charIndex, uint8_t attrSize,
                                                  uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_WSS_SERVER */

#ifdef CY_BLE_WSS_CLIENT

/**
 * \addtogroup group_ble_service_api_WSS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_WSSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_wss_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_WSSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_wss_char_index_t charIndex,
                                                               cy_en_ble_wss_descr_index_t
                                                               descrIndex, uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_WSSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_wss_char_index_t charIndex,
                                                               cy_en_ble_wss_descr_index_t
                                                               descrIndex);

/** @} */
#endif /* CY_BLE_WSS_CLIENT */


/***************************************
* External data references
***************************************/

#ifdef CY_BLE_WSS_CLIENT
extern cy_stc_ble_wssc_t cy_ble_wssc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_WSS_CLIENT */

extern cy_stc_ble_wss_config_t cy_ble_wssConfig;
extern cy_stc_ble_wss_config_t *cy_ble_wssConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_WSS) */
#endif /* CY_BLE_WSS_H */

/* [] END OF FILE */
