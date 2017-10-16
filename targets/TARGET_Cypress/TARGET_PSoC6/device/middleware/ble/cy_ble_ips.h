/***************************************************************************//**
* \file cy_ble_ips.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for the Indoor Positioning
*  Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_IPS_H
#define CY_BLE_IPS_H

#include "cy_ble_gatt.h"

/**
 * \addtogroup group_ble_service_api_IPS_definitions
 * @{
 */

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_IPS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Data Struct Definition
***************************************/

/** IPS Characteristic indexes */
typedef enum
{
    CY_BLE_IPS_INDOOR_POSITINING_CONFIG,         /**< Set of characteristic values included in the Indoor Positioning Service AD type. */
    CY_BLE_IPS_LATITUDE,                         /**< WGS84 North coordinate of the device. */
    CY_BLE_IPS_LONGITUDE,                        /**< WGS84 East coordinate of the device. */
    CY_BLE_IPS_LOCAL_NORTH_COORDINATE,           /**< North coordinate of the device using local coordinate system. */
    CY_BLE_IPS_LOCAL_EAST_COORDINATE,            /**< East coordinate of the device using local coordinate system. */
    CY_BLE_IPS_FLOOR_NUMBER,                     /**< Describes in which floor the device is installed. */
    CY_BLE_IPS_ALTITUDE,                         /**< Altitude of the device. */
    CY_BLE_IPS_UNCERTAINTY,                      /**< Uncertainty of the location information the device exposes. */
    CY_BLE_IPS_LOCATION_NAME,                    /**< Name of the location where the device is installed. */
    CY_BLE_IPS_CHAR_COUNT                        /**< Total count of IPS characteristics */
}cy_en_ble_ips_char_index_t;

/** IPS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_IPS_CEPD,                              /**< Characteristic Extended Properties descriptor index */
    CY_BLE_IPS_SCCD,                              /**< Server Characteristic Configuration Descriptor index */
    CY_BLE_IPS_DESCR_COUNT                        /**< Total count of descriptors */
}cy_en_ble_ips_descr_index_t;

/** IPS Characteristic with descriptors */
typedef struct
{
    /** Handles of Characteristic value */
    cy_ble_gatt_db_attr_handle_t charHandle;

    /** Array of Descriptor handles */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_IPS_DESCR_COUNT];
} cy_stc_ble_ipss_char_t;

/** Structure to hold pointer to cy_stc_ble_ipss_char_t */
typedef struct
{
    /** Pointer to cy_stc_ble_ipss_char_t that holds information about specific
     * IP Characteristic
     */
    cy_stc_ble_ipss_char_t *charInfoPtr;
} cy_stc_ble_ipss_char_info_ptr_t;

/** Structure with Indoor Positioning Service attribute handles */
typedef struct
{
    /** Indoor Positioning Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Indoor Positioning Service Array with pointers to
     * Characteristic handles.
     */
    cy_stc_ble_ipss_char_t charInfo[CY_BLE_IPS_CHAR_COUNT];
} cy_stc_ble_ipss_t;

/** IPS Characteristic with descriptors */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t valueHandle;                         /**< Handle of characteristic value */
    cy_ble_gatt_db_attr_handle_t endHandle;                           /**< End handle of characteristic */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_IPS_DESCR_COUNT]; /**< Array of Descriptor handles */
    uint8_t                      properties;                          /**< Properties for value field */
} cy_stc_ble_ipsc_char_t;

/** Structure to hold pointer to cy_stc_ble_ipsc_char_t */
typedef struct
{
    /** Pointer to cy_stc_ble_ipsc_char_t that holds information about specific IP Characteristic. */
    cy_stc_ble_ipsc_char_t *charInfoPtr;
} cy_stc_ble_ipsc_char_info_ptr_t;

/** Structure with discovered attributes information of Indoor Positioning Service. */
typedef struct
{
    /** Indoor Positioning Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Indoor Positioning Service characteristics info array */
    cy_stc_ble_ipsc_char_t       charInfo[CY_BLE_IPS_CHAR_COUNT];
} cy_stc_ble_ipsc_t;


/** IPS Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;                      /**< Peer device handle */
    cy_en_ble_ips_char_index_t charIndex;                       /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t    *value;                          /**< Characteristic value */
    cy_en_ble_gatt_err_code_t  gattErrorCode;                   /**< GATT error code for access control */
} cy_stc_ble_ips_char_value_t;

/** IPS Characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;                     /**< Peer device handle */
    cy_en_ble_ips_char_index_t  charIndex;                      /**< Index of service characteristic */
    cy_en_ble_ips_descr_index_t descrIndex;                     /**< Index of descriptor */
    cy_en_ble_gatt_err_code_t   gattErrorCode;                  /**< Error code received from application (optional) */
    cy_stc_ble_gatt_value_t     *value;                         /**< Characteristic value */
} cy_stc_ble_ips_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** Indoor Positioning Service GATT DB handles structure */
    const cy_stc_ble_ipss_t *ipss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_ips_config_t;

/** @} */

/***************************************
* API Constants
***************************************/
/* IPS specific GATT errors  */
#define CY_BLE_GATT_ERR_WRITE_REQ_REJECTED    (0x80u)


#if (CY_BLE_GAP_ROLE_PERIPHERAL)

#define CY_BLE_IPS_AD_TYPE                              (0x25u)
#define CY_BLE_IPS_SERVICE_DATA_LENGTH                  (0x05u)

#define CY_BLE_IPS_CHARACTERISTICS_IN_ADVERTISING       (0x7Du)
#define CY_BLE_IPS_COORDINATES_IN_ADVERTISING           (0x01u)
#define CY_BLE_IPS_COORDINATES_NOT_PRESENT              (0u)
#define CY_BLE_IPS_COORDINATES_PRESENT                  (1u)
#define CY_BLE_IPS_TYPE_OF_COORDINATE_IN_ADVERTISING    (0x02u)
#define CY_BLE_IPS_WGS84_COORDINATE                     (0u)
#define CY_BLE_IPS_LOCAL_COORDINATE                     (1u)
#define CY_BLE_IPS_TX_POWER_IN_ADVERTISING              (0x04u)
#define CY_BLE_IPS_TX_POWER_NOT_PRESENT                 (0u)
#define CY_BLE_IPS_TX_POWER_PRESENT                     (1u)
#define CY_BLE_IPS_ALTITUDE_IN_ADVERTISING              (0x08u)
#define CY_BLE_IPS_ALTITUDE_NOT_PRESENT                 (0u)
#define CY_BLE_IPS_ALTITUDE_PRESENT                     (1u)
#define CY_BLE_IPS_FLOOR_NUMBER_IN_ADVERTISING          (0x10u)
#define CY_BLE_IPS_FLOOR_NUMBER_NOT_PRESENT             (0u)
#define CY_BLE_IPS_FLOOR_NUMBER_PRESENT                 (1u)
#define CY_BLE_IPS_UNCERTAINTY_NUMBER_IN_ADVERTISING    (0x20u)
#define CY_BLE_IPS_UNCERTAINTY_NUMBER_NOT_PRESENT       (0u)
#define CY_BLE_IPS_UNCERTAINTY_NUMBER_PRESENT           (1u)
#define CY_BLE_IPS_LOCATION_NAME_IN_GATT_DATABASE       (0x40u)
#define CY_BLE_IPS_LOCATION_NAME_NOT_PRESENT            (0u)
#define CY_BLE_IPS_LOCATION_NAME_PRESENT                (1u)


#endif /* (CY_BLE_GAP_ROLE_PERIPHERAL) */


/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_IPS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_IPS_Init(cy_stc_ble_ips_config_t *config);
cy_en_ble_api_result_t Cy_BLE_IPS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_IPS_SERVER
/**
 * \addtogroup group_ble_service_api_IPS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_IPSS_SetCharacteristicValue(cy_en_ble_ips_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_IPSS_GetCharacteristicValue(cy_en_ble_ips_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_IPSS_SetCharacteristicDescriptor(cy_en_ble_ips_char_index_t charIndex,
                                                               cy_en_ble_ips_descr_index_t descrIndex, uint8_t attrSize,
                                                               uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_IPSS_GetCharacteristicDescriptor(cy_en_ble_ips_char_index_t charIndex,
                                                               cy_en_ble_ips_descr_index_t descrIndex, uint8_t attrSize,
                                                               uint8_t *attrValue);


#endif /* CY_BLE_IPS_SERVER */

/** @} */

#ifdef CY_BLE_IPS_CLIENT
/**
 * \addtogroup group_ble_service_api_IPS_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_IPSC_SetCharacteristicValueWithoutResponse(cy_stc_ble_conn_handle_t connHandle,
                                                                         cy_en_ble_ips_char_index_t charIndex,
                                                                         uint8_t attrSize, uint8_t * attrValue);

cy_en_ble_api_result_t Cy_BLE_IPSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_ips_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_IPSC_ReliableWriteCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                                    cy_en_ble_ips_char_index_t charIndex,
                                                                    uint8_t attrSize, uint8_t * attrValue);

cy_en_ble_api_result_t Cy_BLE_IPSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_ips_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_IPSC_GetMultipleCharacteristicValues(cy_stc_ble_conn_handle_t connHandle,
                                                                   const cy_en_ble_ips_char_index_t *charIndexesList,
                                                                   uint8_t numberOfCharIndexes);

cy_en_ble_api_result_t Cy_BLE_IPSC_GetLongCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                              cy_en_ble_ips_char_index_t charIndex, uint16_t attrSize,
                                                              uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_IPSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ips_char_index_t charIndex,
                                                               cy_en_ble_ips_descr_index_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_IPSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ips_char_index_t charIndex,
                                                               cy_en_ble_ips_descr_index_t descrIndex);

/** @} */
#endif /* CY_BLE_IPS_CLIENT */


/***************************************
* Macro Functions
***************************************/

#define Cy_BLE_IPS_Get16ByPtr    Cy_BLE_Get16ByPtr

#ifdef CY_BLE_IPS_CLIENT

#define Cy_BLE_IPSC_GetCharacteristicValueHandle(connHandle, charIndex) \
    (((charIndex) >= CY_BLE_IPS_CHAR_COUNT) ?                           \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                            \
     cy_ble_ipsc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfo[charIndex].valueHandle)

#define Cy_BLE_IPSC_GetCharacteristicDescriptorHandle(connHandle, charIndex, descrIndex)    \
    ((((charIndex) >= CY_BLE_IPS_CHAR_COUNT) || ((descrIndex) >= CY_BLE_IPS_DESCR_COUNT)) ? \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                                                \
     cy_ble_ipsc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfo[charIndex].descrHandle[descrIndex])

#endif /* (CY_BLE_IPS_CLIENT) */


/***************************************
* External data references
***************************************/
#ifdef CY_BLE_IPS_CLIENT
extern cy_stc_ble_ipsc_t cy_ble_ipsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_IPS_CLIENT) */

extern cy_stc_ble_ips_config_t cy_ble_ipsConfig;
extern cy_stc_ble_ips_config_t *cy_ble_ipsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_IPS) */
#endif /* CY_BLE_IPS_H */


/* [] END OF FILE */
