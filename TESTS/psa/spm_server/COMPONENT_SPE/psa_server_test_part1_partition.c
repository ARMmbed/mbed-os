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
#include "psa_server_test_part1_partition.h"
#include "psa_server_test_part1_ifs.h"
#include "psa_server_test_part2_ifs.h"


/* Threads stacks */
MBED_ALIGN(8) uint8_t server_test_part1_thread_stack[1024] = {0};

/* Threads control blocks */
osRtxThread_t server_test_part1_thread_cb = {0};

/* Thread attributes - for thread initialization */
osThreadAttr_t server_test_part1_thread_attr = {
    .name = "server_test_part1",
    .attr_bits = 0,
    .cb_mem = &server_test_part1_thread_cb,
    .cb_size = sizeof(server_test_part1_thread_cb),
    .stack_mem = server_test_part1_thread_stack,
    .stack_size = 1024,
    .priority = osPriorityNormal,
    .tz_module = 0,
    .reserved = 0
    };

spm_rot_service_t server_test_part1_rot_services[SERVER_TEST_PART1_ROT_SRV_COUNT] = {
    {
        .sid = CONTROL,
        .mask = CONTROL_MSK,
        .partition = NULL,
        .min_version = 5,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_RELAXED,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
    {
        .sid = TEST,
        .mask = TEST_MSK,
        .partition = NULL,
        .min_version = 12,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_STRICT,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
};

/* External SIDs used by SERVER_TEST_PART1 */
const uint32_t server_test_part1_external_sids[2] =
{
    ROT_SRV_REVERSE,
    ROT_SRV_DB_TST,
};

static osRtxMutex_t server_test_part1_mutex = {0};
static const osMutexAttr_t server_test_part1_mutex_attr = {
    .name = "server_test_part1_mutex",
    .attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust,
    .cb_mem = &server_test_part1_mutex,
    .cb_size = sizeof(server_test_part1_mutex),
};


extern void part1_main(void *ptr);

void server_test_part1_init(spm_partition_t *partition)
{
    if (NULL == partition) {
        SPM_PANIC("partition is NULL!\n");
    }

    partition->mutex = osMutexNew(&server_test_part1_mutex_attr);
    if (NULL == partition->mutex) {
        SPM_PANIC("Failed to create mutex for secure partition server_test_part1!\n");
    }

    for (uint32_t i = 0; i < SERVER_TEST_PART1_ROT_SRV_COUNT; ++i) {
        server_test_part1_rot_services[i].partition = partition;
    }
    partition->rot_services = server_test_part1_rot_services;

    partition->thread_id = osThreadNew(part1_main, NULL, &server_test_part1_thread_attr);
    if (NULL == partition->thread_id) {
        SPM_PANIC("Failed to create start main thread of partition server_test_part1!\n");
    }
}
