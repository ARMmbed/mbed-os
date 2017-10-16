/*******************************************************************************
* \file cy_ble_stack_gatt.h
* \version 2.0
*
* \brief
*  This file contains declarations of generic public BLE APIs of Generic Attribute Profile.
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
#ifndef CY_BLE_STACK_GATT_H_
#define CY_BLE_STACK_GATT_H_


/***************************************
* Common stack includes
***************************************/

#include "cy_ble_stack_host_main.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
** GATT Constants
***************************************/

/** ATT 16-bit UUID Size */
#define CY_BLE_GATT_16_BIT_UUID_SIZE            0x02u

/** GATT 128-bit UUID Size */
#define CY_BLE_GATT_128_BIT_UUID_SIZE           0x10u

/** GATT Identification for 16-bit UUID Format */
#define CY_BLE_GATT_16_BIT_UUID_FORMAT          0x01u

/** GATT Identification for 128-bit UUID Format */
#define CY_BLE_GATT_128_BIT_UUID_FORMAT         0x02u

/** GATT MTU Size */
#define CY_BLE_GATT_DEFAULT_MTU                 0x17u /**< Default MTU size */
#define CY_BLE_GATT_MAX_MTU                     0x200u /**< MAX MTU size */

/** ATT Execute Write Cancel Flag */
#define CY_BLE_GATT_EXECUTE_WRITE_CANCEL_FLAG   0x00u

/** ATT Execute Write Execute Flag */
#define CY_BLE_GATT_EXECUTE_WRITE_EXEC_FLAG     0x01u

/** ATT Invalid Attribute Handle Value */
#define CY_BLE_GATT_INVALID_ATTR_HANDLE_VALUE   0x0000u

/** ATT Attribute Handle Start Range */
#define CY_BLE_GATT_ATTR_HANDLE_START_RANGE     0x0001u

/** ATT Attribute Handle End Range */
#define CY_BLE_GATT_ATTR_HANDLE_END_RANGE       0xFFFFu

/**
 \addtogroup group_ble_common_api_gatt_definitions
 @{
*/

/***************************************
** Enumerated Types
***************************************/

/** Opcode that has resulted in the error */
typedef enum
{
    /** Error Response PDU */
    CY_BLE_GATT_ERROR_RSP = 0x01u,

    /** Exchange GATT MTU Request PDU */
    CY_BLE_GATT_XCNHG_MTU_REQ,

    /** Exchange GATT MTU Response PDU */
    CY_BLE_GATT_XCHNG_MTU_RSP,

    /** Find Information Request PDU */
    CY_BLE_GATT_FIND_INFO_REQ,

    /** Find Information Response PDU */
    CY_BLE_GATT_FIND_INFO_RSP,

    /** Find By Type Value Request PDU */
    CY_BLE_GATT_FIND_BY_TYPE_VALUE_REQ,

    /** Find By Type Value Response PDU */
    CY_BLE_GATT_FIND_BY_TYPE_VALUE_RSP,

    /** Read By Type Request PDU */
    CY_BLE_GATT_READ_BY_TYPE_REQ,

    /** Read By Type Response PDU */
    CY_BLE_GATT_READ_BY_TYPE_RSP,

    /** Read Request PDU */
    CY_BLE_GATT_READ_REQ,

    /** Read Response PDU */
    CY_BLE_GATT_READ_RSP,

    /** Read Blob Request PDU */
    CY_BLE_GATT_READ_BLOB_REQ,

    /** Read Blob Response PDU */
    CY_BLE_GATT_READ_BLOB_RSP,

    /** Read Multiple Request PDU */
    CY_BLE_GATT_READ_MULTIPLE_REQ,

    /** Read Multiple Response PDU */
    CY_BLE_GATT_READ_MULTIPLE_RSP,

    /** Read Group Type Request PDU */
    CY_BLE_GATT_READ_BY_GROUP_REQ,

    /** Read Group Type Response PDU */
    CY_BLE_GATT_READ_BY_GROUP_RSP,

    /** Write Request PDU */
    CY_BLE_GATT_WRITE_REQ,

    /** Write Response PDU */
    CY_BLE_GATT_WRITE_RSP,

    /** Write Command PDU */
    CY_BLE_GATT_WRITE_CMD = 0x52u,

    /** Prepare Write Request PDU */
    CY_BLE_GATT_PREPARE_WRITE_REQ = 0x16u,

    /** Prepare Write Response PDU */
    CY_BLE_GATT_PREPARE_WRITE_RSP,

    /** Execute Write Request PDU */
    CY_BLE_GATT_EXECUTE_WRITE_REQ,

    /** Execute Write Response PDU */
    CY_BLE_GATT_EXECUTE_WRITE_RSP,

    /** Handle Value Notification PDU */
    CY_BLE_GATT_HANDLE_VALUE_NTF = 0x1Bu,

    /** Handle Value Indication PDU */
    CY_BLE_GATT_HANDLE_VALUE_IND = 0x1Du,

    /** Handle Value Confirmation PDU */
    CY_BLE_GATT_HANDLE_VALUE_CNF = 0x1Eu,

    /**Signed Write Command PDU */
    CY_BLE_GATT_SIGNED_WRITE_CMD = 0xD2u,
    
    /** Unknown or Unhandled PDU */
    CY_BLE_GATT_UNKNOWN_PDU_IND = 0xFFu

}cy_en_ble_gatt_pdu_t;

/** GATT error codes */
typedef enum
{
    /** No Error    */
    CY_BLE_GATT_ERR_NONE,

    /** Invalid Handle error code is used when the ATT handle in the
       ATT request PDU is invalid. */
    CY_BLE_GATT_ERR_INVALID_HANDLE = 0x0001u,

    /** Read Not Permitted error code is used when the permission to
       read the value of an ATT handle is not permitted on the ATT server. */
    CY_BLE_GATT_ERR_READ_NOT_PERMITTED,

    /** Write Not Permitted error code is used when the permission to
       write the value of an ATT handle is not permitted on the ATT server. */
    CY_BLE_GATT_ERR_WRITE_NOT_PERMITTED,

    /** Invalid PDU error code is used when the format of the PDU sent
       from the ATT Client is incorrect. */
    CY_BLE_GATT_ERR_INVALID_PDU,

    /** Insufficient Authentication error code is used when
       an access to a handle is attempted on a un-authenticated link but the
       attribute requires that the link be authenticated before a client can access it. */
    CY_BLE_GATT_ERR_INSUFFICIENT_AUTHENTICATION,

    /** Request not supported error code is used when
       the server does not support the processing of an ATT request sent from the client. */
    CY_BLE_GATT_ERR_REQUEST_NOT_SUPPORTED,

    /** Invalid Offset error code is used when
       the offset sent by the client in the Read blob/Prepare Write Request is
       invalid with respect to the length of the value in the server. */
    CY_BLE_GATT_ERR_INVALID_OFFSET,

    /** Insufficient Authorization error code is used when
       the ATT server does not authorize the client and hence prohibits the
       client from reading the handle value. */
    CY_BLE_GATT_ERR_INSUFFICIENT_AUTHORIZATION,

    /** Write queue full error code is used when there is no more space left in the
       prepare write queue on the server to entertain any more prepare writes from a
       client. */
    CY_BLE_GATT_ERR_PREPARE_WRITE_QUEUE_FULL,

    /** Attribute not found error is used when the ATT server cannot find any handle
       that belongs to the Attribute type in the given range of handles that the
       client specified in its request. This error code can be sent to the client in
       response to the following request PDUs - Find Information, Find by Type Value,
       Read by Type, and Read by Group Type requests. */
    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_FOUND,

    /** Attribute Not Long error code is used when the client tries to read or write
       an attribute handle's value that cannot be read or written through Read Blob
       or multiple prepare write requests. */
    CY_BLE_GATT_ERR_ATTRIBUTE_NOT_LONG,

    /** Insufficient encryption key size error code is used when the client tries to
       access an Attribute Handle's Value for which the link must be encrypted
       with a key of certain minimum key size and the current link is encrypted
       with a key of smaller size than the minimum required. */
    CY_BLE_GATT_ERR_INSUFFICIENT_ENC_KEY_SIZE,

    /**
      Invalid Attribute length error code is used when the attribute value's
      length is not correct to process the request containing the value.
    */
    CY_BLE_GATT_ERR_INVALID_ATTRIBUTE_LEN,

    /**
      Unlikely error is used when the processing of the attribute request has
      encountered an error that is not covered by any other error code.
    */
    CY_BLE_GATT_ERR_UNLIKELY_ERROR,

    /** Insufficient encryption error code is used when the client tries to read or
       write an attribute handle that requires the link to be encrypted and the
       link is currently not encrypted. */
    CY_BLE_GATT_ERR_INSUFFICIENT_ENCRYPTION,

    /** Unsupported Group Type error code is used when the attribute type requested
       in the Read by Group Type request is not a valid grouping attribute on the
       server. */
    CY_BLE_GATT_ERR_UNSUPPORTED_GROUP_TYPE,

    /** Insufficient Resources error code is used when the ATT server does not have
      enough resources such as memory, etc., to process the request from the client. */
    CY_BLE_GATT_ERR_INSUFFICIENT_RESOURCE = 0x11u,
    
    /** Other Error Groups for ATT - GATT
     * Reserved: GATT-ATT Error codes 0x12 to 0x7F are reserved for 
     * Application Specific Error Code Range: 0x80 to 0x9F
     * Reserved: 0xA0 to 0xDF 
     * Common Profile & Service Error Code : 0xE0 to 0xFF 
     */

    /** Heart Rate Control Point Not Supported error code is used when a unsupported
       code is written into Heart Rate service Control Point characteristic. */
    CY_BLE_GATT_ERR_HEART_RATE_CONTROL_POINT_NOT_SUPPORTED = 0x80u,
    
    /** The user data access is not permitted (i.e., the user has not given
       consent to access the data). */
    CY_BLE_GATT_ERR_USER_DATA_ACCESS_NOT_PERMITTED = 0x80u,
    
    /** The notifications of the Cycling Power Vector characteristic cannot be sent 
       due to inappropriate connection parameters. */
    CY_BLE_GATT_ERR_CPS_INAPPROPRIATE_CONNECTION_PARAMETERS = 0x80u,

    /** The value is considered invalid and outside of the range allowed by 
       the characteristic. */
    CY_BLE_GATT_ERR_HTS_OUT_OF_RANGE = 0x80u,

    /** Procedure Already in Progress error code is used when a profile or service
       request cannot be serviced because an operation that has been previously
       triggered is still in progress. */
    CY_BLE_GATTS_ERR_PROCEDURE_ALREADY_IN_PROGRESS = 0x80u,
    
    /** The Op Code Not Supported error code is used when an unsupported
       Op Code is written into the Control Point characteristic. */
    CY_BLE_GATT_ERR_OP_CODE_NOT_SUPPORTED = 0x80u,
    
    /** The Missing CRC error code is used when the CRC is missed in the 
       incoming characteristic value. */
    CY_BLE_GATT_ERR_MISSING_CRC = 0x80u,
    
    /** Client Characteristic Configuration Descriptor Improperly Configured error
       code is used when a Client Characteristic Configuration descriptor is not
       configured according to the requirements of the profile or service. */
    CY_BLE_GATTS_ERR_CCCD_IMPROPERLY_CONFIGURED = 0x81u,
    
    /** The Operation Failed error code is used when the device is unable to 
       complete a procedure for any reason. */
    CY_BLE_GATTS_ERR_OPERATION_FAILED = 0x81u,
    
    /** The Invalid CRC error code is used when the CRC is invalid in the 
       incoming characteristic value. */
    CY_BLE_GATT_ERR_INVALID_CRC = 0x81u,
    
    /** An HTTP Control Point request cannot be serviced because content of the URI,
        the HTTP Headers, or the HTTP Entity Body characteristics are not set
        correctly. */
    CY_BLE_GATTS_ERR_HPS_INVALID_REQUEST = 0x81u,
    
    /** Network connection not available. */
    CY_BLE_GATTS_ERR_NETWORK_NOT_AVAILABLE = 0x82u,
    
    /** Command Not Supported used by the Alert Notification Server when the Client
       sends an incorrect value of the Command ID or Category ID of to the Alert
       Notification Control Point Characteristic. */
    CY_BLE_GATT_ERR_ANS_COMMAND_NOT_SUPPORTED = 0xA0u,
    
    /** Unknown command error code used by the Apple Notification Center Server when the Client
       sends unknown command value of the Apple Notification Center Service Control Point 
       Characteristic. */
    CY_BLE_GATT_ERR_ANCS_UNKNOWN_COMMAND = 0xA0u,
    
    /** Invalid command error code used by the Apple Notification Center Server when the Client
       sends an invalid command value of the Apple Notification Center Service Control Point 
       Characteristic. */
    CY_BLE_GATT_ERR_ANCS_INVALID_COMMAND = 0xA1u,
    
    /** Invalid parameter error code used by the Apple Notification Center Server when the Client
       sends an invalid parameter value of the Apple Notification Center Service Control Point 
       Characteristic. */
    CY_BLE_GATT_ERR_ANCS_INVALID_PARAMETER = 0xA2u,
    
    /** Action failed error code used by the Apple Notification Center Server when some 
       Apple Notification Center Service Control Point Characteristic command processing goes wrong. */
    CY_BLE_GATT_ERR_ANCS_ACTION_FAILED = 0xA3u,

    /** Client Characteristic Configuration Descriptor Improperly Configured error
       code is used when a Client Characteristic Configuration descriptor is not
       configured according to the requirements of the profile or service. */
    CY_BLE_GATT_ERR_CCCD_IMPROPERLY_CONFIGURED = 0xFDu,

    /** The Procedure Already in Progress error code is used when a profile or service
       request cannot be serviced because an operation that has been previously
       triggered is still in progress. */
    CY_BLE_GATT_ERR_PROCEDURE_ALREADY_IN_PROGRESS = 0xFEu,
    
    /** Out of Range error code is used when an attribute value is out of range as
       defined by a profile or service specification. */
    CY_BLE_GATT_ERR_OUT_OF_RANGE = 0xFFu

}cy_en_ble_gatt_err_code_t;


/***************************************
** Exported data structures
***************************************/

/** GATT 16 Bit UUID */
typedef uint16_t         cy_ble_uuid16_t;

/** GATT 128 Bit UUID type */
typedef struct
{
    /** 128 Bit UUID */
    uint8_t   value[CY_BLE_GATT_128_BIT_UUID_SIZE];
}cy_stc_ble_uuid128_t;


/** GATT UUID type*/
typedef union
{
    /** 16 Bit UUID */
    cy_ble_uuid16_t     uuid16;

    /** 128 Bit UUID */
    cy_stc_ble_uuid128_t    uuid128;

}cy_ble_uuid_t;

/** GATT BD Attribute Handle Type */
typedef uint16_t        cy_ble_gatt_db_attr_handle_t;

/** Abstracts Variable Length Values for GATT.

    Apart from data and length, actual length is needed so that the GATT can
    indicate to the application the actual length of data processed for a PDU.
  
    This is used in multiple commands - see CY_BLE_GATT_READ_RSP,
    CY_BLE_GATT_FIND_BY_TYPE_VALUE_REQ, CY_BLE_GATT_READ_BLOB_RSP, etc.
  
    In GATT Read Response for example, if the attribute length is 30 octets
    and the GATT MTU is 23 octets, then only first 22 octets can be sent by the GATT.
    Therefore the actual length will be 22 (GATT MTU-1).
    However, if the GATT MTU is configured to be 54 for example, all 30 octets
    can be transmitted and the actual length will be 30.
    
    Actual length should be derived as -
    actualLen = length>(GATT MTU-1) ? (GATT MTU-1):len
  
    In case multiple values are being packed, the actual length processed will
    depend on the available GATT MTU. */
typedef struct
{
    /** Pointer to the value to be packed */
    uint8_t    * val;

    /** Length of the Value to be packed */
    uint16_t   len;

    /** Out Parameter Indicating Actual Length Packed and sent over the air. Actual length
       can be less than or equal to the 'len' parameter value. This provides the 
       actual length of data that is transmitted over the 
       air information. Each GATT procedure defines different lengths of data that can be transmitted 
       over the air. If the application sends more than that, all data may not transmit over the air.*/
    uint16_t   actualLen;

}cy_stc_ble_gatt_value_t;

/** GATT handle - value pair type */
typedef struct
{
    /** Attribute Value    */
    cy_stc_ble_gatt_value_t              value;

    /** Attribute Handle of GATT DB */
    cy_ble_gatt_db_attr_handle_t         attrHandle;

}cy_stc_ble_gatt_handle_value_pair_t;

/** GATT Attribute Handle Range type */
typedef struct
{
    /** Start Handle */
    cy_ble_gatt_db_attr_handle_t    startHandle;

    /** End Handle */
    cy_ble_gatt_db_attr_handle_t    endHandle;

}cy_stc_ble_gatt_attr_handle_range_t;

/** GATT MTU exchange parameter type */
typedef struct
{
    /** Connection handle */
    cy_stc_ble_conn_handle_t     connHandle;
    
    /** Client/Server Rx/Tx GATT MTU Size */
    uint16_t                    mtu;

}cy_stc_ble_gatt_xchg_mtu_param_t;

/** GATT Handle Value Pair along with offset type */
typedef struct
{
    /** Attribute Handle & Value to be Written */
    cy_stc_ble_gatt_handle_value_pair_t     handleValuePair;

    /** Offset at which Write is to be performed */
    uint16_t                                offset;

}cy_stc_ble_gatt_handle_value_offset_param_t;

/** Write parameter */
typedef struct
{
    /** handle value pair */
    cy_stc_ble_gatt_handle_value_pair_t         handleValPair;

    /** Connection handle */
    cy_stc_ble_conn_handle_t                    connHandle;
    
}cy_stc_ble_gatt_write_param_t;

/** Error information */
typedef struct
{
    /** Attribute Handle in which error is generated */
    cy_ble_gatt_db_attr_handle_t        attrHandle;

    /** Opcode which has resulted in Error */
    cy_en_ble_gatt_pdu_t                opCode;

    /** Error Code describing cause of error */
    cy_en_ble_gatt_err_code_t           errorCode;
    
}cy_stc_ble_gatt_err_info_t;

/** Error parameter*/
typedef struct
{
    /** Connection handle */ 
    cy_stc_ble_conn_handle_t             connHandle;

    /** Error information */
    cy_stc_ble_gatt_err_info_t           errInfo;
    
}cy_stc_ble_gatt_err_param_t;

/** Prepare Write parameter */
typedef struct
{
    /** handle value offset pair */
    cy_stc_ble_gatt_handle_value_offset_param_t  handleValOffsetPair; 

    /** Connection handle */
    cy_stc_ble_conn_handle_t                     connHandle;
    
}cy_stc_ble_gatt_prep_write_param_t;

/* --------------------------Structure corresponding to events-------------------- */

/** memory request parameters for prepare write request*/
typedef struct
{
    /** buffer to which a prepared write queue request will be stored. The
        buffer can be calculated as - total buffer = totalAttrValueLength
        + prepareWriteQueueSize * sizeof (cy_stc_ble_gatt_handle_value_offset_param_t) */
    uint8_t                 * queueBuffer;

    /** length of attribute value. This value can be the max attribute value length
            or summation of value lengths that support a long write. The value should be multiple of a
            32-bit unsigned integer. */
    uint16_t                    totalAttrValueLength;

    /** Size of prepareWriteQueue buffer. The application may decide the size based on
        (totalAttrValueLength or the max attribute length or the summation of value 
        lengths that supports long write) /(negotiated or default MTU size - 5).
        In case of a reliable write, queue depth should at least be equal to number of handles that have 
        reliable write support. */
    uint16_t                     prepareWriteQueueSize;

    /** Connection Handle */
    cy_stc_ble_conn_handle_t     connHandle;
    
} cy_stc_ble_prepare_write_request_memory_t;

/** @} */

/***************************************
** Exported APIs
***************************************/
/**
 \addtogroup group_ble_common_api_gatt_functions
 @{
*/

/******************************************************************************
* Function Name: Cy_BLE_GATT_GetMtuSize
***************************************************************************//**
* 
*  This function provides the correct GATT MTU used by BLE stack. If the function
*  is called after the GATT MTU configuration procedure, it will provide the final
*  negotiated GATT MTU else default MTU (23 Bytes).
*     
*  \param param: buffer of type 'cy_stc_ble_gatt_xchg_mtu_param_t'.
*             param->connHandle: input parameter
*             param->mtu: output parameter
* 
* \return
*  cy_en_ble_api_result_t : Return value indicates whether the function succeeded or
*  failed. Following are the possible error codes.
*
*   Errors codes                     | Description
*   ------------                     | -----------
*   CY_BLE_SUCCESS                   | On successful operation
*   CY_BLE_ERROR_INVALID_PARAMETER   | If NULL or invalid connection handle was passed
* 
******************************************************************************/
cy_en_ble_api_result_t Cy_BLE_GATT_GetMtuSize
(
    cy_stc_ble_gatt_xchg_mtu_param_t  * param
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */

#endif /* CY_BLE_STACK_GATT_H_ */

/*EOF*/
