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
#include "psa_test_its_reset_partition.h"
#include "psa_test_its_reset_ifs.h"


/* Threads stacks */
MBED_ALIGN(8) uint8_t test_its_reset_thread_stack[1024] = {0};

/* Threads control blocks */
osRtxThread_t test_its_reset_thread_cb = {0};

/* Thread attributes - for thread initialization */
osThreadAttr_t test_its_reset_thread_attr = {
    .name = "test_its_reset",
    .attr_bits = 0,
    .cb_mem = &test_its_reset_thread_cb,
    .cb_size = sizeof(test_its_reset_thread_cb),
    .stack_mem = test_its_reset_thread_stack,
    .stack_size = 1024,
    .priority = osPriorityNormal,
    .tz_module = 0,
    .reserved = 0
};

spm_rot_service_t test_its_reset_rot_services[TEST_ITS_RESET_ROT_SRV_COUNT] = {
    {
        .sid = TEST_PSA_ITS_RESET,
        .mask = TEST_PSA_ITS_RESET_MSK,
        .partition = NULL,
        .min_version = 1,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_RELAXED,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
};


static osRtxMutex_t test_its_reset_mutex = {0};
static const osMutexAttr_t test_its_reset_mutex_attr = {
    .name = "test_its_reset_mutex",
    .attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust,
    .cb_mem = &test_its_reset_mutex,
    .cb_size = sizeof(test_its_reset_mutex),
};


extern void test_pits_entry(void *ptr);

void test_its_reset_init(spm_partition_t *partition)
{
    if (NULL == partition) {
        SPM_PANIC("partition is NULL!\n");
    }

    partition->mutex = osMutexNew(&test_its_reset_mutex_attr);
    if (NULL == partition->mutex) {
        SPM_PANIC("Failed to create mutex for secure partition test_its_reset!\n");
    }

    for (uint32_t i = 0; i < TEST_ITS_RESET_ROT_SRV_COUNT; ++i) {
        test_its_reset_rot_services[i].partition = partition;
    }
    partition->rot_services = test_its_reset_rot_services;

    partition->thread_id = osThreadNew(test_pits_entry, NULL, &test_its_reset_thread_attr);
    if (NULL == partition->thread_id) {
        SPM_PANIC("Failed to create start main thread of partition test_its_reset!\n");
    }
}
