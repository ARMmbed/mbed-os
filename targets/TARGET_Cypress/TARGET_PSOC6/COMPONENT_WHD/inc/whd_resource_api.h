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

/** @file whd_resource_api.h
 *  Prototypes of functions for providing external resources to the radio driver
 *
 *  This file provides prototypes for functions which allow
 *  WHD to download firmware, NVRAM and CLM BLOB on a particular hardware platform.
 *
 */
#include "whd.h"

#ifndef INCLUDED_WHD_RESOURCE_API_H_
#define INCLUDED_WHD_RESOURCE_API_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define BLOCK_SIZE 1024 /**< Size of the block */
/**
 * Type of resources
 */
typedef enum
{
    WHD_RESOURCE_WLAN_FIRMWARE, /**< Resource type: WLAN Firmware */
    WHD_RESOURCE_WLAN_NVRAM,    /**< Resource type: NVRAM file */
    WHD_RESOURCE_WLAN_CLM,      /**< Resource type: CLM_BLOB file */
} whd_resource_type_t;

/******************************************************
*                 Global Variables
******************************************************/

/** @addtogroup res WHD Resource API
 *  @brief Functions that enable WHD to download WLAN firmware, NVRAM and CLM BLOB on a particular hardware platform.
 *  @{
 */

/**
 * Interface to a data source that provides external resources to the radio driver
 */

/** This data structure defines a source for data generally intended to be downloaded to the radio device.
 *
 * The data is assumed to be available as a set of blocks that are all the same size with the exception
 * of the last block. The whd_get_resource_block_size function returns this block size. The whd_get_resource_block call
 * returns a pointer to a block of data. The actual storage for the data block is owned by the data source, so only a pointer
 * to the block is returned. There are two predominate use cases. If the data is stored in the internal
 * flash memory, then whd_get_resource_no_of_blocks will return 1 and a call to whd_get_resource_block will return a pointer to
 * the data image with the size being the size of the data image. If the data is stored in an external flash of some
 * type, each block of data can be read from the external flash one at a time.  whd_get_resource_no_of_blocks will return
 * the physical number of blocks in the data and each call to whd_get_resource_block will read data from the external memory
 * and make it available via an internal buffer.
 */
struct whd_resource_source
{
    /** Gets the size of the resource for respective resource type
     *
     *
     *  @param whd_drv     Pointer to handle instance of the driver
     *  @param resource    Type of resource - WHD_RESOURCE_WLAN_FIRMWARE, WHD_RESOURCE_WLAN_NVRAM, WHD_RESOURCE_WLAN_CLM
     *  @param size_out    Size of the resource
     *
     *  @return            WHD_SUCCESS or error code
     *
     */
    uint32_t (*whd_resource_size)(whd_driver_t whd_drv, whd_resource_type_t resource, uint32_t *size_out);

    /** Gets the resource block for specified resource type
     *
     *  @param whd_drv     Pointer to handle instance of the driver
     *  @param type        Type of resource - WHD_RESOURCE_WLAN_FIRMWARE, WHD_RESOURCE_WLAN_NVRAM, WHD_RESOURCE_WLAN_CLM
     *  @param blockno     The number of block
     *  @param data        Pointer to a block of data
     *  @param size_out    Size of the resource
     *
     *  @return            WHD_SUCCESS or error code
     *
     */
    uint32_t (*whd_get_resource_block)(whd_driver_t whd_drv, whd_resource_type_t type,
                                       uint32_t blockno, const uint8_t **data, uint32_t *size_out);

    /** Gets block count for the specified resource_type
     *
     *  @param whd_drv      Pointer to handle instance of the driver
     *  @param type         Type of resource - WHD_RESOURCE_WLAN_FIRMWARE, WHD_RESOURCE_WLAN_NVRAM, WHD_RESOURCE_WLAN_CLM
     *  @param block_count  Pointer to store block count for the resource
     *
     *  @return             WHD_SUCCESS or error code
     *
     */
    uint32_t (*whd_get_resource_no_of_blocks)(whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *block_count);

    /** Gets block size for the specified resource_type
     *
     *  @param whd_drv      Pointer to handle instance of the driver
     *  @param type         Type of resources - WHD_RESOURCE_WLAN_FIRMWARE, WHD_RESOURCE_WLAN_NVRAM, WHD_RESOURCE_WLAN_CLM
     *  @param size_out     Pointer to store size of the block
     *
     *  @return             WHD_SUCCESS or error code
     *
     */
    uint32_t (*whd_get_resource_block_size)(whd_driver_t whd_drv, whd_resource_type_t type, uint32_t *size_out);
};

/** @} */

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef INCLUDED_WHD_RESOURCE_API_H_ */

