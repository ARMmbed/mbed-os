/***************************************************************************//**
* \file cy_ble_ans.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for the Alert Notification
*  Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_ANS_H
#define CY_BLE_ANS_H

#include "cy_ble_gatt.h"

#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_ANS))

/* The C binding of definitions if building with the C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Data Types
***************************************/

/**
 * \addtogroup group_ble_service_api_ANS_definitions
 * @{
 */

/** ANS Characteristic indexes */
typedef enum
{
    CY_BLE_ANS_SUPPORTED_NEW_ALERT_CAT,              /**< Supported New Alert Category Characteristic index */
    CY_BLE_ANS_NEW_ALERT,                            /**< New Alert Characteristic index */
    CY_BLE_ANS_SUPPORTED_UNREAD_ALERT_CAT,           /**< Supported Unread Alert Category Characteristic index */
    CY_BLE_ANS_UNREAD_ALERT_STATUS,                  /**< Unread Alert Status Characteristic index */
    CY_BLE_ANS_ALERT_NTF_CONTROL_POINT,              /**< Alert Notification Control Point Characteristic index */
    CY_BLE_ANS_CHAR_COUNT                            /**< Total count of ANS characteristics */
}cy_en_ble_ans_char_index_t;

/** ANS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_ANS_CCCD,                                  /**< Client Characteristic Configuration Descriptor index */
    CY_BLE_ANS_DESCR_COUNT                            /**< Total count of descriptors */
}cy_en_ble_ans_descr_index_t;

/** Alert Notification Service Characteristic Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t   connHandle;            /**< Peer device handle */
    cy_en_ble_ans_char_index_t charIndex;             /**< Index of Alert Notification Service Characteristic */
    cy_stc_ble_gatt_value_t    *value;                /**< Pointer to Characteristic value */
} cy_stc_ble_ans_char_value_t;

/** Alert Notification Service Characteristic Descriptor Value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;           /**< Connection handle */
    cy_en_ble_ans_char_index_t  charIndex;            /**< Characteristic index of Service */
    cy_en_ble_ans_descr_index_t descrIndex;           /**< Service Characteristic Descriptor index */
    cy_stc_ble_gatt_value_t     *value;               /**< The pointer to the value of the Service Characteristic
                                                       *   Descriptor value */
} cy_stc_ble_ans_descr_value_t;

/** ANS Characteristic with descriptors */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t charHandle;                          /**< Handle of Characteristic value */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_ANS_DESCR_COUNT]; /**< Handle of Descriptor */
} cy_stc_ble_anss_char_t;

/** The structure with the Alert Notification Service attribute handles */
typedef struct
{
    /** The Alert Notification Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** The array of the Alert Notification Service Characteristics + Descriptors handles */
    cy_stc_ble_anss_char_t       charInfo[CY_BLE_ANS_CHAR_COUNT];
} cy_stc_ble_anss_t;

/** Service Full characteristic information type */
typedef struct
{
    /** Characteristic handle + properties */
    cy_stc_ble_srvr_char_info_t  charInfo;

    /** End handle of the characteristic */
    cy_ble_gatt_db_attr_handle_t endHandle;

    /** Characteristic descriptors handles */
    cy_ble_gatt_db_attr_handle_t descriptors[CY_BLE_ANS_DESCR_COUNT];
} cy_stc_ble_srvr_full_char_info_t;

/** The structure with discovered attributes information of the Alert Notification Service */
typedef struct
{
    /** The structure with Characteristic handles + properties of the Alert Notification Service */
    cy_stc_ble_srvr_full_char_info_t characteristics[CY_BLE_ANS_CHAR_COUNT];
} cy_stc_ble_ansc_t;

/** Service configuration structure */
typedef struct
{
    /** Service GATT DB handles structure */
    const cy_stc_ble_anss_t *anss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t       callbackFunc;
} cy_stc_ble_ans_config_t;

/** @} */


/***************************************
* API Constants
***************************************/

#define CY_BLE_ANS_INVALID_CHAR_INDEX              (0xFFu)

/** Alert Notification Characteristic Command ID */
#define CY_BLE_ANS_EN_NEW_ALERT_NTF                (0x00u) /** Enable New Incoming Alert Notification */
#define CY_BLE_ANS_EN_UNREAD_ALERT_STATUS_NTF      (0x01u) /** Enable Unread Category Status Notification */
#define CY_BLE_ANS_DIS_NEW_ALERT_NTF               (0x02u) /** Disable New Incoming Alert Notification */
#define CY_BLE_ANS_DIS_UNREAD_ALERT_STATUS_NTF     (0x03u) /** Disable Unread Category Status Notification */
#define CY_BLE_ANS_IMM_NEW_ALERT_NTF               (0x04u) /** Notify New Incoming Alert immediately */
#define CY_BLE_ANS_IMM_UNREAD_ALERT_STATUS_NTF     (0x05u) /** Notify Unread Category Status immediately */

/* Alert Notification Characteristic Category ID */
#define CY_BLE_ANS_CAT_ID_SIMPLE_ALERT             (0x00u) /* Simple Alert: General text alert or non-text alert */
#define CY_BLE_ANS_CAT_ID_EMAIL                    (0x01u) /* Email: Alert when Email messages arrives */
#define CY_BLE_ANS_CAT_ID_NEWS                     (0x02u) /* News: News feeds such as RSS, Atom */
#define CY_BLE_ANS_CAT_ID_CALL                     (0x03u) /* Call: Incoming call */
#define CY_BLE_ANS_CAT_ID_MISSED_CALL              (0x04u) /* Missed call: Missed Call */
#define CY_BLE_ANS_CAT_ID_SMS_MMS                  (0x05u) /* SMS/MMS: SMS/MMS message arrives */
#define CY_BLE_ANS_CAT_ID_VOICE_MAIL               (0x06u) /* Voice mail: Voice mail */
#define CY_BLE_ANS_CAT_ID_SCHEDULE                 (0x07u) /* Schedule: Alert occurred on calendar, planner */
#define CY_BLE_ANS_CAT_ID_HI_PRFIORITIZED_ALERT    (0x08u) /* High Prioritized Alert: Alert that should be handled as
                                                            * high priority */
#define CY_BLE_ANS_CAT_ID_INSTANT_MESSAGE          (0x09u) /* Instant Message: Alert for incoming instant messages */

#define CY_BLE_ANS_CAT_ID_ALL                      (0xFFu) /* Category ID - All categories */

#define CY_BLE_ANS_CHAR_VALUE_OFFSET               (0x02u)


/***************************************
* Function Prototypes
***************************************/

/**
 * \addtogroup group_ble_service_api_ANS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_ANS_Init(cy_stc_ble_ans_config_t *config);
cy_en_ble_api_result_t Cy_BLE_ANS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);
/** @} */

#ifdef CY_BLE_ANS_SERVER
/**
 * \addtogroup group_ble_service_api_ANS_server
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_ANSS_SetCharacteristicValue(cy_en_ble_ans_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ANSS_GetCharacteristicValue(cy_en_ble_ans_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ANSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ans_char_index_t charIndex,
                                                               cy_en_ble_ans_descr_index_t
                                                               descrIndex, uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ANSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                    cy_en_ble_ans_char_index_t charIndex, uint8_t attrSize,
                                                    uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_ANS_SERVER */

#ifdef CY_BLE_ANS_CLIENT
/**
 * \addtogroup group_ble_service_api_ANS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_ANSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_ans_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_ANSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                          cy_en_ble_ans_char_index_t charIndex, uint8_t attrSize,
                                                          uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ANSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ans_char_index_t charIndex,
                                                               cy_en_ble_ans_descr_index_t
                                                               descrIndex, uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_ANSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                               cy_en_ble_ans_char_index_t charIndex, uint8_t descrIndex);

/** @} */
#endif /* (CY_BLE_ANS_CLIENT) */


/***************************************
* External data references
***************************************/
#ifdef CY_BLE_ANS_CLIENT
extern cy_stc_ble_ansc_t cy_ble_ansc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* (CY_BLE_ANS_CLIENT) */

extern cy_stc_ble_ans_config_t cy_ble_ansConfig;
extern cy_stc_ble_ans_config_t *cy_ble_ansConfigPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_ANS) */
#endif /* CY_BLE_ANS_H */


/* [] END OF FILE */
