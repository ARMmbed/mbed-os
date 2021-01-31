/***************************************************************************//**
* \file cy_ext_wifi_fw_reserved_region_bd.h
*
* \brief
* Block device use to interact with external memory without interfering with
* the region reserved for the WiFi firmware.
*
* Creates a canonical instance that can be accessed via cy_get_ext_wifi_fw_reserved_region_bd().
* Provides overrides for Mbed OS functions so that storage mechanisms such as KVStore
* will default to using this block device.
*
* See cy_get_ext_wifi_fw_reserved_region_bd() for more details.
*
********************************************************************************
* \copyright
* Copyright 2020 Cypress Semiconductor Corporation
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

#ifndef CY_EXT_WIFI_FW_RESERVED_REGION_BD_H
#define CY_EXT_WIFI_FW_RESERVED_REGION_BD_H

// Use of xip-enable by user overrides definition of CY_EXT_WIFI_FW_STORAGE in target configuration
#if defined(CY_EXT_WIFI_FW_STORAGE) && !MBED_CONF_TARGET_XIP_ENABLE

#include "CyReservedRegionBlockDevice.h"

/**
 * Get the canonical instance of the underlying block device used by the canonical
 * CyReservedRegionBlockDevice instance (retrieved via cy_get_ext_wifi_fw_reserved_region_bd()).
 * Uses the macro CY_EXT_WIFI_FW_STORAGE to determine the type of the underlying block device.
 * Valid values of CY_EXT_WIFI_FW_STORAGE are: QSPIF.
 *
 * @return A pointer to the canonical instance of the underlying block device
 */
BlockDevice *cy_get_ext_wifi_fw_reserved_region_underlying_bd();

/**
 * Get the canonical instance of the block device used for interacting with external memory
 * without interfering with the region reserved for the WiFi firmware.
 *
 * This block device and the overrides that use it will only be defined if the macro CY_EXT_WIFI_FW_STORAGE is defined.
 * If it is, then its value is used by to determine the default block device for accessing
 * the external storage where the WiFi firmware is located.
 * See cy_get_ext_wifi_fw_reserved_region_underlying_bd() for more details.
 *
 * In order to store the WiFi firmware in external storage at all, the macro CY_ENABLE_XIP_PROGRAM must be defined.
 * Further, the macro CY_STORAGE_WIFI_DATA must be set to the name of the input section where the WiFi firmware is
 * to be placed, with surrounding quotation marks. The macro CY_STORAGE_WIFI_DATA_OUTPUT must be set to the name of the
 * output section that the linker script places the input section in, without quotation marks. The name of this output
 * section must not contain a '.' character.
 *
 * Using XIP by adding the xip-enable configuration override will prevent any of this from begin defined.
 *
 * @return A pointer to the canonical instance of the block device
 */
CyReservedRegionBlockDevice *cy_get_ext_wifi_fw_reserved_region_bd();

/**
 * Modify the location and size of the WiFi firmware image/CLM blob.
 *
 * If the WiFi firmware is ever updated, this function must be called afterward as well as
 * on every startup as long as the updated firmware continues to be used.
 * The defaults are the initial locations and sizes on compilation.
 * New firmware end should not exceed the initial end of the WiFi firmware output section
 * so that storage mechanisms keep a consistent storage region.
 *
 * @param image_addr[in]    New address of the WiFi firmware image
 * @param image_size[in]    New size of the WiFi firmware image
 * @param clm_blob_addr[in] New address of the CLM blob
 * @param clm_blob_size[in] New size of the CLM blob
 * @return 0 on success, negative on failure
 */
int cy_update_ext_wifi_fw_location_and_size(mbed::bd_addr_t image_addr, mbed::bd_size_t image_size, mbed::bd_addr_t clm_blob_addr, mbed::bd_size_t clm_blob_size);

#endif /* defined(CY_EXT_WIFI_FW_STORAGE) && !MBED_CONF_TARGET_XIP_ENABLE */

#endif /* CY_EXT_WIFI_FW_RESERVED_REGION_BD_H */
