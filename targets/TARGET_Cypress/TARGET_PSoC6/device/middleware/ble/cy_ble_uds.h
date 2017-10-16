/***************************************************************************//**
* \file cy_ble_uds.h
* \version 2.0
*
* \brief
*  This file contains the function prototypes and constants used in
*  the User Data Service of the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_UDS_H
#define CY_BLE_UDS_H
#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_UDS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CY_BLE_UDS_FLAG_PROCESS       (0x01u)
#define CY_BLE_UDS_LONG_CHAR_COUNT    (3u) /* Count of long UDS characteristics */

/**
 * \addtogroup group_ble_service_api_UDS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** UDS Service Characteristics indexes */
typedef enum
{
    CY_BLE_UDS_FNM,          /**< First Name characteristic index */
    CY_BLE_UDS_LNM,          /**< Last Name characteristic index */
    CY_BLE_UDS_EML,          /**< Email Address characteristic index */
    CY_BLE_UDS_AGE,          /**< Age characteristic index */
    CY_BLE_UDS_DOB,          /**< Date of Birth characteristic index */
    CY_BLE_UDS_GND,          /**< Gender characteristic index */
    CY_BLE_UDS_WGT,          /**< Weight characteristic index */
    CY_BLE_UDS_HGT,          /**< Height characteristic index */
    CY_BLE_UDS_VO2,          /**< VO2 Max characteristic index */
    CY_BLE_UDS_HRM,          /**< Heart Rate Max characteristic index */
    CY_BLE_UDS_RHR,          /**< Resting Heart Rate characteristic index */
    CY_BLE_UDS_MRH,          /**< Maximum Recommended Heart Rate characteristic index */
    CY_BLE_UDS_AET,          /**< Aerobic Threshold characteristic index */
    CY_BLE_UDS_ANT,          /**< Anaerobic Threshold characteristic index */
    CY_BLE_UDS_STP,          /**< Sport Type for Aerobic and Anaerobic Thresholds characteristic index */
    CY_BLE_UDS_DTA,          /**< Date of Threshold Assessment characteristic index */
    CY_BLE_UDS_WCC,          /**< Waist Circumference characteristic index */
    CY_BLE_UDS_HCC,          /**< Hip Circumference characteristic index */
    CY_BLE_UDS_FBL,          /**< Fat Burn Heart Rate Lower Limit characteristic index */
    CY_BLE_UDS_FBU,          /**< Fat Burn Heart Rate Upper Limit characteristic index */
    CY_BLE_UDS_AEL,          /**< Aerobic Heart Rate Lower Limit characteristic index */
    CY_BLE_UDS_AEU,          /**< Aerobic Heart Rate Upper Limit characteristic index */
    CY_BLE_UDS_ANL,          /**< Anaerobic Heart Rate Lower Limit characteristic index */
    CY_BLE_UDS_ANU,          /**< Anaerobic Heart Rate Upper Limit characteristic index */
    CY_BLE_UDS_5ZL,          /**< Five Zone Heart Rate Limits characteristic index */
    CY_BLE_UDS_3ZL,          /**< Three Zone Heart Rate Limits characteristic index */
    CY_BLE_UDS_2ZL,          /**< Two Zone Heart Rate Limit characteristic index */
    CY_BLE_UDS_DCI,          /**< Database Change Increment characteristic index */
    CY_BLE_UDS_UIX,          /**< User Index characteristic index */
    CY_BLE_UDS_UCP,          /**< User Control Point characteristic index */
    CY_BLE_UDS_LNG,          /**< Language characteristic index */
    CY_BLE_UDS_CHAR_COUNT    /**< Total count of UDS characteristics */
}cy_en_ble_uds_char_index_t;

/** UDS Service Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_UDS_CCCD,           /**< Client Characteristic Configuration descriptor index */
    CY_BLE_UDS_DESCR_COUNT     /**< Total count of UDS descriptors */
}cy_en_ble_uds_descr_index_t;

/** User Data Server Characteristic structure type */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t charHandle;                           /**< Handle of characteristic value */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_UDS_DESCR_COUNT];  /**< Handle of descriptor */
}cy_stc_ble_udss_char_t;

/** Structure with User Data Service attribute handles */
typedef struct
{
    /** User Data Service handle*/
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** User Data Service characteristics info array */
    cy_stc_ble_udss_char_t       charInfo[CY_BLE_UDS_CHAR_COUNT];
} cy_stc_ble_udss_t;

/** User Data Client Characteristic structure type */
typedef struct
{
    /** Properties for value field */
    uint8_t                      properties;

    /** Handle of server database attribute value entry */
    cy_ble_gatt_db_attr_handle_t valueHandle;

    /** User Data client char. descriptor handle */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_UDS_DESCR_COUNT];

    /** Characteristic End Handle */
    cy_ble_gatt_db_attr_handle_t endHandle;
}cy_stc_ble_udsc_char_t;

/** Structure with discovered attributes information of User Data Service */
typedef struct
{
    cy_stc_ble_udsc_char_t charInfo[CY_BLE_UDS_CHAR_COUNT];   /**< Characteristics handle + properties array */
}cy_stc_ble_udsc_t;

/** UDS Characteristic Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;                      /**< Peer device handle */
    cy_en_ble_uds_char_index_t charIndex;                       /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t    *value;                          /**< Characteristic value */
    cy_en_ble_gatt_err_code_t  gattErrorCode;                   /**< GATT error code for access control */
} cy_stc_ble_uds_char_value_t;

/** UDS Characteristic Descriptor Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;                     /**< Peer device handle */
    cy_en_ble_uds_char_index_t  charIndex;                      /**< Index of service characteristic */
    cy_en_ble_uds_descr_index_t descrIndex;                     /**< Index of service characteristic descriptor */
    cy_stc_ble_gatt_value_t     *value;                         /**< Descriptor value */
} cy_stc_ble_uds_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** User Data Service GATT DB handles structure */
    const cy_stc_ble_udss_t *udss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_uds_config_t;

/** @} */


/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_UDS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_UDS_Init(cy_stc_ble_uds_config_t *config);
cy_en_ble_api_result_t Cy_BLE_UDS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_UDS_SERVER
/**
 * \addtogroup group_ble_service_api_UDS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_UDSS_SetCharacteristicValue(cy_en_ble_uds_char_index_t charIndex,
                                                          uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_UDSS_GetCharacteristicValue(cy_en_ble_uds_char_index_t charIndex,
                                                          uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_UDSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_uds_char_index_t charIndex,
                                                               cy_en_ble_uds_descr_index_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_UDSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_uds_char_index_t charIndex, uint8_t attrSize,
                                                    uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_UDSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_uds_char_index_t charIndex, uint8_t attrSize,
                                                  uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_UDS_SERVER */

#ifdef CY_BLE_UDS_CLIENT
/**
 * \addtogroup group_ble_service_api_UDS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_UDSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_uds_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_UDSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_uds_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_UDSC_GetLongCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                              cy_en_ble_uds_char_index_t charIndex, uint16_t attrSize,
                                                              uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_UDSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_uds_char_index_t charIndex,
                                                               cy_en_ble_uds_descr_index_t descrIndex, uint8_t attrSize,
                                                               uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_UDSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_uds_char_index_t charIndex,
                                                               cy_en_ble_uds_descr_index_t descrIndex);

/** @} */
#endif /* CY_BLE_UDS_CLIENT */


/***************************************
* Private Function Prototypes
***************************************/

/** \cond IGNORE */

/* Internal macros */
#ifdef CY_BLE_UDS_SERVER
#define CY_BLE_UDS_IS_PROCEDURE_IN_PROGRESS    (0u != (CY_BLE_UDS_FLAG_PROCESS & cy_ble_udsFlag))
#endif /* CY_BLE_UDS_SERVER */
/** \endcond */


/***************************************
*  External data references
***************************************/

#ifdef CY_BLE_UDS_SERVER
extern uint8_t cy_ble_udsFlag;
#endif /* CY_BLE_UDSC_SERVER */

#ifdef CY_BLE_UDS_CLIENT
/* User Data Service server's GATT DB handles structure */
extern cy_stc_ble_udsc_t cy_ble_udsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_UDS_CLIENT */

extern cy_stc_ble_uds_config_t cy_ble_udsConfig;
extern cy_stc_ble_uds_config_t *cy_ble_udsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_UDS) */
#endif /* CY_BLE_UDS_H */

/* [] END OF FILE */
