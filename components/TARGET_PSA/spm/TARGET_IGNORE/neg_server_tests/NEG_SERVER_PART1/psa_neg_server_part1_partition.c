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
#include "psa_neg_server_part1_partition.h"
#include "psa_neg_server_part1_ifs.h"


/* Threads stacks */
MBED_ALIGN(8) uint8_t neg_server_part1_thread_stack[512] = {0};

/* Threads control blocks */
osRtxThread_t neg_server_part1_thread_cb = {0};

/* Thread attributes - for thread initialization */
osThreadAttr_t neg_server_part1_thread_attr = {
    .name = "neg_server_part1",
    .attr_bits = 0,
    .cb_mem = &neg_server_part1_thread_cb,
    .cb_size = sizeof(neg_server_part1_thread_cb),
    .stack_mem = neg_server_part1_thread_stack,
    .stack_size = 512,
    .priority = osPriorityNormal,
    .tz_module = 0,
    .reserved = 0
    };

spm_rot_service_t neg_server_part1_rot_services[NEG_SERVER_PART1_ROT_SRV_COUNT] = {
    {
        .sid = PART1_REPLY_INVALID_RETVAL_CONNECT,
        .mask = PART1_REPLY_INVALID_RETVAL_CONNECT_MSK,
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
        .sid = PART1_REPLY_INVALID_RETVAL_CALL,
        .mask = PART1_REPLY_INVALID_RETVAL_CALL_MSK,
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
        .sid = PART1_CLEAR_NO_DOORBELL,
        .mask = PART1_CLEAR_NO_DOORBELL_MSK,
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
        .sid = PART1_WRITE_OUTVEC_IX_SIZE_0,
        .mask = PART1_WRITE_OUTVEC_IX_SIZE_0_MSK,
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


static osRtxMutex_t neg_server_part1_mutex = {0};
static const osMutexAttr_t neg_server_part1_mutex_attr = {
    .name = "neg_server_part1_mutex",
    .attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust,
    .cb_mem = &neg_server_part1_mutex,
    .cb_size = sizeof(neg_server_part1_mutex),
};


extern void server_main1(void *ptr);

void neg_server_part1_init(spm_partition_t *partition)
{
    if (NULL == partition) {
        SPM_PANIC("partition is NULL!\n");
    }

    partition->mutex = osMutexNew(&neg_server_part1_mutex_attr);
    if (NULL == partition->mutex) {
        SPM_PANIC("Failed to create mutex for secure partition neg_server_part1!\n");
    }

    for (uint32_t i = 0; i < NEG_SERVER_PART1_ROT_SRV_COUNT; ++i) {
        neg_server_part1_rot_services[i].partition = partition;
    }
    partition->rot_services = neg_server_part1_rot_services;

    partition->thread_id = osThreadNew(server_main1, NULL, &neg_server_part1_thread_attr);
    if (NULL == partition->thread_id) {
        SPM_PANIC("Failed to create start main thread of partition neg_server_part1!\n");
    }
}
