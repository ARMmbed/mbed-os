/***************************************************************************//**
* \file cy_ble_hps.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for HTTP Proxy Service.
*
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_HPS_H
#define CY_BLE_HPS_H

#include "cy_ble_event_handler.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_HPS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Conditional Compilation Parameters
***************************************/

/**
 * \addtogroup group_ble_service_api_HPS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** HPS Characteristic indexes */
typedef enum
{
    CY_BLE_HPS_URI,                              /**< Universal Resource Identifier Characteristics index */
    CY_BLE_HPS_HTTP_HEADERS,                     /**< HTTP Headers Characteristics index */
    CY_BLE_HPS_HTTP_ENTITY_BODY,                 /**< HTTP Entity Body Characteristics index */
    CY_BLE_HPS_HTTP_CP,                          /**< HTTP Control Point Characteristics index */
    CY_BLE_HPS_HTTP_STATUS_CODE,                 /**< HTTP Status Code Characteristics index */
    CY_BLE_HPS_HTTPS_SECURITY,                   /**< HTTPS Security Characteristics index */
    CY_BLE_HPS_CHAR_COUNT                        /**< Total count of HPS Characteristics */
}cy_en_ble_hps_char_index_t;

/** HPS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_HPS_CCCD,                              /**< Client Characteristic Configuration Descriptor index */
    CY_BLE_HPS_DESCR_COUNT                        /**< Total count of Descriptors */
} cy_en_ble_hps_descr_index_t;

/** HPS Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;        /**< Peer device handle */
    cy_en_ble_hps_char_index_t charIndex;         /**< Index of service characteristic */
    cy_en_ble_gatt_err_code_t  gattErrorCode;     /**< Error code received from application (optional) */
    cy_stc_ble_gatt_value_t    *value;            /**< Characteristic value */
} cy_stc_ble_hps_char_value_t;

/** HPS Characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;        /**< Peer device handle */
    cy_en_ble_hps_char_index_t  charIndex;         /**< Index of service characteristic */
    cy_en_ble_hps_descr_index_t descrIndex;        /**< Index of descriptor */
    cy_en_ble_gatt_err_code_t   gattErrorCode;     /**< Error code received from application (optional) */
    cy_stc_ble_gatt_value_t     *value;            /**< Characteristic value */
} cy_stc_ble_hps_descr_value_t;

/** Structure with HTTP Proxy Service attribute handles */
typedef struct
{
    /** Handle of characteristic value */
    cy_ble_gatt_db_attr_handle_t charHandle;
    /** Array of descriptor handles */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_HPS_DESCR_COUNT];
} cy_stc_ble_hpss_char_t;

/** HPS Characteristic with descriptors handles */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t serviceHandle;                   /**< HTTP Proxy Service handle */
    cy_stc_ble_hpss_char_t       charInfo[CY_BLE_HPS_CHAR_COUNT]; /**< Array of characteristics and descriptors handles */
} cy_stc_ble_hpss_t;

/** HPS Service full characteristic information structure */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t valueHandle;                         /**< Handle of characteristic value */
    uint8_t                      properties;                          /**< Properties for value field */
    cy_ble_gatt_db_attr_handle_t endHandle;                           /**< End handle of characteristic */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_HPS_DESCR_COUNT]; /**< Array of descriptor handles */
} cy_stc_ble_hpsc_char_t;

/** Structure with discovered attributes information of HTTP Proxy Service */
typedef struct
{
    /** HTTP Proxy Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;
    /** HTTP Proxy Service characteristics info structure */
    cy_stc_ble_hpsc_char_t       charInfo[CY_BLE_HPS_CHAR_COUNT];
} cy_stc_ble_hpsc_t;

/** HTTP Requests */
typedef enum
{
    CY_BLE_HPS_HTTP_GET = 0x01u,                   /**< HTTP GET Request */
    CY_BLE_HPS_HTTP_HEAD,                          /**< HTTP HEAD Request */
    CY_BLE_HPS_HTTP_POST,                          /**< HTTP POST Request */
    CY_BLE_HPS_HTTP_PUT,                           /**< HTTP PUT Request */
    CY_BLE_HPS_HTTP_DELETE,                        /**< HTTP DELETE Request */
    CY_BLE_HPS_HTTPS_GET,                          /**< HTTS GET Request */
    CY_BLE_HPS_HTTPS_HEAD,                         /**< HTTPS HEAD Request */
    CY_BLE_HPS_HTTPS_POST,                         /**< HTTPS POST Request */
    CY_BLE_HPS_HTTPS_PUT,                          /**< HTTPS PUT Request */
    CY_BLE_HPS_HTTPS_DELETE,                       /**< HTTPS DELETE Request */
    CY_BLE_HPS_HTTP_REQ_CANCEL                     /**< HTTP CANCEL Request */
}cy_en_ble_hps_http_request_t;

/** Service configuration structure */
typedef struct
{
    /** HTTP Proxy Service GATT DB handles structure */
    const cy_stc_ble_hpss_t *hpss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_hps_config_t;

/** @} */


/***************************************
* API Constants
***************************************/
/* HTTP Status Code Data Status bits  */
#define CY_BLE_HPS_HTTP_HEADERS_RECEIVED     (0x01u)
#define CY_BLE_HPS_HTTP_HEADERS_TRUNCATED    (0x02u)
#define CY_BLE_HPS_HTTP_BODY_RECEIVED        (0x04u)
#define CY_BLE_HPS_HTTP_BODY_TRUNCATED       (0x08u)


/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_HPS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_HPS_Init(cy_stc_ble_hps_config_t *config);
cy_en_ble_api_result_t Cy_BLE_HPS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

/**
 * \addtogroup group_ble_service_api_HPS_server
 * @{
 */

#ifdef CY_BLE_HPS_SERVER

cy_en_ble_api_result_t Cy_BLE_HPSS_SetCharacteristicValue(cy_en_ble_hps_char_index_t charIndex, uint16_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HPSS_GetCharacteristicValue(cy_en_ble_hps_char_index_t charIndex, uint16_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HPSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hps_char_index_t charIndex,
                                                               cy_en_ble_hps_descr_index_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HPSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hps_char_index_t charIndex,
                                                               cy_en_ble_hps_descr_index_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HPSS_SendNotification(cy_stc_ble_conn_handle_t connHandle, cy_en_ble_hps_char_index_t
                                                    charIndex, uint8_t attrSize, uint8_t *attrValue);

#endif /* CY_BLE_HPS_SERVER */

/** @} */

/**
 * \addtogroup group_ble_service_api_HPS_client
 * @{
 */

#ifdef CY_BLE_HPS_CLIENT

cy_en_ble_api_result_t Cy_BLE_HPSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_hps_char_index_t charIndex, uint16_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HPSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_hps_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_HPSC_SetLongCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                              cy_en_ble_hps_char_index_t charIndex, uint16_t attrSize,
                                                              uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HPSC_GetLongCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                              cy_en_ble_hps_char_index_t charIndex, uint16_t attrSize,
                                                              uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HPSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hps_char_index_t charIndex,
                                                               cy_en_ble_hps_descr_index_t descrIndex, uint8_t attrSize,
                                                               uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HPSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hps_char_index_t charIndex,
                                                               cy_en_ble_hps_descr_index_t descrIndex);

#endif /* CY_BLE_HPS_CLIENT */

/** @} */


/***************************************
* External data references
***************************************/
#ifdef CY_BLE_HPS_CLIENT
extern cy_stc_ble_hpsc_t cy_ble_hpsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_HPS_CLIENT */

extern cy_stc_ble_hps_config_t cy_ble_hpsConfig;
extern cy_stc_ble_hps_config_t *cy_ble_hpsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_HPS) */
#endif /* CY_BLE_HPS_H */

/* [] END OF FILE */
