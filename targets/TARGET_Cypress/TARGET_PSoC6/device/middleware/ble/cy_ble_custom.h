/***************************************************************************//**
* \file cy_ble_custom.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for the Custom Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_CUSTOM_H
#define CY_BLE_CUSTOM_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_CUSTOM))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Conditional Compilation Parameters
***************************************/

/* Maximum supported Custom Services */
#define CY_BLE_CUSTOMS_SERVICE_COUNT                    (CY_BLE_CONFIG_CUSTOMS_SERVICE_COUNT)
#define CY_BLE_CUSTOMC_SERVICE_COUNT                    (CY_BLE_CONFIG_CUSTOMC_SERVICE_COUNT)
#define CY_BLE_CUSTOM_SERVICE_CHAR_COUNT                (CY_BLE_CONFIG_CUSTOM_SERVICE_CHAR_COUNT)
#define CY_BLE_CUSTOM_SERVICE_CHAR_DESCRIPTORS_COUNT    (CY_BLE_CONFIG_CUSTOM_SERVICE_CHAR_DESCRIPTORS_COUNT)

#if (CY_BLE_CUSTOMS_SERVICE_COUNT != 0u)
    #define CY_BLE_CUSTOM_SERVER
#endif /* (CY_BLE_CUSTOMS_SERVICE_COUNT != 0u) */

#if (CY_BLE_CUSTOMC_SERVICE_COUNT != 0u)
    #define CY_BLE_CUSTOM_CLIENT
#endif /* (CY_BLE_CUSTOMC_SERVICE_COUNT != 0u) */

/***************************************
* Data Types
***************************************/

/**
 * \addtogroup group_ble_service_api_custom
 * @{
 */

#ifdef CY_BLE_CUSTOM_SERVER

/** Contains information about Custom Characteristic structure */
typedef struct
{
    /** Custom Characteristic handle */
    cy_ble_gatt_db_attr_handle_t customServCharHandle;

    /** Custom Characteristic Descriptors handles */
    cy_ble_gatt_db_attr_handle_t customServCharDesc[     /* MDK doesn't allow array with zero length */
        (CY_BLE_CUSTOM_SERVICE_CHAR_DESCRIPTORS_COUNT == 0u) ? 1u : CY_BLE_CUSTOM_SERVICE_CHAR_DESCRIPTORS_COUNT];
} cy_stc_ble_customs_info_t;

/** Structure with Custom Service attribute handles. */
typedef struct
{
    /** Handle of a Custom Service */
    cy_ble_gatt_db_attr_handle_t customServHandle;

    /** Information about Custom Characteristics */
    cy_stc_ble_customs_info_t    customServInfo[        /* MDK doesn't allow array with zero length */
        (CY_BLE_CUSTOM_SERVICE_CHAR_COUNT == 0u ) ? 1u : CY_BLE_CUSTOM_SERVICE_CHAR_COUNT];
} cy_stc_ble_customs_t;

#endif /* CY_BLE_CUSTOM_SERVER */

#ifdef CY_BLE_CUSTOM_CLIENT

/** Structure with discovered attributes information of Custom Service Descriptors */
typedef struct
{
    /** Custom Descriptor handle */
    cy_ble_gatt_db_attr_handle_t descHandle[CY_BLE_CONFIG_GATTC_COUNT];

    /** Custom Descriptor 128 bit UUID */
    const void                   *uuid;

    /** UUID Format - 16-bit (0x01) or 128-bit (0x02) */
    uint8_t                      uuidFormat;
} cy_stc_ble_customc_desc_t;

/** Structure with discovered attributes information of Custom Service Characteristics */
typedef struct
{
    /** Characteristic handle */
    cy_ble_gatt_db_attr_handle_t customServCharHandle[CY_BLE_CONFIG_GATTC_COUNT];

    /** Characteristic end handle */
    cy_ble_gatt_db_attr_handle_t customServCharEndHandle[CY_BLE_CONFIG_GATTC_COUNT];

    /** Custom Characteristic UUID */
    const void                   *uuid;

    /** UUID Format - 16-bit (0x01) or 128-bit (0x02) */
    uint8_t                      uuidFormat;

    /** Properties for value field */
    uint8_t                      properties[CY_BLE_CONFIG_GATTC_COUNT];

    /** Number of descriptors */
    uint8_t                      descCount;

    /** Characteristic Descriptors */
    cy_stc_ble_customc_desc_t    * customServCharDesc;
} cy_stc_ble_customc_char_t;

/** Structure with discovered attributes information of Custom Service */
typedef struct
{
    /** Custom Service handle */
    cy_ble_gatt_db_attr_handle_t customServHandle[CY_BLE_CONFIG_GATTC_COUNT];

    /** Custom Service UUID */
    const void                   *uuid;

    /** UUID Format - 16-bit (0x01) or 128-bit (0x02) */
    uint8_t                      uuidFormat;

    /** Number of characteristics */
    uint8_t                      charCount;

    /** Custom Service Characteristics */
    cy_stc_ble_customc_char_t    * customServChar;
} cy_stc_ble_customc_t;

#endif /* CY_BLE_CUSTOM_CLIENT */

/** Service configuration structure */
typedef struct
{
#ifdef CY_BLE_CUSTOM_SERVER
    /** Custom Services GATT DB handles structure */
    const cy_stc_ble_customs_t *customs;
#endif /* CY_BLE_CUSTOM_SERVER */

#ifdef CY_BLE_CUSTOM_CLIENT
    /** Custom Service handle */
    cy_stc_ble_customc_t *customc;
#endif /* CY_BLE_CUSTOM_CLIENT */
} cy_stc_ble_custom_config_t;

/** @} */

/***************************************
* Function Prototypes
***************************************/
cy_en_ble_api_result_t Cy_BLE_CUSTOM_Init(cy_stc_ble_custom_config_t *config);


/***************************************
* External data references
***************************************/
/** \cond IGNORE */
#ifdef CY_BLE_CUSTOM_CLIENT
extern cy_stc_ble_customc_t cy_ble_customc[CY_BLE_CUSTOMC_SERVICE_COUNT];
extern cy_stc_ble_customc_t cy_ble_customCServ[CY_BLE_CUSTOMC_SERVICE_COUNT];
#endif /* (CY_BLE_CUSTOM_CLIENT) */

extern cy_stc_ble_custom_config_t cy_ble_customConfig;
extern cy_stc_ble_custom_config_t *cy_ble_customConfigPtr;

/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/
#define customServiceCharHandle         customServCharHandle
#define customServiceCharDescriptors    customServCharDesc
#define customServiceHandle             customServHandle
#define customServiceInfo               customServInfo
/** \endcond */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_CUSTOM) */
#endif /* CY_BLE_CUSTOM_H */

/* [] END OF FILE */
