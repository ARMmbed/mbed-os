/* Copyright (c) 2017 ARM Limited
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

/***********************************************************************************************************************
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * THIS FILE IS AN AUTO-GENERATED FILE - DO NOT MODIFY IT.
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 **********************************************************************************************************************/

#include "spm_panic.h"
#include "spm_internal.h"
#include "handles_manager.h"
#include "cmsis.h"
#include "psa_test_its_reset_partition.h"
#include "psa_its_partition.h"


spm_partition_t g_partitions[2] = {
    {
        .partition_id = TEST_ITS_RESET_ID,
        .thread_id = 0,
        .flags_rot_srv = TEST_ITS_RESET_WAIT_ANY_SID_MSK,
        .flags_interrupts = 0,
        .rot_services = NULL,
        .rot_services_count = TEST_ITS_RESET_ROT_SRV_COUNT,
        .extern_sids = NULL,
        .extern_sids_count = TEST_ITS_RESET_EXT_ROT_SRV_COUNT,
        .irq_mapper = NULL,
    },
    {
        .partition_id = ITS_ID,
        .thread_id = 0,
        .flags_rot_srv = ITS_WAIT_ANY_SID_MSK,
        .flags_interrupts = 0,
        .rot_services = NULL,
        .rot_services_count = ITS_ROT_SRV_COUNT,
        .extern_sids = NULL,
        .extern_sids_count = ITS_EXT_ROT_SRV_COUNT,
        .irq_mapper = NULL,
    },
};

/* Check all the defined memory regions for overlapping. */

/* A list of all the memory regions. */
const mem_region_t *mem_regions = NULL;

const uint32_t mem_region_count = 0;

// forward declaration of partition initializers
void test_its_reset_init(spm_partition_t *partition);
void its_init(spm_partition_t *partition);

uint32_t init_partitions(spm_partition_t **partitions)
{
    if (NULL == partitions) {
        SPM_PANIC("partitions is NULL!\n");
    }

    test_its_reset_init(&(g_partitions[0]));
    its_init(&(g_partitions[1]));

    *partitions = g_partitions;
    return 2;
}

