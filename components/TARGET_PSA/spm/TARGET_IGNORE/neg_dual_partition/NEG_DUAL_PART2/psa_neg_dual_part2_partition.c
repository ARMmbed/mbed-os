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

#include "cmsis.h"
#include "mbed_toolchain.h" /* For using MBED_ALIGN macro */
#include "rtx_os.h"
#include "spm_panic.h"
#include "spm_internal.h"
#include "psa_neg_dual_part2_partition.h"
#include "psa_neg_dual_part2_ifs.h"
#include "psa_neg_dual_part1_ifs.h"


/* Threads stacks */
MBED_ALIGN(8) uint8_t neg_dual_part2_thread_stack[512] = {0};

/* Threads control blocks */
osRtxThread_t neg_dual_part2_thread_cb = {0};

/* Thread attributes - for thread initialization */
osThreadAttr_t neg_dual_part2_thread_attr = {
    .name = "neg_dual_part2",
    .attr_bits = 0,
    .cb_mem = &neg_dual_part2_thread_cb,
    .cb_size = sizeof(neg_dual_part2_thread_cb),
    .stack_mem = neg_dual_part2_thread_stack,
    .stack_size = 512,
    .priority = osPriorityNormal,
    .tz_module = 0,
    .reserved = 0
};

spm_rot_service_t neg_dual_part2_rot_services[NEG_DUAL_PART2_ROT_SRV_COUNT] = {
    {
        .sid = DUMMY,
        .mask = DUMMY_MSK,
        .partition = NULL,
        .min_version = 5,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_RELAXED,
        .allow_nspe = false,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
    {
        .sid = PART2_CALL_INSIDE_PARTITION,
        .mask = PART2_CALL_INSIDE_PARTITION_MSK,
        .partition = NULL,
        .min_version = 5,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_RELAXED,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
};

/* External SIDs used by NEG_DUAL_PART2 */
const uint32_t neg_dual_part2_external_sids[1] = {
    PART1_CALL_NON_EXISTS_EXTERN_SID,
};

static osRtxMutex_t neg_dual_part2_mutex = {0};
static const osMutexAttr_t neg_dual_part2_mutex_attr = {
    .name = "neg_dual_part2_mutex",
    .attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust,
    .cb_mem = &neg_dual_part2_mutex,
    .cb_size = sizeof(neg_dual_part2_mutex),
};


extern void server_main2(void *ptr);

void neg_dual_part2_init(spm_partition_t *partition)
{
    if (NULL == partition) {
        SPM_PANIC("partition is NULL!\n");
    }

    partition->mutex = osMutexNew(&neg_dual_part2_mutex_attr);
    if (NULL == partition->mutex) {
        SPM_PANIC("Failed to create mutex for secure partition neg_dual_part2!\n");
    }

    for (uint32_t i = 0; i < NEG_DUAL_PART2_ROT_SRV_COUNT; ++i) {
        neg_dual_part2_rot_services[i].partition = partition;
    }
    partition->rot_services = neg_dual_part2_rot_services;

    partition->thread_id = osThreadNew(server_main2, NULL, &neg_dual_part2_thread_attr);
    if (NULL == partition->thread_id) {
        SPM_PANIC("Failed to create start main thread of partition neg_dual_part2!\n");
    }
}
