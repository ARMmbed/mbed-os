/**
  ******************************************************************************
  * @file    ble_status.h
  * @author  AMS - RF Application team
  * @version V2.0.0
  * @date    16-April-2018
  * @brief   Header file which contains definition of Bluetooth status and error codes.
  * @note    Error Codes are defined according to Bluetooth Specifications 
  *              Vol.2 Part D.
  *           - HCI/Controller standard codes range is [0x00 : 0x3F].
  *             Their name is prefixed by "BLE_ERROR_" 
  *           - Host specific (and proprietary) error codes range is [0x40 : 0xFF].
  *             Their name is prefixed by "BLE_STATUS_" and they're grouped by Layer
  *           - BLE_STATUS_SUCCESS value is 0x00 and is the unique error code
  *             used by both Controller and Host layers.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
  ******************************************************************************
  */
#ifndef __BLE_STATUS_H__
#define __BLE_STATUS_H__

#include <bluenrg1_api.h> 

/* @brief Standard Error Codes as specified by the Bluetooth Specifications
 * Vol.2, Part D ("Error Codes").
 * According to the spec the error codes range is from 0x00 to 0x3F.
 */
#define BLE_STATUS_SUCCESS                          ((tBleStatus)(0x00))

#define BLE_ERROR_UNKNOWN_HCI_COMMAND               ((tBleStatus)(0x01))
#define BLE_ERROR_UNKNOWN_CONNECTION_ID             ((tBleStatus)(0x02))

#define BLE_ERROR_HARDWARE_FAILURE                  ((tBleStatus)(0x03)) 

#define BLE_ERROR_AUTHENTICATION_FAILURE            ((tBleStatus)(0x05))
#define BLE_ERROR_KEY_MISSING                       ((tBleStatus)(0x06))
#define BLE_ERROR_MEMORY_CAPACITY_EXCEEDED          ((tBleStatus)(0x07))
#define BLE_ERROR_CONNECTION_TIMEOUT                ((tBleStatus)(0x08))

#define BLE_ERROR_COMMAND_DISALLOWED                ((tBleStatus)(0x0C))

#define BLE_ERROR_UNSUPPORTED_FEATURE               ((tBleStatus)(0x11))

#define BLE_ERROR_INVALID_HCI_CMD_PARAMS            ((tBleStatus)(0x12))

#define BLE_ERROR_TERMINATED_REMOTE_USER            ((tBleStatus)(0x13))

#define BLE_ERROR_TERMINATED_LOCAL_HOST             ((tBleStatus)(0x16))

#define BLE_ERROR_UNSUPP_RMT_FEATURE                ((tBleStatus)(0x1A))

#define BLE_ERROR_UNSPECIFIED                       ((tBleStatus)(0x1F))

#define BLE_ERROR_PROCEDURE_TIMEOUT                 ((tBleStatus)(0x22))

#define BLE_ERROR_INSTANT_PASSED                    ((tBleStatus)(0x28))

#define BLE_ERROR_PARAMETER_OUT_OF_RANGE            ((tBleStatus)(0x30))

#define BLE_ERROR_HOST_BUSY_PAIRING                 ((tBleStatus)(0x38))

#define BLE_ERROR_CONTROLLER_BUSY                   ((tBleStatus)(0x3A))

#define BLE_ERROR_DIRECTED_ADVERTISING_TIMEOUT      ((tBleStatus)(0x3C))

#define BLE_ERROR_CONNECTION_END_WITH_MIC_FAILURE   ((tBleStatus)(0x3D))
#define BLE_ERROR_CONNECTION_FAILED_TO_ESTABLISH    ((tBleStatus)(0x3E))


/**
* @name Generic/System error codes
* @brief The error codes as reported by the different Protocol layers.
*        They start with 0x40
* @{
*/

/**
 * @brief The Connection Identifier does not exist.
 * Temporary remapped to corresponding Controller Error.
 */
#define BLE_STATUS_UNKNOWN_CONNECTION_ID    ((tBleStatus)(0x40))

/**
 * @brief The Host failed while performing the requested operation.
 */
#define BLE_STATUS_FAILED                   ((tBleStatus)(0x41))

/*! @enum tBleStatus::BLE_STATUS_INVALID_PARAMS
 * 
 */
#define BLE_STATUS_INVALID_PARAMS           ((tBleStatus)(0x42))

/**
 * @brief The Host is already processing another request received in advance.
 */
#define BLE_STATUS_BUSY                     ((tBleStatus)(0x43))

/*
 * Currently not used, because this error is always signalled with an explicit
 * Error Response message seny by the relative layer.
 */
//#define BLE_STATUS_INVALID_LEN_PDU          ((tBleStatus)(0x44))

/**
 * @brief The operation requested cannot be completed immediately by the Host
 * (usually because of lack of resources). 
 * The operation is generally put on hold by the caller and it's usually 
 * retried on later time.
 */
#define BLE_STATUS_PENDING                  ((tBleStatus)(0x45))

/**
 * @brief The requested operation cannot be performed by the Host in the current status.
 */
#define BLE_STATUS_NOT_ALLOWED              ((tBleStatus)(0x46))

/**
 * @brief The requested operation violates the logic of the called layer/function or
 * the format of the data to be processed during the operation.
 */
#define BLE_STATUS_ERROR                    ((tBleStatus)(0x47))

/**
 * @brief The requested operation failed because of lack of memory.
 * Out of memory shall be returned for situations where memory will never 
 * become available again (e.g. ATT database)
 */
#define BLE_STATUS_OUT_OF_MEMORY            ((tBleStatus)(0x48))

/**
*@}
*/


/**
 * @name L2CAP error codes
 * @{
 */

/**
 * @brief An invalid L2CAP CID/channel has been selected to send data over.
 */
#define BLE_STATUS_INVALID_CID              ((tBleStatus)(0x50))

/**
*@}
*/


/**
* @name  Security Manager error codes
* @brief Error codes returned by the Security Manager Protocol (SMP)
* @{
*/

/**
 * @brief The remote device in in the Blacklist and the pairing operation it requested
 * cannot be performed.
 */
#define BLE_STATUS_DEV_IN_BLACKLIST         ((tBleStatus)(0x59))

/**
 * @brief CSRK not found during validation of an incoming signed packet
 */
#define BLE_STATUS_CSRK_NOT_FOUND           ((tBleStatus)(0x5A))

/** 
 * @brief Currently not used!
 */
#define BLE_STATUS_IRK_NOT_FOUND            ((tBleStatus)(0x5B))

/**
 * @brief A search for a specific remote device was unsuccessfull because no entry exists
 * either into Security/GATT Database (flash-based) or in volatile database. 
 */
#define BLE_STATUS_DEV_NOT_FOUND            ((tBleStatus)(0x5C))

/**
 * @brief The security database is full and no more records can be added.
 */
#define BLE_STATUS_SEC_DB_FULL              ((tBleStatus)(0x5D))

/**
 * @brief The remote device is not bonded, and no operations related to bonded devices
 * may be performed (e.g. writing Gatt Client data).
 */
#define BLE_STATUS_DEV_NOT_BONDED           ((tBleStatus)(0x5E))

/**
 * @brief The encryption key size used for encrypting the link is insufficient\n
 */
#define BLE_INSUFFICIENT_ENC_KEYSIZE        ((tBleStatus)(0x5F))

/**
*@}
*/


/**
* @name Gatt layer Error Codes
* @brief Error codes returned by the Gatt layer
* @{
*/
#define BLE_STATUS_INVALID_HANDLE           ((tBleStatus)(0x60))

/** 
 * @brief There aren't sufficient Attributes handles available for allocation during
 * creation of Services, Characteristics or Descriptors.
 */
#define BLE_STATUS_OUT_OF_HANDLE            ((tBleStatus)(0x61))

/** 
 * @brief The requested GATT operation is not allowed in this context/status or using 
 * the provided parameters. 
 * This is a specific GATT error, different from generic Not Allowed error, 
 * because it refers to specific GATT specifications/rules.
 */
#define BLE_STATUS_INVALID_OPERATION        ((tBleStatus)(0x62))

/** 
 * @brief The characteristic has already been added to the database.
 */
#define BLE_STATUS_CHARAC_ALREADY_EXISTS    ((tBleStatus)(0x63))

/** 
 * @brief The requested operation failed for a temporary lack of resources
 * (e.g. packet pool or timers), but it may be retried later when resources may
 * become available (packets or timers may have been released by other consumers).
 */
#define BLE_STATUS_INSUFFICIENT_RESOURCES   ((tBleStatus)(0x64))

/** 
 * @brief Notification/Indication can't be sent to the requested remote device because
 * it doesn't satisfy the needed security permission.
 */ 
#define BLE_STATUS_SEC_PERMISSION_ERROR     ((tBleStatus)(0x65))

/**
*@}
*/


/**
* @name GAP layer Error Codes
* @brief Error codes returned externally by the GAP layer
* @{
*/

/** 
 *  @brief The address of the device could not be resolved using the IRK stored\n
 */
#define BLE_STATUS_ADDRESS_NOT_RESOLVED     ((tBleStatus)(0x70))

/**
*@}
*/


/**
* @name Link Layer error Codes
* @brief Error codes returned by the Link layer during advertising/conection.
* @{
*/

/** 
 * @brief Returned when no valid slots are available (e.g. when there are no available state machines).
 */
#define BLE_STATUS_NO_VALID_SLOT            ((tBleStatus)(0x82))

/** 
 * @brief Returned when a scan window shorter than minimum allowed value has been requested (i.e. 2ms)
 */
#define BLE_STATUS_SCAN_WINDOW_SHORT        ((tBleStatus)(0x83))

/** 
 * @brief Returned when the maximum requested interval to be allocated is shorter then the current
 * anchor period and a there is no submultiple for the current anchor period that is between
 * the minimum and the maximum requested intervals.
 */
#define BLE_STATUS_NEW_INTERVAL_FAILED      ((tBleStatus)(0x84))

/** 
 * @brief Returned when the maximum requested interval to be allocated is greater than the current anchor
 * period, and there is no multiple of the anchor period that is between the minimum and the maximum
 * requested intervals.
 */
#define BLE_STATUS_INTERVAL_TOO_LARGE       ((tBleStatus)(0x85))

/** 
 * @brief Returned when the current anchor period or a new one can be found that is compatible to the
 * interval range requested by the new slot, but the maximum available length that can be allocated is
 * less than the minimum requested slot length.
 */
#define BLE_STATUS_LENGTH_FAILED            ((tBleStatus)(0x86))

/**
*@}
*/

/**
* @name flash error codes
* @brief Flash sector read write error codes, which used to start with 0x49
*        and are currently remapped from 0xFA to 0xFC
* @{
*/
#define FLASH_READ_FAILED                   ((tBleStatus)(0xFA))
#define FLASH_WRITE_FAILED                  ((tBleStatus)(0xFB))
#define FLASH_ERASE_FAILED                  ((tBleStatus)(0xFC))

/**
 * @name Profiles Library Error Codes
 * @{
 */
#define BLE_STATUS_TIMEOUT                          (0xFF)
#define BLE_STATUS_PROFILE_ALREADY_INITIALIZED      (0xF0)
#define BLE_STATUS_NULL_PARAM                       (0xF1) 

/**
*@}
*/
     

#endif
