/***************************************************************************//**
* \file CY_BLE_aios.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for the Automation Input
*  Output Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CY_BLE_AIOS_H
#define CY_BLE_AIOS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_AIOS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Conditional Compilation Parameters
***************************************/

#define CY_BLE_AIO_TOTAL_CHAR_COUNT    (CY_BLE_CONFIG_AIO_TOTAL_CHAR_COUNT)

/**
 * \addtogroup group_ble_service_api_AIOS_definitions
 * @{
 */

/***************************************
* Data Struct Definition
***************************************/

/** AIOS Characteristic indexes */
typedef enum
{
    CY_BLE_AIOS_DIGITAL,                          /**< AIOS Digital characteristic*/
    CY_BLE_AIOS_ANALOG,                           /**< AIOS Analog characteristic*/
    CY_BLE_AIOS_AGGREGATE,                        /**< AIOS Agrigate characteristic*/
    CY_BLE_AIOS_CHAR_COUNT                        /**< Total count of AIOS characteristics */
}cy_en_ble_aios_char_index_t;

/** AIOS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_AIOS_CCCD,                             /**< Client Characteristic Configuration Descriptor index */
    CY_BLE_AIOS_CHAR_PRESENTATION_FORMAT,         /**< Characteristic Presentation Format Descriptor index */
    CY_BLE_AIOS_CHAR_USER_DESCRIPTION_DESCR,      /**< Characteristic User Description Descriptor index */
    CY_BLE_AIOS_CHAR_EXTENDED_PROPERTIES,         /**< Characteristic Extended Properties Descriptor index */
    CY_BLE_AIOS_VALUE_TRIGGER_SETTINGS,           /**< AIO Value Trigger Settings Descriptor index */
    CY_BLE_AIOS_TIME_TRIGGER_SETTINGS,            /**< AIO Time Trigger Settings Descriptor index */
    CY_BLE_AIOS_VRD,                              /**< Valid Range Descriptor index */
    CY_BLE_AIOS_NUM_OF_DIGITAL_DESCR = 0x06,      /**< Number of Digitals Descriptor index */
    CY_BLE_AIOS_DESCR_COUNT                       /**< Total count of descriptors */
}cy_en_ble_aios_descr_index_t;

/** AIOS Characteristic with descriptors */
typedef struct
{
    /** Handles of Characteristic value */
    cy_ble_gatt_db_attr_handle_t charHandle;

    /** Array of Descriptor handles */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_AIOS_DESCR_COUNT];
} cy_stc_ble_aioss_char_t;

/** Structure to hold pointer to cy_stc_ble_aioss_char_t */
typedef struct
{
    /** Pointer to cy_stc_ble_aioss_char_t that holds information about specific
     * AIO Characteristic
     */
    cy_stc_ble_aioss_char_t *charInfoPtr;
} cy_stc_ble_aioss_char_info_ptr_t;

/** Structure with Automation Input Output Service attribute handles */
typedef struct
{
    /** Automation Input Output Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** Automation Input Output Service Array with pointers to
     * Characteristic handles.
     */
    cy_stc_ble_aioss_char_info_ptr_t charInfoAddr[CY_BLE_AIOS_CHAR_COUNT];
} cy_stc_ble_aioss_t;

/** AIOS Characteristic with descriptors */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t valueHandle;                          /**< Handle of characteristic value */
    cy_ble_gatt_db_attr_handle_t endHandle;                            /**< End handle of characteristic */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_AIOS_DESCR_COUNT]; /**< Array of Descriptor handles */
    uint8_t                      properties;                           /**< Properties for value field */
} cy_stc_ble_aiosc_char_t;

/** Structure to hold pointer to cy_stc_ble_aiosc_char_t */
typedef struct
{
    /** Pointer to cy_stc_ble_aiosc_char_t that holds information about specific AIO Characteristic. */
    cy_stc_ble_aiosc_char_t *charInfoPtr;
} cy_stc_ble_aiosc_char_info_ptr_t;

/** Structure with discovered attributes information of Automation Input Output Service. */
typedef struct
{
    /** Automation Input Output Service handle */
    cy_ble_gatt_db_attr_handle_t     serviceHandle;

    /** Automation Input Output Service Array with pointers to characteristic information. */
    cy_stc_ble_aiosc_char_info_ptr_t charInfoAddr[CY_BLE_AIOS_CHAR_COUNT];
} cy_stc_ble_aiosc_t;

/** AIOS Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;                     /**< Peer device handle */
    cy_en_ble_aios_char_index_t charIndex;                      /**< Index of service characteristic */
    uint8_t                     charInstance;                   /**< Instance of specific service characteristic */
    cy_stc_ble_gatt_value_t     *value;                         /**< Characteristic value */
    cy_en_ble_gatt_err_code_t   gattErrorCode;                  /**< GATT error code for access control */
} cy_stc_ble_aios_char_value_t;

/** AIOS Characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t     connHandle;                    /**< Peer device handle */
    cy_en_ble_aios_char_index_t  charIndex;                     /**< Index of service characteristic */
    uint8_t                      charInstance;                  /**< Instance of specific service characteristic */
    cy_en_ble_aios_descr_index_t descrIndex;                    /**< Index of descriptor */
    cy_en_ble_gatt_err_code_t    gattErrorCode;                 /**< Error code received from application (optional) */
    cy_stc_ble_gatt_value_t      *value;                        /**< Characteristic value */
} cy_stc_ble_aios_descr_value_t;

/** Service configuration structure */
typedef struct
{
    /** Automation Input Output Service GATT DB handles structure for server */
    const cy_stc_ble_aioss_t *aioss;

    /** Number of AIOS characteristics instances for server */
    uint8_t                  *aiossCharInstances;

    /** Automation Input Output Service GATT DB handles structure for client */
    cy_stc_ble_aiosc_t       *aiosc;

    /** Number of AIOS characteristics instances for client */
    const uint8_t            *aioscCharInstances;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t        callbackFunc;
} cy_stc_ble_aios_config_t;


/***************************************
* API Constants
***************************************/

/** Trigger condition value not supported.*/
#define CY_BLE_GATT_ERR_TRIGGER_CODITION_VALUE_NOT_SUPPORTED    (0x80u)

/** @} */

#ifdef CY_BLE_AIOS_SERVER

#define CY_BLE_AIOS_UNDEFINED_ITEM                   (0x00u)
#define CY_BLE_AIOS_CHARACTERISTIC_ITEM              (0x01u)
#define CY_BLE_AIOS_DESCRIPTOR_ITEM                  (0x02u)

#define CY_BLE_AIOS_CHAR_PRESENTATION_FORMAT_LEN     (0x07u)
#define CY_BLE_AIOS_NUM_OF_DIGITAL_DESCR_LEN         (0x01u)
#define CY_BLE_AIOS_ANALOG_LEN                       (0x02u)

#define CY_BLE_AIOS_VALUE_TRIGGER_DESCR_MAX_VALUE    (0x07u)
#define CY_BLE_AIOS_TIME_TRIGGER_DESCR_MAX_VALUE     (0x03u)

#define CY_BLE_AIOS_MAX_CHAR_DESCRIPTION_VALUE       (0xFFFFu)

#endif /* CY_BLE_AIOS_SERVER */



/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_AIOS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_AIOS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_AIOS_SERVER
/**
 * \addtogroup group_ble_service_api_AIOS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_AIOSS_SetCharacteristicValue(cy_en_ble_aios_char_index_t charIndex,
                                                           uint8_t charInstance, uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_AIOSS_GetCharacteristicValue(cy_en_ble_aios_char_index_t charIndex,
                                                           uint8_t charInstance, uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_AIOSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_aios_char_index_t charIndex, uint8_t charInstance,
                                                                cy_en_ble_aios_descr_index_t descrIndex,
                                                                uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_AIOSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_aios_char_index_t charIndex, uint8_t charInstance,
                                                                cy_en_ble_aios_descr_index_t descrIndex,
                                                                uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_AIOSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     cy_en_ble_aios_char_index_t charIndex, uint8_t charInstance, uint8_t
                                                     attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_AIOSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                   cy_en_ble_aios_char_index_t charIndex, uint8_t charInstance,
                                                   uint8_t attrSize, uint8_t *attrValue);

#endif /* CY_BLE_AIOS_SERVER */

/** @} */

#ifdef CY_BLE_AIOS_CLIENT
/**
 * \addtogroup group_ble_service_api_AIOS_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_AIOSC_SetCharacteristicValueWithoutResponse(cy_stc_ble_conn_handle_t connHandle,
                                                                          cy_en_ble_aios_char_index_t charIndex, uint8_t
                                                                          charInstance, uint8_t attrSize,
                                                                          uint8_t * attrValue);

cy_en_ble_api_result_t Cy_BLE_AIOSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_aios_char_index_t charIndex, uint8_t charInstance,
                                                           uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_AIOSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_aios_char_index_t charIndex, uint8_t charInstance);

cy_en_ble_api_result_t Cy_BLE_AIOSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_aios_char_index_t charIndex, uint8_t charInstance,
                                                                cy_en_ble_aios_descr_index_t descrIndex,
                                                                uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_AIOSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_aios_char_index_t charIndex, uint8_t charInstance,
                                                                cy_en_ble_aios_descr_index_t descrIndex);

/** @} */
#endif /* CY_BLE_AIOS_CLIENT */


/***************************************
* Private Function Prototypes
***************************************/

/** \cond IGNORE */
cy_en_ble_api_result_t Cy_BLE_AIOS_Init(cy_stc_ble_aios_config_t *config);

/** \endcond */



/***************************************
* External data references
***************************************/

#ifdef CY_BLE_AIOS_CLIENT
extern uint8_t cy_ble_aioscActiveCharIndex[CY_BLE_CONFIG_GATTC_COUNT];
extern uint8_t cy_ble_aioscActiveCharInstance[CY_BLE_CONFIG_GATTC_COUNT];
extern uint8_t cy_ble_aioscPrevCharInstIndex[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_AIOS_CLIENT) */

extern cy_stc_ble_aios_config_t cy_ble_aiosConfig;
extern cy_stc_ble_aios_config_t *cy_ble_aiosConfigPtr;

/***************************************
* Macro Functions
***************************************/

#define Cy_BLE_AIOS_Get16ByPtr    Cy_BLE_Get16ByPtr

#ifdef CY_BLE_AIOS_CLIENT

#define Cy_BLE_AIOSC_GetCharacteristicValueHandle(connHandle, charIndex, charInstance)                                      \
    ((((charIndex) >= CY_BLE_AIOS_CHAR_COUNT) || ((charInstance) >= cy_ble_aiosConfigPtr->aiossCharInstances[charIndex])) ? \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                                                                                \
     cy_ble_aiosConfigPtr->aiosc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfoPtr[charInstance].valueHandle)

#define Cy_BLE_AIOSC_GetCharacteristicDescriptorHandle(connHandle, charIndex, charInstance, descrIndex)                     \
    ((((charIndex) >= CY_BLE_AIOS_CHAR_COUNT) || ((charInstance) >= cy_ble_aiosConfigPtr->aiossCharInstances[charIndex]) || \
      ((descrIndex) >= CY_BLE_AIOS_DESCR_COUNT)) ?  CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                                 \
     cy_ble_aiosConfigPtr->aiosc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfoAddr[charIndex].                               \
      charInfoPtr[charInstance].descrHandle[descrIndex])

#endif /* (CY_BLE_AIOS_CLIENT) */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_AIOS) */
#endif /* CY_BLE_AIOS_H */


/* [] END OF FILE */
