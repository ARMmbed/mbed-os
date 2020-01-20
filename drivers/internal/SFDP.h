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
#include "features/storage/blockdevice/BlockDevice.h"
#include "platform/Callback.h"

namespace mbed {

static const int SFDP_HEADER_SIZE = 8; ///< Size of an SFDP header */
static const int SFDP_BASIC_PARAMS_TBL_SIZE = 80; ///< Basic Parameter Table size in Bytes, 20 DWORDS  */

/** SFDP Parameter Table addresses and sizes */
struct sfdp_hdr_info {
    uint32_t basic_table_addr; // Basic Parameter Table address
    size_t basic_table_size;  // Basic Parameter Table size
    uint32_t sector_map_table_addr; // Sector Map Parameter Table address
    size_t sector_map_table_size; // Sector Map Parameter Table size
};

/** SFDP Header */
struct sfdp_hdr {
    uint8_t SIG_B0; // SFDP Signature, Byte 0
    uint8_t SIG_B1; // SFDP Signature, Byte 1
    uint8_t SIG_B2; // SFDP Signature, Byte 2
    uint8_t SIG_B3; // SFDP Signature, Byte 3
    uint8_t R_MINOR; // SFDP Minor Revision
    uint8_t R_MAJOR; // SFDP Major Revision
    uint8_t NPH; // Number of parameter headers (0-based, 0 indicates 1 parameter header)
    uint8_t ACP; // SFDP Access Protocol
};

/** SFDP Parameter header */
struct sfdp_prm_hdr {
    uint8_t PID_LSB; // Parameter ID LSB
    uint8_t P_MINOR; // Parameter Minor Revision
    uint8_t P_MAJOR; // Parameter Major Revision
    uint8_t P_LEN;   // Parameter length in DWORDS
    uint32_t DWORD2; // Parameter ID MSB + Parameter Table Pointer
};

/** Parse SFDP Header
 * @param sfdp_hdr_ptr Pointer to memory holding an SFDP header
 * @return Number of Parameter Headers on success, -1 on failure
 */
int sfdp_parse_sfdp_header(sfdp_hdr *sfdp_hdr_ptr);

/** Parse Parameter Header
 * @param parameter_header Pointer to memory holding a single SFDP Parameter header
 * @param hdr_info Reference to a Parameter Table structure where info about the table is written
 * @return 0 on success, -1 on failure
 */
int sfdp_parse_single_param_header(sfdp_prm_hdr *parameter_header, sfdp_hdr_info &hdr_info);

/** Parse SFDP Headers
 * Retrieves SFDP headers from a device and parses the information contained by the headers
 *
 * @param sfdp_reader Callback function used to read headers from a device
 * @param hdr_info    All information parsed from the headers gets passed back on this structure
 *
 * @return 0 on success, negative error code on failure
 */
int sfdp_parse_headers(Callback<int(bd_addr_t, void *, bd_size_t)> sfdp_reader, sfdp_hdr_info &hdr_info);

} /* namespace mbed */
#endif
