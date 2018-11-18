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
#include "psa_{{partition.name|lower}}_partition.h"
#include "psa_{{partition.name|lower}}_ifs.h"
{% for partition in dependent_partitions %}
#include "psa_{{partition|lower}}_ifs.h"
{% endfor %}


/* Threads stacks */
MBED_ALIGN(8) uint8_t {{partition.name|lower}}_thread_stack[{{partition.stack_size}}] = {0};

/* Threads control blocks */
osRtxThread_t {{partition.name|lower}}_thread_cb = {0};

/* Thread attributes - for thread initialization */
osThreadAttr_t {{partition.name|lower}}_thread_attr = {
    .name = "{{partition.name|lower}}",
    .attr_bits = 0,
    .cb_mem = &{{partition.name|lower}}_thread_cb,
    .cb_size = sizeof({{partition.name|lower}}_thread_cb),
    .stack_mem = {{partition.name|lower}}_thread_stack,
    .stack_size = {{partition.stack_size}},
    .priority = {{partition.priority}},
    .tz_module = 0,
    .reserved = 0
};

{% if partition.rot_services|count > 0 %}
spm_rot_service_t {{partition.name|lower}}_rot_services[{{partition.name|upper}}_ROT_SRV_COUNT] = {
{% for rot_srv in partition.rot_services %}
    {
        .sid = {{rot_srv.name|upper}},
        .mask = {{rot_srv.signal|upper}},
        .partition = NULL,
        .min_version = {{rot_srv.minor_version}},
        .min_version_policy = PSA_MINOR_VERSION_POLICY_{{rot_srv.minor_policy|upper}},
{% if rot_srv.nspe_callable %}
        .allow_nspe = true,
{% else %}
        .allow_nspe = false,
{% endif %}
        .queue = {
            .head = NULL,
            .tail = NULL
        }
    },
{% endfor %}
};
{% endif %}

{% if partition.extern_sids|count > 0 %}
/* External SIDs used by {{partition.name}} */
const uint32_t {{partition.name|lower}}_external_sids[{{partition.extern_sids|count}}] = {
{% for sid in partition.extern_sids %}
    {{sid|upper}},
{% endfor %}
};
{% endif %}
{% for rot_srv in partition.rot_services %}
{% endfor %}

static osRtxMutex_t {{partition.name|lower}}_mutex = {0};
static const osMutexAttr_t {{partition.name|lower}}_mutex_attr = {
    .name = "{{partition.name|lower}}_mutex",
    .attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust,
    .cb_mem = &{{partition.name|lower}}_mutex,
    .cb_size = sizeof({{partition.name|lower}}_mutex),
};

{% if partition.irqs|count > 0 %}
// Mapper function from irq signal to interupts number
IRQn_Type spm_{{partition.name|lower}}_signal_to_irq_mapper(uint32_t signal)
{
    SPM_ASSERT({{partition.name|upper}}_WAIT_ANY_IRQ_MSK & signal);
    switch(signal){
    {% for irq in partition.irqs %}
        case {{ irq.signal }}:
            return (IRQn_Type){{irq.line_num}};
            break;
    {% endfor %}
        default:
            break;
    }

    SPM_PANIC("Unknown signal number %d", signal);
    return 0;
}
{% endif %}

extern void {{partition.entry_point}}(void *ptr);

void {{partition.name|lower}}_init(spm_partition_t *partition)
{
    if (NULL == partition) {
        SPM_PANIC("partition is NULL!\n");
    }

    partition->mutex = osMutexNew(&{{partition.name|lower}}_mutex_attr);
    if (NULL == partition->mutex) {
        SPM_PANIC("Failed to create mutex for secure partition {{partition.name|lower}}!\n");
    }

    {% if partition.rot_services|count > 0 %}
    for (uint32_t i = 0; i < {{partition.name|upper}}_ROT_SRV_COUNT; ++i) {
        {{partition.name|lower}}_rot_services[i].partition = partition;
    }
    partition->rot_services = {{partition.name|lower}}_rot_services;
    {% else %}
    partition->rot_services = NULL;
    {% endif %}

    partition->thread_id = osThreadNew({{partition.entry_point}}, NULL, &{{partition.name|lower}}_thread_attr);
    if (NULL == partition->thread_id) {
        SPM_PANIC("Failed to create start main thread of partition {{partition.name|lower}}!\n");
    }
}
{# End of file #}
