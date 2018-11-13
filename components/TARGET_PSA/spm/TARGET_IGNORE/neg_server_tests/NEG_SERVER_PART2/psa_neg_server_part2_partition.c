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
#include "psa_neg_server_part2_partition.h"
#include "psa_neg_server_part2_ifs.h"


/* Threads stacks */
MBED_ALIGN(8) uint8_t neg_server_part2_thread_stack[640] = {0};

/* Threads control blocks */
osRtxThread_t neg_server_part2_thread_cb = {0};

/* Thread attributes - for thread initialization */
osThreadAttr_t neg_server_part2_thread_attr = {
    .name = "neg_server_part2",
    .attr_bits = 0,
    .cb_mem = &neg_server_part2_thread_cb,
    .cb_size = sizeof(neg_server_part2_thread_cb),
    .stack_mem = neg_server_part2_thread_stack,
    .stack_size = 640,
    .priority = osPriorityNormal,
    .tz_module = 0,
    .reserved = 0
    };

spm_rot_service_t neg_server_part2_rot_services[NEG_SERVER_PART2_ROT_SRV_COUNT] = {
    {
        .sid = PART2_INT_MASK,
        .mask = PART2_INT_MASK_MSK,
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
        .sid = PART2_GET_MSG_NULL,
        .mask = PART2_GET_MSG_NULL_MSK,
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
        .sid = PART2_GET_SIGNUM_MULTIPLE_BIT,
        .mask = PART2_GET_SIGNUM_MULTIPLE_BIT_MSK,
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
        .sid = PART2_GET_SIGNUM_NOT_SUBSET,
        .mask = PART2_GET_SIGNUM_NOT_SUBSET_MSK,
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
        .sid = PART2_GET_SIGNUM_NOT_ACTIVE,
        .mask = PART2_GET_SIGNUM_NOT_ACTIVE_MSK,
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
        .sid = PART2_GET_SIGNUM_TWICE,
        .mask = PART2_GET_SIGNUM_TWICE_MSK,
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
        .sid = PART2_READ_INVALID_HANDLE,
        .mask = PART2_READ_INVALID_HANDLE_MSK,
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
        .sid = PART2_READ_NULL_HANDLE,
        .mask = PART2_READ_NULL_HANDLE_MSK,
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
        .sid = PART2_READ_TX_SIZE_ZERO,
        .mask = PART2_READ_TX_SIZE_ZERO_MSK,
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
        .sid = PART2_WRITE_BUFFER_NULL,
        .mask = PART2_WRITE_BUFFER_NULL_MSK,
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
        .sid = PART2_WRITE_RX_BUFF_NULL,
        .mask = PART2_WRITE_RX_BUFF_NULL_MSK,
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
        .sid = PART2_WRITE_INVALID_HANDLE,
        .mask = PART2_WRITE_INVALID_HANDLE_MSK,
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
        .sid = PART2_WRITE_NULL_HANDLE,
        .mask = PART2_WRITE_NULL_HANDLE_MSK,
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
        .sid = PART2_REPLY_INVALID_HANDLE,
        .mask = PART2_REPLY_INVALID_HANDLE_MSK,
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
        .sid = PART2_REPLY_NULL_HANDLE,
        .mask = PART2_REPLY_NULL_HANDLE_MSK,
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
        .sid = PART2_NOTIFY_PART_ID_INVALID,
        .mask = PART2_NOTIFY_PART_ID_INVALID_MSK,
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
        .sid = PART2_IDENTITY_INVALID_HANDLE,
        .mask = PART2_IDENTITY_INVALID_HANDLE_MSK,
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
        .sid = PART2_IDENTITY_NULL_HANDLE,
        .mask = PART2_IDENTITY_NULL_HANDLE_MSK,
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
        .sid = PART2_SET_RHANDLE_INVALID_HANDLE,
        .mask = PART2_SET_RHANDLE_INVALID_HANDLE_MSK,
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
        .sid = PART2_SET_RHANDLE_NULL_HANDLE,
        .mask = PART2_SET_RHANDLE_NULL_HANDLE_MSK,
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
        .sid = PART2_READ_WRAPAROUND,
        .mask = PART2_READ_WRAPAROUND_MSK,
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
        .sid = PART2_READ_EXCESS_INVEC,
        .mask = PART2_READ_EXCESS_INVEC_MSK,
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
        .sid = PART2_WRITE_WRAPAROUND,
        .mask = PART2_WRITE_WRAPAROUND_MSK,
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
        .sid = PART2_WRITE_EXCESS_OUTVEC,
        .mask = PART2_WRITE_EXCESS_OUTVEC_MSK,
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
        .sid = PART2_WRITE_SIZE_OVERFLOW,
        .mask = PART2_WRITE_SIZE_OVERFLOW_MSK,
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
        .sid = PART2_READ_NULL_BUFFER,
        .mask = PART2_READ_NULL_BUFFER_MSK,
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


static osRtxMutex_t neg_server_part2_mutex = {0};
static const osMutexAttr_t neg_server_part2_mutex_attr = {
    .name = "neg_server_part2_mutex",
    .attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust,
    .cb_mem = &neg_server_part2_mutex,
    .cb_size = sizeof(neg_server_part2_mutex),
};


extern void server_main2(void *ptr);

void neg_server_part2_init(spm_partition_t *partition)
{
    if (NULL == partition) {
        SPM_PANIC("partition is NULL!\n");
    }

    partition->mutex = osMutexNew(&neg_server_part2_mutex_attr);
    if (NULL == partition->mutex) {
        SPM_PANIC("Failed to create mutex for secure partition neg_server_part2!\n");
    }

    for (uint32_t i = 0; i < NEG_SERVER_PART2_ROT_SRV_COUNT; ++i) {
        neg_server_part2_rot_services[i].partition = partition;
    }
    partition->rot_services = neg_server_part2_rot_services;

    partition->thread_id = osThreadNew(server_main2, NULL, &neg_server_part2_thread_attr);
    if (NULL == partition->thread_id) {
        SPM_PANIC("Failed to create start main thread of partition neg_server_part2!\n");
    }
}
