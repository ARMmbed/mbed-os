/***************************************************************************//**
* \file cy_ble_hts.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for Health Thermometer Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_HTS_H
#define CY_BLE_HTS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_HTS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \addtogroup group_ble_service_api_HTS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** HTS Characteristic indexes */
typedef enum
{
    CY_BLE_HTS_TEMP_MEASURE,                         /**< Temperature Measurement characteristic index */
    CY_BLE_HTS_TEMP_TYPE,                            /**< Temperature Type characteristic index */
    CY_BLE_HTS_INTERM_TEMP,                          /**< Intermediate Temperature characteristic index*/
    CY_BLE_HTS_MEASURE_INTERVAL,                     /**< Measurement Interval characteristic index */
    CY_BLE_HTS_CHAR_COUNT                            /**< Total count of HTS characteristics */
}cy_en_ble_hts_char_index_t;

/** HTS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_HTS_CCCD,                                  /**< Client Characteristic Configuration descriptor index */
    CY_BLE_HTS_VRD,                                   /**< Valid Range descriptor index */
    CY_BLE_HTS_DESCR_COUNT                            /**< Total count of descriptors */
}cy_en_ble_hts_descr_index_t;

/** HTS Characteristic with descriptors */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t charHandle;                          /**< Handle of characteristic value */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_HTS_DESCR_COUNT]; /**< Handle of descriptor */
} cy_stc_ble_htss_char_t;

/** Structure with Health Thermometer Service attribute handles */
typedef struct
{
    /** Health Thermometer Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Health Thermometer Service Characteristic handles */
    cy_stc_ble_htss_char_t       charInfo[CY_BLE_HTS_CHAR_COUNT];
} cy_stc_ble_htss_t;

/** HTS Characteristic with descriptors */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_HTS_DESCR_COUNT]; /**< Handle of descriptor */
    cy_ble_gatt_db_attr_handle_t valueHandle;                         /**< Handle of Report characteristic value */
    cy_ble_gatt_db_attr_handle_t endHandle;                           /**< End handle of characteristic */
    uint8_t                      properties;                          /**< Properties for value field */
} cy_stc_ble_htsc_char_t;

/** Structure with discovered attributes information of Health Thermometer Service */
typedef struct
{
    /** Characteristics handles array */
    cy_stc_ble_htsc_char_t charInfo[CY_BLE_HTS_CHAR_COUNT];
} cy_stc_ble_htsc_t;

/** HTS Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;                       /**< Peer device handle */
    cy_en_ble_hts_char_index_t charIndex;                        /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t    *value;                           /**< Characteristic value */
} cy_stc_ble_hts_char_value_t;

/** HTS Characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;                      /**< Peer device handle */
    cy_en_ble_hts_char_index_t  charIndex;                       /**< Index of service characteristic */
    cy_en_ble_hts_descr_index_t descrIndex;                      /**< Index of descriptor */
    cy_stc_ble_gatt_value_t     *value;                          /**< Characteristic value */
} cy_stc_ble_hts_descr_value_t;


/** The IEEE-11073 FLOAT-Type is defined as a 32-bit value
 * with a 24-bit mantissa and an 8-bit exponent. */
typedef struct
{
    int8_t  exponent;                                          /**< Base 10 exponent */
    int32_t mantissa;                                          /**< Mantissa, should be using only 24 bits */
} CY_BLE_HTS_FLOAT32;

/** Temperature Type measurement indicates where the temperature was measured */
typedef enum
{
    CY_BLE_HTS_TEMP_TYPE_ARMPIT = 0x01u,     /**< Armpit */
    CY_BLE_HTS_TEMP_TYPE_BODY,               /**< Body (general) */
    CY_BLE_HTS_TEMP_TYPE_EAR,                /**< Ear (usually ear lobe) */
    CY_BLE_HTS_TEMP_TYPE_FINGER,             /**< Finger */
    CY_BLE_HTS_TEMP_TYPE_GI_TRACT,           /**< Gastro-intestinal Tract */
    CY_BLE_HTS_TEMP_TYPE_MOUTH,              /**< Mouth */
    CY_BLE_HTS_TEMP_TYPE_RECTUM,             /**< Rectum */
    CY_BLE_HTS_TEMP_TYPE_TOE,                /**< Toe */
    CY_BLE_HTS_TEMP_TYPE_TYMPANUM            /**< Tympanum (ear drum) */
}cy_stc_ble_hts_temp_type_t;

/** Service configuration structure */
typedef struct
{
    /** Health Thermometer Service GATT DB handles structure */
    const cy_stc_ble_htss_t *htss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_hts_config_t;

/** @} */


/***************************************
* API Constants
***************************************/

/* Valid Range descriptor length */
#define CY_BLE_HTS_VRD_LEN                     (0x04u)

/* Health Thermometer measurement flag bits */
#define CY_BLE_HTS_MEAS_FLAG_TEMP_UNITS_BIT    (0x01u << 0u)
#define CY_BLE_HTS_MEAS_FLAG_TIME_STAMP_BIT    (0x01u << 1u)
#define CY_BLE_HTS_MEAS_FLAG_TEMP_TYPE_BIT     (0x01u << 2u)


/***************************************
* Function Prototypes
***************************************/

/**
 * \addtogroup group_ble_service_api_HTS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_HTS_Init(cy_stc_ble_hts_config_t *config);
cy_en_ble_api_result_t Cy_BLE_HTS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_HTS_SERVER
/**
 * \addtogroup group_ble_service_api_HTS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_HTSS_SetCharacteristicValue(cy_en_ble_hts_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HTSS_GetCharacteristicValue(cy_en_ble_hts_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HTSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hts_char_index_t charIndex,
                                                               cy_en_ble_hts_descr_index_t descrIndex, uint8_t attrSize,
                                                               uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HTSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hts_char_index_t charIndex,
                                                               cy_en_ble_hts_descr_index_t descrIndex, uint8_t attrSize,
                                                               uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HTSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_hts_char_index_t charIndex, uint8_t attrSize,
                                                    uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HTSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_hts_char_index_t charIndex, uint8_t attrSize,
                                                  uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_HTS_SERVER */

#ifdef CY_BLE_HTS_CLIENT
/**
 * \addtogroup group_ble_service_api_HTS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_HTSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_hts_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HTSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_hts_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_HTSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hts_char_index_t charIndex,
                                                               cy_en_ble_hts_descr_index_t descrIndex, uint8_t attrSize,
                                                               uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_HTSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_hts_char_index_t charIndex,
                                                               cy_en_ble_hts_descr_index_t descrIndex);

/** @} */
#endif /* (CY_BLE_HTS_CLIENT) */


/***************************************
* Macro Functions
***************************************/

#ifdef CY_BLE_HTS_CLIENT

#define Cy_BLE_HTSC_CheckCharHandle(handle)                                  \
    do {                                                                     \
        if((handle).valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)    \
        {                                                                    \
            (handle).valueHandle = discCharInfo->valueHandle;                \
            (handle).properties = discCharInfo->properties;                  \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, \
                                &(discCharInfo->uuid.uuid16));               \
        }                                                                    \
    } while(0)

#define Cy_BLE_HTSC_GetCharacteristicValueHandle(connHandle, charIndex) \
    (((charIndex) >= CY_BLE_HTS_CHAR_COUNT) ?                           \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                            \
     cy_ble_htsc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfo[charIndex].valueHandle)

#define Cy_BLE_HTSC_GetCharacteristicDescriptorHandle(connHandle, charIndex, descrIndex)    \
    ((((charIndex) >= CY_BLE_HTS_CHAR_COUNT) || ((descrIndex) >= CY_BLE_HTS_DESCR_COUNT)) ? \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                                                \
     cy_ble_htsc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfo[charIndex].descrHandle[descrIndex])

#endif /* (CY_BLE_HTS_CLIENT) */
/** \endcond */


/***************************************
* External data references
***************************************/
#ifdef CY_BLE_HTS_CLIENT
extern cy_stc_ble_htsc_t cy_ble_htsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_HTS_CLIENT) */

extern cy_stc_ble_hts_config_t cy_ble_htsConfig;
extern cy_stc_ble_hts_config_t *cy_ble_htsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_HTS) */
#endif /* CY_BLE_HTS_H */

/* [] END OF FILE */
