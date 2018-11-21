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
#include "psa_psa_f_partition.h"
#include "psa_psa_f_ifs.h"
#include "psa_its_ifs.h"


/* Threads stacks */
MBED_ALIGN(8) uint8_t psa_f_thread_stack[16384] = {0};

/* Threads control blocks */
osRtxThread_t psa_f_thread_cb = {0};

/* Thread attributes - for thread initialization */
osThreadAttr_t psa_f_thread_attr = {
    .name = "psa_f",
    .attr_bits = 0,
    .cb_mem = &psa_f_thread_cb,
    .cb_size = sizeof(psa_f_thread_cb),
    .stack_mem = psa_f_thread_stack,
    .stack_size = 16384,
    .priority = osPriorityNormal,
    .tz_module = 0,
    .reserved = 0
};

spm_rot_service_t psa_f_rot_services[PSA_F_ROT_SRV_COUNT] = {
    {
        .sid = PSA_CRYPTO_INIT_ID,
        .mask = PSA_CRYPTO_INIT,
        .partition = NULL,
        .min_version = 1,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_STRICT,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
    {
        .sid = PSA_MAC_ID,
        .mask = PSA_MAC,
        .partition = NULL,
        .min_version = 1,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_STRICT,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
    {
        .sid = PSA_HASH_ID,
        .mask = PSA_HASH,
        .partition = NULL,
        .min_version = 1,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_STRICT,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
    {
        .sid = PSA_ASYMMETRIC_ID,
        .mask = PSA_ASYMMETRIC,
        .partition = NULL,
        .min_version = 1,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_STRICT,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
    {
        .sid = PSA_SYMMETRIC_ID,
        .mask = PSA_SYMMETRIC,
        .partition = NULL,
        .min_version = 1,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_STRICT,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
    {
        .sid = PSA_AEAD_ID,
        .mask = PSA_AEAD,
        .partition = NULL,
        .min_version = 1,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_STRICT,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
    {
        .sid = PSA_KEY_MNG_ID,
        .mask = PSA_KEY_MNG,
        .partition = NULL,
        .min_version = 1,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_STRICT,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
    {
        .sid = PSA_RNG_ID,
        .mask = PSA_RNG,
        .partition = NULL,
        .min_version = 1,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_STRICT,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
    {
        .sid = PSA_CRYPTO_FREE_ID,
        .mask = PSA_CRYPTO_FREE,
        .partition = NULL,
        .min_version = 1,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_STRICT,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
    {
        .sid = PSA_GENERATOR_ID,
        .mask = PSA_GENERATOR,
        .partition = NULL,
        .min_version = 1,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_STRICT,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
    {
        .sid = PSA_ENTROPY_ID,
        .mask = PSA_ENTROPY_INJECT,
        .partition = NULL,
        .min_version = 1,
        .min_version_policy = PSA_MINOR_VERSION_POLICY_STRICT,
        .allow_nspe = true,
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
};

/* External SIDs used by PSA_F */
const uint32_t psa_f_external_sids[4] = {
    PSA_ITS_GET,
    PSA_ITS_SET,
    PSA_ITS_INFO,
    PSA_ITS_REMOVE,
};

static osRtxMutex_t psa_f_mutex = {0};
static const osMutexAttr_t psa_f_mutex_attr = {
    .name = "psa_f_mutex",
    .attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust,
    .cb_mem = &psa_f_mutex,
    .cb_size = sizeof(psa_f_mutex),
};


extern void part_main(void *ptr);

void psa_f_init(spm_partition_t *partition)
{
    if (NULL == partition) {
        SPM_PANIC("partition is NULL!\n");
    }

    partition->mutex = osMutexNew(&psa_f_mutex_attr);
    if (NULL == partition->mutex) {
        SPM_PANIC("Failed to create mutex for secure partition psa_f!\n");
    }

    for (uint32_t i = 0; i < PSA_F_ROT_SRV_COUNT; ++i) {
        psa_f_rot_services[i].partition = partition;
    }
    partition->rot_services = psa_f_rot_services;

    partition->thread_id = osThreadNew(part_main, NULL, &psa_f_thread_attr);
    if (NULL == partition->thread_id) {
        SPM_PANIC("Failed to create start main thread of partition psa_f!\n");
    }
}
