/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "bl2/include/tfm_boot_status.h"
#include "secure_utilities.h"
#include "tfm_internal.h"
#include "tfm_api.h"
#include "flash_layout.h"
#include "secure_fw/spm/spm_api.h"

/*!
 * \def BOOT_DATA_VALID
 *
 * \brief Indicates that shared data between bootloader and runtime firmware was
 *        passed the sanity check with success.
 */
#define BOOT_DATA_VALID (1u)

/*!
 * \def BOOT_DATA_INVALID
 *
 * \brief Indicates that shared data between bootloader and runtime firmware was
 *        failed on sanity check.
 */
#define BOOT_DATA_INVALID (0u)

/*!
 * \var is_boot_data_valid
 *
 * \brief Indicates the status of shared data between bootloader and runtime
 *        firmware
 */
static uint32_t is_boot_data_valid = BOOT_DATA_INVALID;

void tfm_core_validate_boot_data(void)
{
    struct shared_data_tlv_header *tlv_header;

    tlv_header = (struct shared_data_tlv_header *)BOOT_TFM_SHARED_DATA_BASE;

    /* FixMe: Enhance sanity check of shared memory area, it might be invalid:
     *        - temporal exposure of RAM to non-secure actors
     *        - mismatched addresses
     *        - version mismatch between bootloader and runtime binary
     *        - etc.
     */
    if (tlv_header->tlv_magic == SHARED_DATA_TLV_INFO_MAGIC) {
        is_boot_data_valid = BOOT_DATA_VALID;
    }
}

void tfm_core_get_boot_data_handler(uint32_t args[])
{
    uint8_t  tlv_major = (uint8_t)args[0];
    uint8_t *ptr       = (uint8_t *)args[1];
    uint16_t buf_size  = (uint16_t)args[2];
    uint8_t *buf_start = ptr;
    uint32_t running_partition_idx =
            tfm_spm_partition_get_running_partition_idx();
    struct shared_data_tlv_header *tlv_header;
    struct shared_data_tlv_entry *tlv_entry;
    uintptr_t tlv_end, offset;
    uint32_t res;

    /* Make sure that the output pointer points to a memory area that is owned
     * by the partition
     */
    res = tfm_core_check_buffer_access(running_partition_idx,
                                       (void*)buf_start,
                                       buf_size,
                                       2);
    if (!res) {
        /* Not in accessible range, return error */
        args[0] = TFM_ERROR_INVALID_PARAMETER;
        return;
    }

    /* FixMe: Check whether caller has access right to given tlv_major_type */

    if (is_boot_data_valid != BOOT_DATA_VALID) {
        args[0] = TFM_ERROR_INVALID_PARAMETER;
        return;
    }

    /* Get the boundaries of TLV section */
    tlv_header = (struct shared_data_tlv_header *)BOOT_TFM_SHARED_DATA_BASE;
    tlv_end = BOOT_TFM_SHARED_DATA_BASE + tlv_header->tlv_tot_len;
    offset  = BOOT_TFM_SHARED_DATA_BASE + SHARED_DATA_HEADER_SIZE;

    /* Add header to output buffer as well */
    if (buf_size < SHARED_DATA_HEADER_SIZE) {
        args[0] = TFM_ERROR_INVALID_PARAMETER;
        return;
    } else {
        tfm_memcpy(ptr, tlv_header, SHARED_DATA_HEADER_SIZE);
        ptr += SHARED_DATA_HEADER_SIZE;
    }

    /* Iterates over the TLV section and copy TLVs with requested major
     * type to the provided buffer.
     */
    for(; offset < tlv_end; offset += tlv_entry->tlv_len) {
        tlv_entry = (struct shared_data_tlv_entry *)offset;
        if (tlv_entry->tlv_major_type == tlv_major) {
            /* Check buffer overflow */
            if ((ptr - buf_start + tlv_entry->tlv_len) > buf_size) {
                args[0] = TFM_ERROR_INVALID_PARAMETER;
                return;
            }

            tfm_memcpy(ptr, (const void *)tlv_entry, tlv_entry->tlv_len);

            ptr += tlv_entry->tlv_len;
            tlv_header->tlv_tot_len += tlv_entry->tlv_len;
        }
    }
    args[0] = TFM_SUCCESS;
    return;
}
