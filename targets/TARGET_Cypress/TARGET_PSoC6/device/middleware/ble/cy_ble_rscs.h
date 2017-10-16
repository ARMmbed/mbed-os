/***************************************************************************//**
* \file cy_ble_rscs.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for Running Speed and Cadence
*  Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_RSCS_H
#define CY_BLE_RSCS_H


#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_RSCS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \addtogroup group_ble_service_api_RSCS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** RSCS Characteristic indexes */
typedef enum
{
    CY_BLE_RSCS_RSC_MEASUREMENT,                      /**< RSC Measurement Characteristic index */
    CY_BLE_RSCS_RSC_FEATURE,                          /**< RSC Feature Characteristic index */
    CY_BLE_RSCS_SENSOR_LOCATION,                      /**< Sensor Location Characteristic index */
    CY_BLE_RSCS_SC_CONTROL_POINT,                     /**< SC Control Point Characteristic index */
    CY_BLE_RSCS_CHAR_COUNT                            /**< Total count of RSCS characteristics */
} cy_en_ble_rscs_char_index_t;

/** RSCS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_RSCS_CCCD,                                  /**< Client Characteristic Configuration Descriptor index */
    CY_BLE_RSCS_DESCR_COUNT                            /**< Total count of descriptors */
} cy_en_ble_rscs_descr_index_t;

/** Running Speed and Cadence Service Characteristic Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;               /**< Peer device handle */
    cy_en_ble_rscs_char_index_t charIndex;                /**< Index of Running Speed and Cadence Service Characteristic */
    cy_stc_ble_gatt_value_t     *value;                   /**< Characteristic value */
} cy_stc_ble_rscs_char_value_t;

/** Running Speed and Cadence Service Characteristic Descriptor Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t     connHandle;            /**< Connection handle */
    cy_en_ble_rscs_char_index_t  charIndex;             /**< Characteristic index of the Service */
    cy_en_ble_rscs_descr_index_t descrIndex;            /**< Characteristic index Descriptor the Service */
    cy_stc_ble_gatt_value_t      *value;                /**< Pointer to value of the Service Characteristic Descriptor */
} cy_stc_ble_rscs_descr_value_t;

/** RSCS Characteristic with descriptors */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t charHandle;                            /**< Handle of the characteristic value */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_RSCS_DESCR_COUNT];  /**< Handle of the descriptor */
} cy_stc_ble_rscss_char_t;

/** Structure with Running Speed and Cadence Service attribute handles */
typedef struct
{
    /** Running Speed and Cadence Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Array of Running Speed and Cadence Service Characteristics + Descriptors handles */
    cy_stc_ble_rscss_char_t      charInfo[CY_BLE_RSCS_CHAR_COUNT];
} cy_stc_ble_rscss_t;

/** RSCS Service Full characteristic information type */
typedef struct
{
    /** Characteristic handle + properties */
    cy_stc_ble_srvr_char_info_t  charInfo;

    /** Characteristic descriptors handles handle */
    cy_ble_gatt_db_attr_handle_t descriptors[CY_BLE_RSCS_DESCR_COUNT];

    /** End handle of characteristic */
    cy_ble_gatt_db_attr_handle_t endHandle;
} cy_stc_ble_rscsc_srvr_full_char_info_t;

/** Structure with discovered attributes information of Running Speed and Cadence Service */
typedef struct
{
    /** Characteristics handles array */
    cy_stc_ble_rscsc_srvr_full_char_info_t characteristics[CY_BLE_RSCS_CHAR_COUNT];
} cy_stc_ble_rscsc_t;

/** Service configuration structure */
typedef struct
{
    /** Running Speed and Cadence Service GATT DB handles structure */
    const cy_stc_ble_rscss_t *rscss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t        callbackFunc;
} cy_stc_ble_rscs_config_t;
/** @} */


/***************************************
* API Constants
***************************************/

#define CY_BLE_RSCS_INVALID_CHAR_INDEX               (0xFFu)

/* SC Control Point Characteristic Op Codes */
#define CY_BLE_RSCS_SET_CUMMULATIVE_VALUE            (0x01u)
#define CY_BLE_RSCS_START_SENSOR_CALIBRATION         (0x02u)
#define CY_BLE_RSCS_UPDATE_SENSOR_LOCATION           (0x03u)
#define CY_BLE_RSCS_REQ_SUPPORTED_SENSOR_LOCATION    (0x04u)
#define CY_BLE_RSCS_RESPONSE_CODE                    (0x10u)

/* SC Control Point Characteristic Response Codes */
#define CY_BLE_RSCS_ERR_SUCCESS                      (0x01u)
#define CY_BLE_RSCS_ERR_OP_CODE_NOT_SUPPORTED        (0x02u)
#define CY_BLE_RSCS_ERR_INVALID_PARAMETER            (0x03u)
#define CY_BLE_RSCS_ERR_OPERATION_FAILED             (0x04u)


/***************************************
* Function Prototypes
***************************************/

/**
 * \addtogroup group_ble_service_api_RSCS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_RSCS_Init(cy_stc_ble_rscs_config_t *config);
cy_en_ble_api_result_t Cy_BLE_RSCS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#if defined(CY_BLE_RSCS_SERVER)
/**
 * \addtogroup group_ble_service_api_RSCS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_RSCSS_SetCharacteristicValue(cy_en_ble_rscs_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_RSCSS_GetCharacteristicValue(cy_en_ble_rscs_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_RSCSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_rscs_char_index_t charIndex,
                                                                cy_en_ble_rscs_descr_index_t descrIndex, uint8_t attrSize,
                                                                uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_RSCSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     cy_en_ble_rscs_char_index_t charIndex, uint8_t attrSize,
                                                     uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_RSCSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                   cy_en_ble_rscs_char_index_t charIndex, uint8_t attrSize,
                                                   uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_RSCS_SERVER */

#if defined(CY_BLE_RSCS_CLIENT)
/**
 * \addtogroup group_ble_service_api_RSCS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_RSCSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_rscs_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_RSCSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_rscs_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_RSCSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_rscs_char_index_t charIndex,
                                                                cy_en_ble_rscs_descr_index_t descrIndex, uint8_t attrSize,
                                                                uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_RSCSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_rscs_char_index_t charIndex,
                                                                uint8_t descrIndex);

/** @} */
#endif /* (CY_BLE_RSCS_CLIENT) */


/***************************************
* External data references
***************************************/
#if defined(CY_BLE_RSCS_CLIENT)
extern cy_stc_ble_rscsc_t cy_ble_rscsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_RSCS_CLIENT) */

extern cy_stc_ble_rscs_config_t cy_ble_rscsConfig;
extern cy_stc_ble_rscs_config_t *cy_ble_rscsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_RSCS) */
#endif /* CY_BLE_RSCS_H */

/* [] END OF FILE */
