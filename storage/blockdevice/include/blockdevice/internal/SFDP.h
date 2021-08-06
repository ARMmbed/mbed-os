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

#ifndef SFDP_H
#define SFDP_H

#include <cstddef>
#include <cstdint>
#include "blockdevice/BlockDevice.h"
#include "platform/Callback.h"

namespace mbed {

/** \defgroup drivers-internal-api-sfdp SFDP
 * \ingroup drivers-internal-api
 * Serial Flash Discoverable Parameters.
 *
 * Based on <a href="https://www.jedec.org/standards-documents/docs/jesd216b">JESD216D.01 Standard</a>.
 * @{
 */

constexpr int SFDP_HEADER_SIZE = 8; ///< Size of an SFDP header in bytes, 2 DWORDS
constexpr int SFDP_BASIC_PARAMS_TBL_SIZE = 80; ///< Basic Parameter Table size in bytes, 20 DWORDS
constexpr int SFDP_SECTOR_MAP_MAX_REGIONS = 10; ///< Maximum number of regions with different erase granularity

// Erase Types Per Region BitMask
constexpr int SFDP_ERASE_BITMASK_TYPE4 = 0x08; ///< Erase type 4 (erase granularity) identifier
constexpr int SFDP_ERASE_BITMASK_TYPE3 = 0x04; ///< Erase type 3 (erase granularity) identifier
constexpr int SFDP_ERASE_BITMASK_TYPE2 = 0x02; ///< Erase type 2 (erase granularity) identifier
constexpr int SFDP_ERASE_BITMASK_TYPE1 = 0x01; ///< Erase type 1 (erase granularity) identifier
constexpr int SFDP_ERASE_BITMASK_NONE = 0x00;  ///< Erase type None
constexpr int SFDP_ERASE_BITMASK_ALL = 0x0F;   ///< Erase type All

constexpr int SFDP_MAX_NUM_OF_ERASE_TYPES = 4;  ///< Maximum number of different erase types (erase granularity)

// Size of a command specified by SFDP
enum sfdp_cmd_addr_size_t {
    SFDP_CMD_ADDR_NONE = 0x00,      // No address in command
    SFDP_CMD_ADDR_3_BYTE = 0x01,    // 3-byte address
    SFDP_CMD_ADDR_4_BYTE = 0x02,    // 4-byte address
    SFDP_CMD_ADDR_SIZE_VARIABLE = 0x03   // Address size from current setting
};

// Parameters for SFDP Read command
constexpr sfdp_cmd_addr_size_t SFDP_READ_CMD_ADDR_TYPE = SFDP_CMD_ADDR_3_BYTE; // Read SFDP has 3-byte address
constexpr uint8_t SFDP_READ_CMD_INST = 0x5A; // Read SFDP instruction
constexpr uint8_t SFDP_READ_CMD_DUMMY_CYCLES = 8; // READ SFDP dummy cycles

// Special value from SFDP for using dummy cycles from current setting
constexpr uint8_t SFDP_CMD_DUMMY_CYCLES_VARIABLE = 0xF;

/** JEDEC Basic Flash Parameter Table info */
struct sfdp_bptbl_info {
    uint32_t addr; ///< Address
    size_t size; ///< Size
    bd_size_t device_size_bytes;
    int legacy_erase_instruction; ///< Legacy 4K erase instruction
};

/** JEDEC Sector Map Table info */
struct sfdp_smptbl_info {
    uint32_t addr; ///< Address
    size_t size; ///< Size
    int region_cnt; ///< Number of erase regions
    int region_size[SFDP_SECTOR_MAP_MAX_REGIONS]; ///< Erase region size in bytes
    uint8_t region_erase_types_bitfld[SFDP_SECTOR_MAP_MAX_REGIONS]; ///< Each Region can support a bit combination of any of the 4 Erase Types
    unsigned int regions_min_common_erase_size; ///< Minimal common erase size for all regions (0 if none exists)
    bd_size_t region_high_boundary[SFDP_SECTOR_MAP_MAX_REGIONS]; ///< Region high address offset boundary
    int erase_type_inst_arr[SFDP_MAX_NUM_OF_ERASE_TYPES]; ///< // Up To 4 Erase Types are supported by SFDP (each with its own command Instruction and Size)
    unsigned int erase_type_size_arr[SFDP_MAX_NUM_OF_ERASE_TYPES]; ///< Erase sizes for all different erase types
};

/** JEDEC 4-byte Address Instruction Parameter Table info */
struct sfdp_fbatbl_info {
    uint32_t addr; ///< Address
    size_t size; ///< Size
    int erase_type_4_byte_inst_arr[SFDP_MAX_NUM_OF_ERASE_TYPES]; ///< // Up To 4 Erase Types are supported by SFDP (each with its own command Instruction and Size)
};

/** SFDP JEDEC Parameter Table info */
struct sfdp_hdr_info {
    sfdp_bptbl_info bptbl;
    sfdp_smptbl_info smptbl;
    sfdp_fbatbl_info fbatbl;
};

/** Parse SFDP Database
 * Retrieves all headers from within a memory device and parses the information contained by the headers
 *
 * Only JEDEC headers are parsed, not vendor specific ones.
 *
 * @param      sfdp_reader Callback function used to read headers from within a device
 * @param[out] sfdp_info   Contains the results of parsing the SFDP Database JEDEC headers
 *
 * @return MBED_SUCCESS on success, negative error code on failure
 */
int sfdp_parse_headers(Callback<int(bd_addr_t, sfdp_cmd_addr_size_t, uint8_t, uint8_t, void *, bd_size_t)> sfdp_reader, sfdp_hdr_info &sfdp_info);

/** Parse Sector Map Parameter Table
 * Retrieves the table from a device and parses the information contained by the table
 *
 * @param      sfdp_reader Callback function used to read headers from within a device
 * @param[out] sfdp_info   Contains the results of parsing the JEDEC Sector Map Table
 *
 * @return MBED_SUCCESS on success, negative error code on failure
 */
int sfdp_parse_sector_map_table(Callback<int(bd_addr_t, sfdp_cmd_addr_size_t, uint8_t, uint8_t, void *, bd_size_t)> sfdp_reader, sfdp_hdr_info &sfdp_info);

/** Detect page size used for writing on flash
 *
 * @param bptbl_ptr  Pointer to memory holding a Basic Parameter Table structure
 * @param bptbl_size Size of memory holding the Basic Parameter Table
 *
 * @return Page size
 */
size_t sfdp_detect_page_size(uint8_t *bptbl_ptr, size_t bptbl_size);

/** Detect all supported erase types
 *
 * @param         bptbl_ptr Pointer to memory holding a JEDEC Basic Flash Parameter Table
 * @param[in,out] sfdp_info Contains the results of parsing erase type instructions and sizes
 *
 * @return MBED_SUCCESS on success, negative error code on failure
 */
int sfdp_detect_erase_types_inst_and_size(uint8_t *bptbl_ptr, sfdp_hdr_info &sfdp_info);

/** Find the region to which the given offset belongs to
 *
 * @param offset    Offset value
 * @param sfdp_info Region information
 *
 * @return Region number
 */
int sfdp_find_addr_region(bd_addr_t offset, const sfdp_hdr_info &sfdp_info);

/** Finds the largest Erase Type of the Region to which the offset belongs to
 *
 * Iterates from highest type to lowest.
 *
 * @param bitfield Erase types bit field
 * @param size     Upper limit for region size
 * @param offset   Offset value
 * @param region   Region number
 * @param smptbl   Information about different erase types
 *
 * @return Largest erase type, or -1 if none matches the given address and size
 */
int sfdp_iterate_next_largest_erase_type(uint8_t bitfield,
                                         bd_size_t size,
                                         bd_addr_t offset,
                                         int region,
                                         const sfdp_smptbl_info &smptbl);

/** Detect device density
 *
 * @param bptbl_ptr Pointer to memory holding a Basic Parameter Table structure
 * @param bptbl_info Basic Parameter Table information structure
 *
 * @return 0 on success, negative error code on failure
 */
int sfdp_detect_device_density(uint8_t *bptbl_ptr, sfdp_bptbl_info &bptbl_info);

/** Detect is it possible to access the whole memory region
 *
 * @param bptbl_ptr Pointer to memory holding a Basic Parameter Table structure
 * @param bptbl_info Basic Parameter Table information structure
 *
 * @return 0 on success, negative error code on failure
 */
int sfdp_detect_addressability(uint8_t *bptbl_ptr, sfdp_bptbl_info &bptbl_info);

/** @}*/
} /* namespace mbed */
#endif
