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

#ifndef INCLUDED_CHIP_CONSTANTS_H_
#define INCLUDED_CHIP_CONSTANTS_H_

#include "whd.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum chip_var
{
    BUS_CREDIT_DIFF = 1,
    CHIP_RAM_SIZE,
    ATCM_RAM_BASE_ADDRESS,
    SOCRAM_SRMEM_SIZE,
    CHANSPEC_BAND_MASK,
    CHANSPEC_BAND_2G,
    CHANSPEC_BAND_5G,
    CHANSPEC_BAND_SHIFT,
    CHANSPEC_BW_10,
    CHANSPEC_BW_20,
    CHANSPEC_BW_40,
    CHANSPEC_BW_MASK,
    CHANSPEC_BW_SHIFT,
    CHANSPEC_CTL_SB_LOWER,
    CHANSPEC_CTL_SB_UPPER,
    CHANSPEC_CTL_SB_NONE,
    CHANSPEC_CTL_SB_MASK
} chip_var_t;

#define VERIFY_RESULT(x) { whd_result_t verify_result = WHD_SUCCESS; verify_result = (x); \
                           if (verify_result != WHD_SUCCESS){ \
                               WPRINT_WHD_ERROR( ("Function %s failed at line %d \n", __func__, __LINE__) ); \
                               return verify_result; } }
#define GET_C_VAR(whd_driver, var) get_whd_var(whd_driver, var)

uint32_t get_whd_var(whd_driver_t whd_driver, chip_var_t var);

whd_result_t get_chip_max_bus_data_credit_diff(uint16_t, uint32_t *);
whd_result_t get_chip_ram_size(uint16_t, uint32_t *);
whd_result_t get_atcm_ram_base_address(uint16_t, uint32_t *);
whd_result_t get_socsram_srmem_size(uint16_t, uint32_t *);
whd_result_t get_wl_chanspec_band_mask(uint16_t, uint32_t *);
whd_result_t get_wl_chanspec_band_2G(uint16_t, uint32_t *);
whd_result_t get_wl_chanspec_band_5G(uint16_t, uint32_t *);
whd_result_t get_wl_chanspec_band_shift(uint16_t, uint32_t *);
whd_result_t get_wl_chanspec_bw_10(uint16_t, uint32_t *);
whd_result_t get_wl_chanspec_bw_20(uint16_t, uint32_t *);
whd_result_t get_wl_chanspec_bw_40(uint16_t, uint32_t *);
whd_result_t get_wl_chanspec_bw_mask(uint16_t, uint32_t *);
whd_result_t get_wl_chanspec_bw_shift(uint16_t, uint32_t *);
whd_result_t get_wl_chanspec_ctl_sb_lower(uint16_t, uint32_t *);
whd_result_t get_wl_chanspec_ctl_sb_upper(uint16_t, uint32_t *);
whd_result_t get_wl_chanspec_ctl_sb_none(uint16_t, uint32_t *);
whd_result_t get_wl_chanspec_ctl_sb_mask(uint16_t, uint32_t *);

uint32_t whd_chip_set_chip_id(whd_driver_t whd_driver, uint16_t id);
uint16_t whd_chip_get_chip_id(whd_driver_t whd_driver);

#undef CHIP_FIRMWARE_SUPPORTS_PM_LIMIT_IOVAR

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_CHIP_CONSTANTS_H_ */

