/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company)
 * SPDX-License-Identifier: Apache-2.0
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

/** @file
 *  WICED Resource API's
 *  The Resource Management functions reads resource from a resource location
 *  and returns the number of bytes from an offset in an caller filled buffer.
 *
 *  Functions to get the resource size and resource data
 *
 *  The Resource could be one of the three locations
 *
 *  - Wiced Filesystem (File System)
 *  - Internal Memory  (Embedded Flash memory)
 *  - External Storage ( External Flash connected via SPI interface)
 *
 */

#ifndef INCLUDED_RESOURCE_H_
#define INCLUDED_RESOURCE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************
*                     Macros
******************************************************/
#ifndef MIN
#define MIN(x, y) ( (x) < (y) ? (x) : (y) )
#endif /* ifndef MIN */

/* Suppress unused parameter warning */
#ifndef UNUSED_PARAMETER
#define UNUSED_PARAMETER(x) ( (void)(x) )
#endif

#ifndef RESULT_ENUM
#define RESULT_ENUM(prefix, name, value)  prefix ## name = (value)
#endif /* ifndef RESULT_ENUM */

#if defined(CY_SECTION)
#define CY_SECTION_WHD          CY_SECTION
#else
#if !defined(CY_SECTION_WHD)
#if defined(__ARMCC_VERSION)
#define CY_SECTION_WHD(name)    __attribute__ ( (section(name) ) )
#elif defined (__GNUC__)
#if defined (__clang__)
#define CY_SECTION_WHD(name)    __attribute__ ( (section("__DATA, "name) ) )
#else
#define CY_SECTION_WHD(name)    __attribute__ ( (section(name) ) )
#endif
#elif defined (__ICCARM__)
#define CY_PRAGMA_WHD(x)        _Pragma(#x)
#define CY_SECTION_WHD(name)    CY_PRAGMA_WHD(location = name)
#else
#error "An unsupported toolchain"
#endif /* (__ARMCC_VERSION) */
#endif /* !defined(CY_SECTION_WHD) */
#endif /* defined(CY_SECTION) */

/* These Enum result values are for Resource errors
 * Values: 4000 - 4999
 */
#define RESOURCE_RESULT_LIST(prefix)  \
    RESULT_ENUM(prefix, SUCCESS,                         0),      /**< Success */                           \
    RESULT_ENUM(prefix, UNSUPPORTED,                     7),      /**< Unsupported function */              \
    RESULT_ENUM(prefix, OFFSET_TOO_BIG,               4001),      /**< Offset past end of resource */       \
    RESULT_ENUM(prefix, FILE_OPEN_FAIL,               4002),      /**< Failed to open resource file */      \
    RESULT_ENUM(prefix, FILE_SEEK_FAIL,               4003),      /**< Failed to seek to requested offset in resource file */ \
    RESULT_ENUM(prefix, FILE_READ_FAIL,               4004),      /**< Failed to read resource file */

#define resource_get_size(resource) ( (resource)->size )

/******************************************************
*                    Constants
******************************************************/

#define RESOURCE_ENUM_OFFSET  (1300)

/******************************************************
*                Enumerations
******************************************************/

/**
 * Result type for WICED Resource function
 */
typedef enum
{
    RESOURCE_RESULT_LIST(RESOURCE_)
} resource_result_t;

/******************************************************
*                 Type Definitions
******************************************************/

typedef const void *resource_data_t;
typedef unsigned long resource_size_t;

/******************************************************
*                   Structures
******************************************************/

/**
 * Memory handle
 */
typedef struct
{
    const char *data;          /**< resource data */
} memory_resource_handle_t;

/**
 * Filesystem handle
 */
typedef struct
{
    unsigned long offset;      /**< Offset to the start of the resource */
    const char *filename;      /**< name of the resource                */
} filesystem_resource_handle_t;


typedef enum
{
    RESOURCE_IN_MEMORY,           /**< resource location in memory           */
    RESOURCE_IN_FILESYSTEM,       /**< resource location in filesystem       */
    RESOURCE_IN_EXTERNAL_STORAGE  /**< resource location in external storage */
} resource_location_t;

/**
 * Resource handle structure
 */
typedef struct
{
    resource_location_t location;       /**< resource location */
    unsigned long size;                 /**< resource size     */
    union
    {
        filesystem_resource_handle_t fs;                      /** < filesystem resource handle */
        memory_resource_handle_t mem;                         /** < memory resource handle     */
        void *external_storage_context;                       /** < external storage context   */
    } val;
} resource_hnd_t;

/******************************************************
*                 Global Variables
******************************************************/

/******************************************************
*               Function Declarations
******************************************************/

/*****************************************************************************/
/** @addtogroup  resourceapi  Wiced Resource Management API's
 *  @ingroup framework
 *
 *  WCIED Resource Management API's has functions to get the
 *  resource size and reads resource data from a resource
 *  location and returns the number of bytes in an caller
 *  filled buffer
 *
 *  The Resource could be one of the three locations
 *
 *  - Wiced Filesystem ( File System)
 *  - Internal Memory   (Embedded Flash memory)
 *  - External Storage  ( External Flash connected via SPI interface )
 *
 *  @{
 */
/*****************************************************************************/

/** Read resource using the handle specified
 *
 * @param[in]  resource : handle of the resource to read
 * @param[in]  offset   : offset from the beginning of the resource block
 * @param[in]  maxsize  : size of the buffer
 * @param[out] size     : size of the data successfully read
 * @param[in]  buffer   : pointer to a buffer to contain the read data
 *
 * @return @ref resource_result_t
 */
extern resource_result_t resource_read(const resource_hnd_t *resource, uint32_t offset, uint32_t maxsize,
                                       uint32_t *size, void *buffer);

/** Retrieve a read only resource buffer using the handle specified
 *
 * @param[in]  resource : handle of the resource to read
 * @param[in]  offset   : offset from the beginning of the resource block
 * @param[in]  maxsize  : size of the buffer
 * @param[out] size     : size of the data successfully read
 * @param[out] buffer   : pointer to a buffer pointer to point to the resource data
 *
 * @return @ref resource_result_t
 */
extern resource_result_t resource_get_readonly_buffer(const resource_hnd_t *resource, uint32_t offset, uint32_t maxsize,
                                                      uint32_t *size_out, const void **buffer);

/** Free a read only resource buffer using the handle specified
 *
 * @param[in]  resource : handle of the resource to read
 * @param[in]  buffer   : pointer to a buffer set using resource_get_readonly_buffer
 *
 * @return @ref resource_result_t
 */
extern resource_result_t resource_free_readonly_buffer(const resource_hnd_t *handle, const void *buffer);
/* @} */
#ifdef __cplusplus
} /*extern "C" */
#endif

#endif /* ifndef INCLUDED_RESOURCE_H_ */

