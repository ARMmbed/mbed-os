/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_error Error functions
 * @{
 */
/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
 */
#ifndef MBED_ERROR_H
#define MBED_ERROR_H

#include "platform/mbed_retarget.h"
#include "platform/mbed_toolchain.h"

#ifdef __cplusplus
extern "C" {
#endif

//Define this macro to include filenames in error context, this can save memory. For release builds, do not include filename    
//#define MBED_CONF_ERROR_FILENAME_CAPTURE_ENABLED 1    
    
//Define this macro to disable error logging, note that the first and last error capture will still be active by default
//#define MBED_CONF_ERROR_LOG_DISABLED    1

#ifndef MBED_CONF_MAX_ERROR_FILENAME_LEN
#define MBED_CONF_MAX_ERROR_FILENAME_LEN            16
#endif    
    
#define MBED_ERROR_STATUS_CODE_MASK                 (0x0000FFFF)
#define MBED_ERROR_STATUS_CODE_POS                  (0)
#define MBED_ERROR_STATUS_CODE_FIELD_SIZE           (16)

#define MBED_ERROR_STATUS_ENTITY_MASK               (0x00FF0000)
#define MBED_ERROR_STATUS_ENTITY_POS                (16)
#define MBED_ERROR_STATUS_ENTITY_FIELD_SIZE         (8)

#define MBED_ERROR_STATUS_TYPE_MASK                 (0x60000000)
#define MBED_ERROR_STATUS_TYPE_POS                  (29)
#define MBED_ERROR_STATUS_TYPE_FIELD_SIZE           (2)

/* MbedErrorStatus Status Encoding */
//|31(1 bit) Always Negative|30-29(2 bits)  |28-24	            | 23-16(8 bits)	|  15-0(16 bits) |
//|-1	                    |TYPE	        |(unused/reserved)	| ENTITY TYPE	|  ERROR CODE    |

#define MAKE_MBED_ERROR(type, entity, error_code)   (MbedErrorStatus)                                                                   \
                                                    ((0x80000000) |                                                                     \
                                                    (MBED_ERROR_STATUS_CODE_MASK & (error_code << MBED_ERROR_STATUS_CODE_POS)) |        \
                                                    (MBED_ERROR_STATUS_ENTITY_MASK & (entity << MBED_ERROR_STATUS_ENTITY_POS)) |        \
                                                    (MBED_ERROR_STATUS_TYPE_MASK & (type << MBED_ERROR_STATUS_TYPE_POS)))    

#define GET_MBED_ERROR_TYPE( error_status )         ((error_status & MBED_ERROR_STATUS_TYPE_MASK) >> MBED_ERROR_STATUS_TYPE_POS)
#define GET_MBED_ERROR_ENTITY( error_status )       ((error_status & MBED_ERROR_STATUS_ENTITY_MASK) >> MBED_ERROR_STATUS_ENTITY_POS)    
#define GET_MBED_ERROR_CODE( error_status )         (int)((GET_MBED_ERROR_TYPE( error_status ) == ERROR_TYPE_POSIX)?(-error_status):((error_status & MBED_ERROR_STATUS_CODE_MASK) >> MBED_ERROR_STATUS_CODE_POS))

/** MbedErrorStatus description
 *
 * MbedErrorStatus type represents the error status values under MbedOS. MbedErrorStatus values are signed integers and always be negative.\n
 * Internally its encoded as below with bit-fields representing error type, entity and error code:\n\n
 * MbedErrorStatus Status Encoding:\n
 *
 \verbatim
 | 31 Always Negative | 30-29(2 bits)  | 28-24              | 23-16(8 bits)	| 15-0(16 bits) |
 | -1                 | TYPE           | (unused/reserved)  | ENTITY TYPE    | ERROR CODE    |
 \endverbatim
 * 
 * The error status value range for each error type is as follows:\n
 *   Posix Error Status-es  - 0xFFFFFFFF to 0xFFFFFF01(-1 -255) - This corresponds to Posix error codes represented as negative.\n
 *   System Error Status-es - 0x80XX0100 to 0x80XX0FFF - This corresponds to System error codes range(all values are negative). Bits 23-16 will be entity type(marked with XX)\n
 *   Custom Error Status-es - 0xA0XX1000 to 0xA0XXFFFF - This corresponds to Custom error codes range(all values are negative). Bits 23-16 will be entity type(marked with XX)\n\n
 *
 * The ERROR CODE(values encoded into ERROR CODE bit-field in MbedErrorStatus) value range for each error type is also seperated as below:\n
 *   Posix Error Codes  - 1 to 255.\n
 *   System Error Codes - 256 to 4095.\n
 *   Custom Error Codes - 4096 to 65535.\n
 *
 * @note Posix error codes are always encoded as negative of their actual value. For example, EPERM is encoded as -EPERM.
 *       And, the ENTITY TYPE for Posix error codes are always encoded as ENTITY_UNKNOWN.\n
 *       This is to enable easy injection of Posix error codes into MbedOS error handling system without altering the actual Posix error values.\n
 *       Accordingly, Posix error codes are represented as -1 to -255 under MbedOS error status representation.
 */
typedef int MbedErrorStatus;

/**
 * Macro for defining a Posix error status. This macro is mainly used to define Posix error values in MbedErrorCode enumeration.
 * @param  error_name       Name of the error without the ERROR_ prefix
 * @param  error_code       Error code value to be used, must be between 1 and 255(inclusive).
 *
 */
#define DEFINE_POSIX_ERROR( error_name, error_code )   \
                     ERROR_CODE_##error_name = error_code,  \
                     ERROR_##error_name = -(MBED_POSIX_ERROR_BASE + error_code)

/**
 * Macro for defining a System error status. This macro is used to define System error values in MbedErrorCode enumeration.
 * @param  error_name       Name of the error without the ERROR_ prefix
 * @param  error_code       Error code value to be used, must be between 256 and 4096(inclusive).
 *
 */
#define DEFINE_SYSTEM_ERROR( error_name, error_code )   \
                      ERROR_CODE_##error_name = MBED_SYSTEM_ERROR_BASE + error_code,  \
                      ERROR_##error_name = MAKE_MBED_ERROR(ERROR_TYPE_SYSTEM, ENTITY_UNKNOWN, ERROR_CODE_##error_name)

/**
 * Macro for defining a Custom error status. This macro is used to define custom error values in MbedErrorCode enumeration.
 * @param  error_name       Name of the error without the ERROR_ prefix
 * @param  error_code       Error code value to be used, must be between 4097 and 65535(inclusive).
 *
 */
#define DEFINE_CUSTOM_ERROR( error_name, error_code )   \
                      ERROR_CODE_##error_name = MBED_CUSTOM_ERROR_BASE + error_code,  \
                      ERROR_##error_name = MAKE_MBED_ERROR(ERROR_TYPE_CUSTOM, ENTITY_UNKNOWN, ERROR_CODE_##error_name)


/**
 * Macro for setting a system error. This macro will log the error, prints the error report and return to the caller. Its a wrapper for calling set_error API.
 * @param  error_status     MbedErrorStatus status to be set(See MbedErrorStatus enum above for available error status values).
 * @param  error_msg        The error message to be printed out to STDIO/Serial.
 * @param  error_value      Value associated with the error status. This would depend on error code/error scenario.
 *
 * @code
 * 
 * SET_ERROR( ERROR_INVALID_SIZE, "MyDriver: Invalid size in read", size_val )
 *
 * @endcode
 * @note The macro calls set_error API with filename and line number info without caller explicitly passing them.
 *        Since this macro is a wrapper for set_error API callers should process the return value from this macro which is the return value from calling set_error API.
 *
 */
#ifdef MBED_CONF_ERROR_FILENAME_CAPTURE_ENABLED
    #define SET_ERROR( error_status, error_msg, error_value )       set_error( error_status, (const char *)error_msg, (uint32_t)error_value, (const char *)MBED_FILENAME, __LINE__ )
#else
    #define SET_ERROR( error_status, error_msg, error_value )       set_error( error_status, (const char *)error_msg, (uint32_t)error_value, NULL, 0 )
#endif

/**
 * Macro for setting a fatal system error. This macro will log the error, prints the error report and halts the system. Its a wrapper for calling set_error_fatal API
 * @param  error_status     MbedErrorStatus status to be set(See MbedErrorStatus enum above for available error status values).
 * @param  error_msg        The error message to be printed out to STDIO/Serial.
 * @param  error_value      Value associated with the error status. This would depend on error code/error scenario.
 * @return                  0 or ERROR_SUCCESS.
 *                          ERROR_INVALID_ARGUMENT if called with invalid error status/codes
 *
 * @code
 * 
 * SET_ERROR_FATAL( ERROR_MUTEX_LOCK_FAILED, "MyDriver: Can't lock driver Mutex", &my_mutex )
 *
 * @endcode
 * @note The macro calls set_error_fatal API with filename and line number info without caller explicitly passing them.
*        Since this macro is a wrapper for set_error_fatal API callers should process the return value from this macro which is the return value from calling set_error_fatal API. 
 *
 */
#define SET_ERROR_FATAL( error_status, error_msg, error_value ) set_error_fatal( error_status, (const char *)error_msg, (uint32_t)error_value, (const char *)MBED_FILENAME, __LINE__ )           

//Error Type definition
/** MbedErrorType definition
 *  @note
 *  This enumeration defines the Error types supported. The value of these enum values will be encoded into MbedErrorStatus TYPE field.\n
 *      See MbedErrorStatus description for more info.\n
 *         ERROR_TYPE_SYSTEM - Used to indicate that the error status is of System defined Error type.\n
 *         ERROR_TYPE_CUSTOM - Used to indicate that the error status is of Custom defined Error type.\n 
 *         ERROR_TYPE_POSIX  - Used to indicate that the error status is of Posix error type.\n 
 *
 */
typedef enum _MbedErrorType
{
    ERROR_TYPE_SYSTEM = 0,
    ERROR_TYPE_CUSTOM = 1,
    //2 is reserved
    //Use 3 for POSIX because we are mapping -1 to -255 to POSIX error codes
    //and thus we must use 3 to match the type bits in error status representation which are from 0xFFFFFFFF to 0xFFFFFF00
    ERROR_TYPE_POSIX = 3
} MbedErrorType;		

//Entity type/id definitions
/** MbedEntityType definition
 *  @note
 *  This enumeration defines the Entity types. The value of these enum values will be encoded into MbedErrorStatus ENTITY field.\n\n
 *      See MbedErrorStatus description for more info.\n
 *          ENTITY_UNKNOWN - This entity type can be used if caller of the set_error/set_error_fatal doesn't know who is the actual originator of the error.\n
 *                       Other entity values can be used to provide more info on who/where the error originated from.\n\n
 *                       For example, if I2C driver is the component originating the error you can use ENTITY_DRIVER_I2C to provide more info.\n
 *                       Its used in call to MAKE_ERROR/MAKE_SYSTEM_ERROR/MAKE_CUSTOM_ERROR macros.\n
 * @code
 *      MbedErrorStatus i2c_driver_error = MAKE_ERROR( ENTITY_DRIVER_I2C, ERROR_CONFIG_UNSUPPORTED );
 * @endcode
 */
typedef enum _MbedEntityType
{
	ENTITY_APPLICATION = 0,
	ENTITY_PLATFORM,
	ENTITY_KERNEL,
	ENTITY_NETWORK_STACK,
	ENTITY_HAL,
	ENTITY_MEMORY_SUBSYSTEM,
    ENTITY_FILESYSTEM,
    ENTITY_BLOCK_DEVICE,
	ENTITY_DRIVER,
	ENTITY_DRIVER_SERIAL,
	ENTITY_DRIVER_RTC,
	ENTITY_DRIVER_I2C,
	ENTITY_DRIVER_SPI,
	ENTITY_DRIVER_GPIO,
	ENTITY_DRIVER_ANALOG,
	ENTITY_DRIVER_DIGITAL,
	ENTITY_DRIVER_CAN,
    ENTITY_DRIVER_ETHERNET,
	ENTITY_DRIVER_CRC,
	ENTITY_DRIVER_PWM,
	ENTITY_DRIVER_QSPI,
	ENTITY_DRIVER_USB,
    ENTITY_TARGET_SDK,
    /* Add More entities here as required */
    
    ENTITY_UNKNOWN = 255,
    ENTITY_MAX = ENTITY_UNKNOWN
} MbedEntityType;

//Use ERROR_SUCCESS(=0) or any postive number for successful returns
#define ERROR_SUCCESS           0

#define MBED_POSIX_ERROR_BASE   0
#define MBED_SYSTEM_ERROR_BASE  256
#define MBED_CUSTOM_ERROR_BASE  4096

//Error Code definitions
/** MbedErrorCode definition
 *
 *  MbedErrorCode enumeration defines the Error codes and Error status values for ENTITY_UNKNOWN.\n
 *  It defines all of Posix Error Codes/Statuses and Mbed System Error Codes/Statuses.\n\n
 *
 *  @note
 *  Posix Error codes are defined using the macro DEFINE_POSIX_ERROR\n
 *  For example DEFINE_POSIX_ERROR( EPERM, EPERM ). This effectively defines the following values:\n
 *      ERROR_CODE_EPERM = EPERM\n
 *      ERROR_EPERM = -EPERM\n
 *
 *  Posix Error codes are defined using the macro DEFINE_POSIX_ERROR\n
 *  For example DEFINE_POSIX_ERROR( EPERM, EPERM ). This macro defines the following values:\n
 *      ERROR_CODE_EPERM = MBED_POSIX_ERROR_BASE+EPERM\n
 *      ERROR_EPERM = -(MBED_POSIX_ERROR_BASE+EPERM)\n
 *  Its effectively equivalent to:\n
 *      ERROR_CODE_EPERM = 1\n
 *      ERROR_EPERM = -1\n 
 *  All Posix error codes currently supported by MbedOS(defined in mbed_retarget.h) are defined using the DEFINE_POSIX_ERROR macro.\n\n
 *
 *  MbedOS System Error codes are defined using the macro DEFINE_SYSTEM_ERROR\n
 *  For example DEFINE_SYSTEM_ERROR( INVALID_ARGUMENT ,1 ) macro defines the following values:\n
 *      ERROR_CODE_INVALID_ARGUMENT = MBED_SYSTEM_ERROR_BASE+1\n
 *      ERROR_INVALID_ARGUMENT = MAKE_MBED_ERROR(ERROR_TYPE_SYSTEM, ENTITY_UNKNOWN, ERROR_CODE_INVALID_ARGUMENT)\n
 *  Its effectively equivalent to:\n
 *      ERROR_CODE_INVALID_ARGUMENT = 1\n
 *      ERROR_INVALID_ARGUMENT = 0x80FF0001\n (Note that ENTITY field is set to ENTITY_UNKNOWN) 
 *  New System Error codes should be defined using DEFINE_SYSTEM_ERROR macro and must have an unique error code value\n
 *  passed as the second argument in the DEFINE_SYSTEM_ERROR macro.\n\n
 *
 *  Custom Error codes can be defined using the macro DEFINE_CUSTOM_ERROR\n
 *  This is mainly meant to capture non-generic error codes specific to a device.
 *  For example DEFINE_CUSTOM_ERROR( MY_CUSTOM_ERROR ,1 ) macro defines the following values:\n
 *      ERROR_CODE_MY_CUSTOM_ERROR = MBED_CUSTOM_ERROR_BASE+1\n
 *      ERROR_MY_CUSTOM_ERROR = MAKE_MBED_ERROR(ERROR_TYPE_CUSTOM, ENTITY_UNKNOWN, ERROR_CODE_MY_CUSTOM_ERROR)\n
 *  Its effectively equivalent to:\n
 *      ERROR_CODE_MY_CUSTOM_ERROR = 4097\n
 *      ERROR_MY_CUSTOM_ERROR = 0xA0FF1001\n (Note that ENTITY field is set to ENTITY_UNKNOWN) 
 *
 */
 
typedef enum _MbedErrorCode
{
    //Below are POSIX ERROR CODE definitions, which starts at MBED_POSIX_ERROR_BASE(=0)
    //POSIX ERROR CODE definitions starts at offset 0(MBED_POSIX_ERROR_BASE) to align them with actual Posix Error Code
    //defintions in mbed_retarget.h
    //                  Error Name             Error Code  
    DEFINE_POSIX_ERROR( EPERM                 ,EPERM           ),  /* 1       Operation not permitted */
    DEFINE_POSIX_ERROR( ENOENT                ,ENOENT          ),  /* 2       No such file or directory */
    DEFINE_POSIX_ERROR( ESRCH                 ,ESRCH           ),  /* 3       No such process */
    DEFINE_POSIX_ERROR( EINTR                 ,EINTR           ),  /* 4       Interrupted system call */
    DEFINE_POSIX_ERROR( EIO                   ,EIO             ),  /* 5       I/O error */
    DEFINE_POSIX_ERROR( ENXIO                 ,ENXIO           ),  /* 6       No such device or address */
    DEFINE_POSIX_ERROR( E2BIG                 ,E2BIG           ),  /* 7       Argument list too long */
    DEFINE_POSIX_ERROR( ENOEXEC               ,ENOEXEC         ),  /* 8       Exec format error */
    DEFINE_POSIX_ERROR( EBADF                 ,EBADF           ),  /* 9       Bad file number */
    DEFINE_POSIX_ERROR( ECHILD                ,ECHILD          ),  /* 10      No child processes */
    DEFINE_POSIX_ERROR( EAGAIN                ,EAGAIN          ),  /* 11      Try again */
    DEFINE_POSIX_ERROR( ENOMEM                ,ENOMEM          ),  /* 12      Out of memory */
    DEFINE_POSIX_ERROR( EACCES                ,EACCES          ),  /* 13      Permission denied */
    DEFINE_POSIX_ERROR( EFAULT                ,EFAULT          ),  /* 14      Bad address */
    DEFINE_POSIX_ERROR( ENOTBLK               ,ENOTBLK         ),  /* 15      Block device required */
    DEFINE_POSIX_ERROR( EBUSY                 ,EBUSY           ),  /* 16      Device or resource busy */
    DEFINE_POSIX_ERROR( EEXIST                ,EEXIST          ),  /* 17      File exists */
    DEFINE_POSIX_ERROR( EXDEV                 ,EXDEV           ),  /* 18      Cross-device link */
    DEFINE_POSIX_ERROR( ENODEV                ,ENODEV          ),  /* 19      No such device */
    DEFINE_POSIX_ERROR( ENOTDIR               ,ENOTDIR         ),  /* 20      Not a directory */
    DEFINE_POSIX_ERROR( EISDIR                ,EISDIR          ),  /* 21      Is a directory */
    DEFINE_POSIX_ERROR( EINVAL                ,EINVAL          ),  /* 22      Invalid argument */
    DEFINE_POSIX_ERROR( ENFILE                ,ENFILE          ),  /* 23      File table overflow */
    DEFINE_POSIX_ERROR( EMFILE                ,EMFILE          ),  /* 24      Too many open files */
    DEFINE_POSIX_ERROR( ENOTTY                ,ENOTTY          ),  /* 25      Not a typewriter */
    DEFINE_POSIX_ERROR( ETXTBSY               ,ETXTBSY         ),  /* 26      Text file busy */
    DEFINE_POSIX_ERROR( EFBIG                 ,EFBIG           ),  /* 27      File too large */
    DEFINE_POSIX_ERROR( ENOSPC                ,ENOSPC          ),  /* 28      No space left on device */
    DEFINE_POSIX_ERROR( ESPIPE                ,ESPIPE          ),  /* 29      Illegal seek */
    DEFINE_POSIX_ERROR( EROFS                 ,EROFS           ),  /* 30      Read-only file system */
    DEFINE_POSIX_ERROR( EMLINK                ,EMLINK          ),  /* 31      Too many links */
    DEFINE_POSIX_ERROR( EPIPE                 ,EPIPE           ),  /* 32      Broken pipe */
    DEFINE_POSIX_ERROR( EDOM                  ,EDOM            ),  /* 33      Math argument out of domain of func */
    DEFINE_POSIX_ERROR( ERANGE                ,ERANGE          ),  /* 34      Math result not representable */
    DEFINE_POSIX_ERROR( EDEADLK               ,EDEADLK         ),  /* 35      Resource deadlock would occur */
    DEFINE_POSIX_ERROR( ENAMETOOLONG          ,ENAMETOOLONG    ),  /* 36      File name too long */
    DEFINE_POSIX_ERROR( ENOLCK                ,ENOLCK          ),  /* 37      No record locks available */
    DEFINE_POSIX_ERROR( ENOSYS                ,ENOSYS          ),  /* 38      Function not implemented */
    DEFINE_POSIX_ERROR( ENOTEMPTY             ,ENOTEMPTY       ),  /* 39      Directory not empty */
    DEFINE_POSIX_ERROR( ELOOP                 ,ELOOP           ),  /* 40      Too many symbolic links encountered */
    DEFINE_POSIX_ERROR( EWOULDBLOCK           ,EAGAIN          ),  /* EAGAIN  Operation would block */
    DEFINE_POSIX_ERROR( ENOMSG                ,ENOMSG          ),  /* 42      No message of desired type */
    DEFINE_POSIX_ERROR( EIDRM                 ,EIDRM           ),  /* 43      Identifier removed */
    DEFINE_POSIX_ERROR( ECHRNG                ,ECHRNG          ),  /* 44      Channel number out of range */
    DEFINE_POSIX_ERROR( EL2NSYNC              ,EL2NSYNC        ),  /* 45      Level 2 not synchronized */
    DEFINE_POSIX_ERROR( EL3HLT                ,EL3HLT          ),  /* 46      Level 3 halted */
    DEFINE_POSIX_ERROR( EL3RST                ,EL3RST          ),  /* 47      Level 3 reset */
    DEFINE_POSIX_ERROR( ELNRNG                ,ELNRNG          ),  /* 48      Link number out of range */
    DEFINE_POSIX_ERROR( EUNATCH               ,EUNATCH         ),  /* 49      Protocol driver not attached */
    DEFINE_POSIX_ERROR( ENOCSI                ,ENOCSI          ),  /* 50      No CSI structure available */
    DEFINE_POSIX_ERROR( EL2HLT                ,EL2HLT          ),  /* 51      Level 2 halted */
    DEFINE_POSIX_ERROR( EBADE                 ,EBADE           ),  /* 52      Invalid exchange */
    DEFINE_POSIX_ERROR( EBADR                 ,EBADR           ),  /* 53      Invalid request descriptor */
    DEFINE_POSIX_ERROR( EXFULL                ,EXFULL          ),  /* 54      Exchange full */
    DEFINE_POSIX_ERROR( ENOANO                ,ENOANO          ),  /* 55      No anode */
    DEFINE_POSIX_ERROR( EBADRQC               ,EBADRQC         ),  /* 56      Invalid request code */
    DEFINE_POSIX_ERROR( EBADSLT               ,EBADSLT         ),  /* 57      Invalid slot */
    DEFINE_POSIX_ERROR( EDEADLOCK             ,EDEADLK         ),  /* EDEADLK Resource deadlock would occur */
    DEFINE_POSIX_ERROR( EBFONT                ,EBFONT          ),  /* 59      Bad font file format */
    DEFINE_POSIX_ERROR( ENOSTR                ,ENOSTR          ),  /* 60      Device not a stream */
    DEFINE_POSIX_ERROR( ENODATA               ,ENODATA         ),  /* 61      No data available */
    DEFINE_POSIX_ERROR( ETIME                 ,ETIME           ),  /* 62      Timer expired */
    DEFINE_POSIX_ERROR( ENOSR                 ,ENOSR           ),  /* 63      Out of streams resources */
    DEFINE_POSIX_ERROR( ENONET                ,ENONET          ),  /* 64      Machine is not on the network */
    DEFINE_POSIX_ERROR( ENOPKG                ,ENOPKG          ),  /* 65      Package not installed */
    DEFINE_POSIX_ERROR( EREMOTE               ,EREMOTE         ),  /* 66      Object is remote */
    DEFINE_POSIX_ERROR( ENOLINK               ,ENOLINK         ),  /* 67      Link has been severed */
    DEFINE_POSIX_ERROR( EADV                  ,EADV            ),  /* 68      Advertise error */
    DEFINE_POSIX_ERROR( ESRMNT                ,ESRMNT          ),  /* 69      Srmount error */
    DEFINE_POSIX_ERROR( ECOMM                 ,ECOMM           ),  /* 70      Communication error on send */
    DEFINE_POSIX_ERROR( EPROTO                ,EPROTO          ),  /* 71      Protocol error */
    DEFINE_POSIX_ERROR( EMULTIHOP             ,EMULTIHOP       ),  /* 72      Multihop attempted */
    DEFINE_POSIX_ERROR( EDOTDOT               ,EDOTDOT         ),  /* 73      RFS specific error */
    DEFINE_POSIX_ERROR( EBADMSG               ,EBADMSG         ),  /* 74      Not a data message */
    DEFINE_POSIX_ERROR( EOVERFLOW             ,EOVERFLOW       ),  /* 75      Value too large for defined data type */
    DEFINE_POSIX_ERROR( ENOTUNIQ              ,ENOTUNIQ        ),  /* 76      Name not unique on network */
    DEFINE_POSIX_ERROR( EBADFD                ,EBADFD          ),  /* 77      File descriptor in bad state */
    DEFINE_POSIX_ERROR( EREMCHG               ,EREMCHG         ),  /* 78      Remote address changed */
    DEFINE_POSIX_ERROR( ELIBACC               ,ELIBACC         ),  /* 79      Can not access a needed shared library */
    DEFINE_POSIX_ERROR( ELIBBAD               ,ELIBBAD         ),  /* 80      Accessing a corrupted shared library */
    DEFINE_POSIX_ERROR( ELIBSCN               ,ELIBSCN         ),  /* 81      .lib section in a.out corrupted */
    DEFINE_POSIX_ERROR( ELIBMAX               ,ELIBMAX         ),  /* 82      Attempting to link in too many shared libraries */
    DEFINE_POSIX_ERROR( ELIBEXEC              ,ELIBEXEC        ),  /* 83      Cannot exec a shared library directly */
    DEFINE_POSIX_ERROR( EILSEQ                ,EILSEQ          ),  /* 84      Illegal byte sequence */
    DEFINE_POSIX_ERROR( ERESTART              ,ERESTART        ),  /* 85      Interrupted system call should be restarted */
    DEFINE_POSIX_ERROR( ESTRPIPE              ,ESTRPIPE        ),  /* 86      Streams pipe error */
    DEFINE_POSIX_ERROR( EUSERS                ,EUSERS          ),  /* 87      Too many users */
    DEFINE_POSIX_ERROR( ENOTSOCK              ,ENOTSOCK        ),  /* 88      Socket operation on non-socket */
    DEFINE_POSIX_ERROR( EDESTADDRREQ          ,EDESTADDRREQ    ),  /* 89      Destination address required */
    DEFINE_POSIX_ERROR( EMSGSIZE              ,EMSGSIZE        ),  /* 90      Message too long */
    DEFINE_POSIX_ERROR( EPROTOTYPE            ,EPROTOTYPE      ),  /* 91      Protocol wrong type for socket */
    DEFINE_POSIX_ERROR( ENOPROTOOPT           ,ENOPROTOOPT     ),  /* 92      Protocol not available */
    DEFINE_POSIX_ERROR( EPROTONOSUPPORT       ,EPROTONOSUPPORT ),  /* 93      Protocol not supported */
    DEFINE_POSIX_ERROR( ESOCKTNOSUPPORT       ,ESOCKTNOSUPPORT ),  /* 94      Socket type not supported */
    DEFINE_POSIX_ERROR( EOPNOTSUPP            ,EOPNOTSUPP      ),  /* 95      Operation not supported on transport endpoint */
    DEFINE_POSIX_ERROR( EPFNOSUPPORT          ,EPFNOSUPPORT    ),  /* 96      Protocol family not supported */
    DEFINE_POSIX_ERROR( EAFNOSUPPORT          ,EAFNOSUPPORT    ),  /* 97      Address family not supported by protocol */
    DEFINE_POSIX_ERROR( EADDRINUSE            ,EADDRINUSE      ),  /* 98      Address already in use */
    DEFINE_POSIX_ERROR( EADDRNOTAVAIL         ,EADDRNOTAVAIL   ),  /* 99      Cannot assign requested address */
    DEFINE_POSIX_ERROR( ENETDOWN              ,ENETDOWN        ),  /* 100     Network is down */
    DEFINE_POSIX_ERROR( ENETUNREACH           ,ENETUNREACH     ),  /* 101     Network is unreachable */
    DEFINE_POSIX_ERROR( ENETRESET             ,ENETRESET       ),  /* 102     Network dropped connection because of reset */
    DEFINE_POSIX_ERROR( ECONNABORTED          ,ECONNABORTED    ),  /* 103     Software caused connection abort */
    DEFINE_POSIX_ERROR( ECONNRESET            ,ECONNRESET      ),  /* 104     Connection reset by peer */
    DEFINE_POSIX_ERROR( ENOBUFS               ,ENOBUFS         ),  /* 105     No buffer space available */
    DEFINE_POSIX_ERROR( EISCONN               ,EISCONN         ),  /* 106     Transport endpoint is already connected */
    DEFINE_POSIX_ERROR( ENOTCONN              ,ENOTCONN        ),  /* 107     Transport endpoint is not connected */
    DEFINE_POSIX_ERROR( ESHUTDOWN             ,ESHUTDOWN       ),  /* 108     Cannot send after transport endpoint shutdown */
    DEFINE_POSIX_ERROR( ETOOMANYREFS          ,ETOOMANYREFS    ),  /* 109     Too many references: cannot splice */
    DEFINE_POSIX_ERROR( ETIMEDOUT             ,ETIMEDOUT       ),  /* 110     Connection timed out */
    DEFINE_POSIX_ERROR( ECONNREFUSED          ,ECONNREFUSED    ),  /* 111     Connection refused */
    DEFINE_POSIX_ERROR( EHOSTDOWN             ,EHOSTDOWN       ),  /* 112     Host is down */
    DEFINE_POSIX_ERROR( EHOSTUNREACH          ,EHOSTUNREACH    ),  /* 113     No route to host */
    DEFINE_POSIX_ERROR( EALREADY              ,EALREADY        ),  /* 114     Operation already in progress */
    DEFINE_POSIX_ERROR( EINPROGRESS           ,EINPROGRESS     ),  /* 115     Operation now in progress */
    DEFINE_POSIX_ERROR( ESTALE                ,ESTALE          ),  /* 116     Stale NFS file handle */
    DEFINE_POSIX_ERROR( EUCLEAN               ,EUCLEAN         ),  /* 117     Structure needs cleaning */
    DEFINE_POSIX_ERROR( ENOTNAM               ,ENOTNAM         ),  /* 118     Not a XENIX named type file */
    DEFINE_POSIX_ERROR( ENAVAIL               ,ENAVAIL         ),  /* 119     No XENIX semaphores available */
    DEFINE_POSIX_ERROR( EISNAM                ,EISNAM          ),  /* 120     Is a named type file */
    DEFINE_POSIX_ERROR( EREMOTEIO             ,EREMOTEIO       ),  /* 121     Remote I/O error */
    DEFINE_POSIX_ERROR( EDQUOT                ,EDQUOT          ),  /* 122     Quota exceeded */
    DEFINE_POSIX_ERROR( ENOMEDIUM             ,ENOMEDIUM       ),  /* 123     No medium found */
    DEFINE_POSIX_ERROR( EMEDIUMTYPE           ,EMEDIUMTYPE     ),  /* 124     Wrong medium type */
    DEFINE_POSIX_ERROR( ECANCELED             ,ECANCELED       ),  /* 125     Operation Canceled */
    DEFINE_POSIX_ERROR( ENOKEY                ,ENOKEY          ),  /* 126     Required key not available */
    DEFINE_POSIX_ERROR( EKEYEXPIRED           ,EKEYEXPIRED     ),  /* 127     Key has expired */
    DEFINE_POSIX_ERROR( EKEYREVOKED           ,EKEYREVOKED     ),  /* 128     Key has been revoked */
    DEFINE_POSIX_ERROR( EKEYREJECTED          ,EKEYREJECTED    ),  /* 129     Key was rejected by service */
    DEFINE_POSIX_ERROR( EOWNERDEAD            ,EOWNERDEAD      ),  /* 130     Owner died */
    DEFINE_POSIX_ERROR( ENOTRECOVERABLE       ,ENOTRECOVERABLE ),  /* 131     State not recoverable */
    
    //Below are MBED SYSTEM ERROR CODE definitions
    //MBED SYSTEM ERROR CODE definitions starts at offset MBED_SYSTEM_ERROR_BASE, see above.
    //                   Error Name                 Error Code  
	DEFINE_SYSTEM_ERROR( UNKNOWN                    ,0 ),
    DEFINE_SYSTEM_ERROR( INVALID_ARGUMENT           ,1 ),
	DEFINE_SYSTEM_ERROR( INVALID_DATA               ,2 ),
	DEFINE_SYSTEM_ERROR( INVALID_FORMAT             ,3 ),
	DEFINE_SYSTEM_ERROR( INVALID_INDEX              ,4 ),
    DEFINE_SYSTEM_ERROR( INVALID_SIZE               ,5 ),
    DEFINE_SYSTEM_ERROR( INVALID_OPERATION          ,6 ),
	DEFINE_SYSTEM_ERROR( NOT_FOUND                  ,7 ),
	DEFINE_SYSTEM_ERROR( ACCESS_DENIED              ,8 ),
	DEFINE_SYSTEM_ERROR( NOT_SUPPORTED              ,9 ),
	DEFINE_SYSTEM_ERROR( BUFFER_FULL                ,10 ),
	DEFINE_SYSTEM_ERROR( MEDIA_FULL                 ,11 ),
	DEFINE_SYSTEM_ERROR( ALREADY_IN_USE             ,12 ),
	DEFINE_SYSTEM_ERROR( TIMEOUT                    ,13 ),
	DEFINE_SYSTEM_ERROR( NOT_READY                  ,14 ),
	DEFINE_SYSTEM_ERROR( FAILED_OPERATION           ,15 ),
    DEFINE_SYSTEM_ERROR( OPERATION_PROHIBITED       ,16 ),
	DEFINE_SYSTEM_ERROR( OPERATION_ABORTED          ,17 ),
	DEFINE_SYSTEM_ERROR( WRITE_PROTECTED            ,18 ),
	DEFINE_SYSTEM_ERROR( NO_RESPONSE                ,19 ),
	DEFINE_SYSTEM_ERROR( SEMAPHORE_LOCK_FAILED      ,20 ),
	DEFINE_SYSTEM_ERROR( MUTEX_LOCK_FAILED          ,21 ),
    DEFINE_SYSTEM_ERROR( SEMAPHORE_UNLOCK_FAILED    ,22 ),
	DEFINE_SYSTEM_ERROR( MUTEX_UNLOCK_FAILED        ,23 ),
	DEFINE_SYSTEM_ERROR( CRC_ERROR                  ,24 ),
    DEFINE_SYSTEM_ERROR( OPEN_FAILED                ,25 ),
    DEFINE_SYSTEM_ERROR( CLOSE_FAILED               ,26 ),
    DEFINE_SYSTEM_ERROR( READ_FAILED                ,27 ),
    DEFINE_SYSTEM_ERROR( WRITE_FAILED               ,28 ),
	DEFINE_SYSTEM_ERROR( INITIALIZATION_FAILED      ,29 ),
	DEFINE_SYSTEM_ERROR( BOOT_FAILURE               ,30 ),
    DEFINE_SYSTEM_ERROR( OUT_OF_MEMORY              ,31 ),
    DEFINE_SYSTEM_ERROR( OUT_OF_RESOURCES           ,32 ),
    DEFINE_SYSTEM_ERROR( ALLOC_FAILED               ,33 ),
    DEFINE_SYSTEM_ERROR( FREE_FAILED                ,34 ),
    DEFINE_SYSTEM_ERROR( OVERFLOW                   ,35 ),
    DEFINE_SYSTEM_ERROR( UNDERFLOW                  ,36 ),
    DEFINE_SYSTEM_ERROR( STACK_OVERFLOW             ,37 ),
    DEFINE_SYSTEM_ERROR( ISR_QUEUE_OVERFLOW         ,38 ),
    DEFINE_SYSTEM_ERROR( TIMER_QUEUE_OVERFLOW       ,39 ),
    DEFINE_SYSTEM_ERROR( CLIB_SPACE_UNAVAILABLE     ,40 ),
    DEFINE_SYSTEM_ERROR( CLIB_EXCEPTION             ,41 ),
    DEFINE_SYSTEM_ERROR( CLIB_MUTEX_INIT_FAILURE    ,42 ),
    DEFINE_SYSTEM_ERROR( CREATE_FAILED              ,43 ),
    DEFINE_SYSTEM_ERROR( DELETE_FAILED              ,44 ),
    DEFINE_SYSTEM_ERROR( THREAD_CREATE_FAILED       ,45 ),
    DEFINE_SYSTEM_ERROR( THREAD_DELETE_FAILED       ,46 ),
    DEFINE_SYSTEM_ERROR( PROHIBITED_IN_ISR_CONTEXT  ,47 ),
    DEFINE_SYSTEM_ERROR( PINMAP_INVALID             ,48 ),
    DEFINE_SYSTEM_ERROR( RTOS_EVENT                 ,49 ),
    DEFINE_SYSTEM_ERROR( RTOS_THREAD_EVENT          ,50 ),
    DEFINE_SYSTEM_ERROR( RTOS_MUTEX_EVENT           ,51 ),
    DEFINE_SYSTEM_ERROR( RTOS_SEMAPHORE_EVENT       ,52 ),
    DEFINE_SYSTEM_ERROR( RTOS_MEMORY_POOL_EVENT     ,53 ),
    DEFINE_SYSTEM_ERROR( RTOS_TIMER_EVENT           ,54 ),
    DEFINE_SYSTEM_ERROR( RTOS_EVENT_FLAGS_EVENT     ,55 ),
    DEFINE_SYSTEM_ERROR( RTOS_MESSAGE_QUEUE_EVENT   ,56 ),
    DEFINE_SYSTEM_ERROR( DEVICE_BUSY                ,57 ),
    DEFINE_SYSTEM_ERROR( CONFIG_UNSUPPORTED         ,58 ),
    DEFINE_SYSTEM_ERROR( CONFIG_MISMATCH            ,59 ),
    DEFINE_SYSTEM_ERROR( ALREADY_INITIALIZED        ,60 ),
    DEFINE_SYSTEM_ERROR( HARDFAULT_EXCEPTION        ,61 ),
	DEFINE_SYSTEM_ERROR( MEMMANAGE_EXCEPTION        ,62 ),
	DEFINE_SYSTEM_ERROR( BUSFAULT_EXCEPTION         ,63 ),
	DEFINE_SYSTEM_ERROR( USAGEFAULT_EXCEPTION       ,64 ),
        
    //Everytime you add a new system error code, you must update
    //Error documentation under Handbook to capture the info on
    //the new error status/codes
    
    //MBED CUSTOM ERROR CODE definitions starts at offset MBED_CUSTOM_ERROR_BASE, see above.
    /* Add More/Custom Error Codes here, See example below */
    //DEFINE_CUSTOM_ERROR( MY_CUSTOM_ERROR     , 1 ),    
    
} MbedErrorCode;

/** mbed_error_ctx struct
 *
 *  This struct captures the context information at the time of error.\n
 *  It primarily contains information about the thread where the error originated,\n
 *  filename/line number of the source file where the error occurred, a context specific error value(error_value)\n
 *  and the address where the error originated.\n
 *
 *  @note
 *  Below are the members of mbed_error_ctx struct\n
 *      error_status              MbedErrorStatus value for this error\n
 *      error_function_address    Address where the error occurred\n
 *      thread_id                 ID of the thread which generated the error\n
 *      thread_entry_address      Entry function of the thread which generated the error\n
 *      thread_stack_size         Stack Size of the thread which generated the error\n
 *      thread_stack_mem          Stack Top of the thread which generated the error\n
 *      thread_current_sp         Current Stack Pointer of the thread which generated the error\n
 *      error_value               A context/error specific value associated with this error\n
 *      error_filename            Filename where the error originated\n
 *      error_line_number         Line number in error_filename where the error originated\n
 */
typedef struct _mbed_error_ctx {
    MbedErrorStatus error_status;
    uint32_t error_address;
    uint32_t error_value;
    uint32_t thread_id;
    uint32_t thread_entry_address;
    uint32_t thread_stack_size;
    uint32_t thread_stack_mem;
    uint32_t thread_current_sp;
#ifdef MBED_CONF_ERROR_FILENAME_CAPTURE_ENABLED
    char error_filename[MBED_CONF_MAX_ERROR_FILENAME_LEN];
    uint32_t error_line_number;
#endif    
} mbed_error_ctx;

/** To generate a fatal compile-time error, you can use the pre-processor #error directive.
 *
 * @param format    C string that contains data stream to be printed.
 *                  Code snippets below show valid format.
 *
 * @deprecated
 * This function has been deprecated, please use one of SET_ERROR/SET_ERROR_FATAL macros 
 * or one of set_error/set_error_fatal functions.
 *
 * @code
 * #error "That shouldn't have happened!"
 * @endcode
 *
 * If the compiler evaluates this line, it will report the error and stop the compile.
 *
 * For example, you could use this to check some user-defined compile-time variables:
 *
 * @code
 * #define NUM_PORTS 7
 * #if (NUM_PORTS > 4)
 *     #error "NUM_PORTS must be less than 4"
 * #endif
 * @endcode
 *
 * Reporting Run-Time Errors:
 * To generate a fatal run-time error, you can use the mbed error() function.
 *
 * @code
 * error("That shouldn't have happened!");
 * @endcode
 *
 * If the mbed running the program executes this function, it will print the
 * message via the USB serial port, and then die with the blue lights of death!
 *
 * The message can use printf-style formatting, so you can report variables in the
 * message too. For example, you could use this to check a run-time condition:
 *
 * @code
 * if(x >= 5) {
 *     error("expected x to be less than 5, but got %d", x);
 * }
 * @endcode
 *
 *
 */
    
MBED_DEPRECATED_SINCE("mbed-os-5.9", "This function has been deprecated, please use one of SET_ERROR/SET_ERROR_FATAL macros or one of set_error/set_error_fatal functions" )

void error(const char* format, ...);

/**
 * Call this Macro to generate a MbedErrorStatus value for a System error
 * @param  entity           Entity generating the error code. If its unknown, pass ENTITY_UNKNOWN. See MbedEntityType for entity types.
 * @param  error_code       The MbedErrorCode code to be used in generating the MbedErrorStatus. See MbedErrorCode for error codes.
 *
 * @code
 * 
 * MbedErrorStatus driver_error = MAKE_SYSTEM_ERROR( ENTITY_DRIVER_USB, ERROR_CODE_INITIALIZATION_FAILED )
 *
 * @endcode
 * @note This macro generate MbedErrorStatus-es with error type set to ERROR_TYPE_SYSTEM
 *
 */
#define MAKE_SYSTEM_ERROR(entity, error_code)                   MAKE_MBED_ERROR(ERROR_TYPE_SYSTEM, entity, error_code)

/**
 * Call this Macro to generate a MbedErrorStatus value for a Custom error
 * @param  entity           Entity generating the error code. If its unknown, pass ENTITY_UNKNOWN. See MbedEntityType for entity types.
 * @param  error_code       The MbedErrorCode code to be used in generating the MbedErrorStatus. See MbedErrorCode for error codes.
 *
 * @code
 * 
 * MbedErrorStatus custom_error = MAKE_CUSTOM_ERROR( ENTITY_APPLICATION, 0xDEAD//16-bit custom error code )
 *
 * @endcode
 * @note This macro generate MbedErrorStatus-es with error type set to ERROR_TYPE_CUSTOM
 *
 */
#define MAKE_CUSTOM_ERROR(entity, error_code)                   MAKE_MBED_ERROR(ERROR_TYPE_CUSTOM, entity, error_code)

/**
 * Call this Macro to generate a MbedErrorStatus value for a System error
 * @param  entity           Entity generating the error code. If its unknown, pass ENTITY_UNKNOWN. See MbedEntityType for entity types.
 * @param  error_code       The MbedErrorCode code to be used in generating the MbedErrorStatus. See MbedErrorCode for error codes.
 *
 * @code
 * 
 * MbedErrorStatus new_error = MAKE_ERROR( ENTITY_DRIVER_USB, ERROR_INITIALIZATION_FAILED )
 *
 * @endcode
 * @note This macro generate MbedErrorStatus-es with error type set to ERROR_TYPE_SYSTEM
 *
 */
#define MAKE_ERROR(entity, error_code)                          MAKE_SYSTEM_ERROR(entity, error_code)

/**
 * Callback/Error hook function prototype. Applications needing a callback when an error is reported can use set_error_hook function
 * to register a callback/error hook function using the following prototype. When an error happens in the system error handling
 * implementation will invoke this callback with the MbedErrorStatus reported and the error context at the time of error.
 * @param  error_status     MbedErrorStatus status being reported.
 * @param  error_ctx        Error context structure associated with this error.
 * @return                  void
 *
 */
typedef void (*MbedErrorHook)(const mbed_error_ctx *error_ctx);

/**
 * Call this function to set a system error. This function will log the error status with the context info, prints the error report and return to caller.
 *
 * @param  error_status     MbedErrorStatus status to be set(See MbedErrorStatus enum above for available error status values).
 * @param  error_msg        The error message to be printed out to STDIO/Serial.
 * @param  error_value      Value associated with the error status. This would depend on error code/error scenario.
 * @param  filename         Name of the source file originating the error( Most callers can pass __FILE__ here ).
 * @param  line_number      The line number of the source file originating the error( Most callers can pass __LINE__ here ) .
 * @return                  0 or ERROR_SUCCESS.
 *                          ERROR_INVALID_ARGUMENT if called with invalid error status/codes 
 *
 * @code
 * 
 * set_error( ERROR_OUT_OF_MEMORY, "Out of memory error", 0, __FILE__, __LINE__ )
 *
 * @endcode
 *
 * @note See SET_ERROR/SET_ERROR_FATAL macros which provides a wrapper on this API
 */
MbedErrorStatus set_error(MbedErrorStatus error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number);

/**
 * Returns the first system error reported.
 * @return                  MbedErrorStatus code logged for the first error or ERROR_SUCCESS if no errors are logged.
 *
 */
MbedErrorStatus get_first_error(void);

/**
 * Returns the most recent system error reported.
 * @return                  MbedErrorStatus code logged for the last error or ERROR_SUCCESS if no errors are logged.
 *
 */
MbedErrorStatus get_last_error(void);

/**
 * Returns the number of system errors reported after boot.
 * @return                  int Number of errors reported.
 *
 */
int get_error_count(void);

/**
 * Call this function to set a fatal system error and halt the system. This function will log the fatal error with the context info and prints the error report.
 * 
 * @param  error_status     MbedErrorStatus status to be set(See MbedErrorStatus enum above for available error status values).
 * @param  error_msg        The error message to be printed out to STDIO/Serial.
 * @param  error_value      Value associated with the error status. This would depend on error code/error scenario.
 * @param  filename         Name of the source file originating the error( Most callers can pass __FILE__ here ).
 * @param  line_number      The line number of the source file originating the error( Most callers can pass __LINE__ here ) .
 * @return                  0 or ERROR_SUCCESS.
 *                          ERROR_INVALID_ARGUMENT if called with invalid error status/codes 
 *
 * @code
 * 
 * set_error_fatal( ERROR_PROHIBITED_OPERATION, "Prohibited operation tried", 0, __FILE__, __LINE__ )
 *
 * @endcode
 *
 * @note See SET_ERROR/SET_ERROR_FATAL macros which provides a wrapper on this API
 */
MbedErrorStatus set_error_fatal(MbedErrorStatus error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number);

/**
 * Registers an application defined error callback with the error handling system. 
 * This function will be called with error context info whenever system handles a set_error/set_error_fatal call
 * @param  custom_error_hook    MbedErrorStatus status to be set(See MbedErrorStatus enum above for available error status values).
 * @return                      0 or ERROR_SUCCESS on success.
 *                              ERROR_INVALID_ARGUMENT in case of NULL for custom_error_hook
 *
 * @code
 * 
 * MbedErrorStatus my_custom_error_hook(MbedErrorStatus error_status, const mbed_error_ctx *error_ctx) {
 *    //Do something with the error_status or error_ctx
 * }
 *
 * set_error_hook( my_custom_error_hook )
 *
 * @endcode
 *
 */
MbedErrorStatus set_error_hook(MbedErrorHook custom_error_hook);

/**
 * Reads the first error context information logged.
 * @param  error_info           This is the mbed_error_context info captured as part of the first set_error call. The caller should pass a pointer to mbed_error_context struct allocated by the caller.
 * @return                      0 or ERROR_SUCCESS on success.
 *                              ERROR_INVALID_ARGUMENT in case of invalid index
 *
 */
MbedErrorStatus get_first_error_log_info(mbed_error_ctx *error_info);

/**
 * Reads the last error context information logged.
 * @param  error_info           This is the mbed_error_context info captured as part of the last set_error call. The caller should pass a pointer to mbed_error_context struct allocated by the caller.
 * @return                      0 or ERROR_SUCCESS on success.
 *                              ERROR_INVALID_ARGUMENT in case of invalid index
 *
 */
MbedErrorStatus get_last_error_log_info(mbed_error_ctx *error_info);

/**
 * Clears all the last error, error count and all entries in the error log.
 * @return                      0 or ERROR_SUCCESS on success.
 *
 */
MbedErrorStatus clear_all_errors(void);

/**
 * Generates a MbedErrorStatus value based on passed in values for type, entity and error code.
 * @param  error_type           Error type based on MbedErrorType enum.
 * @param  entity               Entity type based on MbedEntityType enum.
 * @param  error_code           Error codes defined by MbedErrorCode enum
 * @return                      0 or ERROR_SUCCESS on success.
 *
 */
MbedErrorStatus make_mbed_error(MbedErrorType error_type, MbedEntityType entity, MbedErrorCode error_code);

#ifndef MBED_CONF_ERROR_LOG_DISABLED
/**
 * Returns the current number of entries in the error log, if there has been more than max number of errors logged the number returned will be max depth of error log.
 * @return                      Current number of entries in the error log.
 *
 */
int get_error_log_count(void);

/**
 * Reads the error context information for a specific error log specified by the index.
 * 
 * @param  index                index of the error context entry in the log to be retrieved.\n
 *                              The number of entries in the error log depth is configured during build and the max index depends on max depth of error log.\n
 *                              index = 0 points to the oldest entry in the log, and index = (max log depth - 1) points to the latest entry in the error log.\n
 * @param  error_info           This is the mbed_error_context info captured as part of the log. The caller should pass a pointer to mbed_error_context struct allocated by the caller.
 * @return                      0 or ERROR_SUCCESS on success.
 *                              ERROR_INVALID_ARGUMENT in case of invalid index
 *
 */
MbedErrorStatus get_error_log_info(int index, mbed_error_ctx *error_info);
#endif

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
/** @}*/


