/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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

#include <algorithm>
#include <memory>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "platform/mbed_error.h"
#include "drivers/internal/SFDP.h"

#if (DEVICE_SPI || DEVICE_QSPI)

#include "features/frameworks/mbed-trace/mbed-trace/mbed_trace.h"
#define TRACE_GROUP "SFDP"

namespace {

/* Extracts Parameter ID MSB from the second DWORD of a parameter header */
inline uint8_t sfdp_get_param_id_msb(uint32_t dword2)
{
    return (dword2 & 0xFF000000) >> 24;
}

/* Extracts Parameter Table Pointer from the second DWORD of a parameter header */
inline uint32_t sfdp_get_param_tbl_ptr(uint32_t dword2)
{
    return dword2 & 0x00FFFFFF;
}
}

namespace mbed {

// Erase Types Params
constexpr int SFDP_BASIC_PARAM_TABLE_ERASE_TYPE_1_BYTE = 29; ///< Erase Type 1 Instruction
constexpr int SFDP_BASIC_PARAM_TABLE_ERASE_TYPE_2_BYTE = 31; ///< Erase Type 2 Instruction
constexpr int SFDP_BASIC_PARAM_TABLE_ERASE_TYPE_3_BYTE = 33; ///< Erase Type 3 Instruction
constexpr int SFDP_BASIC_PARAM_TABLE_ERASE_TYPE_4_BYTE = 35; ///< Erase Type 4 Instruction
constexpr int SFDP_BASIC_PARAM_TABLE_ERASE_TYPE_1_SIZE_BYTE = 28; ///< Erase Type 1 Size
constexpr int SFDP_BASIC_PARAM_TABLE_ERASE_TYPE_2_SIZE_BYTE = 30; ///< Erase Type 2 Size
constexpr int SFDP_BASIC_PARAM_TABLE_ERASE_TYPE_3_SIZE_BYTE = 32; ///< Erase Type 3 Size
constexpr int SFDP_BASIC_PARAM_TABLE_ERASE_TYPE_4_SIZE_BYTE = 34; ///< Erase Type 4 Size
constexpr int SFDP_BASIC_PARAM_TABLE_4K_ERASE_TYPE_BYTE = 1; ///< 4 Kilobyte Erase Instruction

constexpr int SFDP_ERASE_BITMASK_TYPE_4K_ERASE_UNSUPPORTED = 0xFF;

/** SFDP Header */
struct sfdp_hdr {
    uint8_t SIG_B0; ///< SFDP Signature, Byte 0
    uint8_t SIG_B1; ///< SFDP Signature, Byte 1
    uint8_t SIG_B2; ///< SFDP Signature, Byte 2
    uint8_t SIG_B3; ///< SFDP Signature, Byte 3
    uint8_t R_MINOR; ///< SFDP Minor Revision
    uint8_t R_MAJOR; ///< SFDP Major Revision
    uint8_t NPH; ///< Number of parameter headers (0-based, 0 indicates 1 parameter header)
    uint8_t ACP; ///< SFDP Access Protocol
};

/** SFDP Parameter header */
struct sfdp_prm_hdr {
    uint8_t PID_LSB; ///< Parameter ID LSB
    uint8_t P_MINOR; ///< Parameter Minor Revision
    uint8_t P_MAJOR; ///< Parameter Major Revision
    uint8_t P_LEN;   ///< Parameter length in DWORDS
    uint32_t DWORD2; ///< Parameter ID MSB + Parameter Table Pointer
};

/** Parse SFDP Header
 * @param sfdp_hdr_ptr Pointer to memory holding an SFDP header
 * @return Number of Parameter Headers on success, -1 on failure
 */
int sfdp_parse_sfdp_header(sfdp_hdr *sfdp_hdr_ptr)
{
    if (!(memcmp(sfdp_hdr_ptr, "SFDP", 4) == 0 && sfdp_hdr_ptr->R_MAJOR == 1)) {
        tr_error("Verify SFDP signature and version Failed");
        return -1;
    }

    tr_debug("Verified SFDP Signature and version successfully");

    int hdr_cnt = sfdp_hdr_ptr->NPH + 1;
    tr_debug("Number of parameter headers: %d", hdr_cnt);

    return hdr_cnt;
}

/** Parse Parameter Header
 * @param phdr_ptr Pointer to memory holding a single SFDP Parameter header
 * @param hdr_info Reference to a Parameter Table structure where info about the table is written
 * @return 0 on success, -1 on failure
 */
int sfdp_parse_single_param_header(sfdp_prm_hdr *phdr_ptr, sfdp_hdr_info &hdr_info)
{
    if (phdr_ptr->P_MAJOR != 1) {
        tr_error("Parameter header: Major Version must be 1!");
        return -1;
    }

    int param_id_msb = sfdp_get_param_id_msb(phdr_ptr->DWORD2);

    /* MSB JEDEC ID */
    if (param_id_msb == 0xFF) {

        /* LSB JEDEC ID */
        switch (phdr_ptr->PID_LSB) {
            case 0x0:
                tr_debug("Parameter header: JEDEC Basic Flash - Revision %" PRIX8 ".%" PRIX8 "",
                         phdr_ptr->P_MAJOR,
                         phdr_ptr->P_MINOR);
                hdr_info.bptbl.addr = sfdp_get_param_tbl_ptr(phdr_ptr->DWORD2);
                hdr_info.bptbl.size = std::min((phdr_ptr->P_LEN * 4), SFDP_BASIC_PARAMS_TBL_SIZE);
                break;
            case 0x81:
                tr_info("Parameter header: Sector Map");
                hdr_info.smptbl.addr = sfdp_get_param_tbl_ptr(phdr_ptr->DWORD2);
                hdr_info.smptbl.size = phdr_ptr->P_LEN * 4;
                break;
            /* Unsupported */
            case 0x03:
                tr_info("UNSUPPORTED:Parameter header: Replay Protected Monotonic Counters");
                break;
            case 0x84:
                tr_info("UNSUPPORTED:Parameter header: 4-byte Address Instruction");
                break;
            case 0x05:
                tr_info("UNSUPPORTED:Parameter header: eXtended Serial Peripheral Interface (xSPI) Profile 1.0");
                break;
            case 0x06:
                tr_info("UNSUPPORTED:Parameter header: eXtended Serial Peripheral Interface (xSPI) Profile 2.0");
                break;
            case 0x87:
                tr_info("UNSUPPORTED:Parameter header: SCCR Map for SPI Memory Devices");
                break;
            case 0x88:
                tr_info("UNSUPPORTED:Parameter header: SCCR Map Offsets for Multi-Chip SPI Memory Devices");
                break;
            case 0x09:
                tr_info("UNSUPPORTED:Parameter header: SCCR Map for xSPI Profile 2.0 Memory Devices");
                break;
            case 0x0A:
                tr_info("UNSUPPORTED:Parameter header: Command Sequences to Change to Octal DDR (8D-8D-8D) mode");
                break;
            case 0x0C:
                tr_info("UNSUPPORTED:Parameter header: x4 Quad IO with DS");
                break;
            case 0x8D:
                tr_info("UNSUPPORTED:Parameter header: Command Sequences to Change to Quad DDR (4S-4D-4D) mode");
                break;
            default:
                tr_debug("Parameter header: unknown JEDEC header. Parameter ID LSB: 0x%" PRIX8 "; MSB: 0x%" PRIX8 "",
                         phdr_ptr->PID_LSB,
                         sfdp_get_param_id_msb(phdr_ptr->DWORD2));
        }
    } else if (param_id_msb >= 0x80) { // MSB JEDEC ID
        tr_debug("Parameter header: unknown JEDEC header. Parameter ID LSB: 0x%" PRIX8 "; MSB: 0x%" PRIX8 "",
                 phdr_ptr->PID_LSB,
                 sfdp_get_param_id_msb(phdr_ptr->DWORD2));
    } else { // MSB Vendor ID
        tr_info("Parameter header: vendor specific header. Parameter ID LSB: 0x%" PRIX8 "; MSB: 0x%" PRIX8 "",
                phdr_ptr->PID_LSB,
                sfdp_get_param_id_msb(phdr_ptr->DWORD2));
    }

    return 0;
}

int sfdp_parse_headers(Callback<int(bd_addr_t, void *, bd_size_t)> sfdp_reader, sfdp_hdr_info &sfdp_info)
{
    bd_addr_t addr = 0x0;
    int number_of_param_headers = 0;
    size_t data_length;

    {
        data_length = SFDP_HEADER_SIZE;
        uint8_t sfdp_header[SFDP_HEADER_SIZE];

        int status = sfdp_reader(addr, sfdp_header, data_length);
        if (status < 0) {
            tr_error("Retrieving SFDP Header failed");
            return -1;
        }

        number_of_param_headers = sfdp_parse_sfdp_header((sfdp_hdr *)sfdp_header);
        if (number_of_param_headers < 0) {
            return number_of_param_headers;
        }
    }

    addr += SFDP_HEADER_SIZE;

    {
        data_length = SFDP_HEADER_SIZE;
        uint8_t param_header[SFDP_HEADER_SIZE];
        int status;
        int hdr_status;

        // Loop over Param Headers and parse them (currently supports Basic Param Table and Sector Region Map Table)
        for (int idx = 0; idx < number_of_param_headers; idx++) {
            status = sfdp_reader(addr, param_header, data_length);
            if (status < 0) {
                tr_error("Retrieving a parameter header %d failed", idx + 1);
                return -1;
            }

            hdr_status = sfdp_parse_single_param_header((sfdp_prm_hdr *)param_header, sfdp_info);
            if (hdr_status < 0) {
                return hdr_status;
            }

            addr += SFDP_HEADER_SIZE;
        }
    }

    return 0;
}

int sfdp_parse_sector_map_table(Callback<int(bd_addr_t, void *, bd_size_t)> sfdp_reader, sfdp_hdr_info &sfdp_info)
{
    uint32_t tmp_region_size = 0;
    uint8_t type_mask;
    int prev_boundary = 0;
    // Default set to all type bits 1-4 are common
    int min_common_erase_type_bits = SFDP_ERASE_BITMASK_ALL;

    // If there's no region map, we have a single region sized the entire device size
    sfdp_info.smptbl.region_size[0] = sfdp_info.bptbl.device_size_bytes;
    sfdp_info.smptbl.region_high_boundary[0] = sfdp_info.bptbl.device_size_bytes - 1;

    if (!sfdp_info.smptbl.addr || !sfdp_info.smptbl.size) {
        tr_debug("No Sector Map Table");
        return MBED_SUCCESS;
    }

    /* The number of
     * - sector map configuration detection commands
     * - configurations
     * - regions in each configuration
     *  is variable -> the size of this table is variable
     */
    auto smptbl_buff = std::make_unique<uint8_t[]>(sfdp_info.smptbl.size);
    if (!smptbl_buff) {
        tr_error("Failed to allocate memory");
        return -1;
    }

    tr_debug("Parsing Sector Map Table - addr: 0x%" PRIx32 ", Size: %d", sfdp_info.smptbl.addr, sfdp_info.smptbl.size);

    int status = sfdp_reader(sfdp_info.smptbl.addr, smptbl_buff.get(), sfdp_info.smptbl.size);
    if (status < 0) {
        tr_error("Sector Map: Table retrieval failed");
        return -1;
    }

    // Currently we support only Single Map Descriptor
    if (!((smptbl_buff[0] & 0x3) == 0x03) && (smptbl_buff[1] == 0x0)) {
        tr_error("Sector Map: Supporting Only Single Map Descriptor (not map commands)");
        return -1;
    }

    sfdp_info.smptbl.region_cnt = smptbl_buff[2] + 1;
    if (sfdp_info.smptbl.region_cnt > SFDP_SECTOR_MAP_MAX_REGIONS) {
        tr_error("Sector Map: Supporting up to %d regions, current setup to %d regions - fail",
                 SFDP_SECTOR_MAP_MAX_REGIONS,
                 sfdp_info.smptbl.region_cnt);
        return -1;
    }

    // Loop through Regions and set for each one: size, supported erase types, high boundary offset
    // Calculate minimum Common Erase Type for all Regions
    for (auto idx = 0; idx < sfdp_info.smptbl.region_cnt; idx++) {
        tmp_region_size = ((*((uint32_t *)&smptbl_buff[(idx + 1) * 4])) >> 8) & 0x00FFFFFF; // bits 9-32
        sfdp_info.smptbl.region_size[idx] = (tmp_region_size + 1) * 256; // Region size is 0 based multiple of 256 bytes;

        sfdp_info.smptbl.region_erase_types_bitfld[idx] = smptbl_buff[(idx + 1) * 4] & 0x0F; // bits 1-4

        min_common_erase_type_bits &= sfdp_info.smptbl.region_erase_types_bitfld[idx];

        sfdp_info.smptbl.region_high_boundary[idx] = (sfdp_info.smptbl.region_size[idx] - 1) + prev_boundary;

        prev_boundary = sfdp_info.smptbl.region_high_boundary[idx] + 1;
    }

    // Calc minimum Common Erase Size from min_common_erase_type_bits
    type_mask = SFDP_ERASE_BITMASK_TYPE1;
    // If no common erase type is found between regions
    sfdp_info.smptbl.regions_min_common_erase_size = 0;
    for (auto idx = 0; idx < 4; idx++) {
        if (min_common_erase_type_bits & type_mask) {
            sfdp_info.smptbl.regions_min_common_erase_size = sfdp_info.smptbl.erase_type_size_arr[idx];
            break;
        }
        type_mask = type_mask << 1;
    }

    return 0;
}

size_t sfdp_detect_page_size(uint8_t *basic_param_table_ptr, size_t basic_param_table_size)
{
    constexpr int SFDP_BASIC_PARAM_TABLE_PAGE_SIZE = 40;
    constexpr int SFDP_DEFAULT_PAGE_SIZE = 256;

    unsigned int page_size = SFDP_DEFAULT_PAGE_SIZE;

    if (basic_param_table_size > SFDP_BASIC_PARAM_TABLE_PAGE_SIZE) {
        // Page Size is specified by 4 Bits (N), calculated by 2^N
        int page_to_power_size = ((int)basic_param_table_ptr[SFDP_BASIC_PARAM_TABLE_PAGE_SIZE]) >> 4;
        page_size = 1 << page_to_power_size;
        tr_debug("Detected Page Size: %d", page_size);
    } else {
        tr_debug("Using Default Page Size: %d", page_size);
    }
    return page_size;
}

int sfdp_detect_erase_types_inst_and_size(uint8_t *bptbl_ptr, sfdp_hdr_info &sfdp_info)
{
    uint8_t bitfield = 0x01;

    // Erase 4K Inst is taken either from param table legacy 4K erase or superseded by erase Instruction for type of size 4K
    if (sfdp_info.bptbl.size > SFDP_BASIC_PARAM_TABLE_ERASE_TYPE_1_SIZE_BYTE) {
        // Loop Erase Types 1-4
        for (int idx = 0; idx < 4; idx++) {
            sfdp_info.smptbl.erase_type_inst_arr[idx] = -1; // Default for unsupported type
            sfdp_info.smptbl.erase_type_size_arr[idx] = 1
                                                        << bptbl_ptr[SFDP_BASIC_PARAM_TABLE_ERASE_TYPE_1_SIZE_BYTE + 2 * idx]; // Size is 2^N where N is the table value
            tr_debug("Erase Type(A) %d - Inst: 0x%xh, Size: %d", (idx + 1), sfdp_info.smptbl.erase_type_inst_arr[idx],
                     sfdp_info.smptbl.erase_type_size_arr[idx]);
            if (sfdp_info.smptbl.erase_type_size_arr[idx] > 1) {
                // if size==1 type is not supported
                sfdp_info.smptbl.erase_type_inst_arr[idx] = bptbl_ptr[SFDP_BASIC_PARAM_TABLE_ERASE_TYPE_1_BYTE
                                                                      + 2 * idx];

                if ((sfdp_info.smptbl.erase_type_size_arr[idx] < sfdp_info.smptbl.regions_min_common_erase_size)
                        || (sfdp_info.smptbl.regions_min_common_erase_size == 0)) {
                    //Set default minimal common erase for signal region
                    sfdp_info.smptbl.regions_min_common_erase_size = sfdp_info.smptbl.erase_type_size_arr[idx];
                }
                sfdp_info.smptbl.region_erase_types_bitfld[0] |= bitfield; // If there's no region map, set region "0" types bitfield as default
            }

            tr_debug("Erase Type %d - Inst: 0x%xh, Size: %d", (idx + 1), sfdp_info.smptbl.erase_type_inst_arr[idx],
                     sfdp_info.smptbl.erase_type_size_arr[idx]);
            bitfield = bitfield << 1;
        }
    } else {
        tr_debug("Erase types are not available - falling back to legacy 4k erase instruction");

        sfdp_info.bptbl.legacy_erase_instruction = bptbl_ptr[SFDP_BASIC_PARAM_TABLE_4K_ERASE_TYPE_BYTE];
        if (sfdp_info.bptbl.legacy_erase_instruction == SFDP_ERASE_BITMASK_TYPE_4K_ERASE_UNSUPPORTED) {
            tr_error("Legacy 4k erase instruction not supported");
            return -1;
        }
    }

    return 0;
}

int sfdp_find_addr_region(bd_size_t offset, const sfdp_hdr_info &sfdp_info)
{
    if ((offset > sfdp_info.bptbl.device_size_bytes) || (sfdp_info.smptbl.region_cnt == 0)) {
        return -1;
    }

    if (sfdp_info.smptbl.region_cnt == 1) {
        return 0;
    }

    for (int idx = sfdp_info.smptbl.region_cnt - 2; idx >= 0; idx--) {

        if (offset > sfdp_info.smptbl.region_high_boundary[idx]) {
            return (idx + 1);
        }
    }
    return -1;

}

int sfdp_iterate_next_largest_erase_type(uint8_t &bitfield,
                                         int size,
                                         int offset,
                                         int region,
                                         const sfdp_smptbl_info &smptbl)
{
    uint8_t type_mask = SFDP_ERASE_BITMASK_TYPE4;
    int largest_erase_type = 0;

    int idx;
    for (idx = 3; idx >= 0; idx--) {
        if (bitfield & type_mask) {
            largest_erase_type = idx;
            if ((size > (int)(smptbl.erase_type_size_arr[largest_erase_type])) &&
                    ((smptbl.region_high_boundary[region] - offset)
                     > (int)(smptbl.erase_type_size_arr[largest_erase_type]))) {
                break;
            } else {
                bitfield &= ~type_mask;
            }
        }
        type_mask = type_mask >> 1;
    }

    if (idx == -1) {
        tr_error("No erase type was found for current region addr");
    }
    return largest_erase_type;
}

int sfdp_detect_device_density(uint8_t *bptbl_ptr, sfdp_bptbl_info &bptbl_info)
{
    // stored in bits - 1
    uint32_t density_bits = (
                                (bptbl_ptr[7] << 24) |
                                (bptbl_ptr[6] << 16) |
                                (bptbl_ptr[5] << 8) |
                                bptbl_ptr[4]);

    bptbl_info.device_size_bytes = (density_bits + 1) / 8;

    tr_info("Density bits: %" PRIu32 " , device size: %llu bytes", density_bits, bptbl_info.device_size_bytes);

    return 0;
}

#if DEVICE_QSPI
int sfdp_detect_addressability(uint8_t *bptbl_ptr, sfdp_bptbl_info &bptbl_info)
{
    // Check that density is not greater than 4 gigabits (i.e. that addressing beyond 4 bytes is not required)
    if ((bptbl_ptr[7] & 0x80) != 0) {
        return -1;
    }

    return 0;
}
#elif DEVICE_SPI
int sfdp_detect_addressability(uint8_t *bptbl_ptr, sfdp_bptbl_info &bptbl_info)
{
    // Check address size, currently only supports 3byte addresses
    if ((bptbl_ptr[2] & 0x4) != 0 || (bptbl_ptr[7] & 0x80) != 0) {
        return -1;
    }

    return 0;
}
#endif

} /* namespace mbed */
#endif /* (DEVICE_SPI || DEVICE_QSPI) */
