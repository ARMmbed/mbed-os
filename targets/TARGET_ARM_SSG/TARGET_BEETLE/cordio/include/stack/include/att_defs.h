/*************************************************************************************************/
/*!
 *  \file   att_defs.h
 *        
 *  \brief  Attribute protocol constants and definitions from the Bluetooth specification.
 *
 *          $Date: 2015-09-10 14:58:31 -0700 (Thu, 10 Sep 2015) $
 *          $Revision: 3838 $
 *  
 *  Copyright (c) 2009 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#ifndef ATT_DEFS_H
#define ATT_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Attribute PDU format */
#define ATT_HDR_LEN                   1         /*! Attribute PDU header length */
#define ATT_AUTH_SIG_LEN              12        /*! Authentication signature length */
#define ATT_DEFAULT_MTU               23        /*! Default value of ATT_MTU */
#define ATT_MAX_MTU                   517       /*! Maximum value of ATT_MTU */
#define ATT_DEFAULT_PAYLOAD_LEN       20        /*! Default maximum payload length for most PDUs */

/*! Attribute value parameters */
#define ATT_VALUE_MAX_LEN             512       /*! Maximum attribute value length */
#define ATT_VALUE_MAX_OFFSET          511       /*! Maximum attribute value offset */

/*! Transaction timeout */
#define ATT_MAX_TRANS_TIMEOUT         30        /*! Maximum transaction timeout in seconds */

/*! Error codes */
#define ATT_SUCCESS                   0x00      /*! Operation successful */
#define ATT_ERR_HANDLE                0x01      /*! Invalid handle */
#define ATT_ERR_READ                  0x02      /*! Read not permitted */
#define ATT_ERR_WRITE                 0x03      /*! Write not permitted */
#define ATT_ERR_INVALID_PDU           0x04      /*! Invalid pdu */
#define ATT_ERR_AUTH                  0x05      /*! Insufficient authentication */
#define ATT_ERR_NOT_SUP               0x06      /*! Request not supported */
#define ATT_ERR_OFFSET                0x07      /*! Invalid offset */
#define ATT_ERR_AUTHOR                0x08      /*! Insufficient authorization */
#define ATT_ERR_QUEUE_FULL            0x09      /*! Prepare queue full */
#define ATT_ERR_NOT_FOUND             0x0A      /*! Attribute not found */
#define ATT_ERR_NOT_LONG              0x0B      /*! Attribute not long */
#define ATT_ERR_KEY_SIZE              0x0C      /*! Insufficient encryption key size */
#define ATT_ERR_LENGTH                0x0D      /*! Invalid attribute value length */
#define ATT_ERR_UNLIKELY              0x0E      /*! Other unlikely error */
#define ATT_ERR_ENC                   0x0F      /*! Insufficient encryption */
#define ATT_ERR_GROUP_TYPE            0x10      /*! Unsupported group type */
#define ATT_ERR_RESOURCES             0x11      /*! Insufficient resources */
#define ATT_ERR_CCCD                  0xFD      /*! CCCD improperly configured */
#define ATT_ERR_IN_PROGRESS           0xFE      /*! Procedure already in progress */
#define ATT_ERR_RANGE                 0xFF      /*! Value out of range */

/*! Proprietary internal error codes */
#define ATT_ERR_MEMORY                0x70      /*! Out of memory */
#define ATT_ERR_TIMEOUT               0x71      /*! Transaction timeout */
#define ATT_ERR_OVERFLOW              0x72      /*! Transaction overflow */
#define ATT_ERR_INVALID_RSP           0x73      /*! Invalid response PDU */
#define ATT_ERR_CANCELLED             0x74      /*! Request cancelled */
#define ATT_ERR_UNDEFINED             0x75      /*! Other undefined error */
#define ATT_ERR_REQ_NOT_FOUND         0x76      /*! Required characteristic not found */
#define ATT_ERR_MTU_EXCEEDED          0x77      /*! Attribute PDU length exceeded MTU size */
#define ATT_CONTINUING                0x78      /*! Procedure continuing */

/*! Application error codes */
#define ATT_ERR_VALUE_RANGE           0x80      /*! Value out of range */

/*! PDU types */
#define ATT_PDU_ERR_RSP               0x01      /*! Error response */
#define ATT_PDU_MTU_REQ               0x02      /*! Exchange mtu request */
#define ATT_PDU_MTU_RSP               0x03      /*! Exchange mtu response */
#define ATT_PDU_FIND_INFO_REQ         0x04      /*! Find information request */
#define ATT_PDU_FIND_INFO_RSP         0x05      /*! Find information response */
#define ATT_PDU_FIND_TYPE_REQ         0x06      /*! Find by type value request */
#define ATT_PDU_FIND_TYPE_RSP         0x07      /*! Find by type value response */
#define ATT_PDU_READ_TYPE_REQ         0x08      /*! Read by type request */
#define ATT_PDU_READ_TYPE_RSP         0x09      /*! Read by type response */
#define ATT_PDU_READ_REQ              0x0A      /*! Read request */
#define ATT_PDU_READ_RSP              0x0B      /*! Read response */
#define ATT_PDU_READ_BLOB_REQ         0x0C      /*! Read blob request */
#define ATT_PDU_READ_BLOB_RSP         0x0D      /*! Read blob response */
#define ATT_PDU_READ_MULT_REQ         0x0E      /*! Read multiple request */
#define ATT_PDU_READ_MULT_RSP         0x0F      /*! Read multiple response */
#define ATT_PDU_READ_GROUP_TYPE_REQ   0x10      /*! Read by group type request */
#define ATT_PDU_READ_GROUP_TYPE_RSP   0x11      /*! Read by group type response */
#define ATT_PDU_WRITE_REQ             0x12      /*! Write request */
#define ATT_PDU_WRITE_RSP             0x13      /*! Write response */
#define ATT_PDU_WRITE_CMD             0x52      /*! Write command */
#define ATT_PDU_SIGNED_WRITE_CMD      0xD2      /*! Signed write command */
#define ATT_PDU_PREP_WRITE_REQ        0x16      /*! Prepare write request */
#define ATT_PDU_PREP_WRITE_RSP        0x17      /*! Prepare write response */
#define ATT_PDU_EXEC_WRITE_REQ        0x18      /*! Execute write request */
#define ATT_PDU_EXEC_WRITE_RSP        0x19      /*! Execute write response */
#define ATT_PDU_VALUE_NTF             0x1B      /*! Handle value notification */
#define ATT_PDU_VALUE_IND             0x1D      /*! Handle value indication */
#define ATT_PDU_VALUE_CNF             0x1E      /*! Handle value confirmation */
#define ATT_PDU_MAX                   0x1F      /*! PDU Maximum */

/*! Length of PDU fixed length fields */
#define ATT_ERR_RSP_LEN               5
#define ATT_MTU_REQ_LEN               3
#define ATT_MTU_RSP_LEN               3
#define ATT_FIND_INFO_REQ_LEN         5
#define ATT_FIND_INFO_RSP_LEN         2
#define ATT_FIND_TYPE_REQ_LEN         7
#define ATT_FIND_TYPE_RSP_LEN         1
#define ATT_READ_TYPE_REQ_LEN         5
#define ATT_READ_TYPE_RSP_LEN         2
#define ATT_READ_REQ_LEN              3
#define ATT_READ_RSP_LEN              1
#define ATT_READ_BLOB_REQ_LEN         5
#define ATT_READ_BLOB_RSP_LEN         1
#define ATT_READ_MULT_REQ_LEN         1
#define ATT_READ_MULT_RSP_LEN         1
#define ATT_READ_GROUP_TYPE_REQ_LEN   5
#define ATT_READ_GROUP_TYPE_RSP_LEN   2
#define ATT_WRITE_REQ_LEN             3
#define ATT_WRITE_RSP_LEN             1
#define ATT_WRITE_CMD_LEN             3
#define ATT_SIGNED_WRITE_CMD_LEN      (ATT_WRITE_CMD_LEN + ATT_AUTH_SIG_LEN)
#define ATT_PREP_WRITE_REQ_LEN        5
#define ATT_PREP_WRITE_RSP_LEN        5
#define ATT_EXEC_WRITE_REQ_LEN        2
#define ATT_EXEC_WRITE_RSP_LEN        1
#define ATT_VALUE_NTF_LEN             3
#define ATT_VALUE_IND_LEN             3
#define ATT_VALUE_CNF_LEN             1

/*! Find information response format */
#define ATT_FIND_HANDLE_16_UUID       0x01      /*! Handle and 16 bit UUID */
#define ATT_FIND_HANDLE_128_UUID      0x02      /*! Handle and 128 bit UUID */

/*! Execute write request flags */
#define ATT_EXEC_WRITE_CANCEL         0x00      /*! Cancel all prepared writes */
#define ATT_EXEC_WRITE_ALL            0x01      /*! Write all pending prepared writes */

/*! PDU masks */
#define ATT_PDU_MASK_SERVER           0x01      /*! Server bit mask */
#define ATT_PDU_MASK_COMMAND          0x40      /*! Command bit mask */
#define ATT_PDU_MASK_SIGNED           0x80      /*! Auth signature bit mask */

/*! Handles */
#define ATT_HANDLE_NONE               0x0000
#define ATT_HANDLE_START              0x0001
#define ATT_HANDLE_MAX                0xFFFF

/*! UUID lengths */
#define ATT_NO_UUID_LEN               0         /*! Length when no UUID is present ;-) */
#define ATT_16_UUID_LEN               2         /*! Length in bytes of a 16 bit UUID */
#define ATT_128_UUID_LEN              16        /*! Length in bytes of a 128 bit UUID */

/*! GATT characteristic properties */
#define ATT_PROP_BROADCAST            0x01      /*! Permit broadcasts */
#define ATT_PROP_READ                 0x02      /*! Permit reads */
#define ATT_PROP_WRITE_NO_RSP         0x04      /*! Permit writes without response */
#define ATT_PROP_WRITE                0x08      /*! Permit writes with response */
#define ATT_PROP_NOTIFY               0x10      /*! Permit notifications */
#define ATT_PROP_INDICATE             0x20      /*! Permit indications */
#define ATT_PROP_AUTHENTICATED        0x40      /*! Permit signed writes */
#define ATT_PROP_EXTENDED             0x80      /*! More properties defined in extended properties */

/*! GATT characteristic extended properties */
#define ATT_EXT_PROP_RELIABLE_WRITE   0x0001    /*! Permit reliable writes */
#define ATT_EXT_PROP_WRITEABLE_AUX    0x0002    /*! Permit write to characteristic descriptor */

/*! GATT client characteristic configuration */
#define ATT_CLIENT_CFG_NOTIFY         0x0001    /*! Notify the value */
#define ATT_CLIENT_CFG_INDICATE       0x0002    /*! Indicate the value */

/*! GATT server characteristic configuration */
#define ATT_SERVER_CFG_BROADCAST      0x0001    /*! Broadcast the value */

/*! GATT characteristic format */
#define ATT_FORMAT_BOOLEAN            0x01      /*! Boolean */
#define ATT_FORMAT_2BIT               0x02      /*! Unsigned 2 bit integer */
#define ATT_FORMAT_NIBBLE             0x03      /*! Unsigned 4 bit integer */
#define ATT_FORMAT_UINT8              0x04      /*! Unsigned 8 bit integer */
#define ATT_FORMAT_UINT12             0x05      /*! Unsigned 12 bit integer */
#define ATT_FORMAT_UINT16             0x06      /*! Unsigned 16 bit integer */
#define ATT_FORMAT_UINT24             0x07      /*! Unsigned 24 bit integer */
#define ATT_FORMAT_UINT32             0x08      /*! Unsigned 32 bit integer */
#define ATT_FORMAT_UINT48             0x09      /*! Unsigned 48 bit integer */
#define ATT_FORMAT_UINT64             0x0A      /*! Unsigned 64 bit integer */
#define ATT_FORMAT_UINT128            0x0B      /*! Unsigned 128 bit integer */
#define ATT_FORMAT_SINT8              0x0C      /*! Signed 8 bit integer */
#define ATT_FORMAT_SINT12             0x0D      /*! Signed 12 bit integer */
#define ATT_FORMAT_SINT16             0x0E      /*! Signed 16 bit integer */
#define ATT_FORMAT_SINT24             0x0F      /*! Signed 24 bit integer */
#define ATT_FORMAT_SINT32             0x10      /*! Signed 32 bit integer */
#define ATT_FORMAT_SINT48             0x11      /*! Signed 48 bit integer */
#define ATT_FORMAT_SINT64             0x12      /*! Signed 64 bit integer */
#define ATT_FORMAT_SINT128            0x13      /*! Signed 128 bit integer */
#define ATT_FORMAT_FLOAT32            0x14      /*! IEEE-754 32 bit floating point */
#define ATT_FORMAT_FLOAT64            0x15      /*! IEEE-754 64 bit floating point */
#define ATT_FORMAT_SFLOAT             0x16      /*! IEEE-11073 16 bit SFLOAT */
#define ATT_FORMAT_FLOAT              0x17      /*! IEEE-11073 32 bit FLOAT */
#define ATT_FORMAT_DUINT16            0x18      /*! IEEE-20601 format */
#define ATT_FORMAT_UTF8               0x19      /*! UTF-8 string */
#define ATT_FORMAT_UTF16              0x1A      /*! UTF-16 string */
#define ATT_FORMAT_STRUCT             0x1B      /*! Opaque structure */

#ifdef __cplusplus
};
#endif

#endif /* ATT_DEFS_H */
