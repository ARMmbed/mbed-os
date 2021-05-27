/***************************************************************************//**
* \file cy_ext_wifi_fw_reserved_region_bd.cpp
*
* \brief
* Block device use to interact with external memory without interfering with
* the region reserved for the WiFi firmware.
*
* Creates a canonical instance that can be accessed via cy_get_ext_wifi_fw_reserved_region_bd().
* Provides overrides for Mbed OS functions so that storage mechanisms such as KVStore
* will default to using this block device.
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

// Use of xip-enable by user overrides definition of CY_EXT_WIFI_FW_STORAGE in target configuration
#if defined(CY_EXT_WIFI_FW_STORAGE) && !MBED_CONF_TARGET_XIP_ENABLE

#include "cy_ext_wifi_fw_reserved_region_bd.h"
#include "cy_wifi_fw_section.h"
#include "QSPIFBlockDevice.h"

BlockDevice *cy_get_ext_wifi_fw_reserved_region_underlying_bd()
{
#if CY_EXT_WIFI_FW_STORAGE == QSPIF
    static QSPIFBlockDevice underlying_bd;
#else
#error "Invalid WiFi storage type"
#endif

    return &underlying_bd;
}

CyReservedRegionBlockDevice *cy_get_ext_wifi_fw_reserved_region_bd()
{
    BlockDevice *underlying_bd = cy_get_ext_wifi_fw_reserved_region_underlying_bd();
    static CyReservedRegionBlockDevice default_instance(underlying_bd, (mbed::bd_addr_t) CY_WIFI_FW_SECTION_END - (mbed::bd_addr_t) CY_WIFI_FW_SECTION_START);
    return &default_instance;
}

extern "C" {
    extern void cy_ext_wifi_fw_resources_update_handles(void *image_addr, unsigned long image_size, void *clm_blob_addr, unsigned long clm_blob_size);
}

int cy_update_ext_wifi_fw_location_and_size(mbed::bd_addr_t image_addr, mbed::bd_size_t image_size, mbed::bd_addr_t clm_blob_addr, mbed::bd_size_t clm_blob_size)
{
    CyReservedRegionBlockDevice *bd = cy_get_ext_wifi_fw_reserved_region_bd();
    // If the reserved region end is NULL, the block device hasn't been initialized yet, so we can't perform this check
    if ((bd->reserved_region_end() != 0) && (image_addr + image_size > bd->reserved_region_end() || clm_blob_addr + clm_blob_size > bd->reserved_region_end())) {
        // Should not exceed originally computed reserved region size, as this will break anything that was using the rest of external storage
        return -1;
    }

    // These addresses are not valid pointers, but are converted to pointers so as to be used with the WHD resource handles
    cy_ext_wifi_fw_resources_update_handles((void *) image_addr, image_size, (void *) clm_blob_addr, clm_blob_size);

    return 0;
}

//
// Overrides for other Mbed OS storage mechanisms to that this is the default block device
//

BlockDevice *get_other_blockdevice()
{
    return cy_get_ext_wifi_fw_reserved_region_bd();
}

BlockDevice *BlockDevice::get_default_instance()
{
    return cy_get_ext_wifi_fw_reserved_region_bd();
}

#endif /* defined(CY_EXT_WIFI_FW_STORAGE) && !MBED_CONF_TARGET_XIP_ENABLE */
