/***************************************************************************//**
* \file cy_ble_stack_gatt_db.h
* \version 2.0
*
* \brief
*  This file contains declarations of public BLE APIs of Generic Attribute Profile - GATT DB Access layer.
*  Also specified the defines, constants, and data structures required for the APIs.
* 
* Related Document:
*  BLE Standard Spec - CoreV4.2, CSS, CSAs, ESR05, ESR06
* 
********************************************************************************
* \copyright
* Copyright 2014-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CY_BLE_STACK_GATT_DB_H
#define CY_BLE_STACK_GATT_DB_H


/***************************************
* Common stack includes
***************************************/

#include "cy_ble_stack_gatt.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
** Macro Definition
***************************************/

/** GATT Defined Attributes */
#define CY_BLE_GATT_PRIMARY_SERVICE                      (0x2800u) /**< GATT Primary service */
#define CY_BLE_GATT_SECONDARY_SERVICE                    (0x2801u) /**< GATT secondary service */
#define CY_BLE_GATT_INCLUDE_SERVICE                      (0x2802u) /**< GATT included service */
#define CY_BLE_GATT_CHARACTERISTIC                       (0x2803u) /**< GATT characteristic */
#define CY_BLE_GATT_EXTENDED_PROPERTIES                  (0x2900u) /**< GATT extended properties */
#define CY_BLE_GATT_USER_DESCRIPTION                     (0x2901u) /**< GATT user description */
#define CY_BLE_GATT_CLIENT_CONFIG                        (0x2902u) /**< GATT client configuration */
#define CY_BLE_GATT_SERVER_CONFIG                        (0x2903u) /**< GATT server configuration */
#define CY_BLE_GATT_PRESENTATION_FORMAT                  (0x2904u) /**< GATT presentation format */
#define CY_BLE_GATT_AGGREGATE_FORMAT                     (0x2905u) /**< GATT aggregate format */

#define CY_BLE_GATT_GAP_SERVICE_UUID                     (0x1800u) /**< GATT GAP service UUID */

/** GATT Database Properties Field Description
    1. Attribute read permissions B0: Bluetooth Spec Defined
    2. Attribute write permissions B1: Bluetooth Spec Defined
    3. Characteristic properties B2: Bluetooth Spec Defined
    4. B3: Implementation Specific */

/** Attribute Read Permissions B0: Bluetooth Spec Defined */
/**\note: It is mandatory to set 'CY_BLE_GATT_DB_ATTR_PROP_RD_SEC_ENCRYPT' bit if either
          'CY_BLE_GATT_DB_ATTR_PROP_RD_SEC_AUTHENTICATE' or 'CY_BLE_GATT_DB_ATTR_PROP_RD_SEC_SC_AUTHENTICATE'
          bits are set. Otherwise these bits will be ignored. */
#define CY_BLE_GATT_DB_ATTR_PROP_READ                   (0x00000001u) /**< Attribute Read permission */
#define CY_BLE_GATT_DB_ATTR_PROP_RD_SEC_ENCRYPT         (0x00000002u) /**< Attribute Read with encryption */
#define CY_BLE_GATT_DB_ATTR_PROP_RD_SEC_AUTHENTICATE    (0x00000004u) /**< Attribute Read with authentication */
#define CY_BLE_GATT_DB_ATTR_PROP_RD_SEC_AUTHORIZE       (0x00000008u) /**< Attribute Read with authorization */
#define CY_BLE_GATT_DB_ATTR_PROP_RD_SEC_SC_AUTHENTICATE (0x00000010u) /**< Attribute Read with SC authentication */

#define CY_BLE_GATT_DB_ATTR_PROP_RD_MASK                (0x0000001Fu) /**< Attribute Read property bit mask */
#define CY_BLE_GATT_DB_ATTR_PROP_RD_BIT_SHIFT           (0x0u)        /**< Attribute Read property bit shift */
#define CY_BLE_GATT_DB_ATTR_PROP_RD_SECURITY_MASK       (0x0000001Eu) /**< Attribute Read prop security bit mask */
#define CY_BLE_GATT_DB_ATTR_PROP_RD_SECURITY_BIT_SHIFT  (0x1u)        /**< Attribute Read prop security bit shift */

/** Attribute Write Permissions B1: Bluetooth Spec Defined */
/**\note: It is mandatory to set 'CY_BLE_GATT_DB_ATTR_PROP_WR_SEC_ENCRYPT' bit if either 
          'CY_BLE_GATT_DB_ATTR_PROP_WR_SEC_AUTHENTICATE' or 'CY_BLE_GATT_DB_ATTR_PROP_WR_SEC_SC_AUTHENTICATE'
          bits are set. Otherwise these bits will be ignored. */
#define CY_BLE_GATT_DB_ATTR_PROP_WRITE                  (0x00000100u) /**< Attribute Write permission */
#define CY_BLE_GATT_DB_ATTR_PROP_WR_SEC_ENCRYPT         (0x00000200u) /**< Attribute Write with encryption */
#define CY_BLE_GATT_DB_ATTR_PROP_WR_SEC_AUTHENTICATE    (0x00000400u) /**< Attribute Write with authentication */
#define CY_BLE_GATT_DB_ATTR_PROP_WR_SEC_AUTHORIZE       (0x00000800u) /**< Attribute Write with authorization */
#define CY_BLE_GATT_DB_ATTR_PROP_WR_SEC_SC_AUTHENTICATE (0x00001000u) /**< Attribute Write with SC authentication */

#define CY_BLE_GATT_DB_ATTR_PROP_WR_MASK                (0x00001F00u) /**< Attribute Write property bit mask */
#define CY_BLE_GATT_DB_ATTR_PROP_WR_BIT_SHIFT           (0x8u)        /**< Attribute Write property bit shift */
#define CY_BLE_GATT_DB_ATTR_PROP_WR_SECURITY_MASK       (0x00001E00u) /**< Attribute Write prop security bit mask */
#define CY_BLE_GATT_DB_ATTR_PROP_WR_SECURITY_BIT_SHIFT  (0x9u)        /**< Attribute Write prop security bit shift */

/** Characteristic properties B2: Bluetooth Spec Defined */
#define CY_BLE_GATT_DB_CH_PROP_BROADCAST                 (0x00010000u) /**< characteristic property for broadcast */
#define CY_BLE_GATT_DB_CH_PROP_READ                      (0x00020000u) /**< characteristic property for read */
#define CY_BLE_GATT_DB_CH_PROP_RD_WRITE_WO_RESP          (0x00040000u) /**< characteristic property for wtite without response */
#define CY_BLE_GATT_DB_CH_PROP_WRITE                     (0x00080000u) /**< characteristic property for write */
#define CY_BLE_GATT_DB_CH_PROP_NOTIFY                    (0x00100000u) /**< characteristic property for notify */
#define CY_BLE_GATT_DB_CH_PROP_INDICATE                  (0x00200000u) /**< characteristic property for indication */
#define CY_BLE_GATT_DB_CH_PROP_SEC_AUTH_SIGNED_WRITE     (0x00400000u) /**< characteristic property for signed write */
#define CY_BLE_GATT_DB_CH_PROP_EXT_PROP                  (0x00800000u) /**< characteristic property for extended properties */

#define CY_BLE_GATT_DB_CHAR_PROP_MASK                    (0x00FF0000u) /**< characteristic property bit mask */
#define CY_BLE_GATT_DB_CHAR_PROP_BIT_SHIFT               (0x10u)       /**< characteristic property bit shift */

/** Extended properties values.*/
/**\note: These properties are not
   part of the permission field. The peer GATT layer or local GATT
   layer must read "Characteristic Extended Properties 
   Descriptor" value. */
#define CY_BLE_GATT_DB_CH_PROP_EXT_PROP_RELIABLE_WRITE   (0x0001u) /**< extended property of reliable write */
#define CY_BLE_GATT_DB_CH_PROP_EXT_PROP_WRITABLE_AUX     (0x0002u) /**< extended property of writable aux */
    
/** Implementation permissions B3: Implementation specific
   defines to access runtime characteristics. */

/** Bit Mask */
/** Characteristic read access event bit */
#define CY_BLE_GATT_DB_ATTR_CHAR_VAL_RD_EVENT            (0x01000000u)
/** characteristic CCCD bit */
#define CY_BLE_GATT_DB_CCCD_ATTR                         (0x02000000u)
/** attribute format */
#define CY_BLE_GATT_DB_ATTR_UUID_FMT_32                   (0x04000000u)
/** attribute format */
#define CY_BLE_GATT_DB_ATTR_UUID_FMT_128                    (0x08000000u)

/** Encryption Key Size 7 Needed for the Service */
#define CY_BLE_GATT_DB_SER_ENCRYPT_KEY_SIZE_7            (0x10000000u)

/** Encryption Key Size 8 Needed for the Service */
#define CY_BLE_GATT_DB_SER_ENCRYPT_KEY_SIZE_8            (0x20000000u)

/** Encryption Key Size 9 Needed for the Service */
#define CY_BLE_GATT_DB_SER_ENCRYPT_KEY_SIZE_9            (0x30000000u)

/** Encryption Key Size 10 Needed for the Service */
#define CY_BLE_GATT_DB_SER_ENCRYPT_KEY_SIZE_10           (0x40000000u)

/** Encryption Key Size 11 Needed for the Service */
#define CY_BLE_GATT_DB_SER_ENCRYPT_KEY_SIZE_11           (0x50000000u)

/** Encryption Key Size 12 Needed for the Service */
#define CY_BLE_GATT_DB_SER_ENCRYPT_KEY_SIZE_12           (0x60000000u)

/** Encryption Key Size 13 Needed for the Service */
#define CY_BLE_GATT_DB_SER_ENCRYPT_KEY_SIZE_13           (0x70000000u)

/** Encryption Key Size 14 Needed for the Service */
#define CY_BLE_GATT_DB_SER_ENCRYPT_KEY_SIZE_14           (0x80000000u)

/** Encryption Key Size 15 Needed for the Service */
#define CY_BLE_GATT_DB_SER_ENCRYPT_KEY_SIZE_15           (0x90000000u)

/** Encryption Key Size 16 Needed for the Service */
#define CY_BLE_GATT_DB_SER_ENCRYPT_KEY_SIZE_16           (0xA0000000u)

/** No Encryption Needed for the Service */
#define CY_BLE_GATT_DB_SER_NO_ENCRYPT_PROPERTY           (0x00000000u)

/**  Encryption Key Size Mask Needed for the Service */
#define CY_BLE_GATT_DB_ENC_KEY_SIZE_MASK                 (0xF0000000u)

/** By default entries in CY_BLE_GATT DB is 16-bit UUID
   This mask is defined to decide UUID length */
#define CY_BLE_GATT_DB_ATTR_UUID_FMT_16_MASK             (0x03000000u)


/** CY_BLE_GATT Client Configuration values */
/** Client configuration notification */
#define CY_BLE_GATT_CLI_CNFG_NOTIFICATION                (0x0001u)
/** Client configuration indication */
#define CY_BLE_GATT_CLI_CNFG_INDICATION                  (0x0002u)
/** default configuration */
#define CY_BLE_GATT_CLI_CNFG_DEFAULT                     (0x0000u)

/** Unit size is with respect to word (16 bit) */
#define CY_BLE_GATT_DB_128_BIT_UUID_SZ                   (0x10u) /**< 128-bit uuid size */
#define CY_BLE_GATT_DB_32_BIT_UUID_SZ                    (0x04u) /**< 32-bit uuid size */
#define CY_BLE_GATT_DB_16_BIT_UUID_SZ                    (0x02u) /**< 16-bit uuid size */

#define CY_BLE_GATT_DB_ATTR_16_BIT_UUID                  (0x00u) /**< 128-bit uuid */
#define CY_BLE_GATT_DB_ATTR_32_BIT_UUID                  (0x01u) /**< 32-bit uuid */
#define CY_BLE_GATT_DB_ATTR_128_BIT_UUID                 (0x02u) /**< 16-bit uuid */

/** GATT DB attribute property Size */
#define CY_BLE_GATT_DB_ATTR_PRPTY_SZ                     (0x01u)

/** GATT DB attribute Handle Size */
#define CY_BLE_GATT_DB_ATTR_HANDLE_SZ                    (0x02u)

/** Locally initiated operation, needed for local updates based on Attribute
    Handles. */
#define CY_BLE_GATT_DB_LOCALLY_INITIATED                 (0x00u)

/** Peer Initiated GATT DB transaction */
#define CY_BLE_GATT_DB_PEER_INITIATED                    (0x40u)

/** Read operation for Attribute  */
#define CY_BLE_GATT_DB_READ                              (0x01u)

/** Write operation for Attribute */
#define CY_BLE_GATT_DB_WRITE                             (0x02u)

/** Prepare Write operation for Attribute */
#define CY_BLE_GATT_DB_PREP_WRITE                        (0x04u)

/** Write without response operation for Attribute */
#define CY_BLE_GATT_DB_WRITE_WITHOUT_RSP                 (0x80u)

/***************************************
** Data Struct Definition
***************************************/
/**
 \addtogroup group_ble_common_api_gatt_definitions
 @{
*/

/** Attribute value type used in GATT database */
typedef struct
{
    /** Length in number of bytes for attGenericVal */
    uint16_t actualLength;

    /** Buffer to the store generic characteristic value based on
       length or complete UUID value if the attribute is of type 128-bit
       UUID and 32-bit UUID type. */
    void * attGenericVal;

}cy_stc_ble_gatts_att_gen_val_len_t;

/** Attribute value type used in the GATT database */
typedef struct
{
    /** Length in number of bytes for attGenericVal */
    uint16_t maxAttrLength;

    /** Buffer to the store generic characteristic value based on
       length or complete UUID value if the attribute is of type 128-bit
       UUID and 32-bit UUID type. */
    cy_stc_ble_gatts_att_gen_val_len_t * attGenericValLen;

}cy_stc_ble_gatts_att_pack_val_len_t;

/** Attribute value type used in GATT database */
typedef union
{    
   /** Buffer containing 32-bit or 128-bit UUID values for Service and
      Characteristic declaration.
      Attribute format structure: if entry is for characteristic value format, 
      then it has the "attribute format value" of pointer type to represent generic
      structure to cater wide formats of available list of characteristic formats. */
    cy_stc_ble_gatts_att_pack_val_len_t      attFormatValue;

    /** Attribute UUID value */
    uint16_t                                 attValueUuid;

} cy_stc_ble_gatts_att_value_t;

/** GATT database structure used in the GATT Server */
typedef struct
{
    /** Start Handle: Act as an index for querying BLE GATT database */
    uint16_t                 attHandle;
    
    /** UUID: 16-bit UUID type for an attribute entry. For 32-bit and
       128-bit UUIDs, the last 16 bits should be stored in this entry
       that GATT DB access layer shall retrieve complete 128-bit UUID from the
       cy_ble_gatts_att_generic_val_t structure. */
    uint16_t                 attType;
    
    /** The permission bits are clubbed in a 32-bit field. These 
        32-bits can be grouped in four bytes. The lowest significant byte
        is byte 0 (B0) and the most significant byte is byte 3 (B3). The 
        bytes where the permissions have been grouped is as given below.
         * Attribute permissions for read (B0)
         * Attribute permissions for write (B1)
         * Characteristic properties (B2)
         * Implementation specific permission (B3)
         If permission is set to CY_BLE_GATT_DB_ATTR_CHAR_VAL_RD_EVENT, all GATT 
         characteristic read requests will be indicated to the application using
         the CY_BLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ event.
     */
    uint32_t                 permission;
    
    /** Attribute end handle, indicating logical boundary of given attribute. */
    uint16_t                 attEndHandle;
    
    /** Attribute value format, it can be one of following:
        * uint16_t 16bit - UUID for 16-bit service & characteristic declaration
        * cy_ble_gatts_att_generic_val_t attFormatValue - Buffer containing 32-bit
            or 128-bit UUID values for service & characteristic declaration
        * cy_ble_gatts_att_generic_val_t attFormatValue - Buffer containing generic 
            char definition value, or generic descriptor values
     */
    cy_stc_ble_gatts_att_value_t     attValue;
    
 } cy_stc_ble_gatts_db_t; 

/** GATT database information parameters*/
typedef struct
{   
    /** Pointer to GATT DB */
    const cy_stc_ble_gatts_db_t    * gattDbPtr;

    /** Total number of entries in GATT DB */
    uint16_t                    gattDbTotalEntries;

    /**Maximum characteristic value length */
    uint16_t                    gattDbMaxValue;

} cy_stc_ble_gatts_db_info_t;

/** GATT database attribute value parameters*/
typedef struct
{   
    /** handle value pair of type cy_stc_ble_gatt_handle_value_pair_t.
             'handleValuePair.attrHandle' is an input for which a value must be read/written.
             'handleValuePair.value.len' is an input parameter for the length to be read/written.
             'handleValuePair.value.val' is an input parameter for the data buffer when writing and 
              output parameter when reading.
             'handleValuePair.actualLen' must be ignored as it is unused */
    cy_stc_ble_gatt_handle_value_pair_t  handleValuePair;

    /** Offset at which the data (length in number of bytes) is written */
    uint16_t                          offset;

    /** Connection Handle */
    cy_stc_ble_conn_handle_t             connHandle;

    /** Attribute permissions. Allowed values are,
             - CY_BLE_GATT_DB_LOCALLY_INITIATED
             - CY_BLE_GATT_DB_PEER_INITIATED */
    uint8_t                           flags;

} cy_stc_ble_gatts_db_attr_val_info_t;

/** GATT database attribute enable parameters*/
typedef struct
{   
    /** Attribute handle */
    cy_ble_gatt_db_attr_handle_t    attrHandle;

} cy_stc_ble_gatts_db_attr_enable_info_t;

/** GATT database attribute disable parameters*/
typedef cy_stc_ble_gatts_db_attr_enable_info_t      cy_stc_ble_gatts_db_attr_disable_info_t;

/** GATT database authorize parameters*/
typedef struct
{   
    /** Setting to '0' turns off authorization on the entire GATT database and all attributes marked 
            as authorize will return an authorization error. 
         Setting this to any non-zero value will authorize the entire GATT database and all attributes 
         marked as authorize can be read / written based on other allowed permissions. */
    uint8_t    authorize;

} cy_stc_ble_gatts_db_authorize_info_t;

/***************************************
** Characteristic Descriptors definitions
***************************************/
/** All descriptors follow the characteristic value definition entries that belong
   to a logical group of characteristics shall be in following order:
    1. Characteristic Declaration 
    2. Characteristic Value
    3. Characteristic Extended Property (If any)
    4. Characteristic User Description  (If any)
    5. Client Characteristic Configuration  (If any)
    6. Server Characteristic Configuration (If any)
    7. Characteristic Presentation Format (If any)
    8. Characteristic Aggregate Format (If any)
 */

/** Characteristic Extended Property */
typedef cy_stc_ble_gatts_att_value_t      cy_stc_ble_char_ext_prprty_t;
/** Characteristic User Description */
typedef cy_stc_ble_gatts_att_value_t      cy_stc_ble_char_user_description_t;
/** Client Characteristic Configuration */
typedef cy_stc_ble_gatts_att_value_t      cy_stc_ble_client_char_config_t;
/** Server Characteristic Configuration */
typedef cy_stc_ble_gatts_att_value_t      cy_stc_ble_server_char_config_t;
/** Characteristic Presentation Format */
typedef cy_stc_ble_gatts_att_value_t      cy_stc_ble_char_present_fmt_t;
/** Characteristic Aggregate Format */
typedef cy_stc_ble_gatts_att_value_t      cy_stc_ble_char_aggregate_fmt_t;
/** @} */

/***************************************
** Exported APIs
***************************************/

/**
 \addtogroup group_ble_common_api_gatt_server_functions
 @{
*/

/******************************************************************************
* Function Name: Cy_BLE_GATTS_DbRegister
***************************************************************************//**
* 
*  This function registers the GATT database for the GATT Server. The GATT 
*  database stores all the attributes used by the GATT server, along with their
*  permissions. This is a blocking function. No event is generated on calling
*  this function.
* 
*  \param param: Parameter is of type 'cy_stc_ble_gatts_db_info_t'.
* 
* \return
*  cy_en_ble_api_result_t: Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                      | Description
*  ------------                      | -----------
*   CY_BLE_SUCCESS                   | On successful operation.
*   CY_BLE_ERROR_INVALID_PARAMETER   | If the Database has zero entries or is a NULL pointer.
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATTS_DbRegister
(
    cy_stc_ble_gatts_db_info_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_GATTS_WriteAttributeValue
***************************************************************************//**
* 
*  This function is used to write to the value field of the specified attribute
*  in the GATT database of a GATT Server. This is a blocking function. No event
*  is generated on calling this function.
* 
*  If a peer device connected to the GATT Server initiates a write operation, 
*  this function is executed on the GATT Server. During such a call, the function
*  checks for the attribute permissions (flags) before executing the write 
*  operation.
* 
*  \param param: Parameter is of type 'cy_stc_ble_gatts_db_attr_val_info_t'.
* 
* \return
*  cy_en_ble_gatt_err_code_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                                | Description
*  ------------                                | -----------
*  CY_BLE_GATT_ERR_NONE                        | On successful operation.
*  CY_BLE_GATT_ERR_INVALID_HANDLE              | 'handleValuePair.attrHandle' is not valid.
*  CY_BLE_GATT_ERR_WRITE_NOT_PERMITTED         | write operation is not permitted on this attribute (Peer Initiated).
*  CY_BLE_GATT_ERR_UNLIKELY_ERROR              | Invalid arguments passed.
*  CY_BLE_GATT_ERR_INSUFFICIENT_ENCRYPTION     | Link is not encrypted (Peer Initiated).
*  CY_BLE_GATT_ERR_INSUFFICIENT_ENC_KEY_SIZE   | Link is encrypted with insufficient key size (Peer Initiated).
*  CY_BLE_GATT_ERR_INSUFFICIENT_AUTHENTICATION | Link is un-authenticated (Peer Initiated).
*  CY_BLE_GATT_ERR_INSUFFICIENT_AUTHORIZATION  | Peer client is not authorized (Peer Initiated).
*  CY_BLE_GATT_ERR_INVALID_OFFSET              | param->offset is invalid.
*  CY_BLE_GATT_ERR_INVALID_ATTRIBUTE_LEN       | handleValuePair.value.len is invalid.
* 
******************************************************************************/
cy_en_ble_gatt_err_code_t Cy_BLE_GATTS_WriteAttributeValue
(
    cy_stc_ble_gatts_db_attr_val_info_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_GATTS_ReadAttributeValue
***************************************************************************//**
* 
*  This function is used to read the value field of the specified attribute from
*  the GATT database in a GATT Server. This is a blocking function. No event is
*  generated on calling this function.
* 
*  A peer initiated call to this function results in the function checking for
*  attribute permissions before performing this operation.
*     
*  \param param: Parameter is of type 'cy_stc_ble_gatts_db_attr_val_info_t'.
*  param-> offset: not used, to be ignored
* 
* \return
*  cy_en_ble_gatt_err_code_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                                | Description
*  ------------                                | -----------
*  CY_BLE_GATT_ERR_NONE                        | On successful operation.
*  CY_BLE_GATT_ERR_INVALID_HANDLE              | 'handleValuePair.attrHandle' is not valid.
*  CY_BLE_GATT_ERR_READ_NOT_PERMITTED          | Read operation is not permitted on this attribute (Peer Initiated).
*  CY_BLE_GATT_ERR_UNLIKELY_ERROR              | Invalid arguments passed.
*  CY_BLE_GATT_ERR_INSUFFICIENT_ENCRYPTION     | Link is not encrypted (Peer Initiated).
*  CY_BLE_GATT_ERR_INSUFFICIENT_ENC_KEY_SIZE   | Link is encrypted with insufficient key size (Peer Initiated).
*  CY_BLE_GATT_ERR_INSUFFICIENT_AUTHENTICATION | Link is un-authenticated (Peer Initiated).
*  CY_BLE_GATT_ERR_INSUFFICIENT_AUTHORIZATION  | Peer client is not authorized (Peer Initiated).
* 
******************************************************************************/
cy_en_ble_gatt_err_code_t Cy_BLE_GATTS_ReadAttributeValue
(
    cy_stc_ble_gatts_db_attr_val_info_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_GATTS_EnableAttribute
***************************************************************************//**
* 
*  This function enables the attribute entry for a service or characteristic
*  logical group in the GATT database registered in the BLE Stack. This is a
*  blocking function. No event is generated on calling this function.
* 
*  This function returns an error if the attribute does not belong to any 
*  service or characteristic logical group. If the attribute entry is already
*  enabled, then this function returns status CY_BLE_GATT_ERR_NONE.
* 
*  \param param: Parameter is of type 'cy_stc_ble_gatts_db_attr_enable_info_t'.
*              param->attrHandle: Attribute handle of the registered GATT Database to enable
*              particular attribute entry, of type 'cy_ble_gatt_db_attr_handle_t'.
* 
* \return
*  cy_en_ble_gatt_err_code_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                       | Description
*  ------------                       | -----------
*   CY_BLE_GATT_ERR_NONE              | On successful operation.
*   CY_BLE_GATT_ERR_INVALID_HANDLE    | 'attrHandle' is not valid.
*  CY_BLE_GATT_ERR_UNLIKELY_ERROR     | NULL pointer as input parameter.
* 
******************************************************************************/
cy_en_ble_gatt_err_code_t Cy_BLE_GATTS_EnableAttribute
(
    cy_stc_ble_gatts_db_attr_enable_info_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_GATTS_DisableAttribute
***************************************************************************//**
* 
*  This function disables the attribute entry for service or characteristic
*  logical group in the GATT database registered in the BLE Stack. This is
*  a blocking function. No event is generated on calling this function.
* 
*  This function returns error if the attribute does not belong to a service or
*  a characteristic logical group. If attribute entry is already disabled then
*  it returns CY_BLE_GATT_ERR_NONE as the status. All the attribute entries are
*  enabled in the GATT database during stack initialization.
* 
*  \param param: Parameter is of type 'cy_stc_ble_gatts_db_attr_disable_info_t'.
*              param->attrHandle: Attribute handle of the registered GATT Database to disable
*               particular attribute entry, of type 'cy_ble_gatt_db_attr_handle_t'
* 
* \return
*  cy_en_ble_gatt_err_code_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                       | Description
*  ------------                       | -----------
*   CY_BLE_GATT_ERR_NONE              | On successful operation.
*   CY_BLE_GATT_ERR_INVALID_HANDLE    | 'attrHandle' is not valid.
*  CY_BLE_GATT_ERR_UNLIKELY_ERROR     | NULL pointer as input parameter.
* 
******************************************************************************/
cy_en_ble_gatt_err_code_t Cy_BLE_GATTS_DisableAttribute
(
    cy_stc_ble_gatts_db_attr_disable_info_t  * param
);

/******************************************************************************
* Function Name: Cy_BLE_GATTS_DbAuthorize
***************************************************************************//**
* 
*  This Function sets or clears authorization permission for the GATT database
* 
*  \param param: Parameter is of type 'cy_stc_ble_gatts_db_authorize_info_t'.
* 
* \return
*  cy_en_ble_gatt_err_code_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*  Errors codes                       | Description
*  ------------                       | -----------
*  CY_BLE_GATT_ERR_NONE               | On successful operation.
*  CY_BLE_GATT_ERR_UNLIKELY_ERROR     | If 'param' is NULL.
* 
******************************************************************************/
cy_en_ble_gatt_err_code_t Cy_BLE_GATTS_DbAuthorize
(
    cy_stc_ble_gatts_db_authorize_info_t  * param
);
        
/** @} */

/** \cond IGNORE */
/******************************************************************************
* Function Name: Cy_BLE_GATT_DbCheckPermission
***************************************************************************//**
* 
*  This API validates the security permission for the given attribute handle
* 
*  \param attrhandle: Attribute handle of type 'cy_ble_gatt_db_attr_handle_t'
*  \param connHandle: Connection handle of type 'cy_stc_ble_conn_handle_t'
*  \param flags: flag to identify if request is peer initiated (CY_BLE_GATT_DB_PEER_INITIATED) 
*                       or local initiated (CY_BLE_GATT_DB_LOCALLY_INITIATED).
* 
* \return
*  cy_en_ble_gatt_err_code_t : Return value indicates whether the function succeeded or
*  failed with GATT error codes. For details, refer to Bluetooth 4.2 spec, vol3, part F, Table 3.3 
* 
******************************************************************************/
cy_en_ble_gatt_err_code_t Cy_BLE_GATT_DbCheckPermission
(
   cy_ble_gatt_db_attr_handle_t  attrhandle,
   cy_stc_ble_conn_handle_t          * connHandle,
   uint8_t                        flags
);

/** \endcond */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*CY_BLE_STACK_GATT_DB_H*/


/*EOF*/
