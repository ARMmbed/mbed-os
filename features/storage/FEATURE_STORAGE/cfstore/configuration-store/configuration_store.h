/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/** @file  configuration_store.h
 *
 * This is the interface file to configuration store.
 *
 * The following (referred to as [REFERENCE_1] in other parts of the documentation)
 * discusses synchronous/asynchronous completion of \ref ARM_CFSTORE_DRIVER methods
 * and the inter-relationship of
 * \ref ARM_CFSTORE_OPCODE and \ref ARM_CFSTORE_CALLBACK.
 *
 * Configuration Store driver dispatch methods can operate in 2 modes:

 * - Synchronous (i.e. when
 *      \ref ARM_CFSTORE_CAPABILITIES ::asynchronous_ops == 0)
 *   then the \ref ARM_CFSTORE_DRIVER ::Dispatch_Method_Xxx() will return:
 *      - >= 0 => CFSTORE Dispatch_Method_Xxx() completed successfully
 *      - otherwise CFSTORE Dispatch_Method_Xxx() did not complete successfully
 *        (return code supplies further details).
 *
 *   In synchronous mode it is optional as to whether the client registers
 *   a client callback with the Initialize() method. If one is registered, it
 *   will be called to indicate the synchronous completion status.
 *
 * - Asynchronous (i.e. when
 *      \ref ARM_CFSTORE_CAPABILITIES ::asynchronous_ops == 1)
 *   then the \ref ARM_CFSTORE_DRIVER ::Dispatch_Method_Xxx() will return:
 *    - return_value = ARM_DRIVER_OK (i.e. ==0) implies CFSTORE
 *      Dispatch_Method_Xxx() transaction pending. Dispatch_Method_Xxx()
 *      completion status will be indicated via
 *      an asynchronous call to \ref ARM_CFSTORE_CALLBACK registered with the
 *      \ref ARM_CFSTORE_DRIVER ::(*Initialize)(), where the status supplied to
 *      the callback will be the final status of the call.
 *    - return_value > ARM_DRIVER_OK (i.e. > 0) implies CFSTORE Dispatch_Method_Xxx() completely
 *      synchronously and successfully. The return_value has specific
 *      meaning for the Dispatch_Method_Xxx() e.g. for the Read() method
 *      return_value is the number of bytes read.
 *    - otherwise return_value < 0 => CFSTORE Dispatch_Method_Xxx()
 *      completed unsuccessfully (return code supplies further details).
 *
 * The client registered asynchronous callback method \ref ARM_CFSTORE_CALLBACK is
 * registered by the client using:
 *   \ref ARM_CFSTORE_DRIVER ::(*Initialize)(\ref ARM_CFSTORE_CALLBACK callback, void* client_context)
 * See the (*Initialize) documentation for more details.
 *
 * The registered callback has the following prototype:
 *
 * typedef void (* \refARM_CFSTORE_CALLBACK)(
 *                          int32_t status,
 *                          \ref ARM_CFSTORE_OPCODE cmd_code,
 *                          void *client_context,
 *                          \ref ARM_CFSTORE_HANDLE handle);
 *
 * Before an asynchronous notification is received, a client can check on the
 * status of the call by calling \ref ARM_CFSTORE_DRIVER ::(*GetStatus)().
 *
 */

#ifndef __CONFIGURATION_STORE_H
#define __CONFIGURATION_STORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>								/* requierd for memset() in ARM_CFSTORE_HANDLE_INIT() */

#include "mbed_toolchain.h"                     /* required for MBED_DEPRECATED_SINCE */

#define DEVICE_STORAGE 1						/* enable storage */
/// @cond CFSTORE_DOXYGEN_DISABLE
#include <Driver_Storage.h>
#include <Driver_Common.h>

#define ARM_CFSTORE_API_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,0)  /* API version */
#define ARM_CFSTORE_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(0,1)  /* DRV version */

/* CFSTORE Specific error codes*/
#define ARM_CFSTORE_DRIVER_ERROR_UNINITIALISED                                      -1000  /* Start of driver specific errors */
#define ARM_CFSTORE_DRIVER_ERROR_PREEXISTING_KEY                                    -1001
#define ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND                                      -1002
#define ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE                                     -1003
#define ARM_CFSTORE_DRIVER_ERROR_OUT_OF_MEMORY                                      -1004
#define ARM_CFSTORE_DRIVER_ERROR_PREEXISTING_KEY_DELETING                           -1005
#define ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE_BUF                                 -1006
#define ARM_CFSTORE_DRIVER_ERROR_INTERNAL                                           -1007
#define ARM_CFSTORE_DRIVER_ERROR_INVALID_KEY_NAME                                   -1008
#define ARM_CFSTORE_DRIVER_ERROR_VALUE_SIZE_TOO_LARGE                               -1009
#define ARM_CFSTORE_DRIVER_ERROR_KEY_READ_ONLY                                      -1010
#define ARM_CFSTORE_DRIVER_ERROR_INVALID_SEEK                                       -1011
#define ARM_CFSTORE_DRIVER_ERROR_KEY_UNREADABLE                                     -1012
#define ARM_CFSTORE_DRIVER_ERROR_INVALID_WRITE_BUFFER                               -1013
#define ARM_CFSTORE_DRIVER_ERROR_INVALID_KEY_LEN                                    -1014
#define ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED                                      -1015
#define ARM_CFSTORE_DRIVER_ERROR_INVALID_READ_BUFFER                                -1016
#define ARM_CFSTORE_DRIVER_ERROR_INVALID_KEY_DESCRIPTOR                             -1017
#define ARM_CFSTORE_DRIVER_ERROR_PERM_NO_READ_ACCESS                                -1018
#define ARM_CFSTORE_DRIVER_ERROR_PERM_NO_WRITE_ACCESS                               -1019
#define ARM_CFSTORE_DRIVER_ERROR_PERM_NO_EXECUTE_ACCESS                             -1020
#define ARM_CFSTORE_DRIVER_ERROR_NO_PERMISSIONS                                     -1021
#define ARM_CFSTORE_DRIVER_ERROR_HANDLE_COUNT_MAX                                   -1022
#define ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_ERROR                               -1023
#define ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_BUSY                                -1024
#define ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_TIMEOUT                             -1025
#define ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_UNSUPPORTED                         -1026
#define ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_PARAMETER                           -1027
#define ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_BOUNDED_CAPACITY                    -1028
#define ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_STORAGE_API_ERROR                   -1029
#define ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_STORAGE_IO_ERROR                    -1030
#define ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_NOT_INITIALIZED                     -1031
#define ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_ATTEMPTING_COMMIT_WITHOUT_WRITE     -1032
#define ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_EMPTY                               -1033
#define ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_SMALL_LOG_REQUEST                   -1034
#define ARM_CFSTORE_DRIVER_ERROR_OPERATION_PENDING                                  -1035
#define ARM_CFSTORE_DRIVER_ERROR_UVISOR_BOX_ID                                      -1036
#define ARM_CFSTORE_DRIVER_ERROR_UVISOR_NAMESPACE                                   -1037
/// @endcond


/* Type Definitions */
typedef void *ARM_CFSTORE_HANDLE;   //!< opaque cfstore handle for manipulating cfstore data objects e.g. KV pairs.
typedef size_t ARM_CFSTORE_SIZE;    //!< CFSTORE type for size parameters.
typedef size_t ARM_CFSTORE_OFFSET;  //!< CFSTORE type for offset parameters.

/** @brief
 *
 * Status structure returned from the \ref ARM_CFSTORE_DRIVER (*GetStatus)() function.
 */
typedef struct _ARM_CFSTORE_STATUS {
    uint32_t in_progress  : 1;      //!< indicates a previous \ref ARM_CFSTORE_DRIVER ::Dispatch_Method_Xxx()
                                    //!< function is still in progress and will complete sometime in the future.
    uint32_t error : 1;             //!< indicates a previous \ref ARM_CFSTORE_DRIVER ::Dispatch_Method_Xxx()
                                    //!< function is errored and will complete  sometime in the future.
} ARM_CFSTORE_STATUS;

/* Defines */
#define CFSTORE_KEY_NAME_MAX_LENGTH     220         //!< The maximum length of the null terminated character
                                                    //!< string used as a key name string.
#define CFSTORE_VALUE_SIZE_MAX          (1<<26)     //!< Max size of the KV value blob (currently 64MB)
#define CFSTORE_HANDLE_BUFSIZE          24          //!< size of the buffer owned and supplied by client
                                                    //!< to CFSTORE to hold internal data structures, referenced by the key handle.

/** @brief   Helper macro to declare handle and client owned buffer supplied
 *           to CFSTORE for storing opaque handle state
 */
#define ARM_CFSTORE_HANDLE_INIT(__name)                                         \
    uint8_t (__name##_buf_cFsToRe)[CFSTORE_HANDLE_BUFSIZE];                     \
    ARM_CFSTORE_HANDLE (__name) = (ARM_CFSTORE_HANDLE) (__name##_buf_cFsToRe);  \
    memset((__name##_buf_cFsToRe), 0, CFSTORE_HANDLE_BUFSIZE)

#if defined __MBED__ && (defined TOOLCHAIN_GCC_ARM || defined TOOLCHAIN_ARMC6)
/** @brief  Helper macro to swap 2 handles, which is useful for the Find() idiom. */
#define CFSTORE_HANDLE_SWAP(__a_HaNdLe, __b_HaNdLe)         \
    do{ ARM_CFSTORE_HANDLE __temp_HaNdLe = (__a_HaNdLe);    \
        __asm volatile("" ::: "memory");                    \
        (__a_HaNdLe) = (__b_HaNdLe);                        \
        __asm volatile("" ::: "memory");                    \
        (__b_HaNdLe) = (__temp_HaNdLe);                     \
        __asm volatile("" ::: "memory");                    \
    }while(0)

#elif defined __MBED__ && defined TOOLCHAIN_ARM
/** @brief  Helper macro to swap 2 handles, which is useful for the Find() idiom. */
#define CFSTORE_HANDLE_SWAP(__a_HaNdLe, __b_HaNdLe)         \
    do{ ARM_CFSTORE_HANDLE __temp_HaNdLe = (__a_HaNdLe);    \
        __dmb(0xf);                                         \
        (__a_HaNdLe) = (__b_HaNdLe);                        \
        __dmb(0xf);                                         \
        (__b_HaNdLe) = (__temp_HaNdLe);                     \
        __dmb(0xf);                                         \
    }while(0)

#elif defined __MBED__ && defined TOOLCHAIN_IAR
/** @brief  Helper macro to swap 2 handles, which is useful for the Find() idiom. */
/* note, memory barriers may be required in the following implementation */
#define CFSTORE_HANDLE_SWAP(__a_HaNdLe, __b_HaNdLe)         \
    do{ ARM_CFSTORE_HANDLE __temp_HaNdLe = (__a_HaNdLe);    \
        (__a_HaNdLe) = (__b_HaNdLe);                        \
        (__b_HaNdLe) = (__temp_HaNdLe);                     \
    }while(0)
#endif

/** @brief   The access control permissions for the key-value. */
typedef struct ARM_CFSTORE_ACCESS_CONTROL_LIST
{
    uint32_t perm_owner_read : 1;       //!< When set this KV is owner readable
    uint32_t perm_owner_write : 1;      //!< When set this KV is owner writable. The owner should set this bit to be able delete the KV.
    uint32_t perm_owner_execute : 1;    //!< When set this KV is owner executable
    uint32_t perm_other_read : 1;       //!< When set this KV is world readable
    uint32_t perm_other_write : 1;      //!< When set this KV is world writable. If set then a world client can delete this KV.
    uint32_t perm_other_execute : 1;    //!< When set this KV is world executable (currently not supported)
    uint32_t reserved : 26;             //!< reserved for future use.
} ARM_CFSTORE_ACCESS_CONTROL_LIST;


/** @brief
 *
 * File mode bit-field structure for specifying flags for the
 * following operations:
 * - ARM_CFSTORE_DRIVER::(*Create)(), when creating a KV.
 * - ARM_CFSTORE_DRIVER::(*Open)(), when opening a pre-existing KV.
 */
typedef struct ARM_CFSTORE_FMODE
{
    uint32_t continuous : 1;        //!< If set, the key value should be stored in a continuous sequence
                                    //!< of hardware addresses (not implemented).
    uint32_t lazy_flush : 1;        //!< If set then configuration store will defer flushing the KV
                                    //!< changes until an optimal time. e.g. to save energy rather than
                                    //!< performing the operation immediately (not implemented).
    uint32_t flush_on_close : 1;    //!< If set then the key-value should be flushed to the backing store
                                    //!< when the key is closed (not implemented).
    uint32_t read : 1;              //!< If set then the KV can be read.
    uint32_t write : 1;             //!< If set then the KV can be written.
    uint32_t execute : 1;           //!< If set then the KV can be executed (not implemented).
    uint32_t storage_detect : 1;    //!< If set then the call to ARM_CFSTORE_DRIVER::(*Create)() returns
                                    //!< whether a key could be created with the required storage
                                    //!< characteristics. The key is not created.
    uint32_t reserved : 25;         //!< Reserved.
} ARM_CFSTORE_FMODE;


/** @brief   Descriptor used to create keys */
typedef struct ARM_CFSTORE_KEYDESC
{
    /*key descriptor attributes */
    ARM_CFSTORE_ACCESS_CONTROL_LIST acl;        //!< Access Control List specifying the access permissions of the KV.
    uint8_t drl;                                //!< Data retention level for the KV required by the client.
                                                //!< CFSTORE will store the KV in the specified type store/media.
    ARM_STORAGE_SECURITY_FEATURES security;     //!< flash security properties for the KV required by the client.
                                                //!< CFSTORE will store the KV in a storage media supporting
                                                //!< the specified security attributes.
    ARM_CFSTORE_FMODE flags;                    //!< A bitfield containing the access mode setting for the key.
} ARM_CFSTORE_KEYDESC;


/** @brief
 *
 * These are the command code values used for the cmd_code argument to the
 * \ref ARM_CFSTORE_CALLBACK callback handler. The command code indicates the
 * \ref ARM_CFSTORE_DRIVER dispatch method previously invocated.
 */
typedef enum ARM_CFSTORE_OPCODE {
    CFSTORE_OPCODE_CLOSE = 1,       //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*Close)() call.
    CFSTORE_OPCODE_CREATE,          //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*Create)() call.
    CFSTORE_OPCODE_DELETE,          //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*Delete)() call.
    CFSTORE_OPCODE_FIND,            //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*Find)() call.
    CFSTORE_OPCODE_FLUSH,           //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*Flush)() call.
    CFSTORE_OPCODE_GET_KEY_NAME,    //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*GetKeyName)() call.
    CFSTORE_OPCODE_GET_STATUS,      //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*GetStatus)() call.
    CFSTORE_OPCODE_GET_VALUE_LEN,   //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*GetValueLen)() call.
    CFSTORE_OPCODE_INITIALIZE,      //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*Initialize)() call.
    CFSTORE_OPCODE_OPEN,            //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*Open)() call.
    CFSTORE_OPCODE_POWER_CONTROL,   //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*PowerControl)() call.
    CFSTORE_OPCODE_READ,            //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*Read)() call.
    CFSTORE_OPCODE_RSEEK,           //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*Rseek)() call.
    CFSTORE_OPCODE_UNINITIALIZE,    //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*Uninitialize)() call.
    CFSTORE_OPCODE_WRITE,           //!< used for \ref ARM_CFSTORE_CALLBACK ::cmd_code argument when indicating status for a previous \ref ARM_CFSTORE_DRIVER ::(*Write)() call.
    CFSTORE_OPCODE_MAX              //!< Sentinel
} ARM_CFSTORE_OPCODE;


/** @brief   client registered callback type for receiving asynchronous event
 *           notifications.
 *
 * @param   status
 *          In the case that cmd_code is one of ARM_CFSTORE_OPCODE then this
 *          is the asynchronous completion status of the associated
 *          ARM_CFSTORE_DRIVER::Dispatch_Method_Xxx()
 * @param   cmd_code
 *          ARM_CFSTORE_OPCODE for the
 *          ARM_CFSTORE_DRIVER::Dispatch_Method_Xxx() for which this
 *          invocation of the callback is an asynchronous completion event.
 * @param   client_context
 *          The client ARM_CFSTORE_CALLBACK context registered with the
 *          ARM_CFSTORE_DRIVER::(*Initialize)() method.
 * @param   handle
 *          For certain ARM_CFSTORE_DRIVER::Dispatch_Method_Xxx() e.g.
 *              ARM_CFSTORE_DRIVER::(*Create)()
 *              ARM_CFSTORE_DRIVER::(*Open)()
 *              ARM_CFSTORE_DRIVER::(*Find)()
 *          then an open key handle is supplied upon asynchronous completion.
 *          See the documentation of the ARM_CFSTORE_OpCode_e and the
 *          ARM_CFSTORE_DRIVER::Dispatch_Method_Xxx() for further details.
 *
 */
typedef void (*ARM_CFSTORE_CALLBACK)(int32_t status, ARM_CFSTORE_OPCODE cmd_code, void *client_context, ARM_CFSTORE_HANDLE handle);


/** @brief   Find the capabilities of the configuration store. */
typedef struct ARM_CFSTORE_CAPABILITIES
{
    uint32_t asynchronous_ops : 1;          //!< When set then the configuration store dispatch interface is operating in non-blocking (asynchronous) mode.
                                            //!< When unset then the configuration store dispatch interface is operating in blocking (synchronous) mode.
    uint32_t uvisor_support_enabled : 1;    //!< The configuration store is using uvisor security contexts.
} ARM_CFSTORE_CAPABILITIES;


/**
 * This is the set of operations constituting the Configuration Store driver.
 *
 * The functions dispatch methods can operate synchronously or asynchronously
 * depending on the underlying implementation. The client of this interface
 * should not assume one or other mode of operation but use the
 * (*GetCapabilities) method to determine the operational mode and then
 * behave accordingly.
 */
typedef struct _ARM_DRIVER_CFSTORE
{
    /** @brief  Get driver version.
     *
     * The synchronous function GetVersion() returns version information of the
     * driver implementation in ARM_DRIVER_VERSION.
     *    - ARM_DRIVER_VERSION::api
     *        the version of the CMSIS-Driver specification used to implement
     *        this driver.
     *    - ARM_DRIVER_VERSION::drv
     *        the source code version of the actual configuration store driver
     *        implementation.
     *
     * @return    \ref ARM_DRIVER_VERSION, the configuration store driver
     *            version information
     */
    ARM_DRIVER_VERSION (*GetVersion)(void);


    /** @brief  Close the hkey context previously recovered from CFSTORE.
     *
     * @param   hkey
     *          IN: a previously returned handle from (*Create((), (*Open)()
     *          or (*Find)() to close.
     *
     * @return
     *
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation.
     * ARM_CFSTORE_DRIVER::(*Close)() asynchronous completion command code
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param   status
     *          ARM_DRIVER_OK => success, KV deleted, else failure.
     * @param   cmd_code == CFSTORE_OPCODE_CLOSE
     * @param   client context
     *          as registered with ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param   hkey == NULL
     */
    int32_t (*Close)(ARM_CFSTORE_HANDLE hkey);


    /** @brief  Create a key-value int he configuration strore.
     *
     * Once created, the following attributes of a KV cannot be changed:
     *  - the key_name
     *  - the permissions or attributes specifies by the key descriptor kdesc.
     * To change these properties, the key must be deleted and then created
     * again with the new properties.
     *
     * @param   key_name
     *          IN: zero terminated string specifying the key name.
     * @param   value_len
     *          the client specifies the length of the value data item in the
     *          KV being created.
     * @param   kdesc
     *
     * IN: key descriptor, specifying how the details of the key
     * create operations. Note the following cases:
     * - 1) if key_name is not present in the CFSTORE and kdesc is NULL
     *      (i.e. a new KV pair is being created),
     *      then CFSTORE will chose the most appropriate defaults
     *      e.g. CFSTORE will store the KV in nv store if the value size
     *      is large, with no security guarantees just safety.
     * - 2) if key_name is present in the CFSTORE and kdesc is NULL
     *      (i.e. there is a pre-existing KV pair),
     *      then CFSTORE will grow/shrink the value data buffer to
     *      value_len octets. This idiom can be used to increase/decrease
     *      the size of the KV value storage.
     * - 3) If the kdesc->flags.storage_detect == 1 then the function
     *      does not create the key but reports whether the key
     *      with the specified size and storage media attributes
     *      could/could not be created by configuration storage.
     *
     * @param   hkey
     *          IN: pointer to client owned buffer of CFSTORE_HANDLE_BUFSIZE
     *          bytes
     *          OUT: on success, hkey is a valid handle to a KV.
     *
     * @return
     *
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation.
     * ARM_CFSTORE_DRIVER::(*Create)() asynchronous completion command code
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param    status
     *           ARM_DRIVER_OK => success, KV deleted, else failure.
     * @param    cmd_code == CFSTORE_OPCODE_CREATE
     * @param    client context
     *           as registered with ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param    hkey now contains returned handle to newly created key.
     */
    int32_t (*Create)(const char* key_name, ARM_CFSTORE_SIZE value_len, const ARM_CFSTORE_KEYDESC* kdesc, ARM_CFSTORE_HANDLE hkey);


    /** @brief  Delete key-value from configuration store
     *
     * This method is used in the following ways:
     * - (*Open)() to get a handle to the key, (*Delete)() to delete the key,
     *   (*Close)() to close the handle to the key.
     * - (*Find)() to get a handle to the key, (*Delete)() to delete the key,
     *   (*Close)() to close the handle to the key.
     *
     * @param   hkey
     *          IN: the handle of the key to delete.
     *
     * @return
     *
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation.
     * ARM_CFSTORE_DRIVER::(*Delete)() asynchronous completion command code
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param    status
     *           ARM_DRIVER_OK => success, KV deleted, else failure.
     * @param    cmd_code == CFSTORE_OPCODE_DELETE
     * @param    client context
     *           as registered with ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param    hkey, unused.
     *
     */
    int32_t (*Delete)(ARM_CFSTORE_HANDLE hkey);


    /** @brief   find stored keys that match a query string
     *
     *          find a list of pre-existing keys according to a query pattern.
     *          The search pattern can have the following formats
     *           - 'com.arm.mbed.wifi.accesspoint.essid'. Find whether an object
     *              exists that has a key matching
     *              'com.arm.mbed.wifi.accesspoint.essid'
     *           - 'com.arm.mbed.wifi.accesspoint*.essid'. Find all keys that
     *             start with the substring 'com.arm.mbed.wifi.accesspoint' and
     *             end with the substring '.essid'
     *           - 'com.arm.mbed.your-registry-module-name.*'. Find all key_name
     *             strings that begin with the substring
     *             'com.arm.mbed.your-registry-module-name.'
     *           - 'com.arm.mbed.hello-world.animal{dog}{foot}{*}'. Find all key_name
     *             strings beginning com.arm.mbed.hello-world.animal{dog}{foot}
     *           - 'com.arm.mbed.hello-world.animal{dog}{foot}*'
     *           - 'com.arm.mbed.hello-world.animal{dog*3}'
     *
     *          It is possible to iterate over the list of matching keys by
     *          using the idiom below (in the synchronous case):
     * ```
     *          int32_t ret = ARM_DRIVER_ERROR;
     *          const char* key_name_query = "*";
     *          char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
     *          uint8_t len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
     *          ARM_CFSTORE_HANDLE_INIT(next);
     *          ARM_CFSTORE_HANDLE_INIT(prev);
     *
     *          while(drv->Find(key_name_query, prev, next) == ARM_DRIVER_OK)
     *          {
     *              len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
     *              drv->GetKeyName(next, key_name, &len);
     *              printf("Found matching key-value with key_name=%s\n", key_name);
     *              CFSTORE_HANDLE_SWAP(prev, next);
     *          }
     * ```
     *          The iteration over the find results can be terminated before
     *          the end of the list is reached by closing the last open
     *          file handle and not making any further calls to
     *          find() e.g.
     *
     * ```
     *          int32_t ret = ARM_DRIVER_ERROR;
     *          const char* key_name_query = "*";
     *          char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
     *          uint8_t len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
     *          ARM_CFSTORE_HANDLE_INIT(next);
     *          ARM_CFSTORE_HANDLE_INIT(prev);
     *
     *          while(drv->Find(key_name_query, prev, next) == ARM_DRIVER_OK)
     *          {
     *              len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
     *              drv->GetKeyName(next, key_name, &len);
     *              if(strncmp(key_name, "my.key_name", CFSTORE_KEY_NAME_MAX_LENGTH))
     *              {
     *                  printf("Found matching key-value with key_name=%s\n", key_name;
     *                  // terminating walk of find results
     *                  drv->Close(next);
     *                  break;
     *              CFSTORE_HANDLE_SWAP(prev, next);
     *          }
     * ```
     *
     * @param   key_name_query
     *          IN: a search string to find. This can include the wildcard '*'
     *          character
     * @param   previous
     *          IN: On the first call to (*Find) then previous is a pointer
     *          (the handle) to a buffer initialised to 0.
     *          On the subsequent calls to (*Find), a previously returned
     *          key handle can be supplied as the previous argument. The next
     *          available search result will be returned. If no further search
     *          results are available then (*Find) will return
     *          ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND. Once the
     *          file handle has been supplied to the function as the prev
     *          argument, CFSTORE close the open file handle. Otherwise, the
     *          client must call (*Close)() on the open file handle.
     * @param   next
     *          IN: pointer to client owned buffer of CFSTORE_HANDLE_BUFSIZE
     *          bytes to hold the
     *          OUT: Success: In the case that a match has been found then hkey
     *          is a valid handle to an open KV. The key must be explicitly closed
     *          by the client. Note this is a read-only key.
     *          Async operation: The storage at hkey must be available until after
     *          the completion notification has been received by the client.
     * @return
     *          On success (finding a KV matching the query) ARM_DRIVER_OK is
     *          returned. If a KV is not found matching the description then
     *          ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND is returned.
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation.
     * ARM_CFSTORE_DRIVER::(*Find)() asynchronous completion command code
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param    status
     *           ARM_DRIVER_OK => success, hkey contains open key
     *           else failure.
     * @param    cmd_code == CFSTORE_OPCODE_FIND
     * @param    client context
     *           as registered with ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param    hkey
     *           ARM_DRIVER_OK => contains returned handle to newly found key.
     *           else, indeterminate data.
     */
    int32_t (*Find)(const char* key_name_query, const ARM_CFSTORE_HANDLE previous, ARM_CFSTORE_HANDLE next);


    /** @brief
     *
     * Flush (write) the configuration changes to the nv backing store.
     *
     * All open key handles must be closed before flushing the CFSTORE to nv
     * backing store.
     *
     * @return
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation.
     * ARM_CFSTORE_DRIVER::(*Flush)() asynchronous completion command code
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param    status
     *           ARM_DRIVER_OK => success, hkey contains open key
     *           else failure.
     * @param    cmd_code == CFSTORE_OPCODE_FLUSH
     * @param    client context, registered ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param    hkey, unused
     */
    int32_t (*Flush)(void);


    /** @brief  Get configuration store capabilities.
     *
     * This synchronous function returns a ARM_CFSTORE_CAPABILITIES
     * information structure describing configuration store implementation
     * attributes.
     *
     * @return \ref ARM_CFSTORE_CAPABILITIES
     */
    ARM_CFSTORE_CAPABILITIES (*GetCapabilities)(void);


    /** @brief  Get the name of an open key handle.
     *
     * @param   hkey
     *          open handle of key to get the key_name
     * @param   key_name. The key name string is guaranteed to be terminated
     *          with '\0'.
     * @param   key_len, the length of the buffer available at data to receive
     *          the key_name string
     * @return
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation.
     * ARM_CFSTORE_DRIVER::(*GetKeyName)() asynchronous completion command code
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param    status
     *           ARM_DRIVER_OK => success, key_name contains key_name string, len
     *           contains length of string.
     *           else failure.
     * @param    cmd_code == CFSTORE_OPCODE_GET_KEY_NAME
     * @param    client context, registered ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param    hkey supplied to the GetKeyName() call at hkey.
     * @param    key_name
     *           on success, the buffer holds the name of the key
     * @param    key_len
     *           on success, the supplied integer is set to strlen(key_name)+1, where
     *           the additional character corresponds to the terminating null.
     *
     */
    int32_t (*GetKeyName)(ARM_CFSTORE_HANDLE hkey, char* key_name, uint8_t *key_len);


    /** @brief  Get the status of the configuration store.
     *
     * @return
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation
     * ARM_DRIVER_OK if the command has been accepted and
     * enqueued by the underlying controller, else an appropriate
     * error code. Getting a return value of ARM_DRIVER_OK
     * implies that basic argument validation was successful, and the
     * caller can expect a command completion callback to be invoked
     * at a later time.
     *
     * ARM_CFSTORE_DRIVER::(*GetStatus)() asynchronous completion command code
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param    status
     *           status of next command pending completion
     * @param    cmd_code
     *           next command pending completion.
     * @param    client context registered ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param    hkey
     *              unused.
     */
    ARM_CFSTORE_STATUS (*GetStatus)(void);


    /** @brief  Get the value length from an open key handle
     *
     * @param   hkey
     *          open handle of key for which to get value length
     * @param   value_len, the location in which to store the value length.
     * @return
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation.
     * ARM_CFSTORE_DRIVER::(*GetValueLen)() asynchronous completion command code
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param    status
     *           ARM_DRIVER_OK => success, value_len contains length value.
     *           else failure.
     * @param    cmd_code == CFSTORE_OPCODE_GET_VALUE_LEN
     * @param    client context, registered ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param    hkey supplied to the GetValueLen() call at hkey.
     *
     */
    int32_t (*GetValueLen)(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_SIZE *value_len);


    /** @brief  Initialize the Configuration Store
     *
     * This function:
     *    - initialised the configuration store service
     *    - allows the client to subscribe to event notifications, in particular
     *      the asynchronous completion events for the driver dispatch interface
     *      (see ARM_CFSTORE_OPCODE).
     *
     * The configuration store should be initialised as follows:
     *    drv->initialise(client_callback, client_callback_context);
     *    drv->PowerControl(ARM_POWER_FULL)
     * where:
     *    - client_callback is a client implemented callback function.
     *    - client_callback_context is a client registered context that
     *      will be supplied as an argument to the client_callback
     *      when called.
     *    - PowerControl indicates that any underlying system services
     *      that are in the low power state should be powered up.
     *
     * The configuration store should be de-initialised as follows:
     *    drv->PowerControl(ARM_POWER_OFF)
     *    drv->Deinitialise();
     *
     * @param client_callback
     *        this is a client implemented callback function for
     *        receiving asynchronous event notifications (see
     *        ARM_CFSTORE_CALLBACK). NULL indicates client does
     *        not subscribed to event notifications.
     *
     * @param client_callback_context
     *        this is the client registered context that
     *        will be supplied as an argument to the client_callback
     *        when called. This argument can be NULL.
     *
     * @return
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation.
     * ARM_CFSTORE_DRIVER::(*Initialize)() asynchronous completion command code
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param    status
     *           ARM_DRIVER_OK => success, configuration store initialised,
     *           else failure
     * @param    CFSTORE_OPCODE_INITIALIZE
     * @param    client context registered ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param    hkey, unused.
     *
     */
    int32_t (*Initialize)(ARM_CFSTORE_CALLBACK callback, void* client_context);


    /** @brief  Function to set the target configuration store power state.
     *
     * @param state
     *        \ref ARM_POWER_STATE. The target power-state for the
     *        configuration store. The parameter state can have the
     *        following values:
     *          - ARM_POWER_FULL => set the mode to full power state
     *          - ARM_POWER_LOW  => set the mode to low power state
     *          - ARM_POWER_OFF  => set the mode to off power state
     *
     * @return
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation.
     * CFSTORE_OPCODE_POWER_CONTROL
     * ARM_CFSTORE_DRIVER::(*PowerControl)() asynchronous completion command code
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param    status
     *           ARM_DRIVER_OK => success, power control set, else failure.
     * @param    cmd_code == CFSTORE_OPCODE_POWER_CONTROL
     * @param    client context, registered ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param    hkey, unused.
     */
    int32_t (*PowerControl)(ARM_POWER_STATE state);


    /** @brief  Read the value data associated with a specified key.
     *
     * @param   hkey
     *          IN: the handle returned from a previous call to (*Open)() to
     *          get a handle to the key
     * @param   data
     *          IN: a pointer to a data buffer supplied by the caller for CFSTORE to
     *          fill with value data
     *          OUT: on ARM_DRIVER_OK the data is (or will be when asynchronously
     *          completed) available in the buffer. The data will be read from
     *          the current form the current location (see (*Rseek)().
     * @param   len
     *          IN: the client specifies the length of the buffer available at data
     *          OUT: the CFSTORE specifies how many octets have been stored in the
     *          supplied buffer. Note fewer octets may be stored than the input len
     *          depending on the CFSTORE internal representation of the value.
     *
     * @return
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation.
     *          return_value == ARM_DRIVER_OK (==0) => asynchronous
     *          transaction pending.
     *          return_value > 0 => synchronous completion of read with the
     *          number of bytes read == return_value
     *          return_value < 0, error condition.
     *
     * ARM_CFSTORE_DRIVER::(*Read)() asynchronous completion command code
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param    status
     *           >= 0 => success with number of bytes read as indicated by
     *           the value of status
     *           < 0 => error, data in the data buffer is undefined, len is
     *           undefined.
     * @param    cmd_code == CFSTORE_OPCODE_READ
     * @param    client context, registered ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param    hkey, unused.
     */
    int32_t (*Read)(ARM_CFSTORE_HANDLE hkey, void* data, ARM_CFSTORE_SIZE* len);


    /** @brief  Open a key-value object for future operations.
     *
     * @param   key_name
     *          IN: zero terminated string specifying the key name.
     * @param   flags
     *          can open a RW key in read only mode.
     * @param   hkey
     *          IN: pointer to client owned buffer of CFSTORE_HANDLE_BUFSIZE
     *          bytes
     *          OUT: on success, hkey is a valid handle to an open KV.
     *
     * @return
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation.
     * ARM_CFSTORE_DRIVER::(*Open)() asynchronous completion command code
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param    status
     *           ARM_DRIVER_OK => success, KV opened, else failure.
     * @param    cmd_code == CFSTORE_OPCODE_OPEN
     * @param    client context, registered ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param    hkey now contains returned handle to newly opened key.
     */
    int32_t (*Open)(const char* key_name, ARM_CFSTORE_FMODE flags, ARM_CFSTORE_HANDLE hkey);


    /** @brief  Move the position of the read pointer within a value
     *
     * @param   hkey
     *          IN: the key referencing the value data for which the read
     *          location should be updated. Note that this function can
     *          only be called on pre-existing keys opened with read-only
     *          flag. Performing a seek operation on a writable key will fail.
     * @param   offset
     *          IN: the new offset position from the start of the value data
     *
     * @return
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation.
     * ARM_CFSTORE_DRIVER::(*Rseek)() asynchronous completion command code
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param    status
     *           >=0 => success, read location set, else failure.
     *           upon success, the function returns the current read
     *           location measured from the beginning of the data value.
     *           <0 => error
     * @param    cmd_code == CFSTORE_OPCODE_RSEEK
     * @param    client context, registered ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param    hkey, unused.
     */
    int32_t (*Rseek)(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_OFFSET offset);


    /** @brief  Function to de-initialise the Configuration Store
     *
     * @return
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation.
     * CFSTORE_OPCODE_UNINITIALIZE
     * ARM_CFSTORE_DRIVER::(*Uninitialize)() asynchronous completion command code
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param    status
     *           ARM_DRIVER_OK => success, read location set, else failure.
     * @param    cmd_code == CFSTORE_OPCODE_UNINITIALIZE
     * @param    client context, registered ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param    hkey, unused.
     */
    int32_t (*Uninitialize)(void);


    /** @brief  Write the value data associated with a specified key
     *
     * @note    Note that Write() only supports sequential-access.
     *
     * @param   hkey
     *          IN: the key for which value data will be written
     * @param   data
     *          IN: a pointer to a data buffer supplied by the caller for CFSTORE to
     *          write as the binary blob value data.
     *          Async operation: the buffer must be available until after
     *          completion notification is received by the client.
     * @param   len
     *          IN: the client specifies the length of the data buffer available.
     *          len must not exceed the value_len field specified when the
     *          KV pair was created.
     *          OUT: the CFSTORE specifies how many octets have been stored.
     *          Note that fewer octets may be stored than the input len
     *          depending on the CFSTORE internal representation of the value.
     *          Async operation: the len storage location must be available
     *          until after completion notification is received by the client.
     *
     * @return
     * See REFERENCE_1 and the ARM_CFSTORE_CALLBACK documentation.
     *          return_value == ARM_DRIVER_OK (==0) => asynchronous
     *          transaction pending.
     *          return_value > 0 => synchronous completion of write with the
     *          number of bytes written == return_value
     *          return_value < 0, error condition.
     *
     * ARM_CFSTORE_DRIVER::(*Write)() asynchronous completion
     * (*ARM_CFSTORE_CALLBACK) function argument values on return:
     * @param    status
     *           >= 0 => success with the number bytes written equal to the value
     *           of the return status
     *           ARM_CFSTORE_CALLBACK status argument < 0 => error
     * @param    cmd_code == CFSTORE_OPCODE_WRITE
     * @param    client context, registered ARM_CFSTORE_DRIVER::(*Initialize)()
     * @param    hkey, unused.
     */
    int32_t (*Write)(ARM_CFSTORE_HANDLE hkey, const char* data, ARM_CFSTORE_SIZE* len);

} const ARM_CFSTORE_DRIVER;



MBED_DEPRECATED_SINCE("mbed-os-5.5", "CFSTORE replace by FATFileSystem.")
extern ARM_CFSTORE_DRIVER cfstore_driver;

#ifdef __cplusplus
}
#endif

#endif /* __CONFIGURATION_STORE_H */
