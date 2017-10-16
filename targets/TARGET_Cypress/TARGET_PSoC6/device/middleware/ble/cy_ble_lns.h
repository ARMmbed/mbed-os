/***************************************************************************//**
* \file cy_ble_lns.h
* \version 2.0
*
* \brief
*  This file contains the function prototypes and constants used in
*  the Location And Navigation Service of the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_LNS_H
#define CY_BLE_LNS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_LNS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CY_BLE_LNS_FLAG_PROCESS    (0x01u)

/**
 * \addtogroup group_ble_service_api_LNS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** LNS Service Characteristics indexes */
typedef enum
{
    CY_BLE_LNS_FT,           /**< Location and Navigation Feature characteristic index */
    CY_BLE_LNS_LS,           /**< Location and Speed characteristic index */
    CY_BLE_LNS_PQ,           /**< Position Quality characteristic index */
    CY_BLE_LNS_CP,           /**< Location and Navigation Control Point characteristic index */
    CY_BLE_LNS_NV,           /**< Navigation characteristic index */
    CY_BLE_LNS_CHAR_COUNT    /**< Total count of LNS characteristics */
}cy_en_ble_lns_char_index_t;

/** LNS Service Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_LNS_CCCD,           /**< Client Characteristic Configuration descriptor index */
    CY_BLE_LNS_DESCR_COUNT     /**< Total count of LNS descriptors */
}cy_en_ble_lns_descr_index_t;

/** Location and Navigation Server Characteristic structure type */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t charHandle;                           /**< Handle of characteristic value */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_LNS_DESCR_COUNT];  /**< Handle of descriptor */
}cy_stc_ble_lnss_char_t;

/** Structure with Location and Navigation Service attribute handles */
typedef struct
{
    /** Location and Navigation Service handle*/
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Location and Navigation Service characteristics info array */
    cy_stc_ble_lnss_char_t       charInfo[CY_BLE_LNS_CHAR_COUNT];
} cy_stc_ble_lnss_t;

/** Location and Navigation Client Characteristic structure type */
typedef struct
{
    /** Properties for value field */
    uint8_t                      properties;

    /** Handle of server database attribute value entry */
    cy_ble_gatt_db_attr_handle_t valueHandle;

    /** Location and Navigation client char. descriptor handle */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_LNS_DESCR_COUNT];

    /** Characteristic End Handle */
    cy_ble_gatt_db_attr_handle_t endHandle;
}cy_stc_ble_lnsc_char_t;

/** Structure with discovered attributes information of Location and Navigation Service */
typedef struct
{
    cy_stc_ble_lnsc_char_t charInfo[CY_BLE_LNS_CHAR_COUNT];   /**< Characteristics handle + properties array */
}cy_stc_ble_lnsc_t;

/** LNS Characteristic Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;                      /**< Peer device handle */
    cy_en_ble_lns_char_index_t charIndex;                       /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t    *value;                          /**< Characteristic value */
} cy_stc_ble_lns_char_value_t;

/** LNS Characteristic Descriptor Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;                     /**< Peer device handle */
    cy_en_ble_lns_char_index_t  charIndex;                      /**< Index of service characteristic */
    cy_en_ble_lns_descr_index_t descrIndex;                     /**< Index of service characteristic descriptor */
    cy_stc_ble_gatt_value_t     *value;                         /**< Descriptor value */
} cy_stc_ble_lns_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** Location and Navigation GATT DB handles structure */
    const cy_stc_ble_lnss_t *lnss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_lns_config_t;

/** @}
** Service configuration structure
*/


/***************************************
* Function Prototypes
***************************************/

/**
 * \addtogroup group_ble_service_api_LNS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_LNS_Init(cy_stc_ble_lns_config_t *config);
cy_en_ble_api_result_t Cy_BLE_LNS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_LNS_SERVER
/**
 * \addtogroup group_ble_service_api_LNS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_LNSS_SetCharacteristicValue(cy_en_ble_lns_char_index_t charIndex,
                                                          uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_LNSS_GetCharacteristicValue(cy_en_ble_lns_char_index_t charIndex,
                                                          uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_LNSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_lns_char_index_t charIndex,
                                                               cy_en_ble_lns_descr_index_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_LNSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_lns_char_index_t charIndex, uint8_t attrSize,
                                                    uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_LNSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_lns_char_index_t charIndex, uint8_t attrSize,
                                                  uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_LNS_SERVER */

#ifdef CY_BLE_LNS_CLIENT
/**
 * \addtogroup group_ble_service_api_LNS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_LNSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_lns_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_LNSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_lns_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_LNSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_lns_char_index_t charIndex,
                                                               cy_en_ble_lns_descr_index_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_LNSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_lns_char_index_t charIndex,
                                                               cy_en_ble_lns_descr_index_t descrIndex);
/** @} */
#endif /* CY_BLE_LNS_CLIENT */


/* Internal macros */
#ifdef CY_BLE_LNS_SERVER
#define CY_BLE_LNS_IS_PQ_SUPPORTED \
    (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_lnsConfigPtr->lnss->charInfo[CY_BLE_LNS_PQ].charHandle)

#define CY_BLE_LNS_IS_CP_SUPPORTED \
    (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_lnsConfigPtr->lnss->charInfo[CY_BLE_LNS_CP].charHandle)

#define CY_BLE_LNS_IS_NV_SUPPORTED \
    (CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE != cy_ble_lnsConfigPtr->lnss->charInfo[CY_BLE_LNS_NV].charHandle)

#define CY_BLE_LNS_IS_PROCEDURE_IN_PROGRESS \
    (0u != (CY_BLE_LNS_FLAG_PROCESS & cy_ble_lnsFlag))
#endif /* CY_BLE_LNS_SERVER */


#ifdef CY_BLE_LNS_CLIENT
#define Cy_BLE_LNSC_GetCharacteristicValueHandle(connHandle, charIndex) \
    (((charIndex) >= CY_BLE_LNS_CHAR_COUNT) ?                           \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                            \
     cy_ble_lnsc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfo[charIndex].valueHandle)

#define Cy_BLE_LNSC_GetCharacteristicDescriptorHandle(connHandle, charIndex, descrIndex)    \
    ((((charIndex) >= CY_BLE_LNS_CHAR_COUNT) || ((descrIndex) >= CY_BLE_LNS_DESCR_COUNT)) ? \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                                                \
     cy_ble_lnsc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfo[charIndex].descrHandle[descrIndex])
#endif /* (CY_BLE_LNS_CLIENT) */

/** \endcond */


/***************************************
*  External data references
***************************************/
#ifdef CY_BLE_LNS_SERVER
/* Location and Navigation Service GATT DB handles structure */
extern uint8_t cy_ble_lnsFlag;
#endif /* CY_BLE_LNSC_SERVER */

#ifdef CY_BLE_LNS_CLIENT
/* Location and Navigation Service server's GATT DB handles structure */
extern cy_stc_ble_lnsc_t cy_ble_lnsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_LNS_CLIENT */

extern cy_stc_ble_lns_config_t cy_ble_lnsConfig;
extern cy_stc_ble_lns_config_t *cy_ble_lnsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_LNS) */
#endif /* CY_BLE_LNS_H */

/* [] END OF FILE */
