/***************************************************************************//**
* \file cy_ble_ess.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for the Environmental Sensing
*  Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CY_BLE_ESS_H
#define CY_BLE_ESS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_ESS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Conditional Compilation Parameters
***************************************/
#ifdef CY_BLE_ESS_CLIENT
    #define CY_BLE_ES_TOTAL_CHAR_COUNT    (CY_BLE_CONFIG_ES_TOTAL_CHAR_COUNT)
#endif /* CY_BLE_ESS_CLIENT */

#define CY_BLE_ESS_3BYTES_LENGTH          (0x03u)


/**
 * \addtogroup group_ble_service_api_ESS_definitions
 * @{
 */



/***************************************
* Data Types
***************************************/

/** ESS Characteristic indexes */
typedef enum
{
    CY_BLE_ESS_DESCRIPTOR_VALUE_CHANGED,          /**< Descriptor Value Changed Characteristic index */
    CY_BLE_ESS_APPARENT_WIND_DIR,                 /**< Apparent Wind Direction Characteristic index */
    CY_BLE_ESS_APPARENT_WIND_SPEED,               /**< Apparent Wind Speed Characteristic index */
    CY_BLE_ESS_DEW_POINT,                         /**< Dew Point Characteristic index */
    CY_BLE_ESS_ELEVATION,                         /**< Elevation Characteristic index */
    CY_BLE_ESS_GUST_FACTOR,                       /**< Gust Factor Characteristic index */
    CY_BLE_ESS_HEAT_INDEX,                        /**< Heat Index Characteristic index */
    CY_BLE_ESS_HUMIDITY,                          /**< Humidity Characteristic index */
    CY_BLE_ESS_IRRADIANCE,                        /**< Irradiance Characteristic index */
    CY_BLE_ESS_POLLEN_CONCENTRATION,              /**< Pollen Concentration Characteristic index */
    CY_BLE_ESS_RAINFALL,                          /**< Rainfall Characteristic index */
    CY_BLE_ESS_PRESSURE,                          /**< Pressure Characteristic index */
    CY_BLE_ESS_TEMPERATURE,                       /**< Temperature Characteristic index */
    CY_BLE_ESS_TRUE_WIND_DIR,                     /**< True Wind Direction Characteristic index */
    CY_BLE_ESS_TRUE_WIND_SPEED,                   /**< True Wind Speed Characteristic index */
    CY_BLE_ESS_UV_INDEX,                          /**< UV Index Characteristic index */
    CY_BLE_ESS_WIND_CHILL,                        /**< Wind Chill Characteristic index */
    CY_BLE_ESS_BAROMETRIC_PRESSURE_TREND,         /**< Barometric Pressure trend Characteristic index */
    CY_BLE_ESS_MAGNETIC_DECLINATION,              /**< Magnetic Declination Characteristic index */
    CY_BLE_ESS_MAGNETIC_FLUX_DENSITY_2D,          /**< Magnetic Flux Density 2D Characteristic index */
    CY_BLE_ESS_MAGNETIC_FLUX_DENSITY_3D,          /**< Magnetic Flux Density 3D Characteristic index */
    CY_BLE_ESS_CHAR_COUNT                         /**< Total count of ESS characteristics */
}cy_en_ble_ess_char_index_t;

/** ESS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_ESS_CCCD,                              /**< Client Characteristic Configuration Descriptor index */
    CY_BLE_ESS_CHAR_EXTENDED_PROPERTIES,          /**< Characteristic Extended Properties Descriptor index */
    CY_BLE_ESS_ES_MEASUREMENT_DESCR,              /**< ES Measurement Descriptor index */
    CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR1,        /**< ES Trigger Settings Descriptor #1 index */
    CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR2,        /**< ES Trigger Settings Descriptor #2 index */
    CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR3,        /**< ES Trigger Settings Descriptor #3 index */
    CY_BLE_ESS_ES_CONFIG_DESCR,                   /**< ES Configuration Descriptor index */
    CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR,       /**< Characteristic User Description Descriptor index */
    CY_BLE_ESS_VRD,                               /**< Valid Range Descriptor index */
    CY_BLE_ESS_DESCR_COUNT                        /**< Total count of descriptors */
}cy_en_ble_ess_descr_index_t;

/** ESS Characteristic with descriptors */
typedef struct
{
    /** Handles of Characteristic value */
    cy_ble_gatt_db_attr_handle_t charHandle;

    /** Array of Descriptor handles */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_ESS_DESCR_COUNT];
} cy_stc_ble_esss_char_t;

/** Structure to hold pointer to cy_stc_ble_esss_char_t */
typedef struct
{
    /** Pointer to cy_stc_ble_esss_char_t that holds information about specific
     * ES Characteristic
     */
    cy_stc_ble_esss_char_t *charInfoPtr;
} cy_stc_ble_esss_char_info_ptr_t;

/** Structure with Environmental Sensing Service attribute handles */
typedef struct
{
    /** Environmental Sensing Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Environmental Sensing Service Array with pointers to
     * Characteristic handles.
     */
    cy_stc_ble_esss_char_info_ptr_t charInfoAddr[CY_BLE_ESS_CHAR_COUNT];
} cy_stc_ble_esss_t;

/** ESS Characteristic with descriptors */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t valueHandle;                         /**< Handle of characteristic value */
    cy_ble_gatt_db_attr_handle_t endHandle;                           /**< End handle of characteristic */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_ESS_DESCR_COUNT]; /**< Array of Descriptor handles */
    uint8_t                      properties;                          /**< Properties for value field */
} cy_stc_ble_essc_char_t;

/** Structure to hold pointer to cy_stc_ble_essc_char_t */
typedef struct
{
    /** Pointer to cy_stc_ble_essc_char_t that holds information about specific
     * ES Characteristic.
     */
    cy_stc_ble_essc_char_t *charInfoPtr;
} cy_stc_ble_essc_char_info_ptr_t;

/** Structure with discovered attributes information of Environmental Sensing
 * Service.
 */
typedef struct
{
    /** Environmental Sensing Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Environmental Sensing Service Array with pointers to
     * characteristic information.
     */
    cy_stc_ble_essc_char_info_ptr_t charInfoAddr[CY_BLE_ESS_CHAR_COUNT];
} cy_stc_ble_essc_t;

/** ESS Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;                        /**< Peer device handle */
    cy_en_ble_ess_char_index_t charIndex;                         /**< Index of service characteristic */
    uint8_t                    charInstance;                      /**< Instance of specific service characteristic */
    cy_stc_ble_gatt_value_t    *value;                            /**< Characteristic value */
} cy_stc_ble_ess_char_value_t;

/** ESS Characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;                       /**< Peer device handle */
    cy_en_ble_ess_char_index_t  charIndex;                        /**< Index of service characteristic */
    uint8_t                     charInstance;                     /**< Instance of specific service characteristic */
    cy_en_ble_ess_descr_index_t descrIndex;                       /**< Index of descriptor */
    cy_en_ble_gatt_err_code_t   gattErrorCode;                    /**< Error code received from application (optional) */
    cy_stc_ble_gatt_value_t     *value;                           /**< Characteristic value */
} cy_stc_ble_ess_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** Environmental Sensing Service GATT DB handles structure for server */
    const cy_stc_ble_esss_t *esss;

    /** Number of ESS characteristics instances for server */
    uint8_t                 *esssCharInstances;

    /** Environmental Sensing Service GATT DB handles structure for client */
    cy_stc_ble_essc_t       *essc;

    /** Number of ESS characteristics instances for client */
    const uint8_t           *esscCharInstances;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_ess_config_t;

/** @} */


/***************************************
* API Constants
***************************************/
/* ESS specific GATT errors  */
#define CY_BLE_GATT_ERR_WRITE_REQ_REJECTED                 (0x80u)
#define CY_BLE_GATT_ERR_CONDITION_NOT_SUPPORTED            (0x81u)

#define CY_BLE_ESS_UNSIGNED_TYPE                           (0u)
#define CY_BLE_ESS_SIGNED_TYPE                             (1u)

/* ES Trigger Descriptor "Condition" field constants */
#define CY_BLE_ESS_TRIG_TRIGGER_INACTIVE                   (0x00u)
#define CY_BLE_ESS_TRIG_USE_FIXED_TIME_INTERVAL            (0x01u)
#define CY_BLE_ESS_TRIG_NO_LESS_THEN_TIME_INTERVAL         (0x02u)
#define CY_BLE_ESS_TRIG_WHEN_CHANGED                       (0x03u)
#define CY_BLE_ESS_TRIG_WHILE_LESS_THAN                    (0x04u)
#define CY_BLE_ESS_TRIG_WHILE_LESS_OR_EQUAL                (0x05u)
#define CY_BLE_ESS_TRIG_WHILE_GREATER_THAN                 (0x06u)
#define CY_BLE_ESS_TRIG_WHILE_GREATER_OR_EQUAL             (0x07u)
#define CY_BLE_ESS_TRIG_WHILE_EQUAL_TO                     (0x08u)
#define CY_BLE_ESS_TRIG_WHILE_EQUAL_NOT_TO                 (0x09u)

/* ES Measurement Descriptor "Sampling Function" field constants */
#define CY_BLE_ESS_MEASURE_SF_UNSPECIFIED                  (0x00u)
#define CY_BLE_ESS_MEASURE_SF_INSTANTANEOUS                (0x01u)
#define CY_BLE_ESS_MEASURE_SF_ARITHM_MEAN                  (0x02u)
#define CY_BLE_ESS_MEASURE_SF_RMS                          (0x03u)
#define CY_BLE_ESS_MEASURE_SF_MAXIMUM                      (0x04u)
#define CY_BLE_ESS_MEASURE_SF_MINIMUM                      (0x05u)
#define CY_BLE_ESS_MEASURE_SF_ACCUMULATED                  (0x06u)
#define CY_BLE_ESS_MEASURE_SF_COUNT                        (0x07u)

/* ES Measurement Descriptor "Application" field constants */
#define CY_BLE_ESS_MEASURE_APP_UNSPECIFIED                 (0x00u)
#define CY_BLE_ESS_MEASURE_APP_AIR                         (0x01u)
#define CY_BLE_ESS_MEASURE_APP_WATER                       (0x02u)
#define CY_BLE_ESS_MEASURE_APP_BAROMETRIC                  (0x03u)
#define CY_BLE_ESS_MEASURE_APP_SOIL                        (0x04u)
#define CY_BLE_ESS_MEASURE_APP_INFRARED                    (0x05u)
#define CY_BLE_ESS_MEASURE_APP_MAP_DATABASE                (0x06u)
#define CY_BLE_ESS_MEASURE_APP_BAR_ELEVATION_SOURCE        (0x07u)
#define CY_BLE_ESS_MEASURE_APP_GPS_ELEVATION_SOURCE        (0x08u)
#define CY_BLE_ESS_MEASURE_APP_GPS_MAP_ELEVATION_SOURCE    (0x09u)
#define CY_BLE_ESS_MEASURE_APP_VER_ELEVATION_SOURCE        (0x0Au)
#define CY_BLE_ESS_MEASURE_APP_ONSHORE                     (0x0Bu)
#define CY_BLE_ESS_MEASURE_APP_ONBOARD_VES_VEH             (0x0Cu)
#define CY_BLE_ESS_MEASURE_APP_FRONT                       (0x0Du)
#define CY_BLE_ESS_MEASURE_APP_BACK_REAR                   (0x0Eu)
#define CY_BLE_ESS_MEASURE_APP_UPPER                       (0x0Fu)
#define CY_BLE_ESS_MEASURE_APP_LOWER                       (0x10u)
#define CY_BLE_ESS_MEASURE_APP_PRIMARY                     (0x11u)
#define CY_BLE_ESS_MEASURE_APP_SECONDARY                   (0x12u)
#define CY_BLE_ESS_MEASURE_APP_OUTDOOR                     (0x13u)
#define CY_BLE_ESS_MEASURE_APP_INDOOR                      (0x14u)
#define CY_BLE_ESS_MEASURE_APP_TOP                         (0x15u)
#define CY_BLE_ESS_MEASURE_APP_BOTTOM                      (0x16u)
#define CY_BLE_ESS_MEASURE_APP_MAIN                        (0x17u)
#define CY_BLE_ESS_MEASURE_APP_BACKUP                      (0x18u)
#define CY_BLE_ESS_MEASURE_APP_AUXILIARY                   (0x19u)
#define CY_BLE_ESS_MEASURE_APP_SUPLEMENTARY                (0x1Au)
#define CY_BLE_ESS_MEASURE_APP_INSIDE                      (0x1Bu)
#define CY_BLE_ESS_MEASURE_APP_OUTSIDE                     (0x1Cu)
#define CY_BLE_ESS_MEASURE_APP_LEFT                        (0x1Du)
#define CY_BLE_ESS_MEASURE_APP_RIGHT                       (0x1Eu)
#define CY_BLE_ESS_MEASURE_APP_INTERNAL                    (0x1Fu)
#define CY_BLE_ESS_MEASURE_APP_EXTERNAL                    (0x20u)
#define CY_BLE_ESS_MEASURE_APP_SOLAR                       (0x21u)

/* ES Configuration Descriptor Trigger Logic value constants */
#define CY_BLE_ESS_CONF_BOOLEAN_AND                        (0x00u)
#define CY_BLE_ESS_CONF_BOOLEAN_OR                         (0x01u)

/* Descriptor Value Changed Characteristic Flags */
#define CY_BLE_ESS_VALUE_CHANGE_SOURCE_CLIENT              (0x01u)
#define CY_BLE_ESS_VALUE_CHANGE_ES_TRIGGER                 (0x02u)
#define CY_BLE_ESS_VALUE_CHANGE_ES_CONFIG                  (0x04u)
#define CY_BLE_ESS_VALUE_CHANGE_ES_MEASUREMENT             (0x08u)
#define CY_BLE_ESS_VALUE_CHANGE_USER_DESCRIPTION           (0x10u)

/* Internal Constants
** \cond IGNORE
*/
#define CY_BLE_ESS_U8_SIGN_BIT                             (0x00000080ul)
#define CY_BLE_ESS_U16_SIGN_BIT                            (0x00008000ul)
#define CY_BLE_ESS_U24_SIGN_BIT                            (0x00800000ul)

#define CY_BLE_ESS_STATE_0                                 (0x00u)
#define CY_BLE_ESS_STATE_1                                 (0x01u)
#define CY_BLE_ESS_STATE_2                                 (0x02u)
#define CY_BLE_ESS_STATE_3                                 (0x03u)
#define CY_BLE_ESS_STATE_4                                 (0x04u)
#define CY_BLE_ESS_STATE_5                                 (0x05u)
#define CY_BLE_ESS_STATE_6                                 (0x06u)
#define CY_BLE_ESS_STATE_7                                 (0x07u)

#define CY_BLE_ESS_2BYTES_LENGTH                           (0x02u)
#define CY_BLE_ESS_4BYTES_LENGTH                           (0x04u)
#define CY_BLE_ESS_6BYTES_LENGTH                           (0x06u)
#define CY_BLE_ESS_8BYTES_LENGTH                           (0x08u)

#define CY_BLE_ESS_UNDEFINED_ITEM                          (0x00u)
#define CY_BLE_ESS_CHARACTERISTIC_ITEM                     (0x01u)
#define CY_BLE_ESS_DESCRIPTOR_ITEM                         (0x02u)

#if (CY_BLE_GAP_ROLE_PERIPHERAL)
#define CY_BLE_ESS_SERVICE_DATA_LENGTH                     (0x05u)
#endif /* (CY_BLE_GAP_ROLE_PERIPHERAL) */

/** \endcond */

/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_ESS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_ESS_Init(cy_stc_ble_ess_config_t *config);
cy_en_ble_api_result_t Cy_BLE_ESS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_ESS_SERVER
/**
 * \addtogroup group_ble_service_api_ESS_server
 * @{
 */

#if (CY_BLE_GAP_ROLE_PERIPHERAL)

cy_en_ble_api_result_t Cy_BLE_ESSS_SetChangeIndex(uint16_t essIndex, uint8_t advertisingParamIndex);

#endif /* (CY_BLE_GAP_ROLE_PERIPHERAL) */

cy_en_ble_api_result_t Cy_BLE_ESSS_SetCharacteristicValue(cy_en_ble_ess_char_index_t charIndex,
                                                          uint8_t charInstance, uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ESSS_GetCharacteristicValue(cy_en_ble_ess_char_index_t charIndex, uint8_t charInstance,
                                                          uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ESSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ess_char_index_t charIndex, uint8_t charInstance,
                                                               cy_en_ble_ess_descr_index_t descrIndex,
                                                               uint16_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ESSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ess_char_index_t charIndex, uint8_t charInstance,
                                                               cy_en_ble_ess_descr_index_t descrIndex,
                                                               uint16_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ESSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_ess_char_index_t charIndex, uint8_t charInstance,
                                                    uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ESSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                  cy_en_ble_ess_char_index_t charIndex, uint8_t charInstance,
                                                  uint8_t attrSize, uint8_t *attrValue);

#endif /* CY_BLE_ESS_SERVER */

/** @} */

#ifdef CY_BLE_ESS_CLIENT
/**
 * \addtogroup group_ble_service_api_ESS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_ESSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_ess_char_index_t charIndex, uint8_t charInstance,
                                                          uint8_t attrSize, uint8_t * attrValue);

cy_en_ble_api_result_t Cy_BLE_ESSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_ess_char_index_t charIndex, uint8_t charInstance);

cy_en_ble_api_result_t Cy_BLE_ESSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ess_char_index_t charIndex, uint8_t charInstance,
                                                               cy_en_ble_ess_descr_index_t descrIndex, uint8_t attrSize,
                                                               uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ESSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ess_char_index_t charIndex, uint8_t charInstance,
                                                               cy_en_ble_ess_descr_index_t descrIndex);

cy_en_ble_api_result_t Cy_BLE_ESSC_SetLongCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                   cy_en_ble_ess_char_index_t charIndex,
                                                                   uint8_t charInstance,
                                                                   cy_en_ble_ess_descr_index_t descrIndex,
                                                                   uint16_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ESSC_GetLongCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                   cy_en_ble_ess_char_index_t charIndex,
                                                                   uint8_t charInstance,
                                                                   cy_en_ble_ess_descr_index_t descrIndex,
                                                                   uint16_t attrSize, uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_ESS_CLIENT */


/***************************************
* Private Function Prototypes
***************************************/

/** \cond IGNORE */
#ifdef CY_BLE_ESS_SERVER
uint32_t Cy_BLE_ESS_Get24ByPtr(const uint8_t ptr[]);
uint32_t Cy_BLE_ESS_Get32ByPtr(const uint8_t ptr[]);
cy_en_ble_gatt_err_code_t Cy_BLE_ESS_HandleRangeCheck(cy_stc_ble_conn_handle_t connHandle,
                                                      cy_en_ble_ess_char_index_t charIndex, uint8_t charInstance,
                                                      uint16_t length, uint8_t type, const uint8_t pValue[]);
cy_en_ble_gatt_err_code_t Cy_BLE_ESSS_CheckIfInRange(uint32_t min, uint32_t max, uint32_t value, uint8_t state);
#endif /* CY_BLE_ESS_SERVER */
/** \endcond */


/***************************************
* Macro Functions
***************************************/

#define Cy_BLE_ESS_Get16ByPtr    Cy_BLE_Get16ByPtr
#define CY_BLE_ESS_IS_NEGATIVE(value, sign)    (((((uint32_t)(value)) & (sign)) == (sign)) ? (1u) : (0u))


/***************************************
* External data references
***************************************/

#ifdef CY_BLE_ESS_CLIENT
extern uint8_t activeCharIndex[CY_BLE_CONFIG_GATTC_COUNT];
extern uint8_t activeCharInstance[CY_BLE_CONFIG_GATTC_COUNT];
extern uint8_t prevCharInstIndex[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_ESS_CLIENT) */

extern cy_stc_ble_ess_config_t cy_ble_essConfig;
extern cy_stc_ble_ess_config_t *cy_ble_essConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_ESS) */
#endif /* CY_BLE_ESS_H */

/* [] END OF FILE */
