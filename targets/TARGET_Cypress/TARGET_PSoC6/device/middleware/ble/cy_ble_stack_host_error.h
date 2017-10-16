/*******************************************************************************
* \file cy_ble_stack_host_error.h
* \version 2.0
*
* \brief
*  This file contains all the spec defined error code in Core Spec 5.0, Vol2, Part D
* 
* Related Document:
*  BLE Standard Spec - CoreV5.0
* 
********************************************************************************
* \copyright
* Copyright 2014-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CY_BLE_STACK_HOST_ERROR_H_
#define CY_BLE_STACK_HOST_ERROR_H_

/**
 \addtogroup group_ble_common_macros_error
 @{
*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
** Constants
***************************************/
/* HCI Status error code */

/** Success */
#define CY_BLE_HCI_SUCCESS                                                       (0x00u)

/** Unknown HCI Command */
#define CY_BLE_HCI_ERROR_UNKNOWN_HCI_COMMAND                                     (0x01u)

/** Unknown Connection Identifier */
#define CY_BLE_HCI_ERROR_NO_CONNECTION                                           (0x02u)

/** Hardware Failure */
#define CY_BLE_HCI_ERROR_HARDWARE_FAILURE                                        (0x03u)

/** \cond IGNORE */

/** Page Timeout */
#define CY_BLE_HCI_ERROR_PAGE_TIMEOUT                                            (0x04u)

/** Authentication Failure */
#define CY_BLE_HCI_ERROR_AUTHENTICATION_FAILURE                                  (0x05u)

/** PIN or Key Missing */
#define CY_BLE_HCI_ERROR_KEY_MISSING                                             (0x06u)

/** \endcond */

/** Memory Capacity Exceeded */
#define CY_BLE_HCI_ERROR_MEMORY_FULL                                             (0x07u)

/** Connection Timeout */
#define CY_BLE_HCI_ERROR_CONNECTION_TIMEOUT                                      (0x08u)

/** Connection Limit Exceeded*/
#define CY_BLE_HCI_ERROR_MAX_NUMBER_OF_CONNECTIONS                               (0x09u)

/** \cond IGNORE */

/** Synchronous Connection Limit To A Device Exceeded */
#define CY_BLE_HCI_ERROR_MAX_NUMBER_OF_SCO_CONNECTIONS                           (0x0Au)

/** \endcond */

/** ACL Connection Already Exists */
#define CY_BLE_HCI_ERROR_ACL_CONNECTION_ALREADY_EXISTS                           (0x0Bu)

/** Command Disallowed */
#define CY_BLE_HCI_ERROR_COMMAND_DISALLOWED                                      (0x0Cu)

/** \cond IGNORE */

/** Connection Rejected due to Limited Resources */
#define CY_BLE_HCI_ERROR_HOST_REJECTED_LIMITED_RESOURCES                         (0x0Du)

/** Connection Rejected Due To Security Reasons */
#define CY_BLE_HCI_ERROR_HOST_REJECTED_SECURITY_REASONS                          (0x0Eu)

/** Connection Rejected due to Unacceptable BD_ADDR */
#define CY_BLE_HCI_ERROR_HOST_REJECTED_PERSONAL_DEVICE                           (0x0Fu)

/** Connection Accept Timeout Exceeded */
#define CY_BLE_HCI_ERROR_HOST_TIMEOUT                                            (0x10u)

/** Unsupported Feature or Parameter Value */
#define CY_BLE_HCI_ERROR_UNSUPPORTED_FEATURE_OR_PARAMETER                        (0x11u)

/** \endcond */

/** Invalid HCI Command Parameters */
#define CY_BLE_HCI_ERROR_INVALID_HCI_COMMAND_PARAMETERS                          (0x12u)

/** Remote User Terminated Connection */
#define CY_BLE_HCI_ERROR_OTHER_END_TERMINATED_USER                               (0x13u)

/** Remote Device Terminated Connection due to Low Resources */
#define CY_BLE_HCI_ERROR_OTHER_END_TERMINATED_LOW_RESOURCES                      (0x14u)

/** Remote Device Terminated Connection due to Power Off */
#define CY_BLE_HCI_ERROR_OTHER_END_TERMINATED_POWER_OFF                          (0x15u)

/** Connection Terminated By Local Host */
#define CY_BLE_HCI_ERROR_CONNECTION_TERMINATED_BY_LOCAL_HOST                     (0x16u)

/** Repeated Attempts */
#define CY_BLE_HCI_ERROR_REPEATED_ATTEMPTS                                       (0x17u)

/** \cond IGNORE */

/** Pairing Not Allowed */
#define CY_BLE_HCI_ERROR_PAIRING_NOT_ALLOWED                                     (0x18u)

/** Unknown LMP PDU */
#define CY_BLE_HCI_ERROR_UNKNOWN_LMP_PDU                                         (0x19u)

/** Unsupported Remote Feature / Unsupported LMP Feature */
#define CY_BLE_HCI_ERROR_UNSUPPORTED_REMOTE_FEATURE                              (0x1Au)

/** SCO Offset Rejected */
#define CY_BLE_HCI_ERROR_SCO_OFFSET_REJECTED                                     (0x1Bu)

/** SCO Interval Rejected */
#define CY_BLE_HCI_ERROR_SCO_INTERVAL_REJECTED                                   (0x1Cu)

/** SCO Air Mode Rejected */
#define CY_BLE_HCI_ERROR_SCO_AIR_MODE_REJECTED                                   (0x1Du)

/** Invalid LMP Parameters / Invalid LL Parameters */
#define CY_BLE_HCI_ERROR_INVALID_LMP_PARAMETERS                                  (0x1Eu)

/** Unspecified Error */
#define CY_BLE_HCI_ERROR_UNSPECIFIED_ERROR                                       (0x1Fu)

/** Unsupported LMP Parameter Value / Unsupported LL Parameter Value */
#define CY_BLE_HCI_ERROR_UNSUPPORTED_LMP_PARAMETER_VALUE                         (0x20u)

/** Role Change Not Allowed */
#define CY_BLE_HCI_ERROR_ROLE_CHANGE_NOT_ALLOWED                                 (0x21u)

/** LMP Response Timeout / LL Response Timeout */
#define CY_BLE_HCI_ERROR_LMP_RESPONSE_TIMEOUT                                    (0x22u)

/** LMP Error Transaction Collision / LL Procedure Collision  */
#define CY_BLE_HCI_ERROR_LMP_ERROR_TRANSACTION_COLLISION                         (0x23u)

/** LMP PDU Not Allowed */
#define CY_BLE_HCI_ERROR_LMP_PDU_NOT_ALLOWED                                     (0x24u)

/** Encryption Mode Not Acceptable */
#define CY_BLE_HCI_ERROR_ENCRYPTION_MODE_NOT_ACCEPTABLE                          (0x25u)

/** Link Key cannot be Changed */
#define CY_BLE_HCI_ERROR_LINK_KEY_CANNOT_BE_USED                                 (0x26u)

/** Requested QoS Not Supported */
#define CY_BLE_HCI_ERROR_QOS_IS_NOT_SUPPORTED                                    (0x27u)

/** Instant Passed */
#define CY_BLE_HCI_ERROR_INSTANT_PASSED                                          (0x28u)

/** Pairing With Unit Key Not Supported */
#define CY_BLE_HCI_ERROR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED                     (0x29u)

/** Different Transaction Collision */
#define CY_BLE_HCI_ERROR_DIFFERENT_TRANSACTION_COLLISION                         (0x2Au)

/* 0x2B Reserved for Future Use [E6305] */

/** QoS Unacceptable Parameter */
#define CY_BLE_HCI_ERROR_QOS_UNACCEPTABLE_PARAMETER                              (0x2Cu)

/** QoS Rejected */
#define CY_BLE_HCI_ERROR_QOS_REJECTED                                            (0x2Du)

/** Channel Classification Not Supported */
#define CY_BLE_HCI_ERROR_CHANNEL_CLASSIFICATION_NOT_SUPPORTED                    (0x2Eu)

/** Insufficient Security */
#define CY_BLE_HCI_ERROR_INSUFFICIENT_SECURITY                                   (0x2Fu)

/** Parameter Out Of Mandatory Range */
#define CY_BLE_HCI_ERROR_PARAMETER_OUT_OF_MANDATORY_RANGE                        (0x30u)

/* 0x31 Reserved for Future Use */

/** Role Switch Pending */
#define CY_BLE_HCI_ERROR_ROLE_SWITCH_PENDING                                     (0x32u)

/* 0x33 Reserved for Future Use */

/** Reserved Slot Violation */
#define CY_BLE_HCI_ERROR_RESERVED_SLOT_VIOLATION                                 (0x34u)

/** Role Switch Failed */
#define CY_BLE_HCI_ERROR_ROLE_SWITCH_FAILED                                      (0x35u)

/** Extended Inquiry Response Too Large */
#define CY_BLE_HCI_ERROR_ENTENDED_INQUIRY_RESPONSE_TOO_LARGE                     (0x36u)

/** Secure Simple Pairing Not Supported By Host */
#define CY_BLE_HCI_ERROR_SECURE_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST             (0x37u)

/** Host Busy - Pairing */
#define CY_BLE_HCI_ERROR_HOST_BUSY_PAIRING                                       (0x38u)

/** Connection Rejected due to No Suitable Channel Found */
#define CY_BLE_HCI_ERROR_CONNECTION_REJETED_DUE_TO_NO_SUITABLE_SCHANNEL_FOUND    (0x39u)

/** \endcond */

/** Controller Busy */
#define CY_BLE_HCI_ERROR_CONTROLLER_BUSY                                         (0x3Au)

/** Unacceptable Connection Parameters */
#define CY_BLE_HCI_ERROR_UNACCEPTEBALE_CONNECTION_INTERVAL                       (0x3Bu)

/** Directed Advertising Timeout */
#define CY_BLE_HCI_ERROR_DIRECTED_ADVERTISING_TIMEOUT                            (0x3Cu)

/** Connection Terminated due to MIC Failure */
#define CY_BLE_HCI_ERROR_CONNECTION_TREMINATED_DUE_TO_MIC_FAILURE                (0x3Du)

/** Connection Failed to be Established */
#define CY_BLE_HCI_ERROR_CONNECTION_FAILED_TO_BE_ESTABLISHED                     (0x3Eu)

/** MAC Connection Failed */
#define CY_BLE_HCI_ERROR_MAC_CONNECTION_FAILED                                   (0x3Fu)

/** Coarse Clock Adjustment Rejected but Will Try to Adjust Using Clock Dragging */
#define CY_BLE_HCI_ERROR_COARSE_CLK_ADJUSTMENT                                   (0x40u)

/** Type0 Submap Not Defined */
#define CY_BLE_HCI_ERROR_TYPE0_SUBMAP_NOT_DEFINED                                (0x41u)

/** Unknown Advertising Identifier */
#define CY_BLE_HCI_ERROR_UNKNOWN_ADV_IDENTIFIER                                  (0x42u)

/** Limit Reached */
#define CY_BLE_HCI_ERROR_LIMIT_REACHED                                           (0x43u)

/** Operation Cancelled by Host */
#define CY_BLE_HCI_ERROR_OPERATION_CANCELLED_BY_HOST                             (0x44u)


/* Hardware Error event - Error Codes */

/** HCI transport sync lost error */
#define CY_BLE_HW_ERR_SYNC_LOST                   0x01

/** HCI transport unknown pkt */
#define CY_BLE_HW_ERR_UNKNOWN_PKT                 0x02

/** HCI transport pkt length exceeded */
#define CY_BLE_HW_ERR_LEN_EXCEEDED                0x03

/** HCI transport no cmd pkt buffer available */
#define CY_BLE_HW_ERR_CMD_NO_BUFFER               0X04

/** HCI transport no acl pkt buffer available */
#define CY_BLE_HW_ERR_ACL_NO_BUFFER               0X05

/** HCI transport no sync pkt buffer available */
#define CY_BLE_HW_ERR_SYNC_NO_BUFFER              0X06

/** HCI transport cmd pkt length exceeded */
#define CY_BLE_HW_ERR_CMD_LEN_EXCEEDED            0X07

/** HCI transport acl pkt length exceeded */
#define CY_BLE_HW_ERR_ACL_LEN_EXCEEDED            0X08

/** HCI transport sync pkt length exceeded */
#define CY_BLE_HW_ERR_SYNC_LEN_EXCEEDED           0X09

/** HCI transport wrong parameter for acl pkt */
#define CY_BLE_HW_ERR_WRNG_ACL_DATA_PARAM         0x0A

/** HCI transport wrong parameter for sync pkt */
#define CY_BLE_HW_ERR_WRNG_SYNC_DATA_PARAM        0X0B

/** HW init failure */
#define CY_BLE_HW_ERR_HW_INIT_FAILURE             0x0C

#ifdef __cplusplus
}
#endif /* __cplusplus */
/** @} */

#endif /* CY_BLE_STACK_HOST_ERROR_H_ */

