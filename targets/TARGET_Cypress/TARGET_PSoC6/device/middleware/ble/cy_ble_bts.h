/***************************************************************************//**
* \file cy_ble_bts.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for the Bootloader Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_BTS_H
#define CY_BLE_BTS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_BTS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Conditional Compilation Parameters
***************************************/

/* Maximum supported Bootload Services */
#define CY_BLE_BT_SERVICE_COUNT    (0x01u)


/***************************************
* Data Types
***************************************/
/** BTS Characteristic indexes */
typedef enum
{
    CY_BLE_BTS_BT_SERVICE,           /**< Bootloader service characteristic index */
    CY_BLE_BTS_CHAR_COUNT            /**< Total count of characteristics */
}cy_en_ble_bts_char_index_t;

/** BTS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_BTS_BT_SERVICE_CCCD,      /**< Client Characteristic Configuration descriptor index */
    CY_BLE_BTS_DESCR_COUNT           /**< Total count of descriptors */
}cy_en_ble_bts_descr_index_t;

/** Contains information about the Bootloader Characteristic structure */
typedef struct
{
    /** Bootloader Characteristic handle */
    cy_ble_gatt_db_attr_handle_t btServiceCharHandle;

    /** Bootloader Characteristic Descriptors handles */
    cy_ble_gatt_db_attr_handle_t btServiceCharDescriptors[CY_BLE_BTS_DESCR_COUNT];
} cy_stc_ble_bts_info_t;

/** Structure with Bootloader Service attribute handles. */
typedef struct
{
    /** Handle of a Bootloader Service */
    cy_ble_gatt_db_attr_handle_t btServiceHandle;

    /** Information about Bootloader Characteristics */
    cy_stc_ble_bts_info_t        btServiceInfo[CY_BLE_BTS_CHAR_COUNT];
} cy_stc_ble_btss_t;

/** Service configuration structure */
typedef struct
{
    /** Bootloader Service GATT DB handles structure */
    const cy_stc_ble_btss_t *btss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_bts_config_t;

/** BTS Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;          /**< Peer device handle */
    cy_en_ble_bts_char_index_t charIndex;           /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t    *value;              /**< Characteristic value */
} cy_stc_ble_bts_char_value_t;

/***************************************
* API Constants
***************************************/


/***************************************
* Function Prototypes
***************************************/
cy_en_ble_api_result_t Cy_BLE_BTS_Init(cy_stc_ble_bts_config_t *config);

cy_en_ble_api_result_t Cy_BLE_BTSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_bts_char_index_t charIndex, uint32_t attrSize, const
                                                    uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_BTS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);

/***************************************
* External data references
***************************************/
extern uint8_t cy_ble_cmdReceivedFlag;
extern uint16_t cy_ble_cmdLength;
extern uint8_t *cy_ble_btsBuffPtr;

extern cy_stc_ble_bts_config_t cy_ble_btsConfig;
extern cy_stc_ble_bts_config_t *cy_ble_btsConfigPtr;
extern cy_stc_ble_conn_handle_t btConnHandle;
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_BTS) */
#endif /* CY_BLE_BTS_H */


/* [] END OF FILE */
