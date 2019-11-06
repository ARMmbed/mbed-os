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

#include "whd_chip_constants.h"
#include "whd_wlioctl.h"
#include "whd_int.h"
#include "whd_types_int.h"


/******************************************************
*               Function Definitions
******************************************************/

uint32_t whd_chip_set_chip_id(whd_driver_t whd_driver, uint16_t id)
{
    whd_driver->chip_info.chip_id = id;

    return 0;
}

uint16_t whd_chip_get_chip_id(whd_driver_t whd_driver)
{
    return whd_driver->chip_info.chip_id;
}

uint32_t get_whd_var(whd_driver_t whd_driver, chip_var_t var)
{
    uint32_t val = 0;

    uint16_t wlan_chip_id = whd_chip_get_chip_id(whd_driver);
    switch (var)
    {
        case BUS_CREDIT_DIFF:
            CHECK_RETURN(get_chip_max_bus_data_credit_diff(wlan_chip_id, &val) );
            break;
        case CHIP_RAM_SIZE:
            CHECK_RETURN(get_chip_ram_size(wlan_chip_id, &val) );
            break;
        case ATCM_RAM_BASE_ADDRESS:
            CHECK_RETURN(get_atcm_ram_base_address(wlan_chip_id, &val) );
            break;
        case SOCRAM_SRMEM_SIZE:
            CHECK_RETURN(get_socsram_srmem_size(wlan_chip_id, &val) );
            break;
        case CHANSPEC_BAND_MASK:
            CHECK_RETURN(get_wl_chanspec_band_mask(wlan_chip_id, &val) );
            break;
        case CHANSPEC_BAND_2G:
            CHECK_RETURN(get_wl_chanspec_band_2G(wlan_chip_id, &val) );
            break;
        case CHANSPEC_BAND_5G:
            CHECK_RETURN(get_wl_chanspec_band_5G(wlan_chip_id, &val) );
            break;
        case CHANSPEC_BAND_SHIFT:
            CHECK_RETURN(get_wl_chanspec_band_shift(wlan_chip_id, &val) );
            break;
        case CHANSPEC_BW_10:
            CHECK_RETURN(get_wl_chanspec_bw_10(wlan_chip_id, &val) );
            break;
        case CHANSPEC_BW_20:
            CHECK_RETURN(get_wl_chanspec_bw_20(wlan_chip_id, &val) );
            break;
        case CHANSPEC_BW_40:
            CHECK_RETURN(get_wl_chanspec_bw_40(wlan_chip_id, &val) );
            break;
        case CHANSPEC_BW_MASK:
            CHECK_RETURN(get_wl_chanspec_bw_mask(wlan_chip_id, &val) );
            break;
        case CHANSPEC_BW_SHIFT:
            CHECK_RETURN(get_wl_chanspec_bw_shift(wlan_chip_id, &val) );
            break;
        case CHANSPEC_CTL_SB_LOWER:
            CHECK_RETURN(get_wl_chanspec_ctl_sb_lower(wlan_chip_id, &val) );
            break;
        case CHANSPEC_CTL_SB_UPPER:
            CHECK_RETURN(get_wl_chanspec_ctl_sb_upper(wlan_chip_id, &val) );
            break;
        case CHANSPEC_CTL_SB_NONE:
            CHECK_RETURN(get_wl_chanspec_ctl_sb_none(wlan_chip_id, &val) );
            break;
        case CHANSPEC_CTL_SB_MASK:
            CHECK_RETURN(get_wl_chanspec_ctl_sb_mask(wlan_chip_id, &val) );
            break;
        default:
            break;
    }

    return val;
}

whd_result_t get_chip_max_bus_data_credit_diff(uint16_t wlan_chip_id, uint32_t *credit_diff)
{
    *credit_diff = 0;
    if (wlan_chip_id == 43362)
    {
        *credit_diff = 7;
    }
    else if (wlan_chip_id == 43455)
    {
        *credit_diff = 50;
    }
    else
    {
        *credit_diff = 20;
    }
    return WHD_SUCCESS;
}

whd_result_t get_chip_ram_size(uint16_t wlan_chip_id, uint32_t *size)
{
    *size = 0;
    if ( (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) || (wlan_chip_id == 43430) )
    {
        *size = (512 * 1024);
    }
    else if ( (wlan_chip_id == 43362) || (wlan_chip_id == 4390) )
    {
        *size = 0x3C000;
    }
    else if (wlan_chip_id == 43909)
    {
        *size = 0x90000;
    }
    else if (wlan_chip_id == 43012)
    {
        *size = 0xA0000;
    }
    else
    {
        *size = 0x80000;
    }
    return WHD_SUCCESS;
}

whd_result_t get_atcm_ram_base_address(uint16_t wlan_chip_id, uint32_t *size)
{
    *size = 0;
    if (wlan_chip_id == 43455)
    {
        *size = 0x198000;
    }
    else
    {
        *size = 0;
    }
    return WHD_SUCCESS;
}

whd_result_t get_socsram_srmem_size(uint16_t wlan_chip_id, uint32_t *mem_size)
{
    *mem_size = 0;
    if ( (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *mem_size = (32 * 1024);
    }
    else if (wlan_chip_id == 43430)
    {
        *mem_size = (64 * 1024);
    }
    else
    {
        *mem_size = 0;
    }
    return WHD_SUCCESS;
}

whd_result_t get_wl_chanspec_band_mask(uint16_t wlan_chip_id, uint32_t *band_mask)
{
    *band_mask = 0;
    if ( (wlan_chip_id == 43362) || (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *band_mask = 0xf000;
    }
    else
    {
        *band_mask = 0xc000;
    }
    return WHD_SUCCESS;
}

whd_result_t get_wl_chanspec_band_2G(uint16_t wlan_chip_id, uint32_t *band_2g)
{
    *band_2g = 0;
    if ( (wlan_chip_id == 43362) || (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *band_2g = 0x2000;
    }
    else
    {
        *band_2g = 0x0000;
    }
    return WHD_SUCCESS;
}

whd_result_t get_wl_chanspec_band_5G(uint16_t wlan_chip_id, uint32_t *band_5g)
{
    *band_5g = 0;
    if ( (wlan_chip_id == 43362) || (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *band_5g = 0x1000;
    }
    else
    {
        *band_5g = 0xc000;
    }
    return WHD_SUCCESS;
}

whd_result_t get_wl_chanspec_band_shift(uint16_t wlan_chip_id, uint32_t *band_shift)
{
    *band_shift = 0;
    if ( (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *band_shift = 12;
    }
    else
    {
        *band_shift = 14;
    }
    return WHD_SUCCESS;
}

whd_result_t get_wl_chanspec_bw_10(uint16_t wlan_chip_id, uint32_t *bw_10)
{
    *bw_10 = 0;
    if ( (wlan_chip_id == 43362) || (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *bw_10 = 0x0400;
    }
    else
    {
        *bw_10 = 0x0800;
    }
    return WHD_SUCCESS;
}

whd_result_t get_wl_chanspec_bw_20(uint16_t wlan_chip_id, uint32_t *bw_20)
{
    *bw_20 = 0;
    if ( (wlan_chip_id == 43362) || (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *bw_20 = 0x0800;
    }
    else
    {
        *bw_20 = 0x1000;
    }
    return WHD_SUCCESS;
}

whd_result_t get_wl_chanspec_bw_40(uint16_t wlan_chip_id, uint32_t *bw_40)
{
    *bw_40 = 0;
    if ( (wlan_chip_id == 43362) || (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *bw_40 = 0x0C00;
    }
    else
    {
        *bw_40 = 0x1800;
    }
    return WHD_SUCCESS;
}

whd_result_t get_wl_chanspec_bw_mask(uint16_t wlan_chip_id, uint32_t *bw_mask)
{
    *bw_mask = 0;
    if ( (wlan_chip_id == 43362) || (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *bw_mask = 0x0C00;
    }
    else
    {
        *bw_mask = 0x3800;
    }
    return WHD_SUCCESS;
}

whd_result_t get_wl_chanspec_bw_shift(uint16_t wlan_chip_id, uint32_t *bw_shift)
{
    *bw_shift = 0;
    if ( (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *bw_shift = 10;
    }
    else
    {
        *bw_shift = 11;
    }
    return WHD_SUCCESS;
}

whd_result_t get_wl_chanspec_ctl_sb_lower(uint16_t wlan_chip_id, uint32_t *sb_lower)
{
    *sb_lower = 0;
    if ( (wlan_chip_id == 43362) || (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *sb_lower = 0x0100;
    }
    else
    {
        *sb_lower = WL_CHANSPEC_CTL_SB_LLL;
    }
    return WHD_SUCCESS;
}

whd_result_t get_wl_chanspec_ctl_sb_upper(uint16_t wlan_chip_id, uint32_t *sb_upper)
{
    *sb_upper = 0;
    if ( (wlan_chip_id == 43362) || (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *sb_upper = 0x0200;
    }
    else
    {
        *sb_upper = WL_CHANSPEC_CTL_SB_LLU;
    }
    return WHD_SUCCESS;
}

whd_result_t get_wl_chanspec_ctl_sb_none(uint16_t wlan_chip_id, uint32_t *sb_none)
{
    *sb_none = 0;
    if ( (wlan_chip_id == 43362) || (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *sb_none = 0x0300;
    }
    else
    {
        *sb_none = WL_CHANSPEC_CTL_SB_LLL;
    }
    return WHD_SUCCESS;
}

whd_result_t get_wl_chanspec_ctl_sb_mask(uint16_t wlan_chip_id, uint32_t *sb_mask)
{
    *sb_mask = 0;
    if ( (wlan_chip_id == 43362) || (wlan_chip_id == 4334) || (wlan_chip_id == 43340) || (wlan_chip_id == 43342) )
    {
        *sb_mask = 0x0300;
    }
    else
    {
        *sb_mask = 0x0700;
    }
    return WHD_SUCCESS;
}

