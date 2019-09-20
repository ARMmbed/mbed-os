/***************************************************************************//**
* \file cyabs_resource.h
*
* \brief
* Basic abstraction layer for dealing with resources.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
*******************************************************************************/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** 
  * \addtogroup group_abstraction_resource Resource Abstraction 
  * \{
  * 
  * Basic abstraction layer for dealing with resources.
  */

/** Error code for when the specified resource operation is not valid. */
#define CY_RSLT_RSC_ERROR_UNSUPPORTED (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_RESOURCE, 0))
/** Error indicating that memory for the specified resource could not be allocated. */
#define CY_RSLT_RSC_ERROR_UNAVAILABLE (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_RESOURCE, 1))
/** Error code for when the specified resource offset is not valid for the specified resource. */
#define CY_RSLT_RSC_ERROR_OFFSET (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_RESOURCE, 2))
/** Error code for when the specified resource can not be opened for reading. */
#define CY_RSLT_RSC_ERROR_OPEN (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_RESOURCE, 3))
/** Error code for when the specified location can not be accessed in the specified resource. */
#define CY_RSLT_RSC_ERROR_SEEK (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_RESOURCE, 4))
/** Error code for when the specified resource can not be read. */
#define CY_RSLT_RSC_ERROR_READ (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_RESOURCE, 5))

/** Different types of memory that the resources can be stored in. */
typedef enum
{
    CY_RESOURCE_IN_MEMORY,           /**< resource location in memory           */
    CY_RESOURCE_IN_FILESYSTEM,       /**< resource location in filesystem       */
    CY_RESOURCE_IN_EXTERNAL_STORAGE  /**< resource location in external storage */
} cy_resource_location_t;

/** Filesystem handle */ 
typedef struct
{
    unsigned long offset;      /**< Offset to the start of the resource */
    const char* filename;      /**< name of the resource                */
} cy_filesystem_resource_handle_t;

/** Resource handle structure */
typedef struct
{
    cy_resource_location_t location;     /**< resource location */
    unsigned long size;                  /**< resource size     */
    union
    {
        cy_filesystem_resource_handle_t fs;                       /**< handle for resource in filesystem */
        const uint8_t*                  mem_data;                 /**< handle for resource in internal memory */
        void*                           external_storage_context; /**< handle for resource in external storage */
    } val; /**< low-level handle (type varies depending on resource storage location) */
} cy_resource_handle_t;

/**
 * \brief return the block size for the resource
 * \param handle handle to a resource
 * \param size pointer to a uint32_t to receive the size
 * \returns CY_RSLT_SUCCESS if the size was found, otherwise an error
 */
cy_rslt_t cy_resource_get_block_size(const cy_resource_handle_t *handle, uint32_t *size);

/**
 * \brief return the total size for the resource
 * \param handle handle to a resource
 * \param size pointer to a uint32_t to receive the size
 * \returns CY_RSLT_SUCCESS if the size was found, otherwise an error
 */
cy_rslt_t cy_resource_get_total_size(const cy_resource_handle_t *handle, uint32_t *size);

/**
 * \brief return the total number of blocks that exist for the resource
 * \param handle handle to a resource
 * \param blocks pointer to a uint32_t to receive the count
 * \returns CY_RSLT_SUCCESS if the count was found, otherwise an error
 */
cy_rslt_t cy_resource_get_block_count(const cy_resource_handle_t *handle, uint32_t *blocks);

/**
 * \brief read data from a resource
 * \param handle the handle to the resource
 * \param blockno the block number of the data from the resource
 * \param buffer pointer to receive buffer address from resource. The buffer must be freed when done. Eg: free(buffer)
 * \param size location to receive the size of the block read
 * \returns CY_RSLT_SUCCESS if data read, otherwise an error
 */
cy_rslt_t cy_resource_read(const cy_resource_handle_t *handle, uint32_t blockno, uint8_t **buffer, uint32_t *size);

/**
 * \brief optimized version of read for resources stored in memory 
 * \see CY_RESOURCE_IN_MEMORY
 * \param handle the handle to the resource
 * \param buffer pointer to receive buffer address from resource. This does NOT need to be freed.
 * \param size location to receive the size of the block read
 * \returns CY_RSLT_SUCCESS if data read, otherwise an error
 */
cy_rslt_t cy_resource_readonly_memory(const cy_resource_handle_t *handle, const uint8_t **buffer, uint32_t *size);

#if defined(__cplusplus)
}
#endif

/** \} group_abstraction_resource */
