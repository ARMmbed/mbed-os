/***************************************************************************//**
* \file cy_ble_event_handler.h
* \version 2.0
*
* \brief
*  Contains the prototypes and constants used in the Event Handler State Machine
*  of the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef CY_BLE_EVENT_HANDLER_H
#define CY_BLE_EVENT_HANDLER_H

#include "cy_ble_gatt.h"
#include "cy_ble_hal_pvt.h"
#include "flash/cy_flash.h"

#if (CY_BLE_MODE_PROFILE)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
* Service specific includes
***************************************/

#ifdef CY_BLE_ANCS
    #include "cy_ble_ancs.h"
#endif /* CY_BLE_ANCS */
#ifdef CY_BLE_ANS
    #include "cy_ble_ans.h"
#endif /* CY_BLE_ANS */
#ifdef CY_BLE_AIOS
    #include "cy_ble_aios.h"
#endif /* CY_BLE_AIOS */
#ifdef CY_BLE_BAS
    #include "cy_ble_bas.h"
#endif /* CY_BLE_BAS */
#ifdef CY_BLE_BLS
    #include "cy_ble_bls.h"
#endif /* CY_BLE_BLS */
#ifdef CY_BLE_BCS
    #include "cy_ble_bcs.h"
#endif /* CY_BLE_BCS */
#ifdef CY_BLE_BMS
    #include "cy_ble_bms.h"
#endif /* CY_BLE_BMS */
#ifdef CY_BLE_BTS
    #include "cy_ble_bts.h"
#endif /* CY_BLE_BTS */
#ifdef CY_BLE_CGMS
    #include "cy_ble_cgms.h"
#endif /* CY_BLE_CGMS */
#ifdef CY_BLE_CPS
    #include "cy_ble_cps.h"
#endif /* CY_BLE_CPS */
#ifdef CY_BLE_CSCS
    #include "cy_ble_cscs.h"
#endif /* CY_BLE_CSCS */
#ifdef CY_BLE_CTS
    #include "cy_ble_cts.h"
#endif /* CY_BLE_CTS */
#ifdef CY_BLE_CUSTOM
    #include "cy_ble_custom.h"
#endif /* CY_BLE_CUSTOM */
#ifdef CY_BLE_DIS
    #include "cy_ble_dis.h"
#endif /* CY_BLE_DIS */
#ifdef CY_BLE_ESS
    #include "cy_ble_ess.h"
#endif /* CY_BLE_ESS */
#ifdef CY_BLE_GLS
    #include "cy_ble_gls.h"
#endif /* CY_BLE_GLS */
#ifdef CY_BLE_HIDS
    #include "cy_ble_hids.h"
#endif /* CY_BLE_HIDS */
#ifdef CY_BLE_HPS
    #include "cy_ble_hps.h"
#endif /* CY_BLE_HPS */
#ifdef CY_BLE_HRS
    #include "cy_ble_hrs.h"
#endif /* CY_BLE_HRS */
#ifdef CY_BLE_HTS
    #include "cy_ble_hts.h"
#endif /* CY_BLE_HTS */
#ifdef CY_BLE_IAS
    #include "cy_ble_ias.h"
#endif /* CY_BLE_IAS */
#ifdef CY_BLE_IPS
    #include "cy_ble_ips.h"
#endif /* CY_BLE_IPS */
#ifdef CY_BLE_LLS
    #include "cy_ble_lls.h"
#endif /* CY_BLE_LLS */
#ifdef CY_BLE_LNS
    #include "cy_ble_lns.h"
#endif /* CY_BLE_LNS */
#ifdef CY_BLE_NDCS
    #include "cy_ble_ndcs.h"
#endif /* CY_BLE_NDCS */
#ifdef CY_BLE_PASS
    #include "cy_ble_pass.h"
#endif /* CY_BLE_PASS */
#ifdef CY_BLE_PLXS
    #include "cy_ble_plxs.h"
#endif /* CY_BLE_PLXS */
#ifdef CY_BLE_RSCS
    #include "cy_ble_rscs.h"
#endif /* CY_BLE_RSCS */
#ifdef CY_BLE_RTUS
    #include "cy_ble_rtus.h"
#endif /* CY_BLE_RTUS */
#ifdef CY_BLE_SCPS
    #include "cy_ble_scps.h"
#endif /* CY_BLE_SCPS */
#ifdef CY_BLE_TPS
    #include "cy_ble_tps.h"
#endif /* CY_BLE_TPS */
#ifdef CY_BLE_UDS
    #include "cy_ble_uds.h"
#endif /* CY_BLE_UDS */
#ifdef CY_BLE_WPTS
    #include "cy_ble_wpts.h"
#endif /* CY_BLE_WPTS */
#ifdef CY_BLE_WSS
    #include "cy_ble_wss.h"
#endif /* CY_BLE_WSS */



/***************************************
* Definitions
***************************************/
/* cy_ble_eventHandlerFlag defines */
#define CY_BLE_CALLBACK                   (0x01u)
#define CY_BLE_AUTO_DISCOVERY             (0x02u)
#define CY_BLE_START_FLAG                 (0x04u)
#define CY_BLE_STOP_FLAG                  (0x08u)
#define CY_BLE_ENABLE_ALL_EVENTS          (0x10u)
#define CY_BLE_DISABLE_AUTOMATIC_AUTH     (0x20u)

#if (CY_BLE_GATT_ROLE_CLIENT)
#define CY_BLE_DISC_SRVC_INFO_LEN         (4u + CY_BLE_GATT_16_BIT_UUID_SIZE)
#define CY_BLE_DISC_SRVC_INFO_128_LEN     (4u + CY_BLE_GATT_128_BIT_UUID_SIZE)
#define CY_BLE_DISC_INCL_INFO_LEN         (6u + CY_BLE_GATT_16_BIT_UUID_SIZE)
#define CY_BLE_DISC_INCL_INFO_128_LEN     (6u)
#define CY_BLE_DISC_CHAR_INFO_LEN         (5u + CY_BLE_GATT_16_BIT_UUID_SIZE)
#define CY_BLE_DISC_CHAR_INFO_128_LEN     (5u + CY_BLE_GATT_128_BIT_UUID_SIZE)
#define CY_BLE_DISC_DESCR_INFO_LEN        (2u + CY_BLE_GATT_16_BIT_UUID_SIZE)
#define CY_BLE_DISC_DESCR_INFO_128_LEN    (2u + CY_BLE_GATT_128_BIT_UUID_SIZE)
#endif /* CY_BLE_GATT_ROLE_CLIENT */

#define CY_BLE_LE_MASK_LENGTH             (0x8u)
#define CY_BLE_LE_MASK_DEFAULT            (0x003Fu)
#define CY_BLE_LE_MASK_DLE                ((CY_BLE_DLE_FEATURE_ENABLED) ?          (0x0040u) : (0u))
#define CY_BLE_LE_MASK_SECURE_CONN        ((CY_BLE_SECURE_CONN_FEATURE_ENABLED) ?  (0x0180u) : (0u))
#define CY_BLE_LE_MASK_LL_PRIVACY         ((CY_BLE_LL_PRIVACY_FEATURE_ENABLED) ?   (0x0600u) : (0u))
#define CY_BLE_LE_MASK_PHY                ((CY_BLE_PHY_UPDATE_FEATURE_ENABLED) ?   (0x0800u) : (0u))
#define CY_BLE_LE_MASK                    (CY_BLE_LE_MASK_DEFAULT | CY_BLE_LE_MASK_DLE | CY_BLE_LE_MASK_LL_PRIVACY \
                                           | CY_BLE_LE_MASK_SECURE_CONN | CY_BLE_LE_MASK_PHY)
/* Internal command run status defines */
#define CY_BLE_STATUS_SET_TX_PWR_LVL      (uint32_t)(1ul << 0u)

/***************************************
* Type Definitions
***************************************/

/**
 * \addtogroup group_ble_service_api_events
 * @{
 */


/** Service specific events  */
typedef enum
{
    /****************************************
     * Security Events
     ***************************************/
    /* \cond IGNORE */

    CY_BLE_AUTHENTICATION_REQUEST_REPLY = CY_BLE_EVT_MAX + 1, /* 0x10000 */
    CY_BLE_PASSKEY_ENTRY_REQUEST_REPLY,
    CY_BLE_LONG_TERM_KEY_REQUEST_REPLY,
    CY_BLE_KEY_EXCHANGE_INFO_REQUEST_REPLY,
    /* \endcond */

    /****************************************
     * GATT Service Events
     ***************************************/

    /** GATT Server - This event indicates that the CCCD data CRC is wrong.
     *  If this event occurs, removing the bonding information of all devices
     *  by using the Cy_BLE_GAP_RemoveBondedDevice() API is recommended.
     *  The CCCD buffer in the RAM for the current connection is cleaned (set to zero) 
     *  The event parameter is NULL.
     */
    CY_BLE_EVT_GATTS_EVT_CCCD_CORRUPT,

    /** GATT Server - Indication for GATT Service's "Service Changed"
     *  Characteristic was enabled. The parameter of this event is a structure of
     *  cy_stc_ble_gatts_write_cmd_req_param_t type.
     */
    CY_BLE_EVT_GATTS_INDICATION_ENABLED,

    /** GATT Server - Indication for GATT Service's "Service Changed"
     *  Characteristic was disabled. The parameter of this event is a structure of
     *  cy_stc_ble_gatts_write_cmd_req_param_t type.
     */
    CY_BLE_EVT_GATTS_INDICATION_DISABLED,

    /** GATT Client - GATT Service's "Service Changed" Characteristic Indication
     *  was received. The parameter of this event is a structure
     *  of the cy_stc_ble_gattc_handle_value_ind_param_t type.
     */
    CY_BLE_EVT_GATTC_INDICATION,


    /****************************************
     * Service Discovery Events
     ***************************************/

    /** GATT Client - The Service discovery procedure failed. This event may
     *  be generated on calling Cy_BLE_GATTC_DiscoverPrimaryServices().
     *  The parameter of this event is a structure of the cy_stc_ble_conn_handle_t type.
     */
    CY_BLE_EVT_GATTC_SRVC_DISCOVERY_FAILED,

    /** GATT Client - The discovery of included services failed. This event may
     *  be generated on calling Cy_BLE_GATTC_FindIncludedServices().
     *  The parameter of this event is a structure of the cy_stc_ble_conn_handle_t type.
     */
    CY_BLE_EVT_GATTC_INCL_DISCOVERY_FAILED,

    /** GATT Client - The discovery of the service's characteristics failed. This event may
     *  be generated on calling Cy_BLE_GATTC_DiscoverCharacteristics() or
     *  Cy_BLE_GATTC_ReadUsingCharacteristicUuid().
     *  The parameter of this event is a structure of the cy_stc_ble_conn_handle_t type.
     */
    CY_BLE_EVT_GATTC_CHAR_DISCOVERY_FAILED,

    /** GATT Client - The discovery of the service's characteristics failed. This event may
     *  be generated on calling Cy_BLE_GATTC_DiscoverCharacteristicDescriptors().
     *  The parameter of this event is a structure of the cy_stc_ble_conn_handle_t type.
     */
    CY_BLE_EVT_GATTC_DESCR_DISCOVERY_FAILED,

    /** GATT Client - A duplicate service record was found during the server device
     *  discovery. The parameter of this event is a structure of cy_stc_ble_disc_srv_info_t
     *  type.
     */
    CY_BLE_EVT_GATTC_SRVC_DUPLICATION,

    /** GATT Client - Duplicate service's characteristic record was found during
     *  server device discovery. The parameter of this event is a structure
     *  of cy_stc_ble_disc_char_info_t type.
     */
    CY_BLE_EVT_GATTC_CHAR_DUPLICATION,

    /** GATT Client - A duplicate service's characteristic descriptor record was found
     *  during server device discovery. The parameter of this event is a structure
     *  of cy_stc_ble_disc_descr_info_t type.
     */
    CY_BLE_EVT_GATTC_DESCR_DUPLICATION,

    /** GATT Client - The Service discovery procedure completed successfully. This
     *  event may be generated on calling Cy_BLE_GATTC_DiscoverPrimaryServices().
     *  The parameter of this event is a structure of the cy_stc_ble_conn_handle_t type.
     */
    CY_BLE_EVT_GATTC_SRVC_DISCOVERY_COMPLETE,

    /** GATT Client - The included services discovery is completed
     *  successfully. This event may be generated on calling
     *  Cy_BLE_GATTC_FindIncludedServices().
     *  The parameter of this event is a structure of the cy_stc_ble_conn_handle_t type.
     */
    CY_BLE_EVT_GATTC_INCL_DISCOVERY_COMPLETE,

    /** GATT Client - The discovery of service's characteristics discovery is completed
     *  successfully. This event may be generated on calling
     *  Cy_BLE_GATTC_DiscoverCharacteristics() or
     *  Cy_BLE_GATTC_ReadUsingCharacteristicUuid().
     *  The parameter of this event is a structure of the cy_stc_ble_conn_handle_t type.
     */
    CY_BLE_EVT_GATTC_CHAR_DISCOVERY_COMPLETE,
    
    /** GATT Client - The service (not defined in the GATT database) was found during
    *  the server device discovery. The discovery procedure skips this service.
    *  This event parameter is a structure of the cy_stc_ble_disc_srv_info_t type.
    */
    CY_BLE_EVT_GATTC_DISC_SKIPPED_SERVICE,
                    
    /** GATT Client - The discovery of a remote device completed successfully.
     *  The parameter of this event is a structure of the cy_stc_ble_conn_handle_t type.
     */
    CY_BLE_EVT_GATTC_DISCOVERY_COMPLETE,

    /****************************************
     * AIOS Service Events
     ***************************************/

    /** AIOS Server - Notification for Automation Input Output Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  cy_stc_ble_aios_char_value_t type.
     */
    CY_BLE_EVT_AIOSS_NOTIFICATION_ENABLED,

    /** AIOS Server - Notification for Automation Input Output Service Characteristic
     *  was disabled. The parameter of this event is a structure of
     *  cy_stc_ble_aios_char_value_t type.
     */
    CY_BLE_EVT_AIOSS_NOTIFICATION_DISABLED,

    /** AIOS Server - Indication for Automation Input Output Service Characteristic
     *  was enabled. The parameter of this event is a structure
     *  of cy_stc_ble_aios_char_value_t type.
     */
    CY_BLE_EVT_AIOSS_INDICATION_ENABLED,

    /** AIOSS Server - Indication for Automation Input Output Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of cy_stc_ble_aios_char_value_t type.
     */
    CY_BLE_EVT_AIOSS_INDICATION_DISABLED,

    /** AIOS Server - Automation Input Output Service Characteristic
     *  Indication was confirmed. The parameter of this event
     *  is a structure of cy_stc_ble_aios_char_value_t type.
     */
    CY_BLE_EVT_AIOSS_INDICATION_CONFIRMED,

    /** AIOS Server - Write Request for Automation Input Output Service Characteristic
     *  was received. The parameter of this event is a structure
     *  of cy_stc_ble_aios_char_value_t type.
     */
    CY_BLE_EVT_AIOSS_WRITE_CHAR,

    /** AIOSS Server - Write Request for Automation Input Output Service
     *  Characteristic Descriptor was received. The parameter of this event is a structure of
     *  cy_stc_ble_aios_char_value_t type.
     */
    CY_BLE_EVT_AIOSS_DESCR_WRITE,

    /** AIOS Client - Automation Input Output Characteristic Service Notification
     *  was received. The parameter of this event is a structure
     *  of cy_stc_ble_aios_char_value_t type.
     */
    CY_BLE_EVT_AIOSC_NOTIFICATION,

    /** AIOS Client - Automation Input Output Service Characteristic
     *  Indication was received. The parameter of this event
     *  is a structure of cy_stc_ble_aios_char_value_t type.
     */
    CY_BLE_EVT_AIOSC_INDICATION,

    /** AIOS Client - Read Response for Read Request for Automation Input Output Service Characteristic
     *  Value. The parameter of this event is a structure of
     *  cy_stc_ble_aios_char_value_t type.
     */
    CY_BLE_EVT_AIOSC_READ_CHAR_RESPONSE,

    /** AIOS Client - Write Response for Write Request for Automation Input Output Service
     *  Characteristic Value. The parameter of this event is a structure of
     *  cy_stc_ble_aios_char_value_t type.
     */
    CY_BLE_EVT_AIOSC_WRITE_CHAR_RESPONSE,

    /** AIOS Client - Read Response for Read Request for Automation Input Output Service
     *  Characteristic Descriptor Read Request. The parameter of this event is a
     *  structure of cy_stc_ble_aios_descr_value_t type.
     */
    CY_BLE_EVT_AIOSC_READ_DESCR_RESPONSE,

    /** AIOS Client - Write Response for Write Request for Automation Input Output Service
     *  Client Characteristic Configuration Descriptor Value. The parameter of
     *  this event is a structure of cy_stc_ble_aios_descr_value_t type.
     */
    CY_BLE_EVT_AIOSC_WRITE_DESCR_RESPONSE,

    /** AIOS Client - Error Response for Write Request for Automation Input Output Service
     *  Characteristic Value. The parameter of this event is a structure of
     *  cy_stc_ble_ancs_char_value_t type.
     */
    CY_BLE_EVT_AIOSC_ERROR_RESPONSE,

    /****************************************
     * ANCS Service Events
     ***************************************/

    /** ANCS Server - Notification for Apple Notification Center Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_ancs_char_value_t type.
     */
    CY_BLE_EVT_ANCSS_NOTIFICATION_ENABLED,

    /** ANCS Server - Notification for Apple Notification Center Service Characteristic
     *  was disabled. The parameter of this event is a structure of
     *  the cy_stc_ble_ancs_char_value_t type.
     */
    CY_BLE_EVT_ANCSS_NOTIFICATION_DISABLED,

    /** ANCS Server - Write Request for Apple Notification Center Service Characteristic
     *  was received. The parameter of this event is a structure
     *  of the cy_stc_ble_ancs_char_value_t type.
     */
    CY_BLE_EVT_ANCSS_WRITE_CHAR,

    /** ANCS Client - Apple Notification Center Characteristic Service Notification
     *  was received. The parameter of this event is a structure
     *  of the cy_stc_ble_ancs_char_value_t type.
     */
    CY_BLE_EVT_ANCSC_NOTIFICATION,

    /** ANCS Client - Write Response for Write Request for Apple Notification Center Service
     *  Characteristic Value. The parameter of this event is a structure of
     *  cy_stc_ble_ancs_char_value_t type.
     */
    CY_BLE_EVT_ANCSC_WRITE_CHAR_RESPONSE,

    /** ANCS Client - Read Response for Read Request for Apple Notification Center Service
     *  Characteristic Descriptor Read Request. The parameter of this event is a
     *  structure of the cy_stc_ble_ancs_descr_value_t type.
     */
    CY_BLE_EVT_ANCSC_READ_DESCR_RESPONSE,

    /** ANCS Client - Write Response for Write Request for Apple Notification Center Service
     *  Client Characteristic Configuration Descriptor Value. The parameter of
     *  this event is a structure of the cy_stc_ble_ancs_descr_value_t type.
     */
    CY_BLE_EVT_ANCSC_WRITE_DESCR_RESPONSE,

    /** ANCS Client - Error Response for Write Request for Apple Notification Center Service
     *  Characteristic Value. The parameter of this event is a structure of
     *  the cy_stc_ble_ancs_char_value_t type.
     */
    CY_BLE_EVT_ANCSC_ERROR_RESPONSE,


    /****************************************
     * ANS Service Events
     ***************************************/

    /** ANS Server - Notification for Alert Notification Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_ans_char_value_t type.
     */
    CY_BLE_EVT_ANSS_NOTIFICATION_ENABLED,

    /** ANS Server - Notification for Alert Notification Service Characteristic
     *  was disabled. The parameter of this event is a structure of
     *  the cy_stc_ble_ans_char_value_t type.
     */
    CY_BLE_EVT_ANSS_NOTIFICATION_DISABLED,

    /** ANS Server - Write Request for Alert Notification Service Characteristic
     *  was received. The parameter of this event is a structure
     *  of the cy_stc_ble_ans_char_value_t type.
     */
    CY_BLE_EVT_ANSS_WRITE_CHAR,

    /** ANS Client - Alert Notification Characteristic Service Notification
     *  was received. The parameter of this event is a structure
     *  of the cy_stc_ble_ans_char_value_t type.
     */
    CY_BLE_EVT_ANSC_NOTIFICATION,

    /** ANS Client - Read Response for Alert Notification Service Characteristic
     *  Value. The parameter of this event is a structure of
     *  the cy_stc_ble_ans_char_value_t type.
     */
    CY_BLE_EVT_ANSC_READ_CHAR_RESPONSE,

    /** ANS Client - Write Response for Write Request for Alert Notification Service
     *  Characteristic Value. The parameter of this event is a structure of
     *  the cy_stc_ble_ans_char_value_t type.
     */
    CY_BLE_EVT_ANSC_WRITE_CHAR_RESPONSE,

    /** ANS Client - Read Response for Read Request for Alert Notification Service
     *  Characteristic Descriptor Read Request. The parameter of this event is a
     *  structure of the cy_stc_ble_ans_descr_value_t type.
     */
    CY_BLE_EVT_ANSC_READ_DESCR_RESPONSE,

    /** ANS Client - Write Response for Write Request for Alert Notification Service
     *  Client Characteristic Configuration Descriptor Value. The parameter of
     *  this event is a structure of the cy_stc_ble_ans_descr_value_t type.
     */
    CY_BLE_EVT_ANSC_WRITE_DESCR_RESPONSE,


    /****************************************
     * BAS Service Events
     ***************************************/

    /** BAS Server - Notification for Battery Level Characteristic was enabled. The
     *  parameter of this event is a structure of the cy_stc_ble_bas_char_value_t type.
     */
    CY_BLE_EVT_BASS_NOTIFICATION_ENABLED,

    /** BAS Server - Notification for Battery Level Characteristic was disabled. The
     *  parameter of this event is a structure of the cy_stc_ble_bas_char_value_t type.
     */
    CY_BLE_EVT_BASS_NOTIFICATION_DISABLED,

    /** BAS Client - Battery Level Characteristic Notification was received. The
     *  parameter of this event is a structure of the cy_stc_ble_bas_char_value_t type.
     */
    CY_BLE_EVT_BASC_NOTIFICATION,

    /** BAS Client - Read Response for Battery Level Characteristic Value. The
     *  parameter of this event is a structure of the cy_stc_ble_bas_char_value_t type.
     */
    CY_BLE_EVT_BASC_READ_CHAR_RESPONSE,

    /** BAS Client - Read Response for Battery Level Characteristic Descriptor Read
     *  Request. The parameter of this event is a structure of
     *  the cy_stc_ble_bas_descr_value_t type.
     */
    CY_BLE_EVT_BASC_READ_DESCR_RESPONSE,

    /** BAS Client - Write Response for Battery Level Client Characteristic
     *  Configuration Descriptor Value. The parameter of this event is a structure of
     *  the cy_stc_ble_bas_descr_value_t type.
     */
    CY_BLE_EVT_BASC_WRITE_DESCR_RESPONSE,


    /****************************************
     * Body Composition Service Events
     ***************************************/

    /** BCS Server - Indication for Body Composition Service Characteristic
     *  was enabled. The parameter of this event is a structure
     *  of the cy_stc_ble_bcs_char_value_t type.
     */
    CY_BLE_EVT_BCSS_INDICATION_ENABLED,

    /** BCS Server - Indication for Body Composition Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_bcs_char_value_t type.
     */
    CY_BLE_EVT_BCSS_INDICATION_DISABLED,

    /** BCS Server - Body Composition Service Characteristic
     *  Indication was confirmed. The parameter of this event
     *  is a structure of the cy_stc_ble_bcs_char_value_t type.
     */
    CY_BLE_EVT_BCSS_INDICATION_CONFIRMED,

    /** BCS Client - Body Composition Service Characteristic
     *  Indication was received. The parameter of this event
     *  is a structure of the cy_stc_ble_bcs_char_value_t type.
     */
    CY_BLE_EVT_BCSC_INDICATION,

    /** BCS Client - Read Response for Read Request of Body Composition
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_bcs_char_value_t type.
     */
    CY_BLE_EVT_BCSC_READ_CHAR_RESPONSE,

    /** BCS Client - Read Response for Read Request of Body Composition
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_bcs_descr_value_t type.
     */
    CY_BLE_EVT_BCSC_READ_DESCR_RESPONSE,

    /** BCS Client - Write Response for Write Request of Body Composition
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_bcs_descr_value_t type.
     */
    CY_BLE_EVT_BCSC_WRITE_DESCR_RESPONSE,

    /****************************************
     * Blood Pressure Service Events
     ***************************************/

    /** BLS Server - Indication for Blood Pressure Service Characteristic was enabled.
     *  The parameter of this event is a structure of the cy_stc_ble_bls_char_value_t type.
     */
    CY_BLE_EVT_BLSS_INDICATION_ENABLED,

    /** BLS Server - Indication for Blood Pressure Service Characteristic was
     *  disabled. The parameter of this event is a structure of
     *  the cy_stc_ble_bls_char_value_t type.
     */
    CY_BLE_EVT_BLSS_INDICATION_DISABLED,

    /** BLS Server - Blood Pressure Service Characteristic Indication was confirmed.
     *  The parameter of this event is a structure of cy_stc_ble_bls_char_value_t type
     */
    CY_BLE_EVT_BLSS_INDICATION_CONFIRMED,

    /** BLS Server - Notification for Blood Pressure Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_bls_char_value_t type.
     */
    CY_BLE_EVT_BLSS_NOTIFICATION_ENABLED,

    /** BLS Server - Notification for Blood Pressure Service Characteristic
     *  was disabled. The parameter of this event is a structure of
     *  the cy_stc_ble_bls_char_value_t type.
     */
    CY_BLE_EVT_BLSS_NOTIFICATION_DISABLED,

    /** BLS Client - Blood Pressure Service Characteristic Indication was received.
     *  The parameter of this event is a structure of the cy_stc_ble_bls_char_value_t type.
     */
    CY_BLE_EVT_BLSC_INDICATION,

    /** BLS Client - Blood Pressure Service Characteristic Notification was received.
     *  The parameter of this event is a structure of the cy_stc_ble_bls_char_value_t type.
     */
    CY_BLE_EVT_BLSC_NOTIFICATION,

    /** BLS Client - Read Response for Read Request of Blood Pressure Service
     *  Characteristic value. The parameter of this event is a structure of
     *  the cy_stc_ble_bls_char_value_t type.
     */
    CY_BLE_EVT_BLSC_READ_CHAR_RESPONSE,

    /** BLS Client - Read Response for Read Request of Blood Pressure Service
     *  Characteristic Descriptor Read request. The parameter of this event is a
     *  structure of the cy_stc_ble_bls_descr_value_t type.
     */
    CY_BLE_EVT_BLSC_READ_DESCR_RESPONSE,

    /** BLS Client - Write Response for Write Request of Blood Pressure Service
     *  Characteristic Configuration Descriptor value. The parameter of this event
     *  is a structure of the cy_stc_ble_bls_descr_value_t type.
     */
    CY_BLE_EVT_BLSC_WRITE_DESCR_RESPONSE,


    /****************************************
     * Bond Management Service Events
     ***************************************/

    /** BMS Server - Write Request for Bond Management
     *  was received. The parameter of this event is a structure
     *  of the cy_stc_ble_bms_char_value_t type.
     */
    CY_BLE_EVT_BMSS_WRITE_CHAR,

    /** BMS Client - Read Response for Read Request of Bond Management Service
     *  Characteristic value. The parameter of this event is a structure of
     *  the cy_stc_ble_bms_char_value_t type.
     */
    CY_BLE_EVT_BMSC_READ_CHAR_RESPONSE,

    /** BMS Client - Write Response for Write Request of Bond Management
     *  Service Characteristic value. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_bms_char_value_t type.
     */
    CY_BLE_EVT_BMSC_WRITE_CHAR_RESPONSE,

    /** BMS Client - Read Response for Read Request of Bond Management Service
     *  Characteristic Descriptor Read request. The parameter of this event is a
     *  structure of cy_stc_ble_bms_descr_value_t type.
     */
    CY_BLE_EVT_BMSC_READ_DESCR_RESPONSE,


    /****************************************
     * CGM Service Events
     ***************************************/

    /** CGMS Server - Indication for Continuous Glucose Monitoring Service Characteristic
     *  was enabled. The parameter of this event is a structure
     *  of the cy_stc_ble_cgms_char_value_t type.
     */
    CY_BLE_EVT_CGMSS_INDICATION_ENABLED,

    /** CGMS Server - Indication for Continuous Glucose Monitoring Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_cgms_char_value_t type.
     */
    CY_BLE_EVT_CGMSS_INDICATION_DISABLED,

    /** CGMS Server - Continuous Glucose Monitoring Service Characteristic
     *  Indication was confirmed. The parameter of this event
     *  is a structure of the cy_stc_ble_cgms_char_value_t type.
     */
    CY_BLE_EVT_CGMSS_INDICATION_CONFIRMED,

    /** CGMS Server - Notification for Continuous Glucose Monitoring Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_cgms_char_value_t type.
     */
    CY_BLE_EVT_CGMSS_NOTIFICATION_ENABLED,

    /** CGMS Server - Notification for Continuous Glucose Monitoring Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_cgms_char_value_t type.
     */
    CY_BLE_EVT_CGMSS_NOTIFICATION_DISABLED,

    /** CGMS Server - Write Request for Continuous Glucose Monitoring Service
     *  was received. The parameter of this event is a structure
     *  of the cy_stc_ble_cgms_char_value_t type.
     */
    CY_BLE_EVT_CGMSS_WRITE_CHAR,

    /** CGMS Client - Continuous Glucose Monitoring Service Characteristic
     *  Indication was received. The parameter of this event
     *  is a structure of the cy_stc_ble_cgms_char_value_t type.
     */
    CY_BLE_EVT_CGMSC_INDICATION,

    /** CGMS Client - Continuous Glucose Monitoring Service Characteristic
     *  Notification was received. The parameter of this event
     *  is a structure of the cy_stc_ble_cgms_char_value_t type.
     */
    CY_BLE_EVT_CGMSC_NOTIFICATION,

    /** CGMS Client - Read Response for Read Request of Continuous Glucose Monitoring
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_cgms_char_value_t type.
     */
    CY_BLE_EVT_CGMSC_READ_CHAR_RESPONSE,

    /** CGMS Client - Write Response for Write Request of Continuous Glucose Monitoring
     *  Service Characteristic value. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_cgms_char_value_t type.
     */
    CY_BLE_EVT_CGMSC_WRITE_CHAR_RESPONSE,

    /** CGMS Client - Read Response for Read Request of Continuous Glucose Monitoring
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_cgms_descr_value_t type.
     */
    CY_BLE_EVT_CGMSC_READ_DESCR_RESPONSE,

    /** CGMS Client - Write Response for Write Request of Continuous Glucose Monitoring
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_cgms_descr_value_t type.
     */
    CY_BLE_EVT_CGMSC_WRITE_DESCR_RESPONSE,


    /****************************************
     * CPS Service Events
     ***************************************/

    /** CPS Server - Notification for Cycling Power Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_cps_char_value_t type.
     */
    CY_BLE_EVT_CPSS_NOTIFICATION_ENABLED,

    /** CPS Server - Notification for Cycling Power Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_cps_char_value_t type.
     */
    CY_BLE_EVT_CPSS_NOTIFICATION_DISABLED,

    /** CPS Server - Indication for Cycling Power Service Characteristic
     *  was enabled. The parameter of this event is a structure
     *  of the cy_stc_ble_cps_char_value_t type.
     */
    CY_BLE_EVT_CPSS_INDICATION_ENABLED,

    /** CPS Server - Indication for Cycling Power Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_cps_char_value_t type.
     */
    CY_BLE_EVT_CPSS_INDICATION_DISABLED,

    /** CPS Server - Cycling Power Service Characteristic
     *  Indication was confirmed. The parameter of this event
     *  is a structure of the cy_stc_ble_cps_char_value_t type.
     */
    CY_BLE_EVT_CPSS_INDICATION_CONFIRMED,

    /** CPS Server - Broadcast for Cycling Power Service Characteristic
     *  was enabled. The parameter of this event
     *  is a structure of the cy_stc_ble_cps_char_value_t type.
     */
    CY_BLE_EVT_CPSS_BROADCAST_ENABLED,

    /** CPS Server - Broadcast for Cycling Power Service Characteristic
     *  was disabled. The parameter of this event
     *  is a structure of the cy_stc_ble_cps_char_value_t type.
     */
    CY_BLE_EVT_CPSS_BROADCAST_DISABLED,

    /** CPS Server - Write Request for Cycling Power Service Characteristic
     *  was received. The parameter of this event is a structure
     *  of the cy_stc_ble_cps_char_value_t type.
     */
    CY_BLE_EVT_CPSS_WRITE_CHAR,

    /** CPS Client - Cycling Power Service Characteristic
     *  Notification was received. The parameter of this event
     *  is a structure of the cy_stc_ble_cps_char_value_t type.
     */
    CY_BLE_EVT_CPSC_NOTIFICATION,

    /** CPS Client - Cycling Power Service Characteristic
     *  Indication was received. The parameter of this event
     *  is a structure of the cy_stc_ble_cps_char_value_t type.
     */
    CY_BLE_EVT_CPSC_INDICATION,

    /** CPS Client - Read Response for Read Request of Cycling Power Service
     *  Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_cps_char_value_t type.
     */
    CY_BLE_EVT_CPSC_READ_CHAR_RESPONSE,

    /** CPS Client - Write Response for Write Request of Cycling Power Service
     *  Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_cps_char_value_t type.
     */
    CY_BLE_EVT_CPSC_WRITE_CHAR_RESPONSE,

    /** CPS Client - Read Response for Read Request of Cycling Power
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_cps_descr_value_t type.
     */
    CY_BLE_EVT_CPSC_READ_DESCR_RESPONSE,

    /** CPS Client - Write Response for Write Request of Cycling Power
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_cps_descr_value_t type.
     */
    CY_BLE_EVT_CPSC_WRITE_DESCR_RESPONSE,

    /** CPS Client - This event is triggered every time a device receive
     *  non-connectable undirected advertising event.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_cps_char_value_t type.
     */
    CY_BLE_EVT_CPSC_SCAN_PROGRESS_RESULT,

    /** CPS Client - Cycling Power CP procedure timeout was received. The parameter
     *  of this event is a structure of the cy_stc_ble_cps_char_value_t type.
     */
    CY_BLE_EVT_CPSC_TIMEOUT,

    /****************************************
     * Cycling Speed and Cadence Service Events
     ***************************************/

    /** CSCS Server - Notification for Cycling Speed and Cadence Service
     *  Characteristic was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_cscs_char_value_t type.
     */
    CY_BLE_EVT_CSCSS_NOTIFICATION_ENABLED,

    /** CSCS Server - Notification for Cycling Speed and Cadence Service
     *  Characteristic was disabled. The parameter of this event is a structure of
     *  the cy_stc_ble_cscs_char_value_t type.
     */
    CY_BLE_EVT_CSCSS_NOTIFICATION_DISABLED,

    /** CSCS Server - Indication for Cycling Speed and Cadence Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_cscs_char_value_t type.
     */
    CY_BLE_EVT_CSCSS_INDICATION_ENABLED,

    /** CSCS Server - Indication for Cycling Speed and Cadence Service Characteristic
     *  was disabled. The parameter of this event is a structure of
     *  the cy_stc_ble_cscs_char_value_t type.
     */
    CY_BLE_EVT_CSCSS_INDICATION_DISABLED,

    /** CSCS Server - Cycling Speed and Cadence Service Characteristic
     *  Indication was confirmed. The parameter of this event is a structure of
     *  the cy_stc_ble_cscs_char_value_t type.
     */
    CY_BLE_EVT_CSCSS_INDICATION_CONFIRMATION,

    /** CSCS Server - Write Request for Cycling Speed and Cadence Service
     *  Characteristic was received. The parameter of this event is a structure of
     *  the cy_stc_ble_cscs_char_value_t type.
     */
    CY_BLE_EVT_CSCSS_WRITE_CHAR,

    /** CSCS Client - Cycling Speed and Cadence Service Characteristic
     *  Notification was received. The parameter of this event is a structure of
     *  the cy_stc_ble_cscs_char_value_t type.
     */
    CY_BLE_EVT_CSCSC_NOTIFICATION,

    /** CSCS Client - Cycling Speed and Cadence Service Characteristic
     *  Indication was received. The parameter of this event is a structure of
     *  the cy_stc_ble_cscs_char_value_t type.
     */
    CY_BLE_EVT_CSCSC_INDICATION,

    /** CSCS Client - Read Response for Read Request of Cycling Speed and Cadence
     *  Service Characteristic value. The parameter of this event is a structure of
     *  the cy_stc_ble_cscs_char_value_t type.
     */
    CY_BLE_EVT_CSCSC_READ_CHAR_RESPONSE,

    /** CSCS Client - Write Response for Write Request of Cycling Speed and Cadence
     *  Service Characteristic value. The parameter of this event is a structure of
     *  the cy_stc_ble_cscs_char_value_t type.
     */
    CY_BLE_EVT_CSCSC_WRITE_CHAR_RESPONSE,

    /** CSCS Client - Read Response for Read Request of Cycling Speed and Cadence
     *  Service Characteristic Descriptor Read request. The parameter of this event
     *  is a structure of the cy_stc_ble_cscs_descr_value_t type.
     */
    CY_BLE_EVT_CSCSC_READ_DESCR_RESPONSE,

    /** CSCS Client - Write Response for Write Request of Cycling Speed and Cadence
     *  Service Characteristic Configuration Descriptor value. The parameter of
     *  this event is a structure of  the cy_stc_ble_cscs_descr_value_t type.
     */
    CY_BLE_EVT_CSCSC_WRITE_DESCR_RESPONSE,


    /****************************************
     * Current Time Service Events
     ***************************************/

    /** CTS Server - Notification for Current Time Characteristic was enabled. The
     *  parameter of this event is a structure of the cy_stc_ble_cts_char_value_t type.
     */
    CY_BLE_EVT_CTSS_NOTIFICATION_ENABLED,

    /** CTS Server - Notification for Current Time Characteristic was disabled. The
     *  parameter of this event is a structure of the cy_stc_ble_cts_char_value_t type.
     */
    CY_BLE_EVT_CTSS_NOTIFICATION_DISABLED,

    /** CTS Server - Write Request for Current Time Service
     *  Characteristic was received. The parameter of this event is a structure of
     *  the cy_stc_ble_cts_char_value_t type. When this event is received, the user is
     *  responsible for performing any kind of data verification and writing the
     *  data to the GATT database in case of successful verification or setting
     *  the error using Cy_BLE_SetGattError() if data verification fails.
     */
    CY_BLE_EVT_CTSS_WRITE_CHAR,

    /** CTS Client - Current Time Characteristic Notification was received. The
     *  parameter of this event is a structure of the cy_stc_ble_cts_char_value_t type.
     */
    CY_BLE_EVT_CTSC_NOTIFICATION,

    /** CTS Client - Read Response for Current Time Characteristic
     *  Value Read Request. The parameter of this event is a
     *  structure of the cy_stc_ble_cts_char_value_t type.
     */
    CY_BLE_EVT_CTSC_READ_CHAR_RESPONSE,

    /** CTS Client - Read Response for Current Time Client
     *  Characteristic Configuration Descriptor Value Read
     *  Request. The parameter of this event is a
     *  structure of the cy_stc_ble_cts_descr_value_t type.
     */
    CY_BLE_EVT_CTSC_READ_DESCR_RESPONSE,

    /** CTS Client - Write Response for Current Time Characteristic
     *  Configuration Descriptor Value. The parameter of this
     *  event is a structure of the cy_stc_ble_cts_descr_value_t type.
     */
    CY_BLE_EVT_CTSC_WRITE_DESCR_RESPONSE,

    /** CTS Client - Write Response for Current Time or Local
     *  Time Information Characteristic Value. The parameter of this
     *  event is a structure of the cy_stc_ble_cts_descr_value_t type.
     */
    CY_BLE_EVT_CTSC_WRITE_CHAR_RESPONSE,

    /****************************************
     * DIS Service Events
     ***************************************/

    /** DIS Client - Read Response for a Read Request for a
     *  Device Information Service Characteristic. The parameter of this
     *  event is a structure of the cy_stc_ble_dis_char_value_t type.
     */
    CY_BLE_EVT_DISC_READ_CHAR_RESPONSE,


    /****************************************
     * Environmental Sensing Service Events
     ***************************************/

    /** ESS Server - Notification for Environmental Sensing Service
     *  Characteristic was enabled. The parameter of this event is a structure of
     *  cy_stc_ble_ess_char_value_t type.
     */
    CY_BLE_EVT_ESSS_NOTIFICATION_ENABLED,

    /** ESS Server - Notification for Environmental Sensing Service
     *  Characteristic was disabled. The parameter of this event is a structure of
     *  cy_stc_ble_ess_char_value_t type.
     */
    CY_BLE_EVT_ESSS_NOTIFICATION_DISABLED,

    /** ESS Server - Indication for Environmental Sensing Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_ess_char_value_t type.
     */
    CY_BLE_EVT_ESSS_INDICATION_ENABLED,

    /** ESS Server - Indication for Environmental Sensing Service Characteristic
     *  was disabled. The parameter of this event is a structure of
     *  cy_stc_ble_ess_char_value_t type.
     */
    CY_BLE_EVT_ESSS_INDICATION_DISABLED,

    /** ESS Server - Environmental Sensing Service Characteristic
     *  Indication was confirmed. The parameter of this event is a structure of
     *  cy_stc_ble_ess_char_value_t type.
     */
    CY_BLE_EVT_ESSS_INDICATION_CONFIRMATION,

    /** ESS Server - Write Request for Environmental Sensing Service
     *  Characteristic was received. The parameter of this event is a structure of
     *  the cy_stc_ble_ess_char_value_t type.
     */
    CY_BLE_EVT_ESSS_WRITE_CHAR,

    /** ESS Server - Write Request for Environmental Sensing Service
     *  Characteristic Descriptor was received. The parameter of this event is a structure of
     *  the cy_stc_ble_ess_descr_value_t type. This event is generated only when write for
     *  CY_BLE_ESS_CHAR_USER_DESCRIPTION_DESCR, CY_BLE_ESS_ES_TRIGGER_SETTINGS_DESCR or
     *  CY_BLE_ESS_ES_CONFIG_DESCR occurs.
     */
    CY_BLE_EVT_ESSS_DESCR_WRITE,

    /** ESS Client - Environmental Sensing Service Characteristic
     *  Notification was received. The parameter of this event is a structure of
     *  the cy_stc_ble_ess_char_value_t type.
     */
    CY_BLE_EVT_ESSC_NOTIFICATION,

    /** ESS Client - Environmental Sensing Service Characteristic
     *  Indication was received. The parameter of this event is a structure of
     *  cy_stc_ble_ess_char_value_t type.
     */
    CY_BLE_EVT_ESSC_INDICATION,

    /** ESS Client - Read Response for Read Request of Environmental Sensing
     *  Service Characteristic value. The parameter of this event is a structure of
     *  the cy_stc_ble_ess_char_value_t type.
     */
    CY_BLE_EVT_ESSC_READ_CHAR_RESPONSE,

    /** ESS Client - Write Response for Write Request of Environmental Sensing
     *  Service Characteristic value. The parameter of this event is a structure of
     *  the cy_stc_ble_ess_char_value_t type.
     */
    CY_BLE_EVT_ESSC_WRITE_CHAR_RESPONSE,

    /** ESS Client - Read Response for Read Request of Environmental Sensing
     *  Service Characteristic Descriptor Read request. The parameter of this event
     *  is a structure of the cy_stc_ble_ess_descr_value_t type.
     */
    CY_BLE_EVT_ESSC_READ_DESCR_RESPONSE,

    /** ESS Client - Write Response for Write Request of Environmental Sensing
     *  Service Characteristic Configuration Descriptor value. The parameter of
     *  this event is a structure of the cy_stc_ble_ess_descr_value_t type.
     */
    CY_BLE_EVT_ESSC_WRITE_DESCR_RESPONSE,


    /****************************************
     * Glucose Service Events
     ***************************************/

    /** GLS Server - Indication for Glucose Service Characteristic
     *  was enabled. The parameter of this event is a structure
     *  of cy_stc_ble_gls_char_value_t type.
     */
    CY_BLE_EVT_GLSS_INDICATION_ENABLED,

    /** GLS Server - Indication for Glucose Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_gls_char_value_t type.
     */
    CY_BLE_EVT_GLSS_INDICATION_DISABLED,

    /** GLS Server - Glucose Service Characteristic
     *  Indication was confirmed. The parameter of this event
     *  is a structure of the cy_stc_ble_gls_char_value_t type.
     */
    CY_BLE_EVT_GLSS_INDICATION_CONFIRMED,

    /** GLS Server - Notification for Glucose Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_gls_char_value_t type.
     */
    CY_BLE_EVT_GLSS_NOTIFICATION_ENABLED,

    /** GLS Server - Notification for Glucose Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_gls_char_value_t type.
     */
    CY_BLE_EVT_GLSS_NOTIFICATION_DISABLED,

    /** GLS Server - Write Request for Glucose Service
     *  was received. The parameter of this event is a structure
     *  of cy_stc_ble_gls_char_value_t type.
     */
    CY_BLE_EVT_GLSS_WRITE_CHAR,

    /** GLS Client - Glucose Service Characteristic
     *  Indication was received. The parameter of this event
     *  is a structure of the cy_stc_ble_gls_char_value_t type.
     */
    CY_BLE_EVT_GLSC_INDICATION,

    /** GLS Client - Glucose Service Characteristic
     *  Notification was received. The parameter of this event
     *  is a structure of the cy_stc_ble_gls_char_value_t type.
     */
    CY_BLE_EVT_GLSC_NOTIFICATION,

    /** GLS Client - Read Response for Read Request of Glucose
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_gls_char_value_t type.
     */
    CY_BLE_EVT_GLSC_READ_CHAR_RESPONSE,

    /** GLS Client - Write Response for Write Request of Glucose
     *  Service Characteristic value. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_gls_char_value_t type.
     */
    CY_BLE_EVT_GLSC_WRITE_CHAR_RESPONSE,

    /** GLS Client - Read Response for Read Request of Glucose
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_gls_descr_value_t type.
     */
    CY_BLE_EVT_GLSC_READ_DESCR_RESPONSE,

    /** GLS Client - Write Response for Write Request of Glucose
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_gls_descr_value_t type.
     */
    CY_BLE_EVT_GLSC_WRITE_DESCR_RESPONSE,


    /****************************************
     * HIDS Service Events
     ***************************************/

    /** HIDS Server - Notification for HID service was
     *  enabled. The parameter of this event is a
     *  structure of the cy_stc_ble_hids_char_value_t type.
     */
    CY_BLE_EVT_HIDSS_NOTIFICATION_ENABLED,

    /** HIDS Server - Notification for HID service was
     *  disabled. The parameter of this event is a
     *  structure of the cy_stc_ble_hids_char_value_t type.
     */
    CY_BLE_EVT_HIDSS_NOTIFICATION_DISABLED,

    /** HIDS Server - Enter boot mode request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_hids_char_value_t type.
     */
    CY_BLE_EVT_HIDSS_BOOT_MODE_ENTER,

    /** HIDS Server - Enter report mode request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_hids_char_value_t type.
     */
    CY_BLE_EVT_HIDSS_REPORT_MODE_ENTER,

    /** HIDS Server - Enter suspend mode request. The
     *  parameter of this event is a structure of
     *  cy_stc_ble_hids_char_value_t type.
     */
    CY_BLE_EVT_HIDSS_SUSPEND,

    /** HIDS Server - Exit suspend mode request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_hids_char_value_t type.
     */
    CY_BLE_EVT_HIDSS_EXIT_SUSPEND,

    /** HIDS Server - Write Report Characteristic
     *  request. The parameter of this event is a
     *  structure of the cy_stc_ble_hids_char_value_t type.
     */
    CY_BLE_EVT_HIDSS_REPORT_WRITE_CHAR,

    /** HIDS Client - HID Service Characteristic
     *  Notification was received. The parameter of this
     *  event is a structure of the cy_stc_ble_hids_char_value_t type.
     */
    CY_BLE_EVT_HIDSC_NOTIFICATION,

    /** HIDS Client - Read Response for Read Request of HID
     *  Service Characteristic value. The parameter of this
     *  event is a structure of the cy_stc_ble_hids_descr_value_t type.
     */
    CY_BLE_EVT_HIDSC_READ_CHAR_RESPONSE,

    /** HIDS Client - Write Response for Write Request of
     *  HID Service Characteristic value. The parameter
     *  of this event is a structure of
     *  the cy_stc_ble_hids_char_value_t type.
     */
    CY_BLE_EVT_HIDSC_WRITE_CHAR_RESPONSE,

    /** HIDS Client - Read Response for Read Request of HID
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_hids_descr_value_t type.
     */
    CY_BLE_EVT_HIDSC_READ_DESCR_RESPONSE,

    /** HIDS Client - Write Response for Write Request of HID
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_hids_char_value_t type.
     */
    CY_BLE_EVT_HIDSC_WRITE_DESCR_RESPONSE,


    /****************************************
     *  HTTP Proxy Service Events
     ***************************************/

    /** HPS Server - Notification for HTTP Proxy Service Characteristic
     *  was enabled. The parameter of this event is a structure
     *  of the cy_stc_ble_hps_char_value_t type.
     */
    CY_BLE_EVT_HPSS_NOTIFICATION_ENABLED,

    /** HPS Server - Notification for HTTP Proxy Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_hps_char_value_t type.
     */
    CY_BLE_EVT_HPSS_NOTIFICATION_DISABLED,

    /** HPS Server - Write Request for HTTP Proxy Service
     *  Characteristic was received. The parameter of this event is a structure of
     *  the cy_stc_ble_hps_char_value_t type.
     */
    CY_BLE_EVT_HPSS_WRITE_CHAR,

    /** HPS Client - HTTP Proxy Service Characteristic
     *  Notification was received. The parameter of this event
     *  is a structure of the cy_stc_ble_hps_char_value_t type.
     */
    CY_BLE_EVT_HPSC_NOTIFICATION,

    /** HPS Client - Read Response for Read Request of HTTP Proxy
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_hps_char_value_t type.
     */
    CY_BLE_EVT_HPSC_READ_CHAR_RESPONSE,

    /** HPS Client - Read Response for Read Request of HTTP Proxy
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_hps_descr_value_t type.
     */
    CY_BLE_EVT_HPSC_READ_DESCR_RESPONSE,

    /** HPS Client - Write Response for Write Request of HTTP Proxy
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_hps_descr_value_t type.
     */
    CY_BLE_EVT_HPSC_WRITE_DESCR_RESPONSE,

    /** HPS Client - Write Response for Write Request of HPS
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_hps_char_value_t type.
     */
    CY_BLE_EVT_HPSC_WRITE_CHAR_RESPONSE,

    /****************************************
     * HRS Service Events
     ***************************************/

    /** HRS Server - Reset Energy Expended. The parameter of
     *  this event is a structure of the cy_stc_ble_hrs_char_value_t type.
     */
    CY_BLE_EVT_HRSS_ENERGY_EXPENDED_RESET,

    /** HRS Server - Notification for Heart Rate Measurement
     *  Characteristic was enabled. The parameter of this
     *  event is a structure of the cy_stc_ble_hrs_char_value_t type.
     */

    CY_BLE_EVT_HRSS_NOTIFICATION_ENABLED,

    /** HRS Server - Notification for Heart Rate Measurement
     *  Characteristic was disabled. The parameter of this
     *  event is a structure of the cy_stc_ble_hrs_char_value_t type.
     */
    CY_BLE_EVT_HRSS_NOTIFICATION_DISABLED,

    /** HRS Client - Heart Rate Measurement Characteristic
     *  Notification was received. The parameter of this event
     *  is a structure of the cy_stc_ble_hrs_char_value_t type.
     */
    CY_BLE_EVT_HRSC_NOTIFICATION,

    /** HRS Client - Read Response for Read Request of HRS
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_hrs_char_value_t type.
     */
    CY_BLE_EVT_HRSC_READ_CHAR_RESPONSE,

    /** HRS Client - Write Response for Write Request of HRS
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_hrs_char_value_t type.
     */
    CY_BLE_EVT_HRSC_WRITE_CHAR_RESPONSE,

    /** HRS Client - Read Response for Read Request of HRS
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_hrs_char_value_t type.
     */
    CY_BLE_EVT_HRSC_READ_DESCR_RESPONSE,

    /** HRS Client - Write Response for Write Request of HRS
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_hrs_char_value_t type.
     */
    CY_BLE_EVT_HRSC_WRITE_DESCR_RESPONSE,


    /****************************************
     * HTS Service Events
     ***************************************/

    /** HTS Server - Notification for Health Thermometer Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_hts_char_value_t type.
     */
    CY_BLE_EVT_HTSS_NOTIFICATION_ENABLED,

    /** HTS Server - Notification for Health Thermometer Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_hts_char_value_t type.
     */
    CY_BLE_EVT_HTSS_NOTIFICATION_DISABLED,

    /** HTS Server - Indication for Health Thermometer Service Characteristic
     *  was enabled. The parameter of this event is a structure
     *  of the cy_stc_ble_hts_char_value_t type.
     */
    CY_BLE_EVT_HTSS_INDICATION_ENABLED,

    /** HTS Server - Indication for Health Thermometer Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_hts_char_value_t type.
     */
    CY_BLE_EVT_HTSS_INDICATION_DISABLED,

    /** HTS Server - Health Thermometer Service Characteristic
     *  Indication was confirmed. The parameter of this event
     *  is a structure of the cy_stc_ble_hts_char_value_t type.
     */
    CY_BLE_EVT_HTSS_INDICATION_CONFIRMED,

    /** HTS Server - Write Request for Health Thermometer Service Characteristic
     *  was received. The parameter of this event is a structure
     *  of the cy_stc_ble_hts_char_value_t type.
     */
    CY_BLE_EVT_HTSS_WRITE_CHAR,

    /** HTS Client - Health Thermometer Service Characteristic
     *  Notification was received. The parameter of this event
     *  is a structure of the cy_stc_ble_hts_char_value_t type.
     */
    CY_BLE_EVT_HTSC_NOTIFICATION,

    /** HTS Client - Health Thermometer Service Characteristic
     *  Indication was received. The parameter of this event
     *  is a structure of the cy_stc_ble_hts_char_value_t type.
     */
    CY_BLE_EVT_HTSC_INDICATION,

    /** HTS Client - Read Response for Read Request of Health Thermometer
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_hts_char_value_t type.
     */
    CY_BLE_EVT_HTSC_READ_CHAR_RESPONSE,

    /** HTS Client - Write Response for Write Request of Health Thermometer
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_hts_char_value_t type.
     */
    CY_BLE_EVT_HTSC_WRITE_CHAR_RESPONSE,

    /** HTS Client - Read Response for Read Request of Health Thermometer
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_hts_descr_value_t type.
     */
    CY_BLE_EVT_HTSC_READ_DESCR_RESPONSE,

    /** HTS Client - Write Response for Write Request of Health Thermometer
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_hts_descr_value_t type.
     */
    CY_BLE_EVT_HTSC_WRITE_DESCR_RESPONSE,


    /****************************************
     * Immediate Alert Service Events
     ***************************************/

    /** IAS Server - Write Command Request for Alert Level
     *  Characteristic. The parameter of this event
     *  is a structure of the cy_stc_ble_ias_char_value_t type.
     */
    CY_BLE_EVT_IASS_WRITE_CHAR_CMD,

    /****************************************
     * Indoor Positioning Service Events
     ***************************************/

    /** IPS Server - Read Request for Indoor Positioning Service Characteristic
     *  was received. The parameter of this event is a structure
     *  of the cy_stc_ble_ips_char_value_t type.
     */
    CY_BLE_EVT_IPSS_READ_CHAR,

    /** IPS Server - Write Request for Indoor Positioning Service Characteristic
     *  was received. The parameter of this event is a structure
     *  of cy_stc_ble_ips_char_value_t type.
     */
    CY_BLE_EVT_IPSS_WRITE_CHAR,

    /** IPS Server - Write command request for Indoor Positioning Service
     *  Characteristic. The parameter of this event
     *  is a structure of the cy_stc_ble_ips_char_value_t type.
     */
    CY_BLE_EVT_IPSS_WRITE_CHAR_CMD,

    /** IPS Client - Read Response for Read Request of Indoor Positioning
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_ips_char_value_t type.
     */
    CY_BLE_EVT_IPSC_READ_CHAR_RESPONSE,

    /** IPS Client - Read Multiple Response for Read Multiple Request of
     *  Indoor Positioning Service Characteristic value. The parameter
     *  of this event is a structure of the cy_stc_ble_ips_char_value_t type.
     */
    CY_BLE_EVT_IPSC_READ_MULTIPLE_CHAR_RESPONSE,

    /** IPS Client - Write Response for Write Request of Indoor Positioning
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_ips_char_value_t type.
     */
    CY_BLE_EVT_IPSC_WRITE_CHAR_RESPONSE,

    /** IPS Client - Read Response for Read Request of Indoor Positioning
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_ips_descr_value_t type.
     */
    CY_BLE_EVT_IPSC_READ_DESCR_RESPONSE,

    /** IPS Client - Write Response for Write Request of Indoor Positioning
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_ips_descr_value_t type.
     */
    CY_BLE_EVT_IPSC_WRITE_DESCR_RESPONSE,

    /** IPS Client - Error Response for Write Request for Indoor Positioning
     *  Service Characteristic Value. The parameter of this event is a structure of
     *  the cy_stc_ble_ips_char_value_t type.
     */
    CY_BLE_EVT_IPSC_ERROR_RESPONSE,

    /** IPS Client - Read Response for Long Read Request of Indoor Positioning
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_ips_char_value_t type.
     */
    CY_BLE_EVT_IPSC_READ_BLOB_RSP,

    /****************************************
     * Link Loss Service Events
     ***************************************/

    /** LLS Server - Write request for Alert Level Characteristic.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_lls_char_value_t type.
     */
    CY_BLE_EVT_LLSS_WRITE_CHAR_REQ,

    /** LLS Client - Read response for Alert Level Characteristic.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_lls_char_value_t type.
     */
    CY_BLE_EVT_LLSC_READ_CHAR_RESPONSE,

    /** LLS Client - Write response for write request of Alert
     *  Level Characteristic. The parameter of this event is a
     *  structure of the cy_stc_ble_lls_char_value_t type.
     */
    CY_BLE_EVT_LLSC_WRITE_CHAR_RESPONSE,


    /****************************************
     * Location and Navigation Service Events
     ***************************************/

    /** LNS Server - Indication for Location and Navigation Service Characteristic
     *  was enabled. The parameter of this event is a structure
     *  of the cy_stc_ble_lns_char_value_t type.
     */
    CY_BLE_EVT_LNSS_INDICATION_ENABLED,

    /** LNS Server - Indication for Location and Navigation Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_lns_char_value_t type.
     */
    CY_BLE_EVT_LNSS_INDICATION_DISABLED,

    /** LNS Server - Location and Navigation Service Characteristic
     *  Indication was confirmed. The parameter of this event
     *  is a structure of the cy_stc_ble_lns_char_value_t type.
     */
    CY_BLE_EVT_LNSS_INDICATION_CONFIRMED,

    /** LNS Server - Notification for Location and Navigation Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_lns_char_value_t type.
     */
    CY_BLE_EVT_LNSS_NOTIFICATION_ENABLED,

    /** LNS Server - Notification for Location and Navigation Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_lns_char_value_t type.
     */
    CY_BLE_EVT_LNSS_NOTIFICATION_DISABLED,

    /** LNS Server - Write Request for Location and Navigation Service Characteristic
     *  was received. The parameter of this event is a structure
     *  of the cy_stc_ble_lns_char_value_t type.
     */
    CY_BLE_EVT_LNSS_WRITE_CHAR,

    /** LNS Client - Location and Navigation Service Characteristic
     *  Indication was received. The parameter of this event
     *  is a structure of the cy_stc_ble_lns_char_value_t type.
     */
    CY_BLE_EVT_LNSC_INDICATION,

    /** LNS Client - Location and Navigation Service Characteristic
     *  Notification was received. The parameter of this event
     *  is a structure of the cy_stc_ble_lns_char_value_t type.
     */
    CY_BLE_EVT_LNSC_NOTIFICATION,

    /** LNS Client - Read Response for Read Request of Location and Navigation
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_lns_char_value_t type.
     */
    CY_BLE_EVT_LNSC_READ_CHAR_RESPONSE,

    /** LNS Client - Write Response for Write Request of Location and Navigation
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_lns_char_value_t type.
     */
    CY_BLE_EVT_LNSC_WRITE_CHAR_RESPONSE,

    /** LNS Client - Read Response for Read Request of Location and Navigation
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_lns_descr_value_t type.
     */
    CY_BLE_EVT_LNSC_READ_DESCR_RESPONSE,

    /** LNS Client - Write Response for Write Request of Location and Navigation
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_lns_descr_value_t type.
     */
    CY_BLE_EVT_LNSC_WRITE_DESCR_RESPONSE,


    /****************************************
     * Next DST Change Service Events
     ***************************************/

    /** NDCS Client - Read Response for Read Request of Next DST Change
     * Service Characteristic value. The parameter of this event
     * is a structure of the cy_stc_ble_ndcs_char_value_t type.
     */
    CY_BLE_EVT_NDCSC_READ_CHAR_RESPONSE,


    /****************************************
     * Phone Alert Status Service Events
     ***************************************/

    /** PASS Server - Notification for Phone Alert Status Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_pass_char_value_t type.
     */
    CY_BLE_EVT_PASSS_NOTIFICATION_ENABLED,

    /** PASS Server - Notification for Phone Alert Status Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_pass_char_value_t type.
     */
    CY_BLE_EVT_PASSS_NOTIFICATION_DISABLED,

    /** PASS Server - Write Request for Phone Alert Status Service Characteristic
     *  was received. The parameter of this event is a structure
     *  of the cy_stc_ble_pass_char_value_t type.
     */
    CY_BLE_EVT_PASSS_WRITE_CHAR,

    /** PASS Client - Phone Alert Status Service Characteristic
     *  Notification was received. The parameter of this event
     *  is a structure of the cy_stc_ble_pass_char_value_t type.
     */
    CY_BLE_EVT_PASSC_NOTIFICATION,

    /** PASS Client - Read Response for Read Request of Phone Alert Status
     *  Service Characteristic value. The parameter of this event
     *  is a structure of cy_stc_ble_pass_char_value_t type.
     */
    CY_BLE_EVT_PASSC_READ_CHAR_RESPONSE,

    /** PASS Client - Read Response for Read Request of Phone Alert Status
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_pass_descr_value_t type.
     */
    CY_BLE_EVT_PASSC_READ_DESCR_RESPONSE,

    /** PASS Client - Write Response for Write Request of Phone Alert Status
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_pass_descr_value_t type.
     */
    CY_BLE_EVT_PASSC_WRITE_DESCR_RESPONSE,


    /****************************************
     * Pulse Oximeter Service Events
     ***************************************/

    /** PLXS Server - Indication for Pulse Oximeter Service Characteristic
     *  was enabled. The parameter of this event is a structure
     *  of cy_stc_ble_plxs_char_value_t type.
     */
    CY_BLE_EVT_PLXSS_INDICATION_ENABLED,

    /** PLXS Server - Indication for Pulse Oximeter Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_gls_char_value_t type.
     */
    CY_BLE_EVT_PLXSS_INDICATION_DISABLED,

    /** PLXS Server - Pulse Oximeter Service Characteristic
     *  Indication was confirmed. The parameter of this event
     *  is a structure of the cy_stc_ble_plxs_char_value_t type.
     */
    CY_BLE_EVT_PLXSS_INDICATION_CONFIRMED,

    /** PLXS Server - Notification for Pulse Oximeter Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_plxs_char_value_t type.
     */
    CY_BLE_EVT_PLXSS_NOTIFICATION_ENABLED,

    /** PLXS Server - Notification for Pulse Oximeter Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_plxs_char_value_t type.
     */
    CY_BLE_EVT_PLXSS_NOTIFICATION_DISABLED,

    /** PLXS Server - Write Request for Pulse Oximeter Service
     *  was received. The parameter of this event is a structure
     *  of cy_stc_ble_plxs_char_value_t type.
     */
    CY_BLE_EVT_PLXSS_WRITE_CHAR,

    /** PLXS Client - Pulse Oximeter Service Characteristic
     *  Indication was received. The parameter of this event
     *  is a structure of the cy_stc_ble_plxs_char_value_t type.
     */
    CY_BLE_EVT_PLXSC_INDICATION,

    /** PLXS Client - Pulse Oximeter Service Characteristic
     *  Notification was received. The parameter of this event
     *  is a structure of the cy_stc_ble_plxs_char_value_t type.
     */
    CY_BLE_EVT_PLXSC_NOTIFICATION,

    /** PLXS Client - Read Response for Read Request of Pulse Oximeter
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_plxs_char_value_t type.
     */
    CY_BLE_EVT_PLXSC_READ_CHAR_RESPONSE,

    /** PLXS Client - Write Response for Write Request of Pulse Oximeter
     *  Service Characteristic value. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_plxs_char_value_t type.
     */
    CY_BLE_EVT_PLXSC_WRITE_CHAR_RESPONSE,

    /** PLXS Client - Read Response for Read Request of Pulse Oximeter
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_plxs_descr_value_t type.
     */
    CY_BLE_EVT_PLXSC_READ_DESCR_RESPONSE,

    /** PLXS Client - Write Response for Write Request of Pulse Oximeter
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_plxs_descr_value_t type.
     */
    CY_BLE_EVT_PLXSC_WRITE_DESCR_RESPONSE,

    /** PLXS Client - PLX RACP procedure timeout was received. The parameter
     *  of this event is a structure of the cy_stc_ble_plxs_char_value_t type.
     */
    CY_BLE_EVT_PLXSC_TIMEOUT,

    /****************************************
     * Running Speed and Cadence Service Events
     ***************************************/

    /** RSCS Server - Notification for Running Speed and Cadence Service
     *  Characteristic was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_rscs_char_value_t type.
     */
    CY_BLE_EVT_RSCSS_NOTIFICATION_ENABLED,

    /** RSCS Server - Notification for Running Speed and Cadence Service
     *  Characteristic was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_rscs_char_value_t type.
     */
    CY_BLE_EVT_RSCSS_NOTIFICATION_DISABLED,

    /** RSCS Server - Indication for Running Speed and Cadence Service Characteristic
     *  was enabled. The parameter of this event is a structure
     *  of the cy_stc_ble_rscs_char_value_t type.
     */
    CY_BLE_EVT_RSCSS_INDICATION_ENABLED,

    /** RSCS Server - Indication for Running Speed and Cadence Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_rscs_char_value_t type.
     */
    CY_BLE_EVT_RSCSS_INDICATION_DISABLED,

    /** RSCS Server - Running Speed and Cadence Service Characteristic
     *  Indication was confirmed. The parameter of this event
     *  is a structure of the cy_stc_ble_rscs_char_value_t type.
     */
    CY_BLE_EVT_RSCSS_INDICATION_CONFIRMATION,

    /** RSCS Server - Write Request for Running Speed and Cadence Service
     *  Characteristic was received. The parameter of this event is a structure
     *  of cy_stc_ble_rscs_char_value_t type.
     */
    CY_BLE_EVT_RSCSS_WRITE_CHAR,

    /** RSCS Client - Running Speed and Cadence Service Characteristic
     *  Notification was received. The parameter of this event
     *  is a structure of the cy_stc_ble_rscs_char_value_t type.
     */
    CY_BLE_EVT_RSCSC_NOTIFICATION,

    /** RSCS Client - Running Speed and Cadence Service Characteristic
     *  Indication was received. The parameter of this event
     *  is a structure of the cy_stc_ble_rscs_char_value_t type.
     */
    CY_BLE_EVT_RSCSC_INDICATION,

    /** RSCS Client - Read Response for Read Request of Running Speed and Cadence
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_rscs_char_value_t type.
     */
    CY_BLE_EVT_RSCSC_READ_CHAR_RESPONSE,

    /** RSCS Client - Write Response for Write Request of Running Speed and Cadence
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_rscs_char_value_t type.
     */
    CY_BLE_EVT_RSCSC_WRITE_CHAR_RESPONSE,

    /** RSCS Client - Read Response for Read Request of Running Speed and Cadence
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_rscs_descr_value_t type.
     */
    CY_BLE_EVT_RSCSC_READ_DESCR_RESPONSE,

    /** RSCS Client - Write Response for Write Request of Running Speed and Cadence
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_rscs_descr_value_t type.
     */
    CY_BLE_EVT_RSCSC_WRITE_DESCR_RESPONSE,


    /****************************************
     * Reference Time Update Service Events
     ***************************************/

    /** RTUS Server - Write command request for Reference Time Update
     *  Characteristic value. The parameter of this event
     *  is a structure of cy_stc_ble_rtus_char_value_t type.
     */
    CY_BLE_EVT_RTUSS_WRITE_CHAR_CMD,

    /** RTUS Client - Read Response for Read Request of Reference Time Update
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_rtus_char_value_t type.
     */
    CY_BLE_EVT_RTUSC_READ_CHAR_RESPONSE,


    /****************************************
     * Scan Parameters Service Events
     ***************************************/

    /** ScPS Server - Notification for Scan Refresh Characteristic
     *  was enabled. The parameter of this event is a structure
     *  of the cy_stc_ble_scps_char_value_t type.
     */
    CY_BLE_EVT_SCPSS_NOTIFICATION_ENABLED,

    /** ScPS Server - Notification for Scan Refresh Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_scps_char_value_t type.
     */
    CY_BLE_EVT_SCPSS_NOTIFICATION_DISABLED,

    /** ScPS Client - Read Response for Scan Interval Window
     *  Characteristic Value of Scan Parameters Service. The
     *  parameter of this event is a structure
     *  of the cy_stc_ble_scps_char_value_t type.
     */
    CY_BLE_EVT_SCPSS_SCAN_INT_WIN_WRITE_CHAR,

    /** ScPS Client - Scan Refresh Characteristic Notification
     *  was received. The parameter of this event is a
     *  structure of the cy_stc_ble_scps_char_value_t type.
     */
    CY_BLE_EVT_SCPSC_NOTIFICATION,

    /** ScPS Client - Read Response for Scan Refresh Characteristic
     *  Descriptor Read Request. The parameter of this event is a
     *  structure of the cy_stc_ble_scps_descr_value_t type.
     */
    CY_BLE_EVT_SCPSC_READ_DESCR_RESPONSE,

    /** ScPS Client - Write Response for Scan Refresh Client
     *  Characteristic Configuration Descriptor Value. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_scps_descr_value_t type.
     */
    CY_BLE_EVT_SCPSC_WRITE_DESCR_RESPONSE,


    /****************************************
     * Tx Power Service Events
     ***************************************/

    /** TPS Server - Notification for Tx Power Level Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_tps_char_value_t type.
     */
    CY_BLE_EVT_TPSS_NOTIFICATION_ENABLED,

    /** TPS Server - Notification for Tx Power Level Characteristic
     *  was disabled. The parameter of this event is a structure of
     *  the cy_stc_ble_tps_char_value_t type.
     */
    CY_BLE_EVT_TPSS_NOTIFICATION_DISABLED,

    /** TPS Client - Tx Power Level Characteristic Notification.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_tps_char_value_t type.
     */
    CY_BLE_EVT_TPSC_NOTIFICATION,

    /** TPS Client - Read Response for Tx Power Level Characteristic
     *  Value Read Request. The parameter of this event is a
     *  structure of the cy_stc_ble_tps_char_value_t type.
     */
    CY_BLE_EVT_TPSC_READ_CHAR_RESPONSE,

    /** TPS Client - Read Response for Tx Power Level Client
     *  Characteristic Configuration Descriptor Value Read Request.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_tps_descr_value_t type.
     */
    CY_BLE_EVT_TPSC_READ_DESCR_RESPONSE,

    /** TPS Client - Write Response for Tx Power Level Characteristic
     *  Descriptor Value Write Request. The parameter of this event
     *  is a structure of the cy_stc_ble_tps_descr_value_t type.
     */
    CY_BLE_EVT_TPSC_WRITE_DESCR_RESPONSE,


    /****************************************
     * User Data Service Events
     ***************************************/

    /** UDS Server - Indication for User Data Service Characteristic
     *  was enabled. The parameter of this event is a structure
     *  of cy_stc_ble_uds_char_value_t type.
     */
    CY_BLE_EVT_UDSS_INDICATION_ENABLED,

    /** UDS Server - Indication for User Data Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_uds_char_value_t type.
     */
    CY_BLE_EVT_UDSS_INDICATION_DISABLED,

    /** UDS Server - User Data Service Characteristic
     *  Indication was confirmed. The parameter of this event
     *  is a structure of the cy_stc_ble_uds_char_value_t type.
     */
    CY_BLE_EVT_UDSS_INDICATION_CONFIRMED,

    /** UDS Server - Notification for User Data Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_uds_char_value_t type.
     */
    CY_BLE_EVT_UDSS_NOTIFICATION_ENABLED,

    /** UDS Server - Notification for User Data Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_uds_char_value_t type.
     */
    CY_BLE_EVT_UDSS_NOTIFICATION_DISABLED,

    /** UDS Server - Read Request for User Data Service Characteristic
     *  was received. The parameter of this event is a structure
     *  of the cy_stc_ble_uds_char_value_t type.
     */
    CY_BLE_EVT_UDSS_READ_CHAR,

    /** UDS Server - Write Request for User Data Service Characteristic
     *  was received. The parameter of this event is a structure
     *  of the cy_stc_ble_uds_char_value_t type.
     */
    CY_BLE_EVT_UDSS_WRITE_CHAR,

    /** UDS Client - User Data Service Characteristic
     *  Indication was received. The parameter of this event
     *  is a structure of the cy_stc_ble_uds_char_value_t type.
     */
    CY_BLE_EVT_UDSC_INDICATION,

    /** UDS Client - User Data Service Characteristic
     *  Notification was received. The parameter of this event
     *  is a structure of the cy_stc_ble_uds_char_value_t type.
     */
    CY_BLE_EVT_UDSC_NOTIFICATION,

    /** UDS Client - Read Response for Read Request of User Data
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_uds_char_value_t type.
     */
    CY_BLE_EVT_UDSC_READ_CHAR_RESPONSE,

    /** UDS Client - Write Response for Write Request of User Data
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_uds_char_value_t type.
     */
    CY_BLE_EVT_UDSC_WRITE_CHAR_RESPONSE,

    /** UDS Client - Read Response for Read Request of User Data
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_uds_descr_value_t type.
     */
    CY_BLE_EVT_UDSC_READ_DESCR_RESPONSE,

    /** UDS Client - Write Response for Write Request of User Data
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_uds_descr_value_t type.
     */
    CY_BLE_EVT_UDSC_WRITE_DESCR_RESPONSE,

    /** UDS Client - Error Response for Write Request for User Data Service
     *  Characteristic Value. The parameter of this event is a structure of
     *  the cy_stc_ble_uds_char_value_t type.
     */
    CY_BLE_EVT_UDSC_ERROR_RESPONSE,


    /****************************************
    * Wireless Power Transfer Service Events
    ****************************************/

    /** WPTS Server - Notification for Wireless Power Transfer Service Characteristic
     *  was enabled. The parameter of this event is a structure of
     *  the cy_stc_ble_wpts_char_value_t type.
     */
    CY_BLE_EVT_WPTSS_NOTIFICATION_ENABLED,

    /** WPTS Server - Notification for Wireless Power Transfer Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_wpts_char_value_t type.
     */
    CY_BLE_EVT_WPTSS_NOTIFICATION_DISABLED,

    /** WPTS Server - Indication for Wireless Power Transfer Service Characteristic
     *  was enabled. The parameter of this event is a structure
     *  of the cy_stc_ble_wpts_char_value_t type.
     */
    CY_BLE_EVT_WPTSS_INDICATION_ENABLED,

    /** WPTS Server - Indication for Wireless Power Transfer Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_wpts_char_value_t type.
     */
    CY_BLE_EVT_WPTSS_INDICATION_DISABLED,

    /** WPTS Server - Wireless Power Transfer Service Characteristic
     *  Indication was confirmed. The parameter of this event
     *  is a structure of the cy_stc_ble_wpts_char_value_t type.
     */
    CY_BLE_EVT_WPTSS_INDICATION_CONFIRMED,

    /** WPTS Server - Write Request for Wireless Power Transfer Service Characteristic
     *  was received. The parameter of this event is a structure
     *  of cy_stc_ble_wpts_char_value_t type.
     */
    CY_BLE_EVT_WPTSS_WRITE_CHAR,

    /** WPTS Client - Wireless Power Transfer Service Characteristic
     *  Notification was received. The parameter of this event
     *  is a structure of the cy_stc_ble_wpts_char_value_t type.
     */
    CY_BLE_EVT_WPTSC_NOTIFICATION,

    /** WPTS Client - Wireless Power Transfer Service Characteristic
     *  Indication was received. The parameter of this event
     *  is a structure of the cy_stc_ble_wpts_char_value_t type.
     */
    CY_BLE_EVT_WPTSC_INDICATION,

    /** WPTS Client - Write Response for Read Request of Wireless Power Transfer
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_wpts_char_value_t type.
     */
    CY_BLE_EVT_WPTSC_WRITE_CHAR_RESPONSE,

    /** WPTS Client - Read Response for Read Request of Wireless Power Transfer
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_wpts_char_value_t type.
     */
    CY_BLE_EVT_WPTSC_READ_CHAR_RESPONSE,

    /** WPTS Client - Read Response for Read Request of Wireless Power Transfer
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  the cy_stc_ble_wpts_descr_value_t type.
     */
    CY_BLE_EVT_WPTSC_READ_DESCR_RESPONSE,

    /** WPTS Client - Write Response for Write Request of Wireless Power Transfer
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_wpts_descr_value_t type.
     */
    CY_BLE_EVT_WPTSC_WRITE_DESCR_RESPONSE,


    /****************************************
    *     Weight Scale Service Events
    ****************************************/

    /** WSS Server - Indication for Weight Scale Service Characteristic
     *  was enabled. The parameter of this event is a structure
     *  of the cy_stc_ble_wss_char_value_t type.
     */
    CY_BLE_EVT_WSSS_INDICATION_ENABLED,

    /** WSS Server - Indication for Weight Scale Service Characteristic
     *  was disabled. The parameter of this event is a structure
     *  of the cy_stc_ble_wss_char_value_t type.
     */
    CY_BLE_EVT_WSSS_INDICATION_DISABLED,

    /** WSS Server - Weight Scale Service Characteristic
     *  Indication was confirmed. The parameter of this event
     *  is a structure of the cy_stc_ble_wss_char_value_t type.
     */
    CY_BLE_EVT_WSSS_INDICATION_CONFIRMED,

    /** WSS Client - Weight Scale Service Characteristic
     *  Indication was received. The parameter of this event
     *  is a structure of the cy_stc_ble_wss_char_value_t type.
     */
    CY_BLE_EVT_WSSC_INDICATION,
    /** WSS Client - Read Response for Read Request of Weight Scale
     *  Service Characteristic value. The parameter of this event
     *  is a structure of the cy_stc_ble_wss_char_value_t type.
     */
    CY_BLE_EVT_WSSC_READ_CHAR_RESPONSE,

    /** WSS Client - Read Response for Read Request of Weight Scale
     *  Service Characteristic Descriptor Read request. The
     *  parameter of this event is a structure of
     *  cy_stc_ble_wss_descr_value_t type.
     */
    CY_BLE_EVT_WSSC_READ_DESCR_RESPONSE,

    /** WSS Client - Write Response for Write Request of Weight Scale
     *  Service Characteristic Configuration Descriptor value.
     *  The parameter of this event is a structure of
     *  the cy_stc_ble_wss_descr_value_t type.
     */
    CY_BLE_EVT_WSSC_WRITE_DESCR_RESPONSE,

    /****************************************
    *     Bootloader Service Events
    ****************************************/
    /** BT Server - Notification for Bootloader Service Characteristic
     *  was enabled.
     */
    CY_BLE_EVT_BTSS_NOTIFICATION_ENABLED,

    /** BT Server - Notification for Bootloader Service Characteristic
     *  was disabled.
     */
    CY_BLE_EVT_BTSS_NOTIFICATION_DISABLED,
    /** BT Server - Write Request event for the Bootloader Service Characteristic.
     *  The parameter of this event is a structure of the cy_stc_ble_bts_char_value_t type.
     */
    CY_BLE_EVT_BTSS_WRITE_REQ,
    /** BT Server - Write Without Response Request event for the Bootloader Service Characteristic.
     *  The parameter of this event is a structure of the cy_stc_ble_bts_char_value_t type.
     */
    CY_BLE_EVT_BTSS_WRITE_CMD_REQ,
    /** Send Prepare Write Response that identifies acknowledgement for
     *  long characteristic value write. The parameter of this event is a structure of
     *  the cy_stc_ble_gatts_prep_write_req_param_t type.
     */
    CY_BLE_EVT_BTSS_PREP_WRITE_REQ,

    /** Execute Write Request for Bootloader Service
     *  Characteristic was received. The parameter of this event is a structure of
     *  the cy_stc_ble_gatts_exec_write_req_t type.
     */
    CY_BLE_EVT_BTSS_EXEC_WRITE_REQ,


    /****************************************
     * Discovery Procedure Events
     ***************************************/

    /** Discovery Services Event. The parameter of this event is a structure of
     *  the cy_stc_ble_disc_srv_info_t type.
     */
    CY_BLE_EVT_GATTC_DISC_SERVICE,

    /** Discovery Includes Event. The parameter of this event is a structure of
     *  the cy_stc_ble_disc_incl_info_t type.
     */
    CY_BLE_EVT_GATTC_DISC_INCL,

    /** Discovery Characteristic Event. The parameter of this event is a structure of
     *  the cy_stc_ble_disc_char_info_t type.
     */
    CY_BLE_EVT_GATTC_DISC_CHAR,

    /** Discovery Descriptors Event. The parameter of this event is a structure of
     *  the cy_stc_ble_disc_descr_info_t type.
     */
    CY_BLE_EVT_GATTC_DISC_DESCR,

    /** Event for run a procedure which returns a possible range of the current
     *  characteristic descriptor. The parameter of this event is a structure
     *  of the cy_stc_ble_disc_range_info_t type.
     */
    CY_BLE_EVT_GATTC_DISC_DESCR_GET_RANGE
}cy_en_ble_evt_t;

/** @} group_service_api_events */

#if (CY_BLE_GATT_ROLE_CLIENT)


/** Service indexes (SRVI) */
typedef enum
{
    CY_BLE_SRVI_GAP,
    CY_BLE_SRVI_GATT,

#ifdef CY_BLE_ANCS_CLIENT
    CY_BLE_SRVI_ANCS,
#endif /* CY_BLE_ANCS_CLIENT */
#ifdef CY_BLE_ANS_CLIENT
    CY_BLE_SRVI_ANS,
#endif /* CY_BLE_ANS_CLIENT */
#ifdef CY_BLE_AIOS_CLIENT
    CY_BLE_SRVI_AIOS,
#endif /* CY_BLE_AIOS_CLIENT */
#ifdef CY_BLE_BAS_CLIENT
    CY_BLE_SRVI_BAS,
    CY_BLE_SRVI_BAS_END = (uint8_t)(CY_BLE_SRVI_BAS) + CY_BLE_BASC_SERVICE_COUNT - 1u,
#endif /* CY_BLE_BAS_CLIENT */
#ifdef CY_BLE_BCS_CLIENT
    CY_BLE_SRVI_BCS,
#endif /* CY_BLE_BCS_CLIENT */
#ifdef CY_BLE_BLS_CLIENT
    CY_BLE_SRVI_BLS,
#endif /* CY_BLE_BLS_CLIENT */
#ifdef CY_BLE_BMS_CLIENT
    CY_BLE_SRVI_BMS,
#endif /* CY_BLE_BMS_CLIENT */
#ifdef CY_BLE_CGMS_CLIENT
    CY_BLE_SRVI_CGMS,
#endif /* CY_BLE_CGMS_CLIENT */
#ifdef CY_BLE_CPS_CLIENT
    CY_BLE_SRVI_CPS,
#endif /* CY_BLE_CPS_CLIENT */
#ifdef CY_BLE_CSCS_CLIENT
    CY_BLE_SRVI_CSCS,
#endif /* CY_BLE_CSCS_CLIENT */
#ifdef CY_BLE_CTS_CLIENT
    CY_BLE_SRVI_CTS,
#endif /* CY_BLE_CTS_CLIENT */
#ifdef CY_BLE_CUSTOM_CLIENT
    CY_BLE_SRVI_CUSTOMS,
    CY_BLE_SRVI_CUSTOMS_END =(uint8_t)(CY_BLE_SRVI_CUSTOMS )+ CY_BLE_CUSTOMC_SERVICE_COUNT - 1u,
#endif /* CY_BLE_CUSTOM_CLIENT */
#ifdef CY_BLE_DIS_CLIENT
    CY_BLE_SRVI_DIS,
#endif /* CY_BLE_DIS_CLIENT */
#ifdef CY_BLE_ESS_CLIENT
    CY_BLE_SRVI_ESS,
#endif /* CY_BLE_ESS_CLIENT */
#ifdef CY_BLE_GLS_CLIENT
    CY_BLE_SRVI_GLS,
#endif /* CY_BLE_GLS_CLIENT */
#ifdef CY_BLE_HIDS_CLIENT
    CY_BLE_SRVI_HIDS,
    CY_BLE_SRVI_HIDS_END = (uint8_t)(CY_BLE_SRVI_HIDS )+ CY_BLE_HIDSC_SERVICE_COUNT - 1u,
#endif /* CY_BLE_HIDS_CLIENT */
#ifdef CY_BLE_HPS_CLIENT
    CY_BLE_SRVI_HPS,
#endif /* CY_BLE_HPS_CLIENT */
#ifdef CY_BLE_HRS_CLIENT
    CY_BLE_SRVI_HRS,
#endif /* CY_BLE_HRS_CLIENT */
#ifdef CY_BLE_HTS_CLIENT
    CY_BLE_SRVI_HTS,
#endif /* CY_BLE_HTS_CLIENT */
#ifdef CY_BLE_IAS_CLIENT
    CY_BLE_SRVI_IAS,
#endif /* CY_BLE_IAS_CLIENT */
#ifdef CY_BLE_IPS_CLIENT
    CY_BLE_SRVI_IPS,
#endif /* CY_BLE_IPS_CLIENT */
#ifdef CY_BLE_LLS_CLIENT
    CY_BLE_SRVI_LLS,
#endif /* CY_BLE_LLS_CLIENT */
#ifdef CY_BLE_LNS_CLIENT
    CY_BLE_SRVI_LNS,
#endif /* CY_BLE_LNS_CLIENT */
#ifdef CY_BLE_NDCS_CLIENT
    CY_BLE_SRVI_NDCS,
#endif /* CY_BLE_NDCS_CLIENT */
#ifdef CY_BLE_IPSS_CLIENT
    CY_BLE_SRVI_IPSS,
#endif /* CY_BLE_IPSS_CLIENT */
#ifdef CY_BLE_PASS_CLIENT
    CY_BLE_SRVI_PASS,
#endif /* CY_BLE_PASS_CLIENT */
#ifdef CY_BLE_PLXS_CLIENT
    CY_BLE_SRVI_PLXS,
#endif /* CY_BLE_PLXS_CLIENT */
#ifdef CY_BLE_RSCS_CLIENT
    CY_BLE_SRVI_RSCS,
#endif /* CY_BLE_RSCS_CLIENT */
#ifdef CY_BLE_RTUS_CLIENT
    CY_BLE_SRVI_RTUS,
#endif /* CY_BLE_RTUS_CLIENT */
#ifdef CY_BLE_SCPS_CLIENT
    CY_BLE_SRVI_SCPS,
#endif /* CY_BLE_SCPS_CLIENT */
#ifdef CY_BLE_TPS_CLIENT
    CY_BLE_SRVI_TPS,
#endif /* CY_BLE_TPS_CLIENT */
#ifdef CY_BLE_UDS_CLIENT
    CY_BLE_SRVI_UDS,
#endif /* CY_BLE_UDS_CLIENT */
#ifdef CY_BLE_WPTS_CLIENT
    CY_BLE_SRVI_WPTS,
#endif /* CY_BLE_WPTS_CLIENT */
#ifdef CY_BLE_WSS_CLIENT
    CY_BLE_SRVI_WSS,
#endif /* CY_BLE_WSS_CLIENT */
    CY_BLE_SRVI_COUNT /**< Total count of services */
}cy_en_ble_srvi_t;

/** Discovery information structure type */
typedef struct
{
    /** AUTO DISCOVERY flag */
    uint8_t  autoDiscoveryFlag;

    /** Counter for discovery procedures */
    uint32_t servCount;

    /** Characteristic Counter for discovery procedures */
    uint32_t charCount;

    /** Discovery included service information, used for 128-bit UUID read response handle
     *  and for continue discovery.
     */
    cy_stc_ble_disc_incl_info_t         inclInfo;

    /** GAP Service characteristics server's GATT DB handles structure */
    cy_stc_ble_gapc_t                   gapc;

    /** GATT Service characteristics server's GATT DB handles structure */
    cy_stc_ble_gattc_t                  gattc;

    /** GATT Attribute Handle Range type */
    cy_stc_ble_gatt_attr_handle_range_t gattcDiscoveryRange;

    /** Peer device handle index */
    uint32_t                            connIndex;
}cy_stc_ble_discovery_t;


#endif /* CY_BLE_GATT_ROLE_CLIENT */

/***************************************
* Registration Callback
***************************************/

#if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL)
typedef cy_en_ble_gatt_err_code_t (* cy_ble_event_handle_t) (uint32_t eventCode, void *eventParam);

/** Registration Service's Event Handler structure type */
typedef struct
{
    /** The array for storing reference to the service's event handler procedure. */
    cy_ble_event_handle_t serviceEventHandler[CY_BLE_MAX_SRVI];

    /** Count of functions in the array */
    uint8_t               count;
} cy_stc_ble_event_handler_t;

/* using: cy_ble_eventHandler.serviceEventHandler(eventCode, eventParam); */

/* Registration Service's Event Handler functions */
cy_en_ble_api_result_t Cy_BLE_RegisterServiceEventHandler(cy_ble_event_handle_t eventHandlerFunc);
cy_en_ble_gatt_err_code_t Cy_BLE_InvokeServiceEventHandler(uint32_t eventCode, void *eventParam);

extern cy_stc_ble_event_handler_t cy_ble_eventHandler;
#endif /* CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_CENTRAL*/


/***************************************
* Function Prototypes
***************************************/

/* Event Handler functions */
void Cy_BLE_EventHandler(cy_en_ble_event_t event, void *evParam);
void Cy_BLE_ReadByGroupEventHandler(cy_stc_ble_gattc_read_by_grp_rsp_param_t *eventParam);
void Cy_BLE_ReadByTypeEventHandler(cy_stc_ble_gattc_read_by_type_rsp_param_t *eventParam);
void Cy_BLE_FindInfoEventHandler(cy_stc_ble_gattc_find_info_rsp_param_t *eventParam);
void Cy_BLE_ErrorResponseEventHandler(const cy_stc_ble_gatt_err_param_t *eventParam);
void Cy_BLE_LongProcedureEndEventHandler(cy_stc_ble_conn_handle_t connHandle);
void Cy_BLE_NextInclDiscovery(cy_stc_ble_conn_handle_t connHandle, uint8_t incrementIndex);
void Cy_BLE_NextCharDiscovery(cy_stc_ble_conn_handle_t connHandle, uint8_t incrementIndex);
void Cy_BLE_NextCharDscrDiscovery(cy_stc_ble_conn_handle_t connHandle, uint8_t incrementIndex);

/***************************************
* External data references
***************************************/

extern volatile uint8_t cy_ble_busyStatus[CY_BLE_CONN_COUNT];
extern volatile uint32_t cy_ble_cmdStatus;

#if (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)
extern uint8_t cy_ble_advertisingIntervalType;
#endif /* (CY_BLE_GAP_ROLE_PERIPHERAL || CY_BLE_GAP_ROLE_BROADCASTER)  */

#if (CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER)
extern uint8_t cy_ble_scanningIntervalType;
#endif /* CY_BLE_GAP_ROLE_CENTRAL || CY_BLE_GAP_ROLE_OBSERVER */

#if (CY_BLE_GATT_ROLE_CLIENT)
extern cy_stc_ble_discovery_t cy_ble_discovery[CY_BLE_CONFIG_GATTC_COUNT];
extern cy_stc_ble_disc_srvc_info_t cy_ble_serverInfo[CY_BLE_CONFIG_GATTC_COUNT][CY_BLE_SRVI_COUNT];
#endif /* CY_BLE_GATT_ROLE_CLIENT */

/**
 * \addtogroup group_ble_common_api_gatt_functions
 * @{
 */

/******************************************************************************
* Function Name: Cy_BLE_GATT_GetBusyStatus
***************************************************************************//**
*
*  This function returns a status of the BLE stack (busy or not busy).
*  The status is changed after the CY_BLE_EVT_STACK_BUSY_STATUS event.
*
*  \param attId: Identifies the active ATT connection Instance.
*
* \return
*  uint8_t: The busy status.
*   * CY_BLE_STACK_STATE_BUSY - The BLE stack is busy.
*   * CY_BLE_STACK_STATE_FREE - The BLE stack is not busy.
*
******************************************************************************/
#define Cy_BLE_GATT_GetBusyStatus(attId)    (cy_ble_busyStatus[attId])
/** @} group_ble_common_api_gatt_functions */

/**
 * \addtogroup group_ble_common_api_functions
 * @{
 */

/** \cond IGNORE */
/** Macro to check the characteristic handle */
#define CY_BLE_GapcCheckCharHandle(handle, discCharInfo)                     \
    do {                                                                     \
        if((handle) == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)                \
        {                                                                    \
            (handle) = (discCharInfo)->valueHandle;                          \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, \
                                &(discCharInfo));                            \
        }                                                                    \
    } while(0)

/** Macro to check and store the characteristic handle */
#define Cy_BLE_CheckStoreCharHandle(handle)                                  \
    do {                                                                     \
        if((handle).valueHandle == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)    \
        {                                                                    \
            (handle).valueHandle = discCharInfo->valueHandle;                \
            (handle).properties = discCharInfo->properties;                  \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, \
                                &discCharInfo);                              \
        }                                                                    \
    } while(0)

/** Macro to check and store the characteristic descriptor handle */
#define Cy_BLE_CheckStoreCharDescrHandle(handle)                             \
    do {                                                                     \
        if((handle) == CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE)                \
        {                                                                    \
            (handle) = discDescrInfo->descrHandle;                           \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            Cy_BLE_ApplCallback((uint32_t)CY_BLE_EVT_GATTC_CHAR_DUPLICATION, \
                                &discDescrInfo);                             \
        }                                                                    \
    } while(0)


/******************************************************************************
* Function Name: Cy_BLE_Get16ByPtr
***************************************************************************//**
*
*  Returns the two-bytes value by using a pointer to the LSB.
*
*  \param ptr: Pointer to the LSB of two-bytes data (little-endian).
*
*  \return
*  uint16_t: Two-bytes data.
*
******************************************************************************/
__STATIC_INLINE uint16_t Cy_BLE_Get16ByPtr(const uint8_t ptr[])
{
    return((uint16_t)ptr[0u] | ((uint16_t)(((uint16_t)ptr[1u]) << 8u)));
}


/******************************************************************************
* Function Name: Cy_BLE_Set16ByPtr
***************************************************************************//**
*
*  Sets the two-bytes value by using a pointer to the LSB.
*
*  \param ptr:    Pointer to the LSB of two-bytes data (little-endian).
*  \param value:  Two-bytes data to be written.
*
*  \return
*  None
*
******************************************************************************/
__STATIC_INLINE void Cy_BLE_Set16ByPtr(uint8_t ptr[],
                                       uint16_t value)
{
    ptr[0u] = (uint8_t)value;
    ptr[1u] = (uint8_t)(value >> 8u);
}

/******************************************************************************
* Function Name: Cy_BLE_Get24ByPtr
***************************************************************************//**
*
*  Returns the three-bytes value by using a pointer to the LSB.
*
*  \param ptr: Pointer to the LSB of two-bytes data (little-endian).
*
*  \return
*  uint16_t: Two-bytes data.
*
******************************************************************************/
__STATIC_INLINE uint32_t Cy_BLE_Get24ByPtr(const uint8_t ptr[])
{
    return(((uint32_t)ptr[0u]) | ((uint32_t)(((uint32_t)ptr[1u]) << 8u)) | ((uint32_t)((uint32_t)ptr[2u]) << 16u));
}


/** \endcond */


/***************************************
* Function Prototypes
***************************************/

#if (CY_BLE_GATT_ROLE_SERVER || CY_BLE_GATT_ROLE_CLIENT)

uint8_t Cy_BLE_IsDeviceAddressValid(const cy_stc_ble_gap_bd_addr_t *deviceAddress);

#endif /* (CY_BLE_GATT_ROLE_SERVER || CY_BLE_GATT_ROLE_CLIENT) */

/** @} group_common_api_functions */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE) */
#endif /* CY_BLE_EVENT_HANDLER_H */


/* [] END OF FILE */
