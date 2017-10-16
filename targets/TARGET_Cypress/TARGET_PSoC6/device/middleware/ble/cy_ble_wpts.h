/***************************************************************************//**
* \file cy_ble_wpts.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for Wireless Power Transfer
*  Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_WPTS_H
#define CY_BLE_WPTS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_WPTS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \addtogroup group_ble_service_api_WPTS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** WPTS Characteristic indexes */
typedef enum
{
    CY_BLE_WPTS_PRU_CONTROL,                     /**< PRU Control Characteristic index */
    CY_BLE_WPTS_PTU_STATIC_PAR,                  /**< PTU Static Parameter Characteristic index */
    CY_BLE_WPTS_PRU_ALERT,                       /**< PRU Alert Characteristic index */
    CY_BLE_WPTS_PRU_STATIC_PAR,                  /**< PRU Static Parameter Characteristic index */
    CY_BLE_WPTS_PRU_DYNAMIC_PAR,                 /**< PRU Dynamic Parameter Characteristic index */
    CY_BLE_WPTS_CHAR_COUNT                       /**< Total count of WPTS Characteristics */
}cy_en_ble_wpts_char_index_t;

/** WPTS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_WPTS_CCCD,                            /**< Client Characteristic Configuration Descriptor index */
    CY_BLE_WPTS_DESCR_COUNT                      /**< Total count of Descriptors */
}cy_en_ble_wpts_descr_index_t;

/** Characteristic with descriptors */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t charHandle;                           /**< Handle of characteristic value */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_WPTS_DESCR_COUNT]; /**< Handle of descriptor */
} cy_stc_ble_wptss_char_t;

/** WPTS Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;         /**< Peer device handle */
    cy_en_ble_wpts_char_index_t charIndex;          /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t     *value;             /**< Characteristic value */
} cy_stc_ble_wpts_char_value_t;

/** WPTS Characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t     connHandle;        /**< Peer device handle */
    cy_en_ble_wpts_char_index_t  charIndex;         /**< Index of service characteristic */
    cy_en_ble_wpts_descr_index_t descrIndex;        /**< Index of descriptor */
    cy_stc_ble_gatt_value_t      *value;            /**< Characteristic value */
} cy_stc_ble_wpts_descr_value_t;

/** WPTS Server Characteristic structure type */
typedef struct
{
    /** Wireless Power Transfer Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Wireless Power Transfer Characteristic handles */
    cy_stc_ble_wptss_char_t      charInfo[CY_BLE_WPTS_CHAR_COUNT];
} cy_stc_ble_wptss_t;

/** Characteristic information structure */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_WPTS_DESCR_COUNT]; /**< Handles of descriptors */
    cy_ble_gatt_db_attr_handle_t valueHandle;                          /**< Handle of characteristic value */
    cy_ble_gatt_db_attr_handle_t endHandle;                            /**< End handle of a characteristic */
    uint8_t                      properties;                           /**< Properties for value field */
} cy_stc_ble_wptsc_char_t;

/** WPTS Characteristic with descriptors */
typedef struct
{
    /** Wireless Power Transfer Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;
    /** Wireless Power Transfer Service characteristics info structure */
    cy_stc_ble_wptsc_char_t      charInfo[CY_BLE_WPTS_CHAR_COUNT];
} cy_stc_ble_wptsc_t;

/** Service configuration structure */
typedef struct
{
    /** Wireless Power Transfer Service GATT DB handles structure */
    const cy_stc_ble_wptss_t *wptss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t        callbackFunc;
} cy_stc_ble_wpts_config_t;

/** @} */

/***************************************
* API Constants
***************************************/

/* WPT service handle offsets */
#define CY_BLE_WPTS_WPT_SERVICE_OFFSET                   (0x00u)
#define CY_BLE_WPTS_PRU_CONTROL_CHAR_DECL_OFFSET         (0x01u)
#define CY_BLE_WPTS_PRU_CONTROL_CHAR_VALUE_OFFSET        (0x02u)
#define CY_BLE_WPTS_PTU_STATIC_PAR_CHAR_DECL_OFFSET      (0x03u)
#define CY_BLE_WPTS_PTU_STATIC_PAR_CHAR_VALUE_OFFSET     (0x04u)
#define CY_BLE_WPTS_PRU_ALERT_PAR_CHAR_DECL_OFFSET       (0x05u)
#define CY_BLE_WPTS_PRU_ALERT_PAR_CHAR_VALUE_OFFSET      (0x06u)
#define CY_BLE_WPTS_PRU_ALERT_PAR_CHAR_CCCD_OFFSET       (0x07u)
#define CY_BLE_WPTS_PRU_STATIC_PAR_CHAR_DECL_OFFSET      (0x08u)
#define CY_BLE_WPTS_PRU_STATIC_PAR_CHAR_VALUE_OFFSET     (0x09u)
#define CY_BLE_WPTS_PRU_DYNAMIC_PAR_CHAR_DECL_OFFSET     (0x0Au)
#define CY_BLE_WPTS_PRU_DYNAMIC_PAR_CHAR_VALUE_OFFSET    (0x0Bu)


/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_WPTS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_WPTS_Init(cy_stc_ble_wpts_config_t *config);
cy_en_ble_api_result_t Cy_BLE_WPTS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_WPTS_SERVER
/**
 * \addtogroup group_ble_service_api_WPTS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_WPTSS_SetCharacteristicValue(cy_en_ble_wpts_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_WPTSS_GetCharacteristicValue(cy_en_ble_wpts_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_WPTSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_wpts_char_index_t charIndex,
                                                                cy_en_ble_wpts_descr_index_t descrIndex,
                                                                uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_WPTSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_wpts_char_index_t charIndex,
                                                                cy_en_ble_wpts_descr_index_t descrIndex,
                                                                uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_WPTSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     cy_en_ble_wpts_char_index_t charIndex, uint8_t attrSize,
                                                     uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_WPTSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                   cy_en_ble_wpts_char_index_t charIndex, uint8_t attrSize,
                                                   uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_WPTS_SERVER */

#ifdef CY_BLE_WPTS_CLIENT
/**
 * \addtogroup group_ble_service_api_WPTS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_WPTSC_Discovery(cy_ble_gatt_db_attr_handle_t servHandle, 
                                              cy_stc_ble_conn_handle_t connHandle);

cy_en_ble_api_result_t Cy_BLE_WPTSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_wpts_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t * attrValue);

cy_en_ble_api_result_t Cy_BLE_WPTSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_wpts_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_WPTSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_wpts_char_index_t charIndex,
                                                                cy_en_ble_wpts_descr_index_t descrIndex,
                                                                uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_WPTSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_wpts_char_index_t charIndex,
                                                                cy_en_ble_wpts_descr_index_t descrIndex);

/** @} */
#endif /* CY_BLE_WPTS_CLIENT */


/* ##Macro Functions */
#ifdef CY_BLE_WPTS_CLIENT

#define Cy_BLE_WPTSC_GetCharacteristicValueHandle(connHandle, charIndex) \
    (((charIndex) >= CY_BLE_WPTS_CHAR_COUNT) ?                           \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                             \
     cy_ble_wptsc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfo[charIndex].valueHandle)

#define Cy_BLE_WPTSC_GetCharacteristicDescriptorHandle(connHandle, charIndex, descrIndex)     \
    ((((charIndex) >= CY_BLE_WPTS_CHAR_COUNT) || ((descrIndex) >= CY_BLE_WPTS_DESCR_COUNT)) ? \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                                                  \
     cy_ble_wptsc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfo[charIndex].descrHandle[descrIndex])

#endif /* (CY_BLE_WPTS_CLIENT) */


/***************************************
* External data references
***************************************/
#ifdef CY_BLE_WPTS_CLIENT
extern cy_stc_ble_wptsc_t cy_ble_wptsc[CY_BLE_CONFIG_GATTC_COUNT];
extern const cy_stc_ble_uuid128_t cy_ble_wptscCharUuid128[CY_BLE_WPTS_CHAR_COUNT];
#endif /* CY_BLE_WPTS_CLIENT */

extern cy_stc_ble_wpts_config_t cy_ble_wptsConfig;
extern cy_stc_ble_wpts_config_t *cy_ble_wptsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_WPTS) */
#endif /* CY_BLE_WPTS_H */

/* [] END OF FILE */
