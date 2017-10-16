/***************************************************************************//**
* \file cy_ble_cts.h
* \version 2.0
*
* \brief
*  This file contains the function prototypes and constants used in
*  the Current Time Service of the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_CTS_H
#define CY_BLE_CTS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_CTS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \addtogroup group_ble_service_api_CTS_definitions
 * @{
 */

/***************************************
* Data Types
***************************************/

/** Service Characteristics indexes */
typedef enum
{
    CY_BLE_CTS_CURRENT_TIME,            /**< Current Time characteristic index */
    CY_BLE_CTS_LOCAL_TIME_INFO,         /**< Local Time Information characteristic index */
    CY_BLE_CTS_REFERENCE_TIME_INFO,     /**< Reference Time Information characteristic index */
    CY_BLE_CTS_CHAR_COUNT               /**< Total count of Current Time Service characteristics */
}cy_en_ble_cts_char_index_t;

/** Service Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_CTS_CURRENT_TIME_CCCD,       /**< Current Time Client Characteristic configuration descriptor index */
    CY_BLE_CTS_COUNT                    /**< Total count of Current Time Service characteristic descriptors */
}cy_en_ble_cts_char_descriptors_t;

/** Current Time Characteristic structure */
typedef struct
{
    uint8_t yearLow;                    /**< LSB of current year */
    uint8_t yearHigh;                   /**< MSB of current year */
    uint8_t month;                      /**< Current month */
    uint8_t day;                        /**< Current day */
    uint8_t hours;                      /**< Current time - hours */
    uint8_t minutes;                    /**< Current time - minutes */
    uint8_t seconds;                    /**< Current time - seconds */
    uint8_t dayOfWeek;                  /**< Current day of week */
    uint8_t fractions256;               /**< The value of 1/256th of second */
    uint8_t adjustReason;               /**< Reason of Current Time service characteristics change */
} cy_stc_ble_cts_current_time_t;

/** Local Time Information Characteristic structure */
typedef struct
{
    int8_t  timeZone;                   /**< Current Time Zone */
    uint8_t dst;                        /**< Daylight Saving Time value */
} cy_stc_ble_cts_local_time_info_t;

/** Reference Time Information Characteristic structure */
typedef struct
{
    uint8_t timeSource;                 /**< Time update source */
    uint8_t timeAccuracy;               /**< Time accuracy */
    uint8_t daysSinceUpdate;            /**< Days since last time update */
    uint8_t hoursSinseUpdate;           /**< Hours since last time update */
} cy_stc_ble_cts_reference_time_info_t;

/** Current Time Service Characteristic Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;      /**< Connection handle */
    cy_en_ble_cts_char_index_t charIndex;       /**< Characteristic index of Current Time Service */
    cy_en_ble_gatt_err_code_t  gattErrorCode;   /**< GATT error code for access control */
    cy_stc_ble_gatt_value_t    *value;          /**< Pointer to value of Current Time Service characteristic */
} cy_stc_ble_cts_char_value_t;

/** Current Time Service Characteristic Descriptor Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t         connHandle; /**< Connection handle */
    cy_en_ble_cts_char_index_t       charIndex;  /**< Characteristic index of Current Time Service */
    cy_en_ble_cts_char_descriptors_t descrIndex; /**< Characteristic index Descriptor of Current Time Service */
    cy_stc_ble_gatt_value_t          *value;     /**< Pointer to value of Current Time Service characteristic */
} cy_stc_ble_cts_descr_value_t;

/** Structure with Current Time Service attribute handles */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t serviceHandle;              /**< Current Time Service handle */
    cy_ble_gatt_db_attr_handle_t currTimeCharHandle;         /**< Current Time Characteristic handle */
    cy_ble_gatt_db_attr_handle_t currTimeCccdHandle;         /**< Current Time Client Characteristic Configuration
                                                              * Characteristic handle */
    cy_ble_gatt_db_attr_handle_t localTimeInfCharHandle;     /**< Local Time Information Characteristic handle */
    cy_ble_gatt_db_attr_handle_t refTimeInfCharHandle;       /**< Reference Time Information Characteristic handle */
} cy_stc_ble_ctss_t;

/** Structure with discovered attributes information of Current Time Service */
typedef struct
{
    /** Structure with Characteristic handles + properties of Current Time Service */
    cy_stc_ble_srvr_char_info_t  currTimeCharacteristics[CY_BLE_CTS_CHAR_COUNT];

    /** Current Time Client Characteristic Configuration handle of Current Time Service */
    cy_ble_gatt_db_attr_handle_t currTimeCccdHandle;
} cy_stc_ble_ctsc_t;

/** Service configuration structure */
typedef struct
{
    /** Current Time Service GATT DB handles structure */
    const cy_stc_ble_ctss_t *ctss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_cts_config_t;

/** @} */


/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_CTS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_CTS_Init(cy_stc_ble_cts_config_t *config);
cy_en_ble_api_result_t Cy_BLE_CTS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

/* GATT Server API functions */
#ifdef CY_BLE_CTS_SERVER
/**
 * \addtogroup group_ble_service_api_CTS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_CTSS_SetCharacteristicValue(cy_en_ble_cts_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CTSS_GetCharacteristicValue(cy_en_ble_cts_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CTSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_cts_char_index_t charIndex,
                                                               cy_en_ble_cts_char_descriptors_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CTSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_cts_char_index_t charIndex, uint8_t attrSize,
                                                    uint8_t *attrValue);

/** @} */
#endif /* (CY_BLE_CTS_SERVER) */

/* GATT Client API functions */
#ifdef CY_BLE_CTS_CLIENT
/**
 * \addtogroup group_ble_service_api_CTS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_CTSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_cts_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CTSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_cts_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_CTSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_cts_char_index_t charIndex,
                                                               cy_en_ble_cts_char_descriptors_t descrIndex,
                                                               uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_CTSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_cts_char_index_t charIndex, uint8_t descrIndex);

/** @} */
#endif /* (CY_BLE_CTS_CLIENT) */


/***************************************
* API Constants
***************************************/

#define CY_BLE_CTS_CURRENT_TIME_SIZE              (10u)
#define CY_BLE_CTS_LOCAL_TIME_INFO_SIZE           (2u)
#define CY_BLE_CTS_REFERENCE_TIME_INFO_SIZE       (4u)
#define CY_BLE_CTS_CLIENT_CONFIG_DESCR_SIZE       (2u)

/* CTS GATT error codes */
#define CY_BLE_GATT_ERR_CTS_DATA_FIELD_IGNORED    ((cy_en_ble_gatt_err_code_t)(0x80u))


/***************************************
* Current Time ranges
***************************************/

/* The range of "Year" is from 1582 to 9999. "Year" is split into two bytes and
 * the following values define the maximum and minimum allowed years. */
#define CY_BLE_CTS_YEAR_MAX                     (9999u)
#define CY_BLE_CTS_YEAR_MIN                     (1582u)

/* Range of "Month" is 0 to 12  */
#define CY_BLE_CTS_MONTH_MAX                    (12u)

/* Range of "Month" is 0 to 31  */
#define CY_BLE_CTS_DAY_MAX                      (31u)

/* Range of "Hours" is 0 to 24  */
#define CY_BLE_CTS_HOURS_MAX                    (23u)

/* Range of "Minutes" is 0 to 59  */
#define CY_BLE_CTS_MINUTES_MAX                  (59u)

/* Range of "Seconds" is 0 to 59  */
#define CY_BLE_CTS_SECONDS_MAX                  (CY_BLE_CTS_MINUTES_MAX)

/* Range of "dayOfWeek" is 1 to 7  */
#define CY_BLE_CTS_DAY_OF_WEEK_MAX              (7u)

/* Current Time update reasons */
#define CY_BLE_CTS_REASON_UNKNOWN               (0u)
#define CY_BLE_CTS_MANUAL_UPDATE                (1u)
#define CY_BLE_CTS_EXTERNAL_REF_UPDATE          (2u)
#define CY_BLE_CTS_TIME_ZONE_CHANGE             (4u)
#define CY_BLE_CTS_DST_CHANGE                   (8u)

#define CY_BLE_CTS_MAX_DAYS_SINCE_UPDATE        (255u)
#define CY_BLE_CTS_MAX_HOURS_SINCE_UPDATE       (CY_BLE_CTS_MAX_DAYS_SINCE_UPDATE)

/* DST offset values */
#define CY_BLE_CTS_STANDARD_TIME                (0u)
#define CY_BLE_CTS_DAYLIGHT_TIME_0_5            (2u)
#define CY_BLE_CTS_DAYLIGHT_TIME_1_0            (4u)
#define CY_BLE_CTS_DAYLIGHT_TIME_2_0            (8u)
#define CY_BLE_CTS_DST_UNKNOWN                  (255u)

#define CY_BLE_CTS_UTC_OFFSET_MAX               (56)
#define CY_BLE_CTS_UTC_OFFSET_MIN               (-48)
#define CY_BLE_CTS_UTC_OFFSET_UNKNOWN           (-127)

/* Time update source constants */
#define CY_BLE_CTS_TIME_SRC_UNKNOWN             (0u)
#define CY_BLE_CTS_TIME_SRC_NTP                 (1u)
#define CY_BLE_CTS_TIME_SRC_GPS                 (2u)
#define CY_BLE_CTS_TIME_SRC_RADIO               (3u)
#define CY_BLE_CTS_TIME_SRC_MANUAL              (4u)
#define CY_BLE_CTS_TIME_SRC_ATOMIC_CLK          (5u)
#define CY_BLE_CTS_TIME_SRC_CELL_NET            (6u)

/* Client Characteristic Configuration Descriptor constants */
#define CY_BLE_CTS_CLIENT_CONFIG_MASK           (0x03u)
#define CY_BLE_CTS_CLIENT_CONFIG_NTF_ENABLED    (0x01u)
#define CY_BLE_CTS_CLIENT_CONFIG_IND_ENABLED    (0x02u)

#define CY_BLE_CTS_INVALID_CHAR_INDEX           (0xFFu)

#define CY_BLE_CTS_8_BIT_OFFSET                 (8u)

#ifdef CY_BLE_CTS_SERVER

/* Performs validation of 'adjReason' if it has invalid value, it will be set to
 * "Unknown" reason. */
#define CY_BLE_CTS_CHECK_ADJUST_REASON(adjReason)    ((adjReason) =                                            \
                                                          (((adjReason) == CY_BLE_CTS_REASON_UNKNOWN) ?        \
                                                           CY_BLE_CTS_REASON_UNKNOWN :                         \
                                                           (((adjReason) == CY_BLE_CTS_MANUAL_UPDATE) ?        \
                                                            CY_BLE_CTS_MANUAL_UPDATE :                         \
                                                            (((adjReason) == CY_BLE_CTS_EXTERNAL_REF_UPDATE) ? \
                                                             CY_BLE_CTS_EXTERNAL_REF_UPDATE :                  \
                                                             (((adjReason) == CY_BLE_CTS_TIME_ZONE_CHANGE) ?   \
                                                              CY_BLE_CTS_TIME_ZONE_CHANGE :                    \
                                                              (((adjReason) == CY_BLE_CTS_DST_CHANGE) ?        \
                                                               CY_BLE_CTS_DST_CHANGE :                         \
                                                               CY_BLE_CTS_REASON_UNKNOWN))))))

#endif /* (CY_BLE_CTS_SERVER) */


/***************************************
* Variables with external linkage
***************************************/
#ifdef CY_BLE_CTS_CLIENT
extern cy_stc_ble_ctsc_t cy_ble_ctsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_CTS_CLIENT) */

extern cy_stc_ble_cts_config_t cy_ble_ctsConfig;
extern cy_stc_ble_cts_config_t *cy_ble_ctsConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_CTS) */
#endif /* CY_BLE_CTS_H */


/* [] END OF FILE */
