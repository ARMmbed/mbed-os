/***************************************************************************//**
* \file cy_ble_plxs.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for the Pulse Oximeter
*  Service.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CY_BLE_PLXS_H
#define CY_BLE_PLXS_H

#include "cy_ble_gatt.h"
#include "cy_ble_plxs.h"
/**
 * \addtogroup group_ble_service_api_PLXS_definitions
 * @{
 */


#if (CY_BLE_MODE_PROFILE && defined(CY_BLE_PLXS))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/***************************************
* Data Struct Definition
***************************************/

/** PLXS Characteristic indexes */
typedef enum
{
    CY_BLE_PLXS_SCMT,                        /**< The PLX Spot-check Measurement characteristic, if supported,
                                              * is used to send Spot-check measurements of SpO2 (Percent oxygen
                                              * saturation of hemoglobin) and PR (pulse rate). This
                                              * characteristic is a variable length structure containing the
                                              * Flags field, the SpO2PR-Spot-Check field, and depending on the
                                              * contents of the Flags field, the Timestamp field, the Measurement
                                              * Status field, the Device and Sensor Status field, and/or the
                                              * Pulse Amplitude Index field. */

    CY_BLE_PLXS_CTMT,                        /**< The PLX Continuous Measurement characteristic, if supported,
                                              * is used to send periodic pulse oximetry measurements. This
                                              * characteristic is a variable length structure containing the
                                              * Flags field (to indicate presence of optional fields), the
                                              * SpO2PR-Normal field, and depending on the contents of the Flags
                                              * field, the SpO2PR-Fast field, the SpO2PR-Slow field, the
                                              * Measurement Status field, the Device and Sensor Status field,
                                              * and/or the Pulse Amplitude Index field.*/

    CY_BLE_PLXS_FEAT,                        /**< The PLX Features characteristic is used to describe the
                                              * supported features of the Server. Included in the characteristic
                                              * is a PLX Features field, and, depending on the contents of the
                                              * PLX Features field, the Measurement Status Support field, and
                                              * the Device and Sensor Status Support field. */

    CY_BLE_PLXS_RACP,                        /**< This control point is used with a service to provide basic
                                              * management functionality for the PLX Sensor patient record
                                              * database. This enables functions including counting records,
                                              * transmitting records and clearing records based on filter
                                              * criterion. The filter criterion in the Operand field is defined
                                              * by the service that references this characteristic as is the
                                              * format of a record (that may be comprised of one or more
                                              * characteristics) and the sequence of transferred records.*/

    CY_BLE_PLXS_CHAR_COUNT                   /**< Total count of PLXS characteristics */
}cy_en_ble_plxs_char_index_t;


/** PLXS Characteristic Descriptors indexes */
typedef enum
{
    CY_BLE_PLXS_CCCD,                        /**< Client Characteristic Configuration Descriptor index */
    CY_BLE_PLXS_DESCR_COUNT                  /**< Total count of descriptors */
}cy_en_ble_plxs_descr_index_t;

/** PLXS Characteristic with descriptors */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t charHandle;                            /**< Handle of characteristic value */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_PLXS_DESCR_COUNT];  /**< Handle of descriptor */
} cy_stc_ble_plxss_char_t;

/** Structure with PLXS attribute handles */
typedef struct
{
    /** PLXS handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** PLXS Characteristic handles */
    cy_stc_ble_plxss_char_t      charInfo[CY_BLE_PLXS_CHAR_COUNT];
} cy_stc_ble_plxss_t;

/** PLXS Characteristic with descriptors */
typedef struct
{
    cy_ble_gatt_db_attr_handle_t valueHandle;                           /**< Handle of characteristic value */
    cy_ble_gatt_db_attr_handle_t endHandle;                             /**< End handle of characteristic */
    cy_ble_gatt_db_attr_handle_t descrHandle[CY_BLE_PLXS_DESCR_COUNT];  /**< Array of Descriptor handles */
    uint8_t                      properties;                            /**< Properties for value field */
} cy_stc_ble_plxsc_char_t;

/** Structure with discovered attributes information of Pulse Oximeter Service. */
typedef struct
{
    /** Pulse Oximeter Service handle */
    cy_ble_gatt_db_attr_handle_t serviceHandle;

    /** PLXS characteristics info array */
    cy_stc_ble_plxsc_char_t      charInfo[CY_BLE_PLXS_CHAR_COUNT];
} cy_stc_ble_plxsc_t;


/** PLXS Characteristic value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t    connHandle;      /**< Peer device handle */
    cy_en_ble_plxs_char_index_t charIndex;       /**< Index of service characteristic */
    cy_stc_ble_gatt_value_t     *value;          /**< Characteristic value */
    cy_en_ble_gatt_err_code_t   gattErrorCode;   /**< GATT error code for access control */
} cy_stc_ble_plxs_char_value_t;

/** PLXS Characteristic descriptor value parameter structure */
typedef struct
{
    cy_stc_ble_conn_handle_t     connHandle;     /**< Peer device handle */
    cy_en_ble_plxs_char_index_t  charIndex;      /**< Index of service characteristic */
    cy_en_ble_plxs_descr_index_t descrIndex;     /**< Index of descriptor */
    cy_en_ble_gatt_err_code_t    gattErrorCode;  /**< Error code received from application (optional) */
    cy_stc_ble_gatt_value_t      *value;         /**< Characteristic value */
} cy_stc_ble_plxs_descr_value_t;

/** Record Access Control Point characteristic fields defines
** Opcode of the Record Access Control Point characteristic value type
*/
typedef enum
{
    CY_BLE_PLXS_RACP_OPC_RESERVED = 0u,          /**< Reserved for future use (Operator:N/A) */
    CY_BLE_PLXS_RACP_OPC_REPORT_REC,             /**< Report stored records (Operator: Value from Operator Table) */
    CY_BLE_PLXS_RACP_OPC_DELETE_REC,             /**< Delete stored records (Operator: Value from Operator Table) */
    CY_BLE_PLXS_RACP_OPC_ABORT_OPN,              /**< Abort operation (Operator: Null 'value of 0x00 from Operator Table') */
    CY_BLE_PLXS_RACP_OPC_REPORT_NUM_REC,         /**< Report number of stored records (Operator: Value from Operator Table) */
    CY_BLE_PLXS_RACP_OPC_NUM_REC_RSP,            /**< Number of stored records response (Operator: Null 'value of 0x00 from Operator Table') */
    CY_BLE_PLXS_RACP_OPC_RSP_CODE,               /**< Response Code (Operator: Null 'value of 0x00 from Operator Table') */
} cy_en_ble_plxs_racp_opc_t;

/** Operator of the Record Access Control Point characteristic value type */
typedef enum
{
    CY_BLE_PLXS_RACP_OPR_NULL = 0u,              /**< Null */
    CY_BLE_PLXS_RACP_OPR_ALL,                    /**< All records */
    CY_BLE_PLXS_RACP_OPR_LESS,                   /**< Less than or equal to */
    CY_BLE_PLXS_RACP_OPR_GREAT,                  /**< Greater than or equal to */
    CY_BLE_PLXS_RACP_OPR_WITHIN,                 /**< Within range of (inclusive) */
    CY_BLE_PLXS_RACP_OPR_FIRST,                  /**< First record(i.e. oldest record) */
    CY_BLE_PLXS_RACP_OPR_LAST                    /**< Last record (i.e. most recent record) */
} cy_en_ble_plxs_racp_opr_t;

/** Operand of the Record Access Control Point characteristic value type */
typedef enum
{
    CY_BLE_PLXS_RACP_OPD_NA = 0u,                /**< N/A */
    CY_BLE_PLXS_RACP_OPD_1,                      /**< Filter parameters (as appropriate to Operator and Service) */
    CY_BLE_PLXS_RACP_OPD_2,                      /**< Filter parameters (as appropriate to Operator and Service) */
    CY_BLE_PLXS_RACP_OPD_NO_INCL,                /**< Not included */
    CY_BLE_PLXS_RACP_OPD_4,                      /**< Filter parameters (as appropriate to Operator and Service) */
    CY_BLE_PLXS_RACP_OPD_NUM_REC,                /**< Number of Records (Field size defined per service) */
    CY_BLE_PLXS_RACP_OPD_RSP,                    /**< Request Op Code, Response Code Value */
} cy_en_ble_plxs_racp_opd_t;

/** Operand Response Code Values of the Record Access Control Point characteristic value type */
typedef enum
{
    CY_BLE_PLXS_RACP_RSP_NA = 0u,                /**< N/A */
    CY_BLE_PLXS_RACP_RSP_SUCCESS,                /**< Normal response for successful operation */
    CY_BLE_PLXS_RACP_RSP_UNSPRT_OPC,             /**< Normal response if unsupported Op Code is received */
    CY_BLE_PLXS_RACP_RSP_INV_OPR,                /**< Normal response if Operator received does not meet the
                                                 *   requirements of the service (e.g. Null was expected) */
    CY_BLE_PLXS_RACP_RSP_UNSPRT_OPR,             /**< Normal response if unsupported Operator is received */
    CY_BLE_PLXS_RACP_RSP_INV_OPD,                /**< Normal response if Operand received does not meet the
                                                  *   requirements of the service */
    CY_BLE_PLXS_RACP_RSP_NO_REC,                 /**< Normal response if request to report stored records
                                                  *   or request to delete stored records resulted in no
                                                  *   records meeting criteria.  */
    CY_BLE_PLXS_RACP_RSP_UNSUCCESS,              /**< Normal response if request for Abort cannot be completed */
    CY_BLE_PLXS_RACP_RSP_NO_COMPL,               /**< Normal response if request for Abort cannot be completed */
    CY_BLE_PLXS_RACP_RSP_UNSPRT_OPD              /**< Normal response if unsupported Operand is received */
} cy_en_ble_plxs_racp_rsp_t;

/** Service configuration structure */
typedef struct
{
    /** The Pulse Oximeter Service. GATT DB handles structure */
    const cy_stc_ble_plxss_t *plxss;

    /** An application layer event callback function to receive service events from the BLE Component. */
    cy_ble_callback_t        callbackFunc;
} cy_stc_ble_plxs_config_t;


/***************************************
* API Constants
***************************************/
/** RACP Procedure Timeout */
#define CY_BLE_PLXS_RACP_PROCEDURE_TIMEOUT    (5u)             /**< Seconds */

/** PLX internal flag */
#define CY_BLE_PLXS_FLAG_PROCESS              (1u)             /**< Flag indicates that RACP is running */

/** "Device and Sensor Status" field bits */
#define CY_BLE_PLXS_DSS_EDU_BIT               (0x01u << 0u)    /**< Extended Display Update Ongoing bit */
#define CY_BLE_PLXS_DSS_EMD_BIT               (0x01u << 1u)    /**< Equipment Malfunction Detected bit */
#define CY_BLE_PLXS_DSS_SPID_BIT              (0x01u << 2u)    /**< Signal Processing Irregularity Detected bit */
#define CY_BLE_PLXS_DSS_ISD_BIT               (0x01u << 3u)    /**< Inadequite Signal Detected bit */
#define CY_BLE_PLXS_DSS_PSD_BIT               (0x01u << 4u)    /**< Poor Signal Detected bit */
#define CY_BLE_PLXS_DSS_LPD_BIT               (0x01u << 5u)    /**< Low Perfusion Detected bit */
#define CY_BLE_PLXS_DSS_ESD_BIT               (0x01u << 6u)    /**< Erratic Signal Detected bit */
#define CY_BLE_PLXS_DSS_NSD_BIT               (0x01u << 7u)    /**< Nonpulsatile Signal Detected bit */
#define CY_BLE_PLXS_DSS_QPD_BIT               (0x01u << 8u)    /**< Questionable Pulse Detected bit */
#define CY_BLE_PLXS_DSS_SA_BIT                (0x01u << 9u)    /**< Signal Analysis Ongoing bit */
#define CY_BLE_PLXS_DSS_SID_BIT               (0x01u << 10u)   /**< Sensor Interface Detected bit */
#define CY_BLE_PLXS_DSS_SUTU_BIT              (0x01u << 11u)   /**< Sensor Unconnected to User bit */
#define CY_BLE_PLXS_DSS_USC_BIT               (0x01u << 12u)   /**< Unknown Sensor Connected bit */
#define CY_BLE_PLXS_DSS_SD_BIT                (0x01u << 13u)   /**< Sensor Displaced bit */
#define CY_BLE_PLXS_DSS_SM_BIT                (0x01u << 14u)   /**< Sensor Malfunctioning bit */
#define CY_BLE_PLXS_DSS_SDISC_BIT             (0x01u << 15u)   /**< Sensor Disconnected bit */

/** "Measurement Status" field bits */
#define CY_BLE_PLXS_MS_MEAS_BIT               (0x01u << 5u)    /**< Measurement Ongoing bit */
#define CY_BLE_PLXS_MS_EED_BIT                (0x01u << 6u)    /**< Early Estimated Data bit */
#define CY_BLE_PLXS_MS_VDATA_BIT              (0x01u << 7u)    /**< Validated Data bit */
#define CY_BLE_PLXS_MS_FQDATA_BIT             (0x01u << 8u)    /**< Fully Qualified Data bit */
#define CY_BLE_PLXS_MS_DFMS_BIT               (0x01u << 9u)    /**< Data from Measurement Storage bit */
#define CY_BLE_PLXS_MS_DFDEMO_BIT             (0x01u << 10u)   /**< Data for Demonstration bit */
#define CY_BLE_PLXS_MS_DFTEST_BIT             (0x01u << 11u)   /**< Data for Testing bit */
#define CY_BLE_PLXS_MS_CALIB_BIT              (0x01u << 12u)   /**< Calibration Ongoing bit */
#define CY_BLE_PLXS_MS_MUN_BIT                (0x01u << 13u)   /**< Measurement Unavailable bit */
#define CY_BLE_PLXS_MS_QMD_BIT                (0x01u << 14u)   /**< Questionable Measurement Detected bit */
#define CY_BLE_PLXS_MS_IMD_BIT                (0x01u << 15u)   /**< Invalid Measurement Detected bit */

/** "Flag" field bits of PLX Spot-Check Measurement characteristic */
#define CY_BLE_PLXS_SCMT_FLAG_TMSF_BIT        (0x01u << 0u)    /**< Timestamp field bit */
#define CY_BLE_PLXS_SCMT_FLAG_MSF_BIT         (0x01u << 1u)    /**< Measurement Status Field bit */
#define CY_BLE_PLXS_SCMT_FLAG_DSSF_BIT        (0x01u << 2u)    /**< Device and Sensor Status Field bit */
#define CY_BLE_PLXS_SCMT_FLAG_PAIF_BIT        (0x01u << 3u)    /**< Pulse Amplitude Index field bit */
#define CY_BLE_PLXS_SCMT_FLAG_DEVCLK_BIT      (0x01u << 4u)    /**< Device Clock is Not Set bit */

/** "Flag" field bits of PLX Continuous Measurement characteristic */
#define CY_BLE_PLXS_CTMT_FLAG_FAST_BIT        (0x01u << 0u)    /**< SpO2PR-Fast field bit */
#define CY_BLE_PLXS_CTMT_FLAG_SLOW_BIT        (0x01u << 1u)    /**< SpO2PR-Slow field bit */
#define CY_BLE_PLXS_CTMT_FLAG_MSF_BIT         (0x01u << 2u)    /**< Measurement Status field bit */
#define CY_BLE_PLXS_CTMT_FLAG_DSSF_BIT        (0x01u << 3u)    /**< Device and Sensor Status field bit */
#define CY_BLE_PLXS_CTMT_FLAG_PAIF_BIT        (0x01u << 4u)    /**< Pulse Amplitude Index field bit */

/** "Supported Features"  bits of PLX Features characteristic */
#define CY_BLE_PLXS_FEAT_SUPPORT_MEAS_BIT     (0x01u << 0u)    /**< Measurement Status support bit */
#define CY_BLE_PLXS_FEAT_SUPPORT_DSS_BIT      (0x01u << 1u)    /**< Device and Sensor Status support bit */
#define CY_BLE_PLXS_FEAT_SUPPORT_MSSC_BIT     (0x01u << 2u)    /**< Measurement Storage for Spot-check measurements bit */
#define CY_BLE_PLXS_FEAT_SUPPORT_TMSF_BIT     (0x01u << 3u)    /**< Timestamp for Spot-check measurements bit */
#define CY_BLE_PLXS_FEAT_SUPPORT_FAST_BIT     (0x01u << 4u)    /**< SpO2PR-Fast metric bit */
#define CY_BLE_PLXS_FEAT_SUPPORT_SLOW_BIT     (0x01u << 5u)    /**< SpO2PR-Slow metric bit */
#define CY_BLE_PLXS_FEAT_SUPPORT_PAI_BIT      (0x01u << 6u)    /**< Pulse Amplitude Index field bit */
#define CY_BLE_PLXS_FEAT_SUPPORT_MBS_BIT      (0x01u << 7u)    /**< Multiple Bonds Supported bit */


/** @} */

/***************************************
* Function Prototypes
***************************************/

/** \addtogroup group_ble_service_api_PLXS_server_client
 * @{
 */
cy_en_ble_api_result_t Cy_BLE_PLXS_Init(cy_stc_ble_plxs_config_t *config);
cy_en_ble_api_result_t Cy_BLE_PLXS_RegisterAttrCallback(cy_ble_callback_t callbackFunc);

/** @} */

#ifdef CY_BLE_PLXS_SERVER
/**
 * \addtogroup group_ble_service_api_PLXS_server
 * @{
 */


cy_en_ble_api_result_t Cy_BLE_PLXSS_SetCharacteristicValue(cy_en_ble_plxs_char_index_t charIndex,
                                                           uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_PLXSS_GetCharacteristicValue(cy_en_ble_plxs_char_index_t charIndex,
                                                           uint8_t attrSize, uint8_t *attrValue);


cy_en_ble_api_result_t Cy_BLE_PLXSS_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_plxs_char_index_t charIndex,
                                                                cy_en_ble_plxs_descr_index_t descrIndex,
                                                                uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_PLXSS_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_plxs_char_index_t charIndex,
                                                                cy_en_ble_plxs_descr_index_t descrIndex,
                                                                uint8_t attrSize, uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_PLXSS_SendNotification(cy_stc_ble_conn_handle_t connHandle,
                                                     cy_en_ble_plxs_char_index_t charIndex, uint8_t attrSize,
                                                     uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_PLXSS_SendIndication(cy_stc_ble_conn_handle_t connHandle,
                                                   cy_en_ble_plxs_char_index_t charIndex, uint8_t attrSize,
                                                   uint8_t *attrValue);

/** @} */
#endif /* CY_BLE_PLXS_SERVER */



#ifdef CY_BLE_PLXS_CLIENT
/**
 * \addtogroup group_ble_service_api_PLXS_client
 * @{
 */

cy_en_ble_api_result_t Cy_BLE_PLXSC_SetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_plxs_char_index_t charIndex, uint8_t attrSize,
                                                           uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_PLXSC_GetCharacteristicValue(cy_stc_ble_conn_handle_t connHandle,
                                                           cy_en_ble_plxs_char_index_t charIndex);

cy_en_ble_api_result_t Cy_BLE_PLXSC_SetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_plxs_char_index_t charIndex,
                                                                cy_en_ble_plxs_descr_index_t descrIndex, uint8_t attrSize,
                                                                uint8_t *attrValue);

cy_en_ble_api_result_t Cy_BLE_PLXSC_GetCharacteristicDescriptor(cy_stc_ble_conn_handle_t connHandle,
                                                                cy_en_ble_plxs_char_index_t charIndex,
                                                                cy_en_ble_plxs_descr_index_t descrIndex);

/** @} */
#endif /* CY_BLE_PLXS_CLIENT */

/***************************************
* Macro Functions
***************************************/

#ifdef CY_BLE_PLXS_CLIENT
#define Cy_BLE_PLXSC_GetCharacteristicValueHandle(connHandle, charIndex)               \
    (((charIndex) >= CY_BLE_PLXS_CHAR_COUNT) ? CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE : \
     cy_ble_plxsc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfo[charIndex].valueHandle)

#define Cy_BLE_PLXSC_GetCharacteristicDescriptorHandle(connHandle, charIndex, descrIndex)     \
    ((((charIndex) >= CY_BLE_PLXS_CHAR_COUNT) || ((descrIndex) >= CY_BLE_PLXS_DESCR_COUNT)) ? \
     CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE :                                                  \
     cy_ble_plxsc[Cy_BLE_GetDiscoveryIdx(connHandle)].charInfo[charIndex].descrHandle[descrIndex])
#endif /* (CY_BLE_PLXS_CLIENT) */

#ifdef CY_BLE_PLXS_SERVER
#define CY_BLE_PLXSS_IS_PROCEDURE_IN_PROGRESS    ((cy_ble_plxssFlag & CY_BLE_PLXS_FLAG_PROCESS) != 0u)
#endif /* CY_BLE_PLXS_SERVER */
/***************************************
*  External data references
***************************************/

#ifdef CY_BLE_PLXS_SERVER
extern uint32_t cy_ble_plxssFlag;
#endif /* CY_BLE_PLXS_SERVER */

#ifdef CY_BLE_PLXS_CLIENT
extern uint32_t cy_ble_plxscFlag[CY_BLE_CONFIG_GATTC_COUNT];
extern cy_stc_ble_plxsc_t cy_ble_plxsc[CY_BLE_CONFIG_GATTC_COUNT];
#endif /* CY_BLE_PLXS_CLIENT */

extern cy_stc_ble_plxs_config_t cy_ble_plxsConfig;
extern cy_stc_ble_plxs_config_t *cy_ble_plxsConfigPtr;

#ifdef __cplusplus
}

#endif /* __cplusplus */
#endif /* CY_BLE_MODE_PROFILE && defined(CY_BLE_PLXS) */
#endif /* CY_BLE_PLXS_H */

/* [] END OF FILE */
