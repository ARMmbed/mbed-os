/*
* Copyright (c) 2018-2019 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <stdint.h>
#include <string.h>
#include "attestation.h"
#include "attestation_bootloader_data.h"
#include "tfm_impl/tfm_boot_status.h"

/*!
 * \def SHARED_DATA_INITIALZED and SHARED_DATA_UNNITIALZED
 *
 * \brief Indicates that shared data was already initialized.
 */
#define SHARED_DATA_UNNITIALZED  (0u)
#define SHARED_DATA_INITIALZED   (1u)

/*!
 * \var shared_data_init_done
 *
 * \brief Indicates whether shared data area was already initialized.
 *
 */
static uint32_t shared_data_init_done = SHARED_DATA_UNNITIALZED;

enum psa_attest_err_t
attest_get_boot_data(uint8_t major_type, void *ptr, uint32_t len) {
    if (shared_data_init_done == SHARED_DATA_INITIALZED)
    {
        return PSA_ATTEST_ERR_SUCCESS;
    }
    struct shared_data_tlv_header *tlv_header;
    struct shared_data_tlv_header *ptr_tlv_header;
    struct shared_data_tlv_entry *tlv_entry;
    uintptr_t tlv_end, offset;

    /* Get the boundaries of TLV section */
    tlv_header = (struct shared_data_tlv_header *)BOOT_TFM_SHARED_DATA_BASE;
    if (tlv_header->tlv_magic != SHARED_DATA_TLV_INFO_MAGIC)
    {
        return PSA_ATTEST_ERR_INIT_FAILED;
    }
    tlv_end = (uintptr_t)BOOT_TFM_SHARED_DATA_BASE + (uintptr_t)tlv_header->tlv_tot_len;
    offset  = (uintptr_t)BOOT_TFM_SHARED_DATA_BASE + (uintptr_t)SHARED_DATA_HEADER_SIZE;

    /* Add header to output buffer as well */
    if (len < SHARED_DATA_HEADER_SIZE)
    {
        return PSA_ATTEST_ERR_INIT_FAILED;
    }
    ptr_tlv_header = (struct shared_data_tlv_header *)ptr;
    ptr_tlv_header->tlv_magic   = SHARED_DATA_TLV_INFO_MAGIC;
    ptr_tlv_header->tlv_tot_len = SHARED_DATA_HEADER_SIZE;

    ptr = (uint8_t *)ptr + SHARED_DATA_HEADER_SIZE;
    /* Iterates over the TLV section and copy TLVs with requested major
     * type to the provided buffer.
     */
    for (; offset < tlv_end; offset += tlv_entry->tlv_len)
    {
        tlv_entry = (struct shared_data_tlv_entry *)offset;
        if (GET_MAJOR(tlv_entry->tlv_type) == major_type) {
            if (len < ptr_tlv_header->tlv_tot_len + tlv_entry->tlv_len) {
                return PSA_ATTEST_ERR_INIT_FAILED;
            }
            memcpy(ptr, (const void *)tlv_entry, tlv_entry->tlv_len);
            ptr = (uint8_t *)ptr + tlv_entry->tlv_len;
            ptr_tlv_header->tlv_tot_len += tlv_entry->tlv_len;
        }
        if (tlv_entry->tlv_len == 0) {
            break;
        }
    }

    shared_data_init_done = SHARED_DATA_INITIALZED;
    return PSA_ATTEST_ERR_SUCCESS;
}
