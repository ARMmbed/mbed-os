/***************************************************************************//**
* \file cy_ble_gls.h
* \version 2.0
*
* \brief
*  This file contains the function prototypes and constants used in the
*  Glucose Profile of the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_GLS_H
#define CY_BLE_GLS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_GLS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CY_BLE_GLS_FLAG_PROCESS         (0x01u)
#define CY_BLE_GLS_RACP_OPCODE_ABORT    (0x03u)

/**
 * \addtogroup group_ble_service_api_GLS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** Service Characteristics indexes */
typedef enum
{
    CY_BLE_GLS_GLMT,           /**< Glucose Measurement characteristic index */
    CY_BLE_GLS_GLMC,           /**< Glucose Measurement Context characteristic index */
    CY_BLE_GLS_GLFT,           /**< Glucose Feature characteristic index */
    CY_BLE_GLS_RACP,           /**< Record Access Control Point characteristic index */
    CY_BLE_GLS_CHAR_COUNT      /**< Total count of GLS characteristics */
}cy_en_ble_gls_char_index_t;

/** Service Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_GLS_CCCD,           /**< Client Characteristic Configuration descriptor index */
    CY_BLE_GLS_DESCR_COUNT     /**< Total count of GLS descriptors */
}cy_en_ble_gls_descr_index_t;

/** Glucose Server Characteristic structure type */
typedef struct
{
    /** Glucose Service char handle */
    cy_ble_gatt_db_attr_handle_t charHandle;

    /** Glucose Service CCCD handle */
    cy_ble_gatt_db_attr_handle_t cccdHandle;
}cy_stc_ble_glss_char_t;

/** Structure with Glucose Service attribute handles */
typedef struct
{
    /** Glucose Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Glucose Service characteristics info array */
    cy_stc_ble_glss_char_t       charInfo[CY_BLE_GLS_CHAR_COUNT];
} cy_stc_ble_glss_t;

/** Glucose Client Characteristic structure type */
typedef struct
{
    /** Properties for value field */
    uint8_t                      properties;

    /** Handle of server database attribute value entry */
    cy_ble_gatt_db_attr_handle_t valueHandle;

    /** Glucose client char. descriptor handle */
    cy_ble_gatt_db_attr_handle_t cccdHandle;

    /** Characteristic End Handle */
    cy_ble_gatt_db_attr_handle_t endHandle;
}cy_stc_ble_glsc_char_t;

/** Glucose Service structure type */
typedef struct
{
    /** Characteristics handle + properties array */
    cy_stc_ble_glsc_char_t charInfo[CY_BLE_GLS_CHAR_COUNT];
}cy_stc_ble_glsc_t;

/** Glucose Service Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;                      /**< Peer device handle */
    cy_en_ble_gls_char_index_t charIndex;                       /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t    *value;                          /**< Characteristic value */
} cy_stc_ble_gls_char_value_t;

/** Glucose Service Characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;                     /**< Peer device handle */
    cy_en_ble_gls_char_index_t  charIndex;                      /**< Index of service characteristic */
    cy_en_ble_gls_descr_index_t descrIndex;                     /**< Index of service characteristic descriptor */
    cy_stc_ble_gatt_value_t     *value;                         /**< Descriptor value */
} cy_stc_ble_gls_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** Glucose Profile  GATT DB handles structure */
    const cy_stc_ble_glss_t *glss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_gls_config_t;
/** @} */


/***************************************
* Function Prototypes
***************************************/

/**
 * \addtogroup group_ble_service_api_GLS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_GLS_Init(cy_stc_ble_gls_config_t *config);
cy_en_ble_api_result_t Cy_BLE_GLS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_GLS_SERVER
/**
 * \addtogroup group_ble_service_api_GLS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_GLSS_SetCharacteristicValue(cy_en_ble_gls_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);
cy_en_ble_api_result_t Cy_BLE_GLSS_GetCharacteristicValue(cy_en_ble_gls_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_GLSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_gls_char_index_t charIndex,
                                                               cy_en_ble_gls_descr_index_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_GLSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_gls_char_index_t charIndex, uint8_t attrSize,
                                                    uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_GLSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_gls_char_index_t charIndex, uint8_t attrSize,
                                                  uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_GLS_SERVER */

#ifdef CY_BLE_GLS_CLIENT
/**
 * \addtogroup group_ble_service_api_GLS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_GLSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_gls_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_GLSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_gls_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_GLSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_gls_char_index_t charIndex,
                                                               cy_en_ble_gls_descr_index_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_GLSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_gls_char_index_t charIndex,
                                                               cy_en_ble_gls_descr_index_t descrIndex);

/** @} */
#endif /* CY_BLE_GLS_CLIENT */


/* Internal macros */
#ifdef CY_BLE_GLS_SERVER
#define CY_BLE_GLS_IS_GMC_SUPPORTED \
    (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_glsConfigPtr->glss->charInfo[CY_BLE_GLS_GLMC].charHandle)

#define CY_BLE_GLS_IS_PROCEDURE_IN_PROGRESS \
    (0u != (CY_BLE_GLS_FLAG_PROCESS & cy_ble_glsFlag))
#endif /* CY_BLE_GLS_SERVER */
/** \endcond */


/***************************************
*  External data references
***************************************/

#ifdef CY_BLE_GLS_SERVER
/* Glucose Service GATT DB handles structure */
extern uint8_t cy_ble_glsFlag;
#endif /* CY_BLE_GLSC_SERVER */

#ifdef CY_BLE_GLS_CLIENT
/* Glucose Service server's GATT DB handles structure */
extern cy_stc_ble_glsc_t cy_ble_glsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_GLS_CLIENT */

extern cy_stc_ble_gls_config_t cy_ble_glsConfig;
extern cy_stc_ble_gls_config_t *cy_ble_glsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_GLS) */
#endif /* CY_BLE_GLS_H */

/* [] END OF FILE */
