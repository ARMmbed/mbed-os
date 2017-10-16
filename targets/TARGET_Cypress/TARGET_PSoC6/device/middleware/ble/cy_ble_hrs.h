/***************************************************************************//**
* \file cy_ble_hrs.h
* \version 2.0
*
* \brief
*  This file contains the function prototypes and constants used in
*  the Heart Rate Service of the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_HRS_H
#define CY_BLE_HRS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_HRS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Definitions for Body Sensor Location characteristic */
#define CY_BLE_HRS_BSL_CHAR_LEN             (1u)

/* Definitions for ControlPoint characteristic */
#define CY_BLE_HRS_CPT_CHAR_LEN             (1u)
#define CY_BLE_HRS_RESET_ENERGY_EXPENDED    (0x01u)


/**
 * \addtogroup group_ble_service_api_HRS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** HRS Characteristics indexes */
typedef enum
{
    CY_BLE_HRS_HRM,          /**< Heart Rate Measurement characteristic index */
    CY_BLE_HRS_BSL,          /**< Body Sensor Location characteristic index */
    CY_BLE_HRS_CPT,          /**< Control Point characteristic index */
    CY_BLE_HRS_CHAR_COUNT    /**< Total count of HRS characteristics */
}cy_en_ble_hrs_char_index_t;

/** HRS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_HRS_HRM_CCCD,    /**< Heart Rate Measurement client char. config. descriptor index */
    CY_BLE_HRS_DESCR_COUNT  /**< Total count of HRS HRM descriptors */
}cy_en_ble_hrs_descr_index_t;

/** Structure with Heart Rate Service attribute handles */
typedef struct
{
    /** Heart Rate Service handle*/
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Heart Rate Service characteristics handles and properties array */
    cy_ble_gatt_db_attr_handle_t charHandle[CY_BLE_HRS_CHAR_COUNT];

    /** Heart Rate Measurement client char. config. descriptor Handle */
    cy_ble_gatt_db_attr_handle_t hrmCccdHandle;
} cy_stc_ble_hrss_t;

/** Structure with discovered attributes information of Heart Rate Service */
typedef struct
{
    /** Heart Rate Service characteristics handles and properties array */
    cy_stc_ble_srvr_char_info_t  charInfo[CY_BLE_HRS_CHAR_COUNT];

    /** Heart Rate Measurement client char. config. descriptor Handle */
    cy_ble_gatt_db_attr_handle_t hrmCccdHandle;
}cy_stc_ble_hrsc_t;

/** HRS Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;                      /**< Peer device handle */
    cy_en_ble_hrs_char_index_t charIndex;                       /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t    *value;                          /**< Characteristic value */
} cy_stc_ble_hrs_char_value_t;

/** HRS Characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;                     /**< Peer device handle */
    cy_en_ble_hrs_char_index_t  charIndex;                      /**< Index of service characteristic */
    cy_en_ble_hrs_descr_index_t descrIndex;                     /**< Index of service characteristic descriptor */
    cy_stc_ble_gatt_value_t     *value;                         /**< Descriptor value */
} cy_stc_ble_hrs_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** Heart Rate Service GATT DB handles structure */
    const cy_stc_ble_hrss_t *hrss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_hrs_config_t;
/** @} */


/***************************************
* Function Prototypes
***************************************/

/**
 * \addtogroup group_ble_service_api_HRS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_HRS_Init(cy_stc_ble_hrs_config_t *config);
cy_en_ble_api_result_t Cy_BLE_HRS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_HRS_SERVER
/**
 * \addtogroup group_ble_service_api_HRS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_HRSS_SetCharacteristicValue(cy_en_ble_hrs_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HRSS_GetCharacteristicValue(cy_en_ble_hrs_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HRSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hrs_char_index_t charIndex,
                                                               cy_en_ble_hrs_descr_index_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HRSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_hrs_char_index_t charIndex, uint8_t attrSize,
                                                    uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_HRS_SERVER */

#ifdef CY_BLE_HRS_CLIENT
/**
 * \addtogroup group_ble_service_api_HRS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_HRSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_hrs_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HRSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_hrs_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_HRSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hrs_char_index_t charIndex,
                                                               cy_en_ble_hrs_descr_index_t descrIndex, uint8_t attrSize,
                                                               uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HRSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hrs_char_index_t charIndex,
                                                               cy_en_ble_hrs_descr_index_t descrIndex);

/** @} */
#endif /* CY_BLE_HRS_CLIENT */


/***************************************
* Private Function Prototypes
***************************************/

/** \cond IGNORE
 * Internal macros
 */
#ifdef CY_BLE_HRS_SERVER
#define CY_BLE_HRS_IS_BSL_SUPPORTED \
    (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_hrsConfigPtr->hrss->charHandle[CY_BLE_HRS_BSL])
#endif /* CY_BLE_HRS_SERVER */
/** \endcond */


/***************************************
* External data references
***************************************/
#ifdef CY_BLE_HRS_CLIENT
/* Heart Rate Service server's GATT DB handles structure */
extern cy_stc_ble_hrsc_t cy_ble_hrsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_HRS_CLIENT */

extern cy_stc_ble_hrs_config_t cy_ble_hrsConfig;
extern cy_stc_ble_hrs_config_t *cy_ble_hrsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_HRS) */
#endif /* CY_BLE_HRS_H */


/* [] END OF FILE */
