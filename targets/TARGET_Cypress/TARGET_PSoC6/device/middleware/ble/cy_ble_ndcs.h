/***************************************************************************//**
* \file cy_ble_ndcs.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for Next DST Change
*  Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_NDCS_H
#define CY_BLE_NDCS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_NDCS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "cy_ble_gatt.h"

/**
 * \addtogroup group_ble_service_api_NDCS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** Characteristic indexes */
typedef enum
{
    CY_BLE_NDCS_TIME_WITH_DST,                           /**< Time with DST Characteristic index */
    CY_BLE_NDCS_CHAR_COUNT                               /**< Total count of NDCS Characteristics */
} cy_en_ble_ndcs_char_index_t;

/** Next DST Change Service Characteristic Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;              /**< Peer device handle */
    cy_en_ble_ndcs_char_index_t charIndex;               /**< Index of Next DST Change Service Characteristic */
    cy_stc_ble_gatt_value_t     *value;                  /**< Characteristic value */
} cy_stc_ble_ndcs_char_value_t;

/** Structure with Device Information Service attribute handles */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t serviceHandle;          /**< Handle of the Next DST Change Service */
    cy_ble_gatt_db_attr_handle_t timeWithDst;            /**< Handle of the Time with DST Characteristic */
} cy_stc_ble_ndcss_t;

/** Structure with discovered attributes information of Next DST Change Service */
typedef struct
{
    /** Characteristic handle and properties */
    cy_stc_ble_srvr_char_info_t charInfo[CY_BLE_NDCS_CHAR_COUNT];
} cy_stc_ble_ndcsc_t;

/** Service configuration structure */
typedef struct
{
    /** Next DST Change Service GATT DB handles structure */
    const cy_stc_ble_ndcss_t *ndcss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t        callbackFunc;
} cy_stc_ble_ndcs_config_t;

/** @} */


/***************************************
* Function Prototypes
***************************************/

/**
 * \addtogroup group_ble_service_api_NDCS_server_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_NDCS_Init(cy_stc_ble_ndcs_config_t *config);
cy_en_ble_api_result_t Cy_BLE_NDCS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);

/** @} */

#if defined(CY_BLE_NDCS_SERVER)
/**
 * \addtogroup group_ble_service_api_NDCS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_NDCSS_SetCharacteristicValue(cy_en_ble_ndcs_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_NDCSS_GetCharacteristicValue(cy_en_ble_ndcs_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_NDCS_SERVER */

#if defined(CY_BLE_NDCS_CLIENT)
/**
 * \addtogroup group_ble_service_api_NDCS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_NDCSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_ndcs_char_index_t charIndex);

/** @} */
#endif /* (CY_BLE_NDCS_CLIENT) */


/***************************************
* External data references
***************************************/
#if defined(CY_BLE_NDCS_CLIENT)
extern cy_stc_ble_ndcsc_t cy_ble_ndcsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_NDCS_CLIENT) */

extern cy_stc_ble_ndcs_config_t cy_ble_ndcsConfig;
extern cy_stc_ble_ndcs_config_t *cy_ble_ndcsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_NDCS) */
#endif /* CY_BLE_NDCS_H */


/* [] END OF FILE */
