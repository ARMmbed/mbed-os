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

/** Define this macro to include filenames in error context. For release builds, do not include filename to save memory.    
 *  MBED_PLATFORM_CONF_ERROR_FILENAME_CAPTURE_ENABLED
 */   

/** Define this macro to enable error history
 *  MBED_PLATFORM_CONF_ERROR_HIST_ENABLED  
 */
    
#ifndef MBED_CONF_PLATFORM_MAX_ERROR_FILENAME_LEN
#define MBED_CONF_PLATFORM_MAX_ERROR_FILENAME_LEN            16
#else //MBED_CONF_PLATFORM_MAX_ERROR_FILENAME_LEN
#if MBED_CONF_PLATFORM_MAX_ERROR_FILENAME_LEN > 64
//We have to limit this to 64 bytes since we use mbed_error_printf for error reporting
//and mbed_error_vfprintf uses 128bytes internal buffer which may not be sufficient for anything
//longer that 64 bytes with the current implementation.
#error "Unsupported error filename buffer length detected, max supported length is 64 chars. Please change MBED_CONF_PLATFORM_MAX_ERROR_FILENAME_LEN or max-error-filename-len in configuration."
#endif
#endif    

#define MBED_ERROR_STATUS_CODE_MASK                 (0x0000FFFF)
#define MBED_ERROR_STATUS_CODE_POS                  (0)
#define MBED_ERROR_STATUS_CODE_FIELD_SIZE           (16)

#define MBED_ERROR_STATUS_MODULE_MASK               (0x00FF0000)
#define MBED_ERROR_STATUS_MODULE_POS                (16)
#define MBED_ERROR_STATUS_MODULE_FIELD_SIZE         (8)

#define MBED_ERROR_STATUS_TYPE_MASK                 (0x60000000)
#define MBED_ERROR_STATUS_TYPE_POS                  (29)
#define MBED_ERROR_STATUS_TYPE_FIELD_SIZE           (2)

/* mbed_error_status_t Status Encoding */
//|31(1 bit) Always Negative|30-29(2 bits)  |28-24              | 23-16(8 bits) |  15-0(16 bits) |
//|-1                       |TYPE           |(unused/reserved)  | MODULE TYPE   |  ERROR CODE    |

#define MAKE_MBED_ERROR(type, module, error_code)   (mbed_error_status_t)                                                                   \
                                                    ((0x80000000) |                                                                     \
                                                    (MBED_ERROR_STATUS_CODE_MASK & (error_code << MBED_ERROR_STATUS_CODE_POS)) |        \
                                                    (MBED_ERROR_STATUS_MODULE_MASK & (module << MBED_ERROR_STATUS_MODULE_POS)) |        \
                                                    (MBED_ERROR_STATUS_TYPE_MASK & (type << MBED_ERROR_STATUS_TYPE_POS)))    

#define MBED_GET_ERROR_TYPE( error_status )         ((error_status & MBED_ERROR_STATUS_TYPE_MASK) >> MBED_ERROR_STATUS_TYPE_POS)
#define MBED_GET_ERROR_MODULE( error_status )       ((error_status & MBED_ERROR_STATUS_MODULE_MASK) >> MBED_ERROR_STATUS_MODULE_POS)    
#define MBED_GET_ERROR_CODE( error_status )         (int)((MBED_GET_ERROR_TYPE( error_status ) == MBED_ERROR_TYPE_POSIX)?(-error_status):((error_status & MBED_ERROR_STATUS_CODE_MASK) >> MBED_ERROR_STATUS_CODE_POS))

/** mbed_error_status_t description
 *
 * mbed_error_status_t type represents the error status values under MbedOS. mbed_error_status_t values are signed integers and always be negative.\n
 * Internally its encoded as below with bit-fields representing error type, module and error code:\n\n
 * mbed_error_status_t Status Encoding:\n
 *
 \verbatim
 | 31 Always Negative | 30-29(2 bits)  | 28-24              | 23-16(8 bits) | 15-0(16 bits) |
 | -1                 | TYPE           | (unused/reserved)  | MODULE TYPE    | ERROR CODE    |
 \endverbatim
 * 
 * The error status value range for each error type is as follows:\n
 *   Posix Error Status-es  - 0xFFFFFFFF to 0xFFFFFF01(-1 -255) - This corresponds to Posix error codes represented as negative.\n
 *   System Error Status-es - 0x80XX0100 to 0x80XX0FFF - This corresponds to System error codes range(all values are negative). Bits 23-16 will be module type(marked with XX)\n
 *   Custom Error Status-es - 0xA0XX1000 to 0xA0XXFFFF - This corresponds to Custom error codes range(all values are negative). Bits 23-16 will be module type(marked with XX)\n\n
 *
 * The ERROR CODE(values encoded into ERROR CODE bit-field in mbed_error_status_t) value range for each error type is also seperated as below:\n
 *   Posix Error Codes  - 1 to 255.\n
 *   System Error Codes - 256 to 4095.\n
 *   Custom Error Codes - 4096 to 65535.\n
 *
 * @note Posix error codes are always encoded as negative of their actual value. For example, EPERM is encoded as -EPERM.
 *       And, the MODULE TYPE for Posix error codes are always encoded as MBED_MODULE_UNKNOWN.\n
 *       This is to enable easy injection of Posix error codes into MbedOS error handling system without altering the actual Posix error values.\n
 *       Accordingly, Posix error codes are represented as -1 to -255 under MbedOS error status representation.
 */
typedef int mbed_error_status_t;

/**
 * Macro for defining a Posix error status. This macro is mainly used to define Posix error values in mbed_error_code_t enumeration.
 * @param  error_name       Name of the error without the ERROR_ prefix
 * @param  error_code       Error code value to be used, must be between 1 and 255(inclusive).
 *
 */
#define MBED_DEFINE_POSIX_ERROR( error_name, error_code )   \
                     MBED_ERROR_CODE_##error_name = error_code,  \
                     MBED_ERROR_##error_name = -(MBED_POSIX_ERROR_BASE + error_code)

/**
 * Macro for defining a System error status. This macro is used to define System error values in mbed_error_code_t enumeration.
 * @param  error_name       Name of the error without the ERROR_ prefix
 * @param  error_code       Error code value to be used, must be between 256 and 4096(inclusive).
 *
 */
#define MBED_DEFINE_SYSTEM_ERROR( error_name, error_code )   \
                      MBED_ERROR_CODE_##error_name = MBED_SYSTEM_ERROR_BASE + error_code,  \
                      MBED_ERROR_##error_name = MAKE_MBED_ERROR(MBED_ERROR_TYPE_SYSTEM, MBED_MODULE_UNKNOWN, MBED_ERROR_CODE_##error_name)

/**
 * Macro for defining a Custom error status. This macro is used to define custom error values in mbed_error_code_t enumeration.
 * @param  error_name       Name of the error without the ERROR_ prefix
 * @param  error_code       Error code value to be used, must be between 4097 and 65535(inclusive).
 *
 */
#define MBED_DEFINE_CUSTOM_ERROR( error_name, error_code )   \
                      MBED_ERROR_CODE_##error_name = MBED_CUSTOM_ERROR_BASE + error_code,  \
                      MBED_ERROR_##error_name = MAKE_MBED_ERROR(MBED_ERROR_TYPE_CUSTOM, MBED_MODULE_UNKNOWN, MBED_ERROR_CODE_##error_name)


/**
 * Macros for setting a system warning. These macros will log the error, Its a wrapper for calling mbed_warning API.
 * There are 2 versions of this macro. MBED_WARNING takes status and message. MBED_WARNING1 takes an additional context specific argument
 * @param  error_status     mbed_error_status_t status to be set(See mbed_error_status_t enum above for available error status values).
 * @param  error_msg        The error message to be printed out to STDIO/Serial.
 * @param  error_value      Value associated with the error status. This would depend on error code/error scenario.
 *
 * @code
 * 
 * MBED_WARNING( ERROR_INVALID_SIZE, "MyDriver: Invalid size in read" )
 * MBED_WARNING1( ERROR_INVALID_SIZE, "MyDriver: Invalid size in read", size_val )
 *
 * @endcode
 * @note The macro calls mbed_warning API with filename and line number info without caller explicitly passing them.
 *        Since this macro is a wrapper for mbed_warning API callers should process the return value from this macro which is the return value from calling mbed_error API.
 *
 */
#ifdef NDEBUG
#define MBED_WARNING1( error_status, error_msg, error_value )         mbed_warning( error_status, (const char *)NULL, (uint32_t)error_value, NULL, 0 )
#define MBED_WARNING( error_status, error_msg )                       mbed_warning( error_status, (const char *)NULL, (uint32_t)0,           NULL, 0 )    
#else //NDEBUG
#if MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED
#define MBED_WARNING1( error_status, error_msg, error_value )     mbed_warning( error_status, (const char *)error_msg, (uint32_t)error_value, (const char *)MBED_FILENAME, __LINE__ )
#define MBED_WARNING( error_status, error_msg )                   mbed_warning( error_status, (const char *)error_msg, (uint32_t)0          , (const char *)MBED_FILENAME, __LINE__ )
#else //MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED
#define MBED_WARNING1( error_status, error_msg, error_value )     mbed_warning( error_status, (const char *)error_msg, (uint32_t)error_value, NULL, 0 )
#define MBED_WARNING( error_status, error_msg )                   mbed_warning( error_status, (const char *)error_msg, (uint32_t)0,           NULL, 0 )    
#endif    
#endif

/**
 * Macros for setting a fatal system error. These macros will log the error, prints the error report and halts the system. Its a wrapper for calling mbed_error API.
 * There are 2 versions of this macro. MBED_ERROR takes status and message. MBED_ERROR1 takes an additional context specific argument
 * @param  error_status     mbed_error_status_t status to be set(See mbed_error_status_t enum above for available error status values).
 * @param  error_msg        The error message to be printed out to STDIO/Serial.
 * @param  error_value      Value associated with the error status. This would depend on error code/error scenario. Only available with MBED_ERROR1
 * @return                  0 or MBED_SUCCESS.
 *                          MBED_ERROR_INVALID_ARGUMENT if called with invalid error status/codes
 *
 * @code
 * 
 * MBED_ERROR( MBED_ERROR_MUTEX_LOCK_FAILED, "MyDriver: Can't lock driver Mutex" )
 * MBED_ERROR1( MBED_ERROR_MUTEX_LOCK_FAILED, "MyDriver: Can't lock driver Mutex", &my_mutex )
 *
 * @endcode
 * @note The macro calls mbed_error API with filename and line number info without caller explicitly passing them.
 *       Since this macro is a wrapper for mbed_error API callers should process the return value from this macro which is the return value from calling mbed_error API. 
 *
 */
#ifdef NDEBUG
#define MBED_ERROR1( error_status, error_msg, error_value )           mbed_error( error_status, (const char *)NULL, (uint32_t)error_value, NULL, 0 )
#define MBED_ERROR( error_status, error_msg )                         mbed_error( error_status, (const char *)NULL, (uint32_t)0          , NULL, 0 )
#else //NDEBUG
#if MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED
#define MBED_ERROR1( error_status, error_msg, error_value )       mbed_error( error_status, (const char *)error_msg, (uint32_t)error_value, (const char *)MBED_FILENAME, __LINE__ )
#define MBED_ERROR( error_status, error_msg )                     mbed_error( error_status, (const char *)error_msg, (uint32_t)0          , (const char *)MBED_FILENAME, __LINE__ )
#else //MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED
#define MBED_ERROR1( error_status, error_msg, error_value )       mbed_error( error_status, (const char *)error_msg, (uint32_t)error_value, NULL, 0 )
#define MBED_ERROR( error_status, error_msg )                     mbed_error( error_status, (const char *)error_msg, (uint32_t)0          , NULL, 0 )
#endif    
#endif

//Error Type definition
/** mbed_error_type_t definition
 *  @note
 *  This enumeration defines the Error types supported. The value of these enum values will be encoded into mbed_error_status_t TYPE field.\n
 *      See mbed_error_status_t description for more info.\n
 *         MBED_ERROR_TYPE_SYSTEM - Used to indicate that the error status is of System defined Error type.\n
 *         MBED_ERROR_TYPE_CUSTOM - Used to indicate that the error status is of Custom defined Error type.\n 
 *         MBED_ERROR_TYPE_POSIX  - Used to indicate that the error status is of Posix error type.\n 
 *
 */
typedef enum _mbed_error_type_t
{
    MBED_ERROR_TYPE_SYSTEM = 0,
    MBED_ERROR_TYPE_CUSTOM = 1,
    //2 is reserved
    //Use 3 for POSIX because we are mapping -1 to -255 to POSIX error codes
    //and thus we must use 3 to match the type bits in error status representation which are from 0xFFFFFFFF to 0xFFFFFF00
    MBED_ERROR_TYPE_POSIX = 3
} mbed_error_type_t;        

//Module type/id definitions
/** mbed_module_type_t definition
 * @note
 * This enumeration defines the module types. The value of these enum values will be encoded into mbed_error_status_t MODULE field.\n\n
 *      See mbed_error_status_t description for more info.\n
 *          MBED_MODULE_UNKNOWN - This module type can be used if caller of the mbed_error/mbed_warning doesn't know who is the actual originator of the error.\n
 *                       Other module values can be used to provide more info on who/where the error originated from.\n\n
 *                       For example, if I2C driver is the component originating the error you can use MBED_MODULE_DRIVER_I2C to provide more info.\n
 *                       Its used in call to MBED_MAKE_ERROR/MBED_MAKE_SYSTEM_ERROR/MBED_MAKE_CUSTOM_ERROR macros.\n
 *
 * @code
 *      Example: mbed_error_status_t i2c_driver_error = MBED_MAKE_ERROR( MBED_MODULE_DRIVER_I2C, MBED_ERROR_CONFIG_UNSUPPORTED );
 * @endcode
 * 
 * @note
 *  \n Below are the module code mappings:\n
    \verbatim
    MBED_MODULE_APPLICATION              0       Application     
    MBED_MODULE_PLATFORM                 1       Platform
    MODULE_KERNEL                   2       RTX Kernel
    MBED_MODULE_NETWORK_STACK            3       Network stack
    MBED_MODULE_HAL                      4       HAL - Hardware Abstraction Layer
    MBED_MODULE_NETWORK_STACKMODULE_MEMORY_SUBSYSTEM         5       Memory Subsystem    
    MBED_MODULE_FILESYSTEM               6       Filesystem
    MBED_MODULE_BLOCK_DEVICE             7       Block device
    MBED_MODULE_DRIVER                   8       Driver
    MBED_MODULE_DRIVER_SERIAL            9       Serial Driver
    MBED_MODULE_DRIVER_RTC               10      RTC Driver
    MBED_MODULE_DRIVER_I2C               11      I2C Driver
    MODULE_DRIVER_SPI               12      SPI Driver
    MODULE_DRIVER_GPIO              13      GPIO Driver
    MODULE_DRIVER_ANALOG            14      Analog Driver
    MODULE_DRIVER_DIGITAL           15      DigitalIO Driver
    MODULE_DRIVER_CAN               16      CAN Driver
    MODULE_DRIVER_ETHERNET          17      Ethernet Driver
    MODULE_DRIVER_CRC               18      CRC Module
    MODULE_DRIVER_PWM               19      PWM Driver
    MODULE_DRIVER_QSPI              20      QSPI Driver
    MODULE_DRIVER_USB               21      USB Driver
    MODULE_TARGET_SDK               22      SDK 
    
    MBED_MODULE_UNKNOWN                  255     Unknown module
    \endverbatim
 *       
 */
typedef enum _mbed_module_type {
    MBED_MODULE_APPLICATION = 0,
    MBED_MODULE_PLATFORM,
    MBED_MODULE_KERNEL,
    MBED_MODULE_NETWORK_STACK,
    MBED_MODULE_HAL,
    MBED_MODULE_NETWORK_STACKMODULE_MEMORY_SUBSYSTEM,
    MBED_MODULE_FILESYSTEM,
    MBED_MODULE_BLOCK_DEVICE,
    MBED_MODULE_DRIVER,
    MBED_MODULE_DRIVER_SERIAL,
    MBED_MODULE_DRIVER_RTC,
    MBED_MODULE_DRIVER_I2C,
    MBED_MODULE_DRIVER_SPI,
    MBED_MODULE_DRIVER_GPIO,
    MBED_MODULE_DRIVER_ANALOG,
    MBED_MODULE_DRIVER_DIGITAL,
    MBED_MODULE_DRIVER_CAN,
    MBED_MODULE_DRIVER_ETHERNET,
    MBED_MODULE_DRIVER_CRC,
    MBED_MODULE_DRIVER_PWM,
    MBED_MODULE_DRIVER_QSPI,
    MBED_MODULE_DRIVER_USB,
    MBED_MODULE_TARGET_SDK,
    /* Add More entities here as required */
    
    MBED_MODULE_UNKNOWN = 255,
    MBED_MODULE_MAX = MBED_MODULE_UNKNOWN
} mbed_module_type_t;

//Use MBED_SUCCESS(=0) or any postive number for successful returns
#define MBED_SUCCESS           0

#define MBED_POSIX_ERROR_BASE   0
#define MBED_SYSTEM_ERROR_BASE  256
#define MBED_CUSTOM_ERROR_BASE  4096

//Error Code definitions
/** mbed_error_code_t definition
 *
 *  mbed_error_code_t enumeration defines the Error codes and Error status values for MBED_MODULE_UNKNOWN.\n
 *  It defines all of Posix Error Codes/Statuses and Mbed System Error Codes/Statuses.\n\n
 *
 *  @note
 *  Posix Error codes are defined using the macro MBED_DEFINE_POSIX_ERROR\n
 *  For example MBED_DEFINE_POSIX_ERROR( EPERM, EPERM ). This effectively defines the following values:\n
 *      ERROR_CODE_EPERM = EPERM\n
 *      ERROR_EPERM = -EPERM\n
 *
 *  Posix Error codes are defined using the macro MBED_DEFINE_POSIX_ERROR\n
 *  For example MBED_DEFINE_POSIX_ERROR( EPERM, EPERM ). This macro defines the following values:\n
 *      ERROR_CODE_EPERM = MBED_POSIX_ERROR_BASE+EPERM\n
 *      ERROR_EPERM = -(MBED_POSIX_ERROR_BASE+EPERM)\n
 *  Its effectively equivalent to:\n
 *      ERROR_CODE_EPERM = 1\n
 *      ERROR_EPERM = -1\n 
 *  All Posix error codes currently supported by MbedOS(defined in mbed_retarget.h) are defined using the MBED_DEFINE_POSIX_ERROR macro.\n\n
 *  Below are the Posic error codes and the description:\n
 *  \verbatim
    EPERM                      1        Operation not permitted 
    ENOENT                     2        No such file or directory 
    ESRCH                      3        No such process 
    EINTR                      4        Interrupted system call 
    EIO                        5        I/O error 
    ENXIO                      6        No such device or address 
    E2BIG                      7        Argument list too long 
    ENOEXEC                    8        Exec format error 
    EBADF                      9        Bad file number 
    ECHILD                     10       No child processes 
    EAGAIN                     11       Try again 
    ENOMEM                     12       Out of memory 
    EACCES                     13       Permission denied 
    EFAULT                     14       Bad address 
    ENOTBLK                    15       Block device required 
    EBUSY                      16       Device or resource busy 
    EEXIST                     17       File exists 
    EXDEV                      18       Cross-device link 
    ENODEV                     19       No such device 
    ENOTDIR                    20       Not a directory 
    EISDIR                     21       Is a directory 
    EINVAL                     22       Invalid argument 
    ENFILE                     23       File table overflow 
    EMFILE                     24       Too many open files 
    ENOTTY                     25       Not a typewriter 
    ETXTBSY                    26       Text file busy 
    EFBIG                      27       File too large 
    ENOSPC                     28       No space left on device 
    ESPIPE                     29       Illegal seek 
    EROFS                      30       Read-only file system 
    EMLINK                     31       Too many links 
    EPIPE                      32       Broken pipe 
    EDOM                       33       Math argument out of domain of func 
    ERANGE                     34       Math result not representable 
    EDEADLK                    35       Resource deadlock would occur 
    ENAMETOOLONG               36       File name too long 
    ENOLCK                     37       No record locks available 
    ENOSYS                     38       Function not implemented 
    ENOTEMPTY                  39       Directory not empty 
    ELOOP                      40       Too many symbolic links encountered 
    EWOULDBLOCK                EAGAIN   Operation would block 
    ENOMSG                     42       No message of desired type 
    EIDRM                      43       Identifier removed 
    ECHRNG                     44       Channel number out of range 
    EL2NSYNC                   45       Level 2 not synchronized 
    EL3HLT                     46       Level 3 halted 
    EL3RST                     47       Level 3 reset 
    ELNRNG                     48       Link number out of range 
    EUNATCH                    49       Protocol driver not attached 
    ENOCSI                     50       No CSI structure available 
    EL2HLT                     51       Level 2 halted 
    EBADE                      52       Invalid exchange 
    EBADR                      53       Invalid request descriptor 
    EXFULL                     54       Exchange full 
    ENOANO                     55       No anode 
    EBADRQC                    56       Invalid request code 
    EBADSLT                    57       Invalid slot 
    EDEADLOCK                  EDEADLK  Resource deadlock would occur 
    EBFONT                     59       Bad font file format 
    ENOSTR                     60       Device not a stream 
    ENODATA                    61       No data available 
    ETIME                      62       Timer expired 
    ENOSR                      63       Out of streams resources 
    ENONET                     64       Machine is not on the network 
    ENOPKG                     65       Package not installed 
    EREMOTE                    66       Object is remote 
    ENOLINK                    67       Link has been severed 
    EADV                       68       Advertise error 
    ESRMNT                     69       Srmount error 
    ECOMM                      70       Communication error on send 
    EPROTO                     71       Protocol error 
    EMULTIHOP                  72       Multihop attempted 
    EDOTDOT                    73       RFS specific error 
    EBADMSG                    74       Not a data message 
    EOVERFLOW                  75       Value too large for defined data type 
    ENOTUNIQ                   76       Name not unique on network 
    EBADFD                     77       File descriptor in bad state 
    EREMCHG                    78       Remote address changed 
    ELIBACC                    79       Can not access a needed shared library 
    ELIBBAD                    80       Accessing a corrupted shared library 
    ELIBSCN                    81       .lib section in a.out corrupted 
    ELIBMAX                    82       Attempting to link in too many shared libraries 
    ELIBEXEC                   83       Cannot exec a shared library directly 
    EILSEQ                     84       Illegal byte sequence 
    ERESTART                   85       Interrupted system call should be restarted 
    ESTRPIPE                   86       Streams pipe error 
    EUSERS                     87       Too many users 
    ENOTSOCK                   88       Socket operation on non-socket 
    EDESTADDRREQ               89       Destination address required 
    EMSGSIZE                   90       Message too long 
    EPROTOTYPE                 91       Protocol wrong type for socket 
    ENOPROTOOPT                92       Protocol not available 
    EPROTONOSUPPORT            93       Protocol not supported 
    ESOCKTNOSUPPORT            94       Socket type not supported 
    EOPNOTSUPP                 95       Operation not supported on transport endpoint 
    EPFNOSUPPORT               96       Protocol family not supported 
    EAFNOSUPPORT               97       Address family not supported by protocol 
    EADDRINUSE                 98       Address already in use 
    EADDRNOTAVAIL              99       Cannot assign requested address 
    ENETDOWN                   100      Network is down 
    ENETUNREACH                101      Network is unreachable 
    ENETRESET                  102      Network dropped connection because of reset 
    ECONNABORTED               103      Software caused connection abort 
    ECONNRESET                 104      Connection reset by peer 
    ENOBUFS                    105      No buffer space available 
    EISCONN                    106      Transport endpoint is already connected 
    ENOTCONN                   107      Transport endpoint is not connected 
    ESHUTDOWN                  108      Cannot send after transport endpoint shutdown 
    ETOOMANYREFS               109      Too many references: cannot splice 
    ETIMEDOUT                  110      Connection timed out 
    ECONNREFUSED               111      Connection refused 
    EHOSTDOWN                  112      Host is down 
    EHOSTUNREACH               113      No route to host 
    EALREADY                   114      Operation already in progress 
    EINPROGRESS                115      Operation now in progress 
    ESTALE                     116      Stale NFS file handle 
    EUCLEAN                    117      Structure needs cleaning 
    ENOTNAM                    118      Not a XENIX named type file 
    ENAVAIL                    119      No XENIX semaphores available 
    EISNAM                     120      Is a named type file 
    EREMOTEIO                  121      Remote I/O error 
    EDQUOT                     122      Quota exceeded 
    ENOMEDIUM                  123      No medium found 
    EMEDIUMTYPE                124      Wrong medium type 
    ECANCELED                  125      Operation Canceled 
    ENOKEY                     126      Required key not available 
    EKEYEXPIRED                127      Key has expired 
    EKEYREVOKED                128      Key has been revoked 
    EKEYREJECTED               129      Key was rejected by service 
    EOWNERDEAD                 130      Owner died 
    ENOTRECOVERABLE            131      State not recoverable 
    \endverbatim
 *
 *  @note
 *  MbedOS System Error codes are defined using the macro MBED_DEFINE_SYSTEM_ERROR\n
 *  For example MBED_DEFINE_SYSTEM_ERROR( INVALID_ARGUMENT ,1 ) macro defines the following values:\n
 *      ERROR_CODE_INVALID_ARGUMENT = MBED_SYSTEM_ERROR_BASE+1\n
 *      ERROR_INVALID_ARGUMENT = MAKE_MBED_ERROR(ERROR_TYPE_SYSTEM, MBED_MODULE_UNKNOWN, ERROR_CODE_INVALID_ARGUMENT)\n
 *  Its effectively equivalent to:\n
 *      ERROR_CODE_INVALID_ARGUMENT = 1\n
 *      ERROR_INVALID_ARGUMENT = 0x80FF0001\n (Note that MODULE field is set to MBED_MODULE_UNKNOWN) 
 *  New System Error codes should be defined using MBED_DEFINE_SYSTEM_ERROR macro and must have an unique error code value\n
 *  passed as the second argument in the MBED_DEFINE_SYSTEM_ERROR macro.\n\n
 *  Below are the Mbed System error codes and the description:
 *  \verbatim 
    UNKNOWN                    256      Unknown error 
    INVALID_ARGUMENT           257      Invalid Argument 
    INVALID_DATA               258      Invalid data 
    INVALID_FORMAT             259      Invalid format 
    INVALID_INDEX              260      Invalid Index 
    INVALID_SIZE               261      Inavlid Size 
    INVALID_OPERATION          262      Invalid Operation 
    NOT_FOUND                  263      Not Found 
    ACCESS_DENIED              264      Access Denied 
    NOT_SUPPORTED              265      Not supported 
    BUFFER_FULL                266      Buffer Full 
    MEDIA_FULL                 267      Media/Disk Full 
    ALREADY_IN_USE             268      Already in use 
    TIMEOUT                    269      Timeout error 
    NOT_READY                  270      Not Ready 
    FAILED_OPERATION           271      Requested Operation failed 
    OPERATION_PROHIBITED       272      Operation prohibited 
    OPERATION_ABORTED          273      Operation failed 
    WRITE_PROTECTED            274      Attempt to write to write-protected resource 
    NO_RESPONSE                275      No response 
    SEMAPHORE_LOCK_FAILED      276      Sempahore lock failed 
    MUTEX_LOCK_FAILED          277      Mutex lock failed 
    SEMAPHORE_UNLOCK_FAILED    278      Sempahore unlock failed 
    MUTEX_UNLOCK_FAILED        279      Mutex unlock failed 
    CRC_ERROR                  280      CRC error or mismatch 
    OPEN_FAILED                281      Open failed 
    CLOSE_FAILED               282      Close failed 
    READ_FAILED                283      Read failed 
    WRITE_FAILED               284      Write failed 
    INITIALIZATION_FAILED      285      Initialization failed 
    BOOT_FAILURE               286      Boot failure 
    OUT_OF_MEMORY              287      Out of memory 
    OUT_OF_RESOURCES           288      Out of resources 
    ALLOC_FAILED               289      Alloc failed 
    FREE_FAILED                290      Free failed 
    OVERFLOW                   291      Overflow error 
    UNDERFLOW                  292      Underflow error 
    STACK_OVERFLOW             293      Stack overflow error 
    ISR_QUEUE_OVERFLOW         294      ISR queue overflow 
    TIMER_QUEUE_OVERFLOW       295      Timer Queue overflow 
    CLIB_SPACE_UNAVAILABLE     296      Standard library error - Space unavailable 
    CLIB_EXCEPTION             297      Standard library error - Exception 
    CLIB_MUTEX_INIT_FAILURE    298      Standard library error - Mutex Init failure 
    CREATE_FAILED              299      Create failed 
    DELETE_FAILED              300      Delete failed 
    THREAD_CREATE_FAILED       301      Thread Create failed 
    THREAD_DELETE_FAILED       302      Thread Delete failed 
    PROHIBITED_IN_ISR_CONTEXT  303      Operation Prohibited in ISR context 
    PINMAP_INVALID             304      Pinmap Invalid 
    RTOS_EVENT                 305      Unknown Rtos Error 
    RTOS_THREAD_EVENT          306      Rtos Thread Error 
    RTOS_MUTEX_EVENT           307      Rtos Mutex Error 
    RTOS_SEMAPHORE_EVENT       308      Rtos Semaphore Error 
    RTOS_MEMORY_POOL_EVENT     309      Rtos Memory Pool Error 
    RTOS_TIMER_EVENT           310      Rtos Timer Error 
    RTOS_EVENT_FLAGS_EVENT     311      Rtos Event flags Error 
    RTOS_MESSAGE_QUEUE_EVENT   312      Rtos Message queue Error 
    DEVICE_BUSY                313      Device Busy 
    CONFIG_UNSUPPORTED         314      Configuration not supported 
    CONFIG_MISMATCH            315      Configuration mismatch 
    ALREADY_INITIALIZED        316      Already initialzied 
    HARDFAULT_EXCEPTION        317      HardFault exception 
    MEMMANAGE_EXCEPTION        318      MemManage exception 
    BUSFAULT_EXCEPTION         319      BusFault exception 
    USAGEFAULT_EXCEPTION       320      UsageFault exception
    \endverbatim
 *
 *  @note
 *  Custom Error codes can be defined using the macro DEFINE_CUSTOM_ERROR\n
 *  This is mainly meant to capture non-generic error codes specific to a device.
 *  For example DEFINE_CUSTOM_ERROR( MY_CUSTOM_ERROR ,1 ) macro defines the following values:\n
 *      ERROR_CODE_MY_CUSTOM_ERROR = MBED_CUSTOM_ERROR_BASE+1\n
 *      ERROR_MY_CUSTOM_ERROR = MAKE_MBED_ERROR(ERROR_TYPE_CUSTOM, MBED_MODULE_UNKNOWN, ERROR_CODE_MY_CUSTOM_ERROR)\n
 *  Its effectively equivalent to:\n
 *      ERROR_CODE_MY_CUSTOM_ERROR = 4097\n
 *      ERROR_MY_CUSTOM_ERROR = 0xA0FF1001\n (Note that MODULE field is set to MBED_MODULE_UNKNOWN) \n\n
 *
 *  @note
 *  **Using error codes:** \n  
 *  Posix error codes may be used in modules/functions currently using Posix error codes and switching them to Mbed-OS error codes
 *  may cause interoperability issues. For example, some of the filesystem, network stack implementations may need to use 
 *  Posix error codes in order to keep them compatible with other modules interfacing with them, and may continue to use Posix error codes. 
 *  
 *  In all other cases, like for any native development of Mbed-OS modules Mbed-OS error codes should be used.
 *  This makes it easy to use Mbed-OS error reporting/logging infrastructure and makes debugging error scenarios
 *  much more efficient.
 * 
 *  @note
 *  **Searching for error codes in mbed-os source tree:** \n
 *  If you get an error report as below which you want to search for in mbed-os source tree, first take note of "Error Code" number. \n
 *  For example, the below error report has an error code of \b 259. Find the error name associated with the error code and in this case its \b INVALID_FORMAT. \n
 *  Use that error name(\b INVALID_FORMAT) to search the source tree for code locations setting that specific error code. \n 
 *  If the Error module reported is not 255(which indicates unknown module), you can also use that to narrow down to the specific component reporting the error.
 *  See mbed_module_type_t enum above for module mapping. \n
 *  
 *  \verbatim
    ++ MbedOS Error Info ++
    Error Status: 0x80040103
    Error Code: 259
    Error Module: 04
    Error Message: HAL Module error
    Error Location: 0x000067C7
    Error Value: 0x00005566
    Current Thread: Id: 0x200024A8 EntryFn: 0x0000FB0D StackSize: 0x00001000 StackMem: 0x200014A8 SP: 0x2002FFD8
    -- MbedOS Error Info --
    \endverbatim
 */
 
typedef enum _mbed_error_code {
    //Below are POSIX ERROR CODE definitions, which starts at MBED_POSIX_ERROR_BASE(=0)
    //POSIX ERROR CODE definitions starts at offset 0(MBED_POSIX_ERROR_BASE) to align them with actual Posix Error Code
    //defintions in mbed_retarget.h
    //                  Error Name                                    Error Code
    MBED_DEFINE_POSIX_ERROR( EPERM                 ,EPERM           ),  /* 1       Operation not permitted */
    MBED_DEFINE_POSIX_ERROR( ENOENT                ,ENOENT          ),  /* 2       No such file or directory */
    MBED_DEFINE_POSIX_ERROR( ESRCH                 ,ESRCH           ),  /* 3       No such process */
    MBED_DEFINE_POSIX_ERROR( EINTR                 ,EINTR           ),  /* 4       Interrupted system call */
    MBED_DEFINE_POSIX_ERROR( EIO                   ,EIO             ),  /* 5       I/O error */
    MBED_DEFINE_POSIX_ERROR( ENXIO                 ,ENXIO           ),  /* 6       No such device or address */
    MBED_DEFINE_POSIX_ERROR( E2BIG                 ,E2BIG           ),  /* 7       Argument list too long */
    MBED_DEFINE_POSIX_ERROR( ENOEXEC               ,ENOEXEC         ),  /* 8       Exec format error */
    MBED_DEFINE_POSIX_ERROR( EBADF                 ,EBADF           ),  /* 9       Bad file number */
    MBED_DEFINE_POSIX_ERROR( ECHILD                ,ECHILD          ),  /* 10      No child processes */
    MBED_DEFINE_POSIX_ERROR( EAGAIN                ,EAGAIN          ),  /* 11      Try again */
    MBED_DEFINE_POSIX_ERROR( ENOMEM                ,ENOMEM          ),  /* 12      Out of memory */
    MBED_DEFINE_POSIX_ERROR( EACCES                ,EACCES          ),  /* 13      Permission denied */
    MBED_DEFINE_POSIX_ERROR( EFAULT                ,EFAULT          ),  /* 14      Bad address */
    MBED_DEFINE_POSIX_ERROR( ENOTBLK               ,ENOTBLK         ),  /* 15      Block device required */
    MBED_DEFINE_POSIX_ERROR( EBUSY                 ,EBUSY           ),  /* 16      Device or resource busy */
    MBED_DEFINE_POSIX_ERROR( EEXIST                ,EEXIST          ),  /* 17      File exists */
    MBED_DEFINE_POSIX_ERROR( EXDEV                 ,EXDEV           ),  /* 18      Cross-device link */
    MBED_DEFINE_POSIX_ERROR( ENODEV                ,ENODEV          ),  /* 19      No such device */
    MBED_DEFINE_POSIX_ERROR( ENOTDIR               ,ENOTDIR         ),  /* 20      Not a directory */
    MBED_DEFINE_POSIX_ERROR( EISDIR                ,EISDIR          ),  /* 21      Is a directory */
    MBED_DEFINE_POSIX_ERROR( EINVAL                ,EINVAL          ),  /* 22      Invalid argument */
    MBED_DEFINE_POSIX_ERROR( ENFILE                ,ENFILE          ),  /* 23      File table overflow */
    MBED_DEFINE_POSIX_ERROR( EMFILE                ,EMFILE          ),  /* 24      Too many open files */
    MBED_DEFINE_POSIX_ERROR( ENOTTY                ,ENOTTY          ),  /* 25      Not a typewriter */
    MBED_DEFINE_POSIX_ERROR( ETXTBSY               ,ETXTBSY         ),  /* 26      Text file busy */
    MBED_DEFINE_POSIX_ERROR( EFBIG                 ,EFBIG           ),  /* 27      File too large */
    MBED_DEFINE_POSIX_ERROR( ENOSPC                ,ENOSPC          ),  /* 28      No space left on device */
    MBED_DEFINE_POSIX_ERROR( ESPIPE                ,ESPIPE          ),  /* 29      Illegal seek */
    MBED_DEFINE_POSIX_ERROR( EROFS                 ,EROFS           ),  /* 30      Read-only file system */
    MBED_DEFINE_POSIX_ERROR( EMLINK                ,EMLINK          ),  /* 31      Too many links */
    MBED_DEFINE_POSIX_ERROR( EPIPE                 ,EPIPE           ),  /* 32      Broken pipe */
    MBED_DEFINE_POSIX_ERROR( EDOM                  ,EDOM            ),  /* 33      Math argument out of domain of func */
    MBED_DEFINE_POSIX_ERROR( ERANGE                ,ERANGE          ),  /* 34      Math result not representable */
    MBED_DEFINE_POSIX_ERROR( EDEADLK               ,EDEADLK         ),  /* 35      Resource deadlock would occur */
    MBED_DEFINE_POSIX_ERROR( ENAMETOOLONG          ,ENAMETOOLONG    ),  /* 36      File name too long */
    MBED_DEFINE_POSIX_ERROR( ENOLCK                ,ENOLCK          ),  /* 37      No record locks available */
    MBED_DEFINE_POSIX_ERROR( ENOSYS                ,ENOSYS          ),  /* 38      Function not implemented */
    MBED_DEFINE_POSIX_ERROR( ENOTEMPTY             ,ENOTEMPTY       ),  /* 39      Directory not empty */
    MBED_DEFINE_POSIX_ERROR( ELOOP                 ,ELOOP           ),  /* 40      Too many symbolic links encountered */
    MBED_DEFINE_POSIX_ERROR( EWOULDBLOCK           ,EAGAIN          ),  /* EAGAIN  Operation would block */
    MBED_DEFINE_POSIX_ERROR( ENOMSG                ,ENOMSG          ),  /* 42      No message of desired type */
    MBED_DEFINE_POSIX_ERROR( EIDRM                 ,EIDRM           ),  /* 43      Identifier removed */
    MBED_DEFINE_POSIX_ERROR( ECHRNG                ,ECHRNG          ),  /* 44      Channel number out of range */
    MBED_DEFINE_POSIX_ERROR( EL2NSYNC              ,EL2NSYNC        ),  /* 45      Level 2 not synchronized */
    MBED_DEFINE_POSIX_ERROR( EL3HLT                ,EL3HLT          ),  /* 46      Level 3 halted */
    MBED_DEFINE_POSIX_ERROR( EL3RST                ,EL3RST          ),  /* 47      Level 3 reset */
    MBED_DEFINE_POSIX_ERROR( ELNRNG                ,ELNRNG          ),  /* 48      Link number out of range */
    MBED_DEFINE_POSIX_ERROR( EUNATCH               ,EUNATCH         ),  /* 49      Protocol driver not attached */
    MBED_DEFINE_POSIX_ERROR( ENOCSI                ,ENOCSI          ),  /* 50      No CSI structure available */
    MBED_DEFINE_POSIX_ERROR( EL2HLT                ,EL2HLT          ),  /* 51      Level 2 halted */
    MBED_DEFINE_POSIX_ERROR( EBADE                 ,EBADE           ),  /* 52      Invalid exchange */
    MBED_DEFINE_POSIX_ERROR( EBADR                 ,EBADR           ),  /* 53      Invalid request descriptor */
    MBED_DEFINE_POSIX_ERROR( EXFULL                ,EXFULL          ),  /* 54      Exchange full */
    MBED_DEFINE_POSIX_ERROR( ENOANO                ,ENOANO          ),  /* 55      No anode */
    MBED_DEFINE_POSIX_ERROR( EBADRQC               ,EBADRQC         ),  /* 56      Invalid request code */
    MBED_DEFINE_POSIX_ERROR( EBADSLT               ,EBADSLT         ),  /* 57      Invalid slot */
    MBED_DEFINE_POSIX_ERROR( EDEADLOCK             ,EDEADLK         ),  /* EDEADLK Resource deadlock would occur */
    MBED_DEFINE_POSIX_ERROR( EBFONT                ,EBFONT          ),  /* 59      Bad font file format */
    MBED_DEFINE_POSIX_ERROR( ENOSTR                ,ENOSTR          ),  /* 60      Device not a stream */
    MBED_DEFINE_POSIX_ERROR( ENODATA               ,ENODATA         ),  /* 61      No data available */
    MBED_DEFINE_POSIX_ERROR( ETIME                 ,ETIME           ),  /* 62      Timer expired */
    MBED_DEFINE_POSIX_ERROR( ENOSR                 ,ENOSR           ),  /* 63      Out of streams resources */
    MBED_DEFINE_POSIX_ERROR( ENONET                ,ENONET          ),  /* 64      Machine is not on the network */
    MBED_DEFINE_POSIX_ERROR( ENOPKG                ,ENOPKG          ),  /* 65      Package not installed */
    MBED_DEFINE_POSIX_ERROR( EREMOTE               ,EREMOTE         ),  /* 66      Object is remote */
    MBED_DEFINE_POSIX_ERROR( ENOLINK               ,ENOLINK         ),  /* 67      Link has been severed */
    MBED_DEFINE_POSIX_ERROR( EADV                  ,EADV            ),  /* 68      Advertise error */
    MBED_DEFINE_POSIX_ERROR( ESRMNT                ,ESRMNT          ),  /* 69      Srmount error */
    MBED_DEFINE_POSIX_ERROR( ECOMM                 ,ECOMM           ),  /* 70      Communication error on send */
    MBED_DEFINE_POSIX_ERROR( EPROTO                ,EPROTO          ),  /* 71      Protocol error */
    MBED_DEFINE_POSIX_ERROR( EMULTIHOP             ,EMULTIHOP       ),  /* 72      Multihop attempted */
    MBED_DEFINE_POSIX_ERROR( EDOTDOT               ,EDOTDOT         ),  /* 73      RFS specific error */
    MBED_DEFINE_POSIX_ERROR( EBADMSG               ,EBADMSG         ),  /* 74      Not a data message */
    MBED_DEFINE_POSIX_ERROR( EOVERFLOW             ,EOVERFLOW       ),  /* 75      Value too large for defined data type */
    MBED_DEFINE_POSIX_ERROR( ENOTUNIQ              ,ENOTUNIQ        ),  /* 76      Name not unique on network */
    MBED_DEFINE_POSIX_ERROR( EBADFD                ,EBADFD          ),  /* 77      File descriptor in bad state */
    MBED_DEFINE_POSIX_ERROR( EREMCHG               ,EREMCHG         ),  /* 78      Remote address changed */
    MBED_DEFINE_POSIX_ERROR( ELIBACC               ,ELIBACC         ),  /* 79      Can not access a needed shared library */
    MBED_DEFINE_POSIX_ERROR( ELIBBAD               ,ELIBBAD         ),  /* 80      Accessing a corrupted shared library */
    MBED_DEFINE_POSIX_ERROR( ELIBSCN               ,ELIBSCN         ),  /* 81      .lib section in a.out corrupted */
    MBED_DEFINE_POSIX_ERROR( ELIBMAX               ,ELIBMAX         ),  /* 82      Attempting to link in too many shared libraries */
    MBED_DEFINE_POSIX_ERROR( ELIBEXEC              ,ELIBEXEC        ),  /* 83      Cannot exec a shared library directly */
    MBED_DEFINE_POSIX_ERROR( EILSEQ                ,EILSEQ          ),  /* 84      Illegal byte sequence */
    MBED_DEFINE_POSIX_ERROR( ERESTART              ,ERESTART        ),  /* 85      Interrupted system call should be restarted */
    MBED_DEFINE_POSIX_ERROR( ESTRPIPE              ,ESTRPIPE        ),  /* 86      Streams pipe error */
    MBED_DEFINE_POSIX_ERROR( EUSERS                ,EUSERS          ),  /* 87      Too many users */
    MBED_DEFINE_POSIX_ERROR( ENOTSOCK              ,ENOTSOCK        ),  /* 88      Socket operation on non-socket */
    MBED_DEFINE_POSIX_ERROR( EDESTADDRREQ          ,EDESTADDRREQ    ),  /* 89      Destination address required */
    MBED_DEFINE_POSIX_ERROR( EMSGSIZE              ,EMSGSIZE        ),  /* 90      Message too long */
    MBED_DEFINE_POSIX_ERROR( EPROTOTYPE            ,EPROTOTYPE      ),  /* 91      Protocol wrong type for socket */
    MBED_DEFINE_POSIX_ERROR( ENOPROTOOPT           ,ENOPROTOOPT     ),  /* 92      Protocol not available */
    MBED_DEFINE_POSIX_ERROR( EPROTONOSUPPORT       ,EPROTONOSUPPORT ),  /* 93      Protocol not supported */
    MBED_DEFINE_POSIX_ERROR( ESOCKTNOSUPPORT       ,ESOCKTNOSUPPORT ),  /* 94      Socket type not supported */
    MBED_DEFINE_POSIX_ERROR( EOPNOTSUPP            ,EOPNOTSUPP      ),  /* 95      Operation not supported on transport endpoint */
    MBED_DEFINE_POSIX_ERROR( EPFNOSUPPORT          ,EPFNOSUPPORT    ),  /* 96      Protocol family not supported */
    MBED_DEFINE_POSIX_ERROR( EAFNOSUPPORT          ,EAFNOSUPPORT    ),  /* 97      Address family not supported by protocol */
    MBED_DEFINE_POSIX_ERROR( EADDRINUSE            ,EADDRINUSE      ),  /* 98      Address already in use */
    MBED_DEFINE_POSIX_ERROR( EADDRNOTAVAIL         ,EADDRNOTAVAIL   ),  /* 99      Cannot assign requested address */
    MBED_DEFINE_POSIX_ERROR( ENETDOWN              ,ENETDOWN        ),  /* 100     Network is down */
    MBED_DEFINE_POSIX_ERROR( ENETUNREACH           ,ENETUNREACH     ),  /* 101     Network is unreachable */
    MBED_DEFINE_POSIX_ERROR( ENETRESET             ,ENETRESET       ),  /* 102     Network dropped connection because of reset */
    MBED_DEFINE_POSIX_ERROR( ECONNABORTED          ,ECONNABORTED    ),  /* 103     Software caused connection abort */
    MBED_DEFINE_POSIX_ERROR( ECONNRESET            ,ECONNRESET      ),  /* 104     Connection reset by peer */
    MBED_DEFINE_POSIX_ERROR( ENOBUFS               ,ENOBUFS         ),  /* 105     No buffer space available */
    MBED_DEFINE_POSIX_ERROR( EISCONN               ,EISCONN         ),  /* 106     Transport endpoint is already connected */
    MBED_DEFINE_POSIX_ERROR( ENOTCONN              ,ENOTCONN        ),  /* 107     Transport endpoint is not connected */
    MBED_DEFINE_POSIX_ERROR( ESHUTDOWN             ,ESHUTDOWN       ),  /* 108     Cannot send after transport endpoint shutdown */
    MBED_DEFINE_POSIX_ERROR( ETOOMANYREFS          ,ETOOMANYREFS    ),  /* 109     Too many references: cannot splice */
    MBED_DEFINE_POSIX_ERROR( ETIMEDOUT             ,ETIMEDOUT       ),  /* 110     Connection timed out */
    MBED_DEFINE_POSIX_ERROR( ECONNREFUSED          ,ECONNREFUSED    ),  /* 111     Connection refused */
    MBED_DEFINE_POSIX_ERROR( EHOSTDOWN             ,EHOSTDOWN       ),  /* 112     Host is down */
    MBED_DEFINE_POSIX_ERROR( EHOSTUNREACH          ,EHOSTUNREACH    ),  /* 113     No route to host */
    MBED_DEFINE_POSIX_ERROR( EALREADY              ,EALREADY        ),  /* 114     Operation already in progress */
    MBED_DEFINE_POSIX_ERROR( EINPROGRESS           ,EINPROGRESS     ),  /* 115     Operation now in progress */
    MBED_DEFINE_POSIX_ERROR( ESTALE                ,ESTALE          ),  /* 116     Stale NFS file handle */
    MBED_DEFINE_POSIX_ERROR( EUCLEAN               ,EUCLEAN         ),  /* 117     Structure needs cleaning */
    MBED_DEFINE_POSIX_ERROR( ENOTNAM               ,ENOTNAM         ),  /* 118     Not a XENIX named type file */
    MBED_DEFINE_POSIX_ERROR( ENAVAIL               ,ENAVAIL         ),  /* 119     No XENIX semaphores available */
    MBED_DEFINE_POSIX_ERROR( EISNAM                ,EISNAM          ),  /* 120     Is a named type file */
    MBED_DEFINE_POSIX_ERROR( EREMOTEIO             ,EREMOTEIO       ),  /* 121     Remote I/O error */
    MBED_DEFINE_POSIX_ERROR( EDQUOT                ,EDQUOT          ),  /* 122     Quota exceeded */
    MBED_DEFINE_POSIX_ERROR( ENOMEDIUM             ,ENOMEDIUM       ),  /* 123     No medium found */
    MBED_DEFINE_POSIX_ERROR( EMEDIUMTYPE           ,EMEDIUMTYPE     ),  /* 124     Wrong medium type */
    MBED_DEFINE_POSIX_ERROR( ECANCELED             ,ECANCELED       ),  /* 125     Operation Canceled */
    MBED_DEFINE_POSIX_ERROR( ENOKEY                ,ENOKEY          ),  /* 126     Required key not available */
    MBED_DEFINE_POSIX_ERROR( EKEYEXPIRED           ,EKEYEXPIRED     ),  /* 127     Key has expired */
    MBED_DEFINE_POSIX_ERROR( EKEYREVOKED           ,EKEYREVOKED     ),  /* 128     Key has been revoked */
    MBED_DEFINE_POSIX_ERROR( EKEYREJECTED          ,EKEYREJECTED    ),  /* 129     Key was rejected by service */
    MBED_DEFINE_POSIX_ERROR( EOWNERDEAD            ,EOWNERDEAD      ),  /* 130     Owner died */
    MBED_DEFINE_POSIX_ERROR( ENOTRECOVERABLE       ,ENOTRECOVERABLE ),  /* 131     State not recoverable */
    
    //Below are MBED SYSTEM ERROR CODE definitions
    //MBED SYSTEM ERROR CODE definitions starts at offset MBED_SYSTEM_ERROR_BASE, see above.
    //                   Error Name                 Error Offset   Error Code
    MBED_DEFINE_SYSTEM_ERROR( UNKNOWN                    ,0 ),          /* 256      Unknown error */
    MBED_DEFINE_SYSTEM_ERROR( INVALID_ARGUMENT           ,1 ),          /* 257      Invalid Argument */
    MBED_DEFINE_SYSTEM_ERROR( INVALID_DATA_DETECTED      ,2 ),          /* 258      Invalid data detected */
    MBED_DEFINE_SYSTEM_ERROR( INVALID_FORMAT             ,3 ),          /* 259      Invalid format */
    MBED_DEFINE_SYSTEM_ERROR( INVALID_INDEX              ,4 ),          /* 260      Invalid Index */
    MBED_DEFINE_SYSTEM_ERROR( INVALID_SIZE               ,5 ),          /* 261      Inavlid Size */
    MBED_DEFINE_SYSTEM_ERROR( INVALID_OPERATION          ,6 ),          /* 262      Invalid Operation */
    MBED_DEFINE_SYSTEM_ERROR( ITEM_NOT_FOUND             ,7 ),          /* 263      Item Not Found */
    MBED_DEFINE_SYSTEM_ERROR( ACCESS_DENIED              ,8 ),          /* 264      Access Denied */
    MBED_DEFINE_SYSTEM_ERROR( UNSUPPORTED                ,9 ),          /* 265      Unsupported */
    MBED_DEFINE_SYSTEM_ERROR( BUFFER_FULL                ,10 ),         /* 266      Buffer Full */
    MBED_DEFINE_SYSTEM_ERROR( MEDIA_FULL                 ,11 ),         /* 267      Media/Disk Full */
    MBED_DEFINE_SYSTEM_ERROR( ALREADY_IN_USE             ,12 ),         /* 268      Already in use */
    MBED_DEFINE_SYSTEM_ERROR( TIME_OUT                   ,13 ),         /* 269      Timeout error */
    MBED_DEFINE_SYSTEM_ERROR( NOT_READY                  ,14 ),         /* 270      Not Ready */
    MBED_DEFINE_SYSTEM_ERROR( FAILED_OPERATION           ,15 ),         /* 271      Requested Operation failed */
    MBED_DEFINE_SYSTEM_ERROR( OPERATION_PROHIBITED       ,16 ),         /* 272      Operation prohibited */
    MBED_DEFINE_SYSTEM_ERROR( OPERATION_ABORTED          ,17 ),         /* 273      Operation failed */
    MBED_DEFINE_SYSTEM_ERROR( WRITE_PROTECTED            ,18 ),         /* 274      Attempt to write to write-protected resource */
    MBED_DEFINE_SYSTEM_ERROR( NO_RESPONSE                ,19 ),         /* 275      No response */
    MBED_DEFINE_SYSTEM_ERROR( SEMAPHORE_LOCK_FAILED      ,20 ),         /* 276      Sempahore lock failed */
    MBED_DEFINE_SYSTEM_ERROR( MUTEX_LOCK_FAILED          ,21 ),         /* 277      Mutex lock failed */
    MBED_DEFINE_SYSTEM_ERROR( SEMAPHORE_UNLOCK_FAILED    ,22 ),         /* 278      Sempahore unlock failed */
    MBED_DEFINE_SYSTEM_ERROR( MUTEX_UNLOCK_FAILED        ,23 ),         /* 279      Mutex unlock failed */
    MBED_DEFINE_SYSTEM_ERROR( CRC_ERROR                  ,24 ),         /* 280      CRC error or mismatch */
    MBED_DEFINE_SYSTEM_ERROR( OPEN_FAILED                ,25 ),         /* 281      Open failed */
    MBED_DEFINE_SYSTEM_ERROR( CLOSE_FAILED               ,26 ),         /* 282      Close failed */
    MBED_DEFINE_SYSTEM_ERROR( READ_FAILED                ,27 ),         /* 283      Read failed */
    MBED_DEFINE_SYSTEM_ERROR( WRITE_FAILED               ,28 ),         /* 284      Write failed */
    MBED_DEFINE_SYSTEM_ERROR( INITIALIZATION_FAILED      ,29 ),         /* 285      Initialization failed */
    MBED_DEFINE_SYSTEM_ERROR( BOOT_FAILURE               ,30 ),         /* 286      Boot failure */
    MBED_DEFINE_SYSTEM_ERROR( OUT_OF_MEMORY              ,31 ),         /* 287      Out of memory */
    MBED_DEFINE_SYSTEM_ERROR( OUT_OF_RESOURCES           ,32 ),         /* 288      Out of resources */
    MBED_DEFINE_SYSTEM_ERROR( ALLOC_FAILED               ,33 ),         /* 289      Alloc failed */
    MBED_DEFINE_SYSTEM_ERROR( FREE_FAILED                ,34 ),         /* 290      Free failed */
    MBED_DEFINE_SYSTEM_ERROR( OVERFLOW                   ,35 ),         /* 291      Overflow error */
    MBED_DEFINE_SYSTEM_ERROR( UNDERFLOW                  ,36 ),         /* 292      Underflow error */
    MBED_DEFINE_SYSTEM_ERROR( STACK_OVERFLOW             ,37 ),         /* 293      Stack overflow error */
    MBED_DEFINE_SYSTEM_ERROR( ISR_QUEUE_OVERFLOW         ,38 ),         /* 294      ISR queue overflow */
    MBED_DEFINE_SYSTEM_ERROR( TIMER_QUEUE_OVERFLOW       ,39 ),         /* 295      Timer Queue overflow */
    MBED_DEFINE_SYSTEM_ERROR( CLIB_SPACE_UNAVAILABLE     ,40 ),         /* 296      Standard library error - Space unavailable */
    MBED_DEFINE_SYSTEM_ERROR( CLIB_EXCEPTION             ,41 ),         /* 297      Standard library error - Exception */
    MBED_DEFINE_SYSTEM_ERROR( CLIB_MUTEX_INIT_FAILURE    ,42 ),         /* 298      Standard library error - Mutex Init failure */
    MBED_DEFINE_SYSTEM_ERROR( CREATE_FAILED              ,43 ),         /* 299      Create failed */
    MBED_DEFINE_SYSTEM_ERROR( DELETE_FAILED              ,44 ),         /* 300      Delete failed */
    MBED_DEFINE_SYSTEM_ERROR( THREAD_CREATE_FAILED       ,45 ),         /* 301      Thread Create failed */
    MBED_DEFINE_SYSTEM_ERROR( THREAD_DELETE_FAILED       ,46 ),         /* 302      Thread Delete failed */
    MBED_DEFINE_SYSTEM_ERROR( PROHIBITED_IN_ISR_CONTEXT  ,47 ),         /* 303      Operation Prohibited in ISR context */
    MBED_DEFINE_SYSTEM_ERROR( PINMAP_INVALID             ,48 ),         /* 304      Pinmap Invalid */
    MBED_DEFINE_SYSTEM_ERROR( RTOS_EVENT                 ,49 ),         /* 305      Unknown Rtos Error */
    MBED_DEFINE_SYSTEM_ERROR( RTOS_THREAD_EVENT          ,50 ),         /* 306      Rtos Thread Error */
    MBED_DEFINE_SYSTEM_ERROR( RTOS_MUTEX_EVENT           ,51 ),         /* 307      Rtos Mutex Error */
    MBED_DEFINE_SYSTEM_ERROR( RTOS_SEMAPHORE_EVENT       ,52 ),         /* 308      Rtos Semaphore Error */
    MBED_DEFINE_SYSTEM_ERROR( RTOS_MEMORY_POOL_EVENT     ,53 ),         /* 309      Rtos Memory Pool Error */
    MBED_DEFINE_SYSTEM_ERROR( RTOS_TIMER_EVENT           ,54 ),         /* 310      Rtos Timer Error */
    MBED_DEFINE_SYSTEM_ERROR( RTOS_EVENT_FLAGS_EVENT     ,55 ),         /* 311      Rtos Event flags Error */
    MBED_DEFINE_SYSTEM_ERROR( RTOS_MESSAGE_QUEUE_EVENT   ,56 ),         /* 312      Rtos Message queue Error */
    MBED_DEFINE_SYSTEM_ERROR( DEVICE_BUSY                ,57 ),         /* 313      Device Busy */
    MBED_DEFINE_SYSTEM_ERROR( CONFIG_UNSUPPORTED         ,58 ),         /* 314      Configuration not supported */
    MBED_DEFINE_SYSTEM_ERROR( CONFIG_MISMATCH            ,59 ),         /* 315      Configuration mismatch */
    MBED_DEFINE_SYSTEM_ERROR( ALREADY_INITIALIZED        ,60 ),         /* 316      Already initialzied */
    MBED_DEFINE_SYSTEM_ERROR( HARDFAULT_EXCEPTION        ,61 ),         /* 317      HardFault exception */
    MBED_DEFINE_SYSTEM_ERROR( MEMMANAGE_EXCEPTION        ,62 ),         /* 318      MemManage exception */
    MBED_DEFINE_SYSTEM_ERROR( BUSFAULT_EXCEPTION         ,63 ),         /* 319      BusFault exception */
    MBED_DEFINE_SYSTEM_ERROR( USAGEFAULT_EXCEPTION       ,64 ),         /* 320      UsageFault exception*/
        
    //Everytime you add a new system error code, you must update
    //Error documentation under Handbook to capture the info on
    //the new error status/codes
    
    //MBED CUSTOM ERROR CODE definitions starts at offset MBED_CUSTOM_ERROR_BASE, see above.
    /* Add More/Custom Error Codes here, See example below */
    //DEFINE_CUSTOM_ERROR( MY_CUSTOM_ERROR     , 1 ),    
    
} mbed_error_code_t;

/** mbed_error_ctx struct
 *
 *  This struct captures the context information at the time of error.\n
 *  It primarily contains information about the thread where the error originated,\n
 *  filename/line number of the source file where the error occurred, a context specific error value(error_value)\n
 *  and the address where the error originated.\n
 *
 *  @note
 *  Below are the members of mbed_error_ctx struct\n
 *      error_status              mbed_error_status_t value for this error\n
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
    mbed_error_status_t error_status;
    uint32_t error_address;
    uint32_t error_value;
    uint32_t thread_id;
    uint32_t thread_entry_address;
    uint32_t thread_stack_size;
    uint32_t thread_stack_mem;
    uint32_t thread_current_sp;
#ifdef MBED_CONF_PLATFORM_MAX_ERROR_FILENAME_LEN
    char error_filename[MBED_CONF_PLATFORM_MAX_ERROR_FILENAME_LEN];
    uint32_t error_line_number;
#endif    
} mbed_error_ctx;

/** To generate a fatal compile-time error, you can use the pre-processor #error directive.
 *
 * @param format    C string that contains data stream to be printed.
 *                  Code snippets below show valid format.
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
    
void error(const char* format, ...);

/**
 * Call this Macro to generate a mbed_error_status_t value for a System error
 * @param  module           Module generating the error code. If its unknown, pass MBED_MODULE_UNKNOWN. See mbed_module_type_t for module types.
 * @param  error_code       The mbed_error_code_t code to be used in generating the mbed_error_status_t. See mbed_error_code_t for error codes.
 *
 * @code
 * 
 * mbed_error_status_t driver_error = MBED_MAKE_SYSTEM_ERROR( MODULE_DRIVER_USB, MBED_ERROR_CODE_INITIALIZATION_FAILED )
 *
 * @endcode
 * @note This macro generate mbed_error_status_t-es with error type set to MBED_ERROR_TYPE_SYSTEM
 *
 */
#define MBED_MAKE_SYSTEM_ERROR(module, error_code)                   MAKE_MBED_ERROR(MBED_ERROR_TYPE_SYSTEM, module, error_code)

/**
 * Call this Macro to generate a mbed_error_status_t value for a Custom error
 * @param  module           Module generating the error code. If its unknown, pass MBED_MODULE_UNKNOWN. See mbed_module_type_t for module types.
 * @param  error_code       The mbed_error_code_t code to be used in generating the mbed_error_status_t. See mbed_error_code_t for error codes.
 *
 * @code
 * 
 * mbed_error_status_t custom_error = MBED_MAKE_CUSTOM_ERROR( MBED_MODULE_APPLICATION, 0xDEAD//16-bit custom error code )
 *
 * @endcode
 * @note This macro generate mbed_error_status_t-es with error type set to MBED_ERROR_TYPE_CUSTOM
 *
 */
#define MBED_MAKE_CUSTOM_ERROR(module, error_code)                   MAKE_MBED_ERROR(MBED_ERROR_TYPE_CUSTOM, module, error_code)

/**
 * Call this Macro to generate a mbed_error_status_t value for a System error
 * @param  module           Module generating the error code. If its unknown, pass MBED_MODULE_UNKNOWN. See mbed_module_type_t for module types.
 * @param  error_code       The mbed_error_code_t code to be used in generating the mbed_error_status_t. See mbed_error_code_t for error codes.
 *
 * @code
 * 
 * mbed_error_status_t new_error = MBED_MAKE_ERROR( MODULE_DRIVER_USB, MBED_ERROR_INITIALIZATION_FAILED )
 *
 * @endcode
 * @note This macro generate mbed_error_status_t-es with error type set to MBED_ERROR_TYPE_SYSTEM
 *
 */
#define MBED_MAKE_ERROR(module, error_code)                          MBED_MAKE_SYSTEM_ERROR(module, error_code)

/**
 * Callback/Error hook function prototype. Applications needing a callback when an error is reported can use mbed_set_error_hook function
 * to register a callback/error hook function using the following prototype. When an error happens in the system error handling
 * implementation will invoke this callback with the mbed_error_status_t reported and the error context at the time of error.
 * @param  error_status     mbed_error_status_t status being reported.
 * @param  error_ctx        Error context structure associated with this error.
 * @return                  void
 *
 */
typedef void (*mbed_error_hook_t)(const mbed_error_ctx *error_ctx);

/**
 * Call this function to set a system error/warning. This function will log the error status with the context info and return to caller.
 *
 * @param  error_status     mbed_error_status_t status to be set(See mbed_error_status_t enum above for available error status values).
 * @param  error_msg        The error message to be printed out to STDIO/Serial.
 * @param  error_value      Value associated with the error status. This would depend on error code/error scenario.
 * @param  filename         Name of the source file originating the error( Most callers can pass __FILE__ here ).
 * @param  line_number      The line number of the source file originating the error( Most callers can pass __LINE__ here ) .
 * @return                  0 or MBED_SUCCESS.
 *                          MBED_ERROR_INVALID_ARGUMENT if called with invalid error status/codes 
 *
 * @code
 * 
 * mbed_error( ERROR_OUT_OF_MEMORY, "Out of memory error", 0, __FILE__, __LINE__ )
 *
 * @endcode
 *
 * @note See MBED_WARNING/MBED_ERROR macros which provides a wrapper on this API
 */
mbed_error_status_t mbed_warning(mbed_error_status_t error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number);

/**
 * Returns the first system error reported.
 * @return                  mbed_error_status_t code logged for the first error or MBED_SUCCESS if no errors are logged.
 *
 */
mbed_error_status_t mbed_get_first_error(void);

/**
 * Returns the most recent system error reported.
 * @return                  mbed_error_status_t code logged for the last error or MBED_SUCCESS if no errors are logged.
 *
 */
mbed_error_status_t mbed_get_last_error(void);

/**
 * Returns the number of system errors reported after boot.
 * @return                  int Number of errors reported.
 *
 */
int mbed_get_error_count(void);

/**
 * Call this function to set a fatal system error and halt the system. This function will log the fatal error with the context info and prints the error report and halts the system.
 * 
 * @param  error_status     mbed_error_status_t status to be set(See mbed_error_status_t enum above for available error status values).
 * @param  error_msg        The error message to be printed out to STDIO/Serial.
 * @param  error_value      Value associated with the error status. This would depend on error code/error scenario.
 * @param  filename         Name of the source file originating the error( Most callers can pass __FILE__ here ).
 * @param  line_number      The line number of the source file originating the error( Most callers can pass __LINE__ here ) .
 * @return                  0 or MBED_SUCCESS.
 *                          MBED_ERROR_INVALID_ARGUMENT if called with invalid error status/codes 
 *
 * @code
 * 
 * mbed_error( MBED_ERROR_PROHIBITED_OPERATION, "Prohibited operation tried", 0, __FILE__, __LINE__ )
 *
 * @endcode
 *
 * @note See MBED_WARNING/MBED_ERROR macros which provides a wrapper on this API
 */
mbed_error_status_t mbed_error(mbed_error_status_t error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number);

/**
 * Registers an application defined error callback with the error handling system. 
 * This function will be called with error context info whenever system handles a mbed_error/mbed_warning call
 * NOTE: This function should be implemented for re-entrancy as multiple threads may invoke mbed_error which may cause error hook to be called.
 * @param  custom_error_hook    mbed_error_status_t status to be set(See mbed_error_status_t enum above for available error status values).
 * @return                      0 or MBED_SUCCESS on success.
 *                              MBED_ERROR_INVALID_ARGUMENT in case of NULL for custom_error_hook
 *
 * @code
 * 
 * mbed_error_status_t my_custom_error_hook(mbed_error_status_t error_status, const mbed_error_ctx *error_ctx) {
 *    //Do something with the error_status or error_ctx
 * }
 *
 * mbed_set_error_hook( my_custom_error_hook )
 *
 * @endcode
 * @note The erro hook function implementation should be re-entrant.
 *
 */
mbed_error_status_t mbed_set_error_hook(mbed_error_hook_t custom_error_hook);

/**
 * Reads the first error context information captured.
 * @param  error_info           This is the mbed_error_context info captured as part of the first mbed_error call. The caller should pass a pointer to mbed_error_context struct allocated by the caller.
 * @return                      0 or MBED_SUCCESS on success.
 *                              MBED_ERROR_INVALID_ARGUMENT in case of invalid index
 *
 */
mbed_error_status_t mbed_get_first_error_info(mbed_error_ctx *error_info);

/**
 * Reads the last error context information captured.
 * @param  error_info           This is the mbed_error_context info captured as part of the last mbed_error call. The caller should pass a pointer to mbed_error_context struct allocated by the caller.
 * @return                      0 or MBED_ERROR_SUCCESS on success.
 *                              MBED_ERROR_INVALID_ARGUMENT in case of invalid index
 *
 */
mbed_error_status_t mbed_get_last_error_info(mbed_error_ctx *error_info);

/**
 * Clears the last error, first error, error count and all entries in the error history.
 * @return                      0 or MBED_SUCCESS on success.
 *
 */
mbed_error_status_t mbed_clear_all_errors(void);

/**
 * Generates a mbed_error_status_t value based on passed in values for type, module and error code.
 * @param  error_type           Error type based on mbed_error_type_t enum.
 * @param  module               Module type based on mbed_module_type_t enum.
 * @param  error_code           Error codes defined by mbed_error_code_t enum
 * @return                      0 or MBED_ERROR_SUCCESS on success.
 *
 */
mbed_error_status_t mbed_make_error(mbed_error_type_t error_type, mbed_module_type_t module, mbed_error_code_t error_code);

/**
 * Returns the current number of entries in the error history, if there has been more than max number of errors logged the number returned will be max depth of error history.
 * @return                      Current number of entries in the error history.
 *
 */
int mbed_get_error_hist_count(void);

/**
 * Reads the error context information for a specific error from error history, specified by the index.
 * 
 * @param  index                index of the error context entry in the history to be retrieved.\n
 *                              The number of entries in the error history is configured during build and the max index depends on max depth of error history.\n
 *                              index = 0 points to the oldest entry in the history, and index = (max history depth - 1) points to the latest entry in the error history.\n
 * @param  error_info           This is the mbed_error_context info captured as part of the error history. The caller should pass a pointer to mbed_error_context struct allocated by the caller.
 * @return                      0 or MBED_SUCCESS on success.
 *                              MBED_ERROR_INVALID_ARGUMENT in case of invalid index
 *
 */
mbed_error_status_t mbed_get_error_hist_info(int index, mbed_error_ctx *error_info);

/**
 * Saves the error history information to a file
 * 
 * @param  path                 path to the file in the filesystem
 * @return                      0 or MBED_ERROR_SUCCESS on success.
 *                              MBED_ERROR_WRITE_FAILED if writing to file failed
 *                              MBED_ERROR_INVALID_ARGUMENT if path is not valid 
 *
 * @note                        Filesystem support is required in order for this function to work.
 *
 */
mbed_error_status_t mbed_save_error_hist(const char *path);

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
/** @}*/


