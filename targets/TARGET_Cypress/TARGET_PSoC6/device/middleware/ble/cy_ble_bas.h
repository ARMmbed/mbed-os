/***************************************************************************//**
* \file cy_ble_bas.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for the Battery Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_BAS_H
#define CY_BLE_BAS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_BAS))

/* The C binding of definitions if building with the C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Conditional Compilation Parameters
***************************************/

#ifdef CY_BLE_BAS_SERVER

/* The maximum supported Battery Services */
    #define CY_BLE_BASS_SERVICE_COUNT    (CY_BLE_CONFIG_BASS_SERVICE_COUNT)

#endif /* CY_BLE_BAS_SERVER */

#ifdef CY_BLE_BAS_CLIENT
/* The maximum supported Battery Services */
    #define CY_BLE_BASC_SERVICE_COUNT    (CY_BLE_CONFIG_BASC_SERVICE_COUNT)

#endif /* (CY_BLE_BAS_CLIENT) */


/***************************************
* Data Types
***************************************/

/**
 * \addtogroup group_ble_service_api_BAS_definitions
 * @{
 */

/** BAS Characteristic indexes */
typedef enum
{
    CY_BLE_BAS_BATTERY_LEVEL,                            /**< Battery Level characteristic index */
    CY_BLE_BAS_CHAR_COUNT                                /**< Total count of characteristics */
}cy_en_ble_bas_char_index_t;

/** BAS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_BAS_BATTERY_LEVEL_CCCD,                       /**< Client Characteristic Configuration descriptor index */
    CY_BLE_BAS_BATTERY_LEVEL_CPFD,                       /**< Characteristic Presentation Format descriptor index */
    CY_BLE_BAS_DESCR_COUNT                               /**< Total count of descriptors */
}cy_en_ble_bas_descr_index_t;

/** Structure with Battery Service attribute handles */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t serviceHandle;          /**< Battery Service handle */
    cy_ble_gatt_db_attr_handle_t batteryLevelHandle;     /**< Battery Level characteristic handle */
    cy_ble_gatt_db_attr_handle_t cpfdHandle;             /**< Characteristic Presentation Format Descriptor handle */
    cy_ble_gatt_db_attr_handle_t cccdHandle;             /**< Client Characteristic Configuration descriptor handle */
} cy_stc_ble_bass_t;

/** Structure with discovered attributes information of Battery Service */
typedef struct
{
    cy_stc_ble_conn_handle_t     connHandle;             /**< Peer device handle */
    cy_stc_ble_srvr_char_info_t  batteryLevel;           /**< Battery Level characteristic info */
    cy_ble_gatt_db_attr_handle_t cpfdHandle;             /**< Characteristic Presentation Format descriptor handle */
    cy_ble_gatt_db_attr_handle_t cccdHandle;             /**< Client Characteristic Configuration descriptor handle */
    cy_ble_gatt_db_attr_handle_t rrdHandle;              /**< Report Reference descriptor handle */
} cy_stc_ble_basc_t;

/** Battery Service Characteristic Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;               /**< Peer device handle */
    uint8_t                    serviceIndex;             /**< Service instance */
    cy_en_ble_bas_char_index_t charIndex;                /**< Index of a service characteristic */
    cy_stc_ble_gatt_value_t    *value;                   /**< Characteristic value */
} cy_stc_ble_bas_char_value_t;

/** Battery Service Characteristic Descriptor Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;              /**< Peer device handle */
    uint8_t                     serviceIndex;            /**< Service instance */
    cy_en_ble_bas_char_index_t  charIndex;               /**< Index of service characteristic */
    cy_en_ble_bas_descr_index_t descrIndex;              /**< Index of service characteristic descriptor */
    cy_stc_ble_gatt_value_t     *value;                  /**< Descriptor value */
} cy_stc_ble_bas_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** Service GATT DB handles structure */
    const cy_stc_ble_bass_t *bass;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_bas_config_t;

/** @} */


/***************************************
* API Constants
***************************************/

/* Battery Level characteristic length */
#define CY_BLE_BAS_BATTERY_LEVEL_LEN          (0x01u)

/* Maximum Battery Level value */
#define CY_BLE_BAS_MAX_BATTERY_LEVEL_VALUE    (100u)


/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_BAS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_BAS_Init(cy_stc_ble_bas_config_t *config);
cy_en_ble_api_result_t Cy_BLE_BAS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */



#ifdef CY_BLE_BAS_SERVER
/**
 * \addtogroup group_ble_service_api_BAS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_BASS_SetCharacteristicValue(uint8_t serviceIndex, cy_en_ble_bas_char_index_t charIndex,
                                                          uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BASS_GetCharacteristicValue(uint8_t serviceIndex, cy_en_ble_bas_char_index_t charIndex,
                                                          uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BASS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle, uint8_t serviceIndex,
                                                               cy_en_ble_bas_char_index_t charIndex,
                                                               cy_en_ble_bas_descr_index_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BASS_SendNotification(cy_stc_ble_conn_handle_t connHandle, uint8_t serviceIndex,
                                                    cy_en_ble_bas_char_index_t charIndex, uint8_t attrSize,
                                                    uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_BAS_SERVER */

#ifdef CY_BLE_BAS_CLIENT
/**
 * \addtogroup group_ble_service_api_BAS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_BASC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle, uint8_t serviceIndex,
                                                          cy_en_ble_bas_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_BASC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle, uint8_t serviceIndex,
                                                               cy_en_ble_bas_char_index_t charIndex,
                                                               cy_en_ble_bas_descr_index_t descrIndex, uint8_t attrSize,
                                                               uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BASC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle, uint8_t serviceIndex,
                                                               cy_en_ble_bas_char_index_t charIndex,
                                                               cy_en_ble_bas_descr_index_t descrIndex);

/** @} */
#endif /* (CY_BLE_BAS_CLIENT) */


/***************************************
* Macro Functions
***************************************/

#ifdef CY_BLE_BAS_CLIENT
#define Cy_BLE_BASC_GetCharacteristicValueHandle(connHandle, serviceIndex, charIndex)              \
    ((((serviceIndex) >= CY_BLE_BASC_SERVICE_COUNT) || ((charIndex) > CY_BLE_BAS_BATTERY_LEVEL)) ? \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                                                       \
     cy_ble_basc[Cy_BLE_GetDiscoveryIdx(connHandle)][serviceIndex].batteryLevel.valueHandle)

#define Cy_BLE_BASC_GetCharacteristicDescriptorHandle(connHandle, serviceIndex, charIndex, descrIndex) \
    ((((serviceIndex) >= CY_BLE_BASC_SERVICE_COUNT) || ((charIndex) > CY_BLE_BAS_BATTERY_LEVEL) ||     \
      ((descrIndex) >= CY_BLE_BAS_DESCR_COUNT)) ? CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :              \
     ((descrIndex) == CY_BLE_BAS_BATTERY_LEVEL_CCCD) ?                                                 \
     cy_ble_basc[Cy_BLE_GetDiscoveryIdx(connHandle)][serviceIndex].cccdHandle :                        \
     cy_ble_basc[Cy_BLE_GetDiscoveryIdx(connHandle)][serviceIndex].cpfdHandle)

#endif /* (CY_BLE_BAS_CLIENT) */


/***************************************
* External data references
***************************************/
#ifdef CY_BLE_BAS_CLIENT
extern cy_stc_ble_basc_t cy_ble_basc[CY_BLE_CONFIG_GATTC_COUNT][CY_BLE_BASC_SERVICE_COUNT];
#endif /* (CY_BLE_BAS_CLIENT) */

extern cy_stc_ble_bas_config_t cy_ble_basConfig;
extern cy_stc_ble_bas_config_t *cy_ble_basConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_BASS) */
#endif /* CY_BLE_BAS_H */



/* [] END OF FILE */
