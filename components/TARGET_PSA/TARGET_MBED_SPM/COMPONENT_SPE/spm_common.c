/* Copyright (c) 2017-2018 ARM Limited
 *
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

#include "cmsis_os2.h"
#include "mbed_atomic.h"
#include "psa_defs.h"
#include "spm_internal.h"
#include "spm_panic.h"

inline void validate_iovec(
    const void *in_vec,
    const uint32_t in_len,
    const void *out_vec,
    const uint32_t out_len
)
{
    if (
        !(
            ((in_vec != NULL) || (in_len == 0)) &&
            ((out_vec != NULL) || (out_len == 0)) &&
            (in_len + out_len <= PSA_MAX_IOVEC)
        )
    ) {
        SPM_PANIC("Failed iovec Validation invec=(0X%p) inlen=(%lu) outvec=(0X%p) outlen=(%lu)\n", in_vec, in_len, out_vec, out_len);
    }
}

inline void channel_state_switch(uint8_t *current_state, uint8_t expected_state, uint8_t new_state)
{
    uint8_t actual_state = expected_state;
    if (!core_util_atomic_cas_u8(current_state, &actual_state, new_state)) {
        SPM_PANIC("channel in incorrect processing state: %hhu while %hhu is expected!\n",
                  actual_state, expected_state);
    }
}

inline void channel_state_assert(const uint8_t *current_state, uint8_t expected_state)
{
    uint8_t actual_state = core_util_atomic_load_u8(current_state);
    if (actual_state != expected_state) {
        SPM_PANIC("channel in incorrect processing state: %hhu while %hhu is expected!\n",
                  actual_state, expected_state);
    }
}

extern const mem_region_t *mem_regions;
extern const uint32_t mem_region_count;

const mem_region_t *get_mem_regions(int32_t partition_id, uint32_t *region_count)
{
    uint32_t i;

    SPM_ASSERT(NULL != region_count);
    *region_count = 0;

    if (partition_id == MEM_PARTITIONS_ALL) {
        *region_count = mem_region_count;
        return mem_regions;
    }

    // The entries in the array of memory regions are grouped by partition id.
    // This is ensured by the way this array is automatically generated from the manifest files.
    for (i = 0; i < mem_region_count && mem_regions[i].partition_id != partition_id; i++);

    if (i == mem_region_count) {
        return NULL;
    }

    const mem_region_t *regions = &mem_regions[i];
    for (; i < mem_region_count && mem_regions[i].partition_id == partition_id; i++, (*region_count)++);

    return regions;
}

extern spm_db_t g_spm;

spm_partition_t *get_active_partition(void)
{
    osThreadId_t active_thread_id = osThreadGetId();
    SPM_ASSERT(NULL != active_thread_id);

    for (uint32_t i = 0; i < g_spm.partition_count; ++i) {
        if (g_spm.partitions[i].thread_id == active_thread_id) {
            return &(g_spm.partitions[i]);
        }
    }

    return NULL; // Valid in case of NSPE
}

bool is_buffer_accessible(const void *ptr, size_t size, spm_partition_t *accessing_partition)
{
    if (NULL == ptr) {
        return false;
    }

    if (size == 0) {
        return true;
    }

    // Check wrap around of ptr + size
    if (((uintptr_t)ptr + size - 1) < (uintptr_t)ptr) {
        return false;
    }

    // Note: Sanity checks on platform addresses and sizes is done in psa_spm_init()

    uint32_t secure_ram_base   = PSA_SECURE_RAM_START;
    size_t   secure_ram_len    = PSA_SECURE_RAM_SIZE;
    uint32_t secure_rom_base   = PSA_SECURE_ROM_START;
    size_t   secure_rom_len    = PSA_SECURE_ROM_SIZE;

    uint32_t non_secure_ram_base   = PSA_NON_SECURE_RAM_START;
    size_t   non_secure_ram_len    = PSA_NON_SECURE_RAM_SIZE;
    uint32_t non_secure_rom_base   = PSA_NON_SECURE_ROM_START;
    size_t   non_secure_rom_len    = PSA_NON_SECURE_ROM_SIZE;


    // Check NSPE case
    if (accessing_partition == NULL) {

        // Make sure the NSPE is accessing the non-secure ram range OR the non-secure flash range

        // RAM
        if (((uintptr_t)ptr >= (uintptr_t)non_secure_ram_base) && ((uintptr_t)ptr < ((uintptr_t)non_secure_ram_base + non_secure_ram_len)) &&
                (((uintptr_t)ptr + size) <= ((uintptr_t)non_secure_ram_base + non_secure_ram_len))
           ) {
            return true;
        }

        // FLASH
        if (((uintptr_t)ptr >= (uintptr_t)non_secure_rom_base) && ((uintptr_t)ptr < ((uintptr_t)non_secure_rom_base + non_secure_rom_len)) &&
                (((uintptr_t)ptr + size) <= ((uintptr_t)non_secure_rom_base + non_secure_rom_len))
           ) {
            return true;
        }
    } else { // Check SPE case
        // As we do not expect secure partitions to use SPE addresses for iovecs, we make sure here that the SPE is accessing
        // the secure ram range OR the secure flash range

        // RAM
        if (((uintptr_t)ptr >= (uintptr_t)secure_ram_base) && ((uintptr_t)ptr < ((uintptr_t)secure_ram_base + secure_ram_len)) &&
                (((uintptr_t)ptr + size) <= ((uintptr_t)secure_ram_base + secure_ram_len))
           ) {
            return true;
        }

        // FLASH
        if (((uintptr_t)ptr >= (uintptr_t)secure_rom_base) && ((uintptr_t)ptr < ((uintptr_t)secure_rom_base + secure_rom_len)) &&
                (((uintptr_t)ptr + size) <= ((uintptr_t)secure_rom_base + secure_rom_len))
           ) {
            return true;
        }

    }

    return false;
}
