/***************************************************************************//**
* \file cy_ble_cscs.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for Cycling Speed and Cadence
*  Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CY_BLE_CSCS_H
#define CY_BLE_CSCS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_CSCS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \addtogroup group_ble_service_api_CSCS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/
/** Characteristic indexes */
typedef enum
{
    CY_BLE_CSCS_CSC_MEASUREMENT,                      /**< CSC Measurement Characteristic index */
    CY_BLE_CSCS_CSC_FEATURE,                          /**< CSC Feature Characteristic index */
    CY_BLE_CSCS_SENSOR_LOCATION,                      /**< CSC Sensor Location Characteristic index */
    CY_BLE_CSCS_SC_CONTROL_POINT,                     /**< CSC SC Control Point Characteristic index */
    CY_BLE_CSCS_CHAR_COUNT                            /**< Total count of CSCS Characteristics */
} cy_en_ble_cscs_char_index_t;

/** Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_CSCS_CCCD,                                 /**< Client Characteristic Configuration Descriptor index */
    CY_BLE_CSCS_DESCR_COUNT                           /**< Total count of Descriptors */
} cy_en_ble_cscs_descr_index_t;

/** Cycling Speed and Cadence Service Characteristic Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;            /**< Peer device handle */
    cy_en_ble_cscs_char_index_t charIndex;             /**< Index of Cycling Speed and Cadence Service Characteristic */
    cy_stc_ble_gatt_value_t     *value;                /**< Characteristic value */
} cy_stc_ble_cscs_char_value_t;

/** Cycling Speed and Cadence Service Characteristic Descriptor Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t     connHandle;           /**< Connection handle */
    cy_en_ble_cscs_char_index_t  charIndex;            /**< Characteristic index of the Service */
    cy_en_ble_cscs_descr_index_t descrIndex;           /**< Characteristic Descriptor index */
    cy_stc_ble_gatt_value_t      *value;               /**< Pointer to value of the Service Characteristic Descriptor */
} cy_stc_ble_cscs_descr_value_t;

/** Characteristic with descriptors type */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t charHandle;                            /**< Handle of the Characteristic value */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_CSCS_DESCR_COUNT];  /**< Handles of the Descriptors */
} cy_stc_ble_cscss_char_t;

/** Structure with Cycling Speed and Cadence Service attribute handles */
typedef struct
{
    /** Cycling Speed and Cadence Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Array of Cycling Speed and Cadence Service Characteristics and Descriptors handles */
    cy_stc_ble_cscss_char_t      charInfo[CY_BLE_CSCS_CHAR_COUNT];
} cy_stc_ble_cscss_t;

/** Service full Characteristic information type */
typedef struct
{
    /** Characteristic handle and properties */
    cy_stc_ble_srvr_char_info_t  charInfo;

    /** Characteristic descriptors handles */
    cy_ble_gatt_db_attr_handle_t descriptors[CY_BLE_CSCS_DESCR_COUNT];

    /** End handle of Characteristic */
    cy_ble_gatt_db_attr_handle_t endHandle;
} cy_stc_ble_cscsc_srvr_full_char_info_t;

/** Structure with discovered attributes information of Cycling Speed and Cadence Service */
typedef struct
{
    /** Characteristics handles array */
    cy_stc_ble_cscsc_srvr_full_char_info_t characteristics[CY_BLE_CSCS_CHAR_COUNT];
} cy_stc_ble_cscsc_t;

/** Service configuration structure */
typedef struct
{
    /** Cycling Speed and Cadence Service GATT DB handles structure */
    const cy_stc_ble_cscss_t *cscss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t        callbackFunc;
} cy_stc_ble_cscs_config_t;

/** @} */


/***************************************
* API Constants
***************************************/

#define CY_BLE_CSCS_INVALID_CHAR_INDEX               (0xFFu)

/* SC Control Point Characteristic Op Codes */
#define CY_BLE_CSCS_SET_CUMMULATIVE_VALUE            (0x01u)
#define CY_BLE_CSCS_START_SENSOR_CALIBRATION         (0x02u)
#define CY_BLE_CSCS_UPDATE_SENSOR_LOCATION           (0x03u)
#define CY_BLE_CSCS_REQ_SUPPORTED_SENSOR_LOCATION    (0x04u)
#define CY_BLE_CSCS_RESPONSE_CODE                    (0x10u)

/* SC Control Point Characteristic Response Codes */
#define CY_BLE_CSCS_ERR_SUCCESS                      (0x01u)
#define CY_BLE_CSCS_ERR_OP_CODE_NOT_SUPPORTED        (0x02u)
#define CY_BLE_CSCS_ERR_INVALID_PARAMETER            (0x03u)
#define CY_BLE_CSCS_ERR_OPERATION_FAILED             (0x04u)


/***************************************
* Function Prototypes
***************************************/

/**
 * \addtogroup group_ble_service_api_CSCS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_CSCS_Init(cy_stc_ble_cscs_config_t *config);
cy_en_ble_api_result_t Cy_BLE_CSCS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_CSCS_SERVER
/**
 * \addtogroup group_ble_service_api_CSCS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_CSCSS_SetCharacteristicValue(cy_en_ble_cscs_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CSCSS_GetCharacteristicValue(cy_en_ble_cscs_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CSCSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_cscs_char_index_t charIndex,
                                                                cy_en_ble_cscs_descr_index_t descrIndex, uint8_t attrSize,
                                                                uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CSCSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     cy_en_ble_cscs_char_index_t charIndex, uint8_t attrSize,
                                                     uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CSCSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                   cy_en_ble_cscs_char_index_t charIndex, uint8_t attrSize,
                                                   uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_CSCS_SERVER */

#ifdef CY_BLE_CSCS_CLIENT
/**
 * \addtogroup group_ble_service_api_CSCS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_CSCSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_cscs_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CSCSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_cscs_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_CSCSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_cscs_char_index_t charIndex,
                                                                cy_en_ble_cscs_descr_index_t descrIndex,
                                                                uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CSCSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_cscs_char_index_t charIndex,
                                                                cy_en_ble_cscs_descr_index_t descrIndex);

/** @} */
#endif /* (CY_BLE_CSCS_CLIENT) */


/***************************************
* Private Function Prototypes
***************************************/

/** \cond IGNORE */

#ifdef CY_BLE_CSCS_CLIENT
cy_en_ble_api_result_t Cy_BLE_CSCSC_StoreProfileData(void);
#endif /* (CY_BLE_CSCS_CLIENT) */

/** \endcond */


/***************************************
* External data references
***************************************/
#ifdef CY_BLE_CSCS_CLIENT
extern cy_stc_ble_cscsc_t cy_ble_cscsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_CSCS_CLIENT) */

extern cy_stc_ble_cscs_config_t cy_ble_cscsConfig;
extern cy_stc_ble_cscs_config_t *cy_ble_cscsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_CSCS) */
#endif /* CY_BLE_CSCS_H */

/* [] END OF FILE */
