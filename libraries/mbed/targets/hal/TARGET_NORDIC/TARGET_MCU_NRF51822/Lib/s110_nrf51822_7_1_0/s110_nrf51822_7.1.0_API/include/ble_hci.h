/*
  Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.

  The information contained herein is confidential property of Nordic Semiconductor. The use,
  copying, transfer or disclosure of such information is prohibited except by express written
  agreement with Nordic Semiconductor.
 */
/**
  @addtogroup BLE_COMMON
  @{
*/


#ifndef BLE_HCI_H__
#define BLE_HCI_H__

/** @defgroup BLE_HCI_STATUS_CODES Bluetooth status codes
 * @{ */

#define BLE_HCI_STATUS_CODE_SUCCESS                        0x00
#define BLE_HCI_STATUS_CODE_UNKNOWN_BTLE_COMMAND           0x01
#define BLE_HCI_STATUS_CODE_UNKNOWN_CONNECTION_IDENTIFIER  0x02
/*0x03 Hardware Failure
0x04 Page Timeout
*/
#define BLE_HCI_AUTHENTICATION_FAILURE                     0x05
#define BLE_HCI_STATUS_CODE_PIN_OR_KEY_MISSING             0x06
#define BLE_HCI_MEMORY_CAPACITY_EXCEEDED                   0x07
#define BLE_HCI_CONNECTION_TIMEOUT                         0x08
/*0x09 Connection Limit Exceeded
0x0A Synchronous Connection Limit To A Device Exceeded
0x0B ACL Connection Already Exists*/
#define BLE_HCI_STATUS_CODE_COMMAND_DISALLOWED             0x0C
/*0x0D Connection Rejected due to Limited Resources
0x0E Connection Rejected Due To Security Reasons
0x0F Connection Rejected due to Unacceptable BD_ADDR
0x10 Connection Accept Timeout Exceeded
0x11 Unsupported Feature or Parameter Value*/
#define BLE_HCI_STATUS_CODE_INVALID_BTLE_COMMAND_PARAMETERS 0x12
#define BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION           0x13
#define BLE_HCI_REMOTE_DEV_TERMINATION_DUE_TO_LOW_RESOURCES 0x14
#define BLE_HCI_REMOTE_DEV_TERMINATION_DUE_TO_POWER_OFF     0x15
#define BLE_HCI_LOCAL_HOST_TERMINATED_CONNECTION            0x16
/*
0x17 Repeated Attempts
0x18 Pairing Not Allowed
0x19 Unknown LMP PDU
*/
#define BLE_HCI_UNSUPPORTED_REMOTE_FEATURE 0x1A
/*
0x1B SCO Offset Rejected
0x1C SCO Interval Rejected
0x1D SCO Air Mode Rejected*/
#define BLE_HCI_STATUS_CODE_INVALID_LMP_PARAMETERS     0x1E
#define BLE_HCI_STATUS_CODE_UNSPECIFIED_ERROR          0x1F
/*0x20 Unsupported LMP Parameter Value
0x21 Role Change Not Allowed
*/
#define BLE_HCI_STATUS_CODE_LMP_RESPONSE_TIMEOUT       0x22
/*0x23 LMP Error Transaction Collision*/
#define BLE_HCI_STATUS_CODE_LMP_PDU_NOT_ALLOWED        0x24
/*0x25 Encryption Mode Not Acceptable
0x26 Link Key Can Not be Changed
0x27 Requested QoS Not Supported
*/
#define BLE_HCI_INSTANT_PASSED                         0x28
#define BLE_HCI_PAIRING_WITH_UNIT_KEY_UNSUPPORTED      0x29
#define BLE_HCI_DIFFERENT_TRANSACTION_COLLISION        0x2A
/*
0x2B Reserved
0x2C QoS Unacceptable Parameter
0x2D QoS Rejected
0x2E Channel Classification Not Supported
0x2F Insufficient Security
0x30 Parameter Out Of Mandatory Range
0x31 Reserved
0x32 Role Switch Pending
0x33 Reserved
0x34 Reserved Slot Violation
0x35 Role Switch Failed
0x36 Extended Inquiry Response Too Large
0x37 Secure Simple Pairing Not Supported By Host.
0x38 Host Busy - Pairing
0x39 Connection Rejected due to No Suitable Channel Found*/
#define BLE_HCI_CONTROLLER_BUSY                        0x3A
#define BLE_HCI_CONN_INTERVAL_UNACCEPTABLE             0x3B
#define BLE_HCI_DIRECTED_ADVERTISER_TIMEOUT            0x3C
#define BLE_HCI_CONN_TERMINATED_DUE_TO_MIC_FAILURE     0x3D
#define BLE_HCI_CONN_FAILED_TO_BE_ESTABLISHED          0x3E

/** @} */


#endif // BLE_HCI_H__

/** @} */
