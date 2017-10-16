/***************************************************************************//**
* \file cy_ble_dis.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for Device Information
*  Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_DIS_H
#define CY_BLE_DIS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_DIS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \addtogroup group_ble_service_api_DIS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** DIS characteristic index */
typedef enum
{
    CY_BLE_DIS_MANUFACTURER_NAME,                    /**< Manufacturer Name String characteristic index */
    CY_BLE_DIS_MODEL_NUMBER,                         /**< Model Number String characteristic index */
    CY_BLE_DIS_SERIAL_NUMBER,                        /**< Serial Number String characteristic index*/
    CY_BLE_DIS_HARDWARE_REV,                         /**< Hardware Revision String characteristic index */
    CY_BLE_DIS_FIRMWARE_REV,                         /**< Firmware Revision String characteristic index */
    CY_BLE_DIS_SOFTWARE_REV,                         /**< Software Revision String characteristic index */
    CY_BLE_DIS_SYSTEM_ID,                            /**< System ID characteristic index */
    CY_BLE_DIS_REG_CERT_DATA,                        /**< IEEE 11073-20601 characteristic index */
    CY_BLE_DIS_PNP_ID,                               /**< PnP ID characteristic index */
    CY_BLE_DIS_CHAR_COUNT                            /**< Total count of DIS characteristics */
}cy_en_ble_dis_char_index_t;

/** Structure with Device Information Service attribute handles */
typedef struct
{
    /** Device Information Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Device Information Service Characteristic handles */
    cy_ble_gatt_db_attr_handle_t charHandle[CY_BLE_DIS_CHAR_COUNT];
} cy_stc_ble_diss_t;

/** Structure with discovered attributes information of Device Information Service */
typedef struct
{
    /** Characteristics handle + properties array */
    cy_stc_ble_srvr_char_info_t charInfo[CY_BLE_DIS_CHAR_COUNT];
} cy_stc_ble_disc_t;

/** Device Information Service Characteristic Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;           /**< Peer device handle */
    cy_en_ble_dis_char_index_t charIndex;            /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t    *value;               /**< Characteristic value */
} cy_stc_ble_dis_char_value_t;

/** Service configuration structure */
typedef struct
{
    /** Device Information Service GATT DB handles structure */
    const cy_stc_ble_diss_t *diss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_dis_config_t;

/** @} */


/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_DIS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_DIS_Init(cy_stc_ble_dis_config_t *config);

cy_en_ble_api_result_t Cy_BLE_DIS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */


#ifdef CY_BLE_DIS_SERVER
/**
 * \addtogroup group_ble_service_api_DIS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_DISS_SetCharacteristicValue(cy_en_ble_dis_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_DISS_GetCharacteristicValue(cy_en_ble_dis_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_DIS_SERVER */

#ifdef CY_BLE_DIS_CLIENT
/**
 * \addtogroup group_ble_service_api_DIS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_DISC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_dis_char_index_t charIndex);

/** @} */
#endif /* (CY_BLE_DIS_CLIENT) */


/***************************************
* Macro Functions
***************************************/

#ifdef CY_BLE_DIS_CLIENT

#define Cy_BLE_DISC_CheckCharHandle(handle)                                  \
    do {                                                                     \
        if((handle).valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)    \
        {                                                                    \
            (handle).valueHandle = discCharInfo->valueHandle;                \
            (handle).properties = discCharInfo->properties;                  \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, \
                                &(discCharInfo));                            \
        }                                                                    \
    } while(0)

#define Cy_BLE_DISC_GetCharacteristicValueHandle(connHandle, charIndex) \
    (((charIndex) >= CY_BLE_DIS_CHAR_COUNT) ?                           \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                            \
     cy_ble_disc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfo[charIndex].valueHandle)

#endif /* (CY_BLE_DIS_CLIENT) */
/** \endcond */


/***************************************
* External data references
***************************************/
#ifdef CY_BLE_DIS_CLIENT
extern cy_stc_ble_disc_t cy_ble_disc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_DIS_CLIENT) */

extern cy_stc_ble_dis_config_t cy_ble_disConfig;
extern cy_stc_ble_dis_config_t *cy_ble_disConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_DIS) */
#endif /* CY_BLE_DIS_H */


/* [] END OF FILE */
