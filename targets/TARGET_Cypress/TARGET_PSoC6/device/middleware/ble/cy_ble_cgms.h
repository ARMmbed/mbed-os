/***************************************************************************//**
* \file cy_ble_cgms.h
* \version 2.0
*
* \brief
*  This file contains the function prototypes and constants used in the
*  Continuous Glucose Monitoring Service of the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_CGMS_H
#define CY_BLE_CGMS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_CGMS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Data Types
***************************************/

/**
 * \addtogroup group_ble_service_api_CGMS_definitions
 * @{
 */

/** Service Characteristics indexes */
typedef enum
{
    CY_BLE_CGMS_CGMT,           /**< CGM Measurement characteristic index */
    CY_BLE_CGMS_CGFT,           /**< CGM Feature characteristic index */
    CY_BLE_CGMS_CGST,           /**< CGM Status characteristic index */
    CY_BLE_CGMS_SSTM,           /**< CGM Session Start Time characteristic index */
    CY_BLE_CGMS_SRTM,           /**< CGM Session Run Time characteristic index */
    CY_BLE_CGMS_RACP,           /**< Record Access Control Point characteristic index */
    CY_BLE_CGMS_SOCP,           /**< CGM Specific Ops Control Point characteristic index */
    CY_BLE_CGMS_CHAR_COUNT      /**< Total count of CGMS characteristics */
}cy_en_ble_cgms_char_index_t;

/** Service Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_CGMS_CCCD,           /**< Client Characteristic Configuration descriptor index */
    CY_BLE_CGMS_DESCR_COUNT     /**< Total count of CGMS descriptors */
}cy_en_ble_cgms_descr_index_t;

/** Characteristic with descriptors type */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t charHandle;                              /**< Handle of Characteristic value */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_CGMS_DESCR_COUNT];    /**< Handles of Descriptors */
} cy_stc_ble_cgmss_char_t;

/** Structure with CGM Service attribute handles */
typedef struct
{
    /** CGM Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** CGM Service characteristics info array */
    cy_stc_ble_cgmss_char_t      charInfo[CY_BLE_CGMS_CHAR_COUNT];
} cy_stc_ble_cgmss_t;

/** CGM Client Characteristic structure type */
typedef struct
{
    /** Properties for value field */
    uint8_t                      properties;

    /** Handle of Server database attribute value entry */
    cy_ble_gatt_db_attr_handle_t valueHandle;

    /** Characteristics descriptors handles */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_CGMS_DESCR_COUNT];

    /** Characteristic End Handle */
    cy_ble_gatt_db_attr_handle_t endHandle;
}cy_stc_ble_cgmsc_char_t;

/** CGM Service structure type */
typedef struct
{
    /** Characteristics handle + properties array */
    cy_stc_ble_cgmsc_char_t charInfo[CY_BLE_CGMS_CHAR_COUNT];
}cy_stc_ble_cgmsc_t;


/** CGM Service Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;                     /**< Peer device handle */
    cy_en_ble_cgms_char_index_t charIndex;                      /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t     *value;                         /**< Characteristic value */
    cy_en_ble_gatt_err_code_t   gattErrorCode;                  /**< GATT error code for access control */
} cy_stc_ble_cgms_char_value_t;

/** CGM Service Characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t     connHandle;                    /**< Peer device handle */
    cy_en_ble_cgms_char_index_t  charIndex;                     /**< Index of service characteristic */
    cy_en_ble_cgms_descr_index_t descrIndex;                    /**< Index of service characteristic descriptor */
    cy_stc_ble_gatt_value_t      *value;                        /**< Descriptor value */
} cy_stc_ble_cgms_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** Continuous Glucose Monitoring Service GATT DB handles structure */
    const cy_stc_ble_cgmss_t *cgmss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t        callbackFunc;
} cy_stc_ble_cgms_config_t;

/** @} */


/***************************************
* API Constants
***************************************/

#define CY_BLE_CGMS_FLAG_PROCESS         (0x01u)
#define CY_BLE_CGMS_RACP_OPCODE_ABORT    (0x03u)


/***************************************
* Function Prototypes
***************************************/

/**
 * \addtogroup group_ble_service_api_CGMS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_CGMS_Init(cy_stc_ble_cgms_config_t *config);
cy_en_ble_api_result_t Cy_BLE_CGMS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_CGMS_SERVER
/**
 * \addtogroup group_ble_service_api_CGMS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_CGMSS_SetCharacteristicValue(cy_en_ble_cgms_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CGMSS_GetCharacteristicValue(cy_en_ble_cgms_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CGMSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_cgms_char_index_t charIndex,
                                                                cy_en_ble_cgms_descr_index_t descrIndex,
                                                                uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CGMSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_cgms_char_index_t charIndex,
                                                                cy_en_ble_cgms_descr_index_t descrIndex,
                                                                uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CGMSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     cy_en_ble_cgms_char_index_t charIndex, uint8_t attrSize,
                                                     uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CGMSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                   cy_en_ble_cgms_char_index_t charIndex, uint8_t attrSize,
                                                   uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_CGMS_SERVER */

#ifdef CY_BLE_CGMS_CLIENT
/**
 * \addtogroup group_ble_service_api_CGMS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_CGMSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_cgms_char_index_t charIndex,
                                                           uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CGMSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_cgms_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_CGMSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_cgms_char_index_t charIndex,
                                                                cy_en_ble_cgms_descr_index_t descrIndex, uint8_t attrSize,
                                                                uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CGMSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_cgms_char_index_t charIndex,
                                                                cy_en_ble_cgms_descr_index_t descrIndex);

/** @} */
#endif /* CY_BLE_CGMS_CLIENT */


/***************************************
* Private Function Prototypes
***************************************/

/** \cond IGNORE
 * Internal macros
 */
#ifdef CY_BLE_CGMS_SERVER
#define CY_BLE_CGMS_IS_PROCEDURE_IN_PROGRESS \
    (0u != (CY_BLE_CGMS_FLAG_PROCESS & cy_ble_cgmsFlag))
#endif /* CY_BLE_CGMS_SERVER */
/** \endcond */


/***************************************
*  External data references
***************************************/

#ifdef CY_BLE_CGMS_SERVER
extern uint8_t cy_ble_cgmsFlag;
#endif /* CY_BLE_CGMS_SERVER */

#ifdef CY_BLE_CGMS_CLIENT
/* CGM Service server's GATT DB handles structure */
extern cy_stc_ble_cgmsc_t cy_ble_cgmsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_CGMS_CLIENT */

extern cy_stc_ble_cgms_config_t cy_ble_cgmsConfig;
extern cy_stc_ble_cgms_config_t *cy_ble_cgmsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_CGMS) */
#endif /* CY_BLE_CGMS_H */

/* [] END OF FILE */
