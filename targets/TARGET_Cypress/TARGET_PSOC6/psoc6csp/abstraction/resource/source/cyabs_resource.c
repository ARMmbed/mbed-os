/***************************************************************************//**
* \file cyabs_resource.c
*
* \brief
* Provides API for reading data from resource files however they are stored.
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cyabs_resource.h"
#include "cy_utils.h"

#if defined(__cplusplus)
extern "C" {
#endif

cy_rslt_t cy_resource_get_block_size(const cy_resource_handle_t *handle, uint32_t *size)
{
    switch (handle->location)
    {
        case CY_RESOURCE_IN_MEMORY:
            *size = handle->size;
            return CY_RSLT_SUCCESS;
        case CY_RESOURCE_IN_FILESYSTEM:
        case CY_RESOURCE_IN_EXTERNAL_STORAGE:
            //TODO: implement
        default:
            CY_ASSERT(false);
            *size = 0;
            return CY_RSLT_RSC_ERROR_UNSUPPORTED;
    }
}

cy_rslt_t cy_resource_get_total_size(const cy_resource_handle_t *handle, uint32_t *size)
{
    *size = handle->size;
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cy_resource_get_block_count(const cy_resource_handle_t *handle, uint32_t *blocks)
{
    uint32_t block_size;
    uint32_t total_size = handle->size;
    cy_rslt_t rslt = cy_resource_get_block_size(handle, &block_size);
    if (rslt == CY_RSLT_SUCCESS && block_size > 0)
    {
        *blocks = (total_size + block_size - 1) / block_size;
    }
    else
    {
        *blocks = 0;
    }
    return rslt;
}

cy_rslt_t cy_resource_read(const cy_resource_handle_t *handle, uint32_t blockno, uint8_t **buffer, uint32_t *size)
{
    cy_rslt_t result = cy_resource_get_block_size(handle, size);
    if (CY_RSLT_SUCCESS != result)
    {
        return result;
    }

    void *p = malloc(*size);
    if (!p)
    {
        return CY_RSLT_RSC_ERROR_UNAVAILABLE;
    }

    *buffer = p;
    switch (handle->location)
    {
        case CY_RESOURCE_IN_MEMORY:
            CY_ASSERT(0 == blockno);
            memcpy(*buffer, handle->val.mem_data, *size);
            return CY_RSLT_SUCCESS;
        case CY_RESOURCE_IN_FILESYSTEM:
        case CY_RESOURCE_IN_EXTERNAL_STORAGE:
            //TODO: implement
        default:
            free(p);
            CY_ASSERT(false);
            *size = 0;
            return CY_RSLT_RSC_ERROR_UNSUPPORTED;
    }
}

cy_rslt_t cy_resource_readonly_memory(const cy_resource_handle_t *handle, const uint8_t **buffer, uint32_t *size)
{
    if (CY_RESOURCE_IN_MEMORY == handle->location)
    {
        *buffer = handle->val.mem_data;
        *size = handle->size;
        return CY_RSLT_SUCCESS;
    }
    else
    {
        return CY_RSLT_RSC_ERROR_UNSUPPORTED;
    }
}

#if defined(__cplusplus)
}
#endif
