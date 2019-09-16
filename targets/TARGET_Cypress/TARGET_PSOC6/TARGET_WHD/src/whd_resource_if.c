/*
 * Copyright 2019 Cypress Semiconductor Corporation
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
 *  Provides generic APSTA functionality that chip specific files use
 */

#include "whd_debug.h"
#include "whd_int.h"
#include "whd_resource_if.h"

/******************************************************
 *  * @cond               Constants
 *   ******************************************************/

/******************************************************
 *  *                   Enumerations
 *   ******************************************************/

/******************************************************
 *  *               Function Declarations
 *   ******************************************************/

/******************************************************
 *        Variables Definitions
 *****************************************************/

/******************************************************
*               Function Definitions
******************************************************/
uint32_t whd_resource_size(whd_driver_t whd_driver, whd_resource_type_t resource, uint32_t *size_out)
{
    if (whd_driver->resource_if->whd_resource_size)
    {
        return whd_driver->resource_if->whd_resource_size(whd_driver, resource, size_out);
    }
    else
    {
        WPRINT_WHD_ERROR( ("Function pointers not provided .\n") );
    }

    return WHD_WLAN_NOFUNCTION;
}

uint32_t whd_get_resource_block_size(whd_driver_t whd_driver, whd_resource_type_t type, uint32_t *size_out)
{

    if (whd_driver->resource_if->whd_get_resource_block_size)
    {
        return whd_driver->resource_if->whd_get_resource_block_size(whd_driver, type, size_out);
    }
    else
    {
        WPRINT_WHD_ERROR( ("Function pointers not provided .\n") );
    }

    return WHD_WLAN_NOFUNCTION;
}

uint32_t whd_get_resource_no_of_blocks(whd_driver_t whd_driver, whd_resource_type_t type, uint32_t *block_count)
{
    if (whd_driver->resource_if->whd_get_resource_no_of_blocks)
    {
        return whd_driver->resource_if->whd_get_resource_no_of_blocks(whd_driver, type, block_count);
    }
    else
    {
        WPRINT_WHD_ERROR( ("Function pointers not provided .\n") );
    }

    return WHD_WLAN_NOFUNCTION;
}

uint32_t whd_get_resource_block(whd_driver_t whd_driver, whd_resource_type_t type,
                                uint32_t blockno, const uint8_t **data, uint32_t *size_out)
{

    if (whd_driver->resource_if->whd_get_resource_block)
    {
        return whd_driver->resource_if->whd_get_resource_block(whd_driver, type, blockno, data, size_out);
    }
    else
    {
        WPRINT_WHD_ERROR( ("Function pointers not provided .\n") );
    }

    return WHD_WLAN_NOFUNCTION;
}

